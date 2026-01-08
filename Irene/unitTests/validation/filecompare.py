import os
import sys
import glob
import argparse
import numpy as np
import logging


# parse the header information of a model run results file, 
#  looking for specific strings to determine needed attributes
def getFileAttrib( filename ):
  """
  Parse model run result text file header looking for specific strings to
  determine number of data fields, directions, and the delimiter used.

  Parameters
  ----------
  filename: path-like

  Returns
  -------
  tuple (ndata[int], ndir[int], delim[str])
  """

  #  initialize #data as indeterminate
  ndata = -1
  # assume a single direction (omnidirectional)
  ndir = 1
  # assume comma, unless otherwise specified
  delim = ","

  # process each line in file until standardized header parsed or end of file
  for line in open(filename):

    #  Look for "irection:" (could be 'Direction:' or 'Flux direction:")
    if "irection:" in line:
      # split on spaces, discard empty strings
      vdl = line.split(" ")
      vdl = list(filter(None, vdl))
      # find the field after 'irection:'
      ii = 0
      while "irection:" not in vdl[ii]:
        ii = ii + 1
      # if not 'Omnidirectional', read the next number as number of directions
      if not vdl[ii+1].startswith("Omni"):
        if vdl[ii+1].startswith("Bin"):  # Cammice PA bin range
          ndir = 1
        else:
          # convert string into integer value
          ndir = int(vdl[ii+1])

    # look for "Delimiter"
    if "Delimiter" in line:
      # set delimiter as comma when explictly specified
      if "Comma" in line or "comma" in line or "COMMA" in line:
        delim = ','
      else:
        # otherwise, use default value, which means 'whitespace'
        delim = None

    # look for "datetime" in standardized header data column label line
    if "datetime" in line:
      # count number of commas, add 1
      ntotal = line.count(",")+1
      # isolate time field value labels, count commas
      timestr =  line[line.index("(") + 1:line.index(")")]
      ntime = timestr.count(",")+1
      # assume n
      ndirdata = 0
      if "pitchAngle" in line:
        ndirdata = 1
      if "dirx,diry,dirz" in line:
        ndirdata = 4
      ndata = ntotal - ntime - ndirdata - 3 # position

      # end of standardized header - no more lines need to be processed
      logger.info(f"determined attributes of file {filename}")
      logger.info(f"   total number of fields: {ntotal}")
      logger.info(f"   number of time fields: {ntime}")
      logger.info(f"   number of position fields: 3")
      logger.info(f"   number of direction fields: {ndirdata}")
      logger.info(f"   number of 'data' fields: {ndata}")
      break

  # return collected values
  return (ndata, ndir, delim)


def file_comp( strFile1, strFile2, bPdiff=False , pct_thresh=0.01):

  # attempt to get the attributes for both of these files (should be the same)
  delim1 = None
  ndata1, ndir1, delim1 = getFileAttrib( strFile1 )
  delim2 = None
  ndata2, ndir2, delim2 = getFileAttrib( strFile2 )

  # as long as one of the files has the expected standardized headers,
  #  we can continue the processing
  ndata = max(ndata1,ndata2)
  ndir = max(ndir1,ndir2)

  # report error when cannot parse file info
  if ndata < 1:
    logger.info(' special case for ephemeris only')
    ndata = 3

  # slurp up data from these files, verify that same amount of data exists
  data1 = np.loadtxt(strFile1,delimiter=delim1)
  data2 = np.loadtxt(strFile2,delimiter=delim2)
  if np.shape(data1) != np.shape(data2):
    logger.error(' shape mismatch between input files')
    return -1

  # set number of data parameters in files
  if data1.size == np.shape(data1)[0]:
    data1 = np.reshape( data1, (1,data1.size))
    data2 = np.reshape( data2, (1,data2.size))
    logger.info("   *** single record of data ***")
  nfield = np.shape(data1)[1]

  # calculate difference of values between the two files
  delta = data2 - data1
  # ignore these differences anytime one or both report 'invalid' values
  delta[data1==-1.0e31] = 0.0
  delta[data2==-1.0e31] = 0.0
  delta[data1==-1.0] = 0.0
  delta[data2==-1.0] = 0.0

  # calculate a relative difference, based on the second file values
  with np.errstate(divide='ignore', invalid='ignore'): # ignore value->0 changes
    perc = delta/data2
  # zero out difference when invalid
  perc[np.isnan(perc)] = 0.0
  perc[np.isinf(perc)] = 0.0
  perc[perc==1.0] = 0.0  # ignore 0->value changes
  # turn into actual percentage difference 0-100%
  perc = perc*100.0
  perc[perc<-1.0] = 0.0  # ignore 0->value changes
  perc[perc>1.0] = 0.0  # ignore 0->value changes

  # do a battery of statistical calculations on these percentage differences
  #  keeping directional-based data separate from each other
  minperc = np.zeros((ndir,nfield))
  maxperc = np.zeros((ndir,nfield))
  p05perc = np.zeros((ndir,nfield))
  p50perc = np.zeros((ndir,nfield))
  p95perc = np.zeros((ndir,nfield))
  stdperc = np.zeros((ndir,nfield))
  for ii in range(ndir):
    for nn in range(nfield):
      minperc[ii,nn] = np.min(perc[ii::ndir,nn])
      maxperc[ii,nn] = np.max(perc[ii::ndir,nn])
      p05perc[ii,nn] = np.percentile(perc[ii::ndir,nn],5)
      p50perc[ii,nn] = np.median(perc[ii::ndir,nn])
      p95perc[ii,nn] = np.percentile(perc[ii::ndir,nn],95)
      stdperc[ii,nn] = np.std(perc[ii::ndir,nn])

  # check for unlikely changes in time and coordinate values
  for nn in range(nfield-ndata):
    if maxperc[0,nn] != 0.0 or minperc[0,nn] != 0.0:
      logger.warning(f' unexpected delta in time/coords/dir: index={nn}')
    perc[:,nn] = data1[:,nn]

  if bPdiff:
    # write new file with these calcuated percentage difference values
    #  for each entry and direction
    strFilePerc = os.path.join ( os.path.dirname(strFile1),
                   'PercDiff_' + os.path.basename(strFile1) )
    percfile = open(strFilePerc,'w')
    logger.info(f"writing to {strFilePerc}")
    percfile.write(f"# Percentage threshold: {pct_thresh:0.5f}\n")
    percfile.write("# Percentage differences between files:\n")
    percfile.write("#   %s\n" %(strFile1) )
    percfile.write("#   %s\n" %(strFile2) )
    for mm in range(np.shape(data1)[0]):
      percfile.write("%14.8f"%(perc[mm,0]))
      for nn in range(1,nfield):
        percfile.write(",%9.3e"%(perc[mm,nn]))
      percfile.write("\n")

    # at end of file, write statistical results of these differences,
    #   a set for each direction
    percfile.write("\n# ======= summary of differences ==========")
  logger.info("Summary of percentage differences over all data fields:")

  # each time a stat exceeds the given percentage threshold, this
  # counter variable will be incremented; it is returned at the end
  # of the routine
  exceeds_threshold_count = 0

  for ii in range(ndir):
    logger.info(f" data fields stat ranges: dir# {ii+1}")
    if bPdiff:
      percfile.write("\n# --dir #%d--\n"%(ii+1))

    minperc_min = np.min(minperc[ii,nfield-ndata+1:nfield])
    minperc_max = np.max(minperc[ii,nfield-ndata+1::nfield])

    flag = ""
    if ( np.abs(minperc_min)>pct_thresh or np.abs(minperc_max)>pct_thresh ):
      exceeds_threshold_count += 1
      flag = "#####"
    logger.info("   min:  %10.3e  %10.3e  %s" %( minperc_min,minperc_max,flag ))
    if bPdiff:
      percfile.write("# minimum percentage differences:  range(%10.3e,%10.3e)\n" %( minperc_min,minperc_max ))
      percfile.write("%10.3e"%(minperc[ii,0]))
      for nn in range(1,nfield):
        percfile.write(",%10.3e"%(minperc[ii,nn]))
      percfile.write("\n")

    maxperc_min = np.min(maxperc[ii,nfield-ndata+1:])
    maxperc_max = np.max(maxperc[ii,nfield-ndata+1:])

    flag = ""
    if ( np.abs(maxperc_min)>pct_thresh or np.abs(maxperc_max)>pct_thresh ):
      exceeds_threshold_count += 1
      flag = "#####"
    logger.info("   max:  %10.3e  %10.3e  %s" %( maxperc_min,maxperc_max,flag ))
    if bPdiff:
      percfile.write("# maximum percentage differences:  range(%10.3e,%10.3e)\n" %( maxperc_min,maxperc_max ))
      percfile.write("%10.3e"%(maxperc[ii,0]))
      for nn in range(1,nfield):
        percfile.write(",%10.3e"%(maxperc[ii,nn]))
      percfile.write("\n")

    p05perc_min = np.min(p05perc[ii,nfield-ndata+1:])
    p05perc_max = np.max(p05perc[ii,nfield-ndata+1:])

    flag = ""
    if ( np.abs(p05perc_min)>pct_thresh or np.abs(p05perc_max)>pct_thresh ):
      exceeds_threshold_count += 1
      flag = "#####"
    logger.info("  05th:  %10.3e  %10.3e  %s" %( p05perc_min,p05perc_max,flag ))
    if bPdiff:
      percfile.write("# 05th%% percentage differences:  range(%10.3e,%10.3e)\n" %( p05perc_min,p05perc_max ))
      percfile.write("%10.3e"%(p05perc[ii,0]))
      for nn in range(1,nfield):
        percfile.write(",%10.3e"%(p05perc[ii,nn]))
      percfile.write("\n")

    p50perc_min = np.min(p50perc[ii,nfield-ndata+1:])
    p50perc_max = np.max(p50perc[ii,nfield-ndata+1:])

    flag = ""
    if ( np.abs(p50perc_min)>pct_thresh or np.abs(p50perc_max)>pct_thresh ):
      exceeds_threshold_count += 1
      flag = "#####"
    logger.info("  50th:  %10.3e  %10.3e  %s" %( p50perc_min,p50perc_max,flag ))
    if bPdiff:
      percfile.write("# median percentage differences:  range(%10.3e,%10.3e)\n" %( p50perc_min,p50perc_max ))
      percfile.write("%10.3e"%(p50perc[ii,0]))
      for nn in range(1,nfield):
        percfile.write(",%10.3e"%(p50perc[ii,nn]))
      percfile.write("\n")

    p95perc_min = np.min(p95perc[ii,nfield-ndata+1:])
    p95perc_max = np.max(p95perc[ii,nfield-ndata+1:])

    flag = ""
    if ( np.abs(p95perc_min)>pct_thresh or np.abs(p95perc_max)>pct_thresh ):
      exceeds_threshold_count += 1
      flag = "#####"
    logger.info("  95th:  %10.3e  %10.3e  %s" %( p95perc_min, p95perc_max,flag ))
    if bPdiff:
      percfile.write("# 95th%% percentage differences:  range(%10.3e,%10.3e)\n" %( p95perc_min,p95perc_max ))
      percfile.write("%10.3e"%(p95perc[ii,0]))
      for nn in range(1,nfield):
        percfile.write(",%10.3e"%(p95perc[ii,nn]))
      percfile.write("\n")

    stdperc_min = np.min(stdperc[ii,nfield-ndata+1:])
    stdperc_max = np.max(stdperc[ii,nfield-ndata+1:])

    flag = ""
    if ( np.abs(stdperc_min)>pct_thresh or np.abs(stdperc_max)>pct_thresh ):
      exceeds_threshold_count += 1
      flag = "#####"
    logger.info("  StdD:  %10.3e  %10.3e  %s" %( stdperc_min,stdperc_max,flag ))
    if bPdiff:
      percfile.write("# std dev percentage differences:  range(%10.3e,%10.3e)\n" %( stdperc_min, stdperc_max ))
      percfile.write("%10.3e"%(stdperc[ii,0]))
      for nn in range(1,nfield):
        percfile.write(",%10.3e"%(stdperc[ii,nn]))
      percfile.write("\n")

  if bPdiff:
    percfile.close()

  return exceeds_threshold_count



# --------------------------------------------------------------------

def main():

  parser = argparse.ArgumentParser(description='compare two versions of IRENE unit test output files')
  parser.add_argument( "-P", action="store_true", default=False, dest="pdiff",
         help="Adds writing of point-wise percentage difference file" )
  parser.add_argument('dir1', type=str, help='directory of first file')
  parser.add_argument('dir2', type=str, help='directory of second file')
  parser.add_argument('filename', type=str, nargs='+',
                      help=('name of file in both directories (or nameS in first and second);\n'
                            ' or collection of filenames specified by wildcards (in quotes)'))
  parser.add_argument('--pct-thresh', dest="pct_thresh", type=float, default="0.001",
                      help='percentage threshold to flag files, e.g. supply 0.01 for 1%; minimum of 0.0001 accepted')
  parser.add_argument("--quiet", action="store_true", default=False,
                      help="Show only the pass/fail result of one or more comparisons")
  args = parser.parse_args()

  # set global logging level
  loglev = logging.WARNING if args.quiet else logging.INFO
  logger.setLevel(loglev)
  #    end logging configuration

  if len(args.filename)>2:
    logger.error("too many file specified; wildcards must be in quotes")
    return 1

  # check that the percentage threshold is above 0.0001 as indicated in message
  if (args.pct_thresh) < 0.0001:
      logger.error(f"Percent threshold supplied {args.pct_thresh} < 0.0001 (0.01%). Exiting.")
      return 1

  # get list of file(s) in first directory
  lstrDir1Files = glob.glob( os.path.join( args.dir1,args.filename[0] ) )


  # exit when none
  if len(lstrDir1Files)==0:
    logger.error("Error: no matching files in first directory")
    return 1

  # assemble list of file(s) in second directory
  lstrDir2Files = []
  ii = 0

  while ii < len(lstrDir1Files):

    # check for explicit second filename for second directory [single file]
    if len(args.filename) > 2:
      strFile = os.path.join( args.dir2, args.filename[1] )

    else:

    # compose filename in second directory
      strFile = os.path.join( args.dir2, os.path.basename(lstrDir1Files[ii]) )

    # test that second directory file exists
    if os.path.isfile( strFile ):

      # add to list for second directory
      lstrDir2Files.append( strFile )
      ii += 1

    else:

      # otherwise remove filename from first directory
      del lstrDir1Files[ii]

  # save number of files to be compared
  iNumFiles = len(lstrDir1Files)
  # return when none available
  if iNumFiles==0:
    logger.error("Error: no matching files in second directory")
    return 0

  # variable hold number of files that fail
  nfailed = 0

  # loop over comparison for each pair of files
  if iNumFiles > 1:
    logger.info(f"Info: Performing comparison of {iNumFiles} files")
  for ii in range(iNumFiles):
    logger.info("-----------------------------------------------------------------------")
    logger.info("Comparing model output results in these files:")
    logger.info(f" -- {lstrDir1Files[ii]}")
    logger.info(f" -- {lstrDir2Files[ii]}")

    # perform comparison
    ret = file_comp( lstrDir1Files[ii], lstrDir2Files[ii], bPdiff=args.pdiff , pct_thresh=args.pct_thresh)
    if ret < 0:
      logger.warning("An error occurred during the previous comparison.")
    if ret > 0:
      logger.error(f"-----------------------> Flagging file {lstrDir1Files[ii]}")
      nfailed += 1

  # print out (NOTE that it's not filtered by logging rule) pass/fail result
  print("COMPARISON RESULT: PASS" if nfailed == 0 else "COMPARISON RESULT: FAILURE DETECTED")

  # return number of files that failed
  return nfailed



if __name__ == '__main__':

    # declare logger with global scope; this will be used by most functions
    logging.basicConfig(format='%(message)s', level=logging.INFO)
    logger = logging.getLogger(__name__)

    # call main function, returning number of failed files to shell
    sys.exit(main())

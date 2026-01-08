
# enable use of python3 print function form in python 2.x
from __future__ import print_function

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Ae9Ap9 root directory here:
#sys.path.append(os.environ['HOME']+'/Ae9Ap9')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

from ephemModel import EphemModel
import pytest
import numpy as np


@pytest.fixture
def ephem():
    ephem = EphemModel()
    print('ephem zHandle=', ephem.zHandle)
    return ephem


@pytest.fixture
def ephem_default():
    ephem = EphemModel()
    print('ephem zHandle=', ephem.zHandle)
    ephem.set_magfieldDBFile('../../modelData/igrfDB.h5')
    ephem.set_chunkSize(256)

    dValue = 25.0
    ierr = ephem.set_inclination(dValue)
    assert(ierr == 0)
    dValue = 0.25
    ierr = ephem.set_eccentricity(dValue)
    assert(ierr == 0)
    dValue = 8.5
    ierr = ephem.set_meanMotion(dValue)
    assert(ierr == 0)
    dValue = 218.5
    ierr = ephem.set_meanAnomaly(dValue)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ephem.set_rightAscension(dValue)
    assert(ierr == 0)
    dMjd = 58119.5
    ierr = ephem.set_elementTime(dMjd)
    assert(ierr == 0)
    dStep = 60.0
    dStart = 58119.5
    dEnd = 58120.5
    ierr = ephem.set_times(dStart, dEnd, dStep)
    assert(ierr == 0)

    return ephem

# In general, each method is tested first with 'invalid' inputs, then valid ones.
# The associated 'get' methods are also tested, when available, in the same manner.
# Some also include tests with invalid 'handle' values for completeness.


def test_set_magfieldDBFile(ephem):
    ierr = ephem.test_badHandle()
    print(' test_badHandle => ', ierr)
    assert(ierr < 0)

    strRet = ephem.get_magfieldDBFile()
    print(' get_magfieldDBFile: => ', strRet)
    assert(strRet == "UNDEF")

    # put in a valid file, so to fully test reset on invalid one
    ierr = ephem.set_magfieldDBFile('../../modelData/igrfDB.h5')
    assert(ierr == 0)

    for strFile in ['igrfDB.h5', '$DATADIR/igrfDB.h5']:
        ierr = ephem.set_magfieldDBFile(strFile)
        print(' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr < 0)

        strRet = ephem.get_magfieldDBFile()
        print(' get_magfieldDBFile: => ', strRet)
        assert(strRet == "UNDEF")

    for strFile in ['../../modelData/igrfDB.h5']:
        ierr = ephem.set_magfieldDBFile(strFile)
        print(' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr == 0)

        strRet = ephem.get_magfieldDBFile()
        print(' get_magfieldDBFile: => ', strRet)
        assert(strRet == strFile)

def test_set_modelDBDir():

    ephemT= EphemModel()
    print('ephem zHandle=', ephemT.zHandle)
    ephemT.set_chunkSize(256)

    dValue = 25.0
    ierr = ephemT.set_inclination(dValue)
    assert(ierr == 0)
    dValue = 0.25
    ierr = ephemT.set_eccentricity(dValue)
    assert(ierr == 0)
    dValue = 8.5
    ierr = ephemT.set_meanMotion(dValue)
    assert(ierr == 0)
    dValue = 218.5
    ierr = ephemT.set_meanAnomaly(dValue)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ephemT.set_rightAscension(dValue)
    assert(ierr == 0)
    dMjd = 58119.5
    ierr = ephemT.set_elementTime(dMjd)
    assert(ierr == 0)
    dStep = 60.0
    dStart = 58119.5
    dEnd = 58120.5
    ierr = ephemT.set_times(dStart, dEnd, dStep)
    assert(ierr == 0)

    strCoordSys = 'GDZ'
    strCoordUnits = 'km'

    # this fails on magfield db not defined
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephemT.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum<0)
    for strString in ["../../modelData/","goober","../../kernelXml/"]:
      ierr = ephemT.set_modelDBDir(strString)
      print(' set_modelDBDir: strString=', strString, ' => ', ierr)
      if strString=="goober":
        assert(ierr < 0)
      else:
        assert(ierr == 0)

      strRet = ephemT.get_modelDBDir()
      print(' get_modelDBDir: => ', strRet)
      if strString=="goober":
        assert(strRet == "")
      else:
        assert(strRet == strString)
    # last ModelDBDir is a valid directory, but incorrect
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephemT.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum<0)
    strString = "../../modelData/"
    ierr = ephemT.set_modelDBDir(strString)
    print(' set_modelDBDir: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strRet = ephemT.get_modelDBDir()
    print(' get_modelDBDir: => ', strRet)
    assert(strRet == strString)
    print(' rerun with correct modelDBDir:')
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephemT.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum>0)
    print('-- ephemT: automatically defined via ModelDBDir:')
    print(' get_magfieldDBFile: => ', ephemT.get_magfieldDBFile())

def test_set_modelDBDir2():

    ephemT= EphemModel()
    print('ephem zHandle=', ephemT.zHandle)
    ephemT.set_chunkSize(256)

    dValue = 25.0
    ierr = ephemT.set_inclination(dValue)
    assert(ierr == 0)
    dValue = 0.25
    ierr = ephemT.set_eccentricity(dValue)
    assert(ierr == 0)
    dValue = 8.5
    ierr = ephemT.set_meanMotion(dValue)
    assert(ierr == 0)
    dValue = 218.5
    ierr = ephemT.set_meanAnomaly(dValue)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ephemT.set_rightAscension(dValue)
    assert(ierr == 0)
    dMjd = 58119.5
    ierr = ephemT.set_elementTime(dMjd)
    assert(ierr == 0)
    dStep = 60.0
    dStart = 58119.5
    dEnd = 58120.5
    ierr = ephemT.set_times(dStart, dEnd, dStep)
    assert(ierr == 0)

    # this succeeds, despite magfield db not defined - no coord conversion needed
    iNum, daTime, daXpos, daYpos, daZpos, daXvel, daYvel, daZvel \
        = ephemT.computeGei()
    print(' computeGei:  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daXpos))
    assert(iNum>0)

    strCoordSys = 'GEI'
    strCoordUnits = 'km'
    # try conversion GDZ to GDZ
    strCoordSysOut = 'GDZ'
    strCoordUnitsOut = 'km'
    ierr, dCoord1, dCoord2, dCoord3  \
        = ephemT.convertCoordsSingle(strCoordSys, strCoordUnits,
                                            daTime[0], daXpos[0], daYpos[0], daZpos[0], 
                                            strCoordSysOut, strCoordUnitsOut)
    print('  convertCoordsSingle: inCoords=', strCoordSys, strCoordUnits)
    print('     time=', daTime[0], ', pos=',
          daXpos[0], daYpos[0], daZpos[0], '; outCoords=', strCoordSysOut, strCoordUnitsOut)
    print('     returned ierr=', ierr, ', pos=', dCoord1, dCoord2, dCoord3)
    assert(ierr < 0)

    for strString in ["../../modelData/","goober","../../kernelXml/"]:
      ierr = ephemT.set_modelDBDir(strString)
      print(' set_modelDBDir: strString=', strString, ' => ', ierr)
      if strString=="goober":
        assert(ierr < 0)
      else:
        assert(ierr == 0)

      strRet = ephemT.get_modelDBDir()
      print(' get_modelDBDir: => ', strRet)
      if strString=="goober":
        assert(strRet == "")
      else:
        assert(strRet == strString)
    # last ModelDBDir is a valid directory, but incorrect
    ierr, dCoord1, dCoord2, dCoord3  \
        = ephemT.convertCoordsSingle(strCoordSys, strCoordUnits,
                                            daTime[0], daXpos[0], daYpos[0], daZpos[0], 
                                            strCoordSysOut, strCoordUnitsOut)
    print('  convertCoordsSingle: inCoords=', strCoordSys, strCoordUnits)
    print('     time=', daTime[0], ', pos=',
          daXpos[0], daYpos[0], daZpos[0], '; outCoords=', strCoordSysOut, strCoordUnitsOut)
    print('     returned ierr=', ierr, ', pos=', dCoord1, dCoord2, dCoord3)
    assert(ierr < 0)

    strString = "../../modelData/"
    ierr = ephemT.set_modelDBDir(strString)
    print(' set_modelDBDir: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strRet = ephemT.get_modelDBDir()
    print(' get_modelDBDir: => ', strRet)
    assert(strRet == strString)
    print(' rerun with correct modelDBDir:')
    ierr, dCoord1, dCoord2, dCoord3  \
        = ephemT.convertCoordsSingle(strCoordSys, strCoordUnits,
                                            daTime[0], daXpos[0], daYpos[0], daZpos[0], 
                                            strCoordSysOut, strCoordUnitsOut)
    print('  convertCoordsSingle: inCoords=', strCoordSys, strCoordUnits)
    print('     time=', daTime[0], ', pos=',
          daXpos[0], daYpos[0], daZpos[0], '; outCoords=', strCoordSysOut, strCoordUnitsOut)
    print('     returned ierr=', ierr, ', pos=', dCoord1, dCoord2, dCoord3)
    assert(ierr == 0)
    print('-- ephemT: automatically defined via ModelDBDir:')
    print(' get_magfieldDBFile: => ', ephemT.get_magfieldDBFile())

def test_set_prop(ephem):
    for strProp in ['kelper', 'SPG4']:
        ierr = ephem.set_prop(strProp)
        print(' set_prop: prop=', strProp, ' => ', ierr)
        assert(ierr < 0)

        strRet = ephem.get_prop()
        print(' get_prop: returned prop=', strProp)
        assert(strRet == "*UNDEFINED*")

    for strProp in ['Kepler', 'SGP4', 'SatEph', 'KEPLER']:
        ierr = ephem.set_prop(strProp)
        print(' set_prop: prop=', strProp, ' => ', ierr)
        assert(ierr == 0)

    strProp = ephem.get_prop()
    print(' get_prop: returned prop=', strProp)
    assert(strProp == 'Kepler')  # with J2' )


def test_set_keplerUseJ2(ephem):
    bVerdict = False
    ierr = ephem.set_keplerUseJ2(bVerdict)
    print(' set_keplerUseJ2: verdict=', bVerdict)
    assert(ierr == 0)

    strProp = ephem.get_prop()
    print(' get_prop: returned prop=', strProp)
    assert(strProp == 'Kepler')
    useJ2 = ephem.get_keplerUseJ2()
    print(' get_keplerUseJ2: => ', useJ2)
    assert(useJ2 == 0)

    ierr = ephem.set_keplerUseJ2()
    print(' set_keplerUseJ2: verdict=<unspecified> => ', ierr)
    assert(ierr == 0)

    strProp = ephem.get_prop()
    print(' get_prop: returned prop=', strProp)
    assert(strProp == 'Kepler')  # with J2' )
    useJ2 = ephem.get_keplerUseJ2()
    print(' get_keplerUseJ2: => ', useJ2)
    assert(useJ2 == 1)


def test_set_sgp4Param(ephem):
    strProp = 'sgp4'
    ierr = ephem.set_prop(strProp)
    print(' set_prop: prop=', strProp, ' => ', ierr)
    assert(ierr == 0)
    strProp = ephem.get_prop()
    print(' get_prop: returned prop=', strProp)
    assert(strProp == 'SGP4')  # (AFSPC,WGS84)' )
    strMode = ephem.get_sgp4Mode()
    print(' get_sgp4Mode: => ', strMode)
    assert(strMode == 'AFSPC')
    strWGS = ephem.get_sgp4Wgs()
    print(' get_sgp4Wgs: => ', strWGS)
    assert(strWGS == '84')

    for strMode in ['Standard', 'std', 'new', 'Standard', 'imp', 'improved']:
        for strWgs in ['72old', '56', '72new', '84', '72']:
            ierr = ephem.set_sgp4Param(strMode, strWgs)
            print(' set_sgp4Param: mode=', strMode,
                  ', wgs=', strWgs, ' => ', ierr)
            if strMode == 'new' or strMode == 'imp' or strWgs == '56' or strWgs == '72new':
                assert(ierr < 0)
            else:
                assert(ierr == 0)
    strProp = ephem.get_prop()
    print(' get_prop: returned prop=', strProp)
    assert(strProp == 'SGP4')  # (Improved,WGS72)' )
    strMode = ephem.get_sgp4Mode()
    print(' get_sgp4Mode: => ', strMode)
    assert(strMode == 'Improved')
    strWGS = ephem.get_sgp4Wgs()
    print(' get_sgp4Wgs: => ', strWGS)
    assert(strWGS == '72')


def test_set_tleFile(ephem):
    for strFile in ['cnofs_tle.dat', '$DATADIR/cnofs_tle.dat']:
        ierr = ephem.set_tleFile(strFile)
        print(' set_tleFile: strFile=', strFile, ' => ', ierr)
        assert(ierr < 0)

    for strFile in ['../../unitTests/cnofs_tle.dat']:
        ierr = ephem.set_tleFile(strFile)
        print(' set_tleFile: strFile=', strFile, ' => ', ierr)
        assert(ierr == 0)

    strFile = ephem.get_tleFile()
    print(' get_tleFile: returned tleFile=', strFile)
    assert(len(strFile) > 0)


def test_set_times(ephem):
    dStep = 60.0
    for dStart in [31000.2, 56789.4, 51145.5, 70123.4, 58119.5]:
        for dEnd in [56779.0, 80123.4, 58120.5]:
            ierr = ephem.set_times(dStart, dEnd, dStep)
            print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' %
                  (dStart, dEnd, dStep, ierr))
            if dStart < 33282.0 or dStart > 69807.0 or dEnd < 33282.0 or dEnd > 69807.0:
                assert(ierr < 0)
            elif dStart >= dEnd:
                assert(ierr < 0)
            else:
                assert(ierr == 0)
    # most recent ones used here were valid
    dStart2, dEnd2, dStep2 = ephem.get_times()
    assert(dStart2==dStart)
    assert(dEnd2==dEnd)
    assert(dStep2==dStep)
    # verify individual time values are correct
    dStart2 = ephem.get_startTime()
    dEnd2 = ephem.get_endTime()
    dStep2 = ephem.get_timeStep()
    assert(dStart2==dStart)
    assert(dEnd2==dEnd)
    assert(dStep2==dStep)
    for dStep in [-25.0, 0.0]:
        ierr = ephem.set_times(dStart, dEnd, dStep)
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' %
              (dStart, dEnd, dStep, ierr))
        assert(ierr < 0)
    for dStep in [30.0, 3600.0, 300.0]:
        ierr = ephem.set_times(dStart, dEnd, dStep)
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' %
              (dStart, dEnd, dStep, ierr))
        assert(ierr == 0)
    iNumT = ephem.get_numTimesList()
    assert(iNumT>0)
    print(' iNumT=',iNumT)
    iNumT2, daTimes = ephem.get_timesList()
    assert(iNumT==iNumT2)
    print(' iNumT2=',iNumT2)
    assert(daTimes[0]==dStart)
    assert(daTimes[-1]==dEnd)
    assert(len(daTimes)==iNumT2)
    for dVarMinStep in [5.0, 300.0, 30.0]:
      for dVarMaxStep in [120.0, 4800.0, 1200.0]:
        for dVarRound in [0.0, 1.5, 60.0, 5.0]:
          ierr = ephem.set_varTimes(dStart, dEnd, dVarMinStep, dVarMaxStep, dVarRound)
          print(' set_varTimes: start=%.1f, end=%.1f, vstep=%.0f,%.0f,%.1f => %i' %
              (dStart, dEnd, dVarMinStep, dVarMaxStep, dVarRound, ierr))
          iExpect = 0
          if dVarMinStep<10.0 or dVarMinStep>=dVarMaxStep or dVarMaxStep>3600.0: iExpect = -1
          if dVarRound>=dVarMinStep or dVarRound-int(dVarRound)>0.0: iExpect = -1
          assert(ierr == iExpect)
    dVarMinStep2, dVarMaxStep2, dVarRound2 = ephem.get_varTimeStep()
    assert(dVarMinStep2==dVarMinStep)
    assert(dVarMaxStep2==dVarMaxStep)
    assert(dVarRound2==dVarRound)
    # verify that fixed timestep is invalidated
    dStep2 = ephem.get_timeStep()
    assert(dStep2==-1.0)
    iNumT = ephem.get_numTimesList()
    assert(iNumT<0)
    print(' var iNumT=',iNumT)
    iNumT2, daTimes2 = ephem.get_timesList()
    assert(iNumT==iNumT2)
    print('var iNumT2=',iNumT2)
    assert(len(daTimes2)==1)

    # set time specification separately
    for dStart in [31000.2, 56789.4, 51145.5, 70123.4, 58119.5]:
      ierr = ephem.set_startTime(dStart)
      print(' set_startTime: start=%.1f => %i' % (dStart, ierr))
      if dStart < 33282.0 or dStart > 69807.0:
        assert(ierr < 0)
      else:
        assert(ierr == 0)
    for dEnd in [56779.0, 80123.4, 58120.5]:
      ierr = ephem.set_endTime(dEnd)
      print(' set_endTime: end=%.1f => %i' % (dEnd, ierr))
      if dEnd < 33282.0 or dEnd > 69807.0:
        assert(ierr < 0)
      else:
        assert(ierr == 0)
    for dStep in [-25.0, 0.0]:
        ierr = ephem.set_timeStep(dStep)
        print(' set_timeStep: step=%.0f => %i' % (dStep, ierr))
        assert(ierr < 0)
    for dStep in [30.0, 3600.0, 300.0]:
        ierr = ephem.set_timeStep(dStep)
        print(' set_timeStep: step=%.0f => %i' % (dStep, ierr))
        assert(ierr == 0)
    # verify that variable timestep limits are invalidated
    dVarMinStep2, dVarMaxStep2, dVarRound2 = ephem.get_varTimeStep()
    assert(dVarMinStep2==-1.0)
    assert(dVarMaxStep2==-1.0)
    assert(dVarRound2==-1.0)
    # verify time range is correct
    dStart2 = ephem.get_startTime()
    dEnd2 = ephem.get_endTime()
    assert(dStart2==dStart)
    assert(dEnd2==dEnd)

    # test time array input, using previously generated list
    ierr = ephem.set_timesList( daTimes )
    print(' set_timesList( ',daTimes[0],'...',daTimes[-1],' ) n=',len(daTimes),' =>',ierr )
    assert(ierr==0)
    daTimes[len(daTimes)//2] = daTimes[(len(daTimes)//2)-1]
    ierr = ephem.set_timesList( daTimes )
    print(' set_timesList( w/ repeated time ) =>',ierr)
    assert(ierr==-1)
    daTimes[len(daTimes)//2] = daTimes[(len(daTimes)//2)-1]+30000.0
    ierr = ephem.set_timesList( daTimes )
    print(' set_timesList( w/ invalid time ) =>',ierr)
    assert(ierr==-1)

    # test variable timestep specifications
    ierr = ephem.set_varTimeStep(dVarMinStep,dVarMaxStep,dVarRound)
    print(' set_varTimeStep: vstep=%.0f,%.0f,%.1f => %i' % (dVarMinStep,dVarMaxStep,dVarRound,ierr))
    assert(ierr==0)
    dVarMinStep2, dVarMaxStep2, dVarRound2 = ephem.get_varTimeStep()
    assert(dVarMinStep2==dVarMinStep)
    assert(dVarMaxStep2==dVarMaxStep)
    assert(dVarRound2==dVarRound)
    # test defaults for variable timestep specifications
    ierr = ephem.set_varTimeStep(dVarMinStep)
    print(' set_varTimeStep: vstep min(only)=%.0f => %i' % (dVarMinStep, ierr))
    assert(ierr==0)
    dVarMinStep2, dVarMaxStep2, dVarRound2 = ephem.get_varTimeStep()
    assert(dVarMinStep2==dVarMinStep)
    assert(dVarMaxStep2==3600.0)
    assert(dVarRound2==5.0)
    print( ' variable specifications default max=',dVarMaxStep2,', round=',dVarRound2)

    # current ephemeris start time 58119.5 is 01 Jan 2018 1200 GMT
    # This TLE epoch is 27 Jan 2005 0653 GMT; 
    strLine1 = '1 20712U 90065A   05027.28729034  .00000029  00000-0  20288-4 0   166'
    strLine2 = '2 20712  18.2943  30.1164 7147621 257.9138  20.6811  2.41951061118231'
    ierr = ephem.set_tle(strLine1, strLine2)
    print(' set_tle( <CRRES satellite> ) =>',ierr)
    ierr = ephem.set_startTime(dStart)
    print(' set_startTime: start=%.1f => %i' % (dStart, ierr))
    ierr = ephem.set_endTime(dEnd)
    print(' set_endTime: end=%.1f => %i' % (dEnd, ierr))
    # verify that fixed timestep is invalidated
    dStepSec = ephem.get_timeStep()
    assert(dStepSec==-1.0)
    iNumT = ephem.get_numTimesList()
    assert(iNumT>0)
    print(' CRRES var iNumT=',iNumT)
    iNumT2, daTimes2 = ephem.get_timesList()
    assert(iNumT==iNumT2)
    print(' CRRES var iNumT2=',iNumT2)
    assert(len(daTimes2)==iNumT2)
    # use time values to determine timesteps, 
    #   and then verify that resulting range is within limits
    iMin = int(dVarMaxStep2)
    iMax = int(dVarMinStep2)
    for ii in range(iNumT2-1):
      iDelta = int((daTimes2[ii+1]-daTimes2[ii])*86400.0+0.0001)
      print(ii,': ',iDelta )
      if iDelta < iMin: iMin = iDelta
      if iDelta > iMax: iMax = iDelta
    print(' timestep range:',iMin,' -',iMax)
    assert(iMin>=int(dVarMinStep2))
    assert(iMax<=int(dVarMaxStep2))


def test_set_tle(ephem):
    strFile = ephem.get_tleFile()
    print(' get_tleFile: returned tleFile=', strFile)
    assert(len(strFile) == 0)

    # load up 3 sets of TLEs
    strLine1 = '1 20712U 90065A   05027.28729034  .00000029  00000-0  20288-4 0   166'
    strLine2 = '2 20712  18.2943  30.1164 7147621 257.9138  20.6811  2.41951061118231'
    ierr = ephem.set_tle(strLine1, strLine2)
    print(' set_tle: <2 lines, no validation> => ', ierr)
    assert(ierr == 0)

    strFile = ephem.get_tleFile()
    print(' get_tleFile: returned tleFile=', strFile)
    assert(len(strFile) == 0)

    strLine1 = '1 20712U 90065A   05035.13698615 -.00000073  00000-0  47384-3 0   254'
    strLine2 = '2 20712  18.3032  26.8942 7146260 263.9307  18.0655  2.41962566118420'
    ierr = ephem.set_tle(strLine1, strLine2)
    print(' set_tle: <2 lines, no validation> => ', ierr)
    assert(ierr == 0)

    strLine1 = '1 20712U 90065A   05041.33434282 +.00000913 +00000-0 +72155-3 0 00303'
    strLine2 = '2 20712 018.3067 024.4000 7145197 268.6357 016.3605 02.41970578118576'
    ierr = ephem.set_tle(strLine1, strLine2)
    print(' set_tle: <2 lines, no validation> => ', ierr)
    assert(ierr == 0)

    # verify number of TLEs loaded
    iNum = ephem.get_numTle()
    print(' get_numTle: => ', iNum)
    assert(iNum == 3)

    # attempt to retrieve these TLEs, and imaginary ones
    for iEntry in [3, -1]:
        ierr, strLine1, strLine2 = ephem.get_tle(iEntry)
        print(' get_tle: iEntry=', iEntry, ' (start at 0) => ', ierr)
        print('  > Line1 = ', strLine1)
        print('  > Line2 = ', strLine2)
        assert(ierr < 0)
        assert(len(strLine1) == 0)
    for iEntry in [1, 2, 0]:
        ierr, strLine1, strLine2 = ephem.get_tle(iEntry)
        print(' get_tle: iEntry=', iEntry, ' (start at 0) => ', ierr)
        print('  > Line1 = ', strLine1)
        print('  > Line2 = ', strLine2)
        assert(ierr == 0)
        assert(len(strLine1) > 0)

    # reset of TLEs, and confirm
    ierr = ephem.reset_tleInputs()
    print(' reset_tleInputs: => ', ierr)
    assert(ierr == 0)
    iNum = ephem.get_numTle()
    print(' get_numTle: => ', iNum)
    assert(iNum == 0)

    iEntry = 0
    ierr, strLine1, strLine2 = ephem.get_tle(iEntry)
    print(' get_tle: iEntry=', iEntry, ' (start at 0) => ', ierr)
    print('  > Line1 = ', strLine1)
    print('  > Line2 = ', strLine2)
    assert(ierr < 0)
    assert(len(strLine1) == 0)
    strFile = ephem.get_tleFile()
    print(' get_tleFile: returned tleFile=', strFile)
    assert(len(strLine1) == 0)


def test_set_elementTime(ephem):
    for dMjd in [31000.2, 70123.45]:
        ierr = ephem.set_elementTime(dMjd)
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr < 0)

        elementTime = ephem.get_elementTime()
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == 0.0)

    for dMjd in [56789.4, 51145.5, 58119.5]:
        ierr = ephem.set_elementTime(dMjd)
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)

        elementTime = ephem.get_elementTime()
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)


def test_set_inclination(ephem):
    for dValue in [-35.0, 191.0]:
        ierr = ephem.set_inclination(dValue)
        print(' set_inclination: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        inclination = ephem.get_inclination()
        print(' get_inclination: => ', inclination)
        assert(inclination == 0.0)

    for dValue in [45.0, 0.0, 98.7, 180.0, 25.0]:
        ierr = ephem.set_inclination(dValue)
        print(' set_inclination: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        inclination = ephem.get_inclination()
        print(' get_inclination: => ', inclination)
        assert(inclination == dValue)


def test_set_argOfPerigee(ephem):
    for dValue in [-35.0, 391.0]:
        ierr = ephem.set_argOfPerigee(dValue)
        print(' set_argOfPerigee: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        argOfPerigee = ephem.get_argOfPerigee()
        print(' get_argOfPerigee: => ', argOfPerigee)
        assert(argOfPerigee == 0.0)

    for dValue in [0.0, 45.0, 298.7, 180.0, 125.0]:
        ierr = ephem.set_argOfPerigee(dValue)
        print(' set_argOfPerigee: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        argOfPerigee = ephem.get_argOfPerigee()
        print(' get_argOfPerigee: => ', argOfPerigee)
        assert(argOfPerigee == dValue)


def test_set_meanAnomaly(ephem):
    for dValue in [-35.0]:
        ierr = ephem.set_meanAnomaly(dValue)
        print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        meanAnomaly = ephem.get_meanAnomaly()
        print(' get_meanAnomaly: => ', meanAnomaly)
        assert(meanAnomaly == 0.0)

    for dValue in [0.0, 45.0, 98.7, 180.0, 191.0, 25.0]:
        ierr = ephem.set_meanAnomaly(dValue)
        print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        meanAnomaly = ephem.get_meanAnomaly()
        print(' get_meanAnomaly: => ', meanAnomaly)
        assert(meanAnomaly == dValue)


def test_set_eccentricity(ephem):
    for dValue in [-0.5, 1.0, 4.5]:
        ierr = ephem.set_eccentricity(dValue)
        print(' set_eccentricity: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        eccentricity = ephem.get_eccentricity()
        print(' get_eccentricity: => ', eccentricity)
        assert(eccentricity == 0.0)

    for dValue in [0.0, 0.9999, 0.025, 0.25]:
        ierr = ephem.set_eccentricity(dValue)
        print(' set_eccentricity: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        eccentricity = ephem.get_eccentricity()
        print(' get_eccentricity: => ', eccentricity)
        assert(eccentricity == dValue)


def test_set_rightAscension(ephem):
    for dValue in [-35.0, 391.0]:
        ierr = ephem.set_rightAscension(dValue)
        print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        rightAscension = ephem.get_rightAscension()
        print(' get_rightAscension: => ', rightAscension)
        assert(rightAscension == 0.0)

    for dValue in [0.0, 45.0, 298.7, 180.0, 125.0]:
        ierr = ephem.set_rightAscension(dValue)
        print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        rightAscension = ephem.get_rightAscension()
        print(' get_rightAscension: => ', rightAscension)
        assert(rightAscension == dValue)


def test_set_meanMotion(ephem):
    for dValue in [0.0, -3.5,  31.0]:
        ierr = ephem.set_meanMotion(dValue)
        print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        meanMotion = ephem.get_meanMotion()
        print(' get_meanMotion: => ', meanMotion)
        assert(meanMotion == 0.0)

    for dValue in [4.5, 12.7, 17.0, 8.5]:
        ierr = ephem.set_meanMotion(dValue)
        print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        meanMotion = ephem.get_meanMotion()
        print(' get_meanMotion: => ', meanMotion)
        assert(meanMotion == dValue)


def test_set_meanMotion1stDeriv(ephem):
    for dValue in [12.7, -11.0]:
        ierr = ephem.set_meanMotion1stDeriv(dValue)
        print(' set_meanMotion1stDeriv: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        meanMotion1stDeriv = ephem.get_meanMotion1stDeriv()
        print(' get_meanMotion1stDeriv: => ', meanMotion1stDeriv)
        assert(meanMotion1stDeriv == 0.0)

    for dValue in [0.0, -3.5, 4.5, -7.0, 1.5]:
        ierr = ephem.set_meanMotion1stDeriv(dValue)
        print(' set_meanMotion1stDeriv: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        meanMotion1stDeriv = ephem.get_meanMotion1stDeriv()
        print(' get_meanMotion1stDeriv: => ', meanMotion1stDeriv)
        assert(meanMotion1stDeriv == dValue)


def test_set_meanMotion2ndDeriv(ephem):
    for dValue in [-1.5, 2.7]:
        ierr = ephem.set_meanMotion2ndDeriv(dValue)
        print(' set_meanMotion2ndDeriv: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        meanMotion2ndDeriv = ephem.get_meanMotion2ndDeriv()
        print(' get_meanMotion2ndDeriv: => ', meanMotion2ndDeriv)
        assert(meanMotion2ndDeriv == 0.0)

    for dValue in [0.0, 0.5, -0.7, 1.0, 0.05]:
        ierr = ephem.set_meanMotion2ndDeriv(dValue)
        print(' set_meanMotion2ndDeriv: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        meanMotion2ndDeriv = ephem.get_meanMotion2ndDeriv()
        print(' get_meanMotion2ndDeriv: => ', meanMotion2ndDeriv)
        assert(meanMotion2ndDeriv == dValue)


def test_set_bStar(ephem):
    for dValue in [-1.5, 2.7]:
        ierr = ephem.set_bStar(dValue)
        print(' set_bstar: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        bStar = ephem.get_bStar()
        print(' get_bStar: => ', bStar)
        assert(bStar == 0.0)

    for dValue in [0.0, 0.5, -0.7, 1.0, 0.05]:
        ierr = ephem.set_bStar(dValue)
        print(' set_bstar: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        bStar = ephem.get_bStar()
        print(' get_bStar: => ', bStar)
        assert(bStar == dValue)


def test_set_altitudeOfApogee(ephem):
    for dValue in [0.0, -35.0]:
        ierr = ephem.set_altitudeOfApogee(dValue)
        print(' set_altitudeOfApogee: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        altitudeOfApogee = ephem.get_altitudeOfApogee()
        print(' get_altitudeOfApogee: => ', altitudeOfApogee)
        assert(altitudeOfApogee == 0.0)

    for dValue in [45.0, 298.7, 180.0, 391.0, 525.0]:
        ierr = ephem.set_altitudeOfApogee(dValue)
        print(' set_altitudeOfApogee: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        altitudeOfApogee = ephem.get_altitudeOfApogee()
        print(' get_altitudeOfApogee: => ', altitudeOfApogee)
        assert(altitudeOfApogee == dValue)


def test_set_altitudeOfPerigee(ephem):
    for dValue in [0.0, -35.0]:
        ierr = ephem.set_altitudeOfPerigee(dValue)
        print(' set_altitudeOfPerigee: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        altitudeOfPerigee = ephem.get_altitudeOfPerigee()
        print(' get_altitudeOfPerigee: => ', altitudeOfPerigee)
        assert(altitudeOfPerigee == 0.0)

    for dValue in [45.0, 298.7, 180.0, 391.0, 525.0]:
        ierr = ephem.set_altitudeOfPerigee(dValue)
        print(' set_altitudeOfPerigee: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        altitudeOfPerigee = ephem.get_altitudeOfPerigee()
        print(' get_altitudeOfPerigee: => ', altitudeOfPerigee)
        assert(altitudeOfPerigee == dValue)


def test_set_localTimeOfApogee(ephem):
    for dValue in [-3.5, 31.0]:
        ierr = ephem.set_localTimeOfApogee(dValue)
        print(' set_localTimeOfApogee: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        localTimeOfApogee = ephem.get_localTimeOfApogee()
        print(' get_localTimeOfApogee: => ', localTimeOfApogee)
        assert(localTimeOfApogee == -1.0)

    for dValue in [0.0, 4.5, 12.7, 17.0, 8.5]:
        ierr = ephem.set_localTimeOfApogee(dValue)
        print(' set_localTimeOfApogee: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        localTimeOfApogee = ephem.get_localTimeOfApogee()
        print(' get_localTimeOfApogee: => ', localTimeOfApogee)
        assert(localTimeOfApogee == dValue)


def test_set_localTimeMaxInclination(ephem):
    for dValue in [-3.5, 31.0]:
        ierr = ephem.set_localTimeMaxInclination(dValue)
        print(' set_localTimeMaxInclination: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        localTimeMaxInclination = ephem.get_localTimeMaxInclination()
        print(' get_localTimeMaxInclination: => ', localTimeMaxInclination)
        assert(localTimeMaxInclination == -1.0)

    for dValue in [0.0, 4.5, 12.7, 17.0, 8.5]:
        ierr = ephem.set_localTimeMaxInclination(dValue)
        print(' set_localTimeMaxInclination: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        localTimeMaxInclination = ephem.get_localTimeMaxInclination()
        print(' get_localTimeMaxInclination: => ', localTimeMaxInclination)
        assert(localTimeMaxInclination == dValue)


def test_set_timeOfPerigee(ephem):
    for dMjd in [31000.2, 70123.4]:
        ierr = ephem.set_timeOfPerigee(dMjd)
        print(' set_timeOfPerigee: dMjd=', dMjd, ' => ', ierr)
        assert(ierr < 0)

        timeOfPerigee = ephem.get_timeOfPerigee()
        print(' get_timeOfPerigee: => ', timeOfPerigee)
        assert(timeOfPerigee == 0.0)

    for dMjd in [56789.4, 51145.5, 58119.5]:
        ierr = ephem.set_timeOfPerigee(dMjd)
        print(' set_timeOfPerigee: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)

        timeOfPerigee = ephem.get_timeOfPerigee()
        print(' get_timeOfPerigee: => ', timeOfPerigee)
        assert(timeOfPerigee == dMjd)


def test_set_semiMajorAxis(ephem):
    for dValue in [0.0, -1.5, 0.5, -6.7]:
        ierr = ephem.set_semiMajorAxis(dValue)
        print(' set_semiMajorAxis: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        semiMajorAxis = ephem.get_semiMajorAxis()
        print(' get_semiMajorAxis: => ', semiMajorAxis)
        assert(semiMajorAxis == 0.0)

    for dValue in [2.7, 1.0, 2.05]:
        ierr = ephem.set_semiMajorAxis(dValue)
        print(' set_semiMajorAxis: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        semiMajorAxis = ephem.get_semiMajorAxis()
        print(' get_semiMajorAxis: => ', semiMajorAxis)
        assert(semiMajorAxis == dValue)


def test_set_geosynchLon(ephem):
    for dValue in [-195.1, 395.6]:
        ierr = ephem.set_geosynchLon(dValue)
        print(' set_geosynchLon: dValue=', dValue, ' => ', ierr)
        assert(ierr < 0)

        geosynchLon = ephem.get_geosynchLon()
        print(' get_geosynchLon: => ', geosynchLon)
        assert(geosynchLon == -360.0)

    for dValue in [0.0, -35.0, 45.0, 298.7, 180.0, -180.0, 125.0]:
        ierr = ephem.set_geosynchLon(dValue)
        print(' set_geosynchLon: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)

        geosynchLon = ephem.get_geosynchLon()
        print(' get_geosynchLon: => ', geosynchLon)
        assert(geosynchLon == dValue)


def test_set_stateVectors(ephem):
    dValue1 = dValue2 = dValue3 = 0.0
    ierr = ephem.set_positionGei(dValue1, dValue2, dValue3)
    print(' set_positionGei: dValue=', dValue1,
          dValue2, dValue3, ' => ', ierr)
    assert(ierr < 0)
    ierr = ephem.set_velocityGei(dValue1, dValue2, dValue3)
    print(' set_velocityGei: dValue=', dValue1,
          dValue2, dValue3, ' => ', ierr)
    assert(ierr < 0)

    # position will be rejected, inside Earth
    dValue1 = dValue2 = dValue3 = 600.0
    ierr = ephem.set_positionGei(dValue1, dValue2, dValue3)
    print(' set_positionGei: dValue=', dValue1,
          dValue2, dValue3, ' => ', ierr)
    assert(ierr < 0)

    dPos = 5000.0
    ierr = ephem.set_positionGei(dPos, -dPos/2.0, dPos*1.5)
    print(' set_positionGei: dValue=', dPos, -
          dPos/2.0, dPos*1.5, ' => ', ierr)
    assert(ierr == 0)

    dVel = 6.0
    ierr = ephem.set_velocityGei(-dVel, dVel+1.5, -dVel/3.0)
    print(' set_velocityGei: dValue=', -dVel,
          dVel+1.5, -dVel/3.0, ' => ', ierr)
    assert(ierr == 0)

    daPos, daVel = ephem.get_stateVectors()
    print(' get_stateVectors: => ', daPos, daVel)
    assert(daPos[0] == dPos)
    assert(daPos[1] == -dPos/2.0)
    assert(daPos[2] == dPos*1.5)
    assert(daVel[0] == -dVel)
    assert(daVel[1] == dVel+1.5)
    assert(daVel[2] == -dVel/3.0)

    dPosX, dPosY, dPosZ = ephem.get_positionGei()
    dVelX, dVelY, dVelZ = ephem.get_velocityGei()

    assert(daPos[0] == dPosX)
    assert(daPos[1] == dPosY)
    assert(daPos[2] == dPosZ)
    assert(daVel[0] == dVelX)
    assert(daVel[1] == dVelY)
    assert(daVel[2] == dVelZ)

    # flip signs
    daPos[0] = -dPos
    daPos[1] = dPos/2.0
    daPos[2] = -dPos*1.5
    daVel[0] = dVel
    daVel[1] = -dVel+1.5
    daVel[2] = dVel/3.0
    ierr = ephem.set_stateVectors(daPos, daVel)
    print(' set_stateVectors: ', daPos, daVel, ' => ', ierr)
    assert(ierr == 0)

    dPosX, dPosY, dPosZ = ephem.get_positionGei()
    dVelX, dVelY, dVelZ = ephem.get_velocityGei()

    assert(daPos[0] == dPosX)
    assert(daPos[1] == dPosY)
    assert(daPos[2] == dPosZ)
    assert(daVel[0] == dVelX)
    assert(daVel[1] == dVelY)
    assert(daVel[2] == dVelZ)


def test_reset_orbitParameters(ephem):
    ephem.set_elementTime(55144.5)
    ephem.set_geosynchLon(155.4)
    print(' get_elementTime => ', ephem.get_elementTime())
    print(' get_geosynchLon => ', ephem.get_geosynchLon())
    ierr = ephem.reset_orbitParameters()
    print(' reset_orbitParameters: => ', ierr)
    assert(ierr == 0)
    dValue = ephem.get_elementTime()
    print(' get_elementTime => ', dValue)
    assert(dValue == 0)
    dValue = ephem.get_geosynchLon()
    print(' get_geosynchLon => ', dValue)
    assert(dValue == -360.0)


def test_set_chunkSize(ephem):
    iSize = ephem.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == 0)

    for iValue in [-5]:
        ierr = ephem.set_chunkSize(iValue)
        print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
        assert(ierr < 0)
    for iValue in [0, 2400, 960, 256]:
        ierr = ephem.set_chunkSize(iValue)
        print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
        assert(ierr == 0)

    iSize = ephem.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == 256)


def test_compute(ephem):
    ephem.set_magfieldDBFile('../../modelData/igrfDB.h5')
    ephem.set_chunkSize(256)

    dValue = 25.0
    ierr = ephem.set_inclination(dValue)
    print(' set_inclination: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 0.25
    ierr = ephem.set_eccentricity(dValue)
    print(' set_eccentricity: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = -8.5  # purposely set to invalid value -> fails
    ierr = ephem.set_meanMotion(dValue)
    print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
    assert(ierr < 0)
    dValue = 218.5
    ierr = ephem.set_meanAnomaly(dValue)
    print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ephem.set_rightAscension(dValue)
    print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dMjd = 58119.5
    ierr = ephem.set_elementTime(dMjd)
    print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
    assert(ierr == 0)
    dStep = 60.0
    dStart = 58119.5
    dEnd = 58120.5
    ierr = ephem.set_times(dStart, dEnd, dStep)
    print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' %
          (dStart, dEnd, dStep, ierr))
    assert(ierr == 0)
    dValue = -8.5  # purposely set to invalid value -> fails
    ierr = ephem.set_meanMotion(dValue)
    print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
    assert(ierr < 0)

    # try with invalid coordsys spec
    strCoordSys = 'GDX'
    strCoordUnits = 'ft'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum < 0)

    strCoordSys = 'GDZ'
    strCoordUnits = 'km'

    # try again, but fails on incomplete orbit specification, mean motion is not defined
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum < 0)

    # complete orbit specifications
    dValue = 8.5
    ierr = ephem.set_meanMotion(dValue)
    print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)

    print(' magfieldDB=',ephem.get_magfieldDBFile())
    print(' propagator=',ephem.get_prop())

    # get first chunk of ephemeris
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)
    if iNum > 0:
        print('   [   0 ]: ', daTime[0],
              daCoord1[0], daCoord2[0], daCoord3[0])
        print('   [', iNum-1, ']: ', daTime[iNum-1],
              daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    # keep going until end time reached
    while iNum > 0:
        iNum, daTime, daCoord1, daCoord2, daCoord3 \
            = ephem.compute(strCoordSys, strCoordUnits)
        print(' compute: CoordSys=', strCoordSys, strCoordUnits)
        print('  returned iNum=', iNum, '; len(daTime)=',
              len(daTime), ', len(daCoord1)=', len(daCoord1))
        assert(iNum >= 0)
        if iNum > 0:
            print('   [   0 ]: ', daTime[0],
                  daCoord1[0], daCoord2[0], daCoord3[0])
            print('   [', iNum-1, ']: ', daTime[iNum-1],
                  daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])

    # restart ephemeris
    ierr = ephem.restart()
    print(' restart: => ', ierr)
    assert(ierr == 0)
    print(' =============================================')

    # get first chunk again, verify starting time
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)
    assert(daTime[0] == dStart)
    if iNum > 0:
        print('   [   0 ]: ', daTime[0],
              daCoord1[0], daCoord2[0], daCoord3[0])
        print('   [', iNum-1, ']: ', daTime[iNum-1],
              daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])


def test_computeGei(ephem_default):
    # get first chunk of ephemeris in GEI
    iNum, daTime, daXPos, daYPos, daZPos, daXVel, daYVel, daZVel \
        = ephem_default.computeGei()  # , iSize=0 )
    print(' computeGei: ')
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daXPos)=', len(daXPos))
    assert(iNum > 0)
    if iNum > 0:
        print('   [   0 ]: ', daTime[0], daXPos[0], daYPos[0], daZPos[0])
        print('   [', iNum-1, ']: ', daTime[iNum-1],
              daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])


def test_convertCoords(ephem_default):
    # change chunk size, and verify
    iValue = 128
    ierr = ephem_default.set_chunkSize(iValue)
    print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
    assert(ierr == 0)
    iSize = ephem_default.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == 128)

    # get first chunk of ephmeris in GEI
    iNum, daTime, daXPos, daYPos, daZPos, daXVel, daYVel, daZVel \
        = ephem_default.computeGei()  # , iSize=0 )
    print(' computeGei: ')
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daXPos)=', len(daXPos))
    assert(iNum > 0)
    if iNum > 0:
        print('   [   0 ]: ', daTime[0], daXPos[0], daYPos[0], daZPos[0])
        print('   [', iNum-1, ']: ', daTime[iNum-1],
              daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])

    # get next chunk of ephemeris in GDZ
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem_default.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # try coordinate conversion to imaginary coord sys
    strCoordSysIn = 'ABC'
    strCoordUnitsIn = 'in'
    ierr, daCoord1Out, daCoord2Out, daCoord3Out \
        = ephem_default.convertCoords(strCoordSysIn, strCoordUnitsIn,
                                      daTime, daXPos, daYPos, daZPos,
                                      strCoordSys, strCoordUnits)
    print(' convertCoords: inCoords=', strCoordSysIn, strCoordUnitsIn,
          ' len(daTime)=', len(daTime), ', len(daXPos)=', len(daXPos), ' outCoords=', strCoordSys, strCoordUnits)
    print('  returned ierr=', ierr, '; len(daTime)=', len(
        daTime), ', len(daCoord1Out)=', len(daCoord1Out))
    assert(ierr < 0)

    # now convert GEI to GDZ
    strCoordSysIn = 'GEI'
    strCoordUnitsIn = 'km'
    ierr, daCoord1Out, daCoord2Out, daCoord3Out \
        = ephem_default.convertCoords(strCoordSysIn, strCoordUnitsIn,
                                      daTime, daXPos, daYPos, daZPos,
                                      strCoordSys, strCoordUnits)
    print(' convertCoords: inCoords=', strCoordSysIn, strCoordUnitsIn,
          ' len(daTime)=', len(daTime), ', len(daXPos)=', len(daXPos), ' outCoords=', strCoordSys, strCoordUnits)
    print('  returned ierr=', ierr, '; len(daTime)=', len(
        daTime), ', len(daCoord1Out)=', len(daCoord1Out))
    assert(ierr == 0)
    if ierr == 0:
        iNum = len(daTime)
        print('   [   0 ]: ', daTime[0], daCoord1Out[0],
              daCoord2Out[0], daCoord3Out[0])
        print('   [', iNum-1, ']: ', daTime[iNum-1],
              daCoord1Out[iNum-1], daCoord2Out[iNum-1], daCoord3Out[iNum-1])


def test_convertCoordsSingle(ephem_default):
    # change chunk size, confirm
    iValue = 256
    ierr = ephem_default.set_chunkSize(iValue)
    print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
    assert(ierr == 0)

    iSize = ephem_default.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == 256)

    # get first chunk of ephemeris in GDZ
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem_default.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # restart ephemeris
    ierr = ephem_default.restart()
    print(' restart: => ', ierr)
    assert(ierr == 0)

    # save the time and position coordinate values for future reference
    dTime = daTime[iNum-1]
    daPos = [daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1]]
    print(' saved values time=', dTime,
          ', pos=', daPos[0], daPos[1], daPos[2])

    # try coord conversion with invalid specification
    strCoordSysOut = 'Goo'
    strCoordUnitsOut = 'km'
    ierr, dCoord1, dCoord2, dCoord3  \
        = ephem_default.convertCoordsSingle(strCoordSys, strCoordUnits,
                                            dTime, daPos[0], daPos[1], daPos[2], strCoordSysOut, strCoordUnitsOut)
    print('  convertCoordsSingle: inCoords=', strCoordSys, strCoordUnits)
    print('     time=', dTime, ', pos=',
          daPos[0], daPos[1], daPos[2], '; outCoords=', strCoordSysOut, strCoordUnitsOut)
    print('     returned ierr=', ierr, ', pos=', dCoord1, dCoord2, dCoord3)
    assert(ierr < 0)

    # try conversion GDZ to GDZ
    strCoordSysOut = 'GDZ'
    strCoordUnitsOut = 'km'
    ierr, dCoord1, dCoord2, dCoord3  \
        = ephem_default.convertCoordsSingle(strCoordSys, strCoordUnits,
                                            dTime, daPos[0], daPos[1], daPos[2], strCoordSysOut, strCoordUnitsOut)
    print('  convertCoordsSingle: inCoords=', strCoordSys, strCoordUnits)
    print('     time=', dTime, ', pos=',
          daPos[0], daPos[1], daPos[2], '; outCoords=', strCoordSysOut, strCoordUnitsOut)
    print('     returned ierr=', ierr, ', pos=', dCoord1, dCoord2, dCoord3)
    assert(ierr < 0)

    # now finally do the single point coordinate conversion GDZ -> GEI
    strCoordSysOut = 'GEI'
    strCoordUnitsOut = 'km'
    ierr, dCoord1, dCoord2, dCoord3  \
        = ephem_default.convertCoordsSingle(strCoordSys, strCoordUnits,
                                            dTime, daPos[0], daPos[1], daPos[2], strCoordSysOut, strCoordUnitsOut)
    print('  convertCoordsSingle: inCoords=', strCoordSys, strCoordUnits)
    print('     time=', dTime, ', pos=',
          daPos[0], daPos[1], daPos[2], '; outCoords=', strCoordSysOut, strCoordUnitsOut)
    print('     returned ierr=', ierr, ', pos=', dCoord1, dCoord2, dCoord3)
    assert(ierr == 0)

    # generate the first chunk of ephemeris in GEI,
    #   then confirm match to previous converted coords
    iNum, daTime, daXPos, daYPos, daZPos, daXVel, daYVel, daZVel \
        = ephem_default.computeGei()  # , iSize=0 )
    print(' computeGei: ')
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daXPos)=', len(daXPos))
    assert(iNum > 0)
    if iNum > 0:
        print('   [   0 ]: ', daTime[0], daXPos[0], daYPos[0], daZPos[0])
        print('   [', iNum-1, ']: ', daTime[iNum-1],
              daXPos[iNum-1], daYPos[iNum-1], daZPos[iNum-1])
        assert (pytest.approx(daTime[iNum-1], 0.0001) == dTime)
        assert (pytest.approx(daXPos[iNum-1], 0.0001) == dCoord1)
        assert (pytest.approx(daYPos[iNum-1], 0.0001) == dCoord2)
        assert (pytest.approx(daZPos[iNum-1], 0.0001) == dCoord3)

def test_set_kpValue(ephem_default):
    # default Kp value should be -1 (undefined)
    ierr, dKp = ephem_default.get_kpValue()
    assert(dKp == -1.0)
    # set initial valid value, verify
    dKpSet = 4.33
    ierr = ephem_default.set_kpValue(dKpSet)
    assert(ierr == 0)
    ierr, dKp = ephem_default.get_kpValue()
    assert(dKp == dKpSet)
    # check for rejecting invalid values
    for dKp in [-1.0, 9.1, 14.4]:
      ierr = ephem_default.set_kpValue(dKp)
      assert(ierr != 0)
      # previously set value unchanged
      ierr, dKpCheck = ephem_default.get_kpValue()
      assert(dKpCheck == dKpSet)
    # verify acceptance of valid values
    for dKp in [0.0, 3.33, 4.66, 9.0, 2.66]:
      ierr = ephem_default.set_kpValue(dKp)
      assert(ierr == 0)
      # value updated
      ierr, dKpCheck = ephem_default.get_kpValue()
      assert(dKpCheck == dKp)
    # verify default ref & end times are undefined
    ierr, dRef = ephem_default.get_kpValuesRefTime()
    assert(dRef == -1.0)
    ierr, dEnd = ephem_default.get_kpValuesEndTime()
    assert(dEnd == -1.0)
    # set arbitrary reference time for Kp list
    dTimeRef = 53505.0
    # start with empty list, verify rejection
    daKp=np.array([]) 
    ierr = ephem_default.set_kpValues(dTimeRef,daKp)
    assert(ierr != 0)
    # define arbitrary two-day list of Kp values
    daKp=np.array([0.33,0.66,1.33,2.33,5.00,6.33,6.00,5.66, \
                   5.33,5.00,5.33,5.66,4.66,3.33,1.00,2.00])
    # try to define with bad reference time
    ierr = ephem_default.set_kpValues(dTimeRef+0.25,daKp)
    assert(ierr != 0)
    # verify ref time is still undefined
    ierr, dRef = ephem_default.get_kpValuesRefTime()
    assert(dRef == -1.0)
    # confirm successful list definition
    ierr = ephem_default.set_kpValues(dTimeRef,daKp)
    assert(ierr == 0)
    # verify now-defined ref/end times
    ierr, dRef = ephem_default.get_kpValuesRefTime()
    assert(dRef == dTimeRef)
    ierr, dEnd = ephem_default.get_kpValuesEndTime()
    assert(dEnd == dTimeRef+len(daKp)/8.0)
    # set time midway through sequence of Kp list
    dTime = dTimeRef+1.0
    ierr = ephem_default.update_time( dTime )
    assert(ierr == 0)
    # verify resulting Kp value is what was expected
    ierr, dKpCheck = ephem_default.get_kpValue()
    iIndex = int((dTime-dTimeRef)*8.0)
    if iIndex < 0: iIndex = 0
    assert(dKpCheck == daKp[iIndex])
    # advance time, and verify again
    dTime = dTime + 6.0*0.125
    ierr = ephem_default.update_time( dTime )
    assert(ierr == 0)
    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(dKpCheck == daKp[iIndex+6])
    # advance time beyond end of Kp data
    dTime = dTime + 6.0*0.125
    ierr = ephem_default.update_time( dTime )
    assert(ierr == 0)
    assert(dTime > dEnd)
    # verify kp is set to last available value
    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(dKpCheck == daKp[-1])
    # change time to be before reference time
    dTime = dTimeRef-1.0
    ierr = ephem_default.update_time( dTime )
    assert(ierr == 0)
    assert(dTime < dRef)
    # verify kp is set to first available value
    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(dKpCheck == daKp[0])
    # recheck end time is still defined
    ierr, dEnd = ephem_default.get_kpValuesEndTime()
    assert(dEnd > 0.0)
    # now define a 'static' Kp value again
    dKp = 5.33
    ierr = ephem_default.set_kpValue(dKp)
    assert(ierr == 0)
    # verify that kp value is updated
    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(dKpCheck == dKp)
    # change to a different time
    ierr = ephem_default.update_time( dTimeRef+1.75 )
    assert(ierr == 0)
    # confirm that kp value is unchanged with time change
    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(dKpCheck == dKp)
    # verify ref & end times are back to being undefined
    ierr, dRef = ephem_default.get_kpValuesRefTime()
    assert(dRef == -1.0)
    ierr, dEnd = ephem_default.get_kpValuesEndTime()
    assert(dEnd == -1.0)

def test_computeBfieldSingle(ephem_default):
    # change chunk size, confirm
    iValue = 256
    ierr = ephem_default.set_chunkSize(iValue)
    print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
    assert(ierr == 0)

    iSize = ephem_default.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == 256)

    # get first chunk of ephemeris in GDZ
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem_default.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # save the time and position coordinate values for future reference
    dTime = daTime[iNum-1]
    daPos = [daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1]]

    print( " Default magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    # try specifying bad coord system
    strCoordSys = 'Goo'
    strCoordUnits = 'km'
    ierr, daBVecGeo, dBMag, dBMin, dLm  \
        = ephem_default.compute_bFieldSingle(strCoordSys, strCoordUnits,
                                            dTime, daPos[0], daPos[1], daPos[2] )
    print('  compute_bFieldSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
    assert(ierr < 0)

    strCoordSys = 'Gdz'
    ierr, daBVecGeo, dBMag, dBMin, dLm  \
        = ephem_default.compute_bFieldSingle(strCoordSys, strCoordUnits,
                                            dTime, daPos[0], daPos[1], daPos[2] )
    print('  compute_bFieldSingle: inCoords=', strCoordSys, strCoordUnits)
    print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
       %(dTime, daPos[0], daPos[1], daPos[2], \
        daBVecGeo[0], daBVecGeo[1], daBVecGeo[2], \
        dBMag, dBMin, dLm )
         )
    assert(ierr == 0)

    # set external to 'none' to allow main fields to be set to dipole-types
    strExt = "none"
    ierr = ephem_default.set_externalField( strExt )
    assert(ierr == 0)

    for strMain in ["irfg", "tsyg89", "igrf", "offset", "fastigrf"]:
      ierr = ephem_default.set_mainField( strMain )
      if ierr == 0:
        print(" --accepted: input=",strMain,"; returned=",ephem_default.get_mainField() )
      else:
        print(" ##rejected: input=",strMain,";  current=",ephem_default.get_mainField() )
      if strMain == "irfg" or strMain == "tsyg89":
        assert(ierr != 0)

    for strExt in ["irfg", "op", "igrf", "none", "tsyg89"]:
      ierr = ephem_default.set_externalField( strExt )
      if ierr == 0:
        print(" --accepted: input=",strExt,"; returned=",ephem_default.get_externalField() )
      else:
        print(" ##rejected: input=",strExt,";  current=",ephem_default.get_externalField() )
      if strExt == "irfg" or strExt == "igrf":
        assert(ierr != 0)

    ierr, daBVecGeo, dBMag, dBMin, dLm  \
        = ephem_default.compute_bFieldSingle(strCoordSys, strCoordUnits,
                                            dTime, daPos[0], daPos[1], daPos[2] )
    print('  compute_bFieldSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
    assert(ierr != 0)

    print(" defining Kp value for T89 usage")
    for dKpVal in [-1.0, 10.0, 0.0, 1.0, 2.00, 3.66, 5.66, 9.0]:
      ierr = ephem_default.set_kpValue( dKpVal )
      if ierr == 0:
        print(" --accepted: KpVal=",dKpVal,"; returned=",ephem_default.get_kpValue() )
        ierr, daBVecGeo, dBMag, dBMin, dLm  \
            = ephem_default.compute_bFieldSingle(strCoordSys, strCoordUnits,
                                                dTime, daPos[0], daPos[1], daPos[2] )
        print('  compute_bFieldSingle: inCoords=', strCoordSys, strCoordUnits)
        print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
           %(dTime, daPos[0], daPos[1], daPos[2], \
            daBVecGeo[0], daBVecGeo[1], daBVecGeo[2], \
            dBMag, dBMin, dLm )
             )
        assert(ierr == 0)
      else:
        print(" ##rejected: KpVal=",dKpVal,";  current=",ephem_default.get_kpValue() )
        assert(ierr != 0)

    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(ierr == 0)
    assert(dKpCheck == dKpVal)
    print( " Kp value set to constant =",dKpCheck)
    dAlt = 500.0
    dLat = -55.0
    dLon = 340.0
    ierr, daBVecGeo, dBMag, dBMin, dLm \
        = ephem_default.compute_bFieldSingle(strCoordSys, strCoordUnits,
                                       dTime, dAlt, dLat, dLon )
    print('  compute_bFieldSingle: Alternate inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)
    print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
       %(dTime, dAlt, dLat, dLon, \
        daBVecGeo[0], daBVecGeo[1], daBVecGeo[2], \
        dBMag, dBMin, dLm )
         )

    strExt = "op"
    ierr = ephem_default.set_externalField( strExt )
    assert(ierr == 0)
    print('back to OP/Q external field model')
    print(" --accepted: input=",strExt,"; returned=",ephem_default.get_externalField() )
    ierr, daBVecGeo, dBMag, dBMin, dLm  \
          = ephem_default.compute_bFieldSingle(strCoordSys, strCoordUnits,
                                              dTime, daPos[0], daPos[1], daPos[2] )
    print('  compute_bFieldSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
    print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
       %(dTime, daPos[0], daPos[1], daPos[2], \
         daBVecGeo[0], daBVecGeo[1], daBVecGeo[2], \
         dBMag, dBMin, dLm )
         )
    ierr, daBVecGeo, dBMag, dBMin, dLm  \
        = ephem_default.compute_bFieldSingle(strCoordSys, strCoordUnits,
                                       dTime, dAlt, dLat, dLon )
    print('  compute_bFieldSingle: Alternate inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)
    print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
       %(dTime, dAlt, dLat, dLon, \
        daBVecGeo[0], daBVecGeo[1], daBVecGeo[2], \
        dBMag, dBMin, dLm )
         )

    for dLon in np.arange(0.0, 360.0, 15.0):
      for dLat in np.arange(-85.0,90.0,5.0):
        ierr, daBVecGeo, dBMag, dBMin, dLm  \
          = ephem_default.compute_bFieldSingle(strCoordSys, strCoordUnits,
                                       dTime, dAlt, dLat, dLon )
        print('  compute_bFieldSingle: grid inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
        assert(ierr == 0)
        print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
         %(dTime, dAlt, dLat, dLon, \
          daBVecGeo[0], daBVecGeo[1], daBVecGeo[2], \
          dBMag, dBMin, dLm )
           )

def test_computeBfield(ephem_default):
    # change chunk size, confirm
    iValue = 960
    ierr = ephem_default.set_chunkSize(iValue)
    print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
    assert(ierr == 0)

    iSize = ephem_default.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == iValue)

    # get first chunk of ephemeris in GEI  (960 chunk = 16 hours at 60sec timesteps)
    strCoordSys = 'GEI'
    strCoordUnits = 'km'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem_default.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # save the time and position coordinate values for future reference
    dTime = daTime[iNum-1]
    daPos = [daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1]]
    print(' saved values time=', dTime,
          ', pos=', daPos[0], daPos[1], daPos[2])

    # try specifying bad coord system
    strCoordSys = 'Goo'
    strCoordUnits = 'km'
    ierr, da2BVecGeo, daBMag, daBMin, daLm  \
        = ephem_default.compute_bField(strCoordSys, strCoordUnits,
                                       daTime, daCoord1, daCoord2, daCoord3 )
    print('  compute_bField: inCoords=', strCoordSys, strCoordUnits)
    print('     returned ierr=', ierr )
    assert(ierr < 0)

    print( " Default magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    strCoordSys = 'Gei'
    ierr, da2BVecGeo, daBMag, daBMin, daLm  \
        = ephem_default.compute_bField(strCoordSys, strCoordUnits,
                                       daTime, daCoord1, daCoord2, daCoord3 )
    print('  compute_bField: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)

    # report sampling of results, every two hours
    for ii in range(0,960,120):
      print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
         %(daTime[ii], daCoord1[ii], daCoord2[ii], daCoord3[ii], \
           da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2], \
           daBMag[ii], daBMin[ii], daLm[ii] )
           )

    strExt= "tsyg89"
    ierr = ephem_default.set_externalField( strExt )
    assert(ierr == 0)

    print( " Active magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    # confirm that kp value is undefined initially
    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(ierr == 0)
    assert(dKpCheck < 0.0)

    # bfield call w/ T89 will fail with undefined Kp value
    ierr, da2BVecGeo, daBMag, daBMin, daLm  \
        = ephem_default.compute_bField(strCoordSys, strCoordUnits,
                                       daTime, daCoord1, daCoord2, daCoord3 )
    print('  compute_bField: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr != 0)

    # define constant Kp value
    dKpVal = 1.33
    ierr = ephem_default.set_kpValue( dKpVal )
    assert(ierr == 0)

    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(ierr == 0)
    assert(dKpCheck == dKpVal)
    print( " Kp value set to constant =",dKpCheck)
    ierr, da2BVecGeo, daBMag, daBMin, daLm  \
        = ephem_default.compute_bField(strCoordSys, strCoordUnits,
                                       daTime, daCoord1, daCoord2, daCoord3 )
    print('  compute_bField: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)

    # report sampling of results, every two hours
    for ii in range(0,960,120):
      print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
         %(daTime[ii], daCoord1[ii], daCoord2[ii], daCoord3[ii], \
           da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2], \
           daBMag[ii], daBMin[ii], daLm[ii] )
           )

    dTimeRef = float(int(daTime[0]))
    # define arbitrary two-day list of Kp values, starts at 0000GMT
    daKpVals=np.array([0.33,0.66,1.33,2.33,5.00,6.33,6.00,3.66, \
                       2.33,4.00,5.33,6.66,4.66,3.33,1.00,2.00])
    ierr = ephem_default.set_kpValues( dTimeRef, daKpVals )
    assert(ierr == 0)
    print( " Kp value set to time history =",daKpVals)
    print( " Kp data referenced to time ",dTimeRef)
    ierr, da2BVecGeo, daBMag, daBMin, daLm  \
        = ephem_default.compute_bField(strCoordSys, strCoordUnits,
                                       daTime, daCoord1, daCoord2, daCoord3 )
    print('  compute_bField: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)

    # report sampling of results, every two hours
    for ii in range(0,960,120):
      print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
         %(daTime[ii], daCoord1[ii], daCoord2[ii], daCoord3[ii], \
           da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2], \
           daBMag[ii], daBMin[ii], daLm[ii] )
           )

    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(ierr == 0)
    print(" Kp at last time (",daTime[-1],") =",dKpCheck)
    iIndex = int((daTime[-1]-dTimeRef)*8.0)
    if iIndex < 0: iIndex = 0
    if iIndex >= len(daKpVals): iIndex = len(daKpVals)-1
    assert(dKpCheck == daKpVals[iIndex])
    # define constant Kp value
    dKpVal = 1.33
    ierr = ephem_default.set_kpValue( dKpVal )
    assert(ierr == 0)

    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(ierr == 0)
    assert(dKpCheck == dKpVal)
    print( " Kp value set to constant =",dKpCheck)
    ierr, da2BVecGeo, daBMag, daBMin, daLm  \
        = ephem_default.compute_bField(strCoordSys, strCoordUnits,
                                       daTime, daCoord3, daCoord2, daCoord1 )
    print('  compute_bField: Munged inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0 )
    # report sampling of results, every two hours
    for ii in range(0,960,30):
      print(" %10.4f at (%10.3e %10.3e %10.3e): [%12.5e %12.5e %12.5e]=>%11.4e min=%11.4e Lm=%7.5f"\
         %(daTime[ii], daCoord3[ii], daCoord2[ii], daCoord1[ii], \
           da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2], \
           daBMag[ii], daBMin[ii], daLm[ii] )
           )


def test_computeInvariantsSingle(ephem_default):
    # change chunk size, confirm
    iValue = 256
    ierr = ephem_default.set_chunkSize(iValue)
    print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
    assert(ierr == 0)

    iSize = ephem_default.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == 256)

    # get first chunk of ephemeris in GDZ
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem_default.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # save the time and position coordinate values for future reference
    dTime = daTime[iNum-1]
    daPos = [daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1]]

    print( " Default magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    # try specifying bad coord system
    strCoordSys = 'Goo'
    strCoordUnits = 'km'
    daPitchAngles = np.array([90.0, 80.0, 70.0])
    ierr, dBMin, daBMinPosGeo, daBVecGeo, daLm, daI  \
        = ephem_default.compute_invariantsSingle( strCoordSys, strCoordUnits,
                                                  dTime, daPos[0], daPos[1], daPos[2],
                                                  daPitchAngles )
    print('  compute_invariantsSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
    assert(ierr != 0)

    strCoordSys = 'Gdz'
    ierr, dBMin, daBMinPosGeo, daBVecGeo, daLm, daI  \
        = ephem_default.compute_invariantsSingle( strCoordSys, strCoordUnits,
                                                  dTime, daPos[0], daPos[1], daPos[2],
                                                  daPitchAngles )
    print('  compute_invariantsSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
    print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
       %(dTime, daPos[0], daPos[1], daPos[2], dBMin,\
         daBMinPosGeo[0], daBMinPosGeo[1], daBMinPosGeo[2]) )
    print("    Bvec=[%12.5e %12.5e %12.5e]" %(daBVecGeo[0], daBVecGeo[1], daBVecGeo[2]) )
    print("    PA:[",daPitchAngles,"]; Lm:[",daLm,"]; I:[",daI,"]")
    assert(ierr == 0)

    print(' setting external field to T89')
    for strExt in ["irfg", "op", "igrf", "none", "tsyg89"]:
      ierr = ephem_default.set_externalField( strExt )
      if ierr == 0:
        print(" --accepted: input=",strExt,"; returned=",ephem_default.get_externalField() )
      else:
        print(" ##rejected: input=",strExt,";  current=",ephem_default.get_externalField() )
      if strExt == "irfg" or strExt == "igrf":
        assert(ierr != 0)

    print( " Active magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    # confirm that kp value is undefined initially
    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(ierr == 0)
    assert(dKpCheck < 0.0)

    # will fail on Kp -1 value
    ierr, dBMin, daBMinPosGeo, daBVecGeo, daLm, daI  \
        = ephem_default.compute_invariantsSingle( strCoordSys, strCoordUnits,
                                                  dTime, daPos[0], daPos[1], daPos[2],
                                                  daPitchAngles )
    print('  compute_invariantsSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
    assert(ierr != 0)

    print(" defining Kp value for T89 usage")
    for dKpVal in [-1.0, 10.0, 0.0, 1.0, 2.00, 3.66, 5.66, 9.0]:
      ierr = ephem_default.set_kpValue( dKpVal )
      if ierr == 0:
        print(" --accepted: KpVal=",dKpVal,"; returned=",ephem_default.get_kpValue() )
        ierr, dBMin, daBMinPosGeo, daBVecGeo, daLm, daI  \
            = ephem_default.compute_invariantsSingle( strCoordSys, strCoordUnits,
                                                      dTime, daPos[0], daPos[1], daPos[2],
                                                      daPitchAngles )
        print('  compute_invariantsSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
        print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
           %(dTime, daPos[0], daPos[1], daPos[2], dBMin,\
             daBMinPosGeo[0], daBMinPosGeo[1], daBMinPosGeo[2]) )
        print("    Bvec=[%12.5e %12.5e %12.5e]" %(daBVecGeo[0], daBVecGeo[1], daBVecGeo[2]) )
        print("    PA:[",daPitchAngles,"]; Lm:[",daLm,"]; I:[",daI,"]")
        assert(ierr == 0)
      else:
        print(" ##rejected: KpVal=",dKpVal,";  current=",ephem_default.get_kpValue() )
        assert(ierr != 0)

    strExt = "op"
    ierr = ephem_default.set_externalField( strExt )
    if ierr == 0:
      print('back to OP/Q external field model')
      print(" --accepted: input=",strExt,"; returned=",ephem_default.get_externalField() )
    assert( ierr == 0 )

    daPitchAngles = np.array([90.0, 80.0, 70.0])
    print(' defining pitch angle list in *decending* order: ',daPitchAngles);
    ierr, dBMin, daBMinPosGeo, daBVecGeo, daLm, daI  \
        = ephem_default.compute_invariantsSingle( strCoordSys, strCoordUnits,
                                                  dTime, daPos[0], daPos[1], daPos[2],
                                                  daPitchAngles )
    print('  compute_invariantsSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
    print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
       %(dTime, daPos[0], daPos[1], daPos[2], dBMin,\
         daBMinPosGeo[0], daBMinPosGeo[1], daBMinPosGeo[2]) )
    print("    Bvec=[%12.5e %12.5e %12.5e]" %(daBVecGeo[0], daBVecGeo[1], daBVecGeo[2]) )
    print("    PA:[",daPitchAngles,"]; Lm:[",daLm,"]; I:[",daI,"]")
    assert(ierr == 0)
    daPitchAngles = np.array([70.0, 80.0, 90.0])
    print(' defining pitch angle list in *ascending* order: ',daPitchAngles);
    ierr, dBMin, daBMinPosGeo, daBVecGeo, daLm, daI  \
        = ephem_default.compute_invariantsSingle( strCoordSys, strCoordUnits,
                                                  dTime, daPos[0], daPos[1], daPos[2],
                                                  daPitchAngles )
    print('  compute_invariantsSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
    assert(ierr != 0)

    print(' expanded pitch angle list')
    daPitchAngles = np.array([90.0, 75.0, 60.0, 45.0, 30.0, 15.0])

    ierr, dBMin, daBMinPosGeo, daBVecGeo, daLm, daI  \
        = ephem_default.compute_invariantsSingle( strCoordSys, strCoordUnits,
                                                  dTime, daPos[0], daPos[1], daPos[2],
                                                  daPitchAngles )
    print('  compute_invariantsSingle: inCoords=', strCoordSys, strCoordUnits,'; return=',ierr)
    print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
       %(dTime, daPos[0], daPos[1], daPos[2], dBMin,\
         daBMinPosGeo[0], daBMinPosGeo[1], daBMinPosGeo[2]) )
    print("    Bvec=[%12.5e %12.5e %12.5e]" %(daBVecGeo[0], daBVecGeo[1], daBVecGeo[2]) )
    print("    PA:[",daPitchAngles,"]; Lm:[",daLm,"]; I:[",daI,"]")
    assert(ierr == 0)

def test_computeInvariants(ephem_default):
    # change chunk size, confirm
    iValue = 960
    ierr = ephem_default.set_chunkSize(iValue)
    print(' set_chunkSize: iValue=', iValue, ' => ', ierr)
    assert(ierr == 0)

    iSize = ephem_default.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == iValue)

    # get first chunk of ephemeris in GEI  (960 chunk = 16 hours at 60sec timesteps)
    strCoordSys = 'GEI'
    strCoordUnits = 'km'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem_default.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)

    # try specifying bad coord system
    strCoordSys = 'Goo'
    strCoordUnits = 'km'
    daPitchAngles = np.array([90.0, 80.0, 70.0])
    ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I  \
        = ephem_default.compute_invariants( strCoordSys, strCoordUnits,
                                            daTime, daCoord1, daCoord2, daCoord3,
                                            daPitchAngles )
    print('  compute_invariants: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr < 0)

    print( " Default magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    strCoordSys = 'Gei'
    ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I  \
        = ephem_default.compute_invariants( strCoordSys, strCoordUnits,
                                            daTime, daCoord1, daCoord2, daCoord3,
                                            daPitchAngles )
    print('  compute_invariants: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)

    # report sampling of results, every two hours
    for ii in range(0,960,120):
      print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
         %(daTime[ii], daCoord1[ii], daCoord2[ii], daCoord3[ii], daBMin[ii],\
           da2BMinPosGeo[ii][0], da2BMinPosGeo[ii][1], da2BMinPosGeo[ii][2]) )
      print("    Bvec=[%12.5e %12.5e %12.5e]" %(da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2]) )
      print("    PA:[",daPitchAngles,"]; Lm:[",da2Lm[ii],"]; I:[",da2I[ii],"]")
      ## "tripwire" to catch occurences of expected occasional negative Lm values
      ## assert(np.sum(da2Lm[ii]>-15.0)==np.sum(da2Lm[ii]>0.0))

    # set external to 'none' to allow main fields to be set to dipole-types
    strExt = "none"
    ierr = ephem_default.set_externalField( strExt )
    assert(ierr == 0)

    strMain = "tilteddipole"
    ierr = ephem_default.set_mainField( strMain )
    assert(ierr == 0)
    # only 'none' external field is allowed for dipole main field
    strExt= "tsyg89"
    ierr = ephem_default.set_externalField( strExt )
    assert(ierr != 0)

    print( " Active magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    strMain= "fastigrf"
    ierr = ephem_default.set_mainField( strMain )
    assert(ierr == 0)
    strExt= "tsyg89"
    ierr = ephem_default.set_externalField( strExt )
    assert(ierr == 0)

    print( " Active magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    # confirm that kp value is undefined initially
    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(ierr == 0)
    assert(dKpCheck < 0.0)

    # invariants call w/ T89 will fail with undefined Kp value
    ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I  \
        = ephem_default.compute_invariants( strCoordSys, strCoordUnits,
                                            daTime, daCoord1, daCoord2, daCoord3,
                                            daPitchAngles )
    print('  compute_invariants: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr != 0)

    # define constant Kp value
    dKpVal = 1.33
    ierr = ephem_default.set_kpValue( dKpVal )
    assert(ierr == 0)

    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(ierr == 0)
    assert(dKpCheck == dKpVal)
    print( " Kp value set to constant =",dKpCheck)
    ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I  \
        = ephem_default.compute_invariants( strCoordSys, strCoordUnits,
                                            daTime, daCoord1, daCoord2, daCoord3,
                                            daPitchAngles )
    print('  compute_invariants: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)

    # report sampling of results, every two hours
    for ii in range(0,960,120):
      print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
         %(daTime[ii], daCoord1[ii], daCoord2[ii], daCoord3[ii], daBMin[ii],\
           da2BMinPosGeo[ii][0], da2BMinPosGeo[ii][1], da2BMinPosGeo[ii][2]) )
      print("    Bvec=[%12.5e %12.5e %12.5e]" %(da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2]) )
      print("    PA:[",daPitchAngles,"]; Lm:[",da2Lm[ii],"]; I:[",da2I[ii],"]")
      ## "tripwire" to catch occurences of expected occasional negative Lm values
      ## assert(np.sum(da2Lm[ii]>-15.0)==np.sum(da2Lm[ii]>0.0))

    dTimeRef = float(int(daTime[0]))
    # define arbitrary two-day list of Kp values, starts at 0000GMT
    daKpVals=np.array([0.33,0.66,1.33,2.33,5.00,6.33,6.00,3.66, \
                       2.33,4.00,5.33,6.66,4.66,3.33,1.00,2.00])
    ierr = ephem_default.set_kpValues( dTimeRef, daKpVals )
    assert(ierr == 0)
    print( " Kp value set to time history =",daKpVals)
    print( " Kp data referenced to time ",dTimeRef)
    ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I  \
        = ephem_default.compute_invariants( strCoordSys, strCoordUnits,
                                            daTime, daCoord1, daCoord2, daCoord3,
                                            daPitchAngles )
    print('  compute_invariants: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)

    # report sampling of results, every two hours
    for ii in range(0,960,120):
      print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
         %(daTime[ii], daCoord1[ii], daCoord2[ii], daCoord3[ii], daBMin[ii],\
           da2BMinPosGeo[ii][0], da2BMinPosGeo[ii][1], da2BMinPosGeo[ii][2]) )
      print("    Bvec=[%12.5e %12.5e %12.5e]" %(da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2]) )
      print("    PA:[",daPitchAngles,"]; Lm:[",da2Lm[ii],"]; I:[",da2I[ii],"]")
      ## "tripwire" to catch occurences of expected occasional negative Lm values
      ## assert(np.sum(da2Lm[ii]>-15.0)==np.sum(da2Lm[ii]>0.0))

    ierr, dKpCheck = ephem_default.get_kpValue()
    assert(ierr == 0)
    print(" Kp at last time (",daTime[-1],") =",dKpCheck)
    iIndex = int((daTime[-1]-dTimeRef)*8.0)
    if iIndex < 0: iIndex = 0
    if iIndex >= len(daKpVals): iIndex = len(daKpVals)-1
    assert(dKpCheck == daKpVals[iIndex])

    print(" changing back to OP external field ------")
    strExt= "OP"
    ierr = ephem_default.set_externalField( strExt )
    assert(ierr == 0)

    print( " Active magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    print( " defining input arrays with single entry")
    #2021 182 10140.0
    daTime = np.array([59396.117361111])
    strCoordSys = 'Gdz'
    daCoord2 = np.array([-69.60])
    daCoord3 = np.array([206.77])
    daCoord1 = np.array([912.2])
    print( " using larger pitch angle list");
    daPitchAngles = np.array([90.0, 75.0, 60.0, 50.0, 40.0])
    ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I  \
        = ephem_default.compute_invariants( strCoordSys, strCoordUnits,
                                            daTime, daCoord1, daCoord2, daCoord3,
                                            daPitchAngles )
    print('  compute_invariants: [single coord] inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)
    for ii in [0]:
      print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
         %(daTime[ii], daCoord1[ii], daCoord2[ii], daCoord3[ii], daBMin[ii],\
           da2BMinPosGeo[ii][0], da2BMinPosGeo[ii][1], da2BMinPosGeo[ii][2]) )
      print("    Bvec=[%12.5e %12.5e %12.5e]" %(da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2]) )
      print("    PA:[",daPitchAngles,"]; Lm:[",da2Lm[ii],"]; I:[",da2I[ii],"]")
      ## "tripwire" to catch occurences of expected occasional negative Lm values
      ## assert(np.sum(da2Lm[ii]>-15.0)==np.sum(da2Lm[ii]>0.0))

    daPitchAngles = np.array([70.0, 80.0, 90.0])
    print(' defining pitch angle list in *ascending* order: ',daPitchAngles);
    ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I  \
        = ephem_default.compute_invariants( strCoordSys, strCoordUnits,
                                            daTime, daCoord1, daCoord2, daCoord3,
                                            daPitchAngles )
    print('  compute_invariants: [single coord] inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr != 0)
   
    ### comment out next line to do calculations for full GDZ grid at one altitude
    if dKpCheck>0: return

    print(" ====== using set of grid coordinates at single time ====")
    daPitchAngles = np.array([90.0, 75.0, 60.0, 50.0, 40.0])
    strCoordSys = 'Gdz'
    # set up a grid pattern
    dTime = daTime[0]
    dAlt = 5000.0
    daLon = np.arange(0.0, 360.0, 10.0)
    daLat = np.arange(-80.0,85.0,5.0)
    daCoord2 = np.repeat(daLat,daLon.size)
    daCoord3 = np.hstack([daLon]*daLat.size)
    daCoord1 = np.array([dAlt]*daLon.size*daLat.size)
    daTime = np.array([dTime]*daLon.size*daLat.size)

    #print ('T[',daTime.size,']=',daTime)
    #print ('1[',daCoord1.size,']=',daCoord1)
    #print ('2[',daCoord2.size,']=',daCoord2)
    #print ('3[',daCoord3.size,']=',daCoord3)

    print( " Active magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I  \
        = ephem_default.compute_invariants( strCoordSys, strCoordUnits,
                                            daTime, daCoord1, daCoord2, daCoord3,
                                            daPitchAngles )
    print('  compute_invariants: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)
    for ii in range(daTime.size):
      print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
         %(daTime[ii], daCoord1[ii], daCoord2[ii], daCoord3[ii], daBMin[ii],\
           da2BMinPosGeo[ii][0], da2BMinPosGeo[ii][1], da2BMinPosGeo[ii][2]) )
      print("    Bvec=[%12.5e %12.5e %12.5e]" %(da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2]) )
      print("    PA:[",daPitchAngles,"]; Lm:[",da2Lm[ii],"]; I:[",da2I[ii],"]")
      ## "tripwire" to catch occurences of expected occasional negative Lm values
      ## assert(np.sum(da2Lm[ii]>-15.0)==np.sum(da2Lm[ii]>0.0))
 
    print(" changing back to OP external field ------")
    strExt= "OP"
    ierr = ephem_default.set_externalField( strExt )
    assert(ierr == 0)

    print( " Active magfield models: ",
      ephem_default.get_mainField(), ephem_default.get_externalField() )

    ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I  \
        = ephem_default.compute_invariants( strCoordSys, strCoordUnits,
                                            daTime, daCoord1, daCoord2, daCoord3,
                                            daPitchAngles )
    print('  compute_invariants: inCoords=', strCoordSys, strCoordUnits,' return=',ierr)
    assert(ierr == 0)
    for ii in range(daTime.size):
      print(" => %10.4f at (%10.3e %10.3e %10.3e): min=%11.4e at [%12.5e %12.5e %12.5e]"\
         %(daTime[ii], daCoord1[ii], daCoord2[ii], daCoord3[ii], daBMin[ii],\
           da2BMinPosGeo[ii][0], da2BMinPosGeo[ii][1], da2BMinPosGeo[ii][2]) )
      print("    Bvec=[%12.5e %12.5e %12.5e]" %(da2BVecGeo[ii][0], da2BVecGeo[ii][1], da2BVecGeo[ii][2]) )
      print("    PA:[",daPitchAngles,"]; Lm:[",da2Lm[ii],"]; I:[",da2I[ii],"]")
      ## "tripwire" to catch occurences of expected occasional negative Lm values
      ## assert(np.sum(da2Lm[ii]>-15.0)==np.sum(da2Lm[ii]>0.0))

def test_second_instance(ephem_default):
    # start a second instance of the ephemeris object, verify not the same as the first
    ephem2 = EphemModel()
    print('2nd ephem zHandle=', ephem2.zHandle)
    assert(ephem2.zHandle > 0)
    assert(ephem2.zHandle != ephem_default.zHandle)

    # define a random orbit
    dValue = 55.0
    ierr = ephem2.set_inclination(dValue)
    print(' set_inclination: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 5400.0
    ierr = ephem2.set_altitudeOfApogee(dValue)
    print(' set_altitudeOfApogee: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 400.0
    ierr = ephem2.set_altitudeOfPerigee(dValue)
    print(' set_altitudeOfPerigee: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ephem2.set_rightAscension(dValue)
    print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 105.0
    ierr = ephem2.set_argOfPerigee(dValue)
    print(' set_argOfPerigee: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 235.0
    ierr = ephem2.set_meanAnomaly(dValue)
    print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dMjd = 58119.5
    ierr = ephem2.set_elementTime(dMjd)
    print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
    assert(ierr == 0)

    # verify that ZERO chunk size is specified
    iSize = ephem2.get_chunkSize()
    print(' get_chunkSize: returned iSize=', iSize)
    assert(iSize == 0)

    # make call without ephemeris generation limits defined -> fails
    strCoordSys = 'RLL'
    strCoordUnits = 'Re'
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem2.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum < 0)

    # define ephemeris generation limits
    dStep = 60.0
    dStart = 58119.5
    dEnd = 58120.5
    ierr = ephem2.set_times(dStart, dEnd, dStep)
    print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' %
          (dStart, dEnd, dStep, ierr))
    assert(ierr == 0)

    # make call again, with generation limits defined
    #   -> but fails on lack of mag field db specification
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem2.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum < 0)

    # specify the mag field db file
    strFile = '../../modelData/igrfDB.h5'
    ierr = ephem2.set_magfieldDBFile(strFile)
    print(' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
    assert(ierr == 0)

    # make call, this time successful
    #  -> verify sizing of data returned
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem2.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    assert(iNum == 1441)
    assert(len(daTime) == iNum)
    assert(len(daCoord1) == iNum)
    # get associated time list length
    iNumT = ephem2.get_numTimesList()
    assert(iNumT==iNum)

    # test variable timestep ephemeris generation
    dVarMinStep = 60.0
    dVarMaxStep = 1200.0
    dVarRound = 1.0
    ierr = ephem2.set_varTimeStep(dVarMinStep,dVarMaxStep,dVarRound)
    print(' set_varTimeStep: vstep=%.0f,%.0f,%.1f => %i' % (dVarMinStep,dVarMaxStep,dVarRound,ierr))
    assert(ierr==0)
    dVarMinStep2, dVarMaxStep2, dVarRound2 = ephem2.get_varTimeStep()
    assert(dVarMinStep2==dVarMinStep)
    assert(dVarMaxStep2==dVarMaxStep)
    assert(dVarRound2==dVarRound)
    # make call, this time successful
    #  -> verify sizing of data returned
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem2.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    # get associated time list length
    iNumT = ephem2.get_numTimesList()
    assert(iNumT==iNum)
    # use time values to determine timesteps, 
    #   and then verify that resulting range is within limits
    iMin = int(dVarMaxStep2)
    iMax = int(dVarMinStep2)
    for ii in range(iNum-1):
      iDelta = int((daTime[ii+1]-daTime[ii])*86400.0+0.0001)
#      print(ii,': ',iDelta )
      if iDelta < iMin: iMin = iDelta
      if iDelta > iMax: iMax = iDelta
    print(' timestep range:',iMin,' -',iMax)
    assert(iMin>=int(dVarMinStep2))
    assert(iMax<=int(dVarMaxStep2))

    # modify orbit, increasing apogee altitude
    dValue = ephem2.get_altitudeOfApogee() * 6.0
    ierr = ephem2.set_altitudeOfApogee(dValue)
    print(' set_altitudeOfApogee: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    # make call, this time successful
    #  -> verify sizing of data returned
    iNum, daTime, daCoord1, daCoord2, daCoord3 \
        = ephem2.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTime)=',
          len(daTime), ', len(daCoord1)=', len(daCoord1))
    # get associated time list length
    iNumT2 = ephem2.get_numTimesList()
    assert(iNumT2==iNum)
    # should be shorter length, due to longer timesteps
    assert(iNumT2<iNumT)
    # use time values to determine timesteps, 
    #   and then verify that resulting range is bound by limits
    iMin = int(dVarMaxStep2)
    iMax = int(dVarMinStep2)
    for ii in range(iNum-1):
      iDelta = int((daTime[ii+1]-daTime[ii])*86400.0+0.0001)
#      print(ii,': ',iDelta )
      if iDelta < iMin: iMin = iDelta
      if iDelta > iMax: iMax = iDelta
    print(' timestep range:',iMin,' -',iMax)
    assert(iMin>=int(dVarMinStep2))
    assert(iMax<=int(dVarMaxStep2))

    # shut down this alternate instance
    del ephem2

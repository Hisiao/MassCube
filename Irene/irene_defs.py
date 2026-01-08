##############################################################################
# IMPORTANT - Do _NOT_ copy or move this file from the Irene root directory
##############################################################################
# irene_defs.py
#  Defines several IRENE-related environment variables
#   that enables the platform-specific libraries to be 
#   properly loaded and executed.
#  Also defines other environment variables for database
#   and supporting file locations.
#
#  python scripts using the IRENE (nee Ae9Ap9) python modules need to
#  first import this file, which will enable the proper loading of others
#
#  Successful 'import irene_defs.py' requires that the Irene root directory
#  path be either:
#   -included in the user's PYTHONPATH environment variable
#   -used in the call to  sys.path.append( <dir> ) prior to 'import irene_defs.py'
#

# enable use of python3 print function form in python 2.x
from __future__ import print_function

import os
import sys
import platform

# get directory in which this 'irene_defs.py' file is located
#  ---should be in root directory of Irene installation---
strIreneHome = os.path.dirname(os.path.realpath(__file__))

# check for previously-defined IRENE_HOME env var
strIreneEnv = os.getenv("IRENE_HOME")
if strIreneEnv is not None:
  # get 'true' path of this environment variable
  strTestFile = os.path.sep.join([os.getenv("IRENE_HOME"),'irene_defs.py'])
  strIreneTest = os.path.dirname(os.path.realpath(strTestFile))

  # compare the derived location to this env var directory  
  if strIreneHome != strIreneTest:
    # describe conflict and exit
    print (' Previously-defined IRENE_HOME variable conflict', file=sys.stderr)
    print ('   IRENE_HOME =',strIreneTest, file=sys.stderr)
    print ('   import dir =',strIreneHome, ' (from "import irene_defs")', file=sys.stderr)
    sys.exit(1)

# determine system type
strSystem = 'linux'
if platform.system() == "Windows":
  strSystem = 'win32'
  if platform.architecture()[0] == "64bit":
    strSystem = 'win64'
print (' detected platform =',strSystem, file=sys.stderr)

# verify the system-specific installation bin path actually exists
#  (also confirms that the irene_defs.py file has not been moved)
strTestDir = os.path.sep.join([strIreneHome,strSystem,'bin'])
if not os.path.isdir( strTestDir ):
  sys.exit( ' IRENE bin directory '+strTestDir+' does not exist\n exiting...' )

# set the various environment variables for later reference
os.environ['IRENE_HOME'] = strIreneHome
os.environ['IRENE_BIN'] = os.path.sep.join([os.environ['IRENE_HOME'],strSystem,'bin'])
strLibDir = os.path.sep.join([os.environ['IRENE_HOME'],strSystem,'lib'])
if not os.path.isdir( strLibDir ):
  strLibDir = os.path.sep.join([os.environ['IRENE_HOME'],strSystem,'lib64'])
  if not os.path.isdir( strLibDir ):
    sys.exit( ' IRENE lib directory '+strLibDir+' does not exist\n exiting...' )
os.environ['IRENE_LIB'] = strLibDir
os.environ['IRENE_SYS'] = strSystem
os.environ['IRENE_DB']  = os.path.sep.join([os.environ['IRENE_HOME'],'modelData'])
os.environ['IRENE_XML'] = os.path.sep.join([os.environ['IRENE_HOME'],'kernelXml'])

print (' IRENE_HOME =', os.environ['IRENE_HOME'], file=sys.stderr)
print (' IRENE_BIN  =', os.environ['IRENE_BIN'], file=sys.stderr)

# add the platform-specific bin directory to both python 'sys.path' and the system 'PATH'
sys.path.insert(0,os.environ['IRENE_BIN'])
os.environ['PATH'] = os.pathsep.join([os.environ['IRENE_BIN'],os.environ['PATH']])

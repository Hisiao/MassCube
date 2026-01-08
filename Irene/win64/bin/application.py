#******************************************************************************
#
# File: application.py
#
# Description: Python language routines for the programmatic interface
#   with the Ae9/Ap9/Spm/Legacy radiation belt model calculations.  The model
#   run parameters are specified via a collection of 'set' method calls.  The
#   actual model calculation run is performed 'behind the scenes'. The generated
#   calculation results are accessible (from binary data files) via a collection 
#   of 'get' and 'flyin' method calls.
#
# Classification:
#
#   Unclassified
#
# Project Name:
#
#   AE9/AP9/SPM Radiation Environment Models
#
#   Developed under US Government contract # FA9453-12-C-0231
#
# Rights and Restrictions:
#
#   Copyright 2018 Atmospheric and Environmental Research, Inc. (AER)
#
#   DISTRIBUTION A. Approved for public release; distribution is unlimited.
#
#   The AE9/AP9/SPM software license is contained in the 'documents/Licenses' 
#   folder of this distribution file collection.
#
# Author:
#
#   This software was developed by AER staff
#
# Contact:
#
#   Atmospheric and Environmental Research, Inc.
#   131 Hartwell Avenue
#   Lexington, MA 02421-3126 USA
#   Phone: 781.761.2288
#   email: spwx@aer.com
#
# References:
# 
#   None
#
# Revision history:
#
#  Version      Date        Notes
#  1.0          12/28/2017  Created 
#  
#******************************************************************************/

import os
import ctypes
import numpy as np
import numpy.ctypeslib as npct
import sys

array_1d_double = npct.ndpointer(dtype=np.double, ndim=1, flags='CONTIGUOUS')
array_2d_double = npct.ndpointer(dtype=np.double, ndim=2, flags='CONTIGUOUS')
array_3d_double = npct.ndpointer(dtype=np.double, ndim=3, flags='CONTIGUOUS')
array_1d_int    = npct.ndpointer(dtype=np.intc,   ndim=1, flags='CONTIGUOUS')

c_double_p = ctypes.POINTER(ctypes.c_double)
c_int_p = ctypes.POINTER( ctypes.c_int )

if 'IRENE_SYS' not in os.environ:
  sys.exit('Error: undefined IRENE environment variables - calling script requires "import irene_defs.py"')
  
class Application:

  def __init__( self, bMpi=False ):
    self.bMpi = bMpi
    if os.environ['IRENE_SYS'] == 'linux':
      if self.bMpi:
        self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libapplicationmpi.so')
      else:
        self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libapplication.so')
    else:
      if self.bMpi:
        self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libapplicationmpi_c.dll')
      else:
        self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libapplication_c.dll')
    self.zHandle = self.lib.AppStartUp()

  #/ directory in which temporary directory and files are written/read
  #/  when not specified, defaults to the current working directory
  def set_workDir( self, strWorkDir ):
    self.lib.AppSetWorkDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetWorkDir( self.zHandle, strWorkDir.encode() )

  def get_workDir( self ):
    self.lib.AppGetWorkDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strWorkDir = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetWorkDir( self.zHandle, strWorkDir )
    return strWorkDir.value.decode()

  #/ **Required** to be able to execute CmdLine 
  #/  and various Executor programs behind-the-scenes
  def set_execDir( self, strExecDir ):
    self.lib.AppSetExecDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetExecDir( self.zHandle, strExecDir.encode() )

  def get_execDir( self ):
    self.lib.AppGetExecDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strExecDir = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetExecDir( self.zHandle, strExecDir )
    return strExecDir.value.decode()

  def clear_inEphemeris( self ):
    self.lib.AppClearInEphemeris.argtypes = [ctypes.c_int]
    return self.lib.AppClearInEphemeris( self.zHandle )

  def set_inEphemeris( self, daTimes, 
                     daCoords1, daCoords2, daCoords3, iAppend=0 ):
    self.lib.AppSetInEphemeris.argtypes = [ctypes.c_int, array_1d_double, 
      array_1d_double, array_1d_double, array_1d_double, ctypes.c_int, ctypes.c_int]
    iNum = len( daTimes )
    if iNum < 1 or daTimes.shape != daCoords1.shape or daCoords1.shape != daCoords2.shape \
            or daCoords2.shape != daCoords3.shape:
        return -1
    return self.lib.AppSetInEphemeris( self.zHandle, daTimes, 
                     daCoords1, daCoords2, daCoords3, len(daTimes), iAppend )

  def get_numInEphemeris( self ):
    self.lib.AppGetNumInEphemeris.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumInEphemeris( self.zHandle )

  def get_inEphemeris( self ):
    self.lib.AppGetInEphemeris.argtypes = [ctypes.c_int, \
             array_1d_double, array_1d_double, array_1d_double, array_1d_double]
    iNum = self.get_numInEphemeris( )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1)
    daTimes = np.zeros( iNum )  # Default data type is numpy.float64.
    daCoords1 = np.zeros( iNum )
    daCoords2 = np.zeros( iNum )
    daCoords3 = np.zeros( iNum )
    iNum = self.lib.AppGetInEphemeris( self.zHandle, daTimes, 
                                       daCoords1, daCoords2, daCoords3 )
    return iNum, daTimes, daCoords1, daCoords2, daCoords3

  def reset_modelData( self ):
    self.lib.AppResetModelData.argtypes = [ctypes.c_int]
    return self.lib.AppResetModelData( self.zHandle )

  def reset_modelRun( self, iDelBinDir=1, iResetParam=0 ):
    self.lib.AppResetModelRun.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
    return self.lib.AppResetModelRun( self.zHandle, iDelBinDir, iResetParam )

  #/ Windows MPI mode for 'mpiexec' launcher extra arguments:
  #/     Local  0    add '-localonly'      for use on local Windows machine with multiple processors
  #/     SSH    1    add '-bootstrap ssh'  for use on Windows cluster, using ssh for MPI communication
  #/     Hydra  2    --no extra arg--      for use on Windows cluster, relies on external 'hydra_service' 
  def set_windowsMpiMode( self, strMode ):
    self.lib.AppSetWindowsMpiMode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetWindowsMpiMode( self.zHandle, strMode.encode() )

  def get_windowsMpiMode( self ):
    self.lib.AppGetWindowMpiMode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strMode = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetWindowsMpiMode( self.zHandle, strMode )
    return strMode.value.decode()

  #/ directory in the distribution data files are located
  def set_modelDBDir( self, strModelDBDir ):
    self.lib.AppSetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetModelDBDir( self.zHandle, strModelDBDir.encode() )

  def get_modelDBDir( self ):
    self.lib.AppGetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelDBDir = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetModelDBDir( self.zHandle, strModelDBDir )
    return strModelDBDir.value.decode()

  def run_model( self ):
    self.lib.AppRunModel.argtypes = [ctypes.c_int]
    return self.lib.AppRunModel( self.zHandle )

  def set_coordSys( self, strCoordSys, strCoordSysUnits ):
    self.lib.AppSetCoordSys.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p]
    return self.lib.AppSetCoordSys( self.zHandle, strCoordSys.encode(), 
                                           strCoordSysUnits.encode() )

  def set_inCoordSys( self, strCoordSys, strCoordSysUnits ):
    self.lib.AppSetInCoordSys.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p]
    return self.lib.AppSetInCoordSys( self.zHandle, strCoordSys.encode(), 
                                             strCoordSysUnits.encode() )

  def get_coordSys( self ):
    self.lib.AppGetCoordSys.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strCoordSys = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetCoordSys( self.zHandle, strCoordSys )
    return strCoordSys.value.decode()

  def get_coordSysUnits( self ):
    self.lib.AppGetCoordSysUnits.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strCoordSysUnits = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetCoordSysUnits( self.zHandle, strCoordSysUnits )
    return strCoordSysUnits.value.decode()

  def get_inCoordSys( self ):
    self.lib.AppGetInCoordSys.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strInCoordSys = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetInCoordSys( self.zHandle, strInCoordSys )
    return strInCoordSys.value.decode()

  def get_inCoordSysUnits( self ):
    self.lib.AppGetInCoordSysUnits.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strInCoordSysUnits = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetInCoordSysUnits( self.zHandle, strInCoordSysUnits )
    return strInCoordSysUnits.value.decode()

  # valid only *after* call to 'run_model'
  #  -will return 1 for omnidirectional, numPitchAngles for list of Pitch Angles
  def get_numDir( self ):
    self.lib.AppGetNumDir.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumDir( self.zHandle )

    #/ analogous to computeEphemeris; accesses data in chunks  
    # (returns iNumTimes value; max=chunk size)
  def get_ephemeris( self ):
    self.lib.AppGetEphemeris.argtypes = [ctypes.c_int, array_1d_double, array_1d_double, 
                                         array_1d_double, array_1d_double]
    iSize = self.get_chunkSize( )
    daTimes = np.zeros( iSize )  # Default data type is numpy.float64.
    daCoord1 = np.zeros( iSize )
    daCoord2 = np.zeros( iSize )
    daCoord3 = np.zeros( iSize )
    iNum = self.lib.AppGetEphemeris( self.zHandle, daTimes, daCoord1, daCoord2, daCoord3 )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1)
    if iNum < iSize:
      daTimes = np.resize( daTimes, iNum )
      daCoord1 = np.resize( daCoord1, iNum )
      daCoord2 = np.resize( daCoord2, iNum )
      daCoord3 = np.resize( daCoord3, iNum )
    return iNum, daTimes, daCoord1, daCoord2, daCoord3

  #replication of previous version flux calculation access methods: 
  # (all of these methods return iNumTimes value [max=chunk size, but can be zero]; 
  #   the other dimension sizes iNumEnergies and iNumDir will already be known)
  #these are currently configured to provide data in 'chunks' for each call;
  #  --> need to be able to have setting to change to reading entire file...
  # Optional 'bFluxAccumAvg' argument, when 'true' accesses the accumulation-dependent
  #    *flux* averages (Intv,Full,Boxcar,Expon) instead of the straight flux values

  # unidirectional; will also get Legacy results
  def flyin_mean2d( self, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppFlyinMean2D.argtypes = [ctypes.c_int,array_2d_double,
                                        ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    if iNumE < 1:
      return -1, np.zeros((1,1))
    da2FluxData = np.zeros( (iSize,iNumE) )  # Default data type is numpy.float64.
    iNum = self.lib.AppFlyinMean2D( self.zHandle, da2FluxData,
                                    strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros((1,1))
    if iNum < iSize:
      da2FluxData = np.resize( da2FluxData, (iNum,iNumE) )
    return iNum, da2FluxData

  def flyin_mean( self, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppFlyinMean.argtypes = [ctypes.c_int,array_3d_double,
                                      ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    iNumD = self.get_numDir( )
    if iNumE < 1 or iNumD < 1:
      return -1, np.zeros((1,1,1))
    da3FluxData = np.zeros( (iSize, iNumE, iNumD) ) #/ will also get Legacy results
    iNum = self.lib.AppFlyinMean( self.zHandle, da3FluxData,
                                  strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros((1,1,1))
    if iNum < iSize:
      da3FluxData = np.resize( da3FluxData, (iNum,iNumE,iNumD) )
    return iNum, da3FluxData

  #/ also get time, ephemeris and pitch angle data
  def flyin_meanPlus( self, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppFlyinMeanPlus.argtypes = [ctypes.c_int,array_1d_double,array_1d_double,
                       array_1d_double,array_1d_double,array_2d_double,array_3d_double,
                       ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    iNumD = self.get_numDir( )
    if iNumE < 1 or iNumD < 1:
      return -1, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                 np.zeros((1,1)), np.zeros((1,1,1))
    da2PitchAngles = np.zeros( (iSize, iNumD) )
    da3FluxData = np.zeros( (iSize, iNumE, iNumD) ) #/ will also get Legacy results
    daTimes = np.zeros( iSize )
    daCoord1 = np.zeros( iSize )
    daCoord2 = np.zeros( iSize )
    daCoord3 = np.zeros( iSize )
    ### if omnidir, da2PitchAngles will still be all zeroes
    iNum = self.lib.AppFlyinMeanPlus( self.zHandle,
                                      daTimes, daCoord1, daCoord2, daCoord3,
                                      da2PitchAngles, da3FluxData,
                                      strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                   np.zeros((1,1)), np.zeros((1,1,1))
    iNumP = self.get_numPitchAngles()
    if iNum < iSize:
      daTimes = np.resize( daTimes, iNum )
      daCoord1 = np.resize( daCoord1, iNum )
      daCoord2 = np.resize( daCoord2, iNum )
      daCoord3 = np.resize( daCoord3, iNum )
      da2PitchAngles = np.resize( da2PitchAngles, (iNum,iNumD) )
      da3FluxData = np.resize( da3FluxData, (iNum,iNumE,iNumD) )
    if (iNumD != iNumP):
      da2PitchAngles = np.zeros((1,1))
    return iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData

  def flyin_percentile( self, iPercentile, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppFlyinPercentile.argtypes = [ctypes.c_int,ctypes.c_int, 
                                            array_3d_double,ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    iNumD = self.get_numDir( )
    if iNumE < 1 or iNumD < 1:
      return -1, np.zeros( (1,1,1) )
    da3FluxData = np.zeros( (iSize, iNumE, iNumD) ) #/ will also get Legacy results
    iNum = self.lib.AppFlyinPercentile( self.zHandle, iPercentile, da3FluxData,
                                        strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros((1,1,1))
    if iNum < iSize:
      da3FluxData = np.resize( da3FluxData, (iNum,iNumE,iNumD) )
    return iNum, da3FluxData

  def flyin_percentilePlus( self, iPercentile, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppFlyinPercentilePlus.argtypes = [ctypes.c_int,ctypes.c_int,
                      array_1d_double,array_1d_double,array_1d_double,array_1d_double,
                      array_2d_double,array_3d_double,ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    iNumD = self.get_numDir( )
    if iNumE < 1 or iNumD < 1:
      return -1, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                 np.zeros((1,1)), np.zeros((1,1,1))
    da2PitchAngles = np.zeros( (iSize, iNumD) )
    da3FluxData = np.zeros( (iSize, iNumE, iNumD) ) #/ will also get Legacy results
    daTimes = np.zeros( iSize )
    daCoord1 = np.zeros( iSize )
    daCoord2 = np.zeros( iSize )
    daCoord3 = np.zeros( iSize )
    ### if omnidir, da2PitchAngles will still be all zeroes
    iNum = self.lib.AppFlyinPercentilePlus( self.zHandle, iPercentile,
                                            daTimes, daCoord1, daCoord2, daCoord3,
                                            da2PitchAngles, da3FluxData,
                                            strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                   np.zeros((1,1)), np.zeros((1,1,1))
    iNumP = self.get_numPitchAngles()
    if iNum < iSize:
      daTimes = np.resize( daTimes, iNum )
      daCoord1 = np.resize( daCoord1, iNum )
      daCoord2 = np.resize( daCoord2, iNum )
      daCoord3 = np.resize( daCoord3, iNum )
      da2PitchAngles = np.resize( da2PitchAngles, (iNum,iNumD) )
      da3FluxData = np.resize( da3FluxData, (iNum,iNumE,iNumD) )
    if (iNumD != iNumP):
      da2PitchAngles = np.zeros((1,1))
    return iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData

  def flyin_perturbedMean( self, iScenario, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppFlyinPerturbedMean.argtypes = [ctypes.c_int,ctypes.c_int,
                                           array_3d_double,ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    iNumD = self.get_numDir( )
    if iNumE < 1 or iNumD < 1:
      return -1, np.zeros( (1,1,1) )
    da3FluxData = np.zeros( (iSize, iNumE, iNumD) ) #/ will also get Legacy results
    iNum = self.lib.AppFlyinPerturbedMean( self.zHandle, iScenario, da3FluxData,
                                           strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros((1,1,1))
    if iNum < iSize:
      da3FluxData = np.resize( da3FluxData, (iNum,iNumE,iNumD) )
    return iNum, da3FluxData

  def flyin_perturbedMeanPlus( self, iScenario, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppFlyinPerturbedMeanPlus.argtypes = [ctypes.c_int,ctypes.c_int,
                        array_1d_double,array_1d_double,array_1d_double,array_1d_double,
                        array_2d_double,array_3d_double,ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    iNumD = self.get_numDir( )
    if iNumE < 1 or iNumD < 1:
      return -1, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                 np.zeros((1,1)), np.zeros((1,1,1))
    da2PitchAngles = np.zeros( (iSize, iNumD) )
    da3FluxData = np.zeros( (iSize, iNumE, iNumD) ) #/ will also get Legacy results
    daTimes = np.zeros( iSize )
    daCoord1 = np.zeros( iSize )
    daCoord2 = np.zeros( iSize )
    daCoord3 = np.zeros( iSize )
    ### if omnidir, da2PitchAngles will still be all zeroes
    iNum = self.lib.AppFlyinPerturbedMeanPlus( self.zHandle, iScenario, 
                                               daTimes, daCoord1, daCoord2, daCoord3, 
                                               da2PitchAngles, da3FluxData,
                                               strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                   np.zeros((1,1)), np.zeros((1,1,1))
    iNumP = self.get_numPitchAngles()
    if iNum < iSize:
      daTimes = np.resize( daTimes, iNum )
      daCoord1 = np.resize( daCoord1, iNum )
      daCoord2 = np.resize( daCoord2, iNum )
      daCoord3 = np.resize( daCoord3, iNum )
      da2PitchAngles = np.resize( da2PitchAngles, (iNum,iNumD) )
      da3FluxData = np.resize( da3FluxData, (iNum,iNumE,iNumD) )
    if (iNumD != iNumP):
      da2PitchAngles = np.zeros((1,1))
    return iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData

     #// replacement for previous version 'flyinScenario' method;
     #//  (additional arguments that it used are not relevant here)
  def flyin_monteCarlo( self, iScenario, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppFlyinMonteCarlo.argtypes = [ctypes.c_int,ctypes.c_int,array_3d_double,
                                            ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    iNumD = self.get_numDir( )
    if iNumE < 1 or iNumD < 1:
      return -1, np.zeros( (1,1,1) )
    da3FluxData = np.zeros( (iSize, iNumE, iNumD) ) #/ will also get Legacy results
    iNum = self.lib.AppFlyinMonteCarlo( self.zHandle, iScenario, da3FluxData,
                                        strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros((1,1,1))
    if iNum < iSize:
      da3FluxData = np.resize( da3FluxData, (iNum,iNumE,iNumD) )
    return iNum, da3FluxData

  def flyin_monteCarloPlus( self, iScenario, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppFlyinMonteCarloPlus.argtypes = [ctypes.c_int,ctypes.c_int,
                       array_1d_double,array_1d_double,array_1d_double,array_1d_double,
                       array_2d_double,array_3d_double,ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    iNumD = self.get_numDir( )
    if iNumE < 1 or iNumD < 1:
      return -1, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                 np.zeros((1,1)), np.zeros((1,1,1))
    da2PitchAngles = np.zeros( (iSize, iNumD) )
    da3FluxData = np.zeros( (iSize, iNumE, iNumD) ) #/ will also get Legacy results
    daTimes = np.zeros( iSize )
    daCoord1 = np.zeros( iSize )
    daCoord2 = np.zeros( iSize )
    daCoord3 = np.zeros( iSize )
    ### if omnidir, da2PitchAngles will still be all zeroes
    iNum = self.lib.AppFlyinMonteCarloPlus( self.zHandle, iScenario,
                                            daTimes, daCoord1, daCoord2, daCoord3,
                                            da2PitchAngles, da3FluxData,
                                            strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                   np.zeros((1,1)), np.zeros((1,1,1))
    iNumP = self.get_numPitchAngles()
    if iNum < iSize:
      daTimes = np.resize( daTimes, iNum )
      daCoord1 = np.resize( daCoord1, iNum )
      daCoord2 = np.resize( daCoord2, iNum )
      daCoord3 = np.resize( daCoord3, iNum )
      da2PitchAngles = np.resize( da2PitchAngles, (iNum,iNumD) )
      da3FluxData = np.resize( da3FluxData, (iNum,iNumE,iNumD) )
    if (iNumD != iNumP):
      da2PitchAngles = np.zeros((1,1))
    return iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData

  def get_adiabaticCoords( self ):
    self.lib.AppGetAdiabaticCoords.argtypes = [ctypes.c_int,array_2d_double,
      array_2d_double,array_2d_double,array_2d_double,array_2d_double,array_2d_double,
      array_1d_double,array_1d_double,array_1d_double]
    iSize = self.get_chunkSize( )
    iNumD = self.get_numDir( )
    if iNumD < 1:
      return iNumD, np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                    np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                    np.zeros(1), np.zeros(1), np.zeros(1)
    da2Alpha = np.zeros( (iSize, iNumD) )  # Default data type is numpy.float64.
    da2Lm = np.zeros( (iSize, iNumD) )
    da2K = np.zeros( (iSize, iNumD) )
    da2Phi = np.zeros( (iSize, iNumD) )
    da2Hmin = np.zeros( (iSize, iNumD) )
    da2Lstar = np.zeros( (iSize, iNumD) )
    daBmin = np.zeros( iSize )
    daBlocal = np.zeros( iSize )
    daMagLT = np.zeros( iSize )
    iNum = self.lib.AppGetAdiabaticCoords( self.zHandle, da2Alpha, da2Lm, da2K, da2Phi,
                                           da2Hmin, da2Lstar, daBmin, daBlocal, daMagLT )
    if iNum < 1:
      return iNum, np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                   np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                   np.zeros(1), np.zeros(1), np.zeros(1)
    if iNum < iSize:
      da2Alpha = np.resize( da2Alpha, (iNum,iNumD) )
      da2Lm = np.resize( da2Lm, (iNum,iNumD) )
      da2K = np.resize( da2K, (iNum,iNumD) )
      da2Phi = np.resize( da2Phi, (iNum,iNumD) )
      da2Hmin = np.resize( da2Hmin, (iNum,iNumD) )
      da2Lstar = np.resize( da2Lstar, (iNum,iNumD) )
      daBmin = np.resize( daBmin, iNum )
      daBlocal = np.resize( daBlocal, iNum )
      daMagLT = np.resize( daMagLT, iNum )
    return iNum, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
                 daBmin, daBlocal, daMagLT

  def get_adiabaticCoordsPlus( self ):
    self.lib.AppGetAdiabaticCoordsPlus.argtypes = [ctypes.c_int,array_1d_double,
      array_1d_double,array_1d_double,array_1d_double,array_2d_double,array_2d_double,
      array_2d_double,array_2d_double,array_2d_double,array_2d_double,array_2d_double,
      array_1d_double,array_1d_double,array_1d_double]
    iSize = self.get_chunkSize( )
    iNumD = self.get_numDir( )
    if iNumD < 1:
      return iNumD, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), np.zeros((1,1)), \
                    np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                    np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                    np.zeros(1), np.zeros(1), np.zeros(1)
    daTimes = np.zeros( iSize )
    daCoord1 = np.zeros( iSize )
    daCoord2 = np.zeros( iSize )
    daCoord3 = np.zeros( iSize )
    da2PitchAngles = np.zeros( (iSize, iNumD) )
    da2Alpha = np.zeros( (iSize, iNumD) )  # Default data type is numpy.float64.
    da2Lm = np.zeros( (iSize, iNumD) )
    da2K = np.zeros( (iSize, iNumD) )
    da2Phi = np.zeros( (iSize, iNumD) )
    da2Hmin = np.zeros( (iSize, iNumD) )
    da2Lstar = np.zeros( (iSize, iNumD) )
    daBmin = np.zeros( iSize )
    daBlocal = np.zeros( iSize )
    daMagLT = np.zeros( iSize )
    iNum = self.lib.AppGetAdiabaticCoordsPlus( self.zHandle,
                             daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles,
                             da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar,
                             daBmin, daBlocal, daMagLT )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), np.zeros((1,1)), \
                   np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                   np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                   np.zeros(1), np.zeros(1), np.zeros(1) 
    if iNum < iSize:
      daTimes = np.resize( daTimes, iNum )
      daCoord1 = np.resize( daCoord1, iNum )
      daCoord2 = np.resize( daCoord2, iNum )
      daCoord3 = np.resize( daCoord3, iNum )
      da2PitchAngles = np.resize( da2PitchAngles, (iNum,iNumD) )
      da2Alpha = np.resize( da2Alpha, (iNum,iNumD) )
      da2Lm = np.resize( da2Lm, (iNum,iNumD) )
      da2K = np.resize( da2K, (iNum,iNumD) )
      da2Phi = np.resize( da2Phi, (iNum,iNumD) )
      da2Hmin = np.resize( da2Hmin, (iNum,iNumD) )
      da2Lstar = np.resize( da2Lstar, (iNum,iNumD) )
      daBmin = np.resize( daBmin, iNum )
      daBlocal = np.resize( daBlocal, iNum )
      daMagLT = np.resize( daMagLT, iNum )
    return iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, \
                 da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
                 daBmin, daBlocal, daMagLT

  # strDataType  // "flux"|"fluence"|"doserate"|"doseaccum"
  # strFluxMode  // "mean"|"percent"|"perturbed"|"montecarlo"|"adiabat"(only w/ flux)
  # iCalcVal     //   -1  |  1-99   |  1-999    |  1-999     |  -2
  # strAccumMode // "default"|"cumul"|"full"|"interval"|"boxcar"|"expon"|("undefined"<-adiabat)
  # iAccumIntvId // 1 - N (N=get_numAccumModes()
  def get_modelData( self, strDataType, strFluxMode, iCalcVal=-1, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppGetModelData.argtypes = [ctypes.c_int,ctypes.c_char_p,ctypes.c_char_p,
      ctypes.c_int,array_1d_double,array_1d_double,array_1d_double,array_1d_double,
      array_2d_double,array_3d_double,ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumV = self.get_numFluxEnergies( )
    if strDataType == "doserate" or strDataType == "doseaccum":
      iNumV = self.get_numDoseDepths( )
    iNumD = self.get_numDir( )
    if iNumV < 1 or iNumD < 1:
      return -1, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                 np.zeros((1,1)), np.zeros((1,1,1))
    da2PitchAngles = np.zeros( (iSize, iNumD) )
    da3Data = np.zeros( (iSize, iNumV, iNumD) ) #/ will also get Legacy results
    daTimes = np.zeros( iSize )
    daCoord1 = np.zeros( iSize )
    daCoord2 = np.zeros( iSize )
    daCoord3 = np.zeros( iSize )
    ### if omnidir, da2PitchAngles will still be all zeroes
    iNum = self.lib.AppGetModelData( self.zHandle, strDataType.encode(),
                                     strFluxMode.encode(), iCalcVal,
                                     daTimes, daCoord1, daCoord2, daCoord3,
                                     da2PitchAngles, da3Data,
                                     strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                   np.zeros((1,1)), np.zeros((1,1,1))
    iNumP = self.get_numPitchAngles()
    if iNum < iSize:
      daTimes = np.resize( daTimes, iNum )
      daCoord1 = np.resize( daCoord1, iNum )
      daCoord2 = np.resize( daCoord2, iNum )
      daCoord3 = np.resize( daCoord3, iNum )
      da2PitchAngles = np.resize( da2PitchAngles, (iNum,iNumD) )
      da3Data = np.resize( da3Data, (iNum,iNumV,iNumD) )
    if (iNumD != iNumP):
      da2PitchAngles = np.zeros((1,1))
    return iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data

#--these are just convience functions for get_modelData
# (could be extended to DoseRate, DoseAccum, and Flux too)
# (Flux could also have another set of separate methods with strAccumMode+iAccumIntvId)
  def get_meanFluence( self ):
    iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data \
      = self.get_meanFluencePlus( )
    return iNum, da3Data

  def get_meanFluencePlus( self ):
    return self.get_modelData( 'fluence', 'mean', -1 )

  def get_percentileFluence( self, iPercent ):
    iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data \
      = self.get_percentileFluencePlus( iPercent )
    return iNum, da3Data

  def get_percentileFluencePlus( self, iPercent ):
    return self.get_modelData( 'fluence', 'percentile', iPercent )

  def get_perturbedMeanFluence( self, iScenario ):
    iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data \
      = self.get_perturbedMeanFluencePlus( iScenario )
    return iNum, da3Data

  def get_perturbedMeanFluencePlus( self, iScenario ):
    return self.get_modelData( 'fluence', 'perturbed', iScenario )

  def get_monteCarloFluence( self, iScenario ):
    iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data \
      = self.get_monteCarloFluencePlus( iScenario )
    return iNum, da3Data

  def get_monteCarloFluencePlus( self, iScenario ):
    return self.get_modelData( 'fluence', 'montecarlo', iScenario )
#-------

  # strDataType  // "flux"|"fluence"|"doserate"|"doseaccum"
  # strFluxMode  // "perturbed"|"montecarlo"
  # iPercent     //  0-100
  # strAccumMode // "default"|"cumul"|"full"|"interval"|"boxcar"|"expon"|("undefined"<-adiabat)
  # iAccumIntvId // 1 - N (N=get_numAccumModes()
  def get_aggregData( self, strDataType, strFluxMode, iPercent, strAccumMode="default", iAccumIntvId=1 ):
    self.lib.AppGetAggregData.argtypes = [ctypes.c_int,ctypes.c_char_p,ctypes.c_char_p,
      ctypes.c_int,array_1d_double,array_1d_double,array_1d_double,array_1d_double,
      array_2d_double,array_3d_double,ctypes.c_char_p,ctypes.c_int]
    iSize = self.get_chunkSize( )
    iNumE = self.get_numFluxEnergies( )
    if strDataType == "doserate" or strDataType == "doseaccum":
      iNumE = self.get_numDoseDepths( )
    iNumD = self.get_numDir( )
    if iNumE < 1 or iNumD < 1:
      return -1, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                 np.zeros((1,1)), np.zeros((1,1,1))
    da2PitchAngles = np.zeros( (iSize, iNumD) )
    da3Data = np.zeros( (iSize, iNumE, iNumD) ) #/ will also get Legacy results
    daTimes = np.zeros( iSize )
    daCoord1 = np.zeros( iSize )
    daCoord2 = np.zeros( iSize )
    daCoord3 = np.zeros( iSize )
    ### if omnidir, da2PitchAngles will still be all zeroes
    iNum = self.lib.AppGetAggregData( self.zHandle, strDataType.encode(),
                                      strFluxMode.encode(), iPercent,
                                      daTimes, daCoord1, daCoord2, daCoord3,
                                      da2PitchAngles, da3Data,
                                      strAccumMode.encode(), iAccumIntvId )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1), np.zeros(1), np.zeros(1), \
                   np.zeros((1,1)), np.zeros((1,1,1))
    iNumP = self.get_numPitchAngles()
    if iNum < iSize:
      daTimes = np.resize( daTimes, iNum )
      daCoord1 = np.resize( daCoord1, iNum )
      daCoord2 = np.resize( daCoord2, iNum )
      daCoord3 = np.resize( daCoord3, iNum )
      da2PitchAngles = np.resize( da2PitchAngles, (iNum,iNumD) )
      da3Data = np.resize( da3Data, (iNum,iNumE,iNumD) )
    if (iNumD != iNumP):
      da2PitchAngles = np.zeros((1,1))
    return iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data

  #  // setting for optionally retaining binary directory and its contents
  #  // by default, the directory is cleared and removed within 'resetModel'
  def set_delBinDir( self, iVerdict=1 ):
    self.lib.AppSetDelBinDir.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetDelBinDir( self.zHandle, iVerdict )

  def get_delBinDir( self ):
    self.lib.AppGetDelBinDir.argtypes = [ctypes.c_int]
    return self.lib.AppGetDelBinDir( self.zHandle )

  #//  ==== incorporate CInputParameters c wrappers here

  def reset_parameters( self ):
    self.lib.AppResetParameters.argtypes = [ctypes.c_int]
    return self.lib.AppResetParameters( self.zHandle )

  def reset_orbitParameters( self ):
    self.lib.AppResetOrbitParameters.argtypes = [ctypes.c_int]
    return self.lib.AppResetOrbitParameters( self.zHandle )

  def validate_parameters( self ):
    self.lib.AppValidateParameters.argtypes = [ctypes.c_int]
    return self.lib.AppValidateParameters( self.zHandle )

  def set_numProc( self, iNumProc ):
    self.lib.AppSetNumProc.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetNumProc( self.zHandle, iNumProc )

  def get_numProc( self ):
    self.lib.AppGetNumProc.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumProc( self.zHandle )

  def set_numFileIo( self, iNumIo ):
    self.lib.AppSetNumFileIo.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetNumFileIo( self.zHandle, iNumIo )

  def get_numFileIo( self ):
    self.lib.AppGetNumFileIo.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumFileIo( self.zHandle )

  def set_chunkSize( self, iChunkSize ):
    self.lib.AppSetChunkSize.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetChunkSize( self.zHandle, iChunkSize )

  def get_chunkSize( self ):
    self.lib.AppGetChunkSize.argtypes = [ctypes.c_int]
    return self.lib.AppGetChunkSize( self.zHandle )

  def set_taskDelay( self, iTaskDelay ):
    self.lib.AppSetTaskDelay.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetTaskDelay( self.zHandle, iTaskDelay )

  def get_taskDelay( self ):
    self.lib.AppGetTaskDelay.argtypes = [ctypes.c_int]
    return self.lib.AppGetTaskDelay( self.zHandle )

  def set_model( self, strModel):
    self.lib.AppSetModel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetModel( self.zHandle, strModel.encode() )

  def get_model( self ):
    self.lib.AppGetModel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModel = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetModel( self.zHandle, strModel )
    return strModel.value.decode()

  def set_modelDBFile( self, strModelDB ):
    self.lib.AppSetModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetModelDBFile( self.zHandle, strModelDB.encode() )

  def get_modelDBFile( self ):
    self.lib.AppGetModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelDB = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetModelDBFile( self.zHandle, strModelDB )
    return strModelDB.value.decode()

  def set_kPhiDBFile( self, strKPhiDB ):
    self.lib.AppSetKPhiDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetKPhiDBFile( self.zHandle, strKPhiDB.encode() )

  def get_kPhiDBFile( self ):
    self.lib.AppGetKPhiDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strKPhiDB = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetKPhiDBFile( self.zHandle, strKPhiDB )
    return strKPhiDB.value.decode()

  def set_kHMinDBFile( self, strKHMinDB ):
    self.lib.AppSetKHMinDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetKHMinDBFile( self.zHandle, strKHMinDB.encode() )

  def get_kHMinDBFile( self ):
    self.lib.AppGetKHMinDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strKHMinDB = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetKHMinDBFile( self.zHandle, strKHMinDB )
    return strKHMinDB.value.decode()

  def set_magfieldDBFile( self, strMagfieldDB ):
    self.lib.AppSetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetMagfieldDBFile( self.zHandle, strMagfieldDB.encode() )

  def get_magfieldDBFile( self ):
    self.lib.AppGetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strMagfieldDB = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetMagfieldDBFile( self.zHandle, strMagfieldDB )
    return strMagfieldDB.value.decode()

  def set_doseDBFile( self, strDoseDB ):
    self.lib.AppSetDoseModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetDoseModelDBFile( self.zHandle, strDoseDB.encode() )

  def get_doseDBFile( self ):
    self.lib.AppGetDoseModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strDoseDB = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetDoseModelDBFile( self.zHandle, strDoseDB )
    return strDoseDB.value.decode()

  def add_extraInfo( self, strExtraInfo ):
    self.lib.AppAddExtraInfo.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppAddExtraInfo( self.zHandle, strExtraInfo.encode() )

  def get_numExtraInfo( self ):
    self.lib.AppGetNumExtraInfo.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumExtraInfo( self.zHandle )

  def clear_extraInfo( self ):
    self.lib.AppClearExtraInfo.argtypes = [ctypes.c_int]
    return self.lib.AppClearExtraInfo( self.zHandle )

  def set_timesList( self, daTimesList ):
    self.lib.AppSetTimesList.argtypes = [ctypes.c_int, array_1d_double, ctypes.c_int]
    return self.lib.AppSetTimesList( self.zHandle, daTimesList.astype(np.float64), len(daTimesList) )

  def get_numTimesList( self ):
    self.lib.AppGetNumTimesList.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumTimesList( self.zHandle )

  def get_timesList( self ):
    self.lib.AppGetTimesList.argtypes = [ctypes.c_int, array_1d_double]
    iNumA = self.get_numTimesList( )
    if iNumA < 1:
      return 0, np.zeros(1)
    daTimesList = np.zeros( iNumA )
    iNum = self.lib.AppGetTimesList( self.zHandle, daTimesList )
    return iNum, daTimesList 
    
  def clear_timesList( self ):
    self.lib.AppClearTimesList.argtypes = [ctypes.c_int]
    return self.lib.AppClearTimesList( self.zHandle )

  def set_times( self, dStartTimeMjd, dEndTimeMjd, dTimeStepSec ):
    self.lib.AppSetTimes.argtypes = [ctypes.c_int,
                        ctypes.c_double, ctypes.c_double, ctypes.c_double]
    return self.lib.AppSetTimes( self.zHandle, dStartTimeMjd, dEndTimeMjd, 
                                 dTimeStepSec )

  def get_times( self ):
    self.lib.AppGetTimes.argtypes = [ctypes.c_int, c_double_p, c_double_p, c_double_p]
    pdStartTime = ctypes.c_double()
    pdEndTime = ctypes.c_double()
    pdTimeStepSec = ctypes.c_double()
    ierr = self.lib.AppGetTimes( self.zHandle, 
                                 ctypes.byref(pdStartTime), ctypes.byref(pdEndTime), 
                                 ctypes.byref(pdTimeStepSec) )
    return pdStartTime.value, pdEndTime.value, pdTimeStepSec.value

  def set_varTimes( self, dStartTimeMjd, dEndTimeMjd, 
                    dTimeMinStepSec, dTimeMaxStepSec=3600.0, dTimeRoundSec=5.0 ):
    self.lib.AppSetVarTimes.argtypes = [ctypes.c_int,
                        ctypes.c_double, ctypes.c_double, ctypes.c_double,
                        ctypes.c_double, ctypes.c_double]
    return self.lib.AppSetVarTimes( self.zHandle, dStartTimeMjd, dEndTimeMjd, 
                                    dTimeMinStepSec, dTimeMaxStepSec, dTimeRoundSec )

  def get_varTimes( self ):
    self.lib.AppGetVarTimes.argtypes = [ctypes.c_int, c_double_p, c_double_p, 
                                        c_double_p, c_double_p, c_double_p]
    pdStartTime = ctypes.c_double()
    pdEndTime = ctypes.c_double()
    pdTimeMinStepSec = ctypes.c_double()
    pdTimeMaxStepSec = ctypes.c_double()
    pdTimeRoundSec = ctypes.c_double()
    ierr = self.lib.AppGetVarTimes( self.zHandle, 
                                    ctypes.byref(pdStartTime),
                                    ctypes.byref(pdEndTime), 
                                    ctypes.byref(pdTimeMinStepSec),
                                    ctypes.byref(pdTimeMaxStepSec),
                                    ctypes.byref(pdTimeRoundSec) )
    return pdStartTime.value, pdEndTime.value, \
           pdTimeMinStepSec.value, pdTimeMaxStepSec.value, pdTimeRoundSec.value

  def set_prop( self, strProp ):
    self.lib.AppSetPropagator.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetPropagator( self.zHandle, strProp.encode() )

  def get_prop( self ):
    self.lib.AppGetPropagator.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strProp = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetPropagator( self.zHandle, strProp )
    return strProp.value.decode()

  def set_sgp4Param( self, strMode, strWgs ):
    self.lib.AppSetSGP4Param.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p]
    return self.lib.AppSetSGP4Param( self.zHandle, strMode.encode(), strWgs.encode() )

  def set_sgp4Mode( self, strMode ):
    self.lib.AppSetSGP4Mode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetSGP4Mode( self.zHandle, strMode.encode() )

  def get_sgp4Mode( self ):
    self.lib.AppGetSGP4Mode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strMode = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetSGP4Mode( self.zHandle, strMode )
    return strMode.value.decode()

  def set_sgp4Datum( self, strWgs ):
    self.lib.AppSetSGP4Datum.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetSGP4Datum( self.zHandle, strWgs.encode() )

  def get_sgp4Datum( self ):
    self.lib.AppGetSGP4Datum.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strDatum = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetSGP4Datum( self.zHandle, strDatum )
    return strDatum.value.decode()

  def set_keplerUseJ2( self, iVerdict=1 ):
    self.lib.AppSetKeplerUseJ2.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.AppSetKeplerUseJ2( self.zHandle, iVerdict )

  def get_keplerUseJ2( self ):
    self.lib.AppGetKeplerUseJ2.argtypes = [ctypes.c_int]
    return self.lib.AppGetKeplerUseJ2( self.zHandle )

  def set_tleFile( self, strTleFile ):
    self.lib.AppSetTLEFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetTLEFile( self.zHandle, strTleFile.encode() )

  def get_tleFile( self ):
    self.lib.AppGetTLEFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strFile = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetTLEFile( self.zHandle, strFile )
    return strFile.value.decode()

  def clear_tleFile( self ):
    self.lib.AppClearTLEFile.argtypes = [ctypes.c_int]
    return self.lib.AppClearTLEFile( self.zHandle )

  def set_elementTime( self, dElementTimeMjd ):
    self.lib.AppSetElementTime.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetElementTime( self.zHandle, dElementTimeMjd )

  def get_elementTime( self ):
    self.lib.AppGetElementTime.argtypes = [ctypes.c_int, c_double_p]
    pdElementTime = ctypes.c_double()
    ierr = self.lib.AppGetElementTime( self.zHandle, ctypes.byref(pdElementTime) )
    return pdElementTime.value

  def set_inclination( self, dInclination ):
    self.lib.AppSetInclination.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetInclination( self.zHandle, dInclination )

  def get_inclination( self ):
    self.lib.AppGetInclination.argtypes = [ctypes.c_int, c_double_p]
    pdInclination = ctypes.c_double()
    ierr = self.lib.AppGetInclination( self.zHandle, ctypes.byref(pdInclination) )
    return pdInclination.value

  def set_argOfPerigee( self, dArgPerigee ):
    self.lib.AppSetArgOfPerigee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetArgOfPerigee( self.zHandle, dArgPerigee )

  def get_argOfPerigee( self ):
    self.lib.AppGetArgOfPerigee.argtypes = [ctypes.c_int, c_double_p]
    pdArgPerigee = ctypes.c_double()
    ierr = self.lib.AppGetArgOfPerigee( self.zHandle, ctypes.byref(pdArgPerigee) )
    return pdArgPerigee.value

  def set_meanAnomaly( self, dMeanAnomaly ):
    self.lib.AppSetMeanAnomaly.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetMeanAnomaly( self.zHandle, dMeanAnomaly )

  def get_meanAnomaly( self ):
    self.lib.AppGetMeanAnomaly.argtypes = [ctypes.c_int, c_double_p]
    pdMeanAnomaly = ctypes.c_double()
    ierr = self.lib.AppGetMeanAnomaly( self.zHandle, ctypes.byref(pdMeanAnomaly) )
    return pdMeanAnomaly.value

  def set_eccentricity( self, dEccen ):
    self.lib.AppSetEccentricity.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetEccentricity( self.zHandle, dEccen )

  def get_eccentricity( self ):
    self.lib.AppGetEccentricity.argtypes = [ctypes.c_int, c_double_p]
    pdEccen = ctypes.c_double()
    ierr = self.lib.AppGetEccentricity( self.zHandle, ctypes.byref(pdEccen) )
    return pdEccen.value

  def set_rightAscension( self, dRightAscen ):
    self.lib.AppSetRightAscension.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetRightAscension ( self.zHandle, dRightAscen )

  def get_rightAscension( self ):
    self.lib.AppGetRightAscension.argtypes = [ctypes.c_int, c_double_p]
    pdRightAscen = ctypes.c_double()
    ierr = self.lib.AppGetRightAscension( self.zHandle, ctypes.byref(pdRightAscen) )
    return pdRightAscen.value

  def set_meanMotion( self, dMeanMotion ):
    self.lib.AppSetMeanMotion.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetMeanMotion( self.zHandle, dMeanMotion )

  def get_meanMotion( self ):
    self.lib.AppGetMeanMotion.argtypes = [ctypes.c_int, c_double_p]
    pdMeanMotion = ctypes.c_double()
    ierr = self.lib.AppGetMeanMotion( self.zHandle, ctypes.byref(pdMeanMotion) )
    return pdMeanMotion.value

  def set_meanMotion1stDeriv( self, dMm1stDer ):
    self.lib.AppSetMeanMotion1stDeriv.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetMeanMotion1stDeriv( self.zHandle, dMm1stDer )

  def get_meanMotion1stDeriv( self ):
    self.lib.AppGetMeanMotion1stDeriv.argtypes = [ctypes.c_int, c_double_p]
    pdMm1stDer = ctypes.c_double()
    ierr = self.lib.AppGetMeanMotion1stDeriv( self.zHandle, ctypes.byref(pdMm1stDer) )
    return pdMm1stDer.value

  def set_meanMotion2ndDeriv( self, dMm2ndDer ):
    self.lib.AppSetMeanMotion2ndDeriv.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetMeanMotion2ndDeriv( self.zHandle,dMm2ndDer )

  def get_meanMotion2ndDeriv( self ):
    self.lib.AppGetMeanMotion2ndDeriv.argtypes = [ctypes.c_int, c_double_p]
    pdMm2ndDer = ctypes.c_double()
    ierr = self.lib.AppGetMeanMotion2ndDeriv( self.zHandle, ctypes.byref(pdMm2ndDer) )
    return pdMm2ndDer.value

  def set_bStar( self, dBStar ):
    self.lib.AppSetBStar.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetBStar( self.zHandle, dBStar )

  def get_bStar( self ):
    self.lib.AppGetBStar.argtypes = [ctypes.c_int, c_double_p]
    pdBStar = ctypes.c_double()
    ierr = self.lib.AppGetBStar( self.zHandle, ctypes.byref(pdBStar) )
    return pdBStar.value

  def set_altitudeOfApogee( self, dApogeeAlt ):
    self.lib.AppSetAltitudeOfApogee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetAltitudeOfApogee( self.zHandle, dApogeeAlt )

  def get_altitudeOfApogee( self ):
    self.lib.AppGetAltitudeOfApogee.argtypes = [ctypes.c_int, c_double_p]
    pdApogeeAlt = ctypes.c_double()
    ierr = self.lib.AppGetAltitudeOfApogee( self.zHandle, ctypes.byref(pdApogeeAlt) )
    return pdApogeeAlt.value

  def set_altitudeOfPerigee( self, dPerigeeAlt ):
    self.lib.AppSetAltitudeOfPerigee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetAltitudeOfPerigee( self.zHandle, dPerigeeAlt )

  def get_altitudeOfPerigee( self ):
    self.lib.AppGetAltitudeOfPerigee.argtypes = [ctypes.c_int, c_double_p]
    pdPerigeeAlt = ctypes.c_double()
    ierr = self.lib.AppGetAltitudeOfPerigee( self.zHandle, ctypes.byref(pdPerigeeAlt) )
    return pdPerigeeAlt.value

  def set_localTimeOfApogee( self, dLtApogeeHr ):
    self.lib.AppSetLocalTimeOfApogee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetLocalTimeOfApogee( self.zHandle, dLtApogeeHr )

  def get_localTimeOfApogee( self ):
    self.lib.AppGetLocalTimeOfApogee.argtypes = [ctypes.c_int, c_double_p]
    pdLtApogeeHr = ctypes.c_double()
    ierr = self.lib.AppGetLocalTimeOfApogee( self.zHandle, ctypes.byref(pdLtApogeeHr) )
    return pdLtApogeeHr.value

  def set_localTimeMaxInclination( self, dLtMaxInclHr ):
    self.lib.AppSetLocalTimeMaxInclination.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetLocalTimeMaxInclination( self.zHandle, dLtMaxInclHr )

  def get_localTimeMaxInclination( self ):
    self.lib.AppGetLocalTimeMaxInclination.argtypes = [ctypes.c_int, c_double_p]
    pdLtMaxInclHr = ctypes.c_double()
    ierr = self.lib.AppGetLocalTimeMaxInclination( self.zHandle, ctypes.byref(pdLtMaxInclHr) )
    return pdLtMaxInclHr.value

  def set_timeOfPerigee( self, dTimeOfPerigeeMjd ):
    self.lib.AppSetTimeOfPerigee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetTimeOfPerigee( self.zHandle, dTimeOfPerigeeMjd )

  def get_timeOfPerigee( self ):
    self.lib.AppGetTimeOfPerigee.argtypes = [ctypes.c_int, c_double_p]
    pdTimeOfPerigee = ctypes.c_double()
    ierr = self.lib.AppGetTimeOfPerigee( self.zHandle, ctypes.byref(pdTimeOfPerigee) )
    return pdTimeOfPerigee.value

  def set_semiMajorAxis( self, dSemiMajorAxis ):
    self.lib.AppSetSemiMajorAxis.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetSemiMajorAxis( self.zHandle, dSemiMajorAxis )

  def get_semiMajorAxis( self ):
    self.lib.AppGetSemiMajorAxis.argtypes = [ctypes.c_int, c_double_p]
    pdSemiMajorAxis = ctypes.c_double()
    ierr = self.lib.AppGetSemiMajorAxis( self.zHandle, ctypes.byref(pdSemiMajorAxis) )
    return pdSemiMajorAxis.value

  def set_positionGei( self, dGeiXpos, dGeiYpos, dGeiZpos ):
    self.lib.AppSetPositionGEI.argtypes = [ctypes.c_int, 
                              ctypes.c_double, ctypes.c_double, ctypes.c_double]
    return self.lib.AppSetPositionGEI( self.zHandle, dGeiXpos, dGeiYpos, dGeiZpos )
  def get_positionGei( self ):
    self.lib.AppGetPositionGEI.argtypes = [ctypes.c_int, c_double_p, c_double_p, c_double_p]
    pdGeiXpos = ctypes.c_double()
    pdGeiYpos = ctypes.c_double()
    pdGeiZpos = ctypes.c_double()
    ierr = self.lib.AppGetPositionGEI( self.zHandle, 
      ctypes.byref(pdGeiXpos), ctypes.byref(pdGeiYpos), ctypes.byref(pdGeiZpos) )
    return pdGeiXpos.value, pdGeiYpos.value, pdGeiZpos.value

  def set_velocityGei( self, dGeiXvel, dGeiYvel, dGeiZvel ):
    self.lib.AppSetVelocityGEI.argtypes = [ctypes.c_int, 
                              ctypes.c_double, ctypes.c_double, ctypes.c_double]
    return self.lib.AppSetVelocityGEI( self.zHandle, dGeiXvel, dGeiYvel, dGeiZvel )
  def get_velocityGei( self ):
    self.lib.AppGetVelocityGEI.argtypes = [ctypes.c_int, c_double_p, c_double_p, c_double_p]
    pdGeiXvel = ctypes.c_double()
    pdGeiYvel = ctypes.c_double()
    pdGeiZvel = ctypes.c_double()
    ierr = self.lib.AppGetVelocityGEI( self.zHandle, 
      ctypes.byref(pdGeiXvel), ctypes.byref(pdGeiYvel), ctypes.byref(pdGeiZvel) )
    return pdGeiXvel.value, pdGeiYvel.value, pdGeiZvel.value

  def set_stateVectors( self, daGeiPos, daGeiVel ):
    self.lib.AppSetStateVectors.argtypes = [ctypes.c_int, array_1d_double, array_1d_double]
    return self.lib.AppSetStateVectors( self.zHandle, daGeiPos, daGeiVel )
  def get_stateVectors( self ):
    self.lib.AppGetStateVectors.argtypes = [ctypes.c_int, array_1d_double, array_1d_double]
    daStatePos = np.zeros( 3 )
    daStateVel = np.zeros( 3 )
    self.lib.AppGetStateVectors( self.zHandle, daStatePos, daStateVel )
    return daStatePos, daStateVel 

  def set_geosynchLon( self, dGeosynchLon ):
    self.lib.AppSetGeosynchLon.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetGeosynchLon( self.zHandle, dGeosynchLon )

  def get_geosynchLon( self ):
    self.lib.AppGetGeosynchLon.argtypes = [ctypes.c_int, c_double_p]
    pdGeosynchLon = ctypes.c_double()
    ierr = self.lib.AppGetGeosynchLon( self.zHandle, ctypes.byref(pdGeosynchLon) )
    return pdGeosynchLon.value

  def set_pitchAngle( self, dPitchAngle ):
    self.lib.AppSetPitchAngle.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetPitchAngle( self.zHandle, dPitchAngle )

  def set_pitchAngles( self, daPitchAngles ):
    self.lib.AppSetPitchAngles.argtypes = [ctypes.c_int, array_1d_double, ctypes.c_int]
    return self.lib.AppSetPitchAngles( self.zHandle, daPitchAngles.astype(np.float64), len(daPitchAngles) )

  def get_numPitchAngles( self ):
    self.lib.AppGetNumPitchAngles.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumPitchAngles( self.zHandle )

  def get_pitchAngles( self ):
    self.lib.AppGetPitchAngles.argtypes = [ctypes.c_int, array_1d_double]
    iNumA = self.get_numPitchAngles( )
    if iNumA < 1:
      return 0, np.zeros(1)
    daPitchAngles = np.zeros( iNumA )
    iNum = self.lib.AppGetPitchAngles( self.zHandle, daPitchAngles )
    return iNum, daPitchAngles 
    
  def clear_pitchAngles( self ):
    self.lib.AppClearPitchAngles.argtypes = [ctypes.c_int]
    return self.lib.AppClearPitchAngles( self.zHandle )

  def set_adiabatic( self, iVerdict=1 ):
    self.lib.AppSetAdiabatic.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetAdiabatic( self.zHandle, iVerdict )

  def get_adiabatic( self ):
    self.lib.AppGetAdiabatic.argtypes = [ctypes.c_int]
    return self.lib.AppGetAdiabatic( self.zHandle )

  def set_fluxType( self, strFluxType ):
    self.lib.AppSetFluxType.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetFluxType( self.zHandle, strFluxType.encode() )

  def get_fluxType( self ):
    self.lib.AppGetFluxType.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strFluxType = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetFluxType( self.zHandle, strFluxType )
    return strFluxType.value.decode()

  def clear_fluxEnergies( self ):
    self.lib.AppClearFluxEnergies.argtypes = [ctypes.c_int]
    return self.lib.AppClearFluxEnergies( self.zHandle)

  def set_fluxEnergy( self, dEnergy ):
    self.lib.AppSetFluxEnergy.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetFluxEnergy( self.zHandle, dEnergy )

  def set_fluxEnergies( self, daEnergies ):
    self.lib.AppSetFluxEnergies.argtypes = [ctypes.c_int, array_1d_double, ctypes.c_int]
    return self.lib.AppSetFluxEnergies( self.zHandle, daEnergies.astype(np.float64), len(daEnergies))

  def get_numFluxEnergies( self ):
    self.lib.AppGetNumFluxEnergies.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumFluxEnergies( self.zHandle )

  def get_fluxEnergies( self ):
    self.lib.AppGetFluxEnergies.argtypes = [ctypes.c_int, array_1d_double]
    iNumE = self.get_numFluxEnergies( )
    if iNumE < 1:
      return iNumE, np.zeros(1)
    daEnergies = np.zeros( iNumE )  # Default data type is numpy.float64.
    iNum = self.lib.AppGetFluxEnergies( self.zHandle, daEnergies )
    return iNum, daEnergies

  def clear_fluxEnergies2( self ):
    self.lib.AppClearFluxEnergies2.argtypes = [ctypes.c_int]
    return self.lib.AppClearFluxEnergies2( self.zHandle )

  def set_fluxEnergy2( self, dEnergy ):
    self.lib.AppSetFluxEnergy2.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetFluxEnergy2( self.zHandle, dEnergy )

  def set_fluxEnergies2( self, daEnergies ):
    self.lib.AppSetFluxEnergies2.argtypes = [ctypes.c_int, array_1d_double, ctypes.c_int]
    return self.lib.AppSetFluxEnergies2( self.zHandle, daEnergies.astype(np.float64), len(daEnergies))

  def get_numFluxEnergies2( self ):
    self.lib.AppGetNumFluxEnergies2.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumFluxEnergies2( self.zHandle )

  def get_fluxEnergies2( self ):
    self.lib.AppGetFluxEnergies2.argtypes = [ctypes.c_int, array_1d_double]
    iNumE =  self.get_numFluxEnergies2( )
    if iNumE < 1:
      return iNumE, np.zeros(1)
    daEnergies = np.zeros( iNumE )  # Default data type is numpy.float64.
    iNum = self.lib.AppGetFluxEnergies2( self.zHandle, daEnergies )
    return iNum, daEnergies

  def set_fluxMean( self, iVerdict=1 ):
    self.lib.AppSetFluxMean.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetFluxMean( self.zHandle, iVerdict )

  def get_fluxMean( self ):
    self.lib.AppGetFluxMean.argtypes = [ctypes.c_int]
    return self.lib.AppGetFluxMean( self.zHandle )

  def set_fluxPercentile( self, iPercentile ):
    self.lib.AppSetFluxPercentile.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetFluxPercentile( self.zHandle, iPercentile )

  def set_fluxPercentiles( self, iaPercentiles ):
    self.lib.AppSetFluxPercentiles.argtypes = [ctypes.c_int, array_1d_int, ctypes.c_int]
    return self.lib.AppSetFluxPercentiles( self.zHandle, iaPercentiles.astype(np.int32), len(iaPercentiles))

  def get_numFluxPercentiles( self ):
    self.lib.AppGetNumFluxPercentiles.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumFluxPercentiles( self.zHandle )

  def get_fluxPercentiles( self ):
    self.lib.AppGetFluxPercentiles.argtypes = [ctypes.c_int, array_1d_int]
    iNumP = self.get_numFluxPercentiles( )
    if iNumP < 1:
      return iNumP, np.zeros(1, dtype=np.intc)
    iaPercentiles = np.zeros( iNumP, dtype=np.intc )
    iNum = self.lib.AppGetFluxPercentiles( self.zHandle, iaPercentiles )
    return iNum, iaPercentiles

  def clear_fluxPercentiles( self ):
    self.lib.AppClearFluxPercentiles.argtypes = [ctypes.c_int]
    return self.lib.AppClearFluxPercentiles( self.zHandle )

  def set_fluxPerturbedScenario( self, iScenario ):
    self.lib.AppSetFluxPerturbedScenario.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetFluxPerturbedScenario( self.zHandle, iScenario )

  def set_fluxPerturbedScenarios( self, iaScenarios ):
    self.lib.AppSetFluxPerturbedScenarios.argtypes = [ctypes.c_int,
                                              array_1d_int, ctypes.c_int]
    return self.lib.AppSetFluxPerturbedScenarios( self.zHandle, iaScenarios.astype(np.int32), len(iaScenarios))

  def set_fluxPerturbedScenRange( self, iScenMin, iScenMax ):
    self.lib.AppSetFluxPerturbedScenarios.argtypes = [ctypes.c_int,
                                              ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetFluxPerturbedScenRange( self.zHandle, iScenMin, iScenMax )

  def get_numFluxPerturbedScenarios( self ):
    self.lib.AppGetNumFluxPerturbedScenarios.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumFluxPerturbedScenarios( self.zHandle )

  def get_fluxPerturbedScenarios( self ):
    self.lib.AppGetFluxPerturbedScenarios.argtypes = [ctypes.c_int, array_1d_int]
    iNumS = self.get_numFluxPerturbedScenarios( )
    if iNumS < 1:
      return iNumS, np.zeros(1, dtype=np.intc)
    iaScenarios = np.zeros( iNumS, dtype=np.intc )
    iNum = self.lib.AppGetFluxPerturbedScenarios( self.zHandle, iaScenarios )
    return iNum, iaScenarios

  def clear_fluxPerturbedScenarios( self ):
    self.lib.AppClearFluxPerturbedScenarios.argtypes = [ctypes.c_int]
    return self.lib.AppClearFluxPerturbedScenarios( self.zHandle )

  def set_fluxMonteCarloScenario( self, iScenario ):
    self.lib.AppSetFluxMonteCarloScenario.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetFluxMonteCarloScenario( self.zHandle, iScenario )

  def set_fluxMonteCarloScenarios( self, iaScenarios ):
    self.lib.AppSetFluxMonteCarloScenarios.argtypes = [ctypes.c_int,
                                               array_1d_int, ctypes.c_int]
    return self.lib.AppSetFluxMonteCarloScenarios( self.zHandle, iaScenarios.astype(np.int32), len(iaScenarios))

  def set_fluxMonteCarloScenRange( self, iScenMin, iScenMax ):
    self.lib.AppSetFluxMonteCarloScenarios.argtypes = [ctypes.c_int,
                                              ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetFluxMonteCarloScenRange( self.zHandle, iScenMin, iScenMax )

  def get_numFluxMonteCarloSenarios( self ):
    self.lib.AppGetNumFluxMonteCarloScenarios.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumFluxMonteCarloScenarios( self.zHandle )

  def get_fluxMonteCarloScenarios( self ):
    self.lib.AppGetFluxMonteCarloScenarios.argtypes = [ctypes.c_int, array_1d_int]
    iNumS = self.get_numFluxMonteCarloSenarios( )
    if iNumS < 1:
      return iNumS, np.zeros(1, dtype=np.intc)
    iaScenarios = np.zeros( iNumS, dtype=np.intc )
    iNum = self.lib.AppGetFluxMonteCarloScenarios( self.zHandle, iaScenarios )
    return iNum, iaScenarios

  def clear_fluxMonteCarloScenarios( self ):
    self.lib.AppClearFluxMonteCarloScenarios.argtypes = [ctypes.c_int]
    return self.lib.AppClearFluxMonteCarloScenarios( self.zHandle )

  def set_monteCarloEpochTime( self, dEpochTime ):
    self.lib.AppSetMonteCarloEpochTime.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetMonteCarloEpochTime( self.zHandle, dEpochTime )

  def get_monteCarloEpochTime( self ):
    self.lib.AppGetMonteCarloEpochTime.argtypes = [ctypes.c_int, c_double_p]
    pdEpochTime = ctypes.c_double()
    ierr = self.lib.AppGetMonteCarloEpochTime( self.zHandle, ctypes.byref(pdEpochTime) )
    return pdEpochTime.value

  def set_monteCarloPerturb( self, iVerdict=1 ):
    self.lib.AppSetMonteCarloFluxPerturb.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetMonteCarloFluxPerturb( self.zHandle, iVerdict )

  def get_monteCarloPerturb( self ):
    self.lib.AppGetMonteCarloFluxPerturb.argtypes = [ctypes.c_int]
    return self.lib.AppGetMonteCarloFluxPerturb( self.zHandle )

  def set_monteCarloWorstCase( self, iVerdict=1 ):
    self.lib.AppSetMonteCarloWorstCase.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetMonteCarloWorstCase( self.zHandle, iVerdict )

  def get_monteCarloWorstCase( self ):
    self.lib.AppGetMonteCarloWorstCase.argtypes = [ctypes.c_int]
    return self.lib.AppGetMonteCarloWorstCase( self.zHandle )

  def set_reportTimes( self, dTimeRef, dDeltaDays ):
    self.lib.AppSetReportTimes.argtypes = [ctypes.c_int, 
                              ctypes.c_double, ctypes.c_double]
    return self.lib.AppSetReportTimes( self.zHandle, dTimeRef, dDeltaDays )
  def set_reportTimesSec( self, dTimeRef, dDeltaSec ):
    self.lib.AppSetReportTimesSec.argtypes = [ctypes.c_int, 
                              ctypes.c_double, ctypes.c_double]
    return self.lib.AppSetReportTimesSec( self.zHandle, dTimeRef, dDeltaSec )
  def get_numReportTimes( self ):
    self.lib.AppGetNumReportTimes.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumReportTimes( self.zHandle )
  def get_reportTimesSec( self ):
    self.lib.AppGetReportTimesSec.argtypes = [ctypes.c_int, array_1d_double, array_1d_double]
    iNumT = self.get_numReportTimes( )
    if iNumT < 1:
      return iNumT, np.zeros(1), np.zeros(1)
    daTimeRef = np.zeros( iNumT ) 
    daDeltaSec = np.zeros( iNumT )
    iNum = self.lib.AppGetReportTimesSec( self.zHandle, daTimeRef, daDeltaSec )
    return iNum, daTimeRef, daDeltaSec
  def clear_reportTimes( self ):
    self.lib.AppClearReportTimes.argtypes = [ctypes.c_int]
    return self.lib.AppClearReportTimes( self.zHandle )

  def set_reportAtTime( self, dAtTime ):
    self.lib.AppSetReportAtTime.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetReportAtTime( self.zHandle, dAtTime )
  def get_numReportAtTime( self ):
    self.lib.AppGetNumReportAtTime.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumReportAtTime( self.zHandle )
  def get_reportAtTime( self ):
    self.lib.AppGetReportAtTime.argtypes = [ctypes.c_int, array_1d_double]
    iNumT = self.get_numReportAtTime( )
    if iNumT < 1:
      return iNumT, np.zeros(1)
    daAtTime = np.zeros( iNumT ) 
    iNum = self.lib.AppGetReportAtTime( self.zHandle, daAtTime )
    return iNum, daAtTime
  def clear_reportAtTime( self ):
    self.lib.AppClearReportAtTime.argtypes = [ctypes.c_int]
    return self.lib.AppClearReportAtTime( self.zHandle )

  def set_fluence( self, iVerdict=1 ):
    self.lib.AppSetFluence.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetFluence( self.zHandle, iVerdict )

  def get_fluence( self ):
    self.lib.AppGetFluence.argtypes = [ctypes.c_int]
    return self.lib.AppGetFluence( self.zHandle )

  def set_doseRate( self, iVerdict=1 ):
    self.lib.AppSetDoseRate.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetDoseRate( self.zHandle, iVerdict )

  def get_doseRate( self ):
    self.lib.AppGetDoseRate.argtypes = [ctypes.c_int]
    return self.lib.AppGetDoseRate( self.zHandle )

  def set_doseAccum( self, iVerdict=1 ):
    self.lib.AppSetDoseAccum.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetDoseAccum( self.zHandle, iVerdict )

  def get_doseAccum( self ):
    self.lib.AppGetDoseAccum.argtypes = [ctypes.c_int]
    return self.lib.AppGetDoseAccum( self.zHandle )

  def set_doseDepthUnits( self, strDepthUnits ):
    self.lib.AppSetDoseDepthUnits.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetDoseDepthUnits( self.zHandle, strDepthUnits.encode() )

  def get_doseDepthUnits( self ):
    self.lib.AppGetDoseDepthUnits.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strDepthUnits = ctypes.create_string_buffer( 16 );
    ierr = self.lib.AppGetDoseDepthUnits( self.zHandle, strDepthUnits )
    return strDepthUnits.value.decode()

  def set_doseDepthValues( self, daDepths ):
    self.lib.AppSetDoseDepthValues.argtypes = [ctypes.c_int,
                                           array_1d_double, ctypes.c_int]
    return self.lib.AppSetDoseDepthValues( self.zHandle, daDepths.astype(np.float64), len(daDepths) )

  def get_numDoseDepths( self ):
    self.lib.AppGetNumDoseDepthValues.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumDoseDepthValues( self.zHandle )

  def get_doseDepthValues( self ):
    self.lib.AppGetDoseDepthValues.argtypes = [ctypes.c_int, array_1d_double]
    iNumD = self.get_numDoseDepths( )
    if iNumD < 1:
      return iNumD, np.zeros(1)
    daDepths = np.zeros( iNumD )  # Default data type is numpy.float64.
    iNum = self.lib.AppGetDoseDepthValues( self.zHandle, daDepths )
    return iNum, daDepths

  def set_doseDepths( self, daDepths, strDepthUnits ):
    self.lib.AppSetDoseDepths.argtypes = [ctypes.c_int,
                                      array_1d_double, ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetDoseDepthValues( self.zHandle, daDepths.astype(np.float64), len(daDepths), 
                                         strDepthUnits.encode() )

  def set_doseDetector( self, strDetector ):
    self.lib.AppSetDoseDetector.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetDoseDetector( self.zHandle, strDetector.encode() )

  def get_doseDetector( self ):
    self.lib.AppGetDoseDetector.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strDetector = ctypes.create_string_buffer( 32 );
    ierr = self.lib.AppGetDoseDetector( self.zHandle, strDetector )
    return strDetector.value.decode()

  def set_doseGeometry( self, strGeometry ):
    self.lib.AppSetDoseGeometry.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetDoseGeometry( self.zHandle, strGeometry.encode() )

  def get_doseGeometry( self ):
    self.lib.AppGetDoseGeometry.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strGeometry = ctypes.create_string_buffer( 32 );
    ierr = self.lib.AppGetDoseGeometry( self.zHandle, strGeometry )
    return strGeometry.value.decode()

  def set_doseNuclearAttenMode( self, strNuclearAttenMode ):
    self.lib.AppSetDoseNuclearAttenMode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetDoseNuclearAttenMode( self.zHandle, strNuclearAttenMode.encode() )

  def get_doseNuclearAttenMode( self ):
    self.lib.AppGetDoseNuclearAttenMode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strNuclearAttenMode = ctypes.create_string_buffer( 32 );
    ierr = self.lib.AppGetDoseDoseNuclearAttenMode( self.zHandle, strNuclearAttenMode )
    return strNuclearAttenMode.value.decode()

  def set_doseWithBrems( self, iVerdict=1 ):
    self.lib.AppSetDoseWithBrems.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.AppSetDoseWithBrems( self.zHandle, iVerdict )

  def get_doseWithBrems( self ):
    self.lib.AppGetDoseWithBrems.argtypes = [ctypes.c_int]
    return self.lib.AppGetDoseWithBrems( self.zHandle )

  def set_useDoseKernel( self, iVerdict=1 ):
    self.lib.AppSetUseDoseKernel.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.AppSetUseDoseKernel( self.zHandle, iVerdict )

  def get_useDoseKernel( self ):
    self.lib.AppGetUseDoseKernel.argtypes = [ctypes.c_int]
    return self.lib.AppGetUseDoseKernel( self.zHandle )

  def set_doseKernelDir( self, strDoseKernelDir ):
    self.lib.AppSetDoseKernelDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetDoseKernelDir( self.zHandle, strDoseKernelDir.encode() )

  def get_doseKernelDir( self ):
    self.lib.AppGetDoseKernelDir.ctypes = [ctypes.c_int, ctypes.c_char_p]
    strDoseKernelDir = ctypes.create_string_buffer( 256 );
    ierr = self.lib.AppGetDoseKernelDir( self.zHandle, strDoseKernelDir )
    return strDoseKernelDir.value.decode()

  def set_doseKernelFile( self, strDoseKernelFile ):
    self.lib.AppSetDoseKernelFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetDoseKernelFile( self.zHandle, strDoseKernelFile.encode() )

  def get_doseKernelFile( self ):
    self.lib.AppGetDoseKernelFile.ctypes = [ctypes.c_int, ctypes.c_char_p]
    strDoseKernelFile = ctypes.create_string_buffer( 256 );
    ierr = self.lib.AppGetDoseKernelFile( self.zHandle, strDoseKernelFile )
    return strDoseKernelFile.value.decode()

  def set_accumMode( self, strAccumMode ):
    self.lib.AppSetAccumMode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetAccumMode( self.zHandle, strAccumMode.encode() )

  def clear_accumModes( self ):
    self.lib.AppClearAccumModes.ctypes = [ctypes.c_int]
    return self.lib.AppClearAccumMode( self.zHandle )

  def get_numAccumModes( self ):
    self.lib.AppGetNumAccumModes.ctypes = [ctypes.c_int]
    return self.lib.AppGetNumAccumModes( self.zHandle )

  def get_accumMode( self ):
    return self.get_accumModeEntry( 1 )

  def get_accumModeEntry( self, iIdent ):
    self.lib.AppGetAccumMode.ctypes = [ctypes.c_int, ctypes.c_int, ctypes.c_char_p]
    strAccumMode = ctypes.create_string_buffer( 32 );
    ierr = self.lib.AppGetAccumModeEntry( self.zHandle, iIdent, strAccumMode )
    return strAccumMode.value.decode()

  def set_accumInterval( self, dDays ):
    self.lib.AppSetAccumInterval.argtypes = [ctypes.c_int, ctypes.c_double ]
    return self.lib.AppSetAccumInterval( self.zHandle, dDays )

  def clear_accumIntervals( self ):
    self.lib.AppClearAccumIntervals.ctypes = [ctypes.c_int]
    return self.lib.AppClearAccumIntervals( self.zHandle )

  def set_accumIntervalSec( self, dSec ):
    self.lib.AppSetAccumIntervalSec.argtypes = [ctypes.c_int, ctypes.c_double ]
    return self.lib.AppSetAccumIntervalSec( self.zHandle, dSec )

  def get_accumIntervalSec( self ):
    return self.get_accumIntervalSecEntry( 1 )

  def get_accumIntervalSecEntry( self, iIdent ):
    self.lib.AppGetAccumIntervalSecEntry.argtypes = [ctypes.c_int, ctypes.c_int, c_double_p ]
    pdInterval = ctypes.c_double()
    ierr = self.lib.AppGetAccumIntervalSecEntry( self.zHandle, iIdent, ctypes.byref(pdInterval) )
    return pdInterval.value

  def get_numAccumIntervals( self ):
    self.lib.AppGetNumAccumIntervals.argtypes = [ctypes.c_int] 
    return self.lib.AppGetNumAccumIntervals( self.zHandle )

  def set_accumIncrementSec( self, dSec ):
    self.lib.AppSetAccumIncrementSec.argtypes = [ctypes.c_int, ctypes.c_double] 
    return self.lib.AppSetAccumIncrementSec( self.zHandle, dSec )

  def get_accumIncrementSec( self ):
    self.lib.AppGetAccumIncrementSec.argtypes = [ctypes.c_int, c_double_p]
    pdIncrementSec = ctypes.c_double()
    ierr = self.lib.AppGetAccumIncrementSec( self.zHandle, ctypes.byref(pdIncrementSec) )
    return pdIncrementSec.value

  def set_accumIncrementFrac( self, dFrac ):## units = fraction of interval
    self.lib.AppSetAccumIncrementFrac.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetAccumIncrementFrac( self.zHandle, dFrac ) 

  def get_accumIncrementFrac( self ):
    self.lib.AppGetAccumIncrementFrac.argtypes = [ctypes.c_int, c_double_p]
    pdIncrementFrac = ctypes.c_double()
    ierr = self.lib.AppGetAccumIncrementFrac( self.zHandle, ctypes.byref(pdIncrementFrac) )
    return pdIncrementFrac.value

  def set_aggregMean( self ):
    self.lib.AppSetAggregMean.argtypes = [ctypes.c_int]
    return self.lib.AppSetAggregMean( self.zHandle )

  def set_aggregMedian( self ):
    self.lib.AppSetAggregMedian.argtypes = [ctypes.c_int]
    return self.lib.AppSetAggregMedian( self.zHandle )

  def set_aggregConfLevel( self, iPercent ):
    self.lib.AppSetAggregConfLevel.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetAggregConfLevel( self.zHandle, iPercent )

  def set_aggregConfLevels( self, iaPercent ):
    self.lib.AppSetAggregConfLevels.argtypes = [ctypes.c_int,
                                                array_1d_int, ctypes.c_int]
    return self.lib.AppSetAggregConfLevels( self.zHandle, iaPercent.astype(np.int32), len(iaPercent))

  def clear_aggregConfLevels( self ):
    self.lib.AppClearAggregConfLevels.argtypes = [ctypes.c_int]
    return self.lib.AppClearAggregConfLevels( self.zHandle )

  def get_numAggregConfLevels( self ):
    self.lib.AppGetNumAggregConfLevels.argtypes = [ctypes.c_int]
    return self.lib.AppGetNumAggregConfLevels( self.zHandle )

  def get_aggregConfLevels( self ):
    self.lib.AppGetAggregConfLevels.argtypes = [ctypes.c_int,array_1d_int]
    iNumCL = self.get_numAggregConfLevels( )
    if iNumCL < 1:
      return iNumCL, np.zeros(1, dtype=np.intc)
    iaConfLevel = np.zeros( iNumCL, dtype=np.intc )
    iNum = self.lib.AppGetAggregConfLevels( self.zHandle, iaConfLevel )
    return iNum, iaConfLevel

  def set_legActivityLevel( self, strActivityLevel ):
    self.lib.AppSetLegActivityLevel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetLegActivityLevel( self.zHandle, strActivityLevel.encode() )

  def get_legActivityLevel( self ):
    self.lib.AppGetLegActivityLevel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strActivityLevel = ctypes.create_string_buffer( 32 );
    ierr = self.lib.AppGetLegActivityLevel( self.zHandle, strActivityLevel )
    return strActivityLevel.value.decode()

  def set_legActivityRange( self, strActivityRange ):
    self.lib.AppSetLegActivityRange.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetLegActivityRange( self.zHandle, strActivityRange.encode() )

  def get_legActivityRange( self ):
    self.lib.AppGetLegActivityRange.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strActivityRange = ctypes.create_string_buffer( 32 );
    ierr = self.lib.AppGetLegActivityRange( self.zHandle, strActivityRange )
    return strActivityRange.value.decode()

  def set_leg15DayAvgAp( self, d15DayAvgAp ):
    self.lib.AppSetLeg15DayAvgAp.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AppSetLeg15DayAvgAp( self.zHandle, d15DayAvgAp )

  def get_leg15DayAvgAp( self ):
    self.lib.AppGetLeg15DayAvgAp.argtypes = [ctypes.c_int,c_double_p]
    pd15DayAvgAp = ctypes.c_double()
    ierr = self.lib.AppGetLeg15DayAvgAp( self.zHandle, ctypes.byref(pd15DayAvgAp) )
    return pd15DayAvgAp.value
    
  def set_legFixedEpoch( self, iVerdict=1 ):
    self.lib.AppSetLegFixedEpoch.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetLegFixedEpoch( self.zHandle, iVerdict )

  def get_legFixedEpoch( self ):
    self.lib.AppGetLegFixedEpoch.argtypes = [ctypes.c_int]
    return self.lib.AppGetLegFixedEpoch( self.zHandle )

  def set_legShiftSAA( self, iVerdict=1 ):
    self.lib.AppSetLegShiftSAA.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.AppSetLegShiftSAA( self.zHandle, iVerdict )

  def get_legShiftSAA( self ):
    self.lib.AppGetLegShiftSAA.argtypes = [ctypes.c_int]
    return self.lib.AppGetLegShiftSAA( self.zHandle )

  def set_camMagfieldModel( self, strMFModel ):
    self.lib.AppSetCamMagfieldModel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetCamMagfieldModel( self.zHandle, strMFModel.encode() )

  def get_camMagfieldModel( self ):
    self.lib.AppGetCamMagfieldModel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strMFModel = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetCamMagfieldModel( self.zHandle, strMFModel )
    return strMFModel.value.decode()

  def set_camDataFilter( self, strDataFilter ):
    self.lib.AppSetCamDataFilter.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetCamDataFilter( self.zHandle, strDataFilter .encode() )

  def get_camDataFilter( self ):
    self.lib.AppGetCamDataFilter.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strDataFilter = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetCamDataFilter( self.zHandle, strDataFilter )
    return strDataFilter.value.decode()

  def set_camPitchAngleBin( self, strPitchAngleBin ):
    self.lib.AppSetCamPitchAngleBin.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetCamPitchAngleBin( self.zHandle, strPitchAngleBin.encode() )

  def get_camPitchAngleBin( self ):
    self.lib.AppGetCamPitchAngleBin.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strPitchAngleBin = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetCamPitchAngleBin( self.zHandle, strPitchAngleBin )
    return strPitchAngleBin.value.decode()

  def set_camSpecies( self, strSpecies ):
    self.lib.AppSetCamSpecies.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetCamSpecies( self.zHandle, strSpecies.encode() )

  def get_camSpecies( self ):
    self.lib.AppGetCamSpecies.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strSpecies = ctypes.create_string_buffer(16)
    ierr = self.lib.AppGetCamSpecies( self.zHandle, strSpecies )
    return strSpecies.value.decode()

  def set_binDirName( self, strBinDirName ):
    self.lib.AppSetBinDirName.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AppSetBinDirName( self.zHandle, strBinDirName.encode() )

  def get_binDirName( self ):
    self.lib.AppGetBinDirName.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strBinDirName = ctypes.create_string_buffer(256)
    ierr = self.lib.AppGetBinDirName( self.zHandle, strBinDirName )
    return strBinDirName.value.decode()

  def get_gmtsec( self, iHr, iMin, dSec ):
    self.lib.AppGetGmtSeconds.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_double]
    self.lib.AppGetGmtSeconds.restype = ctypes.c_double #***
    return self.lib.AppGetGmtSeconds( self.zHandle, iHr, iMin, dSec )

  def get_dayYear( self, iYear, iMonth, iDay ):
    self.lib.AppGetDayOfYear.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int]
    return self.lib.AppGetDayOfYear( self.zHandle, iYear, iMonth, iDay )

  def get_modJulDate( self, iYear, iDdd, dGmtsec ):
    self.lib.AppGetModifiedJulianDate.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_double]
    self.lib.AppGetModifiedJulianDate.restype = ctypes.c_double #***
    return self.lib.AppGetModifiedJulianDate( self.zHandle, iYear, iDdd, dGmtsec )

  def get_modJulDateUnix( self, iUnixTime ):
    self.lib.AppGetModifiedJulianDateUnix.argtypes = [ctypes.c_int, ctypes.c_int]
    self.lib.AppGetModifiedJulianDateUnix.restype = ctypes.c_double #***
    return self.lib.AppGetModifiedJulianDateUnix( self.zHandle, iUnixTime )

  def get_dateTime( self, dMjdValue ):
    iYear = ctypes.c_int()
    iDdd = ctypes.c_int()
    dGmtsec = ctypes.c_double()
    self.lib.AppGetDateTime.argtypes = [ctypes.c_int, ctypes.c_double, c_int_p, c_int_p, c_double_p]
    ierr = self.lib.AppGetDateTime( self.zHandle, dMjdValue, 
              ctypes.byref(iYear), ctypes.byref(iDdd), ctypes.byref(dGmtsec) )
    return iYear.value, iDdd.value, dGmtsec.value

  def get_hms( self, dGmtsec ):
    iHr = ctypes.c_int()
    iMin = ctypes.c_int()
    dSec = ctypes.c_double()
    self.lib.AppGetHoursMinSec.argtypes = [ctypes.c_int, ctypes.c_double, c_int_p, c_int_p, c_double_p]
    ierr = self.lib.AppGetHoursMinSec( self.zHandle, dGmtsec, 
              ctypes.byref(iHr), ctypes.byref(iMin), ctypes.byref(dSec) )
    return iHr.value, iMin.value, dSec.value

  def get_monthDay( self, iYear, iDdd ):
    iMon = ctypes.c_int()
    iDay = ctypes.c_int()
    self.lib.AppGetMonthDay.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, c_int_p, c_int_p]
    ierr = self.lib.AppGetMonthDay( self.zHandle, iYear, iDdd, 
              ctypes.byref(iMon), ctypes.byref(iDay) )
    return iMon.value, iDay.value

  def __del__( self ):
    self.lib.AppShutDown.argtypes = [ctypes.c_int]
    self.lib.AppShutDown( self.zHandle )
    self.zHandle = 0

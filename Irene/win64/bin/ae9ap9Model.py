#******************************************************************************
#
# File: ae9ap9Model.py
#
# Description: Python language routines to perform the calculation of flux values
#   using the underlying AE9, AP9, or SPM models.
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
import sys
import ctypes
import numpy as np
import numpy.ctypeslib as npct

array_1d_double = npct.ndpointer(dtype=np.double, ndim=1, flags='CONTIGUOUS')
array_2d_double = npct.ndpointer(dtype=np.double, ndim=2, flags='CONTIGUOUS')
array_3d_double = npct.ndpointer(dtype=np.double, ndim=3, flags='CONTIGUOUS')

if 'IRENE_SYS' not in os.environ:
  sys.exit('Error: undefined IRENE environment variables - calling script requires "import irene_defs.py"')
  
class Ae9Ap9Model:

  def __init__( self ):
    if os.environ['IRENE_SYS'] == 'linux':
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libae9ap9model.so')
    else:
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libae9ap9model_c.dll')
    self.zHandle = self.lib.Ae9Ap9StartUp()

  def set_model( self, strModel ):
    self.lib.Ae9Ap9SetModel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.Ae9Ap9SetModel( self.zHandle, strModel.encode() )
  def get_model( self ):
    self.lib.Ae9Ap9GetModel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModel = ctypes.create_string_buffer(16)
    ierr = self.lib.Ae9Ap9GetModel( self.zHandle,strModel )
    return strModel.value.decode()

  def set_modelDBDir( self, strModelDBDir ):
    self.lib.Ae9Ap9SetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.Ae9Ap9SetModelDBDir( self.zHandle, strModelDBDir.encode() )
  def get_modelDBDir( self ):
    self.lib.Ae9Ap9GetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelDBDir = ctypes.create_string_buffer(256)
    ierr = self.lib.Ae9Ap9GetModelDBDir( self.zHandle,strModelDBDir )
    return strModelDBDir.value.decode()

  def set_modelDBFile( self, strModelDB ):
    self.lib.Ae9Ap9SetModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.Ae9Ap9SetModelDBFile( self.zHandle, strModelDB.encode() )
  def get_modelDBFile( self ):
    self.lib.Ae9Ap9GetModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelDB = ctypes.create_string_buffer(256)
    ierr = self.lib.Ae9Ap9GetModelDBFile( self.zHandle,strModelDB )
    return strModelDB.value.decode()

  def get_modelName( self ):
    self.lib.Ae9Ap9GetModelName.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelName = ctypes.create_string_buffer(32)
    ierr = self.lib.Ae9Ap9GetModelName( self.zHandle, strModelName )
    return strModelName.value.decode()
  def get_modelSpecies( self ):
    self.lib.Ae9Ap9GetModelSpecies.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelSpecies = ctypes.create_string_buffer(32)
    ierr = self.lib.Ae9Ap9GetModelSpecies( self.zHandle, strModelSpecies )
    return strModelSpecies.value.decode()

  def test_badHandle( self ):
    self.lib.Ae9Ap9LoadModelDB.argtypes = [ctypes.c_int]
    return self.lib.Ae9Ap9LoadModelDB( self.zHandle-13 )

  def set_kPhiDBFile( self, strKPhiDB ):
    self.lib.Ae9Ap9SetKPhiDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.Ae9Ap9SetKPhiDBFile( self.zHandle, strKPhiDB.encode() )
  def get_kPhiDBFile( self ):
    self.lib.Ae9Ap9GetKPhiDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strKPhiDB = ctypes.create_string_buffer(256)
    ierr = self.lib.Ae9Ap9GetKPhiDBFile( self.zHandle,strKPhiDB )
    return strKPhiDB.value.decode()

  def set_kHMinDBFile( self, strKHMinDB ):
    self.lib.Ae9Ap9SetKHMinDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.Ae9Ap9SetKHMinDBFile( self.zHandle, strKHMinDB.encode() )
  def get_kHMinDBFile( self ):
    self.lib.Ae9Ap9GetKHMinDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strKHMinDB = ctypes.create_string_buffer(256)
    ierr = self.lib.Ae9Ap9GetKHMinDBFile( self.zHandle,strKHMinDB )
    return strKHMinDB.value.decode()

  def set_magfieldDBFile( self, strMagfieldDB ):
    self.lib.Ae9Ap9SetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.Ae9Ap9SetMagfieldDBFile( self.zHandle, strMagfieldDB.encode() )
  def get_magfieldDBFile( self ):
    self.lib.Ae9Ap9GetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strMagfieldDB = ctypes.create_string_buffer(256)
    ierr = self.lib.Ae9Ap9GetMagfieldDBFile( self.zHandle,strMagfieldDB )
    return strMagfieldDB.value.decode()

  def load_modelDB( self ):
    self.lib.Ae9Ap9LoadModelDB.argtypes = [ctypes.c_int]
    return self.lib.Ae9Ap9LoadModelDB( self.zHandle )

  def set_fluxEnvironOmni( self, strFluxType, daEnergies, daEnergies2, daTimes,
    strCoordSys, strCoordUnits, daCoord1, daCoord2, daCoord3 ):
    self.lib.Ae9Ap9SetFluxEnvironmentOmni.argtypes = [ctypes.c_int, ctypes.c_char_p,
      array_1d_double, array_1d_double, array_1d_double, ctypes.c_char_p, ctypes.c_char_p,
      array_1d_double, array_1d_double, array_1d_double, ctypes.c_int, ctypes.c_int ]
    return self.lib.Ae9Ap9SetFluxEnvironmentOmni( self.zHandle, strFluxType.encode(),
      daEnergies.astype(np.float64), daEnergies2.astype(np.float64), daTimes, strCoordSys.encode(),
      strCoordUnits.encode(), daCoord1, daCoord2, daCoord3, len(daTimes), len(daEnergies) )

  # Set environment for fixed set of pitch angles
  def set_fluxEnvironFixPitch( self, strFluxType, daEnergies, daEnergies2, daTimes,
    strCoordSys, strCoordUnits, daCoord1, daCoord2, daCoord3, daPitchAngles ):
    self.lib.Ae9Ap9SetFluxEnvironmentFixPitch.argtypes = [ctypes.c_int, ctypes.c_char_p,
      array_1d_double, array_1d_double, array_1d_double, ctypes.c_char_p, ctypes.c_char_p,
      array_1d_double, array_1d_double, array_1d_double, array_1d_double,
      ctypes.c_int, ctypes.c_int, ctypes.c_int ]
    return self.lib.Ae9Ap9SetFluxEnvironmentFixPitch( self.zHandle, strFluxType.encode(),
      daEnergies.astype(np.float64), daEnergies2.astype(np.float64), daTimes, strCoordSys.encode(),
      strCoordUnits.encode(), daCoord1, daCoord2, daCoord3, daPitchAngles.astype(np.float64),
      len(daTimes), len(daEnergies), len(daPitchAngles) )

  # Set environment for directional flux
  def set_fluxEnvironDirVec( self, strFluxType, daEnergies, daEnergies2, daTimes,
    strCoordSys, strCoordUnits, daCoord1, daCoord2, daCoord3,
    da2FluxDir1, da2FluxDir2, da2FluxDir3 ):
    self.lib.Ae9Ap9SetFluxEnvironmentDirVec.argtypes = [ctypes.c_int, ctypes.c_char_p,
      array_1d_double, array_1d_double, array_1d_double, ctypes.c_char_p, ctypes.c_char_p,
      array_1d_double, array_1d_double, array_1d_double,
      array_2d_double, array_2d_double, array_2d_double,
      ctypes.c_int, ctypes.c_int, ctypes.c_int ]
    return self.lib.Ae9Ap9SetFluxEnvironmentDirVec( self.zHandle, strFluxType.encode(),
      daEnergies.astype(np.float64), daEnergies2.astype(np.float64), daTimes, strCoordSys.encode(),
      strCoordUnits.encode(), daCoord1, daCoord2, daCoord3, da2FluxDir1, da2FluxDir2, da2FluxDir3,
      len(daTimes), len(daEnergies), da2FluxDir1.shape[1] )

  # Set environment for ephemeris-associated set of pitch angles
  def set_fluxEnvironVarPitch( self, strFluxType, daEnergies, daEnergies2, daTimes,
    strCoordSys, strCoordUnits, daCoord1, daCoord2, daCoord3, da2PitchAngles ):
    self.lib.Ae9Ap9SetFluxEnvironmentVarPitch.argtypes = [ctypes.c_int, ctypes.c_char_p,
      array_1d_double, array_1d_double, array_1d_double, ctypes.c_char_p, ctypes.c_char_p,
      array_1d_double, array_1d_double, array_1d_double, array_2d_double,
      ctypes.c_int, ctypes.c_int, ctypes.c_int ]
    if da2PitchAngles.shape[0] != len(daTimes):
      return -1
    return self.lib.Ae9Ap9SetFluxEnvironmentVarPitch( self.zHandle, strFluxType.encode(),
      daEnergies.astype(np.float64), daEnergies2.astype(np.float64), daTimes, strCoordSys.encode(),
      strCoordUnits.encode(), daCoord1, daCoord2, daCoord3, da2PitchAngles.astype(np.float64),
      len(daTimes), len(daEnergies), da2PitchAngles.shape[1] )

  # retrieve pitch angle values (particularly useful when direction vectors specified)
  def get_pitchAngles( self ):
    self.lib.Ae9Ap9GetPitchAngles.argtypes = [ctypes.c_int, array_2d_double]
    iNumT = self.get_numTimes( )
    if iNumT<1 :
      return iNumT, np.zeros((1,1,1))
    iNumD = self.get_numDirections( )
    if iNumD<1:
      return iNumD, np.zeros((1,1,1))
    da2Pitch = np.zeros( (iNumT, iNumD) )
    ierr = self.lib.Ae9Ap9GetPitchAngles( self.zHandle, da2Pitch )
    return ierr, da2Pitch

  def get_numTimes( self ):
    self.lib.Ae9Ap9GetNumTimes.argtypes = [ctypes.c_int]
    return self.lib.Ae9Ap9GetNumTimes( self.zHandle )

  def get_numEnergies( self ):
    self.lib.Ae9Ap9GetNumEnergies.argtypes = [ctypes.c_int]
    return self.lib.Ae9Ap9GetNumEnergies( self.zHandle )

  def get_numDirections( self ):
    self.lib.Ae9Ap9GetNumDirections.argtypes = [ctypes.c_int]
    return self.lib.Ae9Ap9GetNumDirections( self.zHandle )

  # Compute flux for a mean flux environment (date,energy,direction)
  def computeFluxMean( self ):
    self.lib.Ae9Ap9ComputeFluxMean.argtypes = [ctypes.c_int, array_3d_double]
    iNumT = self.get_numTimes( )
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumE = self.get_numEnergies( )
    if iNumE < 1: iNumE = 1 # error handled at C++ level
    iNumD = self.get_numDirections( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    da3FluxData = np.zeros( (iNumT, iNumE, iNumD) )
    ierr = self.lib.Ae9Ap9ComputeFluxMean( self.zHandle, da3FluxData )
    return ierr, da3FluxData
  def computeFlyinMean( self ):
    return self.computeFluxMean( )

  # Compute flux for a statistically perturbed, time invariant mean
  #    flux environment (date,energy,direction)
  def computeFluxPerturbedMean( self, iScenario ):
    self.lib.Ae9Ap9ComputeFluxPerturbedMean.argtypes = [ctypes.c_int,
                                                       ctypes.c_int, array_3d_double]
    iNumT = self.get_numTimes( )
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumE = self.get_numEnergies( )
    if iNumE < 1: iNumE = 1 # error handled at C++ level
    iNumD = self.get_numDirections( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    da3FluxData = np.zeros( (iNumT, iNumE, iNumD) )
    ierr = self.lib.Ae9Ap9ComputeFluxPerturbedMean( self.zHandle, iScenario, da3FluxData )
    return ierr, da3FluxData
  def computeFlyinPerturbedMean( self, iScenario ):
    return self.computeFluxPerturbedMean( iScenario )

  # Compute flux for a particular percentile flux environment (date,energy,direction)
  def computeFluxPercentile( self, iPercent ):
    self.lib.Ae9Ap9ComputeFluxPercentile.argtypes = [ctypes.c_int,
                                                    ctypes.c_int, array_3d_double]
    iNumT = self.get_numTimes( )
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumE = self.get_numEnergies( )
    if iNumE < 1: iNumE = 1 # error handled at C++ level
    iNumD = self.get_numDirections( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    da3FluxData = np.zeros( (iNumT, iNumE, iNumD) )
    ierr = self.lib.Ae9Ap9ComputeFluxPercentile( self.zHandle, iPercent, da3FluxData )
    return ierr, da3FluxData
  def computeFlyinPercentile( self, iPercent ):
    return self.computeFluxPercentile( iPercent )

  # Compute flux for monte carlo (time variant) scenario
  #   flux environments (date,energy,direction)
  def computeFluxScenario( self, dEpochTime, iScenario, iFluxPert=1 ):
    self.lib.Ae9Ap9ComputeFluxScenario.argtypes = [ctypes.c_int, ctypes.c_double, ctypes.c_int,
                                                   array_3d_double, ctypes.c_int]
    iNumT = self.get_numTimes( )
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumE = self.get_numEnergies( )
    if iNumE < 1: iNumE = 1 # error handled at C++ level
    iNumD = self.get_numDirections( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    da3FluxData = np.zeros( (iNumT, iNumE, iNumD) )
    ierr = self.lib.Ae9Ap9ComputeFluxScenario( self.zHandle, dEpochTime,
                                               iScenario, da3FluxData, iFluxPert )
    return ierr, da3FluxData
  def computeFlyinScenario( self, dEpochTime, iScenario, iFluxPert=1 ):
    return self.computeFluxScenario( dEpochTime, iScenario, iFluxPert )

  # Retrieves a list of pitch angles used in the computation of omnidirectional flux
  def get_defaultPitchAngles( self ):
    self.lib.Ae9Ap9GetDefaultPitchAngles.argtypes = [ctypes.c_int, array_1d_double]
    daDefPitch = np.zeros( 32 )
    ierr = self.lib.Ae9Ap9GetDefaultPitchAngles( self.zHandle, daDefPitch )
    return ierr, daDefPitch

  def __del__( self ):
    self.lib.Ae9Ap9ShutDown.argtypes = [ctypes.c_int]
    self.lib.Ae9Ap9ShutDown( self.zHandle )
    self.zHandle = 0

#******************************************************************************
#
# File: radenvModel.py
#
# Description: Python language routines to perform the calculation of flux values
#  using the underlying AE8, AP8, CRRESELE or CRRESPRO models.
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

c_double_p = ctypes.POINTER(ctypes.c_double)

if 'IRENE_SYS' not in os.environ:
  sys.exit('Error: undefined IRENE environment variables - calling script requires "import irene_defs.py"')
  
class RadEnvModel:

  def __init__( self ):
    if os.environ['IRENE_SYS'] == 'linux':
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/liblegacymodel.so')
    else:
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/liblegacymodel_c.dll')
    self.zHandle = self.lib.RadEnvStartUp()

  def set_model( self, strModel ):
    self.lib.RadEnvSetModel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.RadEnvSetModel( self.zHandle, strModel.encode() )
  def get_model( self ):
    self.lib.RadEnvGetModel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    model = ctypes.create_string_buffer(256)
    ierr = self.lib.RadEnvGetModel( self.zHandle, model )
    return model.value.decode()

  def set_modelDBDir( self, strModelDBDir ):
    self.lib.RadEnvSetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.RadEnvSetModelDBDir( self.zHandle, strModelDBDir.encode() )
  def get_modelDBDir( self ):
    self.lib.RadEnvGetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelDBDir = ctypes.create_string_buffer(256)
    ierr = self.lib.RadEnvGetModelDBDir( self.zHandle,strModelDBDir )
    return strModelDBDir.value.decode()

  def set_modelDBFile( self, strModelDB ):
    self.lib.RadEnvSetModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.RadEnvSetModelDBFile( self.zHandle, strModelDB.encode() )
  def get_modelDBFile( self ):
    self.lib.RadEnvGetModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    modelDB = ctypes.create_string_buffer(256)
    ierr = self.lib.RadEnvGetModelDBFile( self.zHandle, modelDB )
    return modelDB.value.decode()

  def set_magfieldDBFile( self, strMagfieldDB ):
    self.lib.RadEnvSetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.RadEnvSetMagfieldDBFile( self.zHandle, strMagfieldDB.encode() )
  def get_magfieldDBFile( self ):
    self.lib.RadEnvGetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    magfieldDB = ctypes.create_string_buffer(256)
    ierr = self.lib.RadEnvGetMagfieldDBFile( self.zHandle, magfieldDB )
    return magfieldDB.value.decode()

  def set_shielddose2DBFile( self, strShieldDose2DB ):
    self.lib.RadEnvSetShieldDose2DBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.RadEnvSetShieldDose2DBFile( self.zHandle, strShieldDose2DB.encode() )
  def get_shielddose2DBFile( self ):
    self.lib.RadEnvGetShieldDose2DBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    shielddose2DB = ctypes.create_string_buffer(256)
    ierr = self.lib.RadEnvGetShieldDose2DBFile( self.zHandle, shielddose2DB )
    return shielddose2DB.value.decode()

  def set_actLevel( self, strActLevel ):
    self.lib.RadEnvSetActivityLevel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.RadEnvSetActivityLevel( self.zHandle, strActLevel.encode() )
  def get_actLevel( self ):
    self.lib.RadEnvGetActivityLevel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    actLevel = ctypes.create_string_buffer(16)
    ierr = self.lib.RadEnvGetActivityLevel( self.zHandle, actLevel )
    return actLevel.value.decode()

  def set_actRange( self, strActRange ):
    self.lib.RadEnvSetActivityRange.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.RadEnvSetActivityRange( self.zHandle, strActRange.encode() )
  def get_actRange( self ):
    self.lib.RadEnvGetActivityRange.argtypes = [ctypes.c_int, ctypes.c_char_p]
    actRange = ctypes.create_string_buffer(16)
    ierr = self.lib.RadEnvGetActivityRange( self.zHandle, actRange )
    return actRange.value.decode()

  def set_15dayAp( self, d15DayAvgAp ):
    self.lib.RadEnvSet15DayAvgAp.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.RadEnvSet15DayAvgAp( self.zHandle, d15DayAvgAp )
  def get_15dayAp( self ):
    self.lib.RadEnvGet15DayAvgAp.argtypes = [ctypes.c_int, c_double_p]
    pd15DayAvgAp = ctypes.c_double()
    ierr = self.lib.RadEnvGet15DayAvgAp( self.zHandle, ctypes.byref(pd15DayAvgAp) )
    return pd15DayAvgAp.value

  def set_fixedEpoch( self, bVerdict=True ):
    self.lib.RadEnvSetFixedEpoch.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.RadEnvSetFixedEpoch( self.zHandle, bVerdict )
  def get_fixedEpoch( self, bVerdict=True ):
    self.lib.RadEnvGetFixedEpoch.argtypes = [ctypes.c_int]
    return self.lib.RadEnvGetFixedEpoch( self.zHandle )

  def set_shiftSAA( self, bVerdict=True ):
    self.lib.RadEnvSetShiftSAA.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.RadEnvSetShiftSAA( self.zHandle, bVerdict )
  def get_shiftSAA( self, bVerdict=True ):
    self.lib.RadEnvGetShiftSAA.argtypes = [ctypes.c_int]
    return self.lib.RadEnvGetShiftSAA( self.zHandle )

  def set_fluxType( self, strFluxType ):
    self.lib.RadEnvSetFluxType.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.RadEnvSetFluxType( self.zHandle, strFluxType.encode() )
  def get_fluxType( self ):
    self.lib.RadEnvGetFluxType.argtypes = [ctypes.c_int, ctypes.c_char_p]
    fluxType = ctypes.create_string_buffer(32)
    ierr = self.lib.RadEnvGetFluxType( self.zHandle, fluxType )
    return fluxType.value.decode()

  def set_energies( self, daEnergies ):
    self.lib.RadEnvSetEnergies.argtypes = [ctypes.c_int, 
                                          array_1d_double, ctypes.c_int]
    return self.lib.RadEnvSetEnergies( self.zHandle, daEnergies.astype(np.float64), len(daEnergies) )

  def get_numEnergies( self ):
    self.lib.RadEnvGetNumEnergies.argtypes = [ctypes.c_int]
    return self.lib.RadEnvGetNumEnergies( self.zHandle )

  def set_coordSys( self, strCoordSys, strCoordUnits ):
    self.lib.RadEnvSetCoordSys.argtypes = [ctypes.c_int, ctypes.c_char_p, 
                                                        ctypes.c_char_p]
    return self.lib.RadEnvSetCoordSys( self.zHandle, strCoordSys.encode(), 
                                                 strCoordUnits.encode() )
  def get_coordSys( self ):
    self.lib.RadEnvGetCoordSys.argtypes = [ctypes.c_int, ctypes.c_char_p]
    fluxType = ctypes.create_string_buffer(16)
    ierr = self.lib.RadEnvGetCoordSys( self.zHandle, fluxType )
    return fluxType.value.decode()
  def get_coordSysUnits( self ):
    self.lib.RadEnvGetCoordSysUnits.argtypes = [ctypes.c_int, ctypes.c_char_p]
    fluxType = ctypes.create_string_buffer(16)
    ierr = self.lib.RadEnvGetCoordSysUnits( self.zHandle, fluxType )
    return fluxType.value.decode()

  def set_ephemeris( self, daTime, daCoord1, daCoord2, daCoord3):
    self.lib.RadEnvSetEphemeris.argtypes = [ctypes.c_int, array_1d_double,
                                            array_1d_double, array_1d_double,
                                            array_1d_double,ctypes.c_int]
    if (len(daTime) == len(daCoord1) and len(daTime) == len(daCoord2)
            and len(daTime) == len(daCoord3)):
        return self.lib.RadEnvSetEphemeris( self.zHandle, daTime,
                                            daCoord1, daCoord2, daCoord3,
                                            len(daTime))
    else:
        return -3;

  def get_numEphemeris( self ):
    self.lib.RadEnvGetNumEphemeris.argtypes = [ctypes.c_int]
    return self.lib.RadEnvGetNumEphemeris( self.zHandle )

  def computeFlux( self ):
    self.lib.RadEnvComputeFlux.argtypes = [ctypes.c_int, array_2d_double]
    iNumT = self.get_numEphemeris( )
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumE = self.get_numEnergies( )
    if iNumE < 1: iNumE = 1 # error handled at C++ level
    da2FluxData = np.zeros( (iNumT, iNumE) )  # Default data type is numpy.float64.
    ierr = self.lib.RadEnvComputeFlux( self.zHandle, da2FluxData )
    if ierr < 0:
      da2FluxData = np.zeros((1,1))
    return ierr, da2FluxData

  def __del__( self ):
    self.lib.RadEnvShutDown.argtypes = [ctypes.c_int]
    self.lib.RadEnvShutDown( self.zHandle )
    self.zHandle = 0



#******************************************************************************
#
# File: cammiceModel.py
#
# Description: Python language routines to perform the calculation of flux values
#   using the underlying CAMMICE/MICS model.
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
  
class CammiceModel:

  def __init__( self ):
    if os.environ['IRENE_SYS'] == 'linux':
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/liblegacymodel.so')
    else:
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/liblegacymodel_c.dll')
    self.zHandle = self.lib.CammiceStartUp()

  def set_modelDBDir( self, strModelDBDir ):
    self.lib.CammiceSetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.CammiceSetModelDBDir( self.zHandle, strModelDBDir.encode() )
  def get_modelDBDir( self ):
    self.lib.CammiceGetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelDBDir = ctypes.create_string_buffer(256)
    ierr = self.lib.CammiceGetModelDBDir( self.zHandle,strModelDBDir )
    return strModelDBDir.value.decode()

  def set_modelDBFile( self, strModelDB ):
    self.lib.CammiceSetModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.CammiceSetModelDBFile( self.zHandle, strModelDB.encode() )
  def get_modelDBFile( self ):
    self.lib.CammiceGetModelDBFile.argtypes = [ctypes.c_int]
    strFile = ctypes.create_string_buffer(256)
    ierr = self.lib.CammiceGetModelDBFile( self.zHandle, strFile )
    return strFile.value.decode()

  def set_magfieldDBFile( self, strMagfieldDB ):
    self.lib.CammiceSetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.CammiceSetMagfieldDBFile( self.zHandle, strMagfieldDB.encode() )
  def get_magfieldDBFile( self ):
    self.lib.CammiceGetMagfieldDBFile.argtypes = [ctypes.c_int]
    strFile = ctypes.create_string_buffer(256)
    ierr = self.lib.CammiceGetMagfieldDBFile( self.zHandle, strFile )
    return strFile.value.decode()

  def set_magfieldModel( self, strMagfieldModel ):
    self.lib.CammiceSetMagfieldModel.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.CammiceSetMagfieldModel( self.zHandle, strMagfieldModel.encode() )
  def get_magfieldModel( self ):
    self.lib.CammiceGetMagfieldModel.argtypes = [ctypes.c_int]
    strFile = ctypes.create_string_buffer(16)
    ierr = self.lib.CammiceGetMagfieldModel( self.zHandle, strFile )
    return strFile.value.decode()

  def set_dataFilter( self, strDataFilter ):
    self.lib.CammiceSetDataFilter.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.CammiceSetDataFilter( self.zHandle, strDataFilter.encode() )
  def get_dataFilter( self ):
    self.lib.CammiceGetMagfieldModel.argtypes = [ctypes.c_int]
    strFile = ctypes.create_string_buffer(16)
    ierr = self.lib.CammiceGetDataFilter( self.zHandle, strFile )
    return strFile.value.decode()

  def set_pitchAngleBin( self, strPitchAngleBin ):
    self.lib.CammiceSetPitchAngleBin.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.CammiceSetPitchAngleBin( self.zHandle, strPitchAngleBin.encode() )
  def get_pitchAngleBin( self ):
    self.lib.CammiceGetPitchAngleBin.argtypes = [ctypes.c_int]
    strPitchAngleBin = ctypes.create_string_buffer(16)
    ierr = self.lib.CammiceGetPitchAngleBin( self.zHandle, strPitchAngleBin )
    return strPitchAngleBin.value.decode()

  def set_species( self, strSpecies ):
    self.lib.CammiceSetSpecies.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.CammiceSetSpecies( self.zHandle, strSpecies.encode() )
  def get_species( self ):
    self.lib.CammiceGetSpecies.argtypes = [ctypes.c_int]
    strSpecies = ctypes.create_string_buffer(16)
    ierr = self.lib.CammiceGetSpecies( self.zHandle, strSpecies )
    return strSpecies.value.decode()

  def set_coordSys( self, strCoordSys, strCoordUnits ):
    self.lib.CammiceSetCoordSys.argtypes = [ctypes.c_int, 
                                            ctypes.c_char_p, ctypes.c_char_p]
    return self.lib.CammiceSetCoordSys( self.zHandle, strCoordSys.encode(), 
                                                   strCoordUnits.encode() )
  def get_coordSys( self ):
    self.lib.CammiceGetCoordSys.argtypes = [ctypes.c_int]
    strCoordsys = ctypes.create_string_buffer(16)
    ierr = self.lib.CammiceGetCoordSys( self.zHandle, strCoordsys )
    return strCoordsys.value.decode()
  def get_coordSysUnits( self ):
    self.lib.CammiceGetCoordSysUnits.argtypes = [ctypes.c_int]
    strCoordsysUnits = ctypes.create_string_buffer(16)
    ierr = self.lib.CammiceGetCoordSysUnits( self.zHandle, strCoordsysUnits )
    return strCoordsysUnits.value.decode()

  def set_ephemeris( self, daTime, daCoord1, daCoord2, daCoord3 ):
    self.lib.CammiceSetEphemeris.argtypes = [ctypes.c_int, array_1d_double, 
                                array_1d_double, array_1d_double, array_1d_double, 
                                ctypes.c_int]
    if (len(daTime) == len(daCoord1) and len(daTime) == len(daCoord2)
            and len(daTime) == len(daCoord3)):
        return self.lib.CammiceSetEphemeris( self.zHandle, daTime,daCoord1, daCoord2,
                                             daCoord3, len(daTime) )
    else:
        return -3;

  def get_numEphemeris( self ):
    self.lib.CammiceGetNumEphemeris.argtypes = [ctypes.c_int]
    return self.lib.CammiceGetNumEphemeris( self.zHandle )

  def computeFlux( self ):
    self.lib.CammiceComputeFlux.argtypes = [ctypes.c_int, array_2d_double]
    iNumT = self.lib.CammiceGetNumEphemeris( self.zHandle )
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumE = 12 # fixed
    da2FluxData = np.zeros( (iNumT, iNumE) )  # Default data type is numpy.float64.
    ierr = self.lib.CammiceComputeFlux( self.zHandle, da2FluxData )
    if ierr<0:
      da2FluxData = np.zeros((1,1))
    return ierr, da2FluxData

  def __del__( self ):
    self.lib.CammiceShutDown.argtypes = [ctypes.c_int]
    self.lib.CammiceShutDown( self.zHandle )
    self.zHandle = 0


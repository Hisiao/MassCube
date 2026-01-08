#******************************************************************************
#
# File: doseModel.py
#
# Description: Python language routines to perform the calculation of dosage rates
#   and/or accumulations, employing the underlying ShieldDose2 model.
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
  
class DoseModel:

  def __init__( self ):
    if os.environ['IRENE_SYS'] == 'linux':
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libdosemodel.so')
    else:
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libdosemodel_c.dll')
    self.zHandle = self.lib.DoseStartUp()

  def set_modelDBDir( self, strModelDBDir ):
    self.lib.DoseSetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseSetModelDBDir( self.zHandle, strModelDBDir.encode() )
  def get_modelDBDir( self ):
    self.lib.DoseGetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelDBDir = ctypes.create_string_buffer(256)
    ierr = self.lib.DoseGetModelDBDir( self.zHandle,strModelDBDir )
    return strModelDBDir.value.decode()

  def set_modelDBFile( self, strModelDB ):
    self.lib.DoseSetModelDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseSetModelDBFile( self.zHandle, strModelDB.encode() )
  def get_modelDBFile( self ):
    self.lib.DoseGetModelDBFile.argtypes = [ctypes.c_int]
    strFile = ctypes.create_string_buffer(256)
    ierr = self.lib.DoseGetModelDBFile( self.zHandle, strFile )
    return strFile.value.decode()

  def set_species( self, strSpecies ):
    self.lib.DoseSetSpecies.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseSetSpecies( self.zHandle, strSpecies.encode() )
  def get_species( self ):
    self.lib.DoseGetSpecies.argtypes = [ctypes.c_int]
    strSpecies = ctypes.create_string_buffer(16)
    ierr = self.lib.DoseGetSpecies( self.zHandle, strSpecies )
    return strSpecies.value.decode()

  def test_badHandle( self ):
    self.lib.DoseSetSpecies.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strSpecies = 'electrons'
    return self.lib.DoseSetSpecies( self.zHandle-3, strSpecies.encode() )

  def set_energies( self, daEnergies, strEnergyUnits="MeV" ):
    self.lib.DoseSetEnergies.argtypes = [ctypes.c_int, array_1d_double,
                                         ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseSetEnergies( self.zHandle, daEnergies.astype(np.float64), len(daEnergies),
                                     strEnergyUnits.encode() )

  def get_numEnergies( self ):
    self.lib.DoseGetNumEnergies.argtypes = [ctypes.c_int]
    return self.lib.DoseGetNumEnergies( self.zHandle )

  def set_depths( self, daDepths, strDepthUnits ):
    self.lib.DoseSetDepths.argtypes = [ctypes.c_int, array_1d_double,
                                       ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseSetDepths( self.zHandle, daDepths.astype(np.float64), len(daDepths),
                                   strDepthUnits.encode() )
  def get_numDepths( self ):
    self.lib.DoseGetNumDepths.argtypes = [ctypes.c_int]
    return self.lib.DoseGetNumDepths( self.zHandle )

  def set_detector( self, strDetector ):
    self.lib.DoseSetDetector.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseSetDetector( self.zHandle, strDetector.encode() )
  def get_detector( self ):
    self.lib.DoseGetDetector.argtypes = [ctypes.c_int]
    strDetector = ctypes.create_string_buffer(32)
    ierr = self.lib.DoseGetDetector( self.zHandle, strDetector )
    return strDetector.value.decode()

  def set_geometry( self, strGeometry ):
    self.lib.DoseSetGeometry.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseSetGeometry( self.zHandle, strGeometry.encode() )
  def get_geometry( self ):
    self.lib.DoseGetGeometry.argtypes = [ctypes.c_int]
    strGeometry = ctypes.create_string_buffer(32)
    ierr = self.lib.DoseGetGeometry( self.zHandle, strGeometry )
    return strGeometry.value.decode()

  def set_nuclearAttenMode( self, strNuclearAttenMode ):
    self.lib.DoseSetNuclearAttenMode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseSetNuclearAttenMode( self.zHandle, 
                                             strNuclearAttenMode.encode() )
  def get_nuclearAttenMode( self ):
    self.lib.DoseGetNuclearAttenMode.argtypes = [ctypes.c_int]
    strNuclearAttenMode = ctypes.create_string_buffer(32)
    ierr = self.lib.DoseGetNuclearAttenMode( self.zHandle, strNuclearAttenMode )
    return strNuclearAttenMode.value.decode()

  def set_withBrems( self, iVerdict=1 ):
    self.lib.DoseSetWithBrems.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.DoseSetWithBrems( self.zHandle, iVerdict )
  def get_withBrems( self ):
    self.lib.DoseGetWithBrems.argtypes = [ctypes.c_int]
    return self.lib.DoseGetWithBrems( self.zHandle )

  def set_linearInterp( self, iVerdict=1 ):
    self.lib.DoseSetLinearInterp.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.DoseSetLinearInterp( self.zHandle, iVerdict )
  def get_linearInterp( self ):
    self.lib.DoseGetLinearInterp.argtypes = [ctypes.c_int]
    return self.lib.DoseGetLinearInterp( self.zHandle )

  def computeFluxDoseRate( self, da3FluxData ):
    self.lib.DoseComputeFluxDoseRate.argtypes = [ctypes.c_int, array_3d_double,
                                                 ctypes.c_int, ctypes.c_int,
                                                 array_3d_double]
    iNumT,iNumE,iNum0 = da3FluxData.shape
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumD = self.get_numDepths( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    da3DoseRate = np.zeros( (iNumT,iNumD,1) )
    ierr = self.lib.DoseComputeFluxDoseRate( self.zHandle, da3FluxData,
                                             iNumT, iNumE,
                                             da3DoseRate )
    if ierr < 0:
      return ierr, np.zeros( (1,1,1) )
    return ierr, da3DoseRate

  def computeFluenceDose( self, da3FluenceData ):
    self.lib.DoseComputeFluenceDose.argtypes = [ctypes.c_int, array_3d_double,
                                                ctypes.c_int, ctypes.c_int,
                                                array_3d_double]
    iNumT,iNumE,iNum0 = da3FluenceData.shape
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumD = self.get_numDepths( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    da3DoseRate = np.zeros( (iNumT,iNumD,1) )
    ierr = self.lib.DoseComputeFluenceDose( self.zHandle, da3FluenceData,
                                            iNumT, iNumE,
                                            da3DoseRate )
    if ierr < 0:
      return ierr, np.zeros( (1,1,1) )
    return ierr, da3DoseRate

  def computeFluxDose( self, daFluxData ):
    self.lib.DoseComputeFluxDose.argtypes = [ctypes.c_int, array_1d_double,
                                             ctypes.c_int,
                                             array_1d_double]
    iNumE = len( daFluxData )
    iNumD = self.get_numDepths( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    daDoseRate = np.zeros( iNumD )
    ierr = self.lib.DoseComputeFluxDose( self.zHandle, daFluxData,
                                         iNumE,
                                         daDoseRate )
    if ierr < 0:
      return ierr, np.zeros( 1 )
    return ierr, daDoseRate

  def __del__( self ):
    self.lib.DoseShutDown.argtypes = [ctypes.c_int]
    self.lib.DoseShutDown( self.zHandle )
    self.zHandle = 0

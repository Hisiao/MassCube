#******************************************************************************
#$HeadURL$
#
# File: kernelModel.py
#
# Description: Python language routines to perform the dose calculations,
#   employing the underlying Dose Kernel model.
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
#  1.0          01/25/2018  Created 
#  
#SVNTag: $Id$
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
  
class DoseKernel:

  # kernel library may have multiple kernels available
  def __init__( self ):
    if os.environ['IRENE_SYS'] == 'linux':
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libkernelmodel.so')
    else:
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libkernelmodel_c.dll')
    self.zHandle = self.lib.DoseKStartUp()

  def set_kernelXmlPath( self, strXmlPath ):
    self.lib.DoseKSetKernelXmlPath.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseKSetKernelXmlPath( self.zHandle, strXmlPath.encode() )
  def get_kernelXmlPath( self ):
    self.lib.DoseKGetKernelXmlPath.argtypes = [ctypes.c_int]
    strKernelXmlPath = ctypes.create_string_buffer(256)
    ierr = self.lib.DoseKGetKernelXmlPath( self.zHandle, strKernelXmlPath )
    return strKernelXmlPath.value.decode()

  def set_kernelXmlFile( self, strXmlFile ):
    self.lib.DoseKSetKernelXmlFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseKSetKernelXmlFile( self.zHandle, strXmlFile.encode() )
  def get_kernelXmlFile( self ):
    self.lib.DoseKGetKernelXmlFile.argtypes = [ctypes.c_int]
    strKernelXmlFile = ctypes.create_string_buffer(256)
    ierr = self.lib.DoseKGetKernelXmlFile( self.zHandle, strKernelXmlFile )
    return strKernelXmlFile.value.decode()

  def test_badHandle( self ):
    self.lib.DoseKSetKernelXmlFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strXmlFile = "test_badHandle"
    return self.lib.DoseKSetKernelXmlFile( self.zHandle-5, strXmlFile.encode() )

  def set_species( self, strSpecies ):
    self.lib.DoseKSetSpecies.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseKSetSpecies( self.zHandle, strSpecies.encode() )
  def get_species( self ):
    self.lib.DoseKGetSpecies.argtypes = [ctypes.c_int]
    strSpecies = ctypes.create_string_buffer(256)
    ierr = self.lib.DoseKGetSpecies( self.zHandle, strSpecies )
    return strSpecies.value.decode()

  def set_energies( self, daEnergies, strEnergyUnits="MeV" ):
    self.lib.DoseKSetEnergies.argtypes = [ctypes.c_int, array_1d_double,
                                          ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseKSetEnergies( self.zHandle, daEnergies.astype(np.float64), len(daEnergies),
                                      strEnergyUnits.encode() )

  def get_numEnergies( self ):
    self.lib.DoseKGetNumEnergies.argtypes = [ctypes.c_int]
    return self.lib.DoseKGetNumEnergies( self.zHandle )

  def set_depths( self, daDepths, strDepthUnits="mm" ):
    self.lib.DoseKSetDepths.argtypes = [ctypes.c_int, array_1d_double,
                                        ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseKSetDepths( self.zHandle, daDepths.astype(np.float64), len(daDepths),
                                    strDepthUnits.encode() )
  def get_numDepths( self ):
    self.lib.DoseKGetNumDepths.argtypes = [ctypes.c_int]
    return self.lib.DoseKGetNumDepths( self.zHandle )

  def set_detector( self, strDetector ):
    self.lib.DoseKSetDetector.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseKSetDetector( self.zHandle, strDetector.encode() )
  def get_detector( self ):
    self.lib.DoseKGetDetector.argtypes = [ctypes.c_int]
    strDetector = ctypes.create_string_buffer(256)
    ierr = self.lib.DoseKGetDetector( self.zHandle, strDetector )
    return strDetector.value.decode()

  def set_geometry( self, strGeometry ):
    self.lib.DoseKSetGeometry.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseKSetGeometry( self.zHandle, strGeometry.encode() )
  def get_geometry( self ):
    self.lib.DoseKGetGeometry.argtypes = [ctypes.c_int]
    strGeometry = ctypes.create_string_buffer(256)
    ierr = self.lib.DoseKGetGeometry( self.zHandle, strGeometry )
    return strGeometry.value.decode()

  def set_nuclearAttenMode( self, strNuclearAttenMode ):
    self.lib.DoseKSetNuclearAttenMode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.DoseKSetNuclearAttenMode( self.zHandle, strNuclearAttenMode.encode() )
  def get_nuclearAttenMode( self ):
    self.lib.DoseKGetNuclearAttenMode.argtypes = [ctypes.c_int]
    strNuclearAttenMode = ctypes.create_string_buffer(256)
    ierr = self.lib.DoseKGetNuclearAttenMode( self.zHandle, strNuclearAttenMode )
    return strNuclearAttenMode.value.decode()

  def set_withBrems( self, iVerdict=1 ):
    self.lib.DoseKSetWithBrems.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.DoseKSetWithBrems( self.zHandle, iVerdict )
  def get_withBrems( self ):
    self.lib.DoseKGetWithBrems.argtypes = [ctypes.c_int]
    return self.lib.DoseKGetWithBrems( self.zHandle )

  def computeFluxDoseRate( self, da3FluxData ):
    self.lib.DoseKComputeFluxDoseRate.argtypes = [ctypes.c_int, array_3d_double,
                                                  ctypes.c_int, ctypes.c_int,
                                                  array_3d_double]
    iNumT,iNumE,iNum0 = da3FluxData.shape
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumD = self.get_numDepths( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    da3DoseRate = np.zeros( (iNumT,iNumD,1) )
    ierr = self.lib.DoseKComputeFluxDoseRate( self.zHandle, da3FluxData,
                                              iNumT, iNumE,
                                              da3DoseRate )
    if ierr < 0:
      return ierr, np.zeros( (1,1,1) )
    return ierr, da3DoseRate

  def computeFluenceDose( self, da3FluenceData ):
    self.lib.DoseKComputeFluenceDose.argtypes = [ctypes.c_int, array_3d_double,
                                                 ctypes.c_int, ctypes.c_int,
                                                 array_3d_double]
    iNumT,iNumE,iNum0 = da3FluenceData.shape
    if iNumT < 1: iNumT = 1 # error handled at C++ level
    iNumD = self.get_numDepths( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    da3DoseRate = np.zeros( (iNumT,iNumD,1) )
    ierr = self.lib.DoseKComputeFluenceDose( self.zHandle, da3FluenceData,
                                             iNumT, iNumE,
                                             da3DoseRate )
    if ierr < 0:
      return ierr, np.zeros( (1,1,1) )
    return ierr, da3DoseRate

  def computeFluxDose( self, daFluxData ):
    self.lib.DoseKComputeFluxDose.argtypes = [ctypes.c_int, array_1d_double,
                                              ctypes.c_int,
                                              array_1d_double]
    iNumE = len( daFluxData )
    iNumD = self.get_numDepths( )
    if iNumD < 1: iNumD = 1 # error handled at C++ level
    daDoseRate = np.zeros( iNumD )
    ierr = self.lib.DoseKComputeFluxDose( self.zHandle, daFluxData,
                                          iNumE,
                                          daDoseRate )
    if ierr < 0:
      return ierr, np.zeros( 1 )
    return ierr, daDoseRate

  def __del__( self ):
    self.lib.DoseKShutDown.argtypes = [ctypes.c_int]
    ierr = self.lib.DoseKShutDown( self.zHandle )
    self.zHandle = 0

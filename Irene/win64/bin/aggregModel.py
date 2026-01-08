#******************************************************************************
#$HeadURL$
#
# File: aggregModel.py
#
# Description: Python language routines to perform the collection of sets of data
#   for the calculation of statistical confidence levels of the data.
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
c_int_p = ctypes.POINTER(ctypes.c_int)

if 'IRENE_SYS' not in os.environ:
  sys.exit('Error: undefined IRENE environment variables - calling script requires "import irene_defs.py"')
  
class AggregModel:

  def __init__( self ):
    if os.environ['IRENE_SYS'] == 'linux':
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libaggregmodel.so')
    else:
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libaggregmodel_c.dll')
    self.zHandle = self.lib.AggregStartUp()

  def reset( self ):
    self.lib.AggregResetAgg.argtypes = [ctypes.c_int]
    return self.lib.AggregResetAgg( self.zHandle )

  # Set environment for ephemeris-associated set of pitch angles
  def addScenToAgg( self, daTimes, da3Data ):
    self.lib.AggregAddScenToAgg.argtypes = [ctypes.c_int, array_1d_double, 
      array_3d_double, ctypes.c_int, ctypes.c_int, ctypes.c_int ]
    if da3Data.shape[0] != len(daTimes):
      return -1
    return self.lib.AggregAddScenToAgg( self.zHandle, daTimes, da3Data, 
      da3Data.shape[0], da3Data.shape[1], da3Data.shape[2] )

  def get_aggDataDim( self ):
    self.lib.AggregGetAggDimensions.argtypes = [ctypes.c_int, 
                                                 c_int_p, c_int_p, c_int_p]
    iNum1p = ctypes.c_int()
    iNum2p = ctypes.c_int()
    iNum3p = ctypes.c_int()
    ierr = self.lib.AggregGetAggDimensions( self.zHandle, iNum1p, iNum2p, iNum3p )
    if ierr<0:
      return 0, 0, 0
    return iNum1p.value, iNum2p.value, iNum3p.value

  def get_numScenarios( self ):
    self.lib.AggregGetNumScenarios.argtypes = [ctypes.c_int]
    return self.lib.AggregGetNumScenarios( self.zHandle )

  def computeConfLevel( self, iPercent ):
    self.lib.AggregComputeConfLevel.argtypes = [ctypes.c_int, ctypes.c_int, 
      array_1d_double, array_3d_double]
    iNum1, iNum2, iNum3 = self.get_aggDataDim( ) # sizing errors handled in C++ level
    if iNum1 < 1:
      return -1, np.zeros(1), np.zeros((1,1,1))
    daTimes = np.zeros( iNum1 )
    da3clData = np.zeros( (iNum1, iNum2, iNum3) )  # Default data type is numpy.float64.
    ierr = self.lib.AggregComputeConfLevel( self.zHandle, iPercent, daTimes, da3clData )
    return ierr, daTimes, da3clData

  def computeMedian( self ):
    self.lib.AggregComputeMedian.argtypes = [ctypes.c_int, 
                                            array_1d_double, array_3d_double]
    return self.computeConfLevel( 50 )

  def set_scenarioQuery( self, iVerdict ):
    self.lib.AggregScenarioQuery.argtypes = [ctypes.c_int, ctypes.c_int]
    self.lib.AggregScenarioQuery( self.zHandle, iVerdict )

  def find_ConfLevelScenario( self, da3clData, iaScenarios ):
    self.lib.AggregFindConfLevelScenario.argtypes = [ctypes.c_int, array_3d_double, 
      ctypes.c_int, ctypes.c_int, ctypes.c_int, array_1d_int, ctypes.c_int, array_3d_int]
    iNum1, iNum2, iNum3 = self.get_aggDataDim( ) # sizing errors handled in C++ level
    if iNum1 < 1:
      return -1, np.zeros((1,1,1), dtype=np.int32)
    ia3clScenario = np.zeros( (iNum1, iNum2, iNum3), dtype=np.int32 )
    ierr = self.lib.AggregFindConfLevelScenario( self.zHandle, da3clData,
                                                 iNum1, iNum2, iNum3,
                                                 iaScenarios, len(iaScenarios),
                                                 ia3clScenario )
    if ierr < 0: ia3clScenario = np.zeros((1,1,1), dtype=np.int32)
    return ierr, ia3clScenario

  # --- this is NOT a confidence level!!! -- its meaning is indeterminate
  def computeMean( self ):
    self.lib.AggregComputeMean.argtypes = [ctypes.c_int, 
                                          array_1d_double, array_3d_double]
    iNum1, iNum2, iNum3 = self.get_aggDataDim( ) # sizing errors handled in C++ level
    if iNum1 < 1:
      return -1,np.zeros(1), np.zeros(1,1,1)
    daTimes = np.zeros( iNum1 )
    da3clData = np.zeros( (iNum1, iNum2, iNum3) )
    ierr = self.lib.AggregComputeMean( self.zHandle, daTimes, da3clData )
    return ierr, daTimes, da3clData

  def __del__( self ):
    self.lib.AggregShutDown.argtypes = [ctypes.c_int]
    self.lib.AggregShutDown( self.zHandle )
    self.zHandle = 0

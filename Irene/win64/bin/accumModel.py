#******************************************************************************
#$HeadURL$
#
# File: accumModel.py
#
# Description: Python language routines to perform the collection and management
#   of flux data values to be accumulated for the calculation of fluence
#   data values.
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

array_1d_int = npct.ndpointer(dtype=np.intc, ndim=1, flags='CONTIGUOUS')
array_1d_double = npct.ndpointer(dtype=np.double, ndim=1, flags='CONTIGUOUS')
array_2d_double = npct.ndpointer(dtype=np.double, ndim=2, flags='CONTIGUOUS')
array_3d_double = npct.ndpointer(dtype=np.double, ndim=3, flags='CONTIGUOUS')

c_double_p = ctypes.POINTER(ctypes.c_double)
c_int_p = ctypes.POINTER(ctypes.c_int)

if 'IRENE_SYS' not in os.environ:
  sys.exit('Error: undefined IRENE environment variables - calling script requires "import irene_defs.py"')
  
class AccumModel:

  def __init__( self ):
    if os.environ['IRENE_SYS'] == 'linux':
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libaccummodel.so')
    else:
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libaccummodel_c.dll')
    self.zHandle = self.lib.AccumStartUp()

  def set_interval( self, dIntervalDays ):
    self.lib.AccumSetTimeInterval.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AccumSetTimeInterval( self.zHandle, dIntervalDays )

  def set_intervalSec( self, dIntervalSecs ):
    self.lib.AccumSetTimeIntervalSec.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AccumSetTimeIntervalSec( self.zHandle, dIntervalSecs )

  def get_interval( self ):
    self.lib.AccumGetTimeInterval.argtypes = [ctypes.c_int, c_double_p]
    pdInterval = ctypes.c_double()
    ierr = self.lib.AccumGetTimeInterval( self.zHandle, ctypes.byref(pdInterval) )
    return pdInterval.value

  def set_increment( self, dIncrement ):
    self.lib.AccumSetTimeIncrement.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AccumSetTimeIncrement( self.zHandle, dIncrement )

  def get_increment( self ):
    self.lib.AccumGetTimeIncrement.argtypes = [ctypes.c_int, c_double_p]
    pdIncrement = ctypes.c_double()
    ierr = self.lib.AccumGetTimeIncrement( self.zHandle, ctypes.byref(pdIncrement) )
    return pdIncrement.value

#   def test_badHandle( self ):
#     self.lib.AccumSetBufferSize.argtypes = [ctypes.c_int, ctypes.c_int]
#     return self.lib.AccumSetBufferSize( self.zHandle-10, 960 )

  def loadBuffer( self, daTimes, da3Data ):
    self.lib.AccumLoadBuffer.argtypes = [ctypes.c_int, 
                                         array_1d_double, array_3d_double, 
                                         ctypes.c_int, ctypes.c_int, ctypes.c_int]
    return self.lib.AccumLoadBuffer( self.zHandle, daTimes, da3Data,
                                     da3Data.shape[0], da3Data.shape[1], da3Data.shape[2] ) 

  def addToBuffer( self, dTime, da2Data ):
    self.lib.AccumAddToBuffer.argtypes = [ctypes.c_int, 
                                          ctypes.c_double, array_2d_double, 
                                          ctypes.c_int, ctypes.c_int]
    return self.lib.AccumAddToBuffer( self.zHandle, dTime, da2Data,
                                      da2Data.shape[0], da2Data.shape[1] )

  def get_bufferDimensions( self ):
    self.lib.AccumGetBufferDim.argtypes = [ctypes.c_int, c_int_p, c_int_p, c_int_p]
    piDim1 = ctypes.c_int()
    piDim2 = ctypes.c_int()
    piDim3 = ctypes.c_int()
    self.lib.AccumGetBufferDim( self.zHandle, ctypes.byref(piDim1),
                                ctypes.byref(piDim2), ctypes.byref(piDim3) )
    return piDim1.value, piDim2.value, piDim3.value

  def clear_buffer( self ):
    self.lib.AccumClearBuffer.argtypes = [ctypes.c_int]
    return self.lib.AccumClearBuffer( self.zHandle )

  def computeFluence( self ):
    self.lib.AccumComputeFluence.argtypes = [ctypes.c_int, 
                                           array_1d_double, array_3d_double]
    iNum1, iNum2, iNum3 = self.get_bufferDimensions( )
    if ( min(iNum1, iNum2, iNum3) < 1 ):
      return -1, np.zeros(1), np.zeros((1,1,1))
    daFluenceTimes = np.zeros( iNum1 )
    da3Fluence = np.zeros( (iNum1, iNum2, iNum3) )
    iNum = self.lib.AccumComputeFluence( self.zHandle, daFluenceTimes, da3Fluence )
    return iNum, daFluenceTimes, da3Fluence
    
  def computeIntvFluence( self, iPartial=0 ):
    self.lib.AccumComputeIntvFluence.argtypes = [ctypes.c_int, 
                                               array_1d_double, array_3d_double, 
                                               array_1d_int, ctypes.c_int]
    iNum1, iNum2, iNum3 = self.get_bufferDimensions( )
    if ( min(iNum1, iNum2, iNum3)<1 ):
      return -1, np.zeros(1), np.zeros((1,1,1))
    daFluenceTimes = np.zeros( iNum1 )
    da3Fluence = np.zeros( (iNum1, iNum2, iNum3) )
    iaIndices = np.zeros( iNum1, dtype=np.intc )
    iNum = self.lib.AccumComputeIntvFluence( self.zHandle, daFluenceTimes, da3Fluence, 
                                             iaIndices, iPartial )
    if iNum<=0:
      return iNum, np.zeros(1), np.zeros( (1,1,1) ), np.zeros(1)
    if iNum<iNum1:
      daFluenceTimes = np.resize(daFluenceTimes, iNum)
      da3Fluence = np.resize(da3Fluence, (iNum,iNum2,iNum3))
      iaIndices = np.resize(iaIndices, iNum)
    return iNum, daFluenceTimes, da3Fluence, iaIndices

  def computeFullFluence( self, iFinal=0 ):
    self.lib.AccumComputeFullFluence.argtypes = [ctypes.c_int, array_1d_double, 
                                               array_3d_double, ctypes.c_int]
    iNum1, iNum2, iNum3 = self.get_bufferDimensions( )
    if ( min(iNum1, iNum2, iNum3)<1 ):
      return -1, np.zeros(1), np.zeros((1,1,1))
    daFluenceTimes = np.zeros( iNum1 )
    da3Fluence = np.zeros( (iNum1, iNum2, iNum3) )
    iaIndices = np.zeros( iNum1, dtype=np.intc )
    iNum = self.lib.AccumComputeFullFluence( self.zHandle, daFluenceTimes, da3Fluence, 
                                             iFinal )
    if iNum<=0:
      return iNum, np.zeros(1), np.zeros( (1,1,1) )
    if iNum<iNum1:
      daFluenceTimes = np.resize(daFluenceTimes, iNum)
      da3Fluence = np.resize(da3Fluence, (iNum,iNum2,iNum3))
      iaIndices = np.resize(iaIndices, iNum)
    return iNum, daFluenceTimes, da3Fluence

  def resetFluence( self ):
    self.lib.AccumResetFluence.argtypes = [ctypes.c_int]
    return self.lib.AccumResetFluence( self.zHandle )

  def resetIntvFluence( self ):
    self.lib.AccumResetIntvFluence.argtypes = [ctypes.c_int]
    return self.lib.AccumResetIntvFluence( self.zHandle )

  def resetFullFluence( self ):
    self.lib.AccumResetFullFluence.argtypes = [ctypes.c_int]
    return self.lib.AccumResetFullFluence( self.zHandle )

  def resetBoxcarFluence( self ):
    self.lib.AccumResetBoxcarFluence.argtypes = [ctypes.c_int]
    return self.lib.AccumResetBoxcarFluence( self.zHandle )

  def resetExponentialFlux( self ):
    self.lib.AccumResetExponentialFlux.argtypes = [ctypes.c_int]
    return self.lib.AccumResetExponentialFlux( self.zHandle )

  def get_fluenceStartTime( self ):
    self.lib.AccumGetFluenceStartTime.argtypes = [ctypes.c_int, c_double_p]
    pdStartTime = ctypes.c_double()
    self.lib.AccumGetFluenceStartTime( self.zHandle, ctypes.byref(pdStartTime) )
    return pdStartTime.value

  def get_IntvFluenceStartTime( self ):
    self.lib.AccumGetIntvFluenceStartTime.argtypes = [ctypes.c_int, c_double_p]
    pdStartTime = ctypes.c_double()
    self.lib.AccumGetIntvFluenceStartTime( self.zHandle, ctypes.byref(pdStartTime) )
    return pdStartTime.value

  def get_fullFluenceStartTime( self ):
    self.lib.AccumGetFullFluenceStartTime.argtypes = [ctypes.c_int, c_double_p]
    pdStartTime = ctypes.c_double()
    self.lib.AccumGetFullFluenceStartTime( self.zHandle, ctypes.byref(pdStartTime) )
    return pdStartTime.value

  def get_boxcarFluenceStartTime( self ):
    self.lib.AccumGetBoxcarFluenceStartTime.argtypes = [ctypes.c_int, c_double_p]
    pdStartTime = ctypes.c_double()
    self.lib.AccumGetBoxcarFluenceStartTime( self.zHandle, ctypes.byref(pdStartTime) )
    return pdStartTime.value

  def get_lastLength( self ):
    self.lib.AccumGetLastLength.argtypes = [ctypes.c_int, c_double_p]
    pdLastLength = ctypes.c_double()
    self.lib.AccumGetLastLength( self.zHandle, ctypes.byref(pdLastLength) )
    return pdLastLength.value

  def accumIntvFluence( self, daIntvTimes, da3IntvFluence, iReset=0 ):
    self.lib.AccumAccumIntvFluence.argtypes = [ctypes.c_int, 
                                             array_1d_double, array_3d_double,
                                             ctypes.c_int, ctypes.c_int, ctypes.c_int, 
                                             array_3d_double, ctypes.c_int]
    iNumTimes,iNumValues,iNumDir = da3IntvFluence.shape
    da3IntvFluenceAccum = np.zeros( (iNumTimes,iNumValues,iNumDir) )
    iNum = self.lib.AccumAccumIntvFluence( self.zHandle, daIntvTimes, da3IntvFluence,
                                           iNumTimes, iNumValues, iNumDir,
                                           da3IntvFluenceAccum, iReset )
    return iNum, da3IntvFluenceAccum

  def computeAverageFlux( self, daFluenceTimes, da3Fluence, dIntervalSec=-999 ):
    self.lib.AccumComputeAverageFlux.argtypes = [ctypes.c_int, 
                                               array_1d_double, array_3d_double, 
                                               ctypes.c_double,
                                               ctypes.c_int, ctypes.c_int, ctypes.c_int, 
                                               array_3d_double]
    dIntervalSecAvg = dIntervalSec
    # use previously-set interval if not specified
    if dIntervalSecAvg == -999:
      dIntervalSecAvg = self.get_interval( )
    iNumTimes,iNumValues,iNumDir = da3Fluence.shape
    da3FluxAvg = np.zeros( (iNumTimes,iNumValues,iNumDir) )
    iNum = self.lib.AccumComputeAverageFlux( self.zHandle, daFluenceTimes, da3Fluence,
                                           dIntervalSecAvg,
                                           iNumTimes, iNumValues, iNumDir,
                                           da3FluxAvg )
    return iNum, da3FluxAvg

  def computeBoxcarFluence( self, iPartial=0 ):
    self.lib.AccumComputeBoxcarFluence.argtypes = [ctypes.c_int, 
                                                 array_1d_double, array_3d_double, 
                                                 array_1d_int, ctypes.c_int]
    iNum1, iNum2, iNum3 = self.get_bufferDimensions( )
    daBoxcarTimes = np.zeros( iNum1 )
    da3BoxcarFluence = np.zeros( (iNum1,iNum2,iNum3) )
    iaIndices = np.zeros( iNum1, dtype=np.intc )
    iNum = self.lib.AccumComputeBoxcarFluence( self.zHandle, 
                                               daBoxcarTimes, da3BoxcarFluence,
                                               iaIndices, iPartial )
    if iNum <= 0:
      return iNum, np.zeros(1), np.zeros((1,1,1)), np.zeros(1,dtype=np.intc)
    if iNum < iNum1:
      daBoxcarTimes = np.resize(daBoxcarTimes, iNum)
      da3BoxcarFluence = np.resize(da3BoxcarFluence, (iNum,iNum2,iNum3))
      iaIndices = np.resize(iaIndices, iNum)
    return iNum, daBoxcarTimes, da3BoxcarFluence, iaIndices

  def computeExponentialFlux( self, iFinal=0 ):
    self.lib.AccumComputeExponentialFlux.argtypes = [ctypes.c_int, 
                                                   array_1d_double, array_3d_double, 
                                                   array_1d_int, ctypes.c_int]
    iNum1, iNum2, iNum3 = self.get_bufferDimensions( )
    daExponTimes = np.zeros( iNum1 )
    da3ExponFlux = np.zeros( (iNum1,iNum2,iNum3) )
    iaIndices = np.zeros( iNum1, dtype=np.intc )
    iNum = self.lib.AccumComputeExponentialFlux( self.zHandle, daExponTimes, da3ExponFlux,
                                                 iaIndices, iFinal )
    if iNum <= 0:
      return iNum, np.zeros(1), np.zeros((1,1,1)), np.zeros(1,dtype=np.intc)
    if iNum < iNum1:
      daExponTimes = np.resize(daExponTimes, iNum) 
      da3ExponFlux = np.resize(da3ExponFlux, (iNum,iNum2,iNum3))
      iaIndices = np.resize(iaIndices, iNum)
    return iNum, daExponTimes, da3ExponFlux, iaIndices

  def applyWorstToDate( self, da3Data, da2MaxData, iReset=0 ):
    self.lib.AccumApplyWorstToDate.argtypes = [ctypes.c_int, array_3d_double,
                                             ctypes.c_int, ctypes.c_int, ctypes.c_int, 
                                             array_2d_double, array_3d_double, 
                                             ctypes.c_int]
    iNumTimes,iNumValues,iNumDir = da3Data.shape
    da3DataWorst = np.zeros( (iNumTimes,iNumValues,iNumDir) )
    ierr = self.lib.AccumApplyWorstToDate( self.zHandle, da3Data,
                                           iNumTimes, iNumValues, iNumDir,
                                           da2MaxData, da3DataWorst, iReset )
    return ierr, da3DataWorst, da2MaxData

  def __del__( self ):
    self.lib.AccumShutDown.argtypes = [ctypes.c_int]
    self.lib.AccumShutDown( self.zHandle )
    self.zHandle = 0

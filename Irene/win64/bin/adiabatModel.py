#******************************************************************************
#
# File: adiabatModel.py
#
# Description: Python language routines to perform the calculation of adiabatic
#   invariant values for the input time and spatial coordinate values.
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
  
class AdiabatModel:

  def __init__( self ):
    if os.environ['IRENE_SYS'] == 'linux':
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libadiabatmodel.so')
    else:
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libadiabatmodel_c.dll')
    self.zHandle = self.lib.AdiabatStartUp()

  def set_modelDBDir( self, strModelDBDir ):
    self.lib.AdiabatSetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AdiabatSetModelDBDir( self.zHandle, strModelDBDir.encode() )

  def get_modelDBDir( self ):
    self.lib.AdiabatGetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelDBDir = ctypes.create_string_buffer(256)
    ierr = self.lib.AdiabatGetModelDBDir( self.zHandle,strModelDBDir )
    return strModelDBDir.value.decode()

  def set_kPhiDBFile( self, strKPhiDB ):
    self.lib.AdiabatSetKPhiDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AdiabatSetKPhiDBFile( self.zHandle, strKPhiDB.encode() )

  def get_kPhiDBFile( self ):
    self.lib.AdiabatGetKPhiDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strFile = ctypes.create_string_buffer(256)
    ierr = self.lib.AdiabatGetKPhiDBFile( self.zHandle, strFile )
    return strFile.value.decode()

  def set_kHMinDBFile( self, strKHMinDB ):
    self.lib.AdiabatSetKHMinDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AdiabatSetKHMinDBFile( self.zHandle, strKHMinDB.encode() )

  def get_kHMinDBFile( self ):
    self.lib.AdiabatGetKHMinDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strFile = ctypes.create_string_buffer(256)
    ierr = self.lib.AdiabatGetKHMinDBFile( self.zHandle, strFile )
    return strFile.value.decode()

  def set_magfieldDBFile( self, strMagfieldDB ):
    self.lib.AdiabatSetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.AdiabatSetMagfieldDBFile( self.zHandle, strMagfieldDB.encode() )

  def get_magfieldDBFile( self ):
    self.lib.AdiabatGetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strFile = ctypes.create_string_buffer(256)
    ierr = self.lib.AdiabatGetMagfieldDBFile( self.zHandle, strFile )
    return strFile.value.decode()

  def set_kMin( self, dKMin=0.0 ):
    self.lib.AdiabatSetKMin.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AdiabatSetKMin( self.zHandle, dKMin )
  def get_kMin( self ):
    self.lib.AdiabatGetKMin.argtypes = [ctypes.c_int, c_double_p]
    pdKMin = ctypes.c_double()
    ierr = self.lib.AdiabatGetKMin( self.zHandle, ctypes.byref(pdKMin) )
    return pdKMin.value
  def set_kMax( self, dKMax=25.0 ):
    self.lib.AdiabatSetKMax.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AdiabatSetKMax( self.zHandle, dKMax )
  def get_kMax( self ):
    self.lib.AdiabatGetKMax.argtypes = [ctypes.c_int, c_double_p]
    pdKMax = ctypes.c_double()
    ierr = self.lib.AdiabatGetKMax( self.zHandle, ctypes.byref(pdKMax) )
    return pdKMax.value

  def set_hminMin( self, dHminMin=-1500.0 ):
    self.lib.AdiabatSetHMinMin.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AdiabatSetHMinMin( self.zHandle, dHminMin )
  def get_hminMin( self ):
    self.lib.AdiabatGetHMinMin.argtypes = [ctypes.c_int, c_double_p]
    pdHMinMin = ctypes.c_double()
    ierr = self.lib.AdiabatGetHMinMin( self.zHandle, ctypes.byref(pdHMinMin) )
    return pdHMinMin.value
  def set_hminMax( self, dHminMax=50000.0 ):
    self.lib.AdiabatSetHMinMax.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AdiabatSetHMinMax( self.zHandle, dHminMax )
  def get_hminMax( self ):
    self.lib.AdiabatGetHMinMax.argtypes = [ctypes.c_int, c_double_p]
    pdHMinMax = ctypes.c_double()
    ierr = self.lib.AdiabatGetHMinMax( self.zHandle, ctypes.byref(pdHMinMax) )
    return pdHMinMax.value

  def set_phiMin( self, dPhiMin=0.125 ):
    self.lib.AdiabatSetPhiMin.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AdiabatSetPhiMin( self.zHandle, dPhiMin )
  def get_phiMin( self ):
    self.lib.AdiabatGetPhiMin.argtypes = [ctypes.c_int, c_double_p]
    pdPhiMin = ctypes.c_double()
    ierr = self.lib.AdiabatGetPhiMin( self.zHandle, ctypes.byref(pdPhiMin) )
    return pdPhiMin.value
  def set_phiMax( self, dPhiMax=2.5 ):
    self.lib.AdiabatSetPhiMax.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AdiabatSetPhiMax( self.zHandle, dPhiMax )
  def get_phiMax( self ):
    self.lib.AdiabatGetPhiMax.argtypes = [ctypes.c_int, c_double_p]
    pdPhiMax = ctypes.c_double()
    ierr = self.lib.AdiabatGetPhiMax( self.zHandle, ctypes.byref(pdPhiMax) )
    return pdPhiMax.value

  def updateLimits( self ):
    self.lib.AdiabatUpdateLimits.argtypes = [ctypes.c_int]
    return self.lib.AdiabatUpdateLimits( self.zHandle )

   # date for magnetic moment value for Phi -> L* conversion; 0 means time varying
  def set_magMomentTime( self, dMagTimeMjd ):
    self.lib.AdiabatSetMagMomentTime.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.AdiabatSetMagMomentTime( self.zHandle, dMagTimeMjd  )
  def get_magMomentTime( self ):
    self.lib.AdiabatGetMagMomentTime.argtypes = [ctypes.c_int, c_double_p]
    pdMagMomentTime = ctypes.c_double()
    ierr = self.lib.AdiabatGetMagMomentTime( self.zHandle, ctypes.byref(pdMagMomentTime) )
    return pdMagMomentTime.value

  # multiple times, with fixed set of pitch angles.
  def computeCoordinateSet( self, strCoordSys, strCoordUnits, \
    daTimes, daCoord1, daCoord2, daCoord3, daPitchAngles ):
    self.lib.AdiabatComputeCoordinateSet.argtypes = [ctypes.c_int, \
      ctypes.c_char_p, ctypes.c_char_p, \
      array_1d_double, array_1d_double, array_1d_double, array_1d_double, ctypes.c_int, \
      array_1d_double, ctypes.c_int, array_2d_double, array_2d_double, array_2d_double, array_2d_double, \
      array_2d_double, array_2d_double, array_1d_double, array_1d_double, array_1d_double, \
      array_2d_double, array_2d_double ]
    iNumT = len( daTimes )
    iNumD = len( daPitchAngles )
    if iNumT<1 or iNumD<1 or daTimes.shape != daCoord1.shape or daCoord1.shape != daCoord2.shape \
        or daCoord2.shape != daCoord3.shape:
      return -1, np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                               np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                               np.zeros(1), np.zeros(1), np.zeros(1), \
                               np.zeros((1,1)), np.zeros((1,1))
    da2Alpha = np.zeros( (iNumT, iNumD) )  # Default data type is numpy.float64.
    da2Lm = np.zeros( (iNumT, iNumD) )
    da2K = np.zeros( (iNumT, iNumD) )
    da2Phi = np.zeros( (iNumT, iNumD) )
    da2Hmin = np.zeros( (iNumT, iNumD) )
    da2Lstar = np.zeros( (iNumT, iNumD) )
    daBmin = np.zeros( iNumT )
    daBlocal = np.zeros( iNumT )
    daMagLT = np.zeros( iNumT )
    da2B = np.zeros( (iNumT, 3) )
    da2I = np.zeros( (iNumT, iNumD) )
    ierr = self.lib.AdiabatComputeCoordinateSet( self.zHandle, \
      strCoordSys.encode(), strCoordUnits.encode(), \
      daTimes, daCoord1, daCoord2, daCoord3, \
      iNumT, daPitchAngles.astype(np.float64), iNumD, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
      daBmin, daBlocal, daMagLT, da2B, da2I )
    return ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
                 daBmin, daBlocal, daMagLT, da2B, da2I

  #  multiple times, with varying set of pitch angles.
  def computeCoordinateSetVarPitch( self, strCoordSys, strCoordUnits, \
    daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles ):
    self.lib.AdiabatComputeCoordinateSetVarPitch.argtypes = [ctypes.c_int, \
      ctypes.c_char_p, ctypes.c_char_p, \
      array_1d_double, array_1d_double, array_1d_double, array_1d_double, ctypes.c_int, \
      array_2d_double, ctypes.c_int, array_2d_double, array_2d_double, array_2d_double, array_2d_double, \
      array_2d_double, array_2d_double, array_1d_double, array_1d_double, array_1d_double, \
      array_2d_double, array_2d_double ]
    iNumT = len( daTimes )
    iNumD = da2PitchAngles.shape[1]
    if iNumT<1 or iNumD<1 or daTimes.shape != daCoord1.shape or daCoord1.shape != daCoord2.shape \
        or daCoord2.shape != daCoord3.shape:
      return -1, np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                               np.zeros((1,1)), np.zeros((1,1)), np.zeros((1,1)), \
                               np.zeros(1), np.zeros(1), np.zeros(1), \
                               np.zeros((1,1)), np.zeros((1,1))
    da2Alpha = np.zeros( (iNumT, iNumD) )  # Default data type is numpy.float64.
    da2Lm = np.zeros( (iNumT, iNumD) )
    da2K = np.zeros( (iNumT, iNumD) )
    da2Phi = np.zeros( (iNumT, iNumD) )
    da2Hmin = np.zeros( (iNumT, iNumD) )
    da2Lstar = np.zeros( (iNumT, iNumD) )
    daBmin = np.zeros( iNumT )
    daBlocal = np.zeros( iNumT )
    daMagLT = np.zeros( iNumT )
    da2B = np.zeros( (iNumT, 3) )
    da2I = np.zeros( (iNumT, iNumD) )
    ierr = self.lib.AdiabatComputeCoordinateSetVarPitch( self.zHandle, \
      strCoordSys.encode(), strCoordUnits.encode(), \
      daTimes, daCoord1, daCoord2, daCoord3, \
      iNumT, da2PitchAngles.astype(np.float64), iNumD, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
      daBmin, daBlocal, daMagLT, da2B, da2I )
    return ierr, da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
                 daBmin, daBlocal, daMagLT, da2B, da2I

  # determine pitch angles from direction vectors
  def calcDirPitchAngles( self, strCoordSys, strCoordUnits, \
    daTimes, daCoord1, daCoord2, daCoord3, da2Dir1, da2Dir2, da2Dir3 ):
    self.lib.AdiabatCalcDirPitchAngles.argtypes = [ctypes.c_int, \
      ctypes.c_char_p, ctypes.c_char_p, \
      array_1d_double, array_1d_double, array_1d_double, array_1d_double, \
      array_2d_double, array_2d_double, array_2d_double, ctypes.c_int, ctypes.c_int, \
      array_2d_double ]
    iNumT = len( daTimes )
    iNumD = da2Dir1.shape[1]
    if iNumT<1 or iNumD<1 or daTimes.shape != daCoord1.shape or daCoord1.shape != daCoord2.shape \
        or daCoord2.shape != daCoord3.shape:
      return -1, np.zeros((1,1))
    da2PitchAngles = np.zeros( (iNumT, iNumD) )
    ierr = self.lib.AdiabatCalcDirPitchAngles( self.zHandle, \
      strCoordSys.encode(), strCoordUnits.encode(), \
      daTimes, daCoord1, daCoord2, daCoord3, \
      da2Dir1.astype(np.float64), da2Dir2.astype(np.float64), da2Dir3.astype(np.float64), \
      iNumT, iNumD, da2PitchAngles )
    return ierr, da2PitchAngles

  def convertCoordsSingle( self, strCoordSysIn, strCoordUnitsIn, dTime, \
    dCoord1In, dCoord2In, dCoord3In, strCoordSysOut, strCoordUnitsOut ):
    self.lib.AdiabatConvertCoordinatesSingle.argtypes = [ctypes.c_int, \
      ctypes.c_char_p, ctypes.c_char_p, \
      ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, \
      ctypes.c_char_p, ctypes.c_char_p, c_double_p, c_double_p, c_double_p]
    dCoord1Out = ctypes.c_double()
    dCoord2Out = ctypes.c_double()
    dCoord3Out = ctypes.c_double()
    ierr = self.lib.AdiabatConvertCoordinatesSingle( self.zHandle, \
      strCoordSysIn.encode(), strCoordUnitsIn.encode(), \
      dTime, dCoord1In, dCoord2In, dCoord3In, \
      strCoordSysOut.encode(),strCoordUnitsOut.encode(), \
      ctypes.byref(dCoord1Out), ctypes.byref(dCoord2Out), ctypes.byref(dCoord3Out))
    return ierr, dCoord1Out.value, dCoord2Out.value, dCoord3Out.value

  def convertCoords( self, strCoordSysIn, strCoordUnitsIn, \
    daTimes, daCoord1In, daCoord2In, daCoord3In, strCoordSysOut, strCoordUnitsOut ):
    self.lib.AdiabatConvertCoordinates.argtypes = [ctypes.c_int, \
      ctypes.c_char_p, ctypes.c_char_p, \
      array_1d_double, array_1d_double, array_1d_double, array_1d_double, ctypes.c_int, \
      ctypes.c_char_p, ctypes.c_char_p, array_1d_double, array_1d_double, array_1d_double]
    iSize = len(daTimes)
    daCoord1Out = np.zeros( iSize )
    daCoord2Out = np.zeros( iSize )
    daCoord3Out = np.zeros( iSize )
    ierr = self.lib.AdiabatConvertCoordinates( self.zHandle, \
      strCoordSysIn.encode(), strCoordUnitsIn.encode(), \
      daTimes, daCoord1In, daCoord2In, daCoord3In, iSize, \
      strCoordSysOut.encode(),strCoordUnitsOut.encode(), \
      daCoord1Out, daCoord2Out, daCoord3Out )
    return ierr, daCoord1Out, daCoord2Out, daCoord3Out

  def __del__( self ):
    self.lib.AdiabatShutDown.argtypes = [ctypes.c_int]
    self.lib.AdiabatShutDown( self.zHandle )
    self.zHandle = 0

#******************************************************************************
#
# File: ephemModel.py
#
# Description: Python language routines to perform orbit propagation using
#   the underlying Kepler, SGP4 or SatEph (Lokangle) propagator models.
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
c_int_p = ctypes.POINTER(ctypes.c_int)

if 'IRENE_SYS' not in os.environ:
  sys.exit('Error: undefined IRENE environment variables - calling script requires "import irene_defs.py"')

class EphemModel:

  def __init__( self ):
    if os.environ['IRENE_SYS'] == 'linux':
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_LIB']+'/libephemmodel.so')
    else:
      self.lib = ctypes.cdll.LoadLibrary(os.environ['IRENE_BIN']+'/libephemmodel_c.dll')
    self.zHandle = self.lib.EphemStartUp()

  def set_modelDBDir( self, strModelDBDir ):
    self.lib.EphemSetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.EphemSetModelDBDir( self.zHandle, strModelDBDir.encode() )
  def get_modelDBDir( self ):
    self.lib.EphemGetModelDBDir.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strModelDBDir = ctypes.create_string_buffer(256)
    ierr = self.lib.EphemGetModelDBDir( self.zHandle,strModelDBDir )
    return strModelDBDir.value.decode()

  def set_magfieldDBFile( self, strMagfieldDBFile ):
    self.lib.EphemSetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.EphemSetMagfieldDBFile( self.zHandle, strMagfieldDBFile.encode() )
  def get_magfieldDBFile( self ):
    self.lib.EphemGetMagfieldDBFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strMagfieldDBFile = ctypes.create_string_buffer(256)
    ierr = self.lib.EphemGetMagfieldDBFile( self.zHandle, strMagfieldDBFile )
    return strMagfieldDBFile.value.decode()

  def set_prop( self, strProp ):
    self.lib.EphemSetPropagator.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.EphemSetPropagator( self.zHandle, strProp.encode() )
  def get_prop( self ):
    self.lib.EphemGetPropagator.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strProp = ctypes.create_string_buffer(32)
    ierr = self.lib.EphemGetPropagator( self.zHandle, strProp )
    return strProp.value.decode()

  def test_badHandle( self ):
    self.lib.EphemSetPropagator.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strProp = "SatEph"
    return self.lib.EphemSetPropagator( self.zHandle-5, strProp.encode() )

  def set_sgp4Param( self, strMode, strWgs ):
    self.lib.EphemSetSGP4param.argtypes = [ctypes.c_int,
                                           ctypes.c_char_p, ctypes.c_char_p]
    return self.lib.EphemSetSGP4param( self.zHandle, strMode.encode(), strWgs.encode() )
  def get_sgp4Mode( self ):
    self.lib.EphemGetSGP4Mode.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strMode = ctypes.create_string_buffer(32)
    ierr = self.lib.EphemGetSGP4Mode( self.zHandle, strMode )
    return strMode.value.decode()
  def get_sgp4Wgs( self ):
    self.lib.EphemGetSGP4WGS.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strWgs = ctypes.create_string_buffer(32)
    ierr = self.lib.EphemGetSGP4WGS( self.zHandle, strWgs )
    return strWgs.value.decode()

  def set_keplerUseJ2( self, iVerdict=1 ):
    self.lib.EphemSetKeplerUseJ2.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.EphemSetKeplerUseJ2( self.zHandle, iVerdict )
  def get_keplerUseJ2( self ):
    self.lib.EphemGetKeplerUseJ2.argtypes = [ctypes.c_int]
    return self.lib.EphemGetKeplerUseJ2( self.zHandle )

  def set_verbose( self, iVerbose=1 ):
    self.lib.EphemSetVerbose.argtypes = [ctypes.c_int, ctypes.c_bool]
    return self.lib.EphemSetVerbose( self.zHandle, iVerbose )

  def set_times( self, dStartTimeMjd, dEndTimeMjd, dStepSizeSec ):
    self.lib.EphemSetTimes.argtypes = [ctypes.c_int,
                                       ctypes.c_double, ctypes.c_double, ctypes.c_double]
    return self.lib.EphemSetTimes( self.zHandle, dStartTimeMjd, dEndTimeMjd, dStepSizeSec )

  def set_varTimes( self, dStartTimeMjd, dEndTimeMjd, 
                    dTimeMinStepSec, dTimeMaxStepSec=3600.0, dTimeRoundSec=5.0 ):
    self.lib.EphemSetVarTimes.argtypes = [ctypes.c_int,
                        ctypes.c_double, ctypes.c_double, ctypes.c_double,
                        ctypes.c_double, ctypes.c_double]
    return self.lib.EphemSetVarTimes( self.zHandle, dStartTimeMjd, dEndTimeMjd, 
                                      dTimeMinStepSec, dTimeMaxStepSec, dTimeRoundSec )

  def set_startTime( self, dStartTimeMjd ):
    self.lib.EphemSetStartTime.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetStartTime( self.zHandle, dStartTimeMjd )

  def set_endTime( self, dEndTimeMjd ):
    self.lib.EphemSetEndTime.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetEndTime( self.zHandle, dEndTimeMjd )

  def set_timeStep( self, dStepSizeSec ):
    self.lib.EphemSetTimeStep.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetTimeStep( self.zHandle, dStepSizeSec )

  def set_varTimeStep( self, dTimeMinStepSec, dTimeMaxStepSec=3600.0, dTimeRoundSec=5.0 ):
    self.lib.EphemSetVarTimeStep.argtypes = [ctypes.c_int, ctypes.c_double,
                                             ctypes.c_double, ctypes.c_double]
    return self.lib.EphemSetVarTimeStep( self.zHandle, dTimeMinStepSec, 
                                         dTimeMaxStepSec, dTimeRoundSec )

  def set_timesList( self, daTimesMjd ):
    self.lib.EphemSetTimesList.argtypes = [ctypes.c_int,
                                           array_1d_double, ctypes.c_int]
    return self.lib.EphemSetTimesList( self.zHandle, daTimesMjd, len(daTimesMjd) )

  def get_times( self ):
    self.lib.EphemGetTimes.argtypes = [ctypes.c_int, c_double_p, c_double_p, c_double_p]
    pdStartTime = ctypes.c_double()
    pdEndTime = ctypes.c_double()
    pdTimeStepSec = ctypes.c_double()
    ierr = self.lib.EphemGetTimes( self.zHandle, 
                                   ctypes.byref(pdStartTime), ctypes.byref(pdEndTime), 
                                   ctypes.byref(pdTimeStepSec) )
    return pdStartTime.value, pdEndTime.value, pdTimeStepSec.value

  def get_varTimes( self ):
    self.lib.EphemGetVarTimes.argtypes = [ctypes.c_int, c_double_p, c_double_p, 
                                          c_double_p, c_double_p, c_double_p]
    pdStartTime = ctypes.c_double()
    pdEndTime = ctypes.c_double()
    pdTimeMinStepSec = ctypes.c_double()
    pdTimeMaxStepSec = ctypes.c_double()
    pdTimeRoundSec = ctypes.c_double()
    ierr = self.lib.EphemGetVarTimes( self.zHandle, 
                                      ctypes.byref(pdStartTime),
                                      ctypes.byref(pdEndTime),
                                      ctypes.byref(pdTimeMinStepSec),
                                      ctypes.byref(pdTimeMaxStepSec),
                                      ctypes.byref(pdTimeRoundSec) )
    return pdStartTime.value, pdEndTime.value, \
           pdTimeMinStepSec.value, pdTimeMaxStepSec.value, pdTimeRoundSec.value

  def get_startTime( self ):
    self.lib.EphemGetStartTime.argtypes = [ctypes.c_int, c_double_p]
    pdStartTime = ctypes.c_double()
    ierr = self.lib.EphemGetStartTime( self.zHandle, ctypes.byref(pdStartTime) )
    return pdStartTime.value

  def get_endTime( self ):
    self.lib.EphemGetEndTime.argtypes = [ctypes.c_int, c_double_p]
    pdEndTime = ctypes.c_double()
    ierr = self.lib.EphemGetEndTime( self.zHandle, ctypes.byref(pdEndTime) )
    return pdEndTime.value

  def get_timeStep( self ):
    self.lib.EphemGetTimeStep.argtypes = [ctypes.c_int, c_double_p]
    pdTimeStep = ctypes.c_double()
    ierr = self.lib.EphemGetTimeStep( self.zHandle, ctypes.byref(pdTimeStep) )
    return pdTimeStep.value

  def get_varTimeStep( self ):
    self.lib.EphemGetVarTimeStep.argtypes = [ctypes.c_int,
                                             c_double_p, c_double_p, c_double_p]
    pdTimeMinStepSec = ctypes.c_double()
    pdTimeMaxStepSec = ctypes.c_double()
    pdTimeRoundSec = ctypes.c_double()
    ierr = self.lib.EphemGetVarTimeStep( self.zHandle, 
                                         ctypes.byref(pdTimeMinStepSec),
                                         ctypes.byref(pdTimeMaxStepSec),
                                         ctypes.byref(pdTimeRoundSec) )
    return pdTimeMinStepSec.value, pdTimeMaxStepSec.value, pdTimeRoundSec.value

  def get_numTimes( self ):
    return self.get_numTimesList( )

  def get_numTimesList( self ):
    self.lib.EphemGetNumTimesList.argtypes = [ctypes.c_int]
    return self.lib.EphemGetNumTimesList( self.zHandle )

  def get_timesList( self ):
    self.lib.EphemGetTimesList.argtypes = [ctypes.c_int,
                                           array_1d_double]
    iNumT = self.get_numTimesList( )
    if iNumT < 1:
      return iNumT, np.zeros(1)
    daTimesMjd = np.zeros( iNumT )  # Default data type is numpy.float64.
    iNumT = self.lib.EphemGetTimesList( self.zHandle, daTimesMjd )
    return iNumT, daTimesMjd

  def clear_timesList( self ):
    self.lib.EphemClearTimesList.argtypes = [ctypes.c_int]
    return self.lib.EphemClearTimesList( self.zHandle )

  def set_tleFile( self, strTleFile ):
    self.lib.EphemSetTLEFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.EphemSetTLEFile( self.zHandle, strTleFile.encode() )
  def get_tleFile( self ):
    self.lib.EphemGetTLEFile.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strFile = ctypes.create_string_buffer(256)
    ierr = self.lib.EphemGetTLEFile( self.zHandle, strFile )
    return strFile.value.decode()

  def set_tle( self, strLine1, strLine2 ):
    self.lib.EphemSetTLE.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p]
    return self.lib.EphemSetTLE( self.zHandle, strLine1.encode(), strLine2.encode() )

  def get_numTle( self ):
    self.lib.EphemGetNumTLE.argtypes = [ctypes.c_int]
    return self.lib.EphemGetNumTLE( self.zHandle )

  def get_tle( self, iEntry ):
    self.lib.EphemGetTLE.argtypes = [ctypes.c_int,
                                     ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p]
    strLine1 = ctypes.create_string_buffer(128)
    strLine2 = ctypes.create_string_buffer(128)
    ierr = self.lib.EphemGetTLE( self.zHandle, iEntry, strLine1, strLine2 )
    return ierr,strLine1.value.decode(), strLine2.value.decode()

  def reset_tleInputs( self ):
    self.lib.EphemResetTLE.argtypes = [ctypes.c_int]
    return self.lib.EphemResetTLE( self.zHandle )

  def set_elementTime( self, dElementTimeMjd ):
    self.lib.EphemSetElementTime.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetElementTime( self.zHandle, dElementTimeMjd )
  def get_elementTime( self ):
    self.lib.EphemGetElementTime.argtypes = [ctypes.c_int, c_double_p]
    pdElementTime = ctypes.c_double()
    ierr = self.lib.EphemGetElementTime( self.zHandle, ctypes.byref(pdElementTime) )
    return pdElementTime.value

  def set_inclination( self, dInclin ):
    self.lib.EphemSetInclination.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetInclination( self.zHandle, dInclin )
  def get_inclination( self ):
    self.lib.EphemGetInclination.argtypes = [ctypes.c_int, c_double_p]
    pdInclination = ctypes.c_double()
    ierr = self.lib.EphemGetInclination( self.zHandle, ctypes.byref(pdInclination) )
    return pdInclination.value

  def set_argOfPerigee( self, dArgOfPerigee ):
    self.lib.EphemSetArgOfPerigee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetArgOfPerigee( self.zHandle, dArgOfPerigee )
  def get_argOfPerigee( self ):
    self.lib.EphemGetArgOfPerigee.argtypes = [ctypes.c_int, c_double_p]
    pdArgPerigee = ctypes.c_double()
    ierr = self.lib.EphemGetArgOfPerigee( self.zHandle, ctypes.byref(pdArgPerigee) )
    return pdArgPerigee.value

  def set_meanAnomaly( self, dMeanAnom ):
    self.lib.EphemSetMeanAnomaly.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetMeanAnomaly( self.zHandle, dMeanAnom )
  def get_meanAnomaly( self ):
    self.lib.EphemGetMeanAnomaly.argtypes = [ctypes.c_int, c_double_p]
    pdMeanAnomaly = ctypes.c_double()
    ierr = self.lib.EphemGetMeanAnomaly( self.zHandle, ctypes.byref(pdMeanAnomaly) )
    return pdMeanAnomaly.value

  def set_eccentricity( self, dEccen ):
    self.lib.EphemSetEccentricity.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetEccentricity( self.zHandle, dEccen )
  def get_eccentricity( self ):
    self.lib.EphemGetEccentricity.argtypes = [ctypes.c_int, c_double_p]
    pdEccen = ctypes.c_double()
    ierr = self.lib.EphemGetEccentricity( self.zHandle, ctypes.byref(pdEccen) )
    return pdEccen.value

  def set_rightAscension( self, dRightAscen ):
    self.lib.EphemSetRightAscension.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetRightAscension ( self.zHandle, dRightAscen )
  def get_rightAscension( self ):
    self.lib.EphemGetRightAscension.argtypes = [ctypes.c_int, c_double_p]
    pdRightAscen = ctypes.c_double()
    ierr = self.lib.EphemGetRightAscension( self.zHandle, ctypes.byref(pdRightAscen) )
    return pdRightAscen.value

  def set_meanMotion( self, dMeanMot ):
    self.lib.EphemSetMeanMotion.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetMeanMotion( self.zHandle, dMeanMot )
  def get_meanMotion( self ):
    self.lib.EphemGetMeanMotion.argtypes = [ctypes.c_int, c_double_p]
    pdMeanMotion = ctypes.c_double()
    ierr = self.lib.EphemGetMeanMotion( self.zHandle, ctypes.byref(pdMeanMotion) )
    return pdMeanMotion.value

  def set_meanMotion1stDeriv( self, dMm1stDer ):
    self.lib.EphemSetMeanMotion1stDeriv.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetMeanMotion1stDeriv( self.zHandle, dMm1stDer )
  def get_meanMotion1stDeriv( self ):
    self.lib.EphemGetMeanMotion1stDeriv.argtypes = [ctypes.c_int, c_double_p]
    pdMm1stDer = ctypes.c_double()
    ierr = self.lib.EphemGetMeanMotion1stDeriv( self.zHandle, ctypes.byref(pdMm1stDer) )
    return pdMm1stDer.value

  def set_meanMotion2ndDeriv( self, dMm2ndDer ):
    self.lib.EphemSetMeanMotion2ndDeriv.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetMeanMotion2ndDeriv( self.zHandle,dMm2ndDer )
  def get_meanMotion2ndDeriv( self ):
    self.lib.EphemGetMeanMotion2ndDeriv.argtypes = [ctypes.c_int, c_double_p]
    pdMm2ndDer = ctypes.c_double()
    ierr = self.lib.EphemGetMeanMotion2ndDeriv( self.zHandle, ctypes.byref(pdMm2ndDer) )
    return pdMm2ndDer.value

  def set_bStar( self, dBStar ):
    self.lib.EphemSetBStar.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetBStar( self.zHandle, dBStar )
  def get_bStar( self ):
    self.lib.EphemGetBStar.argtypes = [ctypes.c_int, c_double_p]
    pdBStar = ctypes.c_double()
    ierr = self.lib.EphemGetBStar( self.zHandle, ctypes.byref(pdBStar) )
    return pdBStar.value

  def set_altitudeOfApogee( self, dApogeeAlt ):
    self.lib.EphemSetAltitudeOfApogee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetAltitudeOfApogee( self.zHandle, dApogeeAlt )
  def get_altitudeOfApogee( self ):
    self.lib.EphemGetAltitudeOfApogee.argtypes = [ctypes.c_int, c_double_p]
    pdApogeeAlt = ctypes.c_double()
    ierr = self.lib.EphemGetAltitudeOfApogee( self.zHandle, ctypes.byref(pdApogeeAlt) )
    return pdApogeeAlt.value

  def set_altitudeOfPerigee( self, dPerigeeAlt ):
    self.lib.EphemSetAltitudeOfPerigee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetAltitudeOfPerigee( self.zHandle, dPerigeeAlt )
  def get_altitudeOfPerigee( self ):
    self.lib.EphemGetAltitudeOfPerigee.argtypes = [ctypes.c_int, c_double_p]
    pdPerigeeAlt = ctypes.c_double()
    ierr = self.lib.EphemGetAltitudeOfPerigee( self.zHandle, ctypes.byref(pdPerigeeAlt) )
    return pdPerigeeAlt.value

  def set_localTimeOfApogee( self, dLtApogeeHr ):
    self.lib.EphemSetLocalTimeOfApogee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetLocalTimeOfApogee( self.zHandle, dLtApogeeHr )
  def get_localTimeOfApogee( self ):
    self.lib.EphemGetLocalTimeOfApogee.argtypes = [ctypes.c_int, c_double_p]
    pdLtApogeeHr = ctypes.c_double()
    ierr = self.lib.EphemGetLocalTimeOfApogee( self.zHandle, ctypes.byref(pdLtApogeeHr) )
    return pdLtApogeeHr.value

  def set_localTimeMaxInclination( self, dLtMaxInclHr ):
    self.lib.EphemSetLocalTimeMaxInclination.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetLocalTimeMaxInclination( self.zHandle, dLtMaxInclHr )
  def get_localTimeMaxInclination( self ):
    self.lib.EphemGetLocalTimeMaxInclination.argtypes = [ctypes.c_int, c_double_p]
    pdLtMaxInclHr = ctypes.c_double()
    ierr = self.lib.EphemGetLocalTimeMaxInclination( self.zHandle, ctypes.byref(pdLtMaxInclHr) )
    return pdLtMaxInclHr.value

  def set_timeOfPerigee( self, dTimeOfPerigeeMjd ):
    self.lib.EphemSetTimeOfPerigee.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetTimeOfPerigee( self.zHandle, dTimeOfPerigeeMjd )
  def get_timeOfPerigee( self ):
    self.lib.EphemGetTimeOfPerigee.argtypes = [ctypes.c_int, c_double_p]
    pdTimeOfPerigee = ctypes.c_double()
    ierr = self.lib.EphemGetTimeOfPerigee( self.zHandle, ctypes.byref(pdTimeOfPerigee) )
    return pdTimeOfPerigee.value

  def set_semiMajorAxis( self, dSemiMajorAxis ):
    self.lib.EphemSetSemiMajorAxis.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetSemiMajorAxis( self.zHandle, dSemiMajorAxis )
  def get_semiMajorAxis( self ):
    self.lib.EphemGetSemiMajorAxis.argtypes = [ctypes.c_int, c_double_p]
    pdSemiMajorAxis = ctypes.c_double()
    ierr = self.lib.EphemGetSemiMajorAxis( self.zHandle, ctypes.byref(pdSemiMajorAxis) )
    return pdSemiMajorAxis.value

  def set_positionGei( self, dPosX, dPosY, dPosZ ):
    self.lib.EphemSetPositionGEI.argtypes = [ctypes.c_int,
                              ctypes.c_double, ctypes.c_double, ctypes.c_double]
    return self.lib.EphemSetPositionGEI( self.zHandle, dPosX, dPosY, dPosZ )
  def get_positionGei( self ):
    self.lib.EphemGetPositionGEI.argtypes = [ctypes.c_int, c_double_p, c_double_p, c_double_p]
    pdPosX = ctypes.c_double()
    pdPosY = ctypes.c_double()
    pdPosZ = ctypes.c_double()
    ierr = self.lib.EphemGetPositionGEI( self.zHandle,
      ctypes.byref(pdPosX), ctypes.byref(pdPosY), ctypes.byref(pdPosZ) )
    return pdPosX.value, pdPosY.value, pdPosZ.value

  def set_velocityGei( self, dVelX, dVelY, dVelZ ):
    self.lib.EphemSetVelocityGEI.argtypes = [ctypes.c_int,
                              ctypes.c_double, ctypes.c_double, ctypes.c_double]
    return self.lib.EphemSetVelocityGEI( self.zHandle, dVelX, dVelY, dVelZ )
  def get_velocityGei( self ):
    self.lib.EphemGetVelocityGEI.argtypes = [ctypes.c_int, c_double_p, c_double_p, c_double_p]
    pdVelX = ctypes.c_double()
    pdVelY = ctypes.c_double()
    pdVelZ = ctypes.c_double()
    ierr = self.lib.EphemGetVelocityGEI( self.zHandle,
      ctypes.byref(pdVelX), ctypes.byref(pdVelY), ctypes.byref(pdVelZ) )
    return pdVelX.value, pdVelY.value, pdVelZ.value

  def set_stateVectors( self, daGeiPos, daGeiVel ):
    self.lib.EphemSetStateVectors.argtypes = [ctypes.c_int, array_1d_double, array_1d_double]
    return self.lib.EphemSetStateVectors( self.zHandle, daGeiPos, daGeiVel )
  def get_stateVectors( self ):
    self.lib.EphemGetStateVectors.argtypes = [ctypes.c_int, array_1d_double, array_1d_double]
    daStatePos = np.zeros( 3 )
    daStateVel = np.zeros( 3 )
    self.lib.EphemGetStateVectors( self.zHandle, daStatePos, daStateVel )
    return daStatePos, daStateVel

  def set_geosynchLon( self, dGeosynchLon ):
    self.lib.EphemSetGeosynchLon.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetGeosynchLon( self.zHandle, dGeosynchLon )
  def get_geosynchLon( self ):
    self.lib.EphemGetGeosynchLon.argtypes = [ctypes.c_int, c_double_p]
    pdGeosynchLon = ctypes.c_double()
    ierr = self.lib.EphemGetGeosynchLon( self.zHandle, ctypes.byref(pdGeosynchLon) )
    return pdGeosynchLon.value

  def reset_orbitParameters( self ):
    self.lib.EphemResetOrbitParameters.argtypes = [ctypes.c_int]
    return self.lib.EphemResetOrbitParameters( self.zHandle )

  def restart( self ):
    self.lib.EphemRestartEphemeris.argtypes = [ctypes.c_int]
    return self.lib.EphemRestartEphemeris( self.zHandle )

  def set_chunkSize( self, iChunkSize ):
    self.lib.EphemSetChunkSize.argtypes = [ctypes.c_int, ctypes.c_int]
    return self.lib.EphemSetChunkSize( self.zHandle, iChunkSize )
  def get_chunkSize( self ):
    self.lib.EphemGetChunkSize.argtypes = [ctypes.c_int]
    return self.lib.EphemGetChunkSize( self.zHandle )

  def computeGei( self ):
    self.lib.EphemComputeEphemerisGEI.argtypes = [ctypes.c_int, array_1d_double,
      array_1d_double, array_1d_double, array_1d_double,
      array_1d_double, array_1d_double, array_1d_double]
    iNum = self.get_numTimesList( )
    if iNum<1:
      print (' no ephemeris generation limits defined ' )
      return -1, np.zeros(1), np.zeros(1),np.zeros(1),np.zeros(1), np.zeros(1),np.zeros(1),np.zeros(1)
    iSize = self.get_chunkSize( )
    if iSize<1:
      iSize = iNum
    daTime = np.zeros( iSize )  # Default data type is numpy.float64.
    daXPos = np.zeros( iSize )
    daYPos = np.zeros( iSize )
    daZPos = np.zeros( iSize )
    daXVel = np.zeros( iSize )
    daYVel = np.zeros( iSize )
    daZVel = np.zeros( iSize )
    iNum = self.lib.EphemComputeEphemerisGEI( self.zHandle, daTime, daXPos, daYPos, daZPos,
                                                               daXVel, daYVel, daZVel )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1),np.zeros(1),np.zeros(1), np.zeros(1),np.zeros(1),np.zeros(1)
    if iNum < iSize:
      daTime = np.resize( daTime, iNum )
      daXPos = np.resize( daXPos, iNum )
      daYPos = np.resize( daYPos, iNum )
      daZPos = np.resize( daZPos, iNum )
      daXVel = np.resize( daXVel, iNum )
      daYVel = np.resize( daYVel, iNum )
      daZVel = np.resize( daZVel, iNum )
    return iNum, daTime, daXPos,daYPos,daZPos, daXVel,daYVel,daZVel

  def compute( self, strCoordSys, strCoordUnits ):
    self.lib.EphemComputeEphemeris.argtypes = [ctypes.c_int,
      ctypes.c_char_p, ctypes.c_char_p,
      array_1d_double, array_1d_double, array_1d_double, array_1d_double]
    iNum = self.get_numTimesList( )
    if iNum<1:
      print (' no ephemeris generation limits defined ' )
      return -1, np.zeros(1), np.zeros(1),np.zeros(1),np.zeros(1)
    iSize = self.get_chunkSize( )
    if iSize<1:
      iSize = iNum
    daTime = np.zeros( iSize )  # Default data type is numpy.float64.
    daCoord1 = np.zeros( iSize )
    daCoord2 = np.zeros( iSize )
    daCoord3 = np.zeros( iSize )
    iNum = self.lib.EphemComputeEphemeris( self.zHandle,
                                           strCoordSys.encode(), strCoordUnits.encode(),
                                           daTime, daCoord1, daCoord2, daCoord3 )
    if iNum < 1:
      return iNum, np.zeros(1), np.zeros(1),np.zeros(1),np.zeros(1)
    if iNum < iSize:
      daTime = np.resize( daTime, iNum )
      daCoord1 = np.resize( daCoord1, iNum )
      daCoord2 = np.resize( daCoord2, iNum )
      daCoord3 = np.resize( daCoord3, iNum )
    return iNum, daTime, daCoord1, daCoord2, daCoord3

  def convertCoordsSingle( self, strCoordSysIn, strCoordUnitsIn,
                           dTime, dCoord1In, dCoord2In, dCoord3In,
                           strCoordSysOut, strCoordUnitsOut):
    self.lib.EphemConvertCoordinatesSingle.argtypes = [ctypes.c_int,
      ctypes.c_char_p, ctypes.c_char_p,
      ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
      ctypes.c_char_p, ctypes.c_char_p, c_double_p, c_double_p, c_double_p]
    pdCoord1Out = ctypes.c_double()
    pdCoord2Out = ctypes.c_double()
    pdCoord3Out = ctypes.c_double()
    ierr = self.lib.EphemConvertCoordinatesSingle( self.zHandle,
                                   strCoordSysIn.encode(), strCoordUnitsIn.encode(),
                                   dTime, dCoord1In, dCoord2In, dCoord3In,
                                   strCoordSysOut.encode(),strCoordUnitsOut.encode(),
                                   ctypes.byref(pdCoord1Out),
                                   ctypes.byref(pdCoord2Out),
                                   ctypes.byref(pdCoord3Out) )
    return ierr, pdCoord1Out.value, pdCoord2Out.value, pdCoord3Out.value

  def convertCoords( self, strCoordSysIn, strCoordUnitsIn,
                     daTime, daCoord1In, daCoord2In, daCoord3In,
                     strCoordSysOut,strCoordUnitsOut ):
    iSize = len( daTime )
    self.lib.EphemConvertCoordinates.argtypes = [ctypes.c_int,
          ctypes.c_char_p, ctypes.c_char_p, array_1d_double,
          array_1d_double, array_1d_double, array_1d_double, ctypes.c_int,
          ctypes.c_char_p, ctypes.c_char_p,
          array_1d_double, array_1d_double, array_1d_double]
    daCoord1Out = np.zeros( iSize )
    daCoord2Out = np.zeros( iSize )
    daCoord3Out = np.zeros( iSize )
    ierr = self.lib.EphemConvertCoordinates( self.zHandle,
                 strCoordSysIn.encode(),strCoordUnitsIn.encode(),
                 daTime, daCoord1In, daCoord2In, daCoord3In, iSize,
                 strCoordSysOut.encode(), strCoordUnitsOut.encode(),
                 daCoord1Out, daCoord2Out, daCoord3Out )
    return ierr, daCoord1Out, daCoord2Out, daCoord3Out

  def set_mainField( self, strMainField ):
    self.lib.EphemSetMainField.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.EphemSetMainField( self.zHandle, strMainField.encode() )
  def set_externalField( self, strExternalField ):
    self.lib.EphemSetExternalField.argtypes = [ctypes.c_int, ctypes.c_char_p]
    return self.lib.EphemSetExternalField( self.zHandle, strExternalField.encode() )
  def get_mainField( self ):
    self.lib.EphemGetMainField.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strMainField = ctypes.create_string_buffer(16)
    ierr = self.lib.EphemGetMainField( self.zHandle,strMainField )
    return strMainField.value.decode()
  def get_externalField( self ):
    self.lib.EphemGetExternalField.argtypes = [ctypes.c_int, ctypes.c_char_p]
    strExternalField = ctypes.create_string_buffer(16)
    ierr = self.lib.EphemGetExternalField( self.zHandle,strExternalField )
    return strExternalField.value.decode()

  def update_time( self, dTimeMjd ):
    self.lib.EphemUpdateTime.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemUpdateTime( self.zHandle, dTimeMjd )

  def set_kpValue( self, dKpVal ):
    self.lib.EphemSetKpValue.argtypes = [ctypes.c_int, ctypes.c_double]
    return self.lib.EphemSetKpValue( self.zHandle, dKpVal )
  def set_kpValues( self, dRefTime, daKpVals ):
    self.lib.EphemSetKpValues.argtypes = [ctypes.c_int, ctypes.c_double, array_1d_double, ctypes.c_int]
    return self.lib.EphemSetKpValues( self.zHandle, dRefTime, daKpVals, len(daKpVals) )
  def get_kpValue( self ):
    self.lib.EphemGetKpValue.argtypes = [ctypes.c_int, c_double_p]
    pdKpVal = ctypes.c_double()
    ierr = self.lib.EphemGetKpValue( self.zHandle, ctypes.byref(pdKpVal) )
    return ierr, pdKpVal.value
  def get_kpValuesRefTime( self ):
    self.lib.EphemGetKpValuesRefTime.argtypes = [ctypes.c_int, c_double_p]
    pdRefTime = ctypes.c_double()
    ierr = self.lib.EphemGetKpValuesRefTime( self.zHandle, ctypes.byref(pdRefTime) )
    return ierr, pdRefTime.value
  def get_kpValuesEndTime( self ):
    self.lib.EphemGetKpValuesEndTime.argtypes = [ctypes.c_int, c_double_p]
    pdEndTime = ctypes.c_double()
    ierr = self.lib.EphemGetKpValuesEndTime( self.zHandle, ctypes.byref(pdEndTime) )
    return ierr, pdEndTime.value

  def compute_bFieldSingle( self, strCoordSys, strCoordUnits,
                            dTime, dCoord1, dCoord2, dCoord3 ):
    self.lib.EphemComputeBfieldSingle.argtypes = [ctypes.c_int,
      ctypes.c_char_p, ctypes.c_char_p,
      ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
      array_1d_double, c_double_p, c_double_p, c_double_p]
    daBVecGeo = np.zeros( 3 )
    pdBMag = ctypes.c_double()
    pdBMin = ctypes.c_double()
    pdLm = ctypes.c_double()
    ierr = self.lib.EphemComputeBfieldSingle( self.zHandle, 
      strCoordSys.encode(), strCoordUnits.encode(),
      dTime, dCoord1, dCoord2, dCoord3,
      daBVecGeo, ctypes.byref(pdBMag), ctypes.byref(pdBMin), ctypes.byref(pdLm) )
    return ierr, daBVecGeo, pdBMag.value, pdBMin.value, pdLm.value

  def compute_bField( self, strCoordSys, strCoordUnits,
                      daTime, daCoord1, daCoord2, daCoord3 ):
    iSize = len( daTime )
    self.lib.EphemComputeBfield.argtypes = [ctypes.c_int,
          ctypes.c_char_p, ctypes.c_char_p, array_1d_double,
          array_1d_double, array_1d_double, array_1d_double, ctypes.c_int,
          array_2d_double,
          array_1d_double, array_1d_double, array_1d_double]
    da2BVecGeo = np.zeros( (iSize, 3) )
    daBMag = np.zeros( iSize )
    daBMin = np.zeros( iSize )
    daLm = np.zeros( iSize )
    ierr = self.lib.EphemComputeBfield( self.zHandle,
                 strCoordSys.encode(),strCoordUnits.encode(),
                 daTime, daCoord1, daCoord2, daCoord3, iSize,
                 da2BVecGeo, daBMag, daBMin, daLm )
    return ierr, da2BVecGeo, daBMag, daBMin, daLm

  def compute_invariantsSingle( self, strCoordSys, strCoordUnits,
                                dTime, dCoord1, dCoord2, dCoord3,
                                daPitchAngles ):
    self.lib.EphemComputeInvariantsSingle.argtypes = [ctypes.c_int,
      ctypes.c_char_p, ctypes.c_char_p,
      ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
      array_1d_double, ctypes.c_int,
      c_double_p, array_1d_double, array_1d_double, array_1d_double, array_1d_double]
    iNumP = len( daPitchAngles )
    pdBMin = ctypes.c_double()
    daBMinPosGeo = np.zeros( 3 )
    daBVecGeo = np.zeros( 3 )
    daLm = np.zeros( iNumP )
    daI = np.zeros( iNumP )
    ierr = self.lib.EphemComputeInvariantsSingle( self.zHandle, 
      strCoordSys.encode(), strCoordUnits.encode(),
      dTime, dCoord1, dCoord2, dCoord3,
      daPitchAngles, iNumP,
      ctypes.byref(pdBMin), daBMinPosGeo, daBVecGeo, daLm, daI )
    return ierr, pdBMin.value, daBMinPosGeo, daBVecGeo, daLm, daI

  def compute_invariants( self, strCoordSys, strCoordUnits,
                          daTime, daCoord1, daCoord2, daCoord3,
                          daPitchAngles ):
    iSize = len( daTime )
    self.lib.EphemComputeInvariants.argtypes = [ctypes.c_int,
          ctypes.c_char_p, ctypes.c_char_p, array_1d_double,
          array_1d_double, array_1d_double, array_1d_double, ctypes.c_int,
          array_1d_double, ctypes.c_int, array_1d_double,
          array_2d_double, array_2d_double, array_2d_double, array_2d_double]
    iNumP = len( daPitchAngles )
    daBMin = np.zeros( iSize )
    da2BMinPosGeo = np.zeros( (iSize, 3) )
    da2BVecGeo = np.zeros( (iSize, 3) )
    da2Lm = np.zeros( (iSize, iNumP) )
    da2I = np.zeros( (iSize, iNumP) )
    ierr = self.lib.EphemComputeInvariants( self.zHandle,
                 strCoordSys.encode(),strCoordUnits.encode(),
                 daTime, daCoord1, daCoord2, daCoord3, iSize,
                 daPitchAngles, iNumP,
                 daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I )
    return ierr, daBMin, da2BMinPosGeo, da2BVecGeo, da2Lm, da2I

  def __del__( self ):
    self.lib.EphemShutDown.argtypes = [ctypes.c_int]
    self.lib.EphemShutDown( self.zHandle )
    self.zHandle = 0

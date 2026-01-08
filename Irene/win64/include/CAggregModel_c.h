/******************************************************************************
$HeadURL$

 File: CAggregModel_c.h

 Description: Declarations for methods for the collection of sets of data
   for the calculation of statistical confidence levels of the data.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2017 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution is unlimited.

   The AE9/AP9/SPM software license is contained in the 'documents/Licenses' 
   folder of this distribution file collection.

 Author:

   This software was developed by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:
 
   None

 Revision history:

  Version      Date        Notes
  1.0          10/24/2017  Created 
  
SVNTag: $Id$
******************************************************************************/

#ifndef AGGREGMODEL_C_H
#define AGGREGMODEL_C_H

#ifdef __cplusplus
#include "CAggregModel.h"

extern "C" {
#endif

typedef int HANDLE;

HANDLE AggregStartUp( );
int AggregShutDown( HANDLE zHandle );

int AggregSetWcLogging( HANDLE zHandle,
                        int iVerdict );

int AggregResetAgg( HANDLE zHandle );

int AggregAddScenToAgg( HANDLE zHandle,
                        double* pvdTimes,
                        double* pvvvdData,
                        int iNumTimes,
                        int iNumValues,
                        int iNumDir );

int AggregGetAggDimensions( HANDLE zHandle,
                            int* piNumTimes,
                            int* piNumValues,
                            int* piNumDir );
int AggregGetNumScenarios( HANDLE zHandle );

int AggregComputeConfLevel( HANDLE zHandle,
                            int iPercent,  // 0..100
                            double* pvdTimes,
                            double* pvvvdData );

int AggregComputeMedian( HANDLE zHandle,
                         double* pvdTimes,
                         double* pvvvdData );

int AggregSetScenarioQuery( HANDLE zHandle,
                            int iVerdict );

int AggregFindConfLevelScenario( HANDLE zHandle,
                                 double* pvvvdPercData,
                                 int iNumTimes,
                                 int iNumValues,
                                 int iNumDir,
                                 int* pviScenarios,
                                 int iNumScen,
                                 int* pvvviCLScenario );

 // --- this is NOT a confidence level!!! -- its meaning is indeterminate
int AggregComputeMean( HANDLE zHandle,
                       double* pvdTimes,
                       double* pvvvdData );

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // AGGREGMODEL_C_H

/******************************************************************************
$HeadURL$

 File: CAggregModel.h

 Description: Declarations for methods for the collection of sets of data
   for the calculation of statistical confidence levels of the data.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2021 Atmospheric and Environmental Research, Inc. (AER)

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
  1.0          09/18/2015  Created 
  
SVNTag: $Id$
******************************************************************************/

#ifndef AGGREGMODEL_H
#define AGGREGMODEL_H

#include <iostream>
#include <cmath>
#include "VectorTypes.h"

typedef std::vector<vvdvector> vvvdvector; // need 4d vector (energy,dir,time,scenario)

class AggregModel
{
  public:
    // constructors

    AggregModel();

    // destructor

    ~AggregModel();

    //
    // reset: clears any previously-collected data aggregation
    //
    void resetAgg();

    void setWcLogging( bool bState=true ) { m_bWcLogging = bState; }

    // duplicate method for enabling scenario query of CL, via findConfLevelScenario()
    void setScenarioQuery( bool bState=true ) { m_bWcLogging = bState; }

    //
    // add: inserts scenario data into a new or existing aggregation
    //
    int addScenToAgg( const dvector& vdScenTimes, 
                      const vvdvector& vvvdScenData );

    int getAggDimensions( int& iDim1, int& iDim2, int& iDim3 );
    int getAggNumScenarios( );

    //
    // computeConfLevel: calculates the value for the specified confidence level
    // (0-100) at each combination of timestep, energy level and direction.
    //
    int computeConfLevel( const int& iPercent, 
                          dvector& vdPercTimes, 
                          vvdvector& vvvdPercData );

    // methods for collecting logs the time,energy,direction,scenario and value + scen time
    //   from scenarios' 'worst case' events for each energy & direction.
    int findConfLevelWc( const vvdvector& vvvdPercData,
                         vdvector &vvdMaxData );
    int getConfLevelWcLog( const dvector &vdPercTimes,
                           const dvector &vdEnergies,
                           const vdvector &vvdPitchAngles,
                           const ivector &viScenarios,
                           vdvector &vvdWorstCaseLog );
    //
    // computeMedian: calculates the value for the 50th percentile 
    // at each combination of timestep, energy level and direction.
    //
    int computeMedian( dvector& vdPercTimes, 
                       vvdvector& vvvdPercData );

    //
    // Determines the scenario numbers from which the
    // confidence level values were just computed.
    int findConfLevelScenario( const vvdvector& vvvdPercData,
                               const ivector &viScenarios,
                               vvivector &vvviCLScenario );

    //
    // computeMean: calculates the mean of all the values
    // at each combination of timestep, energy level and direction.
    // --- this is NOT a confidence level!!! ---
    int computeMean( dvector& vdPercTimes, 
                     vvdvector& vvvdPercData );

  protected:

    // internal helper methods

    int sortInternal();

    // internal data members 

    bool        m_bSorted;      // sort once on first compute call, reset flag on add/reset
    bool        m_bWcLogging;   // activate optional Worst Case logging feature

    dvector m_vdWorstValueLog;
    vivector m_vviWorstIndicesLog;

    dvector     m_vdDateTimes;  // dates and times associated with input data

    vvvdvector  m_vvvvdData;    // 4d vector (energy,direction,time,scenario)

    vvvdvector  m_vvvvdDataUnsorted; // copy of m_vvvvdData, prior to sorting
                                     //  4d vector (energy,direction,time,scenario)

};


#endif

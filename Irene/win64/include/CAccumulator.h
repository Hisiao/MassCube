/******************************************************************************
$HeadURL$

 File: CAccumulator.h

 Description: Declarations for methods for the summation of data values
   over specified time intervals.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2015 Atmospheric and Environmental Research, Inc. (AER)

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

#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include "VectorTypes.h"
#include "CTimeValue.h"

class Accumulator
{
  public:
    Accumulator();
    virtual ~Accumulator();

    void setIntervalDuration ( const double& dTimeInterval, 
                               bool bUnitsDays=true );
    int setIntervalStart ( ) { return setIntervalStart(m_tTimeEnd.getModJulDate()); } // sets current end time as new start time
    int setIntervalStart ( const double& dTime,  // MJD
                           const double& dOffsetSec=0.0 );
    int setIntervalTimeStart ( ) { return setIntervalTimeStart(m_tTimeEnd); } // sets current end time as new start time
    int setIntervalTimeStart ( const CTimeValue& tTime,
                               const double& dOffsetSec=0.0 );

    double getIntervalDuration () { return m_dTimeInterval; }
    double getIntervalStart () { return m_tTimeStart.getModJulDate(); }
    double getIntervalEnd () { return m_tTimeEnd.getModJulDate(); }
    CTimeValue getIntervalTimeStart () { return m_tTimeStart; }
    CTimeValue getIntervalTimeEnd () { return m_tTimeEnd; }

    // get actual length of (partial) interval
    double getLastIntervalLength () { return (m_tTimePrev-m_tTimeStart); }

    int calcExpFlux ( const double& dTime, const vdvector& vvdData, vdvector& vvdExpFlux );

    int addData ( const double& dTime, const vdvector& vvdData );
    int getData ( double& dTime, vdvector& vvdData, bool bPartial=false );

    void clearData (); // clears accumulated data
    void resetData (); // + clears active interval
    void fullReset (); // + clears interval duration and accum mode(s)

  private:

    // accumulation 'carry-over' values
    // 2d = [energy][direction] 

    vdvector m_vvdExpFlux;  // exponentially-averaged flux accumulation
    vdvector m_vvdExpFluxNext;  // holding place for interval overflow of the same

    vdvector m_vvdFluence;  // time-weighted flux accumulation
    vdvector m_vvdFluenceNext;  // holding place for interval overflow of the same

    vdvector m_vvdFluxPrev; // previous flux values and time
    CTimeValue m_tTimePrev;     //   --needed for interpolation

    // time limits of accumulation period
    CTimeValue m_tTimeStart;
    CTimeValue m_tTimeEnd;

    double m_dTimeInterval; // units = seconds
    bool m_bCompleted;
};

#endif

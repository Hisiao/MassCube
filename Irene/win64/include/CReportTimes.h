/******************************************************************************
$HeadURL$

 File: CReportTimes.h

 Description: Declarations for methods for the management of time lists for the
    scheduled reporting of data values at specific intervals or custom times.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2019 Atmospheric and Environmental Research, Inc. (AER)

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
  1.0          10/01/2019  Created 
  
SVNTag: $Id$
******************************************************************************/

#ifndef CREPORTTIMES_H
#define CREPORTTIMES_H

#include "VectorTypes.h"

#define MJD_TIME_TOL  1.16e-8 

// internal class for defining single list of times
//   and associated set of tracking indices
class CTimeList {
  public:
    CTimeList():iNumTrack(1){viIndex.push_back(0);}
    ~CTimeList(){vdTimes.clear(); viIndex.clear();}
    dvector vdTimes;
    ivector viIndex;
    int iNumTrack;
};

class ReportTimes
{
  public:
    ReportTimes();
    virtual ~ReportTimes();
    // methods to specify and query data report times
    //   --- all times are in MJD form ----
    // define list of times based on reference time, increment and end time
    int setReportTimes( const double& dRefTime, // reference time is not in final list
                        const double& dIntvSeconds,
                        const double& dEndTime,
                        const int& iList = 0,
                        bool bAppend = false );
    // define list of times based on reference time, increment and count
    int setReportTimes( const double& dRefTime, // reference time is not in final list
                        const double& dIntvSeconds,
                        const int& iCount,
                        const int& iList = 0,
                        bool bAppend = false );
    // define list of times from input vector of time values
    int setReportTimes( const dvector& vdTimes, // all times copied as-is
                        const int& iList = 0,
                        bool bAppend = false );
    // insert vector of time values into (existing) chronogically-ordered list
    int insertReportTimes( const dvector& vdTimes, // all times inserted
                           const int& iList = 0 );
    // get number of entries in defined list of times
    int getNumReportTimes( const int& iList = 0 );
    // get full vector of defined list of times
    int getReportTimes( dvector& vdTimes,
                        const int& iList = 0 );
    // get last time in defined list
    double getLastReportTime( const int& iList = 0 );
    // get next time in defined list, after input time
    double getNextReportTime( const double& dInputTime = 0.0, 
                              const int& iTrack = 0,
                              const int& iList = 0 );
    // clear some or all of defined lists of times
    void clearReportTimes( const int& iListMin = 0 );
    // get number of defined lists of times
    int getNumTimeLists( ) { return m_iNumList; }
    // get number of 'tracking' indices maintained for defined list of times
    int getNumListTracks( const int& iList = 0 );

  private:
    vector<CTimeList> m_vTimeList;
    int m_iNumList;

    int addToOrderedList ( std::vector<double>& vdOrderedList, 
                           const double& dNewValue, 
                           const double& dMinValid=0.0, 
                           const double& dMaxValid=0.0 );
};


#endif


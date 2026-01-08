/******************************************************************************
$HeadURL$

 File: CTaskProcess.h

 Description: Declarations for methods for defining a set of attributes
   that describe a specific processing task.

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

#ifndef CTASKPROCESS_H
#define CTASKPROCESS_H

#include <iostream>
#include <string>
#include <vector>

#include "CTaskEnum.h"

struct SSegmentInfo {
  eState eStatus;
  int iProgress;
  int iNode;
  std::string strTaskMode;
};

class TaskProcess
  {
  public:

    TaskProcess ();
    ~TaskProcess ();

    eState getStatus () { return m_eStatus; }
    int getProgress () { return m_iProgress; }
    int getProgWeight () { return m_iProgWeight; }
    bool useBroadcast () { return m_bBroadcast; }

    std::string getExecutor () { return m_strTaskExecutor; }
    std::string getName () { return m_strTaskName; }
    std::string getMode ( const int iSegment ) 
      { return m_svSegmentInfo[iSegment].strTaskMode; }

    int getNumSegments () { return int(m_svSegmentInfo.size()); }
    int getNextSegment ();

    // define overall process attributes    
    void setAttributes ( const std::string& strTaskExecutor,
                         const std::string& strTaskName,
                         int (*pfLoadInputs)(std::string,int,int,int,int),
                         int (*pfErrorHandler)(std::string,int,int,int,int),
                         const int& iProgWeight,
                         bool bBroadcast );

    // set up identical subtasks, divided into specified number of segments
    int setSegments ( const std::string& strTaskMode, const int& iNumSegments );
    // set up differing subtasks, number of segments based on number of TaskMode strings
    int setSegments ( const std::vector<std::string>& strvTaskMode );

    // update SegmentInfo fields
    int setSegmentStatus ( const int& iSegment, const int& iProgress, const int& iNode = -1 );

    // wrappers to the respective task process function pointers
    int loadSegmentInputs ( int iProcess, int iSegment, int iNode );
    int handleSegmentErrors ( int iProcess, int iSegment, int iNode );

    // update overall process progress
    int updateProgressValue ();

  private:
 
    std::string m_strTaskExecutor;
    std::string m_strTaskName;
 
    eState m_eStatus;
    int m_iProgress;
    int m_iProgWeight;
 
    bool m_bBroadcast;
 
    int (*m_pfLoadInputs)(std::string,int,int,int,int);
    int (*m_pfErrorHandler)(std::string,int,int,int,int);
 
    int m_iNextSegment;
    std::vector<SSegmentInfo> m_svSegmentInfo;

};

#endif

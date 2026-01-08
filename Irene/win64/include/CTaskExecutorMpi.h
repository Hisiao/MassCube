/******************************************************************************
$HeadURL$

 File: CTaskExecutorMpi.h

 Description: Declarations for methods for a task processing unit to 
   communicate with the master scheduling process via MPI protocols.

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

#ifndef CTASKEXECUTOR_H
#define CTASKEXECUTOR_H

#include <cstdlib>
#include <iostream>
#include <string>

#ifdef _WIN32
#define OMPI_IMPORTS
#endif
#include <mpi.h>

class TaskExecutor
  {
  public:

    // constructors
    TaskExecutor ();

    // destructor
    ~TaskExecutor ();

    // methods to perform required MPI initialization
    int openComm ();
    int openComm (int argc, char** argv);

    void closeComm ();

    int recvDirective ( bool bBroadcast = true );

    // receive input parameters (strings and/or data block) from scheduler
    //  either as a broadcast (default), or a node-specific 'send' operation
    // [scheduler is expected to be broadcasting/sending these data]
    int recvStringFromComm ( std::string& strParamString,
                             bool bBroadcast = true );
    int recvDataBlockFromComm ( char *pVarPointer, 
                                const int iLength,
                                bool bBroadcast = true );

    // method to send progress value to scheduler
    //  [scheduler is expected to be in 'monitorExecutors' mode]
    int sendProgressToComm ( const int iProgress );

    // methods to use when tranmitting information back to scheduler
    //  (most likely use is for error reporting) 
    int sendDataBlockToComm ( char *pVarPointer, 
                              const int iLength );
    int sendStringToComm ( std::string strString );

    int getNodeId () { return m_iNodeId; }
    int getActiveNodes () { return m_iActiveNodes; }
    // ** warning: these are _not_necessarily_ the same as 
    // **   task segment #, and total # segments...
    // -- task segment number can transmitted from scheduler as
    //    part of input data, but would require NON-broadcast mode.

    // method for accessing time value from MPI utilities
    double getCommTime () { return MPI_Wtime(); }

  private:

    int m_iNodeId;       // identification number of process in spawned set
    int m_iActiveNodes;  // number of executor nodes in spawned set (siblings)

    MPI_Comm m_MPIschedComm; // internal MPI comm channel to Scheduler node

    bool m_bMPIactive;

};

#endif

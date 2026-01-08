/******************************************************************************
$HeadURL$

 File: CTaskScheduler.h

 Description: Declarations for methods for the scheduling and control of
   the necessary task processing units to perform a set of user-specified
   model calculations.  The 'Mpi' version uses multiple processing nodes,
   and communicates with these task processing units via MPI protocols.

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

#ifndef CTASKSCHEDULER_H
#define CTASKSCHEDULER_H

#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef MPI_MODE
#ifdef _WIN32
#define OMPI_IMPORTS
#endif
#include <mpi.h>
#endif
#include "CTaskProcess.h"

class TaskScheduler
  {
  public:

    // constructors
    TaskScheduler();

    // destructor
    ~TaskScheduler();

#ifdef MPI_MODE
    // perform required MPI initialization
    int openComm();
    int openComm( int argc, char** argv );

    // close down MPI communication, with built-in sleep period (default=2sec)
    void closeComm( int iSleep = 2 );
#else
    void setInputFile( const std::string& strInputFile )  { m_strInputFile  = strInputFile; }
    void setProcessDir( const std::string& strProcessDir ) { m_strProcessDir = strProcessDir; }
#endif

    // set (maximum) number of processing nodes to be used
    //  -- this does not include 'master' node 
    int setNumProcNodes( const int iNumProc );

    int getNumProcNodes() { return m_iProcNodes; }

    // define progress filename and initialize w/ '0' value
    int setProgressFile( const std::string strProgressFile );

    // define a task process to be performed;
    //  call this method multiple times to generate a list of task prcesses
    // use first form for uniform executor processes
    int defineTaskProcess( const std::string strTaskExecutor, // exec path/filename
                           const std::string strTaskName,     // task exec label
                           const std::string strTaskMode,     // task mode ident 
                           int (*pfLoadInputs)(std::string,int,int,int,int),
                           int (*pfErrorHandler)(std::string,int,int,int,int),
                           const int iProgWeight,
                           const int iSegmentLimit = -1,
                           bool bBroadcast = true );       // *** def = true

    // use second form for heterogenous executor processes
    int defineTaskProcess( const std::string strTaskExecutor, // exec path/filename
                           const std::string strTaskName,     // task exec label
                           const std::vector<std::string> strvTaskMode, // task mode ident
                           int (*pfLoadInputs)(std::string,int,int,int,int),
                           int (*pfErrorHandler)(std::string,int,int,int,int),
                           const int iProgWeight,
                           const int iSegmentLimit = -1,
                           bool bBroadcast = false );     // *** def = false

    // spawn a single instance of the named program, with argument list
    int executeProcess( const std::string &strExecName,
                        const std::vector<std::string> &vstrArgList,
                        bool bWaitForCompletion = true );

    // invoke the execution of the next task process in the list,
    //  then monitor its progress until full completion; adds delay at task end
    int executeNextTaskProcess( const int iTaskDelay = 1 );

#ifdef MPI_MODE
    // methods for transmitting parameters to executor processes;
    //  either all (default), or specific one in optional iNode
    // [executors are expected to be waiting to receive these data]
    int sendStringToComm( const std::string strString,
                          const int iNode = -1 );
    int sendDataBlockToComm( char *pVarPointer, 
                             const int iLength,
                             const int iNode = -1 );

    // methods for collecting information from specific executor process
    // [executor is expected to be sending these data]
    int recvDataBlockFromComm( const int iNode,
                               char *pVarPointer, 
                               const int iLength );
    int recvStringFromComm( const int iNode,
                            std::string& strString );

    // accesses time value from MPI utilities
    double getCommTime() { return MPI_Wtime(); }
#endif

    int getNumberTasks() { return int(m_ovTaskProcess.size()); }

  private:

    int m_iProcNodes;           // maximum number of *executor* nodes allowed
    int m_iActiveNodes;        // number of *executor* nodes spawned off

    int m_iWeightSum;          // sum of all process weight values  

    std::string m_strProgressFile;  // named ascii file, to write current progress
    int m_iPrevProgress;       // progress value last written to file

#ifdef MPI_MODE
    MPI_Comm m_MPIexecComm;    // internal MPI comm channel to executor nodes
#else
    std::string m_strInputFile;  // input file specification (in leiu of MPI transmittal)
    std::string m_strProcessDir;  // processing dir specification (in leiu of MPI transmittal)
#endif

    bool m_bMPIactive;

    int m_iTaskProcess;                       // index of currently active process
    std::vector<TaskProcess> m_ovTaskProcess; // vector of class objects containing
                                              //   task process information
    std::vector<int> m_ivNodeSegments; // vector to keep track of segment numbers
                                       //  on the active executor processing nodes

    int updateProgressFile( const int iProgress );
#ifdef MPI_MODE
    // monitor all executor processes until all task segments are completed
    //  -updates the named 'progress status' file at each update
    int monitorExecutors( const int iTaskDelay = 1 );

    // manage progress information
    int getAnyProgressUpdate( int& iProgress );
    int updateOverallProgressValue( );
       
    // send directives to executor processing nodes
    int sendStartDirective( bool bBroadcast );
    int sendStartDirective( const int iNode = -1 );
    int sendStopDirective ( bool bBroadcast );
    int sendStopDirective ( const int iNode = -1 );
    int sendDirective( const std::string strAction,
                       bool bBroadcast,
                        const int iNode );
#endif
};

#endif

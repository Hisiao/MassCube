/******************************************************************************
$HeadURL$

 File: CScheduler.h

 Description: Declarations for methods for the spawning and control of
   a set of task processing units.

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

#ifndef CSCHEDULER_H
#define CSCHEDULER_H

// define values for Windows MPI communication modes
//  --> order dependent for GUI radio buttons
#define WINMPI_UNDEF  -1
#define WINMPI_LOCAL   0
#define WINMPI_HYDRA   1
// deprecated by Intel:
#define WINMPI_SSH     2
// Windows MPI mode for 'mpiexec' launcher extra arguments:
//       Local  add '-localonly'      local Windows machine with multiple processors
//       Hydra  --no extra arg--      Windows cluster, relies on external 'hydra_service' 
//depr:  SSH    add '-bootstrap ssh'  Windows cluster, using ssh for MPI communication

#include "CInputParser.h"

class Scheduler: public InputParser
{
  public:
    Scheduler();
    virtual ~Scheduler();
    int schedule( const string& strInputFile, 
                  const string& strExecDir,
                  const int& iNumProcCmd,
                  bool bCmdLineApp=false,
                  int iWinMpiMode=WINMPI_LOCAL );
  protected:
    int getFileStatus( const string strFileName,
                       bool bDirCheck=false );
    int getNumProcessors( const int& iNumProcCmd, 
                          const int& iNumProcInput );
    int checkDirectory( const string& strDirName );
    int setProcessDir( const string& strWorkDir,
                       const string& strBaseFile, 
                       string& strProcessDir ); // in-out
    int removeDirectory( const string& strProcessDir );

};

#endif

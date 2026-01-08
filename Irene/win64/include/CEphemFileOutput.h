/******************************************************************************
$HeadURL$

 File: CEphemFileOutput.h

 Description: Implementation of methods for the generation of output files
   containing the ephemeris information calculated from user-specified
   orbit specifications.

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

#ifndef CEPHEMFILEOUTPUT_H
#define CEPHEMFILEOUTPUT_H

#include "CEphemModel.h"
#include "CDataFileOutput.h"


class EphemFileOutput : public EphemModel, public DataFileOutput
  {
  public:
    EphemFileOutput ();
    virtual ~EphemFileOutput ();

    int setEphemFile( const std::string& strFileName, 
                      bool bBinary = false, 
                      const int& iBinId = -1, 
                      bool bScenarioFile = false )
      { return setDataFile ( strFileName, bBinary, iBinId, bScenarioFile ); }
    int setEphemFile( ) { return setDataFile(); }
    int writeEphemFile ( );

    void setEphemDirSpecs ( const int& iNumDir=1, const int& iDirData=0 );

    int setupEphemFileHeader ( const std::string& strSourceFile = std::string(),
                               const std::string& strDirectionFile = std::string(),
                               const std::string& strPitchAngleFile = std::string() );
  private:

};

#endif

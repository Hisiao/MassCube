/******************************************************************************
$HeadURL$

 File: CAggregFileOutput.h

 Description: Declarations for methods for the generation of output files
   containing the requested confidence level, calculated from the aggregation
   of the respective data values from many existing 'scenario' output files.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2016 Atmospheric and Environmental Research, Inc. (AER)

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

#ifndef CAGGREGFILEOUTPUT_H
#define CAGGREGFILEOUTPUT_H

#include "CAsciiFileIO.h"
#include "CDataFileInput.h"
#include "CDataFileOutput.h"
#include "CDataCalcInfo.h"
#include "CAggregModel.h"

class AggregFileOutput : public DataFileOutput, public DataCalcInfo
{
  public:
    void setChunkSize( int iChunkSize ) { m_iChunkSize = iChunkSize; }
    AggregFileOutput();
    ~AggregFileOutput();
    int setAggregFileBase( const std::string& strDataFileBase,
                           bool bBinary=false,
                           int iBinId=-1 );
    int writeAggregFiles( const int& iType = -1 );
    int setupAggregFileHeader( std::vector<std::string>& strvHeaderInput,
                               structDataCalc& sDataCalc );


  private:
    int m_iChunkSize;
    int m_iFluxType;
    std::vector<double> m_vdDoseDepths;
    std::vector<double> m_vdEnergies;

    std::vector<int> m_viScenarios;

    string getLogFileName( const string &strFileName );
    int findScenarioWcLogTimes( const string &strScenFileBase,
                                vdvector &vvdWorstCaseLog );
    int setupAggregFileWcLoggingHeader( std::vector<std::string>& strvHeaderInput,
                                        structDataCalc& sDataCalc );
};


#endif


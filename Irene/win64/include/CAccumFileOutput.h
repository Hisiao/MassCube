/******************************************************************************
$HeadURL$

 File: CAccumFileOutput.h

 Description: Declarations for methods for the generation of output files
   containing the accumulated data for specified time intervals, calculated 
   from the time-based collection of the respective data values from existing
   flux output files.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2018 Atmospheric and Environmental Research, Inc. (AER)

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

#ifndef CACCUMFILEOUTPUT_H
#define CACCUMFILEOUTPUT_H

#include "CAsciiFileIO.h"
#include "CDataFileInput.h"
#include "CDataFileOutput.h"
#include "CDataCalcInfo.h"
#include "CReportTimes.h"
#include "CAccumModel.h"

class AccumFileOutput : public DataFileOutput, public DataCalcInfo, public ReportTimes
{
  public:
    void setChunkSize( int iChunkSize ) { m_iChunkSize = iChunkSize; }
    AccumFileOutput();
    ~AccumFileOutput();
    int setAccumFileBase( const std::string& strDataFileBase,
                          bool bBinary=false,
                          int iBinId=-1 );
    int writeAccumFiles( const int& iSegment=-1 );
    void collectAssociatedUseCases( structDataCalc& sFluxCalc,
                                    std::vector<structDataCalc>& svFluxAccum );
    int setupAccumFileHeader( );
    int setupAccumFileHeader( std::vector<std::string>& strvHeaderInput,
                              structDataCalc& sDataCalc );
    // special species specification for Cammice model only; needed for proper output file naming
    void setCammiceSpecies( const std::vector<std::string> vstrSpecies ) 
      { m_vstrSpecies = vstrSpecies; }

    void setReportSpec( const dvector& vdReportDef,
                        const dvector& vdReportDelta );
  private:
    int m_iChunkSize;
    int m_iFluxType;

    std::vector<double> m_vdEnergies;

    std::vector<std::string> m_vstrSpecies;

    dvector m_vdReportDef;
    dvector m_vdReportDelta;

    string getLogFileName( const string &strFileName );
    int setupAccumFileWcLoggingHeader( std::vector<std::string>& strvHeaderInput,
                                       structDataCalc& sDataCalc );
};


#endif


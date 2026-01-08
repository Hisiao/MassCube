/******************************************************************************
$HeadURL$

 File: CAsciiFileIO.h

 Description: Declarations for methods for the reading and writing of 
   data values from standardized ASCII format files.

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

#ifndef CASCIIFILEIO_H
#define CASCIIFILEIO_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "CFileIOSpec.h"
#include "CTimeValue.h"

class AsciiFileIO : public FileIOSpec
  {
  public:
    AsciiFileIO();
    virtual ~AsciiFileIO();

    int openRead( const std::string& strFileName );
    int openWrite( const std::string& strFileName, bool bAppend = false );
  
    int reopenRead( bool bAtBookMark = true );
    int reopenWrite( bool bAppend = true );

    int closeRead( bool bUpdateBookMark = true );
    int closeWrite( );

    bool isReadOpen() { return m_fsInFileStream.is_open(); }
    bool isWriteOpen() { return m_fsOutFileStream.is_open(); }

    std::string getFileNameRead() { return m_strInFileName; }
    std::string getFileNameWrite() { return m_strOutFileName; }

    void setFileNameRead( const std::string& strFileName ) { m_strInFileName = strFileName; }
    void setFileNameWrite( const std::string& strFileName ) { m_strOutFileName = strFileName; }

    bool isReadEof() { return m_fsInFileStream.eof(); }
    int rewindRead();

    int getFileReadPos() { return int(m_fsInFileStream.tellg()); }
    void setFileReadPos( const int& iPos );
    void setBookMark() { m_miInFileReadPos[m_strInFileName] = getFileReadPos (); }
    void toBookMark() { setFileReadPos ( m_miInFileReadPos[m_strInFileName] ); }

    int readLine( std::string& strLine );

    int splitLine( const char* szSource,
                   char cSplitOn,
                   std::vector<std::string>& vstrValues );
    int splitLine( const char* szSource,
                   std::vector<std::string>& vstrValues );

    int parseStringsToDoubles( const std::vector<std::string>& vstrValues,
                               std::vector<double>& vdValues,
                               const int& iExpect=0 );

    void trimWhitespace( std::string& strToken );

    int writeTimeCoordValues( double* pdTimeCoord,
                              bool bAddNewLine=false );
    int writeTimeValues( const CTimeValue& tTime);
    int writeCoordValues( const double& dCoord1,
                          const double& dCoord2,
                          const double& dCoord3,
                          bool bAddNewLine=false );
    int writeDirValues( double* pdDir,
                        bool bDirVector,
                        bool bAddNewLine=false );
    int writeAdiabatValues( double* pdAdiabat );
    int writeDataValues( double* pdData,
                         int iNumData );
    int writeLoggingValues( double* pdData,
                            int iNumData );

    int writeLine( std::string strLine );
    int writeLines( std::vector<std::string> vstrLines );

//?    std::fstream inFileStream() { return m_fsInFileStream; }
//?    std::fstream outFileStream() { return m_fsOutFileStream; }

    void transferHeaderSpecs( std::string& strLine );
    
    int getDataCount() { return m_iDataCount; }
    int getDirData() { return m_iDirData; }
    int getNumDir() { return m_iNumDir; }
    int getDataType() { return m_iDataType; }
    

  protected:

  // internal helper methods

    std::istream& safeGetLine(std::istream& inStream, std::string& strLine);

  // internal data members

    std::string m_strModel;
    std::string m_strVersion;

    std::string m_strInFileName;
    std::fstream m_fsInFileStream;
    std::string m_strOutFileName;
    std::fstream m_fsOutFileStream;

    // 'bookmark' for reopening file(s) for continued reading
    std::map<std::string,int> m_miInFileReadPos;

    // parameters used for binary-to-ascii conversion
    int m_iDataCount;  // total number of data fields (time is always single MJD value)
    int m_iDirData;    // number of data fields for direction information
    int m_iNumDir;     // number of directions
    int m_iDataType;   // 0=ephemeris,1=flux,2=dose,3=adiabat

    int openAscii ( const std::string& strFileName, bool bReadMode, bool bAppend = false );
    int writeTimeCoordsDescr();
    int writeTimeCoordsColumnHeader();

};

#endif

/******************************************************************************
$HeadURL$

 File: CDataFileOutput.h

 Description: Declarations for methods for the generation of standardized
   data output files.

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

#ifndef DATAFILEOUTPUT_H
#define DATAFILEOUTPUT_H

#include "CFileIOSpec.h"
#include "CAsciiFileIO.h"
#include "CBinFileIO.h"
#include "VectorTypes.h"

class DataFileOutput : public FileIOSpec
{
  public:
    DataFileOutput ();
    virtual ~DataFileOutput ();

    int setDataFile( const std::string& strFileName, 
                     bool bBinary = false, 
                     const int& iBinId = -1, 
                     bool bMultiFile = false );
    std::string getDataFile() { return m_strFileName; }
    int setDataFile();
    void changeDataFile( const std::string& strFileName );

    std::string getTimeDescr();
    std::string getCoordsDescr();
    std::string getDelimDescr();
    std::string getTimeCoordsLabels();
    void setHeaderLines( vector<string>& vstrLines )
      { m_strvHeader = vstrLines; }
    void setExtraInfo( vector<string>& vstrExtraInfo )
      { m_vstrExtraInfo = vstrExtraInfo; }
    int writeDataFileHeader();

    int getDataCount() { return m_iDataCount; } // total number of data fields (time is always _single_ MJD value)
    void setDataCount( const int& iDataCount ) { m_iDataCount = iDataCount; } 
    int getDirData()   { return m_iDirData; }   // number of data fields for direction information
    void setDirData( const int& iDirData ) { m_iDirData = iDirData; }
    int getNumDir()    { return m_iNumDir; }    // number of directions
    void setNumDir( const int& iNumDir ) { m_iNumDir = iNumDir; }
    int getNumData()   { return m_iNumData; }   // number of values in data record, excluding time, position and direction info
    void setNumData( const int& iNumData ) { m_iNumData = iNumData; }

    double* getDataBuffer();
    void setDataBuffer ( double* pdBuffer );
    
    void setProgressUpdate( int(pfProgressUpdate)(int) )
      { m_pfProgressUpdate = pfProgressUpdate; }

    int setOutputChunkSize( int iChunkSize );
    int writeDataChunk( const dvector&   vdTimes,
                        const dvector&   vdCoord1,
                        const dvector&   vdCoord2,
                        const dvector&   vdCoord3,
                        const vdvector&  vvdData );
    int writeDataChunk( const dvector&   vdTimes,
                        const dvector&   vdCoord1,
                        const dvector&   vdCoord2,
                        const dvector&   vdCoord3,
                        const vvdvector& vvvdData );
    int writeDataChunk( const dvector&   vdTimes,
                        const dvector&   vdCoord1,
                        const dvector&   vdCoord2,
                        const dvector&   vdCoord3,
                        const vdvector&  vvdPitchAngles,
                        const vvdvector& vvvdData );
    int writeDataChunk( const dvector&   vdTimes,
                        const dvector&   vdCoord1,
                        const dvector&   vdCoord2,
                        const dvector&   vdCoord3,
                        const vdvector&  vvdDirX,
                        const vdvector&  vvdDirY,
                        const vdvector&  vvdDirZ,
                        const vdvector&  vvdPitchAngles,
                        const vvdvector& vvvdData );
    int writeDataSegments( const dvector&   vdTimes,
                           const dvector&   vdCoord1,
                           const dvector&   vdCoord2,
                           const dvector&   vdCoord3,
                           const vdvector&  vvdDirX,
                           const vdvector&  vvdDirY,
                           const vdvector&  vvdDirZ,
                           const vdvector&  vvdPitchAngles,
                           const vvdvector& vvvdData );
    int writeAsciiDataChunk( const dvector& vdTimes, 
                             const dvector& vdCoord1, 
                             const dvector& vdCoord2, 
                             const dvector& vdCoord3, 
                             const vdvector& vvdDirX, 
                             const vdvector& vvdDirY, 
                             const vdvector& vvdDirZ, 
                             const vdvector& vvdPitchAngles, 
                             const vvdvector& vvvdData,
                             const int& iLen=0 );
    int writeBinaryDataChunk( const dvector& vdTimes, 
                              const dvector& vdCoord1, 
                              const dvector& vdCoord2, 
                              const dvector& vdCoord3, 
                              const vdvector& vvdDirX, 
                              const vdvector& vvdDirY, 
                              const vdvector& vvdDirZ, 
                              const vdvector& vvdPitchAngles, 
                              const vvdvector& vvvdData,
                              const int& iLen=0 );
    int writeAdiabatChunk( const dvector&   vdTimes,
                           const dvector&   vdCoord1,
                           const dvector&   vdCoord2,
                           const dvector&   vdCoord3,
                           const vdvector&  vvdPitchAngles,
                           const dvector&   vdBLocal,
                           const dvector&   vdBEquat,
                           const dvector&   vdMagLT,
                           const vdvector&  vvdB,
                           const vdvector&  vvdLm,
                           const vdvector&  vvdK,
                           const vdvector&  vvdPhi,
                           const vdvector&  vvdLstar,
                           const vdvector&  vvdHMin,
                           const vdvector&  vvdAlpha );
    int writeAdiabatChunk( const dvector&   vdTimes,
                           const dvector&   vdCoord1,
                           const dvector&   vdCoord2,
                           const dvector&   vdCoord3,
                           const vdvector&  vvdDirX, ///### order change!
                           const vdvector&  vvdDirY,
                           const vdvector&  vvdDirZ,
                           const vdvector&  vvdPitchAngles,
                           const dvector&   vdBLocal,
                           const dvector&   vdBEquat,
                           const dvector&   vdMagLT,
                           const vdvector&  vvdB,
                           const vdvector&  vvdLm,
                           const vdvector&  vvdK,
                           const vdvector&  vvdPhi,
                           const vdvector&  vvdLstar,
                           const vdvector&  vvdHMin,
                           const vdvector&  vvdAlpha );

    int writeLoggingChunk( const vdvector& vvdLogging );

  protected:
    int initDataBuffer();

    std::string m_strFileName;
    std::string m_strBinSuffix;
    std::string m_strHdrSuffix;
    
    AsciiFileIO m_asciiFile;
    BinFileIO m_binaryFile;
    bool m_bHeaderWritten;
    bool m_bBinaryData;
    bool m_bMultiFile;
    bool m_bAllocate;
    int m_iNumEntry;
    int m_iOutputChunkSize;
    int m_iEntry;
    int m_iEntrySize;
    double* m_pdBuffer;
    int m_iDataCount;
    int m_iDirData;
    int m_iNumDir;
    int m_iNumData;
    int m_iDataType;

    // header lines to write to beginning of output file
    vector<string> m_strvHeader;
    vector<string> m_vstrExtraInfo;

    // (optional) function pointer for progress updates
    int (*m_pfProgressUpdate)(int);
};

#endif

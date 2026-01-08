/******************************************************************************
$HeadURL$

 File: CBinFileIO.h

 Description: Declarations for methods for the reading and writing of 
   data values from standardized binary format files.

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

#ifndef CBINFILEIO_H
#define CBINFILEIO_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

class BinFileIO
{

  public:

    BinFileIO ();
    ~BinFileIO ();

    int openRead ( const std::string& strFileName );
    int openWrite ( const std::string& strFileName, bool bAppend = false );

    int reopenRead ( bool bAtBookMark = true );
    int reopenWrite ( bool bAppend = true );

    int closeRead ( bool bUpdateBookMark = true );
    int closeWrite ( );

    bool isReadOpen () { return m_fsInFileStream.is_open(); }
    bool isWriteOpen () { return m_fsOutFileStream.is_open(); }

    int getFileSizeRead () { return m_iInFileSize; }

    std::string getFileNameRead () { return m_strInFileName; }
    std::string getFileNameWrite () { return m_strOutFileName; }

    void setFileNameRead ( const std::string& strFileName ) { m_strInFileName = strFileName; }
    void setFileNameWrite ( const std::string& strFileName ) { m_strOutFileName = strFileName; }

    bool isReadEof () { return m_fsInFileStream.eof(); }
    int rewindRead ();

    int getFileReadPos () { return int(m_fsInFileStream.tellg()); }
    void setFileReadPos ( const int& iPos );
    void setBookMark () { m_miInFileReadPos[m_strInFileName] = getFileReadPos (); }
    void toBookMark () { setFileReadPos ( m_miInFileReadPos[m_strInFileName] ); }

    int readInString ( std::string& strString );
    int readInDataBlock ( char *pVarPointer, int iLength );

    int writeOutString ( const std::string& strString );
    int writeOutDataBlock ( char *pVarPointer, const int iLength );

    int copyFile ( const std::string& strDestFileName, 
                   const std::string& strSourceFileName,
                   const int& iBufferSize = 0 );
    int concatFiles ( const std::string& strFullFileName,
                      const std::vector<std::string>& strvPartFileNames,
                      bool bRemovePartFiles = true,
                      const int& iBufferSize = 0 );
    int concatFiles ( const std::string& strFileName,
                      const int iNumFiles,
                      bool bRemovePartFiles = true,
                      const int& iBufferSize = 0 );

    int convertToAscii ( const std::string& strFileName, 
                         const std::string& strProcessDir = "",
                         int iChunkSize = 960 );

  private:

    std::string m_strInFileName;
    std::fstream m_fsInFileStream;
    std::string m_strOutFileName;
    std::fstream m_fsOutFileStream;

    // 'bookmark' for reopening file(s) for continued reading
    std::map<std::string,int> m_miInFileReadPos;
    int m_iInFileSize;

    int openBinary ( const std::string& strFileName, bool bReadMode, bool bAppend = false );

};

#endif

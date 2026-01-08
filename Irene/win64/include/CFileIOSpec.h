/******************************************************************************
$HeadURL$

 File: CFileIOSpec.h

 Description: Declarations for methods for the specifying the location,
   naming, formatting of output files, and data forms and units of the 
   time and ccoordinate information.

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

#ifndef CFILEIOSPEC_H
#define CFILEIOSPEC_H

#include <string>
// magfield *enums* only!
#include "CMagfieldEnum.h"

typedef enum eTimeSpec
{
    eTimeSpecUndef = 0, //  int(eTimeSpec) = number of values
    eTimeSpecMjd         =1,  // Modified Julian Day.frac
    eTimeSpecYrDddFrac   =2,  // Year DDD.frac
    eTimeSpecYrDddGmt    =3,  // Year DDD Gmtsec
    eTimeSpecYrMoDaGmt   =4,  // Year Month Day Gmtsec
    eTimeSpecYrMoDaHrMnSc=6   // Year Month Day Hour Min Sec
} eTimeSpec;

typedef enum eDataDelimiter
{
    eDataDelimUndef = 0,  
    eDataDelimComma,      // ','
    eDataDelimSpace,      // ' ' on input, multiple spaces or tabs treated as single space
    eDataDelimTab,        // '\t'
} eDataDelimiter;


class FileIOSpec
{
public:
  FileIOSpec();
  virtual ~FileIOSpec() {}

  int setTimeSpec( std::string strTimeSpec );
  int setCoordSys( std::string strCoordSys );
  int setCoordSys( std::string strCoordSys, std::string strCoordUnits );
  int setCoordOrder( std::string strCoordOrder );
  int setDataDelim( std::string strDataDelim );

  void setTimeSpec( eTimeSpec eTimeSpecVal ) { m_eTimeSpec = eTimeSpecVal; }
  void setCoordSys( emfCoordSys eCoordSys ) { m_eCoordSys = eCoordSys; }
  void setStdCoordOrder( bool bStdCoordOrder ) { m_bStdCoordOrder = bStdCoordOrder; }
  void setDataDelim( const eDataDelimiter& eDataDelim );

  int checkCoordOrder ();
  eTimeSpec   getTimeSpec () { return m_eTimeSpec; }
  emfCoordSys getCoordSys () { return m_eCoordSys; }
  bool getStdCoordOrder () { return m_bStdCoordOrder; }
  eDataDelimiter getDataDelim () { return m_eDataDelim; }

  int setInTimeSpec( std::string strTimeSpec );
  int setInCoordSys( std::string strCoordSys );
  int setInCoordSys( std::string strCoordSys, std::string strCoordUnits );
  int setInCoordOrder( std::string strCoordOrder );
  int setInDataDelim( std::string strDataDelim );

  void setInTimeSpec( eTimeSpec eTimeSpecVal ) { m_eInTimeSpec = eTimeSpecVal; }
  void setInCoordSys( emfCoordSys eCoordSys ) { m_eInCoordSys = eCoordSys; }
  void setInStdCoordOrder( bool bStdCoordOrder ) { m_bInStdCoordOrder = bStdCoordOrder; }
  void setInDataDelim( const eDataDelimiter& eDataDelim );

  int checkInCoordOrder ();
  eTimeSpec   getInTimeSpec () { return m_eInTimeSpec; }
  emfCoordSys getInCoordSys () { return m_eInCoordSys; }
  bool getInStdCoordOrder () { return m_bInStdCoordOrder; }
  eDataDelimiter getInDataDelim () { return m_eInDataDelim; }

  int setProcessDir ( const std::string& strProcessDir );

  void setBinaryIdent ( const int& iBinId );
  std::string getBinaryFileName ( const std::string& strFileName, 
                                  bool bHeader=false );
  std::string getBinaryFileName ( const std::string& strFileName, 
                                  const int& iBinId, 
                                  bool bHeader=false );

protected:
  bool strMatchNum( const std::string& strOne, 
                    const std::string& strTwo, 
                    const int& iMaxNum );
  eTimeSpec   m_eTimeSpec;
  emfCoordSys m_eCoordSys;
  bool        m_bStdCoordOrder;
  eDataDelimiter m_eDataDelim;
  char        m_cDataDelim;

  eTimeSpec   m_eInTimeSpec;
  emfCoordSys m_eInCoordSys;
  bool        m_bInStdCoordOrder;
  eDataDelimiter m_eInDataDelim;
  char        m_cInDataDelim;

  std::string m_strProcessDir;
  std::string m_strBinTag;

};

#endif

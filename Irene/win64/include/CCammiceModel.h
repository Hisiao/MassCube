/******************************************************************************

 File: CCammiceModel.h

 Description: Declarations for methods for the calculation of plasma flux 
   values using the underlying CAMMICE/MICS model.

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
  
******************************************************************************/

#ifndef CAMMICEMODEL_H
#define CAMMICEMODEL_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include <CRoederPlasma.h>
#include "VectorTypes.h"
#include "DataBaseNames.h"

class CammiceModel
{
public:
  CammiceModel();
  ~CammiceModel();

  // Set methods for model input data

  int setModelDBDir( const string& strModelDBDir );
  string getModelDBDir( ) { return m_strModelDBDir; }
  int setModelDBFile( const string& strModelDBFile );
  int setMagfieldDBFile( const string& strMagfieldDBFile );
  string getModelDBFile( ) { return m_strModelDBFile; }
  string getMagfieldDBFile( ) { return m_strMagfieldDBFile; }

  int setMagfieldModel( const string& strMFModel ); // 'igrf' vs 'igrfop'
  string getMagfieldModel( );

  int setDataFilter( const string& strDataFilter );
  string getDataFilter( );

  int setPitchAngleBin( const string& strPitchAngleBin );
  string getPitchAngleBin( );

  int setSpecies( const string& strSpecies );
  string getSpecies( );

  void setCoordSys ( const emfCoordSys& eCoordSys ) 
    { m_eCoordSys = eCoordSys; }
  int setCoordSys ( const string& strCoordSys,
                    const string& strCoordUnits );
  string getCoordSys( );
  string getCoordSysUnits( );

  int setEphemeris( const dvector& vdTimes, 
                    const dvector& vdCoords1,
                    const dvector& vdCoords2, 
                    const dvector& vdCoords3 );
  int getNumEphemeris( ) { return int(m_vdTimes.size()); }
  int computeFlux( vdvector& vvdFluxData );

protected:

  // internal helper methods
  int updateModel();

  int expandEnvVarPath( const string& strEnvPath,
                        string& strExpandedPath,
                        const string& strKey );
  int checkFileExists( const string& strFileName );
  int checkDirExists( const string& strDirName );
  string appendDirDelim( const string strDirName );

  // internal data members

  CRoederPlasma        m_CRoederPlasma;

  string               m_strModelDBDir; // dir for automatic file naming
  string               m_strModelDBFile;
  string               m_strMagfieldDBFile;

  eRoederMagField      m_eCammiceMFModel;
  eRoederDataSelect    m_eCammiceDataFilter;
  eRoederPitchAngleBin m_eCammicePABin;
  eRoederSpecie        m_eCammiceSpecies;

  dvector              m_vdEnergies;

  bool                 m_bUpdateNeeded;
  CMagfield            *m_pMagfield;
  emfCoordSys          m_eCoordSys;

  dvector m_vdTimes; 
  dvector m_vdCoords1;
  dvector m_vdCoords2;
  dvector m_vdCoords3;
};


#endif

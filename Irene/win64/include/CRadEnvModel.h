/******************************************************************************

 File: CRadEnvModel.h

 Description: Declarations for methods for the calculation of flux values
   using the underlying AE8, AP8, CRRESELE or CRRESPRO models.

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

#ifndef RADENVMODEL_H
#define RADENVMODEL_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include <CRadEnvSatGrid.h>
#include "VectorTypes.h"
#include "DataBaseNames.h"

class RadEnvModel
{
public:
  RadEnvModel( );
  ~RadEnvModel( );


  // Set methods for model input data

  int setModel( const string& strModel ); // AE8 AP8 CRRESELE CRRESPRO
  string getModel( ) { return m_strModel; }
  int setModelDBDir( const string& strModelDBDir );
  string getModelDBDir( ) { return m_strModelDBDir; }
  int setModelDBFile( const string& strModelDBFile );
  string getModelDBFile( ) { return m_strModelDBFile; }
  int setMagfieldDBFile( const string& strMagfieldDBFile );
  string getMagfieldDBFile( ) { return m_strMagfieldDBFile; }
  int setShieldDose2DBFile( const string& strShieldDose2DBFile );
  string getShieldDose2DBFile( ) { return m_strShieldDose2DBFile; }
  int setActivityLevel( const string& strActivityLevel );
  string getActivityLevel( );
  int setActivityRange( const string& strActivityRange );
  string getActivityRange( );
  int set15DayAvgAp( double d15DayAvgAp );
  double get15DayAvgAp( ) { return m_d15DayAvgAp; }
  void setFixedEpoch( bool bFixedEpoch ) 
    { m_bFixedEpoch = bFixedEpoch; m_bUpdateNeeded = true;}
  bool getFixedEpoch( ) { return m_bFixedEpoch; }
  void setShiftSAA( bool bShiftSAA ) 
    { m_bShiftSAA = bShiftSAA; m_bUpdateNeeded = true;}
  bool getShiftSAA( ) { return m_bShiftSAA; }

  int setFluxType ( const string& strFluxType );
  string getFluxType( ) { return m_strFluxType; }
  int setEnergies( const dvector& vdEnergies );
  void getEnergies( dvector& vdEnergies )
    { vdEnergies = m_vdEnergies; }
  int getNumEnergies( ) { return int(m_vdEnergies.size()); }

  void setCoordSys( const emfCoordSys& eCoordSys ) // enumeration
    { m_eCoordSys = eCoordSys; }
  int setCoordSys( const string& strCoordSys,
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
  int updateModel( );

  int expandEnvVarPath( const string& strEnvPath,
                        string& strExpandedPath,
                        const string& strKey );
  int checkFileExists( const string& strFileName );
  int checkDirExists( const string& strDirName );
  string appendDirDelim( const string strDirName );

  int modelTagToEnums( const string& strModelTag, ereModel& eModel, ereSpecies& eSpecies, ereCompMode& eCompMode );

  int validateEnergyList( CRadEnvMgr& reMgr, ereModel eModel, ereSpecies eSpecies, dvector& vdEnergies );

  void setEnergyList( CRadEnvMgr& reMgr, ereModel eModel, ereSpecies eSpecies, dvector& vdEnergies );

  // internal data members

  CRadEnvSatGrid m_CRadEnvModelGrid;
  ereSpecies m_eSpecies;

  string             m_strModel;
  string             m_strModelDBDir; // dir for automatic file naming

  string             m_strModelDBFile;
  string             m_strMagfieldDBFile;
  string             m_strShieldDose2DBFile;
  string             m_strFluxType;   // integral or differential
  dvector                 m_vdEnergies;    // CRRESx: must be subset of fixed CRRES energies
  double                  m_d15DayAvgAp; // 15 day average Ap index, overrides activity range setting

  ereCrresProActivity     m_eCPActivity;// Solar activity level
  ereCrresRadActivity     m_eCRActivity;
  ereNasaActivity         m_eNasaActivity; //   = solar min/max for Ae8Ap8
  eApexRadActivityRange   m_eARActivityRange;
  eCrresEleActivityRange  m_eCEActivityRange;

  bool                    m_bFixedEpoch;
  bool                    m_bShiftSAA;

  bool                    m_bUpdateNeeded;
  bool                    m_bRadEnvInitialized;

  CMagfield               *m_pMagfield;
  emfCoordSys             m_eCoordSys;

  dvector m_vdTimes; 
  dvector m_vdCoords1;
  dvector m_vdCoords2;
  dvector m_vdCoords3;
};

#endif

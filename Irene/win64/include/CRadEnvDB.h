/***********************************************************************
 File: CRadEnvDB.h

 Description:

   Declarations for radiation belt model database management.

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution is unlimited.

 Author:

   This software was developed and/or modified by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:

   None

 Revision history:


***********************************************************************/
// CRadEnvDB - manages multiple instances of the underlying radiation
// environment databases through the CRadEnvDB class.
// *** Is not intended for multiple instantiation! ***

#include <map>
#include "GenericModelEnum.h"

#include "Hdf5ToolKit.h"

#include "CRadEnvEnums.h"

#ifndef CRADENVDB_MODEL_DEFINE
#define CRADENVDB_MODEL_DEFINE

const string strApexDose  = "ApexDose";
const string strCrresDose = "CrresDose";
const string strCrresEle  = "CrresEle";
const string strCrresPro  = "CrresPro";
const string strNasaEle   = "NasaEle";
const string strNasaPro   = "NasaPro";

const string rbDbNames[]={strApexDose,
			  strCrresDose,
			  strCrresEle,
			  strCrresPro,
			  strNasaEle,
			  strNasaPro };

// class definition
class CRadEnvDB {

public:
  CRadEnvDB();
  ~CRadEnvDB();

  // Initialize(): loads all data into internal map data structure (mDBs)
  eGENERIC_ERROR_CODE Initialize(const string &sDBFullPath);
  eGENERIC_ERROR_CODE Initialize(Hdf5ToolKit* pDb);

  eGENERIC_ERROR_CODE getData(const string sDataSource,
			      int ***pppiData,
			      int &iRows,
			      int &iRowLength);

  eGENERIC_ERROR_CODE getData(const string sDataSource,
			      float ***pppfData,
			      int &iRows,
			      int &iRowLength);

private:

  struct sDBstruct {
    int rows;
    int rowLength;
    float **cfa;
    int **cia;
  };

  eGENERIC_ERROR_CODE loadData(Hdf5ToolKit* pDb, const string &sDBName, sDBstruct& data);

  // Used to manage access to this object as a singleton
  static CRadEnvDB* m_pInstance;

  map<string, sDBstruct> mDBs;

};

#endif

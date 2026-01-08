/***********************************************************************
 File: CRadEnvModelBase.h

 Description:

   Declarations for lower-level radiation belt model methods.

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

#include "CRadEnvEnums.h"
#include "CRadEnvDB.h"
#include "GenericModel.h" // for vector typedefs and error codes only
#include <map>

#ifndef CRADENV_MODEL_BASE_DEFINE
#define CRADENV_MODEL_BASE_DEFINE

#define THIK_NOISE 0.0000001
#define ENRG_NOISE 0.0000001

// class definition
class CRadEnvModelBase
{
public:
  CRadEnvModelBase();
  virtual ~CRadEnvModelBase();

  virtual ereModel getModel() { return ermMODEL_UNKNOWN; }

  // Initialize(): loads all databases into map data structure (mDBs)
  eGENERIC_ERROR_CODE Initialize( CRadEnvDB* pDb );
  eGENERIC_ERROR_CODE Initialize( const string &sDBFullPath );

  // activity settings
  int getActivity() { return ceActivityLevel; }
  virtual string getActivityStr() = 0;
  virtual eGENERIC_ERROR_CODE setActivity(const int& ereActivity) = 0;
  virtual eGENERIC_ERROR_CODE setActivityStr(const string strActivity) = 0;
  virtual eGENERIC_ERROR_CODE getActivityBounds(fvector* pfvActivityBounds);
  virtual eGENERIC_ERROR_CODE getActivityRange(svector* psvActivityRange);

  // Energy list settings
  virtual eGENERIC_ERROR_CODE getDefaultEnergies(fvector* pfvEnergyList);
  int getNumDefaultEnergies() { return (int)(cfvAllEnergies.size()); }
  virtual eGENERIC_ERROR_CODE getCurrentEnergies(fvector* pfvEnergyList);
  virtual eGENERIC_ERROR_CODE setCurrentEnergies(const fvector& dvEnergies);
  virtual eGENERIC_ERROR_CODE resetCurrentEnergies();
  int getNumEnergies() { return (int)(cfvCurrentEnergies.size()); }

  // Shield thickness settings
  virtual eGENERIC_ERROR_CODE getDefaultThicknesses(fvector* pfvThicknessList);
  virtual eGENERIC_ERROR_CODE getDefaultThicknesses(dvector* pdvThicknessList);
  int getNumDefaultThicknesses() { return (int)(cfvAllThicknesses.size()); }
  virtual eGENERIC_ERROR_CODE setShieldThickness(const float& fThickness);
  virtual float getShieldThickness();

  // 15 day AP-based activity
  virtual eGENERIC_ERROR_CODE set15DayAP( const double& d15DayAP )
    { return eNotImplemented; }

  // dose channel
  virtual eGENERIC_ERROR_CODE setDoseChannel(const ereDoseChannel& ereDC)
  {
      return eNotImplemented;
  }

  // getFlux(): return flux values for one or more energies
  virtual eRADENV_ERROR_CODE getFlux(const double& dL,
				                             const double& dBBeq,
				                             fvector* pdvFlux);

  eRADENV_ERROR_CODE getFlux(const dvector& dvL,
                  			     const dvector& dvBBeq,
                             fvector* pdvFlux);

  virtual eRADENV_ERROR_CODE getIntegralFlux( const double& dL,
                                              const double& dBBeq,
                                              fvector* pdvFlux);

  eRADENV_ERROR_CODE getIntegralFlux( const dvector& dvL,
                                      const dvector& dvBBeq,
                                      fvector* pdvFlux);

  // getDose(): return dose values for one or more shield thicknesses
  virtual eRADENV_ERROR_CODE getDose(const double& dL,
				     const double& dBBeq,
				     float* pdDose);

  eRADENV_ERROR_CODE getDose(const dvector& dvL,
                             const dvector& dvBBeq,
			     fvector* pdvDose);

  // Message routines
  const string message(const eRADENV_ERROR_CODE& code);


protected:

  int thicknessIndex( const float& fThickness );
  int energyIndex( const float& fEnergy );
  double integrateFlux( const fvector& vfEnergyBinSizes,
                        const fvector& vfFluxes );

  string     csModelName;        // name uniquely identifying derived model

  CRadEnvDB* cpDb;               // database object holding persistant model data

  int        ceActivityLevel;    // maps to model specific enum value (level or range)

  fvector    cfvAllEnergies;     // list of energies for this model
  fvector    cfvCurrentEnergies;  // list of energies currently in use for this model

  fvector    cfvAllThicknesses;  // list of all shield thicknesses for this dose model
  float      cfCurrentThickness; // thickness in use for this dose model

  double     cd15DayAP;          // 15 day average AP

  // Data for mapping passed L and B values into database data arrays
  // for obtaining flux and dose data from the database.

  virtual eGENERIC_ERROR_CODE initLBSettings( int iNumBValues,
					      float fLMin,
					      float fLMax,
					      float fLDelta );

  int    findIndexOf( fvector& fvAscending, float fValue );

  int        ciNumBValues;
  int        ciNumLValues;

  fvector    cfvBCoords;       // B field values
  fvector    cfvLCoords;       // L values
  ivector    civLIndices;      // L indices into B field data
  ivector    civLBReferences;  // Refs to max B values for L radius

};

#endif

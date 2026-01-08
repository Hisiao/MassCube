/***********************************************************************
 File: CRadEnvMgr.h

 Description:

   Declarations for radiation belt model management.

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
// CRadEnvMgr - manages multiple instances of the underlying radiation
// environment models and data
//
// *** Is not intended for multiple instantiation! ***

#ifndef CRADENVMGR_DEFINE
#define CRADENVMGR_DEFINE

#include "GenericModel.h"
#include "CRadEnvEnums.h"
#include "CRadEnvModelBase.h"

//  radiation belt model identification
enum eModelID {
  emidApexRad,   // apex dose model
  emidCrresRad,  // crres dose model
  emidCrresEle,  // crres electron flux model
  emidCrresPro,  // crres proton flux model
  emidNasaEle,   // NASA electron flux model
  emidNasaPro    // NASA proton flux model
};


// class definition

class CRadEnvMgr:public GenericModel{

  // Declare construction, destruction private to force this to be a singleton class
private:

/*------------------------------------------------------------------*/
// function CRadEnvMgr::CRadEnvMgr
/**
*
* Default constructor
*
*/
  CRadEnvMgr();

/*------------------------------------------------------------------*/
// function CRadEnvMgr::~CRadEnvMgr
/**
*
* Destructor
*
*/
  ~CRadEnvMgr();

public:

/*------------------------------------------------------------------*/
// function CRadEnvMgr::Instance
  static CRadEnvMgr& Instance();  // The single instance of this object

  // Initialize(): loads all databases into map data structure (mDBs)

/*------------------------------------------------------------------*/
// function CRadEnvMgr::Initialize
/**
*
*  Loads database file for the radation environment models
*
*  @param sDBFullPath
*      string: database filename
*
*  @return int
*      0: success, else fail (see GenericModelEnum.h)
*/
  eGENERIC_ERROR_CODE Initialize( const string &sDBFullPath );

  // COM layer primarily must only instantiate, initialize dbs, and
  // set state.  However, 3 convenience gets are supported through COM
  // to return preset DB energy ranges, and 15-day Avg Ap range
  // limits:

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getCrresEnergyLists
/**
 *
 * get the full set of defined CRRES model energy lists
 *
 *  @param[out] pfvCEEL
 *      float vector pointer: vector of floats containing the electron energy levels
 *  @param[out] pfvCPEL
 *      float vector pointer: vector of floats containing the proton energy levels
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
  eGENERIC_ERROR_CODE getCrresEnergyLists( fvector* pfvCEEL,
                                           fvector* pfvCPEL );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getActivityRangeBounds
/**
 *
 * get the activity range bounds
 *
 *  @param[out] pfvApex
 *      float vector pointer: vector of floats containing the Apex activity range bounds
 *  @param[out] pfvCrresEle
 *      float vector pointer: vector of floats containing the CRRES activity range bounds
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
  eGENERIC_ERROR_CODE getActivityRangeBounds( fvector* pfvApex,
                                              fvector* pfvCrresEle );

  // CrresPro/Rad Activity Level Enumeration Sets/Gets
/*------------------------------------------------------------------*/
// function CRadEnvMgr::setCrresProActivity
/**
 *
 * Define CRRES Proton model activity level
 *
 *  @param ereCPA
 *      ereCrresProActivity: enum value for CRRESPRO activity level
 *      * ecpaQuiet
 *      * ecpaActive
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
  eGENERIC_ERROR_CODE setCrresProActivity( const ereCrresProActivity& ereCPA );


/*------------------------------------------------------------------*/
// function CRadEnvMgr::setCrresRadActivity
/**
 *
 * Define CRRES Rad model activity level
 *
 *  @param ereCRA
 *      ereCrresRadActivity: enum value for CRRES Rad activity level
 *      * ecraQuiet
 *      * ecraActive
 *      * ecraAverage
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
  eGENERIC_ERROR_CODE setCrresRadActivity( const ereCrresRadActivity& ereCRA );

  // note: this is hooked up to set both the NasaEle and NasaPro
  // models to ereNA.  ereNA can be: ecnaSolarMin or ecnaSolarMax
/*------------------------------------------------------------------*/
// function CRadEnvMgr::setNasaActivity
/**
 *
 * Define NASA electron or proton model activity level
 *
 *  @param ereNA
 *      ereNasaActivity: enum value for NASA activity level
 *      * ecnaSolarMin
 *      * ecnaSolarMax
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
  eGENERIC_ERROR_CODE setNasaActivity( const ereNasaActivity& ereNA );

  // ApexRad and CrresEle Activity Range sets/gets; the ranges are
  // kept in the corresponding enum class variables, and there is also
  // a string vector with the range names.
  // csvApexActivityRanges contains the vector list of activities
/*------------------------------------------------------------------*/
// function CRadEnvMgr::setApexActivityRange
/**
 *
 * Define Apex model activity range
 *
 *  @param earaIn
 *      eApexRadActivityRange: enum value for Apex activity range (based on 15 day Ap averages)
 *      * eara5to7_5=0
 *      * eara7_5to10
 *      * eara10to15
 *      * eara15to20
 *      * eara20to25
 *      * earaAll
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
  eGENERIC_ERROR_CODE setApexActivityRange( const eApexRadActivityRange& earaIn );

  // csvCrresEleActivityRanges contains the vector list of activities
/*------------------------------------------------------------------*/
// function CRadEnvMgr::setCrresEleActivityRange
/**
 *
 * Define CRRES ELE model activity range
 *
 *  @param eceaIn
 *      eCrresEleActivityRange: enum value for CRRES ele activity range (based on 15 day Ap averages)
 *      * ecear5to7_5=0
 *      * ecea7_5to10
 *      * ecea10to15
 *      * ecea15to20
 *      * ecea20to25
 *      * eceaGT25
 *      * eceaAvg
 *      * eceaMax
 *      * eceaAll
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
  eGENERIC_ERROR_CODE setCrresEleActivityRange( const eCrresEleActivityRange& eceaIn );

  // set15DayAp(): forces use of 15-day-Avg Ap to set both the Apex
  // and CrresEle activity level ranges (see implementation in .cpp)
/*------------------------------------------------------------------*/
// function CRadEnvMgr::set15DayAp
/**
 *
 * Forces use of 15-day Ap average for the Apex and/or CrresEle activity level ranges
 *
 *  @param dAp15
 *      double: 15-day Ap average value
 *
 *  @return int
 *      0: success, else fail (see GenericModelEnum.h)
 */
  eGENERIC_ERROR_CODE set15DayAp( const double& dAp15 );

//*****
//
// The paradigm is for there to be ONE and only ONE active energy
// list.  setEnergyIDs will set this energy list to be a subset of the
// Crres Energy list (either electrons or protons).  ereSpecies now
// contains two members: ermEle and ermPro.  Both the Crres and Nasa
// energy lists will be set to these values.  setNasaEnergies sets an
// arbitrary list of energies *and invalidates the use of the Crres
// model*.  getEnergies returns this uniform list.  Both
// setNasaEnergies and getEnergies take ereSpecies as an input
// parameter.  The low-level models still maintain their own lists,
// but single list uniformity is controlled at this level. -raq
// 2009-06-04
//
//*****

/*------------------------------------------------------------------*/
// function CRadEnvMgr::setEnergyIds
 /**
  * defines energies to use when searching the DB;
  * the first argument tells which of the internal vectors the ID
  * list is with respect too (ereCrresEleEnergies or
  * ereCrresProEnergies).  If this is *not* called before any
  * 'getFlux' call to one of the Crres flux DBs, fluxes for all
  * corresponding energies will be returned.  The Nasa model energies
  * *are* also set through this method when setting them to (some)
  * of the default Crres energies.
  *
  * @param ereSp
  *     ereSpecies: species enum
  * @param ivEIDS
  *     ivector: integer vector of energy IDs
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setEnergyIDs( const ereSpecies& ereSp,
                                    const ivector& ivEIDS );

/*----------------------------setNasaEnergies--------------------------------------*/
// function CRadEnvMgr::
  /**
  * directly setting the energies is only
  * supported for the NASA models, through this method
  *
  * @param ereSp
  *     ereSpecies: species enum
  * @param dvNE
  *     fvector: float vector of energies
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eRADENV_ERROR_CODE setNasaEnergies( const ereSpecies& ereSp,
                                      const fvector& dvNE );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::resetEnergies
  /**
  *
  * resets all Crres and NASA energy lists to their default state
  *
  * @return int
  *    0: success, or enumerated eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE resetEnergies();

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getEnergies
  /**
  * gets list of currently defined energies for model enum eModel
  *
  * @param eModel
  *     ereModel: model enum
  * @param pfvEnergies
  *     fvector*: pointer to float vector of energies
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE getEnergies( ereModel eModel,
                                   fvector* pfvEnergies );

  // Message routines
/*------------------------------------------------------------------*/
// function CRadEnvMgr::message
  /**
  * Return message for a given error code.
  *
  * @param code
  *     eRADENV_ERROR_CODE: error code enum
  *
  * @return
  *    string: associated message
  */
  const string message( const eRADENV_ERROR_CODE &code );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::message
  /**
  * Return error message from underlying class
  *
  * @return
  *    string: error message
  */
  const string message() { return(GenericModel::message()); }

  // **** NO DIRECT GETS ON DB THROUGH COM; that happens through the CRadEnvSatGrid!

  // ------------------------------------------------------ //
  // **** NO-COM Implementation for Methods below HERE **** //
  // ------------------------------------------------------ //

  // The following three will be set, and COM-wrapped, from CRadSatGrid:
  // Apex and CrresRad Dose and Dome settings

/*------------------------------------------------------------------*/
// function CRadEnvMgr::setDoseChannel
  /**
  * Set the dose channel to total, low LET, high LET.
  *
  * @param ereDC
  *     ereDoseChannel: dose channel enum
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setDoseChannel(const ereDoseChannel& ereDC);

  // Here, we keep with the paradigm of a single thickness set per db
  // access (or sequence of accesses.  We simply add a set method to
  // set the thickness directly in value mils.

  // Dome (aka shielding thickness) settings
/*------------------------------------------------------------------*/
// function CRadEnvMgr::setApexShieldThickID
  /**
  * Set the apex shield thickness ID.
  *
  * @param idAST
  *     size_t: shield thickness ID for the apex.
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setApexShieldThickID( const size_t idAST );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::setCrresRadShieldThickID
  /**
  * Set the CRRES radiation shield thickness ID.
  *
  * @param idAST
  *     size_t: shield thickness ID for the CRRES radiation shielding.
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setCrresRadShieldThickID( const size_t idCRST );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::setShieldThick
  /**
  * Set the shielding thickness for the specified radiation model.
  *
  * @param edmIn
  *     ereDoseModel: dose model enum to set shielding thickness for
  * @param dSt
  *     double: shielding thickness to set
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setShieldThick( const ereDoseModel& edmIn,
                                      const double& dSt );

  // ** gets for model state **
/*------------------------------------------------------------------*/
// function CRadEnvMgr::ereCrresProActivity
  /**
  * Get CRRES proton activity enum.
  *
  * @return
  *     ereCrresProActivity
  */
  ereCrresProActivity getCrresProActivity();

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getCrresRadActivity
  /**
  * Get CRRES radiation activity enum.
  *
  * @return
  *     ereCrresRadActivity
  */
  ereCrresRadActivity getCrresRadActivity();

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getNasaActivity
  /**
  * Get NASA activity enum.
  *
  * @return
  *     ereNasaActivity
  */
  ereNasaActivity getNasaActivity();

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getApexActivityRange
  /**
  * Get APEX activity range string.
  *
  * @return
  *     string
  */
  string getApexActivityRange();

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getCrresEleActivityRange
  /**
  * Get CRRES electron activity range string..
  *
  * @return
  *     ereNasaActivity
  */
  string getCrresEleActivityRange();

  // Return preset value lists
/*------------------------------------------------------------------*/
// function CRadEnvMgr::getCrresEleEnergyList
  /**
  * Return (in float vector pointer) CRRES electron energies.
  *
  * @param[out] pfvCEEL
  *     fvector pointer: ptr to float vector to hold electron energies
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE getCrresEleEnergyList( fvector* pfvCEEL );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getCrresProEnergyList
  /**
  * Return (in float vector pointer) CRRES proton energies.
  *
  * @param[out] pfvCPEL
  *     fvector pointer: ptr to float vector to hold proton energies
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE getCrresProEnergyList( fvector* pfvCPEL );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getApexShieldThickList
  /**
  * Return (in float vector pointer) APEX shielding thicknesses.
  *
  * @param[out] pfvASTL
  *     fvector pointer: ptr to float vector to hold shielding thicknesses.
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE getApexShieldThickList( fvector* pfvASTL );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getCrresRadShieldThickList
  /**
  * Return (in float vector pointer) CRRES shielding thicknesses.
  *
  * @param[out] pfvCRSTL
  *     fvector pointer: ptr to float vector to hold shielding thicknesses.
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE getCrresRadShieldThickList( fvector* pfvCRSTL );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getShieldThickLists
  /**
  * Return (in float vector pointer) CRRES shielding thicknesses.
  *
  * @param[out] pfvASTL
  *     fvector pointer: ptr to float vector to hold APEX shielding thicknesses.
  * @param[out] pfvCRSTL
  *     fvector pointer: ptr to float vector to hold CRES shielding thicknesses.
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE getShieldThickLists( fvector* pfvASTL,
                                           fvector* pfvCRSTL );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getActivityRanges
  /**
  * returns vector strings containing the range
  * names for the Apex and CrresEle activity ranges
  *
  * @param[out] psvApex
  *     svector pointer: ptr to string vector for APEX messages
  * @param[out] psvCrresEle
  *     svector pointer: ptr to string vector for CRRES electron messages
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE getActivityRanges( svector* psvApex,
                                         svector* psvCrresEle );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getDoseChannel
  /**
  * Get the current dose channel enum.
  *
  * @return
  *     ereDoseChannel
  */
  ereDoseChannel getDoseChannel();

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getApexShieldThick
  /**
  * Get the current APEX shield thickness.
  *
  * @return
  *     float
  */
  float getApexShieldThick();

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getCrresRadShieldThick
  /**
  * Get the current CRRES radiation shielding thickness.
  *
  * @return
  *     float
  */
  float getCrresRadShieldThick();

  // since this is DB interaction only, there are no computes, only
  // gets for returning the values

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getFlux
  /**
  * getFlux(): return flux values for one or more energies
  *
  * @param efmIn
  *     ereModel: model enum to get flux values for
  * @param dL
  *     double: L-value to get flux values for
  * @param dBBeq
  *     double: B-field strength at equator
  *
  * @param[out] pdvFlux
  *     fvector ptr: ptr to float vector to store flux values
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE getFlux( const ereModel& efmIn,
                              const double& dL,
                              const double& dBBeq,
                              fvector* pdvFlux );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getFlux
  /**
  * getFlux(): return flux values for one or more energies
  *
  * @param efmIn
  *     ereModel: model enum to get flux values for
  * @param dvL
  *     dvector: vector of L-values to get flux values for
  * @param dvBBeq
  *     dvector: vector of B-field strengths at equator
  *
  * @param[out] pdvFlux
  *     fvector ptr: ptr to float vector to store flux values
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE getFlux( const ereModel& efmIn,
                              const dvector& dvL,
                              const dvector& dvBBeq,
                              fvector* pdvFlux );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getIntegralFlux
  /**
  * getIntegralFlux(): return integral flux values for one or more energies
  *
  * @param efmIn
  *     ereModel: model enum to get flux values for
  * @param dL
  *     double: L-value to get flux values for
  * @param dBBeq
  *     double: B-field strength at equator
  *
  * @param[out] pdvFlux
  *     fvector ptr: ptr to float vector to store flux values
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE getIntegralFlux( const ereModel& efmIn,
                                      const double& dL,
                                      const double& dBBeq,
                                      fvector* pdvFlux );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getIntegralFlux
  /**
  * getIntegralFlux(): return flux values for one or more energies
  *
  * @param efmIn
  *     ereModel: model enum to get flux values for
  * @param dvL
  *     dvector: vector of L-values to get flux values for
  * @param dvBBeq
  *     dvector: vector of B-field strengths at equator
  *
  * @param[out] pdvFlux
  *     fvector ptr: ptr to float vector to store flux values
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE getIntegralFlux( const ereModel& efmIn,
                                      const dvector& dvL,
                                      const dvector& dvBBeq,
                                      fvector* pdvFlux );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getDose
  /**
  * getDose: return dose values for a given model, L-shell, and B-field.
  *
  * @param edmIn
  *     ereModel: model enum
  * @param dL
  *     double: L-shell value
  * @param dBBeq
  *     double: B-field strength at equator
  *
  * @param[out] pdDose
  *     float ptr: ptr to float to store doses
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE getDose( const ereModel& edmIn,
                              const double& dL,
                              const double& dBBeq,
                              float* pdDose );

/*------------------------------------------------------------------*/
// function CRadEnvMgr::getDose
  /**
  * getDose: return dose values for a given model, L-shell, and B-field.
  *
  * @param edmIn
  *     ereModel: model enum
  * @param dvL
  *     dvector: vector of L-shell values
  * @param dvBBeq
  *     dvector: vector of B-field strengths at equator
  *
  * @param[out] pdDose
  *     float ptr: ptr to float to store doses
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE getDose( const ereModel& edmIn,
                              const dvector& dvL,
                              const dvector& dvBBeq,
                              fvector* pdvFlux );


private:
  bool cbInitialized;

  // Used to manage access to this object as a singleton
  static CRadEnvMgr* m_pInstance;

  // Underlying HDF5 db wrapper that loads and manages access to
  // RadEnv data for all RadEnv models
  CRadEnvDB* cpREDb;

  // Individual RadEnv models
  CRadEnvModelBase* cpApexRadModel;
  CRadEnvModelBase* cpCrresRadModel;
  CRadEnvModelBase* cpCrresEleModel;
  CRadEnvModelBase* cpCrresProModel;
  CRadEnvModelBase* cpNasaEleModel;
  CRadEnvModelBase* cpNasaProModel;

  vector<CRadEnvModelBase*> cvModels; // convenience for iterative tasks
};

#endif

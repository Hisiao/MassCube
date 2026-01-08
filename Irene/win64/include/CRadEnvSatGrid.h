/***********************************************************************
 File: CRadEnvSatGrid.h
 
 Description:

   Declarations for higher-level radiation belt model methods.

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
// CRadEnvSatGrid - main interface to the Radiation Environment data; gets
// a point to CRadEnvMgr and a copy of CMagfield for its own use.
// Multiply instantiable.  Does not call any sets to CRadEnvMgr or CMagfield!

#include "GenericModel.h"
#include "CRadEnvEnums.h"
#include "CRadEnvMgr.h"
#include "CRadEnvModelBase.h"
#include "CShieldDose2.h"
#include "CMagfield.h"

#ifndef CRADENVSATGRID_MODEL_DEFINE
#define CRADENVSATGRID_MODEL_DEFINE

typedef S3Tuple S3Coord;
typedef S3TupleVec S3CoordVec;
typedef vector<ereModel> evector;
typedef vector<dvector> d2dvector;
typedef vector<d2dvector> d3dvector;
typedef vector<ivector> i2dvector;

class CRadEnvSatGrid:public GenericModel{
public:
/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::CRadEnvSatGrid
/**
*
* Default constructor
*
*/
  CRadEnvSatGrid();

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::~CRadEnvSatGrid
/**
*
* Destructor
*
*/
  ~CRadEnvSatGrid();

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::Initialize
  /**
  * Initialize the model with a path to the database file and pointer to
  * CMagfield class object.
  *
  * @param sSd2DbFullPath string: path to model database
  * @param pcmf CMagfield ptr: ptr to CMagfield object
  *
  * @return eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE Initialize(const string &sSd2DbFullPath, CMagfield* pcmf);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setCompMode
  /**
  * Set Overall Computation Mode; this selects which underlying
  * model(s) will be used in the subsequent computations.
  *
  * @param ecmIn
  *     ereCompMode: computation mode enum
  *
  */
  void setCompMode(const ereCompMode& ecmIn) {ceCompMode=ecmIn;};

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::getCompMode
  /**
  * Retrieve the current computation mode.
  *
  * @return
  *     ereCompMode
  */
  const ereCompMode getCompMode() {return ceCompMode;};
  //eRADENV_ERROR_CODE pushCompMode(const ereModel& ermIn);
  //eRADENV_ERROR_CODE clearCompMode();

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::compModeSpecies2Model
  /**
  * Translate from computation mode, species to model
  *
  * @param eMode
  *     ereCompMode: computation mode enum
  * @param eSpecies
  *     ereSpecies: species enum to translate
  *
  * @return
  *     ereModel
  */
  ereModel compModeSpecies2Model( ereCompMode eMode, ereSpecies eSpecies );

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setUseFixedEpochs
  /**
  * Set NASA and CRRES models to used fixed epochs.
  *
  * The NASA and CRRES models are based on data from fixed epochs.
  * Calculations of Lm and B/Bo using current epochs can result in
  * exceedingly high flux values. [at low altitudes]
  * (see Heynderickx et al, Radiation Measurements, Vol. 26 1996)
  *
  * @param bUseFixed
  *     bool: True to use fixed epochs, false otherwise.
  *
  */
  void setUseFixedEpochs( bool bUseFixed ) { cbUseFixedEpochs = bUseFixed; }

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::getUseFixedEpochs
  /**
  * Return the current setting for using fixed epochs.
  *
  * @return
  *     bool
  */
  bool getUseFixedEpochs() { return cbUseFixedEpochs; }

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setShiftSAAToFixedEpochs
  /**
  * Shift the SAA position FROM the current epoch TO the fixed epoch.
  *
  * Method names "[set|get]ShiftSAAToFixedEpochs" is a misnomer
  *  -when set, this shifts the SAA position *FROM* the fixed epoch
  *   (when set using above setting) to the CURRENT epoch
  * Old method names are being left as-is [for now] to maintain compatibility,
  * but any new development should use corrected names.
  * Internal associated class variable name has been corrected.
  *
  * @param bShiftSAA
  *     bool: True to shift to fixed epoch, false otherwise.
  *
  */
  void setShiftSAAToFixedEpochs( bool bShiftSAA ) { cbShiftSAAToCurrentEpochs = bShiftSAA; }

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::getShiftSAAToFixedEpochs
  /**
  * Get the current setting for shifting the SAA position FROM the current epoch
  * TO the fixed epoch.
  *
  * Method names "[set|get]ShiftSAAToFixedEpochs" is a misnomer
  *  -when set, this shifts the SAA position *FROM* the fixed epoch
  *   (when set using above setting) to the CURRENT epoch
  * Old method names are being left as-is [for now] to maintain compatibility,
  * but any new development should use corrected names.
  * Internal associated class variable name has been corrected.
  *
  * @return
  *     bool
  */
  bool getShiftSAAToFixedEpochs() { return cbShiftSAAToCurrentEpochs; }

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setShiftSAAToCurrentEpochs
  /**
  * Shift the SAA position FROM the fixed epoch TO the current epoch.
  *
  * Method names "[set|get]ShiftSAAToFixedEpochs" is a misnomer
  *  -when set, this shifts the SAA position *FROM* the fixed epoch
  *   (when set using above setting) to the CURRENT epoch
  * Old method names are being left as-is [for now] to maintain compatibility,
  * but any new development should use corrected names.
  * Internal associated class variable name has been corrected.
  *
  * @param bShiftSAA
  *     bool: True to shift to fixed epoch, false otherwise.
  *
  */
  void setShiftSAAToCurrentEpochs( bool bShiftSAA ) { cbShiftSAAToCurrentEpochs = bShiftSAA; }

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::getShiftSAAToCurrentEpochs
  /**
  * Get the current setting for shifting the SAA position FROM the fixed epoch
  * TO the current epoch.
  *
  * Method names "[set|get]ShiftSAAToFixedEpochs" is a misnomer
  *  -when set, this shifts the SAA position *FROM* the fixed epoch
  *   (when set using above setting) to the CURRENT epoch
  * Old method names are being left as-is [for now] to maintain compatibility,
  * but any new development should use corrected names.
  * Internal associated class variable name has been corrected.
  *
  * @return
  *     bool
  */
  bool getShiftSAAToCurrentEpochs() { return cbShiftSAAToCurrentEpochs; }

  // ApexRad and CrresRad shielding thickness sets/gets performed here
  // with direct wraps of the corresponding DB and SD2 methods; full
  // lists are maintained here, however

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::getShieldThickLists
  /**
  * getShieldThickLists(): get the defaults lists for Apex- and
  * Crres-Rad dose-rate models.
  *
  * @param[out] pfvASTL
  *     fvector ptr: ptr to float vector to store Apex shield thicknesses
  * @param[out] pfvCRSTL
  *     fvector ptr: ptr to float vector to store CRRES shield thicknesses
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE getShieldThickLists(fvector* pfvASTL, fvector* pfvCRSTL);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::getShieldThicks
  /**
  * getShieldThicks(): get the current internal shielding thickness list
  *
  * @param[out]
  *     pfvSTs fvector ptr: ptr to float vector to store shield thicknesses
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE getShieldThicks(fvector* pfvSTs);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setShieldThickCommon
  /**
  * setShieldThickCommon() now clears the current list and then
  * pushes on the ApexRad/CrresRad common thicknesses.
  * (error code should be deprecated -raq)
  * previous usage deprecated (unset version deprecated);
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setShieldThickCommon();

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setShieldThicks
  /**
  * setShieldThicks(); clears current list and then pushes on either
  * the ApexRad or CrresRad list of thicknesses (4), or subset
  * thereof.
  *
  * @param edmIn
  *     ereDoseModel: dose model enum
  * @param ivIDs
  *     ivector: int vector
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setShieldThicks(const ereDoseModel& edmIn, const ivector& ivIDs);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::pushShieldThick
  /**
  * pushShieldThick(): push a shielding thickness onto the list
  * previous direct wrap usage deprecated; list is maintained here
  * and pushed down to SD2 as needed.
  *
  * @param fST
  *     float: shielding thickness to push
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE pushShieldThick(const float& fST);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::clearShieldThick
  /**
  * clearShieldThick(): clear the list
  *
  */
  void clearShieldThick();

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setShieldThickUnits
  /**
  * setShieldThickUnits: detector shielding thickness units (default
  * mm = ecsMillimeters); units are handled and converted at this
  * level now.  For safety, a call to this first clears the internal
  * list.
  *
  * @param ecsIn
  *     ecsShieldThickUnits: shielding thickness unit enum
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setShieldThickUnits(const ecsShieldThickUnits& ecsIn);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setDoseChannel
  /**
  * setDoseChannel(): Set Dose channel which applies to either or
  * both ApexRad and CrresRad
  *
  * @param ereDC
  *     ereDoseChannel: dose channel enum
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setDoseChannel(const ereDoseChannel& ereDC);

  // ************************************************************ //
  // NOTE: The activity levels and the energy lists to return are
  // controlled by the "Environment" and are set directly on the
  // CRadEnvDB object -- NOT through this class.  In particular, the
  // "Environment" has full responsibility for setting the 15DayAp in
  // CRadEnvDB and calling this class synchronously with those sets!
  // ************************************************************ //


  // detector material type (default Silicon)
/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setDetectorType
  /**
  * Set shield detector type.
  *
  * @param ecsIn
  *     ecsDetectorType: detector type enum
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setDetectorType(const ecsDetectorType& ecsIn)
  {return ccSD.setDetectorType(ecsIn);};

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setDetectorGeometry
  /**
  * Set // detector geometry (default ecsSpherical)
  *
  * @param ecsIn
  *     ecsDetectorGeometry: detector geometry enum
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setDetectorGeometry(const ecsDetectorGeometry& ecsIn)
  {ceDetectorGeometry=ecsIn; return eNoError;};

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::setNuclearAttenMode
  /**
  * Set nuclear attenuation mode (default none)
  *
  * @param ecsIn
  *     ecsNuclearAttenMode: nuclear attenuation enum
  *
  * @return
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setNuclearAttenMode(const ecsNuclearAttenMode& ecsIn)
  {return ccSD.setNuclearAttenMode(ecsIn);};

  // ** Computation methods **

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeFlux
  /**
  * computeFlux()
  *  input:
  *   Geospace time
  *   Coordinate (ecef km)
  *   Species to return (efmEle or efmPro)
  *  output:
  *   Model used in this computation <- no longer needed
  *   Flux vector
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: coordinate tuple
  * @param dAp15
  *     double: 15-day AP
  * @param ereSp
  *     ereSpecies: species enum
  *
  * @param[out] pdvFlux
  *     float vector fvector ptr: ptr to float vector to store flux
  *    
  * @return
  *     eRADENV_ERROR_CODE
  */
   eRADENV_ERROR_CODE computeFlux(const double& dDTime,
                                 const S3Coord& sc3In,
                                 const double& dAp15,
                                 const ereSpecies& ereSp,
                                 fvector* pdvFlux);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeFlux
  /**
  * computeFlux() overload w/o AP index
  *  input:
  *   Geospace time
  *   Coordinate (ecef km)
  *   Species to return (efmEle or efmPro)
  *  output:
  *   Model used in this computation <- no longer needed
  *   Flux vector
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: coordinate tuple in ECEF in km (GEOinKM)
  * @param ereSp
  *     ereSpecies: species enum
  *
  * @param[out] pdvFlux
  *     float vector fvector ptr: ptr to float vector to store flux
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeFlux(const double& dDTime,
                                 const S3Coord& sc3In,
                                 const ereSpecies& ereSp,
                                 fvector* pdvFlux);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeFlux
  /**
  * computeFlux() overload with dBBeq
  *
  * @param dL
  *     double: L-shell value
  * @param dBBeq
  *     double: B-field strength at equator
  * @param dAp15
  *     double: 15-day AP
  * @param ereSp
  *     ereSpecies: species enum
  *
  * @param[out] pdvFlux
  *     float vector fvector ptr: ptr to float vector to store flux
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeFlux(const double& dL,
                                 const double& dBBeq,
                                 const double& dAp15,
                                 const ereSpecies& ereSp,
                                 fvector* pdvFlux);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeFlux
  /**
  * computeFlux() overload with dBBeq, w/o AP index
  *
  * @param dL
  *     dobule: L-shell value
  * @param dBBeq
  *     double: B-field strength at equator
  * @param ereSp
  *     ereSpecies: species enum
  *
  * @param[out] pdvFlux
  *     float vector fvector ptr: ptr to float vector to store flux
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeFlux(const double& dL,
                                 const double& dBBeq,
                                 const ereSpecies& ereSp,
                                 fvector* pdvFlux);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeIntegralFlux
  /**
  * computeIntegralFlux()
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: coordinate tuple
  * @param dAp15
  *     double: 15-day AP
  * @param ereSp
  *     ereSpecies: species enum
  *
  * @param[out] pdvFlux
  *     float vector fvector ptr: ptr to float vector to store flux
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeIntegralFlux(const double& dDTime,
                                         const S3Coord& sc3In,
                                         const double& dAp15,
                                         const ereSpecies& ereSp,
                                         fvector* pdvFlux);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeIntegralFlux
  /**
  * computeIntegralFlux() w/o AP index
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: coordinate tuple
  * @param ereSp
  *     ereSpecies: species enum
  *
  * @param[out] pdvFlux
  *     float vector fvector ptr: ptr to float vector to store flux
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeIntegralFlux(const double& dDTime,
                                         const S3Coord& sc3In,
                                         const ereSpecies& ereSp,
                                         fvector* pdvFlux);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeIntegralFlux
  /**
  * computeIntegralFlux() with dBBeq
  *
  * @param dL
  *     double: L-shell value
  * @param dBBeq
  *     double: B-field strength at equator
  * @param ereSp
  *     ereSpecies: species enum
  *
  * @param[out] pdvFlux
  *     float vector fvector ptr: ptr to float vector to store flux
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeIntegralFlux(const double& dL,
                                         const double& dBBeq,
                                         const double& dAp15,
                                         const ereSpecies& ereSp,
                                         fvector* pdvFlux);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeIntegralFlux
  /**
  * computeIntegralFlux(), dBBeq w/o AP index
  *
  * @param dL
  *     double: L-shell value
  * @param dBBeq
  *     double: B-field strength at equator
  * @param ereSp
  *     ereSpecies: species enum
  *
  * @param[out] pdvFlux
  *     float vector fvector ptr: ptr to float vector to store flux
  *    
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeIntegralFlux(const double& dL,
                                         const double& dBBeq,
                                         const ereSpecies& ereSp,
                                         fvector* pdvFlux);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeDoseRateData
  /**
  * computeDoseRateData()
  *
  * Retrieves dose rates from ApexRad and
  * CrresRad only; other models on CompMode list will be
  * ignored. ModelUsed is set to -1 if the thickness given can't be
  * resolved by ApexRad and/or CrresRad.
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: S3 coord tuple
  * @param dAp15
  *     double: 15-day AP index
  *
  * @param[out] pivModelUsed
  *     int vector ivector ptr: ptr to vector storing model used IDs
  * @param[out] pfvDoseRate
  *     float vector fvector: ptr to float vector storing dose rates
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeDoseRateData(const double& dDTime,
                                         const S3Coord& sc3In,
                                         const double& dAp15,
                                         ivector* pivModelUsed,
                                         fvector* pfvDoseRate);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeDoseRateData
  /**
  * computeDoseRate(): overload w/o AP index
  *
  * Retrieves dose rates from ApexRad and
  * CrresRad only; other models on CompMode list will be
  * ignored. ModelUsed is set to -1 if the thickness given can't be
  * resolved by ApexRad and/or CrresRad.
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: S3 coord tuple
  *
  * @param[out] pivModelUsed
  *     int vector ivector ptr: ptr to vector storing model used IDs
  * @param[out] pfvDoseRate
  *     float vector fvector: ptr to float vector storing dose rates
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeDoseRateData(const double& dDTime,
                                         const S3Coord& sc3In,
                                         ivector* pivModelUsed,
                                         fvector* pfvDoseRate);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeDoseRateData
  /**
  * computeDoseRate(): overload w dBBeq
  *
  * Retrieves dose rates from ApexRad and
  * CrresRad only; other models on CompMode list will be
  * ignored. ModelUsed is set to -1 if the thickness given can't be
  * resolved by ApexRad and/or CrresRad.
  *
  * @param dL
  *     double: L-shell value
  * @param dBBeq
  *     double: B-field strength at equator
  * @param dAp15
  *     double: 15-day AP index
  *
  * @param[out] pivModelUsed
  *     int vector ivector ptr: ptr to vector storing model used IDs
  * @param[out] pfvDoseRate
  *     float vector fvector: ptr to float vector storing dose rates
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeDoseRateData(const double& dL,
                                         const double& dBBeq,
                                         const double& dAp15,
                                         ivector* pivModelUsed,
                                         fvector* pfvDoseRate);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeDoseRateData
  /**
  * computeDoseRate(): overload w dBBeq w/o AP index
  *
  * Retrieves dose rates from ApexRad and
  * CrresRad only; other models on CompMode list will be
  * ignored. ModelUsed is set to -1 if the thickness given can't be
  * resolved by ApexRad and/or CrresRad.
  *
  * @param dL
  *     double: L-shell value
  * @param dBBeq
  *     double: B-field strength at equator
  *
  * @param[out] pivModelUsed
  *     int vector ivector ptr: ptr to vector storing model used IDs
  * @param[out] pfvDoseRate
  *     float vector fvector: ptr to float vector storing dose rates
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeDoseRateData(const double& dL,
                                         const double& dBBeq,
                                         ivector* pivModelUsed,
                                         fvector* pfvDoseRate);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeDoseRateModel
  /**
  * computeDoseRateModel(): computes dose rate *only* based on flux
  * models plus shielddose2. ignores ApexRadEnv and CrresRadEnv if
  * they are in the CompMode list.  this version must be used in
  * order to obtain dose rates per species.
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: S3 coordinate tuple
  * @param dAp15
  *     double: 15-day AP index
  *
  * @param[out] pfvEleDoseRate
  *     float vector ptr: ptr to float vector to store electron dose rate
  * @param[out] pfvEleBrDoseRate
  *     float vector ptr: ptr to float vector to store ele Br dose rate
  * @param[out] pfvProDoseRate
  *     float vector ptr: ptr yo float vector to store proton dose rate
  * @param[out] pfvSolDoseRate
  *     float vector ptr: ptr to float vector to store Sol dose rate
  * @param[out] pfvDoseRate
  *     float vector ptr: ptr to float vector to store dose rate
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeDoseRateModel(const double& dDTime,
                                          const S3Coord& sc3In,
                                          const double& dAp15,
                                          fvector* pfvEleDoseRate,
                                          fvector* pfvEleBrDoseRate,
                                          fvector* pfvProDoseRate,
                                          fvector* pfvSolDoseRate,
                                          fvector* pfvDoseRate);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeDoseRateModel
  /**
  * computeDoseRateModel(): overload w/o AP index
  * computeDoseRateModel(): computes dose rate *only* based on flux
  * models plus shielddose2. ignores ApexRadEnv and CrresRadEnv if
  * they are in the CompMode list.  this version must be used in
  * order to obtain dose rates per species.
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: S3 coordinate tuple
  *
  * @param[out] pfvEleDoseRate
  *     float vector ptr: ptr to float vector to store electron dose rate
  * @param[out] pfvEleBrDoseRate
  *     float vector ptr: ptr to float vector to store ele Br dose rate
  * @param[out] pfvProDoseRate
  *     float vector ptr: ptr yo float vector to store proton dose rate
  * @param[out] pfvSolDoseRate
  *     float vector ptr: ptr to float vector to store Sol dose rate
  * @param[out] pfvDoseRate
  *     float vector ptr: ptr to float vector to store dose rate
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeDoseRateModel(const double& dDTime,
                                          const S3Coord& sc3In,
                                          fvector* pfvEleDoseRate,
                                          fvector* pfvEleBrDoseRate,
                                          fvector* pfvProDoseRate,
                                          fvector* pfvSolDoseRate,
                                          fvector* pfvDoseRate);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeDoseRateModel
  /**
  * computeDoseRateModel(): overload with dBBeq
  * computeDoseRateModel(): computes dose rate *only* based on flux
  * models plus shielddose2. ignores ApexRadEnv and CrresRadEnv if
  * they are in the CompMode list.  this version must be used in
  * order to obtain dose rates per species.
  *
  * @param dDtime
  *     double: geospace time
  * @param dL
  *     double: L-shell value
  * @param dBBeq
  *     double: B-field value at equator
  * @param dAp15
  *     double: 15-day AP index
  *
  * @param[out] pfvEleDoseRate
  *     float vector ptr: ptr to float vector to store electron dose rate
  * @param[out] pfvEleBrDoseRate
  *     float vector ptr: ptr to float vector to store ele Br dose rate
  * @param[out] pfvProDoseRate
  *     float vector ptr: ptr yo float vector to store proton dose rate
  * @param[out] pfvSolDoseRate
  *     float vector ptr: ptr to float vector to store Sol dose rate
  * @param[out] pfvDoseRate
  *     float vector ptr: ptr to float vector to store dose rate
  *
  * @return eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeDoseRateModel(const double& dL,
                                          const double& dBBeq,
                                          const double& dAp15,
                                          fvector* pfvEleDoseRate,
                                          fvector* pfvEleBrDoseRate,
                                          fvector* pfvProDoseRate,
                                          fvector* pfvSolDoseRate,
                                          fvector* pfvDoseRate);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::computeDoseRateModel
  /**
  * computeDoseRateModel(): overload with dBBeq w/o AP index
  * computeDoseRateModel(): computes dose rate *only* based on flux
  * models plus shielddose2. ignores ApexRadEnv and CrresRadEnv if
  * they are in the CompMode list.  this version must be used in
  * order to obtain dose rates per species.
  *
  * @param dDtime
  *     double: geospace time
  * @param dL
  *     double: L-shell value
  * @param dBBeq
  *     double: B-field value at equator
  *
  * @param[out] pfvEleDoseRate
  *     float vector ptr: ptr to float vector to store electron dose rate
  * @param[out] pfvEleBrDoseRate
  *     float vector ptr: ptr to float vector to store ele Br dose rate
  * @param[out] pfvProDoseRate
  *     float vector ptr: ptr yo float vector to store proton dose rate
  * @param[out] pfvSolDoseRate
  *     float vector ptr: ptr to float vector to store Sol dose rate
  * @param[out] pfvDoseRate
  *     float vector ptr: ptr to float vector to store dose rate
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE computeDoseRateModel(const double& dL,
                                          const double& dBBeq,
                                          fvector* pfvEleDoseRate,
                                          fvector* pfvEleBrDoseRate,
                                          fvector* pfvProDoseRate,
                                          fvector* pfvSolDoseRate,
                                          fvector* pfvDoseRate);


  // New model for dose integration over multiple samples
  // Caller drives the interval, no need to set one.
  // New method: accumulateFluxes simply sums fluxes and count of fluxes over time
  // New method: getAccumulatedDoseRate calculates and returns dose rate from the
  // average, then resets the flux cache
  // New method: clearAccumulatedFluxes allows user to reset cache as needed
  // Old method: computeDoseRateModel reverts back to compute dose at a passed time only
/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::accumulateFluxesForDoseCalc
  /**
  * New method: accumulateFluxes simply sums fluxes and count of fluxes over time
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: S3 coord tuple
  * @param dAp15
  *     double: 15-day AP index
  *
  * @return
  *     eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE accumulateFluxesForDoseCalc( const double& dDTime,
                                                  const S3Coord& sc3In,
                                                  const double& dAp15 );

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::accumulateFluxesForDoseCalc
  /**
  * New method: accumulateFluxes simply sums fluxes and count of fluxes over time
  * Overloaded without AP index.
  *
  * @param dDtime
  *     double: geospace time
  * @param sc3In
  *     S3Coord: S3 coord tuple
  *
  * @return eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE accumulateFluxesForDoseCalc( const double& dDTime,
                                                  const S3Coord& sc3In );

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::getAccumulatedDoseRate
  /**
  * New method: getAccumulatedDoseRate calculates and returns dose rate from the
  * average, then resets the flux cache
  *
  * @param[out] pfvEleDoseRate
  *     float vector ptr: ptr to float vector to store electron dose rate
  * @param[out] pfvEleBrDoseRate
  *     float vector ptr: ptr to float vector to store ele Br dose rate
  * @param[out] pfvProDoseRate
  *     float vector ptr: ptr yo float vector to store proton dose rate
  * @param[out] pfvSolDoseRate
  *     float vector ptr: ptr to float vector to store Sol dose rate
  * @param[out] pfvDoseRate
  *     float vector ptr: ptr to float vector to store dose rate
  *
  * @return eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE getAccumulatedDoseRate( fvector* pfvEleDoseRate,
                                             fvector* pfvEleBrDoseRate,
                                             fvector* pfvProDoseRate,
                                             fvector* pfvSolDoseRate,
                                             fvector* pfvDoseRate);

/*------------------------------------------------------------------*/
// function CRadEnvSatGrid::clearAccumulatedFluxes
  /**
  * New method: clearAccumulatedFluxes allows user to reset cache as needed
  *
  * @return eRADENV_ERROR_CODE
  */
  eRADENV_ERROR_CODE clearAccumulatedFluxes();

  // ************************************************************** //
  // NOTE: This model supports fluence and total dose over orbit at
  // the calling environment level.  Those values are simply time
  // integrals over the values returned by the above methods.  See
  // utCRadEnv.cpp for a simple example.
  // ************************************************************** //

  // ------------------------------------------------------ //
  // **** NO-COM Implementation for Methods below HERE **** //
  // ------------------------------------------------------ //
  //eRADENV_ERROR_CODE getCompMode(ivector* pivOut);
  // need rest of gets and get-wraps on cshielddose2 and cradenvdb for
  // completeness - raq - 08/06/2008

private:
  CMagfield* cpcMf;
  CRadEnvMgr* cpcREMgr;
  CShieldDose2 ccSD;

  // *** gets on DB wrapped here to facilitate x-platform *** //
//   eRADENV_ERROR_CODE getFlux(const ereFluxModel& efmIn,
//                                const double& dL,
//                                const double& dBBeq,
//                                dvector* pdvFlux);

//   eRADENV_ERROR_CODE getDose(const ereDoseModel& edmIn,
//                                const double& dL,
//                                const double& dBBeq,
//                                double* pdDose);

  // updated to use ereModel enum; getFlux returns error if called
  // with ermApexRad or ermCrresRad
  eRADENV_ERROR_CODE getFlux(const ereModel& ermIn,
                             const double& dL,
                             const double& dBBeq,
                             fvector* pfvFlux);

  eRADENV_ERROR_CODE getIntegralFlux(const ereModel& ermIn,
                                     const double& dL,
                                     const double& dBBeq,
                                     fvector* pfvFlux);

  eRADENV_ERROR_CODE getDose(const ereModel& ermIn,
                             const double& dL,
                             const double& dBBeq,
                             float* pfDose);

  eRADENV_ERROR_CODE computeLBBeq( ereCompMode eModel, ereSpecies eSpecies, 
                                   double dTime, const S3Coord& pos,
                                   double& dL, double& dBBeq );

  // \/ \/ rest of model state \/ \/
  ereCompMode ceCompMode;
  ivector civStCommonIds;
  fvector cfvCrresSTs, cfvApexSTs;
  fvector cfvCurShieldThicks;
  float cfSDunitConv, cfREunitConv;
  //bool cbCompModeBest; //, cbShieldThickCommon;
  ecsDetectorGeometry ceDetectorGeometry;

  // The NASA and CRRES models are based on data
  // from fixed epochs. Calculations of Lm and
  // B/Bo using current epochs can result in
  // exceedingly high flux values [at low altitudes]
  // (see Heynderickx et al, Radiation Measurements, Vol. 26 1996)

  bool cbUseFixedEpochs;
  bool cbShiftSAAToCurrentEpochs;

  // Dose integration data and methods
  // -for summing fluxes to compute model dose only at an integration interval, 
  //  for each model, species and energy

  d3dvector cvvvdFluxTotals;                 // current running flux totals [model][species][energy] 
  i2dvector cvviNumFluxesTotaled;            // current running count of fluxed totaled [model][species]

  // Internal methods consolidating code used in multiple methods

  void accumulateFluxes( ereCompMode eModel, ereSpecies eSpecies, fvector& vfFluxes );

  eRADENV_ERROR_CODE getSpeciesFlux( ereSpecies eSpecies, double dL, double dBBeq,
                                     fvector& fvFluxes, fvector& fvEnergies,
                                     double& fEnergyMin, double& fEnergyMax, bool& gotFlux );

  eRADENV_ERROR_CODE getShieldDoseDose( SIntegrationParams& integrationParams, 
                                        bool bUseElectrons, 
                                        fvector& fvEleEnergies, 
                                        fvector& fvEleFluxes,
                                        bool bUseProtons, 
                                        fvector& fvProEnergies, 
                                        fvector& fvProFluxes,
                                        fvector& fvEleDoseRate, 
                                        fvector& fvEleBrDoseRate,
                                        fvector& fvProDoseRate, 
                                        fvector& fvSolDoseRate,
                                        fvector& fvDoseRate);

  int filterFluxAndEnergies( fvector& fvFlux, fvector& fvEnergies );
};

#endif

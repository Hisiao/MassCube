/***********************************************************************

Module: CShieldDose2.h

Description: Class definition for CShieldDose2 object

 Computes model dose rates given trapped electron and proton,
 and solar proton, energy-spectral fluxes; also supports the
 CRadEnvSat Class; has own db

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution is unlimited.

 Author:

   This software was developed and/or modified by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avevenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:

   None

 Revision history:

   VERSION            DATE       NOTES
     1.0       Fri Oct 3, 2008   First release

------------------------------------------------------------------------

***********************************************************************/
#ifndef CSHIELDDOSE2_MODEL_DEFINE
#define CSHIELDDOSE2_MODEL_DEFINE

#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <cmath>

//#include "boost/multi_array.hpp"

using namespace std;

#include "GenericModel.h"
#include "CShieldDoseEnums.h"
#include "CShieldDoseDb.h"
#include "CRadEnvEnums.h"

//
// Public constants
//

const int SD2MAXNUMDETTYPES = 11;

//
// Public structures
//

// structure contains 'float' values - desirable to change to 'double'
struct SDoseRate {
   float fTotal;
   float fTrappedProtons;
   float fElectrons;
   float fBrehmstr;
   float fElecPlusBrehmstr;
   float fSolarProtons;

   SDoseRate():
      fTotal(0.0),
      fTrappedProtons(0.0),
      fElectrons(0.0),
      fBrehmstr(0.0),
      fElecPlusBrehmstr(0.0),
      fSolarProtons(0.0)
   {};
};
typedef std::vector<SDoseRate> SDoseRateVec;

struct SDoseRateD {
   double dTotal;
   double dTrappedProtons;
   double dElectrons;
   double dBrehmstr;
   double dElecPlusBrehmstr;
   double dSolarProtons;

   SDoseRateD():
      dTotal(0.0),
      dTrappedProtons(0.0),
      dElectrons(0.0),
      dBrehmstr(0.0),
      dElecPlusBrehmstr(0.0),
      dSolarProtons(0.0)
   {};
};
typedef std::vector<SDoseRateD> SDoseRateDVec;

// 8/1/2019 - structure changed to 'double' variables from 'float'
typedef struct SIntegrationParams {
   int nptsp;
   int nptse;
   double emins;
   double emaxs;
   double eminp;
   double emaxp;
   double emine;
   double emaxe;
   ecsInterpolationType ceInterpolationType;
// AGL
// May want to initialize integration limits to something other than 0.0
   SIntegrationParams():nptsp(1001),nptse(1001),emins(0.0),emaxs(0.0),eminp(0.0),
      emaxp(0.0),emine(0.0),emaxe(0.0),ceInterpolationType(ecsSpline){};
} SIntegrationParams;


//
// Class: CShieldDose2
//

/**
 * @brief Shielded Dose calculation class
 *
 * The "ShieldDose2" class provides methods to determine the amount of 
 * radiation dose absorbed by a target material behind aluminum shielding.
 *
 * The dosage absorbed by a specified target material is calculated as a 
 * function of shielding depth, evaluated for three different shielding 
 * geometries: semi-infinite slab, finite slab, and spherical.
 *
 * The C++ ShieldDose2 model implemented in the SpWx library has been
 * ported from its original published FORTRAN (see second reference below).
 * The associated ASCII datafiles have been transformed into a HDF5
 * database, but also correcting an error in the published bremsstrahlung
 * data tables (in the 'elbrbas2.dat' file, with the exception of Al
 * detectors, the 'finite slab' and 'semi-inifinite slab' data tables
 * had been switched).
 *
 * Model Quirks:
 * Because the ShieldDose2 model makes use of tables containing
 * pre-calculated dose-depth results, the code normally performs spline
 * fitting of the input fluxes and output dose results.  Depending on the
 * natures of the flux inputs and/or depth specifications, sometimes invalid
 * or inconsistent dose results are returned.  To increase the confidence in
 * the validity of the model results, here are several suggestions:
 *   - avoid discontinuities in the input flux spectrum; zero flux values
 *     adjacent to non-zero ones wil induce unpredictable spline fits that
 *     will cause invalid dose results.  If the situation cannot be avoided,
 *     it is recommended that the 'InterpolationType' be set to 'Linear'.
 *   - specify adequate number of depths (at least five) that are somewhat
 *     evenly spaced (in a log space). It has been observed that uneven
 *     spacing can lead to negative dose results; addition or removal of one
 *     of the depths can cause different dose results for the unchanged depth
 *     levels. Some applications using the ShieldDose2 model (such as RadEnv
 *     or Ae9Ap9) insert additional depth values above and below each depth
 *     specified (termed as 'padding'), as a way to ensure consistent results,
 *     regardless of potentially uneven depth specifications.
 *
 * Detector materials are defined in an enumeration in CShieldDoseEnums.h.
 *
 * All methods using floats are deprecated and kept for backwards
 * compatibility.
 *
 * References:
 *  - Seltzer, S. M., SHIELDOSE, A Computer Code for Space-Shielding Radiation
 *    Dose Calculations, National Bureau of Standards, NBS Technical Note 1116,
 *    U.S. Government Printing Office, Washington, D.C., 1980.
 *  - Seltzer, S. M., Updated calculations for routine space-shielding
 *    radiation dose estimates: SHIELDOSE-2, NIST Publication NISTIR 5477,
 *    Gaithersburg, MD., December 1994.
 *  - Heynderickx, D., 2013, private communication.
 */
class CShieldDose2:public GenericModel
{
public:

/*------------------------------------------------------------------*/
// function CShieldDose2::CShieldDose2
/**
*
* Default constructor
*
*/
  CShieldDose2();

/*------------------------------------------------------------------*/
// function CShieldDose2::~CShieldDose2
/**
*
* Destructor
*
*/
  ~CShieldDose2();

/*------------------------------------------------------------------*/
// function CShieldDose2::CShieldDose2
  /**
  *
  * Constructor, with database file specified
  *
  * @param sDBFullPath
  *     string: relative or absolute path to ShieldDose2 database file
  *
  */
  CShieldDose2(const string& sDBFullPath);

/*------------------------------------------------------------------*/
// function CShieldDose2::Initialize
  /**
  *
  * Reads input database file and initializes internal data tables
  * Use of this routine is not needed if the second form of the constructor is used.
  * Starting in SpWx v2.2, file is located in 'data' subdirectory of install location
  * alternatively found in SpWxData/ModelDatabases.
  *
  * @param sDBFullPath
  *     string: relative or absolute path to ShieldDose2 database file
  *
  * @returns
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE Initialize(const string& sDBFullPath);

  // detector shielding thickness
/*------------------------------------------------------------------*/
// function CShieldDose2::pushShieldThick
  /**
  *
  * Adds a shield thicknesses. Legacy float version.
  *
  * @param fST
  *     float: shielding thickness to add
  *
  * @returns eGENERIC_ERROR_CODE
  *     eNoError=success, otherwise error (see GenericModelEnum.h)
  */
  // legacy 'float' version
  eGENERIC_ERROR_CODE pushShieldThick(const float& fST);

/*------------------------------------------------------------------*/
// function CShieldDose2::pushShieldThick
  /**
  *
  * Adds a shield thicknesses. Legacy float version.
  *
  * @param dST
  *     double: shielding thickness to add
  *
  * @returns eGENERIC_ERROR_CODE
  *    eNoError - success
  */
  eGENERIC_ERROR_CODE pushShieldThick(const double& dST);

/*------------------------------------------------------------------*/
// function CShieldDose2::setShieldThicks
  /**
  *
  * Sets the shield thicknesses. Legacy float version.
  *
  * @param fvST
  *     float vector: vector containing shield thicknesses
  *
  * @return eGENERIC_ERROR_CODE
  *      eNoError - success
  */
  eGENERIC_ERROR_CODE setShieldThicks(const fvector& fvST);

/*------------------------------------------------------------------*/
// function CShieldDose2::setShieldThicks
  /**
  *
  * Sets the shield thicknesses.
  *
  * @param dvST
  *     double vector: vector containing shield thicknesses
  *
  * @return eGENERIC_ERROR_CODE
  *      eNoError - success
  */
  eGENERIC_ERROR_CODE setShieldThicks(const dvector& dvST);

/*------------------------------------------------------------------*/
// function CShieldDose2::clearShieldThick
  /**
  *
  * Removes all shield thicknesses
  *
  * @returns eGENERIC_ERROR_CODE
  *   eNoError - success
  */
  eGENERIC_ERROR_CODE clearShieldThick();

/*------------------------------------------------------------------*/
// function CShieldDose2::getShieldThicks
  /**
  *
  * Retrieves shield thicknesses. Legacy float version.
  *
  * @param pfvST
  *     float pointer: pointer to float array of shield thicknesses (out)
  *
  * @returns eGENERIC_ERROR_CODE
  *     eNoError - success
  */
  eGENERIC_ERROR_CODE getShieldThicks(fvector* pfvST);

/*------------------------------------------------------------------*/
// function CShieldDose2::getShieldThicks
  /**
  *
  * Retrieves shield thicknesses
  *
  * @param pdvST
  *     double pointer: pointer to double array of shield thicknesses (out)
  *
  * @returns eGENERIC_ERROR_CODE
  *     eNoError - success
  */
  eGENERIC_ERROR_CODE getShieldThicks(dvector* pdvST);

/*------------------------------------------------------------------*/
// function CShieldDose2::getNumThicks
  /**
  *
  * return the number of shielding thicknesses
  *
  * @returns int
  */
  int getNumThicks() { return (int)cdvShieldThicks.size(); }

  // detector shielding thickness units (default mm = ecsMillimeters)
/*------------------------------------------------------------------*/
// function CShieldDose2::setShieldThickUnits
  /**
  *
  * Defines the units for shield thickness, via enumeration
  *
  * @param eUnits
  *     ecsShieldingThickUnits: shield thickness units enum
  *     * ecsMils        for 'mils'
  *     * ecsGperCm2     for 'g/cm2'
  *     * ecsMilliMeters for 'mm'
  *
  * @return eGENERIC_ERROR_CODE
  *      eNoError - success
  */
  eGENERIC_ERROR_CODE setShieldThickUnits(const ecsShieldThickUnits& eUnits);

/*------------------------------------------------------------------*/
// function CShieldDose2::setShieldThickUnitsStr
  /**
  *
  * Defines the units for shield thickness, via string
  * Choose from
  *
  * @param sUnits
  *     string: shield thickness units
  *     * 'mils'
  *     * 'gpercm2' or 'g/cm2'
  *     * 'mm' or 'millimeters'
  *
  * @return eGENERIC_ERROR_CODE
  *      eNoError - success
  */
  eGENERIC_ERROR_CODE setShieldThickUnitsStr(const string sUnits);

/*------------------------------------------------------------------*/
// function CShieldDose2::getShieldThickUnits
  /**
  *
  * Returns the shield thickness units as an enumeration
  *
  * @return ecsShieldingThickUnits
  *      current shield thickness units enum
  */
  ecsShieldThickUnits getShieldThickUnits() {return ceShieldThickUnits;}

/*------------------------------------------------------------------*/
// function CShieldDose2::getShieldThickUnitsStr
  /**
  *
  * Returns the shield thickness units as a string
  *
  * @returns string
  *     Current shield thickness units
  */
  string getShieldThickUnitsStr();

  // detector material type (default Aluminum)
/*------------------------------------------------------------------*/
// function CShieldDose2::setDetectorType
  /**
  *
  * set detector material type (default = Aluminum)
  *
  * @param ecsIn
  *     ecsDetectorType: detector material enumeration type
  *     * ecsAluminum        for 'aluminum' (Al)
  *     * ecsGraphite        for 'graphite'
  *     * ecsSilicon         for 'silicon' (Si)
  *     * ecsAir             for 'air' 
  *     * ecsBone            for 'bone'
  *     * ecsCalcium         for 'calcium' (actually CaF2)
  *     * ecsGallium         for 'gallium' (actually GaAs)
  *     * ecsLithium         for 'lithium' (actually LiF)
  *     * ecsSiliconDioxide  for 'glass' (SiO2)
  *     * ecsTissue          for 'tissue'
  *     * ecsWater           for 'water' (H20)
  *
  * @returns
  *     eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setDetectorType( const ecsDetectorType& ecsIn );

/*------------------------------------------------------------------*/
// function CShieldDose2::setDetectorTypeStr
  /**
  *
  * set detector material type (class default = Aluminum)
  *
  * @param sDetector
  *     string: detector material type as string
  *     * 'aluminum', 'Al'
  *     * 'graphite'
  *     * 'silicon', 'Si'
  *     * 'air'
  *     * 'bone'
  *     * 'calcium', 'Ca', 'CaF2'
  *     * 'gallium', 'Ga', 'GaAs'
  *     * 'lithium', 'Li', 'LiF'
  *     * 'glass', 'SiO2'
  *     * 'tissue'
  *     * 'water', 'H20'
  *
  * @returns eGENERIC_ERROR_CODE
  */
  eGENERIC_ERROR_CODE setDetectorTypeStr( const string sDetector );

/*------------------------------------------------------------------*/
// function CShieldDose2::getDetectorType
  /**
  *
  * Return enum detector type
  *
  * @returns ceDetectorType
  *     current detector type enum
  */
  ecsDetectorType getDetectorType() {return ceDetectorType;}

/*------------------------------------------------------------------*/
// function CShieldDose2::getDetectorTypeStr
  /**
  *
  * Returns the type of detector in use as a string
  *
  * @returns string
  *     Current detector type
  */
  string getDetectorTypeStr();

  // nuclear attenuation mode (default none)
/*------------------------------------------------------------------*/
// function CShieldDose2::setNuclearAttenMode
  /**
  *
  * Sets the nuclear attenuation mode for dose calculations (default = None)
  *
  * @param eMode
  *     ecsNuclearAttenMode: enumerated nuclear attenuation mode
  *     * ecsNone               for 'none' (default state)
  *     * ecsLcsed              for 'nuclearinterations'
  *     * ecsLcsedPlusNeutrons  for 'nuclearandneutrons'
  *
  * @return eGENERIC_ERROR_CODE
  *         eNoError - success
  */
  eGENERIC_ERROR_CODE setNuclearAttenMode(const ecsNuclearAttenMode& eMode);

/*------------------------------------------------------------------*/
// function CShieldDose2::setNuclearAttenModeStr
  /**
  *
  * Sets the nuclear attenuation mode for dose calculations (default = None)
  *
  * @param sMode
  *     string: nuclear attenuation mode
  *     * 'none' (default state)
  *     * 'nuclearinterations'
  *     * 'nuclearandneutrons'
  *
  * @return eGENERIC_ERROR_CODE
  *         eNoError - success
  */
  eGENERIC_ERROR_CODE setNuclearAttenModeStr(const string sMode);

/*------------------------------------------------------------------*/
// function CShieldDose2::getNuclearAttenMode
  /**
  *
  * Returns enumeration for nuclear attenuation mode
  *
  * @returns ceNuclearAttenMode
  *      current nuclear attenuation mode enum
  */
  ecsNuclearAttenMode getNuclearAttenMode() {return ceNuclearAttenMode;}

/*------------------------------------------------------------------*/
// function CShieldDose2::getNuclearAttenModeStr
 /**
 *
 * Returns the nuclear attenuation mode in use as a string
 *
 * @returns string
 *     current nuclear attenuation mode
 */
  string getNuclearAttenModeStr();

  //Integration parameters
/*------------------------------------------------------------------*/
// function CShieldDose2::setIntegrationParams
  /**
  *
  * Sets the integration parameters to use for dose calculations
  *
  * @param sParams
  *     SIntegrationParams: struct containing integration parameters
  *
  * @returns eGENERIC_ERROR_CODE
  *         eNoError - success
  */
  eGENERIC_ERROR_CODE setIntegrationParams(const SIntegrationParams& sParams);

/*------------------------------------------------------------------*/
// function CShieldDose2::getIntegrationParams
  /**
  *
  * getIntegrationParams: return the structure of integration parameters
  *
  * @returns SIntegrationParams
  *     structure containing integration parameters
  */
  SIntegrationParams  getIntegrationParams() {return IntegrationParams;}

/*------------------------------------------------------------------*/
// function CShieldDose2::setSolarErange
  /**
  *
  * Sets the Solar min and max energy ranges
  *
  * @param dMinS
  *     double: minimum Solar energy range
  * @param dMaxS
  *     double: maximum Solar energy range
  *
  * @returns eGENERIC_ERROR_CODE
  *      eNoError - success
  */
  eGENERIC_ERROR_CODE setSolarErange(const double& dMinS, const double& dMaxS);

/*------------------------------------------------------------------*/
// function CShieldDose2::getSolarErange
  /**
  *
  * Retrieves Solar min and max energy ranges
  *
  * @param dMinS
  *     double: minimum Solar energy range (out)
  * @param dMaxS
  *     double: maximum Solar energy range (out)
  *
  * @returns eGENERIC_ERROR_CODE
  *   eNoError - success
  */
  eGENERIC_ERROR_CODE getSolarErange(double& dMinS, double& dMaxS);

/*------------------------------------------------------------------*/
// function CShieldDose2::setProtonErange
  /**
  *
  * Sets the Proton min and max energy ranges
  *
  * @param dMinS
  *     double: minimum Proton energy range (out)
  * @param dMaxS
  *     double: maximum Proton energy range (out)
  *
  * @returns eGENERIC_ERROR_CODE
  *      eNoError - success
  */
  eGENERIC_ERROR_CODE setProtonErange(const double& dMinP, const double& dMaxP);

/*------------------------------------------------------------------*/
// function CShieldDose2::getProtonErange
  /**
  *
  * Retrieves Proton min and max energy ranges
  * 
  * @param dMinP
  *     double: minimum proton energy range (out)
  * @param dMaxP
  *     double: maximum proton energy range (out)
  *
  * @returns eGENERIC_ERROR_CODE
  *      eNoError - success
  */
  eGENERIC_ERROR_CODE getProtonErange(double& dMinP, double& dMaxP);

/*------------------------------------------------------------------*/
// function CShieldDose2::setElectronErange
  /**
  *
  * Sets the Electron min and max energy ranges
  *
  * @param dMinS
  *     double: Minimum Electron energy range
  * @param dMaxS
  *     double: Maximum Electron energy range
  *
  * @returns eGENERIC_ERROR_CODE
  *   eNoError - success
  */
  eGENERIC_ERROR_CODE setElectronErange(const double& dMinE, const double& dMaxE);

/*------------------------------------------------------------------*/
// function CShieldDose2::getElectronErange
  /**
  *
  * Retrieves Electron min and max energy ranges
  *
  * @param dMinS
  *     double: Minimum Electron energy range (out)
  * @param dMaxS
  *     double: Maximum Electron energy range (out)
  *
  * @returns eGENERIC_ERROR_CODE
  *      eNoError - success
  */
  eGENERIC_ERROR_CODE getElectronErange(double& dMinE, double& dMaxE);

/*------------------------------------------------------------------*/
// function CShieldDose2::setInterpolationType
  /**
  *
  * Sets the desired Interpolation Type
  *
  * @param eInterpol
  *     eInterpolationType: Interpolation Type Enum
  *     * ecsLinear
  *     * ecsSpline
  *
  * @returns eGENERIC_ERROR_CODE
  *   eNoError - success
  */
  eGENERIC_ERROR_CODE setInterpolationType(const ecsInterpolationType& eInterpolationType);

/*------------------------------------------------------------------*/
// function CShieldDose2::getInterpolationType
  /**
  *
  * Retrieves the set Interpolation Type
  *
  * @returns ecsInterpolationType
  *   current interpolation type enum
  */
  ecsInterpolationType getInterpolationType() {return IntegrationParams.ceInterpolationType;}


  //Unit Energy Conversion Factor
/*------------------------------------------------------------------*/
// function CShieldDose2::setUnitEnergyConversionFactor
  /**
  *
  * Sets the unit energy conversion factor (default 1.0).  This is the
  * conversion factor to take the input flux units into #/MeV/s, e.g.,
  * 1000.0 to go from /KeV to /MeV.  Significantly, CrresEle and NasaEle
  * return flux units of #/KeV/s while CrresPro and NasaPro return units
  * of #/MeV/s. Legacy float version.
  *
  * @param fUECFactor
  *     float: unit energy conversion factor
  *
  * @returns eGENERIC_ERROR_CODE
  *   eNoError - success
  */
  eGENERIC_ERROR_CODE setUnitEnergyConversionFactor(const float& fUECFactor);

/*------------------------------------------------------------------*/
// function CShieldDose2::setUnitEnergyConversionFactor
  /**
  *
  * Sets the unit energy conversion factor (default 1.0).  This is the
  * conversion factor to take the input flux units into #/MeV/s, e.g.,
  * 1000.0 to go from /KeV to /MeV.  Significantly, CrresEle and NasaEle
  * return flux units of #/KeV/s while CrresPro and NasaPro return units
  * of #/MeV/s.
  *
  * @param dUECFactor
  *     double: unit energy conversion factor
  *
  * @return eGENERIC_ERROR_CODE
  *   eNoError - success
  */
  eGENERIC_ERROR_CODE setUnitEnergyConversionFactor(const double& dUECFactor);

/*------------------------------------------------------------------*/
// function CShieldDose2::getUnitEnergyConversionFactor
  /**
  *
  * Retrieve the unit energy conversion factor.
  *
  * @returns double
  */
  double getUnitEnergyConversionFactor() {return cdEnergyUnitConvFactor;}

  //Time Interval
/*------------------------------------------------------------------*/
// function CShieldDose2::setTimeInterval
  /**
  *
  * Sets the duration for the dose calculation; legacy version taking float as parameter.
  *
  * @param fDuration
  *     float: duration of the dose calculation
  *
  * @return eGENERIC_ERROR_CODE
  *   eNoError - success
  */
  eGENERIC_ERROR_CODE setTimeInterval(const float& fDuration);

/*------------------------------------------------------------------*/
// function CShieldDose2::setTimeInterval
  /**
  *
  * Sets the duration for the dose calculation.
  *
  * @param dDuration
  *     double: duration of the dose calculation
  *
  * @returns eGENERIC_ERROR_CODE
  *   eNoError - success
  */
  eGENERIC_ERROR_CODE setTimeInterval(const double& dDuration);

/*------------------------------------------------------------------*/
// function CShieldDose2::getTimeInterval
  /**
  *
  * Retrieve the dose calculation time duration.
  *
  * @returns double
  *     duration of dose calculation.
  */
  double getTimeInterval() {return cdDuration;}


/*------------------------------------------------------------------*/
// function CShieldDose2::setTotalWithBrems
  /**
  *
  *  Define setting for including Bremsstrahlung contributions in 'total'.  <br>
  *   class default state = 'true'
  *
  * @param bVerdict
  *     bool: true or false
  *
  */
  void setTotalWithBrems( bool bVerdict=true )
    { cdTotalBremFactor= (bVerdict?1.0:0.0); }

/*------------------------------------------------------------------*/
// function CShieldDose2::getTotalWithBrems
  /**
  *
  * Retrieve the setting for including Bremsstrahlung contributions in 'total'.
  *
  * @returns bool
  *     current state setting
  */
  bool getTotalWithBrems( ) { return (cdTotalBremFactor>0.0); }


/*------------------------------------------------------------------*/
// function CShieldDose2::computeDose
  /**
  *
  * Computes the dose where no sets have been previously performed. Legacy float version.
  *
  * Compute doses rates and store them internally. Call this method
  * first, then one or more of the get methods below to pull back the
  * output you need. All three detector geometries are computed when
  * one of the computes is called.
  *
  * @param fvShieldThicks
  *     float vector: vector of shield thicknesses to use
  * @param estIn
  *     ecsShieldThickUnits: units of passed shield thickness values
  * @param edtIn
  *     ecsDetectorType: enumerated detector type
  * @param enaIn
  *     ecsNuclearAttenMode: enumerated nuclear attenuation mode
  * @param fvTrappedProtonEnergies
  *     float vector: vector of trapped proton energy values
  * @param fvTrappedProtonFluxes
  *     float vector: vector of trapped proton flux values
  * @param fvElectronEnergies
  *     float vector: vector of trapped electron energy values
  * @param fvElectronFluxes
  *     float vector: vector of trapped electron flux values
  * @param fvSolarProtonEnergies
  *     float vector: vector of solar proton energy values
  * @param fvSolarProtonFluences
  *     float vector: vector of solar proton fluences
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
  eRADENV_ERROR_CODE computeDose( const fvector& fvShieldThicks,
                                  const ecsShieldThickUnits& estIn,
                                  const ecsDetectorType& edtIn,
                                  const ecsNuclearAttenMode& enaIn,
                                  const fvector& fvTrappedProtonEnergies,
                                  const fvector& fvTrappedProtonFluxes,
                                  const fvector& fvElectronEnergies,
                                  const fvector& fvElectronFluxes,
                                  const fvector& fvSolarProtonEnergies,
                                  const fvector& fvSolarProtonFluences);

/*------------------------------------------------------------------*/
// function CShieldDose2::computeDose
  /**
  *
  * Computes the dose where no sets have been previously performed.
  *
  * Compute doses rates and store them internally. Call this method
  * first, then one or more of the get methods below to pull back the
  * output you need. All three detector geometries are computed when
  * one of the computes is called.
  *
  * @param dvShieldThicks
  *     double vector: vector of shield thicknesses to use
  * @param estIn
  *     ecsShieldThickUnits: units of passed shield thickness values
  * @param edtIn
  *     ecsDetectorType: enumerated detector type
  * @param enaIn
  *     ecsNuclearAttenMode: enumerated nuclear attenuation mode
  * @param dvTrappedProtonEnergies
  *     double vector: vector of trapped proton energy values
  * @param dvTrappedProtonFluxes
  *     double vector: vector of trapped proton flux values
  * @param dvElectronEnergies
  *     double vector: vector of trapped electron energy values
  * @param dvElectronFluxes
  *     double vector: vector of trapped electron flux values
  * @param dvSolarProtonEnergies
  *     double vector: vector of solar proton energy values
  * @param dvSolarProtonFluences
  *     double vector: vector of solar proton fluences
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
  eRADENV_ERROR_CODE computeDose( const dvector& dvShieldThicks,
                                  const ecsShieldThickUnits& estIn,
                                  const ecsDetectorType& edtIn,
                                  const ecsNuclearAttenMode& enaIn,
                                  const dvector& dvTrappedProtonEnergies,
                                  const dvector& dvTrappedProtonFluxes,
                                  const dvector& dvElectronEnergies,
                                  const dvector& dvElectronFluxes,
                                  const dvector& dvSolarProtonEnergies,
                                  const dvector& dvSolarProtonFluences);

  // in case you only want to compute proton or electron contribution
  // legacy 'float' version
/*------------------------------------------------------------------*/
// function CShieldDose2::computeDose
  /**
  *
  * Computes the dose where only proton or electron contribution wanted. Legacy float version.
  *
  * @param epIn
  *     ecsParticles: enumerated particle type to compute
  * @param fvShieldThicks
  *     float vector: vector of shield thicknesses to use
  * @param estIn
  *     ecsShieldThickUnits: units of passed shield thickness values
  * @param edtIn
  *     ecsDetectorType: enumerated detector type
  * @param enaIn
  *     ecsNuclearAttenMode: enumerated nuclear attenuation mode
  * @param fvEnergies
  *     float vector: vector of particle energy values
  * @param fvFluxes
  *     float vector: vector of particle energy fluxes
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
  eRADENV_ERROR_CODE computeDose( const ecsParticles& epIn,
                                  const fvector& fvShieldThicks,
                                  const ecsShieldThickUnits& estIn,
                                  const ecsDetectorType& edtIn,
                                  const ecsNuclearAttenMode& enaIn,
                                  const fvector& fvEnergies,
                                  const fvector& fvFluxes);

/*------------------------------------------------------------------*/
// function CShieldDose2::computeDose
  /**
  *
  * Computes the dose where only proton or electron contribution wanted.
  *
  * @param epIn
  *     ecsParticles: enumerated particle type to compute
  * @param dvShieldThicks
  *     double vector: vector of shield thicknesses to use
  * @param estIn
  *     ecsShieldThickUnits: units of passed shield thickness values
  * @param edtIn
  *     ecsDetectorType: enumerated detector type
  * @param enaIn
  *     ecsNuclearAttenMode: enumerated nuclear attenuation mode
  * @param dvEnergies
  *     double vector: vector of particle energy values
  * @param dvFluxes
  *     double vector: vector of particle energy fluxes
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
  eRADENV_ERROR_CODE computeDose( const ecsParticles& epIn,
                                  const dvector& dvShieldThicks,
                                  const ecsShieldThickUnits& estIn,
                                  const ecsDetectorType& edtIn,
                                  const ecsNuclearAttenMode& enaIn,
                                  const dvector& dvEnergies,
                                  const dvector& dvFluxes);

  // use this or the next after sets if the params haven't changed
  // legacy 'float' version
/*------------------------------------------------------------------*/
// function CShieldDose2::computeDose
  /**
  *
  * Computes the dose for all particles when set settable factors unchanged. Legacy float version.
  *
  * @param fvTrappedProtonEnergies
  *     float vector: vector of trapped proton energy values
  * @param fvTrappedProtonFluxes
  *     float vector: vector of trapped proton flux values
  * @param fvElectronEnergies
  *     float vector: vector of trapped electron energy values
  * @param fvElectronFluxes
  *     float vector: vector of trapped electron flux values
  * @param fvSolarProtonEnergies
  *     float vector: vector of solar proton energy values
  * @param fvSolarProtonFluences
  *     float vector: vector of solar proton fluences
  *
  * @returns eRADENV_ERROR_CODE
  *     eNoError - success
  */
  // legacy 'float' version
  eRADENV_ERROR_CODE computeDose( const fvector& fvTrappedProtonEnergies,
                                  const fvector& fvTrappedProtonFluxes,
                                  const fvector& fvElectronEnergies,
                                  const fvector& fvElectronFluxes,
                                  const fvector& fvSolarProtonEnergies,
                                  const fvector& fvSolarProtonFluences);

/*------------------------------------------------------------------*/
// function CShieldDose2::computeDose
  /**
  *
  * Computes the dose for all particles when set settable factors unchanged.
  *
  * @param dvTrappedProtonEnergies
  *     double vector: vector of trapped proton energy values
  * @param dvTrappedProtonFluxes
  *     double vector: vector of trapped proton flux values
  * @param dvElectronEnergies
  *     double vector: vector of trapped electron energy values
  * @param dvElectronFluxes
  *     double vector: vector of trapped electron flux values
  * @param dvSolarProtonEnergies
  *     double vector: vector of solar proton energy values
  * @param dvSolarProtonFluences
  *     double vector: vector of solar proton fluences
  *
  * @returns eRADENV_ERROR_CODE
  *     eNoError - success
  */
  eRADENV_ERROR_CODE computeDose( const dvector& dvTrappedProtonEnergies,
                                  const dvector& dvTrappedProtonFluxes,
                                  const dvector& dvElectronEnergies,
                                  const dvector& dvElectronFluxes,
                                  const dvector& dvSolarProtonEnergies,
                                  const dvector& dvSolarProtonFluences);

  // legacy 'float' version
/*------------------------------------------------------------------*/
// function CShieldDose2::computeDose
  /**
  *
  * Computes the dose for a single particle type. Legacy float version.
  * Computes after sets or the other computes if the params haven't changed.
  *
  * @param epIn
  *     ecsParticles: enumerated particle type to compute
  * @param fvEnergies
  *     double vector: vector of particle energy values
  * @param fvFluxes
  *     float vector: vector of particle energy fluxes
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
  eRADENV_ERROR_CODE computeDose( const ecsParticles& epIn,
                                  const fvector& fvEnergies,
                                  const fvector& fvFluxes);

/*------------------------------------------------------------------*/
// function CShieldDose2::computeDose
  /**
  *
  * Computes the dose for a single particle type.
  *
  * @param epIn
  *     ecsParticles: enumerated particle type to compute
  *     * ecsTrappedProtons  for 'protons'
  *     * ecsElectrons       for 'electrons'
  *     * ecsSolarProtons    for 'solar protons'
  * @param dvEnergies
  *     double vector: vector of particle energy values
  * @param dvFluxes
  *     double vecto: vector of particle energy fluxes
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
  eRADENV_ERROR_CODE computeDose( const ecsParticles& epIn,
                                  const dvector& dvEnergies,
                                  const dvector& dvFluxes);

/*------------------------------------------------------------------*/
// function CShieldDose2::computeDoseStr
  /**
  *
  * Computes the dose for a single particle type. String version.
  *
  * @param sParticles
  *     string: particle type to compute
  *     * 'protons', 'p'
  *     * 'electrons', 'e'
  *     * 'solar', 'solarprotons', 's'
  * @param dvEnergies
  *     double vector: vector of particle energy values
  * @param dvFluxes
  *     double vector: vector of particle energy fluxes
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
  eRADENV_ERROR_CODE computeDoseStr( const string& sParticles,
                                     const dvector& dvEnergies,
                                     const dvector& dvFluxes);

   // get routines to pull out the data; the first pulls the dose rates
   // for one of the detector geometries only, the other pulls all of
   // them. Output vectors will be resized as required.

   // legacy float versions

/*------------------------------------------------------------------*/
// function CShieldDose2::getDose
  /**
  *
  * Get previously computed dose rates for one of the detector geometries. Legacy float version.
  *
  * @param edgIn
  *     ecsDetectorGeometry: Detector geometry
  *
  * @param[out] psfrvDoseOut
  *     SDoseRateVec pointer: set of dose rates retrieved (out)
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
   eRADENV_ERROR_CODE getDose( const ecsDetectorGeometry& edgIn,
                               SDoseRateVec* psfrvDoseOut);

/*------------------------------------------------------------------*/
// function CShieldDose2::getAllDose
  /**
  *
  * Get all of the previously computed dose rates. Legacy float version.
  *
  * @param[out] psfrvSemiSlabDose
  *     SDoseRateVec pointer: dose rates for semi infinite slab geometry
  * @param[out] psfrvFinSlabDose
  *     SDoseRateVec pointer: dose rates for finite slab geometry
  * @param[out] psfrvSphDose
  *     SDoseRateVec pointer: dose rates for spherical geometry
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
   eRADENV_ERROR_CODE getAllDose( SDoseRateVec* psfrvSemiSlabDose,
                                  SDoseRateVec* psfrvFinSlabDose,
                                  SDoseRateVec* psfrvSphDose);

   // overloads with outputs that return double version of structure
/*------------------------------------------------------------------*/
// function CShieldDose2::getDose
  /**
  *
  * Get previously computed dose rates for one of the detector geometries.
  *
  * @param edgIn
  *     ecsDetectorGeometry: Detector geometry
  * @param[out] psdrvDoseOut
  *     SDoseRateDVec pointer: set of dose rates retrieved (out)
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
   eRADENV_ERROR_CODE getDose( const ecsDetectorGeometry& edgIn,
                               SDoseRateDVec* psdrvDoseOut);

/*------------------------------------------------------------------*/
// function CShieldDose2::getDoseStr
  /**
  *
  * Get previously computed dose rates for one of the detector geometries. String version.
  *
  * @param sGeometry
  *     string: Detector geometry
  *     * 'spherical'
  *     * 'finiteslab'
  *     * 'semiinfiniteslab'
  *
  * @param[out] psdrvDoseOut
  *     SDoseRateDVec pointer: set of dose rates retrieved (out)
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
   eRADENV_ERROR_CODE getDoseStr( const string& sGeometry,
                                  SDoseRateDVec* psdrvDoseOut);

/*------------------------------------------------------------------*/
// function CShieldDose2::getAllDose
  /**
  *
  * Get all of the previously computed dose rates.
  *
  * @param[out] psdrvSemiSlabDose
  *     SDoseRateDVec pointer: dose rates for semi infinite slab geometry
  * @param[out] psdrvFinSlabDose
  *     SDoseRateDVec pointer: dose rates for finite slab geometry
  * @param[out] psdrvSphDose
  *     SDoseRateDVec pointer: dose rates for spherical geometry
  *
  * @returns eRADENV_ERROR_CODE
  *   eNoError - success
  */
   eRADENV_ERROR_CODE getAllDose( SDoseRateDVec* psdrvSemiSlabDose,
                                  SDoseRateDVec* psdrvFinSlabDose,
                                  SDoseRateDVec* psdrvSphDose);

/*------------------------------------------------------------------*/
// function CShieldDose2::getParticlesEnum
  /**
   * getParticlesEnum: utility function to retrieve ecsParticles enum from string
   *
   * @param sParticles
   *     const string: particle type string
   *     * 'protons', 'p'
   *     * 'electrons', 'e'
   *     * 'solar', 'solarprotons', 's'
   *
   * @param[out] eParticles
   *     ecsParticle: returned corresponding enumeration value
   *
   * @returns int
   */
   int getParticlesEnum( const string& sParticles,
                         ecsParticles& eParticles);

/*------------------------------------------------------------------*/
// function CShieldDose2::getGeometryEnum
  /**
   * getGeometryEnum: utility function to retrieve ecsDetectorGeometry enum from string
   *
   * @param sGeometry
   *     const string: geometry type string
   *     * 'spherical'
   *     * 'finiteslab'
   *     * 'semiinfiniteslab'
   *
   * @param[out] eGeometry
   *     ecsDetectorGeometry: returned corresponding enumeration value
   *
   * @returns int
   */
   int getGeometryEnum( const string& sGeometry,
                        ecsDetectorGeometry& eGeometry);

 /*------------------------------------------------------------------*/
// function CShieldDose2::printout
  /**
   * Prints all dose data.
   */
   void printout();

   // Message routines
/*------------------------------------------------------------------*/
// function CShieldDose2::message
  /**
  * Return message for a given error code.
  *
  * @param code
  *     eRADENV_ERROR_CODE: error code enum
  *
  * @return
  *    string: associated message
  */
   const string message(const eRADENV_ERROR_CODE &code);

/*------------------------------------------------------------------*/
// function CShieldDose2::message
  /**
  * Return error message from underlying class
  *
  * @return
  *    string: error message
  */
   const string message() { return(GenericModel::message()); }

 private:

   static char* cDetectors[];
   static char* cVersion;

   dvector cdvShieldThicks;
   ecsShieldThickUnits ceShieldThickUnits;

   // internal database reset when the following two change:
   ecsDetectorType ceDetectorType;
   ecsNuclearAttenMode ceNuclearAttenMode;

   // HDF5 format persistent database data
   CShieldDoseDb cDatabase;

   // dose rate vectors
   SDoseRateDVec sdrvSemiSlabDose;
   SDoseRateDVec sdrvFinSlabDose;
   SDoseRateDVec sdrvSphDose;

   int   ciThickParamsSize;
   int   ciAttenModeIndex;
   int   ciAttenModePlusNeutrons;
   double cdEnergyUnitConvFactor;
   double cdDuration;
   double cdTotalBremFactor;

    // dirty flags
   bool cbDatabaseOk;
   bool cbThickparamsOk;
   bool cbMesharraysOk;

   // constant values
   struct sd2const {
      int nbege;
      double zmin;
      double radcon;
      double enmu;
      double zcon;
      double zmcon;
      char *det[SD2MAXNUMDETTYPES];
      char *version;
   } sd2const;

   // thickness parameters
   struct thickparams {
      dvector z;
      dvector zm;
      dvector zmm;
      dvector zl;
   } thickparams;

   // integration parameters
   SIntegrationParams IntegrationParams;

   struct mesharrays {
      int nlensb;
      int nlstsb;
      int nfstsb;
      int nfstpb;
      int nlstpb;
      int nlenpb;
      double delp;
      double dele;
      double dep;
      double dee;
      double eminu;
      double emaxu;
      double eminul;
      dvector tpl;
      dvector tpp;
      dvector enewt;
      dvector tel;
      dvector tee;
      vector <dvector> gp;
      vector <vector <dvector> > gee;
      vector <vector <dvector> > gb;
   } mesharrays;

   struct protondata {
      int mmaxp;
      int nmaxp;
      int lmaxp;
      int kmaxp;
      dvector ep;
      dvector rp;
      dvector tepn;
      dvector fepn;
      dvector tp;
      dvector zrp;
      dvector rpb;
      dvector rpc;
      dvector rpd;
      dvector fepnb;
      dvector fepnc;
      dvector fepnd;
      vector <dvector> dalp;
      vector <dvector> dalpb;
      vector <dvector> dalpc;
      vector <dvector> dalpd;
      vector <dvector> dratp;
      vector <dvector> dratpb;
      vector <dvector> dratpc;
      vector <dvector> dratpd;
   } protondata;

   struct electronbremdata {
      int nmaxe;
      int mmaxe;
      int lmaxs;
      int lmaxe;
      int lmaxt;
      int lmaxb;
      int nlene;
      dvector ee;
      dvector re;
      dvector ye;
      dvector te;
      dvector ar;
      dvector rs;
      dvector bs;
      dvector zre;
      dvector zs;
      dvector zb;
      dvector reb;
      dvector rec;
      dvector red;
      dvector rsb;
      dvector rsc;
      dvector rsd;
      dvector arb;
      dvector arc;
      dvector ard;
      dvector yeb;
      dvector yec;
      dvector yed;
      vector <dvector> dale;
      vector <dvector> dalb;
      vector <dvector> daleb;
      vector <dvector> dalec;
      vector <dvector> daled;
      vector <dvector> dalbb;
      vector <dvector> dalbc;
      vector <dvector> dalbd;
      vector <vector <dvector> > drate;
      vector <vector <dvector> > dratb;
      vector <vector <dvector> > drateb;
      vector <vector <dvector> > dratec;
      vector <vector <dvector> > drated;
      vector <vector <dvector> > dratbb;
      vector <vector <dvector> > dratbc;
      vector <vector <dvector> > dratbd;
   } electronbremdata;


   //
    // protected internal methods
  //

   void InitConstants();
   eRADENV_ERROR_CODE computeProtonData();
   eRADENV_ERROR_CODE computeElectronBremData();
   eRADENV_ERROR_CODE computeThickParams();
   eRADENV_ERROR_CODE computeMeshArrays();

   // internal calculation methods ported from shielddose2 fortran code
   // the names have been modified to be more descriptive

   // cubic spline interpolation
   void splineInterpCoefficients( int n, const dvector& x, const dvector& y,
                                dvector &bb, dvector &cc, dvector &dd);

   // linear interpolation
   void linearInterpCoefficients( int nmax, const dvector& x, const dvector& f,
                                dvector &bb, dvector &cc, dvector &dd);

   void logSpectrumNumPts( double eminb, double de, int npts, double emin, double emax,
                           int& nfst, int& nlst, int& nlen) ;

   // binary search
   double bSplineEval(double s, int n,
                      dvector::const_iterator x, dvector::const_iterator y,
                dvector::const_iterator b, dvector::const_iterator c, dvector::const_iterator d);

   void integratedSpectrum(int jmax, const dvector& eps, const dvector& s, double eunit,
        double eminb, double del, int npts, int nfst,
        int nlst, int nlen, const dvector& t, const dvector& t1,
        dvector& spp);

   // integration
   double simpsonIntegrator(double delta, const dvector& g, int nfst, int nlst, int n);

   void sphere(const dvector& z1, const dvector& dose, const int& imax, dvector *doshptr);

   void sphere(const dvector& z1, const dvector& dose, int imax, dvector& dosh);

   // utility function for conversion from float to double implementation
   void initDoubleFromFloat(const fvector& in, dvector& out);
};

#endif

/******************************************************************************

File: Ae9Ap9Model.h

Description: Main entry point into the model layer of the Ae9Ap9 project
  The Application layer classes interact with the model layer through an
  Ae9Ap9Model object. This class retrieves its initial state from the 
  persistant data repository, then manages the data for subsequent 
  requests and calculations in-memory.

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

  Version      Date       Notes
  1.0         09/02/2009  created 
  
******************************************************************************/
#ifndef AE9AP9MODEL_H
#define AE9AP9MODEL_H

#include "AEEnums.h"
#include "AETypes.h"
#include "AEErrorHandler.h"
#include "AEDBModelBase.h"
#include "AEModelGrid.h"
#include "AEFluxWeightCalculator.h"
#include "AEScenario.h"

#include "DataBaseNames.h"

//
// Uncomment the following statement to enable writing debug
// information to an output stream.
//
//#define AE9_DEBUG_TRACE_ON


#ifdef AE9_DEBUG_TRACE_ON
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#endif

namespace ae9ap9 
{
    class Ae9Ap9Model : public AEErrorHandler
    {
    public:
        // constructor

        Ae9Ap9Model();

        // destructor

        ~Ae9Ap9Model();

        // data member accessor methods

        //
        // Set this to the path and file name of the HDF5 file containing the data for the selected model
        // example: model.setModelDBFile( "../modelData/TEM2_runtime_tables.mat" ;
        //
        int setModel( const string& strModel ); // AE9 AP9 PLASMAE/H/He/O
        string getModel( ) { return m_strModel; }
        int setModelDBDir( const string& strModelDBDir );
        string getModelDBDir( ) { return m_strModelDBDir; }
        int setModelDBFile( const string& strModelDBFile );
        const string& getModelDBFile() { return m_strModelDBFile; }

        // allow access to model identification strings read from data source file
        const string& getModelName() { return m_strModelName; }
        const string& getModelSpecies() { return m_strModelSpecies; }

        //
        // Set this to the path and file of the neural network HDF5 file for the K/Phi
        // model. ie: model.setKPhiDBFile( "../modelData/fastPhi_net.mat" ;
        //

        int setKPhiDBFile( const string& strKPhiDBFile );
        const string& getKPhiDBFile() { return m_strKPhiDBFile; }

        //
        // Set this to the path and file of the neural network HDF5 file for the K/HMin
        // model. ie: model.setKHMinDBFile( "../modelData/fastHmin_net.mat" ;
        //

        int setKHMinDBFile( const string& strKHMinDBFile );
        const string& getKHMinDBFile() { return m_strKHMinDBFile; }

        //
        // Set this to the path and file of the HF5 file containing the magnetic field model to be used 
        // example: model.setMagfieldModelDataSource( "../modelData/igrfDB.h5" ;
        // [ Defaults to igrfDB.h5 at the same path as DataSource ]
        //
        int setMagfieldDBFile(const string& strMagfieldDBFile );
        const string& getMagfieldDBFile() { return m_strMagfieldDBFile; }
        //
        // Retrieve the spatial domain handler object 
        //
        AEModelSpatialDomain& getSpatialDomain() { return m_modelSpatialDomain; }

        // 
        // Load the model from persistant storage
        //
        int loadModelDB();

        // 
        // Initialization of the flux environment settings:
        // Call to reset any or all environment data that requires
        // recalculation of flux weights (flux type, energies,
        // times, positions, directions). Note: the vvdEnergies
        // parameter only accepts a 2nd column of energy values
        // when passed in the context of a 2 pt differential flux request.

        // Set environment for omni-directional flux
        int setFluxEnvironment( eAEEnergyFluxType eFluxType, 
                                const vdvector& vvdEnergies,
                                const dvector& vdTimes, 
                                emfCoordSys    eCoordSys,
                                const dvector& vdCoordsAxis1,
                                const dvector& vdCoordsAxis2, 
                                const dvector& vdCoordsAxis3 );

        // Set environment for fixed set of pitch angles
        int setFluxEnvironment( eAEEnergyFluxType eFluxType, 
                                const vdvector& vvdEnergies,
                                const dvector& vdTimes, 
                                emfCoordSys    eCoordSys,
                                const dvector& vdCoordsAxis1,
                                const dvector& vdCoordsAxis2, 
                                const dvector& vdCoordsAxis3,
                                const dvector& vdPitchAngles );

        // Set environment for directional flux
        int setFluxEnvironment( eAEEnergyFluxType eFluxType, 
                                const vdvector& vvdEnergies,
                                const dvector& vdTimes, 
                                emfCoordSys    eCoordSys,
                                const dvector& vdCoordsAxis1,
                                const dvector& vdCoordsAxis2, 
                                const dvector& vdCoordsAxis3,
                                const vdvector& vvdFluxDir1, 
                                const vdvector& vvdFluxDir2,
                                const vdvector& vvdFluxDir3 );

        // Set environment for ephemeris-associated set of pitch angles
        int setFluxEnvironment( eAEEnergyFluxType eFluxType, 
                                const vdvector& vvdEnergies,
                                const dvector& vdTimes, 
                                emfCoordSys    eCoordSys,
                                const dvector& vdCoordsAxis1,
                                const dvector& vdCoordsAxis2, 
                                const dvector& vdCoordsAxis3,
                                const vdvector& vvdPitchAngles );

  // overloads for more user-friendly interface using strings instead of enumerations
        // Set environment for omni-directional flux
        int setFluxEnvironment( const string& strFluxType, 
                                const dvector& vdEnergies, // 1-d energy list
                                const dvector& vdEnergies2, // 1-d energy list
                                const dvector& vdTimes, 
                                const string& strCoordSys,
                                const string& strCoordUnits,
                                const dvector& vdCoordsAxis1,
                                const dvector& vdCoordsAxis2, 
                                const dvector& vdCoordsAxis3 );

        // Set environment for fixed set of pitch angles
        int setFluxEnvironment( const string& strFluxType, 
                                const dvector& vdEnergies, // 1-d energy list
                                const dvector& vdEnergies2, // 1-d energy list
                                const dvector& vdTimes, 
                                const string& strCoordSys,
                                const string& strCoordUnits,
                                const dvector& vdCoordsAxis1,
                                const dvector& vdCoordsAxis2, 
                                const dvector& vdCoordsAxis3,
                                const dvector& vdPitchAngles );

        // Set environment for directional flux
        int setFluxEnvironment( const string& strFluxType, 
                                const dvector& vdEnergies, // 1-d energy list
                                const dvector& vdEnergies2, // 1-d energy list
                                const dvector& vdTimes, 
                                const string& strCoordSys,
                                const string& strCoordUnits,
                                const dvector& vdCoordsAxis1,
                                const dvector& vdCoordsAxis2, 
                                const dvector& vdCoordsAxis3,
                                const vdvector& vvdFluxDir1, 
                                const vdvector& vvdFluxDir2,
                                const vdvector& vvdFluxDir3 );

        // Set environment for ephemeris-associated set of pitch angles
        int setFluxEnvironment( const string& strFluxType, 
                                const dvector& vdEnergies, // 1-d energy list
                                const dvector& vdEnergies2, // 1-d energy list
                                const dvector& vdTimes, 
                                const string& strCoordSys,
                                const string& strCoordUnits,
                                const dvector& vdCoordsAxis1,
                                const dvector& vdCoordsAxis2, 
                                const dvector& vdCoordsAxis3,
                                const vdvector& vvdPitchAngles );

        int getNumTimes( ) { return m_iNumTimes; }
        int getNumEnergies( ) { return m_iNumEnergies; }
        int getNumDirections( ) { return m_iNumDirections; }

        // retrieve pitch angle values (particularly useful when direction vectors specified)
        int getPitchAngles ( vdvector&   vvdPitchAngles );
        // retrieve geomagnetic and adiabatic invariant coordinate values for multiple
        //  pitch angles at each time, calclulated during calls to setFluxEnvironment
        //
        // The computeFlux methods compute and return flux values for the
        // current set of environment conditions, as passed to one of the
        // setFluxEnvironment methods. Note: flux weights (h) are not recalculated.
        //

        // Compute flux for a mean flux environment (date,energy,direction)

        int computeFluxMean( vvdvector& vvvdFluxData );
        int flyinMean( vvdvector& vvvdFluxData );
        // 2D version as a convience for flyinMean only:
        int flyinMean( vdvector& vvdFluxData );

        // Compute flux for a statistically perturbed, time invariant mean flux environment (date,energy,direction)
        // Note: iScenario specified one of up to 999 perturbed mean scenarios (1..999)

        int computeFluxPerturbedMean( int iScenario,
                                      vvdvector& vvvdFluxData );
        int flyinPerturbedMean( int iScenario,
                                vvdvector& vvvdFluxData );

        // Compute flux for a particular percentile flux environment (date,energy,direction)
        // Note: iPercentile specifies an integer percentile flux (1..99)

        int computeFluxPercentile( int iPercentile,
                                   vvdvector& vvvdFluxData );
        int flyinPercentile( int iPercentile,
                             vvdvector& vvvdFluxData );

        // Compute flux for monte carlo (time variant) scenario flux environments (date,energy,direction)
        // Note: dEpoch specifies the start date for time variant scenarios in modified julian date format
        //       iScenario specified one of up to 999 monte carlo scenarios (1..999)
        // Note2: bPerturbFluxMap supports disabling flux map perturbations for validation where those
        //        perturbations would dwarf observed physical effects needed for validation

        int computeFluxScenario( double dEpoch,
                                 int iScenario,
                                 vvdvector& vvvdFluxData,
                                 bool bPerturbFluxMap = true );
        int flyinScenario( double dEpoch,
                           int iScenario,
                           vvdvector& vvvdFluxData,
                           bool bPerturbFluxMap = true );

        // utility function, useful for omnidirectional flux results
        int reduceDataDimension( const vvdvector& vvvdData,
                                 vdvector& vvdData );
        // public accessors

        // Retrieves a list of pitch angles used in the computation of omnidirectional flux

        void getDefaultPitchAngles( dvector& vdPitchAngles) { vdPitchAngles = m_vdDefaultPitchAngles; }

#ifdef AE9_DEBUG_TRACE_ON
        // output stream used to dump intermediate results (settable by client code)
        static ostream* ae9dbgout;
#endif

    protected:

        //
        // internal methods
        //

        int expandEnvVarPath( const std::string& strEnvPath,
                              std::string& strExpandedPath,
                              const std::string& strKey );

        int computeFluxInternal( AEScenario& scenario,
                                 vvdvector& vvvdFluxData );

        int getAndSplitTheta( ae9ap9::bdvector& bvdMedian,
                              ae9ap9::bdvector& bvd95Minus50Pctile );

        int getSTheta( ae9ap9::bcmdmatrix& bmdSTheta );

        int getW( ae9ap9::bcmdmatrix& bmdW );

        void setDefaultPitchAngles();

        // protected data members

        string      m_strModel;      // model name, needed for automatic file naming
        string      m_strModelDBDir; // dir for automatic file naming

        string      m_strModelName;               // Model name defined in DataSource Hdf5 file
        string      m_strModelSpecies;            // Model Species defined in DataSource Hdf5 file

        string      m_strModelDBFile;             // Path and data file of persistant model data

        AEFluxWeightCalculator m_fluxWtCalc;      // Flux weight calculator

        string      m_strKPhiDBFile;              // Path and file for K, Phi coord calculator neural network

        string      m_strKHMinDBFile;             // Path and file for K, HMin coord calculator neural network

        string      m_strMagfieldDBFile;          // Path and file for magnetic field model database

        AEDBModelBase* m_pDbModel;                // Model data read from persistant storage

        AEModelSpatialDomain m_modelSpatialDomain; // Spatial domain object manages spatial data

        bool        m_bComputeOmnidirectional;    // compute omnidirectional flux

        int         m_iNumTimes;                  // Size of time dimension in current environment settings

        int         m_iNumEnergies;               // Size of energy dimension in current environment settings

        int         m_iNumDirections;             // Size of direction dimension in current environment settings

        dvector     m_vdDefaultPitchAngles;       // Pitch angles calculated, unless overridden

        std::vector<double>     m_vdTimes;        // keep times for advancing through scenarios

        ae9ap9::bcmdmatrix    m_bmdSThetaTranspose; // read 1 copy of large DynScenario param (instead of 1 /scenario/setenv)
        ae9ap9::bcmdmatrix    m_bmdWTranspose;      // read 1 copy of huge history param

        bool        m_bDataBaseLoaded;

        bool        m_bSupportCompMean;           // flux computations supported by model database
        bool        m_bSupportCompMedian;
        bool        m_bSupportCompPercentile;
        bool        m_bSupportCompPerturbedMean;
        bool        m_bSupportCompScenario;
        bool        m_bSupportCompUnperturbedScenario;

        bool        m_bUseUniformPerturbations;   // In p.m. scenario, use either uniform or gaussian (default) perturbations

        vdvector    m_vvdPitchAngles;

        int getCoordSysEnum( const std::string& strCoordSys, 
                             const std::string& strCoordUnits,
                             emfCoordSys& eCoordSys );
        int getFluxTypeEnum( const std::string& strFluxType, 
                             eAEEnergyFluxType& eFluxType );
        int checkFileExists( const string& strFileName );
        int checkDirExists( const string& strDirName );
        string appendDirDelim( const string strDirName );

    private:

        // Defined private to prevent auto-creation, no reason for them to exist

        Ae9Ap9Model( const Ae9Ap9Model& source ); 
        Ae9Ap9Model& operator =( const Ae9Ap9Model& source ); 

    };

} /*namespace*/

#endif

/******************************************************************************
File: CRoederPlasma.h

Description: 

Implementation of the CAMMICE/MICS empirical plasma flux model described in :
   JL Roeder, MW Chen and JF Fennell (2005) 
   "Empirical models of the low-energy plasma in the inner magnetosphere" 
   Space Weather, 3,S12B06  doi:10.1029/2005SW000161.
   
This model consists of average plasma flux values measured by satellite 
instruments, binned by magnetic local time and L, at the magnetic equator.  
There are 12 MLT bins, of 2 hours each; 16 Earth radii bins, 2.0 to 10.0 
in steps of 0.5.
Plasma flux data is available for up to eight species, omnidirectional or 18 
bins of pitch angles, and twelve energy ranges.
Two data sets are available : one including all flux measurement values, and 
the other excluding flux measurements taken when the DST index was below -100.

The plasma flux values are mapped to other latitudes (up to +-70 GMlat) using 
the mapping of local pitch angle bins to their associated fraction of the 
equatorial pitch angle bin.

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

  Version      Date       Notes
  1.0          04/15/2010 Created
  1.1          08/03/2010 Revised off-equatorial flux mapping
  1.2          09/22/2015 Added methods for better usage, mimic other models
  
******************************************************************************/
#ifndef CROEDERPLASMA_H
#define CROEDERPLASMA_H

#include <string>
#include <vector>
using namespace std;

#include "GenericModel.h"
#include "CRoederPlasmaEnum.h"
#include "CMagfield.h"

//  Roeder Database Configuration
typedef struct RoederPlasmaStruct {
    float ave_pa[12][16][8][18][12];
    float sda_pa[12][16][8][18][12];
    float avt_omni[12][16][4][12];
    float sdt_omni[12][16][4][12];
    int npts[12][16][8][18][12];
    } RoederPlasmaStruct;

//
//  Roeder Plasmasphere Model Class Definition
//
class CRoederPlasma : public GenericModel
{
    public:
        CRoederPlasma( );
        ~CRoederPlasma( );
    
        eGENERIC_ERROR_CODE Initialize( const std::string& sDBFullPath );
        bool isInitialized() { return cbModelInitialized; }

        int setMagfieldDBFile( const std::string& strMagfieldDBFile );
        eMAGFIELD_ERROR_CODE setMagField ( eRoederMagField eDbMagField, CMagfield& magfieldModel );
        eRoederMagField getMagField () { return ceDbMagField; };

        void setDataSelect ( eRoederDataSelect eDbSegment ) { ceDbSegment = eDbSegment; }
        eRoederDataSelect getDataSelect () { return ceDbSegment; }

        void setSpecie ( eRoederSpecie eSpecie ) { ceSpecie = eSpecie; };
        eRoederSpecie getSpecie ( ) { return ceSpecie; }
    
        void setPitchAngleBin ( eRoederPitchAngleBin ePitchAngleBin ) { cePitchAngleBin = ePitchAngleBin; }
        eRoederPitchAngleBin getPitchAngleBin ( ) { return cePitchAngleBin; }

        eMAGFIELD_ERROR_CODE setDateTime ( int iYear, int iDdd, double dGmtSec, CMagfield& magfieldModel );
        void getDateTime ( int *piYear, int *piDdd, double *pdGmtsec );
    
        eERROR_CODE getRoederEquatorialFluxes ( double dGMLocalTime,
                                                double dLShell, 
                                                double dPitchAngleMin, 
                                                double dPitchAngleMax, 
                                                double *pdaFlux );
        eMAGFIELD_ERROR_CODE setMagField ( eRoederMagField eDbMagField, CMagfield *pMagfield );
        eERROR_CODE getRoederEquatorialFluxes ( const double& dGMLocalTime, 
                                                const double& dLShell, 
                                                const double& dPitchAngleMin,
                                                const double& dPitchAngleMax,
                                                dvector& vdFlux );
        eERROR_CODE getRoederFluxes ( const double& dTime,
                                      const S3Coord& s3coord_rllkm,  // requires RLLinKM
                                      dvector& vdFlux );
        eERROR_CODE getRoederFluxes ( const double& dTime,
                                      const S3Coord& s3coord_rllkm,  // requires RLLinKM
                                      eRoederSpecie eSpecie,
                                      dvector& vdFlux );

        eERROR_CODE getRoederFluxes ( double dGGLatitude, 
                                      double dGGLongitude,
                                      double dRadius, 
                                      double *pdaFlux, 
                                      CMagfield& magfieldModel );

        eERROR_CODE getRoederFluxes ( int iYear, 
                                      int iDdd, 
                                      double dGmtSec, 
                                      double dGGLatitude, 
                                      double dGGLongitude, 
                                      double dRadius, 
                                      double *pdaFlux, 
                                      CMagfield& magfieldModel );

                    //     latitude & longitude in degrees
                    //     radius : less than 100 is interpreted as in units of RadiusEarth
                    //              greater than 100 is interpreted as in units of km

        void getGMInfo ( double *pdLShell, double *pdGMLocalTime, double *pdBB0Ratio );

        eGENERIC_ERROR_CODE read( Hdf5ToolKit* pDB );         // read from HDF5 db file

    protected:

        // LoadRoederFile method not intended for current use; exists only to support
        // generation of HDF5 format database from original source data files from utility
        // in the ModelDataGen directory.

        // internal data members

        //  current model date and time
        int ciYear;
        int ciDdd;
        double cdGmtSec;

        //  model parameter settings
        eRoederMagField ceDbMagField;
        eRoederDataSelect ceDbSegment;
        eRoederSpecie ceSpecie;
        eRoederPitchAngleBin cePitchAngleBin;

        // vector of pointers to structures containing database tables
        vector <RoederPlasmaStruct *> cvpsPlasmaData;
    
        double cdLShellMin;
        double cdLShellDelta;
        double cdMagLocalTimeDelta;
        double cdPitchAngleBinDelta;
        double cdRadiusEarth;
        double cdGMLatLimit;
        double cdLShell;
        double cdGMLocalTime;
        double cdBB0Ratio;

        CMagfield               *m_pMagfield;
        // angle conversion constants
        double dtor; 
        double rtod;

        bool cbModelInitialized;
    
};

#endif

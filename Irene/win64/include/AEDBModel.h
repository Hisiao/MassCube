/******************************************************************************
$HeadURL$

 File: AEDBModel.h

 Description: Class declaration AEDBModel class in the database layer of the
   Ae9Ap9Model. Clases in the database layer wrap the persistent representation
   and provide i/o for analogous classes in the model layer. Model data are
   persisted in an HDF5 format database. This software is thus dependent on
   the HDF5 interface library.

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
  1.0          10/01/2009 Created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBMODEL_H
#define AEDBMODEL_H

#include <vector>
#include <string>
using namespace std;

#include "AETypes.h"
#include "AEDBModelBase.h"
#include "AEDBManagedResource.h"
#include "AEDBModelSpatialDomain.h"

namespace ae9ap9 
{
    class AEDBModel : public AEDBModelBase, public AEDBManagedResource
    {
    public:
        // construction, destruction
        AEDBModel();
        ~AEDBModel();

        // public methods
        int read();
        /*int write();*/

        // public accessors

        const string& getDataFile() 
                 { return m_strDataFile; }
        void setDataFile( const string& strDataFile ) 
                 { m_strDataFile = strDataFile; }

        const string& getModelName() 
                 { return m_strModelName; }
        void setModelName( const string& strModelName ) 
                 { m_strModelName = strModelName; }

        const string& getModelSpecies() 
                 { return m_strModelSpecies; }
        void setModelSpecies( const string& strModelSpecies ) 
                 { m_strModelSpecies = strModelSpecies; }

        void getC( dvector& vdC, uivector& vuiCDims ) 
                 {vdC = m_vdC; vuiCDims = m_vuiCDims;}
        void setC( const dvector& vdC, const uivector& vuiCDims ) 
                 {m_vdC = vdC; m_vuiCDims = vuiCDims;}

        // As of version 1.0, G is a set of GCount matrices
        int  getGCount() { return int(m_vvdG.size()); }
        int getG( int index, dvector& vdG, uivector& vuiGDims );
        void clearG() { m_vvdG.clear(); m_vvuiGDims.clear(); }
        void addG( const dvector& vdG, const uivector& vuiGDims ) 
                 {m_vvdG.push_back( vdG ); m_vvuiGDims.push_back( vuiGDims );}

        void getQ( dvector& vdQ, uivector& vuiQDims ) 
                 {vdQ = m_vdQ; vuiQDims = m_vuiQDims;}
        void setQ( const dvector& vdQ, const uivector& vuiQDims ) 
                 {m_vdQ = vdQ; m_vuiQDims = vuiQDims;}

        // Stheta and W can be large, but still retrieve in one call
        void getSthetaDims( uivector& vuiSthetaDims ) 
                 { vuiSthetaDims = m_vuiSthetaDims; }
        void getWDims( uivector& vuiWDims ) 
                 { vuiWDims = m_vuiWDims; }
        int getMatrixValues( const std::string& strName,
                             const uivector vuiMatrixDims,
                             dvector& vdValues);

        void getDt( dvector& vdDt, uivector& vuiDtDims )
                  {vdDt = m_vdDt; vuiDtDims = m_vuiDtDims;}
        void setDt( const dvector& vdDt, const uivector& vuiDtDims )
                  {m_vdDt = vdDt; m_vuiDtDims = vuiDtDims;}

        // return "in-place" for performance
        dvector& getTheta( uivector& vuiThetaDims ) { vuiThetaDims = m_vuiThetaDims; return m_vdTheta; }
        void getTheta( dvector& vdTheta, uivector& vuiThetaDims )
                     {vdTheta = m_vdTheta; vuiThetaDims = m_vuiThetaDims;}
        void setTheta( const dvector& vdTheta, const uivector& vuiThetaDims )
                     {m_vdTheta = vdTheta; m_vuiThetaDims = vuiThetaDims;}

        void getEastWestType( string& strEastWestType )
                            {strEastWestType = m_strEastWestType;}
        void setEastWestType( const string& strEastWestType )
                            {m_strEastWestType = strEastWestType;}

        void getMarginalType( string& strMarginalType )
                            {strMarginalType = m_strMarginalType;}
        void setMarginalType( const string& strMarginalType )
                            {m_strMarginalType = strMarginalType;}

        void getConditioningTime( dvector& vdConditioningTime, uivector& vuiConditioningTimeDims )
                  {vdConditioningTime = m_vdConditioningTime; vuiConditioningTimeDims = m_vuiConditioningTimeDims;}
        void setConditioningTime( const dvector& vdConditioningTime, const uivector& vuiConditioningTimeDims )
                  {m_vdConditioningTime = vdConditioningTime; m_vuiConditioningTimeDims = vuiConditioningTimeDims;}

        void getTimeLags( dvector& vdTimeLags, uivector& vuiTimeLagsDims ) 
                 {vdTimeLags = m_vdTimeLags; vuiTimeLagsDims = m_vuiTimeLagsDims;}
        void setTimeLags( const dvector& vdTimeLags, const uivector& vuiTimeLagsDims ) 
                 {m_vdTimeLags = vdTimeLags; m_vuiTimeLagsDims = vuiTimeLagsDims;}

        void getCapabilities( vector<string>& vstrCapabilities ) { vstrCapabilities = m_vstrCapabilities; }
        void setCapabilities( const vector<string>& vstrCapabilities ) { m_vstrCapabilities = vstrCapabilities; }

        void getSthetaPerturbStyle( string& strStyle )       
             {strStyle = m_strSthetaPerturbStyle;}
        void setSthetaPerturbStyle( const string& strStyle ) 
             {m_strSthetaPerturbStyle = strStyle;}

        AEDBModelSpatialDomain& getSpatialDomain() { return m_spatialDomain; }

//        // Don't allow sets of entire grid object (get it and operate on it, thus no sets or consts)
//        AEDBModelGrid& getGrid() { return m_grid; }

        // Constant string types for db model string fields

        static const string AEDB_EASTWEST_NONE;
        static const string AEDB_MARGTYPE_LOGNORMAL;
        static const string AEDB_MARGTYPE_WEIBULL;

        static const string AEDB_COMP_MEAN;
        static const string AEDB_COMP_MEDIAN;
        static const string AEDB_COMP_PERCENTILE;
        static const string AEDB_COMP_PERTURBED_MEAN;
        static const string AEDB_COMP_SCENARIO;
        static const string AEDB_COMP_UNPERTURBED_SCENARIO;

        static const string AEDB_PS_GAUSSIAN;
        static const string AEDB_PS_UNIFORM;

    protected:

        // data members

        bool     m_bModelRead;

        string   m_strDataFile;
        string   m_strModelName;
        string   m_strModelSpecies;

        // persisted fields
        dvector  m_vdC;         // noise conditioning matrix (C)
        uivector m_vuiCDims;

        //dvector  m_vdG;
        vdvector   m_vvdG;      // time evolution matrix (G)
        //uivector m_vuiGDims;
        vuivector  m_vvuiGDims;

        dvector  m_vdQ;         // principal component matrix (Q)
        uivector m_vuiQDims;

        // Stheta large dataset, retrieve slices as needed
        uivector m_vuiSthetaDims;

        // W very large dataset, retrieve slices as needed
        uivector m_vuiWDims;

        dvector  m_vdDt;
        uivector m_vuiDtDims;

        dvector  m_vdTheta;     // describes statistical distribution of flux at each location
        uivector m_vuiThetaDims;

        string   m_strEastWestType;

        string   m_strMarginalType;

        string   m_strWhichModel;

        string   m_strSthetaPerturbStyle;

        // Version 1.0+ fields, defaults supplied for old data files

        dvector  m_vdConditioningTime;
        uivector m_vuiConditioningTimeDims;

        dvector  m_vdTimeLags;
        uivector m_vuiTimeLagsDims;

        vector<string> m_vstrCapabilities;

//        AEDBModelGrid m_grid;

        AEDBModelSpatialDomain m_spatialDomain;
    };
}

#endif

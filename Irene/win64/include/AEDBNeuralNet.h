/******************************************************************************
$HeadURL$

File: AEDBNeuralNet.h

Description: Class declaration AEDBNeuralNet class in the database layer of the
    Ae9Ap9Model. Classes in the database layer wrap the persistent representation
    and provide i/o for analogous classes in the model layer. Model data are
    persisted in an HDF5 format database. This software is thus dependent on
    the HDF5 interface library.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

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
  1.0          12/20/2010 Created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBNEURALNET_H
#define AEDBNEURALNET_H

#include <vector>
#include <string>
using namespace std;

#include "AETypes.h"
#include "AEDBManagedResource.h"

namespace ae9ap9 
{
    class AEDBNeuralNet : public AEDBManagedResource
    {
    public:
        // construction, destruction
        AEDBNeuralNet();
        ~AEDBNeuralNet();

        // public methods
        int read();
        /*int write();*/

        // public accessors

        const string& getDataFile() 
                 { return m_strDataFile; }
        void setDataFile( const string& strDataFile ) 
                 { m_strDataFile = strDataFile; }

        void getNh( uivector& vuiNh, uivector& vuiNhDims ) 
                 {vuiNh = m_vuiNh; vuiNhDims = m_vuiNhDims;}
        void setNh( const uivector& vuiNh, const uivector& vuiNhDims ) 
                 {m_vuiNh = vuiNh; m_vuiNhDims = vuiNhDims;}

        void getNx( uivector& vuiNx, uivector& vuiNxDims ) 
                 {vuiNx = m_vuiNx; vuiNxDims = m_vuiNxDims;}
        void setNx( const uivector& vuiNx, const uivector& vuiNxDims ) 
                 {m_vuiNx = vuiNx; m_vuiNxDims = vuiNxDims;}

        void getNy( uivector& vuiNy, uivector& vuiNyDims ) 
                 {vuiNy = m_vuiNy; vuiNyDims = m_vuiNyDims;}
        void setNy( const uivector& vuiNy, const uivector& vuiNyDims ) 
                 {m_vuiNy = vuiNy; m_vuiNyDims = vuiNyDims;}

        void getSx( dvector& vdSx, uivector& vuiSxDims ) 
                 {vdSx = m_vdSx; vuiSxDims = m_vuiSxDims;}
        void setSx( const dvector& vdSx, const uivector& vuiSxDims ) 
                 {m_vdSx = vdSx; m_vuiSxDims = vuiSxDims;}

        void getSy( dvector& vdSy, uivector& vuiSyDims ) 
                 {vdSy = m_vdSy; vuiSyDims = m_vuiSyDims;}
        void setSy( const dvector& vdSy, const uivector& vuiSyDims ) 
                 {m_vdSy = vdSy; m_vuiSyDims = vuiSyDims;}

        void getTheta( dvector& vdTheta, uivector& vuiThetaDims )
                     {vdTheta = m_vdTheta; vuiThetaDims = m_vuiThetaDims;}
        void setTheta( const dvector& vdTheta, const uivector& vuiThetaDims )
                     {m_vdTheta = vdTheta; m_vuiThetaDims = vuiThetaDims;}

        void getPolyCoeffsLog10B( dvector& vdCoeffs, uivector& vuiCoeffDims ) 
                 {vdCoeffs = m_vdPolyCoeffsLog10B; vuiCoeffDims = m_vuiPolyCoeffsLog10BDims;}
        void setPolyCoeffsLog10B( const dvector& vdCoeffs, const uivector& vuiCoeffsDims ) 
                 {m_vdPolyCoeffsLog10B = vdCoeffs; m_vuiPolyCoeffsLog10BDims = vuiCoeffsDims;}

        void getMaxMJD( dvector& vdMaxMJD, uivector& vuiMaxMJDDims ) 
                 {vdMaxMJD = m_vdMaxMJD; vuiMaxMJDDims = m_vuiMaxMJDDims;}
        void setMaxMJD( const dvector& vdMaxMJD, const uivector& vuiMaxMJDDims ) 
                 {m_vdMaxMJD = vdMaxMJD; m_vuiMaxMJDDims = vuiMaxMJDDims;}

        void getMaxYear( dvector& vdMaxYear, uivector& vuiMaxYearDims ) 
                 {vdMaxYear = m_vdMaxYear; vuiMaxYearDims = m_vuiMaxYearDims;}
        void setMaxYear( const dvector& vdMaxYear, const uivector& vuiMaxYearDims ) 
                 {m_vdMaxYear = vdMaxYear; m_vuiMaxYearDims = vuiMaxYearDims;}

        void getMaxRoot4J( dvector& vdMaxRoot4J, uivector& vuiMaxRoot4JDims ) 
                 {vdMaxRoot4J = m_vdMaxRoot4J; vuiMaxRoot4JDims = m_vuiMaxRoot4JDims;}
        void setMaxRoot4J( const dvector& vdMaxRoot4J, const uivector& vuiMaxRoot4JDims ) 
                 {m_vdMaxRoot4J = vdMaxRoot4J; m_vuiMaxRoot4JDims = vuiMaxRoot4JDims;}

        void getXBar( dvector& vdXBar, uivector& vuiXBarDims ) 
                 {vdXBar = m_vdXBar; vuiXBarDims = m_vuiXBarDims;}
        void setXBar( const dvector& vdXBar, const uivector& vuiXBarDims ) 
                 {m_vdXBar = vdXBar; m_vuiXBarDims = vuiXBarDims;}

        void getYBar( dvector& vdYBar, uivector& vuiYBarDims ) 
                 {vdYBar = m_vdYBar; vuiYBarDims = m_vuiYBarDims;}
        void setYBar( const dvector& vdYBar, const uivector& vuiYBarDims ) 
                 {m_vdYBar = vdYBar; m_vuiYBarDims = vuiYBarDims;}

        void getThetaStructNh( uivector& vuiThetaStructNh, uivector& vuiThetaStructNhDims ) 
                 {vuiThetaStructNh = m_vuiThetaStructNh; vuiThetaStructNhDims = m_vuiThetaStructNhDims;}
        void setThetaStructNh( const uivector& vuiThetaStructNh, const uivector& vuiThetaStructNhDims ) 
                 {m_vuiThetaStructNh = vuiThetaStructNh; m_vuiThetaStructNhDims = vuiThetaStructNhDims;}

        void getThetaStructNx( uivector& vuiThetaStructNx, uivector& vuiThetaStructNxDims ) 
                 {vuiThetaStructNx = m_vuiThetaStructNx; vuiThetaStructNxDims = m_vuiThetaStructNxDims;}
        void setThetaStructNx( const uivector& vuiThetaStructNx, const uivector& vuiThetaStructNxDims ) 
                 {m_vuiThetaStructNx = vuiThetaStructNx; m_vuiThetaStructNxDims = vuiThetaStructNxDims;}

        void getThetaStructNy( uivector& vuiThetaStructNy, uivector& vuiThetaStructNyDims ) 
                 {vuiThetaStructNy = m_vuiThetaStructNy; vuiThetaStructNyDims = m_vuiThetaStructNyDims;}
        void setThetaStructNy( const uivector& vuiThetaStructNy, const uivector& vuiThetaStructNyDims ) 
                 {m_vuiThetaStructNy = vuiThetaStructNy; m_vuiThetaStructNyDims = vuiThetaStructNyDims;}

        void getThetaStructNTheta( dvector& vdThetaStructNTheta, uivector& vuiThetaStructNThetaDims ) 
                 {vdThetaStructNTheta = m_vdThetaStructNTheta; vuiThetaStructNThetaDims = m_vuiThetaStructNThetaDims;}
        void setThetaStructNTheta( const dvector& vdThetaStructNTheta, const uivector& vuiThetaStructNThetaDims ) 
                 {m_vdThetaStructNTheta = vdThetaStructNTheta; m_vuiThetaStructNThetaDims = vuiThetaStructNThetaDims;}

        void getThetaStructV( dvector& vdThetaStructV, uivector& vuiThetaStructVDims ) 
                 {vdThetaStructV = m_vdThetaStructV; vuiThetaStructVDims = m_vuiThetaStructVDims;}
        void setThetaStructV( const dvector& vdThetaStructV, const uivector& vuiThetaStructVDims ) 
                 {m_vdThetaStructV = vdThetaStructV; m_vuiThetaStructVDims = vuiThetaStructVDims;}

        void getThetaStructV0( dvector& vdThetaStructV0, uivector& vuiThetaStructV0Dims ) 
                 {vdThetaStructV0 = m_vdThetaStructV0; vuiThetaStructV0Dims = m_vuiThetaStructV0Dims;}
        void setThetaStructV0( const dvector& vdThetaStructV0, const uivector& vuiThetaStructV0Dims ) 
                 {m_vdThetaStructV0 = vdThetaStructV0; m_vuiThetaStructV0Dims = vuiThetaStructV0Dims;}

        void getThetaStructW( dvector& vdThetaStructW, uivector& vuiThetaStructWDims ) 
                 {vdThetaStructW = m_vdThetaStructW; vuiThetaStructWDims = m_vuiThetaStructWDims;}
        void setThetaStructW( const dvector& vdThetaStructW, const uivector& vuiThetaStructWDims ) 
                 {m_vdThetaStructW = vdThetaStructW; m_vuiThetaStructWDims = vuiThetaStructWDims;}

        void getThetaStructW0( dvector& vdThetaStructW0, uivector& vuiThetaStructW0Dims ) 
                 {vdThetaStructW0 = m_vdThetaStructW0; vuiThetaStructW0Dims = m_vuiThetaStructW0Dims;}
        void setThetaStructW0( const dvector& vdThetaStructW0, const uivector& vuiThetaStructW0Dims ) 
                 {m_vdThetaStructW0 = vdThetaStructW0; m_vuiThetaStructW0Dims = vuiThetaStructW0Dims;}

    protected:

        // data members

        bool     m_bNeuralNetRead;

        string   m_strDataFile;

        // persisted fields

        uivector m_vuiNh;
        uivector m_vuiNhDims;

        uivector m_vuiNx;
        uivector m_vuiNxDims;

        uivector m_vuiNy;
        uivector m_vuiNyDims;

        dvector  m_vdSx;
        uivector m_vuiSxDims;

        dvector  m_vdSy;
        uivector m_vuiSyDims;

        dvector  m_vdTheta;
        uivector m_vuiThetaDims;

        dvector  m_vdPolyCoeffsLog10B;
        uivector m_vuiPolyCoeffsLog10BDims;

        dvector  m_vdMaxMJD;
        uivector m_vuiMaxMJDDims;

        dvector  m_vdMaxYear;
        uivector m_vuiMaxYearDims;

        dvector  m_vdMaxRoot4J;
        uivector m_vuiMaxRoot4JDims;

        dvector  m_vdXBar;
        uivector m_vuiXBarDims;

        dvector  m_vdYBar;
        uivector m_vuiYBarDims;

        uivector m_vuiThetaStructNh;
        uivector m_vuiThetaStructNhDims;

        uivector m_vuiThetaStructNx;
        uivector m_vuiThetaStructNxDims;

        uivector m_vuiThetaStructNy;
        uivector m_vuiThetaStructNyDims;

        dvector m_vdThetaStructNTheta;
        uivector m_vuiThetaStructNThetaDims;

        dvector  m_vdThetaStructV;
        uivector m_vuiThetaStructVDims;

        dvector  m_vdThetaStructV0;
        uivector m_vuiThetaStructV0Dims;

        dvector  m_vdThetaStructW;
        uivector m_vuiThetaStructWDims;

        dvector  m_vdThetaStructW0;
        uivector m_vuiThetaStructW0Dims;
    };
}

#endif

/******************************************************************************
$HeadURL$

File: AEGridDimension.h

Description: Base class for defining dimensions of an AEGrid generic
    multi-dimensional grid object. Child classes are derived from
    AEGridDimension that are specific to the method of defining 
    coordinates for this dimension (see AEListGridDimension.h and
    AEFormulaGridDimension.h).

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

  Version      Date      Notes
  1.0          
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEGRIDDIMENSION_H
#define AEGRIDDIMENSION_H

#include <string>
using namespace std;

#include "AETypes.h"
#include "AEGridDimScaling.h"
#include "AEGridDimTransform.h"
#include "AEGridDimInterpolation.h"

namespace ae9ap9 
{

    class AEGridDimension
    {
    public:
        // Constructors
        AEGridDimension();
        AEGridDimension( const AEGridDimension& source );

        // virtual destructor
        virtual ~AEGridDimension();

        // assignment operator
        AEGridDimension& operator =( const AEGridDimension& source );

        // require derived classes to provide copy instantiation
        virtual AEGridDimension* clone() const = 0;

        // require derived classes to provide get for coordinates
        virtual int getCoordinates( dvector& vdCoords ) = 0;
        int getNumCoordinates() { return m_iNumCoords; }
        void setNumCoords( int iNumCoords ) { m_iNumCoords = iNumCoords; }

        int getTransformedCoordinates( dvector& vdTransformed );

        int getNeighboringCoordIndices( double dCoordinate, int& Before, int& iAfter );
        int getNeighboringCoordIndices( const dvector& vdCoordinates, ivector& viBefore, ivector& viAfter );

        // Data member accessors
        const string& getName() { return m_strName; }
        void setName( const string& strName ) { m_strName = strName; }

        const string& getUnits() { return m_strUnits; }
        void setUnits( const string& strUnits ) { m_strUnits = strUnits; }

        const string& getSymbol() { return m_strSymbol; }
        void setSymbol( const string& strSymbol ) { m_strSymbol = strSymbol; }

        double getMin() { return m_dMin; }
        void   setMin( double dMin ) { m_dMin = dMin; }

        double getMax() { return m_dMax; }
        void   setMax( double dMax ) { m_dMax = dMax; }

        const string& getScalingType() { return m_strScalingType; }
        const AEGridDimScaling& getScaling() { return *m_pScaling; }
        int setScaling( const string& strScalingType, AEGridDimScaling& scaling );

        AEGridDimTransform& getTransform() { return *m_pTransform; }
        int setTransform( AEGridDimTransform& transform );

        const AEGridDimInterpolation& getInterpolation() { return *m_pInterpolation; }
        int setInterpolation( AEGridDimInterpolation& interpolation );

        double getInputScaleFactor() { return m_dInputScaleFactor; }
        void   setInputScaleFactor( double dFactor ) { m_dInputScaleFactor = dFactor; }


    protected:
        string                  m_strName;
        string                  m_strUnits;
        string                  m_strSymbol;
        string                  m_strScalingType;

        double                  m_dInputScaleFactor; // used in E for "toMeV"

        int                     m_iNumCoords;

        double                  m_dMin;             // Range of values in this dimension
        double                  m_dMax;

        AEGridDimScaling*       m_pScaling;         // scaling/unscaling coordinates
        AEGridDimTransform*     m_pTransform;       // grid coord to physical coord and back
        AEGridDimInterpolation* m_pInterpolation;   // interpolation scheme
    };

} /*namespace*/

#endif

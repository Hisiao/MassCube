/******************************************************************************
$HeadURL$

File: AEFormulaGridDimension.h

Description: Class for defining dimensions of an AEGrid generic
    multi-dimensional grid object from a mathmatical formula.

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
#ifndef AEFORMULAGRIDDIMENSION_H
#define AEFORMULAGRIDDIMENSION_H

#include "AETypes.h"
#include "AEEnums.h"
#include "AEGridDimension.h"

namespace ae9ap9 
{

    class AEFormulaGridDimension : public AEGridDimension
    {
    public:
        // Constructors
        AEFormulaGridDimension();
        AEFormulaGridDimension( const AEFormulaGridDimension& source );

        // Destructor
        virtual ~AEFormulaGridDimension();

        // self copy
        AEGridDimension* clone() const;

        // coordinate retrieval (generic, inherited from base class)
        int getCoordinates( dvector& vdCoords );
        //int getNumCoordinates() 
        //    { return ((int)(((m_dCoordMax-m_dCoordMin)/m_dCoordInterval)+1)); }

        // data member accessors
        double getCoordMin() { return m_dCoordMin; }
        void setCoordMin( double dCoordMin ) { m_dCoordMin = dCoordMin; }

        double getCoordMax() { return m_dCoordMax; }
        void setCoordMax( double dCoordMax ) { m_dCoordMax = dCoordMax; }

        double getCoordInterval() { return m_dCoordInterval; }
        void setCoordInterval( double dCoordInterval ) { m_dCoordInterval = dCoordInterval; }

    protected:
        double m_dCoordMin;
        double m_dCoordMax;
        double m_dCoordInterval;
    };

} /*namespace*/

#endif

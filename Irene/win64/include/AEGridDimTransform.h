/******************************************************************************
$HeadURL$

File: AEGridDimTransform.h

Description: AEGridDimTransform defines a suite of mathematical operations that
    can be applied to convert between grid and physical coordinates This class
    also serves as default implementation for untransformed dimensions (ie: 
    physical = grid coordinate). It is intended that this class be derived from
    to support extensions to more complex transformations. As such, methods are
    defined as virtual.

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
  1.0          08/28/2009 Created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEGRIDDIMTRANSFORM
#define AEGRIDDIMTRANSFORM

#include "AETypes.h"
#include "AEEnums.h"

namespace ae9ap9 
{
    class AEGridDimTransform
    {
    public:

        // constructors
        AEGridDimTransform();

        // destructor
        virtual ~AEGridDimTransform();

        // self copy
        virtual AEGridDimTransform* clone();

        virtual int transform( double& dCoord );
        virtual int reverseTransform( double& dCoord );

        virtual int transform( dvector& vdCoords );
        virtual int reverseTransform( dvector& vdCoords );

        void setTransformType( eAETransform eTransform ) { m_eTransform = eTransform; }
        eAETransform getTransformType() { return m_eTransform; }

        void setParameter( double dParameter) { m_dParameter = dParameter; }
        double getParameter() { return m_dParameter; }

    protected:
        eAETransform m_eTransform;
        double       m_dParameter;
    };

} /*namespace*/

#endif

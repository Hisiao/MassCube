/******************************************************************************
$HeadURL$

File: AEListGridDimension.h

Description: Class for defining dimensions of an AEGrid generic
    multi-dimensional grid object from a list of coordinates.


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
#ifndef AELISTGRIDDIMENSION_H
#define AELISTGRIDDIMENSION_H

#include "AETypes.h"
#include "AEGridDimension.h"

namespace ae9ap9 
{

    class AEListGridDimension : public AEGridDimension
    {
    public:
        // constructors
        AEListGridDimension();
        AEListGridDimension( const AEListGridDimension& source );

        // destructor
        virtual ~AEListGridDimension();

        // self copy
        AEGridDimension* clone() const ;

        // coordinate retrieval (generic, inherited from base class)
        int getCoordinates( dvector& vdCoords );
        //int getNumCoordinates() { return (m_vdCoordinates.size()); }

        // data member accessors
        void setCoordinates( const dvector& vdCoords );
        void appendCoordinate( double dCoordinate );

    protected:

        dvector m_vdCoordinates;
    };

} /*namespace*/

#endif

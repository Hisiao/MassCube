/******************************************************************************
$HeadURL$

File: Ae8VetteLossConeFilter.h

Description: This is the class declaration for the Ae8VetteLossConeFilter class
    This class, derived from AECartesianFilter is designed to create a sparse
    cartesian grid from two sets of dimensional coordinates based on filter
    criteria for the coordinates. In the case of the Ae8Vette filter, dimension one is 
    assumed to be pitch angle, while dimension 2 contains L shell values. The filter 
    function identifies for a given L shell, the pitch angle at the loss cone edge.
    Grid coordinates at a given L shell, where the pitch angle falls below that at the 
    loss cone edge are filtered out of the result set. The result set is returned as a 
    vector of offsets corresponding to 2 dimensional gridded data stored in an array
    in row-major order.

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
  1.0          10/16/2009 created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AE8VETTELOSSCONEFILTER
#define AE8VETTELOSSCONEFILTER

#include "AECartesianFilter.h"

namespace ae9ap9 {

    class Ae8VetteLossConeFilter : public AECartesianFilter
    {
    public:
        Ae8VetteLossConeFilter();
        ~Ae8VetteLossConeFilter();

        AECartesianFilter* clone() const ;

        // Template for derived class' filtering method
        int filter( const dvector& vdDim1Coords, 
                    const dvector& vdDim2Coords,
                          dvector& vdFilteredGridIndices);

    protected:
    };
}

#endif

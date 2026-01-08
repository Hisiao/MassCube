/******************************************************************************
$HeadURL$

File: AEOPQMaxKLossConeFilter.h

Description: This is the class declaration for the AEOPQMaxKLossConeFilter class
    This class, derived from AECartesianFilter is designed to create a sparse
    cartesian grid from two sets of dimensional coordinates based on filter
    criteria for the coordinates. In the case of the OPQMaxK filter, dimension one is 
    assumed to be K, while dimension 2 contains Phi values. The filter 
    function evaluates a polynomial defining the K value at the edge of the loss cone
    for a given value of Phi. K values above the threshold are filtered out of the
    retuls set. The result set is returned as a vector of offsets corresponding to 
    2 dimensional gridded data stored in an array in row-major order.

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
#ifndef AEOPQMAXKLOSSCONEFILTER
#define AEOPQMAXKLOSSCONEFILTER

#include "AECartesianFilter.h"

namespace ae9ap9 {

    class AEOPQMaxKLossConeFilter : public AECartesianFilter
    {
    public:
        AEOPQMaxKLossConeFilter();
        ~AEOPQMaxKLossConeFilter();

        AECartesianFilter* clone() const;

        // Template for derived class' filtering method
        int filter( const dvector& vdDim1Coords, 
                    const dvector& vdDim2Coords,
                          dvector& vdFilteredGridIndices);

        void setCoefficients( const dvector& vdCoeffs ) { m_vdCoefficients = vdCoeffs; }
        const dvector& getCoefficients() { return m_vdCoefficients; }

    protected:
        dvector m_vdCoefficients;
    };
}

#endif

/******************************************************************************
$HeadURL$

File: AELinearBasisFunction.h

Description: Class declaration for the AELinearBasisFunction class. 
    This class is used in the evaluation, interpolation and integration of values
    over the assigned dimension space.

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
  1.0          10/19/2009 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AELINEARBASISFUNCTION_H
#define AELINEARBASISFUNCTION_H

#include <vector>
#include <string>
using namespace std;

#include "AETypes.h"

namespace ae9ap9 
{
    class AELinearBasisFunction
    {
    public:
        AELinearBasisFunction();
        ~AELinearBasisFunction();

        int evaluate( double dValue, int iAtIndex, double& dEval );

        int integrateRight( double dValue, int iAtIndex, double& dIntegral );

        // public accessors

        int  getN() { return m_iN; }
        void setN(int iN) { m_iN = iN; }

        const dvector& getX() { return m_vdX; }
        void setX(const dvector& vdX) { m_vdX = vdX; }

        const dvector& getHigh() { return m_vdHigh; }
        void setHigh(const dvector& vdHigh) { m_vdHigh = vdHigh; }

        const dvector& getLow() { return m_vdLow; }
        void setLow(const dvector& vdLow) { m_vdLow = vdLow; }

        const dvector& getMid() { return m_vdMid; }
        void setMid(const dvector& vdMid) { m_vdMid = vdMid; }

    protected:

        // data members

        // persisted fields
        int      m_iN;
        dvector  m_vdX;
        dvector  m_vdHigh;
        dvector  m_vdLow;
        dvector  m_vdMid;
    };

};

#endif

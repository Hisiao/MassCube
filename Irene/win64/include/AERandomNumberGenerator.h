/******************************************************************************
$HeadURL$

File: AERandomNumberGenerator.h

Description: This header file contains the class declaration for the
AERandomNumberGenerator base class. This class defines the interface
for all supported random number generator classes.

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
  1.0          10/22/2009 created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AERANDOMNUMBERGENERATOR_H
#define AERANDOMNUMBERGENERATOR_H

#include "AEBoostrap.h"

namespace ae9ap9
{
    class AERandomNumberGenerator
    {
    public:
        // Construction, destruction

        AERandomNumberGenerator() : m_uiSeed(0){}
        AERandomNumberGenerator( unsigned int uiSeed ) : m_uiSeed(uiSeed) {}
        virtual ~AERandomNumberGenerator() {}

        // Number generation methods

        virtual int randn( bdvector& bvdDataOut, int iSize ) = 0; // random numbers over a standard normal distribution
        virtual int rand( bdvector& bvdDataOut, int iSize ) = 0;  // random numbers over a range: 0 .. 1

        // data member accessors

        unsigned int getSeed() { return m_uiSeed; }
        void    setSeed(unsigned int uiSeed) { m_uiSeed = uiSeed; }

        const   buivector& getSeeds() { return m_bvuiSeeds; }
        void    setSeeds( const buivector& bvuiSeeds ) { m_bvuiSeeds = bvuiSeeds; }

    protected:

        // internal helper methods

        virtual int initialize() = 0;

        // protected data members

        unsigned int m_uiSeed;      // passed initial seed
        buivector    m_bvuiSeeds;   // array of current seeds (based on # calls to rand[n]()
    };
}

#endif

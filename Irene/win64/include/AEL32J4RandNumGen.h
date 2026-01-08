/******************************************************************************
$HeadURL$

File: AEL32J4RandNumGen.h

Description: This header file contains the class declaration for the
L32J4 Random Number Generator class. This class defines a random
number generator based on the following reference:

 P. L'Ecuyer, "Tables of Maximally Equidistributed Combined LFSR Generators",
 Mathematics of Computation, 68, 225 (1999), 261-269

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
#ifndef AEL32J4RANDNUMGEN_H
#define AEL32J4RANDNUMGEN_H

#include "AERandomNumberGenerator.h"

namespace ae9ap9
{
    class AEL32J4RandNumGen : public AERandomNumberGenerator
    {
    public:

        // Construction, destruction

        AEL32J4RandNumGen();
        AEL32J4RandNumGen( unsigned int uiSeed );
        ~AEL32J4RandNumGen();

        // Number generation methods

        int randn( bdvector& bvdDataOut, int iSize ); // random numbers over a standard normal distribution
        int rand( bdvector& bvdDataOut, int iSize );   // random numbers over a range: 0 .. 1

    protected:

        // internal helper methods

        int initialize();
    };
}

#endif

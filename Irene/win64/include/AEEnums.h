/******************************************************************************
$HeadURL$

File: AEEnums.h

Description: Enumerated datatypes used in the AE9AP9 project

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
  1.0          08/28/2009 created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEENUMS_H
#define AEENUMS_H

namespace ae9ap9
{
    // Model types

    typedef enum eAEModel
    {
        eAEModelUndefined = 0,
        eAEModelElectron,
        eAEModelProton,
        eAEModelSpecies,
    } eAEModel;

    // Energy flux types

    typedef enum eAEEnergyFluxType
    {
        eAEEFTUndefined = 0,
        eAEEFTDifferentialAtE1,
        eAEEFTDifferentialE1ToE2,
        eAEEFTIntegral,
    } eAEEnergyFluxType;

    // Transforms available for a grid dimension

    typedef enum eAETransform
    {
        eAETNone = 0,  // x
        eAETMultiply,  // x * parameter
        eAETDivide,    // x / parameter
        eAETAdd,       // x + parameter
        eAETSubtract,  // x - parameter
        eAETPower,     // x ^ parameter
        eAETLog,       // ln(x)
        eAETLog10,     // log10(x)
        eAETExp,       // e ^ x
        eAET10ToX,     // 10 ^ x
        eAETNegPower,  // x ^ (1/parameter)
    } eAETransform;

    // Pre-compute options

    typedef enum eAEPreCompute
    {
        eAEPreComputeNone=0,  // no flux weight pre-computation
        eAEPreComputeHEH23,   // precompute flux weights in E, X2, X3 only
        eAEPreComputeAllH,    // precompute flux weights (H) for all energy, time, directions
    } eAEPreCompute;

} /*namespace*/

#endif

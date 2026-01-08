/******************************************************************************

 File: DataBaseNames.h

 Description: Specification of the base filenames for databases used by IRENE.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2022 Atmospheric and Environmental Research, Inc. (AER)

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

  Version      Date        Notes
  1.0          06/22/2022  Created 
  
******************************************************************************/

#ifndef DATABASENAMES_H
#define DATABASENAMES_H

// specify database base filenames that are part of the IRENE model distribution
//  -- to be used in conjunction with the 'ModelDBDir' specification
#define LEGACYMODELDB   "radiationBeltDB.h5"
#define DOSEMODELDB     "sd2DB.h5"
#define MAGFIELDMODELDB "igrfDB.h5"
#define KHMINMODELDB    "fast_hmin_net.mat"
#define KPHIMODELDB     "fastPhi_net.mat"
#define AE9MODELDB      "AE9V15_runtime_tables.mat"
#define AP9MODELDB      "AP9V15_runtime_tables.mat"
#define PLASMAEMODELDB  "SPMEV12_runtime_tables.mat"
#define PLASMAHMODELDB  "SPMHV12_runtime_tables.mat"
#define PLASMAHEMODELDB "SPMHEV10_runtime_tables.mat"
#define PLASMAOMODELDB  "SPMOV10_runtime_tables.mat"
#define CAMMICEMODELDB  "cammiceDB.h5"

#endif // DATABASENAMES_H

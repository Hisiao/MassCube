/******************************************************************************
$HeadURL$

File: AEDBResourceManager.h

Description: Class declaration for the AEDBResourceManager class
    This class creates and manages the lifetimes of objects in the database
    layer. Managed resources are maintained as singletons with reference
    counts. A single instance of a managed resource is identified by the
    path passed to it.

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
  1.0          10/09/2009 created
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBRESOURCEMANAGER
#define AEDBRESOURCEMANAGER

#include <map>
#include <string>
#include <memory>
using namespace std;

#include "AEDBManagedResource.h"
#include "AEDBModel.h"
#include "AEDBNeuralNet.h"
#include "StkMutex.h"

namespace ae9ap9
{
    class AEDBResourceManager
    {
    public:
        ~AEDBResourceManager();

        static AEDBResourceManager& Instance();

        // public accessors for managed resource types

        AEDBModel* getModel(const string& strPath);
        void releaseModel(const string& strPath);

        AEDBNeuralNet* getNeuralNet(const string& strPath);
        void releaseNeuralNet(const string& strPath);

    protected:
        AEDBResourceManager();     // Singleton class: get via Instance() method

        static unique_ptr<AEDBResourceManager> m_instance;

        static CStkMutex m_syncAccess;

        map<string, AEDBManagedResource*> m_mapModels;
        map<string, AEDBManagedResource*> m_mapNeuralNets;
    };
}

#endif

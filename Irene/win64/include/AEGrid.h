/******************************************************************************
$HeadURL$

File: AEGrid.h

Description: Generic n dimensional grid class from which all AE9AP9 grids derive
    The generic grid supports any number of dimensions, each of which can have
    coordinates defined by exhaustive list or formula calculation. Any or all
    dimensions of the grid can optionally support scaling, interpolation and/or
    filtering.

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
  1.0         8/26/2009  created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEGRID_H
#define AEGRID_H

#include <string>
#include <map>
using namespace std;

#include "AEErrorHandler.h"
#include "AEGridDimension.h"

namespace ae9ap9 
{
    class AEGrid : public AEErrorHandler
    {
    public:
        AEGrid();
        virtual ~AEGrid();

        int addDimension( const string& strName, const AEGridDimension& dimension );
        AEGridDimension* getDimension( const string& strName );

        int numDimensions() { return int(m_mapDimensions.size()); }
        AEGridDimension* getDimension( unsigned int i );

    protected:
        map<string,AEGridDimension*> m_mapDimensions;

    private:
        // prevent auto-creation, no reason for them to exist
        AEGrid( const AEGrid& source ); 
        AEGrid& operator =( const AEGrid& source ); 
    };

} /*namespace*/

#endif

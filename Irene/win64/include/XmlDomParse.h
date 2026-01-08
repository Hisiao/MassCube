/******************************************************************************
$HeadURL$

 File: XmlDomParse.h

 Description: Declaration for methods for loading the expected fields and
   values from the kernel specification xml files.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2018 Atmospheric and Environmental Research, Inc. (AER)

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
  1.0          01/23/2018  Created

SVNTag: $Id$
******************************************************************************/

#ifndef XMLDOMPARSE_H
#define XMLDOMPARSE_H

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <string>
#include <vector>

using namespace std;
using namespace xercesc;

class XmlDomParse
{
    DOMDocument* m_docDom;

  public:
    XmlDomParse( const std::string &strXmlFile );
    ~XmlDomParse();

    std::string getChildValue( const std::string &strParent,
                               const std::string &strChild,
                               int iParent = 0,
                               int iChild = 0 );
    std::string getGrandChildValue( const std::string &strParent,
                               const std::string &strGrandChild,
                               int iParent = 0,
                               int iGrandChild = 0 );
    int getChildVector( const std::string &strParent,
                        const std::string &strChild,
                        std::vector<double>& vdValues,
                        int iParent = 0 );
    int getChildVector( const std::string &strParent,
                        const std::string &strChild,
                        std::vector<std::string>& vstrValues,
                        int iParent = 0 );
    int getChildMatrix( const std::string &strParent,
                        const std::string &strChild,
                        const std::string &strItem,
                        std::vector< std::vector<double> >& vvdMatrix,
                        int iParent = 0 );
    string getChildValueItem( const std::string &strParent,
                              const std::string &strChild,
                              const std::string &strItem,
                              int iItem,
                              int iParent = 0,
                              int iChild = 0 );
    string getChildAttr( const std::string &strParent,
                         const std::string &strChild,
                         const std::string &strAttr,
                         int iParent = 0,
                         int iChild = 0 );
    int getChildCount( const std::string &strParent,
                       const std::string &strChild,
                       int iParent = 0 );

  private:
    XmlDomParse();
};

#endif

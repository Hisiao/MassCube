/******************************************************************************
$HeadURL$

 File: InputParser.h

 Description: Declaration for methods for the parsing of keyword/value pairs
   from an input file, producing a set of parameter values that describes all 
   aspects of a sequence of model calculations and the generation of output.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2015 Atmospheric and Environmental Research, Inc. (AER)

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
  1.0          09/18/2015  Created 
  
SVNTag: $Id$
******************************************************************************/

#ifndef CINPUTPARSER_H
#define CINPUTPARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

typedef vector<string> svector;
#include "CInputParameters.h"

class InputParser : public InputParameters
{
public:
  InputParser();
  virtual ~InputParser();

/*------------------------------------------------------------------*/
// function InputParser::parseInputFile
/**
*
* Reads input settings ascii file and validates the input.
* If input is valid, settings are copied into internal data
* structures for passing to the model.
*
*    @param strInputFile
*           string: Name and path of ascii input file containing model settings
*           (Refer to User's Guide document for details)
*    @param bVerifyExist
*           bool: flag for verifying existence of named files and directories (default=true)
*
*
*    @return int
*           0: success, >0: number of parameter errors, <0: file open error
*/
  int parseInputFile( const string& strInputFile,
                      bool bVerifyExist = true );

protected:

  // internal helper methods

  istream& safeGetLine( istream& inStream,
                        string& strLine);

  int validateKeyValues( const string& strKey,
                         svector& vstrValues );

  int parseTimeFields( const svector& vstrValues,
                       double& dEphemTime,
                       const string strKey );

  int checkCoordSys( const string& strCoordSys,
                     const string& strCoordUnits,
                     string& strCoordOrder );

  int splitParameterLine( const string& strLine,
                          string& strKey,
                          svector& vstrValues );

  int splitLine( string& strSource,
                 char cSplitOn,
                 svector& vstrValues );

  int parseStringsToDoubles( const svector& vstrValues,
                             dvector& vdValues,
                             const int& iExpect=0 );

  int parseStringsToInts( const svector& vstrValues,
                          ivector& viValues,
                          const int& iExpect=0 );

  void trimWhitespace( string& strToken );

  int readExtraInfoFile( const string& strInfoFile,
                         const string& strKey );

  int setTimeSpec( const string& strValue,
                   const string& strKey,
                   string& strTimeSpec );
  int setCoordOrder( const string& strValue,
                     const string& strKey,
                     string& strCoordOrder );
  int setDataDelim( const string& strValue,
                    const string& strKey,
                    string& strDataDelim );
  int setBooleanState( const string& strValue,
                       const string& strKey,
                       bool& bState,
                       bool bDefaultTrue=false );

};


#endif

/**********************************************************************

Module: UnitTestOutput

Description: Provides formatted output processing for unit tests

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution is unlimited. 

 Author:

   This software was developed and/or modified by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:

   None

 Revision history:

     VERSION           DATE        NOTES
       1.0      Thu Aug 25, 2008   First release

----------------------------------------------------------------------

***********************************************************************/
#ifndef UNITTESTOUTOUT_H
#define UNITTESTOUTPUT_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>

typedef std::vector<std::string> svector;
typedef std::vector<double> dvector;
typedef std::vector<long> lvector;

using namespace std;

class UnitTestOutput{
private:
  typedef std::vector<dvector> darray;
  
  typedef struct dLocalData{
    int x,y,islong;
    char name[1024];
    darray data;
  }dLocalData;
  
  typedef std::vector<dLocalData> ddata;

  int width,inSection,sectionSize,rowSize,lineCount,precision,fwidth;
  bool bForceFixed, bForceScientific, bForceComparePct;
  string sectionName;
  streampos seekTo;
  ifstream *input;
  ofstream *output;
  string fileName;
  ddata data;
  int read();
  int close();
  ostream& writeDouble( ostream& os, double d );

public:
  UnitTestOutput(const string &csFileName,int write=0);
  ~UnitTestOutput();

  int startSection(const string &csSectionName,int ciRowSize);
  int endSection();
  int addComment(const string &csComment);
  int addComment(const svector &csComment);
  int addRow(const dvector &cdvData);
  int addRow(const lvector &clvData);
  int addElement(double cdData);
  int addElement(long clData);
  int print();
  int ok();
  int compare(const string &csFileName,double tolerance=0);
  int comparePct( const string& csFileName, double tolerancePct=0.01);
  int difference(ddata *other,double tolerance);

  // Numeric field formatting options

/**
*
* Sets output format width 
*
*    @param newWidth format width
**
*/


  void Width( int newWidth ) { width=newWidth; }

/**
*
* Gets output format width 
*
*    @return format width
**
*/
  int  Width() { return width;}

/**
*
* Sets fixed floating point output format width 
*
*    @param newWidth  fixed format width
**
*/

  void FWidth( int newWidth ) { fwidth=newWidth; }

/**
*
* Gets fixed floating point output format width 
*
*    @return  fixed format width
**
*/

  int  FWidth() { return fwidth;}

/**
*
* Sets floating point output format precision 
*
*    @param newPrecision floating point format precision
**
*/

  void Precision( int newPrecision ) { precision=newPrecision; }
/**
*
* Gets floating point output format precision 
*
*    @return floating point format precision
**
*/

  int  Precision() { return precision; }

/**
*
* Sets fixed floating point output format 
*
*    @param bFixedFmt flag TRUE=fixed floating point format, 
*    FALSE=scientific format
**
*/

  void Fixed( bool bFixedFmt ) {
    bForceFixed = bFixedFmt; 
    if(bForceFixed && bForceScientific) bForceScientific=false;
  }

/**
*
* Gets fixed floating point output format state 
*
*    @return  TRUE=fixed floating point format, 
*    FALSE=scientific format
**
*/

  bool Fixed() { return bForceFixed; }

/**
*
* Sets scientific format 
*
*    @param bSciFmt flag TRUE=scientific format, FALSE=floating point format
**
*/

  void Scientific( bool bSciFmt ) { 
    bForceScientific = bSciFmt; 
    if(bForceFixed && bForceScientific) bForceFixed=false;
  }

/**
*
* Gets scientific format state
*
*    @return  TRUE=scientific format, FALSE=floating point format
**
*/

  bool Scientific() { return bForceScientific; }

/**
 *
 * Sets the Force ComparePct flag
 *
 * @param bCompareAsPct 
 *        true, interpret tolerance as % of data values
 *        false, tolerance is absolute value
 */

  void SetCompareAsPct( bool bCompareAsPct ) { bForceComparePct = bCompareAsPct; }

};

#endif

/***********************************************************************
 File: GenericModel.h

 Description:

   Declarations for 'generic' model methods.

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


***********************************************************************/
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>
#include "Hdf5ToolKit.h"
#include "GenericModelEnum.h"

#include "SpWxTypes.h"

#ifndef Generic_Model_Class_Header
#define Generic_Model_Class_Header


class GenericModel {

public:
  GenericModel();
  GenericModel(const char *cModuleName);
  GenericModel(const string &sDBFullPath);
  virtual ~GenericModel();
  virtual eGENERIC_ERROR_CODE Initialize(const string& sDBFullPath );

  virtual eGENERIC_ERROR_CODE setActivity( eActivity eAin ){
    m_cActivity=eAin; return eNoError;};

  virtual eActivity getActivity(){return m_cActivity;};

  eGENERIC_ERROR_CODE setOutput(FILE *f){
	 cpPfout=f;
	 return(eNoError);
  }

  eGENERIC_ERROR_CODE setOutput(ostream *f){
	 cpPout=f;
	 return(eNoError);
  }

  eGENERIC_ERROR_CODE closeDatabase()
  {
    if (cpDb) {
      delete cpDb;
      cpDb = NULL;
    }
    return eNoError;
  }

protected:
  eActivity m_cActivity; // Class 'state' variables
  Hdf5ToolKit *cpDb; // newer HDF5 interface, compat w/ HDF5 >= 1.8
  string csLastMessage; // exception help string
  string csMODULE; // module name
  FILE *cpPfout;
  ostream *cpPout;
  ostream *cpPerr;

  eGENERIC_ERROR_CODE ExceptionMsg(const string &msg);
  eGENERIC_ERROR_CODE WarningMsg(const string &msg);
  eGENERIC_ERROR_CODE ErrorMsg(const string &msg);

public:
  virtual const string message(){
    string msg=csLastMessage;
    csLastMessage.clear();
    return(msg);
  };
  virtual const string message(const eGENERIC_ERROR_CODE &code);
  static const string message_static(const eGENERIC_ERROR_CODE &code, string module = "");
};

#endif

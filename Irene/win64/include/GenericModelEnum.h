/***********************************************************************
 File: GenericModelEnum.h
 
 Description:

   Definitions of 'generic' model enumerations.

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
#ifndef __AerSpEnv_h__
#ifndef Generic_Model_Enum_Header
#define Generic_Model_Enum_Header

/**
*   Generic AER Space Environment enumerated error codes
*/
 
typedef enum eGENERIC_ERROR_CODE{ 
  eNotImplemented=-2, /*!< Function not implemented*/  
  eUserDefined=-1, /*!< User defined error employed*/  
  eNoError = 0,/*!< No error*/  
  eInActive, /*!< Function inactive*/  
  eException,/*!< Exception throwned in function*/  
  eError, /*!< General error */  
  eWarning, /*!< General warning */  
  eNoDataBase, /*!< Database I/0 error or file not found*/  
  eInitializationFailed,/*!< Class initalization failed*/  
  eVarSizeMisMatch,/*!< Variable size mismatch occured*/  
  eInvalidNullPointer,/*!< Invalid or NULL pointer provided*/
  eOutOfRange,/*!< variable or index out of range*/
  // above here are common and need this order, pending error handling redefine - raq
  eAltOutOfRange,/*!< Input altitiude out of range*/  
  eThresholdOutOfRange,/*!< Input threshold out of range*/  
  eContourError,/*!< Error occured while generating contours*/  
  eNoDbEntry/*!< Input request points to invalid data base entry*/  
} eGENERIC_ERROR_CODE;

// enums provided (global definition) to facilitate clarity in setting
// state variables

/**
*   Generic AER Class acitivity state
*/

typedef enum eActivity {
  eaQuiet, /*!< Object inactive */  
  eaActive /*!< Object active */  
} eActivity;

#endif
#endif

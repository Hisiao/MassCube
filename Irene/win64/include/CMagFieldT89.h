/******************************************************************************
File: CMagFieldT89.h

Description: Declarations of methods for the calculations for the 
      Tsyganenko 89/C version of the external magnetic field model.

 Rights and Restrictions:

   Copyright 2022 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION - tbd

 Author:

   This software was developed and modified by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:

   none

 Revision history:

  Version      Date       Notes
    1.0     12/01/2022   initial development

******************************************************************************/

#ifndef CMAGFIELD_T89_H
#define CMAGFIELD_T89_H

#include "SpWxTypes.h"
#include "CMagFieldErrors.h"

using namespace std;

/**
 *  @brief Tsyganenko 89/C external magnetic field model class
 *
 *  The CMagFieldT89 class returns the magnetic field strength vector
 *   at the specified GSM coordinate position, as a function of the
 *   magnetic pole tilt angle and current Kp index conditions. <br>
 *  All position coordinate values are in units of 'Re', where 1 Re = 6371.2km.
 *
 *  The returned magnetic field strength vector values [GSM, in nanoTeslas] are 
 *   intended to be combined with those from the 'main' magnetic field model.
 *
 *  The user may specify the magnetic pole tilt angle and Kp condition
 *   independently from the input GSM coordinate position.
 *
 *  The current Kp condition may be defined using the Kp index value, or 
 *   in terms of the T89-defined Kp 'bin number' (1-7) based on these ranges:
 *
 *   T89 Kp bins
 *     1.  Kp= 0,0+     (0.00-0.33)
 *     2.  Kp= 1-,1,1+  (0.66-1.33)
 *     3.  Kp= 2-,2,2+  (1.66-2.33)
 *     4.  Kp= 3-,3,3+  (2.66-3.33)
 *     5.  Kp= 4-,4,4+  (3.66-4.33)
 *     6.  Kp= 5-,5,5+  (4.66-5.33)
 *     7.  Kp= 6- and above (5.66-9.66)
 */

class CMagFieldT89
{

  public:
/**
* Constructor 
*/
    CMagFieldT89();
/**
* Destructor 
*/
    ~CMagFieldT89();

/*------------------------------------------------------------------*/
// function CMagFieldT89::setKpValue
/**
*
* Define the Kp index value to be used with all subsequent calls to
*  the 'getField' method, unless otherwise specified.
*
*    @param dKpVal
*           double: Kp index value 
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE setKpValue( const double& dKpVal );

/*------------------------------------------------------------------*/
// function CMagFieldT89::setKpBin
/**
*
* Define the Kp bin number to be used with all subsequent calls to
*  the 'getField' method, unless otherwise specified.
*
*    @param iKpBin
*           int: Kp bin number; 1-7 valid (see main description) 
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE setKpBin( const int& iKpBin );

/*------------------------------------------------------------------*/
// function CMagFieldT89::getKpBin
/**
*
* Returns the most recently defined Kp bin number, via previous calls
*  to the 'setKpBin' or 'setKpValue' methods. 
*
*    @returns
*           int: current Kp bin number, or -1 on error 
*/
    int getKpBin( ) { return ciKpBin; }

/*------------------------------------------------------------------*/
// function CMagFieldT89::setTiltAngle
/**
*
* Define the magnetic polar tilt angle to be used with all subsequent
*  calls to the 'getField' method, unless otherwise specified.
*
*    @param dTiltAngle
*           double: magnetic pole tilt angle [degrees]
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE setTiltAngle( const double& dTiltAngle );

/*------------------------------------------------------------------*/
// function CMagFieldT89::getTiltAngle
/**
*
* Returns the most recently defined magnetic pole tilt angle, via previous
*  call to the 'setTiltAngle' method. 
*
*    @returns
*           double: current magnetic pole tilt angle, or -99.0 when undefined 
*/
    double getTiltAngle( ) { return cdTiltAngle; }

/*------------------------------------------------------------------*/
// function CMagFieldT89::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates, 
*  for the given magnetic pole tilt angle and Kp index value.
*
*    @param vdPos
*           dvector: GSM coordinate position XYZ [Re]
*
*    @param[out] vdField
*           dvector: returned magnetic field strength vector [nanoTeslas]
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE getField( const dvector& vdPos,
                                   dvector& vdField );

/*------------------------------------------------------------------*/
// function CMagFieldT89::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates, 
*  for the given magnetic pole tilt angle and Kp index value.
*
*    @param dTiltAngle
*           double: Magnetic pole tilt angle [degrees]
*    @param vdPos
*           dvector: GSM coordinate position XYZ [Re]
*
*    @param[out] vdField
*           dvector: returned magnetic field strength vector [nanoTeslas]
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE getField( const double& dTiltAngle,
                                   const dvector& vdPos,
                                   dvector& vdField );

/*------------------------------------------------------------------*/
// function CMagFieldT89::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates, 
*  for the given magnetic pole tilt angle and Kp index value.
*
*    @param dKpVal
*           double: Kp index value 
*    @param dTiltAngle
*           double: Magnetic pole tilt angle [degrees]
*    @param vdPos
*           dvector: GSM coordinate position XYZ [Re]
*
*    @param[out] vdField
*           dvector: returned magnetic field strength vector [nanoTeslas]
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE getField( const double& dKpVal,
                                   const double& dTiltAngle,
                                   const dvector& vdPos,
                                   dvector& vdField );

/*------------------------------------------------------------------*/
// function CMagFieldT89::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates, 
*  for the given magnetic pole tilt angle and Kp bin number.
*
*    @param dKpVal
*           double: Kp index value
*    @param dTiltAngle
*           double: Magnetic pole tilt angle [degrees]
*    @param dPosX
*           double: GSM coordinate X position [Re]
*    @param dPosY
*           double: GSM coordinate Y position [Re]
*    @param dPosZ
*           double: GSM coordinate Z position [Re]
*
*    @param[out] dFieldX
*           double: returned X component of magnetic field strength [nanoTeslas]
*    @param[out] dFieldY
*           double: returned Y component of magnetic field strength [nanoTeslas]
*    @param[out] dFieldZ
*           double: returned Z component of magnetic field strength [nanoTeslas]
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE getField( const double& dKpVal,
                                   const double& dTiltAngle,
                                   const double& dPosX,
                                   const double& dPosY,
                                   const double& dPosZ,
                                   double& dFieldX,
                                   double& dFieldY,
                                   double& dFieldZ );
    
/*------------------------------------------------------------------*/
// function CMagFieldT89::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates, 
*  for the given magnetic pole tilt angle and Kp bin number.
*
*    @param iKpBin
*           int: Kp bin number; 1-7 valid (see main description) 
*    @param dTiltAngle
*           double: Magnetic pole tilt angle [degrees]
*    @param vdPos
*           dvector: GSM coordinate position XYZ [Re]
*
*    @param[out] vdField
*           dvector: returned magnetic field strength vector [nanoTeslas]
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE getField( const int& iKpBin,
                                   const double& dTiltAngle,
                                   const dvector& vdPos,
                                   dvector& vdField );

/*------------------------------------------------------------------*/
// function CMagFieldT89::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates, 
*  for the given magnetic pole tilt angle and Kp bin number.
*
*    @param dPosX
*           double: GSM coordinate X position [Re]
*    @param dPosY
*           double: GSM coordinate Y position [Re]
*    @param dPosZ
*           double: GSM coordinate Z position [Re]
*
*    @param[out] dFieldX
*           double: returned X component of magnetic field strength [nanoTeslas]
*    @param[out] dFieldY
*           double: returned Y component of magnetic field strength [nanoTeslas]
*    @param[out] dFieldZ
*           double: returned Z component of magnetic field strength [nanoTeslas]
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE getField( const double& dPosX,
                                   const double& dPosY,
                                   const double& dPosZ,
                                   double& dFieldX,
                                   double& dFieldY,
                                   double& dFieldZ );
    
/*------------------------------------------------------------------*/
// function CMagFieldT89::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates, 
*  for the given magnetic pole tilt angle and Kp bin number.
*
*    @param iKpBin
*           int: Kp bin number; 1-7 valid (see main description)
*    @param dTiltAngle
*           double: Magnetic pole tilt angle [degrees]
*    @param dPosX
*           double: GSM coordinate X position [Re]
*    @param dPosY
*           double: GSM coordinate Y position [Re]
*    @param dPosZ
*           double: GSM coordinate Z position [Re]
*
*    @param[out] dFieldX
*           double: returned X component of magnetic field strength [nanoTeslas]
*    @param[out] dFieldY
*           double: returned Y component of magnetic field strength [nanoTeslas]
*    @param[out] dFieldZ
*           double: returned Z component of magnetic field strength [nanoTeslas]
*
*    @returns
*           eMAGFIELD_ERROR_CODE: emfNoError success, else error code
*/
    eMAGFIELD_ERROR_CODE getField( const int& iKpBin,
                                   const double& dTiltAngle,
                                   const double& dPosX,
                                   const double& dPosY,
                                   const double& dPosZ,
                                   double& dFieldX,
                                   double& dFieldY,
                                   double& dFieldZ );
    
/*------------------------------------------------------------------*/
// function CMagFieldT89::outOfRange
/**
 * Check that the specified coordinates are within the valid range for
 * this model
 *
 * @param dPosX
 *        double: GSM coordinate X position (Re)
 * @param dPosY
 *        double: GSM coordinate Y position (Re)
 * @param dPosZ
 *        double: GSM coordinate Z position (Re)
 *
 * @returns
 *        bool: true coordinates out of range, else false
 */
    bool outOfRange( const double& dPosX,
                     const double& dPosY,
                     const double& dPosZ );

  private:
    int ciKpBin;        // 1-7 valid, translated from Kp index value ranges
    double cdTiltAngle; // degrees

    // T89 parameters for each of the seven Kp bins
    double cda2Param[7][30] = {
       {-116.53,-10719.,42.375,59.753,-11363.,1.7844,30.268,-0.35372E-01,-0.66832E-01,0.16456E-01,
        -1.3024,0.16529E-02,0.20293E-02,20.289,-0.25203E-01,224.91,-9234.8,22.788,7.8813,1.8362,
        -0.27228,8.8184,2.8714,14.468,32.177,0.01,0.0,7.0459,4.0,20.0},
       {-55.553,-13198.,60.647,61.072,-16064.,2.2534,34.407,-0.38887E-01,-0.94571E-01,0.27154E-01,
        -1.3901,0.13460E-02,0.13238E-02,23.005,-0.30565E-01,55.047,-3875.7,20.178,7.9693,1.4575,
        0.89471,9.4039,3.5215,14.474,36.555,0.01,0.0,7.0787,4.0,20.0},
       {-101.34,-13480.,111.35,12.386,-24699.,2.6459,38.948,-0.34080E-01,-0.12404,0.29702E-01,
        -1.4052,0.12103E-02,0.16381E-02,24.49,-0.37705E-01,-298.32,4400.9,18.692,7.9064,1.3047,
        2.4541,9.7012,7.1624,14.288,33.822,0.01,0.0,6.7442,4.0,20.0},
       {-181.69,-12320.,173.79,-96.664,-39051.,3.2633,44.968,-0.46377E-01,-0.16686,0.048298,
        -1.5473,0.10277E-02,0.31632E-02,27.341,-0.50655E-01,-514.10,12482.,16.257,8.5834,1.0194,
        3.6148,8.6042,5.5057,13.778,32.373,0.01,0.0,7.3195,4.0,20.0},
       {-436.54,-9001.0,323.66,-410.08,-50340.,3.9932,58.524,-0.38519E-01,-0.26822,0.74528E-01,
        -1.4268,-0.10985E-02,0.96613E-02,27.557,-0.56522E-01,-867.03,20652.,14.101,8.3501,0.72996,
        3.8149,9.2908,6.4674,13.729,28.353,0.01,0.0,7.4237,4.0,20.0},
       {-707.77,-4471.9,432.81,-435.51,-60400.,4.6229,68.178,-0.88245E-01,-0.21002,0.11846,
        -2.6711,0.22305E-02,0.10910E-01,27.547,-0.54080E-01,-424.23,1100.2,13.954,7.5337,0.89714,
        3.7813,8.2945,5.174,14.213,25.237,0.01,0.0,7.0037,4.0,20.0},
       {-1190.4,2749.9,742.56,-1110.3,-77193.,7.6727,102.05,-0.96015E-01,-0.74507,0.11214,
        -1.3614,0.15157E-02,0.22283E-01,23.164,-0.74146E-01,-2219.1,48253.,12.714,7.6777,0.57138,
        2.9633,9.3909,9.7263,11.123,21.558,0.01,0.0,4.4518,4.0,20.0} };


};
#endif

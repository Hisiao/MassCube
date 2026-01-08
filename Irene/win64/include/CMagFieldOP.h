/******************************************************************************
File: CMagFieldOP.h

Description: Declarations of methods for the calculations for the 
      Olson-Pfitzer version of the external magnetic field model.

 Rights and Restrictions:

   Copyright 2023 Atmospheric and Environmental Research, Inc. (AER)

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
    1.0     02/09/2023   initial development

******************************************************************************/

#ifndef CMAGFIELD_OP_H
#define CMAGFIELD_OP_H

#include "SpWxTypes.h"
#include "CMagFieldErrors.h"

using namespace std;

// Olson-Pfitzer coefficients
struct sExternField_t {
  double dTiltCurr;  // Current tilt angle
  // Olson-Pfitzer coefficients for the current tilt angle
  double daOpA[32];  // abxyz
  double daOpB[32];  // bbxyz
  double daOpC[22];  // cbxyz
  double daOpD[22];  // dbxyz
  double daOpE[32];  // ebxyz
  double daOpF[32];  // fbxyz
};

/**
 *  @brief Olson-Pfitzer external magnetic field model class
 *
 *  The CMagFieldOP class returns the magnetic field strength vector
 *   at the specified GSM coordinate position.
 *  All position coordinate values are in units of 'Re', where 1 Re = 6371.2km.
 *
 *  The returned magnetic field strength vector values [GSM, in nanoTeslas] are 
 *   intended to be combined with those from the 'main' magnetic field model.
 */

class CMagFieldOP
{

  public:
/**
* Constructor 
*/
    CMagFieldOP();
/**
* Destructor 
*/
    ~CMagFieldOP();

/*------------------------------------------------------------------*/
// function CMagFieldOP::setTiltAngle
/**
*
* Define the magnetic polar tilt angle to be used with all subsequent
*  calls to the 'getField' method, unless otherwise specified.
*
*    @param dTiltAngle
*           double: magnetic pole tilt angle [degrees]
*
*    @returns
*           int: 0 success, else error code 
*/
    eMAGFIELD_ERROR_CODE setTiltAngle( double& dTiltAngle );

/*------------------------------------------------------------------*/
// function CMagFieldOP::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates.
*
*    @param dPosX
*           double: GSM coordinate X position [Re]
*    @param dPosY
*           double: GSM coordinate Y position [Re]
*    @param dPosZ
*           double: GSM coordinate Z position [Re]
*    @param[out] dFieldX
*           double: returned magnetic field strength X component [nanoTeslas]
*    @param[out] dFieldY
*           double: returned magnetic field strength Y component [nanoTeslas]
*    @param[out] dFieldZ
*           double: returned magnetic field strength Z component [nanoTeslas]
*
*    @returns
*           int: 0 success, else error code 
*/
    eMAGFIELD_ERROR_CODE getField( double& dPosX,
                                   double& dPosY,
                                   double& dPosZ,
                                   double& dFieldX,
                                   double& dFieldY,
                                   double& dFieldZ );

/*------------------------------------------------------------------*/
// function CMagFieldOP::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates.
*
*    @param vdPos
*           dvector: GSM coordinate position XYZ [Re]
*    @param[out] vdField
*           dvector: returned magnetic field strength vector [nanoTeslas]
*
*    @returns
*           int: 0 success, else error code 
*/
    eMAGFIELD_ERROR_CODE getField( const dvector& vdPos,
                                   dvector& vdField );

/*------------------------------------------------------------------*/
// function CMagFieldOP::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates.
*
*    @param dTiltAngle
*           double: Magnetic pole tilt angle [degrees]
*    @param dPosX
*           double: GSM coordinate X position [Re]
*    @param dPosY
*           double: GSM coordinate Y position [Re]
*    @param dPosZ
*           double: GSM coordinate Z position [Re]
*    @param[out] dFieldX
*           double: returned magnetic field strength X component [nanoTeslas]
*    @param[out] dFieldY
*           double: returned magnetic field strength Y component [nanoTeslas]
*    @param[out] dFieldZ
*           double: returned magnetic field strength Z component [nanoTeslas]
*
*    @returns
*           int: 0 success, else error code 
*/
    eMAGFIELD_ERROR_CODE getField( double& dTiltAngle,
                                   double& dPosX,
                                   double& dPosY,
                                   double& dPosZ,
                                   double& dFieldX,
                                   double& dFieldY,
                                   double& dFieldZ );

/*------------------------------------------------------------------*/
// function CMagFieldOP::getField
/**
*
* Retrieve magnetic field strength vector at the specified coordinates.
*
*    @param dTiltAngle
*           double: Magnetic pole tilt angle [degrees]
*    @param vdPos
*           dvector: GSM coordinate position XYZ [Re]
*    @param[out] vdField
*           dvector: returned magnetic field strength vector [nanoTeslas]
*
*    @returns
*           int: 0 success, else error code 
*/
    eMAGFIELD_ERROR_CODE getField( double& dTiltAngle,
                                   const dvector& vdPos,
                                   dvector& vdField );

/*------------------------------------------------------------------*/
// function CMagFieldOP::outOfRange
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
    // Convert degrees to radians
    double cdDegToRad;

    // Magnetic pole tilt angle in degrees
    double cdTiltAngle;

    // Olson-Pfitzer parameters
    sExternField_t csOP;

    const int ciaIta[32] = { 2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,2,2,1,2,2,2,1,
                             2,1,2,1,2,1,2,2,2,1 };

    const int ciaItb[22] = { 2,1,2,1,2,2,1,2,2,2,1,2,1,2,1,2,1,2,2,2,1,2 };

    const int ciaItc[32] = { 1,2,1,2,1,1,2,1,2,1,2,1,2,1,2,1,1,2,1,1,1,2,
                             1,2,1,2,1,2,1,1,1,2 };

    const double cdaAa[64] = {   -2.26836e-02,-1.01863e-04, 3.42986e+00,
      -3.12195e-04, 9.50629e-03,-2.91512e-06,-1.57317e-03, 8.62856e-08,
      -4.26478e-05, 1.62924e-08,-1.27549e-04, 1.90732e-06,-1.65983e-02,
       8.46680e-09,-5.55850e-05, 1.37404e-08, 9.91815e-05, 1.59296e-08,
       4.52864e-07,-7.17669e-09, 4.98627e-05, 3.33662e-10,-5.97620e-02,
       1.60669e-05,-2.29457e-01,-1.43777e-04, 1.09403e-03,-9.15606e-07,
       1.60658e-03,-4.01198e-07,-3.15064e-06, 2.03125e-09, 4.92887e-04,
      -1.80676e-07,-1.12022e-03, 5.98568e-07,-5.90009e-06, 5.16504e-09,
      -1.48737e-06, 4.83477e-10,-7.44379e-04, 3.82472e-06, 7.41737e-04,
      -1.31468e-05,-1.24729e-04, 1.92930e-08,-1.91764e-04,-5.30371e-08,
       1.38186e-05,-2.81594e-08, 7.46386e-06, 2.64404e-08, 2.45049e-04,
      -1.81802e-07,-1.00278e-03, 1.98742e-06,-1.16425e-05, 1.17556e-08,
      -2.46079e-06,-3.45831e-10, 1.02440e-05,-1.90716e-08,-4.00855e-05,
       1.25818e-07 };

    const double cdaBb[64] = {    9.47753e-02, 1.45981e-04,-1.82933e+00,
       5.54882e-04, 5.03665e-03,-2.07698e-06, 1.10959e-01,-3.45837e-05,
      -4.40075e-05, 5.06464e-07,-1.20112e-03, 3.64911e-06, 1.49849e-01,
      -7.44929e-05, 2.46382e-04, 9.65870e-07,-9.54881e-04, 2.43647e-07,
       3.06520e-04, 3.07836e-07, 6.48301e-03, 1.26251e-06,-7.09548e-03,
      -1.55596e-05, 3.06465e+00,-7.84893e-05, 4.95145e-03, 3.71921e-06,
      -1.52002e-01, 6.81988e-06,-8.55686e-05,-9.01230e-08,-3.71458e-04,
       1.30476e-07,-1.82971e-01, 1.51390e-05,-1.45912e-04,-2.22778e-07,
       6.49278e-05,-3.72758e-08,-1.59932e-03, 8.04921e-06, 5.38012e-01,
      -1.43182e-04, 1.50000e-04, 5.88020e-07,-1.59000e-02, 1.60744e-06,
       3.17837e-04, 1.78959e-07,-8.93794e-03, 6.37549e-06, 1.27887e-03,
      -2.45878e-07,-1.93210e-01, 6.91233e-06,-2.80637e-04,-2.57073e-07,
       5.78343e-05, 4.52128e-10, 1.89621e-04,-4.84911e-08,-1.50058e-02,
       6.21772e-06 };

    const double cdaCc[44] = {   -1.88177e-02,-1.92493e-06,-2.89064e-01,
      -8.49439e-05,-4.76380e-04,-4.52998e-08, 1.61086e-03, 3.18728e-07,
       1.29159e-06, 5.52259e-10, 3.95543e-05, 5.61209e-08, 1.38287e-03,
       5.74237e-07, 1.86489e-06, 7.10175e-10, 1.45243e-07,-2.97591e-10,
      -2.43029e-03,-6.70000e-07,-2.30624e-02,-6.22193e-06,-2.40815e-05,
       2.01689e-08, 1.76721e-04, 3.78689e-08, 9.88496e-06, 7.33820e-09,
       7.32126e-05, 8.43986e-08, 8.82449e-06,-6.11708e-08, 1.78881e-04,
       8.62589e-07, 3.43724e-06, 2.53783e-09,-2.04239e-07, 8.16641e-10,
       1.68075e-05, 7.62815e-09, 2.26026e-04, 3.66341e-08, 3.44637e-07,
       2.25531e-10 };

    const double cdaDd[44] = {    2.50143e-03, 1.01200e-06, 3.23821e+00,
       1.08589e-05,-3.39199e-05,-5.27052e-07,-9.46161e-02,-1.95413e-09,
      -4.23614e-06, 1.43153e-08,-2.62948e-04, 1.05138e-07,-2.15784e-01,
      -2.20717e-07,-2.65687e-05, 1.26370e-08, 5.88917e-07,-1.13658e-08,
       1.64385e-03, 1.44263e-06,-1.66045e-01,-1.46096e-05, 1.22811e-04,
       3.43922e-08, 9.66760e-05,-6.32150e-07,-4.97400e-05,-2.78578e-08,
       1.77366e-02, 2.05401e-07,-1.91756e-03,-9.49392e-07,-1.99488e-01,
      -2.07170e-06,-5.40443e-05, 1.59289e-08, 7.30914e-05, 3.38786e-08,
      -1.59537e-04,-1.65504e-07, 1.90940e-02, 2.03238e-06, 1.01148e-04,
       5.20815e-08 };

    const double cdaEe[64] = {   -2.77924e+01,-1.01457e-03, 9.21436e-02,
      -8.52177e-06, 5.19106e-01, 8.28881e-05,-5.59651e-04, 1.16736e-07,
      -2.11206e-03,-5.35469e-07, 4.41990e-01,-1.33679e-05,-7.18642e-04,
       6.17358e-08,-3.51990e-03,-5.29070e-07, 1.88443e-06,-6.60696e-10,
      -1.34708e-03, 1.02160e-07, 1.58219e-06, 2.05040e-10, 1.18039e+00,
       1.58903e-04, 1.86944e-02,-4.46477e-06, 5.49869e-02, 4.94690e-06,
      -1.18335e-04, 6.95684e-09,-2.73839e-04,-9.17883e-08, 2.79126e-02,
      -1.02567e-05,-1.25427e-04, 3.07143e-08,-5.31826e-04,-2.98476e-08,
      -4.89899e-05, 4.91480e-08, 3.85563e-01, 4.16966e-05, 6.74744e-04,
      -2.08736e-07,-3.42654e-03,-3.13957e-06,-6.31361e-06,-2.92981e-09,
      -2.63883e-03,-1.32235e-07,-6.19406e-06, 3.54334e-09, 6.65986e-03,
      -5.81949e-06,-1.88809e-04, 3.62055e-08,-4.64380e-04,-2.21159e-07,
      -1.77496e-04, 4.95560e-08,-3.18867e-04,-3.17697e-07,-1.05815e-05,
       2.22220e-09 };

    const double cdaFf[64] = {   -5.07092e+00, 4.71960e-03,-3.79851e-03,
      -3.67309e-06,-6.02439e-01, 1.08490e-04, 5.09287e-04, 5.62210e-07,
       7.05718e-02, 5.13160e-06,-2.85571e+00,-4.31728e-05, 1.03185e-03,
       1.05332e-07, 1.04106e-02, 1.60749e-05, 4.18031e-05, 3.32759e-08,
       1.20113e-01, 1.40486e-05,-3.37993e-05, 5.48340e-09, 9.10815e-02,
      -4.00608e-04, 3.75393e-03,-4.69939e-07,-2.48561e-02, 1.31836e-04,
      -2.67755e-04,-7.60285e-08, 3.04443e-03,-3.28956e-06, 5.82367e-01,
       5.39496e-06,-6.15261e-04, 4.05316e-08, 1.13546e-02,-4.26493e-06,
      -2.72007e-02, 5.72523e-08,-2.98576e+00, 3.07325e-05, 1.51645e-03,
       1.25098e-06, 4.07213e-02, 1.05964e-05, 1.04232e-04, 1.77381e-08,
       1.92781e-01, 2.15734e-05,-1.65741e-05,-1.88683e-09, 2.44803e-01,
       1.51316e-05,-3.01157e-04, 8.47006e-08, 1.86971e-02,-6.94074e-06,
       9.13198e-03,-2.38052e-07, 1.28552e-01, 6.92595e-06,-8.36792e-05,
      -6.10021e-08 };

    // Calculate Olson-Pfitzer external field
    eMAGFIELD_ERROR_CODE bxyzmu( double& dX, double& dY, double& dZ, 
                                 double& dBx, double& dBy, double& dBz );

    // Helper function for bxyzmu
    eMAGFIELD_ERROR_CODE bxyzmuZpart( double& dExpr, double& dZeyexb, double& dBx, double& dBz,
                                      int& iKkc, int& iKk, int& iIic, int& iIi );

    // Helper function for bxyzmu
    eMAGFIELD_ERROR_CODE bxyzmuYpart( double& dExpr, double& dZeyexb, double& dY, double& dZ,
                                      double& dBy, int& iJjc, int& iJj, int& iIjkc, int& iIjk,
                                      int& iKc, int& iK );

    // Transform from GSM to SM coordinates and vice versa
    void gsmsm( double& dXgsm, double& dYgsm, double& dZgsm,
                double& dXsm,  double& dYsm, double& dZsm, bool bForward=true );

};
#endif

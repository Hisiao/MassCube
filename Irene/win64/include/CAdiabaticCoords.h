/******************************************************************************
File: CAdiabaticCoords.h

Description: The CAdiabaticCoords class calculates the adiabatic invariant
   coordinate values for specified time/position values.

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

  Version      Date       Notes
  1.0          01/05/2011 Created
  
******************************************************************************/
#ifndef CADIABATICCOORDS_H
#define CADIABATICCOORDS_H

#include "CAdAlphaLmCoordCalc.h"
#include "CAdKHMinCoordCalc.h"
#include "CAdKPhiCoordCalc.h"
#include <memory>

/**
 * @file: CAdiabaticCoords.h
 * The CAdiabaticCoords class calculates the adiabatic invariant coordinate values for specified time/position values.
 */
  class CAdiabaticCoords
  {
  public:


/*------------------------------------------------------------------*/
// function CAdiabaticCoords::CAdiabaticCoords
/**
*
* Default constructor
*
*/
      CAdiabaticCoords();

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::~CAdiabaticCoords
/**
*
* Destructor
*
*/
      ~CAdiabaticCoords();

      // initialize Magfield with database name,
      //    or with pointer to previously-created instance

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::initialize
/**
*
* Initializes the neural net databases used in coordinate calculators,
* and magnetic field model.
*
*    @param string strKPhiNNetDataSource
*           name of K&Phi neural net database file
*    @param string strKHminNNetDataSource
*           name of K&Hmin neural net database file
*    @param string strMagDbName
*           name of IGRF database file
*
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE initialize( const string& strKPhiNNetDataSource,
                            const string& strKHMinNNetDataSource,
                            const string &sMagDbName );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::initialize
/**
*
* Initializes the neural net databases used in coordinate calculators,
* and magnetic field model.
*
*    @param strKPhiNNetDataSource
*           name of K&Phi neural net database file
*    @param strKHminNNetDataSource
*           name of K&Hmin neural net database file
*    @param pCMagfield
*           pointer to previously-initialized magnetic field model
*
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE initialize( const string& strKPhiNNetDataSource,
                            const string& strKHMinNNetDataSource,
                            CMagfield* pCMagfield );

      // settings for the MagField can be changed using the full set
      //  of methods available from the magnetic field model itself
      CMagfield *getMagFieldInstance () { return ( cpMagfield ); }

      // change limits for K, HMin and Phi values returned in computeCoordinateSet
      //  A value of -1e+31 is returned when invalid or outside limits
      //  -when called with no arguments -> resets values to their defaults
/*------------------------------------------------------------------*/
// function CAdiabaticCoords::setLimitsK
/**
*
* Sets the range of K values returned from 'computeCoordinateSet' method
*
*    @param double dKMin
*           minimum K value <br>
*           [default = 0.0]
*    @param double dKMax
*           maximum K value <br>
*           [default = 15.0]
*
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE setLimitsK ( 
                            const double& dKMin = 0.0, 
                            const double& dKMax = 15.0 );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::setLimitsHmin
/**
*
* Sets the range of Hmin values returned from 'computeCoordinateSet' method
*
*    @param double dHminMin
*           minimum Hmin value <br>
*           [default = -500.0]
*    @param double dHminMax
*           maximum Hmin value <br>
*           [default = 10000.0]
*
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE setLimitsHmin ( 
                            const double& dHminMin = -500.0, 
                            const double& dHminMax = 10000.0 );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::setLimitsPhi
/**
*
* Sets the range of Phi values returned from 'computeCoordinateSet' method
*
*    @param double dPhiMin
*           minimum Phi value <br>
*           [default = 0.15849]
*    @param double dPhiMax
*           maximum Phi value <br>
*           [default = 2.0]
*
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE setLimitsPhi ( 
                            const double& dPhiMin = 0.15849, 
                            const double& dPhiMax = 2.0 );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::setLimitsLog10Phi
/**
*
* Sets the range of Phi values returned from 'computeCoordinateSet' method,
*  but in terms of log10(phi)
*
*    @param double dLog10PhiMin
*           minimum Log10(Phi) value <br>
*           [default = -0.8]
*    @param double dLog10PhiMax
*           maximum Log10(Phi) value <br>
*           [default = 0.3]
*
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE setLimitsLog10Phi ( 
                            const double& dLog10PhiMin = -0.8, 
                            const double& dLog10PhiMax = 0.3 );
      // use of time-varying mag moment is NOT recommended

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::setMagMomentTime
/**
*
* Sets the date for the magentic moment calculation.
* This moment value is used in Phi to L* conversion.
*
* Specify 0.0 for a time-varying calculation, but this makes the
* resulting L* __NOT__ an 'adiabatic invariant'
*
*    @param double dMagMomentTime
*           MJD or Composite time value
*
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE setMagMomentTime ( const double& dMagMomentTime );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::computeCoordinateSet
/**
*
* This method computes the adiabatic invariant coordinates for each of a set
* of passed geospatial positions, times and a single pitch angle. The 
* position's computed coordinates are in coordinate domains (alpha-Lm, K-Phi, 
* K-HMin).  The L* values are also returned, associated with the Phi value. 
*  [[ single time, single pitch angle ]]
*
*    @param emfCoordSys eCoordSys
*           Geospatial coordinate system of the input position data
*    @param double dDateTimes
*           time value (MJD or Composite) for input position
*    @param S3Tuple s3Pos
*           coordinates of input position to process
*    @param double dPitchAngle
*           single pitch angle at position to compute
*
*    @return double dAlpha
*           Alpha spatial coordinate
*    @return double dLm
*           Lm spatial coordinate
*    @return double dK
*           K spatial coordinate
*    @return double dPhi
*           Phi spatial coordinate
*    @return double dHmin
*           Hmin spatial coordinate
*    @return double dLstar
*           L* spatial coordinate
*    @return double dvector vdBmin
*           minimum B magnitude
*    @return double dBlocal
*           local B magnitudes
*    @return s3Bcomp
*           local B components (geocentric cartesian)
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE computeCoordinateSet( 
                            const emfCoordSys& eCoordSys,
                            const double&  dDateTime,
                            const S3Tuple& s3Pos,
                            const double&  dPitchAngle,
                            double&        dAlpha,
                            double&        dLm, 
                            double&        dK, 
                            double&        dPhi,
                            double&        dHmin, 
                            double&        dLstar,
                            double&        dBmin,
                            double&        dBlocal,
                            S3Tuple&       s3Bcomp );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::computeCoordinateSet
/**
*
* This method computes the adiabatic invariant coordinates for each of a set
* of passed geospatial positions, times and a single pitch angle. The 
* position's computed coordinates are in coordinate domains (alpha-Lm, K-Phi, 
* K-HMin).  The L* values are also returned, associated with the Phi value. 
*  [[  single time, single pitch angle ]]
*
*    @param emfCoordSys eCoordSys
*           Geospatial coordinate system of the input position data
*    @param double dDateTimes
*           time value (CTimeValue form) for input position
*    @param S3Tuple s3Pos
*           coordinates of input position to process
*    @param double dPitchAngle
*           single pitch angle at position to compute
*
*    @return double dAlpha
*           Alpha spatial coordinate
*    @return double dLm
*           Lm spatial coordinate
*    @return double dK
*           K spatial coordinate
*    @return double dPhi
*           Phi spatial coordinate
*    @return double dHmin
*           Hmin spatial coordinate
*    @return double dLstar
*           L* spatial coordinate
*    @return double dvector vdBmin
*           minimum B magnitude
*    @return double dBlocal
*           local B magnitudes
*    @return s3Bcomp
*           local B components (geocentric cartesian)
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE computeCoordinateSet( 
                            const emfCoordSys& eCoordSys,
                            const CTimeValue& tTime,
                            const S3Tuple& s3Pos,
                            const double&  dPitchAngle,
                            double&        dAlpha,
                            double&        dLm, 
                            double&        dK, 
                            double&        dPhi,
                            double&        dHmin, 
                            double&        dLstar,
                            double&        dBmin,
                            double&        dBlocal,
                            S3Tuple&       s3Bcomp );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::computeCoordinateSet
/**
*
* This method computes the adiabatic invariant coordinates for a set
* of passed geospatial position, time and a single pitch angle. The 
* position's computed coordinates are in coordinate domains (alpha-Lm, K-Phi, 
* K-HMin).  The L* value is also returned, associated with the Phi value. 
*    [[ single time, multiple pitch angles ]]
*
*    @param emfCoordSys eCoordSys
*           Geospatial coordinate system of the input position data
*    @param double dDateTime
*           time value (MJD or Composite) for input position
*    @param SeTuple s3Pos
*           coordinates of input position to process
*    @param dvector vdPitchAngles
*           vector of pitch angles at each position to compute
*
*    @return dvector vdAlpha
*           vector (pitch angles) of Alpha spatial coordinate
*    @return dvector vdLm
*           vector (pitch angles) of Lm spatial coordinate
*    @return dvector vdK
*           vector (pitch angles) of K spatial coordinate
*    @return dvector vdPhi
*           vector (pitch angles) of Phi spatial coordinate
*    @return dvector vdHmin
*           vector (pitch angles) of Hmin spatial coordinate
*    @return dvector vdLstar
*           vector (pitch angles) of L* spatial coordinate
*    @return dBmin
*           minimum B magnitude
*    @return dBlocal
*           local B magnitude
*    @return S3TupleVec s3vBcomp
*           local B components (geocentric cartesian)
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE computeCoordinateSet( 
                            const emfCoordSys& eCoordSys,
                            const double&  dDateTime,
                            const S3Tuple& s3Pos,
                            const dvector& vdPitchAngles,
                            dvector&       vdAlpha,
                            dvector&       vdLm, 
                            dvector&       vdK, 
                            dvector&       vdPhi,
                            dvector&       vdHmin, 
                            dvector&       vdLstar,
                            double&        dBmin,
                            double&        dBlocal,
                            S3Tuple&       s3Bcomp );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::computeCoordinateSet
/**
*
* This method computes the adiabatic invariant coordinates for a set
* of passed geospatial position, time  at multiple pitch angles. The 
* position's computed coordinates are in coordinate domains (alpha-Lm, K-Phi, 
* K-HMin).  The L* value is also returned, associated with the Phi value. 
*   [[ single time, multiple pitch angles ]]
*
*    @param emfCoordSys eCoordSys
*           Geospatial coordinate system of the input position data
*    @param CTimeValue tTime
*           time value (CTimeValue form) for input position
*    @param S3Tuple s3Pos
*           coordinates of input position to process
*    @param dvector vdPitchAngles
*           vector of pitch angles at each position to compute
*
*    @return dvector vdAlpha
*           vector (pitch angles) of Alpha spatial coordinate
*    @return dvector vdLm
*           vector (pitch angles) of Lm spatial coordinate
*    @return dvector vdK
*           vector (pitch angles) of K spatial coordinate
*    @return dvector vdPhi
*           vector (pitch angles) of Phi spatial coordinate
*    @return dvector vdHmin
*           vector (pitch angles) of Hmin spatial coordinate
*    @return dvector vdLstar
*           vector (pitch angles) of L* spatial coordinate
*    @return dBmin
*           minimum B magnitude
*    @return dBlocal
*           local B magnitude
*    @return S3Tuple s3Bcomp
*           local B components (geocentric cartesian)
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE computeCoordinateSet( 
                            const emfCoordSys& eCoordSys,
                            const CTimeValue& tTime,
                            const S3Tuple& s3Pos,
                            const dvector& vdPitchAngles,
                            dvector&       vdAlpha,
                            dvector&       vdLm, 
                            dvector&       vdK, 
                            dvector&       vdPhi,
                            dvector&       vdHmin, 
                            dvector&       vdLstar,
                            double&        dBmin,
                            double&        dBlocal,
                            S3Tuple&       s3Bcomp );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::computeCoordinateSet
/**
*
* This method computes the adiabatic invariant coordinates for a set
* of passed geospatial position, time at multiple pitch angles. The 
* position's computed coordinates are in coordinate domains (alpha-Lm, K-Phi, 
* K-HMin).  The L* values are also returned, associated with the Phi value. 
*
*    @param emfCoordSys eCoordSys
*           Geospatial coordinate system of the input position data
*    @param dvector vdDateTimes
*           vector of time values (MJD or Composite) for each input position
*    @param S3TupleVec s3vPositions
*           vector of coordinates of each input position to process
*    @param double dPitchAngle
*           single pitch angle at each position to compute
*
*    @return dvector vdAlpha
*           vector (times) of Alpha spatial coordinate
*    @return dvector vdLm
*           vector (times) of Lm spatial coordinate
*    @return dvector vdK
*           vector (times) of K spatial coordinate
*    @return dvector vdPhi
*           vector (times) of Phi spatial coordinate
*    @return dvector vdHmin
*           vector (times) of Hmin spatial coordinate
*    @return dvector vdLstar
*           vector (times) of L* spatial coordinate
*    @return dvector vdBmin
*           vector (times) of minimum B magnitudes
*    @return dvector vdBlocal
*           vector (times) of local B magnitudes
*    @return S3TupleVec s3vBcomp
*           vector (times) of local B components (geocentric cartesian)
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE computeCoordinateSet( 
                            const emfCoordSys& eCoordSys,
                            const dvector& vdDateTimes,
                            const S3TupleVec& s3vPositions,
                            const double& dPitchAngle,
                            dvector&      vdAlpha,
                            dvector&      vdLm, 
                            dvector&      vdK, 
                            dvector&      vdPhi,
                            dvector&      vdHmin, 
                            dvector&      vdLstar,
                            dvector&      vdBmin,
                            dvector&      vdBlocal,
                            S3TupleVec&   s3vBcomp );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::computeCoordinateSet
/**
*
* This method computes the adiabatic invariant coordinates for each of a set
* of passed geospatial positions, times and a single pitch angle. The 
* position's computed coordinates are in coordinate domains (alpha-Lm, K-Phi, 
* K-HMin).  The L* values are also returned, associated with the Phi value. 
*
*    @param emfCoordSys eCoordSys
*           Geospatial coordinate system of the input position data
*    @param tvector vtTimes
*           vector of CTimeValue time values for each input position
*    @param S3TupleVec s3vPositions
*           vector of coordinates of each input position to process
*    @param double dPitchAngle
*           single pitch angle at each position to compute
*
*    @return dvector vdAlpha
*           vector (times) of Alpha spatial coordinate
*    @return dvector vdLm
*           vector (times) of Lm spatial coordinate
*    @return dvector vdK
*           vector (times) of K spatial coordinate
*    @return dvector vdPhi
*           vector (times) of Phi spatial coordinate
*    @return dvector vdHmin
*           vector (times) of Hmin spatial coordinate
*    @return dvector vdLstar
*           vector (times) of L* spatial coordinate
*    @return dvector vdBmin
*           vector (times) of minimum B magnitudes
*    @return dvector vdBlocal
*           vector (times) of local B magnitudes
*    @return S3TupleVec s3vBcomp
*           vector (times) of local B components (geocentric cartesian)
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE computeCoordinateSet( 
                            const emfCoordSys& eCoordSys,
                            const tvector& vtTimes,
                            const S3TupleVec& s3vPositions,
                            const double& dPitchAngle,
                            dvector&      vdAlpha,
                            dvector&      vdLm, 
                            dvector&      vdK, 
                            dvector&      vdPhi,
                            dvector&      vdHmin, 
                            dvector&      vdLstar,
                            dvector&      vdBmin,
                            dvector&      vdBlocal,
                            S3TupleVec&   s3vBcomp );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::computeCoordinateSet
/**
*
* This method computes the adiabatic invariant coordinates for each of a set
* of passed geospatial positions, times at multiple pitch angles. The 
* position's computed coordinates are in coordinate domains (alpha-Lm, K-Phi, 
* K-HMin).  The L* values are also returned, associated with the Phi value. 
*
*    @param emfCoordSys eCoordSys
*           Geospatial coordinate system of the input position data
*    @param dvector vdDateTimes
*           vector of time values (MJD or Composite) for each input position
*    @param S3TupleVec s3vPositions
*           vector of coordinates of each input position to process
*    @param vdPitchAngles
*           vector of pitch angles at each position to compute
*
*    @return bmdmatrix bmdAlpha
*           matrix (times x angles) of Alpha spatial coordinate
*    @return bmdmatrix bmdLm
*           matrix (times x angles) of Lm spatial coordinate
*    @return bmdmatrix bmdK
*           matrix (times x angles) of K spatial coordinate
*    @return bmdmatrix bmdPhi
*           matrix (times x angles) of Phi spatial coordinate
*    @return bmdmatrix bmdHmin
*           matrix (times x angles) of Hmin spatial coordinate
*    @return bmLstar
*           matrix (times x angles) of L* spatial coordinate
*    @return dvector vdBmin
*           vector (times) of minimum B magnitudes
*    @return dvector vdBlocal
*           vector (times) of local B magnitudes
*    @return S3TupleVec s3vBcomp
*           vector (times) of local B components (geocentric cartesian)
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE computeCoordinateSet( 
                            const emfCoordSys& eCoordSys,
                            const dvector& vdDateTimes,
                            const S3TupleVec& s3vPositions,
                            const dvector& vdPitchAngles,
                            bdmatrix&      bmdAlpha,
                            bdmatrix&      bmdLm, 
                            bdmatrix&      bmdK, 
                            bdmatrix&      bmdPhi,
                            bdmatrix&      bmdHmin, 
                            bdmatrix&      bmdLstar,
                            dvector&       vdBmin,
                            dvector&       vdBlocal,
                            S3TupleVec&    s3vBcomp );

/*------------------------------------------------------------------*/
// function CAdiabaticCoords::computeCoordinateSet
/**
*
* This method computes the adiabatic invariant coordinates for each of a set
* of passed geospatial positions, times at multiple pitch angles. The 
* position's computed coordinates are in coordinate domains (alpha-Lm, K-Phi, 
* K-HMin).  The L* values are also returned, associated with the Phi value. 
*
*    @param emfCoordSys eCoordSys
*           Geospatial coordinate system of the input position data
*    @param tvector vtTimes
*           vector of CTimeValue time values for each input position
*    @param S3TupleVec s3vPositions
*           vector of coordinates of each input position to process
*    @param vdPitchAngles
*           vector of pitch angles at each position to compute
*
*    @return bmdmatrix bmdAlpha
*           matrix (times x angles) of Alpha spatial coordinate
*    @return bmdmatrix bmdLm
*           matrix (times x angles) of Lm spatial coordinate
*    @return bmdmatrix bmdK
*           matrix (times x angles) of K spatial coordinate
*    @return bmdmatrix bmdPhi
*           matrix (times x angles) of Phi spatial coordinate
*    @return bmdmatrix bmdHmin
*           matrix (times x angles) of Hmin spatial coordinate
*    @return bmdmatrix bmLstar
*           matrix (times x angles) of L* spatial coordinate
*    @return dvector vdBmin
*           vector (times) of minimum B magnitudes
*    @return dvector vdBlocal
*           vector (times) of local B magnitudes
*    @return S3TupleVec s3vBcomp
*           vector (times) of local B components (geocentric cartesian)
*    @return eGENERIC_ERROR_CODE
*           enumerated error code (see GenericModelEnum.h)
*/
      eGENERIC_ERROR_CODE computeCoordinateSet( 
                            const emfCoordSys& eCoordSys,
                            const tvector& vtTimes,
                            const S3TupleVec& s3vPositions,
                            const dvector& vdPitchAngles,
                            bdmatrix&      bmdAlpha,
                            bdmatrix&      bmdLm, 
                            bdmatrix&      bmdK, 
                            bdmatrix&      bmdPhi,
                            bdmatrix&      bmdHmin, 
                            bdmatrix&      bmdLstar,
                            dvector&       vdBmin,
                            dvector&       vdBlocal,
                            S3TupleVec&    s3vBcomp );

      eGENERIC_ERROR_CODE getCoordinateSetMLT( double& dMagLT );

      eGENERIC_ERROR_CODE getCoordinateSetMLT( dvector& vdMagLT );


      eGENERIC_ERROR_CODE getCoordinateSetBVector( dvector& vdB );

      eGENERIC_ERROR_CODE getCoordinateSetBVector( vdvector& vvdB );


      eGENERIC_ERROR_CODE getCoordinateSetI( dvector& vdI );

      eGENERIC_ERROR_CODE getCoordinateSetI( vdvector& vvdI );

  private:

      // internal helper methods

      eGENERIC_ERROR_CODE initializeNeuralNet( 
                            const string& strKPhiNNetDataSource,
                            const string& strKHMinNNetDataSource );
      eGENERIC_ERROR_CODE initializeMagField( const string &sMagDbName );

      eGENERIC_ERROR_CODE computeMagneticFieldData( 
                            const emfCoordSys&  eCoordSys,
                            const tvector& vtTimes,
                            const S3TupleVec& s3vPositions,
                            const dvector& vdPitchAngles,
                            bdmatrix& bmdB,
                            dvector&  vdBmin,
                            dvector&  vdBlocal,
                            dvector&  vdMagLT,
                            dvector&  vdMagMoment,
                            bdmatrix& bmdI,
                            bdmatrix& bmdBmirror,
                            bdmatrix& bmdLm );
                                    
      eGENERIC_ERROR_CODE localComputeAdInvariants( 
                            const CTimeValue& tTime, 
                            const S3Tuple& s3Position, 
                            const dvector& vdPitchAngles,
                            const ivector& viIndices,
                            double&  dBMin,
                            double& dMagLT,
                            dvector& vdB, 
                            dvector& vdLm, 
                            dvector& vdI );

      void quickSortDesc( dvector& vdPitchAngles, ivector& viPAIndices );

      bool isShabanskyOrbit( const double& dGmtsec, 
                             const double& dBMinXGEO, 
                             const double& dBMinYGeo, 
                             const double& dBMinZGeo,
                             double& dMagLocalTime );

      // protected data members

      unique_ptr<CAdAlphaLmCoordCalc> m_ApAlphaLmCoordCalc;  
      unique_ptr<CAdKPhiCoordCalc> m_ApKPhiCoordCalc;  
      unique_ptr<CAdKHMinCoordCalc> m_ApKHMinCoordCalc;  
      CAdAlphaLmCoordCalc* m_pAlphaLmCoordCalc;  
      CAdKPhiCoordCalc* m_pKPhiCoordCalc;  
      CAdKHMinCoordCalc* m_pKHMinCoordCalc;  

      // magfield is allocated and initialized either internally or externally
      unique_ptr<CMagfield> cApMagfield;
      CMagfield* cpMagfield;
      
      double m_dKMin, m_dKMax;
      double m_dHminMin, m_dHminMax;
      double m_dPhiMin, m_dPhiMax;
      double m_dMagMomentTime;

      // additional parameters with separate access
      dvector m_vdMagLT;
      vdvector m_vvdB;
      vdvector m_vvdI;

      // prevent auto-creation, no reason for them to exist
      CAdiabaticCoords( const CAdiabaticCoords& source ); 
      CAdiabaticCoords& operator =( const CAdiabaticCoords& source ); 
  };

#endif

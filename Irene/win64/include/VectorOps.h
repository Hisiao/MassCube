/***********************************************************************

Module: VectorOps.h

Description: Declarations for variety of basic vector-related functions. 

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

   VERSION     DATE         NOTES
     1.0       08/16/2011   Created

------------------------------------------------------------------------

***********************************************************************/

#ifndef VECTOROPS_H
#define VECTOROPS_H

#include "SpWxTypes.h" // needed for S3Tuple definition

class CVectorOps
{
 public:

/*------------------------------------------------------------------*/
// function CVectorOps
/**
* Default constructor
*
*
*/
  CVectorOps();

/*------------------------------------------------------------------*/
// function ~CVectorOps
/**
* Destructor
*
*
*/
  ~CVectorOps();

public:

  // calculate angle between vectors, in degrees or radians
  //  - these vectors do NOT need to be unit vectors

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorAngleDegrees
/**
*  Calculate angle (in degrees) between two vectors.  
*  These vectors do NOT need to be _unit_ vectors.
*
*     @param DV1x, DV1y, DV1z
*         double cartesian coordinates of first vector
*     @param DV2x, DV2y, DV2z
*         double cartesian coordinates of second vector
*
*     @return
*         double angle in degrees
*/
  double calcVectorAngleDegrees( const double& dV1x,
                                 const double& dV1y,
                                 const double& dV1z,
                                 const double& dV2x,
                                 const double& dV2y,
                                 const double& dV2z );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorAngleRadians
/**
*  Calculate angle (in radians) between two vectors.  
*  These vectors do NOT need to be _unit_ vectors.
*
*     @param DV1x, DV1y, DV1z
*         double cartesian coordinates of first vector
*     @param DV2x, DV2y, DV2z
*         double cartesian coordinates of second vector
*
*     @return
*         double angle in radians
*/
  double calcVectorAngleRadians( const double& dV1x,
                                 const double& dV1y,
                                 const double& dV1z,
                                 const double& dV2x,
                                 const double& dV2y,
                                 const double& dV2z );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorAngleDegrees
/**
*  Calculate angle (in degrees) between two vectors.  
*  These vectors do NOT need to be _unit_ vectors.
*
*     @param daVector1
*         double array cartesian coordinates of first vector
*     @param daVector2
*         double array cartesian coordinates of second vector
*
*     @return
*         double angle in degrees
*/
  double calcVectorAngleDegrees( const double daVector1[3],
                                 const double daVector2[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorAngleRadians
/**
*  Calculate angle (in radians) between two vectors.  
*  These vectors do NOT need to be _unit_ vectors.
*
*     @param daVector1
*         double array cartesian coordinates of first vector
*     @param daVector2
*         double array cartesian coordinates of second vector
*
*     @return
*         double angle in radians
*/
  double calcVectorAngleRadians( const double daVector1[3],
                                 const double daVector2[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorAngleDegrees
/**
*  Calculate angle (in degrees) between two vectors.  
*  These vectors do NOT need to be _unit_ vectors.
*
*     @param s3V1
*         S3Tuple coordinates for first vector
*     @param s3V2
*         S3Tuple coordinates for second vector
*
*     @return double
*         angle in degrees
*/
  double calcVectorAngleDegrees( const S3Tuple& s3V1,
                                 const S3Tuple& s3V2 );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorAngleRadians
/**
*  Calculate angle (in radians) between two vectors.  
*  These vectors do NOT need to be _unit_ vectors.
*
*     @param s3V1
*         S3Tuple coordinates for first vector
*     @param s3V2
*         S3Tuple coordinates for second vector
*
*     @return
*         double angle in radians
*/
  double calcVectorAngleRadians( const S3Tuple& s3V1,
                                 const S3Tuple& s3V2 );

  // calculate angle formed from three locations, where V2 is the vertex
/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorAngleDegrees
/**
*  Calculate angle (in degrees) formed from three locations, 
*  where V2 is the vertex.  
*
*     @param s3V1
*         S3Tuple coordinates for first position
*     @param s3V2
*         S3Tuple coordinates for second position = VERTEX
*     @param s3V3
*         S3Tuple coordinates for third position
*
*     @return double
*         angle in degrees
*/
  double calcVectorAngleDegrees( const S3Tuple& s3V1,
                                 const S3Tuple& s3V2,
                                 const S3Tuple& s3V3 );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorAngleRadians

/**
*  Calculate angle (in radians) formed from three locations, 
*  where V2 is the vertex.  
*
*     @param s3V1
*         S3Tuple coordinates for first position
*     @param s3V2
*         S3Tuple coordinates for second position = VERTEX
*     @param s3V3
*         S3Tuple coordinates for third position
*
*     @return
*         double angle in radians
*/
  double calcVectorAngleRadians( const S3Tuple& s3V1,
                                 const S3Tuple& s3V2,
                                 const S3Tuple& s3V3 );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorDotProduct

/**
*  Calculate the dot product of two vectors :
*      V1.V2 = |V1|*|V2|*cos(angle between).
*
*     @param s3V1
*         S3Tuple coordinates for first vector
*     @param s3V2
*         S3Tuple coordinates for second vector
*
*     @return
*         double dot product value
*/
  double calcVectorDotProduct( const S3Tuple& s3V1,
                               const S3Tuple& s3V2 );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorDotProduct

/**
*  Calculate the dot product of two vectors :
*      V1.V2 = |V1|*|V2|*cos(angle between).
*
*     @param daVector1
*         double array coordinates for first vector
*     @param daVector2
*         double array coordinates for second vector
*
*     @return
*         double dot product value
*/
  double calcVectorDotProduct( const double daVector1[3],
                               const double daVector2[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorCrossProduct
/**
*  perform cross product of two vectors, returning new vector
*
*     @param s3V1
*         S3Tuple coordinates for first vector
*     @param s3V2
*         S3Tuple coordinates for second vector
*
*     @param[out] s3VCross
*         S3Tuple coordinates for resultant cross product vector
*
*/
  void calcVectorCrossProduct( const S3Tuple& s3V1,
                               const S3Tuple& s3V2,
                               S3Tuple& s3VCross );
/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorCrossProduct
/**
*  perform cross product of two vectors, returning new vector
*
*     @param daVector1
*         double array coordinates for first vector
*     @param daVector2
*         double array coordinates for second vector
*
*     @param[out] daVectorResult
*         double array coordinates for resultant cross product vector
*
*/
  void calcVectorCrossProduct( const double daVector1[3],
                               const double daVector2[3],
                               double daVectorResult[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::calcUnitVector
/**
*  return unit vector of input vector and its magnitude
*
*     @param s3Vector
*         S3Tuple coordinates for vector (unchanged)
*
*     @param[out] s3UnitVector
*         S3Tuple coordinates for resultant unit vector
*     @return
*         double magnitude of input vector vector
*
*/
  double calcUnitVector( const S3Tuple& s3Vector,
                         S3Tuple& s3UnitVector );

/*------------------------------------------------------------------*/
// function CVectorOps::calcUnitVector
/**
*  return unit vector of input vector and its magnitude
*
*     @param daVector
*         double array coordinates for vector (unchanged)
*
*     @param[out] daUnitVector
*         double array coordinates for resultant unit vector
*     @return
*         double magnitude of input vector vector
*
*/
   double calcUnitVector( const double daVector[3],
                          double daUnitVector[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::calcUnitVector
/**
*  return in-place unit vector of input vector and its magnitude
*
*     @param[in,out] s3VectorInOut
*         S3Tuple coordinates for vector (changed to unit vector)
*
*     @return
*         double magnitude of input vector vector
*
*/
  void calcUnitVector( S3Tuple &s3VectorInOut );

/*------------------------------------------------------------------*/
// function CVectorOps::calcUnitVector
/**
*  return in-place unit vector of input vector and its magnitude
*
*     @param[in,out] daVectorInOut
*         double array coordinates for vector (changed to unit vector)
*
*     @return
*         double magnitude of input vector vector
*
*/
 void calcUnitVector( double daVectorInOut[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::calcUnitVector
/**
*  return unit vector of input vector and its magnitude
*
*     @param daVector
*         double array coordinates for vector (unchanged)
*
*     @param[out] daUnitVector
*         double array coordinates for resultant unit vector
*     @return
*         double magnitude of input vector vector
*
*/
  const double getVectorMagnitude( const double daVector[3] );

  //  set vector component values (to all same, or individual values)
/*------------------------------------------------------------------*/
// function CVectorOps::setVector
/**
*  set all three vector component values to same value
*
*     @param[out] s3Vector
*         S3Tuple coordinates for vector (changed)
*     @param dValue
*         component value for all three vector coordinates
*
*/
  void setVector( S3Tuple& s3Vector,
                  const double& dValue );

/*------------------------------------------------------------------*/
// function CVectorOps::setVector
/**
*  set the three vector component values to specified values
*
*     @param[out] s3Vector
*         S3Tuple coordinates for vector (changed)
*     @param dValue1, dValue2, dValue3
*         double component value for vector X, Y and Z coordinates
*
*/
  void setVector( S3Tuple& s3Vector, 
                  const double& dValue1,
                  const double& dValue2,
                  const double& dValue3 );

/*------------------------------------------------------------------*/
// function CVectorOps::scaleVector
/**
*  scale vector components by specified value, returning *modified* vector
*
*     @param[in,out] s3Vector
*         S3Tuple coordinates for vector (changed)
*     @param dValue
*         double scaling value to be applied to all vector coordinates
*
*/
  void scaleVector( S3Tuple& s3Vector,
                    const double& dValue );

/*------------------------------------------------------------------*/
// function CVectorOps::scaleVector
/**
*  scale vector components by specified value, returning *modified* vector
*
*     @param[in,out] daVector
*         double array coordinates for vector (changed)
*     @param dValue
*         double scaling value to be applied to all vector coordinates
*
*/
  void scaleVector( double daVector[3],
                    const double& dValue );

/*------------------------------------------------------------------*/
// function CVectorOps::scaleVector
/**
*  scale vector components by specified value, returning new vector
*
*     @param s3Vector
*         S3Tuple coordinates for vector (unchanged)
*     @param dValue
*         double scaling value to be applied to all vector coordinates
*
*     @param[out] s3ScaledVector
*         S3Tuple coordinates for resultant scaled vector
*
*/
  void scaleVector( const S3Tuple& s3Vector,
                    const double& dValue, 
                    S3Tuple& s3ScaledVector );

/*------------------------------------------------------------------*/
// function CVectorOps::scaleVector
/**
*  scale vector components by specified value, returning new vector
*
*     @param daVector
*         double array coordinates for vector (unchanged)
*     @param dValue
*         double scaling value to be applied to all vector coordinates
*
*     @param[out] daScaledVector
*         double array coordinates for resultant scaled vector
*
*/
  void scaleVector( const double daVector[3],
                    const double& dValue,
                    double daScaledVector[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::deltaVector
/**
*  adjust vector components by individual values, returning modified vector
*
*     @param[in,out] s3Vector
*         S3Tuple coordinates for vector (changed)
*     @param dValue1, dValue2, dValue3
*         double component values added to vector X, Y and Z coordinates, respectively
*
*/
  void deltaVector( S3Tuple& s3Vector,
                    const double& dValue1,
                    const double& dValue2,
                    const double& dValue3 ); 

/*------------------------------------------------------------------*/
// function CVectorOps::deltaVector
/**
*  adjust vector components by individual values, returning new vector
*
*     @param s3Vectounr
*         S3Tuple coordinates for vector (changed)
*     @param dValue1, dValue2, dValue3
*         double component values added to vector X, Y and Z coordinates, respectively
*     @param[out] s3NewVector
*         S3Tuple coordinates for resultant adjusted vector
*
*/
  void deltaVector( const S3Tuple& s3Vector, 
                    const double& dValue1,
                    const double& dValue2,
                    const double& dValue3, 
                    S3Tuple& s3NewVector );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorDiff
/**
*  calculate difference of two vectors (V1-V2), returning new vector
*
*     @param s3Vector1
*         S3Tuple coordinates for first vector
*     @param s3Vector2
*         S3Tuple coordinates for second vector
*
*     @param[out] s3VectorDiff
*         S3Tuple coordinates for resultant difference vector
*
*/
  void calcVectorDiff( const S3Tuple& s3Vector1,
                       const S3Tuple& s3Vector2,
                       S3Tuple& s3VectorDiff );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorDiff
/**
*  calculate difference of two vectors (V1-V2), returning new vector
*
*     @param daVector1
*         double array coordinates for first vector
*     @param daVector2
*         double array coordinates for second vector
*
*     @param[out] daVectorDiff
*         double array coordinates for resultant difference vector
*
*/
  void calcVectorDiff( const double daVector1[3],
                       const double daVector2[3],
                       double daVectorDiff[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorSum
/**
*  calculate sum of two vectors (V1+V2), returning new vector
*
*     @param s3Vector1
*         S3Tuple coordinates for first vector
*     @param s3Vector2
*         S3Tuple coordinates for second vector
*
*     @param[out] s3VectorSum
*         S3Tuple coordinates for resultant summed vector
*
*/
  void calcVectorSum( const S3Tuple& s3Vector1,
                      const S3Tuple& s3Vector2,
                      S3Tuple& s3VectorSum );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorSum
/**
*  calculate sum of two vectors (V1+V2), returning new vector
*
*     @param daVector1
*         double array coordinates for first vector
*     @param daVector2
*         double array coordinates for second vector
*
*     @param[out] daVectorSum
*         double array of coordinates for resultant summed vector
*
*/
  void calcVectorSum( const double daVector1[3],
                      const double daVector2[3],
                      double daVectorSum[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorSum
/**
*  calculate sum of two vectors (V1+V2), returned in V1 vector
*
*     @param[in,out] s3Vector1
*         S3Tuple coordinates for first vector (changed)
*     @param s3Vector2
*         S3Tuple coordinates for second vector
*
*/
  void calcVectorSum( S3Tuple& s3Vector1,
                      const S3Tuple& s3Vector2 );

/*------------------------------------------------------------------*/
// function CVectorOps::calcVectorRange
/**
*  calculate the range between the two positions, represented by vector values
*
*     @param s3Vector1
*         S3Tuple coordinates for first vector
*     @param s3Vector2
*         S3Tuple coordinates for second vector
*
*     @return
*         double range between the two vector positions
*
*/
  double calcVectorRange( const S3Tuple& s3Vector1,
                          const S3Tuple& s3Vector2 );

/*------------------------------------------------------------------*/
// function CVectorOps::multMatrixMatrix
/**
*  multiply contents of two 3x3 matrices, returning new matrix
*
*     @param da2MatrixA
*         double 3x3 array of first matrix
*     @param da2MatrixB
*         double 3x3 array of second matrix
*
*     @param[out] da2MatrixC
*         double 3x3 array result from matrix multiplication
*
*/
  void multMatrixMatrix( const double da2MatrixA[3][3],
                         const double da2MatrixB[3][3],
                         double daMatrixC[3][3] );

/*------------------------------------------------------------------*/
// function CVectorOps::multVectorMatrix
/**
*  multiply matrix and vector, returning new vector
*
*     @param daVectorIn
*         double array coordinates for vector
*     @param da2Matrix
*         double 3x3 array of matrix
*
*     @param[out] daVectorOut
*         double array result from vector & matrix multiplication
*
*/
  void multVectorMatrix( const double da2VectorIn[3], 
                         const double da2Matrix[3][3],
                         double daVectorOut[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::multVectorInvMatrix
/**
*  multiply *inverse* matrix and vector, returning new vector
*
*     @param daVectorIn
*         double array coordinates for vector
*     @param da2Matrix
*         double 3x3 array of matrix (transposed on the fly)
*
*     @param[out] daVectorOut
*         double array result from vector & inverse matrix multiplication
*
*/
  void multVectorInvMatrix( const double da2VectorIn[3], 
                            const double da2Matrix[3][3],
                            double daVectorOut[3] );

/*------------------------------------------------------------------*/
// function CVectorOps::multVectorMatrix
/**
*  multiply matrix and vector, returning new vector
*
*     @param s3VectorIn
*         S3Tuple coordinates for vector
*     @param da2Matrix
*         double 3x3 array of matrix
*
*     @param[out] daVectorOut
*         S3Tuple result from vector & matrix multiplication
*
*/
  void multVectorMatrix( const S3Tuple &s3VectorIn, 
                         const double da2Matrix[3][3],
                         S3Tuple &s3VectorOut );

/*------------------------------------------------------------------*/
// function CVectorOps::multVectorInvMatrix
/**
*  multiply *inverse* matrix and vector, returning new vector
*
*     @param s3VectorIn
*         S3Tuple coordinates for vector
*     @param da2Matrix
*         double 3x3 array of matrix (transposed on the fly)
*
*     @param[out] daVectorOut
*         S3Tuple result from vector & inverse matrix multiplication
*
*/
  void multVectorInvMatrix( const S3Tuple &s3VectorIn, 
                            const double da2Matrix[3][3],
                            S3Tuple &s3VectorOut );
/*------------------------------------------------------------------*/
// function CVectorOps::invertMatrix
/**
*  invert (transpose) matrix (in-place)
*
*     @param[in,out] da2Matrix
*         double 3x3 array of matrix
*
*/
  void invertMatrix( double da2Matrix[3][3] );

/*------------------------------------------------------------------*/
// function CVectorOps::invertMatrix
/**
*  invert (transpose) matrix, returning new matrix
*
*     @param da2Matrix
*         double 3x3 array of matrix
*
*     @param[out] da2InvMatrix
*         double 3x3 array of matrix, transpose of input matrix
*
*/
  void invertMatrix( const double da2Matrix[3][3],
                     double da2InvMatrix[3][3] );

/*------------------------------------------------------------------*/
// function CVectorOps::getRadToDeg
/**
*  return 'radians to degrees' angle conversion constant
*
*     @return
*         double 'radians to degrees' angle conversion constant
*
*/
  double getRadToDeg() const { return cdRadToDeg; }

/*------------------------------------------------------------------*/
// function CVectorOps::getDegToRad
/**
*  return 'degrees to radians' angle conversion constant
*
*     @return
*         double 'degrees to radians' angle conversion constant
*
*/
  double getDegToRad() const { return cdDegToRad; }

protected:

  // Internal data members
  double cdRadToDeg;
  double cdDegToRad;

};

#endif


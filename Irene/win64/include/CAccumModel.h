/******************************************************************************
$HeadURL$

 File: CAccumModel.h

 Description: Declarations for methods for the collection and management
   of flux data values to be accumulated for the calculation of fluence
   data values.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2021 Atmospheric and Environmental Research, Inc. (AER)

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

#ifndef ACCUMMODEL_H
#define ACCUMMODEL_H

#include "VectorTypes.h"
#include "CircBuffer.h"
#include "CAccumulator.h"

class AccumModel
{
  public:
    AccumModel();
    virtual ~AccumModel();

    int setTimeInterval ( const double& dTimeInterval, 
                          bool bUnitsDays=true,
                          bool bClearList=true );
    int setTimeIntervalSec ( const double& dTimeInterval,
                             bool bClearList=true ) 
                    { return setTimeInterval( dTimeInterval, false, bClearList ); }
    double getTimeInterval ( const int& iIndex=0 );
    int getNumTimeInterval ( ) { return int(m_vdTimeInterval.size()); }
    int setTimeIncrement ( const double& dTimeIncrement );
    double getTimeIncrement () { return m_dTimeIncrement; }

    void getBufferDim( ivector& viBufferDim );
    void clearBuffer( );

    int loadBuffer ( const dvector& vdTimes, 
                     const vvdvector& vvvdData );
    int addToBuffer ( const double dTime, 
                      const vdvector& vvdData );

    void setWcLogging( bool bState=true ) { m_bWcLogging = bState; }

    int computeFluence ( dvector& vdFluenceTimes, 
                         vvdvector& vvvdFluence );
    int computeIntvFluence ( dvector& vdIntvFluenceTimes, 
                             vvdvector& vvvdIntvFluence, 
                             ivector& viIntIndices, 
                             bool bReturnPartial=false );
    int computeIntvFluence ( dvector& vdIntvFluenceTimes, 
                             vvdvector& vvvdIntvFluence, 
                             const int& iIndex,
                             ivector& viIntIndices, 
                             bool bReturnPartial=false );
    int computeFullFluence ( dvector& vdFullFluenceTimes, 
                             vvdvector& vvvdFullFluence, 
                             bool bReturnFinal=false );

    void resetFluence () { m_accumCumul.resetData(); }
    int resetIntvFluence ( const int& iIndex=0 );
    void resetFullFluence () { m_accumFull.resetData(); }
    int resetBoxcarFluence ( const int& iIndex=0 ); 
    int resetExponentialFlux ( const int& iIndex=0 ); 

    double getFluenceStartTime () { return m_accumCumul.getIntervalStart(); }
    double getIntvFluenceStartTime( const int& iIndex=0 );
    double getFullFluenceStartTime () { return m_accumFull.getIntervalStart(); }
    double getBoxcarFluenceStartTime( const int& iIndex=0 );

    double getLastLength( const int& iIndex=0 ); //  Intv accum mode only
    double getBoxcarLastLength( const int& iIndex=0 );

    int accumIntvFluence ( const dvector& vdIntvFluenceTimes, 
                           const vvdvector& vvvdIntvFluence, 
                           vvdvector& vvvdIntvFluenceAccum,
                           bool bAccumReset = false );
    int accumIntvFluence ( const dvector& vdIntvFluenceTimes, 
                           const vvdvector& vvvdIntvFluence, 
                           const int& iIndex,
                           vvdvector& vvvdIntvFluenceAccum,
                           bool bAccumReset = false );
    int computeAverageFlux ( const dvector& vdFluenceTimes, 
                             const vvdvector& vvvdFluence, 
                             const double& dIntervalSec,
                             vvdvector& vvvdFluxAvg );
    int computeBoxcarFluence ( dvector& vdBoxcarFluenceTimes, 
                               vvdvector& vvvdBoxcarFluence, 
                               ivector& viIntIndices, 
                               bool bReturnPartial=false );
    int computeBoxcarFluence ( dvector& vdBoxcarFluenceTimes, 
                               vvdvector& vvvdBoxcarFluence, 
                               const int& iIndex,
                               ivector& viIntIndices, 
                               bool bReturnPartial=false );
    int computeBoxcarAutoFluence ( dvector& vdBoxcarFluenceTimes, 
                                   vvdvector& vvvdBoxcarFluence, 
                                   const int& iIndex,
                                   ivector& viIntIndices, 
                                   bool bReturnPartial=false );
    int computeExponentialFlux ( dvector& vdExpFluxTimes, 
                                 vvdvector& vvvdExpFlux, 
                                 ivector& viIntIndices, 
                                 bool bReturnFinal );
    int computeExponentialFlux ( dvector& vdExpFluxTimes, 
                                 vvdvector& vvvdExpFlux, 
                                 const int& iIndex,
                                 ivector& viIntIndices, 
                                 bool bReturnFinal );
    int applyWorstToDate ( const vvdvector &vvvdData, 
                           vdvector &vvdMaxData, 
                           vvdvector &vvvdDataWorst );
    int getWorstCaseLog( const dvector &vdTimes,
                         const dvector &vdEnergies,
                         const vdvector &vvdPitchAngles,
                         vdvector &vvdWorstCaseLog );

  private:

    void clearIntervalParam( );
    void initIntervalParam( const double& dIntervalSec );

    // time and flux data loaded from input file
    dvector m_vdTime;
    vvdvector m_vvvdData;

    // each accumulation mode has separate accumulator(s)
    Accumulator m_accumCumul;
    vector<Accumulator> m_vaccumIntv;
    Accumulator m_accumFull;
    vector<CircBuffer<Accumulator *> > m_vcbpAccumBoxcar; // need to use pointers for proper manipulation
    vector<Accumulator> m_vaccumExp;

    dvector m_vdTimeInterval;  // vector of time interval for all accumulations
    double m_dTimeIncrement;  // time increment for boxcar accumulations only

    dvector m_vdIntvLastLength;
    dvector m_vdBoxcarLastLength;
    double m_dFullLastLength;
    dvector m_vdLastIntvTime;
    dvector m_vdBoxcarStartTime;

    vvdvector m_vvvdIntvFluenceAccum;

    dvector m_vdWorstValueLog;
    vivector m_vviWorstIndicesLog;

    bool        m_bWcLogging;   // activate optional Worst Case logging feature

};

#endif

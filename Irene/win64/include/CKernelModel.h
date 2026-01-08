/******************************************************************************
$HeadURL$

 File: CKernelModel.h

 Description: Declaration of methods to provide a common interface for
   loading and evaluating kernel-based calculations.

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

#ifndef KERNELMODEL_H
#define KERNELMODEL_H

#include <vector>
#include <string>

typedef std::vector<double> dvector;
typedef std::vector<dvector> vdvector;

class KernelModel {

  public:
    KernelModel();
    ~KernelModel();

    int load( std::string strXmlFile );
    void clear( );

//## should all these 'get' methods first verify m_bXmlLoaded ??? ###

    std::string getFileName( ) { return m_strXmlFile; }
    std::string getTag( ) { return m_strTag; }
    std::string getDescription( ) { return m_strDescription; }
    std::string getVersion( ) { return m_strVersion; }
    std::string getDevelInfo( ) { return m_strDevelInfo; }
    std::vector<std::string> getUses( ) { return m_vstrUses; }

    dvector getEnergyGridMeV( ) { return m_vdEnergies; }
    int getNumEnergies( ) { return int(m_vdEnergies.size()); }
    std::string fullHdf5File( const std::string &strXmlFile, 
                              const std::string &strFileName );
    int loadHdf5Data( const std::string &strHdf5File, 
                      const std::string &strVarName, 
                      dvector &vdData );
    int loadHdf5Data( const std::string &strHdf5File, 
                      const std::string &strVarName, 
                      vdvector &vvdData );
    void transposeMatrix( vdvector &vvdData );
    std::string getTransformType( ) { return m_strTransformType; }
    vdvector getTransform( ) { return m_vvdTransform; }

    dvector getOutputGrid( ) { return m_vdOutputs; }
    int getNumOutputs( ) { return int(m_vdOutputs.size()); }
    std::string getOutputDepthUnits( ) { return m_strOutputDepthUnits; }
    std::string getOutputUnits( ) { return m_strOutputUnits; }
    std::string getOutputShielding( ) { return m_strOutputShielding; }

    std::string getOutputInterp( ) { return m_strOutputInterp; }
    bool   isOutputInterpLog( ) { return m_strOutputInterp.compare("Log")==0; }
    bool   isOutputInterpLinear( ) { return m_strOutputInterp.compare("Linear")==0; }
    bool   isOutputInterpNone( ) { return m_strOutputInterp.compare("None")==0; }
    bool   isApplyDeltaE( ) { return m_bApplyDeltaE; }
    std::string getSpecies( ) { return m_strSpecies; }
    int interpLogLin( const dvector &dvXin,
                      const dvector &dvYin,
                      const dvector &dvXgrid,
                      dvector &dvYgrid,
                      bool bLinOnly = false );
    int interpLinear( const dvector &dvXin,
                      const dvector &dvYin,
                      const dvector &dvXgrid,
                      dvector &dvYgrid )
      { return interpLogLin( dvXin, dvYin, dvXgrid, dvYgrid, true ); }
    int evaluate( const dvector &dvXin,
                  const dvector &dvYin,
                  const dvector &dvZgrid,
                  dvector &dvZvalues );

  protected:
    int interpSegmentLin( const double &dXBegin,
                          const double &dYBegin,
                          const double &dXEnd,
                          const double &dYEnd,
                          const dvector &dvXPoints,
                          dvector &dvYPoints,
                          bool bLogScale = false );
    int matrixDotVector( const vdvector &vvdMatrix,
                         const dvector &vdVector,
                         dvector &vdResult );
    std::string m_strXmlFile;
    std::string m_strTag;
    std::string m_strDescription;
    std::string m_strVersion;
    std::string m_strDevelInfo;
    std::vector<std::string> m_vstrUses;
    std::string m_strSpecies;

    dvector m_vdEnergies;

    std::string m_strTransformType;
    vdvector m_vvdTransform;

    dvector m_vdOutputs;
    std::string m_strOutputUnits;
    std::string m_strOutputDepthUnits;
    std::string m_strOutputShielding;

    std::string m_strOutputInterp;
    bool m_bApplyDeltaE;

    bool m_bXmlLoaded;
};

#endif

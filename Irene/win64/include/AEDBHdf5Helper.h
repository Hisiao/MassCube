/******************************************************************************
$HeadURL$

File: AEDBHdf5Helper.h

Description: This is the class declaration for the AEDBHdf5Helper class, which
    is internal to the Ae9Ap9 database layer. It provides a suite of templated
    methods for retrieving data of various datatypes and multidimensional sizes
    from an hdf5 format database.

 Classification:

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

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

  Version      Date       Notes
  1.0          10/01/2009 created 
  
SVNTag: $Id$
******************************************************************************/
#ifndef AEDBHDF5HELPER_H
#define AEDBHDF5HELPER_H

#include <string>
using namespace std;

#include <hdf5.h>

#if (H5_VERS_MAJOR <=1)
#if (H5_VERS_MINOR >=8)
#if !defined(H5_USE_16_API)
#define HV18P 1
#endif
#endif
#else
#define HV18P 1
#endif

#include "AETypes.h"

namespace ae9ap9 
{
    class AEDBHdf5Helper
    {
    public:
        AEDBHdf5Helper();
        ~AEDBHdf5Helper();

        // data retrieval methods

        int getData( hid_t parent, const string& strName, string& strData );

        template<class  T> 
        int getData( hid_t parent, const string& strName, vector<T>& vData, uivector& vuiDimSizes );

        // partial data retrieval methods (for REALLY big datasets)

        int getDataDimensions( hid_t parent, const string& strName, uivector& vuiDimSizes );

        template<class  T> 
        int getPartialData( hid_t parent, const string& strName, 
                            const uivector& vuiStartIndices, const uivector& vuiCounts,
                            vector<T>& vData );

        // Some data stored via matlab in cell array (represented by object references in hd5)
        // can be either a collection of data or groups

        template<class T>
        int getDataCollection( hid_t parent, const string& strName, vector< vector<T> >& vvData, vector<uivector>& vvuiDimSizes );

        int getGroupCollection( hid_t parent, const string& strName, vector<string>& vstrGroups );

        // control HDF5 display of warning and error messages

        void suppressErrorPrinting();
        void enableErrorPrinting();

    protected:

        // internal data retrieval methods

        int getDataDimensions( hid_t dataset, int& iRank, int** ppiDimSizes );

        template<class  T> 
        int getData( hid_t dataset, T** ppData, int& iRank, int** ppiDimSizes );

        template<class  T> 
        int getPartialData( hid_t dataset, const uivector& vuiStartIndices, 
                            const uivector& vuiCounts, T** ppData );

        template <class T>
        int ra2vector( T* pData, int iDims, int* pDims, vector<T>& vData, uivector& vDims );

        template <class T>
        int vector2ra( vector<T>& vData, uivector& vDims, T** ppData, int& iDims, int** ppiDims );


        template <class TYPE> 
        int isfloat(TYPE test);

#ifdef HV18P
        H5E_auto2_t m_oldFunc;
#else
	    herr_t (*m_oldFunc)(void*);
#endif
	    void* m_oldClientData;
    };
}

#endif

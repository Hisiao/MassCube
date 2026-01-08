/******************************************************************************
File: Hdf5ToolKit.h

Description: This is the class declaration for the Hdf5ToolKit class. It provides a 
    suite of templated methods for retrieving data of various datatypes and multi-
    dimensional sizes from an HDF5 format database.

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
  1.0          10/01/2009 created 
  1.1          03/22/2018 expanded to mimic obsolete 'fio' class methods
  
******************************************************************************/
#ifndef HDF5TOOLKIT_H
#define HDF5TOOLKIT_H

#include <string>
#include <vector>
using namespace std;

#include <hdf5.h>

#ifdef H5_USE_16_API
#define HV16P
#else
#if (H5_VERS_MAJOR <=1)
#if (H5_VERS_MINOR <8)
#define H5_USE_16_API
#define HV16P
#endif
#endif
#endif

using namespace std;

// One dimensional vectors
typedef vector<char>           cvector;
typedef vector<unsigned char>  ucvector;
typedef vector<unsigned short> usvector;
typedef vector<unsigned int>   uivector;
typedef vector<int>            ivector;
typedef vector<float>          fvector;
typedef vector<double>         dvector;

// Two dimensional vectors
typedef vector<uivector>       vuivector;
typedef vector<ivector>        vivector;
typedef vector<fvector>        vfvector;
typedef vector<dvector>        vdvector;

// Three dimensional vectors
typedef vector<vdvector>       vvdvector;

// integer Error codes

const int H5TK_ERR_NOERROR = 0;
const int H5TK_ERR_NOT_FOUND = -1;
const int H5TK_ERR_MEMORY = -2;
const int H5TK_ERR_RANGE_ERROR = -3;
const int H5TK_ERR_MATH_ERROR = -4;
const int H5TK_ERR_LOADFAIL = -5;
const int H5TK_ERR_SAVEFAIL = -6;
const int H5TK_ERR_INVALIDARG = -7;
const int H5TK_ERR_NOTIMPLEMENTED = -8;
const int H5TK_ERR_UNINITIALIZED = -9;
const int H5TK_ERR_DUPLICATE_NAME = -10;
const int H5TK_ERR_UNSUPPORTED_COMPUTATION = -11;

class Hdf5ToolKit {
  public:
      Hdf5ToolKit();
      ~Hdf5ToolKit();

      // alternate constructor, for using internally-maintained file handle
      // (each public method has overload for using this handle)
      Hdf5ToolKit( const string& strName );
      hid_t getFileHandle( ) { return m_hFile; }

      hid_t openFile( const string& strName );

      void closeFile( hid_t fileid );
      void closeFile ( ) {
        if ( m_hFile > 0 ) closeFile( m_hFile );
      }

      // data retrieval methods

      int getData( hid_t parent, const string& strName, string& strData );
      int getData( const string& strName, string& strData ) {
        if ( m_hFile <= 0 ) return -1;
        return getData( m_hFile, strName, strData );
      }

      template <class T> 
      int getData( hid_t parent, const string& strName, vector<T>& vData, uivector& vuiDimSizes );
      template <class T> 
      int getData( const string& strName, vector<T>& vData, uivector& vuiDimSizes ) {
        if ( m_hFile <= 0 ) return -1;
        return getData( m_hFile, strName, vData, vuiDimSizes );
      }
      template <class T> 
      int getData( const string& strName, T& tData ) {
        if ( m_hFile <= 0 ) return -1;
        vector<T> vTData;
        uivector vuiDimSizes;
        int ierr = getData( m_hFile, strName, vTData, vuiDimSizes );
        if ( ierr != 0 ) return ierr;
        tData = vTData[0];
        return 0;
      }

      // quick routines that couch the name in the '/data/<name>/DATA' string
      int getSize(const string& strName);

      // use with 'string' type data entries
      int getString( const string& strName, string& strData );
      // use with '8-bit unsigned integer' arrays that represent a string of characters
      int get( const string& strName, string& strData );

      template <class T> 
      int get( const string& sName, T &tValue );
      template <class T> 
      int get( const string& sName, vector<T> &vtArray );
      template <class T> 
      int get( const string& sName, vector<vector <T> > &vvtArray );
      void clean() { }

      // partial data retrieval methods (for REALLY big datasets)

      int getDataDimensions( hid_t parent, const string& strName, uivector& vuiDimSizes );
      int getDataDimensions( const string& strName, uivector& vuiDimSizes ) {
        if ( m_hFile <= 0 ) return -1;
        return getDataDimensions( m_hFile, strName, vuiDimSizes );
      }

      template<class  T> 
      int getPartialData( hid_t parent, const string& strName, 
                          const uivector& vuiStartIndices, const uivector& vuiCounts,
                          vector<T>& vData );
      template<class  T> 
      int getPartialData( const string& strName, 
                          const uivector& vuiStartIndices, const uivector& vuiCounts,
                          vector<T>& vData ) {
        if ( m_hFile <= 0 ) return -1;
        return getPartialData( m_hFile, strName, vuiStartIndices, vuiCounts, vData );
      }

      // Some data stored via matlab in cell array (represented by object references in hd5)
      // can be either a collection of data or groups

      template<class T>
      int getDataCollection( hid_t parent, const string& strName, vector< vector<T> >& vvData, vector<uivector>& vvuiDimSizes );
      template<class  T> 
      int getDataCollection( const string& strName, vector< vector<T> >& vvData, vector<uivector>& vvuiDimSizes ) {
        if ( m_hFile <= 0 ) return -1;
        return getDataCollection( m_hFile, strName, vvData, vvuiDimSizes );
      }

      int getGroupCollection( hid_t parent, const string& strName, vector<string>& vstrGroups );
      int getGroupCollection( const string& strName, vector<string>& vstrGroups ) {
        if ( m_hFile <= 0 ) return -1;
        return getGroupCollection( m_hFile, strName, vstrGroups );
      }

      // control HDF5 display of warning and error messages

      void suppressErrorPrinting();
      void enableErrorPrinting();
      
      // data writing methods to be added eventually...

  protected:

      // file handle for internal-only usage; set via alternate constructor
      hid_t m_hFile;

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

#ifdef HV16P
	    herr_t (*m_oldFunc)(void*);
#else
      H5E_auto2_t m_oldFunc;
#endif
	    void* m_oldClientData;
};

#endif

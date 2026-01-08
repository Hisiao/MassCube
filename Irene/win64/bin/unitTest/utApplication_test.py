
# enable use of python3 print function form in python 2.x
from __future__ import print_function

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Irene root directory here:
#sys.path.append(os.environ['HOME']+'/Irene')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

from application import Application
from ephemModel import EphemModel
import numpy as np
import math
import os
import shutil


class TestApplication:

    def setup_class( self ):
        self.app = Application()
        print ('app zHandle=', self.app.zHandle)

        # remove 'work' directory, if it exists
        if os.path.isdir('work'):
            shutil.rmtree('work')
        self.bWorkDir = False
        self.bWorkDir_pm_agg = False

    def test_application_vars( self ):
        # test set_execDir
        for strString in ["/temp", "../", "./temp"]:
            ierr = self.app.set_execDir( strString )
            print (' set_execDir: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            strRet = self.app.get_execDir( )
            print (' get_execDir: => ', strRet)
            assert(strRet == "undef")

        for strString in ["", "."]:
            ierr = self.app.set_execDir( strString )
            print (' set_execDir: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        assert(strRet == './')

        # test set_workDir
        for strString in ["", ".", "../", "./temp", "temp", "temp/"]:
            ierr = self.app.set_workDir( strString )
            print (' set_workDir: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

        strRet = self.app.get_workDir( )
        print (' get_workDir: => ', strRet)
        assert(strRet == 'temp/')

        # test set_binDirName
        for strString in ["../bin", "/temp"]:
            ierr = self.app.set_binDirName( strString )
            print (' set_binDirName: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            strRet = self.app.get_binDirName( )
            print (' get_binDirName: => ', strRet)
            assert(strRet == "")

        for strString in ["", "../", "."]:
            ierr = self.app.set_binDirName( strString )
            print (' set_binDirName: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            strRet = self.app.get_binDirName( )
            print (' get_binDirName: => ', strRet)
            assert(strRet == strString)

        # test set_delBinDir
        ierr = self.app.set_delBinDir( 0 )
        print (' set_delBinDir( 0 ): => ', ierr)
        assert(ierr == 0)

        iRetVal = self.app.get_delBinDir( )
        print (' get_delBinDir: => ', iRetVal)
        assert(iRetVal == 0)

        ierr = self.app.set_delBinDir( 1 )
        print (' set_delBinDir( 1 ): => ', ierr)
        assert(ierr == 0)

        iRetVal = self.app.get_delBinDir( )
        print (' get_delBinDir: => ', iRetVal)
        assert(iRetVal == 1)
        self.bWorkDir = False

        # test set_numProc
        for iTestVal in [0, 2]:
            ierr = self.app.set_numProc( iTestVal )
            print (' set_numProc: iTestVal=', iTestVal, ' => ', ierr)
            assert(ierr < 0)

            retVal = self.app.get_numProc()
            print (' get_numProc: => ', retVal)
            assert(retVal == 1)

        for iTestVal in [1, 3, -4, 5, -8]:
            ierr = self.app.set_numProc( iTestVal )
            print (' set_numProc: iTestVal=', iTestVal, ' => ', ierr)
            assert(ierr == 0)

            # value has been set
            retVal = self.app.get_numProc( )
            print (' get_numProc: => ', retVal)
            assert(retVal == iTestVal)

        # test set_numFileIo
        for iTestVal in [0]:
            ierr = self.app.set_numFileIo( iTestVal )
            print (' set_numFileIo: iTestVal=', iTestVal, ' => ', ierr)
            assert(ierr < 0)

            retVal = self.app.get_numFileIo( )
            print (' get_numFileIo: => ', retVal)
            assert(retVal == 1)

        for iTestVal in [-11, 2, -4, 5, -8]:
            ierr = self.app.set_numFileIo( iTestVal )
            print (' set_numFileIo: iTestVal=', iTestVal, ' => ', ierr)
            assert(ierr == 0)

            # value has been set
            retVal = self.app.get_numFileIo( )
            print (' get_numFileIo: => ', retVal)
            assert(retVal == abs( iTestVal ))

        # test set_chunkSize
        for iTestVal in [0, -10]:
            ierr = self.app.set_chunkSize( iTestVal )
            print (' set_chunkSize: iTestVal=', iTestVal, ' => ', ierr)
            assert(ierr < 0)

            retVal = self.app.get_chunkSize( )
            print (' get_chunkSize: => ', retVal)
            assert(retVal == 960)

        for iTestVal in [60, 240, 960, 120]:
            ierr = self.app.set_chunkSize( iTestVal )
            print (' set_chunkSize: iTestVal=', iTestVal, ' => ', ierr)
            assert(ierr == 0)

            # value has been set
            retVal = self.app.get_chunkSize( )
            print (' get_chunkSize: => ', retVal)
            assert(retVal == iTestVal)

    def test_external_ephemeris_vars( self ):
        # test set_coordSys
        testSystems = ["GDZ", "GEO", "GSM", "GSE", "SM", "GEI", "ECI", "MAG",
                       "SPH", "RLL"]  # initial list of accepted values
        badSystems = ["", "ABC", "XYZ", "MLL"]  # list of bad values to test
        testSystems += badSystems  # append bad values

        testUnits = ["KM", "RE"]
        badUnits = ["", "MI", "FT", "CM"]
        testUnits += badUnits  # same as systems

        for strSys in testSystems:
            for strUnits in testUnits:
                ierr = self.app.set_coordSys( strSys, strUnits )
                print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)

                strRetSys = self.app.get_coordSys( )
                print(' get_coordSys: => ', strRetSys)
                strRetSysUnits = self.app.get_coordSysUnits( )
                print(' get_coordSysUnits: => ', strRetSysUnits)

                if strSys == "GDZ":
                    assert(ierr == 0)
                    assert(strRetSys == "GDZ")
                    assert(strRetSysUnits == "KM")

                elif strSys in badSystems or strUnits in badUnits:
                    assert(ierr < 0)

                else:
                    assert(ierr == 0)

                    # ECI and GEI are the same
                    if strSys == "ECI":
                        assert(strRetSys == "GEI")
                    else:
                        assert(strRetSys == strSys)

#                     # unit defaults to RE if not specified
#                     elif strUnits == "":
#                         assert(strRetSysUnits == 'RE')
#                     else:
                    assert(strRetSysUnits == strUnits)

        # test set_inCoordSys
        # uses the same inputs as above
        for strSys in testSystems:
            for strUnits in testUnits:
                ierr = self.app.set_inCoordSys( strSys, strUnits )
                print (' set_inCoordSys: input={', strSys, ',', strUnits, '} => ', ierr)

                strRetSys = self.app.get_inCoordSys( )
                print(' get_inCoordSys: => ', strRetSys)
                strRetSysUnits = self.app.get_inCoordSysUnits( )
                print(' get_inCoordSysUnits: => ', strRetSysUnits)

                if strSys == "GDZ":
                    assert(ierr == 0)
                    assert(strRetSys == "GDZ")
                    assert(strRetSysUnits == "KM")

                elif strSys in badSystems or strUnits in badUnits:
                    assert(ierr < 0)

                else:
                    assert(ierr == 0)

                    # ECI and GEI are the same
                    if strSys == "ECI":
                        assert(strRetSys == "GEI")
                    else:
                        assert(strRetSys == strSys)

#                     # unit defaults to RE if not specified
#                     elif strUnits == "":
#                         assert(strRetSysUnits == 'RE')
#                     else:
                    assert(strRetSysUnits == strUnits)

        # test set_inEphemeris
        daMT = np.zeros( 1 )
        iNum = len(daMT)
        ierr = self.app.set_inEphemeris( daMT, daMT, daMT, daMT )
        print (' set_inEphemeris: ', ' Energies(#)=', len(daMT),
               ' daTimes(#)=', len(daMT), ', daCoord1(#)=', len(daMT))
        assert(ierr == 0)

        iRetVal = self.app.get_numInEphemeris( )
        print(' get_numInEphemeris: =>', iRetVal)
        assert(iRetVal == iNum)

        iRetVal, daTimes, daCoords1, daCoords2, daCoords3 = self.app.get_inEphemeris( )
        print(' get_inEphemeris: => iNum: ', iRetVal, ', daTimes.shape: ', daTimes.shape,
              ', daCoords1.shape:', daCoords1.shape, ', daCoords2.shape:', daCoords2.shape,
              ', daCoords3.shape:', daCoords3.shape)
        assert(iRetVal == iNum)
        assert(np.array_equal(daTimes, daMT))
        assert(np.array_equal(daCoords1, daMT))
        assert(np.array_equal(daCoords2, daMT))
        assert(np.array_equal(daCoords3, daMT))

        daMT3 = np.zeros(3)
        iNum = len(daMT3)
        ierr = self.app.set_inEphemeris( daMT3, daMT3, daMT3, daMT3 )
        print (' set_inEphemeris: ', ' Energies(#)=', len(daMT3),
               ' daTimes(#)=', len(daMT3), ', daCoord1(#)=', len(daMT3))
        assert(ierr == 0)

        iRetVal = self.app.get_numInEphemeris( )
        print(' get_numInEphemeris: =>', iRetVal)
        assert(iRetVal == iNum)

        ierr = self.app.set_inEphemeris( daMT, daMT3, daMT3, daMT3 )
        print (' set_inEphemeris: ', ' Energies(#)=', len(daMT),
               ' daTimes(#)=', len(daMT3), ', daCoord1(#)=', len(daMT3))
        assert(ierr < 0)

        iRetVal = self.app.get_numInEphemeris( )
        print(' get_numInEphemeris: =>', iRetVal)
        assert(iRetVal == iNum)

        iRetVal, daTimes, daCoords1, daCoords2, daCoords3 = self.app.get_inEphemeris( )
        print(' get_inEphemeris: => iNum: ', iRetVal, ', daTimes.shape: ', daTimes.shape,
              ', daCoords1.shape:', daCoords1.shape, ', daCoords2.shape:', daCoords2.shape,
              ', daCoords3.shape:', daCoords3.shape)
        assert(iRetVal == iNum)
        assert(np.array_equal(daTimes, daMT3))
        assert(np.array_equal(daCoords1, daMT3))
        assert(np.array_equal(daCoords2, daMT3))
        assert(np.array_equal(daCoords3, daMT3))

        ierr = self.app.set_inEphemeris( daMT, daMT, daMT, daMT, 1 )
        print (' set_inEphemeris: ', ' Energies(#)=', len(daMT),
               ', daTimes(#)=', len(daMT), ', daCoord1(#)=', len(daMT),
               ', append = 1')
        assert(ierr == 0)

        iRetVal = self.app.get_numInEphemeris( )
        print(' get_numInEphemeris: =>', iRetVal)
        assert(iRetVal == iNum + 1)

        ierr = self.app.clear_inEphemeris( )
        print(' clear_inEphemeris: =>', ierr)
        assert(ierr == 0)

        iRetVal = self.app.get_numInEphemeris( )
        print(' get_numInEphemeris: =>', iRetVal)
        assert(iRetVal == 0)

    def test_ephemeris_vars( self ):
        # test set_prop
        for strString in ["", "none", "default"]:
            ierr = self.app.set_prop( strString )
            print (' set_prop: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            strRet = self.app.get_prop()
            print (' get_prop: => ', strRet)
            assert(strRet == "")

        for strString in ["SatEph", "SGP4", "Kepler"]:
            ierr = self.app.set_prop( strString )
            print (' set_prop: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            strRet = self.app.get_prop()
            print (' get_prop: => ', strRet)
            assert(strRet == strString)

        # test set_sgp4Param
        strProp = 'sgp4'
        ierr = self.app.set_prop( strProp )
        print(' set_prop: prop=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strProp = self.app.get_prop()
        print(' get_prop: returned prop=', strProp)
        assert(strProp == 'SGP4')

        for strMode in ['standard', 'std', 'new', 'imp', 'improved']:
            for strWgs in ['72Old', '56', '72new', '84', '72']:
                ierr = self.app.set_sgp4Param( strMode, strWgs )
                print(' set_sgp4Param: mode=', strMode, ', wgs=', strWgs, ' => ', ierr)
                if strMode == 'new' or strWgs == '56' or strWgs == '72new':
                    assert(ierr < 0)
                else:
                    assert(ierr == 0)
        strStr = self.app.get_sgp4Mode( )
        print(' get_sgp4Mode: => ', strStr)
        assert(strStr == strMode)
        strStr = self.app.get_sgp4Datum()
        print(' get_sgp4Datum: => ', strStr)
        assert(strStr == strWgs)

        # test set_keplerUseJ2
        bVerdict = False
        ierr = self.app.set_keplerUseJ2( bVerdict )
        print(' set_keplerUseJ2: verdict=', bVerdict)
        assert(ierr == 0)

        bUseJ2 = self.app.get_keplerUseJ2()
        print(' get_keplerUseJ2: => ', bUseJ2)
        assert(bUseJ2 == 0)

        ierr = self.app.set_keplerUseJ2()
        print(' set_keplerUseJ2: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)

        bUseJ2 = self.app.get_keplerUseJ2()
        print(' get_keplerUseJ2: => ', bUseJ2)
        assert(bUseJ2 == 1)

        # test set_times
        dStep = 60.0
        for dStart in [31000.2, 56789.4, 51145.5, 70123.4, 58119.5]:
            for dEnd in [56779.0, 80123.4, 58120.5]:
                ierr = self.app.set_times( dStart, dEnd, dStep )
                print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStart, dEnd, dStep, ierr))
                if dStart < 33282.0 or dStart > 69807.0 or dEnd < 33282.0 or dEnd > 69807.0:
                    assert(ierr < 0)
                elif dStart >= dEnd:
                    assert(ierr < 0)
                else:
                    assert(ierr == 0)
                    dStartTime, dEndTime, dTimeStep = self.app.get_times( )
                    print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStartTime, dEndTime, dTimeStep))
                    assert(dStartTime == dStart)
                    assert(dEndTime == dEnd)
                    assert(dTimeStep == dStep)
        for dStep in [-25.0, 0.0]:
            ierr = self.app.set_times( dStart, dEnd, dStep )
            print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStart, dEnd, dStep, ierr))
            assert(ierr < 0)
        for dStep in [30.0, 3600.0, 300.0]:
            ierr = self.app.set_times( dStart, dEnd, dStep )
            print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStart, dEnd, dStep, ierr))
            assert(ierr == 0)
        for dVarMinStep in [5.0, 300.0, 30.0]:
          for dVarMaxStep in [120.0, 4800.0, 1200.0]:
            for dVarRound in [0.0, 1.5, 60.0, 5.0]:
              ierr = self.app.set_varTimes( dStart, dEnd, dVarMinStep, dVarMaxStep, dVarRound )
              print(' set_varTimes: start=%.1f, end=%.1f, vstep=%.0f,%.0f,%.1f => %i' %
                  (dStart, dEnd, dVarMinStep, dVarMaxStep, dVarRound, ierr))
              iExpect = 0
              if dVarMinStep<10.0 or dVarMinStep>=dVarMaxStep or dVarMaxStep>3600.0: iExpect = -1
              if dVarRound>=dVarMinStep or dVarRound-int(dVarRound)>0.0: iExpect = -1
              assert(ierr == iExpect)
        dStart2, dEnd2, dVarMinStep2, dVarMaxStep2, dVarRound2 = self.app.get_varTimes( )
        assert(dVarMinStep2==dVarMinStep)
        assert(dVarMaxStep2==dVarMaxStep)
        assert(dVarRound2==dVarRound)
        iNumT = self.app.get_numTimesList()
        assert(iNumT==0)
        print(' var iNumT=',iNumT)
        iNumT2, daTimes2 = self.app.get_timesList()
        assert(iNumT==iNumT2)
        print('var iNumT2=',iNumT2)
        assert(len(daTimes2)==1)

        # now test time lists
        ierr = self.app.set_times( dStart, dEnd, dStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStart, dEnd, dStep, ierr))
        iNumT = self.app.get_numTimesList()
        assert(iNumT==0)
        print(' (list *not* generated) iNumT=',iNumT)
        iNumT2, daTimes2 = self.app.get_timesList()
        assert(iNumT==iNumT2)
        daTimes3 = np.array([])
        ierr = self.app.set_timesList( daTimes3 )
        print(' set_timesList: (empty list) => %i' % (ierr))
        assert(ierr == -1)
        daTimes3 = np.arange(58119.5,58120.5,0.003472222)
        ierr = self.app.set_timesList( daTimes3 )
        print(' set_timesList: ([0]=%.5f -> [-1]=%.5f) => %i' % (daTimes3[0], daTimes3[-1], ierr))
        assert(ierr == 0)
        iNumT3 = self.app.get_numTimesList()
        print('fix iNumT3=',iNumT3)
        # sabotage middle time list entry
        daTimes3[iNumT3//2] = daTimes3[iNumT3//2-1]
        ierr = self.app.set_timesList( daTimes3 )
        print(' set_timesList: ( <w/ repeated time> ) =>',ierr)
        assert(ierr == -1)
        daTimes3[iNumT3//2] = daTimes3[iNumT3//2-1]+30000.0
        ierr = self.app.set_timesList( daTimes3 )
        print(' set_timesList: ( <w/ bad time> ) =>',ierr)
        assert(ierr == -1)
        ierr = self.app.clear_timesList()
        print(' clear_timesList:() =>',ierr)
        assert(ierr == 0)
        iNumT3 = self.app.get_numTimesList()
        assert(iNumT3==0)
        print(' iNumT3=',iNumT3)

        # test set_tleFile
        for strFile in ['cnofs_tle.dat', '$DATADIR/cnofs_tle.dat']:
            ierr = self.app.set_tleFile(strFile)
            print(' set_tleFile: strFile=', strFile, ' => ', ierr)
            assert(ierr < 0)

        for strFile in ['../../unitTests/cnofs_tle.dat']:
            ierr = self.app.set_tleFile(strFile)
            print(' set_tleFile: strFile=', strFile, ' => ', ierr)
            assert(ierr == 0)

        strFile = self.app.get_tleFile( )
        print(' get_tleFile: returned tleFile=', strFile)
        assert(len(strFile) > 0)

        # test set_elementTime
        for dMjd in [31000.2, 70123.45]:
            ierr = self.app.set_elementTime( dMjd )
            print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
            assert(ierr < 0)

            elementTime = self.app.get_elementTime( )
            print(' get_elementTime: => ', elementTime)
            assert(elementTime == -1.0)

        for dMjd in [56789.4, 51145.5, 58119.5]:
            ierr = self.app.set_elementTime( dMjd )
            print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
            assert(ierr == 0)

            elementTime = self.app.get_elementTime( )
            print(' get_elementTime: => ', elementTime)
            assert(elementTime == dMjd)

        # test set_inclination
        for dValue in [-35.0, 191.0]:
            ierr = self.app.set_inclination( dValue )
            print(' set_inclination: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_inclination()
            print(' get_inclination: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [45.0, 0.0, 98.7, 180.0, 25.0]:
            ierr = self.app.set_inclination( dValue )
            print(' set_inclination: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_inclination()
            print(' get_inclination: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_rightAscension
        for dValue in [-35.0, 391.0]:
            ierr = self.app.set_rightAscension( dValue )
            print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_rightAscension()
            print(' get_rightAscension: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [0.0, 45.0, 298.7, 180.0, 125.0]:
            ierr = self.app.set_rightAscension( dValue )
            print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_rightAscension()
            print(' get_rightAscension: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_eccentricity
        for dValue in [-0.5, 1.0, 4.5]:
            ierr = self.app.set_eccentricity( dValue )
            print(' set_eccentricity: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_eccentricity()
            print(' get_eccentricity: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [0.0, 0.9999, 0.025, 0.25]:
            ierr = self.app.set_eccentricity( dValue )
            print(' set_eccentricity: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_eccentricity()
            print(' get_eccentricity: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_argOfPerigee
        for dValue in [-35.0, 391.0]:
            ierr = self.app.set_argOfPerigee( dValue )
            print(' set_argOfPerigee: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_argOfPerigee( )
            print(' get_argOfPerigee: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [0.0, 45.0, 298.7, 180.0, 125.0]:
            ierr = self.app.set_argOfPerigee( dValue )
            print(' set_argOfPerigee: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_argOfPerigee( )
            print(' get_argOfPerigee: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_meanAnomaly
        for dValue in [-35.0]:
            ierr = self.app.set_meanAnomaly( dValue )
            print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_meanAnomaly()
            print(' get_meanAnomaly: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [0.0, 45.0, 98.7, 180.0, 191.0, 25.0]:
            ierr = self.app.set_meanAnomaly( dValue )
            print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_meanAnomaly()
            print(' get_meanAnomaly: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_meanMotion
        for dValue in [0.0, -3.5, 31.0]:
            ierr = self.app.set_meanMotion( dValue )
            print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_meanMotion()
            print(' get_meanMotion: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [4.5, 12.7, 17.0, 8.5]:
            ierr = self.app.set_meanMotion( dValue )
            print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_meanMotion()
            print(' get_meanMotion: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_meanMotion1stDeriv
        for dValue in [12.7, -11.0]:
            ierr = self.app.set_meanMotion1stDeriv( dValue )
            print(' set_meanMotion1stDeriv: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_meanMotion1stDeriv()
            print(' get_meanMotion1stDeriv: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [0.0, -3.5, 4.5, -7.0, 1.5]:
            ierr = self.app.set_meanMotion1stDeriv( dValue )
            print(' set_meanMotion1stDeriv: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_meanMotion1stDeriv()
            print(' get_meanMotion1stDeriv: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_meanMotion2ndDeriv
        for dValue in [-1.5, 2.7]:
            ierr = self.app.set_meanMotion2ndDeriv( dValue )
            print(' set_meanMotion2ndDeriv: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_meanMotion2ndDeriv()
            print(' get_meanMotion2ndDeriv: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [0.0, 0.5, -0.7, 1.0, 0.05]:
            ierr = self.app.set_meanMotion2ndDeriv( dValue )
            print(' set_meanMotion2ndDeriv: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_meanMotion2ndDeriv()
            print(' get_meanMotion2ndDeriv: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_bStar
        for dValue in [-1.5, 2.7]:
            ierr = self.app.set_bStar( dValue )
            print(' set_bstar: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_bStar()
            print(' get_bStar: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [0.0, 0.5, -0.7, 1.0, 0.05]:
            ierr = self.app.set_bStar( dValue )
            print(' set_bstar: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_bStar()
            print(' get_bStar: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_altitudeOfApogee
        for dValue in [0.0, -35.0]:
            ierr = self.app.set_altitudeOfApogee( dValue )
            print(' set_altitudeOfApogee: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_altitudeOfApogee( )
            print(' get_altitudeOfApogee: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [45.0, 298.7, 180.0, 391.0, 525.0]:
            ierr = self.app.set_altitudeOfApogee( dValue )
            print(' set_altitudeOfApogee: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_altitudeOfApogee( )
            print(' get_altitudeOfApogee: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_altitudeOfPerigee
        for dValue in [0.0, -35.0]:
            ierr = self.app.set_altitudeOfPerigee( dValue )
            print(' set_altitudeOfPerigee: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_altitudeOfPerigee( )
            print(' get_altitudeOfPerigee: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [45.0, 298.7, 180.0, 391.0, 525.0]:
            ierr = self.app.set_altitudeOfPerigee( dValue )
            print(' set_altitudeOfPerigee: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_altitudeOfPerigee( )
            print(' get_altitudeOfPerigee: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_localTimeOfApogee
        for dValue in [-3.5, 31.0]:
            ierr = self.app.set_localTimeOfApogee( dValue )
            print(' set_localTimeOfApogee: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_localTimeOfApogee( )
            print(' get_localTimeOfApogee: => ', dRetValue)
            assert(dRetValue == -1.0)

        for dValue in [0.0, 4.5, 12.7, 17.0, 8.5]:
            ierr = self.app.set_localTimeOfApogee( dValue )
            print(' set_localTimeOfApogee: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_localTimeOfApogee( )
            print(' get_localTimeOfApogee: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_localTimeMaxInclination
        for dValue in [-3.5, 31.0]:
            ierr = self.app.set_localTimeMaxInclination( dValue )
            print(' set_localTimeMaxInclination: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_localTimeMaxInclination()
            print(' get_localTimeMaxInclination: => ', dRetValue)
            assert(dRetValue == -1.0)

        for dValue in [0.0, 4.5, 12.7, 17.0, 8.5]:
            ierr = self.app.set_localTimeMaxInclination( dValue )
            print(' set_localTimeMaxInclination: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_localTimeMaxInclination()
            print(' get_localTimeMaxInclination: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_timeOfPerigee
        for dMjd in [31000.2, 70123.4]:
            ierr = self.app.set_timeOfPerigee( dMjd )
            print(' set_timeOfPerigee: dMjd=', dMjd, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_timeOfPerigee( )
            print(' get_timeOfPerigee: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dMjd in [56789.4, 51145.5, 58119.5]:
            ierr = self.app.set_timeOfPerigee( dMjd )
            print(' set_timeOfPerigee: dMjd=', dMjd, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_timeOfPerigee( )
            print(' get_timeOfPerigee: => ', dRetValue)
            assert(dRetValue == dMjd)

        # test set_semiMajorAxis
        for dValue in [0.0, -1.5, 0.5, -6.7]:
            ierr = self.app.set_semiMajorAxis( dValue )
            print(' set_semiMajorAxis: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_semiMajorAxis( )
            print(' get_semiMajorAxis: => ', dRetValue)
            assert(dRetValue == 0.0)

        for dValue in [2.7, 1.0, 2.05]:
            ierr = self.app.set_semiMajorAxis( dValue )
            print(' set_semiMajorAxis: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_semiMajorAxis( )
            print(' get_semiMajorAxis: => ', dRetValue)
            assert(dRetValue == dValue)

        # test set_geosynchLon
        for dValue in [-195.1, 395.6]:
            ierr = self.app.set_geosynchLon( dValue )
            print(' set_geosynchLon: dValue=', dValue, ' => ', ierr)
            assert(ierr < 0)

            dRetValue = self.app.get_geosynchLon()
            print(' get_geosynchLon: => ', dRetValue)
            assert(dRetValue == -360.0)

        for dValue in [0.0, -35.0, 45.0, 298.7, 180.0, -180.0, 125.0]:
            ierr = self.app.set_geosynchLon( dValue )
            print(' set_geosynchLon: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            dRetValue = self.app.get_geosynchLon()
            print(' get_geosynchLon: => ', dRetValue)
            assert(dRetValue == dValue)

        # test position and velocity GEI
        dValue1 = dValue2 = dValue3 = 0.0
        # 0 values are rejected
        ierr = self.app.set_positionGei(dValue1, dValue2, dValue3)
        print(' set_positionGei: dValue=', dValue1, dValue2, dValue3, ' => ', ierr)
        assert(ierr < 0)
        ierr = self.app.set_velocityGei(dValue1, dValue2, dValue3)
        print(' set_velocityGei: dValue=', dValue1, dValue2, dValue3, ' => ', ierr)
        assert(ierr < 0)

        # position will be rejected, inside Earth
        dValue1 = dValue2 = dValue3 = 600.0
        ierr = self.app.set_positionGei(dValue1, dValue2, dValue3)
        print(' set_positionGei: dValue=', dValue1, dValue2, dValue3, ' => ', ierr)
        assert(ierr < 0)

        # position will be rejected, radius too large
        dValue1 = dValue2 = dValue3 = 280000.0
        ierr = self.app.set_positionGei(dValue1, dValue2, dValue3)
        print(' set_positionGei: dValue=', dValue1, dValue2, dValue3, ' => ', ierr)
        assert(ierr < 0)

        # velocity will be rejected, magnitude too large
        dValue1 = dValue2 = dValue3 = 12.0
        ierr = self.app.set_velocityGei(dValue1, dValue2, dValue3)
        print(' set_velocityGei: dValue=', dValue1, dValue2, dValue3, ' => ', ierr)
        assert(ierr < 0)

        dPos = 5000.0
        ierr = self.app.set_positionGei(dPos, -dPos / 2.0, dPos * 1.5)
        print(' set_positionGei: dValue=', dPos, -dPos / 2.0, dPos * 1.5, ' => ', ierr)
        assert(ierr == 0)

        dVel = 6.0
        ierr = self.app.set_velocityGei(-dVel, dVel + 1.5, -dVel / 3.0)
        print(' set_velocityGei: dValue=', -dVel, dVel + 1.5, -dVel / 3.0, ' => ', ierr)
        assert(ierr == 0)

        daPos, daVel = self.app.get_stateVectors( )
        print(' get_stateVectors: => ', daPos, daVel)
        assert(daPos[0] == dPos)
        assert(daPos[1] == -dPos / 2.0)
        assert(daPos[2] == dPos * 1.5)
        assert(daVel[0] == -dVel)
        assert(daVel[1] == dVel + 1.5)
        assert(daVel[2] == -dVel / 3.0)

        dPosX, dPosY, dPosZ = self.app.get_positionGei()
        dVelX, dVelY, dVelZ = self.app.get_velocityGei()

        assert(daPos[0] == dPosX)
        assert(daPos[1] == dPosY)
        assert(daPos[2] == dPosZ)
        assert(daVel[0] == dVelX)
        assert(daVel[1] == dVelY)
        assert(daVel[2] == dVelZ)

        # flip signs
        daPos[0] = -dPos
        daPos[1] = dPos / 2.0
        daPos[2] = -dPos * 1.5
        daVel[0] = dVel
        daVel[1] = -dVel + 1.5
        daVel[2] = dVel / 3.0
        ierr = self.app.set_stateVectors(daPos, daVel)
        print(' set_stateVectors: ', daPos, daVel, ' => ', ierr)
        assert(ierr == 0)

        dPosX, dPosY, dPosZ = self.app.get_positionGei()
        dVelX, dVelY, dVelZ = self.app.get_velocityGei()

        assert(daPos[0] == dPosX)
        assert(daPos[1] == dPosY)
        assert(daPos[2] == dPosZ)
        assert(daVel[0] == dVelX)
        assert(daVel[1] == dVelY)
        assert(daVel[2] == dVelZ)

    def test_model_vars( self ):
        # test set_model
        for strString in ["AX8", "CRRES", "", "Plasma"]:
            ierr = self.app.set_model( strString )
            print (' set_model: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            strModel = self.app.get_model()
            print (' get_model: => ', strModel)
            assert(strModel == "none")

        for strString in ["AE9", "AP9", "PLASMAE", "AE8", "AP8", "CRRESELE",
                          "CRRESPRO", "CAMMICE"]:
            ierr = self.app.set_model( strString )
            print (' set_model: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            strModel = self.app.get_model()
            print (' get_model: => ', strModel)
            assert(strModel == strString)

        # test set_modelDB
        for strFile in ['AE9V15_runtime_tables.mat', '$DATADIR/AE9V15_runtime_tables.mat']:
            ierr = self.app.set_modelDBFile(strFile)
            print (' set_modelDBFile: dbFile=', strFile, ' => ', ierr)
            assert(ierr < 0)

            strModelDB = self.app.get_modelDBFile( )
            print (' get_modelDBFile: returned file=', strModelDB)
            assert(strModelDB == 'UNDEF')

        for strFile in ['../../modelData/AE9V15_runtime_tables.mat']:
            ierr = self.app.set_modelDBFile(strFile)
            print (' set_modelDBFile: dbFile=', strFile, ' => ', ierr)
            assert(ierr == 0)

            strModelDB = self.app.get_modelDBFile( )
            print (' get_modelDBFile: returned file=', strModelDB)
            assert(strModelDB == strFile)

        # test set_kPhiDB
        for strFile in ['fastPhi_net.mat', '$DATADIR/fastPhi_net.mat']:
            ierr = self.app.set_kPhiDBFile(strFile)
            print (' set_kPhiDBFile: dbFile=', strFile, ' => ', ierr)
            assert(ierr < 0)

            strKPhiDB = self.app.get_kPhiDBFile( )
            print (' get_kPhiDBFile: returned file=', strKPhiDB)
            assert(strKPhiDB == 'UNDEF')

        for strFile in ['../../modelData/fastPhi_net.mat']:
            ierr = self.app.set_kPhiDBFile(strFile)
            print (' set_kPhiDBFile: dbFile=', strFile, ' => ', ierr)
            assert(ierr == 0)

            strKPhiDB = self.app.get_kPhiDBFile( )
            print (' get_kPhiDBFile: returned file=', strKPhiDB)
            assert(strKPhiDB == strFile)

        # test set_kHMinDB
        for strFile in ['fast_hmin_net.mat', '$DATADIR/fast_hmin_net.mat']:
            ierr = self.app.set_kHMinDBFile(strFile)
            print (' set_kHMinDBFile: dbFile=', strFile, ' => ', ierr)
            assert(ierr < 0)

            strKHMinDB = self.app.get_kHMinDBFile( )
            print (' get_kHMinDBFile: returned file=', strKHMinDB)
            assert(strKHMinDB == 'UNDEF')

        for strFile in ['../../modelData/fast_hmin_net.mat']:
            ierr = self.app.set_kHMinDBFile(strFile)
            print (' set_kHMinDBFile: dbFile=', strFile, ' => ', ierr)
            assert(ierr == 0)

            strKHMinDB = self.app.get_kHMinDBFile( )
            print (' get_kHMinDBFile: returned file=', strKHMinDB)
            assert(strKHMinDB == strFile)

        # test set_magfieldDB
        for strFile in ['igrfDB.h5', '$DATADIR/igrfDB.h5']:
            ierr = self.app.set_magfieldDBFile(strFile)
            print (' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
            assert(ierr < 0)

            strMagfieldDB = self.app.get_magfieldDBFile( )
            print (' get_magfieldDBFile: returned file=', strMagfieldDB)
            assert(strMagfieldDB == 'UNDEF')

        for strFile in ['../../modelData/igrfDB.h5']:
            ierr = self.app.set_magfieldDBFile(strFile)
            print (' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
            assert(ierr == 0)

            strMagfieldDB = self.app.get_magfieldDBFile( )
            print (' get_magfieldDBFile: returned file=', strMagfieldDB)
            assert(strMagfieldDB == strFile)

        # test set_doseDB
        for strFile in ['../../modelData/sdDB.h5', '$DATADIR/sdDB.h5']:
            ierr = self.app.set_doseDBFile(strFile)
            print (' set_doseDBFile: strString=', strFile, ' => ', ierr)
            assert(ierr < 0)

            doseDB = self.app.get_doseDBFile( )
            print (' get_doseDBFile: => ', doseDB)
            assert(doseDB == 'UNDEF')

        for strFile in ['../../modelData/sd2DB.h5']:
            ierr = self.app.set_doseDBFile(strFile)
            print (' set_doseDBFile: strString=', strFile, ' => ', ierr)
            assert(ierr == 0)

            doseDB = self.app.get_doseDBFile( )
            print (' get_doseDBFile: => ', doseDB)
            assert(doseDB == strFile)

        # test set_fluxType
        for strFile in ['none', '1', 'One', '3PtDiff']:
            ierr = self.app.set_fluxType(strFile)
            print (' set_fluxType: strString=', strFile, ' => ', ierr)
            assert(ierr < 0)

            strFluxType = self.app.get_fluxType( )
            print (' get_fluxType: => ', strFluxType)
            assert(strFluxType == 'none')

        for strFile in ['1PTDIFF', '2PTDIFF', 'INTEGRAL']:
            ierr = self.app.set_fluxType(strFile)
            print (' set_fluxType: strString=', strFile, ' => ', ierr)
            assert(ierr == 0)

            strFluxType = self.app.get_fluxType( )
            print (' get_fluxType: => ', strFluxType)
            assert(strFluxType == strFile)

        # test flux energy specifications
        # mixed ordering, with two duplicates
        daFluxEnergies = np.array([8, 6, 4, 2, 1, 0.1, 0.2, 0.4, 0.6, 0.8, 1, 2])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        assert(ierr == 0)
        # verify that the two duplicates are ignored
        iNum, daEret = self.app.get_fluxEnergies()
        print(daEret)
        assert(iNum > 0)
        assert(iNumE-2 == iNum)
        # verify that the energies are properly ordered
        assert(daEret[0]==0.1)
        # verify proper clearing of energy list
        ierr = self.app.clear_fluxEnergies( )
        assert(ierr == 0)
        iNum = self.app.get_numFluxEnergies( )
        assert(iNum == 0)

        # test bad energies included in list
        daFluxEnergies = np.array([8, 6, 4, 2, -1, 0.1, 0.2, 0.4, 0.6, 0.8, -2, 5000])
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        # verify list rejected
        assert(ierr < 0)
        # verify saved list is unchanged
        iNum = self.app.get_numFluxEnergies( )
        assert(iNum == 0)

        # add energy values singly
        for dValue in [0.0, -3.5, 4.5, 2.98, 1.8, -1.8, 12500.0, 3.6]:
          ierr = self.app.set_fluxEnergy( dValue )
          print(' set_fluxEnergy: dValue=', dValue, ' => ', ierr)
          if dValue>0.0 and dValue<5000:
            assert(ierr == 0)
          else:
            assert(ierr == -1)
        # verify that the bad values are not in list
        iNum, daEret = self.app.get_fluxEnergies()
        assert(iNum > 0)
        assert(iNum == 4)
        # verify that the energies are properly ordered
        assert(daEret[0]==1.8)

        # test wholesome list supplied overwrites previous contents
        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        # test energy2 with (needed) duplicates
        daFluxEnergies = np.array([0.05, 0.06, 0.08, 0.1, 0.1, 0.1])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies2( daFluxEnergies )
        assert(ierr == 0)
        # verify that all values accepted
        iNum, daEret = self.app.get_fluxEnergies2()
        print(daEret)
        assert(iNum > 0)
        assert(iNumE == iNum)
        # verify that the energies are properly ordered
        assert(daEret[0]==0.05)
        # verify proper clearing of energy list
        ierr = self.app.clear_fluxEnergies2( )
        assert(ierr == 0)
        iNum = self.app.get_numFluxEnergies2( )
        assert(iNum == 0)

        # test bad energies included in list
        daFluxEnergies = np.array([8, 6, 4, 2, -1, 0.1, 0.2, 0.4, 0.6, 0.8, -2, 5000])
        ierr = self.app.set_fluxEnergies2( daFluxEnergies )
        # verify list rejected
        assert(ierr < 0)
        # verify saved list is unchanged
        iNum = self.app.get_numFluxEnergies2( )
        assert(iNum == 0)

        # add energy values singly
        for dValue in [0.0, -3.5, 4.5, 2.98, 1.8, 1.8, -1.8, 12500.0, 3.6, 1.8]:
          ierr = self.app.set_fluxEnergy2( dValue )
          print(' set_fluxEnergy2: dValue=', dValue, ' => ', ierr)
          if dValue>0.0 and dValue<5000:
            assert(ierr == 0)
          else:
            assert(ierr == -1)
        # verify that the bad values are not in list
        iNum, daEret = self.app.get_fluxEnergies2()
        assert(iNum > 0)
        assert(iNum == 6)
        # verify that the energies are taken in order provided
        assert(daEret[0]==4.5)

        # test wholesome list supplied overwrites previous contents
        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies2( daFluxEnergies )
        print(' set_fluxEnergies2: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies2( )
        print(' get_numFluxEnergies2: => ', iRetVal)
        assert(iRetVal == iNumE)

        # test pitch angles
        dPitchAngle = 0.0
        ierr = self.app.set_pitchAngle(dPitchAngle)
        print (' set_pitchAngle: dPitchAngle=', dPitchAngle, ' => ', ierr)
        assert(ierr == 0)

        iNumP = self.app.get_numPitchAngles( )
        print (' get_numPitchAngles: => ', iNumP)
        assert(iNumP == 1)

        ierr, angles = self.app.get_pitchAngles( )
        print (' get_pitchAngles: => ', angles.shape, ', ierr=', ierr)
        assert(angles.shape == (iNumP,))

        # valid values are 0.0 - 180.0
        dPitchAngle = 270.0
        ierr = self.app.set_pitchAngle(dPitchAngle)
        print (' set_pitchAngle: dPitchAngle=', dPitchAngle, ' => ', ierr)
        assert(ierr < 0)

        iNumP = self.app.get_numPitchAngles( )
        print (' get_numPitchAngles: => ', iNumP)
        assert(iNumP == 1)

        dPitchAngle = 90.0
        ierr = self.app.set_pitchAngle(dPitchAngle)
        print (' set_pitchAngle: dPitchAngle=', dPitchAngle, ' => ', ierr)
        assert(ierr == 0)

        iNumP = self.app.get_numPitchAngles( )
        print (' get_numPitchAngles: => ', iNumP)
        assert(iNumP == 2)

        iNum, angles = self.app.get_pitchAngles( )
        print (' get_pitchAngles: => ', angles.shape, ', iNum=', iNum)
        assert(iNum == iNumP)
        assert(angles.shape == (iNumP,))

        daPitchAngles = np.array([5.0, 95.0, 150.0, 180.0])
        ierr = self.app.set_pitchAngles(daPitchAngles)
        print (' set_pitchAngles: daPitchAngles(#)=', len(daPitchAngles), ' => ', ierr)
        assert(ierr == 0)

        iNumP = self.app.get_numPitchAngles( )
        print (' get_numPitchAngles: => ', iNumP)
        assert(iNumP == len(daPitchAngles))

        ierr = self.app.clear_pitchAngles( )
        print (' clear_pitchAngles: => ', ierr)
        assert(ierr == 0)

        iNumP = self.app.get_numPitchAngles( )
        print (' get_numPitchAngles: => ', iNumP)
        assert(iNumP == 0)

        # test flux percentiles
        iPercentile = 10
        ierr = self.app.set_fluxPercentile(iPercentile)
        print (' set_fluxPercentile: iPercentile=', iPercentile, ' => ', ierr)
        assert(ierr == 0)

        iNumFp = self.app.get_numFluxPercentiles( )
        print (' get_numFluxPercentiles: => ', iNumFp)
        assert(iNumFp == 1)

        ierr, angles = self.app.get_fluxPercentiles( )
        print (' get_fluxPercentiles: => ', angles.shape, ', ierr=', ierr)
        assert(angles.shape == (iNumFp,))

        # valid values are 1 - 99
        iPercentile = -10
        ierr = self.app.set_fluxPercentile(iPercentile)
        print (' set_fluxPercentile: dPitchAngle=', iPercentile, ' => ', ierr)
        assert(ierr < 0)

        iNumFp = self.app.get_numFluxPercentiles( )
        print (' get_numFluxPercentiles: => ', iNumFp)
        assert(iNumFp == 1)

        iPercentile = 90
        ierr = self.app.set_fluxPercentile(iPercentile)
        print (' set_fluxPercentile: dPitchAngle=', iPercentile, ' => ', ierr)
        assert(ierr == 0)

        iNumFp = self.app.get_numFluxPercentiles( )
        print (' get_numFluxPercentiles: => ', iNumFp)
        assert(iNumFp == 2)

        iNum, daPercent = self.app.get_fluxPercentiles( )
        print (' get_fluxPercentiles: => ', daPercent.shape, ', iNum=', iNum)
        assert(iNum == iNumFp)
        assert(daPercent.shape == (iNumFp,))

        iaPercentiles = np.array([15, 50, 95, 99]).astype(np.int32)
        iNumFp = len(iaPercentiles)
        ierr = self.app.set_fluxPercentiles( iaPercentiles )
        print (' set_fluxPercentiles: iaPercentiles(#)=', iNumFp, ' => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numFluxPercentiles( )
        print (' get_numFluxPercentiles: => ', iNum)
        assert(iNum == iNumFp)

        ierr = self.app.clear_fluxPercentiles( )
        print (' clear_fluxPercentiles: => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numFluxPercentiles( )
        print (' get_numFluxPercentiles: => ', iNum)
        assert(iNum == 0)

        # test perturbed scenarios
        iScenario = 10
        ierr = self.app.set_fluxPerturbedScenario(iScenario)
        print (' set_fluxPerturbedScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr == 0)

        iNumPs = self.app.get_numFluxPerturbedScenarios( )
        print (' get_numFluxPerturbedScenarios: => ', iNumPs)
        assert(iNumPs == 1)

        ierr, iaScenarios = self.app.get_fluxPerturbedScenarios( )
        print (' get_fluxPerturbedScenarios: => ', iaScenarios.shape, ', ierr=', ierr)
        assert(iaScenarios.shape == (iNumPs,))

        iScenario = -10
        ierr = self.app.set_fluxPerturbedScenario(iScenario)
        print (' set_fluxPerturbedScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr < 0)

        iNumPs = self.app.get_numFluxPerturbedScenarios( )
        print (' get_numFluxPerturbedScenarios: => ', iNumPs)
        assert(iNumPs == 1)

        iScenario = 90
        ierr = self.app.set_fluxPerturbedScenario(iScenario)
        print (' set_fluxPerturbedScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr == 0)

        iNumPs = self.app.get_numFluxPerturbedScenarios( )
        print (' get_numFluxPerturbedScenarios: => ', iNumPs)
        assert(iNumPs == 2)

        iNum, iaScenarios = self.app.get_fluxPerturbedScenarios( )
        print (' get_fluxPerturbedScenarios: => ', iaScenarios.shape, ', iNum=', iNum)
        assert(iNum == iNumPs)
        assert(iaScenarios.shape == (iNumPs,))

        iaScenarios = np.array([1, 50, 150, 999]).astype(np.int32)
        iNumPs = len(iaScenarios)
        ierr = self.app.set_fluxPerturbedScenarios( iaScenarios )
        print (' set_fluxPerturbedScenarios: iaScenarios=', iaScenarios, ' => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numFluxPerturbedScenarios( )
        print (' get_numFluxPerturbedScenarios: => ', iNum)
        assert(iNum == iNumPs)

        ierr = self.app.clear_fluxPerturbedScenarios( )
        print (' clear_fluxPerturbedScenarios: => ', ierr)
        assert(ierr == 0)

        ierr = self.app.set_fluxPerturbedScenRange(1, 10)
        print (' set_fluxPerturbedScenRange: 1-10 => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numFluxPerturbedScenarios( )
        print (' get_numFluxPerturbedScenarios: => ', iNum)
        assert(iNum == 10)

        iNum, iaScenarios = self.app.get_fluxPerturbedScenarios( )
        print (' get_fluxPerturbedScenarios: => ', iaScenarios.shape, ', iNum=', iNum)
        assert(iNum == 10)
        assert(iaScenarios.shape == (10,))

        for i in range(10):
            assert(iaScenarios[i] == i + 1)

        # test Monte Carlo scenarios
        iScenario = 10
        ierr = self.app.set_fluxMonteCarloScenario(iScenario)
        print (' set_fluxMonteCarloScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr == 0)

        iNumPs = self.app.get_numFluxMonteCarloSenarios( )
        print (' get_numFluxMonteCarloSenarios: => ', iNumPs)
        assert(iNumPs == 1)

        ierr, iaScenarios = self.app.get_fluxMonteCarloScenarios( )
        print (' get_fluxMonteCarloScenarios: => ', iaScenarios.shape, ', ierr=', ierr)
        assert(iaScenarios.shape == (iNumPs,))

        iScenario = -10
        ierr = self.app.set_fluxMonteCarloScenario(iScenario)
        print (' set_fluxMonteCarloScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr < 0)

        iNumPs = self.app.get_numFluxMonteCarloSenarios( )
        print (' get_numFluxMonteCarloSenarios: => ', iNumPs)
        assert(iNumPs == 1)

        iScenario = 90
        ierr = self.app.set_fluxMonteCarloScenario(iScenario)
        print (' set_fluxMonteCarloScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr == 0)

        iNumPs = self.app.get_numFluxMonteCarloSenarios( )
        print (' get_numFluxMonteCarloSenarios: => ', iNumPs)
        assert(iNumPs == 2)

        iNum, iaScenarios = self.app.get_fluxMonteCarloScenarios( )
        print (' get_fluxMonteCarloScenarios: => ', iaScenarios.shape, ', iNum=', iNum)
        assert(iNum == iNumPs)
        assert(iaScenarios.shape == (iNumPs,))

        iaScenarios = np.array([1, 50, 150, 999]).astype(np.int32)
        iNumPs = len(iaScenarios)
        ierr = self.app.set_fluxMonteCarloScenarios( iaScenarios )
        print (' set_fluxMonteCarloScenarios: iaScenarios=', iaScenarios, ' => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numFluxMonteCarloSenarios( )
        print (' get_numFluxMonteCarloSenarios: => ', iNum)
        assert(iNum == iNumPs)

        ierr = self.app.clear_fluxMonteCarloScenarios( )
        print (' clear_fluxMonteCarloScenarios: => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numFluxMonteCarloSenarios( )
        print (' get_numFluxMonteCarloSenarios: => ', iNum)
        assert(iNum == 0)

        ierr = self.app.set_fluxMonteCarloScenRange(1, 10)
        print (' set_fluxMonteCarloScenRange: 1-10 => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numFluxMonteCarloSenarios( )
        print (' get_numFluxMonteCarloSenarios: => ', iNum)
        assert(iNum == 10)

        iNum, iaScenarios = self.app.get_fluxMonteCarloScenarios( )
        print (' get_fluxMonteCarloScenarios: => ', iaScenarios.shape, ', iNum=', iNum)
        assert(iNum == 10)
        assert(iaScenarios.shape == (10,))

        for i in range(10):
            assert(iaScenarios[i] == i + 1)

        # test set_monteCarloEpochTime
        for dMjd in [31000.2, 70123.4]:
            ierr = self.app.set_monteCarloEpochTime( dMjd )
            print(' set_monteCarloEpochTime: dMjd=', dMjd, ' => ', ierr)
            assert(ierr < 0)

            timeOfPerigee = self.app.get_monteCarloEpochTime( )
            print(' get_monteCarloEpochTime: => ', timeOfPerigee)
            assert(timeOfPerigee == 0.0)

        for dMjd in [56789.4, 51145.5, 58119.5]:
            ierr = self.app.set_monteCarloEpochTime( dMjd )
            print(' set_monteCarloEpochTime: dMjd=', dMjd, ' => ', ierr)
            assert(ierr == 0)

            timeOfPerigee = self.app.get_monteCarloEpochTime( )
            print(' get_monteCarloEpochTime: => ', timeOfPerigee)
            assert(timeOfPerigee == dMjd)

        # test set_monteCarloPerturb
        ierr = self.app.set_monteCarloPerturb( 0 )
        print (' set_monteCarloPerturb( 0 ): => ', ierr)
        assert(ierr == 0)

        iRetVal = self.app.get_monteCarloPerturb()
        print (' get_monteCarloPerturb: => ', iRetVal)
        assert(iRetVal == 0)

        ierr = self.app.set_monteCarloPerturb()
        print (' set_monteCarloPerturb(): => ', ierr)
        assert(ierr == 0)

        iRetVal = self.app.get_monteCarloPerturb()
        print (' get_monteCarloPerturb: => ', iRetVal)
        assert(iRetVal == 1)

        # test set_monteCarloWorstCase
        ierr = self.app.set_monteCarloWorstCase( )
        print (' set_monteCarloWorstCase( ): => ', ierr)
        assert(ierr == 0)

        iRetVal = self.app.get_monteCarloWorstCase( )
        print (' get_monteCarloWorstCase: => ', iRetVal)
        assert(iRetVal == 1)

        ierr = self.app.set_monteCarloWorstCase( 0 )
        print (' set_monteCarloWorstCase( 0 ): => ', ierr)
        assert(ierr == 0)

        iRetVal = self.app.get_monteCarloWorstCase( )
        print (' get_monteCarloPerturb: => ', iRetVal)
        assert(iRetVal == 0)

        # test set_reportTimes[Sec] 
        ierr = self.app.set_reportTimes(0.0,0.0)
        print (' set_reportTimes(0.0,0.0): => ', ierr)
        assert(ierr != 0)
        ierr = self.app.set_reportTimesSec(-1.0,120.0)
        print (' set_reportTimesSec(-1.0,120.0): => ', ierr)
        assert(ierr != 0)
        ierr = self.app.set_reportTimesSec(55197.0,600.0)
        print (' set_reportTimesSec(55197.0,600.0): => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numReportTimes( )
        print (' get_numReportTimes( ): => ',iRetVal)
        assert(iRetVal == 1)
        ierr = self.app.clear_reportTimes( )
        print (' clear_reportTimes( ): => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numReportTimes( )
        print (' get_numReportTimes( ): => ',iRetVal)
        assert(iRetVal == 0)
        ierr = self.app.set_reportTimesSec(57023.0,900.0)
        print (' set_reportTimesSec(57023.0,900.0): => ', ierr)
        assert(ierr == 0)
        ierr = self.app.set_reportTimesSec(57023.5,1800.0)
        print (' set_reportTimesSec(57023.5,1800.0): => ', ierr)
        assert(ierr == 0)
        ierr = self.app.set_reportTimes(57024.0,1.0/24.0)
        print (' set_reportTimes(57024.0,1.0/24.0): => ', ierr)
        assert(ierr == 0)
        ierr = self.app.set_reportTimesSec(57024.5,0.0)
        print (' set_reportTimesSec(57024.5,0.0): => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numReportTimes( )
        print (' get_numReportTimes( ): => ',iRetVal)
        assert(iRetVal == 4)
        iRetVal, daTimeRef, daDeltaSec = self.app.get_reportTimesSec()
        print (' get_reportTimes( ): => ',iRetVal)
        assert(iRetVal == 4)
        print ('   daTimeRef=',daTimeRef)
        print ('   daDeltaSec=',daDeltaSec)
        assert(daTimeRef[0]==57023.0)
        assert(daTimeRef[2]==57024.0)
        assert(daDeltaSec[1]==1800.0)
        assert(daDeltaSec[3]==0.0)

        # test set_reportAtTime 
        ierr = self.app.set_reportAtTime(0.0)
        print (' set_reportAtTime(0.0): => ', ierr)
        assert(ierr != 0)
        ierr = self.app.set_reportAtTime(-1.0)
        print (' set_reportAtTime(-1.0): => ', ierr)
        assert(ierr != 0)
        ierr = self.app.set_reportAtTime(55197.0)
        print (' set_reportAtTime(55197.0): => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numReportAtTime( )
        print (' get_numReportAtTime( ): => ',iRetVal)
        assert(iRetVal == 1)
        ierr = self.app.clear_reportAtTime( )
        print (' clear_reportAtTime( ): => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numReportAtTime( )
        print (' get_numReportAtTime( ): => ',iRetVal)
        assert(iRetVal == 0)
        ierr = self.app.set_reportAtTime(57023.0)
        print (' set_reportAtTime(57023.0): => ', ierr)
        assert(ierr == 0)
        ierr = self.app.set_reportAtTime(57023.5)
        print (' set_reportAtTime(57023.5): => ', ierr)
        assert(ierr == 0)
        ierr = self.app.set_reportAtTime(57024.0)
        print (' set_reportAtTime(57024.0): => ', ierr)
        assert(ierr == 0)
        ierr = self.app.set_reportAtTime(57024.5)
        print (' set_reportAtTime(57024.5): => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numReportAtTime( )
        print (' get_numReportAtTime( ): => ',iRetVal)
        assert(iRetVal == 4)
        iRetVal, daAtTime = self.app.get_reportAtTime( )
        print (' get_reportAtTime( ): => ',iRetVal)
        assert(iRetVal == 4)
        print ('   daAtTime=',daAtTime)
        assert(daAtTime[0]==57023.0)
        assert(daAtTime[2]==57024.0)

    def test_accum_vars( self ):
        # test set_accumMode w/ invalid strings
        for strString in ["", "none", "default"]:
            ierr = self.app.set_accumMode( strString )
            print (' set_accumMode: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

        # test for 'empty' list of accumModes
        strRet = self.app.get_accumMode( )
        print (' get_accumMode: => ', strRet)
        assert(strRet == "-none-")

        # test set_accumMode w/ valid strings; this is building a *list*
        ii = 0
        for strString in ["Cumul", "Intv", "Boxcar", "Exponential"]:
            ierr = self.app.set_accumMode( strString )
            print (' set_accumMode: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            print (' get_numAccumModes: => ', self.app.get_numAccumModes( ) )
            # get first entry of list
            strRet = self.app.get_accumMode( )
            print (' get_accumMode: => ', strRet)
            # get specific entry of just-added accum in list
            strRet = self.app.get_accumModeEntry( ii+1 )
            print (' get_accumModeEntry(',ii+1,'): => ', strRet)
            assert(strRet == strString)
            ii += 1
        # test that first entry matches first specified:
        strRet = self.app.get_accumMode( )
        print (' get_accumMode: => ', strRet)
        assert(strRet == "Cumul")  # case sensitive; same form as input

        ##  -> validateParameters has NOT been called, so default is empty list
        # test (single) values in set_accumInterval [days]
        dTestVal = self.app.get_accumIntervalSec()
        print(' get_accumIntervalSec: [default] => ', dTestVal)
        assert(dTestVal < 0.0)
        dLastVal = dTestVal
        for dTestVal in [-10, -1.0, 0, 2.5, 14, 9999]:  # time values in days
            ierr = self.app.set_accumInterval(dTestVal)
            print (' set_accumInterval: dTestVal=', dTestVal, ' => ', ierr)
            if dTestVal>0 and dTestVal <9999:
                assert(ierr == 0)
            else:
                assert(ierr == -1)

            iNum = self.app.get_numAccumIntervals( )
            print(' get_numAccumIntervals: => ', iNum)
            dIntervalSec = self.app.get_accumIntervalSecEntry( iNum )
            print(' get_accumIntervalSecEntry(',iNum,'): => ', dIntervalSec)

            if dTestVal <= 0:
                assert(dIntervalSec == dLastVal)
                assert( iNum == 0 )
            if dTestVal == 9999:
                assert(dIntervalSec == dLastVal)
                assert( iNum == 2 )
            if dTestVal > 0 and dTestVal < 9999:
                assert(dIntervalSec == math.floor(dTestVal * 86400.0 + 0.5))
                dLastVal = dIntervalSec
                assert( iNum > 0 )

        # clear interval list, and verify
        self.app.clear_accumIntervals( )
        print(' clear_accumIntervals')
        iNum = self.app.get_numAccumIntervals( )
        print(' get_numAccumIntervals: => ', iNum)
        assert( iNum == 0 )
        dTestVal = self.app.get_accumIntervalSec()
        print(' get_accumIntervalSec: [default] => ', dTestVal)
        assert(dTestVal < 0.0)
        dTestVal = self.app.get_accumIntervalSecEntry( 1 )
        print(' get_accumIntervalSecEntry( 1 ): => ', dTestVal)
        assert(dTestVal < 0.0)
        dLastVal = dTestVal
 
        # test values in set_accumIntervalSec [seconds]
        # -->> these are added to list in *ASCENDING* order
        iCount = 0
        dFirstVal = -1
        for dTestVal in [-5, 0, 10, 90, 5400, 129600]:  # time values in seconds
            ierr = self.app.set_accumIntervalSec(dTestVal)
            print (' set_accumIntervalSec: dTestVal=', dTestVal, ' => ', ierr)
            if dTestVal>0:
                assert(ierr == 0)
            else:
                assert(ierr == -1)

            dIntervalSec = self.app.get_accumIntervalSecEntry( iCount+1 )
            print(' get_accumIntervalSecEntry(',iCount+1,'): => ', dIntervalSec)

            if dTestVal <= 0 :
                assert(dIntervalSec == dLastVal)
            else:
                assert(dIntervalSec == dTestVal)  # works here only because these 
                dLastValid = dIntervalSec         #  are being added in numerical order
                if iCount == 0: dFirstVal = dIntervalSec # smallest value is first
            if dTestVal>0: iCount += 1
            iNum = self.app.get_numAccumIntervals( )
            print(' get_numAccumIntervals: => ', iNum)
            assert( iNum == iCount )
        # verify first value is smallest
        dIntervalSec = self.app.get_accumIntervalSec()
        print(' get_accumIntervalSec: => ', dIntervalSec)
        assert(dIntervalSec == dFirstVal)

        # clear interval list, and verify
        self.app.clear_accumIntervals( )
        print(' clear_accumIntervals')
        iNum = self.app.get_numAccumIntervals( )
        print(' get_numAccumIntervals: => ', iNum)
        assert( iNum == 0 )

        # test multiple out-of-order values in set_accumIntervalSec
        # -->> these are added to list in *ASCENDING* order
        iCount = 0
        dFirstVal = 8640000
        for dTestVal in [90, 5400, 10, 60, -45, 10]:  # time values in seconds
            ierr = self.app.set_accumIntervalSec(dTestVal)
            print (' set_accumIntervalSec: dTestVal=', dTestVal, ' => ', ierr)
            if dTestVal>0:
                if iCount>3:
                    assert(ierr < 0) # reject repeat value
                else:
                    assert(ierr == 0)
                    iCount += 1
                    if dTestVal < dFirstVal:  # find smallest value
                      dFirstVal = dTestVal
            else:
                assert(ierr == -1) # reject invalid value
        
            dIntervalSec = self.app.get_accumIntervalSec()
            print(' iCount=',iCount,': get_accumIntervalSec: => ', dIntervalSec)
            if iCount > 0:
                assert(dIntervalSec == dFirstVal)
            else:
                assert(dIntervalSec < 0.0)
            iNum = self.app.get_numAccumIntervals( )
            print(' get_numAccumIntervals: => ', iNum)
            assert( iNum == iCount )
            dPrev = -99.9
            iInOrder = 1
            # print out current list, verify in ascending order
            print(' accumIntervalSecEntry[ii]: ',end='')
            for ii in range(iNum):
              dCurr = self.app.get_accumIntervalSecEntry( ii+1 )
              print( dCurr,', ',end='')
              if dPrev >= dCurr: iInOrder = 0
              dPrev = dCurr
            print(' <end>')
            assert( iInOrder == 1)

        # test set_accumIncrementSec
        for dTestVal in [-10, -1.0, 0, 14, 9999]:  # only positive values are accepted
            initialVal = self.app.get_accumIncrementSec()  # store initial value

            ierr = self.app.set_accumIncrementSec(dTestVal)
            print(' set_accumIncrementSec: dTestVal=', dTestVal, ' => ', ierr)

            returnVal = self.app.get_accumIncrementSec()
            print(' get_accumIncrementSec: => ', returnVal)

            if dTestVal >= 0:
                assert(ierr == 0)
                assert(returnVal == dTestVal)
            else:
                assert(ierr == -1)
                assert(returnVal == initialVal)

        # test set_accumIncrementFrac
        for dValue in [-1.0, 0.0, 1.0, 5.0]:
            ierr = self.app.set_accumIncrementFrac( dValue )
            print(' set_accumIncrementFrac: dValue=', dValue, ' => ', ierr)
            assert(ierr == -1)

            returnVal = self.app.get_accumIncrementFrac()
            print(' get_accumIncrementFrac: => ', returnVal)
            assert(returnVal == 0)

        for dValue in [0.10, 0.25, 0.75, 0.99]:
            ierr = self.app.set_accumIncrementFrac( dValue )
            print(' set_accumIncrementFrac: dValue=', dValue, ' => ', ierr)
            assert(ierr == 0)

            returnVal = self.app.get_accumIncrementFrac()
            print(' get_accumIncrementFrac: => ', returnVal)
            assert(returnVal == dValue)

    def test_dose_vars( self ):
        # test dose depths
        strString = "inches"
        ierr = self.app.set_doseDepthUnits( strString )
        print (' set_doseDepthUnits: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        units = self.app.get_doseDepthUnits( )
        print (' get_doseDepthUnits: => ', units)
        assert(units == "millimeters")

        strString = "gpercm2"
        ierr = self.app.set_doseDepthUnits( strString )
        print (' set_doseDepthUnits: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        units = self.app.get_doseDepthUnits( )
        print (' get_doseDepthUnits: => ', units)
        assert(units == strString)

        daDepths = np.array([ -0.2, 0.0, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0])
        iNumD = len(daDepths)
        ierr = self.app.set_doseDepthValues( daDepths )
        print (' set_doseDepthValues: daDepths=', daDepths, ' => ', ierr)
        assert(ierr < 0)

        iRetValD = self.app.get_numDoseDepths( )
        print (' get_numDoseDepths: => ', iRetValD)
        assert(iRetValD == 0)

        daDepths = np.array([ 0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 100.0])
        iNumD = len(daDepths)
        ierr = self.app.set_doseDepthValues( daDepths )
        print (' set_doseDepthValues: daDepths=', daDepths, ' => ', ierr)
        assert(ierr == 0)

        iRetValD = self.app.get_numDoseDepths( )
        print (' get_numDoseDepths: => ', iRetValD)
        assert(iRetValD == iNumD)

        daDepths = np.array([ 0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 100.0])
        iNumD = len(daDepths)
        ierr = self.app.set_doseDepths( daDepths, "mm" )
        print (' set_doseDepths: daDepths=', daDepths, ', units=mm => ', ierr)
        assert(ierr == 0)

        iRetValD = self.app.get_numDoseDepths( )
        print (' get_numDoseDepths: => ', iRetValD)
        assert(iRetValD == iNumD)

        # this works even though 100.0 is outside the allowed max for g/cm2
        # because values are not checked until later
        daDepths = np.array([ 0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 100.0])
        iNumD = len(daDepths)
        ierr = self.app.set_doseDepths( daDepths, "g/cm2" )
        print (' set_doseDepths: daDepths=', daDepths, ', units=g/m2 => ', ierr)
        assert(ierr == 0)

        iRetValD = self.app.get_numDoseDepths( )
        print (' get_numDoseDepths: => ', iRetValD)
        assert(iRetValD == iNumD)

        # test set_doseDetector
        for strString in ["He", "NaCl", "CalciumChloride"]:
            ierr = self.app.set_doseDetector( strString )
            print (' set_doseDetector: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            retStr = self.app.get_doseDetector( )
            print (' get_doseDetector: => ', retStr)
            assert(retStr == "Silicon")  # default value

        for strString in ["water", "air", "lithium", "calcium", "silicon", "gallium"]:
            ierr = self.app.set_doseDetector( strString )
            print (' set_doseDetector: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            retStr = self.app.get_doseDetector().lower()
            print (' get_doseDetector: => ', retStr)
            assert(retStr == strString)

        # test set_doseGeometry
        for strString in ["square", "circle", "dodecahedron"]:
            ierr = self.app.set_doseGeometry( strString )
            print (' set_doseGeometry: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            strRet = self.app.get_doseGeometry( )
            print (' get_doseGeometry: => ', strRet)
            assert(strRet == "Spherical2pi")  # Spherical2pi is the default geometry

        for strString in ["SemiInfSlab", "FiniteSlab", "Spherical2pi"]:
            ierr = self.app.set_doseGeometry( strString )
            print (' set_doseGeometry: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            strRet = self.app.get_doseGeometry( )
            print (' get_doseGeometry: => ', strRet)
            assert(strRet == strString)

        # test dose 'with brems' option
        ierr = self.app.set_doseWithBrems( )
        print (' set_doseWithBrems: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        iVal = self.app.get_doseWithBrems( )
        print (' get_doseWithBrems: => ', iVal)
        assert(iVal == 1)
        ierr = self.app.set_doseWithBrems( False )
        print (' set_doseWithBrems: verdict=False => ', ierr)
        assert(ierr == 0)
        iVal = self.app.get_doseWithBrems( )
        print (' get_doseWithBrems: => ', iVal)
        assert(iVal == 0)
        ierr = self.app.set_doseWithBrems( True )
        print (' set_doseWithBrems: verdict=True => ', ierr)
        assert(ierr == 0)
        iVal = self.app.get_doseWithBrems( )
        print (' get_doseWithBrems: => ', iVal)
        assert(iVal == 1)

        # test dose kernel vars
        ierr = self.app.set_useDoseKernel( )
        print (' set_useDoseKernel: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        iVal = self.app.get_useDoseKernel( )
        print (' get_useDoseKernel: => ', iVal)
        assert(iVal == 1)

        ierr = self.app.set_useDoseKernel( False )
        print (' set_useDoseKernel: verdict= False => ', ierr)
        assert(ierr == 0)
        iVal = self.app.get_useDoseKernel( )
        print (' get_useDoseKernel: => ', iVal)
        assert(iVal == 0)

        strString = "/kernelXml"
        ierr = self.app.set_doseKernelDir( strString )
        print (' set_doseKernelDir: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = self.app.get_doseKernelDir( )
        print (' get_doseKernelDir: => ', strRet)
        assert(strRet == "")

        strString = "../../kernelXml"
        ierr = self.app.set_doseKernelDir( strString )
        print (' set_doseKernelDir: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = self.app.get_doseKernelDir( )
        print (' get_doseKernelDir: => ', strRet)
        assert(strRet == strString + '/')

        strFile = 'Dose_electron_hemisphere_NO.xml'
        ierr = self.app.set_doseKernelFile(strFile)
        print (' set_doseKernelFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr < 0)

        strModelDB = self.app.get_doseKernelFile( )
        print (' get_doseKernelFile: returned file=', strModelDB)
        assert(strModelDB == '')

        strFile = '../../kernelXml/Dose_electron_hemisphere_GaAs.xml'
        ierr = self.app.set_doseKernelFile(strFile)
        print (' set_doseKernelFile: dbFile=', strFile, ' => ', ierr)
        assert(ierr == 0)

        strModelDB = self.app.get_doseKernelFile( )
        print (' get_doseKernelFile: returned file=', strModelDB)
        assert(strModelDB == strFile)

    def test_aggreg_vars( self ):
        # test confidence levels
        iConf = 0
        ierr = self.app.set_aggregConfLevel( iConf )
        print (' set_aggregConfLevel: iConf=', iConf, ' => ', ierr)
        assert(ierr == 0)

        iNumAggCon = self.app.get_numAggregConfLevels( )
        print (' get_numAggregConfLevels: => ', iNumAggCon)
        assert(iNumAggCon == 1)

        iConf = -10
        ierr = self.app.set_aggregConfLevel( iConf )
        print (' set_aggregConfLevel: iConf=', iConf, ' => ', ierr)
        assert(ierr < 0)

        iNumAggCon = self.app.get_numAggregConfLevels( )
        print (' get_numAggregConfLevels: => ', iNumAggCon)
        assert(iNumAggCon == 1)

        iConf = 90
        ierr = self.app.set_aggregConfLevel( iConf )
        print (' set_aggregConfLevel: iConf=', iConf, ' => ', ierr)
        assert(ierr == 0)

        iNumAggCon = self.app.get_numAggregConfLevels( )
        print (' get_numAggregConfLevels: => ', iNumAggCon)
        assert(iNumAggCon == 2)

        ierr = self.app.set_aggregMedian()
        print (' set_aggregMedian: => ', ierr)
        assert(ierr == 0)

        iNumAggCon = self.app.get_numAggregConfLevels( )
        print (' get_numAggregConfLevels: => ', iNumAggCon)
        assert(iNumAggCon == 3)

        iNumAggCon, iaConfLevel = self.app.get_aggregConfLevels( )
        print (' get_aggregConfLevels: => ', iaConfLevel.shape, ', iNum=', iNumAggCon)
        assert(iNumAggCon == 3)
        assert(iaConfLevel.shape == (3,))

        # median was inserted in the middle
        assert(iaConfLevel[0] == 0)
        assert(iaConfLevel[1] == 50)
        assert(iaConfLevel[2] == 90)

        ierr = self.app.set_aggregMean( )
        print (' set_aggregMean: => ', ierr)
        assert(ierr < 0)

        iNumAggCon = self.app.get_numAggregConfLevels( )
        print (' get_numAggregConfLevels: => ', iNumAggCon)
        assert(iNumAggCon == 3)

        iNumAggCon, iaConfLevel = self.app.get_aggregConfLevels( )
        print (' get_aggregConfLevels: => ', iaConfLevel.shape, ', iNum=', iNumAggCon)
        assert(iNumAggCon == 3)
        assert(iaConfLevel.shape == (3,))

        iaPercent = np.array([1, 50, 75, 100]).astype(np.int32)
        iNumConf = len(iaPercent)
        ierr = self.app.set_aggregConfLevels( iaPercent )
        print (' set_aggregConfLevels: iaPercent=', iaPercent, ' => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numAggregConfLevels( )
        print (' get_numAggregConfLevels: => ', iNum)
        assert(iNum == iNumConf)

        ierr = self.app.clear_aggregConfLevels( )
        print (' clear_aggregConfLevels: => ', ierr)
        assert(ierr == 0)

        iNumConf = self.app.get_numAggregConfLevels( )
        print (' get_numAggregConfLevels: => ', iNumConf)
        assert(iNumConf == 0)

    def test_legacy_vars( self ):
        # test set_legActivityLevel
        for strString in ["", "Medium", "None"]:
            ierr = self.app.set_legActivityLevel( strString )
            print (' set_legActivityLevel: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            actLevel = self.app.get_legActivityLevel()
            print (' get_legActivityLevel: => ', actLevel)
            assert(actLevel == "undef")

        for strString in ["Quiet", "Min", "Active", "Max"]:
            ierr = self.app.set_legActivityLevel( strString )
            print (' set_legActivityLevel: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            actLevel = self.app.get_legActivityLevel( )
            print (' get_legActivityLevel: => ', actLevel)
            assert(actLevel == strString)

        # test set_legActivityRange
        for strString in ["", "min", "none", "0"]:
            ierr = self.app.set_legActivityRange( strString )
            print (' set_legActivityRange: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            actRange = self.app.get_legActivityRange( )
            print (' get_legActivityRange: => ', actRange)
            assert(actRange == "undef")

        for strString in ["5-7.5", "7.5-10", "10-15", "15-20", "20-25", ">25",
                          "Avg", "Max", "All"]:
            ierr = self.app.set_legActivityRange( strString )
            print (' set_legActivityRange: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            actRange = self.app.get_legActivityRange( )
            print (' get_legActivityRange: => ', actRange)
            assert(actRange == strString)

        # test set_legActivityRange
        for dTestVal in [-1.0, -0.1, 400.1, 9999]:
            ierr = self.app.set_leg15DayAvgAp( dTestVal )
            print (' set_leg15DayAvgAp: dTestVal=', dTestVal, ' => ', ierr)
            assert(ierr < 0)

            dayAp = self.app.get_leg15DayAvgAp( )
            print (' get_leg15DayAvgAp: => ', dayAp)
            assert(dayAp == -1.0)

        for dTestVal in [0.0, 5.0, 6.1, 100.0, 400]:
            ierr = self.app.set_leg15DayAvgAp(dTestVal)
            print (' set_leg15DayAvgAp: dTestVal=', dTestVal, ' => ', ierr)
            assert(ierr == 0)

            dayAp = self.app.get_leg15DayAvgAp( )
            print (' get_leg15DayAvgAp: => ', dayAp)
            assert(dayAp == dTestVal)

    def test_cammice_vars( self ):
        # test set_camMagfieldModel
        for strString in ["AE9", "AP9", "AX8", "CRRES", "none", ""]:
            ierr = self.app.set_camMagfieldModel( strString )
            print (' set_camMagfieldModel: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            strRet = self.app.get_camMagfieldModel( )
            print (' get_camMagfieldModel: => ', strRet)
            assert(strRet == "undef")

        for strString in ["igrf", "igrfop", "igrf/op", "igrf_op"]:
            ierr = self.app.set_camMagfieldModel( strString )
            print (' set_camMagfieldModel: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            strRet = self.app.get_camMagfieldModel( )
            print (' get_camMagfieldModel: => ', strRet)

            if strString == "igrf":
                assert(strRet == "IGRF")
            else:
                assert(strRet == "IGRF/OP")

        # test set_camDataFilter
        for strString in ["none", ""]:
            ierr = self.app.set_camDataFilter( strString )
            print (' set_camDataFilter: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            strRet = self.app.get_camDataFilter( )
            print (' get_camDataFilter: => ', strRet)
            assert(strRet == "undef")

        for strString in ["All", "Filtered"]:
            ierr = self.app.set_camDataFilter( strString )
            print (' set_camDataFilter: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            strRet = self.app.get_camDataFilter( )
            print (' get_camDataFilter: => ', strRet)
            assert(strRet == strString)

        # test set_camPitchAngleBin
        for strString in ["0", "180-190", "0-180", "", "all"]:
            ierr = self.app.set_camPitchAngleBin( strString )
            print (' set_camPitchAngleBin: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            strRet = self.app.get_camPitchAngleBin( )
            print (' get_camPitchAngleBin: => ', strRet)
            assert(strRet == "undef")

        for strString in ["Omnidirectional", "0-10", "10-20", "20-30", "30-40", "40-50",
                          "50-60", "60-70", "70-80", "80-90", "90-100", "100-110", "110-120",
                          "120-130", "130-140", "140-150", "150-160", "160-170", "170-180"]:
            ierr = self.app.set_camPitchAngleBin( strString )
            print (' set_camPitchAngleBin: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            strRet = self.app.get_camPitchAngleBin( )
            print (' get_camPitchAngleBin: => ', strRet)
            assert(strRet == strString)

        # test set_camSpecies
        for strString in ["H-", "", "all"]:
            ierr = self.app.set_camSpecies( strString )
            print (' set_camSpecies: strString=', strString, ' => ', ierr)
            assert(ierr < 0)

            strRet = self.app.get_camSpecies( )
            print (' get_camSpecies: => ', strRet)
            assert(strRet == "none")

        # passing tests
        for strString in ["H+", "PROTONS", "P+", "P", "He+", "He+2", "O+", "H", "He", "O", "Ions"]:
            ierr = self.app.set_camSpecies( strString )
            print (' set_camSpecies: strString=', strString, ' => ', ierr)
            assert(ierr == 0)

            strRet = self.app.get_camSpecies( )
            print (' get_camSpecies: => ', strRet)
            if strString == "PROTONS" or strString == "P" or strString == "P+":
                assert (strRet == "H+")
            else:
                assert(strRet == strString)

    def test_run_model1( self ):
        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)

        strProp = "Kepler"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop( )
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        ierr = self.app.set_keplerUseJ2( )
        print(' set_keplerUseJ2: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        bUseJ2 = self.app.get_keplerUseJ2( )
        print(' get_keplerUseJ2: => ', bUseJ2)
        assert(bUseJ2 == 1)

        dMjd = 57023.0
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dValInput = 30
        ierr = self.app.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 254.1
        ierr = self.app.set_rightAscension( dValInput )
        print(' set_rightAscension: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_rightAscension()
        print(' get_rightAscension: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 45.0
        ierr = self.app.set_argOfPerigee( dValInput )
        print(' set_argOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_argOfPerigee( )
        print(' get_argOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 0.25
        ierr = self.app.set_eccentricity( dValInput )
        print(' set_eccentricity: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_eccentricity()
        print(' get_eccentricity: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 25.4
        ierr = self.app.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanAnomaly( )
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57023
        dEndTime = 57024.5
        dTimeStep = 60
        ierr = self.app.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "GEI"
        strUnits = "RE"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strModel = "AP9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AP9V15_runtime_tables.mat"
        ierr = self.app.set_modelDBFile( strModelDB )
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = self.app.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = self.app.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 4)

        dValInput = 7.5
        ierr = self.app.set_meanMotion( dValInput )
        print(' set_meanMotion: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanMotion()
        print(' get_meanMotion: => ', dValue)
        assert(dValue == dValInput)

        strFluxType = "1PTDIFF"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 15,
                                 20, 30, 50, 60, 80, 100, 150, 200, 300])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        ierr = self.app.set_fluxMean( )
        print(' set_fluxMean: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_fluxMean( )
        print(' get_fluxMean: => ', fluxMean)
        assert(fluxMean == 1)

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 0)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        # first chunk should be default chunk size 960
        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        # new chunk size - will reset data
        iChunkSize = 335
        ierr = self.app.set_chunkSize( iChunkSize )
        print (' set_chunkSize: iChunkSize=', iChunkSize, ' => ', ierr)
        assert(ierr == 0)
        retVal = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', retVal)
        assert(retVal == iChunkSize)

        bFirstRun = True

        # loop while there's still data left
        while iNum > 0:
            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print (' get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))

            if bFirstRun:
                # first returned time is start time
                assert(daTimes[0] == dStartTime)
                assert(iNum == iChunkSize)
                bFirstRun = False
            else:
                assert(iNum <= iChunkSize)

            iNum2, da3FluxData = self.app.flyin_mean( )
            print(' flyin_mean: => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 == iNum)

            if (iNum > 0):
                assert(da3FluxData.shape == (iNum, iNumE, 1))

            iNum2, da3FluxData = self.app.flyin_percentile( 50 )
            print(' flyin_percentile( 50 ): => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

            iNum2, da3FluxData = self.app.flyin_perturbedMean( 1 )
            print(' flyin_perturbedMean( 1 ): => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

            iNum2, da3FluxData = self.app.flyin_monteCarlo( 10 )
            print(' flyin_monteCarlo(10): => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

        # reset data back to beginning
        ierr = self.app.reset_modelData( )
        print (' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print (' get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))

        # first returned time is start time
        assert(daTimes[0] == dStartTime)
        assert(iNum == iChunkSize)
        bFirstRun = False

        iNum2, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
          = self.app.flyin_meanPlus( )
        print(' flyin_meanPlus: => iNum=', iNum2, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == iNum)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da3FluxData.shape == (iNum, iNumE, 1))
        assert(da2PitchAngles.shape == (1, 1))

        # verify pitch angle is 0
        assert(da2PitchAngles[0][0] == 0)

        # reset data back to beginning
        ierr = self.app.reset_modelData( )
        print (' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum2, da2FluxData = self.app.flyin_mean2d( )
        print(' flyin_mean2d: => iNum=', iNum2, ', len(da2FluxData)=', len(da2FluxData))
        assert(iNum2 == iNum)
        assert(da2FluxData.shape == (iNum, iNumE))
        # use slice to remove third dimension and the rest of the data should match
        np.array_equal(da2FluxData, da3FluxData[:, :, 0])

        # reset data back to beginning
        ierr = self.app.reset_modelData()
        print (' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles2, da3Data \
          = self.app.get_modelData( 'flux', 'mean' )
        print(' get_modelData: => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
              ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
              ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum2 == iNum)
        assert(len(daTimes2) == iNum)
        assert(np.array_equal(daTimes2, daTimes))
        assert(len(daCoord12) == iNum)
        assert(np.array_equal(daCoord12, daCoord1))
        assert(len(daCoord22) == iNum)
        assert(np.array_equal(daCoord22, daCoord2))
        assert(len(daCoord32) == iNum)
        assert(np.array_equal(daCoord32, daCoord3))
        assert(da2PitchAngles2.shape == (1, 1))
        assert(da3Data.shape == (iNum, iNumE, 1))

        # verify pitch angles contains all 0's
        assert(da2PitchAngles[0][0] == 0)

        vars = self.app.get_modelData( 'flux', 'mean', -1, "interval" )
        print(' get_modelData: => ', vars[0])
        assert(vars[0] < 0)

        vars = self.app.get_modelData( 'fluence', 'mean' )
        print(' get_modelData: => ', vars[0])
        assert(vars[0] < 0)

        # reset model run: removes data files, but retains parameter settings
        ierr = self.app.reset_modelRun( )
        print (' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        # now errors
        vars = self.app.get_ephemeris( )
        print (' get_ephemeris: => ', vars[0])
        assert(vars[0] < 0)

        dStartTime2, dEndTime2, dTimeStep2 = self.app.get_times( )
        print (' get_times: => dStartTime=', dStartTime2, ', dEndTime=', dEndTime2,
               ', dTimeStep=', dTimeStep2)
        assert(dStartTime2 == dStartTime)
        assert(dEndTime2 == dEndTime)
        assert(dTimeStep2 == dTimeStep)

        strString = self.app.get_fluxType( )
        print(' get_fluxType: => fluxType=', strString)
        assert(strString == strFluxType)

        ## clear orbit parameter settings
        ierr = self.app.reset_orbitParameters( )
        print (' reset_orbitParameters: => ', ierr)
        assert(ierr == 0)

        dStartTime2, dEndTime2, dTimeStep2 = self.app.get_times( )
        print (' get_times: => dStartTime=', dStartTime2, ', dEndTime=', dEndTime2,
               ', dTimeStepSecs=', dTimeStep2)
        assert(dStartTime2 == -1)
        assert(dEndTime2 == -1)
        assert(dTimeStep2 == -1)

        strString = self.app.get_fluxType( )
        print(' get_fluxType: => fluxType=', strString)
        assert(strString == '1PTDIFF')

    def test_run_model1a( self ):
        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)

        strProp = "Kepler"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop( )
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        ierr = self.app.set_keplerUseJ2( )
        print(' set_keplerUseJ2: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        bUseJ2 = self.app.get_keplerUseJ2( )
        print(' get_keplerUseJ2: => ', bUseJ2)
        assert(bUseJ2 == 1)

        dMjd = 57023.0
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dValInput = 30
        ierr = self.app.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 254.1
        ierr = self.app.set_rightAscension( dValInput )
        print(' set_rightAscension: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_rightAscension()
        print(' get_rightAscension: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 45.0
        ierr = self.app.set_argOfPerigee( dValInput )
        print(' set_argOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_argOfPerigee( )
        print(' get_argOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 0.25
        ierr = self.app.set_eccentricity( dValInput )
        print(' set_eccentricity: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_eccentricity()
        print(' get_eccentricity: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 25.4
        ierr = self.app.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanAnomaly( )
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57023
        dEndTime = 57024.5
        dTimeStep = 60
        ierr = self.app.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "GEI"
        strUnits = "RE"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strModel = "AP9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDBDir = "../modelData/"
        ierr = self.app.set_modelDBDir( strModelDBDir )
        print (' set_modelDBDir: dbDir=', strModelDBDir, ' => ', ierr)
        assert(ierr != 0)

        strModelDBDir = "../../modelData/"
        ierr = self.app.set_modelDBDir( strModelDBDir )
        print (' set_modelDBDir: dbDir=', strModelDBDir, ' => ', ierr)
        assert(ierr == 0)
        strDir = self.app.get_modelDBDir( )
        print (' get_modelDBDir: returned dir=', strDir)
        assert(strDir == strModelDBDir)

        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == "UNDEF")

        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == "UNDEF")

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 4)

        dValInput = 7.5
        ierr = self.app.set_meanMotion( dValInput )
        print(' set_meanMotion: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanMotion()
        print(' get_meanMotion: => ', dValue)
        assert(dValue == dValInput)

        strFluxType = "1PTDIFF"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 15,
                                 20, 30, 50, 60, 80, 100, 150, 200, 300])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        ierr = self.app.set_fluxMean( )
        print(' set_fluxMean: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_fluxMean( )
        print(' get_fluxMean: => ', fluxMean)
        assert(fluxMean == 1)

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 0)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        strModelDB = "../../modelData/AP9V15_runtime_tables.mat"
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        # first chunk should be default chunk size 960
        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        # new chunk size - will reset data
        iChunkSize = 335
        ierr = self.app.set_chunkSize( iChunkSize )
        print (' set_chunkSize: iChunkSize=', iChunkSize, ' => ', ierr)
        assert(ierr == 0)
        retVal = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', retVal)
        assert(retVal == iChunkSize)

        bFirstRun = True

        # loop while there's still data left
        while iNum > 0:
            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print (' get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))

            if bFirstRun:
                # first returned time is start time
                assert(daTimes[0] == dStartTime)
                assert(iNum == iChunkSize)
                bFirstRun = False
            else:
                assert(iNum <= iChunkSize)

            iNum2, da3FluxData = self.app.flyin_mean( )
            print(' flyin_mean: => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 == iNum)

            if (iNum > 0):
                assert(da3FluxData.shape == (iNum, iNumE, 1))

            iNum2, da3FluxData = self.app.flyin_percentile( 50 )
            print(' flyin_percentile( 50 ): => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

            iNum2, da3FluxData = self.app.flyin_perturbedMean( 1 )
            print(' flyin_perturbedMean( 1 ): => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

            iNum2, da3FluxData = self.app.flyin_monteCarlo( 10 )
            print(' flyin_monteCarlo(10): => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

        # reset data back to beginning
        ierr = self.app.reset_modelData( )
        print (' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print (' get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))

        # first returned time is start time
        assert(daTimes[0] == dStartTime)
        assert(iNum == iChunkSize)
        bFirstRun = False

        iNum2, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
          = self.app.flyin_meanPlus( )
        print(' flyin_meanPlus: => iNum=', iNum2, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == iNum)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da3FluxData.shape == (iNum, iNumE, 1))
        assert(da2PitchAngles.shape == (1, 1))

        # verify pitch angle is 0
        assert(da2PitchAngles[0][0] == 0)

        # reset data back to beginning
        ierr = self.app.reset_modelData( )
        print (' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum2, da2FluxData = self.app.flyin_mean2d( )
        print(' flyin_mean2d: => iNum=', iNum2, ', len(da2FluxData)=', len(da2FluxData))
        assert(iNum2 == iNum)
        assert(da2FluxData.shape == (iNum, iNumE))
        # use slice to remove third dimension and the rest of the data should match
        np.array_equal(da2FluxData, da3FluxData[:, :, 0])

        # reset data back to beginning
        ierr = self.app.reset_modelData()
        print (' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles2, da3Data \
          = self.app.get_modelData( 'flux', 'mean' )
        print(' get_modelData: => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
              ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
              ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum2 == iNum)
        assert(len(daTimes2) == iNum)
        assert(np.array_equal(daTimes2, daTimes))
        assert(len(daCoord12) == iNum)
        assert(np.array_equal(daCoord12, daCoord1))
        assert(len(daCoord22) == iNum)
        assert(np.array_equal(daCoord22, daCoord2))
        assert(len(daCoord32) == iNum)
        assert(np.array_equal(daCoord32, daCoord3))
        assert(da2PitchAngles2.shape == (1, 1))
        assert(da3Data.shape == (iNum, iNumE, 1))

        # verify pitch angles contains all 0's
        assert(da2PitchAngles[0][0] == 0)

        vars = self.app.get_modelData( 'flux', 'mean', -1, "interval" )
        print(' get_modelData: => ', vars[0])
        assert(vars[0] < 0)

        vars = self.app.get_modelData( 'fluence', 'mean' )
        print(' get_modelData: => ', vars[0])
        assert(vars[0] < 0)

        # reset model run: removes data files, but retains parameter settings
        ierr = self.app.reset_modelRun( )
        print (' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        # now errors
        vars = self.app.get_ephemeris( )
        print (' get_ephemeris: => ', vars[0])
        assert(vars[0] < 0)

        dStartTime2, dEndTime2, dTimeStep2 = self.app.get_times( )
        print (' get_times: => dStartTime=', dStartTime2, ', dEndTime=', dEndTime2,
               ', dTimeStep=', dTimeStep2)
        assert(dStartTime2 == dStartTime)
        assert(dEndTime2 == dEndTime)
        assert(dTimeStep2 == dTimeStep)

        strString = self.app.get_fluxType( )
        print(' get_fluxType: => fluxType=', strString)
        assert(strString == strFluxType)

        ## clear orbit parameter settings
        ierr = self.app.reset_orbitParameters( )
        print (' reset_orbitParameters: => ', ierr)
        assert(ierr == 0)

        dStartTime2, dEndTime2, dTimeStep2 = self.app.get_times( )
        print (' get_times: => dStartTime=', dStartTime2, ', dEndTime=', dEndTime2,
               ', dTimeStepSecs=', dTimeStep2)
        assert(dStartTime2 == -1)
        assert(dEndTime2 == -1)
        assert(dTimeStep2 == -1)

        strString = self.app.get_fluxType( )
        print(' get_fluxType: => fluxType=', strString)
        assert(strString == '1PTDIFF')

    def test_run_model2( self ):
        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)

        strProp = "SatEph"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop()
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        dMjd = 57019.5
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dValInput = 30
        ierr = self.app.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 254.1
        ierr = self.app.set_rightAscension( dValInput )
        print(' set_rightAscension: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_rightAscension()
        print(' get_rightAscension: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 45.0
        ierr = self.app.set_argOfPerigee( dValInput )
        print(' set_argOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_argOfPerigee( )
        print(' get_argOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 12000
        ierr = self.app.set_altitudeOfPerigee( dValInput )
        print(' set_altitudeOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfPerigee( )
        print(' get_altitudeOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 20000
        ierr = self.app.set_altitudeOfApogee( dValInput )
        print(' set_altitudeOfApogee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfApogee( )
        print(' get_altitudeOfApogee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 25.4
        ierr = self.app.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanAnomaly()
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57020.0
        dEndTime = 57021.4
        dTimeStep = 60
        ierr = self.app.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "GDZ"
        strUnits = "KM"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strModel = "AE9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AE9V15_runtime_tables.mat"
        ierr = self.app.set_modelDBFile( strModelDB )
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = self.app.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = self.app.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        strFluxType = "INTEGRAL"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.05, 0.1, 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        iPercentile = 50
        ierr = self.app.set_fluxPercentile( iPercentile )
        print(' set_fluxPercentile: percentile=', iPercentile, ' => ', ierr)
        assert(ierr == 0)

        iPercentile = 75
        ierr = self.app.set_fluxPercentile( iPercentile )
        print(' set_fluxPercentile: percentile=', iPercentile, ' => ', ierr)
        assert(ierr == 0)

        iNumPerc = self.app.get_numFluxPercentiles( )
        print(' get_numFluxPercentiles: => ', iNumPerc)
        assert(iNumPerc == 2)

        iNum, iaPercentiles = self.app.get_fluxPercentiles( )
        print(' get_fluxPercentiles: => iNum=', iNum, ', len(iaPercentiles)=', len(iaPercentiles))
        assert(iNum == iNumPerc)
        assert(len(iaPercentiles) == iNumPerc)
        assert(np.array_equal(iaPercentiles, [50, 75]))

        ierr = self.app.set_fluence( )
        print(' set_fluence: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluence = self.app.get_fluence( )
        print(' get_fluence: => ', fluence)
        assert(fluence == 1)

        strString = "Intv"
        ierr = self.app.set_accumMode( strString )
        print (' set_accumMode: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
        strAccum = self.app.get_accumMode( )
        print (' get_accumMode: => ', strAccum)
        assert(strAccum == strString)
        strString = "Boxcar"
        ierr = self.app.set_accumMode( strString )
        print (' set_accumMode: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
        iNumAccum = self.app.get_numAccumModes( )
        print (' get_numAccumModes: => ', iNumAccum)
        assert(iNumAccum == 2)
        strAccum = self.app.get_accumMode( )
        print (' get_accumMode: => ', strAccum)
        assert(strAccum == "Intv")
        strAccum = self.app.get_accumModeEntry( 2 )
        print (' get_accumModeEntry(2): => ', strAccum)
        assert(strAccum == strString)
        iIncremSec = 0
        ierr = self.app.set_accumIncrementSec(iIncremSec)
        print (' set_accumIncrementSec(',iIncremSec,') => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_accumIncrementSec()
        print (' get_accumIncrementSec: => ', dValue)

        dIntervalSec = 1800
        ierr = self.app.set_accumIntervalSec(dIntervalSec)
        print (' set_accumIntervalSec(',dIntervalSec,') => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_accumIntervalSec()
        print (' get_accumIntervalSec: => ', dValue)
        assert(dValue == dIntervalSec)

        daPitchAngles = np.array([90.0, 85.0, 80.0])
        ierr = self.app.set_pitchAngles( daPitchAngles )
        print (' set_pitchAngles: daPitchAngles=', daPitchAngles, ' => ', ierr)
        assert(ierr == 0)
        iNumP = self.app.get_numPitchAngles( )
        print (' get_numPitchAngles: => ', iNumP)
        assert(iNumP == len(daPitchAngles))

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 0)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        iNum, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum < 0)

        iPercentile = 85
        iNum, da3FluxData = self.app.flyin_percentile( iPercentile )
        print(' flyin_percentile(', iPercentile, '): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum < 0)

        iPercentile = 50
        iNum, da3FluxData = self.app.flyin_percentile( iPercentile )
        print(' flyin_percentile(', iPercentile, '): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == iChunkSize)
        assert(da3FluxData.shape == (iNum, iNumE, iNumP))

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, da3FluxData \
          = self.app.flyin_percentilePlus( 50 )
        print(' flyin_percentilePlus( 50 ): => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
              ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
              ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == iNum)
        assert(len(daTimes2) == iNum)
        assert(np.array_equal(daTimes2, daTimes))
        assert(len(daCoord12) == iNum)
        assert(np.array_equal(daCoord12, daCoord1))
        assert(len(daCoord22) == iNum)
        assert(np.array_equal(daCoord22, daCoord2))
        assert(len(daCoord32) == iNum)
        assert(np.array_equal(daCoord32, daCoord3))
        assert(da3FluxData.shape == (iNum, iNumE, iNumP))
        assert(da2PitchAngles.shape == (iNum, iNumP))

        # verify array contains pitch angles
        assert((da2PitchAngles == daPitchAngles).all())

        # number of intervals = time in sec / interval
        numAccum = math.ceil((dEndTime - dStartTime) * 86400 / dIntervalSec)

        iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, da3FluxData \
          = self.app.flyin_percentilePlus( 50, "intv" )
        print(' flyin_percentilePlus(50, "intv"): => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
              ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
              ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == numAccum)
        assert(len(daTimes2) == numAccum)
        assert(len(daCoord12) == numAccum)
        assert(len(daCoord22) == numAccum)
        assert(len(daCoord32) == numAccum)
        assert(da2PitchAngles.shape == (numAccum, iNumP))
        assert(da3FluxData.shape == (numAccum, iNumE, iNumP))

        for ii in range(numAccum):
            # last value will be end time
            if ii == numAccum - 1:
                assert(daTimes2[ii] == dEndTime)
            else:
                # first time is dStartTime + interval, rest are interval apart
                assert(daTimes2[ii] == (dStartTime * 86400 + dIntervalSec * (ii + 1)) / 86400)

        # verify array contains pitch angles
        assert((da2PitchAngles == daPitchAngles).all())

        # verify all coords are 0
        assert((daCoord12 == 0.0).all())
        assert((daCoord22 == 0.0).all())
        assert((daCoord32 == 0.0).all())

        iNum3, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles, da3Data \
          = self.app.get_modelData( 'fluence', 'percentile', 50 )
        print(' get_modelData(\'fluence\', \'percentile\', 50): => iNum=', iNum3,
              ', len(daTimes)=', len(daTimes3), ', len(daCoord1)=', len(daCoord13),
              ', len(daCoord2)=', len(daCoord23), ', len(daCoord3)=', len(daCoord33),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da3Data.shape=', da3Data.shape)
        assert(iNum3 == numAccum)
        assert(len(daTimes3) == numAccum)
        assert(len(daCoord13) == numAccum)
        assert(len(daCoord23) == numAccum)
        assert(len(daCoord33) == numAccum)
        assert(da2PitchAngles.shape == (numAccum, iNumP))
        assert(da3Data.shape == (numAccum, iNumE, iNumP))

        for ii in range(numAccum):
            # last value will be end time
            if ii == numAccum - 1:
                assert(daTimes3[ii] == dEndTime)
            else:
                # first time is dStartTime + interval, rest are 1800s apart
                assert(daTimes3[ii] == (dStartTime * 86400 + dIntervalSec * (ii + 1)) / 86400)

        # verify array contains pitch angles
        assert((da2PitchAngles == daPitchAngles).all())

        # verify all coords are 0
        assert((daCoord13 == 0.0).all())
        assert((daCoord23 == 0.0).all())
        assert((daCoord33 == 0.0).all())

        # verify fluence values are increasing over intervals
        for ii in range(numAccum - 1):
            for jj in range(iNumE):
                for kk in range(iNumP):
                    assert(da3Data[ii][jj][kk] <= da3Data[ii + 1][jj][kk])

        # error returned
        vars = self.app.get_modelData( 'flux', 'mean', -1, "interval" )
        print(' get_modelData(\'flux\', \'mean\', -1, \'interval\'): => iNum=', vars[0])
        assert(vars[0] < 0)

        # error returned
        vars = self.app.get_modelData( 'flux', 'percentile', 85 )
        print(' get_modelData(\'flux\', \'percentile\', 85): => iNum=', vars[0])
        assert(vars[0] < 0)

        iNum4, daTimes4, daCoord14, daCoord24, daCoord34, da2PitchAngles, da3Data \
          = self.app.get_modelData( 'flux', 'percentile', 75, "interval" )
        print(' get_modelData(\'fluence\', \'percentile\', 75, \'interval\'): => iNum=', iNum4,
              ', len(daTimes)=', len(daTimes4), ', len(daCoord1)=', len(daCoord14),
              ', len(daCoord2)=', len(daCoord24), ', len(daCoord3)=', len(daCoord34),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da3Data.shape=', da3Data.shape)
        assert(iNum4 == numAccum)
        assert(len(daTimes4) == numAccum)
        assert(len(daCoord14) == numAccum)
        assert(len(daCoord24) == numAccum)
        assert(len(daCoord34) == numAccum)
        assert(da2PitchAngles.shape == (numAccum, iNumP))
        assert(da3Data.shape == (numAccum, iNumE, iNumP))

        for ii in range(numAccum):
            # last value will be end time
            if ii == numAccum - 1:
                assert(daTimes4[ii] == dEndTime)
            else:
                # first time is dStartTime + interval, rest are 1800s apart
                assert(daTimes4[ii] == (dStartTime * 86400 + dIntervalSec * (ii + 1)) / 86400)

        # verify array contains pitch angles
        assert((da2PitchAngles == daPitchAngles).all())

        # verify all coords are 0
        assert((daCoord14 == 0.0).all())
        assert((daCoord24 == 0.0).all())
        assert((daCoord34 == 0.0).all())

        # verify flux average values are varying over time
        for ii in range(numAccum - 1):
            for jj in range(iNumE):
                for kk in range(iNumP):
                    d1 = da3Data[ii][jj][kk]
                    d2 = da3Data[ii + 1][jj][kk]
                    # ignore zero values
                    if (d1 != 0 and d2 != 0):
                        assert(d1 != d2)

        iNum5, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
          = self.app.flyin_percentilePlus( 75 )
        print(' flyin_percentilePlus(75): => iNum=', iNum2, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum5 == iNum)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da3FluxData.shape == (iNum, iNumE, iNumP))
        assert(da2PitchAngles.shape == (iNum, iNumP))
        assert(daTimes[0] == dStartTime)

        iNum6, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
          = self.app.flyin_percentilePlus( 50 )
        print(' flyin_percentilePlus( 50 ): => iNum=', iNum2, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)

        # manually calculating length of last bucket - not totally required but added for completeness
        numTimes = math.ceil((dEndTime * 86400 - dStartTime * 86400) / dTimeStep) + 1
        numBuckets = math.ceil(numTimes / iChunkSize)
        lastLength = (dEndTime * 86400 - (dStartTime * 86400 + (numBuckets - 1) * iChunkSize * dTimeStep)) / dTimeStep + 1

        assert(iNum6 == lastLength)
        assert(len(daTimes) == lastLength)
        assert(len(daCoord1) == lastLength)
        assert(len(daCoord2) == lastLength)
        assert(len(daCoord3) == lastLength)
        assert(da3FluxData.shape == (lastLength, iNumE, iNumP))
        assert(da2PitchAngles.shape == (lastLength, iNumP))

        # last time will be the end time
        assert(daTimes[len(daTimes) - 1] == dEndTime)

        # no more data here
        iNum, da3FluxData = self.app.flyin_percentile( 50 )
        print(' flyin_percentile( 50 ): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == 0)
        assert(da3FluxData.shape == (1, 1, 1))

        # still data left
        iNum, da3FluxData = self.app.flyin_percentile( 75 )
        print(' flyin_percentile(75): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == iChunkSize)
        assert(da3FluxData.shape == (iNum, iNumE, iNumP))

        # reset data back to beginning
        ierr = self.app.reset_modelData( )
        print(' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75 )
        print(' flyin_percentilePlus(75): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        print(' first entry: time=',daTimes[0], ' data=',da3FluxData[0,0,0])
        print('  last entry: time=',daTimes[iNum-1], ' data=',da3FluxData[iNum-1,0,0])

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75, "intv" )
        print(' flyin_percentilePlus(75,"intv"): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        print(' first entry: time=',daTimes[0], ' data=',da3FluxData[0,0,0])
        print('  last entry: time=',daTimes[iNum-1], ' data=',da3FluxData[iNum-1,0,0])

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75, "boxcar" )
        print(' flyin_percentilePlus(75,"boxcar"): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        print(' first entry: time=',daTimes[0], ' data=',da3FluxData[0,0,0])
        print('  last entry: time=',daTimes[iNum-1], ' data=',da3FluxData[iNum-1,0,0])

        # ask for second (non-existent) interval results
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75, "boxcar", 2 )
        print(' flyin_percentilePlus(75,"boxcar",2): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert ( iNum<0 )

        # ask for non-existent accum results
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75, "expon" )
        print(' flyin_percentilePlus(75,"expon"): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert ( iNum<0 )

        # another error returned, due to invalid combination of specifications
        vars = self.app.get_modelData( 'fluence', 'percentile', 75, "boxcar" )
        print(' get_modelData(\'fluence\', \'percentile\', 75, \'boxcar\'): => iNum=', vars[0])
        assert(vars[0] < 0)

        # reset run: remove data files and clear all parameter settings
        ierr = self.app.reset_modelRun( 1, 1 )
        print(' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        # error because model run has been reset
        vars = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', vars[0])
        assert(vars[0] < 0)

        # all are -1
        dStartTime, dEndTime, dTimeStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStartTime, dEndTime, dTimeStep))
        assert(dStartTime == -1)
        assert(dEndTime == -1)
        assert(dTimeStep == -1)

        strFluxType = self.app.get_fluxType( )
        print ('  get_fluxType: => ', strFluxType)
        assert(strFluxType == 'none')

    def test_run_model2a( self ):
        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)

        strProp = "Kepler"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop()
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        dMjd = 57019.5
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dValInput = 45
        ierr = self.app.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 154.1
        ierr = self.app.set_rightAscension( dValInput )
        print(' set_rightAscension: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_rightAscension()
        print(' get_rightAscension: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 85.0
        ierr = self.app.set_argOfPerigee( dValInput )
        print(' set_argOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_argOfPerigee( )
        print(' get_argOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 800
        ierr = self.app.set_altitudeOfPerigee( dValInput )
        print(' set_altitudeOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfPerigee( )
        print(' get_altitudeOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 20000
        ierr = self.app.set_altitudeOfApogee( dValInput )
        print(' set_altitudeOfApogee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfApogee( )
        print(' get_altitudeOfApogee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 5.1
        ierr = self.app.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanAnomaly( )
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57020.0
        dEndTime = 57022.5
        dMinStep = 60
        dMaxStep = 1200
        dRoundStep = 5 # default value; not specified in call
        ierr = self.app.set_varTimes( dStartTime, dEndTime, dMinStep, dMaxStep )
        print(' set_varTimes: start=%.1f, end=%.1f, min/max steps=%.0f/%.0f => %i' % (dStartTime, dEndTime, dMinStep, dMaxStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == -1)  # expected non-value
        dStart, dEnd, dVarMinStep, dVarMaxStep, dVarRound = self.app.get_varTimes( )
        print(' get_varTimes: => start=%.1f, end=%.1f, min/max step=%.0f/%.0f, round=%.0f' % (dStart, dEnd, dVarMinStep, dVarMaxStep, dVarRound))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dVarMinStep==dMinStep)
        assert(dVarMaxStep==dMaxStep)
        assert(dVarRound==dRoundStep)

        strSys = "GDZ"
        strUnits = "KM"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strModel = "AE9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AE9V15_runtime_tables.mat"
        ierr = self.app.set_modelDBFile( strModelDB )
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = self.app.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = self.app.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        strFluxType = "INTEGRAL"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.05, 0.1, 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        iPercentile = 50
        ierr = self.app.set_fluxPercentile( iPercentile )
        print(' set_fluxPercentile: percentile=', iPercentile, ' => ', ierr)
        assert(ierr == 0)

        iPercentile = 75
        ierr = self.app.set_fluxPercentile( iPercentile )
        print(' set_fluxPercentile: percentile=', iPercentile, ' => ', ierr)
        assert(ierr == 0)

        iNumPerc = self.app.get_numFluxPercentiles( )
        print(' get_numFluxPercentiles: => ', iNumPerc)
        assert(iNumPerc == 2)

        iNum, iaPercentiles = self.app.get_fluxPercentiles( )
        print(' get_fluxPercentiles: => iNum=', iNum, ', len(iaPercentiles)=', len(iaPercentiles))
        assert(iNum == iNumPerc)
        assert(len(iaPercentiles) == iNumPerc)
        assert(np.array_equal(iaPercentiles, [50, 75]))

        strString = "Boxcar"
        ierr = self.app.set_accumMode( strString )
        print (' set_accumMode: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
        strAccum = self.app.get_accumMode( )
        print (' get_accumMode: => ', strAccum)
        assert(strAccum == strString)
        iNumAccum = self.app.get_numAccumModes( )
        print (' get_numAccumModes: => ', iNumAccum)
        assert(iNumAccum == 1)
        strAccum = self.app.get_accumModeEntry( 1 )
        print (' get_accumModeEntry( 1 ): => ', strAccum)
        assert(strAccum == strString)
        incremSec = 0
        ierr = self.app.set_accumIncrementSec( incremSec )
        print (' set_accumIncrementSec(',incremSec,') => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_accumIncrementSec( )
        print (' get_accumIncrementSec: => ', dValue)

        dIntervalSec = 1800
        ierr = self.app.set_accumIntervalSec( dIntervalSec )
        print (' set_accumIntervalSec(',dIntervalSec,') => ', ierr)
        assert(ierr == 0)
        dIntervalSec = 1200  # smaller than first one specified
        ierr = self.app.set_accumIntervalSec( dIntervalSec )
        print (' set_accumIntervalSec(',dIntervalSec,') => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_accumIntervalSec( )
        print (' get_accumIntervalSec: => ', dValue)
        assert(dValue == dIntervalSec) # 900
        iCount = self.app.get_numAccumIntervals( )
        print (' get_numAccumIntervals: => ', iCount)
        assert(iCount == 2)
        # verify list of intervals is maintained in ascending order
        dValue2 = self.app.get_accumIntervalSecEntry( 2 )
        print (' get_accumIntervalSecEntry(2): => ', dValue2)
        assert(dValue2 > dValue) # 1800 > 1200

        # no pitch angles defined -> omnidirectional flux to be returned
        iNumP = self.app.get_numPitchAngles( )
        print (' get_numPitchAngles: => ', iNumP)
        assert(iNumP == 0)
        iNumP = 1 # for return data dimension size (omnidirectional)

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 0)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        # no flux 'mean'
        iNum, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum < 0)

        # no flux at 85%
        iPercentile = 85
        iNum, da3FluxData = self.app.flyin_percentile( iPercentile )
        print(' flyin_percentile(', iPercentile, '): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum < 0)

        # first chunk of flux at 50%
        iPercentile = 50
        iNum, da3FluxData = self.app.flyin_percentile( iPercentile )
        print(' flyin_percentile(', iPercentile, '): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == iChunkSize)
        assert(da3FluxData.shape == (iNum, iNumE, iNumP))

        # get first chunk of ephemeris data
        iNum0, daTimes0, daCoord10, daCoord20, daCoord30 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum0=', iNum0, '; len(daTimes0)=', len(daTimes0), ', len(daCoord10)=', len(daCoord10))
        assert(iNum0 == iChunkSize)
        assert(len(daTimes0) == iNum0)
        assert(len(daCoord10) == iNum0)
        assert(len(daCoord20) == iNum0)
        assert(len(daCoord30) == iNum0)

        # first returned time is start time
        assert(daTimes0[0] == dStartTime)

        # get second chunk of ephemeris data
        iNum2, daTimes2, daCoord12, daCoord22, daCoord32 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum2, '; len(daTimes2)=', len(daTimes2), ', len(daCoord1)=', len(daCoord12))
        assert(iNum2 > 0)
        assert(len(daTimes2) == iNum2)
        assert(len(daCoord12) == iNum2)
        assert(len(daCoord22) == iNum2)
        assert(len(daCoord32) == iNum2)

        # first returned time is NOT start time
        assert(daTimes2[0] != dStartTime)

        # get second chunk of flux at 50%, but with extra info this time
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
          = self.app.flyin_percentilePlus( 50 )
        print(' flyin_percentilePlus( 50 ): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == iNum2)
        assert(len(daTimes) == iNum2)
        assert(np.array_equal(daTimes, daTimes2))
        assert(len(daCoord1) == iNum2)
        assert(np.array_equal(daCoord1, daCoord12))
        assert(len(daCoord2) == iNum2)
        assert(np.array_equal(daCoord2, daCoord22))
        assert(len(daCoord3) == iNum2)
        assert(np.array_equal(daCoord3, daCoord32))
        assert(da3FluxData.shape == (iNum, iNumE, iNumP))

        # number of intervals is unpredictable, due to variable timesteps

        iNum1, daTimes1, daCoord11, daCoord21, daCoord31, da2PitchAngles, da3FluxData \
          = self.app.flyin_percentilePlus( 50, "intv" )
        print(' flyin_percentilePlus(50, "intv"): => iNum=', iNum1, ', len(daTimes)=', len(daTimes1),
              ', len(daCoord1)=', len(daCoord11), ', len(daCoord2)=', len(daCoord21),
              ', len(daCoord3)=', len(daCoord31), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum1 < 0)

        iNum1, daTimes1, daCoord11, daCoord21, daCoord31, da2PitchAngles, da3FluxData \
          = self.app.flyin_percentilePlus( 50, "boxcar" )
        print(' flyin_percentilePlus(50, "boxcar"): => iNum=', iNum1, ', len(daTimes)=', len(daTimes1),
              ', len(daCoord1)=', len(daCoord11), ', len(daCoord2)=', len(daCoord21),
              ', len(daCoord3)=', len(daCoord31), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum1 > 0)  # may or may not be full chunk size
        assert(len(daTimes1) > 0)
        assert(len(daCoord11) == len(daCoord21))

        # verify all coords are 0 (because it's an accumulation)
        assert((daCoord11 == 0.0).all())
        assert((daCoord21 == 0.0).all())
        assert((daCoord31 == 0.0).all())

        iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, da3FluxData \
          = self.app.flyin_percentilePlus( 50, "boxcar", 2 )
        print(' flyin_percentilePlus(50, "boxcar",2): => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
              ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
              ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 > 0)  # may or may not be full chunk size
        assert(len(daTimes2) > 0) # will start at a later time
        assert(daTimes2[0] > daTimes1[0])
        print( ' boxcar 1 vs 2 interval: first time difference = ',(daTimes2[0]-daTimes1[0])*86400.0)
        assert(len(daCoord12) == len(daCoord22))

        # verify all coords are 0
        assert((daCoord12 == 0.0).all())
        assert((daCoord22 == 0.0).all())
        assert((daCoord32 == 0.0).all())

        # no fluence data available
        iNum3, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles, da3Data \
          = self.app.get_modelData( 'fluence', 'percentile', 50, 'boxcar', 2 )
        print(' get_modelData(\'fluence\', \'percentile\', 50, \'boxcar\', 2): => iNum=', iNum3,
              ', len(daTimes)=', len(daTimes3), ', len(daCoord1)=', len(daCoord13),
              ', len(daCoord2)=', len(daCoord23), ', len(daCoord3)=', len(daCoord33),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da3Data.shape=', da3Data.shape)
        assert(iNum3 < 0)

        iNum3, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles, da3Data \
          = self.app.get_modelData( 'fluence', 'percentile', 50 )
        print(' get_modelData(\'fluence\', \'percentile\', 50): => iNum=', iNum3,
              ', len(daTimes)=', len(daTimes3), ', len(daCoord1)=', len(daCoord13),
              ', len(daCoord2)=', len(daCoord23), ', len(daCoord3)=', len(daCoord33),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da3Data.shape=', da3Data.shape)
        assert(iNum3 < 0)

        # error returned
        vars = self.app.get_modelData( 'flux', 'mean', -1, "interval" )
        print(' get_modelData(\'flux\', \'mean\', -1, \'interval\'): => iNum=', vars[0])
        assert(vars[0] < 0)

        # error returned
        vars = self.app.get_modelData( 'flux', 'percentile', 85 )
        print(' get_modelData(\'flux\', \'percentile\', 85): => iNum=', vars[0])
        assert(vars[0] < 0)

        iNum4, daTimes4, daCoord14, daCoord24, daCoord34, da2PitchAngles, da3Data \
          = self.app.get_modelData( 'flux', 'percentile', 75, "boxcar" )
        print(' get_modelData(\'fluence\', \'percentile\', 75, \'boxcar\'): => iNum=', iNum4,
              ', len(daTimes)=', len(daTimes4), ', len(daCoord1)=', len(daCoord14),
              ', len(daCoord2)=', len(daCoord24), ', len(daCoord3)=', len(daCoord34),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da3Data.shape=', da3Data.shape)
        assert(iNum4 > 0)
        assert(da3Data.shape == (iNum4, iNumE, iNumP))

        # verify all coords are 0
        assert((daCoord14 == 0.0).all())
        assert((daCoord24 == 0.0).all())
        assert((daCoord34 == 0.0).all())

        iNum5, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
          = self.app.flyin_percentilePlus( 75 )
        print(' flyin_percentilePlus(75): => iNum=', iNum2, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum5 == iNum0)
        assert(len(daTimes) == iNum0)
        assert(len(daCoord1) == iNum0)
        assert(len(daCoord2) == iNum0)
        assert(len(daCoord3) == iNum0)
        assert(da3FluxData.shape == (iNum0, iNumE, iNumP))
        assert(daTimes[0] == dStartTime)

        # reset data back to beginning
        ierr = self.app.reset_modelData()
        print(' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75 )
        print(' flyin_percentilePlus(75): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        print(' first entry: time=',daTimes[0], ' data=',da3FluxData[0,0,0])
        print('  last entry: time=',daTimes[iNum-1], ' data=',da3FluxData[iNum-1,0,0])

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75, "boxcar", 2 )
        print(' flyin_percentilePlus(75,"boxcar",2): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        print(' first entry: time=',daTimes[0], ' data=',da3FluxData[0,0,0])
        print('  last entry: time=',daTimes[iNum-1], ' data=',da3FluxData[iNum-1,0,0])

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75, "boxcar", 1 )
        print(' flyin_percentilePlus(75,"boxcar",1): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        print(' first entry: time=',daTimes[0], ' data=',da3FluxData[0,0,0])
        print('  last entry: time=',daTimes[iNum-1], ' data=',da3FluxData[iNum-1,0,0])

        # ask for second (non-existent) interval results
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75, "boxcar", 3 )
        print(' flyin_percentilePlus(75,"boxcar",3): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert ( iNum<0 )

        # ask for non-existent accum results
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData = self.app.flyin_percentilePlus( 75, "expon" )
        print(' flyin_percentilePlus(75,"expon"): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert ( iNum<0 )

        # another error returned, due to invalid combination of specifications
        vars = self.app.get_modelData( 'fluence', 'percentile', 75, "boxcar" )
        print(' get_modelData(\'fluence\', \'percentile\', 75, \'boxcar\'): => iNum=', vars[0])
        assert(vars[0] < 0)

        # reset run: remove data files and clear all parameter settings
        ierr = self.app.reset_modelRun( 1, 1 )
        print(' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        # error because model run has been reset
        vars = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', vars[0])
        assert(vars[0] < 0)

        # all are -1
        dStartTime, dEndTime, dTimeStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStartTime, dEndTime, dTimeStep))
        assert(dStartTime == -1)
        assert(dEndTime == -1)
        assert(dTimeStep == -1)

        strFluxType = self.app.get_fluxType( )
        print ('  get_fluxType: => ', strFluxType)
        assert(strFluxType == 'none')

    def test_run_model3( self ):
        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)

        strProp = "Kepler"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop()
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        dMjd = 55144.21875
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dPosX = -6820.00413015
        dPosY = -12742.645606
        dPosZ = -5469.14602653
        ierr = self.app.set_positionGei( dPosX, dPosY, dPosZ )
        print(' set_positionGei: dValue=', dPosX, dPosY, dPosZ, ' => ', ierr)
        assert(ierr == 0)

        dVelX = 4.20963161074
        dVelY = -1.46626588258
        dVelZ = 1.19624038303
        ierr = self.app.set_velocityGei( dVelX, dVelY, dVelZ )
        print(' set_velocityGei: dValue=', dVelX, dVelY, dVelZ, ' => ', ierr)
        assert(ierr == 0)

        daPos = self.app.get_positionGei( )
        daVel = self.app.get_velocityGei( )

        assert(daPos[0] == dPosX)
        assert(daPos[1] == dPosY)
        assert(daPos[2] == dPosZ)
        assert(daVel[0] == dVelX)
        assert(daVel[1] == dVelY)
        assert(daVel[2] == dVelZ)

        dStartTime = 55144.25
        dEndTime = 55145.75
        dTimeStep = 60
        ierr = self.app.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "GDZ"
        strUnits = "KM"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strModel = "AP9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AP9V15_runtime_tables.mat"
        ierr = self.app.set_modelDBFile(strModelDB)
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = self.app.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = self.app.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        strFluxType = "1PTDIFF"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 15, 20, 30, 50, 60, 80, 100])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        ierr = self.app.set_fluxPerturbedScenRange( 10, 50 )
        print (' set_fluxPerturbedScenRange: 10-50 => ', ierr)
        assert(ierr == 0)

        iScenario = 15
        ierr = self.app.set_fluxPerturbedScenario( iScenario )
        print (' set_fluxPerturbedScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr < 0)

        iScenario = 55
        ierr = self.app.set_fluxPerturbedScenario( iScenario )
        print (' set_fluxPerturbedScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr == 0)

        iNumPS = self.app.get_numFluxPerturbedScenarios( )
        print (' get_numFluxPerturbedScenarios: => ', iNumPS)
        assert(iNumPS == 42)

        iNum, iaScenarios = self.app.get_fluxPerturbedScenarios( )
        print (' get_fluxPerturbedScenarios: => ', iaScenarios.shape, ', iNum=', iNum)
        assert(iNum == iNumPS)
        assert(iaScenarios.shape == (iNumPS,))

        # confirm iaScenarios[0::40] are 10-50 (duplicate 15 is ignored)
        for ii in range(10, 51):
            assert(iaScenarios[ii - 10] == ii)
        # confirm last value is 55
        assert(iaScenarios[41] == 55)

        iaPercent = np.array([50, 75, 85, 95, 100]).astype(np.int32)
        iNumConf = len(iaPercent)
        ierr = self.app.set_aggregConfLevels( iaPercent )
        print (' set_aggregConfLevels: iaPercent=', iaPercent, ' => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numAggregConfLevels( )
        print (' get_numAggregConfLevels: => ', iNum)
        assert(iNum == iNumConf)

        ierr = self.app.set_delBinDir( 0 )
        print (' set_delBinDir( 0 ): => ', ierr)
        assert(ierr == 0)
        delBinDir = self.app.get_delBinDir( )
        print (' get_delBinDir( ): => ', delBinDir)
        assert(delBinDir == 0)

        strWorkDir = "work/"
        ierr = self.app.set_workDir( strWorkDir )
        print (' set_workDir: strWorkDir=', strWorkDir, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_workDir( )
        print (' get_workDir( ): => ', strString)
        assert(strString == strWorkDir)

        strBinDirName = "pm_agg"
        ierr = self.app.set_binDirName( strBinDirName )
        print (' set_binDirName: stBinDirName=', strBinDirName, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_binDirName( )
        print (' get_binDirName( ): => ', strString)
        assert(strString == strBinDirName)
        self.bWorkDir_pm_agg = True

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 0)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        iNum, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum < 0)

        iPercentile = 85
        iNum, da3FluxData = self.app.flyin_percentile( iPercentile )
        print(' flyin_percentile(', iPercentile, '): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum < 0)

        iPercent = 52
        iNum, da3FluxData = self.app.flyin_perturbedMean( iPercent )
        print(' flyin_perturbedMean(', iPercent, '): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum < 0)

        iPercent = 35
        iNum, da3FluxData = self.app.flyin_perturbedMean( iPercent )
        print(' flyin_perturbedMean(', iPercent, '): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == iChunkSize)
        assert(da3FluxData.shape == (iNum, iNumE, 1))

        iPercent = 15
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
          = self.app.flyin_perturbedMeanPlus( iPercent )
        print(' flyin_perturbedMeanPlus(', iPercent, '): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da2PitchAngles.shape == (1, 1))
        assert(da3FluxData.shape == (iNum, iNumE, 1))

        # verify pitch angle is 0
        assert(da2PitchAngles[0][0] == 0)

        vars = self.app.get_aggregData( 'flux', 'perturbed', 99 )
        print(' get_aggregData(\'flux\', \'perturbed\', 99): => ', vars[0])
        assert(vars[0] < 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data \
          = self.app.get_aggregData( 'flux', 'perturbed', 95 )
        print(' get_modelData(\'flux\', \'perturbed\', 95): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da2PitchAngles.shape == (1, 1))
        assert(da3Data.shape == (iNum, iNumE, 1))

        # pitch angle should be 0
        assert(da2PitchAngles[0][0] == 0)

        vars = self.app.get_aggregData('flux', 'perturbed', 95, "interval")
        print(' get_aggregData(\'flux\', \'perturbed\', 95, "interval"): => ', vars[0])
        assert(vars[0] < 0)

    def test_run_model4( self ):
        # reset parameter settings, but retain prev run data files
        ierr = self.app.reset_modelRun( 0, 1 )
        print(' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        ierr = self.app.set_delBinDir( )
        print (' set_delBinDir( ): => ', ierr)
        assert(ierr == 0)
        bDelBinDir = self.app.get_delBinDir( )
        print (' get_delBinDir( ): => ', bDelBinDir)
        assert(bDelBinDir == 1)
        self.bWorkDir = False

        # use ephem model to generate test inputs
        ephem = EphemModel()

        print ('ephem eHandle=', ephem.zHandle)
        strMagfieldDB = '../../modelData/igrfDB.h5'
        ierr = ephem.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)

        dValue = 25.0
        ierr = ephem.set_inclination( dValue )
        print (' set_inclination: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)
        dValue = 0.25
        ierr = ephem.set_eccentricity( dValue )
        print (' set_eccentricity: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)
        dValue = 8.5
        ierr = ephem.set_meanMotion( dValue )
        print (' set_meanMotion: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)
        dValue = 218.5
        ierr = ephem.set_meanAnomaly( dValue )
        print (' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)
        dValue = 125.0
        ierr = ephem.set_rightAscension( dValue )
        print (' set_rightAscension: dValue=', dValue, ' => ', ierr)
        assert(ierr == 0)
        dMjd = 58119.5
        ierr = ephem.set_elementTime( dMjd )
        print (' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        dStep = 60.0
        dStart = 58120.0
        dEnd = 58120.25
        ierr = ephem.set_times(dStart, dEnd, dStep)
        print (' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStart, dEnd, dStep, ierr))
        assert(ierr == 0)

        # set to smaller chunksize (prev default)
        iChunkSize = 240
        ephem.set_chunkSize( iChunkSize )

        # get the first chunk of ephemeris
        strCoordSys = 'RLL'
        strCoordUnits = 'Re'
        iNum, daTimes, daCoord1, daCoord2, daCoord3 \
          = ephem.compute(strCoordSys, strCoordUnits)
        print (' compute: CoordSys=', strCoordSys, strCoordUnits)
        print ('  returned iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iChunkSize)
        assert(len(daCoord1) == iChunkSize)
        assert(len(daCoord2) == iChunkSize)
        assert(len(daCoord3) == iChunkSize)

        ierr = self.app.set_inEphemeris(daTimes, daCoord1, daCoord2, daCoord3)
        print(' set_inEphemeris: length=', iChunkSize, ' => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numInEphemeris( )
        print(' get_numInEphemeris: =>', iNum)
        assert(iNum == iChunkSize)

        # second chunk of ephemeris - 121 left (total of 361 times)
        iNum, daTimes, daCoord1, daCoord2, daCoord3 \
          = ephem.compute(strCoordSys, strCoordUnits)
        print (' compute: CoordSys=', strCoordSys, strCoordUnits)
        print ('  returned iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == 121)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        ierr = self.app.set_inEphemeris(daTimes, daCoord1, daCoord2, daCoord3, 1)
        print(' set_inEphemeris: length=', 121, ' => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numInEphemeris( )
        print(' get_numInEphemeris: =>', iNum)
        assert(iNum == 361)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_inEphemeris( )
        print(' get_inEphemeris: => iNum: ', iNum, ', daTimes.shape: ', daTimes.shape,
              ', daCoords1.shape:', daCoord1.shape, ', daCoords2.shape:', daCoord2.shape,
              ', daCoords3.shape:', daCoord3.shape)
        assert(iNum == 361)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        del ephem

        strInSys = "RLL"
        strInUnits = "RE"
        ierr = self.app.set_inCoordSys(strInSys, strInUnits)
        print (' set_inCoordSys: input={', strInSys, ',', strInUnits, '} => ', ierr)
        strRetSys = self.app.get_inCoordSys( )
        print(' get_inCoordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_inCoordSysUnits( )
        print(' get_inCoordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strInSys)
        assert(strRetSysUnits == strInUnits)

        strSys = "GDZ"
        strUnits = "KM"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        ierr = self.app.set_fluxMean( )
        print(' set_fluxMean: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_fluxMean( )
        print(' get_fluxMean: => ', fluxMean)
        assert(fluxMean == 1)

        ierr = self.app.set_fluence( )
        print(' set_fluence: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluence = self.app.get_fluence( )
        print(' get_fluence: => ', fluence)
        assert(fluence == 1)

        dIntervalDay = 0.015
        ierr = self.app.set_accumInterval(dIntervalDay)
        print (' set_accumInterval: interval(days)=', dIntervalDay, ' => ', ierr)
        assert(ierr == 0)
        dIntervalSec = self.app.get_accumIntervalSec()
        print (' get_accumIntervalSec: => ', dIntervalSec)
        assert(dIntervalSec == dIntervalDay * 86400)

        ierr = self.app.set_adiabatic()
        print(' set_adiabatic: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_adiabatic()
        print(' get_adiabatic: => ', fluxMean)
        assert(fluxMean == 1)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)
		
        iChunkSize = 240
        self.app.set_chunkSize( iChunkSize )
        print (' changing to smaller chunkSize: ',iChunkSize)
        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 240)

        ierr = self.app.set_fluxMonteCarloScenRange(20, 50)
        print (' set_fluxMonteCarloScenRange: 10-50 => ', ierr)
        assert(ierr == 0)

        iScenario = 10
        ierr = self.app.set_fluxMonteCarloScenario(iScenario)
        print (' set_fluxMonteCarloScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr == 0)

        iScenario = 45
        ierr = self.app.set_fluxMonteCarloScenario(iScenario)
        print (' set_fluxMonteCarloScenario: iScenario=', iScenario, ' => ', ierr)
        assert(ierr < 0)

        iNumPS = self.app.get_numFluxMonteCarloSenarios( )
        print (' get_numFluxMonteCarloSenarios: => ', iNumPS)
        assert(iNumPS == 32)

        iNum, iaScenarios = self.app.get_fluxMonteCarloScenarios( )
        print (' get_fluxMonteCarloScenarios: => ', iaScenarios.shape, ', iNum=', iNum)
        assert(iNum == iNumPS)
        assert(len(iaScenarios) == iNumPS)

        assert(iaScenarios[0] == 10)
        # confirm iaScenarios[1::31] are 20-50 (duplicate 45 is ignored)
        for ii in range(20, 51):
            assert(iaScenarios[ii - 19] == ii)

        strModel = "AE9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AE9V15_runtime_tables.mat"
        ierr = self.app.set_modelDBFile(strModelDB)
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = self.app.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = self.app.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        strFluxType = "INTEGRAL"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.05, 0.1, 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0])
        iNumE = len(daFluxEnergies)
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        iaPercent = np.array([0, 25, 50, 75, 100]).astype(np.int32)
        iNumConf = len(iaPercent)
        ierr = self.app.set_aggregConfLevels( iaPercent )
        print (' set_aggregConfLevels: iaPercent=', iaPercent, ' => ', ierr)
        assert(ierr == 0)

        iNum = self.app.get_numAggregConfLevels( )
        print (' get_numAggregConfLevels: => ', iNum)
        assert(iNum == iNumConf)

        strWorkDir = "work2/"
        ierr = self.app.set_workDir( strWorkDir )
        print (' set_workDir: strWorkDir=', strWorkDir, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_workDir( )
        print (' get_workDir( ): => ', strString)
        assert(strString == strWorkDir)

        strBinDirName = "mc_test"
        ierr = self.app.set_binDirName( strBinDirName )
        print (' set_binDirName: strWorkDir=', strBinDirName, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_binDirName( )
        print (' get_binDirName( ): => ', strString)
        assert(strString == strBinDirName)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iNum, daTimesEphem, daCoord1Ephem, daCoord2Ephem, daCoord3Ephem = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimesEphem),
               ', len(daCoord1)=', len(daCoord1Ephem))
        assert(iNum == iChunkSize)
        assert(len(daTimesEphem) == iNum)
        assert(len(daCoord1Ephem) == iNum)
        assert(len(daCoord2Ephem) == iNum)
        assert(len(daCoord3Ephem) == iNum)

        iNum, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == iChunkSize)
        assert(da3FluxData.shape == (iNum, iNumE, 1))

        iNum, da3FluxData = self.app.flyin_perturbedMean( 10 )
        print(' flyin_perturbedMean(10): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum < 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
          = self.app.flyin_monteCarloPlus( 25 )
        print(' flyin_monteCarloPlus(25): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da3FluxData.shape == (iNum, iNumE, 1))
        assert(da2PitchAngles.shape == (1, 1))

        # verify pitch angle is 0
        assert(da2PitchAngles[0][0] == 0)

        iNum, da3FluxData = self.app.flyin_monteCarlo( 25 )
        print(' flyin_monteCarlo(25): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum == 121)
        assert(da3FluxData.shape == (iNum, iNumE, 1))

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data \
          = self.app.get_modelData( 'flux', 'monteCarlo', 50 )
        print(' get_modelData(\'flux\', \'monteCarlo\', 50): => iNum=', iNum,
              ', len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1),
              ', len(daCoord2)=', len(daCoord2), ', len(daCoord3)=', len(daCoord3),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da3Data.shape=', da3Data.shape)
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(np.array_equal(daTimes, daTimesEphem))
        assert(len(daCoord1) == iNum)
        assert(np.array_equal(daCoord1, daCoord1Ephem))
        assert(len(daCoord2) == iNum)
        assert(np.array_equal(daCoord2, daCoord2Ephem))
        assert(len(daCoord3) == iNum)
        assert(np.array_equal(daCoord3, daCoord3Ephem))
        assert(da2PitchAngles.shape == (1, 1))
        assert(da3Data.shape == (iNum, iNumE, 1))

        # first time is start time
        assert(daTimes[0] == dStart)

        # verify pitch angle is 0
        assert(da2PitchAngles[0][0] == 0)

        # first bucket is chunkSize
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, \
            da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
            daBmin, daBlocal, daMagLT \
          = self.app.get_adiabaticCoordsPlus( )
        print(' get_adiabaticCoordsPlus: => iNum=', iNum,
              ', len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1),
              ', len(daCoord2)=', len(daCoord2), ', len(daCoord3)=', len(daCoord3),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da2Alpha.shape=', da2Alpha.shape,
              ', da2Lm.shape=', da2Lm.shape, ', da2K.shape=', da2K.shape,
              ', da2Phi.shape=', da2Phi.shape, ', da2Hmin.shape=', da2Hmin.shape,
              ', da2Lstar.shape=', da2Lstar.shape, ', daBmin.shape=', daBmin.shape,
              ', daBlocal.shape=', daBlocal.shape, ', daMagLT.shape=', daMagLT.shape)
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(np.array_equal(daTimes, daTimesEphem))
        assert(len(daCoord1) == iNum)
        assert(np.array_equal(daCoord1, daCoord1Ephem))
        assert(len(daCoord2) == iNum)
        assert(np.array_equal(daCoord2, daCoord2Ephem))
        assert(len(daCoord3) == iNum)
        assert(np.array_equal(daCoord3, daCoord3Ephem))
        assert(da2PitchAngles.shape == (iNum, 1))
        assert(da2Alpha.shape == (iNum, 1))
        assert(da2Lm.shape == (iNum, 1))
        assert(da2K.shape == (iNum, 1))
        assert(da2Phi.shape == (iNum, 1))
        assert(da2Hmin.shape == (iNum, 1))
        assert(da2Lstar.shape == (iNum, 1))
        assert(len(daBmin) == iNum)
        assert(len(daBlocal) == iNum)
        assert(len(daMagLT) == iNum)

        # first time is start time
        assert(daTimes[0] == dStart)

        # verify pitch angles are all 90's
        assert((da2PitchAngles == 90.0).all())

        # second bucket is 121
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, \
            da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
            daBmin, daBlocal, daMagLT \
          = self.app.get_adiabaticCoordsPlus( )
        print(' get_adiabaticCoordsPlus: => iNum=', iNum,
              ', len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1),
              ', len(daCoord2)=', len(daCoord2), ', len(daCoord3)=', len(daCoord3),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da2Alpha.shape=', da2Alpha.shape,
              ', da2Lm.shape=', da2Lm.shape, ', da2K.shape=', da2K.shape,
              ', da2Phi.shape=', da2Phi.shape, ', da2Hmin.shape=', da2Hmin.shape,
              ', da2Lstar.shape=', da2Lstar.shape, ', daBmin.shape=', daBmin.shape,
              ', daBlocal.shape=', daBlocal.shape, ', daMagLT.shape=', daMagLT.shape)
        assert(iNum == 121)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da2PitchAngles.shape == (iNum, 1))
        assert(da2Alpha.shape == (iNum, 1))
        assert(da2Lm.shape == (iNum, 1))
        assert(da2K.shape == (iNum, 1))
        assert(da2Phi.shape == (iNum, 1))
        assert(da2Hmin.shape == (iNum, 1))
        assert(da2Lstar.shape == (iNum, 1))
        assert(len(daBmin) == iNum)
        assert(len(daBlocal) == iNum)
        assert(len(daMagLT) == iNum)

        # verify pitch angles are all 90's
        assert((da2PitchAngles == 90.0).all())

        # data is empty
        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, \
            da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
            daBmin, daBlocal, daMagLT \
          = self.app.get_adiabaticCoordsPlus( )
        print(' get_adiabaticCoordsPlus: => iNum=', iNum,
              ', len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1),
              ', len(daCoord2)=', len(daCoord2), ', len(daCoord3)=', len(daCoord3),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da2Alpha.shape=', da2Alpha.shape,
              ', da2Lm.shape=', da2Lm.shape, ', da2K.shape=', da2K.shape,
              ', da2Phi.shape=', da2Phi.shape, ', da2Hmin.shape=', da2Hmin.shape,
              ', da2Lstar.shape=', da2Lstar.shape, ', daBmin.shape=', daBmin.shape,
              ', daBlocal.shape=', daBlocal.shape, ', daMagLT.shape=', daMagLT.shape)
        assert(iNum == 0)
        assert(len(daTimes) == 1)
        assert(len(daCoord1) == 1)
        assert(len(daCoord2) == 1)
        assert(len(daCoord3) == 1)
        assert(da2PitchAngles.shape == (1, 1))
        assert(da2Alpha.shape == (1, 1))
        assert(da2Lm.shape == (1, 1))
        assert(da2K.shape == (1, 1))
        assert(da2Phi.shape == (1, 1))
        assert(da2Hmin.shape == (1, 1))
        assert(da2Lstar.shape == (1, 1))
        assert(len(daBmin) == 1)
        assert(len(daBlocal) == 1)
        assert(len(daMagLT) == 1)

        vars = self.app.get_aggregData('flux', 'perturbed', 99)
        print(' get_aggregData(\'flux\', \'perturbed\', 99): => ', vars[0])
        assert(vars[0] < 0)

        vars = self.app.get_aggregData('flux', 'monteCarlo', 99)
        print(' get_aggregData(\'flux\', \'monteCarlo\', 99): => ', vars[0])
        assert(vars[0] < 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data \
          = self.app.get_aggregData('flux', 'monteCarlo', 75)
        print(' get_modelData(\'flux\', \'monteCarlo\', 75): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(np.array_equal(daTimes, daTimesEphem))
        assert(len(daCoord1) == iNum)
        assert(np.array_equal(daCoord1, daCoord1Ephem))
        assert(len(daCoord2) == iNum)
        assert(np.array_equal(daCoord2, daCoord2Ephem))
        assert(len(daCoord3) == iNum)
        assert(np.array_equal(daCoord3, daCoord3Ephem))
        assert(da2PitchAngles.shape == (1, 1))
        assert(da3Data.shape == (iNum, iNumE, 1))

        # first time is start time
        assert(daTimes[0] == dStart)

        # verify pitch angle is 0
        assert(da2PitchAngles[0][0] == 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data \
          = self.app.get_aggregData('flux', 'monteCarlo', 75, "interval")
        print(' get_modelData(\'flux\', \'monteCarlo\', 75, \'interval\'): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum == 17)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da2PitchAngles.shape == (1, 1))
        assert(da3Data.shape == (iNum, iNumE, 1))

        # verify coord values are all 0's
        assert(np.array_equiv(daCoord1, np.zeros(iNum)))
        assert(np.array_equiv(daCoord2, np.zeros(iNum)))
        assert(np.array_equiv(daCoord3, np.zeros(iNum)))

        for ii in range(iNum):
            # last value will be end time
            if ii == iNum - 1:
                assert(daTimes[ii] == dEnd)
            else:
                # first time is dStart + dIntervalSec, rest are dIntervalSec (~1296) apart
                assert(daTimes[ii] == (dStart * 86400 + dIntervalSec * (ii + 1)) / 86400)

        # verify pitch angle is 0
        assert(da2PitchAngles[0][0] == 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data \
          = self.app.get_aggregData('fluence', 'monteCarlo', 75)
        print(' get_modelData(\'fluence\', \'monteCarlo\', 75): => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum == 17)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da2PitchAngles.shape == (1, 1))
        assert(da3Data.shape == (iNum, iNumE, 1))

        # verify coord values are all 0's
        assert(np.array_equiv(daCoord1, np.zeros(iNum)))
        assert(np.array_equiv(daCoord2, np.zeros(iNum)))
        assert(np.array_equiv(daCoord3, np.zeros(iNum)))

        for ii in range(iNum):
            # last value will be end time
            if ii == iNum - 1:
                assert(daTimes[ii] == dEnd)
            else:
                # first time is dStart + dIntervalSec, rest are dIntervalSec (~1296) apart
                assert(daTimes[ii] == (dStart * 86400 + dIntervalSec * (ii + 1)) / 86400)

        # verify pitch angle is 0
        assert(da2PitchAngles[0][0] == 0)

    def test_run_model5( self ):
        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)
        # reset model run: remove data files, but retain parameter settings
        ierr = self.app.reset_modelRun()
        print(' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        ierr = self.app.set_delBinDir( 0 )
        print (' set_delBinDir( 0 ): => ', ierr)
        assert(ierr == 0)
        delBinDir = self.app.get_delBinDir( )
        print (' get_delBinDir( ): => ', delBinDir)
        assert(delBinDir == 0)
#        self.bWorkDir = True

        strProp = "SGP4"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop()
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        strSgp4Mode = "improved"
        ierr = self.app.set_sgp4Mode( strSgp4Mode )
        print (' set_sgp4Mode: strString=', strSgp4Mode, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_sgp4Mode( )
        print (' get_sgp4Mode: => ', strRet)
        assert(strRet == strSgp4Mode)

        dMjd = 57023.0
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dValInput = 25
        ierr = self.app.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 16.1
        ierr = self.app.set_localTimeOfApogee( dValInput )
        print(' set_localTimeOfApogee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_localTimeOfApogee( )
        print(' get_localTimeOfApogee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 5.4
        ierr = self.app.set_localTimeMaxInclination( dValInput )
        print(' set_localTimeMaxInclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_localTimeMaxInclination()
        print(' get_localTimeMaxInclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 750
        ierr = self.app.set_altitudeOfPerigee( dValInput )
        print(' set_altitudeOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfPerigee( )
        print(' get_altitudeOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 11500
        ierr = self.app.set_altitudeOfApogee( dValInput )
        print(' set_altitudeOfApogee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfApogee( )
        print(' get_altitudeOfApogee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 218.1
        ierr = self.app.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanAnomaly()
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57023
        dEndTime = 57024.5
        dTimeStep = 60
        ierr = self.app.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "SPH"
        strUnits = "KM"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strString = "Cumul"
        ierr = self.app.set_accumMode( strString )
        print (' set_accumMode: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
        strAccum = self.app.get_accumMode( )
        print (' get_accumMode: => ', strAccum)
        assert(strAccum == strString)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        # running model with just orbit specifications - no flux models calculated
        # fails because no magfield database has been specified
        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr < 0)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        # successfully runs
        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        # first chunk should be default chunk size 960
        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        for ii in range(iNum):
            assert(daCoord1[ii] > 7100)
            assert(daCoord2[ii] > 0 and daCoord2[ii] <= 180)

        iNum, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum < 0)

        # reset model run: remove data files, but retain parameter settings
        ierr = self.app.reset_modelRun()
        print(' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        strModel = "AP9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AP9V15_runtime_tables.mat"
        ierr = self.app.set_modelDBFile( strModelDB )
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = self.app.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = self.app.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        strWorkDir = "work3/"
        ierr = self.app.set_workDir( strWorkDir )
        print (' set_workDir: strWorkDir=', strWorkDir, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_workDir( )
        print (' get_workDir( ): => ', strString)
        assert(strString == strWorkDir)

        strBinDirName = "mc_test"
        ierr = self.app.set_binDirName( strBinDirName)
        print (' set_binDirName: strWorkDir=', strBinDirName, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_binDirName( )
        print (' get_binDirName( ): => ', strString)
        assert(strString == strBinDirName)

        strFluxType = "INTEGRAL"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 15,
                                 20, 30, 50, 60, 80])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        ierr = self.app.set_fluxMean( )
        print(' set_fluxMean: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_fluxMean( )
        print(' get_fluxMean: => ', fluxMean)
        assert(fluxMean == 1)

        ierr = self.app.set_adiabatic()
        print(' set_adiabatic: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_adiabatic()
        print(' get_adiabatic: => ', fluxMean)
        assert(fluxMean == 1)

        daPitchAngles = np.array([90.0, 85.0, 80.0, 75.0, 70.0])
        ierr = self.app.set_pitchAngles(daPitchAngles)
        print (' set_pitchAngles: daPitchAngles=', daPitchAngles, ' => ', ierr)
        assert(ierr == 0)
        iNumP = self.app.get_numPitchAngles( )
        print (' get_numPitchAngles: => ', iNumP)
        assert(iNumP == len(daPitchAngles))

        strSys = "SPH"
        strUnits = "RE"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        for ii in range(iNum):
            assert(daCoord1[ii] > 1.1 and daCoord1[ii] < 2.85)
            assert(daCoord2[ii] > 0 and daCoord2[ii] <= 180)

        while iNum > 0:
            iNum2, da3FluxData = self.app.flyin_mean( )
            print(' flyin_mean: => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 == iNum)

            iNum3, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, \
                da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
                daBmin, daBlocal, daMagLT \
              = self.app.get_adiabaticCoordsPlus( )
            print(' get_adiabaticCoordsPlus: => iNum=', iNum3,
                  ', len(daTimes)=', len(daTimes2), ', len(daCoord1)=', len(daCoord12),
                  ', len(daCoord2)=', len(daCoord22), ', len(daCoord3)=', len(daCoord32),
                  ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da2Alpha.shape=', da2Alpha.shape,
                  ', da2Lm.shape=', da2Lm.shape, ', da2K.shape=', da2K.shape,
                  ', da2Phi.shape=', da2Phi.shape, ', da2Hmin.shape=', da2Hmin.shape,
                  ', da2Lstar.shape=', da2Lstar.shape, ', daBmin.shape=', daBmin.shape,
                  ', daBlocal.shape=', daBlocal.shape, ', daMagLT.shape=', daMagLT.shape)
            assert(iNum3 == iNum)

            # ephem results
            assert(len(daTimes) == iNum)
            assert(len(daCoord1) == iNum)
            assert(len(daCoord2) == iNum)
            assert(len(daCoord3) == iNum)
            # mean results
            assert(da3FluxData.shape == (iNum, iNumE, iNumP))
            # adiabatic results
            assert(len(daTimes2) == iNum)
            assert(np.array_equal(daTimes2, daTimes))
            assert(len(daCoord12) == iNum)
            assert(np.array_equal(daCoord12, daCoord1))
            assert(len(daCoord22) == iNum)
            assert(np.array_equal(daCoord22, daCoord2))
            assert(len(daCoord32) == iNum)
            assert(np.array_equal(daCoord32, daCoord3))
            assert(da2PitchAngles.shape == (iNum, iNumP))
            assert(da2Alpha.shape == (iNum, iNumP))
            assert(da2Lm.shape == (iNum, iNumP))
            assert(da2K.shape == (iNum, iNumP))
            assert(da2Phi.shape == (iNum, iNumP))
            assert(da2Hmin.shape == (iNum, iNumP))
            assert(da2Lstar.shape == (iNum, iNumP))
            assert(len(daBmin) == iNum)
            assert(len(daBlocal) == iNum)
            assert(len(daMagLT) == iNum)

            iNum2, da3FluxData = self.app.flyin_percentile( 50 )
            print(' flyin_percentile( 50 ): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

            # get data for next run
            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
            assert(iNum >= 0 and iNum <= iChunkSize)

        iNum2, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == 0)

        iNum3, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, \
            da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
            daBmin, daBlocal, daMagLT \
          = self.app.get_adiabaticCoordsPlus( )
        print(' get_adiabaticCoordsPlus: => iNum=', iNum3,
              ', len(daTimes)=', len(daTimes2), ', len(daCoord1)=', len(daCoord12),
              ', len(daCoord2)=', len(daCoord22), ', len(daCoord3)=', len(daCoord32),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da2Alpha.shape=', da2Alpha.shape,
              ', da2Lm.shape=', da2Lm.shape, ', da2K.shape=', da2K.shape,
              ', da2Phi.shape=', da2Phi.shape, ', da2Hmin.shape=', da2Hmin.shape,
              ', da2Lstar.shape=', da2Lstar.shape, ', daBmin.shape=', daBmin.shape,
              ', daBlocal.shape=', daBlocal.shape, ', daMagLT.shape=', daMagLT.shape)
        assert(iNum3 == 0)

        # no data left
        assert(len(daTimes) == 1)
        assert(len(daCoord1) == 1)
        assert(len(daCoord2) == 1)
        assert(len(daCoord3) == 1)
        # mean results
        assert(da3FluxData.shape == (1, 1, 1))
        # adiabatic results
        assert(len(daTimes2) == 1)
        assert(len(daCoord12) == 1)
        assert(len(daCoord22) == 1)
        assert(len(daCoord32) == 1)
        assert(da2PitchAngles.shape == (1, 1))
        assert(da2Alpha.shape == (1, 1))
        assert(da2Lm.shape == (1, 1))
        assert(da2K.shape == (1, 1))
        assert(da2Phi.shape == (1, 1))
        assert(da2Hmin.shape == (1, 1))
        assert(da2Lstar.shape == (1, 1))
        assert(len(daBmin) == 1)
        assert(len(daBlocal) == 1)
        assert(len(daMagLT) == 1)

        # reset model run: remove data files, but retain parameter settings
        ierr = self.app.reset_modelRun()
        print(' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        ierr = self.app.set_doseRate( )
        print(' set_doseRate: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        bDoseRate = self.app.get_doseRate( )
        print(' get_doseRate: => ', bDoseRate)
        assert(bDoseRate == 1)

        strDoseUnits = "millimeters"
        ierr = self.app.set_doseDepthUnits( strDoseUnits )
        print (' set_doseDepthUnits: strString=', strDoseUnits, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_doseDepthUnits( )
        print (' get_doseDepthUnits: => ', strString)
        assert(strString == strDoseUnits)

        strDoseGeometry = "Spherical2pi"
        ierr = self.app.set_doseGeometry( strDoseGeometry )
        print (' set_doseGeometry: strString=', strDoseGeometry, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_doseGeometry()
        print (' get_doseGeometry: => ', strString)
        assert(strString == strDoseGeometry)

        strDoseDetector = "Silicon"
        ierr = self.app.set_doseDetector( strDoseDetector )
        print (' set_doseDetector: strString=', strDoseDetector, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_doseDetector()
        print (' get_doseDetector: => ', strString)
        assert(strString == strDoseDetector)

        daDepths = np.array([0.125, 0.25, 0.5, 1.0, 2.0, 5.0])
        iNumD = len(daDepths)
        ierr = self.app.set_doseDepthValues( daDepths )
        print (' set_doseDepthValues: daDepths=', daDepths, ' => ', ierr)
        assert(ierr == 0)

        iRetValD = self.app.get_numDoseDepths( )
        print (' get_numDoseDepths: => ', iRetValD)
        assert(iRetValD == iNumD)

        # cannot calculate doses for pitch angles
        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr < 0)

        ierr = self.app.clear_pitchAngles( )
        print(' clear_pitchAngles: => ', ierr)
        assert(ierr == 0)
        iNumP = self.app.get_numPitchAngles( )
        print (' get_numPitchAngles: => ', iNumP)
        assert(iNumP == 0)

        # cannot calculate doses for integral flux
        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr < 0)

        strFluxType = "1PTDIFF"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        # successful run
        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = 330
        ierr = self.app.set_chunkSize( iChunkSize )
        print (' set_chunkSize: iChunkSize=', iChunkSize, ' => ', ierr)
        assert(ierr == 0)
        retVal = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', retVal)
        assert(retVal == iChunkSize)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)

        bFirstRun = True
        # saving values of first shieldDose results to compare later
        iNumShield = daTimesShield = daCoord1Shield = daCoord2Shield = daCoord3Shield = \
            da2PitchAnglesShield = da3DataShield = None

        while iNum > 0:
            iNum2, da3FluxData = self.app.flyin_mean( )
            print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 == iNum)

            iNum3, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, \
                da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
                daBmin, daBlocal, daMagLT \
              = self.app.get_adiabaticCoordsPlus( )
            print(' get_adiabaticCoordsPlus: => iNum=', iNum3,
                  ', len(daTimes)=', len(daTimes2), ', len(daCoord1)=', len(daCoord12),
                  ', len(daCoord2)=', len(daCoord22), ', len(daCoord3)=', len(daCoord32),
                  ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da2Alpha.shape=', da2Alpha.shape,
                  ', da2Lm.shape=', da2Lm.shape, ', da2K.shape=', da2K.shape,
                  ', da2Phi.shape=', da2Phi.shape, ', da2Hmin.shape=', da2Hmin.shape,
                  ', da2Lstar.shape=', da2Lstar.shape, ', daBmin.shape=', daBmin.shape,
                  ', daBlocal.shape=', daBlocal.shape, ', daMagLT.shape=', daMagLT.shape)
            assert(iNum3 == iNum)

            iNum4, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles2, da3Data \
              = self.app.get_modelData( 'doserate', 'mean' )
            print(' get_modelData(\'doserate\', \'mean\'): => iNum=', iNum4, ', len(daTimes)=', len(daTimes3),
                  ', len(daCoord1)=', len(daCoord13), ', len(daCoord2)=', len(daCoord23),
                  ', len(daCoord3)=', len(daCoord33), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
                  ', da3Data.shape=', da3Data.shape)
            assert(iNum4 == iNum)

            if bFirstRun:
                iNumShield = iNum4
                daTimesShield = daTimes3
                daCoord1Shield = daCoord13
                daCoord2Shield = daCoord23
                daCoord3Shield = daCoord33
                da2PitchAnglesShield = da2PitchAngles2
                da3DataShield = da3Data
                bFirstRun = False

            # ephem results
            assert(len(daTimes) == iNum)
            assert(len(daCoord1) == iNum)
            assert(len(daCoord2) == iNum)
            assert(len(daCoord3) == iNum)
            # mean results
            assert(da3FluxData.shape == (iNum, iNumE, 1))
            # adiabatic results
            assert(len(daTimes2) == iNum)
            assert(np.array_equal(daTimes2, daTimes))
            assert(len(daCoord12) == iNum)
            assert(np.array_equal(daCoord12, daCoord1))
            assert(len(daCoord22) == iNum)
            assert(np.array_equal(daCoord22, daCoord2))
            assert(len(daCoord32) == iNum)
            assert(np.array_equal(daCoord32, daCoord3))
            assert(da2PitchAngles.shape == (iNum, 1))
            assert(da2Alpha.shape == (iNum, 1))
            assert(da2Lm.shape == (iNum, 1))
            assert(da2K.shape == (iNum, 1))
            assert(da2Phi.shape == (iNum, 1))
            assert(da2Hmin.shape == (iNum, 1))
            assert(da2Lstar.shape == (iNum, 1))
            assert(len(daBmin) == iNum)
            assert(len(daBlocal) == iNum)
            assert(len(daMagLT) == iNum)
            # model data results
            assert(len(daTimes3) == iNum)
            assert(np.array_equal(daTimes3, daTimes))
            assert(len(daCoord13) == iNum)
            assert(np.array_equal(daCoord13, daCoord1))
            assert(len(daCoord23) == iNum)
            assert(np.array_equal(daCoord23, daCoord2))
            assert(len(daCoord33) == iNum)
            assert(np.array_equal(daCoord33, daCoord3))
            assert(da2PitchAngles2.shape == (1, 1))
            assert(da3Data.shape == (iNum, iNumD, 1))

            iNum2, da3FluxData = self.app.flyin_percentile( 50 )
            print(' flyin_percentile( 50 ): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
            assert(iNum >= 0 and iNum <= iChunkSize)

        iNum2, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == iNum)

        iNum3, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, \
            da2Alpha, da2Lm, da2K, da2Phi, da2Hmin, da2Lstar, \
            daBmin, daBlocal, daMagLT \
          = self.app.get_adiabaticCoordsPlus( )
        print(' get_adiabaticCoordsPlus: => iNum=', iNum3,
              ', len(daTimes)=', len(daTimes2), ', len(daCoord1)=', len(daCoord12),
              ', len(daCoord2)=', len(daCoord22), ', len(daCoord3)=', len(daCoord32),
              ', da2PitchAngles.shape=', da2PitchAngles.shape, ', da2Alpha.shape=', da2Alpha.shape,
              ', da2Lm.shape=', da2Lm.shape, ', da2K.shape=', da2K.shape,
              ', da2Phi.shape=', da2Phi.shape, ', da2Hmin.shape=', da2Hmin.shape,
              ', da2Lstar.shape=', da2Lstar.shape, ', daBmin.shape=', daBmin.shape,
              ', daBlocal.shape=', daBlocal.shape, ', daMagLT.shape=', daMagLT.shape)
        assert(iNum3 == iNum)

        iNum4, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles2, da3Data \
          = self.app.get_modelData( 'doserate', 'mean' )
        print(' get_modelData(\'doserate\', \'mean\'): => iNum=', iNum4, ', len(daTimes)=', len(daTimes3),
              ', len(daCoord1)=', len(daCoord13), ', len(daCoord2)=', len(daCoord23),
              ', len(daCoord3)=', len(daCoord33), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum4 == iNum)

        # ephem results
        assert(len(daTimes) == 1)
        assert(len(daCoord1) == 1)
        assert(len(daCoord2) == 1)
        assert(len(daCoord3) == 1)
        # mean results
        assert(da3FluxData.shape == (1, 1, 1))
        # adiabatic results
        assert(len(daTimes2) == 1)
        assert(len(daCoord12) == 1)
        assert(len(daCoord22) == 1)
        assert(len(daCoord32) == 1)
        assert(da2PitchAngles.shape == (1, 1))
        assert(da2Alpha.shape == (1, 1))
        assert(da2Lm.shape == (1, 1))
        assert(da2K.shape == (1, 1))
        assert(da2Phi.shape == (1, 1))
        assert(da2Hmin.shape == (1, 1))
        assert(da2Lstar.shape == (1, 1))
        assert(len(daBmin) == 1)
        assert(len(daBlocal) == 1)
        assert(len(daMagLT) == 1)
        # model data results
        assert(len(daTimes3) == 1)
        assert(len(daCoord13) == 1)
        assert(len(daCoord23) == 1)
        assert(len(daCoord33) == 1)
        assert(da2PitchAngles2.shape == (1, 1))
        assert(da3Data.shape == (1, 1, 1))

        # reset model run: remove data files, but retain parameter settings
        ierr = self.app.reset_modelRun()
        print(' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        # now running with dose kernel
        ierr = self.app.set_useDoseKernel()
        print (' set_useDoseKernel: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        iVal = self.app.get_useDoseKernel()
        print (' get_useDoseKernel: => ', iVal)
        assert(iVal == 1)

        # fails because either kernel directory or file needs to be set
        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr < 0)

        kernelDir = "../../kernelXml"
        ierr = self.app.set_doseKernelDir(kernelDir)
        print (' set_doseKernelDir: strString=', kernelDir, ' => ', ierr)
        assert(ierr == 0)

        strString = self.app.get_doseKernelDir( )
        print (' get_doseKernelDir: => ', strString)
        assert(strString == kernelDir + '/')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        # times and coords should match shieldDose run
        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(np.array_equal(iNum, iNumShield))
        assert(len(daTimes) == iNum)
        assert(np.array_equal(daTimes, daTimesShield))
        assert(len(daCoord1) == iNum)
        assert(np.array_equal(daCoord1, daCoord1Shield))
        assert(len(daCoord2) == iNum)
        assert(np.array_equal(daCoord2, daCoord2Shield))
        assert(len(daCoord3) == iNum)
        assert(np.array_equal(daCoord3, daCoord3Shield))

        iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, da3Data \
          = self.app.get_modelData( 'doserate', 'mean' )
        print(' get_modelData(\'doserate\', \'mean\'): => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
              ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
              ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum2 == iNum)
        assert(len(daTimes2) == iNum)
        assert(np.array_equal(daTimes2, daTimes))
        assert(len(daCoord12) == iNum)
        assert(np.array_equal(daCoord12, daCoord1))
        assert(len(daCoord22) == iNum)
        assert(np.array_equal(daCoord22, daCoord2))
        assert(len(daCoord32) == iNum)
        assert(np.array_equal(daCoord32, daCoord3))
        assert(da2PitchAngles.shape == (1, 1))
        assert(da3Data.shape == (iNum, iNumD, 1))

        # kernel model and shieldDose results should not be identical
        failingValues = []
        for ii in range(iNum):
            # picked a random time to print comparison for
            if ii == 250:
                print(" At time", daTimes[ii], ":")
            for jj in range(iNumD):
                d1 = da3DataShield[ii][jj][0]
                d2 = da3Data[ii][jj][0]
                # ignore zero values
                if (d1 != 0 and d2 != 0):
                    assert(d1 != d2)

                    # 25% tolerance for difference, ignoring tiny numbers
                    if d1 > 1e-4 or d2 > 1e-4:
                        assert(abs(d1 - d2) / d1 < 0.25)

                    # collect list of large differences (including tiny numbers)
                    if abs(d1 - d2) / d1 > 0.2:
                        failingValues.append([[daTimes[ii], daDepths[jj]], d1, d2])

                    # dose values decrease as depth increases
                    if (jj < iNumD - 1):
                        assert(da3Data[ii][jj][0] > da3Data[ii][jj + 1][0])

                # printing comparison for a random time value
                if ii == 250:
                    print("   depth", daDepths[jj], ": shieldDose=", d1, ", doseKernel=", d2,
                          ", difference=", abs(d1 - d2) / d1)

        # print large differences
        print(" Number of differences over 20%: ", len(failingValues))
#        print("   [time, depth], shieldDose, doseKernel")
#        for ii in range(len(failingValues)):
#            print("  ", failingValues[ii])

    def test_run_model5a( self ):
        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)
        # reset model run: remove data files, but retain parameter settings
        ierr = self.app.reset_modelRun()
        print(' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        ierr = self.app.set_delBinDir( 0 )
        print (' set_delBinDir( 0 ): => ', ierr)
        assert(ierr == 0)
        delBinDir = self.app.get_delBinDir( )
        print (' get_delBinDir( ): => ', delBinDir)
        assert(delBinDir == 0)
#        self.bWorkDir = True

        strProp = "SGP4"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop()
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        strSgp4Mode = "improved"
        ierr = self.app.set_sgp4Mode( strSgp4Mode )
        print (' set_sgp4Mode: strString=', strSgp4Mode, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_sgp4Mode( )
        print (' get_sgp4Mode: => ', strRet)
        assert(strRet == strSgp4Mode)

        dMjd = 57023.0
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dValInput = 25
        ierr = self.app.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 16.1
        ierr = self.app.set_localTimeOfApogee( dValInput )
        print(' set_localTimeOfApogee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_localTimeOfApogee( )
        print(' get_localTimeOfApogee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 5.4
        ierr = self.app.set_localTimeMaxInclination( dValInput )
        print(' set_localTimeMaxInclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_localTimeMaxInclination()
        print(' get_localTimeMaxInclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 750
        ierr = self.app.set_altitudeOfPerigee( dValInput )
        print(' set_altitudeOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfPerigee( )
        print(' get_altitudeOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 11500
        ierr = self.app.set_altitudeOfApogee( dValInput )
        print(' set_altitudeOfApogee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfApogee( )
        print(' get_altitudeOfApogee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 218.1
        ierr = self.app.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanAnomaly()
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57023
        dEndTime = 57024.5
        dTimeStep = 60
        ierr = self.app.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "GEO"
        strUnits = "km"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits.upper() == strUnits.upper())

        iCount = 0
        for strString in ["Cumul", "Intv", "Expon"]:
          ierr = self.app.set_accumMode( strString )
          print (' set_accumMode: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          if ierr == 0: iCount += 1
          strAccum = self.app.get_accumModeEntry( iCount ) # ident, not index
          print (' get_accumModeEntry(',iCount,'): => ', strAccum)
          assert(strAccum[:5] == strString[:5])
        # verify first entry is 'Cumul' (standardized string)
        strAccum = self.app.get_accumMode( )
        print (' get_accumMode: => ', strAccum)
        assert(strAccum == "Cumul")

        # attempt to add a duplicate accum mode
        strString = "interval"
        ierr = self.app.set_accumMode( strString )
        print (' set_accumMode: strString=', strString, ' => ', ierr)
        assert(ierr == -2)
        iNum = self.app.get_numAccumModes( )
        print (' get_numAccumModes: => ', iNum)
        assert(iNum == iCount)

        iCount = 0
        for dInterval in [1800,600,900]:
          ierr = self.app.set_accumIntervalSec( dInterval )
          print (' set_accumIntervalSec(', dInterval, '): => ', ierr)
          assert(ierr == 0)
          if ierr == 0: iCount += 1
          iRetVal = self.app.get_accumIntervalSecEntry( iCount ) # ident, not index
          print (' get_accumModeEntry(',iCount,'): => ', iRetVal)
          # cannot assert what is returned
        # verify first entry is 600 (smallest value)
        iRetVal = self.app.get_accumIntervalSec( )
        print (' get_accumIntervalSec: => ', iRetVal)
        assert(iRetVal == 600)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strModel = "AP9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AP9V15_runtime_tables.mat"
        ierr = self.app.set_modelDBFile( strModelDB )
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = self.app.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = self.app.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        strWorkDir = "work3/"
        ierr = self.app.set_workDir( strWorkDir )
        print (' set_workDir: strWorkDir=', strWorkDir, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_workDir( )
        print (' get_workDir( ): => ', strString)
        assert(strString == strWorkDir)

        strBinDirName = "mc_test2"
        ierr = self.app.set_binDirName( strBinDirName)
        print (' set_binDirName: strWorkDir=', strBinDirName, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_binDirName( )
        print (' get_binDirName( ): => ', strString)
        assert(strString == strBinDirName)

        strFluxType = "Differ"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_fluxType( )
        print (' get_fluxType: => ', strString)
        assert(strString == "1PTDIFF") ## standardized string

        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 15,
                                 20, 30, 50, 60, 80])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        ierr = self.app.set_fluxMean( )
        print(' set_fluxMean: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_fluxMean( )
        print(' get_fluxMean: => ', fluxMean)
        assert(fluxMean == 1)

        ierr = self.app.set_doseRate( )
        print(' set_doseRate: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        bDoseRate = self.app.get_doseRate( )
        print(' get_doseRate: => ', bDoseRate)
        assert(bDoseRate == 1)

        strDoseUnits = "millimeters"
        ierr = self.app.set_doseDepthUnits( strDoseUnits )
        print (' set_doseDepthUnits: strString=', strDoseUnits, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_doseDepthUnits( )
        print (' get_doseDepthUnits: => ', strString)
        assert(strString == strDoseUnits)

        strDoseGeometry = "Spherical2pi"
        ierr = self.app.set_doseGeometry( strDoseGeometry )
        print (' set_doseGeometry: strString=', strDoseGeometry, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_doseGeometry()
        print (' get_doseGeometry: => ', strString)
        assert(strString == strDoseGeometry)

        strDoseDetector = "Silicon"
        ierr = self.app.set_doseDetector( strDoseDetector )
        print (' set_doseDetector: strString=', strDoseDetector, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_doseDetector()
        print (' get_doseDetector: => ', strString)
        assert(strString == strDoseDetector)

        daDepths = np.array([0.125, 0.25, 0.5, 1.0, 2.0, 5.0])
        iNumD = len(daDepths)
        ierr = self.app.set_doseDepthValues( daDepths )
        print (' set_doseDepthValues: daDepths=', daDepths, ' => ', ierr)
        assert(ierr == 0)

        iRetValD = self.app.get_numDoseDepths( )
        print (' get_numDoseDepths: => ', iRetValD)
        assert(iRetValD == iNumD)

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        # collect data in as many chunks as needed
        while iNum > 0:
            iNum2, da3FluxData = self.app.flyin_mean( )
            print(' flyin_mean: => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 == iNum)

            iNum2, da3FluxData = self.app.flyin_percentile( 50 )
            print(' flyin_percentile( 50 ): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

            # get data for next run
            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
            assert(iNum >= 0 and iNum <= iChunkSize)

        # verify that no more data
        iNum2, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == 0)

        # go back to the beginning...
        ierr = self.app.reset_modelData( )
        print(' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)

        iNum2, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == iNum)
        print(' last entry flux =',da3FluxData[iNum2-1,0,0])

        iNum3, da3FluxData = self.app.flyin_mean( "intv" ) # id=1 default
        print(' flyin_mean( "intv" ): => iNum=', iNum3, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum3 > 0)
        print(' last entry flux =',da3FluxData[iNum3-1,0,0])

        iNum4, da3FluxData = self.app.flyin_mean( "expon" )
        print(' flyin_mean( "expon" ): => iNum=', iNum4, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum4 > 0)
        print(' last entry flux =',da3FluxData[iNum4-1,0,0])

        iNum5, da3FluxData = self.app.flyin_mean( "boxcar" )
        print(' flyin_mean( "boxcar" ): => iNum=', iNum5, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum5 < 0)

        iNum3b, da3FluxData = self.app.flyin_mean( "intv", 2 )
        print(' flyin_mean( "intv", 2 ): => iNum=', iNum3b, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum3b > 0)
        print(' last entry flux =',da3FluxData[iNum3b-1,0,0])

        iNum3c, da3FluxData = self.app.flyin_mean( "intv", 3 )
        print(' flyin_mean( "intv", 3 ): => iNum=', iNum3c, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum3c > 0)
        print(' last entry flux =',da3FluxData[iNum3c-1,0,0])

        iNum3d, da3FluxData = self.app.flyin_mean( "intv", 4 )
        print(' flyin_mean( "intv", 4 ): => iNum=', iNum3d, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum3d < 0)

        iNum4, da3FluxData = self.app.flyin_mean( "expon" )
        print(' flyin_mean( "expon" ): => iNum=', iNum4, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum4 > 0)
        print(' last entry flux =',da3FluxData[iNum4-1,0,0])

        iNum4, da3FluxData = self.app.flyin_mean( "expon", 3 )
        print(' flyin_mean( "expon", 3 ): => iNum=', iNum4, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum4 > 0)
        print(' first entry flux =',da3FluxData[0,0,0])
        print('  last entry flux =',da3FluxData[iNum4-1,0,0])

        iNum4, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles2, da3Data \
             = self.app.get_modelData( 'doserate', 'mean' )
        print(' get_modelData(\'doserate\', \'mean\'): => iNum=', iNum4, ', len(daTimes)=', len(daTimes3),
              ', len(daCoord1)=', len(daCoord13), ', len(daCoord2)=', len(daCoord23),
              ', len(daCoord3)=', len(daCoord33), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum4 == iNum)
        print(' first entry doserate =',da3Data[0,0,0],' at ',daTimes3[0])
        print('  last entry doserate =',da3Data[iNum4-1,0,0],' at ',daTimes3[iNum4-1])

        iNum4a, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles2, da3Data \
             = self.app.get_modelData( 'doserate', 'mean', -1, 'intv', 1 )
        print(' get_modelData(\'doserate\', \'mean\', -1, \'intv\', 1): => iNum=', iNum4a, ', len(daTimes)=', len(daTimes3),
              ', len(daCoord1)=', len(daCoord13), ', len(daCoord2)=', len(daCoord23),
              ', len(daCoord3)=', len(daCoord33), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum4a > 0)
        print(' first entry doserate =',da3Data[0,0,0],' at ',daTimes3[0])
        print('  last entry doserate =',da3Data[iNum4a-1,0,0],' at ',daTimes3[iNum4a-1])

        iNum4a, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles2, da3Data \
             = self.app.get_modelData( 'doserate', 'mean', -1, 'intv', 2 )
        print(' get_modelData(\'doserate\', \'mean\', -1, \'intv\', 2): => iNum=', iNum4a, ', len(daTimes)=', len(daTimes3),
              ', len(daCoord1)=', len(daCoord13), ', len(daCoord2)=', len(daCoord23),
              ', len(daCoord3)=', len(daCoord33), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum4a > 0)
        print(' first entry doserate =',da3Data[0,0,0],' at ',daTimes3[0])
        print('  last entry doserate =',da3Data[iNum4a-1,0,0],' at ',daTimes3[iNum4a-1])

        iNum4a, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles2, da3Data \
             = self.app.get_modelData( 'doserate', 'mean', -1, 'intv', 3 )
        print(' get_modelData(\'doserate\', \'mean\', -1, \'intv\', 3): => iNum=', iNum4a, ', len(daTimes)=', len(daTimes3),
              ', len(daCoord1)=', len(daCoord13), ', len(daCoord2)=', len(daCoord23),
              ', len(daCoord3)=', len(daCoord33), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum4a > 0)
        print(' first entry doserate =',da3Data[0,0,0],' at ',daTimes3[0])
        print('  last entry doserate =',da3Data[iNum4a-1,0,0],' at ',daTimes3[iNum4a-1])

        iNum4b, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles2, da3Data \
             = self.app.get_modelData( 'doserate', 'mean', -1, 'expon', 3 )
        print(' get_modelData(\'doserate\', \'mean\', -1, \'expon\', 3): => iNum=', iNum4b, ', len(daTimes)=', len(daTimes3),
              ', len(daCoord1)=', len(daCoord13), ', len(daCoord2)=', len(daCoord23),
              ', len(daCoord3)=', len(daCoord33), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum4b < 0)


        iNum2, da3FluxData = self.app.flyin_percentile( 50 )
        print(' flyin_percentile( 50 ): => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 < 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum >= 0 and iNum <= iChunkSize)

        iNum2, da3FluxData = self.app.flyin_mean( )
        print(' flyin_mean: => iNum=', iNum, ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == iNum)

        iNum4, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles2, da3Data \
          = self.app.get_modelData( 'doserate', 'mean' )
        print(' get_modelData(\'doserate\', \'mean\'): => iNum=', iNum4, ', len(daTimes)=', len(daTimes3),
              ', len(daCoord1)=', len(daCoord13), ', len(daCoord2)=', len(daCoord23),
              ', len(daCoord3)=', len(daCoord33), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum4 == iNum)

        iNum4c, daTimes3, daCoord13, daCoord23, daCoord33, da2PitchAngles2, da3Data \
          = self.app.get_modelData( 'doserate', 'mean', 2, "cumul", 2 )
        print(' get_modelData(\'doserate\', \'mean\', 2, \'cumul\', 2 ): => iNum=', iNum4c, ', len(daTimes)=', len(daTimes3),
              ', len(daCoord1)=', len(daCoord13), ', len(daCoord2)=', len(daCoord23),
              ', len(daCoord3)=', len(daCoord33), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum4c < 0)


    def test_kernel_comparisons( self ):
        strGeometries = ["Spherical2pi", "SemiInfSlab", "FiniteSlab", "SemiInfSlab", "FiniteSlab", "Spherical2pi"]
        strFileGeo = ["hemisphere", "semiinf", "slab", "semiinf", "slab", "hemisphere"]
        strMaterials = ["Aluminum", "Water", "Tissue", "Calcium", "Silicon", "Bone"]
        strFileMat = ["Al", "H2O", "Tissue", "CaF2", "Si", "bone"]

        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)
        # reset model run: remove data files, but retain parameter settings
        ierr = self.app.reset_modelRun()
        print(' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        ierr = self.app.set_delBinDir( 0 )
        print (' set_delBinDir( 0 ): => ', ierr)
        assert(ierr == 0)
        delBinDir = self.app.get_delBinDir( )
        print (' get_delBinDir( ): => ', delBinDir)
        assert(delBinDir == 0)
#        self.bWorkDir = True

        strProp = "SGP4"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop()
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        strSgp4Mode = "improved"
        ierr = self.app.set_sgp4Mode( strSgp4Mode )
        print (' set_sgp4Mode: strString=', strSgp4Mode, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_sgp4Mode( )
        print (' get_sgp4Mode: => ', strRet)
        assert(strRet == strSgp4Mode)

        dMjd = 57023.0
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dValInput = 25
        ierr = self.app.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 16.1
        ierr = self.app.set_localTimeOfApogee( dValInput )
        print(' set_localTimeOfApogee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_localTimeOfApogee( )
        print(' get_localTimeOfApogee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 5.4
        ierr = self.app.set_localTimeMaxInclination( dValInput )
        print(' set_localTimeMaxInclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_localTimeMaxInclination()
        print(' get_localTimeMaxInclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 750
        ierr = self.app.set_altitudeOfPerigee( dValInput )
        print(' set_altitudeOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfPerigee( )
        print(' get_altitudeOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 11500
        ierr = self.app.set_altitudeOfApogee( dValInput )
        print(' set_altitudeOfApogee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_altitudeOfApogee( )
        print(' get_altitudeOfApogee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 218.1
        ierr = self.app.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanAnomaly()
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57023
        dEndTime = 57024.5
        dTimeStep = 60
        ierr = self.app.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "SPH"
        strUnits = "KM"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strString = "Cumul"
        ierr = self.app.set_accumMode( strString )
        print (' set_accumMode: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
        strAccum = self.app.get_accumMode( )
        print (' get_accumMode: => ', strAccum)
        assert(strAccum == strString)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize > 0)

        strModel = "AP9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AP9V15_runtime_tables.mat"
        ierr = self.app.set_modelDBFile( strModelDB )
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = self.app.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = self.app.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strFluxType = "1PTDIFF"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 15,
                                 20, 30, 50, 60, 80])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        ierr = self.app.set_fluxMean( )
        print(' set_fluxMean: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_fluxMean( )
        print(' get_fluxMean: => ', fluxMean)
        assert(fluxMean == 1)

        ierr = self.app.set_doseRate( )
        print(' set_doseRate: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        bDoseRate = self.app.get_doseRate( )
        print(' get_doseRate: => ', bDoseRate)
        assert(bDoseRate == 1)

        for iTest in range(6):
            # reset model run: remove data files, but retain parameter settings
            ierr = self.app.reset_modelRun()
            print(' reset_modelRun: => ', ierr)
            assert(ierr == 0)

            # first half of tests were protons, second half will be electrons
            if (iTest == 3):
                strModel = "AE9"
                ierr = self.app.set_model( strModel )
                print (' set_model: strString=', strModel, ' => ', ierr)
                assert(ierr == 0)
                strString = self.app.get_model()
                print (' get_model: => ', strString)
                assert(strString == strModel)

                strModelDB = "../../modelData/AE9V15_runtime_tables.mat"
                ierr = self.app.set_modelDBFile( strModelDB )
                print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
                assert(ierr == 0)
                strFile = self.app.get_modelDBFile( )
                print (' get_modelDBFile: returned file=', strFile)
                assert(strFile == strModelDB)

                daFluxEnergies = np.array([0.05, 0.1, 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0])
                iNumE = daFluxEnergies.shape[0]
                ierr = self.app.set_fluxEnergies( daFluxEnergies )
                print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
                assert(ierr == 0)
                iRetVal = self.app.get_numFluxEnergies( )
                print(' get_numFluxEnergies: => ', iRetVal)
                assert(iRetVal == iNumE)

            # first run with shieldDose2 model
            ierr = self.app.set_useDoseKernel( 0 )
            print (' set_useDoseKernel: verdict=0 => ', ierr)
            assert(ierr == 0)
            iVal = self.app.get_useDoseKernel( )
            print (' get_useDoseKernel: => ', iVal)
            assert(iVal == 0)

            strDoseUnits = "millimeters"
            ierr = self.app.set_doseDepthUnits( strDoseUnits )
            print (' set_doseDepthUnits: strString=', strDoseUnits, ' => ', ierr)
            assert(ierr == 0)
            strString = self.app.get_doseDepthUnits( )
            print (' get_doseDepthUnits: => ', strString)
            assert(strString == strDoseUnits)

            strDoseGeometry = strGeometries[iTest]
            ierr = self.app.set_doseGeometry( strDoseGeometry )
            print (' set_doseGeometry: strString=', strDoseGeometry, ' => ', ierr)
            assert(ierr == 0)
            strString = self.app.get_doseGeometry( )
            print (' get_doseGeometry: => ', strString)
            assert(strString == strDoseGeometry)

            strDoseDetector = strMaterials[iTest]
            ierr = self.app.set_doseDetector( strDoseDetector )
            print (' set_doseDetector: strString=', strDoseDetector, ' => ', ierr)
            assert(ierr == 0)
            strString = self.app.get_doseDetector( )
            print (' get_doseDetector: => ', strString)
            assert(strString == strDoseDetector)

            daDepths = np.array([0.125, 0.25, 0.5, 1.0, 2.0, 5.0])
            iNumD = len(daDepths)
            ierr = self.app.set_doseDepthValues( daDepths )
            print (' set_doseDepthValues: daDepths=', daDepths, ' => ', ierr)
            assert(ierr == 0)

            iRetValD = self.app.get_numDoseDepths( )
            print (' get_numDoseDepths: => ', iRetValD)
            assert(iRetValD == iNumD)

            # successful run
            ierr = self.app.run_model( )
            print(' run_model: => ', ierr)
            assert(ierr == 0)

            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
            assert(iNum == iChunkSize)

            # saving values of first shieldDose results to compare later
            iNumShield, daTimesShield, daCoord1Shield, daCoord2Shield, daCoord3Shield, daShieldData = [[] for _ in range(6)]

            while iNum > 0:
                iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, da3Data \
                  = self.app.get_modelData( 'doserate', 'mean' )
                print(' get_modelData(\'doserate\', \'mean\'): => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
                      ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
                      ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles.shape,
                      ', da3Data.shape=', da3Data.shape)
                assert(iNum2 == iNum)

                # save results for later comparison
                iNumShield.append(iNum2)
                daTimesShield.append(daTimes2)
                daCoord1Shield.append(daCoord12)
                daCoord2Shield.append(daCoord22)
                daCoord3Shield.append(daCoord32)
                daShieldData.append(da3Data)

                # ephem results
                assert(len(daTimes) == iNum)
                assert(len(daCoord1) == iNum)
                assert(len(daCoord2) == iNum)
                assert(len(daCoord3) == iNum)
                # model data results
                assert(len(daTimes2) == iNum)
                assert(np.array_equal(daTimes2, daTimes))
                assert(len(daCoord12) == iNum)
                assert(np.array_equal(daCoord12, daCoord1))
                assert(len(daCoord22) == iNum)
                assert(np.array_equal(daCoord22, daCoord2))
                assert(len(daCoord32) == iNum)
                assert(np.array_equal(daCoord32, daCoord3))
                assert(da2PitchAngles.shape == (1, 1))
                assert(da3Data.shape == (iNum, iNumD, 1))

                iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
                print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
                assert(iNum >= 0 and iNum <= iChunkSize)


            # reset model run: remove data files, but retain parameter settings
            ierr = self.app.reset_modelRun( )
            print(' reset_modelRun: => ', ierr)
            assert(ierr == 0)

            # now running with dose kernel
            ierr = self.app.set_useDoseKernel( )
            print (' set_useDoseKernel: verdict=<unspecified> => ', ierr)
            assert(ierr == 0)
            iVal = self.app.get_useDoseKernel( )
            print (' get_useDoseKernel: => ', iVal)
            assert(iVal == 1)

            # set the correct kernel file
            if (iTest < 3):
                strKernelFile = "../../kernelXml/Dose_proton_"+strFileGeo[iTest]+"_"+strFileMat[iTest]+".xml"
            else:
                strKernelFile = "../../kernelXml/Dose_electron_"+strFileGeo[iTest]+"_"+strFileMat[iTest]+".xml"

            ierr = self.app.set_doseKernelFile( strKernelFile )
            print (' set_doseKernelFile: strString=', strKernelFile, ' => ', ierr)
            assert(ierr == 0)
            retKernelFile = self.app.get_doseKernelFile( )
            assert(retKernelFile == strKernelFile)

            # should work with correct file specified
            ierr = self.app.run_model( )
            print(' run_model: => ', ierr)
            assert(ierr == 0)

            # set an invalid kernel file - this will clear the currently set kernel file
            strKernelFile = "../../kernelXml/Dose_proton_semisphere_Si.xml"
            ierr = self.app.set_doseKernelFile(strKernelFile)
            assert(ierr < 0)

            kernelDir = "../../kernelXml"
            ierr = self.app.set_doseKernelDir( kernelDir )
            print (' set_doseKernelDir: strString=', kernelDir, ' => ', ierr)
            assert(ierr == 0)
            strString = self.app.get_doseKernelDir( )
            print (' get_doseKernelDir: => ', strString)
            assert(strString == kernelDir + '/')

            # should work with automatic finding of the file
            ierr = self.app.run_model( )
            print(' run_model: => ', ierr)
            assert(ierr == 0)

            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
            assert(iNum == iChunkSize)
            assert(len(daTimes) == iNum)
            assert(iNum == iNumShield.pop(0))
            assert(np.array_equal(daTimes, daTimesShield.pop(0)))
            assert(len(daCoord1) == iNum)
            assert(np.array_equal(daCoord1, daCoord1Shield.pop(0)))
            assert(len(daCoord2) == iNum)
            assert(np.array_equal(daCoord2, daCoord2Shield.pop(0)))
            assert(len(daCoord3) == iNum)
            assert(np.array_equal(daCoord3, daCoord3Shield.pop(0)))

            failingValues = []

            while iNum > 0:
                iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, da3Data \
                  = self.app.get_modelData( 'doserate', 'mean' )
                print(' get_modelData(\'doserate\', \'mean\'): => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
                      ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
                      ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles.shape,
                      ', da3Data.shape=', da3Data.shape)
                assert(iNum2 == iNum)
                assert(len(daTimes2) == iNum)
                assert(np.array_equal(daTimes2, daTimes))
                assert(len(daCoord12) == iNum)
                assert(np.array_equal(daCoord12, daCoord1))
                assert(len(daCoord22) == iNum)
                assert(np.array_equal(daCoord22, daCoord2))
                assert(len(daCoord32) == iNum)
                assert(np.array_equal(daCoord32, daCoord3))
                assert(da2PitchAngles.shape == (1, 1))
                assert(da3Data.shape == (iNum, iNumD, 1))

                da3DataSD2 = daShieldData.pop(0)

                # kernel model and shieldDose results should not be identical
                for ii in range(iNum):
                    # picked a random time to print comparison for
                    if ii == 250:
                        print(" At time", daTimes[ii], ":")
                    for jj in range(iNumD):
                        d1 = da3DataSD2[ii][jj][0]
                        d2 = da3Data[ii][jj][0]
                        # ignore zero values
                        if (d1 != 0 and d2 != 0):
                            if d1 == d2:
                                print("exact match - ii={}, jj={}, value={:e}".format(ii, jj, d2))
                                #JC - just print exact matches failingValues.append([[daTimes[ii], daDepths[jj]], d1, d2])

                            # 20% tolerance for difference, ignoring tiny numbers
                            diff = abs(d1 - d2) / d1
                            if d1 > 1e-4 or d2 > 1e-4:
                                if diff >= 0.2:
                                    failingValues.append([[daTimes[ii], daDepths[jj]], d1, d2, diff])
                                # JC - turn off fail on large value
                                # assert(diff < 0.2)

                            # dose values decrease as depth increases
                            if (jj < iNumD - 1):
                                assert(da3Data[ii][jj][0] > da3Data[ii][jj + 1][0])

                        # printing comparison for a random time value
                        if ii == 250:
                            print("   depth {}: shieldDose={:.4e}, doseKernel={:.4e}, difference={:.4e}".format(
                                  daDepths[jj], d1, d2, abs(d1 - d2) / d1))

                iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
                print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
                assert(iNum >= 0 and iNum <= iChunkSize)
                if iNum > 0:
                    assert(len(daTimes) == iNum)
                    assert(iNum == iNumShield.pop(0))
                    assert(np.array_equal(daTimes, daTimesShield.pop(0)))
                    assert(len(daCoord1) == iNum)
                    assert(np.array_equal(daCoord1, daCoord1Shield.pop(0)))
                    assert(len(daCoord2) == iNum)
                    assert(np.array_equal(daCoord2, daCoord2Shield.pop(0)))
                    assert(len(daCoord3) == iNum)
                    assert(np.array_equal(daCoord3, daCoord3Shield.pop(0)))

            # print large differences
            print(" Number of differences over 20%: ", len(failingValues))
            print("   [time, depth], shieldDose, doseKernel, difference")
#            for ii in range(len(failingValues)):
#                print("  [{}, {:.4e}, {:.4e}, {:.4e}]".format(
#                    failingValues[ii][0], failingValues[ii][1], failingValues[ii][2], failingValues[ii][3]))

    def test_run_model6( self ):
        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)

        strProp = "Kepler"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop( )
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        ierr = self.app.set_keplerUseJ2( )
        print(' set_keplerUseJ2: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        bUseJ2 = self.app.get_keplerUseJ2()
        print(' get_keplerUseJ2: => ', bUseJ2)
        assert(bUseJ2 == 1)

        dMjd = 57023.0
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        dTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', dTime)
        assert(dTime == dMjd)

        dValInput = 30
        ierr = self.app.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 254.1
        ierr = self.app.set_rightAscension( dValInput )
        print(' set_rightAscension: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_rightAscension()
        print(' get_rightAscension: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 45.0
        ierr = self.app.set_argOfPerigee( dValInput )
        print(' set_argOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_argOfPerigee( )
        print(' get_argOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 0.25
        ierr = self.app.set_eccentricity( dValInput )
        print(' set_eccentricity: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_eccentricity()
        print(' get_eccentricity: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 25.4
        ierr = self.app.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanAnomaly()
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57023
        dEndTime = 57024.5
        dTimeStep = 60
        ierr = self.app.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "GEI"
        strUnits = "RE"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strModel = "AP8"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/radiationBeltDB.h5"
        ierr = self.app.set_modelDBFile( strModelDB )
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strActLevel = "Max"
        ierr = self.app.set_legActivityLevel( strActLevel )
        print (' set_legActivityLevel: strString=', strActLevel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_legActivityLevel( )
        print (' get_legActivityLevel: => ', strString)
        assert(strString == strActLevel)

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 3)

        dValInput = 7.5
        ierr = self.app.set_meanMotion( dValInput )
        print(' set_meanMotion: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanMotion()
        print(' get_meanMotion: => ', dValue)
        assert(dValue == dValInput)

        strFluxType = "1PTDIFF"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 15,
                                 20, 30, 50, 60, 80, 100, 150, 200])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        ierr = self.app.set_fluxMean( )
        print(' set_fluxMean: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_fluxMean( )
        print(' get_fluxMean: => ', fluxMean)
        assert(fluxMean == 1)

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 0)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        # first chunk should be default chunk size 960
        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        # new chunk size - will reset data
        iChunkSize = 335
        ierr = self.app.set_chunkSize( iChunkSize )
        print (' set_chunkSize: iChunkSize=', iChunkSize, ' => ', ierr)
        assert(ierr == 0)
        retVal = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', retVal)
        assert(retVal == iChunkSize)

        bFirstRun = True

        # loop while there's still data left
        while iNum > 0:
            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print (' get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))

            if bFirstRun:
                # first returned time is start time
                assert(daTimes[0] == dStartTime)
                assert(iNum == iChunkSize)
                bFirstRun = False
            else:
                assert(iNum <= iChunkSize)

            iNum2, da3FluxData = self.app.flyin_mean( )
            print(' flyin_mean: => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 == iNum)

            if (iNum > 0):
                assert(da3FluxData.shape == (iNum, iNumE, 1))

            iNum2, da3FluxData = self.app.flyin_percentile( 50 )
            print(' flyin_percentile( 50 ): => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

            iNum2, da3FluxData = self.app.flyin_perturbedMean( 1 )
            print(' flyin_perturbedMean( 1 ): => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

            iNum2, da3FluxData = self.app.flyin_monteCarlo( 10 )
            print(' flyin_monteCarlo(10): => iNum=', iNum2, ', da3FluxData.shape=', da3FluxData.shape)
            assert(iNum2 < 0)

        # reset data back to beginning
        ierr = self.app.reset_modelData()
        print (' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print (' get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))

        # first returned time is start time
        assert(daTimes[0] == dStartTime)
        assert(iNum == iChunkSize)
        bFirstRun = False

        iNum2, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3FluxData \
          = self.app.flyin_meanPlus( )
        print(' flyin_meanPlus: => iNum=', iNum2, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3FluxData.shape=', da3FluxData.shape)
        assert(iNum2 == iNum)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da3FluxData.shape == (iNum, iNumE, 1))
        assert(da2PitchAngles.shape == (1, 1))

        # verify pitch angle is 0
        assert(da2PitchAngles[0][0] == 0)

        # reset data back to beginning
        ierr = self.app.reset_modelData()
        print (' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum2, da2FluxData = self.app.flyin_mean2d( )
        print(' flyin_mean2d: => iNum=', iNum2, ', len(da2FluxData)=', len(da2FluxData))
        assert(iNum2 == iNum)
        assert(da2FluxData.shape == (iNum, iNumE))
        # use slice to remove third dimension and the rest of the data should match
        np.array_equal(da2FluxData, da3FluxData[:, :, 0])

        # reset data back to beginning
        ierr = self.app.reset_modelData()
        print (' reset_modelData: => ', ierr)
        assert(ierr == 0)

        iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles2, da3Data \
          = self.app.get_modelData( 'flux', 'mean' )
        print(' get_modelData: => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
              ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
              ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles2.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum2 == iNum)
        assert(len(daTimes2) == iNum)
        assert(np.array_equal(daTimes2, daTimes))
        assert(len(daCoord12) == iNum)
        assert(np.array_equal(daCoord12, daCoord1))
        assert(len(daCoord22) == iNum)
        assert(np.array_equal(daCoord22, daCoord2))
        assert(len(daCoord32) == iNum)
        assert(np.array_equal(daCoord32, daCoord3))
        assert(da2PitchAngles2.shape == (1, 1))
        assert(da3Data.shape == (iNum, iNumE, 1))

        # verify pitch angles contains all 0's
        assert(da2PitchAngles[0][0] == 0)

        vars = self.app.get_modelData( 'flux', 'mean', -1, "interval" )
        print(' get_modelData: => ', vars[0])
        assert(vars[0] < 0)

        vars = self.app.get_modelData( 'fluence', 'mean' )
        print(' get_modelData: => ', vars[0])
        assert(vars[0] < 0)

        # reset model run: remove data files, but retain parameter settings
        ierr = self.app.reset_modelRun()
        print (' reset_modelRun: => ', ierr)
        assert(ierr == 0)

        strActLevel = "Min"
        ierr = self.app.set_legActivityLevel( strActLevel )
        print (' set_legActivityLevel: strString=', strActLevel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_legActivityLevel()
        print (' get_legActivityLevel: => ', strString)
        assert(strString == strActLevel)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iNum, daTimes, daCoord1, daCoord2, daCoord3, da2PitchAngles, da3Data2 \
          = self.app.get_modelData( 'flux', 'mean' )
        print(' get_modelData: => iNum=', iNum, ', len(daTimes)=', len(daTimes),
              ', len(daCoord1)=', len(daCoord1), ', len(daCoord2)=', len(daCoord2),
              ', len(daCoord3)=', len(daCoord3), ', da2PitchAngles.shape=', da2PitchAngles.shape,
              ', da3Data.shape=', da3Data.shape)
        assert(iNum == iNum)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)
        assert(da2PitchAngles.shape == (1, 1))
        assert(da3Data2.shape == (iNum, iNumE, 1))

        # make sure that "Max" activity and "Min" activity produce different results
        for ii in range(iNum):
            for jj in range(iNumE):
                dVal1 = da3Data[ii][jj][0]
                dVal2 = da3Data2[ii][jj][0]
                # ignore zeroes
                if dVal1 != 0 and dVal2 != 0:
                    assert(dVal1 != dVal2)

# For now, the *direct* MPI python operations are disabled
#  -->MPI spawn/comm issues due to loading MPI-enabled libraries by Python
    def test_bMpi( self ):

        # verify that the (optional) multi-threaded feature is present
        #   before testing MPI application-level API operations
        strMpiExec = os.environ['IRENE_BIN']+'/CmdLineIreneMpi'
        if os.environ['IRENE_SYS'] != 'linux': strMpiExec += '.exe'
        if not os.path.exists( strMpiExec ):
          print (' all multi-threaded tests skipped - feature not present')
          print ('    (file ',strMpiExec,' not found)')
          return

        ### using _indirect_ multithreaded operations ###
        ###  until python/mpi lib issues are resolved ###
        bMpiBootstrapWorkaround = True

        ierr = self.app.reset_parameters( )
        print(' reset_parameters: => ', ierr)
        assert(ierr == 0)

        strString = "."
        ierr = self.app.set_execDir( strString )
        print (' set_execDir: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        assert(strRet == './')

        numProc = -4
        ierr = self.app.set_numProc(numProc)
        print (' set_numProc: numProc=', numProc, ' => ', ierr)
        assert(ierr == 0)
        retVal = self.app.get_numProc()
        print (' get_numProc: => ', retVal)
        assert(retVal == numProc)

        strProp = "Kepler"
        ierr = self.app.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = self.app.get_prop()
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        ierr = self.app.set_keplerUseJ2()
        print(' set_keplerUseJ2: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        bUseJ2 = self.app.get_keplerUseJ2()
        print(' get_keplerUseJ2: => ', bUseJ2)
        assert(bUseJ2 == 1)

        dMjd = 57023.0
        ierr = self.app.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = self.app.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dValInput = 30
        ierr = self.app.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 254.1
        ierr = self.app.set_rightAscension( dValInput )
        print(' set_rightAscension: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_rightAscension()
        print(' get_rightAscension: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 45.0
        ierr = self.app.set_argOfPerigee( dValInput )
        print(' set_argOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_argOfPerigee( )
        print(' get_argOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 0.25
        ierr = self.app.set_eccentricity( dValInput )
        print(' set_eccentricity: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_eccentricity()
        print(' get_eccentricity: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 25.4
        ierr = self.app.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanAnomaly()
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57023
        dEndTime = 57024.5
        dTimeStep = 60
        ierr = self.app.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = self.app.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "GEI"
        strUnits = "RE"
        ierr = self.app.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = self.app.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = self.app.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strModel = "AP9"
        ierr = self.app.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = self.app.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AP9V15_runtime_tables.mat"
        ierr = self.app.set_modelDBFile( strModelDB )
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = self.app.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = self.app.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = self.app.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        meanMotion = 7.5
        ierr = self.app.set_meanMotion(meanMotion)
        print(' set_meanMotion: dValue=', meanMotion, ' => ', ierr)
        assert(ierr == 0)
        dValue = self.app.get_meanMotion()
        print(' get_meanMotion: => ', dValue)
        assert(dValue == meanMotion)

        strFluxType = "1PTDIFF"
        ierr = self.app.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = self.app.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 15,
                                 20, 30, 50, 60, 80, 100, 150, 200, 300])
        iNumE = daFluxEnergies.shape[0]
        ierr = self.app.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = self.app.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        ierr = self.app.set_fluxMean( )
        print(' set_fluxMean: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        fluxMean = self.app.get_fluxMean( )
        print(' get_fluxMean: => ', fluxMean)
        assert(fluxMean == 1)

        ierr = self.app.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 0)

        # when method run individually, execDir needs to be set
        strRet = self.app.get_execDir( )
        print (' get_execDir: => ', strRet)
        if strRet == 'undef':
          strString = "."
          ierr = self.app.set_execDir( strString )
          print (' set_execDir: strString=', strString, ' => ', ierr)
          assert(ierr == 0)
          strRet = self.app.get_execDir( )
          print (' get_execDir: => ', strRet)
          assert(strRet == './')

        ierr = self.app.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = self.app.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        # first chunk should be default chunk size 960
        iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(len(daCoord1) == iNum)
        assert(len(daCoord2) == iNum)
        assert(len(daCoord3) == iNum)

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        # saving values of CmdLineIrene results to compare later
        iNumCmd, daTimesCmd, daCoord1Cmd, daCoord2Cmd, daCoord3Cmd, daCmdData = [[] for _ in range(6)]

        while iNum > 0:
            iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, da3Data \
              = self.app.get_modelData( 'flux', 'mean' )
            print(' get_modelData(\'flux\', \'mean\'): => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
                  ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
                  ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles.shape,
                  ', da3Data.shape=', da3Data.shape)
            assert(iNum2 == iNum)

            # save results for later comparison
            iNumCmd.append(iNum2)
            daTimesCmd.append(daTimes2)
            daCoord1Cmd.append(daCoord12)
            daCoord2Cmd.append(daCoord22)
            daCoord3Cmd.append(daCoord32)
            daCmdData.append(da3Data)

            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
            assert(iNum >= 0 and iNum <= iChunkSize)

        print('--- New Application object with multithreaded operation ---' )

        # issues with python loading of mpi-enabled shared object libraries
        #  prevent proper MPI-based spawning/communication of/with helper applications
        # --> until proper solution found, using built-in CmdLineIrene bootstrapping to Mpi version
        if bMpiBootstrapWorkaround:
          print(' ==== using *indirect* MPI operation, due to Python/MPI lib issues ==== ')
          appMpi = Application() ## uses CmdLineIrene mpi bootstrap workaround 
        else:
          appMpi = Application(bMpi=True) ## most likely result in MPI-centric failures

        print ('appMpi zHandle=', appMpi.zHandle)

        strString = "."
        ierr = appMpi.set_execDir( strString )
        print (' set_execDir: strString=', strString, ' => ', ierr)
        assert(ierr == 0)
        strRet = appMpi.get_execDir( )
        print (' get_execDir: => ', strRet)
        assert(strRet == './')

        numProc = -4
        ierr = appMpi.set_numProc(numProc)
        print (' set_numProc: numProc=', numProc, ' => ', ierr)
        assert(ierr == 0)
        retVal = appMpi.get_numProc()
        print (' get_numProc: => ', retVal)
        assert(retVal == numProc)

        strProp = "Kepler"
        ierr = appMpi.set_prop( strProp )
        print (' set_prop: strString=', strProp, ' => ', ierr)
        assert(ierr == 0)
        strRet = appMpi.get_prop()
        print (' get_prop: => ', strRet)
        assert(strRet == strProp)

        ierr = appMpi.set_keplerUseJ2()
        print(' set_keplerUseJ2: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        bUseJ2 = appMpi.get_keplerUseJ2()
        print(' get_keplerUseJ2: => ', bUseJ2)
        assert(bUseJ2 == 1)

        dMjd = 57023.0
        ierr = appMpi.set_elementTime( dMjd )
        print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
        assert(ierr == 0)
        elementTime = appMpi.get_elementTime( )
        print(' get_elementTime: => ', elementTime)
        assert(elementTime == dMjd)

        dValInput = 30
        ierr = appMpi.set_inclination( dValInput )
        print(' set_inclination: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = appMpi.get_inclination()
        print(' get_inclination: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 254.1
        ierr = appMpi.set_rightAscension( dValInput )
        print(' set_rightAscension: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = appMpi.get_rightAscension()
        print(' get_rightAscension: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 45.0
        ierr = appMpi.set_argOfPerigee( dValInput )
        print(' set_argOfPerigee: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = appMpi.get_argOfPerigee( )
        print(' get_argOfPerigee: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 0.25
        ierr = appMpi.set_eccentricity( dValInput )
        print(' set_eccentricity: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = appMpi.get_eccentricity()
        print(' get_eccentricity: => ', dValue)
        assert(dValue == dValInput)

        dValInput = 25.4
        ierr = appMpi.set_meanAnomaly( dValInput )
        print(' set_meanAnomaly: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = appMpi.get_meanAnomaly()
        print(' get_meanAnomaly: => ', dValue)
        assert(dValue == dValInput)

        dStartTime = 57023
        dEndTime = 57024.5
        dTimeStep = 60
        ierr = appMpi.set_times( dStartTime, dEndTime, dTimeStep )
        print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' % (dStartTime, dEndTime, dTimeStep, ierr))
        assert(ierr == 0)
        dStart, dEnd, dStep = appMpi.get_times( )
        print(' get_times: => start=%.1f, end=%.1f, step=%.0f' % (dStart, dEnd, dStep))
        assert(dStart == dStartTime)
        assert(dEnd == dEndTime)
        assert(dStep == dTimeStep)

        strSys = "GEI"
        strUnits = "RE"
        ierr = appMpi.set_coordSys( strSys, strUnits )
        print (' set_coordSys: input={', strSys, ',', strUnits, '} => ', ierr)
        strRetSys = appMpi.get_coordSys( )
        print(' get_coordSys: => ', strRetSys)
        strRetSysUnits = appMpi.get_coordSysUnits( )
        print(' get_coordSysUnits: => ', strRetSysUnits)
        assert(strRetSys == strSys)
        assert(strRetSysUnits == strUnits)

        strModel = "AP9"
        ierr = appMpi.set_model( strModel )
        print (' set_model: strString=', strModel, ' => ', ierr)
        assert(ierr == 0)
        strString = appMpi.get_model()
        print (' get_model: => ', strString)
        assert(strString == strModel)

        strModelDB = "../../modelData/AP9V15_runtime_tables.mat"
        ierr = appMpi.set_modelDBFile( strModelDB )
        print (' set_modelDBFile: dbFile=', strModelDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = appMpi.get_modelDBFile( )
        print (' get_modelDBFile: returned file=', strFile)
        assert(strFile == strModelDB)

        strMagfieldDB = "../../modelData/igrfDB.h5"
        ierr = appMpi.set_magfieldDBFile( strMagfieldDB )
        print (' set_magfieldDBFile: dbFile=', strMagfieldDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = appMpi.get_magfieldDBFile( )
        print (' get_magfieldDBFile: returned file=', strFile)
        assert(strFile == strMagfieldDB)

        strKPhiDB = "../../modelData/fastPhi_net.mat"
        ierr = appMpi.set_kPhiDBFile( strKPhiDB )
        print (' set_kPhiDBFile: dbFile=', strKPhiDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = appMpi.get_kPhiDBFile( )
        print (' get_kPhiDBFile: returned file=', strFile)
        assert(strFile == strKPhiDB)

        strKHMinDB = "../../modelData/fast_hmin_net.mat"
        ierr = appMpi.set_kHMinDBFile( strKHMinDB )
        print (' set_kHMinDBFile: dbFile=', strKHMinDB, ' => ', ierr)
        assert(ierr == 0)
        strFile = appMpi.get_kHMinDBFile( )
        print (' get_kHMinDBFile: returned file=', strFile)
        assert(strFile == strKHMinDB)

        ierr = appMpi.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 4)

        dValInput = 7.5
        ierr = appMpi.set_meanMotion( dValInput )
        print(' set_meanMotion: dValue=', dValInput, ' => ', ierr)
        assert(ierr == 0)
        dValue = appMpi.get_meanMotion()
        print(' get_meanMotion: => ', dValue)
        assert(dValue == dValInput)

        strFluxType = "1PTDIFF"
        ierr = appMpi.set_fluxType( strFluxType )
        print (' set_fluxType: strString=', strFluxType, ' => ', ierr)
        assert(ierr == 0)
        strFile = appMpi.get_fluxType( )
        print (' get_fluxType: => ', strFile)
        assert(strFile == strFluxType)

        daFluxEnergies = np.array([0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 15,
                                 20, 30, 50, 60, 80, 100, 150, 200, 300])
        iNumE = daFluxEnergies.shape[0]
        ierr = appMpi.set_fluxEnergies( daFluxEnergies )
        print(' set_fluxEnergies: fluxEnergies= ' , daFluxEnergies, ' => ', ierr)
        assert(ierr == 0)
        iRetVal = appMpi.get_numFluxEnergies( )
        print(' get_numFluxEnergies: => ', iRetVal)
        assert(iRetVal == iNumE)

        ierr = appMpi.set_fluxMean( )
        print(' set_fluxMean: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        iFluxMean = appMpi.get_fluxMean( )
        print(' get_fluxMean: => ', iFluxMean)
        assert(iFluxMean == 1)

        ierr = appMpi.validate_parameters( )
        print(' validate_parameters: => ', ierr)
        assert(ierr == 0)

        ierr = appMpi.run_model( )
        print(' run_model: => ', ierr)
        assert(ierr == 0)

        iChunkSize = appMpi.get_chunkSize( )
        print (' get_chunkSize: => ', iChunkSize)
        assert(iChunkSize == 960)

        # first chunk should be default chunk size 960
        iNum, daTimes, daCoord1, daCoord2, daCoord3 = appMpi.get_ephemeris( )
        print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
        assert(iNum == iChunkSize)
        assert(len(daTimes) == iNum)
        assert(iNum == iNumCmd.pop(0))
        assert(np.array_equal(daTimes, daTimesCmd.pop(0)))
        assert(len(daCoord1) == iNum)
        assert(np.array_equal(daCoord1, daCoord1Cmd.pop(0)))
        assert(len(daCoord2) == iNum)
        assert(np.array_equal(daCoord2, daCoord2Cmd.pop(0)))
        assert(len(daCoord3) == iNum)
        assert(np.array_equal(daCoord3, daCoord3Cmd.pop(0)))

        # first returned time is start time
        assert(daTimes[0] == dStartTime)

        while iNum > 0:
            iNum2, daTimes2, daCoord12, daCoord22, daCoord32, da2PitchAngles, da3Data \
              = appMpi.get_modelData( 'flux', 'mean' )
            print(' get_modelData(\'flux\', \'mean\'): => iNum=', iNum2, ', len(daTimes)=', len(daTimes2),
                  ', len(daCoord1)=', len(daCoord12), ', len(daCoord2)=', len(daCoord22),
                  ', len(daCoord3)=', len(daCoord32), ', da2PitchAngles.shape=', da2PitchAngles.shape,
                  ', da3Data.shape=', da3Data.shape)
            assert(iNum2 == iNum)
            assert(len(daTimes2) == iNum)
            assert(np.array_equal(daTimes2, daTimes))
            assert(len(daCoord12) == iNum)
            assert(np.array_equal(daCoord12, daCoord1))
            assert(len(daCoord22) == iNum)
            assert(np.array_equal(daCoord22, daCoord2))
            assert(len(daCoord32) == iNum)
            assert(np.array_equal(daCoord32, daCoord3))
            assert(da2PitchAngles.shape == (1, 1))
            assert(da3Data.shape == (iNum, iNumE, 1))

            da3DataCmd = daCmdData.pop(0)

            # results should be identical regardless of how the model was run
            for ii in range(iNum):
                for jj in range(iNumE):
                    d1 = da3DataCmd[ii][jj][0]
                    d2 = da3Data[ii][jj][0]
                    # ignore tiny values
                    if d1 > 1e-4 or d2 > 1e-4:
                        if d1 != d2:
                            print("Fail - ii={}, jj={}, d1(bMpi=True)={:e}, d2(CmdLineIrene)={:e}".format(ii, jj, d1, d2))
                        assert(d1 == d2)

            iNum, daTimes, daCoord1, daCoord2, daCoord3 = self.app.get_ephemeris( )
            print ('  get_ephemeris: => iNum=', iNum, '; len(daTimes)=', len(daTimes), ', len(daCoord1)=', len(daCoord1))
            assert(iNum >= 0 and iNum <= iChunkSize)
            if iNum > 0:
                assert(len(daTimes) == iNum)
                assert(iNum == iNumCmd.pop(0))
                assert(np.array_equal(daTimes, daTimesCmd.pop(0)))
                assert(len(daCoord1) == iNum)
                assert(np.array_equal(daCoord1, daCoord1Cmd.pop(0)))
                assert(len(daCoord2) == iNum)
                assert(np.array_equal(daCoord2, daCoord2Cmd.pop(0)))
                assert(len(daCoord3) == iNum)
                assert(np.array_equal(daCoord3, daCoord3Cmd.pop(0)))

        del appMpi

    def teardown_class( self ):
        del self.app

        # ensure that directories and files were retained after deletion
        # (but only if those particular tests were run)
        if self.bWorkDir:
          bWorkExists = os.path.isdir("work")
          print(' work directory exists: ', bWorkExists)
          assert(bWorkExists)
        if self.bWorkDir_pm_agg:
          bPmaggExists = os.path.isdir("work/pm_agg")
          print(' work/pm_agg directory exists: ', bPmaggExists)
          assert(bPmaggExists)

          strPath, dirs, straFiles = next(os.walk("work/pm_agg"))
          iCount = len(straFiles)
          print(' File count of work/pm_agg/:', iCount)
          assert(iCount > 90)

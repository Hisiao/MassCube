
# enable use of python3 print function form in python 2.x
from __future__ import print_function

# for the 'import irene_defs' to be successful, either:
#     specify the path to the Ae9Ap9 root directory here:
#sys.path.append(os.environ['HOME']+'/Ae9Ap9')
#     or in the PYTHONPATH environement variable
#
# load definitions for required IRENE environment variables
import irene_defs

from ae9ap9Model import Ae9Ap9Model
from ephemModel import EphemModel
from doseModel import DoseModel
import numpy as np
import pytest


@pytest.fixture
def dose():
    dose = DoseModel()
    print('dose zHandle=', dose.zHandle)
    ierr = dose.set_modelDBFile('../../modelData/sd2DB.h5')
    assert(ierr == 0)
    return dose


# In general, each method is tested first with 'invalid' inputs, then valid ones.
# The associated 'get' methods are also tested, when available, in the same manner.
# Some also include tests with invalid 'handle' values for completeness.

def test_set_modelDBFile():
    dose = DoseModel()

    strString = "../../modelData/sdDB.h5"
    ierr = dose.set_modelDBFile(strString)
    print(' set_modelDBFile: strString=', strString, ' => ', ierr)
    assert(ierr < 0)

    retFile = dose.get_modelDBFile()
    print(' get_modelDBFile: => ', retFile)
    assert(retFile == "UNDEF")

    strString = "../../modelData/sd2DB.h5"
    ierr = dose.set_modelDBFile(strString)
    print(' set_modelDBFile: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    retFile = dose.get_modelDBFile()
    print(' get_modelDBFile: => ', retFile)
    assert(retFile == strString)

    ierr = dose.test_badHandle()
    print(' test_badHandle => ', ierr)
    assert(ierr < 0)


def test_set_species(dose):
    for strString in ["quarks", "muons", "neutrons"]:
        ierr = dose.set_species(strString)
        print(' set_species: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        retStr = dose.get_species()
        print(' get_species: => ', retStr)
        assert(retStr == "electrons")  # default value

    for strString in ["protons", "solarprotons", "electrons"]:
        ierr = dose.set_species(strString)
        print(' set_species: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        retStr = dose.get_species()
        print(' get_species: => ', retStr)
        assert(retStr == strString)


def test_set_detector(dose):
    for strString in ["He", "NaCl", "CalciumChloride"]:
        ierr = dose.set_detector(strString)
        print(' set_detector: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        retStr = dose.get_detector()
        print(' get_detector: => ', retStr)
        assert(retStr == "Silicon")  # default value

    for strString in ["water", "air", "lithium", "calcium", "silicon", "gallium"]:
        ierr = dose.set_detector(strString)
        print(' set_detector: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        retStr = dose.get_detector().lower()
        print(' get_detector: => ', retStr)
        assert(retStr == strString)


def test_set_geometry(dose):
    for strString in ["square", "circle", "dodecahedron"]:
        ierr = dose.set_geometry(strString)
        print(' set_geometry: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = dose.get_geometry()
        print(' get_geometry: => ', strRet)
        assert(strRet == "Spherical2pi")  # Spherical2pi is the default geometry

    # set using standardized strings
    for strString in ["SemiInfiniteSlab", "FiniteSlab", "Spherical4pi", "Spherical2pi"]:
        ierr = dose.set_geometry(strString)
        print(' set_geometry: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = dose.get_geometry()
        print(' get_geometry: => ', strRet)
        assert(strRet == strString)


def test_set_energies(dose):
    daEnergies = np.array([-0.1, 0.0, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    ierr = dose.set_energies(daEnergies, "MeV")
    print(' set_energies: daEnergies=', daEnergies, ' => ', ierr)
    assert(ierr < 0)

    retNumD = dose.get_numEnergies()
    print(' get_numEnergies: => ', retNumD)
    assert(retNumD == 0)

    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    ierr = dose.set_energies(daEnergies, "MeV")
    print(' set_energies: daEnergies=', daEnergies, ' => ', ierr)
    assert(ierr == 0)

    retNumD = dose.get_numEnergies()
    print(' get_numEnergies: => ', retNumD)
    assert(retNumD == iNumE)


def test_set_depths(dose):
    daDepths = np.array([-0.2, 0.0, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0])
    iNumD = len(daDepths)
    ierr = dose.set_depths(daDepths, "mm")
    print(' set_depths: daDepths=', daDepths, ' => ', ierr)
    assert(ierr < 0)

    retNumD = dose.get_numDepths()
    print(' get_numDepths: => ', retNumD)
    assert(retNumD == 0)

    daDepths = np.array([0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0])
    iNumD = len(daDepths)
    ierr = dose.set_depths(daDepths, "mm")
    print(' set_depths: daDepths=', daDepths, ' => ', ierr)
    assert(ierr == 0)

    retNumD = dose.get_numDepths()
    print(' get_numDepths: => ', retNumD)
    assert(retNumD == iNumD)


def test_set_nuclearAttenMode(dose):
    for strString in ["nuc", "inter", "other"]:
        ierr = dose.set_nuclearAttenMode(strString)
        print(' set_nuclearAttenMode: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strRet = dose.get_nuclearAttenMode()
        print(' get_nuclearAttenMode: => ', strRet)
        assert(strRet == "None") 

    for strString in ["None", "Nuclear", "NuclearAndNeutrons"]:
        ierr = dose.set_nuclearAttenMode(strString)
        print(' set_nuclearAttenMode: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

        strRet = dose.get_nuclearAttenMode()
        print(' get_nuclearAttenMode: => ', strRet)
        assert(strRet == strString)


def test_set_withBrems(dose):
        ierr = dose.set_withBrems( )
        print (' set_withBrems: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        iVal = dose.get_withBrems( )
        print (' get_withBrems: => ', iVal)
        assert(iVal == 1)
        ierr = dose.set_withBrems( False )
        print (' set_withBrems: verdict=False => ', ierr)
        assert(ierr == 0)
        iVal = dose.get_withBrems( )
        print (' get_withBrems: => ', iVal)
        assert(iVal == 0)
        ierr = dose.set_withBrems( True )
        print (' set_withBrems: verdict=True => ', ierr)
        assert(ierr == 0)
        iVal = dose.get_withBrems( )
        print (' get_withBrems: => ', iVal)
        assert(iVal == 1)


def test_set_fluxEnvironOmni_computeFlux(dose):

    # use ephem model to generate test inputs
    ephem = EphemModel()

    ae9ap9 = Ae9Ap9Model()
    print('ae9ap9 dHandle=', ae9ap9.zHandle)

    print('ephem eHandle=', ephem.zHandle)
    strFile = '../../modelData/igrfDB.h5'
    ierr = ephem.set_magfieldDBFile(strFile)
    print(' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
    assert(ierr == 0)

    dValue = 25.0
    ierr = ephem.set_inclination(dValue)
    print(' set_inclination: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 0.25
    ierr = ephem.set_eccentricity(dValue)
    print(' set_eccentricity: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 8.5
    ierr = ephem.set_meanMotion(dValue)
    print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 218.5
    ierr = ephem.set_meanAnomaly(dValue)
    print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ephem.set_rightAscension(dValue)
    print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dMjd = 58119.5
    ierr = ephem.set_elementTime(dMjd)
    print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
    assert(ierr == 0)
    dStep = 60.0
    dStart = 58120.0
    dEnd = 58120.25
    ierr = ephem.set_times(dStart, dEnd, dStep)
    print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' %
          (dStart, dEnd, dStep, ierr))
    assert(ierr == 0)

    # get about two orbits of ephemeris
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)
    del ephem

    strFile = '../../modelData/AE9V15_runtime_tables.mat'
    ierr = ae9ap9.set_modelDBFile(strFile)
    print(' set_modelDBFile: dbFile=', strFile, ' => ', ierr)
    assert(ierr == 0)
    strFile = '../../modelData/fastPhi_net.mat'
    ierr = ae9ap9.set_kPhiDBFile(strFile)
    print(' set_kPhiDBFile: dbFile=', strFile, ' => ', ierr)
    assert(ierr == 0)
    strFile = '../../modelData/fast_hmin_net.mat'
    ierr = ae9ap9.set_kHMinDBFile(strFile)
    print(' set_kHMinDBFile: dbFile=', strFile, ' => ', ierr)
    assert(ierr == 0)
    strFile = '../../modelData/igrfDB.h5'
    ierr = ae9ap9.set_magfieldDBFile(strFile)
    print(' set_magfieldDBFile: dbFile=', strFile, ' => ', ierr)
    assert(ierr == 0)
    # cannot test with invalid energies - not caught at this level

    # set environment with this ephemeris
    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    strFluxType = '1PtDiff'
    ierr = ae9ap9.set_fluxEnvironOmni(strFluxType,
                                      daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                      daCoord1, daCoord2, daCoord3)
    print(' set_fluxEnvironOmni: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits)
    print('  returned ierr=', ierr)
    assert(ierr == 0)

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    strString = "../../modelData/sd2DB.h5"
    ierr = dose.set_modelDBFile(strString)
    print(' set_modelDBFile: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    ierr = dose.test_badHandle()
    print(' test_badHandle => ', ierr)
    assert(ierr < 0)

    strString = "electrons"
    ierr = dose.set_species(strString)
    print(' set_species: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "Ga"
    ierr = dose.set_detector(strString)
    print(' set_detector: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "spherical2pi"
    ierr = dose.set_geometry(strString)
    print(' set_geometry: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    ierr = dose.set_energies(daEnergies, "MeV")
    print(' set_energies: daEnergies=', daEnergies, ' => ', ierr)
    assert(ierr == 0)

    daDepths = np.array([0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0])
    iNumD = len(daDepths)
    ierr = dose.set_depths(daDepths, "mm")
    print(' set_depths: daDepths=', daDepths, ' => ', ierr)
    assert(ierr == 0)

    ierr, da3DoseRate = dose.computeFluxDoseRate(da3FluxData)
    print(' computeFluxDoseRate: da3FluxData.shape=',
          da3FluxData.shape, ' => ', ierr)
    print('    da3DoseRate.shape =', da3DoseRate.shape)
    assert(da3DoseRate.shape[0] == iNum)
    assert(da3DoseRate.shape[1] == iNumD)
    assert(da3DoseRate.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3DoseRate[0][0][0], da3DoseRate[0][1]
          [0], da3DoseRate[0][2][0], da3DoseRate[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3DoseRate[iNum-1][0][0], da3DoseRate[iNum-1][1]
          [0], da3DoseRate[iNum-1][2][0], da3DoseRate[iNum-1][iNumE-1][0])
    assert(ierr == 0)

    iMid = int(iNum/2)
    daFluxData = da3FluxData[iMid][:].flatten()

    ierr, daDose = dose.computeFluxDose(daFluxData)
    print(' computeFluxDose: daFluxData.shape=',
          daFluxData.shape, ' => ', ierr)
    print('    daDose.shape =', daDose.shape)
    assert(daDose.shape[0] == iNumD)
    print('   [', iMid, ']: ', daTimes[iMid],
          daCoord1[iMid], daCoord2[iMid], daCoord3[iMid])
    print('     ', daDose[0], daDose[1], daDose[2], daDose[iNumE-1])
    assert(ierr == 0)

    print('dose zHandle=', dose.zHandle)

    dose2 = DoseModel()
    print('dose2 zHandle=', dose2.zHandle)
    assert(dose2.zHandle > 0)
    assert(dose2.zHandle != dose.zHandle)

    print(' dose2: only defining ModelDBDir:')
    for strString in ["../../modelData/","goober","../../kernelXml/"]:
        ierr = dose2.set_modelDBDir(strString)
        print(' set_modelDBDir: strString=', strString, ' => ', ierr)
        if strString=="goober":
          assert(ierr < 0)
        else:
          assert(ierr == 0)

        strRet = dose2.get_modelDBDir()
        print(' get_modelDBDir: => ', strRet)
        if strString=="goober":
          assert(strRet == "")
        else:
          assert(strRet == strString)
    # last ModelDBDir is a valid directory, but incorrect
   
    strString = "electrons"
    ierr = dose2.set_species(strString)
    print(' set_species: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "Ga"
    ierr = dose2.set_detector(strString)
    print(' set_detector: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "spherical2pi"
    ierr = dose2.set_geometry(strString)
    print(' set_geometry: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    ierr = dose2.set_energies(daEnergies, "MeV")
    print(' set_energies: daEnergies=', daEnergies, ' => ', ierr)
    assert(ierr == 0)

    daDepths = np.array([0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0])
    iNumD = len(daDepths)
    ierr = dose2.set_depths(daDepths, "mm")
    print(' set_depths: daDepths=', daDepths, ' => ', ierr)
    assert(ierr == 0)

    ierr, da3DoseRate = dose2.computeFluxDoseRate(da3FluxData)
    print(' computeFluxDoseRate: da3FluxData.shape=',
          da3FluxData.shape, ' => ', ierr)
    assert(ierr < 0)
    strString = "../../modelData/"
    ierr = dose2.set_modelDBDir(strString)
    print(' set_modelDBDir: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strRet = dose2.get_modelDBDir()
    print(' get_modelDBDir: => ', strRet)
    assert(strRet == strString)
    print(' rerun with correct modelDBDir:')
    ierr, da3DoseRate = dose2.computeFluxDoseRate(da3FluxData)
    print(' computeFluxDoseRate: da3FluxData.shape=',
          da3FluxData.shape, ' => ', ierr)
    assert(ierr == 0)
    print('    da3DoseRate.shape =', da3DoseRate.shape)
    assert(da3DoseRate.shape[0] == iNum)
    assert(da3DoseRate.shape[1] == iNumD)
    assert(da3DoseRate.shape[2] == 1)
    print(' get_modelDBFile: => ', dose2.get_modelDBFile())



def test_second_instance(dose):
    # start a second instance of the ae9ap9eris object, verify not the same as the first
    dose2 = DoseModel()
    print('dose2 zHandle=', dose2.zHandle)
    assert(dose2.zHandle > 0)
    assert(dose2.zHandle != dose.zHandle)

    # use ephem model to generate test inputs
    ephem = EphemModel()

    ae9ap9 = Ae9Ap9Model()
    print('ae9ap9 dHandle=', ae9ap9.zHandle)

    print('ephem eHandle=', ephem.zHandle)

    print(' -- using ModelDBDir for all classes')
    strModelDBDir = "../../modelData/"
    ierr = ephem.set_modelDBDir( strModelDBDir )
    print(' ephem: set_modelDBDir: dbDir=', strModelDBDir, ' => ', ierr)
    assert(ierr == 0)
    print('        get_modelDBDir:    => ', ephem.get_modelDBDir() )

    dValue = 25.0
    ierr = ephem.set_inclination(dValue)
    print(' set_inclination: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 0.25
    ierr = ephem.set_eccentricity(dValue)
    print(' set_eccentricity: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 8.5
    ierr = ephem.set_meanMotion(dValue)
    print(' set_meanMotion: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 218.5
    ierr = ephem.set_meanAnomaly(dValue)
    print(' set_meanAnomaly: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dValue = 125.0
    ierr = ephem.set_rightAscension(dValue)
    print(' set_rightAscension: dValue=', dValue, ' => ', ierr)
    assert(ierr == 0)
    dMjd = 58119.5
    ierr = ephem.set_elementTime(dMjd)
    print(' set_elementTime: dMjd=', dMjd, ' => ', ierr)
    assert(ierr == 0)
    dStep = 60.0
    dStart = 58120.0
    dEnd = 58120.25
    ierr = ephem.set_times(dStart, dEnd, dStep)
    print(' set_times: start=%.1f, end=%.1f, step=%.0f => %i' %
          (dStart, dEnd, dStep, ierr))
    assert(ierr == 0)

    # get about two orbits of ephemeris
    strCoordSys = 'GDZ'
    strCoordUnits = 'km'
    iNum, daTimes, daCoord1, daCoord2, daCoord3 \
        = ephem.compute(strCoordSys, strCoordUnits)
    print(' compute: CoordSys=', strCoordSys, strCoordUnits)
    print('  returned iNum=', iNum, '; len(daTimes)=',
          len(daTimes), ', len(daCoord1)=', len(daCoord1))
    assert(iNum > 0)
    print('-- ephem: automatically defined via ModelDBDir:')
    print(' get_magfieldDBFile: => ', ephem.get_magfieldDBFile())
    del ephem

    ierr = ae9ap9.set_modelDBDir( strModelDBDir )
    print(' ae9ap9: set_modelDBDir: dbDir=', strModelDBDir, ' => ', ierr)
    assert(ierr == 0)
    print('         get_modelDBDir:    => ', ae9ap9.get_modelDBDir() )

    strModel = "AE9"
    ierr = ae9ap9.set_model( strModel )
    print(' ae9ap9: set_model: model=', strModel, ' => ', ierr)
    assert(ierr == 0)
    strRet = ae9ap9.get_model()
    print(' get_model: => ', strRet)
    assert(strRet == strModel)
    # cannot test with invalid energies - not caught at this level

    # set environment with this ephemeris
    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    strFluxType = '1PtDiff'
    ierr = ae9ap9.set_fluxEnvironOmni(strFluxType,
                                      daEnergies, daEnergies, daTimes, strCoordSys, strCoordUnits,
                                      daCoord1, daCoord2, daCoord3)
    print(' set_fluxEnvironOmni: ',
          ' FluxType=', strFluxType, ' Energies(#)=', iNumE,
          ' daTimes(#)=', len(daTimes), ', daCoord1(#)=', len(daCoord1),
          ' CoordSys=', strCoordSys, strCoordUnits)
    print('  returned ierr=', ierr)
    assert(ierr == 0)
    print('-- ae9ap9: automatically defined via ModelDBDir:')
    print(' get_modelDBFile: => ', ae9ap9.get_modelDBFile())
    print(' get_kPhiDBFile: => ', ae9ap9.get_kPhiDBFile())
    print(' get_kHMinDBFile: => ', ae9ap9.get_kHMinDBFile())
    print(' get_magfieldDBFile: => ', ae9ap9.get_magfieldDBFile())

    ierr, da3FluxData = ae9ap9.computeFluxMean()
    print(' computeFluxMean: ierr=', ierr)
    assert(ierr == 0)
    assert(da3FluxData.shape[0] == iNum)
    assert(da3FluxData.shape[1] == iNumE)
    assert(da3FluxData.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3FluxData[0][0][0], da3FluxData[0][1]
          [0], da3FluxData[0][2][0], da3FluxData[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3FluxData[iNum-1][0][0], da3FluxData[iNum-1][1]
          [0], da3FluxData[iNum-1][2][0], da3FluxData[iNum-1][iNumE-1][0])

    ierr = dose2.set_modelDBDir( strModelDBDir )
    print(' dose2: set_modelDBDir: dbDir=', strModelDBDir, ' => ', ierr)
    assert(ierr == 0)

    ierr = dose2.test_badHandle()
    print(' test_badHandle => ', ierr)
    assert(ierr < 0)

    strString = "electrons"
    ierr = dose2.set_species(strString)
    print(' set_species: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "Ga"
    ierr = dose2.set_detector(strString)
    print(' set_detector: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "spherical2pi"
    ierr = dose2.set_geometry(strString)
    print(' set_geometry: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    ierr = dose2.set_energies(daEnergies, "MeV")
    print(' set_energies: daEnergies=', daEnergies, ' => ', ierr)
    assert(ierr == 0)

    daDepths = np.array([0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0])
    iNumD = len(daDepths)
    ierr = dose2.set_depths(daDepths, "mm")
    print(' set_depths: daDepths=', daDepths, ' => ', ierr)
    assert(ierr == 0)

    ierr, da3DoseRate = dose2.computeFluxDoseRate(da3FluxData)
    print(' computeFluxDoseRate: da3FluxData.shape=',
          da3FluxData.shape, ' => ', ierr)
    print('    da3DoseRate.shape =', da3DoseRate.shape)
    assert(da3DoseRate.shape[0] == iNum)
    assert(da3DoseRate.shape[1] == iNumD)
    assert(da3DoseRate.shape[2] == 1)
    print('   [   0 ]: ', daTimes[0], daCoord1[0], daCoord2[0], daCoord3[0])
    print('     ', da3DoseRate[0][0][0], da3DoseRate[0][1]
          [0], da3DoseRate[0][2][0], da3DoseRate[0][iNumE-1][0])
    print('   [', iNum-1, ']: ', daTimes[iNum-1],
          daCoord1[iNum-1], daCoord2[iNum-1], daCoord3[iNum-1])
    print('     ', da3DoseRate[iNum-1][0][0], da3DoseRate[iNum-1][1]
          [0], da3DoseRate[iNum-1][2][0], da3DoseRate[iNum-1][iNumE-1][0])
    assert(ierr == 0)
    print('-- dose2: automatically defined via ModelDBDir:')
    print(' get_modelDBFile: => ', dose2.get_modelDBFile())

    iMid = int(iNum/2)
    daFluxData = da3FluxData[iMid][:].flatten()

    ierr, daDose = dose2.computeFluxDose(daFluxData)
    print(' computeFluxDose: daFluxData.shape=',
          daFluxData.shape, ' => ', ierr)
    print('    daDose.shape =', daDose.shape)
    assert(daDose.shape[0] == iNumD)
    print('   [', iMid, ']: ', daTimes[iMid],
          daCoord1[iMid], daCoord2[iMid], daCoord3[iMid])
    print('     ', daDose[0], daDose[1], daDose[2], daDose[iNumE-1])
    assert(ierr == 0)

    print('dose2 zHandle=', dose2.zHandle)

    del dose2

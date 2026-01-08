
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
from kernelModel import DoseKernel
import numpy as np
import pytest


@pytest.fixture
def dosek():
    dosek = DoseKernel()
    print('dosek zHandle=', dosek.zHandle)
    return dosek

# In general, each method is tested first with 'invalid' inputs, then valid ones.
# The associated 'get' methods are also tested, when available, in the same manner.
# Some also include tests with invalid 'handle' values for completeness.


def test_set_kernelXmlPath(dosek):
    strString = "../kernelXml"
    ierr = dosek.set_kernelXmlPath(strString)
    print(' set_kernelXmlPath: strString=', strString, ' => ', ierr)
    assert(ierr < 0)

    strXmlPath = dosek.get_kernelXmlPath()
    print(' get_kernelXmlPath: => ', strXmlPath)
    assert(strXmlPath == "")

    strString = "../../kernelXml"
    ierr = dosek.set_kernelXmlPath(strString)
    print(' set_kernelXmlPath: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strXmlPath = dosek.get_kernelXmlPath()
    print(' get_kernelXmlPath: => ', strXmlPath)
    assert(strXmlPath == strString)

    ierr = dosek.test_badHandle()
    print(' test_badHandle => ', ierr)
    assert(ierr < 0)


def test_set_kernelXmlFile(dosek):
    ierr = dosek.set_kernelXmlPath("../../kernelXml")
    assert(ierr == 0)

    strString = "Dose_electron_hemisphere_NO.xml"
    ierr = dosek.set_kernelXmlFile(strString)
    print(' set_kernelXmlFile: strString=', strString, ' => ', ierr)
    assert(ierr < 0)

    strXmlFile = dosek.get_kernelXmlFile()
    print(' get_kernelXmlFile: => ', strXmlFile)
    assert(strXmlFile == "")

    strString = "Dose_electron_hemisphere_GaAs.xml"
    ierr = dosek.set_kernelXmlFile(strString)
    print(' set_kernelXmlFile: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strXmlFile = dosek.get_kernelXmlFile()
    print(' get_kernelXmlFile: => ', strXmlFile)

    # split the string to get rid of the leading directory
    splitString = strXmlFile.split('/')
    # in python, -1 is the last element
    assert(splitString[-1] == strString)


def test_set_species(dosek):
    for strString in ["quarks", "muons", "neutrons"]:
        ierr = dosek.set_species(strString)
        print(' set_species: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strSpecies = dosek.get_species()
        print(' get_species: => ', strSpecies)
        assert(strSpecies == "electron")

    for strString in ["protons", "sprotons", "electrons"]:
        ierr = dosek.set_species(strString)
        print(' set_species: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

    strSpecies = dosek.get_species()
    print(' get_species: => ', strSpecies)
    assert(strSpecies == "electron")


def test_set_detector(dosek):
    for strString in ["He", "NaCl", "CalciumChloride"]:
        ierr = dosek.set_detector(strString)
        print(' set_detector: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        strDetector = dosek.get_detector()
        print(' get_detector: => ', strDetector)
        assert(strDetector == "Si")

    for strString in ["water", "h2o", "air", "lif", "CalciumFluoride", "Si", "Ga", "GaAs"]:
        ierr = dosek.set_detector(strString)
        print(' set_detector: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

    strDetector = dosek.get_detector()
    print(' get_detector: => ', strDetector)
    assert(strDetector == "GaAs")


def test_set_geometry(dosek):
    # attempt to set geometry with unknown ones
    for strString in ["square", "circle", "dodecahedron"]:
        ierr = dosek.set_geometry(strString)
        print(' set_geometry: strString=', strString, ' => ', ierr)
        assert(ierr < 0)

        # verify that the setting is still the default one
        strGeometry = dosek.get_geometry()
        print(' get_geometry: => ', strGeometry)
        assert(strGeometry == "Spherical2pi")

    #  use non-standard, but acceptable, specification strings
    for strString in ["semiinf", "finslab", "sph2pi", "sphere4pi"]:
        ierr = dosek.set_geometry(strString)
        print(' set_geometry: strString=', strString, ' => ', ierr)
        assert(ierr == 0)

    # compare against last one in list
    strGeometry = dosek.get_geometry()
    print(' get_geometry: => ', strGeometry)
    assert(strGeometry == "Spherical4pi")


def test_set_energies(dosek):
    daEnergies = np.array([-0.1, 0.0, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    ierr = dosek.set_energies(daEnergies, "MeV")
    print(' set_energies: daEnergies=', daEnergies, ' => ', ierr)
    assert(ierr < 0)

    daEnergies = np.array([0.1, 0.5, 1.0, 2.0, 3.5, 5.0, 8.0])
    iNumE = len(daEnergies)
    ierr = dosek.set_energies(daEnergies, "MeV")
    print(' set_energies: daEnergies=', daEnergies, ' => ', ierr)
    assert(ierr == 0)


def test_set_depths(dosek):
    daDepths = np.array([-0.2, 0.0, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0])
    ierr = dosek.set_depths(daDepths, "mm")
    print(' set_depths: daDepths=', daDepths, ' => ', ierr)
    assert(ierr < 0)

    daDepths = np.array([0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0])
    iNumD = len(daDepths)
    ierr = dosek.set_depths(daDepths, "mm")
    print(' set_depths: daDepths=', daDepths, ' => ', ierr)
    assert(ierr == 0)


# not implemented
'''def test_set_nuclearAttenMode( dosek ):
  for strString in ["no","nuc","neutron"]:
    ierr = dosek.set_nuclearAttenMode( strString )
    print (' set_nuclearAttenMode: strString=', strString, ' => ', ierr )
    assert( ierr < 0 )
  
    strNuclearAttenMode = dosek.get_nuclearAttenMode( )
    print (' get_nuclearAttenMode: => ', strNuclearAttenMode )
    assert( strNuclearAttenMode == "none" )
  
  for strString in ["Nuclear","NuclearNeutrons","NuclearInteractions"]:
    ierr = dosek.set_nuclearAttenMode( strString )
    print (' set_nuclearAttenMode: strString=', strString, ' => ', ierr )
    assert( ierr == 0 )
  
    strNuclearAttenMode = dosek.get_nuclearAttenMode( )
    print (' get_nuclearAttenMode: => ', strNuclearAttenMode )
    assert( strNuclearAttenMode == strString )'''


def test_set_withBrems(dosek):
        ierr = dosek.set_withBrems( )
        print (' set_withBrems: verdict=<unspecified> => ', ierr)
        assert(ierr == 0)
        iVal = dosek.get_withBrems( )
        print (' get_withBrems: => ', iVal)
        assert(iVal == 1)
        ierr = dosek.set_withBrems( False )
        print (' set_withBrems: verdict=False => ', ierr)
        assert(ierr == 0)
        iVal = dosek.get_withBrems( )
        print (' get_withBrems: => ', iVal)
        assert(iVal == 0)
        ierr = dosek.set_withBrems( True )
        print (' set_withBrems: verdict=True => ', ierr)
        assert(ierr == 0)
        iVal = dosek.get_withBrems( )
        print (' get_withBrems: => ', iVal)
        assert(iVal == 1)


def test_set_fluxEnvironOmni_computeFlux(dosek):

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

    strString = "../../kernelXml"
    ierr = dosek.set_kernelXmlPath(strString)
    print(' set_kernelXmlPath: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    ierr = dosek.test_badHandle()
    print(' test_badHandle => ', ierr)
    assert(ierr < 0)

#    strString = "Dose_electron_hemisphere_Ga.xml"
#    ierr = dosek.set_kernelXmlFile( strString )
#    print (' set_kernelXmlFile: strString=',strString,' => ',ierr )
#    assert( ierr==0 )

    strString = "electrons"
    ierr = dosek.set_species(strString)
    print(' set_species: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "Ga"
    ierr = dosek.set_detector(strString)
    print(' set_detector: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    strString = "sphere2pi"
    ierr = dosek.set_geometry(strString)
    print(' set_geometry: strString=', strString, ' => ', ierr)
    assert(ierr == 0)

    ierr = dosek.set_energies(daEnergies, "MeV")
    print(' set_energies: daEnergies=', daEnergies, ' => ', ierr)
    assert(ierr == 0)

    daDepths = np.array([0.2, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0])
    iNumD = len(daDepths)
    ierr = dosek.set_depths(daDepths, "mm")
    print(' set_depths: daDepths=', daDepths, ' => ', ierr)
    assert(ierr == 0)

    ierr, da3DoseRate = dosek.computeFluxDoseRate(da3FluxData)
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

    ierr, daDose = dosek.computeFluxDose(daFluxData)
    print(' computeFluxDose: daFluxData.shape=',
          daFluxData.shape, ' => ', ierr)
    print('    daDose.shape =', daDose.shape)
    assert(daDose.shape[0] == iNumD)
    print('   [', iMid, ']: ', daTimes[iMid],
          daCoord1[iMid], daCoord2[iMid], daCoord3[iMid])
    print('     ', daDose[0], daDose[1], daDose[2], daDose[iNumE-1])
    assert(ierr == 0)

    print('dosek zHandle=', dosek.zHandle)


def test_second_instance(dosek):
    # start a second instance of the ae9ap9eris object, verify not the same as the first
    dosek2 = DoseKernel()
    print('dosek2 zHandle=', dosek2.zHandle)
    assert(dosek2.zHandle > 0)
    assert(dosek2.zHandle != dosek.zHandle)
    del dosek2

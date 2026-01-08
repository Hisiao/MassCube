cmake_minimum_required (VERSION 3.14)

project(demoModelC LANGUAGES C CXX)

# IRENE_ROOT needs to be platform-specific subdirectory of
#  the install location (ie '<path>/Irene/linux')
if(NOT DEFINED IRENE_ROOT)
  message("Using ../ as default IRENE_ROOT")
  set(IRENE_ROOT "../")
endif()

# check for leading '~' that is not always recognized properly as home directory
string(FIND ${IRENE_ROOT} "~" ITILDE)
if(ITILDE EQUAL 0)
  get_filename_component( IRENE_ROOT ${IRENE_ROOT} REALPATH)
endif()

# if a relative path to IRENE_ROOT was passed, make it an absolute path
if(NOT IS_ABSOLUTE ${IRENE_ROOT})
  get_filename_component( IRENE_ROOT "${CMAKE_BINARY_DIR}/${IRENE_ROOT}" REALPATH)
endif()

# verify that the required 'include' and 'lib[64]' subdirectories of IRENE_ROOT are present
if(NOT EXISTS ${IRENE_ROOT}/include OR (NOT EXISTS ${IRENE_ROOT}/lib AND NOT EXISTS ${IRENE_ROOT}/lib64))
  message( "Error in IRENE_ROOT directory specification:\n    " ${IRENE_ROOT} )
  message( "This needs to be the platform-specific subdirectory\n   of the install location (ie '<path>/Irene/linux')")
  message(FATAL_ERROR "IRENE_ROOT directory specification error")
endif()

# Default install directory to be under 'IRENE_ROOT'
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${IRENE_ROOT} CACHE PATH "default install path" FORCE)
endif()

# define directories for include and library files
include_directories(${IRENE_ROOT}/include)
if(EXISTS ${IRENE_ROOT}/lib)
  link_directories(${IRENE_ROOT}/lib)
  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
else()
  link_directories(${IRENE_ROOT}/lib64)
  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib64")
endif()

# By default CMake strips the library location RPath from the executable at install. Setting it here now
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# set minimum version specifications for third-party library dependencies
# ==>> as needed, update/adjust these version numbers for third-party libraries <<==
if(UNIX)
  set(Boost_Version 1.53)

else() # WIN32
  set(Boost_Version 1.82.0)
  set(Boost_Version_Label 1_82_0)
  set(HDF5_Version 1.14.1)  
endif()

#  ====== Boost ====== 

if(WIN32)
  list(APPEND CMAKE_PREFIX_PATH "C:\\Boost\\Boost_${Boost_Version_Label}")
endif()
find_package(Boost ${Boost_Version} REQUIRED)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "boost library dependency error")
endif()
include_directories(BEFORE ${Boost_INCLUDE_DIRS})

#  ====== HDF5 ====== 

if(APPLE OR WIN32)
  set(HDF5_USE_STATIC_LIBRARIES True)
endif()
if(WIN32)
  list(APPEND CMAKE_PREFIX_PATH "$ENV{ProgramFiles}/HDF_Group/HDF5/${HDF5_Version}")
endif()  
find_package(HDF5)
include_directories(BEFORE ${HDF5_INCLUDE_DIRS})

# add Windows-specific dependencies to configuration
if(WIN32)
  find_library(ZLIB   libz    ${HDF5_ROOT} ) 
  find_library(AECLIB libaec  ${HDF5_ROOT} )  # SZIP library superseded by LIBAEC library
  list(APPEND HDF5_LIBRARIES ${ZLIB} ${AECLIB})
endif()
link_directories(${HDF5_LIBRARIES})

# define list of source files for build
set (demoModelSrc
     demoModelC.c)

# define executable source files
add_executable(DemoModelC ${demoModelSrc})

target_include_directories(DemoModelC PRIVATE ${IRENE_ROOT}/include)

# in Linux, we're linking to shared libraries
if(UNIX)
  target_link_libraries(DemoModelC ephemmodel  datetimeutil ae9ap9model adiabatmodel aggregmodel accummodel dosemodel dl)
endif(UNIX)

# in Windows, we're linking to static libraries and need to include all their dependencies as well
if(WIN32)
# this requires all libraries, at model level and lower, to be listed
  target_link_libraries(DemoModelC libephemmodel_c libdatetimeutil_c libae9ap9model_c libadiabatmodel_c libaggregmodel_c libaccummodel_c libdosemodel_c)
endif(WIN32)

install(TARGETS DemoModelC
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

#------------------------------------------------------------
# Linux shared object linking
#   class          model and dependent libraries
# EphemModel   ephemmodel CTimeValue dl
# Ae9Ap9Model  ae9ap9model CTimeValue dl
# LegacyModel  legacymodel CTimeValue dl
# AccumModel   accummodel CTimeValue
# DoseModel    dosemodel dl
# AggregModel  aggregmodel
# DateTimeUtil datetimeutil CTimeValue
#
# Windows static object linking
# EphemModel   ephemmodel orbitprop cmagfield fio CoordXform CTimeValue ${HDF5_LIBRARIES} dl
# Ae9Ap9Model  ae9ap9model adiabatmodel cmagfield adiabat fio CoordXform CTimeValue Threading ${HDF5_LIBRARIES} dl rt pthread
# LegacyModel  legacymodel cmagfield radenv shielddose2 cammice fio CoordXform CTimeValue ${HDF5_LIBRARIES} dl
# AccumModel   accummodel CTimeValue
# DoseModel    dosemodel fio shielddose2 ${HDF5_LIBRARIES} dl
# AggregModel  aggregmodel
# DateTimeUtil datetimeutil_shared CTimeValue

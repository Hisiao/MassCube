cmake_minimum_required (VERSION 3.14)

project(demoAppC LANGUAGES C CXX)

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

# do not build MPI version unless specified
if(NOT DEFINED USE_MPI)
  set(USE_MPI 0)
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

# define list of source files for build
if(WIN32)
  set(demoAppCSrc
      demoAppC.c
      getoptWindows.c)
else()
  set(demoAppCSrc
      demoAppC.c)
endif()

# define executable source files
add_executable(DemoAppC ${demoAppCSrc})
if(USE_MPI)
  add_executable(DemoAppMpiC ${demoAppCSrc})
endif()

# define libraries to link for executable(s)
if(WIN32)
  target_link_libraries(DemoAppC libapplication_c fileio taskproc inputproc datetimeutil CTimeValue)
  if(USE_MPI)
    target_link_libraries(DemoAppMpiC libapplicationmpi_c fileio taskprocmpi inputproc datetimeutil CTimeValue)
  endif()
else()
  target_link_libraries(DemoAppC application fileio taskproc inputproc datetimeutil CTimeValue)
  if(USE_MPI)
    target_link_libraries(DemoAppMpiC applicationmpi fileio taskprocmpi inputproc datetimeutil CTimeValue)
  endif()
endif()

# define installation properties for executable
install(TARGETS DemoAppC
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
if(USE_MPI)
  install(TARGETS DemoAppMpiC
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
endif()

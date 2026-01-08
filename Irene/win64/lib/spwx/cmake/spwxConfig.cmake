
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was Config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set(spwx_INCLUDE_DIR "${PACKAGE_PREFIX_DIR}/include")
set(spwx_DATA_DIR "${PACKAGE_PREFIX_DIR}/data")
set(spwx_COMMON_INCLUDE_DIR "C:/Users/croth/Desktop/BuildNew/IrenePubRel/temp/SpWx/Common/include")
set(spwx_MODELS_INCLUDE_DIR "C:/Users/croth/Desktop/BuildNew/IrenePubRel/temp/SpWx/Models/include")

set(spwx_INCLUDE_DIRS ${spwx_INCLUDE_DIR} ${spwx_COMMON_INCLUDE_DIR} ${spwx_MODELS_INCLUDE_DIR})


include(CMakeFindDependencyMacro)
find_dependency(HDF5)

include("${CMAKE_CURRENT_LIST_DIR}/spwx.cmake")
check_required_components("spwx")

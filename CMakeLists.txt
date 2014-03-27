CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(microblx_kdl_types)

set(CMAKE_CXX_FLAGS "-Wall -Werror -fvisibility=hidden")
set(CMAKE_CXX_COMPILER clang++ )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake") ## temporary resources, for UBX

# Global
set(INSTALL_LIB_TYPES_DIR lib/microblx/types CACHE PATH "Installation directory for types libraries")
set(INSTALL_LIB_BLOCKS_DIR lib/microblx/blocks CACHE PATH "Installation directory for blocks libraries")
set(INSTALL_INCLUDE_DIR include/microblx CACHE PATH "Installation directory for header files (types)")
set(INSTALL_CMAKE_DIR  share/microblx/cmake CACHE PATH  "Installation directory for CMake files") # default ${DEF_INSTALL_CMAKE_DIR}
# set(INSTALL_LIB_BLOCKS_DIR "${CMAKE_INSTALL_PREFIX}/lib/microblx/blocks" CACHE PATH "Installation directory for blocks libraries")
# set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/microblx/types" CACHE PATH "Installation directory for header files (types)")
# set(INSTALL_CMAKE_DIR  "${CMAKE_INSTALL_PREFIX}/share/microblx/cmake" CACHE PATH  "Installation directory for CMake files") # default ${DEF_INSTALL_CMAKE_DIR}

# Make relative paths absolute (needed later on)
foreach(p LIB_TYPES LIB_BLOCKS BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# Local (remove me if you don't need)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib/ CACHE PATH "Configure the library output path.")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin/ CACHE PATH "Configure the executable output path.")

find_package(PkgConfig)
pkg_check_modules(KDL orocos-kdl)

set (UBX_ROOT $ENV{UBX_ROOT} )
if (NOT UBX_ROOT)
 message( FATAL_ERROR "UBX_ROOT is not defined. CMake install willl abort." )
endif()

find_package(Microblx REQUIRED)

##
# Add uninstall target.
##
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
    
add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
)

##
# Generate config package
##
set(CONF_INCLUDE_DIRS "${INSTALL_INCLUDE_DIR}")
set(CONF_CMAKE_DIR "${INSTALL_CMAKE_DIR}")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/kdl-types-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/kdl-types-config.cmake"
     @ONLY
)


link_directories(
    ${KDL_LIBRARY_DIRS}
)
include_directories(
    ${KDL_INCLUDE_DIRS}
)

include_directories(${KDL_INCLUDE_DIR})
include_directories(${UBX_INCLUDE_DIR})

set(GEN_HEXARR ${UBX_ROOT}/tools/file2carr.lua)

file(GLOB types
  ${CMAKE_CURRENT_SOURCE_DIR}/types/*.h
)


set(outfiles "")
foreach( _type ${types} )
  string(REPLACE ".h" ".h.hexarr" _outfile ${_type})
  add_custom_command(
  OUTPUT ${_outfile}
  COMMAND ${GEN_HEXARR}
  ARGS ${_type}  > ${_outfile}
  )
  list(APPEND outfiles ${_outfile})
endforeach(_type)
add_custom_target( expanded_defs ALL DEPENDS ${outfiles} )

add_library(kdl_types SHARED kdl_types.c)
set_target_properties(kdl_types PROPERTIES PREFIX "") 
set_property(TARGET kdl_types PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(kdl_types ${UBX_LIBRARIES})

### Install libraries
FILE(GLOB types_headers ${CMAKE_CURRENT_SOURCE_DIR}/types/*.h
                        ${CMAKE_CURRENT_SOURCE_DIR}/types/*.hexarr     
)


set_target_properties(kdl_types PROPERTIES PUBLIC_HEADER "${types_headers}")

# install(TARGETS kdl_types DESTINATION lib/microblx/types EXPORT kdl-types-config)
# install(FILES ${types_headers} DESTINATION include/microblx/types)
# install(EXPORT kdl-types-config DESTINATION share/microblx/cmake)

# install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/kdl-types-config.cmake"
#   DESTINATION 
#   COMPONENT dev
# )

install(TARGETS kdl_types 
  EXPORT kdl-types-targets
  LIBRARY DESTINATION "${INSTALL_LIB_TYPES_DIR}" COMPONENT kdl_types
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/types" COMPONENT dev
)

export(PACKAGE kdl-types)

# Install the kdl-types-config.cmake 
install(FILES
  "${PROJECT_BINARY_DIR}/cmake/kdl-types-config.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# # Install the export set for use with the install-tree
install(EXPORT kdl-types-targets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
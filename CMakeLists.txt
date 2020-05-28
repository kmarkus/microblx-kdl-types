# -*- cmake -*-

CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

project(microblx-kdl)

SET(UBXKDL_VERSION 1.0.0)

find_package(PkgConfig REQUIRED) 

#find_package(orocos_kdl REQUIRED)
#include_directories(${orocos_kdl_INCLUDE_DIRS})
#link_directories(${orocos_kdl_LIBRARY_DIRS})

pkg_check_modules(UBX REQUIRED ubx0>=0.8.1)
pkg_get_variable(UBX_MODDIR ubx0 UBX_MODDIR)
find_program(TOCARR ubx-tocarr)

# generate type hexarrays
file(GLOB type_hdrs ${CMAKE_CURRENT_SOURCE_DIR}/src/types/*.h )
set(hexarr_outdir "${CMAKE_CURRENT_BINARY_DIR}/hexarr")
file(MAKE_DIRECTORY "${hexarr_outdir}/types")
set(hexarrays "")

foreach( type ${type_hdrs} )
  get_filename_component(type_name ${type} NAME)
  message("processing ${type} as ${type_name}")
  add_custom_command(
    OUTPUT "${hexarr_outdir}/types/${type_name}.hexarr"
    DEPENDS ${type}
    COMMAND ${TOCARR}
    ARGS -s ${type} -d "${hexarr_outdir}/types/${type_name}.hexarr")
  list(APPEND hexarrays "${hexarr_outdir}/types/${type_name}.hexarr")
endforeach(type)

# kdl_types
add_library(kdl_types SHARED src/kdl_types.c ${hexarrays})

set_target_properties(kdl_types PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH TRUE
  PUBLIC_HEADER "${type_hdrs}"
  PREFIX "")

target_compile_options(kdl_types PUBLIC ${UBX_CFLAGS} -I ${hexarr_outdir})
target_link_libraries(kdl_types ${UBX_LDFLAGS})
install(TARGETS kdl_types
  LIBRARY DESTINATION ${UBX_MODDIR}
  PUBLIC_HEADER DESTINATION "include/ubxkdl")

add_library(ubxkdl INTERFACE)
set_target_properties(ubxkdl PROPERTIES
  PUBLIC_HEADER src/ubxkdl.hpp)

install(TARGETS ubxkdl PUBLIC_HEADER DESTINATION "include")

# pkg-config
set(PREFIX "${CMAKE_INSTALL_PREFIX}")
configure_file(src/ubxkdl.pc.in ${CMAKE_CURRENT_BINARY_DIR}/ubxkdl.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ubxkdl.pc
  DESTINATION "lib${LIB_SUFFIX}/pkgconfig")

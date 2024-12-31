# This file is part of the [clay_citro2d_renderer] project.
#
# (c) 2024 Tommaso Dimatore
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

cmake_minimum_required(VERSION 3.21)

project(CC2D
  VERSION 1.0.0 
  DESCRIPTION "Single-header Clay renderer for the 3DS using Citro2D"
  HOMEPAGE_URL "https://github.com/sonodima/clay_citro2d_renderer"
  LANGUAGES C)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(CC2D INTERFACE)
add_library(CC2D::CC2D ALIAS CC2D)
target_include_directories(CC2D
INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/CC2DConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/CC2DConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/CC2D")
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/CC2DConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT)

install(TARGETS CC2D EXPORT CC2DTargets)
install(EXPORT CC2DTargets
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/CC2D"
  NAMESPACE CC2D::
  FILE "CC2DTargets.cmake")

install(FILES
  "${PROJECT_BINARY_DIR}/CC2DConfigVersion.cmake"
  "${PROJECT_BINARY_DIR}/CC2DConfig.cmake"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/CC2D")
install(DIRECTORY "include/" TYPE INCLUDE)



# set_property(TARGET CC2D PROPERTY EXPORT_NAME CC2D)

# target_compile_features(CC2D INTERFACE c_std_11)
# target_include_directories(CC2D
# INTERFACE
#   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

# if(PROJECT_IS_TOP_LEVEL)
  


#   include(CPack)
# endif()


# # option(CC2D_INSTALL "Include an Install Target" ${CC2D_IS_TOP_LEVEL})
# # option(CC2D_BUILD_EXAMPLES "Build Examples" OFF)


# add_library(CC2D INTERFACE)
# # target_compile_features(CC2D INTERFACE c_std_11)
# target_include_directories(CC2D
#   INTERFACE
#     $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
#     $<INSTALL_INTERFACE:include>)

# if(CC2D_BUILD_EXAMPLES)
#   add_subdirectory(examples)
# endif()

# if(CC2D_INSTALL)
#   install(TARGETS CC2D EXPORT CC2DConfig)
#   install(EXPORT CC2DConfig
#     NAMESPACE CC2D::
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
#   install(FILES "${CMAKE_CURRENT_LIST_DIR}/include/clay_citro2d_renderer.h"
#     DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CC2D")


# endif()

# include(CPack)

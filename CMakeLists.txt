# This file is part of the Clay3DS project.
#
# (c) 2024 Tommaso Dimatore
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

cmake_minimum_required(VERSION 3.21)

project(clay3ds
  VERSION 1.0.0 
  DESCRIPTION "Single-header Clay renderer for the 3DS using Citro2D"
  HOMEPAGE_URL "https://github.com/sonodima/clay3ds"
  LANGUAGES C)

# ================================
# Options
# ================================

option(CLAY3DS_BUILD_EXAMPLES "Build Examples" OFF)

# ================================
# Installation
# ================================

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(clay3ds INTERFACE)
add_library(clay3ds::clay3ds ALIAS clay3ds)
target_include_directories(clay3ds
INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/Clay3DSConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/Clay3DSConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/Clay3DS")
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/Clay3DSConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT)

install(TARGETS clay3ds EXPORT Clay3DSTargets)
install(EXPORT Clay3DSTargets
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/Clay3DS"
  NAMESPACE clay3ds::
  FILE "Clay3DSTargets.cmake")

install(FILES
  "${PROJECT_BINARY_DIR}/Clay3DSConfigVersion.cmake"
  "${PROJECT_BINARY_DIR}/Clay3DSConfig.cmake"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/Clay3DS")
install(DIRECTORY "include/" TYPE INCLUDE)

# ================================
# Examples
# ================================

if(CLAY3DS_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

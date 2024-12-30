# This file is part of the [clay_citro2d_renderer] project.
#
# (c) 2024 Tommaso Dimatore
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

cmake_minimum_required(VERSION 3.14)

project(CC2D
  VERSION 1.0.0 
  DESCRIPTION "Single-header Clay renderer for the 3DS using Citro2D"
  HOMEPAGE_URL "https://github.com/sonodima/clay_citro2d_renderer"
  LANGUAGES C)

option(CC2D_BUILD_EXAMPLES "Build Examples" OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(CC2D INTERFACE)

target_compile_features(CC2D INTERFACE c_std_11)
target_include_directories(CC2D INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if(CC2D_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

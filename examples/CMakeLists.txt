# This file is part of the Clay3DS project.
#
# (c) 2025 Tommaso Dimatore
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

cmake_minimum_required(VERSION 3.14)

project(clay3dse
  DESCRIPTION "Examples for the Clay3DS project"
  LANGUAGES C)

# ================================
# Dependencies
# ================================

include(FetchContent)

FetchContent_Declare(Clay
  GIT_REPOSITORY "https://github.com/nicbarker/clay.git"
  GIT_TAG        "main")
FetchContent_GetProperties(Clay)
if(NOT clay_POPULATED)
  FetchContent_Populate(Clay)
endif()

# ================================
# Examples Definitions
# ================================

function(add_sample SAMPLE_NAME)
  set(SAMPLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_NAME}")

  # Automatically find all the sources in the sample's directory.
  file(GLOB SOURCES CONFIGURE_DEPENDS "${SAMPLE_DIR}/*.c" "${SAMPLE_DIR}/*.h")

  add_executable(clay3dse_${SAMPLE_NAME})
  target_sources(clay3dse_${SAMPLE_NAME} PRIVATE "${SOURCES}")
  target_compile_features(clay3dse_${SAMPLE_NAME} INTERFACE c_std_99)
  target_include_directories(clay3dse_${SAMPLE_NAME} PRIVATE "${clay_SOURCE_DIR}")
  target_link_libraries(clay3dse_${SAMPLE_NAME} PRIVATE citro2d citro3d clay3ds)
  set_target_properties(clay3dse_${SAMPLE_NAME} PROPERTIES OUTPUT_NAME "${SAMPLE_NAME}")

  # Only set the path to ROMFS if the example actually makes use of it.
  set(ROMFS_PATH "${SAMPLE_DIR}/romfs")
  IF(NOT IS_DIRECTORY "${ROMFS_PATH}")
    set(ROMFS_PATH "")
  endif()

  set(SMDH_PATH "${SAMPLE_NAME}.smdh")
  ctr_generate_smdh("${SMDH_PATH}" NAME "Clay3DS Example")
  ctr_create_3dsx(clay3dse_${SAMPLE_NAME} SMDH "${SMDH_PATH}" ROMFS "${ROMFS_PATH}")
endfunction()

add_sample(custom_fonts)
add_sample(dual_screen)
add_sample(minimal)

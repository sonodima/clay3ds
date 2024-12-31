# This file is part of the Clay3DS project.
#
# (c) 2024 Tommaso Dimatore
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

cmake_minimum_required(VERSION 3.21)

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

function(add_sample FILE)
  add_executable("clay3dse_${FILE}")
  target_sources("clay3dse_${FILE}" PRIVATE "${FILE}.c")
  target_include_directories("clay3dse_${FILE}" PRIVATE "${clay_SOURCE_DIR}")
  target_link_libraries("clay3dse_${FILE}" PRIVATE citro2d citro3d clay3ds)

  set_target_properties("clay3dse_${FILE}" PROPERTIES OUTPUT_NAME "${FILE}")
  ctr_generate_smdh("${FILE}.smdh" NAME "Clay3DS Example")
  ctr_create_3dsx("clay3dse_${FILE}" SMDH "${FILE}.smdh")
endfunction()

add_sample(minimal)
add_sample(two_screens)

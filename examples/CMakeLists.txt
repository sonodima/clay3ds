# This file is part of the [clay_citro2d_renderer] project.
#
# (c) 2024 Tommaso Dimatore
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

cmake_minimum_required(VERSION 3.21)

project(CC2DE LANGUAGES C)

function(add_sample NAME)
  add_executable(CC2DE_${NAME})

  target_sources(CC2DE_${NAME} PRIVATE "${NAME}.c")
  target_include_directories(CC2DE_${NAME} PRIVATE "../include" "${clay_SOURCE_DIR}")
  target_link_libraries(CC2DE_${NAME} PRIVATE citro2d citro3d)

  set_target_properties(CC2DE_${NAME} PROPERTIES OUTPUT_NAME "${NAME}")
  ctr_create_3dsx(CC2DE_${NAME})
endfunction()

add_sample(minimal)
add_sample(two_screens)

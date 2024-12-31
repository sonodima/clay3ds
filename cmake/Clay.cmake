# This file is part of the [clay_citro2d_renderer] project.
#
# (c) 2024 Tommaso Dimatore
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

cmake_minimum_required(VERSION 3.21)

project(CC2DE LANGUAGES C)

include(FetchContent)

# Fetch the Clay source code from its repository for the examples
FetchContent_Declare(Clay
  GIT_REPOSITORY https://github.com/nicbarker/clay.git
  GIT_TAG        main)
FetchContent_GetProperties(Clay)
if(NOT clay_POPULATED)
  FetchContent_Populate(Clay)
endif()

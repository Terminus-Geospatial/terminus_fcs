#**************************** INTELLECTUAL PROPERTY RIGHTS ****************************#
#*                                                                                    *#
#*                           Copyright (c) 2025 Terminus LLC                          *#
#*                                                                                    *#
#*                                All Rights Reserved.                                *#
#*                                                                                    *#
#*          Use of this source code is governed by LICENSE in the repo root.          *#
#*                                                                                    *#
#**************************** INTELLECTUAL PROPERTY RIGHTS ****************************#
#
#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    11/20/2025
#
#    Purpose:  Build demo package
cmake_minimum_required( VERSION 4.0.0 FATAL_ERROR )

#  Configure Conan Search Paths
include( "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )

project( FCS_Package_Test )

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# Use local terminus_fcs library
add_executable( example example.cpp )

target_link_libraries( example terminus_fcs )

target_include_directories( example PRIVATE ${CMAKE_SOURCE_DIR}/include )

############################# INTELLECTUAL PROPERTY RIGHTS #############################
##                                                                                    ##
##                           Copyright (c) 2025 Terminus LLC                          ##
##                                All Rights Reserved.                                ##
##                                                                                    ##
##          Use of this source code is governed by LICENSE in the repo root.          ##
##                                                                                    ##
############################# INTELLECTUAL PROPERTY RIGHTS #############################
#
#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    11/21/2025
#
#    Purpose:  Build Terminus Framework Configuration Service (FCS)
#

#  Set CMake Version
cmake_minimum_required( VERSION 4.0.0 FATAL_ERROR )

#  Configure Conan Search Paths
include( "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )

# Set the Project-Name
project( ${CONAN_PKG_NAME}
            VERSION ${CONAN_PKG_VERSION}
            HOMEPAGE_URL ${CONAN_PKG_URL}
            DESCRIPTION ${CONAN_PKG_DESCRIPTION}
            LANGUAGES CXX
)

#  Set C++ 23 Support
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

#  Give Vscode a fighting chance
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# Bring in CMake helpers for TERMINUS projects
include( terminus_cmake )

# Force the high warning preset published by terminus_cmake across this project
set( TERMINUS_CXX_FLAGS ${TERMINUS_CXX_FLAGS_HIGH} CACHE STRING "Terminus warning preset" FORCE )

#  Treat warnings as errors
set( CMAKE_COMPILE_WARNING_AS_ERROR ON )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )

#----------------------------------------#
#-          Update Version File         -#
#----------------------------------------#
# Get the current date
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d %H:%M:%S" )

configure_file( ${CMAKE_SOURCE_DIR}/templates/exports.hpp.in
                ${CMAKE_BINARY_DIR}/include/terminus/fcs/exports.hpp )

#-------------------------------#
#-      Find Dependencies      -#
#-------------------------------#
find_package( terminus_ipc     REQUIRED )
find_package( terminus_log     REQUIRED )
find_package( terminus_outcome REQUIRED )

#-------------------------------------#
#-          Build Libraries          -#
#-------------------------------------#
add_subdirectory( library )

terminus_lib_configure( ${PROJECT_NAME} )

#  Enable code coverage
if( TERMINUS_IPC_LIB_CPP_ENABLE_COVERAGE )
     terminus_coverage_enable()
endif()

if( TERMINUS_IPC_LIB_CPP_ENABLE_TESTS )
     enable_testing()
     add_subdirectory( test/unit )
endif()

install( DIRECTORY library/include/terminus DESTINATION include)
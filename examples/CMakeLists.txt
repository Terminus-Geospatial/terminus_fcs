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
#    Date:    11/22/2025
#
#    Purpose:  Build Terminus FCS examples
#

#-------------------------------------#
#-          Build Examples           -#
#-------------------------------------#
add_executable( demo_app_01 demo_app_01.cpp )

target_link_libraries( demo_app_01 PUBLIC
    ${PROJECT_NAME}
)
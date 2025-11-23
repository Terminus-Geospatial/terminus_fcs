############################# INTELLECTUAL PROPERTY RIGHTS #############################
##                                                                                    ##
##                           Copyright (c) 2024 Terminus LLC                          ##
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
#    Purpose:  Build Terminus FCS library
#

include_directories( ${PROJECT_SOURCE_DIR}/library/include )

add_library( ${PROJECT_NAME} SHARED
    include/terminus/fcs/cmdline/args.hpp
    include/terminus/fcs/cmdline/log_level.hpp
    include/terminus/fcs/prop/property.hpp
    include/terminus/fcs/schema/builder.hpp
    include/terminus/fcs/schema/constraint_iface.hpp
    include/terminus/fcs/schema/custom_constraint.hpp
    include/terminus/fcs/schema/enum_constraint.hpp
    include/terminus/fcs/schema/property_value_type.hpp
    include/terminus/fcs/schema/range_constraint.hpp
    include/terminus/fcs/schema/schema.hpp
    include/terminus/fcs/configuration.hpp
    include/terminus/fcs/datastore.hpp
    src/cmdline/args.cpp
    src/cmdline/log_level.cpp
    src/prop/property.cpp
    src/schema/builder.cpp
    src/schema/custom_constraint.cpp
    src/schema/enum_constraint.cpp
    src/schema/property_value_type.cpp
    src/schema/schema.cpp
    src/configuration.cpp
    src/datastore.cpp
)

target_link_libraries( ${PROJECT_NAME} PUBLIC
    terminus_ipc::terminus_ipc
    terminus_log::terminus_log
    terminus_outcome::terminus_outcome
)

install( TARGETS ${PROJECT_NAME} )


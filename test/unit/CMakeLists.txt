#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/21/2023

#  Configure GoogleTest
find_package( GTest REQUIRED )
include( GoogleTest )

include_directories( ${CMAKE_SOURCE_DIR}/library/include )
include_directories( ${CMAKE_SOURCE_DIR}/library/src )

set( TEST ${PROJECT_NAME}_test )
add_executable( ${TEST}
    main.cpp
    TEST_config_file_parser.cpp
    TEST_datastore.cpp
    TEST_property.cpp
    TEST_schema.cpp
)

target_link_libraries( ${TEST} PRIVATE
    GTest::gtest
    ${PROJECT_NAME}
)

gtest_discover_tests( ${TEST} PROPERTIES TIMEOUT 600 )

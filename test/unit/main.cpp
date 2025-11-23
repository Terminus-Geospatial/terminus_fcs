/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2025 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    main.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
 */
#include <gtest/gtest.h>

#include <terminus/log/configure.hpp>

int main(int argc, char **argv)
{
    // Setup the Terminus Logger
    std::istringstream config{R"(
                [Sinks.Console]
                Destination=Console
                Filter="%Severity% >= debug"
                Format="%Severity% %Scope% %Message% %File%"
            )" };
    tmns::log::configure( config );

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


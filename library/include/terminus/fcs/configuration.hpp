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
 * @file    configuration.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// Terminus Libraries
#include <terminus/error.hpp>

namespace tmns::fcs {

/**
 * Parse command line arguments and environment variables to configure the FCS.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 * @param envp Array of environment variable strings.
 * @return Configuration result containing the datastore configuration.
 */
Result<Datastore> parse_command_line( int argc, char* argv[], char* envp[] );


} // End of tmns::fcs namespace
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

// C++ Standard Libraries
#include <optional>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/fcs/datastore.hpp>
#include <terminus/fcs/schema/schema.hpp>

namespace tmns::fcs {

/**
 * Parse command line arguments and environment variables to configure the FCS.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 * @param envp Array of environment variable strings.
 * @param schema Optional schema for validation. If provided, the datastore will be validated before returning.
 * @return Configuration result containing the validated datastore configuration.
 */
Result<Datastore> parse_command_line( int                           argc,
                                      char*                         argv[],
                                      char*                         envp[],
                                      std::optional<schema::Schema> schema = std::nullopt );

/**
 * Parse command line arguments with explicit config file path.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 * @param envp Array of environment variable strings.
 * @param config_file Path to configuration file.
 * @param schema Optional schema for validation. If provided, the datastore will be validated before returning.
 * @return Configuration result containing the validated datastore configuration.
 */
Result<Datastore> parse_command_line_with_config( int                           argc,
                                                  char*                         argv[],
                                                  char*                         envp[],
                                                  const std::string&            config_file,
                                                  std::optional<schema::Schema> schema = std::nullopt );

} // End of tmns::fcs namespace
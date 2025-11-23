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
 * @file    configuration.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#include <terminus/fcs/configuration.hpp>

// C++ Standard Libraries
#include <filesystem>
#include <iostream>

// Terminus Libraries
#include <terminus/fcs/cmdline/args.hpp>

namespace tmns::fcs {

/************************************************/
/*          Parse Command-Line Options          */
/************************************************/
Result<Datastore> parse_command_line( int argc,
                                      char* argv[],
                                      char* envp[],
                                      std::optional<schema::Schema> schema )
{
    // Parse command-line arguments using Args class
    auto args_result = cmdline::Args::parse( argc, argv, envp );
    if( !args_result ) {
        return args_result.error();
    }
    const auto& args = args_result.value();

    // Get config file path (if provided)
    std::string config_file;
    if( args.get_config_path() ) {
        config_file = args.get_config_path().value();
    }

    // If config file found, delegate to the with_config function
    if( !config_file.empty() ) {
        return parse_command_line_with_config( argc, argv, envp, config_file, schema );
    }

    // Create the initial datastore
    Datastore datastore;

    // Apply command-line properties to datastore
    for( const auto& [key, value] : args.get_properties() ) {
        auto set_result = datastore.set_property( key, value );
        if( !set_result ) {
            return outcome::fail( error::Error_Code::INVALID_INPUT,
                                  "Failed to set property '" + key + "': " + set_result.error().message() );
        }
    }

    // TODO: Parse environment variables

    // Apply schema validation if provided
    if( schema.has_value() ) {
        // Apply schema to the root
        auto schema_result = datastore.set_schema( "", schema );
        if( !schema_result ) {
            return outcome::fail( error::Error_Code::SCHEMA_NOT_FOUND,
                                  "Failed to apply schema: " + schema_result.error().message() );
        }

        // Validate the entire datastore
        auto validate_result = datastore.validate_property( "" );
        if( !validate_result ) {
            return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                                  "Configuration validation failed: " + validate_result.error().message() );
        }
    }

    return outcome::ok<Datastore>( std::move( datastore ) );
}

/************************************************/
/*          Parse Command-Line Options          */
/************************************************/
Result<Datastore> parse_command_line_with_config( int                           argc,
                                                  char*                         argv[],
                                                  char*                         envp[],
                                                  const std::string&            config_file,
                                                  std::optional<schema::Schema> schema )
{
    // Parse command-line arguments first to get the highest priority settings
    auto args_result = cmdline::Args::parse( argc, argv, envp );
    if( !args_result ) {
        return args_result.error();
    }
    const auto& args = args_result.value();

    // Create the initial datastore
    Datastore datastore;

    // Step 1: Apply config file properties first (lower priority)
    if( !config_file.empty() ) {
        std::cout << "Config file specified: " << config_file << std::endl;
        // TODO: Parse the TOML config file here
        // For now, this is a placeholder
        // Config file properties would be applied here
    }

    // Step 2: Apply command-line properties (higher priority - overrides config file)
    for( const auto& [key, value] : args.get_properties() ) {
        auto set_result = datastore.set_property( key, value );
        if( !set_result ) {
            return outcome::fail( error::Error_Code::INVALID_INPUT,
                                  "Failed to set property '" + key + "': " + set_result.error().message() );
        }
    }

    // Step 3: Parse environment variables (lowest priority - only if not set elsewhere)
    // TODO: Parse environment variables here

    // Apply schema validation if provided
    if( schema.has_value() ) {
        // Apply schema to the root
        auto schema_result = datastore.set_schema( "", schema );
        if( !schema_result ) {
            return outcome::fail( error::Error_Code::SCHEMA_NOT_FOUND,
                                  "Failed to apply schema: " + schema_result.error().message() );
        }

        // Validate the entire datastore
        auto validate_result = datastore.validate_property( "" );
        if( !validate_result ) {
            return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                                  "Configuration validation failed: " + validate_result.error().message() );
        }
    }

    return outcome::ok<Datastore>( std::move( datastore ) );
}

} // namespace tmns::fcs
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2025 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/*************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    args.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#include <terminus/fcs/cmdline/args.hpp>

// C++ Standard Libraries
#include <filesystem>
#include <iostream>
#include <sstream>

namespace tmns::fcs::cmdline {

/******************************/
/*          Parse Args         */
/******************************/
Result<Args> Args::parse( int argc, char* argv[], char* envp[] )
{
    Args args;

    // Suppress unused parameter warnings for now
    (void)envp;

    if( argc < 1 ) {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                              "No arguments provided" );
    }

    // Store the application path
    args.m_app_path = std::filesystem::path( argv[0] );

    // Parse command line arguments
    for( int i = 1; i < argc; i++ ) {
        std::string arg = argv[i];

        // Help flags
        if( arg == "--help" || arg == "-h" ) {
            args.print_help();
            return outcome::ok<Args>( args ); // Early return after help
        }

        // Config file flag
        else if( arg == "-c" || arg == "--config" ) {
            if( i + 1 >= argc ) {
                return outcome::fail( error::Error_Code::INVALID_INPUT,
                                      "Config file path required after " + arg );
            }
            args.m_config_path = argv[++i];
        }

        // Log severity flags
        else if( arg == "--trace" ) {
            args.m_log_level = Log_Level::TRACE;
        }
        else if( arg == "--debug" ) {
            args.m_log_level = Log_Level::DEBUG;
        }
        else if( arg == "--info" ) {
            args.m_log_level = Log_Level::INFO;
        }
        else if( arg == "--warning" ) {
            args.m_log_level = Log_Level::WARNING;
        }
        else if( arg == "--error" ) {
            args.m_log_level = Log_Level::ERROR;
        }

        // Property setting (-p key=value)
        else if( arg == "-p" || arg == "--property" ) {
            if( i + 1 >= argc ) {
                return outcome::fail( error::Error_Code::INVALID_INPUT,
                                      "Property key=value required after " + arg );
            }
            std::string prop_arg = argv[++i];
            auto prop_result = args.parse_property_argument( prop_arg );
            if( !prop_result ) {
                return outcome::fail( error::Error_Code::INVALID_INPUT,
                                      "Invalid property argument: " + prop_arg );
            }
        }

        // Generate config flag
        else if( arg == "-g" || arg == "--gen-config" ) {
            args.print_usage();
            return outcome::ok<Args>( args ); // Early return after usage
        }

        // Unknown argument
        else {
            return outcome::fail( error::Error_Code::INVALID_INPUT,
                                  "Unknown argument: " + arg );
        }
    }

    return outcome::ok<Args>( args );
}

/******************************/
/*        Parse Property       */
/******************************/
Result<void> Args::parse_property_argument( const std::string& prop_arg )
{
    // Look for key=value format
    auto eq_pos = prop_arg.find( '=' );
    if( eq_pos == std::string::npos ) {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                              "Property must be in key=value format: " + prop_arg );
    }

    std::string key = prop_arg.substr( 0, eq_pos );
    std::string value = prop_arg.substr( eq_pos + 1 );

    if( key.empty() ) {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                              "Property key cannot be empty" );
    }

    m_properties[key] = value;
    return outcome::ok();
}

/******************************/
/*          Print Help         */
/******************************/
void Args::print_help() const
{
    std::cout << "Usage: " << m_app_path.filename().string() << " [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -c, --config FILE        Path to configuration file\n";
    std::cout << "  -p, --property KEY=VALUE Set a configuration property\n";
    std::cout << "                           (supports nested keys like app.database.host)\n";
    std::cout << "  --trace                   Set log level to TRACE\n";
    std::cout << "  --debug                   Set log level to DEBUG\n";
    std::cout << "  --info                    Set log level to INFO\n";
    std::cout << "  --warning                 Set log level to WARNING\n";
    std::cout << "  --error                   Set log level to ERROR\n";
    std::cout << "  -g, --gen-config          Generate default configuration file\n";
    std::cout << "  -h, --help                Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << m_app_path.filename().string() << " -c config.toml\n";
    std::cout << "  " << m_app_path.filename().string() << " -c config.toml -p app.database.host=localhost\n";
    std::cout << "  " << m_app_path.filename().string() << " -p app.server.workers=4 --debug\n";
}

/******************************/
/*          Print Usage        */
/******************************/
void Args::print_usage() const
{
    std::cout << "Usage: " << m_app_path.filename().string() << " [OPTIONS]\n";
    std::cout << "Use --help for detailed usage information.\n";
}

/******************************/
/*        Get Config Path      */
/******************************/
std::optional<std::string> Args::get_config_path() const
{
    return m_config_path;
}

/******************************/
/*        Get Log Level        */
/******************************/
std::optional<Log_Level> Args::get_log_level() const
{
    return m_log_level;
}

/******************************/
/*        Get Properties       */
/******************************/
const std::map<std::string, std::string>& Args::get_properties() const
{
    return m_properties;
}

/******************************/
/*        Get App Path         */
/******************************/
std::filesystem::path Args::get_app_path() const
{
    return m_app_path;
}

} // namespace tmns::fcs::cmdline
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
 * @file    config_file_parser_impl.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/

// C++ Standard Libraries
#include <fstream>
#include <iostream>
#include <sstream>

// Third-party Libraries
#include <toml.hpp>

// Terminus Libraries
#include "config_file_parser_impl.hpp"

namespace tmns::fcs::impl {

/*********************************/
/*        Parse TOML File        */
/*********************************/
Result<void> Config_File_Parser_Impl::parse_file( const std::filesystem::path&    config_path,
                                                  Datastore&                      datastore,
                                                  [[maybe_unused]] std::optional<schema::Schema>   schema )
{
    // Check if file exists and is readable
    if( !is_file_readable( config_path ) ) {
        return outcome::fail( error::Error_Code::FILE_NOT_FOUND,
                              "Config file not found or not readable: " + config_path.string() );
    }

    try {
        // Read the file content first
        std::ifstream file( config_path );
        if( !file.is_open() ) {
            return outcome::fail( error::Error_Code::FILE_NOT_FOUND,
                                  "Could not open config file: " + config_path.string() );
        }

        std::string content( (std::istreambuf_iterator<char>(file) ),
                             std::istreambuf_iterator<char>() );

        // Parse the TOML content as string
        std::istringstream stream( content );

        // Try parsing with more specific error handling
        try {
            auto toml_data = toml::parse( stream );

            // If it's a table, parse it recursively but don't set the table itself as a property
            if( toml_data.is_table() ) {
                auto result = parse_toml_table( "", *toml_data.as_table(), datastore );
                if( !result ) {
                    return result;
                }
            }

            return outcome::ok();
        }
        catch( const toml::parse_error& e ) {
            return outcome::fail( error::Error_Code::PARSING_ERROR,
                                  "Failed to parse TOML file: " + std::string(e.what()) );
        }
    }
    catch( const toml::parse_error& e ) {
        return outcome::fail( error::Error_Code::PARSING_ERROR,
                              "Failed to parse TOML file: " + std::string(e.what()) );
    }
    catch( const std::exception& e ) {
        return outcome::fail( error::Error_Code::UNKNOWN,
                              "Unexpected error parsing TOML file: " + std::string(e.what()) );
    }
}

/*********************************/
/*      Parse TOML String       */
/*********************************/
Result<void> Config_File_Parser_Impl::parse_string( const std::string&              config_content,
                                                    Datastore&                      datastore,
                                                    [[maybe_unused]] std::optional<schema::Schema>   schema )
{
    try {
        // Parse the TOML string
        std::istringstream stream( config_content );
        auto toml_data = toml::parse( stream );

        // If it's a table, parse it recursively but don't set the table itself as a property
        if( toml_data.is_table() ) {
            auto result = parse_toml_table( "", *toml_data.as_table(), datastore );
            if( !result ) {
                return result;
            }
        }

        return outcome::ok();
    }
    catch( const toml::parse_error& e ) {
        return outcome::fail( error::Error_Code::PARSING_ERROR,
                              "Failed to parse TOML string: " + std::string(e.what()) );
    }
    catch( const std::exception& e ) {
        return outcome::fail( error::Error_Code::UNKNOWN,
                              "Unexpected error parsing TOML string: " + std::string(e.what()) );
    }
}

/*********************************/
/*     Check File Readable      */
/*********************************/
bool Config_File_Parser_Impl::is_file_readable( const std::filesystem::path& config_path )
{
    return std::filesystem::exists( config_path ) &&
           std::filesystem::is_regular_file( config_path );
}

/*********************************/
/*      Find Config File       */
/*********************************/
std::filesystem::path Config_File_Parser_Impl::find_config_file( const std::string& config_name )
{
    // Check current directory first
    auto current_path = std::filesystem::current_path() / config_name;
    if( is_file_readable( current_path ) ) {
        return current_path;
    }

    // Check home directory
    auto home_path = std::filesystem::path( std::getenv("HOME") ) / config_name;
    if( is_file_readable( home_path ) ) {
        return home_path;
    }

    // Check /etc directory
    auto etc_path = std::filesystem::path("/etc") / config_name;
    if( is_file_readable( etc_path ) ) {
        return etc_path;
    }

    return {}; // Return empty path if not found
}

/*********************************/
/*    Parse TOML Value          */
/*********************************/
Result<void> Config_File_Parser_Impl::parse_toml_value( const std::string&  key,
                                                        const toml::node&   value,
                                                        Datastore&          datastore )
{
    // Convert TOML value to std::any
    auto any_result = toml_value_to_any( value );
    if( !any_result ) {
        return any_result.error();
    }

    // Set the property in the datastore
    auto set_result = datastore.set_property( key, any_result.value() );
    if( !set_result ) {
        return set_result;
    }

    return outcome::ok();
}

/*********************************/
/*     Parse TOML Table         */
/*********************************/
Result<void> Config_File_Parser_Impl::parse_toml_table( const std::string&   key_prefix,
                                                        const toml::table&   table,
                                                        Datastore&           datastore )
{
    for( const auto& [key, value] : table ) {
        std::string full_key = key_prefix.empty() ? std::string(key) : key_prefix + "." + std::string(key);

        if( value.is_table() ) {
            // Recursively parse nested tables
            auto result = parse_toml_table( full_key, *value.as_table(), datastore );
            if( !result ) {
                return result;
            }
        }
        else if( value.is_array() ) {
            // Handle arrays
            auto result = parse_toml_array( full_key, *value.as_array(), datastore );
            if( !result ) {
                return result;
            }
        }
        else {
            // Handle scalar values - only set these as properties
            std::cout << "Setting property: " << full_key << std::endl;
            auto result = parse_toml_value( full_key, value, datastore );
            if( !result ) {
                std::cout << "Failed to set property " << full_key << ": " << result.error().message() << std::endl;
                return result;
            }
        }
    }

    return outcome::ok();
}

/*********************************/
/*     Parse TOML Array         */
/*********************************/
Result<void> Config_File_Parser_Impl::parse_toml_array( const std::string& key,
                                                          const toml::array& array,
                                                          Datastore& datastore )
{
    // For now, just convert the array to a vector of std::any
    std::vector<std::any> array_values;

    for( const auto& element : array ) {
        auto any_result = toml_value_to_any( element );
        if( !any_result ) {
            return any_result.error();
        }
        array_values.push_back( any_result.value() );
    }

    // Set the array property
    auto set_result = datastore.set_property( key, array_values );
    if( !set_result ) {
        return set_result;
    }

    return outcome::ok();
}

/*********************************/
/*    Convert TOML to Any       */
/*********************************/
Result<std::any> Config_File_Parser_Impl::toml_value_to_any( const toml::node& value )
{
    if( value.is_string() ) {
        return outcome::ok<std::any>( std::any( *value.as_string() ) );
    }
    else if( value.is_integer() ) {
        return outcome::ok<std::any>( std::any( *value.as_integer() ) );
    }
    else if( value.is_floating_point() ) {
        return outcome::ok<std::any>( std::any( *value.as_floating_point() ) );
    }
    else if( value.is_boolean() ) {
        return outcome::ok<std::any>( std::any( *value.as_boolean() ) );
    }
    else {
        return outcome::fail( error::Error_Code::TYPE_MISMATCH,
                              "Unsupported TOML value type" );
    }
}

/*********************************/
/*    Validate Property         */
/*********************************/
Result<void> Config_File_Parser_Impl::validate_property( const std::string& /*key*/,
                                                         const std::any& /*value*/,
                                                         std::optional<schema::Schema> schema )
{
    if( !schema.has_value() ) {
        return outcome::ok(); // No validation needed
    }

    // For now, just basic type checking
    // TODO: Implement full schema validation
    return outcome::ok();
}

} // End of tmns::fcs::impl namespace

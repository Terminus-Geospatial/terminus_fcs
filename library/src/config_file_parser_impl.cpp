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
#include <filesystem>

// Third-party Libraries
#include <toml.hpp>

// Terminus Libraries
#include "config_file_parser_impl.hpp"
#include <terminus/fcs/prop/typed_property.hpp>
#include <terminus/fcs/prop/object_property.hpp>
#include <terminus/fcs/prop/array_property.hpp>

namespace tmns::fcs::impl {

/*********************************/
/*  Split Path Helper           */
/*********************************/
std::vector<std::string> split_path( const std::string& path )
{
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string part;

    while( std::getline(ss, part, '.') ) {
        parts.push_back(part);
    }

    return parts;
}

/*********************************/
/*  Create Property from TOML    */
/*********************************/
Result<std::shared_ptr<prop::Property>> create_property_from_toml( const std::string& key,
                                                                    const toml::node& value )
{
    std::shared_ptr<prop::Property> property;

    // Determine property type based on TOML value
    if( value.is_string() ) {
        // For now, treat all strings as regular strings
        // TODO: Add path detection logic based on key name or content
        property = std::make_shared<prop::String_Property>(key);
    }
    else if( value.is_integer() ) {
        property = std::make_shared<prop::Integer_Property>(key);
    }
    else if( value.is_floating_point() ) {
        // Use Double_Property for TOML floating point values
        property = std::make_shared<prop::Double_Property>(key);
    }
    else if( value.is_boolean() ) {
        auto bool_val = *value.as_boolean();
        property = std::make_shared<prop::Boolean_Property>(key);
    }
    else if( value.is_array() ) {
        property = std::make_shared<prop::Array_Property>(key);
    }
    else if( value.is_table() ) {
        property = std::make_shared<prop::Object_Property>(key);
    }
    else {
        return outcome::fail( error::Error_Code::TYPE_MISMATCH,
                              "Unsupported TOML value type for key: " + key );
    }

    return outcome::ok<std::shared_ptr<prop::Property>>(property);
}

/*********************************/
/*  Ensure Property Exists       */
/*********************************/
Result<void> ensure_property_exists( const std::string& key,
                                     const toml::node& value,
                                     Datastore& datastore )
{
    // Try to get the property first
    auto get_result = datastore.get_property( key );
    if( get_result ) {
        // Property already exists, nothing to do
        return outcome::ok();
    }

    // Property doesn't exist, create it
    auto create_result = create_property_from_toml( key, value );
    if( !create_result ) {
        return create_result.error();
    }

    auto property = create_result.value();

    // Split the key path to handle hierarchical properties
    auto path_parts = split_path( key );

    if( path_parts.size() == 1 ) {
        // Simple property at root level
        auto root = datastore.get_root();
        auto add_result = root->add_property( property );
        if( !add_result ) {
            return add_result;
        }
    }
    else {
        // Nested property - create parent objects as needed
        auto current_obj = datastore.get_root();

        // Create all parent objects except the last one (which is the actual property)
        for( size_t i = 0; i < path_parts.size() - 1; i++ ) {
            const std::string& parent_key = path_parts[i];

            // Check if parent object exists
            auto parent_result = current_obj->get_property( parent_key );
            std::shared_ptr<prop::Property> parent_prop;

            if( parent_result ) {
                parent_prop = parent_result.value();
            } else {
                // Create parent object
                parent_prop = std::make_shared<prop::Object_Property>( parent_key );
                auto add_parent = current_obj->add_property( parent_prop );
                if( !add_parent ) {
                    return add_parent;
                }
            }

            // Move to the next level
            current_obj = std::dynamic_pointer_cast<prop::Object_Property>( parent_prop );
            if( !current_obj ) {
                return outcome::fail( error::Error_Code::TYPE_MISMATCH,
                                      "Expected object property at: " + parent_key );
            }
        }

        // Create a new property with just the leaf name (not the full path)
        std::string leaf_name = path_parts.back();
        auto leaf_result = create_property_from_toml( leaf_name, value );
        if( !leaf_result ) {
            return leaf_result.error();
        }

        // Add the final property to the deepest nested object
        auto add_result = current_obj->add_property( leaf_result.value() );
        if( !add_result ) {
            return add_result;
        }
    }

    return outcome::ok();
}

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
    // Ensure the property exists before trying to set its value
    auto ensure_result = ensure_property_exists( key, value, datastore );
    if( !ensure_result ) {
        return ensure_result;
    }

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
    // Ensure the array property exists
    auto ensure_result = ensure_property_exists( key, array, datastore );
    if( !ensure_result ) {
        return ensure_result;
    }

    // Get the array property
    auto get_result = datastore.get_property( key );
    if( !get_result ) {
        return get_result.error();
    }

    auto array_prop = std::dynamic_pointer_cast<prop::Array_Property>( get_result.value() );
    if( !array_prop ) {
        return outcome::fail( error::Error_Code::TYPE_MISMATCH,
                              "Expected array property for key: " + key );
    }

    // Add each element to the array
    for( const auto& element : array ) {
        auto any_result = toml_value_to_any( element );
        if( !any_result ) {
            return any_result.error();
        }

        // Create a temporary property for the array element
        std::string element_key = "element_" + std::to_string(array_prop->size());
        auto element_prop = create_property_from_toml( element_key, element );
        if( !element_prop ) {
            return element_prop.error();
        }

        // Set the element value
        auto set_element = element_prop.value()->set_value( any_result.value() );
        if( !set_element ) {
            return set_element;
        }

        // Add the element to the array
        auto add_result = array_prop->add_item( element_prop.value() );
        if( !add_result ) {
            return add_result;
        }
    }

    return outcome::ok();
}

/*********************************/
/*    Convert TOML to Any       */
/*********************************/
Result<std::any> Config_File_Parser_Impl::toml_value_to_any( const toml::node& value )
{
    if( value.is_string() ) {
        // Convert TOML string to regular string
        std::string str_val = static_cast<std::string>(*value.as_string());
        return outcome::ok<std::any>( std::any( str_val ) );
    }
    else if( value.is_integer() ) {
        // Convert TOML integer to int64_t
        int64_t int_val = static_cast<int64_t>(*value.as_integer());
        return outcome::ok<std::any>( std::any( int_val ) );
    }
    else if( value.is_floating_point() ) {
        // Convert TOML floating point to double
        double double_val = static_cast<double>(*value.as_floating_point());
        return outcome::ok<std::any>( std::any( double_val ) );
    }
    else if( value.is_boolean() ) {
        // Convert TOML boolean to regular bool
        bool bool_val = static_cast<bool>(*value.as_boolean());
        return outcome::ok<std::any>( std::any( bool_val ) );
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

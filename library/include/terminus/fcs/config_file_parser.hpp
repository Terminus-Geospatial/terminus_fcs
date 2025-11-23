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
 * @file    config_file_parser.hpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#pragma once

// C++ Standard Libraries
#include <filesystem>
#include <string>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/fcs/datastore.hpp>
#include <terminus/fcs/schema/schema.hpp>

namespace tmns::fcs {

/**
 * Configuration File Parser for TOML files
 */
class Config_File_Parser {

    public:

        /**
         * Constructor
         */
        Config_File_Parser() = default;

        /**
         * Parse a TOML configuration file and populate the datastore
         *
         * @param config_path Path to the TOML configuration file
         * @param datastore Datastore to populate with configuration values
         * @param schema Optional schema for validation
         * @return Result indicating success or failure
         */
        Result<void> parse_file( const std::filesystem::path&    config_path,
                                 std::shared_ptr<Datastore>      datastore,
                                 std::shared_ptr<schema::Schema> schema = nullptr );

        /**
         * Parse a TOML configuration string and populate the datastore
         *
         * @param config_content TOML configuration content as string
         * @param datastore Datastore to populate with configuration values
         * @param schema Optional schema for validation
         * @return Result indicating success or failure
         */
        Result<void> parse_string( const std::string&              config_content,
                                   std::shared_ptr<Datastore>      datastore,
                                   std::shared_ptr<schema::Schema> schema = nullptr );

        /**
         * Check if a file exists and is readable
         *
         * @param config_path Path to check
         * @return True if file exists and is readable
         */
        static bool is_file_readable( const std::filesystem::path& config_path );

        /**
         * Find configuration file in standard locations
         *
         * @param config_name Name of the config file (e.g., "config.toml")
         * @return Path to the found config file or empty path if not found
         */
        static std::filesystem::path find_config_file( const std::string& config_name );

    private:

        /**
         * Parse a TOML value and convert to appropriate property type
         *
         * @param key Property key path
         * @param value TOML value
         * @param datastore Datastore to populate
         * @return Result indicating success or failure
         */
        Result<void> parse_toml_value( const std::string&            key,
                                       const toml::value&            value,
                                       std::shared_ptr<Datastore>    datastore );

        /**
         * Parse a TOML table (object) and recursively populate datastore
         *
         * @param key_prefix Prefix for property keys
         * @param table TOML table to parse
         * @param datastore Datastore to populate
         * @return Result indicating success or failure
         */
        Result<void> parse_toml_table( const std::string&   key_prefix,
                                       const toml::table&   table,
                                       Datastore&           datastore );

        /**
         * Parse a TOML array and populate datastore
         *
         * @param key Property key path
         * @param array TOML array to parse
         * @param datastore Datastore to populate
         * @return Result indicating success or failure
         */
        outcome::Result<void> parse_toml_array(
            const std::string& key,
            const toml::array& array,
            std::shared_ptr<Datastore> datastore );

        /**
         * Convert TOML value to std::any
         *
         * @param value TOML value to convert
         * @return Converted value as std::any
         */
        outcome::Result<std::any> toml_value_to_any( const toml::value& value );

        /**
         * Validate a property against schema if provided
         *
         * @param key Property key
         * @param value Property value
         * @param schema Schema for validation
         * @return Result indicating success or failure
         */
        outcome::Result<void> validate_property(
            const std::string& key,
            const std::any& value,
            std::shared_ptr<schema::Schema> schema );

}; // End of Config_File_Parser class

} // End of tmns::fcs namespace

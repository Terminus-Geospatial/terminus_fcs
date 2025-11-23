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
 * @file    config_file_parser_impl.hpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#pragma once

// C++ Standard Libraries
#include <filesystem>
#include <memory>
#include <string>

// Third-party Libraries
#include <toml.hpp>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/fcs/datastore.hpp>
#include <terminus/fcs/schema/schema.hpp>

namespace tmns::fcs::impl {

/**
 * Implementation class for Config_File_Parser
 * This contains all the toml-specific implementation details
 */
class Config_File_Parser_Impl {
    public:

        /**
         * Parse a TOML configuration file and populate the datastore
         */
        Result<void> parse_file( const std::filesystem::path&    config_path,
                                 Datastore&                      datastore,
                                 std::optional<schema::Schema>   schema );

        /**
         * Parse a TOML configuration string and populate the datastore
         */
        Result<void> parse_string( const std::string&              config_content,
                                   Datastore&                      datastore,
                                   std::optional<schema::Schema>   schema );

        /**
         * Check if a file exists and is readable
         */
        static bool is_file_readable( const std::filesystem::path& config_path );

        /**
         * Find configuration file in standard locations
         */
        static std::filesystem::path find_config_file( const std::string& config_name );

    private:

        /**
         * Parse a TOML value and convert to appropriate property type
         */
        Result<void> parse_toml_value( const std::string&  key,
                                       const toml::node&   value,
                                       Datastore&          datastore );

        /**
         * Parse a TOML table (object) and recursively populate datastore
         */
        Result<void> parse_toml_table( const std::string&   key_prefix,
                                       const toml::table&   table,
                                       Datastore&           datastore );

        /**
         * Parse a TOML array and populate datastore
         */
        Result<void> parse_toml_array( const std::string& key,
                                      const toml::array& array,
                                      Datastore& datastore );

        /**
         * Convert TOML value to std::any
         */
        Result<std::any> toml_value_to_any( const toml::node& value );

        /**
         * Validate a property against schema if provided
         */
        Result<void> validate_property( const std::string& key,
                                        const std::any& value,
                                        std::optional<schema::Schema> schema );
};

} // End of tmns::fcs::impl namespace

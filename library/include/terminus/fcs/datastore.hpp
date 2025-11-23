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
 * @file    datastore.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <memory>
#include <optional>
#include <string>
#include <vector>

// Terminus Libraries
#include <terminus/outcome/result.hpp>

// Project Libraries
#include <terminus/fcs/prop/object_property.hpp>
#include <terminus/fcs/schema/schema.hpp>

namespace tmns::fcs {

/**
 * Main datastore for managing property trees
 */
class Datastore {

    public:

        /**
         * Default constructor
         */
        Datastore();

        /**
         * Constructor with root property
         */
        explicit Datastore(std::shared_ptr<prop::Object_Property> root);

        // Core property operations
        Result<void> set_property( const std::string& path, const std::any& value );
        Result<std::shared_ptr<prop::Property>> get_property( const std::string& path ) const;
        Result<void> remove_property( const std::string& path );

        /**
         * Set the Schema for a property
         */
        Result<void> set_schema( const std::string& path, std::optional<schema::Schema> schema );

        /**
         * Get the Schema for a property
         */
        Result<std::optional<schema::Schema>> get_schema( const std::string& path ) const;

        /**
         * Validate a property
         */
        Result<void> validate_property( const std::string& path ) const;

        /**
         * Validate the entire datastore
         */
        Result<void> validate_all() const;

        /**
         * Parse a key-value pair
         */
        Result<void> parse_key_value_pair( const std::string& key_value_str );

        /**
         * Parse command-line arguments
         */
        Result<void> parse_command_line_args( const std::vector<std::string>& args );

        /**
         * List properties
         */
        Result<std::vector<std::string>> list_properties( const std::string& base_path = "" ) const;

        /**
         * Check if a property exists
         */
        Result<bool> has_property( const std::string& path ) const;

        /**
         * Root access
         */
        std::shared_ptr<prop::Object_Property> get_root() const { return m_root; }

        /**
         * Set the root property
         */
        void set_root(std::shared_ptr<prop::Object_Property> root) { m_root = root; }

        /**
         * Clear the datastore
         */
        void clear();

        /**
         * Get the size of the datastore
         */
        size_t size() const;

    private:
        std::shared_ptr<prop::Object_Property> m_root;

        // Helper methods
        std::pair<std::string, std::string> parse_key_value( const std::string& input ) const;
        Result<std::shared_ptr<prop::Property>> create_property_for_value( const std::string& key, const std::string& value ) const;
        std::shared_ptr<prop::Property> infer_property_from_string( const std::string& key, const std::string& value ) const;
        std::vector<std::string> split_path( const std::string& path ) const;

}; // End of Datastore class

} // namespace tmns::fcs
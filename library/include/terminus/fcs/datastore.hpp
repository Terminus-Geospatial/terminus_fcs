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
#include <string>
#include <vector>

// Terminus Libraries
#include <terminus/outcome/result.hpp>

// Project Libraries
#include <terminus/fcs/props/property.hpp>
#include <terminus/fcs/schemas/property_schema.hpp>

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
        explicit Datastore(std::shared_ptr<ObjectProperty> root);

        // Core property operations
        Result<void> set_property(const std::string& path, const std::any& value);
        Result<std::shared_ptr<Property>> get_property(const std::string& path) const;
        Result<void> remove_property(const std::string& path);

        // Schema operations
        Result<void> set_schema(const std::string& path, std::shared_ptr<PropertySchema> schema);
        Result<std::shared_ptr<PropertySchema>> get_schema(const std::string& path) const;
        Result<void> validate_property(const std::string& path) const;
        Result<void> validate_all() const;

        // Parsing operations
        Result<void> parse_key_value_pair(const std::string& key_value_str);
        Result<void> parse_command_line_args(const std::vector<std::string>& args);

        // Tree operations
        Result<std::vector<std::string>> list_properties(const std::string& base_path = "") const;
        Result<bool> has_property(const std::string& path) const;

        // Root access
        std::shared_ptr<ObjectProperty> get_root() const { return m_root; }
        void set_root(std::shared_ptr<ObjectProperty> root) { m_root = root; }

        // Utility
        void clear();
        size_t size() const;

    private:
        std::shared_ptr<ObjectProperty> m_root;

        // Helper methods
        std::pair<std::string, std::string> parse_key_value(const std::string& input) const;
        Result<std::shared_ptr<Property>> create_property_for_value(const std::string& key, const std::string& value) const;
        std::shared_ptr<Property> infer_property_from_string(const std::string& key, const std::string& value) const;
        std::vector<std::string> split_path(const std::string& path) const;

} // namespace tmns::fcs
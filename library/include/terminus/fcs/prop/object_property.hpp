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
 * @file    object_property.hpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#pragma once

// C++ Standard Libraries
#include <any>

// Terminus Libraries
#include <terminus/error.hpp>

// Project Libraries
#include <terminus/fcs/prop/property.hpp>

namespace tmns::fcs::prop {

/**
 * Property that represents a nested object (tree node)
 */
class Object_Property : public Property,
                        public std::enable_shared_from_this<Object_Property>
{
    public:

        /**
         * Default constructor
         */
        Object_Property() = default;

        /**
         * Constructor with key
         */
        explicit Object_Property( const std::string& key );

        /**
         * Set the value of the property
         */
        Result<void> set_value( const std::any& value ) override;

        /**
         * Get the value of the property
         */
        Result<std::any> get_value() const override;

        /**
         * Validate the property
         */
        Result<void> validate() const override;

        /**
         * Add a property to the object
         */
        Result<void> add_property(std::shared_ptr<Property> property);

        /**
         * Get a property from the object
         */
        Result<std::shared_ptr<Property>> get_property(const std::string& key) const;

        /**
         * Remove a property from the object
         */
        Result<void> remove_property(const std::string& key);

        /**
         * Resolve a path to a property
         */
        Result<std::shared_ptr<Property>> resolve_path(const std::string& path) const;

        /**
         * Set a value at a path
         */
        Result<void> set_path_value(const std::string& path, const std::any& value);

        /**
         * Get the child keys
         */
        std::vector<std::string> get_child_keys() const;

        /**
         * Check if the object has children
         */
        bool has_children() const { return !m_children.empty(); }

        /**
         * Get the type of the property
         */
        PropertyValueType get_type() const override { return PropertyValueType::OBJECT; }

        /**
         * Get the type string of the property
         */
        std::string get_type_string() const override;

    private:
        std::unordered_map<std::string, std::shared_ptr<Property>> m_children;

        std::vector<std::string> split_path(const std::string& path) const;
};

} // namespace tmns::fcs::prop
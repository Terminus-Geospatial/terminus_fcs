/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2025 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/***************************# INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    schema.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <any>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/fcs/schema/property_value_type.hpp>
#include <terminus/fcs/schema/constraint_iface.hpp>

namespace tmns::fcs::prop {
class Property;
} // end of tmns::fcs prop namespace

namespace tmns::fcs::schema {

/**
 * Schema definition for a property
 */
class Schema {

    public:

        Schema() = default;

        explicit Schema( Property_Value_Type type);

        // Basic type information
        Property_Value_Type get_type() const { return m_type; }
        void set_type(Property_Value_Type type) { m_type = type; }

        // Required/optional
        bool is_required() const { return m_required; }
        void set_required(bool required) { m_required = required; }

        // Default value
        bool has_default() const { return m_has_default; }
        Result<std::any> get_default() const;
        void set_default_value(const std::any& default_value);

        // Constraints
        void add_constraint( std::shared_ptr<Constraint_Iface> constraint );
        const std::vector<std::shared_ptr<Constraint_Iface>>& get_constraints() const;

        // Validation
        Result<void> validate(const std::any& value) const;
        Result<void> validate_property( const prop::Property& property ) const;

        // Schema composition for objects and arrays
        void add_property_schema( const std::string& key,
                                  std::shared_ptr<Schema> schema );

        std::shared_ptr<Schema> get_property_schema( const std::string& key ) const;

        void set_item_schema( std::shared_ptr<Schema> schema );

        std::shared_ptr<Schema> get_item_schema() const;

        // Description
        const std::string& get_description() const { return m_description; }
        void set_description(const std::string& description) { m_description = description; }

    private:

        Property_Value_Type m_type;
        bool m_required{ false };
        bool m_has_default{ false };
        std::any m_default_value;
        std::vector<std::shared_ptr<Constraint_Iface>> m_constraints;
        std::unordered_map<std::string, std::shared_ptr<Schema>> m_property_schemas;
        std::shared_ptr<Schema> m_item_schema;
        std::string m_description;
};

// Utility functions for creating common schemas
std::shared_ptr<schema::Schema> create_string_schema();
std::shared_ptr<schema::Schema> create_integer_schema();
std::shared_ptr<schema::Schema> create_float_schema();
std::shared_ptr<schema::Schema> create_boolean_schema();
std::shared_ptr<schema::Schema> create_object_schema();
std::shared_ptr<schema::Schema> create_array_schema();

/**
 * Convert a Property_Value_Type to a string
 */
std::string type_to_string( schema::Property_Value_Type type );

} // namespace tmns::fcs

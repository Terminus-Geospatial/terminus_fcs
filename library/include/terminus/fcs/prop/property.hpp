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
 * @file    property.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <any>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/fcs/schema/property_value_type.hpp>
#include <terminus/fcs/schema/schema.hpp>

namespace tmns::fcs::prop {

/**
 * Base Property class that stores a value and provides validation
 */
class Property {

    public:

        Property() = default;

        explicit Property(const std::string& key);

        virtual ~Property() = default;

        // Core value operations
        virtual Result<void> set_value(const std::any& value) = 0;
        virtual Result<std::any> get_value() const = 0;
        virtual Result<void> validate() const = 0;

        // Key operations
        const std::string& get_key() const { return m_key; }
        void set_key(const std::string& key) { m_key = key; }

        // Schema operations
        void set_schema(std::optional<schema::Schema> schema) { m_schema = std::move(schema); }
        const std::optional<schema::Schema>& get_schema() const { return m_schema; }

        // Type information
        virtual schema::Property_Value_Type get_type() const = 0;
        virtual std::string get_type_string() const = 0;

    protected:

        std::string m_key;
        schema::Property_Value_Type m_type;
        std::optional<schema::Schema> m_schema;
};

} // namespace tmns::fcs::prop
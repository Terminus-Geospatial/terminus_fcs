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
 * @file    builder.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <any>
#include <string>

// Terminus Libraries
#include <terminus/outcome/result.hpp>

namespace tmns::fcs::schema {

/**
 * Schema builder for convenient schema construction
 */
class Builder {
    public:
        explicit Builder(Property_Value_Type type);

        // Chainable methods
        Builder& required(bool is_required = true);
        Builder& default_value(const std::any& value);
        Builder& description(const std::string& desc);

        // Constraints
        Builder& range(double min_val, double max_val);
        Builder& range(int64_t min_val, int64_t max_val);
        Builder& enum_values(const std::vector<std::string>& values);
        Builder& custom(std::shared_ptr<ValidationConstraint> constraint);

        // Object/Array composition
        Builder& property(const std::string& key, std::shared_ptr<PropertySchema> schema);
        Builder& items(std::shared_ptr<PropertySchema> schema);

        // Build
        std::shared_ptr<PropertySchema> build();

    private:
        std::shared_ptr<PropertySchema> m_schema;
};

// Utility functions for creating common schemas
std::shared_ptr<Schema> create_string_schema();
std::shared_ptr<Schema> create_integer_schema();
std::shared_ptr<Schema> create_float_schema();
std::shared_ptr<Schema> create_boolean_schema();
std::shared_ptr<Schema> create_object_schema();
std::shared_ptr<Schema> create_array_schema();



} // namespace tmns::fcs
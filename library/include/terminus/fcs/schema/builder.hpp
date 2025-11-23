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
#include <terminus/error.hpp>
#include <terminus/fcs/schema/schema.hpp>


namespace tmns::fcs::schema {

/**
 * Schema builder for convenient schema construction
 */
class Builder
{
    public:

        /**
         * Constructor
         */
        explicit Builder(Property_Value_Type type);

        /**
         * Set the schema as required
         */
        Builder& required( bool is_required = true );

        /**
         * Set the default value
         */
        Builder& default_value( const std::any& value );

        /**
         * Set the description
         */
        Builder& description( const std::string& desc );

        /**
         * Add a range constraint
         */
        Builder& range( double min_val, double max_val );

        /**
         * Add a range constraint
         */
        Builder& range( int64_t min_val, int64_t max_val );

        /**
         * Add enum values
         */
        Builder& enum_values( const std::vector<std::string>& values );

        /**
         * Add a custom constraint
         */
        Builder& custom( std::shared_ptr<Constraint_Iface> constraint );

        /**
         * Object/Array composition
         */
        Builder& property( const std::string&      key,
                           std::shared_ptr<Schema> schema );

        /**
         * Object/Array composition
         */
        Builder& items( std::shared_ptr<Schema> schema );

        /**
         * Build
         */
        std::shared_ptr<Schema> build();

    private:
        std::shared_ptr<Schema> m_schema;
};

// Utility functions for creating common schemas
std::shared_ptr<Schema> create_string_schema();
std::shared_ptr<Schema> create_integer_schema();
std::shared_ptr<Schema> create_float_schema();
std::shared_ptr<Schema> create_boolean_schema();
std::shared_ptr<Schema> create_object_schema();
std::shared_ptr<Schema> create_array_schema();



} // namespace tmns::fcs
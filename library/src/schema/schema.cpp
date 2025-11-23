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
 * @file    schema.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#include <terminus/fcs/schema/schema.hpp>

// C++ Standard Libraries
#include <algorithm>
#include <sstream>

// Terminus Libraries
#include <terminus/fcs/prop/property.hpp>

namespace tmns::fcs::schema {

/*****************************/
/*      Schema Constructor   */
/*****************************/
Schema::Schema( Property_Value_Type type ) : m_type(type) {}

/***************************/
/*      Get Default        */
/***************************/
Result<std::any> Schema::get_default() const {
    if (!m_has_default) {
        return outcome::fail( error::Error_Code::SCHEMA_NOT_FOUND,
                                 "Schema does not have a default value");
    }
    return outcome::ok<std::any>( m_default_value );
}

/***************************/
/*      Set Default        */
/***************************/
void Schema::set_default_value( const std::any& default_value ) {
    m_default_value = default_value;
    m_has_default = true;
}

/***************************/
/*      Add Constraint     */
/***************************/
void Schema::add_constraint(std::shared_ptr<Constraint_Iface> constraint) {
    m_constraints.push_back(constraint);
}

/***************************/
/*      Get Constraints    */
/***************************/
const std::vector<std::shared_ptr<Constraint_Iface>>& Schema::get_constraints() const {
    return m_constraints;
}

/***************************/
/*       Validate          */
/***************************/
Result<void> Schema::validate(const std::any& value) const {
    // Check all constraints
    for (const auto& constraint : m_constraints) {
        auto result = constraint->validate(value);
        if (!result) {
            return result;
        }
    }
    return outcome::ok();
}

/********************************/
/*       Validate Property      */
/********************************/
Result<void> Schema::validate_property( const prop::Property& property ) const
{
    // Check type compatibility
    if( property.get_type() != m_type ) {
        return outcome::fail( error::Error_Code::TYPE_MISMATCH,
                                 "Property type '" + property.get_type_string() +
                                 "' does not match schema type '" + type_to_string( m_type ) + "'" );
    }

    // Validate the property's value
    auto value_result = property.get_value();
    if( value_result ) {
        return validate( value_result.value() );
    }

    // If property doesn't have a value but is required, fail
    if( m_required ) {
        return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                                 "Required property '" + property.get_key() + "' is missing value" );
    }

    return outcome::ok();
}

/********************************/
/*       Add Property Schema    */
/********************************/
void Schema::add_property_schema( const std::string&      key,
                                  std::shared_ptr<Schema> schema )
{
    m_property_schemas[key] = schema;
}

/********************************/
/*       Get Property Schema    */
/********************************/
std::shared_ptr<Schema> Schema::get_property_schema( const std::string& key ) const
{
    auto it = m_property_schemas.find(key);
    return (it != m_property_schemas.end()) ? it->second : nullptr;
}

/********************************/
/*       Set Item Schema        */
/********************************/
void Schema::set_item_schema( std::shared_ptr<Schema> schema ) {
    m_item_schema = schema;
}

/********************************/
/*       Get Item Schema        */
/********************************/
std::shared_ptr<Schema> Schema::get_item_schema() const {
    return m_item_schema;
}

} // namespace tmns::fcs::schema

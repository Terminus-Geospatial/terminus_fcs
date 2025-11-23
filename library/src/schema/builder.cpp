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
 * @file    builder.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#include <terminus/fcs/schema/builder.hpp>

// Terminus Libraries
#include <terminus/fcs/schema/enum_constraint.hpp>
#include <terminus/fcs/schema/range_constraint.hpp>

namespace tmns::fcs::schema {

/*********************************/
/*          Constructor          */
/*********************************/
Builder::Builder( Property_Value_Type type )
    : m_schema( std::make_shared<Schema>(type) )
{}

/*********************************/
/*        Set as required        */
/*********************************/
Builder& Builder::required( bool is_required )
{
    m_schema->set_required( is_required );
    return *this;
}

/*********************************/
/*        Set default value      */
/*********************************/
Builder& Builder::default_value( const std::any& value )
{
    m_schema->set_default_value( value );
    return *this;
}

/*********************************/
/*        Set description        */
/*********************************/
Builder& Builder::description( const std::string& desc )
{
    m_schema->set_description( desc );
    return *this;
}

/*********************************/
/*    Add range constraint       */
/*********************************/
Builder& Builder::range( double min_val, double max_val )
{
    auto constraint = std::make_shared<Range_Constraint<double>>( min_val, max_val );
    m_schema->add_constraint(constraint);
    return *this;
}

/*********************************/
/*    Add range constraint       */
/*********************************/
Builder& Builder::range( int64_t min_val, int64_t max_val )
{
    auto constraint = std::make_shared<Range_Constraint<int64_t>>( min_val, max_val );
    m_schema->add_constraint(constraint);
    return *this;
}

/*********************************/
/*    Add enum values            */
/*********************************/
Builder& Builder::enum_values( const std::vector<std::string>& values )
{
    auto constraint = std::make_shared<Enum_Constraint>( values );
    m_schema->add_constraint(constraint);
    return *this;
}

/*********************************/
/*    Add a custom constraint    */
/*********************************/
Builder& Builder::custom( std::shared_ptr<Constraint_Iface> constraint )
{
    m_schema->add_constraint( constraint );
    return *this;
}

/*********************************/
/*    Add a property schema      */
/*********************************/
Builder& Builder::property( const std::string& key, std::shared_ptr<Schema> schema )
{
    m_schema->add_property_schema( key, schema );
    return *this;
}

/*********************************/
/*    Add an item schema         */
/*********************************/
Builder& Builder::items( std::shared_ptr<Schema> schema )
{
    m_schema->set_item_schema( schema );
    return *this;
}

/*********************************/
/*    Build the schema           */
/*********************************/
std::shared_ptr<Schema> Builder::build()
{
    return m_schema;
}

/*********************************/
/*    Utility functions          */
/*********************************/
std::shared_ptr<Schema> create_string_schema()
{
    return Builder( Property_Value_Type::STRING ).build();
}

/*********************************/
/*    Create integer schema      */
/*********************************/
std::shared_ptr<Schema> create_integer_schema()
{
    return Builder( Property_Value_Type::INTEGER ).build();
}

/*********************************/
/*    Create float schema        */
/*********************************/
std::shared_ptr<Schema> create_float_schema()
{
    return Builder( Property_Value_Type::FLOAT ).build();
}

/*********************************/
/*    Create boolean schema      */
/*********************************/
std::shared_ptr<Schema> create_boolean_schema()
{
    return Builder( Property_Value_Type::BOOLEAN ).build();
}

/*********************************/
/*    Create object schema       */
/*********************************/
std::shared_ptr<Schema> create_object_schema()
{
    return Builder( Property_Value_Type::OBJECT ).build();
}

/*********************************/
/*    Create array schema        */
/*********************************/
std::shared_ptr<Schema> create_array_schema()
{
    return Builder( Property_Value_Type::ARRAY ).build();
}

} // namespace tmns::fcs::schema
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
 * @file    property_value_type.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#include <terminus/fcs/schema/property_value_type.hpp>

namespace tmns::fcs::schema {

/***************************/
/*      Type to String     */
/***************************/
std::string type_to_string( Property_Value_Type type ) {
    switch (type) {
        case Property_Value_Type::STRING:  return "STRING";
        case Property_Value_Type::INTEGER: return "INTEGER";
        case Property_Value_Type::FLOAT:   return "FLOAT";
        case Property_Value_Type::DOUBLE:  return "DOUBLE";
        case Property_Value_Type::BOOLEAN: return "BOOLEAN";
        case Property_Value_Type::PATH:    return "PATH";
        case Property_Value_Type::ARRAY:   return "ARRAY";
        case Property_Value_Type::OBJECT:  return "OBJECT";
        default: return "UNKNOWN";
    }
}

/***************************/
/*      String to Type     */
/***************************/
Property_Value_Type string_to_type( const std::string& type ) {
    if( type == "STRING" )  return Property_Value_Type::STRING;
    if( type == "INTEGER" ) return Property_Value_Type::INTEGER;
    if( type == "FLOAT" )   return Property_Value_Type::FLOAT;
    if( type == "DOUBLE" )  return Property_Value_Type::DOUBLE;
    if( type == "BOOLEAN" ) return Property_Value_Type::BOOLEAN;
    if( type == "PATH" )    return Property_Value_Type::PATH;
    if( type == "ARRAY" )   return Property_Value_Type::ARRAY;
    if( type == "OBJECT" )  return Property_Value_Type::OBJECT;

    // Default fallback
    return Property_Value_Type::STRING;
}

} // End of namespace tmns::fcs::schema
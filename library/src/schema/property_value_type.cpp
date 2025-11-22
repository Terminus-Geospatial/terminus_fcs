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
        case Property_Value_Type::BOOLEAN: return "BOOLEAN";
        case Property_Value_Type::ARRAY:   return "ARRAY";
        case Property_Value_Type::OBJECT:  return "OBJECT";
        default: return "UNKNOWN";
    }
}

} // End of namespace tmns::fcs::schema
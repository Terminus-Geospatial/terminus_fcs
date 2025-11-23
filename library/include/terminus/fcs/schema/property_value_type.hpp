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
 * @file    property_value_type.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <string>

namespace tmns::fcs::schema {

/**
 * Property value type
 */
enum class Property_Value_Type {
    STRING,
    INTEGER,
    FLOAT,
    DOUBLE,
    BOOLEAN,
    PATH,
    OBJECT,
    ARRAY
};

// Helper function
std::string type_to_string( Property_Value_Type type );

Property_Value_Type string_to_type( const std::string& type );

} // End of namespace tmns::fcs::schema
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
 * @file    enum_constraint.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/

// C++ Standard Libraries
#include <algorithm>
#include <sstream>

// Terminus Libraries
#include <terminus/fcs/schema/enum_constraint.hpp>

namespace tmns::fcs::schema {


/*****************************/
/*        Constructor        */
/*****************************/
Enum_Constraint::Enum_Constraint( const std::vector<std::string>& allowed_values )
    : m_allowed_values(allowed_values) {}

/***************************/
/*        Validate         */
/***************************/
Result<void> Enum_Constraint::validate( const std::any& value ) const {
    try {
        std::string str_value = std::any_cast<std::string>(value);
        auto it = std::find(m_allowed_values.begin(), m_allowed_values.end(), str_value);
        if (it == m_allowed_values.end()) {
            std::ostringstream oss;
            oss << "Value '" << str_value << "' is not in allowed values: [";
            for (size_t i = 0; i < m_allowed_values.size(); ++i) {
                if (i > 0) oss << ", ";
                oss << "'" << m_allowed_values[i] << "'";
            }
            oss << "]";
            return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                                  oss.str() );
        }
        return outcome::ok();
    } catch (const std::bad_any_cast& e) {
        return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                              "Value is not a string");
    }
}

/***************************/
/*        Description      */
/***************************/
std::string Enum_Constraint::description() const {
    std::ostringstream oss;
    oss << "Value must be one of: [";
    for (size_t i = 0; i < m_allowed_values.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "'" << m_allowed_values[i] << "'";
    }
    oss << "]";
    return oss.str();
}

} // namespace tmns::fcs::schema
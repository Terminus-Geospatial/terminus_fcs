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
 * @file    custom_constraint.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#include <terminus/fcs/schema/custom_constraint.hpp>

// C++ Standard Libraries
#include <algorithm>
#include <sstream>

// Terminus Libraries
#include <terminus/error.hpp>

namespace tmns::fcs::schema {

/***************************/
/*        Constructor      */
/***************************/
Custom_Constraint::Custom_Constraint( Validator_Func     validator,
                                      const std::string& description )
    : m_validator(validator), m_description(description) {}

/***************************/
/*        Validate         */
/***************************/
Result<void> Custom_Constraint::validate( const std::any& value ) const {
    return m_validator(value);
}

/***************************/
/*        Description      */
/***************************/
std::string Custom_Constraint::description() const {
    return m_description;
}

} // End of namespace tmns::fcs::schema
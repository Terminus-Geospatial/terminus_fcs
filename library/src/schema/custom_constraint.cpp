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

// C++ Standard Libraries
#include <algorithm>
#include <sstream>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/fcs/schema/custom_constraint.hpp>

namespace tmns::fcs::schema {

/***************************/
/*        Constructor      */
/***************************/
CustomConstraint::CustomConstraint( ValidatorFunc validator, const std::string& description )
    : m_validator(validator), m_description(description) {}

/***************************/
/*        Validate         */
/***************************/
Result<void> CustomConstraint::validate( const std::any& value ) const {
    return m_validator(value);
}

/***************************/
/*        Description      */
/***************************/
std::string CustomConstraint::description() const {
    return m_description;
}

} // End of namespace tmns::fcs::schema
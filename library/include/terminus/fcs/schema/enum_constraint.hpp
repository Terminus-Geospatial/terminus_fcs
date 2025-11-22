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
 * @file    enum_constraint.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <any>
#include <string>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/fcs/schema/constraint_iface.hpp>

namespace tmns::fcs::schema {

/**
 * Enum constraint for string values
 */
class Enum_Constraint : public Constraint_Iface {
    public:
        explicit Enum_Constraint( const std::vector<std::string>& allowed_values );

        Result<void> validate( const std::any& value ) const override;

        std::string description() const override;

    private:
        std::vector<std::string> m_allowed_values;
};

} // End of namespace tmns::fcs::schema
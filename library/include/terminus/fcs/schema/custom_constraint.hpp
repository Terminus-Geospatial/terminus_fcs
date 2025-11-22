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
 * @file    custom_constraint.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <any>
#include <string>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/fcs/schemas/constraint_iface.hpp>

namespace tmns::fcs {

/**
 * Custom constraint using a validation function
 */
class Custom_Constraint : public Constraint_Iface {
    public:

        using Validator_Func = std::function<Result<void>(const std::any&)>;

        explicit Custom_Constraint( Validator_Func     validator,
                                    const std::string& description );

        Result<void> validate( const std::any& value ) const override;

        std::string description() const override;

    private:

        Validator_Func m_validator;

        std::string m_description;
};

}
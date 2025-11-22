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
 * @file    constraint_iface.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <any>
#include <string>

// Terminus Libraries
#include <terminus/error.hpp>

namespace tmns::fcs {

/**
 * Validation constraint interface
 */
class Constraint_Iface {

    public:

        virtual ~Constraint_Iface() = default;

        virtual Result<void> validate(const std::any& value) const = 0;

        virtual std::string description() const = 0;
};

} // End of namespace tmns::fcs namespace
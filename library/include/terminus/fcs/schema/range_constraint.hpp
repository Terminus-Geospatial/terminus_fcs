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
 * @file    range_constraint.hpp
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
 * Range constraint for numeric values
 */
template<typename T>
class Range_Constraint : public Constraint_Iface {

    public:

        /**
         * Constructor
         */
        Range_Constraint( const T& min_value, const T& max_value )
            : m_min_value(min_value), m_max_value(max_value) {}

        /**
         * Validate the value
         */
        Result<void> validate( const std::any& value ) const override {
            try {
                T typed_value = std::any_cast<T>(value);
                if (typed_value < m_min_value || typed_value > m_max_value) {
                    return outcome::fail( error::Error_Code::VALIDATION_FAILED,
                                          "Value " + std::to_string(typed_value) +
                                          " is outside range [" + std::to_string(m_min_value) +
                                          ", " + std::to_string(m_max_value) + "]");
                }
                return outcome::ok<void>();
            } catch (const std::bad_any_cast& e) {
                return outcome::fail( error::Error_Code::TYPE_MISMATCH,
                                     "Cannot cast value to numeric type for range validation");
            }
        }

        /**
         * Get the description
         */
        std::string description() const override {
            return "Value must be in range [" + std::to_string(m_min_value) +
                   ", " + std::to_string(m_max_value) + "]";
        }

    private:

        T m_min_value;

        T m_max_value;
};
} // End of namespace tmns::fcs namespace.
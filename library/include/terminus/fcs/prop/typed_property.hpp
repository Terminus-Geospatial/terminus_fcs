/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2025 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/*************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    typed_property.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <filesystem>

// Terminus Libraries
#include <terminus/error.hpp>

// Project Libraries
#include <terminus/fcs/prop/property.hpp>

namespace tmns::fcs::prop {

/**
 * Template specialization for typed properties
 */
template<typename T>
class Typed_Property : public Property {
    public:
        using ValueType = T;

        Typed_Property() = default;

        explicit Typed_Property(const std::string& key, const T& value = T{})
            : Property(key), m_value(value) {}

        Result<void> set_value(const std::any& value) override
        {
            try {
                m_value = std::any_cast<T>(value);
                return outcome::ok();
            } catch (const std::bad_any_cast& e) {
                return outcome::fail( error::Error_Code::TYPE_MISMATCH,
                                      "Cannot cast value to type " + get_type_string());
            }
        }
        Result<std::any> get_value() const override
        {
            return outcome::ok<std::any>(m_value);
        }

        /**
         * Validate the property
         */
        Result<void> validate() const override
        {
            if (m_schema) {
                return m_schema->validate(m_value);
            }
            return outcome::ok();
        }

        /**
         * Set the value of the property
         */
        Result<void> set_typed_value(const T& value)
        {
            m_value = value;
            return outcome::ok();
        }

        /**
         * Get the value of the property
         */
        Result<T> get_typed_value() const
        {
            return outcome::ok<T>( m_value );
        }

        /**
         * Get the type of the property
         */
        schema::Property_Value_Type get_type() const override;

        /**
         * Get the type string of the property
         */
        std::string get_type_string() const override;

    private:
        T m_value{};
};

/**
 * Get the type for the property
 */
template<>
inline schema::Property_Value_Type Typed_Property<std::string>::get_type() const {
    return schema::Property_Value_Type::STRING;
}

/**
 * Get the type for the property
 */
template<>
inline schema::Property_Value_Type Typed_Property<int64_t>::get_type() const {
    return schema::Property_Value_Type::INTEGER;
}

/**
 * Get the type for the property
 */
template<>
inline schema::Property_Value_Type Typed_Property<float>::get_type() const {
    return schema::Property_Value_Type::FLOAT;
}

/**
 * Get the type for the property
 */
template<>
inline schema::Property_Value_Type Typed_Property<double>::get_type() const {
    return schema::Property_Value_Type::DOUBLE;
}

/**
 * Get the type for the property
 */
template<>
inline schema::Property_Value_Type Typed_Property<bool>::get_type() const {
    return schema::Property_Value_Type::BOOLEAN;
}

/**
 * Get the type for the property
 */
template<>
inline schema::Property_Value_Type Typed_Property<std::filesystem::path>::get_type() const {
    return schema::Property_Value_Type::PATH;
}

/**
 * Get the type string for the property
 */
template<>
inline std::string Typed_Property<std::string>::get_type_string() const {
    return "string";
}

/**
 * Get the type string for the property
 */
template<>
inline std::string Typed_Property<int64_t>::get_type_string() const {
    return "integer";
}

/**
 * Get the type string for the property
 */
template<>
inline std::string Typed_Property<float>::get_type_string() const {
    return "float";
}

/**
 * Get the type string for the property
 */
template<>
inline std::string Typed_Property<double>::get_type_string() const {
    return "double";
}

/**
 * Get the type string for the property
 */
template<>
inline std::string Typed_Property<bool>::get_type_string() const {
    return "boolean";
}

/**
 * Get the type string for the property
 */
template<>
inline std::string Typed_Property<std::filesystem::path>::get_type_string() const {
    return "path";
}

// Type aliases for common property types
using String_Property  = Typed_Property<std::string>;
using Integer_Property = Typed_Property<int64_t>;
using Float_Property   = Typed_Property<float>;
using Double_Property  = Typed_Property<double>;
using Boolean_Property = Typed_Property<bool>;
using Path_Property    = Typed_Property<std::filesystem::path>;

} // namespace tmns::fcs::prop
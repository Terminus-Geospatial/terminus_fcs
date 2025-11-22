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
 * @file    property.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/

// C++ Standard Libraries
#include <algorithm>
#include <sstream>
#include <stdexcept>

// Terminus Libraries
#include <terminus/fcs/fcs_error.hpp>
#include <terminus/fcs/property.hpp>

namespace tmns::fcs {

/*****************************/
/*        Constructor        */
/*****************************/
Property::Property(const std::string& key)
    : m_key(key) {}

/*****************************/
/*        Typed_Property      */
/*****************************/
template<typename T>
Typed_Property<T>::Typed_Property(const std::string& key, const T& value)
    : Property(key), m_value(value) {}

template<typename T>
Result<void> Typed_Property<T>::set_value(const std::any& value) {
    try {
        m_value = std::any_cast<T>(value);
        return ok<void>();
    } catch (const std::bad_any_cast& e) {
        return fail<FcsErrorCode>(FcsErrorCode::TYPE_MISMATCH,
                                 "Cannot cast value to type " + get_type_string());
    }
}

template<typename T>
Result<std::any> Typed_Property<T>::get_value() const {
    return ok<std::any>(m_value);
}

template<typename T>
Result<void> Typed_Property<T>::validate() const {
    if (m_schema) {
        return m_schema->validate(m_value);
    }
    return ok<void>();
}

template<typename T>
Result<void> Typed_Property<T>::set_typed_value(const T& value) {
    m_value = value;
    return ok<void>();
}

template<typename T>
Result<T> TypedProperty<T>::get_typed_value() const {
    return ok<T>(m_value);
}

// Specializations for different types
template<>
PropertyValueType TypedProperty<std::string>::get_type() const {
    return PropertyValueType::STRING;
}

template<>
PropertyValueType TypedProperty<int64_t>::get_type() const {
    return PropertyValueType::INTEGER;
}

template<>
PropertyValueType TypedProperty<double>::get_type() const {
    return PropertyValueType::FLOAT;
}

template<>
PropertyValueType TypedProperty<bool>::get_type() const {
    return PropertyValueType::BOOLEAN;
}

template<>
std::string TypedProperty<std::string>::get_type_string() const {
    return "string";
}

template<>
std::string TypedProperty<int64_t>::get_type_string() const {
    return "integer";
}

template<>
std::string TypedProperty<double>::get_type_string() const {
    return "float";
}

template<>
std::string TypedProperty<bool>::get_type_string() const {
    return "boolean";
}


// ArrayProperty implementation
ArrayProperty::ArrayProperty(const std::string& key) : Property(key) {}

Result<void> ArrayProperty::set_value(const std::any& value) {
    return fail<FcsErrorCode>(FcsErrorCode::INVALID_OPERATION,
                             "Cannot set value directly on ArrayProperty. Use add_item instead.");
}

Result<std::any> ArrayProperty::get_value() const {
    return fail<FcsErrorCode>(FcsErrorCode::INVALID_OPERATION,
                             "ArrayProperty does not have a direct value. Use get_item instead.");
}

Result<void> ArrayProperty::validate() const {
    if (m_schema) {
        auto result = m_schema->validate_property(*this);
        if (!result) {
            return result;
        }
    }

    // Validate all items
    for (const auto& item : m_items) {
        auto item_result = item->validate();
        if (!item_result) {
            return item_result;
        }
    }

    return ok<void>();
}

Result<void> ArrayProperty::add_item(std::shared_ptr<Property> item) {
    if (!item) {
        return fail<FcsErrorCode>(FcsErrorCode::NULL_POINTER,
                                 "Cannot add null item to array");
    }

    m_items.push_back(item);
    return ok<void>();
}

Result<std::shared_ptr<Property>> ArrayProperty::get_item(size_t index) const {
    if (index >= m_items.size()) {
        return fail<FcsErrorCode>(FcsErrorCode::INDEX_OUT_OF_BOUNDS,
                                 "Array index out of bounds: " + std::to_string(index));
    }
    return ok<std::shared_ptr<Property>>(m_items[index]);
}

Result<void> ArrayProperty::remove_item(size_t index) {
    if (index >= m_items.size()) {
        return fail<FcsErrorCode>(FcsErrorCode::INDEX_OUT_OF_BOUNDS,
                                 "Array index out of bounds: " + std::to_string(index));
    }
    m_items.erase(m_items.begin() + index);
    return ok<void>();
}

// Explicit template instantiations
template class TypedProperty<std::string>;
template class TypedProperty<int64_t>;
template class TypedProperty<double>;
template class TypedProperty<bool>;

} // namespace tmns::fcs

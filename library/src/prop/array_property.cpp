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
 * @file    array_property.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/

// C++ Standard Libraries
#include <stdexcept>

// Terminus Libraries
#include <terminus/fcs/prop/property.hpp>

namespace tmns::fcs::prop {

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

} // namespace tmns::fcs::prop
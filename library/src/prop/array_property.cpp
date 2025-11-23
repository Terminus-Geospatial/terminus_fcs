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
#include <terminus/fcs/prop/array_property.hpp>

// C++ Standard Libraries
#include <stdexcept>

// Terminus Libraries
#include <terminus/error.hpp>


namespace tmns::fcs::prop {

/*****************************************/
/*          Constructor                  */
/*****************************************/
Array_Property::Array_Property(const std::string& key) : Property(key) {}

/*****************************************/
/*        Set the Property Value         */
/*****************************************/
Result<void> Array_Property::set_value( [[maybe_unused]] const std::any& value ) {
    return outcome::fail( error::Error_Code::NOT_SUPPORTED,
                          "Cannot set value directly on Array_Property. Use add_item instead.");
}

/*****************************************/
/*        Get the Property Value         */
/*****************************************/
Result<std::any> Array_Property::get_value() const {
    return outcome::fail( error::Error_Code::NOT_SUPPORTED,
                          "Array_Property does not have a direct value. Use get_item instead." );
}

/*****************************************/
/*        Validate the Property          */
/*****************************************/
Result<void> Array_Property::validate() const {
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

    return outcome::ok();
}

/*****************************************/
/*        Add an Item to the Array       */
/*****************************************/
Result<void> Array_Property::add_item(std::shared_ptr<Property> item) {
    if (!item) {
        return outcome::fail( error::Error_Code::UNINITIALIZED,
                                 "Cannot add null item to array" );
    }

    m_items.push_back(item);
    return outcome::ok();
}

/*****************************************/
/*        Get an Item from the Array     */
/*****************************************/
Result<std::shared_ptr<Property>> Array_Property::get_item( size_t index ) const
{
    if (index >= m_items.size()) {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                 "Array index out of bounds: " + std::to_string(index) );
    }
    return outcome::ok<std::shared_ptr<Property>>(m_items[index]);
}

/*****************************************/
/*        Remove an Item from the Array  */
/*****************************************/
Result<void> Array_Property::remove_item( size_t index )
{
    if (index >= m_items.size()) {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                 "Array index out of bounds: " + std::to_string(index) );
    }
    m_items.erase( m_items.begin() + static_cast<long>(index) );
    return outcome::ok();
}

} // namespace tmns::fcs::prop
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
 * @file    array_property.hpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#pragma once

// C++ Standard Libraries
#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Terminus Libraries
#include <terminus/outcome/result.hpp>

// Project Libraries
#include <terminus/fcs/prop/property.hpp>

namespace tmns::fcs::prop {

/**
 * Property that represents an array of values
 */
class Array_Property : public Property
{
    public:

        Array_Property() = default;

        explicit Array_Property(const std::string& key);

        Result<void> set_value(const std::any& value) override;

        Result<std::any> get_value() const override;

        Result<void> validate() const override;

        // Array operations
        Result<void> add_item(std::shared_ptr<Property> item);

        Result<std::shared_ptr<Property>> get_item(size_t index) const;

        Result<void> remove_item(size_t index);

        size_t size() const { return m_items.size(); }

        schema::Property_Value_Type get_type() const override { return schema::Property_Value_Type::ARRAY; }

        std::string get_type_string() const override { return "array"; }

    private:

        std::vector<std::shared_ptr<Property>> m_items;
};

} // namespace tmns::fcs::prop
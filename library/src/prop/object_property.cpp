/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2025 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    object_property.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#include <terminus/fcs/prop/object_property.hpp>

namespace tmns::fcs::prop {

/**********************************/
/*          Constructor           */
/**********************************/
Object_Property::Object_Property( const std::string& key )
    : Property(key) {}

/**********************************/
/*          Set Value             */
/**********************************/
Result<void> Object_Property::set_value( const std::any& value )
{
    return outcome::fail( error::Error_Code::INVALID_OPERATION,
                          "Cannot set value directly on Object_Property. Use add_property instead." );
}

/**********************************/
/*          Get Value             */
/**********************************/
Result<std::any> Object_Property::get_value() const {
    return outcome::fail( error::Error_Code::INVALID_OPERATION,
                          "Object_Property does not have a direct value. Use get_property instead." );
}

/**********************************/
/*          Validate              */
/**********************************/
Result<void> Object_Property::validate() const
{
    if (m_schema) {
        auto result = m_schema->validate_property(*this);
        if (!result) {
            return result;
        }
    }

    // Validate all children
    for (const auto& [key, child] : m_children) {
        auto child_result = child->validate();
        if (!child_result) {
            return child_result;
        }
    }

    return outcome::ok();
}

/**********************************/
/*          Add Property          */
/**********************************/
Result<void> Object_Property::add_property( std::shared_ptr<Property> property )
{
    if (!property) {
        return outcome::fail( error::Error_Code::NULL_POINTER,
                              "Cannot add null property" );
    }

    m_children[property->get_key()] = property;
    return outcome::ok();
}

/**********************************/
/*          Get Property          */
/**********************************/
Result<std::shared_ptr<Property>> Object_Property::get_property(const std::string& key) const
{
    auto it = m_children.find(key);
    if (it == m_children.end()) {
        return outcome::fail( error::Error_Code::PROPERTY_NOT_FOUND,
                              "Property not found: " + key );
    }
    return outcome::ok<std::shared_ptr<Property>>( it->second );
}

/**********************************/
/*          Remove Property       */
/**********************************/
Result<void> Object_Property::remove_property(const std::string& key)
{
    auto it = m_children.find(key);
    if (it == m_children.end()) {
        return fail<FcsErrorCode>(FcsErrorCode::PROPERTY_NOT_FOUND,
                                 "Property not found: " + key);
    }
    m_children.erase(it);
    return outcome::ok();
}

/**********************************/
/*          Resolve Path          */
/**********************************/
Result<std::shared_ptr<Property>> Object_Property::resolve_path(const std::string& path) const
{
    auto path_parts = split_path(path);
    if (path_parts.empty()) {
        return outcome::ok<std::shared_ptr<Property>>(std::const_pointer_cast<Property>(shared_from_this()));
    }

    std::shared_ptr<Property> current = std::const_pointer_cast<Property>(shared_from_this());

    for (const auto& part : path_parts) {
        if (current->get_type() != PropertyValueType::OBJECT) {
            return fail<FcsErrorCode>(FcsErrorCode::INVALID_PATH,
                                     "Path component '" + part + "' is not an object");
        }

        auto obj_prop = std::dynamic_pointer_cast<ObjectProperty>(current);
        auto result = obj_prop->get_property(part);
        if (!result) {
            return result;
        }
        current = result.value();
    }

    return outcome::ok<std::shared_ptr<Property>>(current);
}

/**********************************/
/*          Set Path Value        */
/**********************************/
Result<void> Object_Property::set_path_value(const std::string& path, const std::any& value)
{
    auto path_parts = split_path(path);
    if (path_parts.empty()) {
        return outcome::fail( error::Error_Code::INVALID_PATH,
                              "Cannot set value on empty path" );
    }

    if (path_parts.size() == 1) {
        // Direct child property
        auto it = m_children.find(path_parts[0]);
        if (it != m_children.end()) {
            return it->second->set_value(value);
        } else {
            return fail<FcsErrorCode>(FcsErrorCode::PROPERTY_NOT_FOUND,
                                     "Property not found: " + path_parts[0]);
        }
    }

    // Navigate to parent
    std::shared_ptr<Property> current = shared_from_this();
    for (size_t i = 0; i < path_parts.size() - 1; ++i) {
        if (current->get_type() != PropertyValueType::OBJECT) {
            return fail<FcsErrorCode>(FcsErrorCode::INVALID_PATH,
                                     "Path component '" + path_parts[i] + "' is not an object");
        }

        auto obj_prop = std::dynamic_pointer_cast<ObjectProperty>(current);
        auto result = obj_prop->get_property(path_parts[i]);
        if (!result) {
            return result;
        }
        current = result.value();
    }

    // Set value on final property
    auto final_obj = std::dynamic_pointer_cast<ObjectProperty>(current);
    if (!final_obj) {
        return fail<FcsErrorCode>(FcsErrorCode::INVALID_PATH,
                                 "Final path component is not an object");
    }

    auto final_prop = final_obj->get_property(path_parts.back());
    if (!final_prop) {
        return final_prop.error();
    }

    return final_prop.value()->set_value(value);
}

/**********************************/
/*          Get Child Keys        */
/**********************************/
std::vector<std::string> Object_Property::get_child_keys() const
{
    std::vector<std::string> keys;
    keys.reserve(m_children.size());

    for (const auto& [key, child] : m_children) {
        keys.push_back(key);
    }

    std::sort(keys.begin(), keys.end());
    return keys;
}

/**********************************/
/*          Split Path            */
/**********************************/
std::vector<std::string> Object_Property::split_path(const std::string& path) const
{
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string part;

    while (std::getline(ss, part, '.')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }

    return parts;
}

/**********************************/
/*          To Type String        */
/**********************************/
std::string Object_Property::to_type_string() const
{
    return "object";
}

} // namespace tmns::fcs::prop
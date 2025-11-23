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
 * @file    datastore.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
*/
#include <terminus/fcs/datastore.hpp>

// C++ Standard Libraries
#include <sstream>
#include <algorithm>
#include <cctype>

// Terminus Libraries
#include <terminus/fcs/prop/array_property.hpp>
#include <terminus/fcs/prop/object_property.hpp>
#include <terminus/fcs/prop/typed_property.hpp>

namespace tmns::fcs {

/******************************/
/*         Constructor        */
/******************************/
Datastore::Datastore() : m_root( std::make_shared<prop::Object_Property>("root") )
{}

/******************************/
/*         Constructor        */
/******************************/
Datastore::Datastore( std::shared_ptr<prop::Object_Property> root ) : m_root(root) {
    if (!m_root) {
        m_root = std::make_shared<prop::Object_Property>("root");
    }
}

/******************************/
/*         Set Property       */
/******************************/
Result<void> Datastore::set_property( const std::string& path, const std::any& value ) {
    return m_root->set_path_value( path, value );
}

/******************************/
/*         Get Property       */
/******************************/
Result<std::shared_ptr<prop::Property>> Datastore::get_property(const std::string& path) const {
    return m_root->resolve_path(path);
}

/********************************/
/*         Remove Property      */
/********************************/
Result<void> Datastore::remove_property( const std::string& path )
{
    auto path_parts = split_path(path);
    if (path_parts.empty()) {
        return outcome::fail( error::Error_Code::NOT_FOUND,
                              "Cannot remove root property" );
    }

    if( path_parts.size() == 1 ) {
        return m_root->remove_property( path_parts[0] );
    }

    // Navigate to parent
    std::string parent_path = std::string( path.begin(),
                                           path.end() - static_cast<long>(path_parts.back().length()) - 1 );
    auto parent_result = get_property( parent_path );
    if (!parent_result) {
        return parent_result.error();
    }

    auto parent_obj = std::dynamic_pointer_cast<prop::Object_Property>( parent_result.value() );
    if (!parent_obj) {
        return outcome::fail( error::Error_Code::NOT_FOUND,
                              "Parent path is not an object" );
    }

    return parent_obj->remove_property( path_parts.back() );
}

/********************************/
/*         Set Schema           */
/********************************/
Result<void> Datastore::set_schema( const std::string& path,
                                     std::optional<schema::Schema> schema )
{
    auto prop_result = get_property( path );
    if (!prop_result) {
        return prop_result.error();
    }

    prop_result.value()->set_schema(std::move(schema));
    return outcome::ok();
}

/********************************/
/*         Get Schema           */
/********************************/
Result<std::optional<schema::Schema>> Datastore::get_schema( const std::string& path ) const
{
    auto prop_result = get_property( path );
    if ( !prop_result ) {
        return prop_result.error();
    }

    const auto& schema = prop_result.value()->get_schema();
    if( !schema ) {
        return outcome::fail( error::Error_Code::SCHEMA_NOT_FOUND,
                              "No schema found for path: " + path );
    }

    return outcome::ok<std::optional<schema::Schema>>( schema );
}

/********************************/
/*         Validate Property    */
/********************************/
Result<void> Datastore::validate_property(const std::string& path) const {
    auto prop_result = get_property(path);
    if (!prop_result) {
        return prop_result.error();
    }

    return prop_result.value()->validate();
}

/********************************/
/*         Validate All         */
/********************************/
Result<void> Datastore::validate_all() const {
    return m_root->validate();
}

/****************************************/
/*         Parse Key-Value Pair         */
/****************************************/
Result<void> Datastore::parse_key_value_pair(const std::string& key_value_str) {
    auto [key, value] = parse_key_value(key_value_str);

    auto prop_result = create_property_for_value(key, value);
    if (!prop_result) {
        return prop_result.error();
    }

    return set_property(key, value);
}

/***********************************/
/*         Parse Command Line      */
/***********************************/
Result<void> Datastore::parse_command_line_args(const std::vector<std::string>& args) {
    for (const auto& arg : args) {
        if (arg.starts_with("-p")) {
            std::string kv_str = arg.substr(2); // Skip "-p"
            if (kv_str.empty()) {
                return outcome::fail( error::Error_Code::PARSING_ERROR,
                                         "Empty key-value pair after -p flag");
            }

            auto result = parse_key_value_pair(kv_str);
            if (!result) {
                return result;
            }
        }
    }
    return outcome::ok();
}

/****************************************/
/*         List Properties              */
/****************************************/
Result<std::vector<std::string>> Datastore::list_properties(const std::string& base_path) const {
    std::vector<std::string> properties;

    if (base_path.empty()) {
        // List root-level properties
        auto children = m_root->get_child_keys();
        for (const auto& child : children) {
            properties.push_back(child);
        }
    } else {
        // List properties under the specified path
        auto base_result = get_property(base_path);
        if (!base_result) {
            return base_result.error();
        }

        auto base_obj = std::dynamic_pointer_cast<prop::Object_Property>(base_result.value());
        if (!base_obj) {
            return outcome::fail( error::Error_Code::NOT_FOUND,
                                     "Path is not an object: " + base_path);
        }

        auto children = base_obj->get_child_keys();
        for (const auto& child : children) {
            properties.push_back(base_path + "." + child);
        }
    }

    std::sort(properties.begin(), properties.end());
    return outcome::ok<std::vector<std::string>>(properties);
}

/****************************************/
/*         Has Property                 */
/****************************************/
Result<bool> Datastore::has_property(const std::string& path) const {
    auto result = get_property(path);
    return outcome::ok<bool>(result.has_value());
}

/****************************************/
/*         Clear                        */
/****************************************/
void Datastore::clear() {
    m_root = std::make_shared<prop::Object_Property>("root");
}

/****************************************/
/*         Size                         */
/****************************************/
size_t Datastore::size() const {
    // Count all properties recursively
    std::function<size_t(std::shared_ptr<prop::Property>)> count_recursive =
        [&](std::shared_ptr<prop::Property> prop) -> size_t {
            size_t count = 1;
            if (prop->get_type() == schema::Property_Value_Type::OBJECT) {
                auto obj = std::dynamic_pointer_cast<prop::Object_Property>(prop);
                auto children = obj->get_child_keys();
                for (const auto& child_key : children) {
                    auto child_prop = obj->get_property(child_key);
                    if (child_prop) {
                        count += count_recursive(child_prop.value());
                    }
                }
            } else if (prop->get_type() == schema::Property_Value_Type::ARRAY) {
                auto arr = std::dynamic_pointer_cast<prop::Array_Property>(prop);
                for (size_t i = 0; i < arr->size(); ++i) {
                    auto item = arr->get_item(i);
                    if (item) {
                        count += count_recursive(item.value());
                    }
                }
            }
            return count;
        };

    return count_recursive(m_root) - 1; // Subtract 1 for root
}

/****************************************/
/*         Parse Key-Value Pair         */
/****************************************/
std::pair<std::string, std::string> Datastore::parse_key_value(const std::string& input) const {
    size_t eq_pos = input.find('=');
    if (eq_pos == std::string::npos) {
        return {input, ""};
    }

    std::string key = input.substr(0, eq_pos);
    std::string value = input.substr(eq_pos + 1);

    return {key, value};
}

/****************************************/
/*         Create Property For Value    */
/****************************************/
Result<std::shared_ptr<prop::Property>> Datastore::create_property_for_value(const std::string& key, const std::string& value) const {
    auto property = infer_property_from_string(key, value);
    if (!property) {
        return outcome::fail( error::Error_Code::PARSING_ERROR,
                                 "Failed to infer property type from value: " + value);
    }

    return outcome::ok<std::shared_ptr<prop::Property>>(property);
}

/*******************************************/
/*         Infer Property From String      */
/*******************************************/
std::shared_ptr<prop::Property> Datastore::infer_property_from_string(const std::string& key, const std::string& value) const {
    // Try to infer the type from the string value

    // Boolean values
    if (value == "true" || value == "false") {
        auto prop = std::make_shared<prop::Boolean_Property>(key);
        prop->set_typed_value(value == "true");
        return prop;
    }

    // Integer values
    if (!value.empty() && std::all_of(value.begin(), value.end(), [](char c) {
        return std::isdigit(c) || c == '-';
    })) {
        try {
            int64_t int_val = std::stoll(value);
            auto prop = std::make_shared<prop::Integer_Property>(key);
            prop->set_typed_value(int_val);
            return prop;
        } catch (...) {
            // Fall through to other types
        }
    }

    // Float values
    if (!value.empty() && std::any_of(value.begin(), value.end(), [](char c) {
        return c == '.' || c == 'e' || c == 'E';
    })) {
        try {
            double float_val = std::stod(value);
            auto prop = std::make_shared<prop::Float_Property>(key);
            prop->set_typed_value(static_cast<float>(float_val));
            return prop;
        } catch (...) {
            // Fall through to string
        }
    }

    // Default to string
    auto prop = std::make_shared<prop::String_Property>(key);
    prop->set_typed_value(value);
    return prop;
}

/********************************/
/*         Split Path           */
/********************************/
std::vector<std::string> Datastore::split_path( const std::string& path ) const
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

} // namespace tmns::fcs

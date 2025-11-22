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
 * @file    property.hpp
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

namespace tmns::fcs::prop {


/**
 * Enum defining the supported property value types
 */
enum class PropertyValueType {
    STRING,
    INTEGER,
    FLOAT,
    BOOLEAN,
    ARRAY,
    OBJECT
};

/**
 * Base Property class that stores a value and provides validation
 */
class Property {

    public:

        Property() = default;

        explicit Property(const std::string& key);

        virtual ~Property() = default;

        // Core value operations
        virtual Result<void> set_value(const std::any& value) = 0;
        virtual Result<std::any> get_value() const = 0;
        virtual Result<void> validate() const = 0;

        // Key operations
        const std::string& get_key() const { return m_key; }
        void set_key(const std::string& key) { m_key = key; }

        // Schema operations
        void set_schema(std::shared_ptr<Property_Schema> schema) { m_schema = schema; }
        std::shared_ptr<Property_Schema> get_schema() const { return m_schema; }

        // Type information
        virtual PropertyValueType get_type() const = 0;
        virtual std::string get_type_string() const = 0;

    protected:
        std::string m_key;
        std::shared_ptr<Property_Schema> m_schema;
};

/**
 * Template specialization for typed properties
 */
template<typename T>
class Typed_Property : public Property {
    public:
        using ValueType = T;

        TypedProperty() = default;
        explicit TypedProperty(const std::string& key, const T& value = T{});

        Result<void> set_value(const std::any& value) override;
        Result<std::any> get_value() const override;
        Result<void> validate() const override;

        Result<void> set_typed_value(const T& value);
        Result<T> get_typed_value() const;

        PropertyValueType get_type() const override;
        std::string get_type_string() const override;

    private:
        T m_value{};
};


/**
 * Property that represents an array of values
 */
class ArrayProperty : public Property {
    public:
        ArrayProperty() = default;
        explicit ArrayProperty(const std::string& key);

        Result<void> set_value(const std::any& value) override;
        Result<std::any> get_value() const override;
        Result<void> validate() const override;

        // Array operations
        Result<void> add_item(std::shared_ptr<Property> item);
        Result<std::shared_ptr<Property>> get_item(size_t index) const;
        Result<void> remove_item(size_t index);
        size_t size() const { return m_items.size(); }

        PropertyValueType get_type() const override { return PropertyValueType::ARRAY; }
        std::string get_type_string() const override { return "array"; }

    private:
        std::vector<std::shared_ptr<Property>> m_items;
};

// Type aliases for common property types
using StringProperty = TypedProperty<std::string>;
using IntegerProperty = TypedProperty<int64_t>;
using FloatProperty = TypedProperty<double>;
using BooleanProperty = TypedProperty<bool>;

} // namespace tmns::fcs::prop
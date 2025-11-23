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
 * @file    schema_tests.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/

// C++ Standard Libraries
#include <string>

// Google Test Libraries
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/fcs/datastore.hpp>
#include <terminus/fcs/schema/schema.hpp>
#include <terminus/fcs/schema/builder.hpp>
#include <terminus/fcs/prop/typed_property.hpp>

using namespace tmns::fcs;

class fcs_schema_Schema : public ::testing::Test
{
    protected:
        void SetUp() override {
            datastore = std::make_unique<Datastore>();
        }

    private:
        std::unique_ptr<Datastore> datastore;
};

/*******************************************/
/*        Test basic schema creation       */
/*******************************************/
TEST_F( fcs_schema_Schema, basic_schema_creation )
{
    // Create a simple schema
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Simple test schema")
        .build();

    ASSERT_TRUE(schema) << "Failed to create schema";
    EXPECT_EQ(schema->get_type(), schema::Property_Value_Type::OBJECT);
}

/*******************************************/
/*        Test schema with string property */
/*******************************************/
TEST_F( fcs_schema_Schema, schema_with_string_property )
{
    // Create a schema with a string property
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Schema with string property")
        .property("name", schema::Builder(schema::Property_Value_Type::STRING)
            .required(true)
            .description("Name property")
            .build())
        .build();

    ASSERT_TRUE(schema) << "Failed to create schema";

    // Apply schema to datastore
    auto schema_result = datastore->set_schema("", *schema);
    ASSERT_TRUE(schema_result) << "Failed to apply schema: " << schema_result.error().message();

    // Try to get the schema back
    auto get_result = datastore->get_schema("");
    ASSERT_TRUE(get_result) << "Failed to get schema: " << get_result.error().message();
    ASSERT_TRUE(get_result.value()) << "Schema should be present";
}

/****************************************************/
/*        Test schema validation with valid data    */
/****************************************************/
TEST_F( fcs_schema_Schema, schema_validation_with_valid_data )
{
    // Create a schema
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Test schema")
        .property("name", schema::Builder(schema::Property_Value_Type::STRING)
            .required(true)
            .description("Name property")
            .build())
        .build();

    // Apply schema to datastore
    auto schema_result = datastore->set_schema("", *schema);
    ASSERT_TRUE(schema_result) << "Failed to apply schema: " << schema_result.error().message();

    // Create and add the property
    auto name_prop = std::make_shared<prop::String_Property>("name");
    auto root = datastore->get_root();
    auto add_result = root->add_property(name_prop);
    ASSERT_TRUE(add_result) << "Failed to add property: " << add_result.error().message();

    // Set a valid value
    auto set_result = datastore->set_property("name", std::string("test_app"));
    ASSERT_TRUE(set_result) << "Failed to set property: " << set_result.error().message();

    // Validate the datastore
    auto validate_result = datastore->validate_all();
    EXPECT_TRUE(validate_result) << "Validation failed: " << validate_result.error().message();
}

/*********************************************************/
/*        Test schema validation with invalid data       */
/*        (missing required property)                    */
/*********************************************************/
TEST_F( fcs_schema_Schema, schema_validation_with_missing_required )
{
    // Create a schema with a required property
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Test schema")
        .property("name", schema::Builder(schema::Property_Value_Type::STRING)
            .required(true)
            .description("Required name property")
            .build())
        .build();

    // Apply schema to datastore
    auto schema_result = datastore->set_schema("", *schema);
    ASSERT_TRUE(schema_result) << "Failed to apply schema: " << schema_result.error().message();

    // Don't add the required property - validation should fail
    auto validate_result = datastore->validate_all();
    EXPECT_FALSE(validate_result) << "Validation should have failed with missing required property";
}

/*************************************/
/*        Test nested schema         */
/*************************************/
TEST_F( fcs_schema_Schema, nested_schema )
{
    // Create a nested schema
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Nested schema")
        .property("app", schema::Builder(schema::Property_Value_Type::OBJECT)
            .required(true)
            .description("App object")
            .property("name", schema::Builder(schema::Property_Value_Type::STRING)
                .required(true)
                .description("App name")
                .build())
            .property("version", schema::Builder(schema::Property_Value_Type::STRING)
                .required(false)
                .default_value(std::string("1.0.0"))
                .description("App version")
                .build())
            .build())
        .build();

    ASSERT_TRUE(schema) << "Failed to create nested schema";

    // Apply schema to datastore
    auto schema_result = datastore->set_schema("", *schema);
    ASSERT_TRUE(schema_result) << "Failed to apply nested schema: " << schema_result.error().message();
}

/****************************************************/
/*        Test schema with different property types */
/****************************************************/
TEST_F( fcs_schema_Schema, schema_with_different_types )
{
    // Create a schema with various property types
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Multi-type schema")
        .property("string_val", schema::Builder(schema::Property_Value_Type::STRING)
            .required(true)
            .description("String value")
            .build())
        .property("int_val", schema::Builder(schema::Property_Value_Type::INTEGER)
            .required(true)
            .description("Integer value")
            .build())
        .property("float_val", schema::Builder(schema::Property_Value_Type::FLOAT)
            .required(false)
            .default_value(3.14)
            .description("Float value")
            .build())
        .property("bool_val", schema::Builder(schema::Property_Value_Type::BOOLEAN)
            .required(false)
            .default_value(false)
            .description("Boolean value")
            .build())
        .build();

    ASSERT_TRUE(schema) << "Failed to create multi-type schema";

    // Apply schema to datastore
    auto schema_result = datastore->set_schema("", *schema);
    ASSERT_TRUE(schema_result) << "Failed to apply multi-type schema: " << schema_result.error().message();

    // Create and add the properties
    auto root = datastore->get_root();

    auto string_prop = std::make_shared<prop::String_Property>("string_val");
    auto int_prop = std::make_shared<prop::Integer_Property>("int_val");
    auto double_prop = std::make_shared<prop::Float_Property>("double_val");
    auto bool_prop = std::make_shared<prop::Boolean_Property>("bool_val");

    ASSERT_TRUE(root->add_property(string_prop));
    ASSERT_TRUE(root->add_property(int_prop));
    ASSERT_TRUE(root->add_property(double_prop));
    ASSERT_TRUE(root->add_property(bool_prop));

    // Set values
    ASSERT_TRUE(datastore->set_property("string_val", std::string("test")));
    ASSERT_TRUE(datastore->set_property("int_val", static_cast<int64_t>(42)));
    ASSERT_TRUE(datastore->set_property("double_val", 2.71));
    ASSERT_TRUE(datastore->set_property("bool_val", true));

    // Validate
    auto validate_result = datastore->validate_all();
    EXPECT_TRUE(validate_result) << "Multi-type validation failed: " << validate_result.error().message();
}

/****************************************************/
/*        Test schema property range constraints    */
/****************************************************/
TEST_F( fcs_schema_Schema, schema_with_range_constraints )
{
    // Create a schema with range constraints
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Range constraint schema")
        .property("port", schema::Builder(schema::Property_Value_Type::INTEGER)
            .required(true)
            .range(static_cast<int64_t>(1), static_cast<int64_t>(65535))
            .description("Port number")
            .build())
        .property("percentage", schema::Builder(schema::Property_Value_Type::FLOAT)
            .required(true)
            .range(0.0, 100.0)
            .description("Percentage")
            .build())
        .build();

    ASSERT_TRUE(schema) << "Failed to create range constraint schema";

    // Apply schema to datastore
    auto schema_result = datastore->set_schema("", *schema);
    ASSERT_TRUE(schema_result) << "Failed to apply range constraint schema: " << schema_result.error().message();
}

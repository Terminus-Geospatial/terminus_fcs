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
 * @file    datastore_test.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#include <gtest/gtest.h>

// C++ Standard Libraries
#include <string>

// Terminus Libraries
#include <terminus/fcs/datastore.hpp>
#include <terminus/fcs/schema/schema.hpp>
#include <terminus/fcs/schema/builder.hpp>
#include <terminus/fcs/prop/typed_property.hpp>

using namespace tmns::fcs;

/***********************************/
/*      Datastore Test Class       */
/***********************************/
class fcs_Datastore : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            datastore = std::make_unique<Datastore>();
        }

    std::unique_ptr<Datastore> datastore;
};

/***********************************/
/*      Datastore Tests            */
/***********************************/
// Test basic property setting without schema
TEST_F( fcs_Datastore, basic_property_setting )
{
    // Create a property first, then set its value
    auto string_prop = std::make_shared<tmns::fcs::prop::String_Property>("test_prop");
    auto root = datastore->get_root();
    auto add_result = root->add_property(string_prop);
    ASSERT_TRUE(add_result) << "Failed to add property: " << add_result.error().message();

    // Try setting the property value
    auto result = datastore->set_property("test_prop", std::string("test_value"));

    // Check if setting succeeded
    ASSERT_TRUE(result) << "Setting property failed: " << result.error().message();
}

/***********************************/
/*      Datastore Tests            */
/***********************************/
TEST_F( fcs_Datastore, nested_property_setting )
{
    // Create the nested structure first
    auto app_obj = std::make_shared<tmns::fcs::prop::Object_Property>("app");
    auto db_obj = std::make_shared<tmns::fcs::prop::Object_Property>("database");
    auto host_prop = std::make_shared<tmns::fcs::prop::String_Property>("host");

    // Add properties to create the hierarchy
    auto root = datastore->get_root();
    ASSERT_TRUE(root->add_property(app_obj));
    ASSERT_TRUE(app_obj->add_property(db_obj));
    ASSERT_TRUE(db_obj->add_property(host_prop));

    // Try setting the nested property value
    auto result = datastore->set_property("app.database.host", std::string("localhost"));

    // Check if setting succeeded
    ASSERT_TRUE(result) << "Setting nested property failed: " << result.error().message();
}

/***********************************/
/*      Datastore Tests            */
/***********************************/
TEST_F( fcs_Datastore, property_setting_with_simple_schema )
{
    // Create a simple schema
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Simple test schema")
        .property("test_prop", schema::Builder(schema::Property_Value_Type::STRING)
            .required(true)
            .description("Test property")
            .build())
        .build();

    // Apply schema to datastore
    auto schema_result = datastore->set_schema("", *schema);
    ASSERT_TRUE(schema_result) << "Schema application failed: " << schema_result.error().message();

    // Create the property that the schema defines
    auto string_prop = std::make_shared<tmns::fcs::prop::String_Property>("test_prop");
    auto root = datastore->get_root();
    auto add_result = root->add_property(string_prop);
    ASSERT_TRUE(add_result) << "Failed to add property: " << add_result.error().message();

    // Try setting a property that exists in schema
    auto result = datastore->set_property("test_prop", std::string("test_value"));
    ASSERT_TRUE(result) << "Setting property failed: " << result.error().message();
}

/***********************************/
/*      Datastore Tests            */
/***********************************/
TEST_F( fcs_Datastore, property_setting_with_nested_schema )
{
    // Create a nested schema
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Nested test schema")
        .property("app", schema::Builder(schema::Property_Value_Type::OBJECT)
            .required(true)
            .description("App object")
            .property("database", schema::Builder(schema::Property_Value_Type::OBJECT)
                .required(true)
                .description("Database object")
                .property("host", schema::Builder(schema::Property_Value_Type::STRING)
                    .required(true)
                    .description("Database host")
                    .build())
                .build())
            .build())
        .build();

    // Apply schema to datastore
    auto schema_result = datastore->set_schema("", *schema);
    ASSERT_TRUE(schema_result) << "Schema application failed: " << schema_result.error().message();

    // Create the nested property structure
    auto app_obj = std::make_shared<tmns::fcs::prop::Object_Property>("app");
    auto db_obj = std::make_shared<tmns::fcs::prop::Object_Property>("database");
    auto host_prop = std::make_shared<tmns::fcs::prop::String_Property>("host");

    // Add properties to create the hierarchy
    auto root = datastore->get_root();
    ASSERT_TRUE(root->add_property(app_obj));
    ASSERT_TRUE(app_obj->add_property(db_obj));
    ASSERT_TRUE(db_obj->add_property(host_prop));

    // Try setting a nested property that exists in schema
    auto result = datastore->set_property("app.database.host", std::string("localhost"));

    // Check if setting succeeded
    ASSERT_TRUE(result) << "Setting nested property failed: " << result.error().message();
}

/***********************************/
/*      Datastore Tests            */
/***********************************/
TEST_F( fcs_Datastore, invalid_property_setting )
{
    // Create a simple schema
    auto schema = schema::Builder(schema::Property_Value_Type::OBJECT)
        .required(true)
        .description("Simple test schema")
        .property("valid_prop", schema::Builder(schema::Property_Value_Type::STRING)
            .required(true)
            .description("Valid property")
            .build())
        .build();

    // Apply schema to datastore
    auto schema_result = datastore->set_schema("", *schema);
    ASSERT_TRUE(schema_result) << "Schema application failed: " << schema_result.error().message();

    // Try setting a property that doesn't exist in schema
    auto result = datastore->set_property("invalid_prop", std::string("test_value"));

    // Check if setting failed as expected
    ASSERT_FALSE(result) << "Setting invalid property should have failed";

    // Check that we get a not found error
    EXPECT_EQ(result.error().code(), tmns::error::Error_Code::NOT_FOUND);
}

/***********************************/
/*      Datastore Tests            */
/***********************************/
TEST_F( fcs_Datastore, create_property_dynamically )
{
    // Try to create a property first, then set its value
    auto string_prop = std::make_shared<tmns::fcs::prop::String_Property>("test_prop");
    auto root = datastore->get_root();
    auto add_result = root->add_property(string_prop);
    if( !add_result ) {
        std::cout << "add_property failed: " << add_result.error().message() << std::endl;
    }

    // Now try to set the property value
    auto result = datastore->set_property("test_prop", std::string("test_value"));
    ASSERT_TRUE(result) << "Setting property failed: " << result.error().message();
}

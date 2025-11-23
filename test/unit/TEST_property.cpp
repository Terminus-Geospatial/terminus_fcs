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
 * @file    property_tests.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/

// C++ Standard Libraries
#include <string>
#include <vector>

// Google Test Libraries
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/fcs/datastore.hpp>
#include <terminus/fcs/prop/typed_property.hpp>
#include <terminus/fcs/prop/object_property.hpp>
#include <terminus/fcs/prop/array_property.hpp>

using namespace tmns::fcs;

/*******************************************/
/*        Test Property Creation           */
/*******************************************/
class fcs_prop_Property : public ::testing::Test
{
    protected:
        void SetUp() override {
            datastore = std::make_unique<Datastore>();
        }

        std::unique_ptr<Datastore> datastore;
};

/*******************************************/
/*        Test basic string property       */
/*******************************************/
TEST_F( fcs_prop_Property, string_property_basic )
{
    // Create a string property
    auto string_prop = std::make_shared<prop::String_Property>("test_string");

    // Add it to the root
    auto root = datastore->get_root();
    auto add_result = root->add_property(string_prop);
    ASSERT_TRUE(add_result) << "Failed to add string property: " << add_result.error().message();

    // Set its value
    auto set_result = datastore->set_property("test_string", std::string("hello world"));
    ASSERT_TRUE(set_result) << "Failed to set string property: " << set_result.error().message();

    // Get the property back
    auto get_result = datastore->get_property("test_string");
    ASSERT_TRUE(get_result) << "Failed to get string property: " << get_result.error().message();

    // Check the value
    auto value_result = get_result.value()->get_value();
    ASSERT_TRUE(value_result) << "Failed to get string value: " << value_result.error().message();

    auto value = std::any_cast<std::string>(value_result.value());
    EXPECT_EQ(value, "hello world");
}

/*******************************************/
/*        Test integer property basic      */
/*******************************************/
TEST_F( fcs_prop_Property, integer_property_basic )
{
    // Create an integer property
    auto int_prop = std::make_shared<prop::Integer_Property>("test_int");

    // Add it to the root
    auto root = datastore->get_root();
    auto add_result = root->add_property(int_prop);
    ASSERT_TRUE(add_result) << "Failed to add integer property: " << add_result.error().message();

    // Set its value
    auto set_result = datastore->set_property("test_int", static_cast<int64_t>(42));
    ASSERT_TRUE(set_result) << "Failed to set integer property: " << set_result.error().message();

    // Get the property back
    auto get_result = datastore->get_property("test_int");
    ASSERT_TRUE(get_result) << "Failed to get integer property: " << get_result.error().message();

    // Check the value
    auto value_result = get_result.value()->get_value();
    ASSERT_TRUE(value_result) << "Failed to get integer value: " << value_result.error().message();

    auto value = std::any_cast<int64_t>(value_result.value());
    EXPECT_EQ(value, 42);
}

/*******************************************/
/*        Test boolean property basic      */
/*******************************************/
TEST_F( fcs_prop_Property, boolean_property_basic )
{
    // Create a boolean property
    auto bool_prop = std::make_shared<prop::Boolean_Property>("test_bool");

    // Add it to the root
    auto root = datastore->get_root();
    auto add_result = root->add_property(bool_prop);
    ASSERT_TRUE(add_result) << "Failed to add boolean property: " << add_result.error().message();

    // Set its value
    auto set_result = datastore->set_property("test_bool", true);
    ASSERT_TRUE(set_result) << "Failed to set boolean property: " << set_result.error().message();

    // Get the property back
    auto get_result = datastore->get_property("test_bool");
    ASSERT_TRUE(get_result) << "Failed to get boolean property: " << get_result.error().message();

    // Check the value
    auto value_result = get_result.value()->get_value();
    ASSERT_TRUE(value_result) << "Failed to get boolean value: " << value_result.error().message();

    auto value = std::any_cast<bool>(value_result.value());
    EXPECT_TRUE(value);
}

/*******************************************/
/*        Test float property basic       */
/*******************************************/
TEST_F( fcs_prop_Property, float_property_basic )
{
    // Create a float property
    auto float_prop = std::make_shared<prop::Float_Property>("test_float");

    // Add it to the root
    auto root = datastore->get_root();
    auto add_result = root->add_property(float_prop);
    ASSERT_TRUE(add_result) << "Failed to add float property: " << add_result.error().message();

    // Set its value
    auto set_result = datastore->set_property("test_float", 3.14159f);
    ASSERT_TRUE(set_result) << "Failed to set float property: " << set_result.error().message();

    // Get the property back
    auto get_result = datastore->get_property("test_float");
    ASSERT_TRUE(get_result) << "Failed to get float property: " << get_result.error().message();

    // Check the value
    auto value_result = get_result.value()->get_value();
    ASSERT_TRUE(value_result) << "Failed to get float value: " << value_result.error().message();

    auto value = std::any_cast<float>(value_result.value());
    EXPECT_NEAR(value, 3.14159f, 0.00001f);
}

/*******************************************/
/*        Test nested object properties    */
/*******************************************/
TEST_F( fcs_prop_Property, nested_object_properties )
{
    // Create a parent object property
    auto parent_obj = std::make_shared<prop::Object_Property>("app");

    // Add it to the root
    auto root = datastore->get_root();
    auto add_result = root->add_property(parent_obj);
    ASSERT_TRUE(add_result) << "Failed to add parent object: " << add_result.error().message();

    // Create a child string property
    auto child_prop = std::make_shared<prop::String_Property>("name");
    auto add_child_result = parent_obj->add_property(child_prop);
    ASSERT_TRUE(add_child_result) << "Failed to add child property: " << add_child_result.error().message();

    // Set the nested property value
    auto set_result = datastore->set_property("app.name", std::string("my_app"));
    ASSERT_TRUE(set_result) << "Failed to set nested property: " << set_result.error().message();

    // Get the property back
    auto get_result = datastore->get_property("app.name");
    ASSERT_TRUE(get_result) << "Failed to get nested property: " << get_result.error().message();

    // Check the value
    auto value_result = get_result.value()->get_value();
    ASSERT_TRUE(value_result) << "Failed to get nested value: " << value_result.error().message();

    auto value = std::any_cast<std::string>(value_result.value());
    EXPECT_EQ(value, "my_app");
}

/*******************************************/
/*        Test deeply nested properties    */
/*******************************************/
TEST_F( fcs_prop_Property, deeply_nested_properties )
{
    // Create the nested structure: app.database.host
    auto app_obj = std::make_shared<prop::Object_Property>("app");
    auto db_obj = std::make_shared<prop::Object_Property>("database");
    auto host_prop = std::make_shared<prop::String_Property>("host");

    // Build the structure
    auto root = datastore->get_root();
    auto add_app = root->add_property(app_obj);
    ASSERT_TRUE(add_app) << "Failed to add app object: " << add_app.error().message();

    auto add_db = app_obj->add_property(db_obj);
    ASSERT_TRUE(add_db) << "Failed to add database object: " << add_db.error().message();

    auto add_host = db_obj->add_property(host_prop);
    ASSERT_TRUE(add_host) << "Failed to add host property: " << add_host.error().message();

    // Set the deeply nested value
    auto set_result = datastore->set_property("app.database.host", std::string("localhost"));
    ASSERT_TRUE(set_result) << "Failed to set deeply nested property: " << set_result.error().message();

    // Get the property back
    auto get_result = datastore->get_property("app.database.host");
    ASSERT_TRUE(get_result) << "Failed to get deeply nested property: " << get_result.error().message();

    // Check the value
    auto value_result = get_result.value()->get_value();
    ASSERT_TRUE(value_result) << "Failed to get deeply nested value: " << value_result.error().message();

    auto value = std::any_cast<std::string>(value_result.value());
    EXPECT_EQ(value, "localhost");
}

/*******************************************/
/*        Test array properties            */
/*******************************************/
TEST_F( fcs_prop_Property, array_property_basic )
{
    // Create an array property
    auto array_prop = std::make_shared<prop::Array_Property>("test_array");

    // Add it to the root
    auto root = datastore->get_root();
    auto add_result = root->add_property(array_prop);
    ASSERT_TRUE(add_result) << "Failed to add array property: " << add_result.error().message();

    // Create some string items
    auto item1 = std::make_shared<prop::String_Property>("item1");
    auto item2 = std::make_shared<prop::String_Property>("item2");

    // Set their values
    item1->set_value(std::string("first"));
    item2->set_value(std::string("second"));

    // Add items to the array
    auto add_item1 = array_prop->add_item(item1);
    ASSERT_TRUE(add_item1) << "Failed to add item1 to array: " << add_item1.error().message();

    auto add_item2 = array_prop->add_item(item2);
    ASSERT_TRUE(add_item2) << "Failed to add item2 to array: " << add_item2.error().message();

    // Get the array back
    auto get_result = datastore->get_property("test_array");
    ASSERT_TRUE(get_result) << "Failed to get array property: " << get_result.error().message();

    // Check that it's an array
    auto array_cast = std::dynamic_pointer_cast<prop::Array_Property>(get_result.value());
    ASSERT_NE(array_cast, nullptr) << "Property is not an array";

    // Check the array size
    EXPECT_EQ(array_cast->size(), 2);
}

/*****************************************/
/*       Double Property Tests           */
/*****************************************/
TEST_F( fcs_prop_Property, double_property_basic )
{
    // Create a double property
    auto prop = std::make_shared<prop::Double_Property>("test_double");

    // Test initial value
    auto get_result = prop->get_value();
    ASSERT_TRUE(get_result) << "Failed to get initial value: " << get_result.error().message();

    auto initial_val = std::any_cast<double>(get_result.value());
    EXPECT_EQ(initial_val, 0.0);

    // Test setting value
    auto set_result = prop->set_value(3.14159);
    ASSERT_TRUE(set_result) << "Failed to set value: " << set_result.error().message();

    // Test getting value back
    get_result = prop->get_value();
    ASSERT_TRUE(get_result) << "Failed to get value: " << get_result.error().message();

    auto val = std::any_cast<double>(get_result.value());
    EXPECT_DOUBLE_EQ(val, 3.14159);

    // Test type information
    EXPECT_EQ(prop->get_type(), schema::Property_Value_Type::DOUBLE);
    EXPECT_EQ(prop->get_type_string(), "double");
}

/*****************************************/
/*        Path Property Tests           */
/*****************************************/
TEST_F( fcs_prop_Property, path_property_basic )
{
    // Create a path property
    auto prop = std::make_shared<prop::Path_Property>("test_path");

    // Test initial value
    auto get_result = prop->get_value();
    ASSERT_TRUE(get_result) << "Failed to get initial value: " << get_result.error().message();

    auto initial_val = std::any_cast<std::filesystem::path>(get_result.value());
    EXPECT_EQ(initial_val, std::filesystem::path());

    // Test setting value
    std::filesystem::path test_path("/tmp/test.txt");
    auto set_result = prop->set_value(test_path);
    ASSERT_TRUE(set_result) << "Failed to set value: " << set_result.error().message();

    // Test getting value back
    get_result = prop->get_value();
    ASSERT_TRUE(get_result) << "Failed to get value: " << get_result.error().message();

    auto val = std::any_cast<std::filesystem::path>(get_result.value());
    EXPECT_EQ(val, test_path);

    // Test type information
    EXPECT_EQ(prop->get_type(), schema::Property_Value_Type::PATH);
    EXPECT_EQ(prop->get_type_string(), "path");
}

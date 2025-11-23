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
 * @file    TEST_config_file_parser.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#include <gtest/gtest.h>

// C++ Standard Libraries
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

// Terminus Libraries
#include <terminus/fcs/config_file_parser.hpp>
#include <terminus/fcs/datastore.hpp>

using namespace tmns::fcs;

/*******************************************/
/*        Test Config File Parser          */
/*******************************************/
class fcs_Config_File_Parser : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            // Create a temporary directory for test files
            test_dir = std::filesystem::temp_directory_path() / "terminus_fcs_test";
            std::filesystem::create_directories(test_dir);
        }

        void TearDown() override
        {
            // Clean up test files
            std::filesystem::remove_all(test_dir);
        }

        std::filesystem::path test_dir;
};

/*******************************************/
/*        Test Basic TOML Parsing          */
/*******************************************/
TEST_F( fcs_Config_File_Parser, basic_toml_parsing )
{
    // Create a simple TOML file
    std::filesystem::path test_file = test_dir / "simple.toml";
    std::ofstream file(test_file);
    file << R"(
name = "test_app"
port = 8080
debug = true
)";
    file.close();

    // Create datastore and parser
    Datastore datastore;
    Config_File_Parser parser;

    // Parse the file
    auto result = parser.parse_file(test_file, datastore, std::nullopt);

    // Check if parsing succeeded
    ASSERT_TRUE(result) << "Parsing failed: " << result.error().message();

    // Note: We can't easily check the datastore contents without schema,
    // but the fact that parsing succeeded is a good start
}

/*******************************************/
/*        Test Nested TOML Parsing         */
/*******************************************/
TEST_F( fcs_Config_File_Parser, nested_toml_parsing )
{
    // Create a TOML file with nested tables
    std::filesystem::path test_file = test_dir / "nested.toml";
    std::ofstream file(test_file);
    file << R"(
[app]
name = "test_app"

[app.database]
host = "localhost"
port = 5432
ssl = true

[app.server]
workers = 4
memory_limit = 1024
)";
    file.close();

    // Create datastore and parser
    Datastore datastore;
    Config_File_Parser parser;

    // Parse the file
    auto result = parser.parse_file(test_file, datastore, std::nullopt);

    // Check if parsing succeeded
    ASSERT_TRUE(result) << "Parsing failed: " << result.error().message();
}

/*******************************************/
/*        Test TOML String Parsing         */
/*******************************************/
TEST_F( fcs_Config_File_Parser, string_parsing )
{
    std::string toml_content = R"(
name = "test_app"
port = 8080
debug = true
)";

    // Create datastore and parser
    Datastore datastore;
    Config_File_Parser parser;

    // Parse the string
    auto result = parser.parse_string(toml_content, datastore, std::nullopt);

    // Check if parsing succeeded
    ASSERT_TRUE(result) << "Parsing failed: " << result.error().message();
}

/*******************************************/
/*        Test Invalid TOML Parsing        */
/*******************************************/
TEST_F( fcs_Config_File_Parser, invalid_toml_parsing )
{
    // Create an invalid TOML file
    std::filesystem::path test_file = test_dir / "invalid.toml";
    std::ofstream file(test_file);
    file << R"(
name = "test_app"
port = 8080
debug = true
invalid_syntax = [
)";
    file.close();

    // Create datastore and parser
    Datastore datastore;
    Config_File_Parser parser;

    // Parse the file
    auto result = parser.parse_file(test_file, datastore, std::nullopt);

    // Check if parsing failed as expected
    ASSERT_FALSE(result) << "Parsing should have failed but succeeded";

    // Check that we get a parsing error
    EXPECT_EQ(result.error().code(), tmns::error::Error_Code::PARSING_ERROR);
}

/*******************************************/
/*        Test File Not Found              */
/*******************************************/
TEST_F( fcs_Config_File_Parser, file_not_found )
{
    // Try to parse a non-existent file
    std::filesystem::path non_existent_file = test_dir / "non_existent.toml";

    // Create datastore and parser
    Datastore datastore;
    Config_File_Parser parser;

    // Parse the file
    auto result = parser.parse_file(non_existent_file, datastore, std::nullopt);

    // Check if parsing failed as expected
    ASSERT_FALSE(result) << "Parsing should have failed but succeeded";

    // Check that we get a file not found error
    EXPECT_EQ(result.error().code(), tmns::error::Error_Code::FILE_NOT_FOUND);
}

/*******************************************/
/*        Test TOML Parsing with Arrays    */
/*******************************************/
TEST_F( fcs_Config_File_Parser, array_parsing )
{
    // Create a TOML file with arrays
    std::filesystem::path test_file = test_dir / "arrays.toml";
    std::ofstream file(test_file);
    file << R"(
servers = ["server1", "server2", "server3"]
ports = [8080, 8081, 8082]
flags = [true, false, true]
)";
    file.close();

    // Create datastore and parser
    Datastore datastore;
    Config_File_Parser parser;

    // Parse the file
    auto result = parser.parse_file(test_file, datastore, std::nullopt);

    // Check if parsing succeeded
    ASSERT_TRUE(result) << "Parsing failed: " << result.error().message();
}

/*******************************************/
/*        Test Different TOML Value Types  */
/*******************************************/
TEST_F( fcs_Config_File_Parser, value_types )
{
    // Create a TOML file with different value types
    std::filesystem::path test_file = test_dir / "types.toml";
    std::ofstream file(test_file);
    file << R"(
string_value = "hello world"
integer_value = 42
float_value = 3.14159
boolean_value = true
)";
    file.close();

    // Create datastore and parser
    Datastore datastore;
    Config_File_Parser parser;

    // Parse the file
    auto result = parser.parse_file(test_file, datastore, std::nullopt);

    // Check if parsing succeeded
    ASSERT_TRUE(result) << "Parsing failed: " << result.error().message();
}

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
 * @file    example_usage.cpp
 * @author  Marvin Smith
 * @date    11/21/2025
 *
 * Example demonstrating how to use the FCS Property system
*/

// C++ Standard Libraries
#include <iostream>
#include <vector>

// Terminus Libraries
#include <terminus/fcs/datastore.hpp>
#include <terminus/fcs/property_schema.hpp>

int main() {
    using namespace tmns::fcs;

    // Create a datastore
    Datastore datastore;

    // Parse command-line style arguments
    std::vector<std::string> args = {
        "-pkey1.key2.key3=value123",
        "-pconfig.database.host=localhost",
        "-pconfig.database.port=5432",
        "-pconfig.database.ssl=true",
        "-pserver.workers=4",
        "-pserver.memory_limit=1024"
    };

    std::cout << "Parsing command-line arguments...\n";
    auto parse_result = datastore.parse_command_line_args(args);
    if (!parse_result) {
        std::cerr << "Failed to parse arguments: " << parse_result.error().message() << std::endl;
        return 1;
    }

    // List all properties
    std::cout << "\nAll properties:\n";
    auto props_result = datastore.list_properties();
    if (props_result) {
        for (const auto& prop_path : props_result.value()) {
            auto prop = datastore.get_property(prop_path);
            if (prop) {
                auto value = prop.value()->get_value();
                if (value) {
                    std::cout << "  " << prop_path << " = "
                              << value.value().type().name() << std::endl;
                }
            }
        }
    }

    // Create a schema for the database configuration
    auto db_schema = SchemaBuilder(PropertyValueType::OBJECT)
        .required(true)
        .description("Database configuration")
        .property("host", SchemaBuilder(PropertyValueType::STRING)
            .required(true)
            .description("Database hostname")
            .build())
        .property("port", SchemaBuilder(PropertyValueType::INTEGER)
            .required(true)
            .range(1, 65535)
            .description("Database port")
            .build())
        .property("ssl", SchemaBuilder(PropertyValueType::BOOLEAN)
            .default_value(false)
            .description("Enable SSL")
            .build())
        .build();

    // Apply the schema
    std::cout << "\nApplying schema to config.database...\n";
    auto schema_result = datastore.set_schema("config.database", db_schema);
    if (!schema_result) {
        std::cerr << "Failed to set schema: " << schema_result.error().message() << std::endl;
        return 1;
    }

    // Validate the database configuration
    std::cout << "Validating config.database...\n";
    auto validate_result = datastore.validate_property("config.database");
    if (!validate_result) {
        std::cerr << "Validation failed: " << validate_result.error().message() << std::endl;
        return 1;
    }
    std::cout << "Validation passed!\n";

    // Get specific values
    std::cout << "\nRetrieving specific values:\n";
    auto host_result = datastore.get_property("config.database.host");
    if (host_result) {
        auto host_prop = std::dynamic_pointer_cast<StringProperty>(host_result.value());
        if (host_prop) {
            auto host_value = host_prop->get_typed_value();
            if (host_value) {
                std::cout << "Database host: " << host_value.value() << std::endl;
            }
        }
    }

    auto port_result = datastore.get_property("config.database.port");
    if (port_result) {
        auto port_prop = std::dynamic_pointer_cast<IntegerProperty>(port_result.value());
        if (port_prop) {
            auto port_value = port_prop->get_typed_value();
            if (port_value) {
                std::cout << "Database port: " << port_value.value() << std::endl;
            }
        }
    }

    // Demonstrate tree navigation
    std::cout << "\nDemonstrating tree navigation:\n";
    auto config_result = datastore.get_property("config");
    if (config_result) {
        auto config_obj = std::dynamic_pointer_cast<ObjectProperty>(config_result.value());
        if (config_obj) {
            std::cout << "Config has children: " << config_obj->has_children() << std::endl;
            auto children = config_obj->get_child_keys();
            std::cout << "Config children: ";
            for (const auto& child : children) {
                std::cout << child << " ";
            }
            std::cout << std::endl;
        }
    }

    std::cout << "\nTotal properties in datastore: " << datastore.size() << std::endl;

    return 0;
}

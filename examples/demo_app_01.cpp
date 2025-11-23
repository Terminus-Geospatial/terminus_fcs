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
#include <terminus/fcs/schema/builder.hpp>
#include <terminus/fcs/configuration.hpp>

using namespace tmns;

int main( int argc, char* argv[], char* envp[] ) {

    // Create a schema for the entire application configuration
    auto app_schema = fcs::schema::Builder( fcs::schema::Property_Value_Type::OBJECT )
        .required(true)
        .description("Application configuration")
        .property("database", fcs::schema::Builder( fcs::schema::Property_Value_Type::OBJECT )
            .required(true)
            .description("Database configuration")
            .property("host", fcs::schema::Builder( fcs::schema::Property_Value_Type::STRING )
                .required(true)
                .description("Database hostname")
                .build())
            .property("port", fcs::schema::Builder( fcs::schema::Property_Value_Type::INTEGER )
                .required(true)
                .range(1, 65535)
                .description("Database port")
                .build())
            .property("ssl", fcs::schema::Builder( fcs::schema::Property_Value_Type::BOOLEAN )
                .default_value(false)
                .description("Enable SSL")
                .build())
            .build())
        .property("server", fcs::schema::Builder( fcs::schema::Property_Value_Type::OBJECT )
            .required(true)
            .description("Server configuration")
            .property("workers", fcs::schema::Builder( fcs::schema::Property_Value_Type::INTEGER )
                .default_value(4)
                .range(1, 64)
                .description("Number of worker threads")
                .build())
            .property("memory_limit", fcs::schema::Builder( fcs::schema::Property_Value_Type::INTEGER )
                .default_value(1024)
                .range(128, 32768)
                .description("Memory limit in MB")
                .build())
            .build())
        .build();


    auto datastore = fcs::parse_command_line( argc, argv, envp, app_schema );
    if( !datastore ) {
        std::cerr << "Failed to parse arguments: " << datastore.error().message() << std::endl;
        return 1;
    }

    std::cout << "Configuration successfully parsed and validated!" << std::endl;

    // List all properties
    std::cout << "\nAll properties:\n";
    auto props_result = datastore.value().list_properties();
    if (props_result) {
        for (const auto& prop_path : props_result.value()) {
            auto prop = datastore.value().get_property(prop_path);
            if (prop) {
                auto value = prop.value()->get_value();
                if (value) {
                    std::cout << "  " << prop_path << " = "
                              << value.value().type().name() << std::endl;
                }
            }
        }
    }



    // Get specific values
    std::cout << "\nRetrieving specific values:\n";
    auto host_result = datastore.value().get_property("app.database.host");
    if (host_result) {
        auto host_prop = std::dynamic_pointer_cast<prop::String_Property>(host_result.value());
        if (host_prop) {
            auto host_value = host_prop->get_typed_value();
            if (host_value) {
                std::cout << "Database host: " << host_value.value() << std::endl;
            }
        }
    }

    auto port_result = datastore.value().get_property("app.database.port");
    if (port_result) {
        auto port_prop = std::dynamic_pointer_cast<prop::Integer_Property>(port_result.value());
        if (port_prop) {
            auto port_value = port_prop->get_typed_value();
            if (port_value) {
                std::cout << "Database port: " << port_value.value() << std::endl;
            }
        }
    }

    // Get server configuration
    auto workers_result = datastore.value().get_property("app.server.workers");
    if (workers_result) {
        auto workers_prop = std::dynamic_pointer_cast<prop::Integer_Property>(workers_result.value());
        if (workers_prop) {
            auto workers_value = workers_prop->get_typed_value();
            if (workers_value) {
                std::cout << "Server workers: " << workers_value.value() << std::endl;
            }
        }
    }

    auto memory_result = datastore.value().get_property("app.server.memory_limit");
    if (memory_result) {
        auto memory_prop = std::dynamic_pointer_cast<prop::Integer_Property>(memory_result.value());
        if (memory_prop) {
            auto memory_value = memory_prop->get_typed_value();
            if (memory_value) {
                std::cout << "Memory limit (MB): " << memory_value.value() << std::endl;
            }
        }
    }

    // Demonstrate tree navigation
    std::cout << "\nDemonstrating tree navigation:\n";
    auto app_result = datastore.value().get_property("app");
    if (app_result) {
        auto app_obj = std::dynamic_pointer_cast<prop::Object_Property>(app_result.value());
        if (app_obj) {
            std::cout << "App has children: " << app_obj->has_children() << std::endl;
            auto children = app_obj->get_child_keys();
            std::cout << "App children: ";
            for (const auto& child : children) {
                std::cout << child << " ";
            }
            std::cout << std::endl;
        }
    }

    std::cout << "\nTotal properties in datastore: " << datastore.value().size() << std::endl;

    return 0;
}

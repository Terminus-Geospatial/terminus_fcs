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
 * @file    args.hpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#pragma once

// C++ Standard Libraries
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/fcs/cmdline/log_level.hpp>

namespace tmns::fcs::cmdline {

class Args {
    public:

        Args() = default;

        /**
         * Parse the command-line arguments into Properties and Log-Severity
         */
        static Result<Args> parse( int argc, char* argv[], char* envp[] );

        /**
         * Get the config file path (if provided)
         */
        std::optional<std::string> get_config_path() const;

        /**
         * Get the log level (if provided)
         */
        std::optional<Log_Level> get_log_level() const;

        /**
         * Get the command-line properties
         */
        const std::map<std::string, std::string>& get_properties() const;

        /**
         * Get the application path
         */
        std::filesystem::path get_app_path() const;

    private:

        /**
         * Parse a property argument in key=value format
         */
        Result<void> parse_property_argument( const std::string& prop_arg );

        /**
         * Print help message
         */
        void print_help() const;

        /**
         * Print usage message
         */
        void print_usage() const;

        std::filesystem::path m_app_path;

        std::optional<std::string> m_config_path;

        std::optional<Log_Level> m_log_level;

        std::map<std::string,std::string> m_properties;
};

} // namespace tmns::fcs::cmdline
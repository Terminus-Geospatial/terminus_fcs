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
 * @file    log_level.hpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#pragma once

// C++ Standard Libraries
#include <string>

namespace tmns::fcs::cmdline {

enum class Log_Level {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
};

std::string log_level_to_string( Log_Level level );

Log_Level string_to_log_level( const std::string& level );

} // namespace tmns::fcs::cmdline
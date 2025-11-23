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
 * @file    log_level.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/
#include <terminus/fcs/cmdline/log_level.hpp>

namespace tmns::fcs::cmdline {

std::string log_level_to_string( Log_Level level ) {
    switch( level ) {
        case Log_Level::TRACE:
            return "TRACE";
        case Log_Level::DEBUG:
            return "DEBUG";
        case Log_Level::INFO:
            return "INFO";
        case Log_Level::WARNING:
            return "WARNING";
        case Log_Level::ERROR:
            return "ERROR";
        case Log_Level::FATAL:
            return "FATAL";
    }
    return "UNKNOWN";
}

Log_Level string_to_log_level( const std::string& level ) {
    if( level == "TRACE" ) {
        return Log_Level::TRACE;
    }
    else if( level == "DEBUG" ) {
        return Log_Level::DEBUG;
    }
    else if( level == "INFO" ) {
        return Log_Level::INFO;
    }
    else if( level == "WARNING" ) {
        return Log_Level::WARNING;
    }
    else if( level == "ERROR" ) {
        return Log_Level::ERROR;
    }
    else if( level == "FATAL" ) {
        return Log_Level::FATAL;
    }
    return Log_Level::INFO;
}

} // namespace tmns::fcs::cmdline
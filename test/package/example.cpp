/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2025 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    example.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
 */

// C++ Standard Libraries
#include <map>
#include <sstream>
#include <string>

// Terminus Libraries
#include <terminus/fcs.hpp>
#include <terminus/log.hpp>

int main()
{
    tmns::log::Logger logger;
    logger.info("Hello World!");

    // Get build information and log this
    auto build_info = tmns::fcs::get_build_info();
    std::stringstream ss;
    ss << "Terminus FCS Build Information:\n";
    for( const auto& [key, value] : build_info )
    {
        ss << key << ": " << value << "\n";
    }
    logger.info(ss.str());

    return 0;
}
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
 * @file    config_file_parser.cpp
 * @author  Marvin Smith
 * @date    11/22/2025
*/

// C++ Standard Libraries
#include <fstream>
#include <iostream>
#include <sstream>

// Terminus Libraries
#include <terminus/fcs/config_file_parser.hpp>
#include "config_file_parser_impl.hpp"

namespace tmns::fcs {

/*********************************/
/*          Constructor          */
/*********************************/
Config_File_Parser::Config_File_Parser()
    : m_impl( std::make_unique<impl::Config_File_Parser_Impl>() )
{}

/*********************************/
/*          Destructor           */
/*********************************/
Config_File_Parser::~Config_File_Parser() = default;

/*********************************/
/*        Parse TOML File        */
/*********************************/
Result<void> Config_File_Parser::parse_file( const std::filesystem::path&    config_path,
                                              Datastore&                      datastore,
                                              std::optional<schema::Schema>   schema )
{
    return m_impl->parse_file( config_path, datastore, schema );
}

/*********************************/
/*      Parse TOML String       */
/*********************************/
Result<void> Config_File_Parser::parse_string( const std::string&              config_content,
                                                Datastore&                      datastore,
                                                std::optional<schema::Schema>   schema )
{
    return m_impl->parse_string( config_content, datastore, schema );
}

/*********************************/
/*     Check File Readable      */
/*********************************/
bool Config_File_Parser::is_file_readable( const std::filesystem::path& config_path )
{
    return impl::Config_File_Parser_Impl::is_file_readable( config_path );
}

/*********************************/
/*      Find Config File       */
/*********************************/
std::filesystem::path Config_File_Parser::find_config_file( const std::string& config_name )
{
    return impl::Config_File_Parser_Impl::find_config_file( config_name );
}

} // End of tmns::fcs namespace

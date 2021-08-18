// This file is part of RaSt mod_ecl.
//
// RaSt mod_ecl is  an extension module for the Apache  HTTP Server.  It enables
// the Apache HTTP  Server to run scripts written in  the Embeddable Common-Lisp
// language.
//
// Copyright (c) 2021 Ralf Stephan (RaSt) <me@ralf-stephan.name>
//
// RaSt mod_ecl is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either  version 3 of the  License, or (at your  option) any later
// version.
//
// RaSt mod_ecl is distributed  in the hope that it will  be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR  A PARTICULAR  PURPOSE.   See the  GNU General  Public  License for  more
// details.
//
// You should have received a copy of  the GNU General Public License along with
// RaSt     mod_ecl     in     the     file    COPYING.      If     not,     see
// <https://www.gnu.org/licenses/>.



/**
 * @file
 *
 * @brief This file contains the core functionality of this module.
 *
 * @details
 */



#ifndef MOD_ECL_H
#define MOD_ECL_H

static int ecl_hook_handler(request_rec * request);
static void * per_directory_configuration_handler(apr_pool_t * pool, char * directory);
static void * per_directory_configuration_merge_handler(apr_pool_t * pool, void * base_configuration, void * new_configuration);
static void * per_server_configuration_handler(apr_pool_t * pool, server_rec * server);
static void * per_server_configuration_merge_handler(apr_pool_t * pool, void * base_configuration, void * new_configuration);
static const command_rec config_file_commands[1];
static void register_hooks(apr_pool_t * pool);
extern module AP_MODULE_DECLARE_DATA ecl_module;

#endif

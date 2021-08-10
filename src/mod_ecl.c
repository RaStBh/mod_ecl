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



/*
**  mod_ecl.c -- Apache sample ecl module
**  [Autogenerated via ``apxs -n ecl -g'']
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory
**  by running:
**
**    $ apxs -c -i mod_ecl.c
**
**  Then activate it in Apache's apache2.conf file for instance
**  for the URL /ecl in as follows:
**
**    #   apache2.conf
**    LoadModule ecl_module modules/mod_ecl.so
**    <Location /ecl>
**    SetHandler ecl
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /ecl and watch for the
**  output of this module. This can be achieved for instance via:
**
**    $ lynx -mime_header http://localhost/ecl
**
**  The output should be similar to the following one:
**
**    HTTP/1.1 200 OK
**    Date: Tue, 31 Mar 1998 14:42:22 GMT
**    Server: Apache/1.3.4 (Unix)
**    Connection: close
**    Content-Type: text/html
**
**    The sample page from mod_ecl.c
*/



/**
 * @mainpage RaSt mod_ecl
 *
 * RaSt mod_ecl is  an extension module for the Apache  HTTP Server.  It enables
 * the Apache HTTP  Server to run scripts written in  the Embeddable Common-Lisp
 * language.
 *
 * Copyright (c) 2021 Ralf Stephan (RaSt)
 * <[mailto:me@ralf-stephan.name](mailto:me@ralf-stephan.name)>
 *
 * RaSt mod_ecl is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either  version 3 of the  License, or (at your  option) any later
 * version.
 *
 * RaSt mod_ecl is distributed  in the hope that it will  be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR  A PARTICULAR  PURPOSE.   See the  GNU General  Public  License for  more
 * details.
 *
 * You should have received a copy of  the GNU General Public License along with
 * RaSt     mod_ecl     in     the     file    COPYING.      If     not,     see
 * <[https://www.gnu.org/licenses/](https://www.gnu.org/licenses/)>.
 *
 * @tableofcontents
 */



/**
 * @file mod_ecl.c
 *
 * @brief This file contains the core functionality of this module.
 *
 * @details
 */



#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

#include "mod_ecl.h"



/* The sample content handler */
static int ecl_handler(request_rec *r)
{
    if (strcmp(r->handler, "application/x-httpd-ecl")) {
        return DECLINED;
    }
    r->content_type = "text/html";

    if (!r->header_only)
        ap_rputs("The sample page from mod_ecl.c\n", r);
    return OK;
}


 
/**
 * @brief Per-directory configuration handler. / Create per-directory config
 *   structures.
 *
 * @details
 *
 * @param[in] pool
 *   The pool to use for all allocations.
 *
 * @param[in] directory
 *   The directory currently being processed.
 *
 * @return
 *   The per-directory structure created.
 */

static void * per_directory_configuration_handler(apr_pool_t * pool, char * directory)
{
  return NULL;
}



/**
 * @brief  Per-directory  configuration  merge handler.  /  Merge  per-directory
 *   config structures.
 *
 * @details
 *
 * @param[in] pool
 *   The pool to use for all allocations.
 *
 * @param[in] base_configuration
 *   The directory structure created for the parent directory.
 *
 * @param[in] new_configuration
 *   The directory structure currently being processed.
 *
 * @return
 *    The new per-directory structure created.
 */

static void * per_directory_configuration_merge_handler(apr_pool_t * pool, void * base_configuration, void * new_configuration)
{
  return NULL;
}



/**
 * @brief  Per-server   configuration  handler.   /  Create   per-server  config
 *   structures.
 *
 * @details
 *
 * @param[in] pool
 *   The pool to use for all allocations.
 *
 * @param[in] server
 *   The server currently being processed.
 *
 * @return
 *    The per-server structure created.
 */

static void * per_server_configuration_handler(apr_pool_t * pool, server_rec * server)
{
  return NULL;
}



/**
 * @brief  Per-server configuration  merge  handler. /  Merge per-server  config
 *   structures.
 *
 * @details
 *
 * @param[in] pool
 *   The pool to use for all allocations.
 *
 * @param[in] base_configuration
 *   The directory structure created for the parent directory.
 *
 * @param[in] new_configuration
 *   The directory structure currently being processed.
 *
 * @return
 *    The new per-directory structure created
 */

static void * per_server_configuration_merge_handler(apr_pool_t * pool, void * base_configuration, void * new_configuration)
{
  return NULL;
}



/**
 * @brief Table of config file commands. / Any directives we may have for httpd.
 *
 * @details
 *
 * List the configuration directives we want to register with the server.
 *
 * Register directives with:
 *
 * * [AP_TAKE_ARGV](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONFIG.html#gaeafa400a7c6388b13a38d469f6cbca4a)
 *   --- This configuration directive will handle its own parsing of arguments.
 *
 * * [AP_TAKE1](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONFIG.html#ga1f074abcaa85b893817adee6ea2dc816)
 *   --- This configuration directive takes 1 argument.
 *
 * * [AP_TAKE2](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONFIG.html#gab5ae54825b0b4b414b21bbd619e6c092)
 *   --- This configuration directive takes 2 argument.
 *
 * * [AP_TAKE3](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONFIG.html#gaacf930c9d842396ad08f860e3f25b9ec)
 *
 *   This configuration directive takes 3 argument.
 *
 * @see [Apache HTTP Server --- Core routines --- Configuration](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONFIG.html)
 */

static const command_rec config_file_commands[] =
{
  {NULL}
};



/**
 * @brief Register hooks. / Our hook registering function.
 *
 * @details
 *
 * A hook to allow  modules to hook other points in  the request processing.  In
 * this function, modules  should call the ap_hook_*() functions  to register an
 * interest in a specific step in processing the current request.
 *
 * Available hooks functions:
 *
 * * ap_hook_access_checker
 *
 * * ap_hook_access_checker_ex
 *
 * * ap_hook_auth_checker
 *
 * * ap_hook_check_config
 *
 * * ap_hook_check_user_id
 *
 * * ap_hook_child_init
 *
 * * ap_hook_child_status
 *
 * * ap_hook_create_connection
 *
 * * ap_hook_create_request
 *
 * * ap_hook_default_port
 *
 * * ap_hook_dirwalk_stat
 *
 * * ap_hook_drop_privileges
 *
 * * ap_hook_end_generation
 *
 * * ap_hook_error_log
 *
 * * ap_hook_expr_lookup
 *
 * * ap_hook_fatal_exception
 *
 * * ap_hook_fixups
 *
 * * ap_hook_force_authn
 *
 * * ap_hook_generate_log_id
 *
 * * ap_hook_get_mgmt_items
 *
 * * ap_hook_get_suexec_identity
 *
 * * ap_hook_handler
 *
 * * ap_hook_header_parser
 *
 * * ap_hook_http_scheme
 *
 * * ap_hook_insert_error_filter
 *
 * * ap_hook_insert_filter
 *
 * * ap_hook_insert_network_bucket
 *
 * * ap_hook_log_transaction
 *
 * * ap_hook_map_to_storage
 *
 * * ap_hook_monitor
 *
 * * ap_hook_mpm
 *
 * * ap_hook_mpm_get_name
 *
 * * ap_hook_mpm_query
 *
 * * ap_hook_mpm_register_timed_callback
 *
 * * ap_hook_note_auth_failure
 *
 * * ap_hook_open_htaccess
 *
 * * ap_hook_open_logs
 *
 * * ap_hook_optional_fn_retrieve
 *
 * * ap_hook_optional_hook_test
 *
 * * ap_hook_post_config
 *
 * * ap_hook_post_perdir_config
 *
 * * ap_hook_post_read_request
 *
 * * ap_hook_pre_close_connectio
 *
 * * ap_hook_pre_config
 *
 * * ap_hook_pre_connection
 *
 * * ap_hook_pre_mpm
 *
 * * ap_hook_pre_read_request
 *
 * * ap_hook_process_connection
 *
 * * ap_hook_protocol_get
 *
 * * ap_hook_protocol_propose
 *
 * * ap_hook_protocol_switch
 *
 * * ap_hook_quick_handler
 *
 * * ap_hook_resume_connection
 *
 * * ap_hook_ssl_add_cert_files
 *
 * * ap_hook_ssl_add_fallback_cert_files
 *
 * * ap_hook_ssl_answer_challenge
 *
 * * ap_hook_ssl_conn_is_ssl
 *
 * * ap_hook_ssl_ocsp_get_resp_hook
 *
 * * ap_hook_ssl_ocsp_prime_hook
 *
 * * ap_hook_ssl_var_lookup
 *
 * * ap_hook_suspend_connection
 *
 * * ap_hook_test_config
 *
 * * ap_hook_translate_name
 *
 * * ap_hook_type_checker
 *
 * @see [Apache HTTP Server --- Core routines --- Apache Hooks](https://ci.apache.org/projects/httpd/trunk/doxygen/group__hooks.html)
 *
 * @see [ap_expr.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__expr_8h.html)
 * @see [ap_expr.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__expr_8h_source.html)
 *
 * @see [ap_hooks.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__hooks_8h.html)
 * @see [ap_hooks.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__hooks_8h_source.html)
 *
 * @see [ap_mpm.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__mpm_8h.html)
 * @see [ap_mpm.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__mpm_8h_source.html)
 *
 * @see [http_config.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__config_8h.html)
 * @see [http_config.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/http__config_8h_source.html)
 *
 * @see [http_connection.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__connection_8h.html)
 * @see [http_connection.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/http__connection_8h_source.html)
 *
 * @see [http_core.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__core_8h.html)
 * @see [http_core.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/http__core_8h_source.html)
 *
 * @see [http_log.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__log_8h.html)
 * @see [http_log.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/http__log_8h_source.html)
 *
 * @see [http_protocol.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__protocol_8h.html)
 * @see [http_protocol.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/http__protocol_8h_source.html)
 *
 * @see [http_request.h File Reference]](https://ci.apache.org/projects/httpd/trunk/doxygen/http__request_8h.html)
 * @see [http_request.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/http__request_8h_source.html)
 *
 * @see [http_ssl.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__ssl_8h.html)
 * @see [http_ssl.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/http__ssl_8h_source.html)
 *
 * @see [mpm_common.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/mpm__common_8h.html)
 * @see [mpm_common.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/mpm__common_8h_source.html)
 *
 * @see [scoreboard.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/scoreboard_8h.html)
 * @see [scoreboard.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/scoreboard_8h_source.html)
 *
 * @see [mod_optional_hook_export File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/mod__optional__hook__export_8h.html)
 * @see [mod_optional_hook_export Source](https://ci.apache.org/projects/httpd/trunk/doxygen/mod__optional__hook__export_8h_source.html)
 =
 * @see [unixd.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/unixd_8h.html)
 * @see [unixd.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/unixd_8h_source.html)
 *
 * @param[in] pool
 *   This is the pool to use for all allocations.
 *
 * @return void
 */

static void register_hooks(apr_pool_t * pool)
{
    ap_hook_handler(ecl_handler, NULL, NULL, APR_HOOK_MIDDLE);
}



/**
 * @brief Register mod_ecl as Apache HTTP Server module.
 *
 * @details
 *
 * This is for setting up configuration handlers and directives.
 *
 *
 *
 * [AP_MODULE_DECLARE_DATA](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__config_8h.html#ae2cb2b956e7f274f8d91581331debbe0)
 *   --- Apache  Core dso  variables are  declared with  AP_MODULE_DECLARE_DATA.
 *   This  assures  the appropriate  indirection  is  invoked at  compile  time.
 *   Symbols declared with AP_MODULE_DECLARE_DATA are always exported.
 *
 * @see [ap_config.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__config_8h.html)
 * @see [ap_config.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__config_8h_source.html)
 *
 * @see [macros.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/macros_8h.html)
 * @see [macros.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/macros_8h_source.html)
 *
 *
 *
 * Dispatch list for API hooks:
 *
 * * [STANDARD20_MODULE_STUFF](https://ci.apache.org/projects/httpd/trunk/doxygen/group__ModuleInit.html#ga852c139212175cbc1e47a608a198da13)
 *   --- Use this  in all standard modules.  / FIll in with  boilerplate code to
 *   make this module a standard 2.x module.
 *
 * * per_directory_configuration_handler    ---   Per-directory    configuration
 *   handler. / Create per-directory config structures.
 *
 * * per_directory_configuration_merge_handler  --- Per-directory  configuration
 *   merge handler. / Merge per-directory config structures.
 *
 * * per_server_configuration_handler ---  Per-server configuration  handler.  /
 *   Create per-server config structures.
 *
 * * per_server_configuration_merge_handler  --- Per-server  configuration merge
 *   handler. / Merge per-server config structures.
 *
 * * config_file_commands --- Table of config file commands. / Any directives we
 *   may have for httpd.
 *
 * * register_hooks --- Register hooks. / Our hook registering function.
 *
 * @see [Apache HTTP Server --- Core routines --- Configuration --- Module structure initializers](https://ci.apache.org/projects/httpd/trunk/doxygen/group__ModuleInit.html)
 *
 * @see [http_config.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__config_8h.html)
 * @see [http_config.h Source](https://ci.apache.org/projects/httpd/trunk/doxygen/http__config_8h_source.html)
 *
 *
 *
 * [module](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONFIG.html#ga0ea4f633a5f9f88e1603aaeb1f2b2e69):
 *
 * @code{.c}typedef struct module_struct module@endcode
 *
 *
 *
 * [module_struct](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html)
 *  Data Fields:
 *
 * * [version](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a9fe73d6d2e8c6e3a0cfa0b7f14a1fcef)
 *   --- API version, not  module version; check that module  is compatible with
 *   this version of the server.
 *
 * * [minor_version](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a89a9373f6c817795a0580bb2deaef72c)
 *   --- API minor version.  Provides API feature milestones. Not checked during
 *   module init.
 *
 * * [module_index](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a03b0e116532b88363264cb60d98a919b)
 *   --- Index to this modules structures in config vectors.
 *
 * * [name](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a88f43c2576387678ac534313fd0e7ece)
 *   --- The name of the module's C file.
 *
 * * [dynamic_load_handle](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#af80ce97e178e8d98add86954cccd09f4)
 *   --- The handle for the DSO.  Internal use only.
 *
 * * [next](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#aacf77ae0ad30879346ae153037ed41ea)
 *   --- A pointer to the next module in the list.
 *
 * * [magic](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a7394965e36ccaa275a5cc4f64ccb354c)
 *   --- Magic Cookie to identify a module structure.  It's mainly important for
 *   the DSO facility (see also mod_so).
 *
 * * [rewrite_args](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a413175b9e3a97400995e7f03d6954dee)
 *   --- Function to allow  MPMs to re-write command line  arguments.  This hook
 *   is only available to MPMs.
 *
 * * [create_dir_config](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#ae3ea1f05d13c0d1e7f7e1fa3a34edf1c)
 *   --- Function to  allow all  modules to  create per  directory configuration
 *   structures.
 *
 * * [merge_dir_config](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a7c75a2a7334554c5c92ddfae35bd4ab8)
 *   --- Function to allow all modules  to merge the per directory configuration
 *   structures for two directories.
 *
 * * [create_server_config](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a486e6f27e4b1d885fd4ea21f22b38a68)
 *    --- Function  to allow  all  modules to  create  per server  configuration
 *    structures.
 *
 * * [merge_server_config](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a4bab1e811d95219325c24a431be75c6f)
 *   --- Function to  allow all  modules to merge  the per  server configuration
 *   structures for two servers.
 *
 * * [cmds](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#ad2bdc0c0e8af7118b21bde0911c74cca)
 *   --- A command_rec  table that describes  all of the directives  this module
 *   defines.
 *
 * * [register_hooks](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#a628b4e635e922228275919f1b595c105)
 *   --- A hook to allow modules to hook other points in the request processing.
 *   In this function, modules should call the ap_hook_*() functions to register
 *   an interest in a specific step in processing the current request.
 *
 * * [flags](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html#af0c4e456d45b11fbaaabec8203886592)
 *   --- A bitmask of AP_MODULE_FLAG_*.
 *
 * @see [Apache HTTP Server --- Core routines --- Configuration](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONFIG.html)
 *
 * @see [Apache HTTP Server --- Core routines --- Configuration --- module_struct Struct Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html)
 */

module AP_MODULE_DECLARE_DATA ecl_module =
{
  STANDARD20_MODULE_STUFF,                   // STANDARD20 MODULE STUFF
  per_directory_configuration_handler,       // Per-directory configuration handler.
  per_directory_configuration_merge_handler, // Per-directory configuration merge handler.
  per_server_configuration_handler,          // Per-server configuration handler.
  per_server_configuration_merge_handler,    // Per-server configuration merge handler.
  config_file_commands,                      // Config file Commands.
  register_hooks                             // Register hooks.
};

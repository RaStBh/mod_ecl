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

/* Register hooks */
static void register_hooks(apr_pool_t *p)
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
 * AP_MODULE_DECLARE_DATA  ---  Apache  Core  dso variables  are  declared  with
 *   AP_MODULE_DECLARE_DATA.   This  assures   the  appropriate  indirection  is
 *   invoked at compile time.   Symbols declared with AP_MODULE_DECLARE_DATA are
 *   always exported.
 *
 * Dispatch list for API hooks:
 *
 * * STANDARD20_MODULE_STUFF ---  Use this  in all standard  modules. /  FIll in
 *   with boilerplate code to make this module a standard 2.x module.
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
 * @code{.c}typedef struct module_struct module@endcode
 *
 * module_struct Data Fields:
 *
 * * version  ---  API  version,  not  module  version;  check  that  module  is
 *   compatible with this version of the server.
 *
 * * minor_version --- API minor version.   Provides API feature milestones. Not
 *   checked during module init.
 *
 * * module_index --- Index to this modules structures in config vectors.
 *
 * * name --- The name of the module's C file.
 *
 * * dynamic_load_handle --- The handle for the DSO.  Internal use only.
 *
 * * next --- A pointer to the next module in the list.
 *
 * * magic  ---  Magic Cookie  to  identify  a  module structure.   It's  mainly
 *   important for the DSO facility (see also mod_so).
 *
 * * rewrite_args --- Function to allow MPMs to re-write command line arguments.
 *   This hook is only available to MPMs.
 *
 * * create_dir_config --- Function to allow all modules to create per directory
 *   configuration structures.
 *
 * * merge_dir_config  ---  Function to  allow  all  modules  to merge  the  per
 *   directory configuration structures for two directories.
 *
 * * create_server_config --- Function to allow all modules to create per server
 *    configuration structures.
 *
 * * merge_server_config  --- Function  to allow  all modules  to merge  the per
 *   server configuration structures for two servers.
 *
 * * cmds ---  A command_rec  table that  describes all  of the  directives this
 *   module defines.
 *
 * * register_hooks ---  A hook  to allow  modules to hook  other points  in the
 *   request processing.  In this function,  modules should call the ap_hook_*()
 *   functions to  register an  interest in  a specific  step in  processing the
 *   current request.
 *
 * * flags --- A bitmask of AP_MODULE_FLAG_*.
 *
 * @see [Apache HTTP Server - Core routines - Configuration](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONFIG.html)
 *
 * @see [Apache HTTP Server - Core routines - Configuration - module_struct Struct Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/structmodule__struct.html)
 *
 * @see [ap_config.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/ap__config_8h.html)
 * @see [ap_config.h](file://httpd-2.4.48/include/ap_config.h)
 *
 * @see [http_config.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__config_8h.html)
 * @see [http_config.h](httpd-2.4.48/include/http_config.h)
 *
 * @see [macros.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/macros_8h.html)
 * @see [macros.h](file://httpd-2.4.48/include/macros.h)
 */

module AP_MODULE_DECLARE_DATA ecl_module =
{
  STANDARD20_MODULE_STUFF, // STANDARD20 MODULE STUFF
  NULL,                    // Per-directory configuration handler.
  NULL,                    // Per-directory configuration merge handler.
  NULL,                    // Per-server configuration handler.
  NULL,                    // Per-server configuration merge handler.
  NULL,                    // Config file Commands.
  register_hooks           // Register hooks.
};

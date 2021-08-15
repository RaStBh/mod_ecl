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
 * @brief struct server_rec: A structure to store information for each virtual 
 *   server.
 *
 * @details
 * 
 * @see [server_rec Struct Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/structserver__rec.html)
 *
 * @see [http.h Source Code](https://ci.apache.org/projects/httpd/trunk/doxygen/httpd_8h_source.html)
 */



///**
// * @brief A structure to store information for each virtual server
// */
//struct server_rec {
//    /** The process this server is running in */
//    process_rec *process;
//    /** The next server in the list */
//    server_rec *next;
//
//    /* Log files --- note that transfer log is now in the modules... */
//
//    /** The name of the error log */
//    char *error_fname;
//    /** A file descriptor that references the error log */
//    apr_file_t *error_log;
//    /** The log level configuration */
//    struct ap_logconf log;
//
//    /* Module-specific configuration for server, and defaults... */
//
//    /** Config vector containing pointers to modules' per-server config
//     *  structures. */
//    struct ap_conf_vector_t *module_config;
//    /** MIME type info, etc., before we start checking per-directory info */
//    struct ap_conf_vector_t *lookup_defaults;
//
//    /** The path to the config file that the server was defined in */
//    const char *defn_name;
//    /** The line of the config file that the server was defined on */
//    unsigned defn_line_number;
//    /** true if this is the virtual server */
//    char is_virtual;
//
//
//    /* Information for redirects */
//
//    /** for redirects, etc. */
//    apr_port_t port;
//    /** The server request scheme for redirect responses */
//    const char *server_scheme;
//
//    /* Contact information */
//
//    /** The admin's contact information */
//    char *server_admin;
//    /** The server hostname */
//    char *server_hostname;
//
//    /* Transaction handling */
//
//    /** I haven't got a clue */
//    server_addr_rec *addrs;
//    /** Timeout, as an apr interval, before we give up */
//    apr_interval_time_t timeout;
//    /** The apr interval we will wait for another request */
//    apr_interval_time_t keep_alive_timeout;
//    /** Maximum requests per connection */
//    int keep_alive_max;
//    /** Use persistent connections? */
//    int keep_alive;
//
//    /** Normal names for ServerAlias servers */
//    apr_array_header_t *names;
//    /** Wildcarded names for ServerAlias servers */
//    apr_array_header_t *wild_names;
//
//    /** Pathname for ServerPath */
//    const char *path;
//    /** Length of path */
//    int pathlen;
//
//    /** limit on size of the HTTP request line    */
//    int limit_req_line;
//    /** limit on size of any request header field */
//    int limit_req_fieldsize;
//    /** limit on number of request header fields  */
//    int limit_req_fields;
//
//    /** Opaque storage location */
//    void *context;
//
//    /** Whether the keepalive timeout is explicit (1) or
//     *  inherited (0) from the base server (either first
//     *  server on the same IP:port or main server) */
//    unsigned int keep_alive_timeout_set:1;
//};

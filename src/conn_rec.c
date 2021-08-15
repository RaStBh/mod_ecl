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
 * @brief struct conn_rec: Structure to store things which are per connection.
 *
 * @details
 * 
 * @see [conn_rec Struct Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/structconn__rec.html)
 *
 * @see [http.h Source Code](https://ci.apache.org/projects/httpd/trunk/doxygen/httpd_8h_source.html)
 */



//struct conn_rec {
//    /** Pool associated with this connection */
//    apr_pool_t *pool;
//    /** Physical vhost this conn came in on */
//    server_rec *base_server;
//    /** used by http_vhost.c */
//    void *vhost_lookup_data;
//
//    /* Information about the connection itself */
//    /** local address */
//    apr_sockaddr_t *local_addr;
//    /** remote address; this is the end-point of the next hop, for the address
//     *  of the request creator, see useragent_addr in request_rec
//     */
//    apr_sockaddr_t *client_addr;
//
//    /** Client's IP address; this is the end-point of the next hop, for the
//     *  IP of the request creator, see useragent_ip in request_rec
//     */
//    char *client_ip;
//    /** Client's DNS name, if known.  NULL if DNS hasn't been checked,
//     *  "" if it has and no address was found.  N.B. Only access this though
//     * get_remote_host() */
//    char *remote_host;
//    /** Only ever set if doing rfc1413 lookups.  N.B. Only access this through
//     *  get_remote_logname() */
//    char *remote_logname;
//
//    /** server IP address */
//    char *local_ip;
//    /** used for ap_get_server_name when UseCanonicalName is set to DNS
//     *  (ignores setting of HostnameLookups) */
//    char *local_host;
//
//    /** ID of this connection; unique at any point in time */
//    long id;
//    /** Config vector containing pointers to connections per-server
//     *  config structures. */
//    struct ap_conf_vector_t *conn_config;
//    /** Notes on *this* connection: send note from one module to
//     *  another. must remain valid for all requests on this conn */
//    apr_table_t *notes;
//    /** A list of input filters to be used for this connection */
//    struct ap_filter_t *input_filters;
//    /** A list of output filters to be used for this connection */
//    struct ap_filter_t *output_filters;
//    /** handle to scoreboard information for this connection */
//    void *sbh;
//    /** The bucket allocator to use for all bucket/brigade creations */
//    struct apr_bucket_alloc_t *bucket_alloc;
//    /** The current state of this connection; may be NULL if not used by MPM */
//    conn_state_t *cs;
//    /** Is there data pending in the input filters? */
//    int data_in_input_filters;
//    /** Is there data pending in the output filters? */
//    int data_in_output_filters;
//
//    /** Are there any filters that clogg/buffer the input stream, breaking
//     *  the event mpm.
//     */
//    unsigned int clogging_input_filters:1;
//
//    /** have we done double-reverse DNS? -1 yes/failure, 0 not yet,
//     *  1 yes/success */
//    signed int double_reverse:2;
//
//    /** Are we still talking? */
//    unsigned aborted;
//
//    /** Are we going to keep the connection alive for another request?
//     * @see ap_conn_keepalive_e */
//    ap_conn_keepalive_e keepalive;
//
//    /** How many times have we used it? */
//    int keepalives;
//
//    /** Optional connection log level configuration. May point to a server or
//     *  per_dir config, i.e. must be copied before modifying */
//    const struct ap_logconf *log;
//
//    /** Id to identify this connection in error log. Set when the first
//     *  error log entry for this connection is generated.
//     */
//    const char *log_id;
//
//
//    /** This points to the current thread being used to process this request,
//     * over the lifetime of a request, the value may change. Users of the connection
//     * record should not rely upon it staying the same between calls that involve
//     * the MPM.
//     */
//#if APR_HAS_THREADS
//    apr_thread_t *current_thread;
//#endif
//
//    /** The "real" master connection. NULL if I am the master. */
//    conn_rec *master;
//};

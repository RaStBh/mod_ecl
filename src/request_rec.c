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
 * @brief struct request_rec: A structure that represents the current request.
 *
 * @details
 *
 * @see {request_rec Struct Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/structrequest__rec.html)
 *
 * @see [http.h Source Code](https://ci.apache.org/projects/httpd/trunk/doxygen/httpd_8h_source.html)
 */



//==============================================================================
//
// Headers to include.
//
//==============================================================================



// Headers from Apache HTTP Server.

// Header from Apache Request Library.

// Header from RaSt mod_ecl.



//==============================================================================
//
// Data from struct request_rec.
//
//==============================================================================



//struct request_rec {
//    /** The pool associated with the request */
//    apr_pool_t *pool;
//    /** The connection to the client */
//    conn_rec *connection;
//    /** The virtual host for this request */
//    server_rec *server;
//
//    /** Pointer to the redirected request if this is an external redirect */
//    request_rec *next;
//    /** Pointer to the previous request if this is an internal redirect */
//    request_rec *prev;
//
//    /** Pointer to the main request if this is a sub-request
//     * (see http_request.h) */
//    request_rec *main;
//
//    /* Info about the request itself... we begin with stuff that only
//     * protocol.c should ever touch...
//     */
//    /** First line of request */
//    char *the_request;
//    /** HTTP/0.9, "simple" request (e.g. GET /foo\n w/no headers) */
//    int assbackwards;
//    /** A proxy request (calculated during post_read_request/translate_name)
//     *  possible values PROXYREQ_NONE, PROXYREQ_PROXY, PROXYREQ_REVERSE,
//     *                  PROXYREQ_RESPONSE
//     */
//    int proxyreq;
//    /** HEAD request, as opposed to GET */
//    int header_only;
//    /** Protocol version number of protocol; 1.1 = 1001 */
//    int proto_num;
//    /** Protocol string, as given to us, or HTTP/0.9 */
//    char *protocol;
//    /** Host, as set by full URI or Host: header.
//     *  For literal IPv6 addresses, this does NOT include the surrounding [ ]
//     */
//    const char *hostname;
//
//    /** Time when the request started */
//    apr_time_t request_time;
//
//    /** Status line, if set by script */
//    const char *status_line;
//    /** Status line */
//    int status;
//
//    /* Request method, two ways; also, protocol, etc..  Outside of protocol.c,
//     * look, but don't touch.
//     */
//
//    /** M_GET, M_POST, etc. */
//    int method_number;
//    /** Request method (eg. GET, HEAD, POST, etc.) */
//    const char *method;
//
//    /**
//     *  'allowed' is a bitvector of the allowed methods.
//     *
//     *  A handler must ensure that the request method is one that
//     *  it is capable of handling.  Generally modules should DECLINE
//     *  any request methods they do not handle.  Prior to aborting the
//     *  handler like this the handler should set r->allowed to the list
//     *  of methods that it is willing to handle.  This bitvector is used
//     *  to construct the "Allow:" header required for OPTIONS requests,
//     *  and HTTP_METHOD_NOT_ALLOWED and HTTP_NOT_IMPLEMENTED status codes.
//     *
//     *  Since the default_handler deals with OPTIONS, all modules can
//     *  usually decline to deal with OPTIONS.  TRACE is always allowed,
//     *  modules don't need to set it explicitly.
//     *
//     *  Since the default_handler will always handle a GET, a
//     *  module which does *not* implement GET should probably return
//     *  HTTP_METHOD_NOT_ALLOWED.  Unfortunately this means that a Script GET
//     *  handler can't be installed by mod_actions.
//     */
//    apr_int64_t allowed;
//    /** Array of extension methods */
//    apr_array_header_t *allowed_xmethods;
//    /** List of allowed methods */
//    ap_method_list_t *allowed_methods;
//
//    /** byte count in stream is for body */
//    apr_off_t sent_bodyct;
//    /** body byte count, for easy access */
//    apr_off_t bytes_sent;
//    /** Last modified time of the requested resource */
//    apr_time_t mtime;
//
//    /* HTTP/1.1 connection-level features */
//
//    /** The Range: header */
//    const char *range;
//    /** The "real" content length */
//    apr_off_t clength;
//    /** sending chunked transfer-coding */
//    int chunked;
//
//    /** Method for reading the request body
//     * (eg. REQUEST_CHUNKED_ERROR, REQUEST_NO_BODY,
//     *  REQUEST_CHUNKED_DECHUNK, etc...) */
//    int read_body;
//    /** reading chunked transfer-coding */
//    int read_chunked;
//    /** is client waiting for a 100 response? */
//    unsigned expecting_100;
//    /** The optional kept body of the request. */
//    apr_bucket_brigade *kept_body;
//    /** For ap_body_to_table(): parsed body */
//    /* XXX: ap_body_to_table has been removed. Remove body_table too or
//     * XXX: keep it to reintroduce ap_body_to_table without major bump? */
//    apr_table_t *body_table;
//    /** Remaining bytes left to read from the request body */
//    apr_off_t remaining;
//    /** Number of bytes that have been read  from the request body */
//    apr_off_t read_length;
//
//    /* MIME header environments, in and out.  Also, an array containing
//     * environment variables to be passed to subprocesses, so people can
//     * write modules to add to that environment.
//     *
//     * The difference between headers_out and err_headers_out is that the
//     * latter are printed even on error, and persist across internal redirects
//     * (so the headers printed for ErrorDocument handlers will have them).
//     *
//     * The 'notes' apr_table_t is for notes from one module to another, with no
//     * other set purpose in mind...
//     */
//
//    /** MIME header environment from the request */
//    apr_table_t *headers_in;
//    /** MIME header environment for the response */
//    apr_table_t *headers_out;
//    /** MIME header environment for the response, printed even on errors and
//     * persist across internal redirects */
//    apr_table_t *err_headers_out;
//    /** Array of environment variables to be used for sub processes */
//    apr_table_t *subprocess_env;
//    /** Notes from one module to another */
//    apr_table_t *notes;
//
//    /* content_type, handler, content_encoding, and all content_languages
//     * MUST be lowercased strings.  They may be pointers to static strings;
//     * they should not be modified in place.
//     */
//    /** The content-type for the current request */
//    const char *content_type;   /* Break these out --- we dispatch on 'em */
//    /** The handler string that we use to call a handler function */
//    const char *handler;        /* What we *really* dispatch on */
//
//    /** How to encode the data */
//    const char *content_encoding;
//    /** Array of strings representing the content languages */
//    apr_array_header_t *content_languages;
//
//    /** variant list validator (if negotiated) */
//    char *vlist_validator;
//
//    /** If an authentication check was made, this gets set to the user name. */
//    char *user;
//    /** If an authentication check was made, this gets set to the auth type. */
//    char *ap_auth_type;
//
//    /* What object is being requested (either directly, or via include
//     * or content-negotiation mapping).
//     */
//
//    /** The URI without any parsing performed */
//    char *unparsed_uri;
//    /** The path portion of the URI, or "/" if no path provided */
//    char *uri;
//    /** The filename on disk corresponding to this response */
//    char *filename;
//    /** The true filename stored in the filesystem, as in the true alpha case
//     *  and alias correction, e.g. "Image.jpeg" not "IMAGE$1.JPE" on Windows.
//     *  The core map_to_storage canonicalizes r->filename when they mismatch */
//    char *canonical_filename;
//    /** The PATH_INFO extracted from this request */
//    char *path_info;
//    /** The QUERY_ARGS extracted from this request */
//    char *args;
//
//    /**
//     * Flag for the handler to accept or reject path_info on
//     * the current request.  All modules should respect the
//     * AP_REQ_ACCEPT_PATH_INFO and AP_REQ_REJECT_PATH_INFO
//     * values, while AP_REQ_DEFAULT_PATH_INFO indicates they
//     * may follow existing conventions.  This is set to the
//     * user's preference upon HOOK_VERY_FIRST of the fixups.
//     */
//    int used_path_info;
//
//    /** A flag to determine if the eos bucket has been sent yet */
//    int eos_sent;
//
//    /* Various other config info which may change with .htaccess files
//     * These are config vectors, with one void* pointer for each module
//     * (the thing pointed to being the module's business).
//     */
//
//    /** Options set in config files, etc. */
//    struct ap_conf_vector_t *per_dir_config;
//    /** Notes on *this* request */
//    struct ap_conf_vector_t *request_config;
//
//    /** Optional request log level configuration. Will usually point
//     *  to a server or per_dir config, i.e. must be copied before
//     *  modifying */
//    const struct ap_logconf *log;
//
//    /** Id to identify request in access and error log. Set when the first
//     *  error log entry for this request is generated.
//     */
//    const char *log_id;
//
//    /**
//     * A linked list of the .htaccess configuration directives
//     * accessed by this request.
//     * N.B. always add to the head of the list, _never_ to the end.
//     * that way, a sub request's list can (temporarily) point to a parent's list
//     */
//    const struct htaccess_result *htaccess;
//
//    /** A list of output filters to be used for this request */
//    struct ap_filter_t *output_filters;
//    /** A list of input filters to be used for this request */
//    struct ap_filter_t *input_filters;
//
//    /** A list of protocol level output filters to be used for this
//     *  request */
//    struct ap_filter_t *proto_output_filters;
//    /** A list of protocol level input filters to be used for this
//     *  request */
//    struct ap_filter_t *proto_input_filters;
//
//    /** This response can not be cached */
//    int no_cache;
//    /** There is no local copy of this response */
//    int no_local_copy;
//
//    /** Mutex protect callbacks registered with ap_mpm_register_timed_callback
//     * from being run before the original handler finishes running
//     */
//    apr_thread_mutex_t *invoke_mtx;
//
//    /** A struct containing the components of URI */
//    apr_uri_t parsed_uri;
//    /**  finfo.protection (st_mode) set to zero if no such file */
//    apr_finfo_t finfo;
//
//    /** remote address information from conn_rec, can be overridden if
//     * necessary by a module.
//     * This is the address that originated the request.
//     */
//    apr_sockaddr_t *useragent_addr;
//    char *useragent_ip;
//
//    /** MIME trailer environment from the request */
//    apr_table_t *trailers_in;
//    /** MIME trailer environment from the response */
//    apr_table_t *trailers_out;
//
//    /** Originator's DNS name, if known.  NULL if DNS hasn't been checked,
//     *  "" if it has and no address was found.  N.B. Only access this though
//     *  ap_get_useragent_host() */
//    char *useragent_host;
//    /** have we done double-reverse DNS? -1 yes/failure, 0 not yet,
//     *  1 yes/success
//     */
//    int double_reverse;
//    /** Request flags associated with this request. Use
//     * AP_REQUEST_GET_FLAGS() and AP_REQUEST_SET_FLAGS() to access
//     * the elements of this field.
//     */
//    ap_request_bnotes_t bnotes;
//};

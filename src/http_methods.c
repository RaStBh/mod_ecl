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
 * @brief Methods recognized (but not necessarily handled) by the server.
 *
 * @details
 * 
 * @see [http.h Source Code](https://ci.apache.org/projects/httpd/trunk/doxygen/httpd_8h_source.html)
 */



///**
// * @defgroup Methods List of Methods recognized by the server
// * @ingroup APACHE_CORE_DAEMON
// * @{
// *
// * @brief Methods recognized (but not necessarily handled) by the server.
// *
// * These constants are used in bit shifting masks of size int, so it is
// * unsafe to have more methods than bits in an int.  HEAD == M_GET.
// * This list must be tracked by the list in http_protocol.c in routine
// * ap_method_name_of().
// *
// */
//
//#define M_GET                   0       /** RFC 2616: HTTP */
//#define M_PUT                   1       /*  :             */
//#define M_POST                  2
//#define M_DELETE                3
//#define M_CONNECT               4
//#define M_OPTIONS               5
//#define M_TRACE                 6       /** RFC 2616: HTTP */
//#define M_PATCH                 7       /** RFC 5789: PATCH Method for HTTP */
//#define M_PROPFIND              8       /** RFC 2518: WebDAV */
//#define M_PROPPATCH             9       /*  :               */
//#define M_MKCOL                 10
//#define M_COPY                  11
//#define M_MOVE                  12
//#define M_LOCK                  13
//#define M_UNLOCK                14      /** RFC 2518: WebDAV */
//#define M_VERSION_CONTROL       15      /** RFC 3253: WebDAV Versioning */
//#define M_CHECKOUT              16      /*  :                          */
//#define M_UNCHECKOUT            17
//#define M_CHECKIN               18
//#define M_UPDATE                19
//#define M_LABEL                 20
//#define M_REPORT                21
//#define M_MKWORKSPACE           22
//#define M_MKACTIVITY            23
//#define M_BASELINE_CONTROL      24
//#define M_MERGE                 25
//#define M_INVALID               26      /** no valid method */

// This file is part of RaSt mod_ecl.
//
// RaSt mod_ecl is  an extension module for the Apache  HTTP Server.  It enables
// the Apache HTTP  Server to run scripts written in  the Embeddable Common-Lisp
// language.
//
// Copyright (C) 2021 Ralf Stephan (RaSt) <me@ralf-stephan.name>
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
// this     program     in     the     file    COPYING.      If     not,     see
// <https://www.gnu.org/licenses/>.



/**
 * @mainpage RaSt mod_ecl
 *
 * RaSt mod_ecl is  an extension module for the Apache  HTTP Server.  It enables
 * the Apache HTTP  Server to run scripts written in  the Embeddable Common-Lisp
 * language.
 *
 * Copyright (C) 2021 Ralf Stephan (RaSt)
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
 * this     program     in     the     file    COPYING.      If     not,     see
 * <[https://www.gnu.org/licenses/](https://www.gnu.org/licenses/)>
 *
 * @tableofcontents
 */



/**
 * @file
 *
 * @brief This file contains the core functionality of this module.
 *
 * @details
 */



// Header files from Apache HTTP Server.

#include "ap_config.h"
#include "ap_regex.h"
#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_log.h"
#include "util_varbuf.h"
#include "util_script.h"

// Header files from Apache Runtime Library.

#include "apr_file_info.h"
#include "apr_strings.h"
#include "apr_pools.h"
#include "apr_strmatch.h"
#include "apr_buckets.h"
#include "apr_errno.h"

// Header files from Embeddable Common-Lisp.

#include "ecl/ecl.h"

// Header files from RaSt mod_ecl.

#include "mod_ecl.h"



/**
 * @brief Number of byts to read at once to read from a file.
 */

#define MECL_READ_BYTES (4 * 1024)



/**
 * @brief Failure status codes.
 */

#define APR_FAILURE 1



/**
 * @brief Mark this file to belong to this module.
 *
 * @details This macro is  used choose which module a file  belongs to.  This is
 * necessary to allow per-module loglevel configuration.
 */

APLOG_USE_MODULE(ecl);



/**
 * @brief Search for string and replace string by replacemant string.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] original_string
 *   : the string to search in
 *
 * @param[in] search_string
 *   : the string to search for
 *
 * @param[in] replace_string
 *   : the replacemant string
 *
 * @param[in] case_sensitive
 *   : 0: case insensitive search / 1: case sensitive search
 *
 * @return result
 *   : no match: return the original string / with one or more match: return
 *   string with replacements
 */

static apr_status_t replace_string(request_rec * request, char * original_string, char * search_string, char * replace_string, int case_sensitive, char ** result)
{
    // The status code.

    apr_status_t status = APR_FAILURE;

    // Get length of strings.

    apr_size_t original_length = strlen(original_string);
    apr_size_t search_length = strlen(search_string);
    apr_size_t replace_length = strlen(replace_string);

    // Compile the pattern.

    const apr_strmatch_pattern * pattern = apr_strmatch_precompile(request->pool, search_string, case_sensitive);

    // Create the bucket allocator.

    apr_bucket_alloc_t * bucket_allocator = apr_bucket_alloc_create(request->pool);

    // Create the brigade.

    apr_bucket_brigade * brigade = apr_brigade_create(request->pool, bucket_allocator);

    // Fill brigade with first bucket.

    apr_bucket * first_bucket = apr_bucket_transient_create(original_string, original_length, bucket_allocator);
    APR_BRIGADE_INSERT_HEAD(brigade, first_bucket);

    // Search for the pattern in the string.

    const char * match = NULL;
    const char * match_string = original_string;
    apr_size_t match_length = original_length;
    const char * after_string = NULL;
    apr_size_t after_length = 0;
    const char * before_string = NULL;
    apr_size_t before_length = 0;
    int has_match = 0;
    apr_bucket * next_bucket = first_bucket;
    apr_bucket * replacement_bucket = NULL;
    apr_bucket * before_bucket = NULL;
    while (match = apr_strmatch(pattern, match_string, match_length))
    {
        // Do we have a match?

        has_match = 1;

        // Get the string after the match (with the string we search for).

        after_string = (char *) (match);
        after_length = strlen(after_string);

        // Get the string before the match (without the string we search for).

        before_length = match_length - after_length;
        before_string = (char *) apr_pstrmemdup(request->pool, match_string, before_length);

        // The next substring to search.

        match_string = (char * ) (match + search_length);
        match_length = strlen(match_string);

        // Split the bucket after the match.

        apr_bucket_split(next_bucket, (before_length + search_length));

        // Insert the replacment.

        replacement_bucket = apr_bucket_transient_create(replace_string, replace_length, bucket_allocator);
        APR_BUCKET_INSERT_AFTER(next_bucket, replacement_bucket);

        // Insert string before match.

        before_bucket = apr_bucket_transient_create(before_string, before_length, bucket_allocator);
        APR_BUCKET_INSERT_BEFORE(replacement_bucket, before_bucket);

        // Delete bucket.
        apr_bucket_delete(next_bucket);

        // Get the next bucket.
        next_bucket = APR_BRIGADE_LAST(brigade);
    }

    // If we have no match we return the original string.

    if (!has_match)
    {
      * result = original_string;
      status = APR_SUCCESS;
    }

    // Check if we have a bucket.

    apr_bucket * read_bucket = NULL;
    const char * read_buffer = NULL;
    apr_size_t read_bytes = 0;
    * result = apr_pstrcat(request->pool, "", NULL);
    if (APR_BRIGADE_EMPTY(brigade))
    {
        // We have no bucket.

        * result = original_string;
        status = APR_SUCCESS;
    }
    else
    {
        // We have a bucket.

        for (read_bucket = APR_BRIGADE_FIRST(brigade); read_bucket != APR_BRIGADE_SENTINEL(brigade); read_bucket = APR_BUCKET_NEXT(read_bucket))
        {
            if (APR_BUCKET_IS_METADATA(read_bucket))
            {
                continue;
            }
            if (apr_bucket_read(read_bucket, & read_buffer, & read_bytes, APR_BLOCK_READ) == APR_SUCCESS)
            {
              * result = apr_pstrcat(request->pool, * result, read_buffer, NULL);
            }
        }
        status = APR_SUCCESS;
    }

    // Return status code.

    return status;

}



/**
 * @brief Search for regular expression and replace string by replacemant
 *   string.
 *
 * @details
 *
 *   s/search_string/replace_string/cflags
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] original_string
 *   : the string to search in
 *
 * @param[in] search_string
 *   : the string to search for
 *
 * @param[in] replace_string
 *   : the replacemant string
 *
 * @param[in] cflags
 *   : flags for compiling the regular expression
 *
 *   AP_REG_ICASE          use a case-insensitive match                   ap_pregcomp
 *   AP_REG_NEWLINE        don't match newlines against '.' etc           ap_pregcomp
 *   AP_REG_NOTBOL         ^ will not match against start-of-string       ap_regexec_len
 *   AP_REG_NOTEOL         $ will not match against end-of-string         ap_regexec_len
 *   AP_REG_EXTENDED       unused                                         ap_pregcomp
 *   AP_REG_NOSUB          unused                                         ap_pregcomp
 *   AP_REG_MULTI          perl's /g
 *   AP_REG_NOMEM          nomem in our code
 *   AP_REG_DOTALL         perl's /s flag
 *   AP_REG_DOLLAR_ENDONLY $ matches at end of subject string only
 *   AP_REG_NO_DEFAULT     Don't implicitely add AP_REG_DEFAULT options
 *
 * @param[in] eflags
 *   : flags for compiling the regular expression
 *
 * @param[in] case_sensitive
 *   : 0: case insensitive search / 1: case sensitive search
 *
 * @return result
 *   : no match: return the original string / with one or more match: return
 *   string with replacements
 */

static apr_status_t replace_regex(request_rec * request, char * original_string, char * search_string, char * replace_string, int cflags, int eflags, char ** result)
{
    // The status code.

    apr_status_t status = APR_FAILURE;

    // Get length of strings.

    apr_size_t original_length = strlen(original_string);
    apr_size_t search_length = strlen(search_string);
    apr_size_t replace_length = strlen(replace_string);

    // Compile the pattern.
    ap_regex_t * regex = ap_pregcomp(request->pool, search_string, cflags);

    // Create the bucket allocator.

    apr_bucket_alloc_t * bucket_allocator = apr_bucket_alloc_create(request->pool);

    // Create the brigade.

    apr_bucket_brigade * brigade = apr_brigade_create(request->pool, bucket_allocator);

    // Fill brigade with first bucket.

    apr_bucket * first_bucket = apr_bucket_transient_create(original_string, original_length, bucket_allocator);
    APR_BRIGADE_INSERT_HEAD(brigade, first_bucket);

    // Search for the pattern in the string.

    char * match = NULL;
    apr_size_t length = 0;
    apr_size_t nmatch = AP_MAX_REG_MATCH;
    ap_regmatch_t pmatch[nmatch];
    const char * match_string = original_string;
    apr_size_t match_length = original_length;
    int has_match = 0;
    int maxlen = 0;
    apr_bucket * next_bucket = first_bucket;
    apr_bucket * last_bucket = NULL;
    char * before_string = NULL;
    apr_size_t before_length = 0;
    apr_bucket * before_bucket = NULL;
    apr_bucket * replacement_bucket = NULL;
    char * after_string = NULL;
    apr_size_t after_length = 0;
    apr_bucket * after_bucket = NULL;

    while (!ap_regexec_len(regex, match_string, match_length, nmatch, pmatch, eflags))
    {
        // Do we have a match?

        has_match = 1;

        // Perform a series of string substitution.

        status = ap_pregsub_ex(request->pool, & match, replace_string, match_string, nmatch, pmatch, maxlen);
        if (status != APR_SUCCESS)
        {
            return status;
        }
        length = strlen(match);

        // Get the string after the match (with the string we search for).

        after_string = (char *) (match_string + pmatch[0].rm_eo);
        after_length = strlen(after_string);

        // Get the string before the match (without the string we search for).

        before_string = (char *) apr_pstrmemdup(request->pool, match_string, pmatch[0].rm_so);
        before_length = strlen(before_string);

        // The next substring to search.

        match_string = (char *) (match_string + pmatch[0].rm_eo);
        match_length = strlen(match_string);

        // Split the bucket after the match.

        apr_bucket_split(next_bucket, (before_length + (pmatch[0].rm_eo - pmatch[0].rm_so)));

        // Insert the replacment.

        replacement_bucket = apr_bucket_transient_create(match, length, bucket_allocator);
        APR_BUCKET_INSERT_AFTER(next_bucket, replacement_bucket);

        // Insert string before match.

        before_bucket = apr_bucket_transient_create(before_string, before_length, bucket_allocator);
        APR_BUCKET_INSERT_BEFORE(replacement_bucket, before_bucket);

        // Delete bucket.

        apr_bucket_delete(next_bucket);

        // Get the next bucket.
        next_bucket = APR_BRIGADE_LAST(brigade);
    }

    // If we have no match we return the original string.

    if (!has_match)
    {
      * result = original_string;
      status = APR_SUCCESS;
    }

    // Check if we have a bucket.

    apr_bucket * read_bucket = NULL;
    const char * read_buffer = NULL;
    apr_size_t read_bytes = 0;
    * result = apr_pstrcat(request->pool, "", NULL);
    if (APR_BRIGADE_EMPTY(brigade))
    {
        // We have no bucket.

        * result = original_string;
        status = APR_SUCCESS;
    }
    else
    {
        // We have a bucket.

        for (read_bucket = APR_BRIGADE_FIRST(brigade); read_bucket != APR_BRIGADE_SENTINEL(brigade); read_bucket = APR_BUCKET_NEXT(read_bucket))
        {
            if (APR_BUCKET_IS_METADATA(read_bucket))
            {
                continue;
            }
            if (apr_bucket_read(read_bucket, & read_buffer, & read_bytes, APR_BLOCK_READ) == APR_SUCCESS)
            {
              * result = apr_pstrcat(request->pool, * result, "(", read_buffer, ")", NULL);
            }
        }
        status = APR_SUCCESS;
    }

    // Return status code.

    return status;
}



/**
 * @brief Replace certain characters by there HTML character encoding.
 *
 * @details
 *
 *   &  &amp;    &#38;
 *   <  &lt;     &#60;
 *   >  &gt;     &#62;
 *   "  &quote;  &#34;
 *   '  &apos;   &#39;
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] string_unescaped
 *   : the string in whitch we make the replacement
 *
 * @retunr string_escaped
 *   : the string with the characters replaced
 */

char * escape_html(request_rec * request, char * string_unescaped)
{
    int case_sensitive = 0;
    char * string_escaped = string_unescaped;

    replace_string(request, string_escaped, "&", "&#38;", case_sensitive, & string_escaped);
    replace_string(request, string_escaped, "<", "&#60;", case_sensitive, & string_escaped);
    replace_string(request, string_escaped, ">", "&#62;", case_sensitive, & string_escaped);
    replace_string(request, string_escaped, "\"", "&#34;", case_sensitive, & string_escaped);
    replace_string(request, string_escaped, "'", "&#39;", case_sensitive, & string_escaped);

    return string_escaped;
}



/**
 * @brief Get the content-type for the current request.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in,out] content_type
 *   : the content type
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t getContentType(request_rec * request, char ** content_type)
{
    apr_status_t status = APR_FAILURE;

    * content_type = NULL;
    if (request)
    {
        * content_type = apr_pstrdup(request->pool, request->content_type);
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief Set the content-type for the current request.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] content_type
 *   : the content type
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t setContentType(request_rec * request, char * content_type)
{
    apr_status_t status = APR_FAILURE;

    if (request)
    {
        request->content_type = content_type;
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief The filename on disk corresponding to this response.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in,out] filename
 *   : the filename
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t getFilename(request_rec * request, char ** filename)
{
    apr_status_t status = APR_FAILURE;

    * filename = NULL;
    if (request)
    {
        * filename = apr_pstrdup(request->pool, request->filename);
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief The true filename stored in the  filesystem, as in the true alpha case
 *   and alias correction, e.g. "Image.jpeg"  not "IMAGE$1.JPE" on Windows.  The
 *   core map_to_storage canonicalizes r->filename when they mismatch.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in,out] canonical_filename
 *   : the canonical filename
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t getCanonicalFilename(request_rec * request, char ** canonical_filename)
{
    apr_status_t status = APR_FAILURE;

    * canonical_filename = NULL;
    if (request)
    {
        * canonical_filename = apr_pstrdup(request->pool, request->canonical_filename);
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief MIME header environment from the request.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in,out] headers_in
 *   : the header
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t getHeadersIn(request_rec * request, apr_table_t ** headers_in)
{
    apr_status_t status = APR_FAILURE;

    * headers_in = NULL;
    if (request)
    {
        * headers_in = request->headers_in;
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief MIME header environment from the response.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in,out] headers_out
 *   : the header
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t getHeadersOut(request_rec * request, apr_table_t ** headers_out)
{
    apr_status_t status = APR_FAILURE;

    * headers_out = NULL;
    if (request)
    {
        * headers_out = request->headers_out;
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief Get the HTTP verb name.
 *
 * @details
 *
 *     GET
 *     PUT
 *     POST
 *     DELETE
 *     CONNECT
 *     OPTIONS
 *     TRACE
 *     PATCH
 *     PROPFIND
 *     PROPPATCH
 *     MKCOL
 *     COPY
 *     MOVE
 *     LOCK
 *     UNLOCK
 *     VERSION_CONTROL
 *     CHECKOUT
 *     UNCHECKOUT
 *     CHECKIN
 *     UPDATE
 *     LABEL
 *     REPORT
 *     MKWORKSPACE
 *     MKACTIVITY
 *     BASELINE_CONTROL
 *     MERGE
 *     INVALID
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in,out] method
 *   : request method (eg. GET, POST, etc.)
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t getMethod(request_rec * request, const char ** method)
{
    apr_status_t status = APR_FAILURE;

    * method = NULL;
    if (request)
    {
        * method = request->method;
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief Get the HTTP verb number.
 *
 * @details
 *
 *     M_GET
 *     M_PUT
 *     M_POST
 *     M_DELETE
 *     M_CONNECT
 *     M_OPTIONS
 *     M_TRACE
 *     M_PATCH
 *     M_PROPFIND
 *     M_PROPPATCH
 *     M_MKCOL
 *     M_COPY
 *     M_MOVE
 *     M_LOCK
 *     M_UNLOCK
 *     M_VERSION_CONTROL
 *     M_CHECKOUT
 *     M_UNCHECKOUT
 *     M_CHECKIN
 *     M_UPDATE
 *     M_LABEL
 *     M_REPORT
 *     M_MKWORKSPACE
 *     M_MKACTIVITY
 *     M_BASELINE_CONTROL
 *     M_MERGE
 *     M_INVALID
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in,out] method_number
 *   : request number (eg. M_GET, M_POST, etc.)
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t getMethodNumber(request_rec * request, int * method_number)
{
    apr_status_t status = APR_FAILURE;

    * method_number = M_INVALID;
    if (request)
    {
      * method_number = request->method_number;
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief Get request data from GET request.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in,out] get_data
 *   : the request data from GET request
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t getGetData(request_rec * request, apr_table_t ** get_data)
{
    apr_status_t status = APR_FAILURE;

    * get_data = NULL;
    if (request)
    {
        ap_args_to_table(request, & * get_data);
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief Get request data from POST request.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in,out] post_data
 *   : the request data from POST request
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t getPostData(request_rec * request, apr_array_header_t ** post_data)
{
    apr_status_t status = APR_FAILURE;

    * post_data = NULL;
    if (request)
    {
        status = ap_parse_form_data(request, NULL, post_data, -1, HUGE_STRING_LEN);
        status = APR_SUCCESS;
    }

    return status;
}



/**
 * @brief Build lisp code to access the MIME header environment from the
 *   request.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] headers_in
 *   : MIME header environment from the request
 *
 * @return string
 *   lisp code to access the request data
 */

char * printHeadersIn(request_rec * request, apr_table_t * headers_in)
{
    char * string =  apr_pstrcat(request->pool, "", NULL);
    const apr_array_header_t * fields = NULL;
    apr_table_entry_t * elements = NULL;
    int index = 0;
    int first = 1;
    char * result = apr_pstrcat(request->pool, "", NULL);

    // Get the elements from the table.

    fields = apr_table_elts(headers_in);

    // The elements in the array.

    elements = (apr_table_entry_t *) fields->elts;

    // We build a lisp lisp code.

    string = apr_pstrcat(request->pool, string, "(eval-when-compile\n", NULL);
    string = apr_pstrcat(request->pool, string, "(defparameter *_header-in_* (make-hash-table :test 'equal))\n", NULL);

    // Check if we have elements in the array.

    if (0 != fields->nelts)
    {
        // We have elementws in the array. So we build the string.

        // For each element get the key and the value.

        for(index = 0; index < fields->nelts; index++)
        {
            replace_string(request, elements[index].val, "\"", "\\\"", 0, & result);
            string = apr_pstrcat(request->pool, string, "(setf (gethash \"", elements[index].key, "\" *_header-in_*) \"", result , "\")\n", NULL);
        }
    }

    // We build a lisp lisp code.

    string = apr_pstrcat(request->pool, string, ")\n", NULL);

  return string;
}



/**
 * @brief Build lisp code to access the MIME header environment from the
 *   response.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] headers_out
 *   : MIME header environment from the response
 *
 * @return string
 *   lisp code to access the request data
 */

char * printHeadersOut(request_rec * request, apr_table_t * headers_out)
{
    char * string = apr_pstrcat(request->pool, "", NULL);
    const apr_array_header_t * fields = NULL;
    apr_table_entry_t * elements = NULL;
    int index = 0;
    int first = 1;
    char * result = apr_pstrcat(request->pool, "", NULL);

    // Get the elements from the table.

    fields = apr_table_elts(headers_out);

    // The elements in the array.

    elements = (apr_table_entry_t *) fields->elts;

    // We build a lisp lisp code.

    string = apr_pstrcat(request->pool, string, "(eval-when-compile\n", NULL);
    string = apr_pstrcat(request->pool, string, "(defparameter *_header-out_* (make-hash-table :test 'equal))\n", NULL);

    // Check if we have elements in the array.

    if (0 != fields->nelts)
    {
        // We have elementws in the array. So we build the string.

        // For each element get the key and the value.

        for(index = 0; index < fields->nelts; index++)
        {
            replace_string(request, elements[index].val, "\"", "\\\"", 0, & result);
            string = apr_pstrcat(request->pool, string, "(setf (gethash \"", elements[index].key, "\" *_header-out_*) \"", result , "\")\n", NULL);
        }
    }

    // We build a lisp lisp code.

    string = apr_pstrcat(request->pool, string, ")\n", NULL);

  return string;
}



/**
 * @brief Build lisp code to access the request data from the GET request.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] get_data
 *   : the request data from GET request
 *
 * @return string
 *   lisp code to access the request data from GET request
 */

char * printGetData(request_rec * request, apr_table_t * get_data)
{
    char * string = apr_pstrcat(request->pool, "", NULL);
    const apr_array_header_t * fields = NULL;
    apr_table_entry_t * elements = NULL;
    int index = 0;
    int first = 1;
    char * result = apr_pstrcat(request->pool, "", NULL);

    // Get the elements from the table.

    fields = apr_table_elts(get_data);

    // The elements in the array.

    elements = (apr_table_entry_t *) fields->elts;

    // We build a lisp lisp code.

    string = apr_pstrcat(request->pool, string, "(eval-when-compile\n", NULL);
    string = apr_pstrcat(request->pool, string, "(defparameter *_get_* (make-hash-table :test 'equal))\n", NULL);

    // Check if we have elements in the array.

    if (0 != fields->nelts)
    {
        // We have elementws in the array. So we build the string.

        // For each element get the key and the value.

        for(index = 0; index < fields->nelts; index++)
        {
            replace_string(request, elements[index].val, "\"", "\\\"", 0, & result);
            string = apr_pstrcat(request->pool, string, "(setf (gethash \"", elements[index].key, "\" *_get_*) \"", result , "\")\n", NULL);
        }
    }

    // We build a lisp lisp code.

    string = apr_pstrcat(request->pool, string, ")\n", NULL);

  return string;
}



/**
 * @brief Build lisp code to access the request data from the POST request.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] get_data
 *   : the request data from GET request
 *
 * @return string
 *   lisp code to access the request data from GET request
 */

char * printPostData(request_rec * request, apr_array_header_t * post_data)
{
    char * string = apr_pstrcat(request->pool, "", NULL);
    apr_off_t value_length = 0;
    apr_size_t size = 0;
    int res = 0;
    int i = 0;
    const char * key = NULL;
    char * value = NULL;
    ap_form_pair_t * key_value = NULL;
    char * result = apr_pstrcat(request->pool, "", NULL);

    // We build a lisp lisp code.

    string = apr_pstrcat(request->pool, string, "(eval-when-compile\n", NULL);
    string = apr_pstrcat(request->pool, string, "(defparameter *_post_* (make-hash-table :test 'equal))\n", NULL);

    // Check if we have elements in the array.

    if (0 != post_data->nelts)
    {
        // We have elementws in the array. So we build the string.

        // For each element get the key and the value.

        while (post_data && !apr_is_empty_array(post_data))
        {
            // Get key and value.

            key_value = (ap_form_pair_t *) apr_array_pop(post_data);

            // Get key.

            key = key_value->name;

            // Get value.

            apr_brigade_length(key_value->value, 1, & value_length);
            size = (apr_size_t) value_length + 1;
            value = apr_palloc(request->pool, size);
            apr_brigade_flatten(key_value->value, value, & size);
            value[value_length] = 0;

            // Add to hash.

            replace_string(request, value, "\"", "\\\"", 0, & result);
            string = apr_pstrcat(request->pool, string, "(setf (gethash \"", key, "\" *_post_*) \"", result, "\")\n", NULL);
        }
    }

    // We build a lisp lisp code.

    string = apr_pstrcat(request->pool, string, ")\n", NULL);

    return string;
}



/**
 * @brief Build lisp code to access the HTTP request verb name.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] method
 *   : the request method (eg. GET, POST, etc.)
 *
 * @return string
 *  : lisp code to access the HTTP request verb name
 */

char * printMethodName(request_rec * request, const char * method)
{
    char * string = apr_pstrcat(request->pool, "", NULL);

    string = apr_pstrcat(request->pool, string, "(defparameter *_http-method-name_* \"", method, "\")", NULL);

    return string;
}



/**
 * @brief Build lisp code to access the HTTP request verb number.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] method_number
 *   : the request method (eg. M_GET, M_POST, etc.)
 *
 * @return string
 *  : lisp code to access the HTTP request verb number
 */

char * printMethodNumber(request_rec * request, int method_number)
{
    char * string = apr_pstrcat(request->pool, "", NULL);

    string = apr_pstrcat(request->pool, string, "(defparameter *_http-method-number_* ", apr_psprintf(request->pool, "%u", method_number), ")", NULL);

    return string;
}



/**
 * @brief Function to get the file content from the request data.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] filename
 *   : the file name
 *
 * @param[in,out] filecontent
 *   : the file content
 *
 * @return status
 *  : on failure: HTTP_NOT_FOUND or HTTP_FORBIDDEN / on success: APR_SUCCESS
 */

static apr_status_t getFilecontent(request_rec * request, char * filename, char ** filecontent)
{
    apr_status_t status = APR_FAILURE;
    apr_finfo_t fileinfo = {};
    apr_file_t * file = NULL;
    apr_size_t buffersize = MECL_READ_BYTES;
    apr_size_t bytes_read = MECL_READ_BYTES;
    apr_pool_t * buffer_pool = NULL;
    char * buffer = NULL;

    // Initialize the filecontent.

    * filecontent = apr_pstrdup(request->pool, "");

    // Get the filestats.

    status = apr_stat(& fileinfo, filename, APR_FINFO_MIN, request->pool);

    // Check the status.

    if (APR_SUCCESS != status)
    {
        // Getting the file stats has not been successfully.

        return status;
    }

    // Getting the file stats has been successfully.

    // Check the file stats.

    if (fileinfo.filetype == APR_NOFILE)
    {
        // Script is not found or unable to stat.

        return HTTP_NOT_FOUND;
    }
    if (fileinfo.filetype == APR_DIR)
    {
        // Attempt to invoke a directory as script.

        return HTTP_FORBIDDEN;
    }

    // Open the file.

    status = apr_file_open(& file, filename, APR_READ | APR_FOPEN_BUFFERED, APR_OS_DEFAULT, request->pool);

    // Check the status.

    if (APR_SUCCESS != status)
    {
        // OPening the file has not been successfully.

        return status;
    }

    // Opening the file has been successfully.

    // Lock the file.

    status = apr_file_lock(file, APR_FLOCK_SHARED | APR_FLOCK_NONBLOCK);

    // Check the status.

    if (APR_SUCCESS != status)
    {
        // Locking the file has not been successfully.

        return status;
    }

    // Locking the file has been successfully.

    // Create new pool.

    status = apr_pool_create(& buffer_pool, NULL);

    // Check the status.

    if (APR_SUCCESS != status)
    {
        // Creating the pool has not been successfully.

        return status;
    }

    // Creating the pool has been successfully.

    // Read the file content.

    while (   (APR_SUCCESS == status)
           && (APR_EOF != status))
    {
        // Allocate empty buffer.

        buffer = (char *) apr_pcalloc(buffer_pool, buffersize + 1);

        // Check the buffer.

        if (NULL == buffer)
        {
            // Allocating the buffer has not been successfully.

            return APR_FAILURE;
        }

        // Allocating the buffer has been successfully.

        // Read bytes into the buffer.
        //
        // After reading read_bytes  contains the number of byte to  read in the
        // next loop iteration.

        status = apr_file_read_full(file, buffer, buffersize, & bytes_read);

        // Check the status.

        if ((APR_SUCCESS != status) && (APR_EOF != status))
        {
            // Reading the filecontent has not been successfully.

            return status;
        }

        // Reading the filecontent has been successfully.

        // Append the buffer to the file content.

        * filecontent = apr_pstrcat(request->pool, * filecontent, buffer, NULL);

        // Ajust the buffer size;

        buffersize = bytes_read;

        // Clear the pool.

        apr_pool_clear(buffer_pool);
    }

    // Destroy the buffer.

    apr_pool_destroy(buffer_pool);

    // Close the file.

    status = apr_file_close(file);

    // Check the state.

    if (APR_SUCCESS != status)
    {
        // Closing the file has not been successfully.

        return status;
    }

    // Closing the file has been successfully.

    // If we reach this line we assume everything worked fina.

    return status;
}



/**
 * @brief Evaluate the script.
 *
 * @details
 *
 * @param[in] request
 *   : the request data
 *
 * @param[in] script
 *   : the script
 *
 * @param[in,out] result
 *   : the result of the evaluation of the script
 *
 * @return status
 *   : on failure: APR_FAILURE / on success: APR_SUCCESS
 */

static apr_status_t evaluateByEcl(request_rec * request, char * script, char ** result)
{
    apr_status_t status = APR_FAILURE;
    unsigned int argc = 0;
    char * argv[0] = {};
    cl_object string = ECL_NIL;
    signed int const length = -1;
    unsigned int const start = 0;
    unsigned int end = 0;
    cl_object stream = ECL_NIL;
    cl_object form = ECL_NIL;
    cl_object const eof_error = ECL_NIL;
    cl_object const eof_value = OBJNULL;
    cl_object eval = ECL_NIL;
    cl_object lexical_environment = ECL_NIL;
    cl_env_ptr process_environment = NULL;
    cl_object const error = OBJNULL;
    unsigned int dim = 0;
    cl_index index = 0;
    char * character = apr_pstrdup(request->pool, " \0");
    apr_table_t * headers_in = NULL;
    apr_table_t * headers_out = NULL;
    const char * method_name = NULL;
    int method_number = M_INVALID;
    apr_table_t * get_data = NULL;
    apr_array_header_t * post_data = NULL;
    char * content_type = NULL;

    // Get the method number.

    status = getMethodNumber(request, & method_number);

    // Setup the lisp environment.

    cl_boot(argc, argv);

    // Create a base string.

    string = ecl_make_simple_base_string(script, length);

    // Make a input string stream.

    end = ecl_length(string);
    stream = ecl_make_string_input_stream(string, start, end);

    // Get the process environment

    process_environment = ecl_process_env();

    // Create a protected region.

    ECL_CATCH_ALL_BEGIN(process_environment)
    {
        // Add a lisp constant so we can identify if we run as embedded ECL.

        eval = si_safe_eval(3, ecl_read_from_cstring("(eval-when-compile (defconstant *mod_ecl* \"mod_ecl\"))"), lexical_environment, error);

        // Ass a lisp variable so we can set the content type from the script.

        status = getContentType(request, & content_type);
        eval = si_safe_eval(
            3,
            ecl_read_from_cstring(
                apr_pstrcat(
                    request->pool,
                    "(eval-when-compile (defparameter +_content_type_+ \"",
                    content_type,
                    "\"))",
                    NULL
                )
            ),
            lexical_environment,
            error
        );

        // Add a hash table for MIME header environment from the request.

        status = getHeadersIn(request, & headers_in);
        eval = si_safe_eval(3, ecl_read_from_cstring(printHeadersIn(request, headers_in)), lexical_environment, error);

        // Add a hash table for MIME header environment from the request.

        status = getHeadersOut(request, & headers_out);
        eval = si_safe_eval(3, ecl_read_from_cstring(printHeadersOut(request, headers_out)), lexical_environment, error);

        // Add a string for the HTTP method name.

        status = getMethod(request, & method_name);
        eval = si_safe_eval(3, ecl_read_from_cstring(printMethodName(request, method_name)), lexical_environment, error);

        // Add a string for the HTTP method number.

        status = getMethodNumber(request, & method_number);
        eval = si_safe_eval(3, ecl_read_from_cstring(printMethodNumber(request, method_number)), lexical_environment, error);

        // Add s string for the request data from the GET request.

        if (method_number == M_GET)
        {
            status = getGetData(request, & get_data);
            eval = si_safe_eval(3, ecl_read_from_cstring(printGetData(request, get_data)), lexical_environment, error);
        }
        else
        {
            eval = si_safe_eval(3, ecl_read_from_cstring("(eval-when-compile (defparameter *_get_* (make-hash-table :test 'equal)))\n"), lexical_environment, error);
        }

        // Add s string for the request data from the POST request.

        if (method_number == M_POST)
        {
            status = getPostData(request, & post_data);
            eval = si_safe_eval(3, ecl_read_from_cstring(printPostData(request, post_data)), lexical_environment, error);
        }
        else
        {
            eval = si_safe_eval(3, ecl_read_from_cstring("(eval-when-compile (defparameter *_post_* (make-hash-table :test 'equal)))\n"), lexical_environment, error);
        }

        // The protected code.

        // Evaluate the script.

        do
        {
            // Parse the printed  representation of an object  from input-stream and
            // builds such an object.

            form = cl_read(3, stream, eof_error, eof_value);

            // Check form to see if we are done.

            if (form == OBJNULL)
            {
                // We reached the end of the script.

                status = APR_SUCCESS;
                break;
            }

            // Evaluate form in the lexical environment.

            eval = si_safe_eval(3, form, lexical_environment, error);

            // Check the eval.

            if (   (eval != ECL_NIL)
                && (eval != OBJNULL))
            {
                // The evaluation was successful.

                // Get length of resulting string.

                dim = eval->string.dim;

                // Get result by each single character and join them to a string.

                for (index = 0; index < dim; index++)
                {
                    character[0] = ecl_char(eval, index);
                    * result = apr_pstrcat(request->pool, * result, & character[0], NULL);
                }
            }

        } while (1);

        // Get and set the content type

        eval = si_safe_eval(3, ecl_read_from_cstring("(princ +_content_type_+)"), lexical_environment, error);
        if (   (eval != ECL_NIL)
            && (eval != OBJNULL))
        {
            // The evaluation was successful.

            // Get length of resulting string.

            dim = eval->string.dim;

            // Get result by each single character and join them to a string.

            content_type = "";
            for (index = 0; index < dim; index++)
            {
                character[0] = ecl_char(eval, index);
                content_type = apr_pstrcat(request->pool, content_type, & character[0], NULL);
            }
        }

    }
    ECL_CATCH_ALL_IF_CAUGHT
    {
        // If something is caught this code is is caught.

        // Write error massage to error log.

        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, request, "%s: %s", "mod_ecl", "Something happened. There is something wront with the ECL scirpt.");
    }
    ECL_CATCH_ALL_END;

    // In all cases we exit the ECL_CATCH_ALL here.

    // Close the lisp environment.

    cl_shutdown();

    // Set the content type.

    ap_set_content_type(request, content_type);

    // If we reach this line we assume everything worked fina.

    return status;
}



/**
 * @brief Function to handle the request.
 *
 * @details Return  a general response Non-HTTP  status code or a  HTTP specific
 * return code.
 *
 * General Non-HTTP status codes:
 *
 * * \c OK
 *
 *   Module has handled this stage.
 *
 * * \c DECLINED
 *
 *   Module declines to handle.
 *
 * * \c DONE
 *
 *   Module has served  the response completely.  It's safe to  \c die() with no
 *   more output.
 *
 * * \c SUSPENDED
 *
 *   Module  will handle  the remainder  of the  request.  The  core will  never
 *   invoke the request again.
 *
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/httpd_8h.html
 * @see httpd-2.4.25/include/httpd.h
 *
 * HTTP status codes:
 *
 * * 1xx informational response
 *
 *   &rarr; The request was received, continuing process.
 *
 *   An  informational response  indicates  that the  request  was received  and
 *   understood.  It is  issued on a provisional basis  while request processing
 *   continues.  It alerts the client to wait for a final response.  The message
 *   consists  only of  the  status  line and  optional  header  fields, and  is
 *   terminated by an  empty line.  As the HTTP/1.0 standard  did not define any
 *   1xx  status codes,  servers must  not send  a 1xx  response to  an HTTP/1.0
 *   compliant client except under experimental conditions.
 *
 *   * \c HTTP_CONTINUE
 *
 *     \c 100 \c Continue
 *
 *     The server has received the request headers and the client should proceed
 *     to send the request body (in the case of a request for which a body needs
 *     to be  sent; for example,  a \c POST  request).  Sending a  large request
 *     body to  a server  after a  request has  been rejected  for inappropriate
 *     headers  would be  inefficient.  To  have  a server  check the  request's
 *     headers, a client must send \c Expect: \c 100-continue as a header in its
 *     initial request and receive a \c  100 \c Continue status code in response
 *     before sending  the body.  If the  client receives an error  code such as
 *     403 (Forbidden) or  405 (Method Not Allowed) then it  should not send the
 *     request's body.  The  response \c 417 \c Expectation  \c Failed indicates
 *     that the  request should be repeated  without the \c Expect  header as it
 *     indicates  that the  server does  not support  expectations (this  is the
 *     case, for example, of HTTP/1.0 servers).
 *
 *   * \c HTTP_SWITCHING_PROTOCOLS
 *
 *     \c 101 \c Switching Protocols
 *
 *     The requester has asked the server to switch protocols and the server has
 *     agreed to do so.
 *
 *   * \c HTTP_PROCESSING
 *
 *     \c 102 \c Processing
 *
 *     A WebDAV request may contain many sub-requests involving file operations,
 *     requiring a long time to complete  the request.  This code indicates that
 *     the server has received and is processing the request, but no response is
 *     available yet.  This prevents the client from timing out and assuming the
 *     request was lost.
 *
 *   * \c HTTP_EARLY_HINTS
 *
 *     \c 103 \c Early Hints
 *
 *     Used to return some response headers before final HTTP message.
 *
 * * 2xx success
 *
 *   &rarr; The request was successfully received, understood, and accepted.
 *
 *   This class of status codes indicates the action requested by the client was
 *   received, understood, and accepted.
 *
 *   * \c HTTP_OK
 *
 *     \c 200 \c OK
 *
 *     Standard response for successful HTTP requests.  The actual response will
 *     depend on  the request method  used.  In a  \c GET request,  the response
 *     will contain an entity corresponding to  the requested resource.  In a \c
 *     POST  request,  the  response  will   contain  an  entity  describing  or
 *     containing the result of the action.
 *
 *   * \c HTTP_CREATED
 *
 *     \c 201 \c Created
 *
 *     The  request has  been  fulfilled, resulting  in the  creation  of a  new
 *     resource.
 *
 *   * \c HTTP_ACCEPTED
 *
 *     \c 202 \c Accepted
 *
 *     The request has been accepted for  processing, but the processing has not
 *     been completed. The request might or  might not be eventually acted upon,
 *     and may be disallowed when processing occurs.
 *
 *   * \c HTTP_NON_AUTHORITATIVE
 *
 *     \c 203 \c Non-Authoritative \c Information
 *
 *     The server is a transforming proxy (e.g. a Web accelerator) that received
 *     a \c 200 \c OK from its origin, but is returning a modified version of
 *     the origin's response.
 *
 *   * \c HTTP_NO_CONTENT
 *
 *     \c 204 \c No \c Content
 *
 *     The server successfully  processed the request, and is  not returning any
 *     content.
 *
 *   * \c HTTP_RESET_CONTENT
 *
 *     \c 205 \c Reset \c Content
 *
 *     The server  successfully processed the  request, asks that  the requester
 *     reset its document view, and is not returning any content.
 *
 *   * \c HTTP_PARTIAL_CONTENT
 *
 *     \c 206 \c Partial \c Content
 *
 *     The server is delivering only part  of the resource (byte serving) due to
 *     a range  header sent  by the client.   The range header  is used  by HTTP
 *     clients to enable resuming of  interrupted downloads, or split a download
 *     into multiple simultaneous streams.
 *
 *   * \c HTTP_MULTI_STATUS
 *
 *     \c 207 \c Multi-Status
 *
 *     The  message body  that follows  is  by default  an XML  message and  can
 *     contain  a number  of  separate  response codes,  depending  on how  many
 *     sub-requests were made.
 *
 *   * \c HTTP_ALREADY_REPORTED
 *
 *     \c 208 \c Already \c Reported
 *
 *     The members of a DAV binding  have already been enumerated in a preceding
 *     part of the (multistatus) response, and are not being included again.
 *
 *   * \c HTTP_IM_USED
 *
 *     \c 226 \c IM \c Used
 *
 *     The server has fulfilled a request  for the resource, and the response is
 *     a  representation of  the result  of one  or more  instance-manipulations
 *     applied to the current instance.
 *
 * * 3xx redirection
 *
 *   &rarr; Further action needs to be taken in order to complete the request.
 *
 *   This class of status code indicates  the client must take additional action
 *   to  complete the  request.  Many  of  these status  codes are  used in  URL
 *   redirection.
 *
 *   A user agent  may carry out the additional action  with no user interaction
 *   only if the method used in the second request is GET or HEAD.  A user agent
 *   may  automatically redirect  a request.   A  user agent  should detect  and
 *   intervene to prevent cyclical redirects.
 *
 *   * \c HTTP_MULTIPLE_CHOICES
 *
 *     \c 300 \c Multiple \c Choices
 *
 *     Indicates multiple  options for  the resource from  which the  client may
 *     choose (via  agent-driven content  negotiation).  For example,  this code
 *     could be  used to present  multiple video  format options, to  list files
 *     with   different   filename   extensions,  or   to   suggest   word-sense
 *     disambiguation.
 *
 *   * \c HTTP_MOVED_PERMANENTLY
 *
 *     \c 301 \c Moved \c Permanently
 *
 *     This and all future requests should be directed to the given URI.
 *
 *   * \c HTTP_MOVED_TEMPORARILY
 *
 *     \c 302 \c Moved \c Temporarily (previous definition)
 *
 *     An HTTP response with this status code will additionally provide a URL in
 *     the  header field  Location.  This  is an  invitation to  the user  agent
 *     (e.g. a  web browser) to make  a second, otherwise identical,  request to
 *     the  new URL  specified  in the  location  field.  The  end  result is  a
 *     redirection to the new URL.
 *
 *     \c 302 \c Found
 *
 *     Tells the client to look at (browse to) another URL.  \c 302 \c Found has
 *     been superseded  by \c 303  \c See  \c Other and  \c 307 \c  Temporary \c
 *     Redirect.   This is  an example  of industry  practice contradicting  the
 *     standard.  The HTTP/1.0  specification (RFC 1945) required  the client to
 *     perform a temporary  redirect (the original describing  phrase was "Moved
 *     Temporarily"), but popular browsers implemented  \c 302 \c Found with the
 *     functionality of  a \c 303  \c See  \c Other.  Therefore,  HTTP/1.1 added
 *     status codes \c 303  \c See \c Other and \c 307  \c Temporary \c Redirect
 *     to   distinguish  between   the  two   behaviours.   However,   some  Web
 *     applications and frameworks use the \c 302  \c Found status code as if it
 *     were the \c 303 \c See \c Other.
 *
 *   * \c HTTP_SEE_OTHER
 *
 *     \c 303 \c See \c Other
 *
 *     The response to the  request can be found under another  URI using the \c
 *     GET method.  When  received in response to a \c  POST (or \c PUT/DELETE),
 *     the  client should  presume that  the server  has received  the data  and
 *     should issue a new \c GET request to the given URI.
 *
 *   * \c HTTP_NOT_MODIFIED
 *
 *     \c 304 \c Not \c Modified
 *
 *     Indicates  that the  resource has  not  been modified  since the  version
 *     specified by the request  headers If-Modified-Since or If-None-Match.  In
 *     such case, there  is no need to retransmit the  resource since the client
 *     still has a previously-downloaded copy.
 *
 *   * \c HTTP_USE_PROXY
 *
 *     \c 305 \c Use \c Proxy
 *
 *     The requested resource is available only through a proxy, the address for
 *     which  is provided  in the  response.   For security  reasons, many  HTTP
 *     clients (such as Mozilla Firefox and  Internet Explorer) do not obey this
 *     status code.
 *
 *   * \c HTTP_SWITCH_PROXY
 *
 *     \c 306 \c Switch \c Proxy
 *
 *     No longer  used.  Originally  meant "Subsequent  requests should  use the
 *     specified proxy."
 *
 *   * \c HTTP_TEMPORARY_REDIRECT
 *
 *     \c 307 \c Temporary \c Redirect
 *
 *     In this case,  the request should be repeated with  another URI; however,
 *     future requests should still use the original URI.  In contrast to how \c
 *     302 was historically implemented, the request method is not allowed to be
 *     changed  when reissuing  the original  request.  For  example, a  \c POST
 *     request should be repeated using another \c POST request.
 *
 *   * \c HTTP_PERMANENT_REDIRECT
 *
 *     \c 308 \c Permanent \c Redirect
 *
 *     The request and all future requests should be repeated using another URI.
 *     \c  307 \c  Temporary \c  Redirect and  \c 308  \c Permanent  \c Redirect
 *     parallel the behaviors  of \c 302 \c  Moved \c Temporarily and  \c 301 \c
 *     Moved \c  Permanently, but do not  allow the HTTP method  to change.  So,
 *     for example, submitting  a form to a permanently  redirected resource may
 *     continue smoothly.
 *
 * * 4xx client errors
 *
 *   &rarr; The request contains bad syntax or cannot be fulfilled.
 *
 *   This class  of status code  is intended for  situations in which  the error
 *   seems to have been caused by the  client.  Except when responding to a HEAD
 *   request, the server  should include an entity containing  an explanation of
 *   the error situation, and whether it  is a temporary or permanent condition.
 *   These  status codes  are applicable  to  any request  method.  User  agents
 *   should display any included entity to the user.
 *
 *   * \c HTTP_BAD_REQUEST
 *
 *     \c 400 \c Bad \c Request
 *
 *     The server  cannot or  will not  process the request  due to  an apparent
 *     client error  (e.g., malformed  request syntax,  size too  large, invalid
 *     request message framing, or deceptive request routing).
 *
 *   * \c HTTP_UNAUTHORIZED
 *
 *     \c 401 \c Unauthorized
 *
 *     Similar  to  \c   403  \c  Forbidden,  but  specifically   for  use  when
 *     authentication is required  and has failed or has not  yet been provided.
 *     The response  must include a  WWW-Authenticate header field  containing a
 *     challenge  applicable  to  the  requested  resource.   See  Basic  access
 *     authentication and  Digest access authentication. \c  401 \c Unauthorized
 *     semantically  means   "unauthorised",  the  user  does   not  have  valid
 *     authentication  credentials for  the target  resource.  Note:  Some sites
 *     incorrectly issue  \c 401 \c  Unauthorized when  an IP address  is banned
 *     from the website  (usually the website domain) and  that specific address
 *     is refused permission to access a website.
 *
 *   * \c HTTP_PAYMENT_REQUIRED
 *
 *     \c 402 \c Payment \c Required
 *
 *     Reserved for future use.  The original intention was that this code might
 *     be used as part  of some form of digital cash  or micropayment scheme, as
 *     proposed, for example,  by GNU Taler, but that has  not yet happened, and
 *     this code is not widely used.   Google Developers API uses this status if
 *     a particular developer has exceeded the daily limit on requests.  Sipgate
 *     uses this code  if an account does  not have sufficient funds  to start a
 *     call.  Shopify uses this code when the  store has not paid their fees and
 *     is temporarily disabled.  Stripe uses this code for failed payments where
 *     parameters were correct, for example blocked fraudulent payments.
 *
 *   * \c HTTP_FORBIDDEN
 *
 *     \c 403 \c Forbidden
 *
 *     The request  contained valid data and  was understood by the  server, but
 *     the server is  refusing action.  This may  be due to the  user not having
 *     the necessary  permissions for a resource  or needing an account  of some
 *     sort, or attempting a prohibited action (e.g. creating a duplicate record
 *     where only  one is  allowed).  This  code is also  typically used  if the
 *     request provided authentication by  answering the WWW-Authenticate header
 *     field challenge, but the server  did not accept that authentication.  The
 *     request should not be repeated.
 *
 *   * \c HTTP_NOT_FOUND
 *
 *     \c 404 \c Not \c Found
 *
 *     The requested  resource could not  be found but  may be available  in the
 *     future.  Subsequent requests by the client are permissible.
 *
 *   * \c HTTP_METHOD_NOT_ALLOWED
 *
 *     \c 405 \c Method \c Not \c Allowed
 *
 *     A  request  method is  not  supported  for  the requested  resource;  for
 *     example, a \c  GET request on a  form that requires data  to be presented
 *     via \c POST, or a \c PUT request on a read-only resource.
 *
 *   * \c HTTP_NOT_ACCEPTABLE
 *
 *     \c 406 \c Not \c Acceptable
 *
 *     The  requested  resource  is  capable  of  generating  only  content  not
 *     acceptable according to the Accept headers sent in the request.
 *
 *   * \c HTTP_PROXY_AUTHENTICATION_REQUIRED
 *
 *     \c 407 \c Proxy \c Authentication \c Required
 *
 *     The client must first authenticate itself with the proxy.
 *
 *   * \c HTTP_REQUEST_TIME_OUT
 *
 *     \c 408 \c Request \c Timeout
 *
 *     The  server  timed  out  waiting  for the  request.   According  to  HTTP
 *     specifications: "The  client did  not produce a  request within  the time
 *     that the server was prepared to  wait.  The client MAY repeat the request
 *     without modifications at any later time."
 *
 *   * \c HTTP_CONFLICT
 *
 *     \c 409 \c Conflict
 *
 *     Indicates that the request could not be processed because of conflict in
 *     the current state of the resource, such as an edit conflict between
 *     multiple simultaneous updates.
 *
 *   * \c HTTP_GONE
 *
 *     \c 410 \c Gone
 *
 *     Indicates that the resource requested is no longer available and will not
 *     be  available again.   This  should  be used  when  a  resource has  been
 *     intentionally removed and the resource  should be purged.  Upon receiving
 *     a 410  status code,  the client  should not request  the resource  in the
 *     future.  Clients such  as search engines should remove  the resource from
 *     their indices.  Most use cases do  not require clients and search engines
 *     to purge the resource, and a \c 404 \c Not \c Found may be used instead.
 *
 *   * \c HTTP_LENGTH_REQUIRED
 *
 *     \c 411 \c Length \c Required
 *
 *     The request did not specify the length of its content, which is required
 *     by the requested resource.
 *
 *   * \c HTTP_PRECONDITION_FAILED
 *
 *     \c 412 \c Precondition \c Failed
 *
 *     The server does not meet one  of the preconditions that the requester put
 *     on the request header fields.
 *
 *   * \c HTTP_REQUEST_ENTITY_TOO_LARGE
 *
 *     \c 413 \c Payload \c Too \c Large
 *
 *     The request  is larger  than the  server is willing  or able  to process.
 *     Previously called "Request Entity Too Large".
 *
 *   * \c HTTP_REQUEST_URI_TOO_LARGE
 *
 *     \c Request-URI \c Too \c Long (previous definition)
 *
 *     \c 414 \c URI \c Too \c Long
 *
 *     The  URI provided  was too  long for  the server  to process.   Often the
 *     result of  too much  data being  encoded as  a query-string  of a  \c GET
 *     request, in which case it should be converted to a \c POST request.
 *
 *   * \c HTTP_UNSUPPORTED_MEDIA_TYPE
 *
 *     \c 415 \c Unsupported \c Media \c Type
 *
 *     The request entity has a media type which the server or resource does not
 *     support.  For example, the client  uploads an image as image/svg+xml, but
 *     the server requires that images use a different format.
 *
 *   * \c HTTP_RANGE_NOT_SATISFIABLE
 *
 *     \c Requested \c Range \c Not \c Satisfiable (previous definition)
 *
 *     \c 416 \c Range \c Not \c Satisfiable
 *
 *     The client has  asked for a portion  of the file (byte  serving), but the
 *     server cannot supply that portion.  For  example, if the client asked for
 *     a part of the file that lies beyond the end of the file.
 *
 *   * \c HTTP_EXPECTATION_FAILED
 *
 *     \c 417 \c Expectation \c Failed
 *
 *     The server cannot meet the requirements of the Expect request-header
 *     field.
 *
 *   * \c HTTP_IM_A_TEAPOT
 *
 *     \c 418 \c I'M \c a \c teapot
 *
 *     This code was defined in 1998 as one of the traditional IETF April Fools'
 *     jokes, in  RFC 2324, Hyper Text  Coffee Pot Control Protocol,  and is not
 *     expected to  be implemented  by actual HTTP  servers.  The  RFC specifies
 *     this code should  be returned by teapots requested to  brew coffee.  This
 *     HTTP  status  is  used  as  an  Easter egg  in  some  websites,  such  as
 *     Google.com's I'm a teapot easter egg.
 *
 *   * \c HTTP_MISDIRECTED_REQUEST
 *
 *     \c 421 \c Misdirected \c Request
 *
 *     The  request was  directed at  a server  that is  not able  to produce  a
 *     response (for example because of connection reuse).
 *
 *   * \c HTTP_UNPROCESSABLE_ENTITY
 *
 *     \c 422 \c Unprocessable \c Entity
 *
 *     The request was well-formed but was unable to be followed due to semantic
 *     errors.
 *
 *   * \c HTTP_LOCKED
 *
 *     \c 423 \c Locked
 *
 *     The resource that is being accessed is locked.
 *
 *   * \c HTTP_FAILED_DEPENDENCY
 *
 *     \c 424 \c Failed \c Dependency
 *
 *     The  request failed  because  it  depended on  another  request and  that
 *     request failed (e.g., a \c PROPPATCH).
 *
 *   * \c HTTP_TOO_EARLY
 *
 *     \c 425 \c Too \c Early
 *
 *     Indicates that the server is unwilling  to risk processing a request that
 *     might be replayed.
 *
 *   * \c HTTP_UPGRADE_REQUIRED
 *
 *     \c 426 \c Upgrade \c Required
 *
 *     The client should  switch to a different protocol such  as TLS/1.3, given
 *     in the Upgrade header field.
 *
 *   * \c HTTP_PRECONDITION_REQUIRED
 *
 *     \c 428 \c Precondition \c Required
 *
 *     The origin  server requires the  request to be conditional.   Intended to
 *     prevent the  'lost update' problem, where  a client \c GETs  a resource's
 *     state, modifies it, and  \c PUTs it back to the  server, when meanwhile a
 *     third party has modified the state on the server, leading to a conflict.
 *
 *   * \c HTTP_TOO_MANY_REQUESTS
 *
 *     \c 429 \c Too \c Many \c Requests
 *
 *     The user has sent too many requests  in a given amount of time.  Intended
 *     for use with rate-limiting schemes.
 *
 *   * \c HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE
 *
 *     \c 431 \c Request \c Header \c Fields \c Too \c Large
 *
 *     The  server  is  unwilling  to  process the  request  because  either  an
 *     individual header field,  or all the header fields  collectively, are too
 *     large.
 *
 *   * \c HTTP_UNAVAILABLE_FOR_LEGAL_REASONS
 *
 *     \c 451 \c Unavailable \c For \c Legal \c Reasons
 *
 *     A  server operator  has  received a  legal  demand to  deny  access to  a
 *     resource or to  a set of resources that includes  the requested resource.
 *     The code \c 451 \c Unavailable \c For \c Legal \c Reasons was chosen as a
 *     reference to  the novel Fahrenheit  451 (see the Acknowledgements  in the
 *     RFC).
 *
 * * 5xx server errors
 *
 *   &rarr; The server failed to fulfil an apparently valid request.
 *
 *   Response status codes beginning with the  digit "5" indicate cases in which
 *   the  server is  aware that  it  has encountered  an error  or is  otherwise
 *   incapable  of performing  the request.  Except  when responding  to a  HEAD
 *   request, the server  should include an entity containing  an explanation of
 *   the error  situation, and indicate whether  it is a temporary  or permanent
 *   condition.  Likewise, user agents should display any included entity to the
 *   user.  These response codes are applicable to any request method.
 *
 *   * \c HTTP_INTERNAL_SERVER_ERROR
 *
 *     \c 500 \c Internal \c Server \c Error
 *
 *     A  generic  error  message,  given   when  an  unexpected  condition  was
 *     encountered and no more specific message is suitable.
 *
 *   * \c HTTP_NOT_IMPLEMENTED
 *
 *     \c 501 \c Not \c Implemented
 *
 *     The server either does not recognize  the request method, or it lacks the
 *     ability to fulfil  the request.  qUsually this  implies future availability
 *     (e.g., a new feature of a web-service API).
 *
 *   * \c HTTP_BAD_GATEWAY
 *
 *     \c 502 \c Bad \c Gateway
 *
 *     The  server was  acting as  a gateway  or proxy  and received  an invalid
 *     response from the upstream server.
 *
 *   * \c HTTP_SERVICE_UNAVAILABLE
 *
 *     \c 503 \c Service \c Unavailable
 *
 *     The server  cannot handle the request  (because it is overloaded  or down
 *     for maintenance).  Generally, this is a temporary state.
 *
 *   * \c HTTP_GATEWAY_TIME_OUT
 *
 *     \c 504 \c Gateway \c Timeout
 *
 *     The server was acting as a gateway  or proxy and did not receive a timely
 *     response from the upstream server.
 *
 *   * \c HTTP_VERSION_NOT_SUPPORTED
 *
 *     \c 505 \c HTTP \c Version \c Not \c Supported
 *
 *     The  server does  not  support  the HTTP  protocol  version  used in  the
 *     request.
 *
 *   * \c HTTP_VARIANT_ALSO_VARIES
 *
 *     \c 506 \c Variant \c Also \c Negotiates
 *
 *     Transparent content  negotiation for  the request  results in  a circular
 *     reference.
 *
 *   * \c HTTP_INSUFFICIENT_STORAGE
 *
 *     \c 507 \c Insufficient \c Storage
 *
 *     The server is  unable to store the representation needed  to complete the
 *     request.
 *
 *   * \c HTTP_LOOP_DETECTED
 *
 *     \c 508 \c Loop \c Detected
 *
 *     The server detected  an infinite loop while processing  the request (sent
 *     instead of \c 208 \c Already \c Reported).
 *
 *   * \c HTTP_NOT_EXTENDED
 *
 *     \c 510 \c Not \c Extended
 *
 *     Further extensions to  the request are required for the  server to fulfil
 *     it.
 *
 *   * \c HTTP_NETWORK_AUTHENTICATION_REQUIRED
 *
 *     \c 511 \c Network \c Authentication \c Required
 *
 *     The client  needs to authenticate  to gain network access.   Intended for
 *     use by intercepting proxies used to  control access to the network (e.g.,
 *     "captive portals"  used to require  agreement to Terms of  Service before
 *     granting full Internet access via a Wi-Fi hotspot).
 *
 * @see https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/group__HTTP__Status.html
 * @see httpd-2.4.25/include/httpd.h
 *
 * @param[in] request
 *   : the request data
 *
 * @return
 *   : on success: OK / on failure: DECLINED
 */

static int ecl_handler(request_rec * request)
{
    apr_status_t status = APR_SUCCESS;
    char * filename = apr_pstrdup(request->pool, "");
    char * filecontent = apr_pstrdup(request->pool, "");
    char * result = apr_pstrdup(request->pool, "");

    // Shall we decline to handle the request?

    if (   (!request->handler)
        || strcmp(request->handler, "application/x-httpd-ecl"))
    {
        return DECLINED;
    }

    // The output.  For the current request we output our message als html code.

    // Set the content type.

    ap_set_content_type(request, "text/html");

    // Output for development.

    if (!request->header_only)
    {
        //

        // Get and output file name.

        status = getFilename(request, & filename);
        if (APR_SUCCESS != status)
        {
            return status;
        }

        // Get and output the file content.

        status = getFilecontent(request, filename, & filecontent);
        if (APR_SUCCESS != status)
        {
            return status;
        }

        // Get result of evaluation.

        status =  evaluateByEcl(request, filecontent, & result);
        if (APR_SUCCESS == status)
        {
            ap_rprintf(request, "%s", result);
        }
        else
        {
            return status;
        }
    }

    // We have successfully handeled the requet.

    return OK;
}



/**
 * @brief Function to hook in the request processing.
 *
 * @details A  hook to  allow this module  to hook other  points in  the request
 * processing.
 *
 * In  this  function, modules  should  call  the  \c ap_hook_*()  functions  to
 * register an interest in a specific step in processing the current request.
 *
 * Hooks:
 *
 * * \c ap_hook_access_checker
 *
 *   This hook is used to apply  additional access control to this resource.  It
 *   runs  before a  user is  authenticated,  so this  hook is  really to  apply
 *   additional restrictions independent of a user.  It also runs independent of
 *   'Require'  directive  usage.   This  hook  should  be  registered  with  \c
 *   ap_hook_check_access().
 *
 * * \c ap_hook_access_checker_ex
 *
 *   This  hook  is  used  to  apply additional  access  control  and/or  bypass
 *   authentication for this resource.  It  runs before a user is authenticated,
 *   but  after  the  \c  ap_hook_access_checker  hook.   This  hook  should  be
 *   registered  with  \c ap_hook_check_access_ex().   If  "Satisfy  any" is  in
 *   effect, this hook may be skipped.
 *
 * * \c ap_hook_auth_checker
 *
 *   This  hook is  used to  check to  see if  the resource  being requested  is
 *   available  for   the  authenticated  user  (\c   request&rarr;user  and  \c
 *   request&rarr;ap_auth_type).   It runs  after the  \c ap_hook_access_checker
 *   and \c  ap_hook_check_user_id hooks.  Note that  it will only be  called if
 *   Apache determines  that access  control has been  applied to  this resource
 *   (through a  'Require' directive).  This  hook should be registered  with \c
 *   ap_hook_check_authz().  If  "Satisfy any"  is in effect,  this hook  may be
 *   skipped.
 *
 * * \c ap_hook_cache_status<p></p>
 *
 * * \c ap_hook_canon_handler<p></p>
 *
 * * \c ap_hook_check_access<p></p>
 *
 * * \c ap_hook_check_access_ex<p></p>
 *
 * * \c ap_hook_check_authn<p></p>
 *
 * * \c ap_hook_check_authz<p></p>
 *
 * * \c ap_hook_check_config
 *
 *   Run the \c check_config function for each module.
 *
 * * \c ap_hook_check_user_id
 *
 *   This hook  is used to analyze  the request headers, authenticate  the user,
 *   and set  the user information  in the request record  (\c request&rarr;user
 *   and \c  \c request&rarr;ap_auth_type).  This  hook is only run  when Apache
 *   determines that authentication/authorization is  required for this resource
 *   (as  determined  by  the  'Require'  directive).   It  runs  after  the  \c
 *   ap_hook_access_checker hook,  and before the \c  ap_hook_auth_checker hook.
 *   This hook should be registered  with \c ap_hook_check_authn().  If "Satisfy
 *   any" is in effect, this hook may be skipped.
 *
 * * \c ap_hook_child_init
 *
 *   &rarr; Place a hook that executes when a child process is spawned (commonly
 *   used for initializing modules after the server has forked).
 *
 *   Run the \c child_init function for each module.
 *
 * * \c ap_hook_child_status
 *
 *   Allow a  module to remain aware  of MPM child process  state changes, along
 *   with the generation and scoreboard slot of the process changing state.
 *
 *   With some MPMs (event and worker), an active MPM child process may lose its
 *   scoreboard slot if the child process  is exiting and the scoreboard slot is
 *   needed by other processes.  When this  occurs, the hook will be called with
 *   the \c MPM_CHILD_LOST_SLOT state.
 *
 * * \c ap_hook_client_login<p></p>
 *
 * * \c ap_hook_create_connection<p></p>
 *
 * * \c ap_hook_create_req<p></p>
 *
 * * \c ap_hook_create_request
 *
 *   Gives modules  a chance to  create their  \c request_config entry  when the
 *   request is created.
 *
 * * \c ap_hook_default_port<p></p>
 *
 * * \c ap_hook_dirwalk_stat
 *
 *   This hook allows  modules to handle/emulate the \c  apr_stat() calls needed
 *   for directory walk.
 *
 * * \c ap_hook_drop_privileges<p></p>
 *
 * * \c ap_hook_end_generation
 *
 *   Allow a module to  be notified when the last child  process of a generation
 *   exits.
 *
 * * \c ap_hook_error_log
 *
 *   Hook method to log error messages.
 *
 * * \c ap_hook_expr_lookup<p></p>
 *
 * * \c ap_hook_fatal_exception<p></p>
 *
 * * \c ap_hook_find_liveprop<p></p>
 *
 * * \c ap_hook_fixups
 *
 *   Allows modules to perform module-specific fixing of header fields.  This is
 *   invoked just before any content-handler.
 *
 * * \c ap_hook_foo<p></p>
 *
 * * \c ap_hook_force_authn
 *
 *   This hook allows a  module to force authn to be  required when processing a
 *   request. This hook should be registered with \c ap_hook_force_authn().
 *
 * * \c ap_hook_gather_propsets<p></p>
 *
 * * \c ap_hook_generate_log_id
 *
 *   Hook method to generate unique id for connection or request.
 *
 * * \c ap_hook_get_access_checker<p></p>
 *
 * * \c ap_hook_get_access_checker_ex<p></p>
 *
 * * \c ap_hook_get_auth_checker<p></p>
 *
 * * \c ap_hook_get_check_config<p></p>
 *
 * * \c ap_hook_get_check_user_id<p></p>
 *
 * * \c ap_hook_get_child_init<p></p>
 *
 * * \c ap_hook_get_child_status<p></p>
 *
 * * \c ap_hook_get_create_connection<p></p>
 *
 * * \c ap_hook_get_create_request<p></p>
 *
 * * \c ap_hook_get_default_port<p></p>
 *
 * * \c ap_hook_get_drop_privileges<p></p>
 *
 * * \c ap_hook_get_end_generation<p></p>
 *
 * * \c ap_hook_get_error_log<p></p>
 *
 * * \c ap_hook_get_expr_lookup<p></p>
 *
 * * \c ap_hook_get_fatal_exception<p></p>
 *
 * * \c ap_hook_get_fixups<p></p>
 *
 * * \c ap_hook_get_generate_log_id<p></p>
 *
 * * \c ap_hook_get_get_mgmt_items<p></p>
 *
 * * \c ap_hook_get_handler<p></p>
 *
 * * \c ap_hook_get_header_parser<p></p>
 *
 * * \c ap_hook_get_http_scheme<p></p>
 *
 * * \c ap_hook_get_insert_error_filter<p></p>
 *
 * * \c ap_hook_get_insert_filter<p></p>
 *
 * * \c ap_hook_get_log_transaction<p></p>
 *
 * * \c ap_hook_get_map_to_storage<p></p>
 *
 * * \c ap_hook_get_mgmt_items
 *
 *   This hook  provdes a  way for modules  to provide  metrics/statistics about
 *   their operational status.
 *
 * * \c ap_hook_get_monitor<p></p>
 *
 * * \c ap_hook_get_mpm<p></p>
 *
 * * \c ap_hook_get_mpm_get_name<p></p>
 *
 * * \c ap_hook_get_mpm_query<p></p>
 *
 * * \c ap_hook_get_mpm_register_timed_callback<p></p>
 *
 * * \c ap_hook_get_note_auth_failure<p></p>
 *
 * * \c ap_hook_get_open_logs<p></p>
 *
 * * \c ap_hook_get_optional_fn_retrieve<p></p>
 *
 * * \c ap_hook_get_post_config<p></p>
 *
 * * \c ap_hook_get_post_read_request<p></p>
 *
 * * \c ap_hook_get_pre_config<p></p>
 *
 * * \c ap_hook_get_pre_connection<p></p>
 *
 * * \c ap_hook_get_pre_mpm<p></p>
 *
 * * \c ap_hook_get_pre_read_request<p></p>
 *
 * * \c ap_hook_get_process_connection<p></p>
 *
 * * \c ap_hook_get_quick_handler<p></p>
 *
 * * \c ap_hook_get_suexec_identity<p></p>
 *
 * * \c ap_hook_get_test_config<p></p>
 *
 * * \c ap_hook_get_translate_name<p></p>
 *
 * * \c ap_hook_get_type_checker<p></p>
 *
 * * ap_hook_handler
 *
 *   &rarr; Place a hook that gets called after all the rewrites, access control
 *   etc has been handled.
 *
 *   Run the \c handler function for each module.
 *
 * * ap_hook_header_parser
 *
 *   Run the \c header_parser function for each module.
 *
 * * \c ap_hook_http_method<p></p>
 *
 * * \c ap_hook_http_scheme<p></p>
 *
 * * \c ap_hook_init_server<p></p>
 *
 * * \c ap_hook_input_pending
 *
 *   Hook called to determine whether any data is pending in the input filters.
 *
 * * \c ap_hook_insert_all_liveprops<p></p>
 *
 * * \c ap_hook_insert_error_filter
 *
 *   This hook allows modules to insert filters for the current error response.
 *
 * * \c ap_hook_insert_filter
 *
 *   This hook allows modules to insert filters for the current request.
 *
 * * \c ap_hook_insert_network_bucket<p></p>
 *
 * * \c ap_hook_log_transaction
 *
 *   &rarr; Place  a hook that executes  when the server  is about to add  a log
 *   entry of the current request.
 *
 * * \c ap_hook_lua_open<p></p>
 *
 * * \c ap_hook_lua_request<p></p>
 *
 * * \c ap_hook_map_to_storage
 *
 *   This hook  allow modules to  set the \c  per_dir_config based on  their own
 *   context (such as  "<Proxy>" sections) and responds  to contextless requests
 *   such as  TRACE that need  no security or  filesystem mapping, based  on the
 *   filesystem.
 *
 * * \c ap_hook_monitor
 *
 *   This hook  allows modules  to be called  at intervals by  some MPMs  in the
 *   parent process.  This is not portable to all platforms or MPMs.
 *
 * * \c ap_hook_mpm
 *
 *   Pass control to the MPM for steady-state processing.  It is responsible for
 *   controlling  the  parent  and  child   processes.   It  will  run  until  a
 *   restart/shutdown is indicated.
 *
 * * \c ap_hook_mpm_get_name
 *
 *   Get MPM name (e.g., "prefork" or "event").
 *
 * * \c ap_hook_mpm_query
 *
 *   Implement  the  \c  ap_mpm_query()  function.  The  MPM  should  return  \c
 *   OK+APR_ENOTIMPL for any unimplemented  query codes; modules which intercede
 *   for specific query codes should return \c DECLINED for others.
 *
 * * \c ap_hook_mpm_register_poll_callback
 *
 *   Register the specified callback.
 *
 * * \c ap_hook_mpm_register_timed_callback
 *
 *   Register the specified callback.
 *
 * * \c ap_hook_mpm_resume_suspended
 *
 *   Resume the suspended connection.
 *
 * * \c ap_hook_note_auth_failure<p></p>
 *
 * * \c ap_hook_open_htaccess
 *
 *   Allow modules to open htaccess files or perform operations before doing so.
 *
 * * \c ap_hook_open_logs
 *
 *   Run the \c open_logs function for each module.
 *
 * * \c ap_hook_optional_fn_retrieve
 *
 *   Retrieve the optional  functions for each module.  This  is run immediately
 *   before the server  starts.  Optional functions should  be registered during
 *   the hook registration phase.
 *
 * * \c ap_hook_optional_hook_test<p></p>
 *
 * * \c ap_hook_output_pending
 *
 *   Hook called to determine whether we should stay within the write completion
 *   phase.
 *
 * * \c ap_hook_post_config
 *
 *   &rarr; Place a hook that executes  after configuration has been parsed, but
 *   before the server has forked.
 *
 *   Run the \c post_config function for each module.
 *
 * * \c ap_hook_post_connect<p></p>
 *
 * * \c ap_hook_post_perdir_config
 *
 *   This  hook allows  modules  to  affect the  request  immediately after  the
 *   per-directory configuration for the request has been generated.
 *
 * * \c ap_hook_post_read_request<p></p>
 *
 * * \c ap_hook_post_request<p></p>
 *
 * * \c ap_hook_pre_close_connection<p></p>
 *
 * * \c ap_hook_pre_config
 *
 *   &rarr; Place  a hook that executes  before any configuration data  has been
 *   read (very early hook).
 *
 *   Run the \c pre_config function for each module.
 *
 * * \c ap_hook_pre_connection<p></p>
 *
 * * \c ap_hook_pre_handshake<p></p>
 *
 * * \c ap_hook_pre_mpm
 *
 *   Hook for post scoreboard creation, pre mpm.
 *
 * * \c ap_hook_pre_read_request<p></p>
 *
 * * \p ap_hook_pre_request<p></p>
 *
 * * \c ap_hook_pre_translate_name
 *
 *   This hook allow modules an opportunity  to translate the URI into an actual
 *   filename, before URL decoding happens.
 *
 * * \c ap_hook_preprocess<p></p>
 *
 * * \c ap_hook_process_connection<p></p>
 *
 * * \c ap_hook_protocol_get<p></p>
 *
 * * \c ap_hook_protocol_propose<p></p>
 *
 * * \c ap_hook_protocol_switch<p></p>
 *
 * * \c ap_hook_proxy_post_handshake<p></p>
 *
 * * \c ap_hook_quick_handler
 *
 *   &rarr; Similar  to \c ap_hook_handler,  except it  is run before  any other
 *   request hooks (translation, auth, fixups etc).
 *
 *   Run the \c quick_handler function for each module.  The \c quick_handler is
 *   run  before any  other  requests  hooks are  called  (\c location_walk,  \c
 *   directory_walk,  \c access_checker,  et.   al.).  This  hook  was added  to
 *   provide a quick way to serve content from a URI keyed cache.
 *
 * * \c ap_hook_request_status<p></p>
 *
 * * \c ap_hook_resume_connection
 *
 *   Notification  that  connection handling  is  resuming  (associating with  a
 *   thread).
 *
 * * \c ap_hook_scheme_handler<p></p>
 *
 * * \c ap_hook_session_decode<p></p>
 *
 * * \c ap_hook_session_encode<p></p>
 *
 * * \c ap_hook_session_load<p></p>
 *
 * * \c ap_hook_session_save<p></p>
 *
 * * \c ap_hook_ssl_conn_is_ssl
 *
 *   This  hook allows  modules that  manage  SSL connection  to register  their
 *   inquiry function for checking if a connection is using SSL from them.
 *
 * * \c ap_hook_ssl_var_lookup
 *
 *   This  hook  allows  modules  to  look   up  SSL  related  variables  for  a
 *   server/connection/request, depending on what  they inquire.  Some variables
 *   will only be available for a connection/request, for example.
 *
 * * \c ap_hook_status_hook<p></p>
 *
 * * \c ap_hook_suspend_connection
 *
 *   Notification that  connection handling  is suspending  (disassociating from
 *   the current thread).
 *
 * * \c ap_hook_test_config
 *
 *   Run the \c test_config  function for each module; this hook  is run only if
 *   the server was invoked to test the configuration syntax.
 *
 * * \c ap_hook_translate_name
 *
 *   &rarr; Place a hook that executes when  a URI needs to be translated into a
 *   filename on the server (think mod_rewrite).
 *
 *   This hook allow modules an opportunity  to translate the URI into an actual
 *   filename.  If  no modules do  anything special, the server's  default rules
 *   will be followed.
 *
 * * \c ap_hook_type_checker
 *
 *   This routine  is called to determine  and/or set the various  document type
 *   information  bits, like  \c  Content-type  (via \c  request->content_type),
 *   language, et cetera.
 *
 * * \c ap_hook_watchdog_exit<p></p>
 *
 * * \c ap_hook_watchdog_init<p></p>
 *
 * * \c ap_hook_watchdog_need<p></p>
 *
 * * \c ap_hook_watchdog_step<p></p>
 *
 * @see https://httpd.apache.org/docs/2.4/en/developer/modguide.html
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/group__hooks.html
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Hook.html
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/ap__expr_8h.html
 * @see httpd-2.4.25/include/ap_expr.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/ap__hooks_8h.html
 * @see httpd-2.4.25/include/ap_hooks.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/ap__mpm_8h.html
 * @see httpd-2.4.25/include/ap_mpm.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/http__config_8h.html
 * @see httpd-2.4.25/include/http_config.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/http__connection_8h.html
 * @see httpd-2.4.25/include/http_connection.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/http__core_8h.html
 * @see httpd-2.4.25/include/http_core.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/http__log_8h.html
 * @see httpd-2.4.25/include/http_log.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/http__protocol_8h.html
 * @see httpd-2.4.25/include/http_protocol.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/http__request_8h.html
 * @see httpd-2.4.25/include/http_request.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mpm__common_8h.html
 * @see httpd-2.4.25/include/mpm_common.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/scoreboard_8h.html
 * @see httpd-2.4.25/include/scoreboard.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__authz__dbd_8h.html
 * @see httpd-2.4.25/modules/aaa/mod_authz_dbd.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__cache_8h.html
 * @see httpd-2.4.25/modules/cache/mod_cache.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__watchdog_8h.html
 * @see httpd-2.4.25/modules/core/mod_watchdog.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__dbd_8h.html
 * @see httpd-2.4.25/modules/database/mod_dbd.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__dav_8h.html
 * @see httpd-2.4.25/modules/dav/main/mod_dav.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__xml2enc_8h.html
 * @see httpd-2.4.25/modules/filters/mod_xml2enc.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__status_8h.html
 * @see httpd-2.4.25/modules/generators/mod_status.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__lua_8h.html
 * @see httpd-2.4.25/modules/lua/mod_lua.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__proxy_8h.html
 * @see httpd-2.4.25/modules/proxy/mod_proxy.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__session_8h.html
 * @see httpd-2.4.25/modules/session/mod_session.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__ssl__openssl_8h.html
 * @see httpd-2.4.25/modules/ssl/mod_ssl_openssl.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/mod__optional__hook__export_8h.html
 * @see httpd-2.4.25/modules/test/mod_optional_hook_export.h
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/unixd_8h.html
 * @see httpd-2.4.25/os/unix/unixd.h
 *
 * To set the hook orderings:
 *
 * * \c APR_HOOK_REALLY_FIRST
 *
 *   Run this hook first, before ANYTHING.
 *
 * * \c APR_HOOK_FIRST
 *
 *   Run this hook first.
 *
 * * \c APR_HOOK_MIDDLE
 *
 *   Run this hook somewhere.
 *
 * * \c APR_HOOK_LAST
 *
 *   Run this hook after every other hook which is defined.
 *
 * * \c APR_HOOK_REALLY_LAST
 *
 *   Run this hook last, after EVERYTHING.
 *
 * * \c ap_register_input_filter
 *
 *   This function is  used to register an input filter  with the system.  After
 *   this registration is performed, then a  filter may be added into the filter
 *   chain by using \c ap_add_input_filter() and simply specifying the name.
 *
 * * \c ap_add_input_filter
 *
 *   Adds a named filter into the  filter chain on the specified request record.
 *   The filter will be installed with the specified context pointer.
 *
 * * \c ap_register_output_filter
 *
 *   deprecated
 *
 *   see ap_register_output_filter_protocol
 *
 * * \c ap_register_output_filter_protocol
 *
 *   This function is used to register  an output filter with the system.  After
 *   this registration is performed, then a  filter may be added directly to the
 *   filter chain by  using \c ap_add_output_filter() and  simply specifying the
 *   name, or as a provider under \c mod_filter.
 *
 * * \c ap_add_output_filter
 *
 *   Add a filter to  the current request.  Filters are added  in a FIFO manner.
 *   The first filter added will be the first filter called.
 *
 * @see https://ci.apache.org/projects/httpd/trunk/doxygen/util__filter_8h.html
 * @see httpd-2.4.25/include/util_filter.h
 *
 * @param[in] pool
 * : the memory pool
 *
 * @return void
 */

static void register_hooks(apr_pool_t * pool)
{
    // Function to handle the request.

    ap_hook_handler(ecl_handler, NULL, NULL, APR_HOOK_MIDDLE);
}



/**
 * @brief Apache HTTP Server Core DSO variable.
 *
 * @details Register this module as \c mod_ecl.
 *
 * The variable has the datatype \c typedef \c struct  \c module_struct.
 *
 * The elements  of the variable are  as follows in this  order (The identifiers
 * are chosen arbitrarily except for \c STANDARD20_MODULE_STUFF):
 *
 * 1. \c STANDARD20_MODULE_STUFF
 *
 *    This fills the  first element of this module with  boilerplate code.  This
 *    make the module a standard 2.x module.
 *
 * 2. \c create_directory_configuration
 *
 *    A  function to  allow this  module to  create per  directory configuration
 *    structures.
 *
 * 3. \c merge_directory_configuration
 *
 *    A function to  allow this module to merge the  per directory configuration
 *    structures for two directories.
 *
 * 4. \c create_server_configuration
 *
 *    A  function  to allow  this  module  to  create per  server  configuration
 *    structures.
 *
 * 5. \c merge_server_configuration
 *
 *    A function  to allow  this module  to merge  the per  server configuration
 *    structures for two servers.
 *
 * 6. \c directives_list
 *
 *    List  of  directives which  we  respond  to  in  a configuration  file  or
 *    .htaccess file.
 *
 * 7. \c register_hooks
 *
 *    A  hook  to  allow  this  module  to hook  other  points  in  the  request
 *    processing.   In this  function, modules  should call  the \c  ap_hook_*()
 *    functions to  register an interest  in a  specific step in  processing the
 *    current request.
 *
 * @remark  \c  httpd-2.4.25/include/http_config.h:  \c  typedef  \c  struct  \c
 * module_struct  \c  module;:  Module  structure.   Just  about  everything  is
 * dispatched through  these, directly  or indirectly  (through the  command and
 * handler tables).
 *
 * @remark    \c    httpd-2.4.25/include/http_config.h:     \c    \#define    \c
 * AP_DECLARE_MODULE(foo): This is  a convenience macro that combines  a call of
 * \c %APLOG_USE_MODULE with the definition of the module symbol.
 *
 * @remark    \c    httpd-2.4.25/include/http_config.h:     \c    \#define    \c
 * APLOG_USE_MODULE(foo): This macro is used  choose which module a file belongs
 * to.  This is necessary to allow per-module loglevel configuration.
 */

module AP_MODULE_DECLARE_DATA ecl_module =
{
    STANDARD20_MODULE_STUFF, // FIll  in  with  boilerplate code  to  make  this
                             // module a standard 2.x module.
    NULL,                    // Per-directory configuration handler.
    NULL,                    // Merge handler for per-directory configurations.
    NULL,                    // Per-server configuration handler.
    NULL,                    // Merge handler for per-server configurations.
    NULL,                    // Any directives we may have for httpd1.
    register_hooks           // Funtction to handel the request..
};

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
 * @brief Functions to handle input and output filters.
 *
 * @details
 */



//==============================================================================
//
// Headers to include.
//
//==============================================================================



// Headers from Apache HTTP Server.

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_log.h" // ??? io-filter

#include "ap_config.h"

#include "util_filter.h"

// Header from Apache Request Library.

#include "apr_buckets.h"

// Header from RaSt mod_ecl.

#include "boolean.h"
#include "status_codes.h"

#include "io_filter.h"



//==============================================================================
//
// Functions for input filter handling.
//
//==============================================================================

/**
 * @brief Context data to set in the input filter.
 *
 * @details
 *
 * @see [Apache HTTP Server --- Core routines --- Filter Chain](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html)
 *
 * @see [ap_add_input_filter](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html#gad60a001b4bddb5eafa7490b04aa182c0)
 */

ecl_input_filter_context_t * ecl_input_filter_context = NULL;



/**
 * @brief The function to call before the input filter handlers are invoked.
 *
 * @details
 *
 * @code{.c}typedef int (* ap_init_filter_func) (ap_filter_t * f)@endcode
 *
 * @see[Apache HTTP Server --- Core routines --- Filter Chain](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html)
 *
 * @see[ap_init_filter_func](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html#ga7ecd47b41aacab276198f6d1fc05a56c)
 *
 * @param[in,out] input_filter
 *   The input filter.
 *
 * @return ap_status ---  on success: OK /  on failure: one of  the (none-) HTTP
 *   status codes (HTTP_INTERNAL_SERVER_ERROR)
 */

int ecl_input_filter_initalize(ap_filter_t * input_filter)
{
  int ap_status = HTTP_INTERNAL_SERVER_ERROR;

  // Allocate memory for the input filter.

  input_filter->ctx = (ecl_input_filter_context_t *) apr_pcalloc(input_filter->r->pool, sizeof(ecl_input_filter_context_t));

  // Initalize the input filter context.

  ecl_input_filter_context_t * ecl_input_filter_context = NULL;
  ecl_input_filter_context = (ecl_input_filter_context_t *) input_filter->ctx;
  ecl_input_filter_context->dummy = 0; // dummy value, we can remove this later
  ecl_input_filter_context->brigade = apr_brigade_create(input_filter->r->pool, input_filter->r->connection->bucket_alloc);

  // Return status code.

  ap_status = OK;
  return (ap_status);
}



/**
 * @brief The input filter function.
 *
 * @details
 *
 * <b>Problem</b>
 *
 *    Alert!: Unexpected network read error; connection aborted.
 *
 * <b>Solution:</b>
 *
 * Minimum required code:
 *
 * @code{.c}@endcode
 *
 * @code{.c}typedef apr_status_t (* ap_in_filter_func) (ap_filter_t * f, apr_bucket_brigade * b, ap_input_mode_t mode, apr_read_type_e block, apr_off_t readbytes)@endcode
 *
 * @see[Apache HTTP Server --- Core routines --- Filter Chain](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html)
 *
 * @see[ap_in_filter_func](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html#ga8e6e76deedf9ef3e3a4aa33419663875)
 *
 * @param[in,out] output_filter
 *   The input filter.
 *
 * @param[in,out] brigade
 *   The input brigade.
 *
 * @return ap_status ---  on success: OK /  on failure: one of  the (none-) HTTP
 *   status codes (HTTP_INTERNAL_SERVER_ERROR)
 */

int ecl_input_filter_hander(ap_filter_t * input_filter,
                            apr_bucket_brigade * input_brigade,
                            ap_input_mode_t mode,
                            apr_read_type_e block,
                            apr_off_t readbytes)
{
  int ap_status = HTTP_INTERNAL_SERVER_ERROR;
  apr_status_t apr_status = APR_FAILURE;

  //
  // Initalize the input filter.
  //

  // Check if the input filter is already initialised.

  ecl_input_filter_context_t * ecl_input_filter_context = NULL;
  ecl_input_filter_context = (ecl_input_filter_context_t *) input_filter->ctx;

  if (NULL == ecl_input_filter_context)
  {
    // Input filter is not initialised.

    ap_status = ecl_input_filter_initalize(input_filter);
    ecl_input_filter_context = (ecl_input_filter_context_t *) input_filter->ctx;

    // Check if we have initialised the input filter.

    if (OK == ap_status)
    {
      // We have initialised the input filter.
    }
    else
    {
       // We have not initialised the input filter.

      // Return status code.

      return (ap_status);
    }
  }
  else
  {
    // Input filter is initialised.
  }

  //
  // Create EOS bucket.
  //

  // Create EOS bucket.

  apr_bucket * eos_bucket = NULL;
  eos_bucket = apr_bucket_eos_create(input_filter->r->connection->bucket_alloc);

  // Check if we have an EOS bucket.

  if (NULL == eos_bucket)
  {
    // We have no EOS bucket.

    // Return status code.

    return (ap_status);
  }
  else
  {
    // We have an EOS bucket.
  }

  //
  // Create a new bucket and fill it  using the data form the input bucket.  The
  // bucket    is    in   ecl_input_filter_context->brigade    initialised    by
  // ecl_input_filter_initalize.
  //

  // Check if we have a input filter context brigade.

  if (NULL == ecl_input_filter_context->brigade)
  {
    // We have no brigade.

    // Return status code

    return (ap_status);
  }
  else
  {
    // We have a brigade.
  }

  // dummy value, we can remove this later
  char * input_begin_text = "( input begin text )";
  apr_bucket * input_begin_bucket = NULL;
  input_begin_bucket = apr_bucket_heap_create(input_begin_text, strlen(input_begin_text), NULL, input_filter->r->connection->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(ecl_input_filter_context->brigade, input_begin_bucket);

  // Loop throu output brigade and build brigade.

  apr_bucket * read_bucket = NULL;
  apr_bucket * temp_bucket = NULL;
  const char * data = NULL;
  apr_size_t lenght = 0;
  for (read_bucket = APR_BRIGADE_FIRST(input_brigade); read_bucket != APR_BRIGADE_SENTINEL(input_brigade); read_bucket = APR_BUCKET_NEXT(read_bucket))
  {

    // Read from bucket.

    apr_status = apr_bucket_read(read_bucket, & data, & lenght, APR_BLOCK_READ);

    // Check if we have read from bucket.

    if (APR_SUCCESS == apr_status)
    {
      // We have read from bucket.
    }
    else
    {
      // We have not read from bucket.

      // Return status code.

      return (ap_status);
    }

    // We create a bucket.

    temp_bucket = apr_bucket_heap_create(data, lenght, NULL, input_filter->r->connection->bucket_alloc);

    // Check if we have a bucket.

    if (NULL == temp_bucket)
    {
      // We have not a bucket.

      // Return status code.

      return (ap_status);
    }
    else
    {
      // We have a bucket.
    }

    // Add to tail of bucket.

    APR_BRIGADE_INSERT_TAIL(ecl_input_filter_context->brigade, temp_bucket);
  }

  // dummy value, we can remove this later
  char * input_end_text = "( input end text )";
  apr_bucket * input_end_bucket = NULL;
  input_end_bucket = apr_bucket_heap_create(input_end_text, strlen(input_end_text), NULL, input_filter->r->connection->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(ecl_input_filter_context->brigade, input_end_bucket);

  //
  // We add an EOS bucket to the end of the input brigade.
  //

  // Check if we have already an EOS bucket.

  if (APR_BRIGADE_LAST(ecl_input_filter_context->brigade) == APR_BRIGADE_SENTINEL(ecl_input_filter_context->brigade))
  {
    // We already have an EOS bucket.
  }
  else
  {
   // We do not have an Eos BUCKET:

    APR_BRIGADE_INSERT_TAIL(ecl_input_filter_context->brigade, eos_bucket);
  }

  //
  // Pass the output bucket.
  //

  // Pass the output bucket down to the next filter on the filter stack.

  apr_status = ap_get_brigade(input_filter->next, input_brigade, mode, block, readbytes);

  // Check if we passed the output brigade.

  if (APR_SUCCESS == apr_status)
  {
    // We have passed the output brigase.
  }
  else
  {
    // We have not passed the brigade.

    // Return status code.

    return (ap_status);
  }

  //
  // Clean up the brigade.
  //
/*
  // Clean up the brigade.

  apr_status = apr_brigade_cleanup(output_brigade);

  // Check if we have cleandup the brigade.

  if (APR_SUCCESS == apr_status)
  {
    // We have passed the output brigase.
  }
  else
  {
    // We have not passed the brigade.

    // Return status code.

    return (ap_status);
  }
*/
  // Return the status code.

  ap_status = OK;
  return ap_status;
}



//==============================================================================
//
// Functions for output filter handling.
//
//==============================================================================

/**
 * @brief Context data to set in the output filter.
 *
 * @details
 *
 * @see [Apache HTTP Server --- Core routines --- Filter Chain](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html)
 *
 * @see [ap_add_output_filter](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html#ga551cd1de5706c74d5d8643823791d679)
 */

ecl_output_filter_context_t * ecl_output_filter_context = NULL;



/**
 * @brief The function to call before the output filter handlers are invoked.
 *
 * @details
 *
 * @code{.c}typedef int (* ap_init_filter_func) (ap_filter_t * f)@endcode
 *
 * @see[Apache HTTP Server --- Core routines --- Filter Chain](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html)
 *
 * @see[ap_init_filter_func](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html#ga7ecd47b41aacab276198f6d1fc05a56c)
 *
 * @param[in,out] output_filter
 *   The output filter.
 *
 * @return ap_status ---  on success: OK /  on failure: one of  the (none-) HTTP
 *   status codes (HTTP_INTERNAL_SERVER_ERROR)
 */

int ecl_output_filter_initalize(ap_filter_t * output_filter)
{
  int ap_status = HTTP_INTERNAL_SERVER_ERROR;

  // Allocate memory for the output filter.

  output_filter->ctx = (ecl_output_filter_context_t *) apr_pcalloc(output_filter->r->pool, sizeof(ecl_output_filter_context_t));

  // Initalize the output filter context.

  ecl_output_filter_context_t * ecl_output_filter_context = NULL;
  ecl_output_filter_context = (ecl_output_filter_context_t *) output_filter->ctx;
  ecl_output_filter_context->dummy = 0; // dummy value, we can remove this later
  ecl_output_filter_context->brigade = apr_brigade_create(output_filter->r->pool, output_filter->r->connection->bucket_alloc);

  // Return status code.

  ap_status = OK;
  return (ap_status);
}



/**
 * @brief The output filter function.
 *
 * @details
 *
 * <b>Problem</b>
 *
 *    Alert!: Unexpected network read error; connection aborted.
 *
 * <b>Solution:</b>
 *
 * Minimum required code:
 *
 * @code{.c}apr_bucket * eos_bucket = NULL;
 * eos_bucket = apr_bucket_eos_create(output_filter->r->connection->bucket_alloc);
 * APR_BRIGADE_INSERT_TAIL(output_brigade, eos_bucket);
 * ap_pass_brigade(output_filter->next, output_brigade);
 * apr_brigade_cleanup(output_brigade);@endcode
 *
 * @code{.c}typedef apr_status_t (* ap_out_filter_func) (ap_filter_t * f, apr_bucket_brigade * b)@endcode
 *
 * @see[Apache HTTP Server --- Core routines --- Filter Chain](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html)
 *
 * @see[ap_out_filter_func](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html#ga5c80541a5a1dcc9a383e90aa82be59b3)
 *
 * @param[in,out] output_filter
 *   The output filter.
 *
 * @param[in,out] brigade
 *   The output brigade.
 *
 * @return ap_status ---  on success: OK /  on failure: one of  the (none-) HTTP
 *   status codes (HTTP_INTERNAL_SERVER_ERROR)
 */

int ecl_output_filter_hander(ap_filter_t * output_filter, apr_bucket_brigade * output_brigade)
{
  int ap_status = HTTP_INTERNAL_SERVER_ERROR;
  apr_status_t apr_status = APR_FAILURE;

  //
  // Initalize the output filter.
  //

  // Check if the output filter is already initialised.

  ecl_output_filter_context_t * ecl_output_filter_context = NULL;
  ecl_output_filter_context = (ecl_output_filter_context_t *) output_filter->ctx;

  if (NULL == ecl_output_filter_context)
  {
    // Output filter is not initialised.

    ap_status = ecl_output_filter_initalize(output_filter);
    ecl_output_filter_context = (ecl_output_filter_context_t *) output_filter->ctx;

    // Check if we have initialised the output filter.

    if (OK == ap_status)
    {
      // We have initialised the output filter.
    }
    else
    {
       // We have not initialised the output filter.

      // Return status code.

      return (ap_status);
    }
  }
  else
  {
    // Output filter is initialised.
  }

  //
  // Create EOS bucket.
  //

  // Create EOS bucket.

  apr_bucket * eos_bucket = NULL;
  eos_bucket = apr_bucket_eos_create(output_filter->r->connection->bucket_alloc);

  // Check if we have an EOS bucket.

  if (NULL == eos_bucket)
  {
    // We have no EOS bucket.

    // Return status code.

    return (ap_status);
  }
  else
  {
    // We have an EOS bucket.
  }

  //
  // Create a new bucket and fill it using the data form the output bucket.  The
  // bucket    is   in    ecl_output_filter_context->brigade   initialised    by
  // ecl_output_filter_initalize.
  //

  // Check if we have a output filter context brigade.

  if (NULL == ecl_output_filter_context->brigade)
  {
    // We have no brigade.

    // Return status code

    return (ap_status);
  }
  else
  {
    // We have a brigade.
  }

  // dummy value, we can remove this later
  char * output_begin_text = "( output begin text )";
  apr_bucket * output_begin_bucket = NULL;
  output_begin_bucket = apr_bucket_heap_create(output_begin_text, strlen(output_begin_text), NULL, output_filter->r->connection->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(ecl_output_filter_context->brigade, output_begin_bucket);

  // Loop throu output brigade and build brigade.

  apr_bucket * read_bucket = NULL;
  apr_bucket * temp_bucket = NULL;
  const char * data = NULL;
  apr_size_t lenght = 0;
  for (read_bucket = APR_BRIGADE_FIRST(output_brigade); read_bucket != APR_BRIGADE_SENTINEL(output_brigade); read_bucket = APR_BUCKET_NEXT(read_bucket))
  {

    // Read from bucket.

    apr_status = apr_bucket_read(read_bucket, & data, & lenght, APR_BLOCK_READ);

    // Check if we have read from bucket.

    if (APR_SUCCESS == apr_status)
    {
      // We have read from bucket.
    }
    else
    {
      // We have not read from bucket.

      // Return status code.

      return (ap_status);
    }

    // We create a bucket.

    temp_bucket = apr_bucket_heap_create(data, lenght, NULL, output_filter->r->connection->bucket_alloc);

    // Check if we have a bucket.

    if (NULL == temp_bucket)
    {
      // We have not a bucket.

      // Return status code.

      return (ap_status);
    }
    else
    {
      // We have a bucket.
    }

    // Add to tail of bucket.

    APR_BRIGADE_INSERT_TAIL(ecl_output_filter_context->brigade, temp_bucket);
  }

  // dummy value, we can remove this later
  char * output_end_text = "( output end text )";
  apr_bucket * output_end_bucket = NULL;
  output_end_bucket = apr_bucket_heap_create(output_end_text, strlen(output_end_text), NULL, output_filter->r->connection->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(ecl_output_filter_context->brigade, output_end_bucket);

  //
  // We add an EOS bucket to the end of the input brigade.
  //

  // Check if we have already an EOS bucket.

  if (APR_BRIGADE_LAST(ecl_output_filter_context->brigade) == APR_BRIGADE_SENTINEL(ecl_output_filter_context->brigade))
  {
    // We already have an EOS bucket.
  }
  else
  {
   // We do not have an Eos BUCKET:

   APR_BRIGADE_INSERT_TAIL(ecl_output_filter_context->brigade, eos_bucket);
  }

  //
  // Pass the output bucket.
  //

  // Pass the output bucket down to the next filter on the filter stack.

  apr_status = ap_pass_brigade(output_filter->next, ecl_output_filter_context->brigade);

  // Check if we passed the output brigade.

  if (APR_SUCCESS == apr_status)
  {
    // We have passed the output brigase.
  }
  else
  {
    // We have not passed the brigade.

    // Return status code.

    return (ap_status);
  }

  //
  // Clean up the brigade.
  //

  // Clean up the brigade.

  apr_status = apr_brigade_cleanup(output_brigade);

  // Check if we have cleandup the brigade.

  if (APR_SUCCESS == apr_status)
  {
    // We have passed the output brigase.
  }
  else
  {
    // We have not passed the brigade.

    // Return status code.

    return (ap_status);
  }

  //
  // We are done.
  //

  // Return status code.

  ap_status = OK;
  return (ap_status);
}

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
#include "http_log.h"
#include "http_request.h"
#include "http_protocol.h"
#include "http_connection.h"

#include "ap_config.h"

#include "util_filter.h"

// Header from Apache Request Library.

#include "apr_buckets.h"

// Header from RaSt mod_ecl.

#include "boolean.h"

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
  ecl_input_filter_context->dummy = 123; // dummy value, we can remove this later
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
 * mode

AP_MODE_READBYTES

AP_MODE_GETLINE

AP_MODE_EATCRLF

AP_MODE_SPECULATIVE

P_MODE_EXHAUSTIVE

AP_MODE_INIT

 * block

APR_BLOCK_READ

APR_NONBLOCK_READ

 * readbytes


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



/* ??? filter
int ecl_input_filter_hander(ap_filter_t * input_filter,
                            apr_bucket_brigade * input_brigade,
                            ap_input_mode_t mode,
                            apr_read_type_e block,
                            apr_off_t readbytes)
{
  int ap_status = HTTP_INTERNAL_SERVER_ERROR;
  apr_status_t apr_status = APR_EGENERAL;

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
//
//  // Clean up the brigade.
//
//  apr_status = apr_brigade_cleanup(output_brigade);
//
//  // Check if we have cleandup the brigade.
//
//  if (APR_SUCCESS == apr_status)
//  {
//    // We have passed the output brigase.
//  }
//  else
//  {
//    // We have not passed the brigade.
//
//    // Return status code.
//
//    return (ap_status);
//  }
//
  // Return the status code.

  ap_status = OK;
  return ap_status;
}
*/


/*
int ecl_input_filter_hander(__attribute__((unused)) ap_filter_t * f, //input_filter,
                            __attribute__((unused)) apr_bucket_brigade * bb, //input_brigade,
                            __attribute__((unused)) ap_input_mode_t mode,
                            __attribute__((unused)) apr_read_type_e block,
                            __attribute__((unused)) apr_off_t readbytes)
{
  int ap_status = HTTP_INTERNAL_SERVER_ERROR;
  __attribute__((unused)) apr_status_t apr_status = APR_EGENERAL;



  // Allocate memory for the input filter.
  f->ctx = (ecl_input_filter_context_t *) apr_pcalloc(f->r->pool, sizeof(ecl_input_filter_context_t));
  // Initalize the input filter context.
  ecl_input_filter_context_t * ecl_input_filter_context = NULL;
  ecl_input_filter_context = (ecl_input_filter_context_t *) f->ctx;
  ecl_input_filter_context->dummy = 0; // dummy value, we can remove this later
  ecl_input_filter_context->brigade = apr_brigade_create(f->r->pool, f->r->connection->bucket_alloc);
f->ctx = ecl_input_filter_context;

return ap_get_brigade(f->next, bb, mode, block, readbytes);

if (mode != AP_MODE_READBYTES)
{
  return ap_get_brigade(f->next, bb, mode, block, readbytes);
}



  //
  // We are done.
  //

  // Return status code.

  ap_status = OK;
  return ap_status;
}
*/



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
  ecl_output_filter_context->dummy = 123; // dummy value, we can remove this later
  ecl_output_filter_context->brigade = apr_brigade_create(output_filter->r->pool, output_filter->r->connection->bucket_alloc);

  // Return status code.

  ap_status = OK;
  return (ap_status);
}



/**
 * @brief Function to use to free the data.
 *
 * @details
 *
 * @param[in] data
 *   The data to free.
 *
 * @param
 */

void heap_free(__attribute__((unused)) void * data)
{
}



/**
 * @brief The output filter function.
 *
 * @details
 *
 * <b>Ten rules for output filters</b>
 *
 * In summary, here is a set of rules for all output filters to follow:
 *
 * 01. Output filters should not pass  empty brigades down the filter chain, but
 *     should be tolerant of being passed empty brigades.
 *
 * 02. Output  filters must  pass all  metadata buckets  down the  filter chain;
 *     FLUSH  buckets should  be respected  by passing  any pending  or buffered
 *     buckets down the filter chain.
 *
 * 03. Output filters should ignore any buckets following an EOS bucket.
 *
 * 04. Output filters must  process a fixed amount of data at  a time, to ensure
 *     that memory  consumption is not proportional  to the size of  the content
 *     being filtered.
 *
 * 05. Output filters should be agnostic  with respect to bucket types, and must
 *     be able to process buckets of unfamiliar type.
 *
 * 06. After  calling ap_pass_brigade to pass  a brigade down the  filter chain,
 *     output filters should  call apr_brigade_cleanup to ensure  the brigade is
 *     empty before reusing that brigade  structure; output filters should never
 *     use apr_brigade_destroy to "destroy" brigades.
 *
 * 07. Output filters  must setaside any buckets which are  preserved beyond the
 *     duration of the filter function.
 *
 * 08. Output filters  must not ignore the return value  of ap_pass_brigade, and
 *     must return appropriate errors back up the filter chain.
 *
 * 09. Output  filters must only  create a fixed  number of bucket  brigades for
 *     each response, rather than one per invocation.
 *
 * 10. Output  filters should  first attempt non-blocking  reads from  each data
 *     bucket, and send a FLUSH bucket down the filter chain if the read blocks,
 *     before retrying with a blocking read.
 *
 * @see [Guide to writing output filters](https://httpd.apache.org/docs/2.4/en/developer/output-filters.html)
 *
 *
 *
 * [AP_BUCKET_IS_ERROR](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__PROTO.html#gaab791518d091322fdc35b084f5b9f642)
 *   --- Determine if a bucket is an error bucket.
 *
 * [AP_BUCKET_IS_EOC](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONNECTION.html#ga6ae3ca117a7265de04b91849d6b5acb1)
 *   --- determine if a bucket is an End Of Connection (EOC) bucket.
 *
 * [AP_BUCKET_IS_EOR](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__REQ.html#ga8318214baeaec5af97903afeef38f253)
 *   --- Determine if a bucket is an End Of REQUEST (EOR) bucket.
 *
 * [APR_BUCKET_IS_EOS](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga89b225e1c08473766eec719b985ca0d6)
 *   --- Determine if a bucket is an EOS  bucket.  Indicates that the end of the
 *   response has  been reached and  no further  buckets need be  processed.  (A
 *   metadata bucket.)
 *
 * [APR_BUCKET_IS_FILE](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga1d54037bb6953c73d5c61f64c311d470)
 *   --- Determine if a bucket is a FILE bucket.
 *
 * [APR_BUCKET_IS_FLUSH](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga24105da0bb755a775c4b2a519d7c25f9)
 *   --- Determine if  a bucket is  a FLUSH  bucket.  Indicates that  the filter
 *   should flush  any buffered  buckets (if applicable)  down the  filter chain
 *   immediately (A metadata bucket.)
 *
 * [APR_BUCKET_IS_HEAP](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga3dee4fbd5b4e16e5188a6cfec40b6961)
 *   --- Determine if a bucket is a HEAP bucket.
 *
 * [APR_BUCKET_IS_IMMORTAL](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga53f1188c168acf7dcfabfa1ac5a08655)
 *    --- Determine if a bucket is a IMMORTAL bucket.
 *
 * [APR_BUCKET_IS_METADATA](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga506cb29cc1ec1abeb487e01b122bd4d9)
 *   --- Determine if  a bucket contains metadata.   An empty bucket is  safe to
 *   arbitrarily remove  if and only if  this is false.  (metadata  buckets are:
 *   APR_BUCKET_IS_EOS, APR_BUCKET_IS_FLUSH)
 *
 * [APR_BUCKET_IS_MMAP](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga1693d49aeb0287e3b171c922c6b633d2)
 *   --- Determine if a bucket is a MMAP bucket.
 *
 * [APR_BUCKET_IS_PIPE](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga03e62d86c994d1d1cc9be3eb8b8f6ac6)
 *   --- Determine if a bucket is a PIPE bucket.
 *
 * [APR_BUCKET_IS_POOL](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#gadbe80a9016da6a06ab414ba61d6625d0)
 *   --- Determine if a bucket is a POOL bucket.
 *
 * [APR_BUCKET_IS_SOCKET](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga4b8315b498e4eb67efde1c83bdbe8b87)
 *   --- Determine if a bucket is a SOCKET bucket.
 *
 * [APR_BUCKET_IS_TRANSIENT](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga1a7e7ac5fddbab4ce189e8d3007446ff)
 *   --- Determine if a bucket is a TRANSIENT bucket.
 *
 * @see [ap_bucket_error_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__PROTO.html#gac8fe53c5d98109d10de42eda3c4854d3)
 * @see [ap_bucket_eoc_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__CONNECTION.html#ga32f8ba5ef2f1c4dbc0846c92f4718fbd)
 * @see [ap_bucket_eor_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__REQ.html#ga3950e8f906fce861eecaec889614d6d5)
 * @see [apr_bucket_eos_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#gaac308b459ecdab87e94b12adad1b8760)
 * @see [apr_bucket_file_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga910659d919d24d38cd1751d873a5223e)
 * @see [apr_bucket_flush_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga59aa3bf16eeb371aae885a75585eb0c0)
 * @see [apr_bucket_heap_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga72b7829934231a0001f366d800e81dd8)
 * @see [apr_bucket_immortal_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga874c647bd995827fd8371f212720d3c9)
 * @see metadata
 * @see [apr_bucket_mmap_create)[https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#gabd2fdc2084c960530be7f2bb0d49b287]
 * @see [apr_bucket_pipe_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#gac700245e5a2d7c560d8469b9d7fd6461)
 * @see [apr_bucket_pool_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga1e8a21739b2f3befc217194008110913)
 * @see [apr_bucket_socket_create] https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#ga3636d608bff769fa8cc97b1a5ea89fa7
 * @see [apr_bucket_transient_create](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html#gaff38144f90292f9eeb6c1acb3fe97ce1)
 *
 * @see [Apache Portability Runtime library --- Bucket Brigades](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APR__Util__Bucket__Brigades.html)
 *
 * @see [http_connection.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__connection_8h.html)
 * @see [http_connection.h Source Code](https://ci.apache.org/projects/httpd/trunk/doxygen/http__connection_8h_source.html)
 *
 * @see [http_protocol.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__protocol_8h.html)
 * @see [http_protocol.h Source Code](https://ci.apache.org/projects/httpd/trunk/doxygen/http__protocol_8h_source.html)
 *
 * @see [http_request.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/http__request_8h.html)
 * @see [http_request.h Source Code](https://ci.apache.org/projects/httpd/trunk/doxygen/http__request_8h_source.html)
 *
 * @see [apr_buckets.h File Reference](https://ci.apache.org/projects/httpd/trunk/doxygen/apr__buckets_8h.html)
 * @see [apr_buckets.h Source Code](https://ci.apache.org/projects/httpd/trunk/doxygen/apr__buckets_8h_source.html)
 *
 * @code{.c}typedef apr_status_t (* ap_out_filter_func) (ap_filter_t * f, apr_bucket_brigade * b)@endcode
 *
 * @see[Apache HTTP Server --- Core routines --- Filter Chain](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html)
 *
 * @see[ap_out_filter_func](https://ci.apache.org/projects/httpd/trunk/doxygen/group__APACHE__CORE__FILTER.html#ga5c80541a5a1dcc9a383e90aa82be59b3)
 *
 * @code{.c}int ecl_output_filter_hander(ap_filter_t * output_filter,
 *                                       apr_bucket_brigade * output_brigade
 * )
 * {
 *   // initialize context
 *
 *   output_filter->ctx = (ecl_output_filter_context_t *) apr_pcalloc(output_filter->r->pool, sizeof(ecl_output_filter_context_t));
 *   ecl_output_filter_context_t * ecl_output_filter_context = NULL;
 *   ecl_output_filter_context = (ecl_output_filter_context_t *) output_filter->ctx;
 *   ecl_output_filter_context->dummy = 0; // dummy value, we can remove this later
 *   ecl_output_filter_context->brigade = apr_brigade_create(output_filter->r->pool, output_filter->r->connection->bucket_alloc);
 *   output_filter->ctx = ecl_output_filter_context;
 *
 *   // pass brigade to next filter
 *
 *   ap_pass_brigade(output_filter->next, output_brigade);
 *
 *   // clean up
 *
 *   apr_brigade_cleanup(ecl_output_filter_context->brigade);
 *   apr_brigade_cleanup(output_brigade);
 *
 *   return (OK);
 * }@endcode
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

int ecl_output_filter_hander(ap_filter_t * output_filter,
                             apr_bucket_brigade * output_brigade
)
{
  int ap_status = HTTP_INTERNAL_SERVER_ERROR;
  apr_status_t apr_status = APR_EGENERAL;

  //
  // Check the function parameter.
  //

  // Check if the  output brigade is empty.   If the output brigade  is empty we
  // exit immediately.  This is to avoid passing an empty brigade.

  if (APR_BRIGADE_EMPTY(output_brigade))
  {
    // The output brigade is empty.

    // Pass the output brigade down to the next filter on the filter stack.

    apr_status = ap_pass_brigade(output_filter->next, output_brigade);

    // Check if we have passed the output brigade down to the next filter on the
    // filter stack.

    if (APR_SUCCESS == apr_status)
    {
      // We have passed the output brigade down to the next filter on the filter
      // stack.
    }
    else
    {
      // We have not passed the output brigade down to the next filter on the
      // filter stack.

      // Return status code.

      return (ap_status);
    }

    // Empty and destroy the entire bucket brigade.

    apr_status = apr_brigade_cleanup(output_brigade);

    // Check if we have emptied and destroyed the entire bucket brigade.

    if (APR_SUCCESS == apr_status)
    {
      // We have emptied and destroyed the entire bucket brigade.
    }
    else
    {
      // We have not emptied and destroyed the entire bucket brigade.

      // Return status code.

      return (ap_status);
    }

    // Return exit code.

    ap_status = OK;
    return (ap_status);
  }
  else
  {
    // The output brigade is not empty.
  }

  // Check if  the first bucket  is an  EOS bucket.  No  need to run  the output
  // filter for HEAD requests.

  if (APR_BUCKET_IS_EOS(APR_BRIGADE_FIRST(output_brigade)))
  {
    // The first bucket is an EOS bucket.

    // Remove the output filter from the request.

    ap_remove_output_filter(output_filter);

    // Pass the output brigade down to the next filter on the filter stack.

    apr_status = ap_pass_brigade(output_filter->next, output_brigade);

    // Check if  we have  passed the output  brigade to the  next filter  on the
    // filter stack.

    if (APR_SUCCESS == apr_status)
    {
      // We have  passed the  output brigade  to the next  filter on  the filter
      // stack.
    }
    else
    {
      // We have not passed the output brigade  to the next filter on the filter
      // stack.

      // Return status code.

      return (ap_status);
    }

    // Empty and destroy the entire bucket brigade.

    apr_status = apr_brigade_cleanup(output_brigade);

    // Check if we have emptied and destroyed the entire bucket brigade.

    if (APR_SUCCESS == apr_status)
    {
      // We have emptied and destroyed the entire bucket brigade.
    }
    else
    {
      // We have not emptied and destroyed the entire bucket brigade.

      // Return status code.

      return (ap_status);
    }

    // Do not send out 0 Content-Lengths if it is a head request.

    apr_table_unset(output_filter->r->headers_out, "Content-Length");
  }
  else
  {
    // The first bucket is not an EOS bucket.
  }

  // Check if  the output filter context  is empty.  If  it is empty we  neet do
  // initialize it.

  if (NULL == output_filter->ctx)
  {
    // The output filter context is empty

    // Get pointer to the output filter context.

    //ecl_output_filter_context_t * ecl_output_filter_context = NULL;
    ecl_output_filter_context = (ecl_output_filter_context_t *) output_filter->ctx;

    // Check if we have the output filter context.

    if (NULL == ecl_output_filter_context)
    {
      // We have not the output filter context.

      // Return status code.

      //return (ap_status);
    }
    else
    {
      // We have the output filter context.
    }

    // Initalize the coutput filter context.

    ap_status = ecl_output_filter_initalize(output_filter);

    // Check if we have initialized the output filter context.

    if (OK == ap_status)
    {
      // We have initialized the output filter context.
    }
    else
    {
      // We have not initialized the output filter context.

      // Return status code.

      return (ap_status);
    }

    // Initalize the coutput filter context.

    ecl_output_filter_context = output_filter->ctx;
  }
  else
  {
    // The output filter is not empty
  }

  //
  // Check Apache HTTP Server environment.
  //

  // Check  if the  request  is a  initial  request.   We do  not  handle not  a
  // subrequest.

  if (ap_is_initial_req(output_filter->r))
  {
    // The request is a initial request.
  }
  else
  {
    // The request is not a initial request.

    // Remove the output filter from the request.

    ap_remove_output_filter(output_filter);

    // Pass the output brigade down to the next filter on the filter stack.

    apr_status = ap_pass_brigade(output_filter->next, output_brigade);

    // Check if  we have  passed the output  brigade to the  next filter  on the
    // filter stack.

    if (APR_SUCCESS == apr_status)
    {
      // We have  passed the  output brigade  to the next  filter on  the filter
      // stack.
    }
    else
    {
      // We have not passed the output brigade  to the next filter on the filter
      // stack.

      // Return status code.

      return (ap_status);
    }

    // Empty and destroy the entire bucket brigade.

    apr_status = apr_brigade_cleanup(output_brigade);

    // Check if we have emptied and destroyed the entire bucket brigade.

    if (APR_SUCCESS == apr_status)
    {
      // We have emptied and destroyed the entire bucket brigade.
    }
    else
    {
      // We have not emptied and destroyed the entire bucket brigade.

      // Return status code.

      return (ap_status);
    }
  }

  //
  // Process the output brigade buckets of the output brigade.
  //



// dummy value, we can remove this later
char * output_begin_string = "( output begin )";
apr_bucket * output_begin_bucket = apr_bucket_transient_create(output_begin_string, strlen(output_begin_string), output_filter->r->connection->bucket_alloc);

// dummy value, we can remove this later
char * output_end_string = "( output end )";
apr_bucket * output_end_bucket = apr_bucket_transient_create(output_end_string, strlen(output_end_string), output_filter->r->connection->bucket_alloc);



  // Loop through the output brigade buckets.
  //
  // * Remove the output brigade buckets from from output brigade till the first
  //   EOS bucket.
  // * Fill the output filter context brigade with either
  //   * a copy of the output brigade buckets or
  //   * read the data  contained in the output brigade bucket  and create a new
  //     bucket.

  apr_bucket * current_bucket = APR_BRIGADE_FIRST(output_brigade);
  apr_bucket * new_bucket = NULL;
  apr_bucket * flush_bucket = NULL;
  typedef enum {copy, read, undefined} action_t;
  action_t action = undefined;
  const char * data = NULL;
  apr_size_t length = 0;
  int mode = APR_NONBLOCK_READ;
  while (current_bucket != APR_BRIGADE_SENTINEL(output_brigade))
  {
    // Loop through the output brigade buckets.

    // Check if the  current bucket is an  EOS bucket.  If the bucket  is an EOS
    // bucket we break the loop.

    if (APR_BUCKET_IS_EOS(current_bucket))
    {
      // The current bucket is an EOS bucket.

      // Break the loop.

      break;
    }
    else
    {
      // The current bucket is not an EOS bucket.
    }



// dummy value, we can remove this later
APR_BRIGADE_INSERT_TAIL(ecl_output_filter_context->brigade, output_begin_bucket);
apr_status = ap_pass_brigade(output_filter->next, ecl_output_filter_context->brigade);
apr_status = apr_brigade_cleanup(ecl_output_filter_context->brigade);



    // See what action to perform.

    if      (AP_BUCKET_IS_ERROR(current_bucket))      {action = copy;}
    else if (AP_BUCKET_IS_EOC(current_bucket))        {action = copy;}
    else if (AP_BUCKET_IS_EOR(current_bucket))        {action = copy;}
    else if (APR_BUCKET_IS_EOS(current_bucket))       {action = copy;}
    else if (APR_BUCKET_IS_FILE(current_bucket))      {action = read;}
    else if (APR_BUCKET_IS_FLUSH(current_bucket))     {action = copy;}
    else if (APR_BUCKET_IS_HEAP(current_bucket))      {action = read;}
    else if (APR_BUCKET_IS_IMMORTAL(current_bucket))  {action = read;}
    else if (APR_BUCKET_IS_METADATA(current_bucket))  {action = copy;}
    else if (APR_BUCKET_IS_MMAP(current_bucket))      {action = read;}
    else if (APR_BUCKET_IS_PIPE(current_bucket))      {action = read;}
    else if (APR_BUCKET_IS_POOL(current_bucket))      {action = read;}
    else if (APR_BUCKET_IS_SOCKET(current_bucket))    {action = read;}
    else if (APR_BUCKET_IS_TRANSIENT(current_bucket)) {action = read;}
    else                                              {action = undefined;}

    // Check if the action to perform is set.

    if (undefined == action)
    {
      // The action to perform is not set.

      // Return status code.

      return (ap_status);
    }
    else
    {
      // The action to perform is set.
    }

    // Perform the selected action.

    if   (copy == action)
    {
      // Make a copy of the current output bucket.

      apr_status = apr_bucket_copy(current_bucket, & new_bucket);

      // Check if we have a copy of the current output bucket.

      if (APR_SUCCESS == apr_status)
      {
	// We have a copy of the current output bucket.
      }
      else
      {
	// We have not a copy of the current output bucket.

	// Return status code.

	return (ap_status);
      }
    }
    else if (read == action)
    {
      // Read the data contained in the current output bucket and create an new
      // bucket.  First try to read non-blocking.  If this fails read blocking.

      while (TRUE)
      {
        // Read the data contained in the  current output bucket.  We might read
	// the data more than once.

	// Read the data contained in the current output bucket.

        apr_status = apr_bucket_read(current_bucket, & data, & length, mode);

        // Check if  we need  to read  again and if  we need  to create  a flush
        // bucket.

        if (apr_status == APR_EAGAIN && mode == APR_NONBLOCK_READ)
	{
          // We need to read again and if we need to create a flush bucket.

          // Create a flush bucket.

	  flush_bucket = apr_bucket_flush_create(output_filter->r->connection->bucket_alloc);

          // Check if we have created a flush bucket.

	  if (NULL == flush_bucket)
	  {
	    // We have not created a flush bucket.

	    // Return status code.

	    return (ap_status);
	  }
	  else
	  {
	    // We have created a flush bucket.
	  }

          // Insert the  flush bucket at the  end of the output  brigade context
          // bucket.

          APR_BRIGADE_INSERT_TAIL(ecl_output_filter_context->brigade, flush_bucket);

          // Pass the output brigade down to the next filter on the filter stack.

          apr_status = ap_pass_brigade(output_filter->next, ecl_output_filter_context->brigade);

	   // Check if we  have passed the output brigade to  the next filter on
           // the filter stack.

           if (APR_SUCCESS == apr_status)
           {
             // We have  passed the  output brigade  to the  next filter  on the
             // filter stack.
           }
           else
           {
             // We have not passed the output  brigade to the next filter on the
             // filter stack.

             // Return status code.

             return (ap_status);
           }

           // Empty and destroy the entire bucket brigade.

           apr_status = apr_brigade_cleanup(ecl_output_filter_context->brigade);

           // Check if we have emptied and destroyed the entire bucket brigade.

           if (APR_SUCCESS == apr_status)
           {
             // We have emptied and destroyed the entire bucket brigade.
           }
           else
           {
             // We have not emptied and destroyed the entire bucket brigade.

	     // Return status codes.

	     return (ap_status);
           }

           // Next time read blocking.

           mode = APR_BLOCK_READ;

	   // Continue the loop.

           continue;
        }

        // Next time read non blocking.

        mode = APR_NONBLOCK_READ;

	// Check if we have read the data contained in the current output bucket.

        if (apr_status == APR_SUCCESS)
        {
          // We have read the data contained in the current output bucket.

	  // Break the loop.

          break;
        }
        else
        {
	  // We have not read the data contained in the current output bucket.
        }
      }

      // Create a new bucket.

      if      (AP_BUCKET_IS_ERROR(current_bucket))      {new_bucket = NULL;} //temp_bucket = ap_bucket_error_create(error, buffer, filter->r->pool, filter->r->connection->bucket_alloc);}
      else if (AP_BUCKET_IS_EOC(current_bucket))        {new_bucket = NULL;} //temp_bucket = ap_bucket_eoc_create(filter->r->connection->bucket_alloc);}
      else if (AP_BUCKET_IS_EOR(current_bucket))        {new_bucket = NULL;} //temp_bucket = ap_bucket_eor_create(filter->r->connection->bucket_alloc, filter->r)}
      else if (APR_BUCKET_IS_EOS(current_bucket))       {new_bucket = NULL;} //temp_bucket = apr_bucket_eos_create(filter->r->connection->bucket_alloc);}
      else if (APR_BUCKET_IS_FILE(current_bucket))      {new_bucket = NULL;} //temp_bucket = file_create(file, offset, length, filter->r->pool, filter->r->connection->bucket_alloc):}
      else if (APR_BUCKET_IS_FLUSH(current_bucket))     {new_bucket = NULL;} //temp_bucket = apr_bucket_flush_create(filter->r->connection->bucket_alloc);}
      else if (APR_BUCKET_IS_HEAP(current_bucket))      {new_bucket = apr_bucket_heap_create(data, length, heap_free, output_filter->r->connection->bucket_alloc);}
      else if (APR_BUCKET_IS_IMMORTAL(current_bucket))  {new_bucket = NULL;} //temp_bucket = apr_bucket_immortal_create(data, length, filter->r->connection->bucket_alloc);}
      else if (APR_BUCKET_IS_METADATA(current_bucket))  {new_bucket = NULL;} //temp_bucket = NULL;}
      else if (APR_BUCKET_IS_MMAP(current_bucket))      {new_bucket = NULL;} //temp_bucket = apr_bucket_mmap_create(mmap, start, offset, filter->r->connection->bucket_alloc);}
      else if (APR_BUCKET_IS_PIPE(current_bucket))      {new_bucket = NULL;} //temp_bucket = apr_bucket_pipe_create(pipe, filter->r->connection->bucket_alloc);}
      else if (APR_BUCKET_IS_POOL(current_bucket))      {new_bucket = NULL;} //temp_bucket = apr_bucket_pool_create(data, length, filter->r->pool, filter->r->connection->bucket_alloc);}
      else if (APR_BUCKET_IS_SOCKET(current_bucket))    {new_bucket = NULL;} //temp_bucket = apr_bucket_socket_create(socket, filter->r->connection->bucket_alloc);}
      else if (APR_BUCKET_IS_TRANSIENT(current_bucket)) {new_bucket = NULL;} //temp_bucket = apr_bucket_transient_create(data, length, filter->r->connection->bucket_alloc);}
      else                                              {new_bucket = NULL;} //temp_bucket = NULL;}

      // Check if we have created a new bucket.

      if (NULL == new_bucket)
      {
	// We have not created a new bucket.

	return (ap_status);
      }
      else
      {
	// We have created a new bucket.
      }
    }
    else
    {
      // No action to perform is set.

      // Return status code.

      return (ap_status);
    }



// dummy value, we can remove this later
APR_BRIGADE_INSERT_TAIL(ecl_output_filter_context->brigade, new_bucket);
apr_status = ap_pass_brigade(output_filter->next, ecl_output_filter_context->brigade);
if (APR_SUCCESS == apr_status) {} else {return (ap_status);}



    // Empty and destroy the entire bucket brigade.

    apr_status = apr_brigade_cleanup(ecl_output_filter_context->brigade);

    // Check if we have emptied and destroyed the entire bucket brigade.

    if (APR_SUCCESS == apr_status)
    {
      // We have emptied and destroyed the entire bucket brigade.
    }
    else
    {
      // We have not emptied and destroyed the entire bucket brigade.

      // Return status code.

      return (ap_status);
    }



// dummy value, we can remove this later
APR_BRIGADE_INSERT_TAIL(ecl_output_filter_context->brigade, output_end_bucket);
apr_status = ap_pass_brigade(output_filter->next, ecl_output_filter_context->brigade);
apr_status = apr_brigade_cleanup(ecl_output_filter_context->brigade);



    // Delete the  current bucket from the  output brigade and then  destroy it.
    // (apr_bucket_delete() before APR_BRIGADE_FIRST())

    apr_bucket_delete(current_bucket);

    // Get the current bucket for the  next iteratio (the now first bucket) from
    // the output brigade.

    current_bucket = APR_BRIGADE_FIRST(output_brigade);

    // Check if we have the current bucket.

    if (NULL == current_bucket)
    {
      // We have not the current bucket.

      return (ap_status);
    }
    else
    {
      // We have the current bucket.
    }
  }

  // Create an EOS bucket.

  apr_bucket * eos_bucket = NULL;
  eos_bucket = apr_bucket_eos_create(output_filter->r->connection->bucket_alloc);

  // Check if we have created an EOS bucket.

  if (NULL == eos_bucket)
  {
    // We have not an EOS bucket.

    // Return status code.

    return (ap_status);
  }
  else
  {
    // We have an EOS bucket.
  }

  // Add EOS bucket ad the end of the output filter context brigade.

  APR_BRIGADE_INSERT_TAIL(ecl_output_filter_context->brigade, eos_bucket);

  // Pass the output brigade down to the next filter on the filter stack.

  apr_status = ap_pass_brigade(output_filter->next, ecl_output_filter_context->brigade);

  // Check if we have passed the output brigade to the next filter on the filter
  // stack.

  if (APR_SUCCESS == apr_status)
  {
    // We have passed the output brigade to the next filter on the filter stack.
  }
  else
  {
    // We have not  passed the output brigade  to the next filter  on the filter
    // stack.

    // Return status code.

    return (ap_status);
  }

  // Empty and destroy the entire bucket brigade.

  apr_status = apr_brigade_cleanup(ecl_output_filter_context->brigade);

  // Check if we have emptied and destroyed the entire bucket brigade.

  if (APR_SUCCESS == apr_status)
  {
    // We have emptied and destroyed the entire bucket brigade.
  }
  else
  {
    // We have not emptied and destroyed the entire bucket brigade.

    // Return status code.

    return (ap_status);
  }

  //
  // Clean up.
  //

  // If the output filter context brigade is empty we will add an EOS bucket.

  if (APR_BRIGADE_EMPTY(ecl_output_filter_context->brigade))
  {
    // The output filter context brigade is empty.

    // Add an EOS bucket.

    APR_BRIGADE_INSERT_HEAD(ecl_output_filter_context->brigade, eos_bucket);
  }
  else
  {
    // The output filter context brigade is not empty.
  }

  //  If the output brigade is empty we will add an EOS bucket.

  if (APR_BRIGADE_EMPTY(output_brigade))
  {
    // The output brigade is empty.

    APR_BRIGADE_INSERT_HEAD(output_brigade, eos_bucket);
  }
  else
  {
    // The output brigade is not empty.
  }

  // Pass  the output  filter context  brigade down  to the  next filter  on the
  // filter stack.

  apr_status = ap_pass_brigade(output_filter->next, ecl_output_filter_context->brigade);

  // Check if we have passed the output brigade to the next filter on the filter
  // stack.

  if (APR_SUCCESS == apr_status)
  {
    // We have passed the output brigade to the next filter on the filter stack.
  }
  else
  {
    // We have not  passed the output brigade  to the next filter  on the filter
    // stack.

    // Return status code.

    return (ap_status);
  }

  // Pass the output brigade.

  apr_status = ap_pass_brigade(output_filter->next, output_brigade);

  // Check if we have passed the output brigade.

  if (APR_SUCCESS == apr_status)
  {
    // We have passed the output brigade.
  }
  else
  {
    // We have not passed the output brigade.

    // Return status code.

    return (ap_status);
  }

  // Empty and destroy the entire bucket brigade.

  apr_status = apr_brigade_cleanup(ecl_output_filter_context->brigade);

  // Check if we have emptied and destroyed the entire bucket brigade.

  if (APR_SUCCESS == apr_status)
  {
    // We have emptied and destroyed the entire bucket brigade.
  }
  else
  {
    // We have not emptied and destroyed the entire bucket brigade.

    // Return status code.

    return (ap_status);
  }

  // Empty and destroy the entire bucket brigade.

  apr_status = apr_brigade_cleanup(output_brigade);

    // Check if we have emptied and destroyed the entire bucket brigade.

  if (APR_SUCCESS == apr_status)
  {
    // We have emptied and destroyed the entire bucket brigade.
  }
  else
  {
    // We have not emptied and destroyed the entire bucket brigade.

    // Return status code.

    return (ap_status);
  }

  //
  // We are done.
  //

  // Return status code.

  ap_status = OK;
  return ap_status;
}

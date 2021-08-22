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
 * @brief Functions to handle input output filter.
 *
 * @details
 */



#ifndef IO_FILTER_H
#define IO_FILTER_H

typedef struct
{
  int dummy; // dummy value, we can remove this later
  apr_bucket_brigade * brigade;
} ecl_input_filter_context_t;
extern ecl_input_filter_context_t * ecl_input_filter_context;
int ecl_input_filter_initalize(ap_filter_t * input_filter);
int ecl_input_filter_hander(ap_filter_t * input_filter, apr_bucket_brigade * input_brigade, ap_input_mode_t mode, apr_read_type_e block, apr_off_t readbytes);

typedef struct
{
  int dummy; // dummy value, we can remove this later
  apr_bucket_brigade * brigade;
} ecl_output_filter_context_t;
extern ecl_output_filter_context_t * ecl_output_filter_context;
int ecl_output_filter_initalize(ap_filter_t * output_filter);
int ecl_output_filter_hander(ap_filter_t * output_filter, apr_bucket_brigade * output_brigade);

#endif

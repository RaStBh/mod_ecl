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
 * @brief Boolean values.
 *
 * @see [apr_general.h File Reference](https://apr.apache.org/docs/apr/1.7/apr__general_8h.html)
 * @see [apr_general.h Source](https://apr.apache.org/docs/apr/1.7/apr__general_8h_source.html)
 *
 * @details
 */



//==============================================================================
//
// Headers to include.
//
//==============================================================================



#include "boolean.h"



//==============================================================================
//
// The boolean values.
//
//==============================================================================



/**
 * @brief Boolean value for false.
 *
 * @details
 *
 * @see [apr_general.h File Reference](https://apr.apache.org/docs/apr/1.7/apr__general_8h.html)
 * @see [apr_general.h Source](https://apr.apache.org/docs/apr/1.7/apr__general_8h_source.html)
 */

const int FALSE = 0;



/**
 * @brief Boolean value for true.
 *
 * @details
 *
 * @see [apr_general.h File Reference](https://apr.apache.org/docs/apr/1.7/apr__general_8h.html)
 * @see [apr_general.h Source](https://apr.apache.org/docs/apr/1.7/apr__general_8h_source.html)
 */

const int TRUE = 1;

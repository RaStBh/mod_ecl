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
 * @brief This file contains the core functionality of mod_ecl.
 */



#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

/* The sample content handler */
static int ecl_handler(request_rec *r)
{
    if (strcmp(r->handler, "ecl")) {
        return DECLINED;
    }
    r->content_type = "text/html";      

    if (!r->header_only)
        ap_rputs("The sample page from mod_ecl.c\n", r);
    return OK;
}

static void ecl_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(ecl_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA ecl_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    ecl_register_hooks     /* register hooks                      */
};

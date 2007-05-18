/* guile-cairo
 * Copyright (C) 2007 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo.h:
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *                                                                  
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *                                                                  
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, contact:
 *
 * Free Software Foundation, Inc.     Voice:  +1-617-542-5942
 * 51 Franklin St, Fifth Floor        Fax:    +1-617-542-2652
 * Boston, MA  02110-1301,  USA       gnu@gnu.org
 */

#ifndef __GUILE_CAIRO_H__
#define __GUILE_CAIRO_H__

#include <guile-cairo-smob-types.h>
#include <guile-cairo-enum-types.h>
#include <guile-cairo-vector-types.h>



void scm_c_check_cairo_status (cairo_status_t status, const char *subr);



void scm_init_cairo (void);

#endif

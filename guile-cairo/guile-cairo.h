/* guile-cairo
 * Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo.h:
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *                                                                  
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *                                                                  
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef __GUILE_CAIRO_H__
#define __GUILE_CAIRO_H__

#include <cairo.h>
#include <libguile.h>

#include <guile-cairo-smob-types.h>
#include <guile-cairo-enum-types.h>
#include <guile-cairo-vector-types.h>



void scm_c_check_cairo_status (cairo_status_t status, const char *subr);



void scm_init_cairo (void);

#endif

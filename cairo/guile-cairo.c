/* guile-cairo
 * Copyright (C) 2007 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo.c: Cairo for Guile
 *
 * This program is free software; you can redistribute it and/or    
 * modify it under the terms of the GNU General Public License as   
 * published by the Free Software Foundation; either version 2 of   
 * the License, or (at your option) any later version.              
 *                                                                  
 * This program is distributed in the hope that it will be useful,  
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    
 * GNU General Public License for more details.                     
 *                                                                  
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, contact:
 *
 * Free Software Foundation           Voice:  +1-617-542-5942
 * 59 Temple Place - Suite 330        Fax:    +1-617-542-2652
 * Boston, MA  02111-1307,  USA       gnu@gnu.org
 */

#include <libguile.h>

#include <cairo.h>

SCM_DEFINE (scm_cairo_version, "cairo-version", 0, 0, 0,
	    (void),
	    "Retrieves the version of the cairo library.")
#define FUNC_NAME s_scm_cairo_version
{
    return scm_from_int (cairo_version ());
}
#undef FUNC_NAME



void
scm_init_cairo (void)
{
#ifndef SCM_MAGIC_SNARFER
#include "guile-cairo.x"
#endif

    scm_c_export (NULL);
}

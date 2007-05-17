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

#include "guile-cairo.h"

void
scm_c_check_cairo_status (cairo_status_t status, const char *subr)
{
    if (status == CAIRO_STATUS_SUCCESS)
        return;

    scm_error (scm_from_locale_symbol ("cairo-error"),
               subr,
               cairo_status_to_string (status),
               scm_list_1 (scm_from_cairo_status (status)),
               scm_list_1 (scm_from_int (status)));
}

SCM_DEFINE (scm_cairo_version, "cairo-version", 0, 0, 0,
	    (void),
	    "Retrieves the version of the cairo library.")
#define FUNC_NAME s_scm_cairo_version
{
    return scm_from_int (cairo_version ());
}
#undef FUNC_NAME

static SCM make_point (double x, double y) 
{
    return scm_f64vector (scm_list_2 (scm_from_double (x),
                                      scm_from_double (y)));
}

SCM_DEFINE (scm_cairo_path_fold, "cairo-path-fold", 3, 0, 0,
	    (SCM spath, SCM proc, SCM init),
	    "Fold over a cairo path.")
#define FUNC_NAME s_scm_cairo_path_fold
{
    cairo_path_t *path;
    int i, j, len;
    SCM ret = init;
    cairo_path_data_t *data;
    cairo_path_data_type_t type;
    SCM args, tail;

    path = scm_to_cairo_path (spath);

    for (i = 0; i < path->num_data; /* i incremented below */) {
        type = path->data[i].header.type;
        len = path->data[i].header.length;
        args = scm_cons (scm_from_cairo_path_data_type (type), SCM_EOL);

        for (j = 0, i++, tail = args; j < len; j++, i++, tail = scm_cdr (tail)) {
            data = &path->data[i];
            scm_set_cdr_x (tail, scm_cons (make_point (data->point.x,
                                                       data->point.y),
                                           SCM_EOL));
        }
            
        ret = scm_call_2 (proc, args, ret);
    }
    
    return ret;
}
#undef FUNC_NAME



void
scm_init_cairo (void)
{
#ifndef SCM_MAGIC_SNARFER
#include "guile-cairo.x"
#endif

    scm_init_cairo_smob_types ();
    scm_init_cairo_vector_types ();
    scm_init_cairo_enum_types ();
    scm_c_export (NULL);
}

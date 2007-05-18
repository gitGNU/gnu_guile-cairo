/* guile-cairo
 * Copyright (C) 2007 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-vector-types.c: Cairo for Guile
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

#include "guile-cairo-vector-types.h"


SCM_SYMBOL (sym_f64, "f64");


/**********************************************************************
 * cairo_matrix_t
 **********************************************************************/

SCM
scm_from_cairo_matrix (cairo_matrix_t *matrix)
{
    SCM ret;
    double *area;
    scm_t_array_handle handle;

    ret = scm_make_typed_array (sym_f64, scm_from_double (0.0),
                                scm_list_2 (scm_from_int (3),
                                            scm_from_int (3)));
    scm_array_get_handle (ret, &handle);
    
    area = scm_array_handle_f64_writable_elements (&handle);
    area[0] = matrix->xx;
    area[1] = matrix->xy;
    area[2] = matrix->x0;
    area[3] = matrix->yx;
    area[4] = matrix->yy;
    area[5] = matrix->y0;
    area[8] = 1.0;

    scm_array_handle_release (&handle);

    return ret;
}

void
scm_fill_cairo_matrix (SCM scm, cairo_matrix_t *matrix)
{
    ssize_t i;
    const double *area;
    scm_t_array_handle handle;

    scm_array_get_handle (scm, &handle);
    
    area = scm_array_handle_f64_elements (&handle);

    i = scm_array_handle_pos (&handle, scm_list_2 (scm_from_int(0),
                                                   scm_from_int(0)));
    matrix->xx = area[i];
    i = scm_array_handle_pos (&handle, scm_list_2 (scm_from_int(0),
                                                   scm_from_int(1)));
    matrix->xy = area[i];
    i = scm_array_handle_pos (&handle, scm_list_2 (scm_from_int(0),
                                                   scm_from_int(2)));
    matrix->x0 = area[i];
    i = scm_array_handle_pos (&handle, scm_list_2 (scm_from_int(1),
                                                   scm_from_int(0)));
    matrix->yx = area[i];
    i = scm_array_handle_pos (&handle, scm_list_2 (scm_from_int(1),
                                                   scm_from_int(1)));
    matrix->yy = area[i];
    i = scm_array_handle_pos (&handle, scm_list_2 (scm_from_int(1),
                                                   scm_from_int(2)));
    matrix->y0 = area[i];

    scm_array_handle_release (&handle);
}

/**********************************************************************
 * cairo_rectangle_t
 **********************************************************************/

SCM
scm_from_cairo_rectangle (cairo_rectangle_t *rect)
{
    return scm_f64vector(scm_list_n(scm_from_double (rect->x),
                                    scm_from_double (rect->y),
                                    scm_from_double (rect->width),
                                    scm_from_double (rect->height),
                                    SCM_UNDEFINED));
}

void
scm_fill_cairo_rectangle (SCM scm, cairo_rectangle_t *rect)
{
#define GET(v,i) \
    scm_to_double (scm_vector_ref (v, scm_from_int (i)))

    rect->x = GET (scm, 0);
    rect->y = GET (scm, 1);
    rect->width = GET (scm, 2);
    rect->height = GET (scm, 3);
#undef GET
}

/**********************************************************************
 * cairo_glyph_t
 **********************************************************************/

SCM
scm_from_cairo_glyph (cairo_glyph_t *glyph)
{
    return scm_vector(scm_list_n(scm_from_int (glyph->index),
                                 scm_from_double (glyph->x),
                                 scm_from_double (glyph->y),
                                 SCM_UNDEFINED));
}

void
scm_fill_cairo_glyph (SCM scm, cairo_glyph_t *glyph)
{
#define GET(func,v,i) \
    func (scm_vector_ref (v, scm_from_int (i)))

    glyph->index = GET (scm_to_int, scm, 0);
    glyph->x = GET (scm_to_double, scm, 1);
    glyph->y = GET (scm_to_double, scm, 2);
#undef GET
}

/**********************************************************************
 * cairo_text_extents_t
 **********************************************************************/

SCM
scm_from_cairo_text_extents (cairo_text_extents_t *text)
{
    return scm_f64vector(scm_list_n(scm_from_double (text->x_bearing),
                                    scm_from_double (text->y_bearing),
                                    scm_from_double (text->width),
                                    scm_from_double (text->height),
                                    scm_from_double (text->x_advance),
                                    scm_from_double (text->y_advance),
                                    SCM_UNDEFINED));
}

void
scm_fill_cairo_text_extents (SCM scm, cairo_text_extents_t *text)
{
#define GET(v,i) \
    scm_to_double (scm_f64vector_ref (v, scm_from_int (i)))

    text->x_bearing = GET (scm, 0);
    text->y_bearing = GET (scm, 1);
    text->width = GET (scm, 2);
    text->height = GET (scm, 3);
    text->x_advance = GET (scm, 4);
    text->y_advance = GET (scm, 5);
#undef GET
}

/**********************************************************************
 * cairo_font_extents_t
 **********************************************************************/

SCM
scm_from_cairo_font_extents (cairo_font_extents_t *fext)
{
    return scm_f64vector(scm_list_n(scm_from_double (fext->ascent),
                                    scm_from_double (fext->descent),
                                    scm_from_double (fext->height),
                                    scm_from_double (fext->max_x_advance),
                                    scm_from_double (fext->max_y_advance),
                                    SCM_UNDEFINED));
}

void
scm_fill_cairo_font_extents (SCM scm, cairo_font_extents_t *fext)
{
#define GET(v,i) \
    scm_to_double (scm_f64vector_ref (v, scm_from_int (i)))

    fext->ascent = GET (scm, 0);
    fext->descent = GET (scm, 1);
    fext->height = GET (scm, 2);
    fext->max_x_advance = GET (scm, 3);
    fext->max_y_advance = GET (scm, 4);
#undef GET
}

void
scm_init_cairo_vector_types (void)
{
#ifndef SCM_MAGIC_SNARFER
#include "guile-cairo-vector-types.x"
#endif
}

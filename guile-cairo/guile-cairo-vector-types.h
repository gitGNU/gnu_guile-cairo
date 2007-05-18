/* guile-cairo
 * Copyright (C) 2007 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-vector-types.h:
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

#ifndef __GUILE_CAIRO_VECTOR_TYPES_H__
#define __GUILE_CAIRO_VECTOR_TYPES_H__



/* FIXME: add scheme-level accessors and constructors */

SCM scm_from_cairo_matrix (cairo_matrix_t *matrix);
void scm_fill_cairo_matrix (SCM scm, cairo_matrix_t *matrix);

SCM scm_from_cairo_rectangle (cairo_rectangle_t *rect);
void scm_fill_cairo_rectangle (SCM scm, cairo_rectangle_t *rect);

SCM scm_from_cairo_glyph (cairo_glyph_t *glyph);
void scm_fill_cairo_glyph (SCM scm, cairo_glyph_t *glyph);

SCM scm_from_cairo_text_extents (cairo_text_extents_t *text);
void scm_fill_cairo_text_extents (SCM scm, cairo_text_extents_t *text);

SCM scm_from_cairo_font_extents (cairo_font_extents_t *fext);
void scm_fill_cairo_font_extents (SCM scm, cairo_font_extents_t *fext);



void scm_init_cairo_vector_types (void);

#endif

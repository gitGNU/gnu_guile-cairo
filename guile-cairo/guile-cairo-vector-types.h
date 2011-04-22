/* guile-cairo
 * Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-vector-types.h:
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

#ifndef __GUILE_CAIRO_VECTOR_TYPES_H__
#define __GUILE_CAIRO_VECTOR_TYPES_H__



/* FIXME: add scheme-level accessors and constructors */

SCM scm_from_cairo_matrix (cairo_matrix_t *matrix);
void scm_fill_cairo_matrix (SCM scm, cairo_matrix_t *matrix);

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
SCM scm_from_cairo_rectangle (cairo_rectangle_t *rect);
void scm_fill_cairo_rectangle (SCM scm, cairo_rectangle_t *rect);
#endif

SCM scm_from_cairo_glyph (cairo_glyph_t *glyph);
void scm_fill_cairo_glyph (SCM scm, cairo_glyph_t *glyph);

SCM scm_from_cairo_text_extents (cairo_text_extents_t *text);
void scm_fill_cairo_text_extents (SCM scm, cairo_text_extents_t *text);

SCM scm_from_cairo_font_extents (cairo_font_extents_t *fext);
void scm_fill_cairo_font_extents (SCM scm, cairo_font_extents_t *fext);

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,8,0)
/* Always returns forward cluster ordering. */
SCM scm_from_cairo_text_clusters (SCM str, cairo_text_cluster_t *clusters,
                                  int n_clusters,
                                  cairo_text_cluster_flags_t flags);
/* Always fills forward cluster ordering. */
void scm_fill_cairo_text_clusters (SCM str, SCM scm,
                                   cairo_text_cluster_t *clusters);
#endif



void scm_init_cairo_vector_types (void);

#endif

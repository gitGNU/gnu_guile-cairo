/* guile-cairo
 * Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-smob-types.h:
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

#ifndef __GUILE_CAIRO_SMOB_TYPES_H__
#define __GUILE_CAIRO_SMOB_TYPES_H__



SCM scm_take_cairo (cairo_t *ctx);
SCM scm_from_cairo (cairo_t *ctx);
cairo_t* scm_to_cairo (SCM scm);

SCM scm_take_cairo_surface (cairo_surface_t *surf);
SCM scm_from_cairo_surface (cairo_surface_t *surf);
cairo_surface_t* scm_to_cairo_surface (SCM scm);

SCM scm_take_cairo_pattern (cairo_pattern_t *pat);
SCM scm_from_cairo_pattern (cairo_pattern_t *pat);
cairo_pattern_t* scm_to_cairo_pattern (SCM scm);

SCM scm_take_cairo_font_face (cairo_font_face_t *pat);
SCM scm_from_cairo_font_face (cairo_font_face_t *pat);
cairo_font_face_t* scm_to_cairo_font_face (SCM scm);

SCM scm_take_cairo_scaled_font (cairo_scaled_font_t *pat);
SCM scm_from_cairo_scaled_font (cairo_scaled_font_t *pat);
cairo_scaled_font_t* scm_to_cairo_scaled_font (SCM scm);

SCM scm_take_cairo_font_options (cairo_font_options_t *fopt);
cairo_font_options_t* scm_to_cairo_font_options (SCM scm);

SCM scm_take_cairo_path (cairo_path_t *path);
cairo_path_t* scm_to_cairo_path (SCM scm);

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
SCM scm_take_cairo_device (cairo_device_t *dev);
SCM scm_from_cairo_device (cairo_device_t *dev);
cairo_device_t* scm_to_cairo_device (SCM scm);

SCM scm_take_cairo_region (cairo_region_t *reg);
SCM scm_from_cairo_region (cairo_region_t *reg);
cairo_region_t* scm_to_cairo_region (SCM scm);
#endif /* 1.10 */



void scm_init_cairo_smob_types (void);

#endif

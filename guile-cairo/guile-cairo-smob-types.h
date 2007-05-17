/* guile-cairo
 * Copyright (C) 2007 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-smob-types.h:
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

#ifndef __GUILE_CAIRO_SMOB_TYPES_H__
#define __GUILE_CAIRO_SMOB_TYPES_H__



SCM scm_from_cairo (cairo_t *ctx);
cairo_t* scm_to_cairo (SCM scm);

SCM scm_from_cairo_surface (cairo_surface_t *surf);
cairo_surface_t* scm_to_cairo_surface (SCM scm);

SCM scm_from_cairo_pattern (cairo_pattern_t *pat);
cairo_pattern_t* scm_to_cairo_pattern (SCM scm);

SCM scm_from_cairo_font_face (cairo_font_face_t *pat);
cairo_font_face_t* scm_to_cairo_font_face (SCM scm);

SCM scm_from_cairo_scaled_font (cairo_scaled_font_t *pat);
cairo_scaled_font_t* scm_to_cairo_scaled_font (SCM scm);

SCM scm_from_cairo_font_options (cairo_font_options_t *fopt);
cairo_font_options_t* scm_to_cairo_font_options (SCM scm);

SCM scm_from_cairo_path (cairo_path_t *path);
cairo_path_t* scm_to_cairo_path (SCM scm);



void scm_init_cairo_smob_types (void);

#endif

/* guile-cairo
 * Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-enum-types.h:
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

#ifndef __GUILE_CAIRO_ENUM_TYPES_H__
#define __GUILE_CAIRO_ENUM_TYPES_H__

#if CAIRO_HAS_SVG_SURFACE
#include <cairo-svg.h>
#endif

#if CAIRO_HAS_PS_SURFACE
#include <cairo-ps.h>
#endif



SCM scm_from_cairo_status (cairo_status_t cval);
cairo_status_t scm_to_cairo_status (SCM scm);

SCM scm_from_cairo_content (cairo_content_t cval);
cairo_content_t scm_to_cairo_content (SCM scm);

SCM scm_from_cairo_operator (cairo_operator_t cval);
cairo_operator_t scm_to_cairo_operator (SCM scm);

SCM scm_from_cairo_antialias (cairo_antialias_t cval);
cairo_antialias_t scm_to_cairo_antialias (SCM scm);

SCM scm_from_cairo_fill_rule (cairo_fill_rule_t cval);
cairo_fill_rule_t scm_to_cairo_fill_rule (SCM scm);

SCM scm_from_cairo_line_cap (cairo_line_cap_t cval);
cairo_line_cap_t scm_to_cairo_line_cap (SCM scm);

SCM scm_from_cairo_line_join (cairo_line_join_t cval);
cairo_line_join_t scm_to_cairo_line_join (SCM scm);

SCM scm_from_cairo_font_slant (cairo_font_slant_t cval);
cairo_font_slant_t scm_to_cairo_font_slant (SCM scm);

SCM scm_from_cairo_font_weight (cairo_font_weight_t cval);
cairo_font_weight_t scm_to_cairo_font_weight (SCM scm);

SCM scm_from_cairo_subpixel_order (cairo_subpixel_order_t cval);
cairo_subpixel_order_t scm_to_cairo_subpixel_order (SCM scm);

SCM scm_from_cairo_hint_style (cairo_hint_style_t cval);
cairo_hint_style_t scm_to_cairo_hint_style (SCM scm);

SCM scm_from_cairo_hint_metrics (cairo_hint_metrics_t cval);
cairo_hint_metrics_t scm_to_cairo_hint_metrics (SCM scm);

SCM scm_from_cairo_font_type (cairo_font_type_t cval);
cairo_font_type_t scm_to_cairo_font_type (SCM scm);

SCM scm_from_cairo_path_data_type (cairo_path_data_type_t cval);
cairo_path_data_type_t scm_to_cairo_path_data_type (SCM scm);

SCM scm_from_cairo_surface_type (cairo_surface_type_t cval);
cairo_surface_type_t scm_to_cairo_surface_type (SCM scm);

SCM scm_from_cairo_format (cairo_format_t cval);
cairo_format_t scm_to_cairo_format (SCM scm);

SCM scm_from_cairo_pattern_type (cairo_pattern_type_t cval);
cairo_pattern_type_t scm_to_cairo_pattern_type (SCM scm);

SCM scm_from_cairo_extend (cairo_extend_t cval);
cairo_extend_t scm_to_cairo_extend (SCM scm);

SCM scm_from_cairo_filter (cairo_filter_t cval);
cairo_filter_t scm_to_cairo_filter (SCM scm);

#if CAIRO_HAS_SVG_SURFACE
SCM scm_from_cairo_svg_version (cairo_svg_version_t cval);
cairo_svg_version_t scm_to_cairo_svg_version (SCM scm);
#endif  /* CAIRO_HAS_SVG_SURFACE */

#if CAIRO_HAS_PS_SURFACE
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,6,0)
SCM scm_from_cairo_ps_level (cairo_ps_level_t cval);
cairo_ps_level_t scm_to_cairo_ps_level (SCM scm);
#endif  /* 1.6 */
#endif  /* CAIRO_HAS_PS_SURFACE */



void scm_init_cairo_enum_types (void);

#endif

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

/* TODO: checking of cairo_status */

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

SCM_DEFINE_PUBLIC (scm_cairo_version, "cairo-version", 0, 0, 0,
	    (void),
	    "Retrieves the version of the cairo library.")
#define FUNC_NAME s_scm_cairo_version
{
    return scm_from_int (cairo_version ());
}
#undef FUNC_NAME

SCM_DEFINE_PUBLIC (scm_cairo_version_string, "cairo-version-string", 0, 0, 0,
	    (void),
	    "Retrieves the version of the cairo library as a string.")
{
    return scm_from_locale_string (cairo_version_string ());
}

SCM_DEFINE_PUBLIC (scm_cairo_create, "cairo-create", 1, 0, 0,
	    (SCM surf),
	    "")
{
    return scm_take_cairo (cairo_create (scm_to_cairo_surface (surf)));
}

#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_get_reference_count, "cairo-get-reference-count", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_uint (cairo_get_reference_count (scm_to_cairo (ctx)));
}
#endif

/* not wrapping the user_data because I need to mail the guile list,
 * possibility of calling gc_unprotect during gc */

SCM_DEFINE_PUBLIC (scm_cairo_save, "cairo-save", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_save (scm_to_cairo (ctx));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_restore, "cairo-restore", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_restore (scm_to_cairo (ctx));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_push_group, "cairo-push-group", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_push_group (scm_to_cairo (ctx));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_push_group_with_context, "cairo-push-group-with-context", 2, 0, 0,
	    (SCM ctx, SCM content),
	    "")
{
    cairo_push_group_with_content (scm_to_cairo (ctx),
                                   scm_to_cairo_content (content));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_pop_group, "cairo-pop-group", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_pop_group (scm_to_cairo (ctx));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_pop_group_to_source, "cairo-pop-group-to-source", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_pop_group_to_source (scm_to_cairo (ctx));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_operator, "cairo-set-operator", 2, 0, 0,
	    (SCM ctx, SCM op),
	    "")
{
    cairo_set_operator (scm_to_cairo (ctx),
                        scm_to_cairo_operator (op));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_source, "cairo-set-source", 2, 0, 0,
	    (SCM ctx, SCM pat),
	    "")
{
    cairo_set_source (scm_to_cairo (ctx),
                      scm_to_cairo_pattern (pat));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_source_rgb, "cairo-set-source-rgb", 4, 0, 0,
	    (SCM ctx, SCM r, SCM g, SCM b),
	    "")
{
    cairo_set_source_rgb (scm_to_cairo (ctx),
                          scm_to_double (r),
                          scm_to_double (g),
                          scm_to_double (b));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_source_rgba, "cairo-set-source-rgb", 5, 0, 0,
	    (SCM ctx, SCM r, SCM g, SCM b, SCM a),
	    "")
{
    cairo_set_source_rgba (scm_to_cairo (ctx),
                           scm_to_double (r),
                           scm_to_double (g),
                           scm_to_double (b),
                           scm_to_double (a));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_source_surface, "cairo-set-source-surface", 4, 0, 0,
	    (SCM ctx, SCM surf, SCM x, SCM y),
	    "")
{
    cairo_set_source_surface (scm_to_cairo (ctx),
                              scm_to_cairo_surface (surf),
                              scm_to_double (x),
                              scm_to_double (y));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_tolerance, "cairo-set-tolerance", 2, 0, 0,
	    (SCM ctx, SCM tolerance),
	    "")
{
    cairo_set_tolerance (scm_to_cairo (ctx),
                         scm_to_double (tolerance));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_antialias, "cairo-set-antialias", 2, 0, 0,
	    (SCM ctx, SCM antialias),
	    "")
{
    cairo_set_antialias (scm_to_cairo (ctx),
                         scm_to_cairo_antialias (antialias));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_fill_rule, "cairo-set-fill-rule", 2, 0, 0,
	    (SCM ctx, SCM fill_rule),
	    "")
{
    cairo_set_fill_rule (scm_to_cairo (ctx),
                         scm_to_cairo_fill_rule (fill_rule));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_line_width, "cairo-set-line-width", 2, 0, 0,
	    (SCM ctx, SCM line_width),
	    "")
{
    cairo_set_line_width (scm_to_cairo (ctx),
                          scm_to_double (line_width));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_line_cap, "cairo-set-line-cap", 2, 0, 0,
	    (SCM ctx, SCM line_cap),
	    "")
{
    cairo_set_line_cap (scm_to_cairo (ctx),
                        scm_to_cairo_line_cap (line_cap));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_line_join, "cairo-set-line-join", 2, 0, 0,
	    (SCM ctx, SCM line_join),
	    "")
{
    cairo_set_line_join (scm_to_cairo (ctx),
                         scm_to_cairo_line_join (line_join));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_dash, "cairo-set-dash", 3, 0, 0,
	    (SCM ctx, SCM sdashes, SCM offset),
	    "")
{
    double *dashes;
    int ndashes;

    scm_dynwind_begin (0);

    ndashes = scm_to_signed_integer (scm_vector_length (sdashes),
                                     0, 1<<27);
    dashes = scm_malloc (ndashes * sizeof(double));
    scm_dynwind_free (dashes);
    
    cairo_set_dash (scm_to_cairo (ctx),
                    dashes, ndashes, scm_to_double (offset));

    scm_dynwind_end ();
    
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_miter_limit, "cairo-set-miter-limit", 2, 0, 0,
	    (SCM ctx, SCM limit),
	    "")
{
    cairo_set_miter_limit (scm_to_cairo (ctx),
                           scm_to_double (limit));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_translate, "cairo-translate", 3, 0, 0,
	    (SCM ctx, SCM tx, SCM ty),
	    "")
{
    cairo_translate (scm_to_cairo (ctx),
                     scm_to_double (tx), scm_to_double (ty));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_scale, "cairo-scale", 3, 0, 0,
	    (SCM ctx, SCM sx, SCM sy),
	    "")
{
    cairo_scale (scm_to_cairo (ctx),
                 scm_to_double (sx), scm_to_double (sy));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_rotate, "cairo-rotate", 2, 0, 0,
	    (SCM ctx, SCM radians),
	    "")
{
    cairo_rotate (scm_to_cairo (ctx),
                  scm_to_double (radians));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_transform, "cairo-transform", 2, 0, 0,
	    (SCM ctx, SCM smatrix),
	    "")
{
    cairo_matrix_t matrix;
    scm_fill_cairo_matrix (smatrix, &matrix);
    
    cairo_transform (scm_to_cairo (ctx), &matrix);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_matrix, "cairo-set-matrix", 2, 0, 0,
	    (SCM ctx, SCM smatrix),
	    "")
{
    cairo_matrix_t matrix;
    scm_fill_cairo_matrix (smatrix, &matrix);
    
    cairo_set_matrix (scm_to_cairo (ctx), &matrix);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_identity_matrix, "cairo-identity-matrix", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_identity_matrix (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_user_to_device, "cairo-user-to-device", 3, 0, 0,
	    (SCM ctx, SCM sx, SCM sy),
	    "")
{
    double x, y;
    
    x = scm_to_double (sx);
    y = scm_to_double (sy);

    cairo_user_to_device (scm_to_cairo (ctx), &x, &y);

    return scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y)));
}

SCM_DEFINE_PUBLIC (scm_cairo_user_to_device_distance, "cairo-user-to-device-distance", 3, 0, 0,
	    (SCM ctx, SCM sx, SCM sy),
	    "")
{
    double x, y;
    
    x = scm_to_double (sx);
    y = scm_to_double (sy);

    cairo_user_to_device_distance (scm_to_cairo (ctx), &x, &y);

    return scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y)));
}

SCM_DEFINE_PUBLIC (scm_cairo_device_to_user, "cairo-device-to-user", 3, 0, 0,
	    (SCM ctx, SCM sx, SCM sy),
	    "")
{
    double x, y;
    
    x = scm_to_double (sx);
    y = scm_to_double (sy);

    cairo_device_to_user (scm_to_cairo (ctx), &x, &y);

    return scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y)));
}

SCM_DEFINE_PUBLIC (scm_cairo_device_to_user_distance, "cairo-device-to-user-distance", 3, 0, 0,
	    (SCM ctx, SCM sx, SCM sy),
	    "")
{
    double x, y;
    
    x = scm_to_double (sx);
    y = scm_to_double (sy);

    cairo_device_to_user_distance (scm_to_cairo (ctx), &x, &y);

    return scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y)));
}

SCM_DEFINE_PUBLIC (scm_cairo_new_path, "cairo-new-path", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_new_path (scm_to_cairo (ctx));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_move_to, "cairo-move-to", 3, 0, 0,
	    (SCM ctx, SCM x, SCM y),
	    "")
{
    cairo_move_to (scm_to_cairo (ctx),
                   scm_to_double (x), scm_to_double (y));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_new_sub_path, "cairo-new-sub-path", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_new_sub_path (scm_to_cairo (ctx));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_line_to, "cairo-line-to", 3, 0, 0,
	    (SCM ctx, SCM x, SCM y),
	    "")
{
    cairo_line_to (scm_to_cairo (ctx),
                   scm_to_double (x), scm_to_double (y));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_curve_to, "cairo-curve-to", 7, 0, 0,
	    (SCM ctx, SCM x1, SCM y1, SCM x2, SCM y2, SCM x3, SCM y3),
	    "")
{
    cairo_curve_to (scm_to_cairo (ctx),
                    scm_to_double (x1), scm_to_double (y1),
                    scm_to_double (x2), scm_to_double (y2),
                    scm_to_double (x3), scm_to_double (y3));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_arc, "cairo-arc", 6, 0, 0,
	    (SCM ctx, SCM xc, SCM yc, SCM radius, SCM angle1, SCM angle2),
	    "")
{
    cairo_arc (scm_to_cairo (ctx),
               scm_to_double (xc), scm_to_double (yc),
               scm_to_double (radius),
               scm_to_double (angle1), scm_to_double (angle2));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_arc_negative, "cairo-arc-negative", 6, 0, 0,
	    (SCM ctx, SCM xc, SCM yc, SCM radius, SCM angle1, SCM angle2),
	    "")
{
    cairo_arc_negative (scm_to_cairo (ctx),
                        scm_to_double (xc), scm_to_double (yc),
                        scm_to_double (radius),
                        scm_to_double (angle1), scm_to_double (angle2));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_rel_move_to, "cairo-rel-move-to", 3, 0, 0,
	    (SCM ctx, SCM x, SCM y),
	    "")
{
    cairo_rel_move_to (scm_to_cairo (ctx),
                       scm_to_double (x), scm_to_double (y));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_rel_line_to, "cairo-rel-line-to", 3, 0, 0,
	    (SCM ctx, SCM x, SCM y),
	    "")
{
    cairo_rel_line_to (scm_to_cairo (ctx),
                       scm_to_double (x), scm_to_double (y));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_rel_curve_to, "cairo-rel-curve-to", 7, 0, 0,
	    (SCM ctx, SCM x1, SCM y1, SCM x2, SCM y2, SCM x3, SCM y3),
	    "")
{
    cairo_rel_curve_to (scm_to_cairo (ctx),
                        scm_to_double (x1), scm_to_double (y1),
                        scm_to_double (x2), scm_to_double (y2),
                        scm_to_double (x3), scm_to_double (y3));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_rectangle, "cairo-rectangle", 5, 0, 0,
	    (SCM ctx, SCM x, SCM y, SCM width, SCM height),
	    "")
{
    cairo_rectangle (scm_to_cairo (ctx),
                     scm_to_double (x), scm_to_double (y),
                     scm_to_double (width), scm_to_double (height));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_close_path, "cairo-close-path", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_close_path (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_paint, "cairo-paint", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_paint (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_paint_with_alpha, "cairo-paint-with-alpha", 2, 0, 0,
	    (SCM ctx, SCM alpha),
	    "")
{
    cairo_paint_with_alpha (scm_to_cairo (ctx),
                            scm_to_double (alpha));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_mask, "cairo-mask", 2, 0, 0,
	    (SCM ctx, SCM pat),
	    "")
{
    cairo_mask (scm_to_cairo (ctx),
                scm_to_cairo_pattern (pat));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_mask_surface, "cairo-mask-surface", 2, 0, 0,
	    (SCM ctx, SCM surf, SCM x, SCM y),
	    "")
{
    cairo_mask_surface (scm_to_cairo (ctx),
                        scm_to_cairo_surface (surf),
                        scm_to_double (x), scm_to_double (y));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_stroke, "cairo-stroke", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_stroke (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_stroke_preserve, "cairo-stroke-preserve", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_stroke_preserve (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_fill, "cairo-fill", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_fill (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_fill_preserve, "cairo-fill-preserve", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_fill_preserve (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_copy_page, "cairo-copy-page", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_copy_page (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_show_page, "cairo-show-page", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_show_page (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_in_stroke, "cairo-in-stroke", 3, 0, 0,
	    (SCM ctx, SCM x, SCM y),
	    "")
{
    return scm_from_bool (cairo_in_stroke (scm_to_cairo (ctx),
                                           scm_to_double (x),
                                           scm_to_double (y)));
}

SCM_DEFINE_PUBLIC (scm_cairo_in_fill, "cairo-in-fill", 3, 0, 0,
	    (SCM ctx, SCM x, SCM y),
	    "")
{
    return scm_from_bool (cairo_in_fill (scm_to_cairo (ctx),
                                         scm_to_double (x),
                                         scm_to_double (y)));
}

SCM_DEFINE_PUBLIC (scm_cairo_stroke_extents, "cairo-stroke-extents", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    double x1, y1, x2, y2;
    
    cairo_stroke_extents (scm_to_cairo (ctx), &x1, &y1, &x2, &y2);

    return scm_values (scm_list_4 (scm_from_double (x1),
                                   scm_from_double (y1),
                                   scm_from_double (x2),
                                   scm_from_double (y2)));
}

SCM_DEFINE_PUBLIC (scm_cairo_fill_extents, "cairo-fill-extents", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    double x1, y1, x2, y2;
    
    cairo_fill_extents (scm_to_cairo (ctx), &x1, &y1, &x2, &y2);

    return scm_values (scm_list_4 (scm_from_double (x1),
                                   scm_from_double (y1),
                                   scm_from_double (x2),
                                   scm_from_double (y2)));
}

SCM_DEFINE_PUBLIC (scm_cairo_reset_clip, "cairo-reset-clip", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_reset_clip (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_clip, "cairo-clip", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_clip (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_clip_preserve, "cairo-clip-preserve", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_clip_preserve (scm_to_cairo (ctx));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_clip_extents, "cairo-clip-extents", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    double x1, y1, x2, y2;
    
    cairo_clip_extents (scm_to_cairo (ctx), &x1, &y1, &x2, &y2);

    return scm_values (scm_list_4 (scm_from_double (x1),
                                   scm_from_double (y1),
                                   scm_from_double (x2),
                                   scm_from_double (y2)));
}

SCM_DEFINE_PUBLIC (scm_cairo_copy_clip_rectangle_list, "cairo-copy-clip-rectangle-list", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_rectangle_list_t *rlist;
    SCM ret = SCM_EOL;
    int i, len;
    
    rlist = cairo_copy_clip_rectangle_list (scm_to_cairo (ctx));
    len = rlist->num_rectangles;
    for (i = len - 1; i >= 0; i--)
        ret = scm_cons (scm_from_cairo_rectangle (&rlist->rectangles[i]), ret);
    cairo_rectangle_list_destroy (rlist);

    return ret;
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_create, "cairo-font-options-create", 0, 0, 0,
	    (void),
	    "")
{
    return scm_take_cairo_font_options (cairo_font_options_create ());
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_copy, "cairo-font-options-copy", 1, 0, 0,
	    (SCM fopt),
	    "")
{
    cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
    return scm_take_cairo_font_options (cairo_font_options_copy (opt));
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_merge, "cairo-font-options-merge", 2, 0, 0,
	    (SCM fopt, SCM other),
	    "")
{
    cairo_font_options_merge (scm_to_cairo_font_options (fopt),
                              scm_to_cairo_font_options (other));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_hash, "cairo-font-options-hash", 1, 0, 0,
	    (SCM fopt),
	    "")
{
    return scm_from_ulong (cairo_font_options_hash (scm_to_cairo_font_options (fopt)));
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_set_antialias, "cairo-font-options-set-antialias", 2, 0, 0,
	    (SCM fopt, SCM val),
	    "")
{
    cairo_font_options_set_antialias (scm_to_cairo_font_options (fopt),
                                      scm_to_cairo_antialias (val));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_get_antialias, "cairo-font-options-get-antialias", 1, 0, 0,
	    (SCM fopt),
	    "")
{
    cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
    return scm_from_cairo_antialias (cairo_font_options_get_antialias (opt));
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_set_subpixel_order, "cairo-font-options-set-subpixel-order", 2, 0, 0,
	    (SCM fopt, SCM val),
	    "")
{
    cairo_font_options_set_subpixel_order (scm_to_cairo_font_options (fopt),
                                           scm_to_cairo_subpixel_order (val));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_get_subpixel_order, "cairo-font-options-get-subpixel-order", 1, 0, 0,
	    (SCM fopt),
	    "")
{
    cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
    return scm_from_cairo_subpixel_order (cairo_font_options_get_subpixel_order (opt));
}


SCM_DEFINE_PUBLIC (scm_cairo_font_options_set_hint_style, "cairo-font-options-set-hint-style", 2, 0, 0,
	    (SCM fopt, SCM val),
	    "")
{
    cairo_font_options_set_hint_style (scm_to_cairo_font_options (fopt),
                                      scm_to_cairo_hint_style (val));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_get_hint_style, "cairo-font-options-get-hint-style", 1, 0, 0,
	    (SCM fopt),
	    "")
{
    cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
    return scm_from_cairo_hint_style (cairo_font_options_get_hint_style (opt));
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_set_hint_metrics, "cairo-font-options-set-hint-metrics", 2, 0, 0,
	    (SCM fopt, SCM val),
	    "")
{
    cairo_font_options_set_hint_metrics (scm_to_cairo_font_options (fopt),
                                      scm_to_cairo_hint_metrics (val));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_get_hint_metrics, "cairo-font-options-get-hint-metrics", 1, 0, 0,
	    (SCM fopt),
	    "")
{
    cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
    return scm_from_cairo_hint_metrics (cairo_font_options_get_hint_metrics (opt));
}

SCM_DEFINE_PUBLIC (scm_cairo_select_font_face, "cairo-select-font-face", 4, 0, 0,
	    (SCM ctx, SCM sfamily, SCM slant, SCM weight),
	    "")
{
    char *family;

    scm_dynwind_begin (0);
    family = scm_to_locale_string (sfamily);
    scm_dynwind_free (family);

    cairo_select_font_face (scm_to_cairo (ctx), family,
                            scm_to_cairo_font_slant (slant),
                            scm_to_cairo_font_weight (slant));

    scm_dynwind_end ();

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_font_size, "cairo-set-font-size", 2, 0, 0,
	    (SCM ctx, SCM size),
	    "")
{
    cairo_set_font_size (scm_to_cairo (ctx),
                         scm_to_double (size));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_set_font_matrix, "cairo-set-font-matrix", 2, 0, 0,
	    (SCM ctx, SCM smatrix),
	    "")
{
    cairo_matrix_t matrix;

    scm_fill_cairo_matrix (smatrix, &matrix);
    cairo_set_font_matrix (scm_to_cairo (ctx), &matrix);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_get_font_matrix, "cairo-get-font-matrix", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_matrix_t matrix;

    cairo_get_font_matrix (scm_to_cairo (ctx), &matrix);

    return scm_from_cairo_matrix (&matrix);
}
 
SCM_DEFINE_PUBLIC (scm_cairo_set_font_options, "cairo-set-font-options", 2, 0, 0,
	    (SCM ctx, SCM fopts),
	    "")
{
    cairo_set_font_options (scm_to_cairo (ctx),
                            scm_to_cairo_font_options (fopts));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_get_font_options, "cairo-get-font-options", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_font_options_t *opts = cairo_font_options_create ();
    cairo_get_font_options (scm_to_cairo (ctx), opts);
    return scm_take_cairo_font_options (opts);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_font_face, "cairo-set-font-face", 2, 0, 0,
	    (SCM ctx, SCM face),
	    "")
{
    cairo_set_font_face (scm_to_cairo (ctx),
                         scm_to_cairo_font_face (face));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_get_font_face, "cairo-get-font-face", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_font_face (cairo_get_font_face (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_set_scaled_font, "cairo-set-scaled-font", 2, 0, 0,
	    (SCM ctx, SCM face),
	    "")
{
    cairo_set_scaled_font (scm_to_cairo (ctx),
                           scm_to_cairo_scaled_font (face));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_get_scaled_font, "cairo-get-scaled-font", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_scaled_font (cairo_get_scaled_font (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_show_text, "cairo-show-text", 2, 0, 0,
	    (SCM ctx, SCM val),
	    "")
{
    char *text;

    scm_dynwind_begin (0); 
    text = scm_to_locale_string (val);
    scm_dynwind_free (text);

    cairo_show_text (scm_to_cairo (ctx), text);

    scm_dynwind_end ();

    return SCM_UNSPECIFIED;
}


SCM_DEFINE_PUBLIC (scm_cairo_show_glyphs, "cairo-show-glyphs", 2, 0, 0,
	    (SCM ctx, SCM sglyphs),
	    "")
{
    cairo_glyph_t *glyphs;
    int nglyphs, i;

    scm_dynwind_begin (0);

    nglyphs = scm_to_signed_integer (scm_vector_length (sglyphs),
                                     0, (1<<31) / sizeof (cairo_glyph_t));
    glyphs = scm_malloc (nglyphs * sizeof(cairo_glyph_t));
    scm_dynwind_free (glyphs);
    
    for (i = 0; i < nglyphs; i++)
        scm_fill_cairo_glyph (scm_c_vector_ref (sglyphs, i), &glyphs[i]);

    cairo_show_glyphs (scm_to_cairo (ctx), glyphs, nglyphs);

    scm_dynwind_end ();
    
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_text_path, "cairo-text-path", 2, 0, 0,
	    (SCM ctx, SCM val),
	    "")
{
    char *text;

    scm_dynwind_begin (0); 
    text = scm_to_locale_string (val);
    scm_dynwind_free (text);

    cairo_text_path (scm_to_cairo (ctx), text);

    scm_dynwind_end ();

    return SCM_UNSPECIFIED;
}


SCM_DEFINE_PUBLIC (scm_cairo_glyph_path, "cairo-glyph-path", 2, 0, 0,
	    (SCM ctx, SCM sglyphs),
	    "")
{
    cairo_glyph_t *glyphs;
    int nglyphs, i;

    scm_dynwind_begin (0);

    nglyphs = scm_to_signed_integer (scm_vector_length (sglyphs),
                                     0, (1<<31) / sizeof (cairo_glyph_t));
    glyphs = scm_malloc (nglyphs * sizeof(cairo_glyph_t));
    scm_dynwind_free (glyphs);
    
    for (i = 0; i < nglyphs; i++)
        scm_fill_cairo_glyph (scm_c_vector_ref (sglyphs, i), &glyphs[i]);

    cairo_glyph_path (scm_to_cairo (ctx), glyphs, nglyphs);

    scm_dynwind_end ();
    
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_text_extents, "cairo-text-extents", 2, 0, 0,
	    (SCM ctx, SCM val),
	    "")
{
    SCM ret;
    char *text;
    cairo_text_extents_t extents;

    scm_dynwind_begin (0); 
    text = scm_to_locale_string (val);
    scm_dynwind_free (text);

    cairo_text_extents (scm_to_cairo (ctx), text, &extents);
    ret = scm_from_cairo_text_extents (&extents);

    scm_dynwind_end ();

    return ret;
}

SCM_DEFINE_PUBLIC (scm_cairo_glyph_extents, "cairo-glyph-extents", 2, 0, 0,
	    (SCM ctx, SCM sglyphs),
	    "")
{
    cairo_glyph_t *glyphs;
    int nglyphs, i;
    SCM ret;
    cairo_text_extents_t extents;

    scm_dynwind_begin (0);

    nglyphs = scm_to_signed_integer (scm_vector_length (sglyphs),
                                     0, (1<<31) / sizeof (cairo_glyph_t));
    glyphs = scm_malloc (nglyphs * sizeof(cairo_glyph_t));
    scm_dynwind_free (glyphs);
    
    for (i = 0; i < nglyphs; i++)
        scm_fill_cairo_glyph (scm_c_vector_ref (sglyphs, i), &glyphs[i]);

    cairo_glyph_extents (scm_to_cairo (ctx), glyphs, nglyphs, &extents);
    ret = scm_from_cairo_text_extents (&extents);

    scm_dynwind_end ();
    
    return ret;
}

SCM_DEFINE_PUBLIC (scm_cairo_font_extents, "cairo-font-extents", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    SCM ret;
    cairo_font_extents_t extents;

    cairo_font_extents (scm_to_cairo (ctx), &extents);
    ret = scm_from_cairo_font_extents (&extents);

    return ret;
}

#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_font_face_get_reference_count, "cairo-font-face-get-reference-count", 1, 0, 0,
	    (SCM face),
	    "")
{
    return scm_from_uint (cairo_font_face_get_reference_count (scm_to_cairo_font_face (face)));
}
#endif

SCM_DEFINE_PUBLIC (scm_cairo_font_face_get_type, "cairo-font-face-get-type", 1, 0, 0,
	    (SCM face),
	    "")
{
    return scm_from_cairo_font_type (cairo_font_face_get_type (scm_to_cairo_font_face (face)));
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_create, "cairo-scaled-font-create", 3, 0, 0,
                   (SCM face, SCM smatrix, SCM sctm, SCM options),
	    "")
{
    cairo_matrix_t matrix, ctm;

    scm_fill_cairo_matrix (smatrix, &matrix);
    scm_fill_cairo_matrix (sctm, &ctm);
    
    return scm_take_cairo_scaled_font (cairo_scaled_font_create (scm_to_cairo_font_face (face),
                                                                 &matrix, &ctm,
                                                                 scm_to_cairo_font_options (options)));
}

#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_reference_count, "cairo-scaled-font-get-reference-count", 1, 0, 0,
	    (SCM font),
	    "")
{
p    return scm_from_uint (cairo_scaled_font_get_reference_count (scm_to_cairo_scaled_font (font)));
}
#endif

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_type, "cairo-scaled-font-get-type", 1, 0, 0,
	    (SCM font),
	    "")
{
    return scm_from_cairo_font_type (cairo_scaled_font_get_type (scm_to_cairo_scaled_font (font)));
}


SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_extents, "cairo-scaled-font-extents", 1, 0, 0,
	    (SCM font),
	    "")
{
    SCM ret;
    cairo_font_extents_t extents;

    cairo_scaled_font_extents (scm_to_cairo_scaled_font (font), &extents);
    ret = scm_from_cairo_font_extents (&extents);

    return ret;
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_text_extents, "cairo-scaled-font-text-extents", 2, 0, 0,
                   (SCM font, SCM val),
	    "")
{
    SCM ret;
    char *text;
    cairo_text_extents_t extents;

    scm_dynwind_begin (0); 
    text = scm_to_locale_string (val);
    scm_dynwind_free (text);

    cairo_scaled_font_text_extents (scm_to_cairo_scaled_font (font), text, &extents);
    ret = scm_from_cairo_text_extents (&extents);

    scm_dynwind_end ();

    return ret;
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_glyph_extents, "cairo-scaled-font-glyph-extents", 2, 0, 0,
	    (SCM font, SCM sglyphs),
	    "")
{
    cairo_glyph_t *glyphs;
    int nglyphs, i;
    SCM ret;
    cairo_text_extents_t extents;

    scm_dynwind_begin (0);

    nglyphs = scm_to_signed_integer (scm_vector_length (sglyphs),
                                     0, (1<<31) / sizeof (cairo_glyph_t));
    glyphs = scm_malloc (nglyphs * sizeof(cairo_glyph_t));
    scm_dynwind_free (glyphs);
    
    for (i = 0; i < nglyphs; i++)
        scm_fill_cairo_glyph (scm_c_vector_ref (sglyphs, i), &glyphs[i]);

    cairo_scaled_font_glyph_extents (scm_to_cairo_scaled_font (font), glyphs, nglyphs, &extents);
    ret = scm_from_cairo_text_extents (&extents);

    scm_dynwind_end ();
    
    return ret;
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_font_face, "cairo-scaled-font-get-font-face", 1, 0, 0,
	    (SCM font),
	    "")
{
    return scm_from_cairo_font_face (cairo_scaled_font_get_font_face (scm_to_cairo_scaled_font (font)));
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_font_matrix, "cairo-scaled-font-get-font-matrix", 1, 0, 0,
	    (SCM font),
	    "")
{
    cairo_matrix_t matrix;

    cairo_scaled_font_get_font_matrix (scm_to_cairo_scaled_font (font), &matrix);

    return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_ctm, "cairo-scaled-font-get-ctm", 1, 0, 0,
	    (SCM font),
	    "")
{
    cairo_matrix_t matrix;

    cairo_scaled_font_get_ctm (scm_to_cairo_scaled_font (font), &matrix);

    return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_font_options, "cairo-scaled-font-get-font-options", 1, 0, 0,
	    (SCM font),
	    "")
{
    cairo_font_options_t *opts = cairo_font_options_create ();

    cairo_scaled_font_get_font_options (scm_to_cairo_scaled_font (font), opts);

    return scm_take_cairo_font_options (opts);
}

SCM_DEFINE_PUBLIC (scm_cairo_get_operator, "cairo-get-operator", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_operator (cairo_get_operator (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_source, "cairo-get-source", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_pattern (cairo_get_source (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_tolerance, "cairo-get-tolerance", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_double (cairo_get_tolerance (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_antialias, "cairo-get-antialias", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_antialias (cairo_get_antialias (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_current_point, "cairo-get-current-point", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    double x, y;
    cairo_get_current_point (scm_to_cairo (ctx), &x, &y);
    return scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_fill_rule, "cairo-get-fill-rule", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_fill_rule (cairo_get_fill_rule (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_line_width, "cairo-get-line-width", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_double (cairo_get_line_width (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_line_cap, "cairo-get-line-cap", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_line_cap (cairo_get_line_cap (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_line_join, "cairo-get-line-join", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_line_join (cairo_get_line_join (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_miter_limit, "cairo-get-miter-limit", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_double (cairo_get_miter_limit (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_dash_count, "cairo-get-dash-count", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_int (cairo_get_dash_count (scm_to_cairo (ctx)));
}
SCM_DEFINE_PUBLIC (scm_cairo_get_dash, "cairo-get-line-join", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    double *data = NULL;
    int ndoubles;
    cairo_t *cr;
    double offset;
    
    cr = scm_to_cairo (ctx);
    ndoubles = cairo_get_dash_count (cr);
    if (ndoubles)
        data = scm_malloc (ndoubles * sizeof (double));
    
    cairo_get_dash (cr, data, &offset);
    
    return scm_values (scm_list_2 (data ? scm_take_f64vector(data, ndoubles) : SCM_BOOL_F,
                                   scm_from_double (offset)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_matrix, "cairo-get-matrix", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    cairo_matrix_t matrix;

    cairo_get_matrix (scm_to_cairo (ctx), &matrix);

    return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_get_target, "cairo-get-target", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_surface (cairo_get_target (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_group_target, "cairo-get-group-target", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_from_cairo_surface (cairo_get_group_target (scm_to_cairo (ctx)));
}

static SCM make_point (double x, double y) 
{
    return scm_f64vector (scm_list_2 (scm_from_double (x),
                                      scm_from_double (y)));
}

SCM_DEFINE_PUBLIC (scm_cairo_path_fold, "cairo-path-fold", 3, 0, 0,
	    (SCM spath, SCM proc, SCM init),
	    "Fold over a cairo path.")
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

SCM_DEFINE_PUBLIC (scm_cairo_copy_path, "cairo-copy-path", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_take_cairo_path (cairo_copy_path (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_copy_path_flat, "cairo-copy-path-flat", 1, 0, 0,
	    (SCM ctx),
	    "")
{
    return scm_take_cairo_path (cairo_copy_path_flat (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_append_path, "cairo-append-path", 2, 0, 0,
                   (SCM ctx, SCM path),
	    "")
{
    cairo_append_path (scm_to_cairo (ctx), scm_to_cairo_path (path));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_create_similar, "cairo-surface-create-similar", 4, 0, 0,
                   (SCM other, SCM content, SCM w, SCM h),
	    "")
{
    return scm_take_cairo_surface (cairo_surface_create_similar (scm_to_cairo_surface (other),
                                                                 scm_to_cairo_content (content),
                                                                 scm_to_int (w),
                                                                 scm_to_int (h)));
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_finish, "cairo-surface-finish", 1, 0, 0,
                   (SCM surf),
	    "")
{
    cairo_surface_finish (scm_to_cairo_surface (surf));
    return SCM_UNSPECIFIED;
}

#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_surface_get_reference_count, "cairo-surface-get-reference-count", 1, 0, 0,
	    (SCM surf),
	    "")
{
    return scm_from_uint (cairo_surface_get_reference_count (scm_to_cairo_surface (surf)));
}
#endif

SCM_DEFINE_PUBLIC (scm_cairo_surface_get_type, "cairo-surface-get-type", 1, 0, 0,
	    (SCM surf),
	    "")
{
    return scm_from_cairo_surface_type (cairo_surface_get_type (scm_to_cairo_surface (surf)));
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_get_content, "cairo-surface-get-content", 1, 0, 0,
	    (SCM surf),
	    "")
{
    return scm_from_cairo_content (cairo_surface_get_content (scm_to_cairo_surface (surf)));
}

#if CAIRO_HAS_PNG_FUNCTIONS

SCM_DEFINE_PUBLIC (scm_cairo_surface_write_to_png, "cairo-surface-write-to-png", 1, 0, 0,
                   (SCM surf, SCM filename),
	    "")
{
    char *str;
    cairo_status_t status;

    scm_dynwind_begin (0);
    str = scm_to_locale_string (filename);
    scm_dynwind_free (str);
    
    status = cairo_surface_write_to_png (scm_to_cairo_surface (surf), str);
    scm_c_check_cairo_status (status, s_scm_cairo_surface_write_to_png);

    return SCM_UNSPECIFIED;
}

#if 0
cairo_public cairo_status_t
cairo_surface_write_to_png_stream (cairo_surface_t	*surface,
				   cairo_write_func_t	write_func,
				   void			*closure);
#endif /* 0 */

#endif /* CAIRO_HAS_PNG_FUNCTIONS */

SCM_DEFINE_PUBLIC (scm_cairo_surface_get_font_options, "cairo-surface-get-font-options", 1, 0, 0,
	    (SCM surf),
	    "")
{
    cairo_font_options_t *opts = cairo_font_options_create ();
    
    cairo_surface_get_font_options (scm_to_cairo_surface (surf), opts);
    return scm_take_cairo_font_options(opts);
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_flush, "cairo-surface-flush", 1, 0, 0,
	    (SCM surf),
	    "")
{
    cairo_surface_flush (scm_to_cairo_surface (surf));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_mark_dirty, "cairo-surface-mark-dirty", 1, 0, 0,
	    (SCM surf),
	    "")
{
    cairo_surface_mark_dirty (scm_to_cairo_surface (surf));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_mark_dirty_rectangle, "cairo-surface-mark-dirty-rectangle", 5, 0, 0,
                   (SCM surf, SCM x, SCM y, SCM width, SCM height),
	    "")
{
    cairo_surface_mark_dirty_rectangle (scm_to_cairo_surface (surf),
                                        scm_to_double (x),
                                        scm_to_double (y),
                                        scm_to_double (width),
                                        scm_to_double (height));
    
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_set_device_offset, "cairo-surface-set-device-offset", 3, 0, 0,
                   (SCM surf, SCM x, SCM y),
	    "")
{
    cairo_surface_set_device_offset (scm_to_cairo_surface (surf),
                                     scm_to_double (x),
                                     scm_to_double (y));
    
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_get_device_offset, "cairo-surface-get-device-offset", 1, 0, 0,
                   (SCM surf),
	    "")
{
    double x, y;

    cairo_surface_get_device_offset (scm_to_cairo_surface (surf),
                                     &x, &y);
    return scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y)));
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_set_fallback_resolution, "cairo-surface-set-fallback_resolution", 3, 0, 0,
                   (SCM surf, SCM x, SCM y),
	    "")
{
    cairo_surface_set_fallback_resolution (scm_to_cairo_surface (surf),
                                           scm_to_double (x),
                                           scm_to_double (y));
    
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_create, "cairo-image-surface-create", 3, 0, 0,
                   (SCM format, SCM x, SCM y),
	    "")
{
    return scm_take_cairo_surface (cairo_image_surface_create (scm_to_cairo_format (format),
                                                               scm_to_double (x),
                                                               scm_to_double (y)));
}

#if 0
cairo_public cairo_surface_t *
cairo_image_surface_create_for_data (unsigned char	       *data,
				     cairo_format_t		format,
				     int			width,
				     int			height,
				     int			stride);
cairo_public unsigned char *
cairo_image_surface_get_data (cairo_surface_t *surface);

#endif /* 0 */

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_get_format, "cairo-image-surface-get-format", 1, 0, 0,
                   (SCM surf),
	    "")
{
    return scm_from_cairo_format (cairo_image_surface_get_format (scm_to_cairo_surface (surf)));
}

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_get_width, "cairo-image-surface-get-width", 1, 0, 0,
                   (SCM surf),
	    "")
{
    return scm_from_double (cairo_image_surface_get_width (scm_to_cairo_surface (surf)));
}

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_get_height, "cairo-image-surface-get-heigt", 1, 0, 0,
                   (SCM surf),
	    "")
{
    return scm_from_double (cairo_image_surface_get_height (scm_to_cairo_surface (surf)));
}

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_get_stride, "cairo-image-surface-get-heigt", 1, 0, 0,
                   (SCM surf),
	    "")
{
    return scm_from_double (cairo_image_surface_get_stride (scm_to_cairo_surface (surf)));
}

#if CAIRO_HAS_PNG_FUNCTIONS

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_create_from_png, "cairo-image-surface-create-from-png", 1, 0, 0,
                   (SCM sfilename),
	    "")
{
    char *filename;
    SCM ret;
    
    scm_dynwind_begin (0);
    filename = scm_to_locale_string (sfilename);
    scm_dynwind_free (filename);

    ret = scm_take_cairo_surface (cairo_image_surface_create_from_png (filename));

    scm_dynwind_end ();

    return ret;
}

#if 0
cairo_public cairo_surface_t *
cairo_image_surface_create_from_png_stream (cairo_read_func_t	read_func,
					    void		*closure);
#endif /* 0 */

#endif /* CAIRO_HAS_PNG_FUNCTIONS */

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_rgb, "cairo-pattern-create-rgb", 3, 0, 0,
                   (SCM r, SCM g, SCM b),
	    "")
{
    return scm_take_cairo_pattern (cairo_pattern_create_rgb (scm_to_double (r),
                                                             scm_to_double (g),
                                                             scm_to_double (b)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_rgba, "cairo-pattern-create-rgba", 4, 0, 0,
                   (SCM r, SCM g, SCM b, SCM a),
	    "")
{
    return scm_take_cairo_pattern (cairo_pattern_create_rgba (scm_to_double (r),
                                                              scm_to_double (g),
                                                              scm_to_double (b),
                                                              scm_to_double (a)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_for_surface, "cairo-pattern-create-for-surface", 1, 0, 0,
                   (SCM surf),
	    "")
{
    return scm_take_cairo_pattern (cairo_pattern_create_for_surface (scm_to_cairo_surface (surf)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_linear, "cairo-pattern-create-linear", 4, 0, 0,
                   (SCM x0, SCM y0, SCM x1, SCM y1),
	    "")
{
    return scm_take_cairo_pattern (cairo_pattern_create_linear (scm_to_double (x0),
                                                                scm_to_double (y0),
                                                                scm_to_double (x1),
                                                                scm_to_double (y1)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_radial, "cairo-pattern-create-radial", 6, 0, 0,
                   (SCM cx0, SCM cy0, SCM r0, SCM cx1, SCM cy1, SCM r1),
	    "")
{
    return scm_take_cairo_pattern (cairo_pattern_create_radial (scm_to_double (cx0),
                                                                scm_to_double (cy0),
                                                                scm_to_double (r0),
                                                                scm_to_double (cx1),
                                                                scm_to_double (cy1),
                                                                scm_to_double (r1)));
}

#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_reference_count, "cairo-pattern-get-reference-count", 1, 0, 0,
	    (SCM pat),
	    "")
{
    return scm_from_uint (cairo_pattern_get_reference_count (scm_to_cairo_pattern (pat)));
}
#endif

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_type, "cairo-pattern-get-type", 1, 0, 0,
	    (SCM pat),
	    "")
{
    return scm_from_cairo_pattern_type (cairo_pattern_get_type (scm_to_cairo_pattern (pat)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_add_color_stop_rgb, "cairo-pattern-add-color-stop-rgb", 5, 0, 0,
                   (SCM pat, SCM offset, SCM r, SCM g, SCM b),
	    "")
{
    cairo_pattern_add_color_stop_rgb (scm_to_cairo_pattern (pat),
                                      scm_to_double (offset),
                                      scm_to_double (r),
                                      scm_to_double (g),
                                      scm_to_double (b));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_add_color_stop_rgba, "cairo-pattern-add-color-stop-rgba", 6, 0, 0,
                   (SCM pat, SCM offset, SCM r, SCM g, SCM b, SCM a),
	    "")
{
    cairo_pattern_add_color_stop_rgba (scm_to_cairo_pattern (pat),
                                       scm_to_double (offset),
                                       scm_to_double (r),
                                       scm_to_double (g),
                                       scm_to_double (b),
                                       scm_to_double (a));
    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_set_matrix, "cairo-pattern-set-matrix", 2, 0, 0,
	    (SCM pat, SCM smatrix),
	    "")
{
    cairo_matrix_t matrix;
    scm_fill_cairo_matrix (smatrix, &matrix);
    
    cairo_pattern_set_matrix (scm_to_cairo_pattern (pat), &matrix);

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_matrix, "cairo-pattern-get-matrix", 1, 0, 0,
	    (SCM pat),
	    "")
{
    cairo_matrix_t matrix;
    
    cairo_pattern_get_matrix (scm_to_cairo_pattern (pat), &matrix);

    return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_set_extend, "cairo-pattern-set-extend", 2, 0, 0,
	    (SCM pat, SCM extend),
	    "")
{
    cairo_pattern_set_extend (scm_to_cairo_pattern (pat),
                              scm_to_cairo_extend (extend));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_extend, "cairo-pattern-get-extend", 1, 0, 0,
	    (SCM pat),
	    "")
{
    return scm_from_cairo_extend (cairo_pattern_get_extend (scm_to_cairo_pattern (pat)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_set_filter, "cairo-pattern-set-filter", 2, 0, 0,
	    (SCM pat, SCM filter),
	    "")
{
    cairo_pattern_set_filter (scm_to_cairo_pattern (pat),
                              scm_to_cairo_filter (filter));

    return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_filter, "cairo-pattern-get-filter", 1, 0, 0,
	    (SCM pat),
	    "")
{
    return scm_from_cairo_filter (cairo_pattern_get_filter (scm_to_cairo_pattern (pat)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_rgba, "cairo-pattern-get-rgba", 1, 0, 0,
	    (SCM pat),
	    "")
{
    double r, g, b, a;
    cairo_pattern_get_rgba (scm_to_cairo_pattern (pat), &r, &b, &g, &a);
    return scm_values (scm_list_4 (scm_from_double (r), scm_from_double (g),
                                   scm_from_double (b), scm_from_double (a)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_surface, "cairo-pattern-get-surface", 1, 0, 0,
	    (SCM pat),
	    "")
{
    cairo_surface_t *surf = NULL;
    cairo_status_t status;
    
    status = cairo_pattern_get_surface (scm_to_cairo_pattern (pat), &surf);
    scm_c_check_cairo_status (status, s_scm_cairo_pattern_get_surface);
    
    return scm_from_cairo_surface (surf);
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_color_stop_rgba, "cairo-pattern-get-color-stop-rgba", 2, 0, 0,
                   (SCM pat, SCM i),
	    "")
{
    double off, r, g, b, a;
    cairo_pattern_get_color_stop_rgba (scm_to_cairo_pattern (pat), scm_to_int (i),
                                       &off, &r, &b, &g, &a);
    return scm_values (scm_list_5 (scm_from_double (off),
                                   scm_from_double (r), scm_from_double (g),
                                   scm_from_double (b), scm_from_double (a)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_color_stop_count, "cairo-pattern-get-color-count", 1, 0, 0,
                   (SCM pat),
	    "")
{
    int count;
    cairo_pattern_get_color_stop_count (scm_to_cairo_pattern (pat), &count);
    return scm_from_int (count);
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_linear_points, "cairo-pattern-get-linear-points", 1, 0, 0,
                   (SCM pat),
	    "")
{
    double x0, y0, x1, y1;
    cairo_pattern_get_linear_points (scm_to_cairo_pattern (pat),
                                     &x0, &y0, &x1, &y1);
    return scm_values (scm_list_4 (scm_from_double (x0), scm_from_double (y0),
                                   scm_from_double (x1), scm_from_double (y1)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_radial_circles, "cairo-pattern-get-radial-circles", 1, 0, 0,
                   (SCM pat),
	    "")
{
    double x0, y0, r0, x1, y1, r1;
    
    cairo_pattern_get_radial_circles (scm_to_cairo_pattern (pat),
                                      &x0, &y0, &r0, &x1, &y1, &r1);
    return scm_values (scm_list_n (scm_from_double (x0), scm_from_double (y0),
                                   scm_from_double (r0),
                                   scm_from_double (x1), scm_from_double (y1),
                                   scm_from_double (r1),
                                   SCM_UNDEFINED));
}

#if 0

/* Matrix functions */

cairo_public void
cairo_matrix_init (cairo_matrix_t *matrix,
		   double  xx, double  yx,
		   double  xy, double  yy,
		   double  x0, double  y0);

cairo_public void
cairo_matrix_init_identity (cairo_matrix_t *matrix);

cairo_public void
cairo_matrix_init_translate (cairo_matrix_t *matrix,
			     double tx, double ty);

cairo_public void
cairo_matrix_init_scale (cairo_matrix_t *matrix,
			 double sx, double sy);

cairo_public void
cairo_matrix_init_rotate (cairo_matrix_t *matrix,
			  double radians);

cairo_public void
cairo_matrix_translate (cairo_matrix_t *matrix, double tx, double ty);

cairo_public void
cairo_matrix_scale (cairo_matrix_t *matrix, double sx, double sy);

cairo_public void
cairo_matrix_rotate (cairo_matrix_t *matrix, double radians);

cairo_public cairo_status_t
cairo_matrix_invert (cairo_matrix_t *matrix);

cairo_public void
cairo_matrix_multiply (cairo_matrix_t	    *result,
		       const cairo_matrix_t *a,
		       const cairo_matrix_t *b);

cairo_public void
cairo_matrix_transform_distance (const cairo_matrix_t *matrix,
				 double *dx, double *dy);

cairo_public void
cairo_matrix_transform_point (const cairo_matrix_t *matrix,
			      double *x, double *y);
#endif


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

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

SCM_DEFINE_PUBLIC (scm_cairo_version, "cairo-version", 0, 0, 0,
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

SCM_DEFINE_PUBLIC (scm_cairo_path_fold, "cairo-path-fold", 3, 0, 0,
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

SCM_DEFINE_PUBLIC (scm_cairo_version_string, "cairo-version-string", 0, 0, 0,
	    (void),
	    "Retrieves the version of the cairo library as a string.")
{
    return scm_from_locale_string (cairo_version_string ());
}

SCM_DEFINE_PUBLIC (scm_cairo_cairo, "cairo-create", 1, 0, 0,
	    (SCM surf),
	    "")
{
    return scm_from_cairo (cairo_create (scm_to_cairo_surface (surf)));
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
    return scm_from_cairo_font_options (cairo_font_options_create ());
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_copy, "cairo-font-options-copy", 1, 0, 0,
	    (SCM fopt),
	    "")
{
    cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
    return scm_from_cairo_font_options (cairo_font_options_copy (opt));
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
    return scm_from_cairo_font_options (opts);
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


#if 0
cairo_public void
cairo_show_text (cairo_t *cr, const char *utf8);

cairo_public void
cairo_show_glyphs (cairo_t *cr, const cairo_glyph_t *glyphs, int num_glyphs);

cairo_public void
cairo_text_path  (cairo_t *cr, const char *utf8);

cairo_public void
cairo_glyph_path (cairo_t *cr, const cairo_glyph_t *glyphs, int num_glyphs);

cairo_public void
cairo_text_extents (cairo_t              *cr,
		    const char    	 *utf8,
		    cairo_text_extents_t *extents);

cairo_public void
cairo_glyph_extents (cairo_t               *cr,
		     const cairo_glyph_t   *glyphs,
		     int                   num_glyphs,
		     cairo_text_extents_t  *extents);

cairo_public void
cairo_font_extents (cairo_t              *cr,
		    cairo_font_extents_t *extents);

/* Generic identifier for a font style */

cairo_public cairo_font_face_t *
cairo_font_face_reference (cairo_font_face_t *font_face);

cairo_public void
cairo_font_face_destroy (cairo_font_face_t *font_face);

cairo_public unsigned int
cairo_font_face_get_reference_count (cairo_font_face_t *font_face);

cairo_public cairo_status_t
cairo_font_face_status (cairo_font_face_t *font_face);

/**
 * cairo_font_type_t
 * @CAIRO_FONT_TYPE_TOY: The font was created using cairo's toy font api
 * @CAIRO_FONT_TYPE_FT: The font is of type FreeType
 * @CAIRO_FONT_TYPE_WIN32: The font is of type Win32
 * @CAIRO_FONT_TYPE_ATSUI: The font is of type ATSUI
 *
 * #cairo_font_type_t is used to describe the type of a given font
 * face or scaled font. The font types are also known as "font
 * backends" within cairo.
 *
 * The type of a font face is determined by the function used to
 * create it, which will generally be of the form
 * cairo_<emphasis>type</emphasis>_font_face_create. The font face type can be queried
 * with cairo_font_face_get_type()
 *
 * The various cairo_font_face functions can be used with a font face
 * of any type.
 *
 * The type of a scaled font is determined by the type of the font
 * face passed to cairo_scaled_font_create. The scaled font type can
 * be queried with cairo_scaled_font_get_type()
 *
 * The various cairo_scaled_font functions can be used with scaled
 * fonts of any type, but some font backends also provide
 * type-specific functions that must only be called with a scaled font
 * of the appropriate type. These functions have names that begin with
 * cairo_<emphasis>type</emphasis>_scaled_font such as cairo_ft_scaled_font_lock_face.
 *
 * The behavior of calling a type-specific function with a scaled font
 * of the wrong type is undefined.
 *
 * New entries may be added in future versions.
 *
 * Since: 1.2
 **/
typedef enum _cairo_font_type {
    CAIRO_FONT_TYPE_TOY,
    CAIRO_FONT_TYPE_FT,
    CAIRO_FONT_TYPE_WIN32,
    CAIRO_FONT_TYPE_ATSUI
} cairo_font_type_t;

cairo_public cairo_font_type_t
cairo_font_face_get_type (cairo_font_face_t *font_face);

cairo_public void *
cairo_font_face_get_user_data (cairo_font_face_t	   *font_face,
			       const cairo_user_data_key_t *key);

cairo_public cairo_status_t
cairo_font_face_set_user_data (cairo_font_face_t	   *font_face,
			       const cairo_user_data_key_t *key,
			       void			   *user_data,
			       cairo_destroy_func_t	    destroy);

/* Portable interface to general font features. */

cairo_public cairo_scaled_font_t *
cairo_scaled_font_create (cairo_font_face_t          *font_face,
			  const cairo_matrix_t       *font_matrix,
			  const cairo_matrix_t       *ctm,
			  const cairo_font_options_t *options);

cairo_public cairo_scaled_font_t *
cairo_scaled_font_reference (cairo_scaled_font_t *scaled_font);

cairo_public void
cairo_scaled_font_destroy (cairo_scaled_font_t *scaled_font);

cairo_public unsigned int
cairo_scaled_font_get_reference_count (cairo_scaled_font_t *scaled_font);

cairo_public cairo_status_t
cairo_scaled_font_status (cairo_scaled_font_t *scaled_font);

cairo_public cairo_font_type_t
cairo_scaled_font_get_type (cairo_scaled_font_t *scaled_font);

cairo_public void *
cairo_scaled_font_get_user_data (cairo_scaled_font_t         *scaled_font,
				 const cairo_user_data_key_t *key);

cairo_public cairo_status_t
cairo_scaled_font_set_user_data (cairo_scaled_font_t         *scaled_font,
				 const cairo_user_data_key_t *key,
				 void                        *user_data,
				 cairo_destroy_func_t	      destroy);

cairo_public void
cairo_scaled_font_extents (cairo_scaled_font_t  *scaled_font,
			   cairo_font_extents_t *extents);

cairo_public void
cairo_scaled_font_text_extents (cairo_scaled_font_t  *scaled_font,
				const char  	     *utf8,
				cairo_text_extents_t *extents);

cairo_public void
cairo_scaled_font_glyph_extents (cairo_scaled_font_t   *scaled_font,
				 const cairo_glyph_t   *glyphs,
				 int                   num_glyphs,
				 cairo_text_extents_t  *extents);

cairo_public cairo_font_face_t *
cairo_scaled_font_get_font_face (cairo_scaled_font_t *scaled_font);

cairo_public void
cairo_scaled_font_get_font_matrix (cairo_scaled_font_t	*scaled_font,
				   cairo_matrix_t	*font_matrix);

cairo_public void
cairo_scaled_font_get_ctm (cairo_scaled_font_t	*scaled_font,
			   cairo_matrix_t	*ctm);

cairo_public void
cairo_scaled_font_get_font_options (cairo_scaled_font_t		*scaled_font,
				    cairo_font_options_t	*options);

/* Query functions */

cairo_public cairo_operator_t
cairo_get_operator (cairo_t *cr);

cairo_public cairo_pattern_t *
cairo_get_source (cairo_t *cr);

cairo_public double
cairo_get_tolerance (cairo_t *cr);

cairo_public cairo_antialias_t
cairo_get_antialias (cairo_t *cr);

cairo_public void
cairo_get_current_point (cairo_t *cr, double *x, double *y);

cairo_public cairo_fill_rule_t
cairo_get_fill_rule (cairo_t *cr);

cairo_public double
cairo_get_line_width (cairo_t *cr);

cairo_public cairo_line_cap_t
cairo_get_line_cap (cairo_t *cr);

cairo_public cairo_line_join_t
cairo_get_line_join (cairo_t *cr);

cairo_public double
cairo_get_miter_limit (cairo_t *cr);

cairo_public int
cairo_get_dash_count (cairo_t *cr);

cairo_public void
cairo_get_dash (cairo_t *cr, double *dashes, double *offset);

cairo_public void
cairo_get_matrix (cairo_t *cr, cairo_matrix_t *matrix);

cairo_public cairo_surface_t *
cairo_get_target (cairo_t *cr);

cairo_public cairo_surface_t *
cairo_get_group_target (cairo_t *cr);

/**
 * cairo_path_data_type_t:
 * @CAIRO_PATH_MOVE_TO: A move-to operation
 * @CAIRO_PATH_LINE_TO: A line-to operation
 * @CAIRO_PATH_CURVE_TO: A curve-to operation
 * @CAIRO_PATH_CLOSE_PATH: A close-path operation
 *
 * #cairo_path_data_t is used to describe the type of one portion
 * of a path when represented as a #cairo_path_t.
 * See #cairo_path_data_t for details.
 **/
typedef enum _cairo_path_data_type {
    CAIRO_PATH_MOVE_TO,
    CAIRO_PATH_LINE_TO,
    CAIRO_PATH_CURVE_TO,
    CAIRO_PATH_CLOSE_PATH
} cairo_path_data_type_t;

/**
 * cairo_path_data_t:
 *
 * #cairo_path_data_t is used to represent the path data inside a
 * #cairo_path_t.
 *
 * The data structure is designed to try to balance the demands of
 * efficiency and ease-of-use. A path is represented as an array of
 * #cairo_path_data_t, which is a union of headers and points.
 *
 * Each portion of the path is represented by one or more elements in
 * the array, (one header followed by 0 or more points). The length
 * value of the header is the number of array elements for the current
 * portion including the header, (ie. length == 1 + # of points), and
 * where the number of points for each element type is as follows:
 *
 * <programlisting>
 *     %CAIRO_PATH_MOVE_TO:     1 point
 *     %CAIRO_PATH_LINE_TO:     1 point
 *     %CAIRO_PATH_CURVE_TO:    3 points
 *     %CAIRO_PATH_CLOSE_PATH:  0 points
 * </programlisting>
 *
 * The semantics and ordering of the coordinate values are consistent
 * with cairo_move_to(), cairo_line_to(), cairo_curve_to(), and
 * cairo_close_path().
 *
 * Here is sample code for iterating through a #cairo_path_t:
 *
 * <informalexample><programlisting>
 *      int i;
 *      cairo_path_t *path;
 *      cairo_path_data_t *data;
 * &nbsp;
 *      path = cairo_copy_path (cr);
 * &nbsp;
 *      for (i=0; i < path->num_data; i += path->data[i].header.length) {
 *          data = &amp;path->data[i];
 *          switch (data->header.type) {
 *          case CAIRO_PATH_MOVE_TO:
 *              do_move_to_things (data[1].point.x, data[1].point.y);
 *              break;
 *          case CAIRO_PATH_LINE_TO:
 *              do_line_to_things (data[1].point.x, data[1].point.y);
 *              break;
 *          case CAIRO_PATH_CURVE_TO:
 *              do_curve_to_things (data[1].point.x, data[1].point.y,
 *                                  data[2].point.x, data[2].point.y,
 *                                  data[3].point.x, data[3].point.y);
 *              break;
 *          case CAIRO_PATH_CLOSE_PATH:
 *              do_close_path_things ();
 *              break;
 *          }
 *      }
 *      cairo_path_destroy (path);
 * </programlisting></informalexample>
 *
 * As of cairo 1.4, cairo does not mind if there are more elements in
 * a portion of the path than needed.  Such elements can be used by
 * users of the cairo API to hold extra values in the path data
 * structure.  For this reason, it is recommended that applications
 * always use <literal>data->header.length</literal> to
 * iterate over the path data, instead of hardcoding the number of
 * elements for each element type.
 **/
typedef union _cairo_path_data_t cairo_path_data_t;
union _cairo_path_data_t {
    struct {
	cairo_path_data_type_t type;
	int length;
    } header;
    struct {
	double x, y;
    } point;
};

/**
 * cairo_path_t:
 * @status: the current error status
 * @data: the elements in the path
 * @num_data: the number of elements in the data array
 *
 * A data structure for holding a path. This data structure serves as
 * the return value for cairo_copy_path() and
 * cairo_copy_path_flat() as well the input value for
 * cairo_append_path().
 *
 * See #cairo_path_data_t for hints on how to iterate over the
 * actual data within the path.
 *
 * The num_data member gives the number of elements in the data
 * array. This number is larger than the number of independent path
 * portions (defined in #cairo_path_data_type_t), since the data
 * includes both headers and coordinates for each portion.
 **/
typedef struct cairo_path {
    cairo_status_t status;
    cairo_path_data_t *data;
    int num_data;
} cairo_path_t;

cairo_public cairo_path_t *
cairo_copy_path (cairo_t *cr);

cairo_public cairo_path_t *
cairo_copy_path_flat (cairo_t *cr);

cairo_public void
cairo_append_path (cairo_t		*cr,
		   const cairo_path_t	*path);

cairo_public void
cairo_path_destroy (cairo_path_t *path);

/* Error status queries */

cairo_public cairo_status_t
cairo_status (cairo_t *cr);

cairo_public const char *
cairo_status_to_string (cairo_status_t status);

/* Surface manipulation */

cairo_public cairo_surface_t *
cairo_surface_create_similar (cairo_surface_t  *other,
			      cairo_content_t	content,
			      int		width,
			      int		height);

cairo_public cairo_surface_t *
cairo_surface_reference (cairo_surface_t *surface);

cairo_public void
cairo_surface_finish (cairo_surface_t *surface);

cairo_public void
cairo_surface_destroy (cairo_surface_t *surface);

cairo_public unsigned int
cairo_surface_get_reference_count (cairo_surface_t *surface);

cairo_public cairo_status_t
cairo_surface_status (cairo_surface_t *surface);

/**
 * cairo_surface_type_t
 * @CAIRO_SURFACE_TYPE_IMAGE: The surface is of type image
 * @CAIRO_SURFACE_TYPE_PDF: The surface is of type pdf
 * @CAIRO_SURFACE_TYPE_PS: The surface is of type ps
 * @CAIRO_SURFACE_TYPE_XLIB: The surface is of type xlib
 * @CAIRO_SURFACE_TYPE_XCB: The surface is of type xcb
 * @CAIRO_SURFACE_TYPE_GLITZ: The surface is of type glitz
 * @CAIRO_SURFACE_TYPE_QUARTZ: The surface is of type quartz
 * @CAIRO_SURFACE_TYPE_WIN32: The surface is of type win32
 * @CAIRO_SURFACE_TYPE_BEOS: The surface is of type beos
 * @CAIRO_SURFACE_TYPE_DIRECTFB: The surface is of type directfb
 * @CAIRO_SURFACE_TYPE_SVG: The surface is of type svg
 * @CAIRO_SURFACE_TYPE_OS2: The surface is of type os2
 *
 * #cairo_surface_type_t is used to describe the type of a given
 * surface. The surface types are also known as "backends" or "surface
 * backends" within cairo.
 *
 * The type of a surface is determined by the function used to create
 * it, which will generally be of the form cairo_<emphasis>type</emphasis>_surface_create,
 * (though see cairo_surface_create_similar as well).
 *
 * The surface type can be queried with cairo_surface_get_type()
 *
 * The various cairo_surface functions can be used with surfaces of
 * any type, but some backends also provide type-specific functions
 * that must only be called with a surface of the appropriate
 * type. These functions have names that begin with
 * cairo_<emphasis>type</emphasis>_surface such as cairo_image_surface_get_width().
 *
 * The behavior of calling a type-specific function with a surface of
 * the wrong type is undefined.
 *
 * New entries may be added in future versions.
 *
 * Since: 1.2
 **/
typedef enum _cairo_surface_type {
    CAIRO_SURFACE_TYPE_IMAGE,
    CAIRO_SURFACE_TYPE_PDF,
    CAIRO_SURFACE_TYPE_PS,
    CAIRO_SURFACE_TYPE_XLIB,
    CAIRO_SURFACE_TYPE_XCB,
    CAIRO_SURFACE_TYPE_GLITZ,
    CAIRO_SURFACE_TYPE_QUARTZ,
    CAIRO_SURFACE_TYPE_WIN32,
    CAIRO_SURFACE_TYPE_BEOS,
    CAIRO_SURFACE_TYPE_DIRECTFB,
    CAIRO_SURFACE_TYPE_SVG,
    CAIRO_SURFACE_TYPE_OS2
} cairo_surface_type_t;

cairo_public cairo_surface_type_t
cairo_surface_get_type (cairo_surface_t *surface);

cairo_public cairo_content_t
cairo_surface_get_content (cairo_surface_t *surface);

#if CAIRO_HAS_PNG_FUNCTIONS

cairo_public cairo_status_t
cairo_surface_write_to_png (cairo_surface_t	*surface,
			    const char		*filename);

cairo_public cairo_status_t
cairo_surface_write_to_png_stream (cairo_surface_t	*surface,
				   cairo_write_func_t	write_func,
				   void			*closure);

#endif

cairo_public void *
cairo_surface_get_user_data (cairo_surface_t		 *surface,
			     const cairo_user_data_key_t *key);

cairo_public cairo_status_t
cairo_surface_set_user_data (cairo_surface_t		 *surface,
			     const cairo_user_data_key_t *key,
			     void			 *user_data,
			     cairo_destroy_func_t	 destroy);

cairo_public void
cairo_surface_get_font_options (cairo_surface_t      *surface,
				cairo_font_options_t *options);

cairo_public void
cairo_surface_flush (cairo_surface_t *surface);

cairo_public void
cairo_surface_mark_dirty (cairo_surface_t *surface);

cairo_public void
cairo_surface_mark_dirty_rectangle (cairo_surface_t *surface,
				    int              x,
				    int              y,
				    int              width,
				    int              height);

cairo_public void
cairo_surface_set_device_offset (cairo_surface_t *surface,
				 double           x_offset,
				 double           y_offset);

cairo_public void
cairo_surface_get_device_offset (cairo_surface_t *surface,
				 double          *x_offset,
				 double          *y_offset);

cairo_public void
cairo_surface_set_fallback_resolution (cairo_surface_t	*surface,
				       double		 x_pixels_per_inch,
				       double		 y_pixels_per_inch);

/* Image-surface functions */

/**
 * cairo_format_t
 * @CAIRO_FORMAT_ARGB32: each pixel is a 32-bit quantity, with
 *   alpha in the upper 8 bits, then red, then green, then blue.
 *   The 32-bit quantities are stored native-endian. Pre-multiplied
 *   alpha is used. (That is, 50% transparent red is 0x80800000,
 *   not 0x80ff0000.)
 * @CAIRO_FORMAT_RGB24: each pixel is a 32-bit quantity, with
 *   the upper 8 bits unused. Red, Green, and Blue are stored
 *   in the remaining 24 bits in that order.
 * @CAIRO_FORMAT_A8: each pixel is a 8-bit quantity holding
 *   an alpha value.
 * @CAIRO_FORMAT_A1: each pixel is a 1-bit quantity holding
 *   an alpha value. Pixels are packed together into 32-bit
 *   quantities. The ordering of the bits matches the
 *   endianess of the platform. On a big-endian machine, the
 *   first pixel is in the uppermost bit, on a little-endian
 *   machine the first pixel is in the least-significant bit.
 * @CAIRO_FORMAT_RGB16_565: This format value is deprecated. It has
 *   never been properly implemented in cairo and should not be used
 *   by applications. (since 1.2)
 *
 * #cairo_format_t is used to identify the memory format of
 * image data.
 *
 * New entries may be added in future versions.
 **/
typedef enum _cairo_format {
    CAIRO_FORMAT_ARGB32,
    CAIRO_FORMAT_RGB24,
    CAIRO_FORMAT_A8,
    CAIRO_FORMAT_A1
    /* The value of 4 is reserved by a deprecated enum value.
     * The next format added must have an explicit value of 5.
    CAIRO_FORMAT_RGB16_565 = 4,
    */
} cairo_format_t;

cairo_public cairo_surface_t *
cairo_image_surface_create (cairo_format_t	format,
			    int			width,
			    int			height);

cairo_public cairo_surface_t *
cairo_image_surface_create_for_data (unsigned char	       *data,
				     cairo_format_t		format,
				     int			width,
				     int			height,
				     int			stride);

cairo_public unsigned char *
cairo_image_surface_get_data (cairo_surface_t *surface);

cairo_public cairo_format_t
cairo_image_surface_get_format (cairo_surface_t *surface);

cairo_public int
cairo_image_surface_get_width (cairo_surface_t *surface);

cairo_public int
cairo_image_surface_get_height (cairo_surface_t *surface);

cairo_public int
cairo_image_surface_get_stride (cairo_surface_t *surface);

#if CAIRO_HAS_PNG_FUNCTIONS

cairo_public cairo_surface_t *
cairo_image_surface_create_from_png (const char	*filename);

cairo_public cairo_surface_t *
cairo_image_surface_create_from_png_stream (cairo_read_func_t	read_func,
					    void		*closure);

#endif

/* Pattern creation functions */

cairo_public cairo_pattern_t *
cairo_pattern_create_rgb (double red, double green, double blue);

cairo_public cairo_pattern_t *
cairo_pattern_create_rgba (double red, double green, double blue,
			   double alpha);

cairo_public cairo_pattern_t *
cairo_pattern_create_for_surface (cairo_surface_t *surface);

cairo_public cairo_pattern_t *
cairo_pattern_create_linear (double x0, double y0,
			     double x1, double y1);

cairo_public cairo_pattern_t *
cairo_pattern_create_radial (double cx0, double cy0, double radius0,
			     double cx1, double cy1, double radius1);

cairo_public cairo_pattern_t *
cairo_pattern_reference (cairo_pattern_t *pattern);

cairo_public void
cairo_pattern_destroy (cairo_pattern_t *pattern);

cairo_public unsigned int
cairo_pattern_get_reference_count (cairo_pattern_t *pattern);

cairo_public cairo_status_t
cairo_pattern_status (cairo_pattern_t *pattern);

cairo_public void *
cairo_pattern_get_user_data (cairo_pattern_t		 *pattern,
			     const cairo_user_data_key_t *key);

cairo_public cairo_status_t
cairo_pattern_set_user_data (cairo_pattern_t		 *pattern,
			     const cairo_user_data_key_t *key,
			     void			 *user_data,
			     cairo_destroy_func_t	  destroy);

/**
 * cairo_pattern_type_t
 * @CAIRO_PATTERN_TYPE_SOLID: The pattern is a solid (uniform)
 * color. It may be opaque or translucent.
 * @CAIRO_PATTERN_TYPE_SURFACE: The pattern is a based on a surface (an image).
 * @CAIRO_PATTERN_TYPE_LINEAR: The pattern is a linear gradient.
 * @CAIRO_PATTERN_TYPE_RADIAL: The pattern is a radial gradient.
 *
 * #cairo_pattern_type_t is used to describe the type of a given pattern.
 *
 * The type of a pattern is determined by the function used to create
 * it. The cairo_pattern_create_rgb() and cairo_pattern_create_rgba()
 * functions create SOLID patterns. The remaining
 * cairo_pattern_create functions map to pattern types in obvious
 * ways.
 *
 * The pattern type can be queried with cairo_pattern_get_type()
 *
 * Most cairo_pattern functions can be called with a pattern of any
 * type, (though trying to change the extend or filter for a solid
 * pattern will have no effect). A notable exception is
 * cairo_pattern_add_color_stop_rgb() and
 * cairo_pattern_add_color_stop_rgba() which must only be called with
 * gradient patterns (either LINEAR or RADIAL). Otherwise the pattern
 * will be shutdown and put into an error state.
 *
 * New entries may be added in future versions.
 *
 * Since: 1.2
 **/
typedef enum _cairo_pattern_type {
    CAIRO_PATTERN_TYPE_SOLID,
    CAIRO_PATTERN_TYPE_SURFACE,
    CAIRO_PATTERN_TYPE_LINEAR,
    CAIRO_PATTERN_TYPE_RADIAL
} cairo_pattern_type_t;

cairo_public cairo_pattern_type_t
cairo_pattern_get_type (cairo_pattern_t *pattern);

cairo_public void
cairo_pattern_add_color_stop_rgb (cairo_pattern_t *pattern,
				  double offset,
				  double red, double green, double blue);

cairo_public void
cairo_pattern_add_color_stop_rgba (cairo_pattern_t *pattern,
				   double offset,
				   double red, double green, double blue,
				   double alpha);

cairo_public void
cairo_pattern_set_matrix (cairo_pattern_t      *pattern,
			  const cairo_matrix_t *matrix);

cairo_public void
cairo_pattern_get_matrix (cairo_pattern_t *pattern,
			  cairo_matrix_t  *matrix);

/**
 * cairo_extend_t
 * @CAIRO_EXTEND_NONE: pixels outside of the source pattern
 *   are fully transparent
 * @CAIRO_EXTEND_REPEAT: the pattern is tiled by repeating
 * @CAIRO_EXTEND_REFLECT: the pattern is tiled by reflecting
 *   at the edges (not implemented for surface patterns currently)
 * @CAIRO_EXTEND_PAD: pixels outside of the pattern copy
 *   the closest pixel from the source (Since 1.2; not implemented
 *   for surface patterns currently)
 *
 * #cairo_extend_t is used to describe how the area outside
 * of a pattern will be drawn.
 *
 * New entries may be added in future versions.
 **/
typedef enum _cairo_extend {
    CAIRO_EXTEND_NONE,
    CAIRO_EXTEND_REPEAT,
    CAIRO_EXTEND_REFLECT,
    CAIRO_EXTEND_PAD
} cairo_extend_t;

cairo_public void
cairo_pattern_set_extend (cairo_pattern_t *pattern, cairo_extend_t extend);

cairo_public cairo_extend_t
cairo_pattern_get_extend (cairo_pattern_t *pattern);

typedef enum _cairo_filter {
    CAIRO_FILTER_FAST,
    CAIRO_FILTER_GOOD,
    CAIRO_FILTER_BEST,
    CAIRO_FILTER_NEAREST,
    CAIRO_FILTER_BILINEAR,
    CAIRO_FILTER_GAUSSIAN
} cairo_filter_t;

cairo_public void
cairo_pattern_set_filter (cairo_pattern_t *pattern, cairo_filter_t filter);

cairo_public cairo_filter_t
cairo_pattern_get_filter (cairo_pattern_t *pattern);

cairo_public cairo_status_t
cairo_pattern_get_rgba (cairo_pattern_t *pattern,
			double *red, double *green,
			double *blue, double *alpha);

cairo_public cairo_status_t
cairo_pattern_get_surface (cairo_pattern_t *pattern,
			   cairo_surface_t **surface);


cairo_public cairo_status_t
cairo_pattern_get_color_stop_rgba (cairo_pattern_t *pattern,
				   int index, double *offset,
				   double *red, double *green,
				   double *blue, double *alpha);

cairo_public cairo_status_t
cairo_pattern_get_color_stop_count (cairo_pattern_t *pattern,
				    int *count);

cairo_public cairo_status_t
cairo_pattern_get_linear_points (cairo_pattern_t *pattern,
				 double *x0, double *y0,
				 double *x1, double *y1);

cairo_public cairo_status_t
cairo_pattern_get_radial_circles (cairo_pattern_t *pattern,
				  double *x0, double *y0, double *r0,
				  double *x1, double *y1, double *r1);

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

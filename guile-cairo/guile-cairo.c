/* guile-cairo
 * Copyright (C) 2007, 2011, 2012, 2014 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo.c: Cairo for Guile
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

#include <libguile.h>

#include <cairo.h>

#if CAIRO_HAS_PDF_SURFACE
#include <cairo-pdf.h>
#endif /* CAIRO_HAS_PDF_SURFACE */

#if CAIRO_HAS_PS_SURFACE
#include <cairo-ps.h>
#endif /* CAIRO_HAS_PS_SURFACE */

#if CAIRO_HAS_SVG_SURFACE
#include <cairo-svg.h>
#endif  /* CAIRO_HAS_SVG_SURFACE */

#include "guile-cairo-compat.h"
#include "guile-cairo.h"

static cairo_user_data_key_t scm_cairo_key;

#if SCM_MAJOR_VERSION >= 2
#define UNPROTECT (cairo_destroy_func_t)scm_gc_unprotect_object
#else
/* We can't unprotect an object from within GC, so just leak memory.  */
#define UNPROTECT NULL
#endif

/* cairo_device_t checking return */
#define DCHKRET(sdv,ret)                                                \
  do {SCM _ret = ret;                                                   \
    scm_c_check_cairo_status (cairo_device_status                       \
                              (scm_to_cairo_device (sdv)), NULL);       \
    return _ret;                                                        \
  } while (0)
    
/* cairo_t constructor return */
#define CCONSRET(cr)                                                    \
  do {cairo_t *_ret = cr;                                               \
    scm_c_check_cairo_status (cairo_status (_ret), NULL);               \
    return scm_take_cairo (_ret);                                       \
  } while (0)

/* cairo_t checking return */
#define CCHKRET(scr,ret)                                                \
  do {SCM _ret = ret;                                                   \
    scm_c_check_cairo_status (cairo_status (scm_to_cairo (scr)), NULL); \
    return _ret;                                                        \
  } while (0)
    
/* cairo_font_options_t constructor return */
#define FOCONSRET(fo)                                                   \
  do {cairo_font_options_t *_ret = fo;                                  \
    scm_c_check_cairo_status (cairo_font_options_status (_ret), NULL);  \
    return scm_take_cairo_font_options (_ret);                          \
  } while (0)

/* cairo_font_options_t checking return */
#define FOCHKRET(sfo,ret)                                               \
  do {SCM _ret = ret;                                                   \
    scm_c_check_cairo_status (cairo_font_options_status                 \
                              (scm_to_cairo_font_options (sfo)), NULL); \
    return _ret;                                                        \
  } while (0)
    
/* cairo_font_face_t constructor return */
#define FFCONSRET(ff)                                                   \
  do {cairo_font_face_t *_ret = ff;                                     \
    scm_c_check_cairo_status (cairo_font_face_status (_ret), NULL);     \
    return scm_take_cairo_font_face (_ret);                             \
  } while (0)

/* cairo_font_face_t checking return */
#define FFCHKRET(sff,ret)                                               \
  do {SCM _ret = ret;                                                   \
    scm_c_check_cairo_status (cairo_font_face_status                    \
                              (scm_to_cairo_font_face (sff)), NULL);    \
    return _ret;                                                        \
  } while (0)
    
/* cairo_scaled_face_t constructor return */
#define SFCONSRET(sf)                                                   \
  do {cairo_scaled_font_t *_ret = sf;                                   \
    scm_c_check_cairo_status (cairo_scaled_font_status (_ret), NULL);   \
    return scm_take_cairo_scaled_font (_ret);                           \
  } while (0)

/* cairo_scaled_face_t checking return */
#define SFCHKRET(ssf,ret)                                               \
  do {SCM _ret = ret;                                                   \
    scm_c_check_cairo_status (cairo_scaled_font_status                  \
                              (scm_to_cairo_scaled_font (ssf)), NULL);  \
    return _ret;                                                        \
  } while (0)
    
/* cairo_path_t constructor return */
#define PCONSRET(p)                                                     \
  do {cairo_path_t *_ret = p;                                           \
    scm_c_check_cairo_status (_ret->status, NULL);                      \
    return scm_take_cairo_path (_ret);                                  \
  } while (0)

/* cairo_region_t constructor return */
#define RCONSRET(r)                                                     \
  do {cairo_region_t *_ret = r;                                         \
    scm_c_check_cairo_status (cairo_region_status (_ret), NULL);        \
    return scm_take_cairo_region (_ret);                                \
  } while (0)

/* cairo_region_t checking return */
#define RCHKRET(sr,exp)                                                 \
  do {SCM _ret = exp;                                                   \
    scm_c_check_cairo_status (cairo_region_status (scm_to_cairo_region (sr)), NULL); \
    return _ret;                                                        \
  } while (0)
    
/* cairo_surface_t constructor return */
#define SCONSRET(s)                                                     \
  do {cairo_surface_t *_ret = s;                                        \
    scm_c_check_cairo_status (cairo_surface_status (_ret), NULL);       \
    return scm_take_cairo_surface (_ret);                               \
  } while (0)

/* cairo_surface_t checking return */
#define SCHKRET(ss,exp)                                                 \
  do {SCM _ret = exp;                                                   \
    scm_c_check_cairo_status (cairo_surface_status (scm_to_cairo_surface (ss)), NULL); \
    return _ret;                                                        \
  } while (0)
    
/* cairo_pattern_t constructor return */
#define PATCONSRET(s)                                                   \
  do {cairo_pattern_t *_ret = s;                                        \
    scm_c_check_cairo_status (cairo_pattern_status (_ret), NULL);       \
    return scm_take_cairo_pattern (_ret);                               \
  } while (0)

/* cairo_pattern_t checking return */
#define PATCHKRET(ss,exp)                                               \
  do {SCM _ret = exp;                                                   \
    scm_c_check_cairo_status (cairo_pattern_status (scm_to_cairo_pattern (ss)), NULL); \
    return _ret;                                                        \
  } while (0)
    

struct stream_data {
  char *data;
  unsigned int len;
  SCM port;
};

static SCM
do_read (struct stream_data *data)
{
  size_t n;
  n = scm_c_read (data->port, data->data, data->len);
  return scm_from_bool (n == data->len);
}

static cairo_status_t
guile_cairo_read_func (void *closure, unsigned char *data, unsigned int length)
{
  struct stream_data sdata;
  SCM success;
    
  sdata.data = (char*)data;
  sdata.len = length;
  sdata.port = closure ? SCM_PACK (closure) : scm_current_input_port ();
    
  success = scm_c_catch (SCM_BOOL_T,
                         (scm_t_catch_body)do_read, &sdata,
                         scm_handle_by_message_noexit, "Guile-Cairo",
                         NULL, NULL);
  if (scm_is_true (success))
    return CAIRO_STATUS_SUCCESS;
  else
    return CAIRO_STATUS_WRITE_ERROR;
}

static SCM
do_write (struct stream_data *data)
{
  scm_lfwrite (data->data, data->len, data->port);
  return SCM_BOOL_T;
}

static cairo_status_t
guile_cairo_write_func (void *closure, const unsigned char *data, unsigned int length)
{
  struct stream_data sdata;
  SCM success;
    
  sdata.data = (char*)data;
  sdata.len = length;
  sdata.port = closure ? SCM_PACK (closure) : scm_current_output_port ();

  success = scm_c_catch (SCM_BOOL_T,
                         (scm_t_catch_body)do_write, &sdata,
                         scm_handle_by_message_noexit, "Guile-Cairo",
                         NULL, NULL);
  if (scm_is_true (success))
    return CAIRO_STATUS_SUCCESS;
  else
    return CAIRO_STATUS_WRITE_ERROR;
}

void
scm_c_check_cairo_status (cairo_status_t status, const char *subr)
{
  if (status == CAIRO_STATUS_SUCCESS)
    return;

  scm_error (scm_from_utf8_symbol ("cairo-error"),
             subr,
             cairo_status_to_string (status),
             SCM_EOL,
             scm_list_1 (scm_from_cairo_status (status)));
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
  return scm_from_utf8_string (cairo_version_string ());
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
SCM_DEFINE_PUBLIC (scm_cairo_device_get_type, "cairo-device-get-type", 1, 0, 0,
                   (SCM dev),
                   "")
{
  DCHKRET (dev, scm_from_cairo_device_type
           (cairo_device_get_type (scm_to_cairo_device (dev))));
}

SCM_DEFINE_PUBLIC (scm_cairo_device_acquire, "cairo-device-acquire", 1, 0, 0,
                   (SCM dev),
                   "")
{
  cairo_device_acquire (scm_to_cairo_device (dev));
  DCHKRET (dev, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_device_release, "cairo-device-release", 1, 0, 0,
                   (SCM dev),
                   "")
{
  cairo_device_release (scm_to_cairo_device (dev));
  DCHKRET (dev, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_device_flush, "cairo-device-flush", 1, 0, 0,
                   (SCM dev),
                   "")
{
  cairo_device_flush (scm_to_cairo_device (dev));
  DCHKRET (dev, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_device_finish, "cairo-device-finish", 1, 0, 0,
                   (SCM dev),
                   "")
{
  cairo_device_finish (scm_to_cairo_device (dev));
  DCHKRET (dev, SCM_UNSPECIFIED);
}

#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_device_get_reference_count, "cairo-device-get-reference-count", 1, 0, 0,
                   (SCM dev),
                   "")
{
  CCHKRET (dev, scm_from_uint (cairo_device_get_reference_count (scm_to_cairo_device (dev))));
}
#endif
#endif /* 1.10 */

SCM_DEFINE_PUBLIC (scm_cairo_create, "cairo-create", 1, 0, 0,
                   (SCM surf),
                   "")
{
  CCONSRET (cairo_create (scm_to_cairo_surface (surf)));
}

SCM_DEFINE_PUBLIC (scm_cairo_destroy, "cairo-destroy", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  scm_release_cairo (ctx);
  return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_destroy, "cairo-surface-destroy", 1, 0, 0,
                   (SCM surf),
                   "")
{
  scm_release_cairo_surface (surf);
  return SCM_UNSPECIFIED;
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_get_reference_count, "cairo-get-reference-count", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, scm_from_uint (cairo_get_reference_count (scm_to_cairo (ctx))));
}
#endif
#endif

/* not wrapping the user_data because I need to mail the guile list,
 * possibility of calling gc_unprotect during gc */

SCM_DEFINE_PUBLIC (scm_cairo_save, "cairo-save", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_save (scm_to_cairo (ctx));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_restore, "cairo-restore", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_restore (scm_to_cairo (ctx));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_push_group, "cairo-push-group", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_push_group (scm_to_cairo (ctx));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_push_group_with_context, "cairo-push-group-with-context", 2, 0, 0,
                   (SCM ctx, SCM content),
                   "")
{
  cairo_push_group_with_content (scm_to_cairo (ctx),
                                 scm_to_cairo_content (content));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_pop_group, "cairo-pop-group", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, scm_from_cairo_pattern (cairo_pop_group (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_pop_group_to_source, "cairo-pop-group-to-source", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_pop_group_to_source (scm_to_cairo (ctx));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_operator, "cairo-set-operator", 2, 0, 0,
                   (SCM ctx, SCM op),
                   "")
{
  cairo_set_operator (scm_to_cairo (ctx),
                      scm_to_cairo_operator (op));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_source, "cairo-set-source", 2, 0, 0,
                   (SCM ctx, SCM pat),
                   "")
{
  cairo_set_source (scm_to_cairo (ctx),
                    scm_to_cairo_pattern (pat));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_source_rgb, "cairo-set-source-rgb", 4, 0, 0,
                   (SCM ctx, SCM r, SCM g, SCM b),
                   "")
{
  cairo_set_source_rgb (scm_to_cairo (ctx),
                        scm_to_double (r),
                        scm_to_double (g),
                        scm_to_double (b));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_source_rgba, "cairo-set-source-rgba", 5, 0, 0,
                   (SCM ctx, SCM r, SCM g, SCM b, SCM a),
                   "")
{
  cairo_set_source_rgba (scm_to_cairo (ctx),
                         scm_to_double (r),
                         scm_to_double (g),
                         scm_to_double (b),
                         scm_to_double (a));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_source_surface, "cairo-set-source-surface", 4, 0, 0,
                   (SCM ctx, SCM surf, SCM x, SCM y),
                   "")
{
  cairo_set_source_surface (scm_to_cairo (ctx),
                            scm_to_cairo_surface (surf),
                            scm_to_double (x),
                            scm_to_double (y));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_tolerance, "cairo-set-tolerance", 2, 0, 0,
                   (SCM ctx, SCM tolerance),
                   "")
{
  cairo_set_tolerance (scm_to_cairo (ctx),
                       scm_to_double (tolerance));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_antialias, "cairo-set-antialias", 2, 0, 0,
                   (SCM ctx, SCM antialias),
                   "")
{
  cairo_set_antialias (scm_to_cairo (ctx),
                       scm_to_cairo_antialias (antialias));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_fill_rule, "cairo-set-fill-rule", 2, 0, 0,
                   (SCM ctx, SCM fill_rule),
                   "")
{
  cairo_set_fill_rule (scm_to_cairo (ctx),
                       scm_to_cairo_fill_rule (fill_rule));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_line_width, "cairo-set-line-width", 2, 0, 0,
                   (SCM ctx, SCM line_width),
                   "")
{
  cairo_set_line_width (scm_to_cairo (ctx),
                        scm_to_double (line_width));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_line_cap, "cairo-set-line-cap", 2, 0, 0,
                   (SCM ctx, SCM line_cap),
                   "")
{
  cairo_set_line_cap (scm_to_cairo (ctx),
                      scm_to_cairo_line_cap (line_cap));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_line_join, "cairo-set-line-join", 2, 0, 0,
                   (SCM ctx, SCM line_join),
                   "")
{
  cairo_set_line_join (scm_to_cairo (ctx),
                       scm_to_cairo_line_join (line_join));
  CCHKRET (ctx, SCM_UNSPECIFIED);
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
    
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_miter_limit, "cairo-set-miter-limit", 2, 0, 0,
                   (SCM ctx, SCM limit),
                   "")
{
  cairo_set_miter_limit (scm_to_cairo (ctx),
                         scm_to_double (limit));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_translate, "cairo-translate", 3, 0, 0,
                   (SCM ctx, SCM tx, SCM ty),
                   "")
{
  cairo_translate (scm_to_cairo (ctx),
                   scm_to_double (tx), scm_to_double (ty));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_scale, "cairo-scale", 3, 0, 0,
                   (SCM ctx, SCM sx, SCM sy),
                   "")
{
  cairo_scale (scm_to_cairo (ctx),
               scm_to_double (sx), scm_to_double (sy));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_rotate, "cairo-rotate", 2, 0, 0,
                   (SCM ctx, SCM radians),
                   "")
{
  cairo_rotate (scm_to_cairo (ctx),
                scm_to_double (radians));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_transform, "cairo-transform", 2, 0, 0,
                   (SCM ctx, SCM smatrix),
                   "")
{
  cairo_matrix_t matrix;
  scm_fill_cairo_matrix (smatrix, &matrix);
    
  cairo_transform (scm_to_cairo (ctx), &matrix);

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_matrix, "cairo-set-matrix", 2, 0, 0,
                   (SCM ctx, SCM smatrix),
                   "")
{
  cairo_matrix_t matrix;
  scm_fill_cairo_matrix (smatrix, &matrix);
    
  cairo_set_matrix (scm_to_cairo (ctx), &matrix);

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_identity_matrix, "cairo-identity-matrix", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_identity_matrix (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_user_to_device, "cairo-user-to-device", 3, 0, 0,
                   (SCM ctx, SCM sx, SCM sy),
                   "")
{
  double x, y;
    
  x = scm_to_double (sx);
  y = scm_to_double (sy);

  cairo_user_to_device (scm_to_cairo (ctx), &x, &y);

  CCHKRET (ctx, scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y))));
}

SCM_DEFINE_PUBLIC (scm_cairo_user_to_device_distance, "cairo-user-to-device-distance", 3, 0, 0,
                   (SCM ctx, SCM sx, SCM sy),
                   "")
{
  double x, y;
    
  x = scm_to_double (sx);
  y = scm_to_double (sy);

  cairo_user_to_device_distance (scm_to_cairo (ctx), &x, &y);

  CCHKRET (ctx, scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y))));
}

SCM_DEFINE_PUBLIC (scm_cairo_device_to_user, "cairo-device-to-user", 3, 0, 0,
                   (SCM ctx, SCM sx, SCM sy),
                   "")
{
  double x, y;
    
  x = scm_to_double (sx);
  y = scm_to_double (sy);

  cairo_device_to_user (scm_to_cairo (ctx), &x, &y);

  CCHKRET (ctx, scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y))));
}

SCM_DEFINE_PUBLIC (scm_cairo_device_to_user_distance, "cairo-device-to-user-distance", 3, 0, 0,
                   (SCM ctx, SCM sx, SCM sy),
                   "")
{
  double x, y;
    
  x = scm_to_double (sx);
  y = scm_to_double (sy);

  cairo_device_to_user_distance (scm_to_cairo (ctx), &x, &y);

  CCHKRET (ctx, scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y))));
}

SCM_DEFINE_PUBLIC (scm_cairo_new_path, "cairo-new-path", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_new_path (scm_to_cairo (ctx));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_move_to, "cairo-move-to", 3, 0, 0,
                   (SCM ctx, SCM x, SCM y),
                   "")
{
  cairo_move_to (scm_to_cairo (ctx),
                 scm_to_double (x), scm_to_double (y));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_new_sub_path, "cairo-new-sub-path", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_new_sub_path (scm_to_cairo (ctx));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_line_to, "cairo-line-to", 3, 0, 0,
                   (SCM ctx, SCM x, SCM y),
                   "")
{
  cairo_line_to (scm_to_cairo (ctx),
                 scm_to_double (x), scm_to_double (y));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_curve_to, "cairo-curve-to", 7, 0, 0,
                   (SCM ctx, SCM x1, SCM y1, SCM x2, SCM y2, SCM x3, SCM y3),
                   "")
{
  cairo_curve_to (scm_to_cairo (ctx),
                  scm_to_double (x1), scm_to_double (y1),
                  scm_to_double (x2), scm_to_double (y2),
                  scm_to_double (x3), scm_to_double (y3));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_arc, "cairo-arc", 6, 0, 0,
                   (SCM ctx, SCM xc, SCM yc, SCM radius, SCM angle1, SCM angle2),
                   "")
{
  cairo_arc (scm_to_cairo (ctx),
             scm_to_double (xc), scm_to_double (yc),
             scm_to_double (radius),
             scm_to_double (angle1), scm_to_double (angle2));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_arc_negative, "cairo-arc-negative", 6, 0, 0,
                   (SCM ctx, SCM xc, SCM yc, SCM radius, SCM angle1, SCM angle2),
                   "")
{
  cairo_arc_negative (scm_to_cairo (ctx),
                      scm_to_double (xc), scm_to_double (yc),
                      scm_to_double (radius),
                      scm_to_double (angle1), scm_to_double (angle2));
  CCHKRET (ctx, SCM_UNSPECIFIED);

}

SCM_DEFINE_PUBLIC (scm_cairo_rel_move_to, "cairo-rel-move-to", 3, 0, 0,
                   (SCM ctx, SCM x, SCM y),
                   "")
{
  cairo_rel_move_to (scm_to_cairo (ctx),
                     scm_to_double (x), scm_to_double (y));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_rel_line_to, "cairo-rel-line-to", 3, 0, 0,
                   (SCM ctx, SCM x, SCM y),
                   "")
{
  cairo_rel_line_to (scm_to_cairo (ctx),
                     scm_to_double (x), scm_to_double (y));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_rel_curve_to, "cairo-rel-curve-to", 7, 0, 0,
                   (SCM ctx, SCM x1, SCM y1, SCM x2, SCM y2, SCM x3, SCM y3),
                   "")
{
  cairo_rel_curve_to (scm_to_cairo (ctx),
                      scm_to_double (x1), scm_to_double (y1),
                      scm_to_double (x2), scm_to_double (y2),
                      scm_to_double (x3), scm_to_double (y3));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_rectangle, "cairo-rectangle", 5, 0, 0,
                   (SCM ctx, SCM x, SCM y, SCM width, SCM height),
                   "")
{
  cairo_rectangle (scm_to_cairo (ctx),
                   scm_to_double (x), scm_to_double (y),
                   scm_to_double (width), scm_to_double (height));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_close_path, "cairo-close-path", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_close_path (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_paint, "cairo-paint", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_paint (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_paint_with_alpha, "cairo-paint-with-alpha", 2, 0, 0,
                   (SCM ctx, SCM alpha),
                   "")
{
  cairo_paint_with_alpha (scm_to_cairo (ctx),
                          scm_to_double (alpha));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_mask, "cairo-mask", 2, 0, 0,
                   (SCM ctx, SCM pat),
                   "")
{
  cairo_mask (scm_to_cairo (ctx),
              scm_to_cairo_pattern (pat));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_mask_surface, "cairo-mask-surface", 2, 0, 0,
                   (SCM ctx, SCM surf, SCM x, SCM y),
                   "")
{
  cairo_mask_surface (scm_to_cairo (ctx),
                      scm_to_cairo_surface (surf),
                      scm_to_double (x), scm_to_double (y));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_stroke, "cairo-stroke", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_stroke (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_stroke_preserve, "cairo-stroke-preserve", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_stroke_preserve (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_fill, "cairo-fill", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_fill (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_fill_preserve, "cairo-fill-preserve", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_fill_preserve (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_copy_page, "cairo-copy-page", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_copy_page (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_show_page, "cairo-show-page", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_show_page (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_in_stroke, "cairo-in-stroke", 3, 0, 0,
                   (SCM ctx, SCM x, SCM y),
                   "")
{
  CCHKRET (ctx,
           scm_from_bool (cairo_in_stroke (scm_to_cairo (ctx),
                                           scm_to_double (x),
                                           scm_to_double (y))));
}

SCM_DEFINE_PUBLIC (scm_cairo_in_fill, "cairo-in-fill", 3, 0, 0,
                   (SCM ctx, SCM x, SCM y),
                   "")
{
  CCHKRET (ctx,
           scm_from_bool (cairo_in_fill (scm_to_cairo (ctx),
                                         scm_to_double (x),
                                         scm_to_double (y))));
}

SCM_DEFINE_PUBLIC (scm_cairo_stroke_extents, "cairo-stroke-extents", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  double x1, y1, x2, y2;
    
  cairo_stroke_extents (scm_to_cairo (ctx), &x1, &y1, &x2, &y2);

  CCHKRET (ctx,
           scm_values (scm_list_4 (scm_from_double (x1),
                                   scm_from_double (y1),
                                   scm_from_double (x2),
                                   scm_from_double (y2))));
}

SCM_DEFINE_PUBLIC (scm_cairo_fill_extents, "cairo-fill-extents", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  double x1, y1, x2, y2;
    
  cairo_fill_extents (scm_to_cairo (ctx), &x1, &y1, &x2, &y2);

  CCHKRET (ctx,
           scm_values (scm_list_4 (scm_from_double (x1),
                                   scm_from_double (y1),
                                   scm_from_double (x2),
                                   scm_from_double (y2))));
}

SCM_DEFINE_PUBLIC (scm_cairo_reset_clip, "cairo-reset-clip", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_reset_clip (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_clip, "cairo-clip", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_clip (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_clip_preserve, "cairo-clip-preserve", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_clip_preserve (scm_to_cairo (ctx));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
SCM_DEFINE_PUBLIC (scm_cairo_clip_extents, "cairo-clip-extents", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  double x1, y1, x2, y2;
    
  cairo_clip_extents (scm_to_cairo (ctx), &x1, &y1, &x2, &y2);

  CCHKRET (ctx,
           scm_values (scm_list_4 (scm_from_double (x1),
                                   scm_from_double (y1),
                                   scm_from_double (x2),
                                   scm_from_double (y2))));
}

SCM_DEFINE_PUBLIC (scm_cairo_copy_clip_rectangle_list, "cairo-copy-clip-rectangle-list", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_rectangle_list_t *rlist;
  SCM ret = SCM_EOL;
  int i, len;
    
  rlist = cairo_copy_clip_rectangle_list (scm_to_cairo (ctx));
  scm_c_check_cairo_status (rlist->status, s_scm_cairo_copy_clip_rectangle_list);
    
  len = rlist->num_rectangles;
  for (i = len - 1; i >= 0; i--)
    ret = scm_cons (scm_from_cairo_rectangle (&rlist->rectangles[i]), ret);
  cairo_rectangle_list_destroy (rlist);

  return ret;
}
#endif /* HAVE_CAIRO_140_API */

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
SCM_DEFINE_PUBLIC (scm_cairo_in_clip, "cairo-in-clip", 3, 0, 0,
                   (SCM ctx, SCM x, SCM y),
                   "")
{
  CCHKRET (ctx,
           scm_from_bool (cairo_in_clip (scm_to_cairo (ctx),
                                         scm_to_double (x),
                                         scm_to_double (y))));
}
#endif /* 1.10 */

SCM_DEFINE_PUBLIC (scm_cairo_font_options_create, "cairo-font-options-create", 0, 0, 0,
                   (void),
                   "")
{
  FOCONSRET (cairo_font_options_create ());
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_copy, "cairo-font-options-copy", 1, 0, 0,
                   (SCM fopt),
                   "")
{
  cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
  FOCONSRET (cairo_font_options_copy (opt));
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_merge, "cairo-font-options-merge", 2, 0, 0,
                   (SCM fopt, SCM other),
                   "")
{
  cairo_font_options_merge (scm_to_cairo_font_options (fopt),
                            scm_to_cairo_font_options (other));
  FOCHKRET (fopt, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_hash, "cairo-font-options-hash", 1, 0, 0,
                   (SCM fopt),
                   "")
{
  FOCHKRET (fopt,
            scm_from_ulong (cairo_font_options_hash (scm_to_cairo_font_options (fopt))));
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_set_antialias, "cairo-font-options-set-antialias", 2, 0, 0,
                   (SCM fopt, SCM val),
                   "")
{
  cairo_font_options_set_antialias (scm_to_cairo_font_options (fopt),
                                    scm_to_cairo_antialias (val));
  FOCHKRET (fopt, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_get_antialias, "cairo-font-options-get-antialias", 1, 0, 0,
                   (SCM fopt),
                   "")
{
  cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
  FOCHKRET (fopt,
            scm_from_cairo_antialias (cairo_font_options_get_antialias (opt)));
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_set_subpixel_order, "cairo-font-options-set-subpixel-order", 2, 0, 0,
                   (SCM fopt, SCM val),
                   "")
{
  cairo_font_options_set_subpixel_order (scm_to_cairo_font_options (fopt),
                                         scm_to_cairo_subpixel_order (val));
  FOCHKRET (fopt, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_get_subpixel_order, "cairo-font-options-get-subpixel-order", 1, 0, 0,
                   (SCM fopt),
                   "")
{
  cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
  FOCHKRET (fopt,
            scm_from_cairo_subpixel_order (cairo_font_options_get_subpixel_order (opt)));
}


SCM_DEFINE_PUBLIC (scm_cairo_font_options_set_hint_style, "cairo-font-options-set-hint-style", 2, 0, 0,
                   (SCM fopt, SCM val),
                   "")
{
  cairo_font_options_set_hint_style (scm_to_cairo_font_options (fopt),
                                     scm_to_cairo_hint_style (val));
  FOCHKRET (fopt, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_get_hint_style, "cairo-font-options-get-hint-style", 1, 0, 0,
                   (SCM fopt),
                   "")
{
  cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
  FOCHKRET (fopt,
            scm_from_cairo_hint_style (cairo_font_options_get_hint_style (opt)));
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_set_hint_metrics, "cairo-font-options-set-hint-metrics", 2, 0, 0,
                   (SCM fopt, SCM val),
                   "")
{
  cairo_font_options_set_hint_metrics (scm_to_cairo_font_options (fopt),
                                       scm_to_cairo_hint_metrics (val));
  FOCHKRET (fopt, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_font_options_get_hint_metrics, "cairo-font-options-get-hint-metrics", 1, 0, 0,
                   (SCM fopt),
                   "")
{
  cairo_font_options_t *opt = scm_to_cairo_font_options (fopt);
  FOCHKRET (fopt, 
            scm_from_cairo_hint_metrics (cairo_font_options_get_hint_metrics (opt)));
}

SCM_DEFINE_PUBLIC (scm_cairo_select_font_face, "cairo-select-font-face", 4, 0, 0,
                   (SCM ctx, SCM sfamily, SCM slant, SCM weight),
                   "")
{
  char *family;

  scm_dynwind_begin (0);
  family = scm_to_utf8_string (sfamily);
  scm_dynwind_free (family);

  cairo_select_font_face (scm_to_cairo (ctx), family,
                          scm_to_cairo_font_slant (slant),
                          scm_to_cairo_font_weight (weight));

  scm_dynwind_end ();

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_font_size, "cairo-set-font-size", 2, 0, 0,
                   (SCM ctx, SCM size),
                   "")
{
  cairo_set_font_size (scm_to_cairo (ctx),
                       scm_to_double (size));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_font_matrix, "cairo-set-font-matrix", 2, 0, 0,
                   (SCM ctx, SCM smatrix),
                   "")
{
  cairo_matrix_t matrix;

  scm_fill_cairo_matrix (smatrix, &matrix);
  cairo_set_font_matrix (scm_to_cairo (ctx), &matrix);

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_get_font_matrix, "cairo-get-font-matrix", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_matrix_t matrix;

  cairo_get_font_matrix (scm_to_cairo (ctx), &matrix);

  CCHKRET (ctx, scm_from_cairo_matrix (&matrix));
}
 
SCM_DEFINE_PUBLIC (scm_cairo_set_font_options, "cairo-set-font-options", 2, 0, 0,
                   (SCM ctx, SCM fopts),
                   "")
{
  cairo_set_font_options (scm_to_cairo (ctx),
                          scm_to_cairo_font_options (fopts));

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_get_font_options, "cairo-get-font-options", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_font_options_t *opts = cairo_font_options_create ();
  cairo_get_font_options (scm_to_cairo (ctx), opts);
  FOCONSRET (opts);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_font_face, "cairo-set-font-face", 2, 0, 0,
                   (SCM ctx, SCM face),
                   "")
{
  cairo_set_font_face (scm_to_cairo (ctx),
                       scm_to_cairo_font_face (face));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_get_font_face, "cairo-get-font-face", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  SCM ret = scm_from_cairo_font_face (cairo_get_font_face (scm_to_cairo (ctx)));
  FFCHKRET (ret, ret);
}

SCM_DEFINE_PUBLIC (scm_cairo_set_scaled_font, "cairo-set-scaled-font", 2, 0, 0,
                   (SCM ctx, SCM face),
                   "")
{
  cairo_set_scaled_font (scm_to_cairo (ctx),
                         scm_to_cairo_scaled_font (face));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
SCM_DEFINE_PUBLIC (scm_cairo_get_scaled_font, "cairo-get-scaled-font", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  SCM ret = scm_from_cairo_scaled_font (cairo_get_scaled_font (scm_to_cairo (ctx)));
  SFCHKRET (ret, ret);
}
#endif

SCM_DEFINE_PUBLIC (scm_cairo_show_text, "cairo-show-text", 2, 0, 0,
                   (SCM ctx, SCM val),
                   "")
{
  char *text;

  scm_dynwind_begin (0); 
  text = scm_to_utf8_string (val);
  scm_dynwind_free (text);

  cairo_show_text (scm_to_cairo (ctx), text);

  scm_dynwind_end ();

  CCHKRET (ctx, SCM_UNSPECIFIED);
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,8,0)

SCM_DEFINE_PUBLIC (scm_cairo_show_text_glyphs, "cairo-show-text-glyphs", 4, 0, 0,
                   (SCM ctx, SCM str, SCM glyphs, SCM clusters),
                   "")
{
  char *utf8;
  size_t utf8_len;
  int n_glyphs, n_clusters, i;
  cairo_glyph_t *glyphv;
  cairo_text_cluster_t *clusterv;

  scm_dynwind_begin (0); 

  utf8 = scm_to_utf8_stringn (str, &utf8_len);
  scm_dynwind_free (utf8);

  n_glyphs = scm_ilength (glyphs);
  if (n_glyphs < 0)
    scm_error (scm_from_utf8_symbol ("cairo-error"),
               NULL,
               "Glyphs should be a list of glyphs: ~S",
               scm_list_1 (glyphs),
               SCM_EOL);
  glyphv = scm_malloc (sizeof (*glyphv) * n_glyphs);
  scm_dynwind_free (glyphv);

  n_clusters = scm_ilength (clusters);
  if (n_clusters < 0)
    scm_error (scm_from_utf8_symbol ("cairo-error"),
               NULL,
               "Clusters should be a list of pairs: ~S",
               scm_list_1 (clusters),
               SCM_EOL);
  clusterv = scm_malloc (sizeof (*clusterv) * n_clusters);
  scm_dynwind_free (clusterv);
  
  for (i = 0; scm_is_pair (glyphs); i++, glyphs = scm_cdr (glyphs))
    scm_fill_cairo_glyph (scm_car (glyphs), &glyphv[i]);

  scm_fill_cairo_text_clusters (str, clusters, clusterv);

  cairo_show_text_glyphs (scm_to_cairo (ctx), utf8, utf8_len,
                          glyphv, n_glyphs, clusterv, n_clusters, 0);

  scm_dynwind_end ();

  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_text_to_glyphs, "cairo-scaled-font-text-to-glyphs", 5, 0, 0,
                   (SCM sf, SCM x, SCM y, SCM str, SCM clusters_p),
                   "")
{
  char *utf8;
  size_t utf8_len;
  int n_glyphs = 0, n_clusters = 0;
  cairo_glyph_t *glyphv = NULL;
  cairo_text_cluster_t *clusterv = NULL;
  cairo_text_cluster_flags_t cluster_flags = 0;
  cairo_status_t status;
  SCM glyphs, clusters;

  scm_dynwind_begin (0); 

  utf8 = scm_to_utf8_stringn (str, &utf8_len);
  scm_dynwind_free (utf8);

  status = cairo_scaled_font_text_to_glyphs
    (scm_to_cairo_scaled_font (sf),
     scm_to_double (x), scm_to_double (y),
     utf8, utf8_len,
     &glyphv, &n_glyphs,
     scm_is_true (clusters_p) ? &clusterv : NULL,
     scm_is_true (clusters_p) ? &n_clusters : NULL,
     scm_is_true (clusters_p) ? &cluster_flags : NULL);

  scm_dynwind_end ();

  scm_c_check_cairo_status (status, s_scm_cairo_scaled_font_text_to_glyphs);

  glyphs = SCM_EOL;
  while (n_glyphs--)
    glyphs = scm_cons (scm_from_cairo_glyph (&glyphv[n_glyphs]), glyphs);
  cairo_glyph_free (glyphv);
  
  if (scm_is_false (clusters_p))
    return glyphs;
  
  clusters = scm_from_cairo_text_clusters (str, clusterv, n_clusters,
                                           cluster_flags);
  cairo_text_cluster_free (clusterv);
  
  return scm_values (scm_list_2 (glyphs, clusters));
}

#endif /* 1.8 */

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
    
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_text_path, "cairo-text-path", 2, 0, 0,
                   (SCM ctx, SCM val),
                   "")
{
  char *text;

  scm_dynwind_begin (0); 
  text = scm_to_utf8_string (val);
  scm_dynwind_free (text);

  cairo_text_path (scm_to_cairo (ctx), text);

  scm_dynwind_end ();

  CCHKRET (ctx, SCM_UNSPECIFIED);
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
    
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_text_extents, "cairo-text-extents", 2, 0, 0,
                   (SCM ctx, SCM val),
                   "")
{
  SCM ret;
  char *text;
  cairo_text_extents_t extents;

  scm_dynwind_begin (0); 
  text = scm_to_utf8_string (val);
  scm_dynwind_free (text);

  cairo_text_extents (scm_to_cairo (ctx), text, &extents);
  ret = scm_from_cairo_text_extents (&extents);

  scm_dynwind_end ();

  CCHKRET (ctx, ret);
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
    
  CCHKRET (ctx, ret);
}

SCM_DEFINE_PUBLIC (scm_cairo_font_extents, "cairo-font-extents", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  SCM ret;
  cairo_font_extents_t extents;

  cairo_font_extents (scm_to_cairo (ctx), &extents);
  ret = scm_from_cairo_font_extents (&extents);

  CCHKRET (ctx, ret);
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_font_face_get_reference_count, "cairo-font-face-get-reference-count", 1, 0, 0,
                   (SCM face),
                   "")
{
  return scm_from_uint (cairo_font_face_get_reference_count (scm_to_cairo_font_face (face)));
}
#endif
#endif

SCM_DEFINE_PUBLIC (scm_cairo_font_face_get_type, "cairo-font-face-get-type", 1, 0, 0,
                   (SCM face),
                   "")
{
  FFCHKRET (face, scm_from_cairo_font_type (cairo_font_face_get_type (scm_to_cairo_font_face (face))));
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_create, "cairo-scaled-font-create", 3, 0, 0,
                   (SCM face, SCM smatrix, SCM sctm, SCM options),
                   "")
{
  cairo_matrix_t matrix, ctm;

  scm_fill_cairo_matrix (smatrix, &matrix);
  scm_fill_cairo_matrix (sctm, &ctm);
    
  SFCONSRET (cairo_scaled_font_create (scm_to_cairo_font_face (face),
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
  SFCHKRET (font, scm_from_cairo_font_type (cairo_scaled_font_get_type (scm_to_cairo_scaled_font (font))));
}


SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_extents, "cairo-scaled-font-extents", 1, 0, 0,
                   (SCM font),
                   "")
{
  SCM ret;
  cairo_font_extents_t extents;

  cairo_scaled_font_extents (scm_to_cairo_scaled_font (font), &extents);
  ret = scm_from_cairo_font_extents (&extents);

  SFCHKRET (font, ret);
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_text_extents, "cairo-scaled-font-text-extents", 2, 0, 0,
                   (SCM font, SCM val),
                   "")
{
  SCM ret;
  char *text;
  cairo_text_extents_t extents;

  scm_dynwind_begin (0); 
  text = scm_to_utf8_string (val);
  scm_dynwind_free (text);

  cairo_scaled_font_text_extents (scm_to_cairo_scaled_font (font), text, &extents);
  ret = scm_from_cairo_text_extents (&extents);

  scm_dynwind_end ();

  SFCHKRET (font, ret);
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
    
  SFCHKRET (font, ret);
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_font_face, "cairo-scaled-font-get-font-face", 1, 0, 0,
                   (SCM font),
                   "")
{
  SFCHKRET (font, 
            scm_from_cairo_font_face (cairo_scaled_font_get_font_face (scm_to_cairo_scaled_font (font))));
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_font_matrix, "cairo-scaled-font-get-font-matrix", 1, 0, 0,
                   (SCM font),
                   "")
{
  cairo_matrix_t matrix;

  cairo_scaled_font_get_font_matrix (scm_to_cairo_scaled_font (font), &matrix);

  SFCHKRET (font, scm_from_cairo_matrix (&matrix));
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_ctm, "cairo-scaled-font-get-ctm", 1, 0, 0,
                   (SCM font),
                   "")
{
  cairo_matrix_t matrix;

  cairo_scaled_font_get_ctm (scm_to_cairo_scaled_font (font), &matrix);

  SFCHKRET (font, scm_from_cairo_matrix (&matrix));
}

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_font_options, "cairo-scaled-font-get-font-options", 1, 0, 0,
                   (SCM font),
                   "")
{
  cairo_font_options_t *opts = cairo_font_options_create ();

  cairo_scaled_font_get_font_options (scm_to_cairo_scaled_font (font), opts);

  FOCONSRET (opts);
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,8,0)

SCM_DEFINE_PUBLIC (scm_cairo_scaled_font_get_scale_matrix, "cairo-scaled-font-get-scale_matrix", 1, 0, 0,
                   (SCM font),
                   "")
{
  cairo_matrix_t matrix;

  cairo_scaled_font_get_scale_matrix (scm_to_cairo_scaled_font (font), &matrix);

  SFCHKRET (font, scm_from_cairo_matrix (&matrix));
}

#endif /* 1.8 */

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,8,0)

/* User fonts.  */

enum user_font_func_idx
{
  USER_FONT_INIT,
  USER_FONT_RENDER_GLYPH,
  USER_FONT_UNICODE_TO_GLYPH,
  USER_FONT_TEXT_TO_GLYPHS,
  USER_FONT_N_FUNCS
};

static void
set_user_font_func (cairo_font_face_t *font_face,
                    enum user_font_func_idx idx, SCM func)
{
  void *data = cairo_font_face_get_user_data (font_face, &scm_cairo_key);

  if (!data)
    {
      data = SCM2PTR (scm_gc_protect_object
                      (scm_c_make_vector (USER_FONT_N_FUNCS, SCM_BOOL_F)));
      cairo_font_face_set_user_data (font_face, &scm_cairo_key,
                                     data, UNPROTECT);
    }

  scm_c_vector_set_x (PTR2SCM (data), idx, func);
}

static SCM
get_user_font_func (cairo_scaled_font_t *scaled_font,
                    enum user_font_func_idx idx)
{
  void *data = cairo_font_face_get_user_data
    (cairo_scaled_font_get_font_face (scaled_font), &scm_cairo_key);

  if (!data)
    return SCM_BOOL_F;
  else
    return scm_c_vector_ref (PTR2SCM (data), idx);
}

struct user_font_data {
  cairo_status_t ret;
  enum user_font_func_idx func;
  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  cairo_font_extents_t *fexts;
  cairo_text_extents_t *texts;
  unsigned long unicode;
  unsigned long glyph;
  const char *utf8;
  int utf8_len;
  cairo_glyph_t **glyphs;
  int *num_glyphs;
  cairo_text_cluster_t **clusters;
  int *num_clusters;
  cairo_text_cluster_flags_t *cluster_flags;
};

static void*
do_user_font (void *user_data)
{
  struct user_font_data *data = user_data;
  SCM func = get_user_font_func (data->scaled_font, data->func);
  SCM font = scm_from_cairo_scaled_font (data->scaled_font);

  switch (data->func)
    {
    case USER_FONT_INIT:
      {
        SCM extents = scm_from_cairo_font_extents (data->fexts);
        SCM cr = scm_from_cairo (data->cr);
        scm_call_3 (func, font, cr, extents);
        scm_fill_cairo_font_extents (extents, data->fexts);
        data->ret = CAIRO_STATUS_SUCCESS;
        break;
      }
    case USER_FONT_RENDER_GLYPH:
      {
        SCM extents = scm_from_cairo_text_extents (data->texts);
        SCM cr = scm_from_cairo (data->cr);
        scm_call_4 (func, font, scm_from_ulong (data->glyph), cr, extents);
        scm_fill_cairo_text_extents (extents, data->texts);
        data->ret = CAIRO_STATUS_SUCCESS;
        break;
      }
    case USER_FONT_UNICODE_TO_GLYPH:
      {
        data->glyph = scm_to_ulong
          (scm_call_2 (func, font, scm_from_ulong (data->unicode)));
        data->ret = CAIRO_STATUS_SUCCESS;
        break;
      }
    case USER_FONT_TEXT_TO_GLYPHS:
      {
        SCM str, ret, values, glyphs, clusters;
        int n_glyphs, n_clusters, i;
        cairo_text_cluster_flags_t flags = 0; /* We only go forwards.  */
        
        str = scm_from_utf8_stringn (data->utf8, data->utf8_len),
        ret = scm_call_3 (func, font, str, scm_from_bool (data->clusters));

        if (data->clusters)
          {
            if (!SCM_VALUESP (ret)
                || scm_ilength ((values = scm_struct_ref (ret, SCM_INUM0))) != 2)
              scm_error (scm_from_utf8_symbol ("cairo-error"),
                         NULL,
                         "Expected two return values (glyphs and clusters): ~S",
                         scm_list_1 (ret),
                         SCM_EOL);
            
            glyphs = scm_car (values);
            clusters = scm_cadr (values);
          }
        else
          {
            if (SCM_VALUESP (ret))
              scm_error (scm_from_utf8_symbol ("cairo-error"),
                         NULL,
                         "Expected one return value (glyphs): ~S",
                         scm_list_1 (ret),
                         SCM_EOL);
            
            glyphs = ret;
            clusters = SCM_EOL;
          }

        n_glyphs = scm_ilength (glyphs);
        n_clusters = scm_ilength (clusters);

        if (n_glyphs < 0 || n_clusters < 0)
          scm_error (scm_from_utf8_symbol ("cairo-error"),
                     NULL,
                     "Glyphs and clusters should be lists: ~S, ~S",
                     scm_list_2 (glyphs, clusters),
                     SCM_EOL);

        if (*data->num_glyphs < n_glyphs)
          {
            *data->num_glyphs = n_glyphs;
            *data->glyphs = cairo_glyph_allocate (n_glyphs);
          }
        else
          *data->num_glyphs = n_glyphs;

        for (i = 0; i < n_glyphs; i++)
          {
            scm_fill_cairo_glyph (scm_car (glyphs), *data->glyphs + i);
            glyphs = scm_cdr (glyphs);
          }

        if (data->clusters)
          {
            if (*data->num_clusters < n_clusters)
              {
                *data->num_clusters = n_clusters;
                *data->clusters = cairo_text_cluster_allocate (n_clusters);
              }
            else
              *data->num_clusters = n_clusters;

            scm_fill_cairo_text_clusters (str, clusters, *data->clusters);

            *data->cluster_flags = flags;
          }

        data->ret = CAIRO_STATUS_SUCCESS;
        break;
      }
    default:
      data->ret = CAIRO_STATUS_USER_FONT_ERROR;
      break;
    }

  return NULL;
}

static cairo_status_t
user_scaled_font_init_func (cairo_scaled_font_t *scaled_font, cairo_t *cr,
                            cairo_font_extents_t *extents)
{
  struct user_font_data data;
    
  data.ret = CAIRO_STATUS_USER_FONT_ERROR;
  data.func = USER_FONT_INIT;
  data.scaled_font = scaled_font;
  data.cr = cr;
  data.fexts = extents;
  
  scm_with_guile (do_user_font, &data);
  
  return data.ret;
}

static cairo_status_t
user_scaled_font_render_glyph_func (cairo_scaled_font_t *scaled_font,
                                    unsigned long glyph, cairo_t *cr,
                                    cairo_text_extents_t *extents)
{
  struct user_font_data data;
    
  data.ret = CAIRO_STATUS_USER_FONT_ERROR;
  data.func = USER_FONT_RENDER_GLYPH;
  data.scaled_font = scaled_font;
  data.glyph = glyph;
  data.cr = cr;
  data.texts = extents;
  
  scm_with_guile (do_user_font, &data);
  
  return data.ret;
}

static cairo_status_t
user_scaled_font_text_to_glyphs_func (cairo_scaled_font_t *scaled_font,
                                      const char *utf8, int utf8_len,
                                      cairo_glyph_t **glyphs, int *num_glyphs,
                                      cairo_text_cluster_t **clusters,
                                      int *num_clusters,
                                      cairo_text_cluster_flags_t *cluster_flags)
{
  struct user_font_data data;
    
  data.ret = CAIRO_STATUS_USER_FONT_ERROR;
  data.func = USER_FONT_TEXT_TO_GLYPHS;
  data.scaled_font = scaled_font;
  data.utf8 = utf8;
  data.utf8_len = utf8_len;
  data.glyphs = glyphs;
  data.num_glyphs = num_glyphs;
  data.clusters = clusters;
  data.num_clusters = num_clusters;
  data.cluster_flags = cluster_flags;
  
  scm_with_guile (do_user_font, &data);
  
  return data.ret;
}

static cairo_status_t
user_scaled_font_unicode_to_glyph_func (cairo_scaled_font_t *scaled_font,
                                        unsigned long unicode,
                                        unsigned long *glyph_index)
{
  struct user_font_data data;
    
  data.ret = CAIRO_STATUS_USER_FONT_ERROR;
  data.func = USER_FONT_UNICODE_TO_GLYPH;
  data.scaled_font = scaled_font;
  data.unicode = unicode;
  
  scm_with_guile (do_user_font, &data);
  
  if (data.ret == CAIRO_STATUS_SUCCESS)
    *glyph_index = data.glyph;

  return data.ret;
}

SCM_DEFINE_PUBLIC (scm_cairo_user_font_face_create, "cairo-user-font-face-create", 0, 0, 0,
                   (void),
                   "")
{
  FFCONSRET (cairo_user_font_face_create ());
}

SCM_DEFINE_PUBLIC (scm_cairo_user_font_face_set_init_func, "cairo-user-font-face-set-init-func", 2, 0, 0,
                   (SCM face, SCM init),
                   "")
{
  set_user_font_func (scm_to_cairo_font_face (face),
                      USER_FONT_INIT, init);
  cairo_user_font_face_set_init_func (scm_to_cairo_font_face (face),
                                      user_scaled_font_init_func);
  FFCHKRET (face, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_user_font_face_set_render_glyph_func, "cairo-user-font-face-set-render-glyph-func", 2, 0, 0,
                   (SCM face, SCM render_glyph),
                   "")
{
  set_user_font_func (scm_to_cairo_font_face (face),
                      USER_FONT_RENDER_GLYPH, render_glyph);
  cairo_user_font_face_set_render_glyph_func (scm_to_cairo_font_face (face),
                                              user_scaled_font_render_glyph_func);
  FFCHKRET (face, SCM_UNSPECIFIED);
}


SCM_DEFINE_PUBLIC (scm_cairo_user_font_face_set_unicode_to_glyph_func, "cairo-user-font-face-set-unicode-to-glyph-func", 2, 0, 0,
                   (SCM face, SCM func),
                   "")
{
  set_user_font_func (scm_to_cairo_font_face (face),
                      USER_FONT_UNICODE_TO_GLYPH, func);
  cairo_user_font_face_set_unicode_to_glyph_func (scm_to_cairo_font_face (face),
                                                  user_scaled_font_unicode_to_glyph_func);
  FFCHKRET (face, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_user_font_face_set_text_to_glyphs_func, "cairo-user-font-face-set-text-to-glyphs-func", 2, 0, 0,
                   (SCM face, SCM ttg),
                   "")
{
  set_user_font_func (scm_to_cairo_font_face (face),
                      USER_FONT_TEXT_TO_GLYPHS, ttg);
  cairo_user_font_face_set_text_to_glyphs_func (scm_to_cairo_font_face (face),
                                                user_scaled_font_text_to_glyphs_func);
  FFCHKRET (face, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_toy_font_face_create, "cairo-toy-font-face-create", 3, 0, 0,
                   (SCM sfamily, SCM slant, SCM weight),
                   "")
{
  cairo_font_face_t *ret;
  char *family;

  scm_dynwind_begin (0);
  family = scm_to_utf8_string (sfamily);
  scm_dynwind_free (family);

  ret = cairo_toy_font_face_create (family, scm_to_cairo_font_slant (slant),
                                    scm_to_cairo_font_weight (weight));

  scm_dynwind_end ();

  FFCONSRET (ret);
}

SCM_DEFINE_PUBLIC (scm_cairo_toy_font_face_get_family, "cairo-toy-font-face-get-family", 1, 0, 0,
                   (SCM ff),
                   "")
{
  FFCHKRET (ff, scm_from_utf8_string
            (cairo_toy_font_face_get_family (scm_to_cairo_font_face (ff))));
}

SCM_DEFINE_PUBLIC (scm_cairo_toy_font_face_get_slant, "cairo-toy-font-face-get-slant", 1, 0, 0,
                   (SCM ff),
                   "")
{
  FFCHKRET (ff, scm_from_cairo_font_slant
            (cairo_toy_font_face_get_slant (scm_to_cairo_font_face (ff))));
}

SCM_DEFINE_PUBLIC (scm_cairo_toy_font_face_get_weight, "cairo-toy-font-face-get-weight", 1, 0, 0,
                   (SCM ff),
                   "")
{
  FFCHKRET (ff, scm_from_cairo_font_weight
            (cairo_toy_font_face_get_weight (scm_to_cairo_font_face (ff))));
}

#endif /* 1.8 (user fonts) */

SCM_DEFINE_PUBLIC (scm_cairo_get_operator, "cairo-get-operator", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_cairo_operator (cairo_get_operator (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_source, "cairo-get-source", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_cairo_pattern (cairo_get_source (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_tolerance, "cairo-get-tolerance", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_double (cairo_get_tolerance (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_antialias, "cairo-get-antialias", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_cairo_antialias (cairo_get_antialias (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_current_point, "cairo-get-current-point", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  double x, y;
  cairo_get_current_point (scm_to_cairo (ctx), &x, &y);
  CCHKRET (ctx, 
           scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y))));
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,6,0)
SCM_DEFINE_PUBLIC (scm_cairo_has_current_point, "cairo-has-current-point", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, scm_from_bool (cairo_has_current_point (scm_to_cairo (ctx))));
}
#endif

SCM_DEFINE_PUBLIC (scm_cairo_get_fill_rule, "cairo-get-fill-rule", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_cairo_fill_rule (cairo_get_fill_rule (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_line_width, "cairo-get-line-width", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_double (cairo_get_line_width (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_line_cap, "cairo-get-line-cap", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_cairo_line_cap (cairo_get_line_cap (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_line_join, "cairo-get-line-join", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_cairo_line_join (cairo_get_line_join (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_miter_limit, "cairo-get-miter-limit", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_double (cairo_get_miter_limit (scm_to_cairo (ctx))));
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
SCM_DEFINE_PUBLIC (scm_cairo_get_dash_count, "cairo-get-dash-count", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_int (cairo_get_dash_count (scm_to_cairo (ctx))));
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
    
  CCHKRET (ctx,
           scm_values (scm_list_2 (data ? scm_take_f64vector(data, ndoubles) : SCM_BOOL_F,
                                   scm_from_double (offset))));
}
#endif /* HAVE_CAIRO_140_API */

SCM_DEFINE_PUBLIC (scm_cairo_get_matrix, "cairo-get-matrix", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_matrix_t matrix;

  cairo_get_matrix (scm_to_cairo (ctx), &matrix);

  CCHKRET (ctx, 
           scm_from_cairo_matrix (&matrix));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_target, "cairo-get-target", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  CCHKRET (ctx, 
           scm_from_cairo_surface (cairo_get_target (scm_to_cairo (ctx))));
}

SCM_DEFINE_PUBLIC (scm_cairo_get_group_target, "cairo-get-group-target", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  cairo_surface_t *surf;
  surf = cairo_get_group_target (scm_to_cairo (ctx));
  CCHKRET (ctx, 
           surf ? scm_from_cairo_surface (surf) : SCM_BOOL_F);
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

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,6,0)
SCM_DEFINE_PUBLIC (scm_cairo_path_extents, "cairo-path-extents", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  double x1, y1, x2, y2;
  cairo_path_extents (scm_to_cairo (ctx), &x1, &y1, &x2, &y2);
  CCHKRET (ctx,
           scm_values (scm_list_4 (scm_from_double (x1), scm_from_double (y1),
                                   scm_from_double (x2), scm_from_double (y2))));
}
#endif

SCM_DEFINE_PUBLIC (scm_cairo_copy_path, "cairo-copy-path", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  PCONSRET (cairo_copy_path (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_copy_path_flat, "cairo-copy-path-flat", 1, 0, 0,
                   (SCM ctx),
                   "")
{
  PCONSRET (cairo_copy_path_flat (scm_to_cairo (ctx)));
}

SCM_DEFINE_PUBLIC (scm_cairo_append_path, "cairo-append-path", 2, 0, 0,
                   (SCM ctx, SCM path),
                   "")
{
  cairo_append_path (scm_to_cairo (ctx), scm_to_cairo_path (path));
  CCHKRET (ctx, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_create_similar, "cairo-surface-create-similar", 4, 0, 0,
                   (SCM other, SCM content, SCM w, SCM h),
                   "")
{
  SCONSRET (cairo_surface_create_similar (scm_to_cairo_surface (other),
                                          scm_to_cairo_content (content),
                                          scm_to_int (w),
                                          scm_to_int (h)));
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
SCM_DEFINE_PUBLIC (scm_cairo_surface_create_for_rectangle, "cairo-surface-create-for-rectangle", 5, 0, 0,
                   (SCM other, SCM x0, SCM y0, SCM w, SCM h),
                   "")
{
  SCONSRET (cairo_surface_create_for_rectangle (scm_to_cairo_surface (other),
                                                scm_to_double (x0),
                                                scm_to_double (y0),
                                                scm_to_double (w),
                                                scm_to_double (h)));
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_get_device, "cairo-surface-get-device", 1, 0, 0,
                   (SCM surf),
                   "")
{
  SCM sdev;
  cairo_device_t *dev;

  dev = cairo_surface_get_device (scm_to_cairo_surface (surf));

  if (!dev)
    return SCM_BOOL_F;

  sdev = scm_from_cairo_device (dev);
  DCHKRET (sdev, sdev);
}
#endif  /* 1.10 */

/* Not implementing cairo_surface_set_mime_data currently, as I don't know when
 * or how to use it.  */

SCM_DEFINE_PUBLIC (scm_cairo_surface_finish, "cairo-surface-finish", 1, 0, 0,
                   (SCM surf),
                   "")
{
  cairo_surface_finish (scm_to_cairo_surface (surf));
  return SCM_UNSPECIFIED;
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_surface_get_reference_count, "cairo-surface-get-reference-count", 1, 0, 0,
                   (SCM surf),
                   "")
{
  return scm_from_uint (cairo_surface_get_reference_count (scm_to_cairo_surface (surf)));
}
#endif
#endif

SCM_DEFINE_PUBLIC (scm_cairo_surface_get_type, "cairo-surface-get-type", 1, 0, 0,
                   (SCM surf),
                   "")
{
  SCHKRET (surf, 
           scm_from_cairo_surface_type (cairo_surface_get_type (scm_to_cairo_surface (surf))));
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,8,0)
SCM_DEFINE_PUBLIC (scm_cairo_surface_has_show_text_glyphs, "cairo-surface-has-show-text-glyphs", 1, 0, 0,
                   (SCM surf),
                   "")
{
  SCHKRET (surf,
           scm_from_bool
           (cairo_surface_has_show_text_glyphs (scm_to_cairo_surface (surf))));
}
#endif  /* 1.8 */

SCM_DEFINE_PUBLIC (scm_cairo_surface_get_content, "cairo-surface-get-content", 1, 0, 0,
                   (SCM surf),
                   "")
{
  SCHKRET (surf, scm_from_cairo_content (cairo_surface_get_content (scm_to_cairo_surface (surf))));
}

#if CAIRO_HAS_PNG_FUNCTIONS

SCM_DEFINE_PUBLIC (scm_cairo_surface_write_to_png, "cairo-surface-write-to-png", 1, 1, 0,
                   (SCM surf, SCM filename),
                   "")
{
  cairo_status_t status;

  if (!SCM_UNBNDP (filename)) {
    char *str;
    scm_dynwind_begin (0);
    str = scm_to_utf8_string (filename);
    scm_dynwind_free (str);
        
    status = cairo_surface_write_to_png (scm_to_cairo_surface (surf), str);

    scm_dynwind_end ();
  } else {
    status = cairo_surface_write_to_png_stream (scm_to_cairo_surface (surf),
                                                guile_cairo_write_func, NULL);
  }

  scm_c_check_cairo_status (status, s_scm_cairo_surface_write_to_png);

  return SCM_UNSPECIFIED;
}

#endif /* CAIRO_HAS_PNG_FUNCTIONS */

SCM_DEFINE_PUBLIC (scm_cairo_surface_get_font_options, "cairo-surface-get-font-options", 1, 0, 0,
                   (SCM surf),
                   "")
{
  cairo_font_options_t *opts = cairo_font_options_create ();
    
  cairo_surface_get_font_options (scm_to_cairo_surface (surf), opts);
  FOCONSRET (opts);
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_flush, "cairo-surface-flush", 1, 0, 0,
                   (SCM surf),
                   "")
{
  cairo_surface_flush (scm_to_cairo_surface (surf));
  SCHKRET (surf, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_mark_dirty, "cairo-surface-mark-dirty", 1, 0, 0,
                   (SCM surf),
                   "")
{
  cairo_surface_mark_dirty (scm_to_cairo_surface (surf));
  SCHKRET (surf, SCM_UNSPECIFIED);
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
    
  SCHKRET (surf, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_set_device_offset, "cairo-surface-set-device-offset", 3, 0, 0,
                   (SCM surf, SCM x, SCM y),
                   "")
{
  cairo_surface_set_device_offset (scm_to_cairo_surface (surf),
                                   scm_to_double (x),
                                   scm_to_double (y));
    
  SCHKRET (surf, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_get_device_offset, "cairo-surface-get-device-offset", 1, 0, 0,
                   (SCM surf),
                   "")
{
  double x, y;

  cairo_surface_get_device_offset (scm_to_cairo_surface (surf),
                                   &x, &y);
  SCHKRET (surf,
           scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y))));
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_set_fallback_resolution, "cairo-surface-set-fallback-resolution", 3, 0, 0,
                   (SCM surf, SCM x, SCM y),
                   "")
{
  cairo_surface_set_fallback_resolution (scm_to_cairo_surface (surf),
                                         scm_to_double (x),
                                         scm_to_double (y));
    
  SCHKRET (surf, SCM_UNSPECIFIED);
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,8,0)
SCM_DEFINE_PUBLIC (scm_cairo_surface_get_fallback_resolution, "cairo-surface-get-fallback-resolution", 1, 0, 0,
                   (SCM surf),
                   "")
{
  double x, y;
  
  cairo_surface_get_fallback_resolution (scm_to_cairo_surface (surf),
                                         &x, &y);
    
  SCHKRET (surf, scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y))));
}
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,6,0)
SCM_DEFINE_PUBLIC (scm_cairo_surface_copy_page, "cairo-surface-copy-page", 1, 0, 0,
                   (SCM surf),
                   "")
{
  cairo_surface_copy_page (scm_to_cairo_surface (surf));

  SCHKRET (surf, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_surface_show_page, "cairo-surface-show-page", 1, 0, 0,
                   (SCM surf),
                   "")
{
  cairo_surface_show_page (scm_to_cairo_surface (surf));

  SCHKRET (surf, SCM_UNSPECIFIED);
}
#endif /* 1.6 */

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_create, "cairo-image-surface-create", 3, 0, 0,
                   (SCM format, SCM width, SCM height),
                   "")
{
  SCONSRET (cairo_image_surface_create (scm_to_cairo_format (format),
                                        scm_to_int (width),
                                        scm_to_int (height)));
}

#if SCM_MAJOR_VERSION >= 2 && CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,6,0)
/* Only Guile 2.0 and later have bytevectors; punt for 1.8.  Need Cairo 1.6 for
   cairo_format_stride_for_width.  */
SCM_DEFINE_PUBLIC (scm_cairo_image_surface_create_for_data, "cairo-image-surface-create-for-data", 4, 1, 0,
                   (SCM data, SCM format, SCM width, SCM height, SCM stride),
                   "")
{
  size_t data_len;
  scm_t_uint8 *data_ptr;
  cairo_format_t iformat;
  int iwidth, iheight, istride;
  cairo_surface_t *surf;

  data_len = scm_c_bytevector_length (data);
  data_ptr = (scm_t_uint8 *) SCM_BYTEVECTOR_CONTENTS (data);
  iformat = scm_to_cairo_format (format);
  iwidth = scm_to_int (width);
  iheight = scm_to_int (height);

  if (iwidth <= 0 || iheight <= 0)
    scm_error (scm_from_utf8_symbol ("cairo-error"),
               NULL,
               "Image surface dimensions should be positive: ~S, ~S",
               scm_list_2 (width, height),
               SCM_EOL);

  if (SCM_UNBNDP (stride))
    istride = data_len / iheight;
  else
    {
      istride = scm_to_int (stride);
      if (istride <= 0)
        scm_error (scm_from_utf8_symbol ("cairo-error"),
                   NULL,
                   "Image stride should be positive: ~S",
                   scm_list_1 (stride),
                   SCM_EOL);
    }

  if (istride != cairo_format_stride_for_width (iformat, iwidth))
    scm_error (scm_from_utf8_symbol ("cairo-error"),
               NULL,
               "Bad image stride: ~S (expected ~S)",
               scm_list_2 (stride,
                           scm_from_int
                           (cairo_format_stride_for_width
                            (iformat, iwidth))),
               SCM_EOL);

  if ((scm_t_uint64) data_len
      != ((scm_t_uint64) iheight) * ((scm_t_uint64) istride))
    scm_error (scm_from_utf8_symbol ("cairo-error"),
               NULL,
               "Data side and image dimensions do not match",
               SCM_EOL, SCM_EOL);

  surf = cairo_image_surface_create_for_data (data_ptr, iformat,
                                              iheight, iwidth, istride);
  scm_c_check_cairo_status (cairo_surface_status (surf),
                            s_scm_cairo_image_surface_create_for_data);
  cairo_surface_set_user_data (surf,
                               &scm_cairo_key,
                               SCM2PTR (scm_gc_protect_object (data)),
                               UNPROTECT);

  SCONSRET (surf);
}

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_get_data, "cairo-image-surface-get-data", 1, 0, 0,
                   (SCM surf),
                   "")
{
  cairo_surface_t *csurf;
  int height, stride;
  size_t len;
  unsigned char *ptr;
  SCM buf;

  scm_cairo_surface_flush (surf);

  csurf = scm_to_cairo_surface (surf);
  height = cairo_image_surface_get_format (csurf);
  stride = cairo_image_surface_get_stride (csurf);
  ptr = cairo_image_surface_get_data (csurf);

  /* If it's not an image surface, the height, stride, and ptr will be 0.  */
  if (height <= 0 || stride <= 0 || ptr == NULL)
    scm_error (scm_from_utf8_symbol ("cairo-error"),
               NULL,
               "Surface type mismatch",
               SCM_EOL, SCM_EOL);
  
  len = ((size_t) height) * ((size_t) stride);
  buf = scm_c_make_bytevector (len);
  memcpy (SCM_BYTEVECTOR_CONTENTS (buf), ptr, len);

  return buf;
}

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_set_data, "cairo-image-surface-set-data", 2, 0, 0,
                   (SCM surf, SCM data),
                   "")
{
  cairo_surface_t *csurf;
  int height, stride;
  size_t len;
  unsigned char *ptr;
  size_t data_len;
  scm_t_uint8 *data_ptr;

  csurf = scm_to_cairo_surface (surf);
  height = cairo_image_surface_get_format (csurf);
  stride = cairo_image_surface_get_stride (csurf);
  ptr = cairo_image_surface_get_data (csurf);

  /* If it's not an image surface, the height, stride, and ptr will be 0.  */
  if (height <= 0 || stride <= 0 || ptr == NULL)
    scm_error (scm_from_utf8_symbol ("cairo-error"),
               NULL,
               "Surface type mismatch",
               SCM_EOL, SCM_EOL);
  
  data_len = scm_c_bytevector_length (data);
  data_ptr = (scm_t_uint8 *) SCM_BYTEVECTOR_CONTENTS (data);

  len = ((size_t) height) * ((size_t) stride);
  if (len != data_len)
    scm_error (scm_from_utf8_symbol ("cairo-error"),
               NULL,
               "Unexpected bytevector length",
               SCM_EOL, SCM_EOL);

  memcpy (ptr, data_ptr, len);

  return scm_cairo_surface_mark_dirty (surf);
}
#endif /* Guile 2.0 and Cairo 1.6 */

#if 0
cairo_public unsigned char *
cairo_image_surface_get_data (cairo_surface_t *surface);

#endif /* 0 */

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_get_format, "cairo-image-surface-get-format", 1, 0, 0,
                   (SCM surf),
                   "")
{
  SCHKRET (surf,
           scm_from_cairo_format (cairo_image_surface_get_format (scm_to_cairo_surface (surf))));
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,6,0)
SCM_DEFINE_PUBLIC (scm_cairo_format_stride_for_width, "cairo-format-stride-for-width", 2, 0, 0,
                   (SCM fmt, SCM stride),
                   "")
{
  return scm_from_int (cairo_format_stride_for_width (scm_to_cairo_format (fmt),
                                                      scm_to_int (stride)));
}
#endif

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_get_width, "cairo-image-surface-get-width", 1, 0, 0,
                   (SCM surf),
                   "")
{
  SCHKRET (surf,
           scm_from_double (cairo_image_surface_get_width (scm_to_cairo_surface (surf))));
}

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_get_height, "cairo-image-surface-get-height", 1, 0, 0,
                   (SCM surf),
                   "")
{
  SCHKRET (surf, 
           scm_from_double (cairo_image_surface_get_height (scm_to_cairo_surface (surf))));
}

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_get_stride, "cairo-image-surface-get-stride", 1, 0, 0,
                   (SCM surf),
                   "")
{
  SCHKRET (surf, 
           scm_from_double (cairo_image_surface_get_stride (scm_to_cairo_surface (surf))));
}

#if CAIRO_HAS_PNG_FUNCTIONS

SCM_DEFINE_PUBLIC (scm_cairo_image_surface_create_from_png, "cairo-image-surface-create-from-png", 0, 1, 0,
                   (SCM sfilename),
                   "")
{
  cairo_surface_t *ret;
    
  if (!SCM_UNBNDP (sfilename)) {
    char *filename;
    scm_dynwind_begin (0);
    filename = scm_to_utf8_string (sfilename);
    scm_dynwind_free (filename);

    ret = cairo_image_surface_create_from_png (filename);

    scm_dynwind_end ();
  } else {
    ret = cairo_image_surface_create_from_png_stream (guile_cairo_read_func, NULL);
  }

  SCONSRET (ret);
}

#endif /* CAIRO_HAS_PNG_FUNCTIONS */

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
/* Recording-surface functions */

SCM_DEFINE_PUBLIC (scm_cairo_recording_surface_create, "cairo-recording-surface-create", 2, 0, 0,
                   (SCM content, SCM rect),
                   "")
{
  cairo_rectangle_t r;
  scm_fill_cairo_rectangle (rect, &r);
  SCONSRET (cairo_recording_surface_create (scm_to_cairo_content (content), &r));
}

SCM_DEFINE_PUBLIC (scm_cairo_recording_surface_ink_extents, "cairo-recording-surface-ink-extents", 1, 0, 0,
                   (SCM surf),
                   "")
{
  double x0, y0, width, height;
  cairo_recording_surface_ink_extents (scm_to_cairo_surface (surf),
                                       &x0, &y0, &width, &height);
  SCHKRET (surf, scm_values (scm_list_4 (scm_from_double (x0),
                                         scm_from_double (y0),
                                         scm_from_double (width),
                                         scm_from_double (height))));
}

#endif /* 1.10 */

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_rgb, "cairo-pattern-create-rgb", 3, 0, 0,
                   (SCM r, SCM g, SCM b),
                   "")
{
  PATCONSRET (cairo_pattern_create_rgb (scm_to_double (r),
                                        scm_to_double (g),
                                        scm_to_double (b)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_rgba, "cairo-pattern-create-rgba", 4, 0, 0,
                   (SCM r, SCM g, SCM b, SCM a),
                   "")
{
  PATCONSRET (cairo_pattern_create_rgba (scm_to_double (r),
                                         scm_to_double (g),
                                         scm_to_double (b),
                                         scm_to_double (a)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_for_surface, "cairo-pattern-create-for-surface", 1, 0, 0,
                   (SCM surf),
                   "")
{
  PATCONSRET (cairo_pattern_create_for_surface (scm_to_cairo_surface (surf)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_linear, "cairo-pattern-create-linear", 4, 0, 0,
                   (SCM x0, SCM y0, SCM x1, SCM y1),
                   "")
{
  PATCONSRET (cairo_pattern_create_linear (scm_to_double (x0),
                                           scm_to_double (y0),
                                           scm_to_double (x1),
                                           scm_to_double (y1)));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_create_radial, "cairo-pattern-create-radial", 6, 0, 0,
                   (SCM cx0, SCM cy0, SCM r0, SCM cx1, SCM cy1, SCM r1),
                   "")
{
  PATCONSRET (cairo_pattern_create_radial (scm_to_double (cx0),
                                           scm_to_double (cy0),
                                           scm_to_double (r0),
                                           scm_to_double (cx1),
                                           scm_to_double (cy1),
                                           scm_to_double (r1)));
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
#ifdef DEBUG_GUILE_CAIRO
SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_reference_count, "cairo-pattern-get-reference-count", 1, 0, 0,
                   (SCM pat),
                   "")
{
  return scm_from_uint (cairo_pattern_get_reference_count (scm_to_cairo_pattern (pat)));
}
#endif
#endif

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_type, "cairo-pattern-get-type", 1, 0, 0,
                   (SCM pat),
                   "")
{
  PATCHKRET(pat, scm_from_cairo_pattern_type (cairo_pattern_get_type (scm_to_cairo_pattern (pat))));
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
  PATCHKRET (pat, SCM_UNSPECIFIED);
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
  PATCHKRET (pat, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_set_matrix, "cairo-pattern-set-matrix", 2, 0, 0,
                   (SCM pat, SCM smatrix),
                   "")
{
  cairo_matrix_t matrix;
  scm_fill_cairo_matrix (smatrix, &matrix);
    
  cairo_pattern_set_matrix (scm_to_cairo_pattern (pat), &matrix);

  PATCHKRET (pat, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_matrix, "cairo-pattern-get-matrix", 1, 0, 0,
                   (SCM pat),
                   "")
{
  cairo_matrix_t matrix;
    
  cairo_pattern_get_matrix (scm_to_cairo_pattern (pat), &matrix);

  PATCHKRET (pat, scm_from_cairo_matrix (&matrix));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_set_extend, "cairo-pattern-set-extend", 2, 0, 0,
                   (SCM pat, SCM extend),
                   "")
{
  cairo_pattern_set_extend (scm_to_cairo_pattern (pat),
                            scm_to_cairo_extend (extend));

  PATCHKRET (pat, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_extend, "cairo-pattern-get-extend", 1, 0, 0,
                   (SCM pat),
                   "")
{
  PATCHKRET (pat,
             scm_from_cairo_extend (cairo_pattern_get_extend (scm_to_cairo_pattern (pat))));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_set_filter, "cairo-pattern-set-filter", 2, 0, 0,
                   (SCM pat, SCM filter),
                   "")
{
  cairo_pattern_set_filter (scm_to_cairo_pattern (pat),
                            scm_to_cairo_filter (filter));

  PATCHKRET (pat, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_filter, "cairo-pattern-get-filter", 1, 0, 0,
                   (SCM pat),
                   "")
{
  PATCHKRET (pat,
             scm_from_cairo_filter (cairo_pattern_get_filter (scm_to_cairo_pattern (pat))));
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_rgba, "cairo-pattern-get-rgba", 1, 0, 0,
                   (SCM pat),
                   "")
{
  double r, g, b, a;
  cairo_pattern_get_rgba (scm_to_cairo_pattern (pat), &r, &b, &g, &a);
  PATCHKRET (pat,
             scm_values (scm_list_4 (scm_from_double (r), scm_from_double (g),
                                     scm_from_double (b), scm_from_double (a))));
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
  PATCHKRET (pat,
             scm_values (scm_list_5 (scm_from_double (off),
                                     scm_from_double (r), scm_from_double (g),
                                     scm_from_double (b), scm_from_double (a))));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_color_stop_count, "cairo-pattern-get-color-count", 1, 0, 0,
                   (SCM pat),
                   "")
{
  int count;
  cairo_pattern_get_color_stop_count (scm_to_cairo_pattern (pat), &count);
  PATCHKRET (pat, scm_from_int (count));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_linear_points, "cairo-pattern-get-linear-points", 1, 0, 0,
                   (SCM pat),
                   "")
{
  double x0, y0, x1, y1;
  cairo_pattern_get_linear_points (scm_to_cairo_pattern (pat),
                                   &x0, &y0, &x1, &y1);
  PATCHKRET (pat, 
             scm_values (scm_list_4 (scm_from_double (x0), scm_from_double (y0),
                                     scm_from_double (x1), scm_from_double (y1))));
}

SCM_DEFINE_PUBLIC (scm_cairo_pattern_get_radial_circles, "cairo-pattern-get-radial-circles", 1, 0, 0,
                   (SCM pat),
                   "")
{
  double x0, y0, r0, x1, y1, r1;
    
  cairo_pattern_get_radial_circles (scm_to_cairo_pattern (pat),
                                    &x0, &y0, &r0, &x1, &y1, &r1);
  PATCHKRET (pat, 
             scm_values (scm_list_n (scm_from_double (x0), scm_from_double (y0),
                                     scm_from_double (r0),
                                     scm_from_double (x1), scm_from_double (y1),
                                     scm_from_double (r1),
                                     SCM_UNDEFINED)));
}
#endif /* HAVE_CAIRO_140_API */

SCM_DEFINE_PUBLIC (scm_cairo_make_matrix, "cairo-make-matrix", 6, 0, 0,
                   (SCM xx, SCM yx, SCM xy, SCM yy, SCM x0, SCM y0),
                   "")
{
  cairo_matrix_t matrix;
    
  cairo_matrix_init (&matrix,
                     scm_to_double (xx), scm_to_double (yx),
                     scm_to_double (xy), scm_to_double (yy),
                     scm_to_double (x0), scm_to_double (y0));

  return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_make_identity_matrix, "cairo-make-identity-matrix", 0, 0, 0,
                   (void),
                   "")
{
  cairo_matrix_t matrix;
    
  cairo_matrix_init_identity (&matrix);

  return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_make_translate_matrix, "cairo-make-translate-matrix", 2, 0, 0,
                   (SCM tx, SCM ty),
                   "")
{
  cairo_matrix_t matrix;
    
  cairo_matrix_init_translate (&matrix, scm_to_double (tx), scm_to_double (ty));

  return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_make_scale_matrix, "cairo-make-scale-matrix", 2, 0, 0,
                   (SCM sx, SCM sy),
                   "")
{
  cairo_matrix_t matrix;
    
  cairo_matrix_init_scale (&matrix, scm_to_double (sx), scm_to_double (sy));

  return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_make_rotate_matrix, "cairo-make-rotate-matrix", 1, 0, 0,
                   (SCM rads),
                   "")
{
  cairo_matrix_t matrix;
    
  cairo_matrix_init_rotate (&matrix, scm_to_double (rads));

  return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_matrix_translate, "cairo-matrix-translate", 3, 0, 0,
                   (SCM smatrix, SCM tx, SCM ty),
                   "")
{
  cairo_matrix_t matrix;

  scm_fill_cairo_matrix (smatrix, &matrix);
    
  cairo_matrix_translate (&matrix, scm_to_double (tx), scm_to_double (ty));

  return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_matrix_scale, "cairo-matrix-scale", 3, 0, 0,
                   (SCM smatrix, SCM sx, SCM sy),
                   "")
{
  cairo_matrix_t matrix;
    
  scm_fill_cairo_matrix (smatrix, &matrix);
    
  cairo_matrix_scale (&matrix, scm_to_double (sx), scm_to_double (sy));

  return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_matrix_rotate, "cairo-matrix-rotate", 2, 0, 0,
                   (SCM smatrix, SCM rads),
                   "")
{
  cairo_matrix_t matrix;
    
  scm_fill_cairo_matrix (smatrix, &matrix);
    
  cairo_matrix_rotate (&matrix, scm_to_double (rads));

  return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_matrix_invert, "cairo-matrix-invert", 1, 0, 0,
                   (SCM smatrix),
                   "")
{
  cairo_matrix_t matrix;
    
  scm_fill_cairo_matrix (smatrix, &matrix);
    
  cairo_matrix_invert (&matrix);

  return scm_from_cairo_matrix (&matrix);
}

SCM_DEFINE_PUBLIC (scm_cairo_matrix_multiply, "cairo-matrix-multiply", 2, 0, 0,
                   (SCM sm1, SCM sm2),
                   "")
{
  cairo_matrix_t m1, m2;
    
  scm_fill_cairo_matrix (sm1, &m1);
  scm_fill_cairo_matrix (sm2, &m2);
    
  cairo_matrix_multiply (&m1, &m1, &m2);

  return scm_from_cairo_matrix (&m1);
}

SCM_DEFINE_PUBLIC (scm_cairo_matrix_transform_distance, "cairo-matrix-transform-distance", 3, 0, 0,
                   (SCM smatrix, SCM sdx, SCM sdy),
                   "")
{
  cairo_matrix_t matrix;
  double dx, dy;
    
  scm_fill_cairo_matrix (smatrix, &matrix);
  dx = scm_to_double (sdx);
  dy = scm_to_double (sdy);
    
  cairo_matrix_transform_distance (&matrix, &dx, &dy);

  return scm_values (scm_list_2 (scm_from_double (dx), scm_from_double (dy)));
}

SCM_DEFINE_PUBLIC (scm_cairo_matrix_transform_point, "cairo-matrix-transform-point", 3, 0, 0,
                   (SCM smatrix, SCM sx, SCM sy),
                   "")
{
  cairo_matrix_t matrix;
  double x, y;
    
  scm_fill_cairo_matrix (smatrix, &matrix);
  x = scm_to_double (sx);
  y = scm_to_double (sy);
    
  cairo_matrix_transform_point (&matrix, &x, &y);

  return scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y)));
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
SCM_DEFINE_PUBLIC (scm_cairo_region_create, "cairo-region-create", 0, 1, 0,
                   (SCM int_rectangle),
                   "")
{
  if (SCM_UNBNDP (int_rectangle))
    {
      RCONSRET (cairo_region_create ());
    }
  else
    {
      cairo_rectangle_int_t rect;
      scm_fill_cairo_rectangle_int (int_rectangle, &rect);
      RCONSRET (cairo_region_create_rectangle (&rect));
    }
}

/* Not wrapping create_rectangles; you can do the fold yourself. */

SCM_DEFINE_PUBLIC (scm_cairo_region_copy, "cairo-region-copy", 1, 0, 0,
                   (SCM region),
                   "")
{
  RCONSRET (cairo_region_copy (scm_to_cairo_region (region)));
}

SCM_DEFINE_PUBLIC (scm_cairo_region_get_extents, "cairo-region-get-extents", 1, 0, 0,
                   (SCM region),
                   "")
{
  cairo_rectangle_int_t rect;
  cairo_region_get_extents (scm_to_cairo_region (region), &rect);
  RCHKRET (region, scm_from_cairo_rectangle_int (&rect));
}

SCM_DEFINE_PUBLIC (scm_cairo_region_get_rectangles, "cairo-region-get-rectangles", 1, 0, 0,
                   (SCM region),
                   "")
{
  SCM ret = SCM_EOL;
  int n = cairo_region_num_rectangles (scm_to_cairo_region (region));
  
  while (n--)
    {
      cairo_rectangle_int_t rect;
      cairo_region_get_rectangle (scm_to_cairo_region (region), n, &rect);
      ret = scm_cons (scm_from_cairo_rectangle_int (&rect), ret);
    }
  
  RCHKRET (region, ret);
}

SCM_DEFINE_PUBLIC (scm_cairo_region_is_empty, "cairo-region-is-empty", 1, 0, 0,
                   (SCM region),
                   "")
{
  RCHKRET (region, scm_from_bool (cairo_region_is_empty (scm_to_cairo_region (region))));
}

SCM_DEFINE_PUBLIC (scm_cairo_region_contains_rectangle, "cairo-region-contains-rectangle", 2, 0, 0,
                   (SCM region, SCM rectangle),
                   "")
{
  cairo_rectangle_int_t rect;
  scm_fill_cairo_rectangle_int (rectangle, &rect);
  RCHKRET (region, scm_from_cairo_region_overlap
           (cairo_region_contains_rectangle (scm_to_cairo_region (region), &rect)));
}

SCM_DEFINE_PUBLIC (scm_cairo_region_contains_point, "cairo-region-contains-point", 3, 0, 0,
                   (SCM region, SCM x, SCM y),
                   "")
{
  RCHKRET (region, scm_from_bool
           (cairo_region_contains_point (scm_to_cairo_region (region),
                                         scm_to_int (x), scm_to_int (y))));
}

SCM_DEFINE_PUBLIC (scm_cairo_region_translate, "cairo-region-translate", 3, 0, 0,
                   (SCM region, SCM dx, SCM dy),
                   "")
{
  cairo_region_translate (scm_to_cairo_region (region),
                          scm_to_int (dx), scm_to_int (dy));

  RCHKRET (region, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_region_subtract, "cairo-region-subtract", 2, 0, 0,
                   (SCM region, SCM other),
                   "")
{
  cairo_status_t status;

  if (scm_is_vector (other))
    {
      cairo_rectangle_int_t rect;
      scm_fill_cairo_rectangle_int (other, &rect);
      status = cairo_region_subtract_rectangle (scm_to_cairo_region (region),
                                                &rect);
    }
  else
    status = cairo_region_subtract (scm_to_cairo_region (region),
                                    scm_to_cairo_region (other));
  scm_c_check_cairo_status (status, s_scm_cairo_region_subtract);
  return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_region_intersect, "cairo-region-intersect", 2, 0, 0,
                   (SCM region, SCM other),
                   "")
{
  cairo_status_t status;

  if (scm_is_vector (other))
    {
      cairo_rectangle_int_t rect;
      scm_fill_cairo_rectangle_int (other, &rect);
      status = cairo_region_intersect_rectangle (scm_to_cairo_region (region),
                                                 &rect);
    }
  else
    status = cairo_region_intersect (scm_to_cairo_region (region),
                                     scm_to_cairo_region (other));
  scm_c_check_cairo_status (status, s_scm_cairo_region_intersect);
  return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_region_union, "cairo-region-union", 2, 0, 0,
                   (SCM region, SCM other),
                   "")
{
  cairo_status_t status;

  if (scm_is_vector (other))
    {
      cairo_rectangle_int_t rect;
      scm_fill_cairo_rectangle_int (other, &rect);
      status = cairo_region_union_rectangle (scm_to_cairo_region (region),
                                             &rect);
    }
  else
    status = cairo_region_union (scm_to_cairo_region (region),
                                 scm_to_cairo_region (other));
  scm_c_check_cairo_status (status, s_scm_cairo_region_union);
  return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_region_xor, "cairo-region-xor", 2, 0, 0,
                   (SCM region, SCM other),
                   "")
{
  cairo_status_t status;

  if (scm_is_vector (other))
    {
      cairo_rectangle_int_t rect;
      scm_fill_cairo_rectangle_int (other, &rect);
      status = cairo_region_xor_rectangle (scm_to_cairo_region (region),
                                           &rect);
    }
  else
    status = cairo_region_xor (scm_to_cairo_region (region),
                               scm_to_cairo_region (other));
  scm_c_check_cairo_status (status, s_scm_cairo_region_xor);
  return SCM_UNSPECIFIED;
}

SCM_DEFINE_PUBLIC (scm_cairo_region, "cairo-matrix-transform-point", 3, 0, 0,
                   (SCM smatrix, SCM sx, SCM sy),
                   "")
{
  cairo_matrix_t matrix;
  double x, y;
    
  scm_fill_cairo_matrix (smatrix, &matrix);
  x = scm_to_double (sx);
  y = scm_to_double (sy);
    
  cairo_matrix_transform_point (&matrix, &x, &y);

  return scm_values (scm_list_2 (scm_from_double (x), scm_from_double (y)));
}
#endif /* 1.10 */

#if CAIRO_HAS_PDF_SURFACE

SCM_DEFINE_PUBLIC (scm_cairo_pdf_surface_create, "cairo-pdf-surface-create", 2, 1, 0,
                   (SCM sx, SCM sy, SCM sfilename),
                   "")
{
  cairo_surface_t *ret;
    
  if (!SCM_UNBNDP (sfilename)) {
    char *filename;
    scm_dynwind_begin (0);
    filename = scm_to_utf8_string (sfilename);
    scm_dynwind_free (filename);
        
    ret = cairo_pdf_surface_create (filename,
                                    scm_to_double (sx),
                                    scm_to_double (sy));
        
    scm_dynwind_end ();
  } else {
    ret = cairo_pdf_surface_create_for_stream
      (guile_cairo_write_func,
       (void*)SCM_UNPACK (scm_current_output_port ()),
       scm_to_double (sx), scm_to_double (sy));
  }
                                                   
  SCONSRET (ret);
}

SCM_DEFINE_PUBLIC (scm_cairo_pdf_surface_set_size, "cairo-pdf-surface-set-size", 3, 0, 0,
                   (SCM surf, SCM sx, SCM sy),
                   "")
{
  cairo_pdf_surface_set_size (scm_to_cairo_surface (surf),
                              scm_to_double (sx),
                              scm_to_double (sy));
  SCHKRET (surf, SCM_UNSPECIFIED);
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)

SCM_DEFINE_PUBLIC (scm_cairo_pdf_get_versions, "cairo-pdf-get-versions", 0, 0, 0,
                   (),
                   "")
{
  const cairo_pdf_version_t *versions;
  int nversions;
  SCM ret = SCM_EOL;

  cairo_pdf_get_versions (&versions, &nversions);
  
  while (nversions--)
    ret = scm_cons (scm_from_cairo_pdf_version (versions[nversions]), ret);

  return ret;
}

SCM_DEFINE_PUBLIC (scm_cairo_pdf_surface_restrict_to_version, "cairo-pdf-surface-restrict-to-version", 2, 0, 0,
                   (SCM surf, SCM version),
                   "")
{
  cairo_pdf_surface_restrict_to_version (scm_to_cairo_surface (surf),
                                         scm_to_cairo_pdf_version (version));
  SCHKRET (surf, SCM_UNSPECIFIED);
}

#endif /* 1.10 */

#endif /* CAIRO_HAS_PDF_SURFACE */

#if CAIRO_HAS_PS_SURFACE

SCM_DEFINE_PUBLIC (scm_cairo_ps_surface_create, "cairo-ps-surface-create", 2, 1, 0,
                   (SCM sx, SCM sy, SCM sfilename),
                   "")
{
  cairo_surface_t *ret;
    
  if (!SCM_UNBNDP (sfilename)) {
    char *filename;
    scm_dynwind_begin (0);
    filename = scm_to_utf8_string (sfilename);
    scm_dynwind_free (filename);
        
    ret = cairo_ps_surface_create (filename,
                                   scm_to_double (sx),
                                   scm_to_double (sy));
        
    scm_dynwind_end ();
  } else {
    ret = cairo_ps_surface_create_for_stream
      (guile_cairo_write_func,
       (void*)SCM_UNPACK (scm_current_output_port ()),
       scm_to_double (sx), scm_to_double (sy));
  }
    
  SCONSRET (ret);
}

SCM_DEFINE_PUBLIC (scm_cairo_ps_surface_set_size, "cairo-ps-surface-set-size", 3, 0, 0,
                   (SCM surf, SCM sx, SCM sy),
                   "")
{
  cairo_ps_surface_set_size (scm_to_cairo_surface (surf),
                             scm_to_double (sx),
                             scm_to_double (sy));
  SCHKRET (surf, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_ps_surface_dsc_comment, "cairo-ps-surface-dsc-comment", 2, 0, 0,
                   (SCM surf, SCM scomment),
                   "")
{
  char *comment;
    
  scm_dynwind_begin (0);
  comment = scm_to_utf8_string (scomment);
  scm_dynwind_free (comment);

  cairo_ps_surface_dsc_comment (scm_to_cairo_surface (surf), comment);

  scm_dynwind_end ();

  SCHKRET (surf, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_ps_surface_dsc_begin_setup, "cairo-ps-surface-begin-setup", 1, 0, 0,
                   (SCM surf),
                   "")
{
  cairo_ps_surface_dsc_begin_setup (scm_to_cairo_surface (surf));
  SCHKRET (surf, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_ps_surface_dsc_begin_page_setup, "cairo-ps-surface-begin-page-setup", 1, 0, 0,
                   (SCM surf),
                   "")
{
  cairo_ps_surface_dsc_begin_page_setup (scm_to_cairo_surface (surf));
  SCHKRET (surf, SCM_UNSPECIFIED);
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,6,0)
SCM_DEFINE_PUBLIC (scm_cairo_ps_get_levels, "cairo-ps-get-levels", 0, 0, 0,
                   (),
                   "")
{
  const cairo_ps_level_t *levels;
  int nlevels;
  SCM ret = SCM_EOL;

  cairo_ps_get_levels (&levels, &nlevels);
  
  while (nlevels--)
    ret = scm_cons (scm_from_cairo_ps_level (levels[nlevels]), ret);

  return ret;
}

SCM_DEFINE_PUBLIC (scm_cairo_ps_surface_restrict_to_level, "cairo-ps-surface-restrict-to-level", 2, 0, 0,
                   (SCM surf, SCM level),
                   "")
{
  cairo_ps_surface_restrict_to_level (scm_to_cairo_surface (surf),
                                      scm_to_cairo_ps_level (level));
  SCHKRET (surf, SCM_UNSPECIFIED);
}

SCM_DEFINE_PUBLIC (scm_cairo_ps_surface_get_eps, "cairo-ps-surface-get-eps", 1, 0, 0,
                   (SCM surf),
                   "")
{
  SCHKRET (surf, scm_from_bool (cairo_ps_surface_get_eps (scm_to_cairo_surface (surf))));
}

SCM_DEFINE_PUBLIC (scm_cairo_ps_surface_set_eps, "cairo-ps-surface-set-eps", 2, 0, 0,
                   (SCM surf, SCM eps),
                   "")
{
  cairo_ps_surface_set_eps (scm_to_cairo_surface (surf), scm_to_bool (eps));
  SCHKRET (surf, SCM_UNSPECIFIED);
}

#endif /* 1.6 */

#endif /* CAIRO_HAS_PS_SURFACE */

#if CAIRO_HAS_SVG_SURFACE

SCM_DEFINE_PUBLIC (scm_cairo_svg_surface_create, "cairo-svg-surface-create", 2, 1, 0,
                   (SCM sx, SCM sy, SCM sfilename),
                   "")
{
  cairo_surface_t *ret;
    
  if (!SCM_UNBNDP (sfilename)) {
    char *filename;
    scm_dynwind_begin (0);
    filename = scm_to_utf8_string (sfilename);
    scm_dynwind_free (filename);
        
    ret = cairo_svg_surface_create (filename,
                                    scm_to_double (sx),
                                    scm_to_double (sy));

    scm_dynwind_end ();
  } else {
    ret = cairo_svg_surface_create_for_stream
      (guile_cairo_write_func,
       (void*)SCM_UNPACK (scm_current_output_port ()),
       scm_to_double (sx), scm_to_double (sy));
  }
    
  SCONSRET (ret);
}

SCM_DEFINE_PUBLIC (scm_cairo_svg_surface_restrict_to_version, "cairo-svg-surface-restrict-to-version", 2, 0, 0,
                   (SCM surf, SCM vers),
                   "")
{
  cairo_svg_surface_restrict_to_version (scm_to_cairo_surface (surf),
                                         scm_to_cairo_svg_version (vers));
  SCHKRET (surf, SCM_UNSPECIFIED);
}

#if 0
cairo_public void
cairo_svg_get_versions (cairo_svg_version_t const	**versions,
                        int                      	 *num_versions);

cairo_public const char *
cairo_svg_version_to_string (cairo_svg_version_t version);
#endif /* 0 */

#endif /* CAIRO_HAS_SVG_SURFACE */



void
scm_init_cairo (void)
{
  static int initialized = 0;

  if (initialized)
    return;

#ifndef SCM_MAGIC_SNARFER
#include "guile-cairo.x"
#endif

  scm_init_cairo_smob_types ();
  scm_init_cairo_vector_types ();
  scm_init_cairo_enum_types ();

  initialized = 1;
}

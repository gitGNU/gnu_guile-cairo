/* guile-cairo
 * Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-vector-types.c: Cairo for Guile
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

#include "guile-cairo-compat.h"
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

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
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
#define GET(v,i)                                                        \
  scm_to_double (scm_vector_ref (v, scm_from_int (i)))

  rect->x = GET (scm, 0);
  rect->y = GET (scm, 1);
  rect->width = GET (scm, 2);
  rect->height = GET (scm, 3);
#undef GET
}
#endif 

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
#define GET(func,v,i)                                                   \
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
#define GET(v,i)                                                        \
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
#define GET(v,i)                                                        \
  scm_to_double (scm_f64vector_ref (v, scm_from_int (i)))

  fext->ascent = GET (scm, 0);
  fext->descent = GET (scm, 1);
  fext->height = GET (scm, 2);
  fext->max_x_advance = GET (scm, 3);
  fext->max_y_advance = GET (scm, 4);
#undef GET
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,8,0)
SCM
scm_from_cairo_text_clusters (SCM str, cairo_text_cluster_t *clusters,
                              int n_clusters, cairo_text_cluster_flags_t flags)
{
  SCM ret = SCM_EOL;
  int backwards = flags & CAIRO_TEXT_CLUSTER_FLAG_BACKWARD;
#if ! SCM_NEEDS_COMPAT (2, 0, 0)
  int n = 0;
#endif

  if (backwards)
    clusters += n_clusters - 1;
  
  while (n_clusters--)
    {
      int num_codepoints, num_bytes, num_glyphs;

      num_codepoints = 0;
      num_bytes = clusters->num_bytes;
      num_glyphs = clusters->num_glyphs;

      while (num_bytes)
        {
#if SCM_NEEDS_COMPAT (2, 0, 0)
          num_bytes--;
#else
          scm_t_wchar c = SCM_CHAR (scm_c_string_ref (str, n++));
          
          if (c < 0x80)
            num_bytes -= 1;
          else if (c < 0x800)
            num_bytes -= 2;
          else if (c < 0x10000)
            num_bytes -= 3;
          else if (c < 0x110000)
            num_bytes -= 4;
          else
            abort ();
#endif

          if (num_bytes < 0)
            abort ();

          num_codepoints++;
        }

      ret = scm_cons (scm_cons (scm_from_int (num_codepoints),
                                scm_from_int (num_glyphs)),
                      ret);

      if (backwards)
        clusters--;
      else
        clusters++;
    }

  return ret;
}

void
scm_fill_cairo_text_clusters (SCM str, SCM scm,
                              cairo_text_cluster_t *clusters)
{
  /* scm must be a list, and it should be of pairs:

       ((N-CODEPOINTS . N-GLYPHS) ...)
  */

#if ! SCM_NEEDS_COMPAT (2, 0, 0)
  size_t n = 0;
#endif
  
  for (; scm_is_pair (scm); scm = scm_cdr (scm))
    {
      int num_codepoints = scm_to_int (scm_caar (scm));
      int num_bytes = 0;
      int num_glyphs = scm_to_int (scm_cdar (scm));

      while (num_codepoints--)
        {
#if SCM_NEEDS_COMPAT (2, 0, 0)
          num_bytes++;
#else
          scm_t_wchar c = SCM_CHAR (scm_c_string_ref (str, n++));
          
          if (c < 0x80)
            num_bytes += 1;
          else if (c < 0x800)
            num_bytes += 2;
          else if (c < 0x10000)
            num_bytes += 3;
          else if (c < 0x110000)
            num_bytes += 4;
          else
            abort ();
#endif
        }

      clusters->num_bytes = num_bytes;
      clusters->num_glyphs = num_glyphs;
      clusters++;
    }
}
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
/**********************************************************************
 * cairo_rectangle_int_t
 **********************************************************************/

SCM
scm_from_cairo_rectangle_int (cairo_rectangle_int_t *rect)
{
  return scm_s32vector (scm_list_4 (scm_from_int (rect->x),
                                    scm_from_int (rect->y),
                                    scm_from_int (rect->width),
                                    scm_from_int (rect->height)));
}

void
scm_fill_cairo_rectangle_int (SCM scm, cairo_rectangle_int_t *rect)
{
#define GET(v,i) scm_to_int (scm_s32vector_ref (v, scm_from_int (i)))

  rect->x = GET (scm, 0);
  rect->y = GET (scm, 1);
  rect->width = GET (scm, 2);
  rect->height = GET (scm, 3);
#undef GET
}
#endif /* 1.10 */

void
scm_init_cairo_vector_types (void)
{
#ifndef SCM_MAGIC_SNARFER
#include "guile-cairo-vector-types.x"
#endif
}

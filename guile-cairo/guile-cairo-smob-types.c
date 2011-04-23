/* guile-cairo
 * Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-smob-types.c: Cairo for Guile
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
#include "guile-cairo-smob-types.h"


scm_t_bits scm_tc16_cairo_t;
scm_t_bits scm_tc16_cairo_surface_t;
scm_t_bits scm_tc16_cairo_pattern_t;
scm_t_bits scm_tc16_cairo_scaled_font_t;
scm_t_bits scm_tc16_cairo_font_face_t;
scm_t_bits scm_tc16_cairo_font_options_t;
scm_t_bits scm_tc16_cairo_path_t;
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
scm_t_bits scm_tc16_cairo_device_t;
scm_t_bits scm_tc16_cairo_region_t;
#endif


/**********************************************************************
 * cairo_t
 **********************************************************************/

SCM
scm_take_cairo (cairo_t *ctx)
{
  SCM sctx;

  SCM_NEWSMOB (sctx, scm_tc16_cairo_t, ctx);

  return sctx;
}

SCM
scm_from_cairo (cairo_t *ctx)
{
  return scm_take_cairo (cairo_reference (ctx));
}

cairo_t*
scm_to_cairo (SCM scm)
{
  scm_assert_smob_type (scm_tc16_cairo_t, scm);
  return (cairo_t*)SCM_SMOB_DATA (scm);
}

static size_t
scm_cairo_free (SCM smob)
{
  cairo_t *ctx = (cairo_t*)SCM_SMOB_DATA (smob);

  SCM_SET_SMOB_DATA (smob, NULL);
  cairo_destroy (ctx);

  return 0;
}

/**********************************************************************
 * cairo_surface_t
 **********************************************************************/

SCM
scm_take_cairo_surface (cairo_surface_t *surf)
{
  SCM ssurf;

  SCM_NEWSMOB (ssurf, scm_tc16_cairo_surface_t, surf);

  return ssurf;
}

SCM
scm_from_cairo_surface (cairo_surface_t *surf)
{
  return scm_take_cairo_surface (cairo_surface_reference (surf));
}

cairo_surface_t*
scm_to_cairo_surface (SCM scm)
{
  scm_assert_smob_type (scm_tc16_cairo_surface_t, scm);
  return (cairo_surface_t*)SCM_SMOB_DATA (scm);
}

static size_t
scm_cairo_surface_free (SCM smob)
{
  cairo_surface_t *surf = (cairo_surface_t*)SCM_SMOB_DATA (smob);

  SCM_SET_SMOB_DATA (smob, NULL);
  cairo_surface_destroy (surf);

  return 0;
}

/**********************************************************************
 * cairo_pattern_t
 **********************************************************************/

SCM
scm_take_cairo_pattern (cairo_pattern_t *pat)
{
  SCM spat;

  SCM_NEWSMOB (spat, scm_tc16_cairo_pattern_t, pat);

  return spat;
}

SCM
scm_from_cairo_pattern (cairo_pattern_t *pat)
{
  return scm_take_cairo_pattern (cairo_pattern_reference (pat));
}

cairo_pattern_t*
scm_to_cairo_pattern (SCM scm)
{
  scm_assert_smob_type (scm_tc16_cairo_pattern_t, scm);
  return (cairo_pattern_t*)SCM_SMOB_DATA (scm);
}

static size_t
scm_cairo_pattern_free (SCM smob)
{
  cairo_pattern_t *pat = (cairo_pattern_t*)SCM_SMOB_DATA (smob);

  SCM_SET_SMOB_DATA (smob, NULL);
  cairo_pattern_destroy (pat);

  return 0;
}

/**********************************************************************
 * cairo_font_face_t
 **********************************************************************/

SCM
scm_take_cairo_font_face (cairo_font_face_t *pat)
{
  SCM spat;

  SCM_NEWSMOB (spat, scm_tc16_cairo_font_face_t, pat);

  return spat;
}

SCM
scm_from_cairo_font_face (cairo_font_face_t *pat)
{
  return scm_take_cairo_font_face (cairo_font_face_reference (pat));
}

cairo_font_face_t*
scm_to_cairo_font_face (SCM scm)
{
  scm_assert_smob_type (scm_tc16_cairo_font_face_t, scm);
  return (cairo_font_face_t*)SCM_SMOB_DATA (scm);
}

static size_t
scm_cairo_font_face_free (SCM smob)
{
  cairo_font_face_t *pat = (cairo_font_face_t*)SCM_SMOB_DATA (smob);

  SCM_SET_SMOB_DATA (smob, NULL);
  cairo_font_face_destroy (pat);

  return 0;
}

/**********************************************************************
 * cairo_scaled_font_t
 **********************************************************************/

SCM
scm_take_cairo_scaled_font (cairo_scaled_font_t *pat)
{
  SCM spat;

  cairo_scaled_font_reference (pat);
  SCM_NEWSMOB (spat, scm_tc16_cairo_scaled_font_t, pat);

  return spat;
}

SCM
scm_from_cairo_scaled_font (cairo_scaled_font_t *pat)
{
  return scm_take_cairo_scaled_font (cairo_scaled_font_reference (pat));
}

cairo_scaled_font_t*
scm_to_cairo_scaled_font (SCM scm)
{
  scm_assert_smob_type (scm_tc16_cairo_scaled_font_t, scm);
  return (cairo_scaled_font_t*)SCM_SMOB_DATA (scm);
}

static size_t
scm_cairo_scaled_font_free (SCM smob)
{
  cairo_scaled_font_t *pat = (cairo_scaled_font_t*)SCM_SMOB_DATA (smob);

  SCM_SET_SMOB_DATA (smob, NULL);
  cairo_scaled_font_destroy (pat);

  return 0;
}

/**********************************************************************
 * cairo_font_options_t
 **********************************************************************/

SCM
scm_take_cairo_font_options (cairo_font_options_t *fopt)
{
  SCM sfopt;

  SCM_NEWSMOB (sfopt, scm_tc16_cairo_font_options_t, fopt);

  return sfopt;
}

cairo_font_options_t*
scm_to_cairo_font_options (SCM scm)
{
  scm_assert_smob_type (scm_tc16_cairo_font_options_t, scm);
  return (cairo_font_options_t*)SCM_SMOB_DATA (scm);
}

static size_t
scm_cairo_font_options_free (SCM smob)
{
  cairo_font_options_t *fopt = (cairo_font_options_t*)SCM_SMOB_DATA (smob);

  SCM_SET_SMOB_DATA (smob, NULL);
  cairo_font_options_destroy (fopt);

  return 0;
}

static SCM
scm_cairo_font_options_equalp (SCM this, SCM other)
{
  if (cairo_font_options_equal (scm_to_cairo_font_options (this),
                                scm_to_cairo_font_options (other)))
    return SCM_BOOL_T;
  else
    return SCM_BOOL_F;
}

/**********************************************************************
 * cairo_path_t
 **********************************************************************/

SCM
scm_take_cairo_path (cairo_path_t *path)
{
  SCM spath;

  SCM_NEWSMOB (spath, scm_tc16_cairo_path_t, path);

  return spath;
}

cairo_path_t*
scm_to_cairo_path (SCM scm)
{
  scm_assert_smob_type (scm_tc16_cairo_path_t, scm);
  return (cairo_path_t*)SCM_SMOB_DATA (scm);
}

static size_t
scm_cairo_path_free (SCM smob)
{
  cairo_path_t *path = (cairo_path_t*)SCM_SMOB_DATA (smob);

  SCM_SET_SMOB_DATA (smob, NULL);
  cairo_path_destroy (path);

  return 0;
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
/**********************************************************************
 * cairo_device_t
 **********************************************************************/

SCM
scm_take_cairo_device (cairo_device_t *device)
{
  SCM sdevice;

  SCM_NEWSMOB (sdevice, scm_tc16_cairo_device_t, device);

  return sdevice;
}

cairo_device_t*
scm_to_cairo_device (SCM scm)
{
  scm_assert_smob_type (scm_tc16_cairo_device_t, scm);
  return (cairo_device_t*)SCM_SMOB_DATA (scm);
}

static size_t
scm_cairo_device_free (SCM smob)
{
  cairo_device_t *device = (cairo_device_t*)SCM_SMOB_DATA (smob);

  SCM_SET_SMOB_DATA (smob, NULL);
  cairo_device_destroy (device);

  return 0;
}


/**********************************************************************
 * cairo_region_t
 **********************************************************************/

SCM
scm_take_cairo_region (cairo_region_t *region)
{
  SCM sregion;

  SCM_NEWSMOB (sregion, scm_tc16_cairo_region_t, region);

  return sregion;
}

cairo_region_t*
scm_to_cairo_region (SCM scm)
{
  scm_assert_smob_type (scm_tc16_cairo_region_t, scm);
  return (cairo_region_t*)SCM_SMOB_DATA (scm);
}

static size_t
scm_cairo_region_free (SCM smob)
{
  cairo_region_t *region = (cairo_region_t*)SCM_SMOB_DATA (smob);

  SCM_SET_SMOB_DATA (smob, NULL);
  cairo_region_destroy (region);

  return 0;
}

static SCM
scm_cairo_region_equalp (SCM this, SCM other)
{
  return scm_from_bool (cairo_region_equal (scm_to_cairo_region (this),
                                            scm_to_cairo_region (other)));
}
#endif /* 1.10 */

void
scm_init_cairo_smob_types (void)
{
#ifndef SCM_MAGIC_SNARFER
#include "guile-cairo-smob-types.x"
#endif
  scm_tc16_cairo_t = scm_make_smob_type ("cairo-context", 0);
  scm_set_smob_free (scm_tc16_cairo_t, scm_cairo_free);

  scm_tc16_cairo_surface_t = scm_make_smob_type ("cairo-surface", 0);
  scm_set_smob_free (scm_tc16_cairo_surface_t, scm_cairo_surface_free);

  scm_tc16_cairo_pattern_t = scm_make_smob_type ("cairo-pattern", 0);
  scm_set_smob_free (scm_tc16_cairo_pattern_t, scm_cairo_pattern_free);

  scm_tc16_cairo_scaled_font_t = scm_make_smob_type ("cairo-scaled-font", 0);
  scm_set_smob_free (scm_tc16_cairo_scaled_font_t, scm_cairo_scaled_font_free);

  scm_tc16_cairo_font_face_t = scm_make_smob_type ("cairo-font-face", 0);
  scm_set_smob_free (scm_tc16_cairo_font_face_t, scm_cairo_font_face_free);

  scm_tc16_cairo_font_options_t = scm_make_smob_type ("cairo-font-options", 0);
  scm_set_smob_free (scm_tc16_cairo_font_options_t, scm_cairo_font_options_free);
  scm_set_smob_equalp (scm_tc16_cairo_font_options_t, scm_cairo_font_options_equalp);

  scm_tc16_cairo_path_t = scm_make_smob_type ("cairo-path", 0);
  scm_set_smob_free (scm_tc16_cairo_path_t, scm_cairo_path_free);

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,10,0)
  scm_tc16_cairo_device_t = scm_make_smob_type ("cairo-device", 0);
  scm_set_smob_free (scm_tc16_cairo_device_t, scm_cairo_device_free);

  scm_tc16_cairo_region_t = scm_make_smob_type ("cairo-region", 0);
  scm_set_smob_free (scm_tc16_cairo_region_t, scm_cairo_region_free);
  scm_set_smob_equalp (scm_tc16_cairo_region_t, scm_cairo_region_equalp);
#endif /* 1.10 */
}

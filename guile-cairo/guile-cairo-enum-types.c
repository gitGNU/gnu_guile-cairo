/* guile-cairo
 * Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-enum-types.c: Cairo for Guile
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

#if CAIRO_HAS_SVG_SURFACE
#include <cairo-svg.h>
#endif  /* CAIRO_HAS_SVG_SURFACE */

#if CAIRO_HAS_PS_SURFACE
#include <cairo-ps.h>
#endif  /* CAIRO_HAS_PS_SURFACE */

#include "guile-cairo-compat.h"
#include "guile-cairo-enum-types.h"


typedef struct {
    int value;
    const char *name;
} EnumPair;


static SCM _scm_from_enum (EnumPair table[], int val)
{
    int i;
    for (i = 0; table[i].name; i++)
        if (table[i].value == val)
            return scm_from_locale_symbol (table[i].name);

    return scm_from_int (val);
}

static int _scm_to_enum (EnumPair table[], SCM scm)
{
    int i;
    for (i = 0; table[i].name; i++)
        if (scm_is_eq (scm, scm_from_locale_symbol (table[i].name)))
            return table[i].value;

    scm_error (scm_from_locale_symbol ("cairo-error"),
               NULL,
               "Unknown enumerated value: ~S",
               scm_list_1 (scm),
               SCM_EOL);
    /* not reached */
    return -1;
}
    
static SCM _get_values (EnumPair table[])
{
    int i;
    SCM ret = SCM_EOL;

    for (i = 0; table[i].name; i++)
        ret = scm_cons (scm_from_locale_symbol (table[i].name), ret);

    return scm_reverse_x (ret, SCM_EOL);
}
    
static EnumPair _status[] = {
    {CAIRO_STATUS_NO_MEMORY, "no-memory"},
    {CAIRO_STATUS_INVALID_RESTORE, "invalid-restore"},
    {CAIRO_STATUS_INVALID_POP_GROUP, "invalid-pop-group"},
    {CAIRO_STATUS_NO_CURRENT_POINT, "no-current-point"},
    {CAIRO_STATUS_INVALID_MATRIX, "invalid-matrix"},
    {CAIRO_STATUS_INVALID_STATUS, "invalid-status"},
    {CAIRO_STATUS_NULL_POINTER, "null-pointer"},
    {CAIRO_STATUS_INVALID_STRING, "invalid-string"},
    {CAIRO_STATUS_INVALID_PATH_DATA, "invalid-path-data"},
    {CAIRO_STATUS_READ_ERROR, "read-error"},
    {CAIRO_STATUS_WRITE_ERROR, "write-error"},
    {CAIRO_STATUS_SURFACE_FINISHED, "surface-finished"},
    {CAIRO_STATUS_SURFACE_TYPE_MISMATCH, "surface-type-mismatch"},
    {CAIRO_STATUS_PATTERN_TYPE_MISMATCH, "pattern-type-mismatch"},
    {CAIRO_STATUS_INVALID_CONTENT, "invalid-content"},
    {CAIRO_STATUS_INVALID_FORMAT, "invalid-format"},
    {CAIRO_STATUS_INVALID_VISUAL, "invalid-visual"},
    {CAIRO_STATUS_FILE_NOT_FOUND, "file-not-found"},
    {CAIRO_STATUS_INVALID_DASH, "invalid-dash"},
    {CAIRO_STATUS_INVALID_DSC_COMMENT, "invalid-dsc-comment"},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
    {CAIRO_STATUS_INVALID_INDEX, "invalid-index"},
    {CAIRO_STATUS_CLIP_NOT_REPRESENTABLE, "clip-not-representable"},
#endif
    {0, NULL}
};

static EnumPair _content[] = {
    {CAIRO_CONTENT_COLOR, "color"},
    {CAIRO_CONTENT_ALPHA, "alpha"},
    {CAIRO_CONTENT_COLOR_ALPHA, "color-alpha"},
    {0, NULL}
};

static EnumPair _operator[] = {
    {CAIRO_OPERATOR_CLEAR, "clear"},
    {CAIRO_OPERATOR_SOURCE, "source"},
    {CAIRO_OPERATOR_OVER, "over"},
    {CAIRO_OPERATOR_IN, "in"},
    {CAIRO_OPERATOR_OUT, "out"},
    {CAIRO_OPERATOR_ATOP, "atop"},
    {CAIRO_OPERATOR_DEST, "dest"},
    {CAIRO_OPERATOR_DEST_OVER, "dest-over"},
    {CAIRO_OPERATOR_DEST_IN, "dest-in"},
    {CAIRO_OPERATOR_DEST_OUT, "dest-out"},
    {CAIRO_OPERATOR_DEST_ATOP, "dest-atop"},
    {CAIRO_OPERATOR_XOR, "xor"},
    {CAIRO_OPERATOR_ADD, "add"},
    {CAIRO_OPERATOR_SATURATE, "saturate"},
    {0, NULL}
};

static EnumPair _antialias[] = {
    {CAIRO_ANTIALIAS_DEFAULT, "default"},
    {CAIRO_ANTIALIAS_NONE, "none"},
    {CAIRO_ANTIALIAS_GRAY, "gray"},
    {CAIRO_ANTIALIAS_SUBPIXEL, "subpixel"},
    {0, NULL}
};

static EnumPair _fill_rule[] = {
    {CAIRO_FILL_RULE_WINDING, "winding"},
    {CAIRO_FILL_RULE_EVEN_ODD, "even-odd"},
    {0, NULL}
};

static EnumPair _line_cap[] = {
    {CAIRO_LINE_CAP_BUTT, "butt"},
    {CAIRO_LINE_CAP_ROUND, "round"},
    {CAIRO_LINE_CAP_SQUARE, "square"},
    {0, NULL}
};

static EnumPair _line_join[] = {
    {CAIRO_LINE_JOIN_MITER, "miter"},
    {CAIRO_LINE_JOIN_ROUND, "round"},
    {CAIRO_LINE_JOIN_BEVEL, "bevel"},
    {0, NULL}
};

static EnumPair _font_slant[] = {
    {CAIRO_FONT_SLANT_NORMAL, "normal"},
    {CAIRO_FONT_SLANT_ITALIC, "italic"},
    {CAIRO_FONT_SLANT_OBLIQUE, "oblique"},
    {0, NULL}
};

static EnumPair _font_weight[] = {
    {CAIRO_FONT_WEIGHT_NORMAL, "normal"},
    {CAIRO_FONT_WEIGHT_BOLD, "bold"},
    {0, NULL}
};

static EnumPair _subpixel_order[] = {
    {CAIRO_SUBPIXEL_ORDER_DEFAULT, "default"},
    {CAIRO_SUBPIXEL_ORDER_RGB, "bold"},
    {CAIRO_SUBPIXEL_ORDER_BGR, "vbgr"},
    {CAIRO_SUBPIXEL_ORDER_VRGB, "vrgb"},
    {CAIRO_SUBPIXEL_ORDER_VBGR, "vbgr"},
    {0, NULL}
};

static EnumPair _hint_style[] = {
    {CAIRO_HINT_STYLE_DEFAULT, "default"},
    {CAIRO_HINT_STYLE_NONE, "none"},
    {CAIRO_HINT_STYLE_SLIGHT, "slight"},
    {CAIRO_HINT_STYLE_MEDIUM, "medium"},
    {CAIRO_HINT_STYLE_FULL, "full"},
    {0, NULL}
};

static EnumPair _hint_metrics[] = {
    {CAIRO_HINT_METRICS_DEFAULT, "default"},
    {CAIRO_HINT_METRICS_OFF, "off"},
    {CAIRO_HINT_METRICS_ON, "on"},
    {0, NULL}
};

static EnumPair _font_type[] = {
    {CAIRO_FONT_TYPE_TOY, "toy"},
    {CAIRO_FONT_TYPE_FT, "ft"},
    {CAIRO_FONT_TYPE_WIN32, "win32"},
    {CAIRO_FONT_TYPE_ATSUI, "atsui"},
    {0, NULL}
};

static EnumPair _path_data_type[] = {
    {CAIRO_PATH_MOVE_TO, "toy"},
    {CAIRO_PATH_LINE_TO, "line-to"},
    {CAIRO_PATH_CURVE_TO, "curve-to"},
    {CAIRO_PATH_CLOSE_PATH, "close-path"},
    {0, NULL}
};

static EnumPair _surface_type[] = {
    {CAIRO_SURFACE_TYPE_IMAGE, "image"},
    {CAIRO_SURFACE_TYPE_PDF, "pdf"},
    {CAIRO_SURFACE_TYPE_PS, "ps"},
    {CAIRO_SURFACE_TYPE_XLIB, "xlib"},
    {CAIRO_SURFACE_TYPE_XCB, "xcb"},
    {CAIRO_SURFACE_TYPE_GLITZ, "glitz"},
    {CAIRO_SURFACE_TYPE_QUARTZ, "quartz"},
    {CAIRO_SURFACE_TYPE_WIN32, "win32"},
    {CAIRO_SURFACE_TYPE_BEOS, "beos"},
    {CAIRO_SURFACE_TYPE_DIRECTFB, "directfb"},
    {CAIRO_SURFACE_TYPE_SVG, "svg"},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,4,0)
    {CAIRO_SURFACE_TYPE_OS2, "os2"},
#endif
    {0, NULL}
};

static EnumPair _format[] = {
    {CAIRO_FORMAT_ARGB32, "argb32"},
    {CAIRO_FORMAT_RGB24, "rgb24"},
    {CAIRO_FORMAT_A8, "a8"},
    {CAIRO_FORMAT_A1, "a1"},
    {0, NULL}
};

static EnumPair _pattern_type[] = {
    {CAIRO_PATTERN_TYPE_SOLID, "solid"},
    {CAIRO_PATTERN_TYPE_SURFACE, "surface"},
    {CAIRO_PATTERN_TYPE_LINEAR, "linear"},
    {CAIRO_PATTERN_TYPE_RADIAL, "radial"},
    {0, NULL}
};

static EnumPair _extend[] = {
    {CAIRO_EXTEND_NONE, "none"},
    {CAIRO_EXTEND_REPEAT, "repeat"},
    {CAIRO_EXTEND_REFLECT, "reflect"},
    {CAIRO_EXTEND_PAD, "pad"},
    {0, NULL}
};

static EnumPair _filter[] = {
    {CAIRO_FILTER_FAST, "solid"},
    {CAIRO_FILTER_GOOD, "good"},
    {CAIRO_FILTER_BEST, "best"},
    {CAIRO_FILTER_NEAREST, "nearest"},
    {CAIRO_FILTER_BILINEAR, "bilinear"},
    {CAIRO_FILTER_GAUSSIAN, "gaussian"},
    {0, NULL}
};

#if CAIRO_HAS_SVG_SURFACE
static EnumPair _svg_version[] = {
    {CAIRO_SVG_VERSION_1_1, "v1.1"},
    {CAIRO_SVG_VERSION_1_2, "v1.2"},
    {0, NULL}
};
#endif  /* CAIRO_HAS_SVG_SURFACE */

#if CAIRO_HAS_PS_SURFACE
static EnumPair _ps_level[] = {
    {CAIRO_PS_LEVEL_2, "level-2"},
    {CAIRO_PS_LEVEL_3, "level-3"},
    {0, NULL}
};
#endif  /* CAIRO_HAS_PS_SURFACE */


SCM scm_from_cairo_status (cairo_status_t cval) { return _scm_from_enum (_status, cval); }
cairo_status_t scm_to_cairo_status (SCM scm) { return _scm_to_enum (_status, scm); }

SCM scm_from_cairo_content (cairo_content_t cval) { return _scm_from_enum (_content, cval); }
cairo_content_t scm_to_cairo_content (SCM scm) { return _scm_to_enum (_content, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_content_get_values, "cairo-content-get-values", 0, 0, 0, (void), "")
  { return _get_values (_content); }

SCM scm_from_cairo_operator (cairo_operator_t cval) { return _scm_from_enum (_operator, cval); }
cairo_operator_t scm_to_cairo_operator (SCM scm) { return _scm_to_enum (_operator, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_operator_get_values, "cairo-operator-get-values", 0, 0, 0, (void), "")
  { return _get_values (_operator); }

SCM scm_from_cairo_antialias (cairo_antialias_t cval) { return _scm_from_enum (_antialias, cval); }
cairo_antialias_t scm_to_cairo_antialias (SCM scm) { return _scm_to_enum (_antialias, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_antialias_get_values, "cairo-antialias-get-values", 0, 0, 0, (void), "")
  { return _get_values (_antialias); }

SCM scm_from_cairo_fill_rule (cairo_fill_rule_t cval) { return _scm_from_enum (_fill_rule, cval); }
cairo_fill_rule_t scm_to_cairo_fill_rule (SCM scm) { return _scm_to_enum (_fill_rule, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_fill_rule_get_values, "cairo-fill-rule-get-values", 0, 0, 0, (void), "")
  { return _get_values (_fill_rule); }

SCM scm_from_cairo_line_cap (cairo_line_cap_t cval) { return _scm_from_enum (_line_cap, cval); }
cairo_line_cap_t scm_to_cairo_line_cap (SCM scm) { return _scm_to_enum (_line_cap, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_line_cap_get_values, "cairo-line-cap-get-values", 0, 0, 0, (void), "")
  { return _get_values (_line_cap); }

SCM scm_from_cairo_line_join (cairo_line_join_t cval) { return _scm_from_enum (_line_join, cval); }
cairo_line_join_t scm_to_cairo_line_join (SCM scm) { return _scm_to_enum (_line_join, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_line_join_get_values, "cairo-line-join-get-values", 0, 0, 0, (void), "")
  { return _get_values (_line_join); }

SCM scm_from_cairo_font_slant (cairo_font_slant_t cval) { return _scm_from_enum (_font_slant, cval); }
cairo_font_slant_t scm_to_cairo_font_slant (SCM scm) { return _scm_to_enum (_font_slant, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_font_slant_get_values, "cairo-font-slant-get-values", 0, 0, 0, (void), "")
  { return _get_values (_font_slant); }

SCM scm_from_cairo_font_weight (cairo_font_weight_t cval) { return _scm_from_enum (_font_weight, cval); }
cairo_font_weight_t scm_to_cairo_font_weight (SCM scm) { return _scm_to_enum (_font_weight, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_font_weight_get_values, "cairo-font-weight-get-values", 0, 0, 0, (void), "")
  { return _get_values (_font_weight); }

SCM scm_from_cairo_subpixel_order (cairo_subpixel_order_t cval) { return _scm_from_enum (_subpixel_order, cval); }
cairo_subpixel_order_t scm_to_cairo_subpixel_order (SCM scm) { return _scm_to_enum (_subpixel_order, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_subpixel_order_get_values, "cairo-subpixel-order-get-values", 0, 0, 0, (void), "")
  { return _get_values (_subpixel_order); }

SCM scm_from_cairo_hint_style (cairo_hint_style_t cval) { return _scm_from_enum (_hint_style, cval); }
cairo_hint_style_t scm_to_cairo_hint_style (SCM scm) { return _scm_to_enum (_hint_style, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_hint_style_get_values, "cairo-hint-style-get-values", 0, 0, 0, (void), "")
  { return _get_values (_hint_style); }

SCM scm_from_cairo_hint_metrics (cairo_hint_metrics_t cval) { return _scm_from_enum (_hint_metrics, cval); }
cairo_hint_metrics_t scm_to_cairo_hint_metrics (SCM scm) { return _scm_to_enum (_hint_metrics, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_hint_metrics_get_values, "cairo-hint-metrics-get-values", 0, 0, 0, (void), "")
  { return _get_values (_hint_metrics); }

SCM scm_from_cairo_font_type (cairo_font_type_t cval) { return _scm_from_enum (_font_type, cval); }
cairo_font_type_t scm_to_cairo_font_type (SCM scm) { return _scm_to_enum (_font_type, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_font_type_get_values, "cairo-font-type-get-values", 0, 0, 0, (void), "")
  { return _get_values (_font_type); }

SCM scm_from_cairo_path_data_type (cairo_path_data_type_t cval) { return _scm_from_enum (_path_data_type, cval); }
cairo_path_data_type_t scm_to_cairo_path_data_type (SCM scm) { return _scm_to_enum (_path_data_type, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_path_data_type_get_values, "cairo-path-data-type-get-values", 0, 0, 0, (void), "")
  { return _get_values (_path_data_type); }

SCM scm_from_cairo_surface_type (cairo_surface_type_t cval) { return _scm_from_enum (_surface_type, cval); }
cairo_surface_type_t scm_to_cairo_surface_type (SCM scm) { return _scm_to_enum (_surface_type, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_surface_type_get_values, "cairo-surface-type-get-values", 0, 0, 0, (void), "")
  { return _get_values (_surface_type); }

SCM scm_from_cairo_format (cairo_format_t cval) { return _scm_from_enum (_format, cval); }
cairo_format_t scm_to_cairo_format (SCM scm) { return _scm_to_enum (_format, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_format_get_values, "cairo-format-get-values", 0, 0, 0, (void), "")
  { return _get_values (_format); }

SCM scm_from_cairo_pattern_type (cairo_pattern_type_t cval) { return _scm_from_enum (_pattern_type, cval); }
cairo_pattern_type_t scm_to_cairo_pattern_type (SCM scm) { return _scm_to_enum (_pattern_type, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_pattern_type_get_values, "cairo-pattern-type-get-values", 0, 0, 0, (void), "")
  { return _get_values (_pattern_type); }

SCM scm_from_cairo_extend (cairo_extend_t cval) { return _scm_from_enum (_extend, cval); }
cairo_extend_t scm_to_cairo_extend (SCM scm) { return _scm_to_enum (_extend, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_extend_get_values, "cairo-extend-get-values", 0, 0, 0, (void), "")
  { return _get_values (_extend); }

SCM scm_from_cairo_filter (cairo_filter_t cval) { return _scm_from_enum (_filter, cval); }
cairo_filter_t scm_to_cairo_filter (SCM scm) { return _scm_to_enum (_filter, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_filter_get_values, "cairo-filter-get-values", 0, 0, 0, (void), "")
  { return _get_values (_filter); }

#if CAIRO_HAS_SVG_SURFACE
SCM scm_from_cairo_svg_version (cairo_svg_version_t cval) { return _scm_from_enum (_svg_version, cval); }
cairo_svg_version_t scm_to_cairo_svg_version (SCM scm) { return _scm_to_enum (_svg_version, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_svg_version_get_values, "cairo-svg-version-get-values", 0, 0, 0, (void), "")
  { return _get_values (_svg_version); }
#endif  /* CAIRO_HAS_SVG_SURFACE */

#if CAIRO_HAS_PS_SURFACE
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,6,0)
SCM scm_from_cairo_ps_level (cairo_ps_level_t cval) { return _scm_from_enum (_ps_level, cval); }
cairo_ps_level_t scm_to_cairo_ps_level (SCM scm) { return _scm_to_enum (_ps_level, scm); }
SCM_DEFINE_PUBLIC (scm_cairo_ps_level_get_values, "cairo-ps-level-get-values", 0, 0, 0, (void), "")
  { return _get_values (_ps_level); }
#endif  /* 1.6 */
#endif  /* CAIRO_HAS_PS_SURFACE */

void
scm_init_cairo_enum_types (void)
{
#ifndef SCM_MAGIC_SNARFER
#include "guile-cairo-enum-types.x"
#endif
    /* noop */
}

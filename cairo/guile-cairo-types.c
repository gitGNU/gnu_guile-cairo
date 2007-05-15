/* guile-cairo
 * Copyright (C) 2007 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-types.c: Cairo for Guile
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

scm_t_bits scm_tc16_cairo_t;
scm_t_bits scm_tc16_cairo_surface_t;
scm_t_bits scm_tc16_cairo_pattern_t;
scm_t_bits scm_tc16_cairo_scaled_font_t;
scm_t_bits scm_tc16_cairo_font_face_t;

SCM_SYMBOL (sym_f64, "f64");


typedef struct {
    int value;
    const char *name;
} EnumPair;


/**********************************************************************
 * cairo_t
 **********************************************************************/

SCM
scm_from_cairo_t (cairo_t *ctx)
{
    SCM sctx;

    cairo_reference (ctx);
    SCM_NEWSMOB (sctx, scm_tc16_cairo_t, ctx);

    return sctx;
}

cairo_t*
scm_to_cairo_t (SCM scm)
{
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

static int
scm_cairo_print (SCM smob, SCM port, scm_print_state *pstate)
{
    cairo_t *ctx = (cairo_t*)SCM_SMOB_DATA (smob);

    scm_puts ("#<cairo-context 0x", port);
    scm_display (scm_number_to_string (scm_from_ulong ((unsigned long)ctx),
                                       scm_from_int (16)),
                 port);    
    scm_puts (">", port);

    return 1;
}

/**********************************************************************
 * cairo_surface_t
 **********************************************************************/

SCM
scm_from_cairo_surface_t (cairo_surface_t *surf)
{
    SCM ssurf;

    cairo_surface_reference (surf);
    SCM_NEWSMOB (ssurf, scm_tc16_cairo_surface_t, surf);

    return ssurf;
}

cairo_surface_t*
scm_to_cairo_surface_t (SCM scm)
{
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

static int
scm_cairo_surface_print (SCM smob, SCM port, scm_print_state *pstate)
{
    cairo_surface_t *surf = (cairo_surface_t*)SCM_SMOB_DATA (smob);

    scm_puts ("#<cairo-surface 0x", port);
    scm_display (scm_number_to_string (scm_from_ulong ((unsigned long)surf),
                                       scm_from_int (16)),
                 port);
    scm_puts (">", port);

    return 1;
}

/**********************************************************************
 * cairo_matrix_t
 **********************************************************************/

SCM
scm_from_cairo_matrix_t (cairo_matrix_t *matrix)
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
scm_fill_cairo_matrix_t (SCM scm, cairo_matrix_t *matrix)
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
 * cairo_pattern_t
 **********************************************************************/

SCM
scm_from_cairo_pattern_t (cairo_pattern_t *pat)
{
    SCM spat;

    cairo_pattern_reference (pat);
    SCM_NEWSMOB (spat, scm_tc16_cairo_pattern_t, pat);

    return spat;
}

cairo_pattern_t*
scm_to_cairo_pattern_t (SCM scm)
{
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

static int
scm_cairo_pattern_print (SCM smob, SCM port, scm_print_state *pstate)
{
    cairo_pattern_t *pat = (cairo_pattern_t*)SCM_SMOB_DATA (smob);

    scm_puts ("#<cairo-pattern 0x", port);
    scm_display (scm_number_to_string (scm_from_ulong ((unsigned long)pat),
                                       scm_from_int (16)),
                 port);
    scm_puts (">", port);

    return 1;
}

/**********************************************************************
 * cairo_status_t
 **********************************************************************/

void
scm_c_check_cairo_status (cairo_status_t status, const char *subr)
{
    if (status == CAIRO_STATUS_SUCCESS)
        return;

    {
        const char *str = NULL;
        int i;
        const EnumPair values[] = {
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
            {CAIRO_STATUS_INVALID_INDEX, "invalid-index"},
            {CAIRO_STATUS_CLIP_NOT_REPRESENTABLE, "clip-not-representable"},
            {0, NULL}
        };

        for (i=0; values[i].name; i++) {
            if (values[i].value == status)
                str = values[i].name;
        }
        if (!str)
            str = "unknown";
            
        scm_error (scm_from_locale_symbol ("cairo-error"),
                   subr,
                   cairo_status_to_string (status),
                   scm_list_1 (scm_from_locale_symbol (str)),
                   scm_list_1 (scm_from_int (status)));
    }
}

/**********************************************************************
 * cairo_content_t
 **********************************************************************/

static EnumPair _content[] = {
    {CAIRO_CONTENT_COLOR, "color"},
    {CAIRO_CONTENT_ALPHA, "alpha"},
    {CAIRO_CONTENT_COLOR_ALPHA, "color-alpha"},
    {0, NULL}
};

/**********************************************************************
 * cairo_operator_t
 **********************************************************************/

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

/**********************************************************************
 * cairo_antialias_t
 **********************************************************************/

static EnumPair _antialias[] = {
    {CAIRO_ANTIALIAS_DEFAULT, "default"},
    {CAIRO_ANTIALIAS_NONE, "none"},
    {CAIRO_ANTIALIAS_GRAY, "gray"},
    {CAIRO_ANTIALIAS_SUBPIXEL, "subpixel"},
    {0, NULL}
};

/**********************************************************************
 * cairo_fill_rule_t
 **********************************************************************/

static EnumPair _fill_rule[] = {
    {CAIRO_FILL_RULE_WINDING, "winding"},
    {CAIRO_FILL_RULE_EVEN_ODD, "even-odd"},
    {0, NULL}
};

/**********************************************************************
 * cairo_line_cap_t
 **********************************************************************/

static EnumPair _line_cap[] = {
    {CAIRO_LINE_CAP_BUTT, "butt"},
    {CAIRO_LINE_CAP_ROUND, "round"},
    {CAIRO_LINE_CAP_SQUARE, "square"},
    {0, NULL}
};

/**********************************************************************
 * cairo_line_join_t
 **********************************************************************/

static EnumPair _line_join[] = {
    {CAIRO_LINE_JOIN_MITER, "miter"},
    {CAIRO_LINE_JOIN_ROUND, "round"},
    {CAIRO_LINE_JOIN_BEVEL, "bevel"},
    {0, NULL}
};

/**********************************************************************
 * cairo_rectangle_t
 **********************************************************************/

SCM
scm_from_cairo_rectangle (cairo_rectangle_t *rect)
{
    SCM ret = scm_make_f64vector (scm_from_int (4), scm_from_double (0.0));

    scm_f64vector_set_x (ret, scm_from_int (0), scm_from_double (rect->x));
    scm_f64vector_set_x (ret, scm_from_int (1), scm_from_double (rect->y));
    scm_f64vector_set_x (ret, scm_from_int (2), scm_from_double (rect->width));
    scm_f64vector_set_x (ret, scm_from_int (3), scm_from_double (rect->height));

    return ret;
}

void
scm_fill_cairo_rectangle (SCM scm, cairo_rectangle_t *rect)
{
#define GET(v,i) \
    scm_to_double (scm_vector_ref (v, scm_from_int (i)))

    rect->x = GET (scm, 0);
    rect->y = GET (scm, 1);
    rect->width = GET (scm, 2);
    rect->height = GET (scm, 3);
#undef GET
}

/**********************************************************************
 * cairo_font_face_t
 **********************************************************************/

SCM
scm_from_cairo_font_face_t (cairo_font_face_t *pat)
{
    SCM spat;

    cairo_font_face_reference (pat);
    SCM_NEWSMOB (spat, scm_tc16_cairo_font_face_t, pat);

    return spat;
}

cairo_font_face_t*
scm_to_cairo_font_face_t (SCM scm)
{
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

static int
scm_cairo_font_face_print (SCM smob, SCM port, scm_print_state *pstate)
{
    cairo_font_face_t *pat = (cairo_font_face_t*)SCM_SMOB_DATA (smob);

    scm_puts ("#<cairo-font-face 0x", port);
    scm_display (scm_number_to_string (scm_from_ulong ((unsigned long)pat),
                                       scm_from_int (16)),
                 port);
    scm_puts (">", port);

    return 1;
}

/**********************************************************************
 * cairo_scaled_font_t
 **********************************************************************/

SCM
scm_from_cairo_scaled_font_t (cairo_scaled_font_t *pat)
{
    SCM spat;

    cairo_scaled_font_reference (pat);
    SCM_NEWSMOB (spat, scm_tc16_cairo_scaled_font_t, pat);

    return spat;
}

cairo_scaled_font_t*
scm_to_cairo_scaled_font_t (SCM scm)
{
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

static int
scm_cairo_scaled_font_print (SCM smob, SCM port, scm_print_state *pstate)
{
    cairo_scaled_font_t *pat = (cairo_scaled_font_t*)SCM_SMOB_DATA (smob);

    scm_puts ("#<cairo-scaled-font 0x", port);
    scm_display (scm_number_to_string (scm_from_ulong ((unsigned long)pat),
                                       scm_from_int (16)),
                 port);
    scm_puts (">", port);

    return 1;
}

/**********************************************************************
 * cairo_glyph_t
 **********************************************************************/

SCM
scm_from_cairo_glyph (cairo_glyph_t *glyph)
{
    SCM ret = scm_make_vector (scm_from_int (4), SCM_BOOL_F);

    scm_vector_set_x (ret, scm_from_int (0), scm_from_int (glyph->index));
    scm_vector_set_x (ret, scm_from_int (1), scm_from_double (glyph->x));
    scm_vector_set_x (ret, scm_from_int (2), scm_from_double (glyph->y));

    return ret;
}

void
scm_fill_cairo_glyph (SCM scm, cairo_glyph_t *glyph)
{
#define GET(func,v,i) \
    func (scm_vector_ref (v, scm_from_int (i)))

    glyph->index = GET (scm_to_int, scm, 0);
    glyph->x = GET (scm_to_double, scm, 1);
    glyph->y = GET (scm_to_double, scm, 2);
}

#if 0
/**
 * cairo_text_extents_t:
 * @x_bearing: the horizontal distance from the origin to the
 *   leftmost part of the glyphs as drawn. Positive if the
 *   glyphs lie entirely to the right of the origin.
 * @y_bearing: the vertical distance from the origin to the
 *   topmost part of the glyphs as drawn. Positive only if the
 *   glyphs lie completely below the origin; will usually be
 *   negative.
 * @width: width of the glyphs as drawn
 * @height: height of the glyphs as drawn
 * @x_advance:distance to advance in the X direction
 *    after drawing these glyphs
 * @y_advance: distance to advance in the Y direction
 *   after drawing these glyphs. Will typically be zero except
 *   for vertical text layout as found in East-Asian languages.
 *
 * The #cairo_text_extents_t structure stores the extents of a single
 * glyph or a string of glyphs in user-space coordinates. Because text
 * extents are in user-space coordinates, they are mostly, but not
 * entirely, independent of the current transformation matrix. If you call
 * <literal>cairo_scale(cr, 2.0, 2.0)</literal>, text will
 * be drawn twice as big, but the reported text extents will not be
 * doubled. They will change slightly due to hinting (so you can't
 * assume that metrics are independent of the transformation matrix),
 * but otherwise will remain unchanged.
 **/
typedef struct {
    double x_bearing;
    double y_bearing;
    double width;
    double height;
    double x_advance;
    double y_advance;
} cairo_text_extents_t;

/**
 * cairo_font_extents_t:
 * @ascent: the distance that the font extends above the baseline.
 *          Note that this is not always exactly equal to the maximum
 *          of the extents of all the glyphs in the font, but rather
 *          is picked to express the font designer's intent as to
 *          how the font should align with elements above it.
 * @descent: the distance that the font extends below the baseline.
 *           This value is positive for typical fonts that include
 *           portions below the baseline. Note that this is not always
 *           exactly equal to the maximum of the extents of all the
 *           glyphs in the font, but rather is picked to express the
 *           font designer's intent as to how the the font should
 *           align with elements below it.
 * @height: the recommended vertical distance between baselines when
 *          setting consecutive lines of text with the font. This
 *          is greater than @ascent+@descent by a
 *          quantity known as the <firstterm>line spacing</firstterm>
 *          or <firstterm>external leading</firstterm>. When space
 *          is at a premium, most fonts can be set with only
 *          a distance of @ascent+@descent between lines.
 * @max_x_advance: the maximum distance in the X direction that
 *         the the origin is advanced for any glyph in the font.
 * @max_y_advance: the maximum distance in the Y direction that
 *         the the origin is advanced for any glyph in the font.
 *         this will be zero for normal fonts used for horizontal
 *         writing. (The scripts of East Asia are sometimes written
 *         vertically.)
 *
 * The #cairo_font_extents_t structure stores metric information for
 * a font. Values are given in the current user-space coordinate
 * system.
 *
 * Because font metrics are in user-space coordinates, they are
 * mostly, but not entirely, independent of the current transformation
 * matrix. If you call <literal>cairo_scale(cr, 2.0, 2.0)</literal>,
 * text will be drawn twice as big, but the reported text extents will
 * not be doubled. They will change slightly due to hinting (so you
 * can't assume that metrics are independent of the transformation
 * matrix), but otherwise will remain unchanged.
 **/
typedef struct {
    double ascent;
    double descent;
    double height;
    double max_x_advance;
    double max_y_advance;
} cairo_font_extents_t;

typedef enum _cairo_font_slant {
  CAIRO_FONT_SLANT_NORMAL,
  CAIRO_FONT_SLANT_ITALIC,
  CAIRO_FONT_SLANT_OBLIQUE
} cairo_font_slant_t;

typedef enum _cairo_font_weight {
  CAIRO_FONT_WEIGHT_NORMAL,
  CAIRO_FONT_WEIGHT_BOLD
} cairo_font_weight_t;

typedef enum _cairo_subpixel_order {
    CAIRO_SUBPIXEL_ORDER_DEFAULT,
    CAIRO_SUBPIXEL_ORDER_RGB,
    CAIRO_SUBPIXEL_ORDER_BGR,
    CAIRO_SUBPIXEL_ORDER_VRGB,
    CAIRO_SUBPIXEL_ORDER_VBGR
} cairo_subpixel_order_t;

typedef enum _cairo_hint_style {
    CAIRO_HINT_STYLE_DEFAULT,
    CAIRO_HINT_STYLE_NONE,
    CAIRO_HINT_STYLE_SLIGHT,
    CAIRO_HINT_STYLE_MEDIUM,
    CAIRO_HINT_STYLE_FULL
} cairo_hint_style_t;

typedef enum _cairo_hint_metrics {
    CAIRO_HINT_METRICS_DEFAULT,
    CAIRO_HINT_METRICS_OFF,
    CAIRO_HINT_METRICS_ON
} cairo_hint_metrics_t;

/**
 * cairo_font_options_t:
 *
 * An opaque structure holding all options that are used when
 * rendering fonts.
 *
 * Individual features of a #cairo_font_options_t can be set or
 * accessed using functions named
 * cairo_font_options_set_<emphasis>feature_name</emphasis> and
 * cairo_font_options_get_<emphasis>feature_name</emphasis>, like
 * cairo_font_options_set_antialias() and
 * cairo_font_options_get_antialias().
 *
 * New features may be added to a #cairo_font_options_t in the
 * future.  For this reason, cairo_font_options_copy(),
 * cairo_font_options_equal(), cairo_font_options_merge(), and
 * cairo_font_options_hash() should be used to copy, check
 * for equality, merge, or compute a hash value of
 * #cairo_font_options_t objects.
 **/
typedef struct _cairo_font_options cairo_font_options_t;

typedef enum _cairo_font_type {
    CAIRO_FONT_TYPE_TOY,
    CAIRO_FONT_TYPE_FT,
    CAIRO_FONT_TYPE_WIN32,
    CAIRO_FONT_TYPE_ATSUI
} cairo_font_type_t;

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

typedef enum _cairo_pattern_type {
    CAIRO_PATTERN_TYPE_SOLID,
    CAIRO_PATTERN_TYPE_SURFACE,
    CAIRO_PATTERN_TYPE_LINEAR,
    CAIRO_PATTERN_TYPE_RADIAL
} cairo_pattern_type_t;

typedef enum _cairo_extend {
    CAIRO_EXTEND_NONE,
    CAIRO_EXTEND_REPEAT,
    CAIRO_EXTEND_REFLECT,
    CAIRO_EXTEND_PAD
} cairo_extend_t;

typedef enum _cairo_filter {
    CAIRO_FILTER_FAST,
    CAIRO_FILTER_GOOD,
    CAIRO_FILTER_BEST,
    CAIRO_FILTER_NEAREST,
    CAIRO_FILTER_BILINEAR,
    CAIRO_FILTER_GAUSSIAN
} cairo_filter_t;
#endif

void
scm_init_cairo_types (void)
{
#ifndef SCM_MAGIC_SNARFER
#include "guile-cairo-types.x"
#endif
    scm_tc16_cairo_t = scm_make_smob_type ("cairo-context", 0);
    scm_set_smob_free (scm_tc16_cairo_t, scm_cairo_free);
    scm_set_smob_print (scm_tc16_cairo_t, scm_cairo_print);

    scm_tc16_cairo_surface_t = scm_make_smob_type ("cairo-surface", 0);
    scm_set_smob_free (scm_tc16_cairo_surface_t, scm_cairo_surface_free);
    scm_set_smob_print (scm_tc16_cairo_surface_t, scm_cairo_surface_print);

    scm_tc16_cairo_pattern_t = scm_make_smob_type ("cairo-pattern", 0);
    scm_set_smob_free (scm_tc16_cairo_pattern_t, scm_cairo_pattern_free);
    scm_set_smob_print (scm_tc16_cairo_pattern_t, scm_cairo_pattern_print);

    scm_tc16_cairo_scaled_font_t = scm_make_smob_type ("cairo-scaled-font", 0);
    scm_set_smob_free (scm_tc16_cairo_scaled_font_t, scm_cairo_scaled_font_free);
    scm_set_smob_print (scm_tc16_cairo_scaled_font_t, scm_cairo_scaled_font_print);

    scm_tc16_cairo_font_face_t = scm_make_smob_type ("cairo-font-face", 0);
    scm_set_smob_free (scm_tc16_cairo_font_face_t, scm_cairo_font_face_free);
    scm_set_smob_print (scm_tc16_cairo_font_face_t, scm_cairo_font_face_print);
}

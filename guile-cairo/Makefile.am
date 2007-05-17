## The library

lib_LTLIBRARIES = libguile-cairo-1.0.la

AM_CFLAGS = -I$(top_srcdir) $(WARN_CFLAGS) $(DEBUG_CFLAGS)

libguile_cairo_1_0_la_SOURCES = guile-cairo.c \
	guile-cairo-smob-types.c \
	guile-cairo-enum-types.c \
	guile-cairo-vector-types.c

libguile_cairo_1_0_la_CFLAGS = $(CAIRO_CFLAGS) $(AM_CFLAGS) $(GUILE_CFLAGS)

libguile_cairo_1_0_la_LIBADD = $(CAIRO_LIBS) $(GUILE_LIBS)
libguile_cairo_1_0_la_LDFLAGS = -export-dynamic

libguile_cairo_1_0_la_includedir = $(includedir)/guile-cairo
libguile_cairo_1_0_la_include_HEADERS = $(libguile_cairo_1_0_la_SOURCES:.c=.h)

## Snarfing

SUFFIXES = .x .doc

GUILE_SNARF_CFLAGS = $(DEFS) $(AM_CFLAGS) $(GUILE_CFLAGS) $(CAIRO_CFLAGS)

.c.x:
	guile-snarf $(GUILE_SNARF_CFLAGS) $< > $@ \
	|| { rm $@; false; }
.c.doc:
	-(guile-func-name-check $<)
	(guile-snarf-docs $(GUILE_SNARF_CFLAGS) $< | \
	guile_filter_doc_snarfage --filter-snarfage) > $@ || { rm $@; false; }

C_FILES = $(libguile_cairo_1_0_la_SOURCES)
DOT_X_FILES = $(C_FILES:.c=.x)
DOT_DOC_FILES = $(C_FILES:.c=.doc)

guile-cairo.texi: $(DOT_DOC_FILES)
	cat $(DOT_DOC_FILES) | guile-snarf-docs-texi > $@ \
	|| { rm $@; false; }

guile-cairo-procedures.txt: guile-cairo.texi
	rm -f $@
	makeinfo --force -o $@ $< || test -f $@

BUILT_SOURCES = $(DOT_X_FILES)

CLEANFILES = $(DOT_X_FILES)

#foo_DATA = guile-cairo-procedures.txt

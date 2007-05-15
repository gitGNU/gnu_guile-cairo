lib_LTLIBRARIES = libguile-cairo-1.0.la

AM_CFLAGS = $(WARN_CFLAGS) $(DEBUG_CFLAGS)

SUFFIXES = .x .doc

GUILE_SNARF_CFLAGS = $(DEFS) $(AM_CFLAGS) $(GUILE_CFLAGS)

.c.x:
	guile-snarf $(GUILE_SNARF_CFLAGS) $< > $@ \
	|| { rm $@; false; }
.c.doc:
	-(guile-func-name-check $<)
	(guile-snarf-docs $(GUILE_SNARF_CFLAGS) $< | \
	guile_filter_doc_snarfage --filter-snarfage) > $@ || { rm $@; false; }

C_FILES = guile-cairo.c guile-cairo-types.c
H_FILES = $(C_FILES:.c=.h)

libguile_cairo_1_0_la_SOURCES = $(C_FILES) $(H_FILES)

libguile_cairo_1_0_la_CFLAGS = $(CAIRO_CFLAGS) $(AM_CFLAGS) $(GUILE_CFLAGS)

libguile_cairo_1_0_la_LIBADD = $(CAIRO_LIBS) $(GUILE_LIBS)
libguile_cairo_1_0_la_LDFLAGS = -export-dynamic

libguile_cairo_1_0_includedir = $(includedir)/guile-cairo
libguile_cairo_1_0_include_HEADERS = $(H_FILES)

# Scheme files

GUILE_SNARF_CFLAGS += $(CAIRO_CFLAGS)

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

all-local: config.scm

real_builddir = $(shell cd $(top_builddir)/cairo && pwd)

moduledir=$(prefix)/share/guile/site/cairo

config.scm: Makefile config.scm.in
	sed -e "s|@cairolibpath\@|$(real_builddir)/libguile-cairo-1.0|" \
	    $(srcdir)/config.scm.in > config.scm

install-data-local: Makefile config.scm.in
	$(mkinstalldirs) $(DESTDIR)$(moduledir)
	sed -e "s|@cairolibpath\@|$(libdir)/libguile-cairo-1.0|" \
	    $(srcdir)/config.scm.in > $(DESTDIR)$(moduledir)/config.scm
	chmod 644 $(DESTDIR)$(moduledir)/config.scm

uninstall-local:
	rm -f $(DESTDIR)$(moduledir)/config.scm

CLEANFILES += config.scm

#guilegnome_DATA = guile-gnome-gobject-procedures.txt

EXTRA_DIST = config.scm.in $(module_DATA)

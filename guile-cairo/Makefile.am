# guile-cairo
# Copyright (C) 2007,2011  Andy Wingo <wingo@pobox.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program.  If not, see
# <http://www.gnu.org/licenses/>.

## The library

lib_LTLIBRARIES = libguile-cairo.la

AM_CFLAGS = -I$(srcdir) $(WARN_CFLAGS) $(DEBUG_CFLAGS)

libguile_cairo_la_SOURCES = guile-cairo.c \
	guile-cairo-smob-types.c \
	guile-cairo-enum-types.c \
	guile-cairo-vector-types.c \
	guile-cairo-compat.c

libguile_cairo_la_CFLAGS = $(CAIRO_CFLAGS) $(AM_CFLAGS) $(GUILE_CFLAGS)

libguile_cairo_la_LIBADD = $(CAIRO_LIBS) $(GUILE_LIBS)
libguile_cairo_la_LDFLAGS = -export-dynamic

libguile_cairo_la_includedir = $(includedir)/guile-cairo
libguile_cairo_la_include_HEADERS = $(libguile_cairo_la_SOURCES:.c=.h)
libguile_cairo_la_include_HEADERS = guile-cairo.h \
	guile-cairo-smob-types.h \
	guile-cairo-enum-types.h \
	guile-cairo-vector-types.h

noinst_HEADERS = guile-cairo-compat.h

## Snarfing

SUFFIXES = .x .doc

GUILE_SNARF_CFLAGS = $(libguile_cairo_la_CFLAGS)

.c.x:
	guile-snarf $(GUILE_SNARF_CFLAGS) $< > $@ \
	|| { rm $@; false; }
.c.doc:
	-(guile-func-name-check $<)
	(guile-snarf-docs $(GUILE_SNARF_CFLAGS) $< | \
	guile_filter_doc_snarfage --filter-snarfage) > $@ || { rm $@; false; }

C_FILES = $(libguile_cairo_la_SOURCES)
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

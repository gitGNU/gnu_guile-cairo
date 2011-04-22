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

all-local: config.scm

lib_builddir = $(shell cd $(top_builddir)/guile-cairo && pwd)
docs_builddir = $(shell cd $(top_builddir)/doc && pwd)

moduledir=$(prefix)/share/guile/site/cairo

module_DATA = vector-types.scm cairo-procedures.txt

config.scm: Makefile config.scm.in
	sed -e "s|@cairolibpath\@|$(lib_builddir)/libguile-cairo|" \
	    -e "s|@cairodocumentationpath\@|$(docs_builddir)/cairo-procedures.txt|" \
	    $(srcdir)/config.scm.in > config.scm

install-data-local: Makefile config.scm.in
	$(mkinstalldirs) $(DESTDIR)$(moduledir)
	sed -e "s|@cairolibpath\@|$(libdir)/libguile-cairo|" \
	    -e "s|@cairodocumentationpath\@|$(moduledir)/cairo-procedures.txt|" \
	    $(srcdir)/config.scm.in > $(DESTDIR)$(moduledir)/config.scm
	chmod 644 $(DESTDIR)$(moduledir)/config.scm

uninstall-local:
	rm -f $(DESTDIR)$(moduledir)/config.scm

CAIRO_SRC_DIR=~/src/gnome2/cairo
cairo-procedures.txt.update:
	echo "Generated from upstream documentation; see COPYING.docs for info." \
	  > $(srcdir)/cairo-procedures.txt
	$(top_srcdir)/doc/docbook-to-guile-doc $(CAIRO_SRC_DIR)/doc/public/xml/*.xml \
	  >> $(srcdir)/cairo-procedures.txt

CLEANFILES = config.scm

EXTRA_DIST = config.scm.in $(module_DATA)

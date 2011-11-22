# guile-cairo
# Copyright (C) 2007,2011  Andy Wingo <wingo@pobox.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 2 of the
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

www: clean-www guile-cairo.pdf guile-cairo-html
	find www -name 'index.scm' -print \
	  | while read f; do \
              guile -l "$$f" -c '(make-index)' \
	        > `echo $$f | sed -e s,\.scm,\.html,`; \
	    done
	cp guile-cairo.pdf www/docs/
	rm -rf www/docs/html
	cp -a guile-cairo/ www/docs/html

WEB_REPO=cvs.sv.gnu.org:/web/guile-cairo
IMPORT_TAG=git_$(shell git show-ref --head --hash HEAD)

www-commit: www
	( cd www; cvs -d $(WEB_REPO) import -m 'make www in guile-cairo/docs' guile-cairo git $(IMPORT_TAG) )

clean-www:
	find www -name 'index.html' -print \
	  | while read f; do rm -f "$$f"; done

clean: clean-www

guile-cairo-html:
	makeinfo --html $(srcdir)/guile-cairo.texi

include generate-docs.mk

GUILE = $(top_builddir)/env guile
cairo_docbook_xml_wildcard = ~/src/gnome2/cairo/doc/public/xml/cairo*.xml
docbook_xml_files = $(wildcard $(cairo_docbook_xml_wildcard))
GTK_DOC_DEFUN_HEURISTICS_METHOD = heuristics
GTK_DOC_DEFUN_HEURISTICS_ARGS = (cairo)

EXTRA_DIST = README overrides.texi generate-docs.mk COPYING.docs COPYING.LGPL-2.1 COPYING.MPL-1.1

info_TEXINFOS=guile-cairo.texi
guile_cairo_TEXINFOS=$(wildcard section-*.texi) $(wildcard defuns-*.texi)

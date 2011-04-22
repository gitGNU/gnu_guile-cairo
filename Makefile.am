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

SUBDIRS = guile-cairo cairo examples tests doc

env: Makefile env.in
	sed -e "s|@top_srcdir\@|$(shell cd $(top_srcdir) && pwd)|" \
	    $(srcdir)/env.in > env
	chmod +x env

BUILT_SOURCES = env
CLEANFILES = env

scmdir=$(prefix)/share/guile/site

scm_DATA = cairo.scm

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = guile-cairo.pc

EXTRA_DIST = env.in autogen.sh HACKING COPYING.LESSER COPYING.docs $(scm_DATA)

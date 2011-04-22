define(GUILE_CAIRO_CONFIGURE_COPYRIGHT,[[

Copyright (C) 2007, 2011 Andy Wingo <wingo@pobox.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program.  If not, see
<http://www.gnu.org/licenses/>.

]])

AC_PREREQ(2.52)
AC_INIT(HACKING)
AC_CONFIG_AUX_DIR([build-aux])
AC_COPYRIGHT(GUILE_CAIRO_CONFIGURE_COPYRIGHT)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(guile-cairo, 1.4.1)

AC_SUBST(VERSION,1.4.1)

AC_DISABLE_STATIC

AC_ISC_POSIX
AC_PROG_CC
AC_STDC_HEADERS
AC_PROG_LIBTOOL

if test "x$prefix" = xNONE; then
   AC_MSG_ERROR([No explicit prefix given.

Guile-Cairo requires you to explicitly enter in a prefix when
configuring. This is because the default prefix, /usr/local, is not in
the default guile load path. What you probably want to do is to run

  $0 --prefix=/usr

so that guile can find the guile-cairo modules when they are installed.

If you choose to install to a prefix that is not in guile's load path,
you will need to modify GUILE_LOAD_PATH and LD_LIBRARY_PATH every time
you run guile. (Which is a pain.)])
fi

WARN_CFLAGS=-Wall
AC_ARG_ENABLE([Werror], AC_HELP_STRING([--disable-Werror],[Don't stop the build on errors]),
        [], WARN_CFLAGS="-Wall -Werror")
AC_SUBST(WARN_CFLAGS)

# Optimize the DSO symbol hash table -- see ulrich drepper's paper,
# "how to write shared libraries"
GNULD_LDFLAGS=-Wl,-O1
AC_SUBST(GNULD_LDFLAGS)
AM_LDFLAGS='$(GNULD_LDFLAGS)'
AC_SUBST(AM_LDFLAGS)

#
# Check for Guile
#
GUILE_EFFECTIVE_VERSION=no
all_versions="2.2 2.0 1.8"
for v in $all_versions; do
  if test $GUILE_EFFECTIVE_VERSION = no; then
    AC_MSG_NOTICE([checking for Guile $v.x])
    PKG_CHECK_MODULES(GUILE, guile-$v, [GUILE_EFFECTIVE_VERSION=$v], [true])
  fi
done

AC_SUBST(GUILE_EFFECTIVE_VERSION)

if test $GUILE_EFFECTIVE_VERSION = no; then
  AC_MSG_ERROR([Failed to find Guile.  Is it installed?])
else
  AC_MSG_NOTICE([found Guile $GUILE_EFFECTIVE_VERSION.x])
fi

PKG_CHECK_MODULES(CAIRO, cairo >= 1.2.0)
AC_SUBST(CAIRO_LIBS)
AC_SUBST(CAIRO_CFLAGS)

AC_CONFIG_FILES(
Makefile
guile-cairo.pc
guile-cairo/Makefile
cairo/Makefile
examples/Makefile
tests/Makefile
tests/unit-tests/Makefile
doc/Makefile
)
AC_OUTPUT

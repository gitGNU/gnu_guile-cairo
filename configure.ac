AC_PREREQ(2.52)
AC_INIT(HACKING)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(guile-cairo, 1.3.93)

AC_SUBST(VERSION,1.3.93)

AM_MAINTAINER_MODE
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
AC_MSG_CHECKING(for Guile)
guile-config link > /dev/null || {
   echo "configure: cannot find guile-config; is Guile installed?" 1>&2
   exit 1
}
GUILE_VERSION=`guile-config info guileversion`
if test "$GUILE_VERSION" \< 1.6.4; then
   AC_MSG_ERROR([Guile 1.6.4 or newer is required, but you only have $GUILEVERSION.])
fi
GUILE_CFLAGS="`guile-config compile`"
GUILE_LIBS="`guile-config link`"
AC_SUBST(GUILE_CFLAGS)
AC_SUBST(GUILE_LIBS)
AC_MSG_RESULT(yes)

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

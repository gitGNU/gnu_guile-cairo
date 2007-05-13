SUBDIRS = cairo examples tests

env: Makefile env.in
	sed -e "s|@top_srcdir\@|$(shell cd $(top_srcdir) && pwd)|" \
	    $(srcdir)/env.in > env
	chmod +x env

BUILT_SOURCES = env
CLEANFILES = env

scmdir=$(prefix)/share/guile/site

scm_DATA = cairo.scm

EXTRA_DIST = env.in autogen.sh autogen-support.sh HACKING $(scm_DATA)

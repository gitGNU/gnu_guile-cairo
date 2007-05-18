all-local: config.scm

lib_builddir = $(shell cd $(top_builddir)/guile-cairo && pwd)

moduledir=$(prefix)/share/guile/site/cairo

module_DATA = vector-types.scm

config.scm: Makefile config.scm.in
	sed -e "s|@cairolibpath\@|$(lib_builddir)/libguile-cairo-1.0|" \
	    $(srcdir)/config.scm.in > config.scm

install-data-local: Makefile config.scm.in
	$(mkinstalldirs) $(DESTDIR)$(moduledir)
	sed -e "s|@cairolibpath\@|$(libdir)/libguile-cairo-1.0|" \
	    $(srcdir)/config.scm.in > $(DESTDIR)$(moduledir)/config.scm
	chmod 644 $(DESTDIR)$(moduledir)/config.scm

uninstall-local:
	rm -f $(DESTDIR)$(moduledir)/config.scm

CLEANFILES = config.scm

EXTRA_DIST = config.scm.in $(module_DATA)

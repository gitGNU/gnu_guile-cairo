TESTS= \
	api-stability.scm \
	version.scm

TESTS_ENVIRONMENT=$(top_builddir)/env guile -s

%.api.update:
	$(srcdir)/update-api $(subst ., ,$*) > $(srcdir)/$*.api

%.check: %
	$(TESTS_ENVIRONMENT) $*

EXTRA_DIST=$(TESTS)

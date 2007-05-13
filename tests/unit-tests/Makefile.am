TESTS= \
	version.scm

TESTS_ENVIRONMENT=$(top_builddir)/env guile -s

%.check: %
	$(TESTS_ENVIRONMENT) $*

EXTRA_DIST=$(TESTS)

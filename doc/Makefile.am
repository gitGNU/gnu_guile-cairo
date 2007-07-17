www: clean-www guile-cairo.pdf guile-cairo-html
	find www -name 'index.scm' -print \
	  | while read f; do \
              guile -l "$$f" -c '(make-index)' \
	        > `echo $$f | sed -e s,\.scm,\.html,`; \
	    done
	cp guile-cairo.pdf www/docs/
	rm -rf www/docs/html
	cp -a guile-cairo/ www/docs/html

www-commit: www
	svn rm -m 'make www in guile-lib/docs' svn+ssh://wingo@svn.gna.org/svn/guile-cairo/website
	svn import -m 'make www in guile-lib/docs' www svn+ssh://wingo@svn.gna.org/svn/guile-cairo/website

clean-www:
	find www -name 'index.html' -print \
	  | while read f; do rm -f "$$f"; done

clean: clean-www

guile-cairo-html:
	makeinfo --html $(srcdir)/guile-cairo.texi

cairo_docbook_xml_wildcard = ~/src/gnome2/cairo/doc/public/xml/cairo*.xml

generate-stubs:
	./generate-doc-stubs $(wildcard $(cairo_docbook_xml_wildcard))

generate-defuns:
	./generate-doc-defuns $(wildcard $(cairo_docbook_xml_wildcard))

EXTRA_DIST = docbook-to-guile-doc

info_TEXINFOS=guile-cairo.texi
guile_cairo_TEXINFOS=$(wildcard section-*.texi) $(wildcard defuns-*.texi)

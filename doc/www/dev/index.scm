(define main-archive-url "http://arch.gna.org/guile-cairo/trunk")

(define page
  `((h2 "developer information")

    (h3 "dependencies")

    (p (code "guile-cairo") " currently requires Cairo 1.4,
although patches providing back compatibility will be accepted.")

    (p (code "guile-cairo") " works with the current "
       (a (@ (href "http://www.gnu.org/software/guile/")) "Guile") " 1.8.")

    (h3 "source repository")

    (p "guile-cairo is managed with "
       (a (@ (href "http://bazaar-vcs.org/")) "bzr") ", a distributed "
       "version control system. To grab guile-cairo, run the following:")

    (pre "bzr get " ,main-archive-url " guile-cairo\n"
         "cd guile-cairo\n"
         "./autogen.sh --prefix=... && make")

    (p "At that point you can install guile-cairo with " 
       (code "make install") ", or run it uninstalled using the "
       (code "env") " script.")

    (h3 "patches")

    (p "Send patches to " (code "guile-user") ", for now, and
copy Andy Wingo (" (code "wingo pobox com") "). We will have a
freedesktop.org bug tracker in the future.")

    (h3 "gna project page")

    (p "We also have
a " (a (@ (href "http://gna.org/projects/guile-cairo")) "page
on GNA!") ".")))

(load "../template.scm")
(define (make-index)
  (output-html page "guile-cairo: developers" "developers" "../"))

(define page
  `((h2 "developer information")

    (h3 "dependencies")

    (p (code "guile-cairo") " wraps the API from Cairo version 1.10 and
later, though you can build it with Cairo as old as 1.2.")

    (p (code "guile-cairo") " works with the current "
       (a (@ (href "http://www.gnu.org/software/guile/")) "Guile") "
version 2.0, as well as with the previous 1.8 stable series.")

    (h3 "source repository")

    (p "guile-cairo is managed with "
       (a (@ (href "http://git-scm.org/")) "git") ", a distributed "
       "version control system. To grab guile-cairo, run the following:")

    (pre "git clone git://git.savannah.nongnu.org/guile-cairo.git\n"
         "cd guile-cairo\n"
         "./autogen.sh --prefix=... && make")

    (p "At that point you can install guile-cairo with " 
       (code "make install") ", or run it uninstalled using the "
       (code "env") " script.")

    (h3 "browse the code")

    (p "Browse the source code on "
       (a (@ (href "http://git.savannah.nongnu.org/cgit/guile-cairo.git/"))
          "Savannah's cgit instance") ".")

    (h3 "patches and bug reports")

    (p "File bug reports, etc in
our " (a (@ (href "https://bugs.freedesktop.org/enter_bug.cgi?product=guile-cairo")) "bug
tracker") " hosted by freedesktop.org. If you're more of the mailing
list type, you can send queries to " (code "guile-user") ", and copy Andy
Wingo (" (code "wingo@pobox.com") ").")

    (h3 "gna project page")

    (p "We also have
a " (a (@ (href "https://savannah.nongnu.org/projects/guile-cairo")) "page
on savannah") ".")))

(load "../template.scm")
(define (make-index)
  (output-html page "guile-cairo: developers" "developers" "../"))

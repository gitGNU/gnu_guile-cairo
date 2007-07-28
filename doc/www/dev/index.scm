(define main-archive-url "http://arch.gna.org/guile-cairo/trunk")

(define page
  `((h2 "developer information")

    (h3 "dependencies")

    (p (code "guile-cairo") " currently requires Cairo 1.2,
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

    (h3 "browse the code, get commit mails, etc")

    (p "Our source code, while hosted primarily at gna.org, is mirrored
over at " (a (@ (href "http://code.launchpad.net/guile-cairo/")) "launchpad.net") ".
Visit that page to browse the source code, register your
branch, " (a (@ (href "https://code.launchpad.net/~andywingo/guile-cairo/trunk/+subscribe")) "sign
up for commit mails") ", etc.")

    (h3 "patches and bug reports")

    (p "File bug reports, etc in
our " (a (@ (href "https://bugs.freedesktop.org/enter_bug.cgi?product=guile-cairo")) "bug
tracker") " hosted by freedesktop.org. If you're more of the mailing
list type, you can send queries to " (code "guile-user") ", and copy Andy
Wingo (" (code "wingo pobox com") ").")

    (h3 "gna project page")

    (p "We also have
a " (a (@ (href "http://gna.org/projects/guile-cairo")) "page
on GNA!") ".")))

(load "../template.scm")
(define (make-index)
  (output-html page "guile-cairo: developers" "developers" "../"))

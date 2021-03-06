(define page
  '((h2 "news")

    (news
     (@ (date "22 November 2011")
        (title "Guile-Cairo moved to savannah; 1.9.91 available"))
     "Guile-Cairo has changed hosting providers, moving to "
     (a (@ (href "http://www.non-gnu.org/guile-cairo/")) "savannah")
     ", where you are probably reading this message.  A new release has
been made, in order to link to the new location of the git repository,
and other such things.  Version 1.9.91 is still a prerelease.  Give it a
try!")

    (news
     (@ (date "23 April 2011")
        (title "Guile-Cairo 1.9.90 available"))
     "Guile-Cairo 1.9.90 has been released, adding support for Cairo API
from versions 1.6, 1.8, and 1.10.  This is a prerelease.  Give it a try!")

    (news
     (@ (date "23 April 2011")
        (title "Guile-Cairo 1.4.1 available"))
     "Guile-Cairo 1.4.1 has been released, updating to the LGPLv3+.")

    (news
     (@ (date "28 July 2007")
        (title "Guile-Cairo 1.4.0 available"))
     "Guile-Cairo 1.4.0 has been released. We are now API stable! Also
check the new code, bug list, and new bug links on all web site pages:
thanks to freedesktop.org for the bugzilla, and launchpad.net for its
sweet code
browser. " (a (@ (href "https://code.launchpad.net/~andywingo/guile-cairo/trunk/+subscribe")) "Sign
up for commit mails!"))

    (news
     (@ (date "28 July 2007")
        (title "Guile-Cairo 1.4.0 available"))
     "Guile-Cairo 1.4.0 has been released. We are now API stable! Also
check the new code, bug list, and new bug links on all web site pages:
thanks to freedesktop.org for the bugzilla, and launchpad.net for its
sweet code
browser. " (a (@ (href "https://code.launchpad.net/~andywingo/guile-cairo/trunk/+subscribe")) "Sign
up for commit mails!"))

    (news
     (@ (date "17 July 2007")
        (title "Guile-Cairo 1.3.93 available"))
     "Guile-Cairo 1.3.93 has been released, featuring " (rlink "docs/" "documentation") ".")
    
    (news
     (@ (date "12 June 2007")
        (title "Guile-Cairo 1.3.92 available"))
     "Guile-Cairo 1.3.92 has been released, featuring input from and output to Scheme ports.")

    (news
     (@ (date "8 June 2007")
        (title "Guile-Cairo 1.3.91 available"))
     "Guile-Cairo 1.3.91 has been released, featuring Cairo 1.2 support.")

    (news
     (@ (date "18 May 2007")
        (title "initial release"))
     "Guile-Cairo 1.3.90 released.")

    ))


(load "../template.scm")

(define (news tag args . body)
  `(div (h4 ,@(assq-ref (cdr args) 'date) ": "
            ,@(assq-ref (cdr args) 'title))
        (p ,@body)))

(define (make-index)
  (output-html page "guile-cairo: news" "news" "../"
               #:transform-rules `((news . ,news))))

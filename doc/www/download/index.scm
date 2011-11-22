(define page
  '((h2 "releases")

    (p "Current releases are available from our "
       (a (@ (href "http://download.savannah.gnu.org/releases/guile-cairo/"))
          "release area")
       " on savannah.")
    (p "Before moving to Savannah, guile-cairo was on GNA!, so you can find the older releases "
       (a (@ (href "http://download.gna.org/guile-cairo/"))
          "over there")
       ".")))

(load "../template.scm")
(define (make-index)
  (output-html page "guile-cairo: download" "download" "../"))

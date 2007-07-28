(define page
  `((h2 "documentation")

    (h3 "reference manual")

    (p (code "guile-cairo") " has a reference manual automatically
generated from upstream Cairo documentation. This means that the manual
is complete, mostly accurate, but at times C-isms remain in the
generated documentation. We will be fixing this with time.")

    (ul (li (a (@ (href "html/")) "HTML"))
        (li (a (@ (href "guile-cairo.pdf")) "PDF")))

    (p "Once you install " (code "guile-cairo") ", you can access
documentation from within Info (" (code "info guile-cairo") ") or from
within Guile itself " (code "(help cairo-create)") ". If you are
interested in helping to improve the documentation, there is information
in the source tree, in the file " (tt "doc/README") ".")))

(load "../template.scm")
(define (make-index)
  (output-html page "guile-cairo: documentation" "documentation" "../"))

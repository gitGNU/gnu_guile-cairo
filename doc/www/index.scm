(define page
  '((h2 "guile-cairo")

    (p "Guile-Cairo wraps
the " (a (@ (href "http://cairographics.org/")) "Cairo graphics
library") "
for " (a (@ (href "http://www.gnu.org/software/guile/")) "Guile
Scheme") ".")

    (p "Guile-Cairo is complete, wrapping almost all of the Cairo API.
It is API stable, providing a firm base on which to do graphics work.
Finally, and importantly, it is pleasant to use. You get a powerful and
well-maintained graphics library with all of the benefits of Scheme:
memory management, exceptions, macros, and a dynamic programming
environment.")

    (h3 (@ (style "text-align: center")) "latest news")

    (latest-news)

    (h4 (rlink "news/" "older news..."))))

(load "template.scm")

(define this-page page)
(load "news/index.scm")
(define news-page page)
(define page this-page)

(define (news tag args . body)
  `(div (h4 ,@(assq-ref (cdr args) 'date) ": "
            ,@(assq-ref (cdr args) 'title))
        (p ,@body)))

(define (latest-news . body)
  (cadr news-page))

(define (make-index)
  (output-html page "guile-cairo" "guile-cairo" ""
               #:transform-rules `((news . ,news)
                                   (latest-news *macro* . ,latest-news))))

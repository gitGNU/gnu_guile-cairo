(define page
  '((h2 "guile-cairo")

    (p "Guile-Cairo wraps
the " (a (@ (href "http://cairographics.org/")) "Cairo graphics
library") "
for " (a (@ (href "http://www.gnu.org/software/guile/")) "Guile
Scheme") ".")

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

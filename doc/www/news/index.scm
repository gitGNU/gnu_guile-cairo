(define page
  '((h2 "news")

    (news
     (@ (date "18 May 2007")
        (title "initial release"))
     (code "guile-cairo") " 1.3.0 released.")
    ))


(load "../template.scm")

(define (news tag args . body)
  `(div (h4 ,@(assq-ref (cdr args) 'date) ": "
            ,@(assq-ref (cdr args) 'title))
        (p ,@body)))

(define (make-index)
  (output-html page "guile-cairo: news" "news" "../"
               #:transform-rules `((news . ,news))))

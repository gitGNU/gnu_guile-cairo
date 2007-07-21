;;; ----------------------------------------------------------------------
;;;    unit test
;;;    Copyright (C) 2007 Andy Wingo
;;;
;;;    This program is free software; you can redistribute it and/or modify
;;;    it under the terms of the GNU General Public License as published by
;;;    the Free Software Foundation; either version 2 of the License, or
;;;    (at your option) any later version.
;;;
;;;    This program is distributed in the hope that it will be useful,
;;;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;;;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;;    GNU General Public License for more details.
;;;
;;;    You should have received a copy of the GNU General Public License
;;;    along with this program; if not, write to the Free Software
;;;    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
;;; ----------------------------------------------------------------------

(use-modules (unit-test)
             (oop goops)
             ((srfi srfi-1) #:select (lset-difference)))

(define (module-all-uses module)
  (let ((direct (module-uses module)))
    (append direct
            (apply append (map module-all-uses direct)))))

(define (module-exports module)
  (module-map (lambda (k v) k) module))

(define (module-all-exports module-name)
  (let ((interface (resolve-interface module-name)))
    (map
     string->symbol
     (sort
      (apply
       append
       (map
        (lambda (module)
          (map symbol->string (module-exports module)))
        (cons interface (module-all-uses interface))))
      string<?))))

(define-class <test-api> (<test-case>))

(define-method (test-api (self <test-api>))
  (let ((expected (call-with-input-file (getenv "API_FILE") read))
        (actual (module-all-exports '(cairo))))
    (let ((new (lset-difference eq? actual expected)))
      (if (not (null? new))
          (warning "New API, run make cairo.api.update" new)))
    (assert-equal '()
                  (lset-difference eq? expected actual))))

(exit-with-summary (run-all-defined-test-cases))

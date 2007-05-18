;; guile-cairo
;; Copyright (C) 2007 Free Software Foundation, Inc.

;; This program is free software; you can redistribute it and/or    
;; modify it under the terms of the GNU General Public License as   
;; published by the Free Software Foundation; either version 2 of   
;; the License, or (at your option) any later version.              
;;                                                                  
;; This program is distributed in the hope that it will be useful,  
;; but WITHOUT ANY WARRANTY; without even the implied warranty of   
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    
;; GNU General Public License for more details.                     
;;                                                                  
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, contact:
;;
;; Free Software Foundation           Voice:  +1-617-542-5942
;; 59 Temple Place - Suite 330        Fax:    +1-617-542-2652
;; Boston, MA  02111-1307,  USA       gnu@gnu.org

;;; Commentary:
;;
;; Constructors and accessors for (cairo) vector types.
;;
;;; Code:

(define-module (cairo vector-types))

(define-macro (define-accessors type . fields)
  `(begin
     ,@(map (lambda (field)
              ;; can't have : as a symbol
              `(define-public (,(string->symbol
                                 (string-append
                                  (symbol->string type) ":" (symbol->string field)))
                               obj)
                 (vector-ref obj ,(list-index fields field))))
            fields)))

(define-macro (define-vector-type module type constructor . fields)
  `(begin
     (define-public (,(symbol-append module '-make- type) ,@fields)
       (,constructor ,@fields))
     (define-accessors ,(symbol-append module '- type) ,@fields)))

(define-vector-type cairo rectangle f64vector
  x y width height)

(define-vector-type cairo glyph vector
  index x y)

(define-vector-type cairo text-extents f64vector
  x-bearing y-bearing width height x-advance y-advance)

(define-vector-type cairo font-extents f64vector
  ascent descent height max-x-advance max-y-advance)

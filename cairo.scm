;; guile-cairo
;; Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU Lesser General Public License as
;; published by the Free Software Foundation; either version 3 of the
;; License, or (at your option) any later version.
;;                                                                  
;; This program is distributed in the hope that it will be useful, but
;; WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
;; Lesser General Public License for more details.
;;                                                                  
;; You should have received a copy of the GNU Lesser General Public
;; License along with this program; if not, see
;; <http://www.gnu.org/licenses/>.

;;; Commentary:
;;
;; This is the Cairo wrapper for Guile.
;;
;; See the cairo documentation for more details.
;;
;;; Code:

(define-module (cairo)
  :use-module (cairo config)
  :use-module ((ice-9 documentation) #:select (documentation-files))
  :use-module (cairo vector-types))

(module-use! (module-public-interface (current-module))
             (resolve-interface '(cairo vector-types)))

;; This will export many things
(dynamic-call "scm_init_cairo"
              (dynamic-link *cairo-lib-path*))

(if (not (member *cairo-documentation-path* documentation-files))
    (set! documentation-files (cons *cairo-documentation-path*
                                    documentation-files)))

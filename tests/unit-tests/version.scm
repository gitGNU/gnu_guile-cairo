;; guile-cairo unit test
;; Copyright (C) 2007, 2011 Andy Wingo <wingo pobox.com>

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3 of the License, or (at
;; your option) any later version.
;;                                                                  
;; This program is distributed in the hope that it will be useful, but
;; WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
;; General Public License for more details.
;;                                                                  
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, see <http://www.gnu.org/licenses/>.

(use-modules (unit-test)
             (oop goops)
             (cairo))

(define-class <test-version> (<test-case>))

(define-method (test-version (self <test-version>))
  (cairo-version))

(exit-with-summary (run-all-defined-test-cases))

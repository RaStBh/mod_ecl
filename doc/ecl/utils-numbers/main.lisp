;; RaSt mod_ecl is  an extension module for the Apache  HTTP Server.  It enables
;; the Apache HTTP  Server to run scripts written in  the Embeddable Common-Lisp
;; language.
;;
;; Copyright (C) 2021 Ralf Stephan (RaSt) <me@ralf-stephan.name>
;;
;; RaSt mod_ecl is free software: you can redistribute it and/or modify it under
;; the terms of the GNU General Public License as published by the Free Software
;; Foundation, either  version 3 of the  License, or (at your  option) any later
;; version.
;;
;; RaSt mod_ecl is distributed  in the hope that it will  be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
;; FOR  A PARTICULAR  PURPOSE.   See the  GNU General  Public  License for  more
;; details.
;;
;; You should have received a copy of  the GNU General Public License along with
;; this     program     in     the     file    COPYING.      If     not,     see
;; <https://www.gnu.org/licenses/>.



(in-package :utils-numbers)

(defparameter *command-line-arguments* nil)

(defun main (&rest arguments)
    (if arguments
        (if (= (length arguments) 3)
            (let ((operator (first arguments))
	          (operand1 (second arguments))
	          (operand2 (third arguments))
	          )
	      (cond
                  ((string= operator "+") (numbers-add operand1 operand2))
                  ((string= operator "-") (numbers-subtract operand1 operand2))
                  ((string= operator "*") (numbers-multiply operand1 operand2))
                  ((string= operator "/") (numbers-divide operand1 operand2))
                  ((string= operator "exp") (numbers-exponent operand1 operand2))
                  ((string= operator "rt") (numbers-root operand1 operand2))
                  ((string= operator "log") (numbers-logarithm operand1 operand2))
                  (t (string "Unknown operator."))
	      )
	    )
            (string "Wrong number of Arguments.")
        )
        (string "I am utils-numbers:main. :-)")
    )
)

(defun entry-point ()
    (apply 'main *command-line-arguments*)
)

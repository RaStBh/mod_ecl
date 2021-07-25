;; This file is part of RaSt mod_ecl.
;;
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



(eval-when-compile
;(eval-when (
;            ;:load-toplevel
;            :compile-toplevel
;            ;:execute
;            )

    ;-------------------------------------------------------------------------------
    ; We load a lisp file.
    ;
    ; When running index.ecl in the mod_ecl module we laod utils-stings here.

    ;(require "utils-strings-2" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-2.lisp")
    ;(require "utils-strings-2" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-2.fas")
    ;(require "utils-strings-2" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-2.fasc")
    (require "utils-strings-2" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-2")

    ;(require "utils-strings-1" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-1.lisp")
    ;(require "utils-strings-1" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-1.fas")
    ;(require "utils-strings-1" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-1.fasc")
    (require "utils-strings-1" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-1")

    ;(require "utils-strings-0" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-0.lisp")
    ;(require "utils-strings-0" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-0.fas")
    ;(require "utils-strings-0" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-0.fasc")
    (require "utils-strings-0" "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-0")

    ;(load "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-bundled.fas")
    ;(load "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-bundled.fasc") ; Bundled fasc does not work as expected.
    ;(load "/var/www/html/mod_ecl.lh.ld/utils-strings/utils-strings-bundled")

)



(eval-when-compile
;(eval-when (
;            ;:load-toplevel
;             :compile-toplevel
;             ;:execute
;           )

    ;-------------------------------------------------------------------------------
    ; We load the ECL ASDF.
    ;
    ; ECL ASDF: /usr/lib/x86_64-linux-gnu/ecl-21.2.1/asdf.fas
    ; Common Lisp ASDF: /usr/share/common-lisp/source/cl-asdf/asdf.lisp

    (require "asdf") ; this works fine
    ;(require :asdf) ; this works fine
    ;(require 'asdf) ; this works fine

    ;(asdf:load-system :asdf) ; this can not work, ASDF is not loaded yet

    ;(require "asdf" "/usr/share/common-lisp/ecl-21.2.1/contrib/asdf/asdf.lisp") ; this works fine
    ;(require "asdf" "/usr/share/common-lisp/ecl-21.2.1/contrib/asdf/asdf") ; this works fine

    ;(require "asdf" "/usr/lib/x86_64-linux-gnu/ecl-21.2.1/asdf.fas") ; this works fine
    ;(require "asdf" "/usr/lib/x86_64-linux-gnu/ecl-21.2.1/asdf") ; this works fine

)



(eval-when-compile
;(eval-when (
;            ;:load-toplevel
;            :compile-toplevel
;            ;:execute
;           )

    ;-------------------------------------------------------------------------------
    ; We load a ASDF system.

    ;(require "utils-numbers")
    ;(require :utils-numbers)
    ;(require 'utils-numbers)

    (asdf:load-system :utils-numbers) ; does not work! why?

    ;(require "utils-numbers" "/var/www/html/mod_ecl.lh.ld/utils-numbers/???.lisp") ; this can not work
    ;(require "utils-numbers" "/var/www/html/mod_ecl.lh.ld/utils-numbers/???") ; this can not work
    ;(require "utils-numbers" "/var/www/html/mod_ecl.lh.ld/utils-numbers") ; does not work

    ;(require "utils-numbers" "/var/www/html/mod_ecl.lh.ld/utils-numbers.fas/utils-numbers.fasb") ; this works fine
    ;(require "utils-numbers" "/var/www/html/mod_ecl.lh.ld/utils-numbers.fas/utils-numbers") ; this works fine

)



(eval-when (
            :load-toplevel
            :compile-toplevel
            :execute
           )

    ;-------------------------------------------------------------------------------
    ; Variable which contains the message we output at the end of the script.

    (defvar message
        ""
        "The message we output at the end of the script."
    )

    ;-------------------------------------------------------------------------------
    ; We show version of ASDF.
    ; We show the loaded systems of ASDF.

    (defparameter message
        (concatenate 'string
            message
            "ASDF version: "
            (when (asdf:asdf-version)
                  (format nil "~S" (asdf:asdf-version))
            )
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "ASDF loadad systems: "
            (when (asdf:already-loaded-systems)
                  (format nil "~S" (asdf:already-loaded-systems))
            )
            "<br>"
            (format nil "~%")
        )
    )

    (setq find-system-error nil)
    (asdf:find-system :utils-numbers find-system-error)
    (defparameter message
        (concatenate 'string
            message
            "utils-numbers system: "
            (if find-system-error
                (format nil "~S" "can not find system")
                (format nil "~S ~S ~S" (asdf/system:system-author (asdf:find-system :utils-numbers))
                                       (asdf/component:component-name (asdf:find-system :utils-numbers))
                                       (asdf/component:component-version (asdf:find-system :utils-numbers))
                )
             )
             "<br>"
             (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-numbers:main) = "
            (when (utils-numbers:main)
                (format nil "~S" (utils-numbers:main))
            )
            "<br>"
            (format nil "~%")
       )
    )

    ;-------------------------------------------------------------------------------
    ; We load a lisp file.
    ;

    ;
    ; When running index.ecl in the ecl interpreter we laod utils-stings here.
    ;

    ; see above

    ;-------------------------------------------------------------------------------
    ; We print a wellcome message.

    (defparameter message
        (concatenate 'string
            message
            "Hello World!"
            "<br>"
            (format nil "~%")
            "This is a message from your index.ecl script."
            "<br>"
            (format nil "~%")
        )
    )

    ;-------------------------------------------------------------------------------
    ; See if we are running the script using the ECL interpreter or the RaSt mod_ecl
    ; module.  Print a message stating in which environment we are in.

    (if (boundp '*mod_ecl*)
        (defparameter message
            (concatenate 'string
                message
                "The script runs inside the RaSt mod_ecl Apache HTTP Server module."
                "<br>"
                (format nil "~%")
            )
        )
        (defparameter message
            (concatenate 'string
                message
                "The script runs inside the Embedabble Common-Lisp (ECL) interpreter."
                "<br>"
                (format nil "~%")
            )
        )
    )

     ;-------------------------------------------------------------------------------
     ; A global string constant.

     (defconstant +global-constant+
         3.14159
     )

     (defparameter message
         (concatenate 'string
             message
             "+global-constant+ = "
             (format nil "~D" +global-constant+)
             "<br>"
             (format nil "~%")
         )
     )

     ;-------------------------------------------------------------------------------
     ; A global string variable.

     (defvar *global-variable-string*
        "one"
     )

     (defparameter message
        (concatenate 'string
            message
            "*global-variable-string* = "
            (format nil "~S" *global-variable-string*)
            "<br>"
            (format nil "~%")
         )
    )

    ;-------------------------------------------------------------------------------
    ; A global function to change a string.

    (defun change-string (string1 string2)
        (concatenate 'string string1 " zero " string2)
     )

    (defparameter message
        (concatenate 'string
            message
            "*global-variable-string* = "
            (format nil "~S" (change-string *global-variable-string* "two"))
            "<br>"
            (format nil "~%")
        )
    )

    ;-------------------------------------------------------------------------------
    ; A global number variable.

    (defvar *global-variable-number*
        1
    )

    (defparameter message
        (concatenate 'string
            message
            "*global-variable-number* = "
            (format nil "~D" *global-variable-number*)
            "<br>"
            (format nil "~%")
        )
    )

    ;-------------------------------------------------------------------------------
    ; A global function to change a number.

    (defun change-number (number1 number2)
        (+ number1 99 number2)
    )

    (defparameter message
        (concatenate 'string
            message
            "*global-variable-number* = "
            (format nil "~D" (change-number *global-variable-number* 2))
            "<br>"
            (format nil "~%")
        )
    )

    ;-------------------------------------------------------------------------------
    ; Get and set a global variable from within a function.
    ; Use macros from utils-strings.

    (defparameter message
        (concatenate 'string
            message
            "(utils-strings-0) = "
            (format nil "~S" (utils-strings-0))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-strings-1) = "
            (format nil "~S" (utils-strings-1))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-strings-2) = "
            (format nil "~S" (utils-strings-2))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter *bar*
        (if (string= "utils-strings-2" (utils-strings-2))
            (uppercase "bar")
            (string "utils-strings-2 not loaded")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "*bar* = "
            (format nil "~S" *bar*)
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter *foo*
        (if (string= "utils-strings-1" (utils-strings-1))
            (lowercase "FOO")
            (string "utils-strings-1 not loaded")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "*foo* = "
            (format nil "~S" *foo*)
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter *foobar*
      (concatenate 'string *bar* "-" *foo*)
    )

    (defparameter message
        (concatenate 'string
            message
            "*foobar* = "
            (format nil "~S" *foobar*)
            "<br>"
            (format nil "~%")
        )
    )

    (defun foobar (foo bar)
        (defparameter *foobar*
            (concatenate 'string foo "-" bar)
        )
    )

    (foobar *foo* *bar*)

    (defparameter message
        (concatenate 'string
            message
            "*foobar* = "
            (format nil "~S" *foobar*)
            "<br>"
            (format nil "~%")
        )
    )

    ;-------------------------------------------------------------------------------
    ; Here we try out a loop.

    (defparameter *remainder-text*
        "The ~D apples a day keeps the doctor away."
        "A friendly remainder. Stay healthy."
    )

    (defparameter *remainder-count*
        3
        "All good things come in threes."
    )

    (defun stay-healthy (text count)
        "Take care of yourself."
        (setq message "")
        (let ((index 0))
            (loop
                (if (> index 0)
                    (setq message
                        (concatenate 'string
                            message
                            (format nil "<br>~%")
                        )
                    )
                    ()
                )
                (setq message
                    (concatenate 'string
                        message
                        (format nil text (+ index 1))
                    )
                )
                (setq index (+ index 1))
                (unless (< index count) (return))
            )
            (block nil
                (return message)
            )
        )
    )

    (defparameter message
        (concatenate 'string
            message
            (format nil "~D" "Stay healthy!")
            "<br>"
            (format nil "~%")
            (format nil "~D" (stay-healthy *remainder-text* *remainder-count*))
            "<br>"
            (format nil "~%")
        )
    )

    ;-------------------------------------------------------------------------------
    ; We use the utils-numbers system

    (defparameter message
        (concatenate 'string
            message
            "(utils-numbers:main \"+\" 10 2) = "
            (format nil "~D" (utils-numbers:main "+" 10 2))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-numbers:main \"-\" 10 2) = "
            (format nil "~D" (utils-numbers:main "-" 10 2))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-numbers:main \"*\" 10 2) = "
            (format nil "~D" (utils-numbers:main "*" 10 2))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-numbers:main \"/\" 10 2) = "
            (format nil "~D" (utils-numbers:main "/" 10 2))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-numbers:main \"exp\" 10 2) = "
            (format nil "~D" (utils-numbers:main "exp" 10 2))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-numbers:main \"rt\" 2 100) = "
            (format nil "~D" (utils-numbers:main "rt" 2 100))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-numbers:main \"log\" 10 100) = "
            (format nil "~D" (utils-numbers:main "log" 10 100))
            "<br>"
            (format nil "~%")
        )
    )

    (defparameter message
        (concatenate 'string
            message
            "(utils-numbers \"^\" 2 10) = "
            (format nil "~D" (utils-numbers:main "&" 2 10))
            "<br>"
            (format nil "~%")
        )
    )

    ;-------------------------------------------------------------------------------
    ; MIME header environment from the request.

    (defparameter message
        (concatenate 'string
            message
            "MIME header environment from the request."
            "<br>"
            (format nil "~%")
        )
    )

    (WITH-HASH-TABLE-ITERATOR (next-item *header-in*)
        (let ((index 0))    
            (loop
                (multiple-value-bind (more key value) (next-item)
                    (unless more (return nil))
                    (defparameter message
                        (concatenate 'string
                            message
                            (format nil "~D: ~D = \"~D\"" index key value)
                            "<br>"
                            (format nil "~%")
                        )
                    )
                )
                (setq index (+ index 1))
            )
        )
    )

    ;-------------------------------------------------------------------------------
    ; We output all the messages at the end of the script.

    (princ message)

)

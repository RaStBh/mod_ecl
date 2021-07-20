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



(defun make-o ()
    (compile-file "./utils-strings-0.lisp"
        :output-file "./utils-strings-0.o"
        :verbose t
        :print t
        :system-p t
    )
    (compile-file "./utils-strings-1.lisp"
        :output-file "./utils-strings-1.o"
        :verbose t
        :print t
        :system-p t
    )
    (compile-file "./utils-strings-2.lisp"
        :output-file "./utils-strings-2.o"
        :verbose t
        :print t
        :system-p t
    )
)

(defun make-fas ()
    (compile-file "./utils-strings-0.lisp"
        :output-file "./utils-strings-0.fas"
        :verbose t
        :print t
        :system-p nil
    )
    (compile-file "./utils-strings-1.lisp"
        :output-file "./utils-strings-1.fas"
        :verbose t
        :print t
        :system-p nil
    )
    (compile-file "./utils-strings-2.lisp"
        :output-file "./utils-strings-2.fas"
        :verbose t
        :print t
        :system-p nil
    )
)

(defun make-fasc ()
    (ext:install-bytecodes-compiler)
    (compile-file "./utils-strings-0.lisp"
        :output-file "./utils-strings-0.fasc"
        :verbose t
        :print t
        :system-p nil
    )
    (compile-file "./utils-strings-1.lisp"
        :output-file "./utils-strings-1.fasc"
        :verbose t
        :print t
        :system-p nil
    )
    (compile-file "./utils-strings-2.lisp"
        :output-file "./utils-strings-2.fasc"
        :verbose t
        :print t
        :system-p nil
    )
    (ext:install-c-compiler)
)

(defun make-bundled-fas ()
    (c:build-fasl "./utils-strings-bundled.fas"
        :lisp-files '("./utils-strings-0.o"
                      "./utils-strings-1.o"
                      "./utils-strings-2.o"
		     )
    )
)

(defun make-bundled-fasc ()
    (with-open-file
        (output "./utils-strings-bundled.fasc"
            :direction :output
            :if-exists :supersede
            :if-does-not-exist :create
            ;:external-format '(:latin-1
            ;                   :utf-8
            ;                   :cr
            ;                   :crlf
            ;                   :lf
            ;                   :little-endian
            ;                   :big-endian
            ;                  )
        )
        (ext:run-program
            "/usr/bin/cat"
            '("./utils-strings-0.fasc"
	      "./utils-strings-1.fasc"
	      "./utils-strings-2.fasc")
            :output output
        )
    )
)



(make-fas)
(make-fasc)
(make-o)
(make-bundled-fas)
(make-bundled-fasc)
(quit)

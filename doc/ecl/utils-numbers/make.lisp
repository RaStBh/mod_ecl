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



(asdf:make-build :utils-numbers
    :type :fasl
    :move-here #P"./utils-numbers.fas/"
)

(asdf:make-build :utils-numbers
    :type :static-library
    :move-here #P"./utils-numbers.a/"
    :monolithic t
    :init-name "init_utils_numbers"
)

(asdf:make-build :utils-numbers
    :type :shared-library
    :move-here #P"./utils-numbers.so/"
    :monolithic t
    :init-name "init_utils_numbers"
)

(asdf:make-build :utils-numbers
    :type :program
    :move-here #P"./utils-numbers.bin/"
    :epilogue-code
    '(progn
         (format t "10 + 2 = ~D~%" (utils-numbers:main "+" 10 2))
         (format t "10 - 2 = ~D~%" (utils-numbers:main "-" 10 2))
         (format t "10 * 2 = ~D~%" (utils-numbers:main "*" 10 2))
         (format t "10 / 2 = ~D~%" (utils-numbers:main "/" 10 2))
         (format t "exp(10,2) = ~D~%" (utils-numbers:main "exp" 10 2))
         (format t "rt(2,100) = ~D~%" (utils-numbers:main "rt" 2 100))
         (format t "log(10,100) = ~D~%" (utils-numbers:main "log" 10 100))
         (format t "10 & 2 = ~D~%" (utils-numbers:main "&" 10 2))
     )
)

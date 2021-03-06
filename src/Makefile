## This file is part of RaSt mod_ecl.
##
## RaSt mod_ecl is  an extension module for the Apache  HTTP Server.  It enables
## the Apache HTTP  Server to run scripts written in  the Embeddable Common-Lisp
## language.
##
## Copyright (C) 2021 Ralf Stephan (RaSt) <me@ralf-stephan.name>
##
## RaSt mod_ecl is free software: you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free Software
## Foundation, either  version 3 of the  License, or (at your  option) any later
## version.
##
## RaSt mod_ecl is distributed  in the hope that it will  be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
## FOR  A PARTICULAR  PURPOSE.   See the  GNU General  Public  License for  more
## details.
##
## You should have received a copy of  the GNU General Public License along with
## this     program     in     the     file    COPYING.      If     not,     see
## <https://www.gnu.org/licenses/>.



##
##  Makefile -- Build procedure for sample ecl Apache module
##  Autogenerated via ``apxs -n ecl -g''.
##

builddir=./
docdir=../doc/
top_srcdir=/usr/share/apache2
top_builddir=/usr/share/apache2
include /usr/share/apache2/build/special.mk

#   the used tools
APACHECTL=apachectl
DOXYGEN=doxygen
CAT=cat

#   additional defines, includes and libraries
#DEFS=-Dmy_define=my_value
#INCLUDES=-Imy/include/dir
#LIBS=-Lmy/lib/dir -lmylib
CFLAGS=-lecl

#   the default target
all: local-shared-build

#   install the shared object file into Apache 
install: install-modules-yes

#   cleanup
clean:
	-rm -f mod_ecl.o mod_ecl.lo mod_ecl.slo mod_ecl.la 

#   simple test
test: reload
	lynx -mime_header http://localhost/mod_ecl/

#   install and activate shared object by reloading Apache to
#   force a reload of the shared object file
reload: install restart

#   the general Apache start/restart/stop
#   procedures
start:
	$(APACHECTL) start
restart:
	$(APACHECTL) restart
stop:
	$(APACHECTL) stop

doc:
	( $(CAT) $(docdir)/Doxyfile ; echo "OUTPUT_DIRECTORY = $(docdir)" ) | $(DOXYGEN) -

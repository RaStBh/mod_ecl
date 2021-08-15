## RaSt mod_ecl is  an extension module for the Apache  HTTP Server.  It enables
## the Apache HTTP  Server to run scripts written in  the Embeddable Common-Lisp
## language.
##
## Copyright (c) 2021 Ralf Stephan (RaSt) <me@ralf-stephan.name>
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
## RaSt     mod_ecl     in     the     file    COPYING.      If     not,     see
## <https://www.gnu.org/licenses/>.



mod_ecl.la: \
	mod_ecl.slo \
	boolean.slo \
	status_codes.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version \
	mod_ecl.lo \
	boolean.lo \
	status_codes.lo

DISTCLEAN_TARGETS = modules.mk

static =

shared = mod_ecl.la

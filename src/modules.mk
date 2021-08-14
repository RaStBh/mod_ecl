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

mod_ecl.la: mod_ecl.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_ecl.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_ecl.la

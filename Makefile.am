## @start 1
## Makefile.am -- Process this file with automake to produce Makefile.in
## @end 1

MAINTAINERCLEANFILES	= aclocal.m4 config.h.in config.h.in~ configure depcomp INSTALL install-sh ltmain.sh missing Makefile.in $(PACKAGE)-$(VERSION).tar.gz src/Makefile.in

## @start 1
SUBDIRS = src
bin_PROGRAMS = 

installdirs:
	if test ! -d "$(HOME)/.terminfo/v"; then \
		mkdir --mode=u+rwx -p "$(HOME)/.terminfo/v"; \
	fi; \
	if test ! -d "$(DESTDIR)/var/games/$(PACKAGE)"; then \
		mkdir --mode=a+rwx,o-w -p "$(DESTDIR)/var/games/$(PACKAGE)"; \
		chgrp games "$(DESTDIR)/var/games/$(PACKAGE)"; \
	fi; \
	if test ! -d "$(DESTDIR)$(bindir)"; then \
		mkdir --mode=a+rwx,o-w -p "$(DESTDIR)$(bindir)"; \
		chgrp staff "$(DESTDIR)$(bindir)"; \
	fi;

EXTRA_DIST =		  	\
  bootstrap			    \
  configure.ac      \
  Makefile.am				\
  src/Makefile.am		\
  src/datesub.l     \
  src/vt100         \
  doc

## @end 1

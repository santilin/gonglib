# <<<<<GONGLIB_M4
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# >>>>>GONGLIB_M4
SUBDIRS=gonglib rtk qt4 dbapp contactos empresa gastos \
	src

EXTRA_DIST=debian Makefile.cvs
dist-hook:
	sed -e s%-I../../%-I../% -i $(distdir)/configure.ac
# 	cp -r ../share $(distdir)
# 	(cd $(distdir) && make -f Makefile.cvs)


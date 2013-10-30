# <<<<<GONGLIB_M4
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# >>>>>GONGLIB_M4
SUBDIRS=gonglib rtk qt4 dbapp contactos empresa contab pagos factu socias sinpapeles reccolor \
	src

EXTRA_DIST=share debian Makefile.cvs
dist-hook:
	rm -rf `find $(distdir)/share -type d -name .svn`
	rm -rf `find $(distdir)/debian -type d -name .svn`
	sed -e s%-I../../%-I../% -i $(distdir)/configure.ac
	(cd $(distdir) && make -f Makefile.cvs)


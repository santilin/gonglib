# <<<<<GONGLIB_M4
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# >>>>>GONGLIB_M4
SUBDIRS=gonglib rtk qt4 dbapp contactos empresa \
	src
# contab pagos factu gastostipo socias sinpapeles reccolor

EXTRA_DIST=debian Makefile.cvs
dist-hook:
	sed -e s%-I../../%-I../% -i $(distdir)/configure.ac

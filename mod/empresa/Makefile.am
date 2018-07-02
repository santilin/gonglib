AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongempresa.la
libgongempresa_ladir = $(includedir)/gonglib

libgongempresa_la_SOURCES = empresamodule.cpp \
	empresafrmeditproyecto.cpp empresarecproyecto.cpp \
	empresafldcuentabanco.cpp empresafldejercicio.cpp empresafldempresaid.cpp \
	empresafrmeditempresa.cpp empresafrmedittipoiva.cpp \
	empresafrmeditmoneda.cpp empresafrmeditdepartamento.cpp \
	empresarecempresa.cpp empresarectipoiva.cpp \
	empresarecmoneda.cpp empresarecdepartamento.cpp

libgongempresa_la_HEADERS = empresamodule.h empresamastertable.h \
	empresafrmeditproyecto.h empresarecproyecto.h \
	empresafldcuentabanco.h empresafldejercicio.h empresafldempresaid.h \
	empresafrmeditempresa.h empresafrmedittipoiva.h \
	empresafrmeditmoneda.h empresafrmeditdepartamento.h \
	empresarecempresa.h empresarectipoiva.h \
	empresarecmoneda.h empresarecdepartamento.h

nodist_libgongempresa_la_SOURCES = \
	moc_empresafrmeditproyecto.cpp \
	moc_empresafrmeditdepartamento.cpp \
	moc_empresafrmeditempresa.cpp \
	moc_empresafrmeditmoneda.cpp \
	moc_empresafrmedittipoiva.cpp \
	moc_empresamodule.cpp

moc_%.cpp: %.h
	moc-qt4 $< -o $@

empresa_sharedir = $(datadir)/gestiong/empresa
dist_empresa_share_DATA = $(srcdir)/share/empresa.ddd

empresa_datosdir = $(datadir)/gestiong/empresa/datos
dist_empresa_datos_DATA = $(srcdir)/share/datos/FormaPago.csv \
	$(srcdir)/share/datos/TipoIva.csv



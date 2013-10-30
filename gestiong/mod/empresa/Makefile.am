INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongempresa.la
libgongempresa_ladir = $(includedir)/gonglib

libgongempresa_la_SOURCES = empresamodule.cpp \
	empresafrmeditproyecto.cpp empresarecproyecto.cpp \
	empresafldcuentabanco.cpp empresafldejercicio.cpp empresafldempresaid.cpp \
	empresafldperiodicidad.cpp \
	empresafrmeditempresa.cpp empresafrmedittipoiva.cpp \
	empresafrmeditmoneda.cpp empresafrmeditdepartamento.cpp \
	empresarecempresa.cpp empresarectipoiva.cpp \
	empresarecmoneda.cpp empresarecdepartamento.cpp

libgongempresa_la_HEADERS = empresamodule.h \
	empresafrmeditproyecto.h empresarecproyecto.h \
	empresafldcuentabanco.h empresafldejercicio.h empresafldempresaid.h \
	empresafldperiodicidad.h \
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
	moc $< -o $@

empresa_sharedir = $(datadir)/gestiong/empresa
empresa_share_DATA = $(top_srcdir)/empresa/share/empresa.ddd

empresa_datosdir = $(datadir)/gestiong/empresa/datos
empresa_datos_DATA = $(top_srcdir)/empresa/share/datos/FormaPago.csv \
	$(top_srcdir)/empresa/share/datos/TipoIva.csv



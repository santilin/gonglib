INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongproduccion.la

libgongproduccion_ladir = $(includedir)/gonglib

libgongproduccion_la_SOURCES = produccionmodule.cpp \
	produccionfrmeditarticulocomponente.cpp produccionrecarticulocomponente.cpp \
	produccionfrmeditarticulobehavior.cpp produccionrecarticulo.cpp \
	produccionfrmeditlote.cpp produccionreclote.cpp \
	produccionfrmeditlotedet.cpp produccionreclotedet.cpp

libgongproduccion_la_HEADERS = produccionmodule.h \
	produccionfrmeditarticulocomponente.h produccionrecarticulocomponente.h \
	produccionfrmeditarticulobehavior.h produccionrecarticulo.h \
	produccionfrmeditlote.h produccionreclote.h \
	produccionfrmeditlotedet.h produccionreclotedet.h

nodist_libgongproduccion_la_SOURCES = moc_produccionmodule.cpp \
	moc_produccionfrmeditarticulocomponente.cpp \
	moc_produccionfrmeditlote.cpp moc_produccionfrmeditlotedet.cpp \
	moc_produccionfrmeditarticulobehavior.cpp

moc_%.cpp: %.h
	moc $< -o $@

produccion_sharedir = $(datadir)/gestiong/produccion
produccion_share_DATA = $(srcdir)/share/produccion.ddd


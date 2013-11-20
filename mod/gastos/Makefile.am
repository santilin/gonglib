INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgonggastos.la
libgonggastos_ladir = $(includedir)/gonglib
libgonggastos_la_SOURCES = gastosmodule.cpp \
 	gastosrecgasto.cpp \
 	gastosfrmeditgasto.cpp

libgonggastos_la_HEADERS = gastosmodule.h gastosmastertable.h \
 	gastosrecgasto.h gastosfrmeditgasto.h

nodist_libgonggastos_la_SOURCES = moc_gastosmodule.cpp \
 	moc_gastosfrmeditgasto.cpp

moc_%.cpp: %.h
	moc $< -o \$@

gastos_sharedir = $(datadir)/gestiong/gastos
dist_gastos_share_DATA = $(srcdir)/share/gastos.ddd



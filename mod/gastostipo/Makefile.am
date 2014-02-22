INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgonggastostipo.la
libgonggastostipo_ladir = $(includedir)/gonglib
libgonggastostipo_la_SOURCES = gastostipomodule.cpp \
 	gastostiporecgastotipo.cpp  gastostipofrmeditgastotipo.cpp \
	gastostipofldpedircampo.cpp

libgonggastostipo_la_HEADERS = gastostipomodule.h gastostipomastertable.h \
 	gastostiporecgastotipo.h gastostipofrmeditgastotipo.h \
	gastostipofldpedircampo.h


nodist_libgonggastostipo_la_SOURCES = moc_gastostipomodule.cpp \
 	moc_gastostipofrmeditgastotipo.cpp

moc_%.cpp: %.h
	moc $< -o \$@

gastostipo_sharedir = $(datadir)/gestiong/gastostipo
dist_gastostipo_share_DATA = $(srcdir)/share/gastostipo.ddd



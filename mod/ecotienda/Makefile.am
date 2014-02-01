INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongecotienda.la
libgongecotienda_ladir = $(includedir)/gonglib
libgongecotienda_la_SOURCES = ecotiendamodule.cpp \
	ecotiendafrmeditarticulobehavior.cpp

libgongecotienda_la_HEADERS = ecotiendamodule.h ecotiendamastertable.h \
	ecotiendafrmeditarticulobehavior.h

nodist_libgongecotienda_la_SOURCES = moc_ecotiendamodule.cpp \
	moc_ecotiendafrmeditarticulobehavior.cpp

moc_%.cpp: %.h
	moc $< -o \$@


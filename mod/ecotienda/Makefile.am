INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongecotienda.la
libgongecotienda_ladir = $(includedir)/gonglib
libgongecotienda_la_SOURCES = ecotiendamodule.cpp
libgongecotienda_la_HEADERS = ecotiendamodule.h
nodist_libgongecotienda_la_SOURCES = moc_ecotiendamodule.cpp

moc_%.cpp: %.h
	moc $< -o \$@


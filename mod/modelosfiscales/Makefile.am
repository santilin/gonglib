INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongmodelosfiscales.la
libgongmodelosfiscales_ladir = $(includedir)/gonglib
libgongmodelosfiscales_la_SOURCES = modelosfiscalesmodule.cpp
libgongmodelosfiscales_la_HEADERS = modelosfiscalesmodule.h
nodist_libgongmodelosfiscales_la_SOURCES = moc_modelosfiscalesmodule.cpp

moc_%.cpp: %.h
	moc $< -o \$@


INCLUDES = @GONGLIB_CPPFLAGS@

AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongreccolor.la

libgongreccolor_ladir = $(includedir)/gonglib

libgongreccolor_la_SOURCES = reccolormodule.cpp \
	reccolorrecordbehavior.cpp reccolorfrmeditrecbehavior.cpp \
	reccolordatatable.cpp

libgongreccolor_la_HEADERS = reccolormodule.h reccolormastertable.h \
	reccolorrecordbehavior.h reccolorfrmeditrecbehavior.h \
	reccolordatatable.h

nodist_libgongreccolor_la_SOURCES = moc_reccolormodule.cpp \
	moc_reccolorfrmeditrecbehavior.cpp

moc_%.cpp: %.h
	moc $< -o $@
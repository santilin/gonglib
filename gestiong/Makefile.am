# <<<<<GONGLIB_M4
AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# >>>>>GONGLIB_M4
INCLUDES = @GONGLIB_CPPFLAGS@
localedir = $(datadir)/locale
AM_CPPFLAGS = @AM_CPPFLAGS@ -DLOCALEDIR=\"$(localedir)\" -DQT_TRANSLATIONS=\""@QT_TRANSLATIONS@"\"
AM_CXXFLAGS = @AM_CXXFLAGS@ @XML_CPPFLAGS@

lib_LTLIBRARIES = libgonglib-0.1.la
libgonglib_0_1_ladir = $(includedir)/gonglib


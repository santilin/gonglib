AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongeditorial.la
libgongeditorial_ladir = $(includedir)/gonglib

libgongeditorial_la_SOURCES = \
	editorialfldisbn.cpp editorialreclibro.cpp \
	editorialfrmediteditorial.cpp \
	editorialfrmeditfamiliabehavior.cpp editorialfrmeditlibro.cpp \
	editorialmodule.cpp editorialreceditorial.cpp \
	editorialisbnsearch.cpp

libgongeditorial_la_HEADERS = editorialfldisbn.h editorialfrmeditlibro.h editorialreceditorial.h \
	editorialmastertable.h editorialmodule.h \
	editorialfrmediteditorial.h editorialreclibro.h \
	editorialfrmeditfamiliabehavior.h \
	editorialisbnsearch.h

nodist_libgongeditorial_la_SOURCES = moc_editorialmodule.cpp \
	moc_editorialfrmediteditorial.cpp moc_editorialfrmeditlibro.cpp

moc_%.cpp: %.h
	moc $< -o $@

editorial_sharedir = $(datadir)/gestiong/editorial
dist_editorial_share_DATA = $(srcdir)/share/editorial.ddd

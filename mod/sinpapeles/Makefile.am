AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongsinpapeles.la

libgongsinpapeles_ladir = $(includedir)/gonglib

libgongsinpapeles_la_SOURCES = sinpapelesmodule.cpp \
	sinpapelesrecordbehavior.cpp sinpapelesfrmeditrecbehavior.cpp

libgongsinpapeles_la_HEADERS = sinpapelesmodule.h sinpapelesmastertable.h \
	sinpapelesrecordbehavior.h sinpapelesfrmeditrecbehavior.h

nodist_libgongsinpapeles_la_SOURCES = moc_sinpapelesmodule.cpp \
	moc_sinpapelesfrmeditrecbehavior.cpp

moc_%.cpp: %.h
	moc $< -o $@

AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongtesoreria.la
libgongtesoreria_ladir = $(includedir)/gonglib

libgongtesoreria_la_SOURCES = tesoreriamodule.cpp \
	tesoreriafrmeditcuentatesoreria.cpp tesoreriareccuentatesoreria.cpp \
	tesoreriafrmeditapuntetesoreria.cpp tesoreriarecapuntetesoreria.cpp \
	tesoreriafrmedittercerotesoreria.cpp tesoreriarectercerotesoreria.cpp \
	tesoreriafrmeditconceptotesoreria.cpp tesoreriarecconceptotesoreria.cpp \
	tesoreriafrmedittipoapuntetesoreria.cpp tesoreriarectipoapuntetesoreria.cpp \
	tesoreriafldpedircampo.cpp tesoreriaactnombrebehavior.cpp \
	tesoreriaiapuntablerecord.cpp tesoreriafrmeditempresabehavior.cpp

libgongtesoreria_la_HEADERS = tesoreriamodule.h \
	tesoreriamastertable.h \
	tesoreriafrmeditcuentatesoreria.h tesoreriareccuentatesoreria.h \
	tesoreriafrmeditapuntetesoreria.h tesoreriarecapuntetesoreria.h \
	tesoreriafrmedittercerotesoreria.h tesoreriarectercerotesoreria.h \
	tesoreriafrmeditconceptotesoreria.h tesoreriarecconceptotesoreria.h \
	tesoreriafrmedittipoapuntetesoreria.h tesoreriarectipoapuntetesoreria.h \
	tesoreriafldpedircampo.h tesoreriaactnombrebehavior.h \
	tesoreriaiapuntablerecord.h tesoreriafrmeditempresabehavior.h

nodist_libgongtesoreria_la_SOURCES = moc_tesoreriamodule.cpp \
	moc_tesoreriafrmeditcuentatesoreria.cpp \
	moc_tesoreriafrmeditapuntetesoreria.cpp \
	moc_tesoreriafrmeditconceptotesoreria.cpp \
	moc_tesoreriafrmedittercerotesoreria.cpp \
	moc_tesoreriafrmedittipoapuntetesoreria.cpp 

moc_%.cpp: %.h
	moc $< -o \$@

tesoreria_sharedir = $(datadir)/gestiong/tesoreria
dist_tesoreria_share_DATA = $(srcdir)/share/tesoreria.ddd

INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongpagos.la
libgongpagos_ladir = $(includedir)/gonglib
libgongpagos_la_SOURCES = pagosmodule.cpp \
	pagosreccobro.cpp pagosrecpago.cpp \
	pagosfrmeditcobro.cpp pagosfrmeditpago.cpp \
	pagosfrmeditremesacobro.cpp pagosrecremesacobro.cpp \
	pagosfrmeditformapago.cpp pagosrecformapago.cpp \
	pagosfrmpagarrecibo.cpp pagosipagablerecord.cpp \
	pagoscuaderno19.cpp

libgongpagos_la_HEADERS = pagosmodule.h \
	pagosfrmeditcobro.h pagosfrmpagarrecibo.h pagosfrmeditpago.h \
	pagosfrmeditformapago.h pagosrecformapago.h \
	pagosfrmeditremesacobro.h pagosrecremesacobro.h \
	pagosreccobro.h pagosrecpago.h pagosipagablerecord.h \
	pagoscuaderno19.h

nodist_libgongpagos_la_SOURCES = moc_pagosmodule.cpp \
	moc_pagosfrmeditcobro.cpp moc_pagosfrmeditpago.cpp \
	moc_pagosfrmeditremesacobro.cpp moc_pagosfrmeditformapago.cpp

moc_%.cpp: %.h
	moc $< -o \$@

pagos_sharedir = $(datadir)/gestiong/pagos
pagos_share_DATA = $(top_srcdir)/pagos/share/pagos.ddd

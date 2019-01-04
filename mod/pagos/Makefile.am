AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongpagos.la
libgongpagos_ladir = $(includedir)/gonglib
libgongpagos_la_SOURCES = pagosmodule.cpp \
	pagosreccobro.cpp pagosrecpago.cpp \
	pagosfrmeditcobro.cpp pagosfrmeditpago.cpp \
	pagosfrmeditremesacobro.cpp pagosrecremesacobro.cpp \
	pagosfrmeditformapago.cpp pagosrecformapago.cpp \
	pagosfrmpagarrecibo.cpp pagosipagablerecord.cpp \
	pagoscuaderno19.cpp pagosfrmeditempresabehavior.cpp

libgongpagos_la_HEADERS = pagosmodule.h pagosmastertable.h \
	pagosfrmeditcobro.h pagosfrmpagarrecibo.h pagosfrmeditpago.h \
	pagosfrmeditformapago.h pagosrecformapago.h \
	pagosfrmeditremesacobro.h pagosrecremesacobro.h \
	pagosreccobro.h pagosrecpago.h pagosipagablerecord.h \
	pagoscuaderno19.h pagosfrmeditempresabehavior.h

nodist_libgongpagos_la_SOURCES = moc_pagosmodule.cpp \
	moc_pagosfrmeditcobro.cpp moc_pagosfrmeditpago.cpp \
	moc_pagosfrmeditremesacobro.cpp moc_pagosfrmeditformapago.cpp

moc_%.cpp: %.h
	moc-qt4 $< -o \$@

pagos_sharedir = $(datadir)/gonglib/pagos
dist_pagos_share_DATA = $(srcdir)/share/pagos.ddd
pagos_informesdir = $(datadir)/gonglib/pagos/informes
dist_pagos_informes_DATA = $(srcdir)/share/informes/COBRO.rtk

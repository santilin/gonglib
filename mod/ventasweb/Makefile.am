AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongventasweb.la

libgongventasweb_ladir = $(includedir)/gonglib

libgongventasweb_la_SOURCES = ventaswebmodule.cpp \
 	ventaswebrecsemanapedido.cpp ventaswebfrmeditsemanapedido.cpp \
	ventaswebfrmeditpedidoweb.cpp ventaswebrecpedidoweb.cpp \
	ventaswebfrmeditpedidowebdet.cpp ventaswebrecpedidowebdet.cpp \
	ventaswebfrmeditarticulobehavior.cpp ventaswebfrmeditfamiliabehavior.cpp \
	ventaswebfrmcambiosmasivosarticulos.cpp ventaswebfrmsubirbajar.cpp

libgongventasweb_la_HEADERS = ventaswebmodule.h ventaswebmastertable.h \
 	ventaswebrecsemanapedido.h ventaswebfrmeditsemanapedido.h \
	ventaswebfrmeditpedidoweb.h ventaswebrecpedidoweb.h \
	ventaswebfrmeditpedidowebdet.h ventaswebrecpedidowebdet.h \
	ventaswebfrmeditarticulobehavior.h ventaswebfrmeditfamiliabehavior.h \
	ventaswebfrmcambiosmasivosarticulos.h ventaswebfrmsubirbajar.h

nodist_libgongventasweb_la_SOURCES = moc_ventaswebmodule.cpp \
 	moc_ventaswebfrmeditsemanapedido.cpp moc_ventaswebfrmsubirbajar.cpp \
	moc_ventaswebfrmeditpedidoweb.cpp moc_ventaswebfrmeditpedidowebdet.cpp \
	moc_ventaswebfrmeditarticulobehavior.cpp

moc_%.cpp: %.h
	moc-qt4 $< -o $@

ventasweb_sharedir = $(datadir)/gonglib/ventasweb
dist_ventasweb_share_DATA = $(srcdir)/share/ventasweb.ddd

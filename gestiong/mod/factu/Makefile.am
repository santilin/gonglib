INCLUDES = @GONGLIB_CPPFLAGS@

AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongfactu.la
libgongfactu_ladir = $(includedir)/gonglib

libgongfactu_la_SOURCES = factumodule.cpp \
 	factufrmeditempresabehavior.cpp \
 	facturectipodoc.cpp factufrmedittipodoc.cpp \
	factufldivadetallado.cpp factufrmeditagente.cpp factufrmeditarticulo.cpp \
	factufrmeditcliente.cpp factufrmeditfamilia.cpp factufrmeditproveedora.cpp \
	factufrmeditpresupuestoventa.cpp factufrmeditpresupuestoventadet.cpp \
	factufrmeditpedidoventa.cpp factufrmeditpedidoventadet.cpp \
	factufrmeditalbaranventa.cpp factufrmeditalbaranventadet.cpp \
	factufrmeditfacturaventa.cpp factufrmeditfacturaventadet.cpp \
	facturecagente.cpp facturecarticulo.cpp factureccliente.cpp \
	factufrmeditpedidocompra.cpp factufrmeditpedidocompradet.cpp \
	factufrmeditalbarancompra.cpp factufrmeditalbarancompradet.cpp \
	factufrmeditfacturacompra.cpp factufrmeditfacturacompradet.cpp \
	facturecfamilia.cpp facturecproveedora.cpp \
	facturecpresupuestoventa.cpp facturecpresupuestoventadet.cpp \
	facturecpedidoventa.cpp facturecpedidoventadet.cpp \
	facturecalbaranventa.cpp facturecalbaranventadet.cpp \
	facturecfacturaventa.cpp facturecfacturaventadet.cpp \
	facturecpedidocompra.cpp facturecpedidocompradet.cpp \
	facturecalbarancompra.cpp facturecalbarancompradet.cpp \
	facturecfacturacompra.cpp facturecfacturacompradet.cpp \
	factufrmestadcompraventa.cpp factufrmgenpedidocompra.cpp \
	factufrmcopiardocumento.cpp factufrmpagar.cpp \
	factufrmbalanceclipro.cpp factufrmcambiosmasivosarticulos.cpp \
	factufrmmod303.cpp factufrmgencodart.cpp \
	factuipagables.cpp factuiivadesglosable.cpp factuiasentables.cpp \
	factuitotalizablerecord.cpp

libgongfactu_la_HEADERS = factumodule.h factumastertable.h \
 	factufrmeditempresabehavior.h \
 	facturectipodoc.h factufrmedittipodoc.h \
	factufldivadetallado.h factufrmeditagente.h factufrmeditarticulo.h \
	factufrmeditcliente.h factufrmeditfamilia.h factufrmeditproveedora.h \
	factufrmeditpresupuestoventa.h factufrmeditpresupuestoventadet.h \
	factufrmeditpedidoventa.h factufrmeditpedidoventadet.h \
	factufrmeditalbaranventa.h factufrmeditalbaranventadet.h \
	factufrmeditfacturaventa.h factufrmeditfacturaventadet.h \
	factufrmeditpedidocompra.h factufrmeditpedidocompradet.h \
	factufrmeditalbarancompra.h factufrmeditalbarancompradet.h \
	factufrmeditfacturacompra.h factufrmeditfacturacompradet.h \
	facturecagente.h facturecarticulo.h factureccliente.h \
	facturecarticuloimagen.h \
	facturecfamilia.h facturecproveedora.h \
	facturecpresupuestoventa.h facturecpresupuestoventadet.h \
	facturecpedidoventa.h facturecpedidoventadet.h \
	facturecalbaranventa.h facturecalbaranventadet.h \
	facturecfacturaventa.h facturecfacturaventadet.h \
	facturecpedidocompra.h facturecpedidocompradet.h \
	facturecalbarancompra.h facturecalbarancompradet.h \
	facturecfacturacompra.h facturecfacturacompradet.h \
	factufrmestadcompraventa.h factufrmgenpedidocompra.h \
	factufrmcopiardocumento.h factufrmpagar.h \
	factufrmbalanceclipro.h factufrmcambiosmasivosarticulos.h \
	factufrmmod303.h factufrmgencodart.h \
	factuipagables.h factuiivadesglosable.h factuiasentables.h \
	factuitotalizablerecord.cpp

nodist_libgongfactu_la_SOURCES = \
	moc_factufrmeditagente.cpp \
	moc_factufrmeditalbarancompradet.cpp \
	moc_factufrmeditalbarancompra.cpp \
	moc_factufrmeditalbaranventadet.cpp \
	moc_factufrmeditalbaranventa.cpp \
	moc_factufrmeditarticulo.cpp \
	moc_factufrmeditcliente.cpp \
	moc_factufrmeditfacturacompradet.cpp \
	moc_factufrmeditfacturacompra.cpp \
	moc_factufrmeditfacturaventadet.cpp \
	moc_factufrmeditfacturaventa.cpp \
	moc_factufrmeditfamilia.cpp \
	moc_factufrmeditpedidocompradet.cpp \
	moc_factufrmeditpedidocompra.cpp \
	moc_factufrmeditpedidoventadet.cpp \
	moc_factufrmeditpedidoventa.cpp \
	moc_factufrmeditpresupuestoventadet.cpp \
	moc_factufrmeditpresupuestoventa.cpp \
	moc_factufrmeditproveedora.cpp \
	moc_factufrmedittipodoc.cpp \
	moc_factufrmestadcompraventa.cpp \
	moc_factumodule.cpp

libgongfactu_la_SOURCES += factufrmregenasientos.cpp
libgongfactu_la_HEADERS += factufrmregenasientos.h

moc_%.cpp: %.h
	moc $< -o $@

factu_sharedir = $(srcdir)/gestiong/factu
factu_share_DATA = $(top_srcdir)/factu/share/factu.ddd \
	$(top_srcdir)/factu/share/stylesheet.css
# act_stocks.sql

factu_informesdir = $(srcdir)/gestiong/factu/informes
factu_informes_DATA = $(top_srcdir)/factu/share/informes/ALBARANCOMPRA.rtk \
	$(top_srcdir)/factu/share/informes/ALBARANVENTA.rtk \
	$(top_srcdir)/factu/share/informes/ALBARANVENTA_IVAINCLUIDO.rtk \
	$(top_srcdir)/factu/share/informes/ALBARANVENTA_SINIVA.rtk \
	$(top_srcdir)/factu/share/informes/ARTICULO_ETIQUETAS.rtk \
	$(top_srcdir)/factu/share/informes/ARTICULO_LISTA_DE_PRECIOS.rtk \
	$(top_srcdir)/factu/share/informes/balanceclipro.inc \
	$(top_srcdir)/factu/share/informes/balanceclipro_cobros.inc \
	$(top_srcdir)/factu/share/informes/balanceclipro_comun.inc \
	$(top_srcdir)/factu/share/informes/CLIENTE.rtk \
	$(top_srcdir)/factu/share/informes/compras_header.inc \
	$(top_srcdir)/factu/share/informes/estadcompraventa.inc \
	$(top_srcdir)/factu/share/informes/estadcompraventa_comun.inc \
	$(top_srcdir)/factu/share/informes/estadcompraventa_resumido.inc \
	$(top_srcdir)/factu/share/informes/estadcompraventa_resumido_comun.inc \
	$(top_srcdir)/factu/share/informes/FACTURACOMPRA.rtk \
	$(top_srcdir)/factu/share/informes/FACTURAVENTA.rtk \
	$(top_srcdir)/factu/share/informes/FACTURAVENTA_IVAINCLUIDO.rtk \
	$(top_srcdir)/factu/share/informes/FACTURAVENTA_SINIVA.rtk \
	$(top_srcdir)/factu/share/informes/PEDIDOCOMPRA.rtk \
	$(top_srcdir)/factu/share/informes/PEDIDOVENTA.rtk \
	$(top_srcdir)/factu/share/informes/PEDIDOVENTA_IVAINCLUIDO.rtk \
	$(top_srcdir)/factu/share/informes/PRESUPUESTOVENTA.rtk \
	$(top_srcdir)/factu/share/informes/ventas_header.inc

# 	$(top_srcdir)/factu/share/informes/FACTURAVENTA_POR_FECHA.rtk
#	$(top_srcdir)/factu/share/informes/ALBARANVENTA_DEPOSITO_IVAINCLUIDO.rtk

factu_plantillasdir = $(srcdir)/gestiong/factu/plantillas/import
factu_plantillas_DATA = $(top_srcdir)/factu/share/plantillas/import/ARTICULO.ods \
	$(top_srcdir)/factu/share/plantillas/import/CLIENTE.ods \
	$(top_srcdir)/factu/share/plantillas/import/CONTACTO.ods \
	$(top_srcdir)/factu/share/plantillas/import/FAMILIA.ods \
	$(top_srcdir)/factu/share/plantillas/import/PROVEEDORA.ods


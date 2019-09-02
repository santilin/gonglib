AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongofertas.la
libgongofertas_ladir = $(includedir)/gonglib
libgongofertas_la_SOURCES = ofertasmodule.cpp \
 	ofertasreccobrooferta.cpp ofertasfrmeditcobrooferta.cpp \
 	ofertasrecpagooferta.cpp ofertasfrmeditpagooferta.cpp \
 	ofertasrecofertaventa.cpp ofertasfrmeditofertaventa.cpp \
 	ofertasrecofertacompra.cpp ofertasfrmeditofertacompra.cpp \
 	ofertasfrmeditofertacompradet.cpp ofertasrecofertacompradet.cpp \
	ofertasfrmeditofertaventadet.cpp ofertasrecofertaventadet.cpp \
	ofertasdbdoubleconnection.cpp ofertasipagables.cpp \
	ofertasbotonpasaraofertabehavior.cpp

#	ofertasreccuentaoferta.cpp ofertasfrmeditcuentaoferta.cpp \
#	ofertasrecapunteoferta.cpp ofertasfrmeditapunteoferta.cpp


libgongofertas_la_HEADERS = ofertasmodule.h ofertasmastertable.h \
 	ofertasreccobrooferta.h ofertasfrmeditcobrooferta.h \
 	ofertasrecpagooferta.h ofertasfrmeditpagooferta.h \
 	ofertasrecofertaventa.h ofertasfrmeditofertaventa.h \
 	ofertasrecofertacompra.h ofertasfrmeditofertacompra.h \
 	ofertasfrmeditofertacompradet.h ofertasrecofertacompradet.h \
	ofertasfrmeditofertaventadet.h ofertasrecofertaventadet.h \
	ofertasdbdoubleconnection.h ofertasipagables.h \
	ofertasbotonpasaraofertabehavior.h

#	ofertasreccuentaoferta.h ofertasfrmeditcuentaoferta.h \
#	ofertasrecapunteoferta.h ofertasfrmeditapunteoferta.h


moc_%.cpp: %.h
	moc-qt4 $< -o \$@

ofertas_sharedir = $(datadir)/gonglib/ofertas
dist_ofertas_share_DATA = $(srcdir)/share/ofertas.ddd

ofertas_informesdir = $(datadir)/gonglib/ofertas/informes
dist_ofertas_informes_DATA = $(srcdir)/share/informes/OFERTACOMPRA.rtk \
	$(srcdir)/share/informes/OFERTAVENTA.rtk

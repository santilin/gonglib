AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongsocias.la
libgongsocias_ladir = $(includedir)/gonglib

libgongsocias_la_SOURCES = sociasmodule.cpp \
	sociasfrmeditmiembro.cpp sociasfrmeditpartidaproyecto.cpp sociasfrmeditproyecto.cpp \
	sociasfrmedittiposocia.cpp sociasfrmgenerarrecibos.cpp \
	sociasrecmiembro.cpp sociasrecproyecto.cpp \
	sociasipagables.cpp sociascobrobehavior.cpp \
	sociasrectiposocia.cpp sociasrecpartidaproyecto.cpp \
	sociasfrmeditcobrobehavior.cpp sociasfrmeditremesacobrobehavior.cpp

libgongsocias_la_HEADERS = sociasmodule.h sociasfrmeditmiembro.h \
	sociasfrmeditpartidaproyecto.h sociasfrmeditproyecto.h \
	sociasfrmedittiposocia.h sociasfrmgenerarrecibos.h \
	sociasmastertable.h sociascobrobehavior.h \
	sociasrecmiembro.h sociasrecproyecto.h sociasipagables.h \
	sociasrectiposocia.h sociasrecpartidaproyecto.h  \
	sociasfrmeditcobrobehavior.h sociasfrmeditremesacobrobehavior.h

nodist_libgongsocias_la_SOURCES = \
	moc_sociasfrmeditmiembro.cpp moc_sociasfrmeditpartidaproyecto.cpp \
	moc_sociasfrmeditproyecto.cpp \
	moc_sociasfrmedittiposocia.cpp moc_sociasfrmgenerarrecibos.cpp \
	moc_sociasmodule.cpp moc_sociasfrmeditremesacobrobehavior.cpp

libgongsocias_la_SOURCES += sociasfrmeditapunte.cpp sociasrecapunte.cpp
libgongsocias_la_HEADERS += sociasfrmeditapunte.h sociasrecapunte.h
nodist_libgongsocias_la_SOURCES += moc_sociasfrmeditapunte.cpp

moc_%.cpp: %.h
	moc $< -o $@

socias_sharedir = $(datadir)/gestiong/socias
dist_socias_share_DATA = $(srcdir)/share/socias.ddd \
	$(srcdir)/share/ca_socias.ddd

socias_informesdir = $(datadir)/gestiong/socias/informes
dist_socias_informes_DATA = $(srcdir)/share/informes/firmasproyecto.rtk \
	$(srcdir)/share/informes/firmasproyecto_al.rtk \
	$(srcdir)/share/informes/firmasproyecto_blanco.rtk \
	$(srcdir)/share/informes/firmasproyecto_mz.rtk \
	$(srcdir)/share/informes/MIEMBRO.rtk \
	$(srcdir)/share/informes/PARTIDAPROYECTO.rtk \
	$(srcdir)/share/informes/PROYECTO.rtk \
	$(srcdir)/share/informes/proyectosdemiembros.rtk \
	$(srcdir)/share/informes/recibi.rtk \
	$(srcdir)/share/informes/recibosdemiembros.rtk \
	$(srcdir)/share/informes/recibosproyectocompleto.rtk \
	$(srcdir)/share/informes/MIEMBRO_LISTADO_DE_SOCIAS.rtk \
	$(srcdir)/share/informes/MIEMBRO_LISTADO_DE_SOCIAS_EXTENSO.rtk \
	$(srcdir)/share/informes/MIEMBRO_CARNETS_SOCIAS.rtk


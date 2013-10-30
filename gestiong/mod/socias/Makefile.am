INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongsocias.la
libgongsocias_ladir = $(includedir)/gonglib

libgongsocias_la_SOURCES = sociasmodule.cpp \
	sociasfrmeditmiembro.cpp sociasfrmeditpartidaproyecto.cpp sociasfrmeditproyecto.cpp \
	sociasfrmedittiposocia.cpp sociasfrmgenerarrecibos.cpp \
	sociasrecmiembro.cpp sociasrecproyecto.cpp \
	sociasipagables.cpp sociascobrobehavior.cpp \
	sociasrectiposocia.cpp sociasrecpartidaproyecto.cpp sociasfrmmailing.cpp \
	sociasfrmeditcobrobehavior.cpp sociasfrmeditremesacobrobehavior.cpp

nodist_libgongsocias_la_HEADERS = sociasmodule.h sociasfrmeditmiembro.h \
	sociasfrmeditpartidaproyecto.h sociasfrmeditproyecto.h \
	sociasfrmgenerarrecibos.h \
	sociasmastertable.h sociascobrobehavior.h \
	sociasrecmiembro.h sociasrecproyecto.h sociasipagables.h \
	sociasrectiposocia.h sociasrecpartidaproyecto.h sociasfrmmailing.h \
	sociasfrmeditcobrobehavior.h sociasfrmeditremesacobrobehavior.h

nodist_libgongsocias_la_SOURCES = \
	moc_sociasfrmeditmiembro.cpp moc_sociasfrmeditpartidaproyecto.cpp \
	moc_sociasfrmeditproyecto.cpp \
	moc_sociasfrmedittiposocia.cpp moc_sociasfrmgenerarrecibos.cpp \
	moc_sociasmodule.cpp moc_sociasfrmeditremesacobrobehavior.cpp

libgongsocias_la_SOURCES += sociasfrmeditapunte.cpp sociasrecapunte.cpp
nodist_libgongsocias_la_HEADERS += sociasfrmeditapunte.h sociasrecapunte.h
nodist_libgongsocias_la_SOURCES += moc_sociasfrmeditapunte.cpp

moc_%.cpp: %.h
	moc $< -o $@

socias_sharedir = $(datadir)/gestiong/socias
socias_share_DATA = $(top_srcdir)/socias/share/socias.ddd \
	$(top_srcdir)/socias/share/ca_socias.ddd

socias_informesdir = $(datadir)/gestiong/socias/informes
socias_informes_DATA = $(top_srcdir)/socias/share/informes/ASIENTO.rtk \
	$(top_srcdir)/socias/share/informes/firmasproyecto.rtk \
	$(top_srcdir)/socias/share/informes/firmasproyecto_al.rtk \
	$(top_srcdir)/socias/share/informes/firmasproyecto_blanco.rtk \
	$(top_srcdir)/socias/share/informes/firmasproyecto_mz.rtk \
	$(top_srcdir)/socias/share/informes/MIEMBRO.rtk \
	$(top_srcdir)/socias/share/informes/PARTIDAPROYECTO.rtk \
	$(top_srcdir)/socias/share/informes/PROYECTO.rtk \
	$(top_srcdir)/socias/share/informes/proyectosdemiembros.rtk \
	$(top_srcdir)/socias/share/informes/recibi.rtk \
	$(top_srcdir)/socias/share/informes/recibosdemiembros.rtk \
	$(top_srcdir)/socias/share/informes/recibosproyectocompleto.rtk

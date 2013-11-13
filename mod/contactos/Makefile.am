INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongcontactos.la
libgongcontactos_ladir = $(includedir)/gonglib

libgongcontactos_la_SOURCES = contactosmodule.cpp \
	contactosfldcif.cpp contactosfldnombrepropio.cpp \
	contactosfrmeditcontacto.cpp contactosreccontacto.cpp \
	contactosfrmmailing.cpp contactosicontactablerecord.cpp \
	contactosfrmeditcontactobehavior.cpp

libgongcontactos_la_HEADERS = contactosmodule.h contactosmastertable.h \
	contactosfldcif.h contactosfldnombrepropio.h \
	contactosfrmeditcontacto.h contactosreccontacto.h \
	contactosfrmmailing.h contactosicontactablerecord.h \
	contactosfrmeditcontactobehavior.h contactosreccontactoimagen.h

nodist_libgongcontactos_la_SOURCES = \
	moc_contactosfrmeditcontacto.cpp moc_contactosmodule.cpp \
	moc_contactosfrmeditcontactobehavior.cpp

moc_%.cpp: %.h
	moc $< -o $@

contactos_sharedir = $(datadir)/gestiong/contactos
contactos_share_DATA = $(top_srcdir)/share/gestiong/contactos/contactos.ddd \
	$(top_srcdir)/share/gestiong/contactos/stylesheet.css \
	$(top_srcdir)/share/gestiong/contactos/provincias.txt \
	$(top_srcdir)/share/gestiong/contactos/codigospostales.txt \
	$(top_srcdir)/share/gestiong/contactos/calles.txt

contactos_informesdir = $(datadir)/gestiong/contactos/informes
contactos_informes_DATA = $(top_srcdir)/share/gestiong/contactos/informes/CONTACTO.rtk


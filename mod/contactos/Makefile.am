AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongcontactos.la
libgongcontactos_ladir = $(includedir)/gonglib

libgongcontactos_la_SOURCES = contactosmodule.cpp \
	contactosfldcif.cpp contactosfldnombrepropio.cpp \
	contactosreccontacto.cpp contactosicontactablerecord.cpp 

libgongcontactos_la_HEADERS = contactosmodule.h contactosmastertable.h \
	contactosfldcif.h contactosfldnombrepropio.h \
	contactosreccontacto.h contactosicontactablerecord.h \
	contactosreccontactoimagen.h

if COMPILE_WITH_GONG_QT4
libgongcontactos_la_SOURCES += contactosfrmeditcontacto.cpp \
	contactosfrmeditcontactobehavior.cpp
libgongcontactos_la_HEADERS += contactosfrmeditcontacto.h \
	contactosfrmeditcontactobehavior.h 
nodist_libgongcontactos_la_SOURCES = \
	moc_contactosfrmeditcontacto.cpp moc_contactosmodule.cpp \
	moc_contactosfrmeditcontactobehavior.cpp
endif

moc_%.cpp: %.h
	moc $< -o $@

contactos_sharedir = $(datadir)/gestiong/contactos
dist_contactos_share_DATA = $(srcdir)/share/contactos.ddd \
	$(srcdir)/share/stylesheet.css \
	$(srcdir)/share/provincias.txt \
	$(srcdir)/share/codigospostales.txt \
	$(srcdir)/share/calles.txt

contactos_informesdir = $(datadir)/gestiong/contactos/informes
dist_contactos_informes_DATA = $(srcdir)/share/informes/CONTACTO.rtk


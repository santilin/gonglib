INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongdbapp.la
libgongdbapp_ladir = $(includedir)/gonglib

libgongdbapp_la_SOURCES = dbappfrmlogin_gui.cpp dbappfrmlogin.cpp \
	dbappdbapplication.cpp dbappdbmodule.cpp \
	dbappfrmcustom.cpp dbappfrmeditrecmaster.cpp dbappsearchbox.cpp \
	dbappfrmeditrec.cpp dbappfrmexport.cpp \
	dbappfrmeditrecdetail.cpp dbappfrmexport_gui.cpp \
	dbappfrmsqlselect.cpp dbappfrmviewedit.cpp dbappmainwindow.cpp \
	dbappfldstringautoinc.cpp dbappsettingstree.cpp dbappfrmsettingsedit.cpp \
	dbappfrmdatabasetools.cpp dbappfrmintegrity.cpp dbappfrmeditrecbehavior.cpp \
	dbappfrmadvancedprint.cpp dbappnameslisttable.cpp dbappfrmeditnameslisttable.cpp \
	dbapprecmetadbdata.cpp dbappfrmcalculator.cpp dbappfrmmultiupdate.cpp \
	dbappfrmmodulesettings.cpp dbappfrmabout.cpp

libgongdbapp_la_HEADERS = dbappdbapplication.h dbappdbmodule.h \
	dbappfrmcustom.h dbappfrmeditrec.h dbappsearchbox.h \
	dbappfrmexport_gui.h \
	dbappfrmeditrecmaster.h dbappfrmexport.h dbappfrmlogin_gui.h \
	dbappfrmeditrecdetail.h dbappfrmlogin.h \
	dbappfrmsqlselect.h dbappfrmviewedit.h dbappmainwindow.h \
	dbappfldstringautoinc.h dbappsettingstree.h dbappfrmsettingsedit.h \
	dbappfrmdatabasetools.h dbappfrmintegrity.h dbappfrmeditrecbehavior.h \
	dbappfrmadvancedprint.h dbappnameslisttable.h dbappfrmeditnameslisttable.h \
	dbapprecmetadbdata.h dbappfrmcalculator.h dbappfrmmultiupdate.h \
	dbappfrmmodulesettings.h dbappfrmabout.h

nodist_libgongdbapp_la_SOURCES = \
	moc_dbappfrmcustom.cpp moc_dbappfrmeditrecdetail.cpp moc_dbappfrmeditrec.cpp \
	moc_dbappfrmeditrecmaster.cpp moc_dbappfrmexport_gui.cpp moc_dbappfrmexport.cpp \
	moc_dbappfrmlogin_gui.cpp \
	moc_dbappfrmlogin.cpp moc_dbappfrmsettingsedit.cpp moc_dbappfrmsqlselect.cpp \
	moc_dbappfrmviewedit.cpp moc_dbappmainwindow.cpp moc_dbappsettingstree.cpp \
	moc_dbappfrmintegrity.cpp moc_dbappfrmcalculator.cpp moc_dbappsearchbox.cpp \
	moc_dbappfrmabout.cpp

if COMPILE_WITH_POCOLIB
libgongdbapp_la_SOURCES += \
	dbappfrmimport.cpp dbappfrmimport_gui.cpp dbappfugitimporter.cpp
libgongdbapp_la_HEADERS += \
	dbappfrmimport.h dbappfrmimport_gui.h dbappfugitimporter.h
nodist_libgongdbapp_la_SOURCES += \
	moc_dbappfrmimport_gui.cpp moc_dbappfrmimport.cpp
endif

if COMPILE_WITH_GONG_RTK
libgongdbapp_la_SOURCES += \
	dbappfrmreports.cpp dbappfrmreports_gui.cpp \
	dbappreport.cpp dbappreportinput.cpp dbappreportqtfrmparams.cpp \
	dbappreportqtviewer.cpp dbappreportqtpagedisplay.cpp \
	dbappreportqtoutput.cpp
libgongdbapp_la_HEADERS += \
	dbappfrmreports.h dbappfrmreports_gui.h \
	dbappreport.h dbappreportinput.h dbappreportqtviewer.h \
	dbappreportqtpagedisplay.h dbappreportqtfrmparams.h \
	dbappreportqtoutput.h
nodist_libgongdbapp_la_SOURCES += \
	moc_dbappfrmreports_gui.cpp moc_dbappfrmreports.cpp \
	moc_dbappreportqtviewer.cpp

opendocumentdir= $(datadir)/gestiong/opendocument/calc/template
dist_opendocument_DATA = $(srcdir)/share/opendocument/calc/template/meta.xml \
	$(srcdir)/share/opendocument/calc/template/mimetype \
	$(srcdir)/share/opendocument/calc/template/settings.xml \
	$(srcdir)/share/opendocument/calc/template/styles.xml

opendocumentmetadir= $(datadir)/gestiong/opendocument/calc/template/META-INF
dist_opendocumentmeta_DATA = $(srcdir)/share/opendocument/calc/template/META-INF/manifest.xml

endif

moc_%.cpp: %.h
	moc $< -o $@

dbapp_sharedir = $(datadir)/gestiong/dbapp
dist_dbapp_share_DATA = $(srcdir)/share/logogestiong.jpg \
	$(srcdir)/share/settings.rc \
	$(srcdir)/share/stylesheet.css


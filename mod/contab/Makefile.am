INCLUDES = @GONGLIB_CPPFLAGS@
AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongcontab.la
libgongcontab_ladir = $(includedir)/gonglib

libgongcontab_la_SOURCES = contabmodule.cpp contabcuenta.cpp \
	contabfrmeditapunte.cpp contabfrmeditasiento.cpp \
	contabfrmeditcuenta.cpp contabfrmeditempresa.cpp \
	contabrecapunte.cpp contabrecasiento.cpp contabreccuenta.cpp \
	contabfrmedittipoivabehavior.cpp \
	contabfldcuenta.cpp contabiasentablerecord.cpp

libgongcontab_la_HEADERS = contabmastertable.h contabcuenta.h \
	contabfrmeditapunte.h contabfrmeditasiento.h \
	contabfrmeditcuenta.h contabfrmeditempresa.h \
	contabmodule.h contabrecapunte.h contabrecasiento.h \
	contabreccuenta.h contabfrmedittipoivabehavior.h \
	contabfldcuenta.h contabiasentablerecord.h

nodist_libgongcontab_la_SOURCES = \
	moc_contabmodule.cpp \
	moc_contabfrmeditapunte.cpp \
	moc_contabfrmeditasiento.cpp \
	moc_contabfrmeditcuenta.cpp \
	moc_contabfrmeditempresa.cpp

moc_%.cpp: %.h
	moc $< -o $@

contab_sharedir = $(datadir)/gestiong/contab
dist_contab_share_DATA = $(top_srcdir)/share/gestiong/contab/contab.ddd \
	$(top_srcdir)/share/gestiong/contab/RealDecreto_1415_2007.fugit \
	$(top_srcdir)/share/gestiong/contab/RealDecreto_1515_2007_ONGS.fugit \
	$(top_srcdir)/share/gestiong/contab/RealDecreto_1515_2007_PYMES.fugit

contab_informesdir = $(datadir)/gestiong/contab/informes
dist_contab_informes_DATA = $(top_srcdir)/share/gestiong/contab/informes/CUENTA.rtk \
	$(top_srcdir)/share/gestiong/contab/informes/cuentas.rtk \
	$(top_srcdir)/share/gestiong/contab/informes/diariobonito.rtk \
	$(top_srcdir)/share/gestiong/contab/informes/sumasaldos.rtk \
	$(top_srcdir)/share/gestiong/contab/informes/tipodoc_recibi.rtk

#dist_bin_SCRIPTS = $(top_srcdir)/share/gestiong/scripts/contaplus2fugit.py

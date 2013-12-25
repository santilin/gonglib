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
	contabfldcuenta.h contabiasentablerecord.h contabvalidatecuenta.inc

nodist_libgongcontab_la_SOURCES = \
	moc_contabmodule.cpp \
	moc_contabfrmeditapunte.cpp \
	moc_contabfrmeditasiento.cpp \
	moc_contabfrmeditcuenta.cpp \
	moc_contabfrmeditempresa.cpp

moc_%.cpp: %.h
	moc $< -o $@

contab_sharedir = $(datadir)/gestiong/contab
dist_contab_share_DATA = $(srcdir)/share/contab.ddd \
	$(srcdir)/share/RealDecreto_1415_2007.fugit \
	$(srcdir)/share/RealDecreto_1515_2007_ONGS.fugit \
	$(srcdir)/share/RealDecreto_1515_2007_PYMES.fugit

contab_informesdir = $(datadir)/gestiong/contab/informes
dist_contab_informes_DATA = $(srcdir)/share/informes/CUENTA.rtk \
	$(srcdir)/share/informes/ASIENTO.rtk \
	$(srcdir)/share/informes/cuentas.rtk \
	$(srcdir)/share/informes/diariobonito.rtk \
	$(srcdir)/share/informes/sumasaldos.rtk \
	$(srcdir)/share/informes/tipodoc_recibi.rtk

#dist_bin_SCRIPTS = $(srcdir)/share/gestiong/scripts/contaplus2fugit.py

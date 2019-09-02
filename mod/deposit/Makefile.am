AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongdeposit.la
libgongdeposit_ladir = $(includedir)/gonglib
libgongdeposit_la_SOURCES = depositmodule.cpp \
	depositfrmeditliquidacionventa.cpp depositfrmeditliquidacionventadet.cpp \
	depositrecliquidacionventa.cpp depositrecliquidacionventadet.cpp \
	depositfrmeditliquidacioncompra.cpp depositfrmeditliquidacioncompradet.cpp \
	depositrecliquidacioncompra.cpp depositrecliquidacioncompradet.cpp \
	depositfrmfactliqventa.cpp depositfrmgenliqcompra.cpp \
	depositfrmestadcompraventa.cpp depositfrmliquidardepositos.cpp \
	depositfrmedittipodocbehavior.cpp depositfrmeditventadetbehavior.cpp

libgongdeposit_la_HEADERS = depositmodule.h depositmastertable.h \
	depositfrmeditliquidacionventa.h depositfrmeditliquidacionventadet.h \
	depositrecliquidacionventa.h depositrecliquidacionventadet.h \
	depositfrmeditliquidacioncompra.h depositfrmeditliquidacioncompradet.h \
	depositrecliquidacioncompra.h depositrecliquidacioncompradet.h \
	depositfrmfactliqventa.h depositfrmgenliqcompra.h \
	depositfrmestadcompraventa.h depositfrmliquidardepositos.h \
	depositfrmedittipodocbehavior.h depositfrmeditventadetbehavior.h

nodist_libgongdeposit_la_SOURCES = moc_depositmodule.cpp \
	moc_depositfrmeditliquidacionventa.cpp moc_depositfrmeditliquidacionventadet.cpp \
	moc_depositfrmeditliquidacioncompra.cpp moc_depositfrmeditliquidacioncompradet.cpp \
	moc_depositfrmeditventadetbehavior.cpp

moc_%.cpp: %.h
	moc $< -o $@

deposit_sharedir = $(datadir)/gestiong/deposit
dist_deposit_share_DATA = $(srcdir)/share/deposit.ddd

deposit_informesdir = $(datadir)/gestiong/deposit/informes
dist_deposit_informes_DATA = \
	$(srcdir)/share/informes/DEPOSITOCOMPRA.rtk \
	$(srcdir)/share/informes/LIQUIDACIONCOMPRA.rtk \
	$(srcdir)/share/informes/depositestadcompraventa.inc \
	$(srcdir)/share/informes/depositestadcompraventa_resumido.inc

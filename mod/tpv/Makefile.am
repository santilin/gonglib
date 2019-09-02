AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgongtpv.la
libgongtpv_ladir = $(includedir)/gonglib

libgongtpv_la_SOURCES = tpvmodule.cpp \
	tpvfrmtpvscreen.cpp tpvimagesgallery.cpp \
	tpvfrmeditfamiliabehavior.cpp tpvfrmeditarticulobehavior.cpp \
	tpvfrmeditagentebehavior.cpp \
	tpvfrmchoosemesa.cpp tpvfrmticketcambio.cpp \
	tpvfrmchoosebyimage.cpp tpvsvgview.cpp tpvbutton.cpp \
	tpvrecsesiontpv.cpp tpvfrmeditsesiontpv.cpp \
	tpvfrmestadcompraventa.cpp tpvkeyboard.cpp \
	rtkinputdbrecord.cpp rs232.cpp tpvfrmcustom.cpp

libgongtpv_la_HEADERS = tpvmodule.h tpvmastertable.h \
	tpvapplication.h \
	tpvfrmtpvscreen.h tpvimagesgallery.h \
	tpvfrmeditfamiliabehavior.h tpvfrmeditarticulobehavior.h \
	tpvfrmeditagentebehavior.h \
	tpvfrmchoosemesa.h tpvfrmticketcambio.h \
	tpvfrmchoosebyimage.h tpvsvgview.h tpvbutton.h \
	tpvrecsesiontpv.h tpvfrmeditsesiontpv.h \
	tpvfrmestadcompraventa.h tpvkeyboard.h \
	rtkinputdbrecord.h rs232.h tpvfrmcustom.h

nodist_libgongtpv_la_SOURCES = moc_tpvmodule.cpp \
	moc_tpvfrmtpvscreen.cpp moc_tpvimagesgallery.cpp \
	moc_tpvsvgview.cpp moc_tpvfrmchoosemesa.cpp moc_tpvfrmticketcambio.cpp \
	moc_tpvfrmeditsesiontpv.cpp moc_tpvkeyboard.cpp \
	moc_tpvfrmchoosebyimage.cpp moc_tpvfrmeditfamiliabehavior.cpp \
	moc_tpvfrmeditarticulobehavior.cpp

moc_%.cpp: %.h
	moc $< -o $@


tpv_sharedir = $(datadir)/gestiong/tpv
dist_tpv_share_DATA = $(srcdir)/share/tpv.ddd \
	$(srcdir)/share/stylesheet.css

tpv_imagesdir = $(datadir)/gestiong/tpv/images
# $(srcdir)/share/images/nextpage.jpg \
dist_tpv_images_DATA = \
	$(srcdir)/share/images/prevpage.jpg

tpv_informesdir = $(datadir)/gestiong/tpv/informes
dist_tpv_informes_DATA = $(srcdir)/share/informes/TICKET.rtk \
	$(srcdir)/share/informes/TICKET_AHORRAPAPEL.rtk \
	$(srcdir)/share/informes/TPVZ.rtk \
	$(srcdir)/share/informes/TPVZ_header.inc \
	$(srcdir)/share/informes/ticket_footer.inc \
	$(srcdir)/share/informes/ticket_header.inc \
	$(srcdir)/share/informes/ticket_header_ahorrapapel.inc

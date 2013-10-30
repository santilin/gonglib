INCLUDES = @GONGLIB_CPPFLAGS@

AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

lib_LTLIBRARIES = libgongrtk.la
libgongrtk_la_LDFLAGS = -version-info 0:5:0
libgongrtk_ladir = $(includedir)/gonglib

libgongrtk_la_SOURCES = rtk.cpp rtkreport.cpp rtkbaseimage.cpp \
	rtkcolor.cpp rtkcompiler.cpp rtkcompiler_save.cpp rtkcontext.cpp rtkerror.cpp \
	rtkfloattable.cpp rtkformula.cpp rtkfunctions.cpp \
	rtkimage.cpp rtkinput.cpp rtkinputcached.cpp rtkinputcsv.cpp rtkinputfield.cpp \
	rtkinputmysql.cpp rtkinputpgsql.cpp rtkinputxml.cpp rtknamespace.cpp rtkobject.cpp \
	rtkopcodes.cpp rtkoutput.cpp rtkoutputcached.cpp rtkoutputcsv.cpp rtkoutputhtml.cpp \
	rtkoutputmysql.cpp rtkoutputodfcalc1.cpp rtkoutputps.cpp \
	rtkoutputtext.cpp rtkoutputxml.cpp rtkparameter.cpp rtkparsertk.cpp rtkproperty.cpp \
	rtksection.cpp rtkstdlib.cpp rtkstringtable.cpp rtkstyle.cpp rtkvalue.cpp \
	rtkvmachine.cpp rtkinputxbase.cpp rtkoutputgnumeric.cpp

nodist_libgongrtk_la_HEADERS = rtkbaseimage.h rtkcolor.h rtkcompiler.h rtkconfig.h rtkcontext.h \
	rtkdscript.h rtkerror.h rtkfloattable.h rtkformula.h rtkfunctions.h \
	rtkimage.h rtkinput.h rtkinputcached.h rtkinputcsv.h rtkinputfield.h \
	rtkinputmysql.h rtkinputpgsql.h rtkinputxbase.h rtkinputxml.h rtkinstruction.h \
	rtknamespace.h rtkobject.h rtkopcodes.h rtkoutput.h rtkoutputcached.h rtkoutputcsv.h \
	rtkoutputhtml.h rtkoutputmysql.h rtkoutputodfcalc1.h rtkoutputps.h \
	rtkoutputtext.h rtkoutputxml.h rtkparameter.h rtkproperty.h \
	rtkreport.h rtksection.h rtkstdlib.h \
	rtkstringtable.h rtkstyle.h rtkvalue.h rtkvmachine.h rtkoutputgnumeric.h


#opendocumentdir= $(datadir)/$(PACKAGE)/opendocument/calc/template
#dist_opendocument_DATA = $(top_srcdir)/share/gestiong/opendocument/calc/template/meta.xml \
#	$(top_srcdir)/share/gestiong/opendocument/calc/template/mimetype \
#	$(top_srcdir)/share/gestiong/opendocument/calc/template/settings.xml \
#	$(top_srcdir)/share/gestiong/opendocument/calc/template/styles.xml

#opendocumentmetadir= $(datadir)/$(PACKAGE)/opendocument/calc/template/META-INF
#dist_opendocumentmeta_DATA = $(top_srcdir)/share/gestiong/opendocument/calc/template/META-INF/manifest.xml





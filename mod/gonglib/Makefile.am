localedir = $(datadir)/locale
AM_CPPFLAGS = @AM_CPPFLAGS@ -DLOCALEDIR=\"$(localedir)\"
AM_CXXFLAGS = @AM_CXXFLAGS@ @XML_CPPFLAGS@

libgonglib_la_LDFLAGS = -version-info 0:5:0
libgonglib_ladir = $(includedir)/gonglib
lib_LTLIBRARIES = libgonglib.la
libgonglib_la_SOURCES = \
	gongcsvutils.cpp \
	gongdate.cpp \
	gongdbconnection.cpp \
	gongdbdefinition.cpp \
	gongdbfielddefinition.cpp \
	gongdbfieldreferenceid.cpp \
	gongdbfieldstyle.cpp \
	gongdbfieldvalue.cpp \
	gongdbindexdefinition.cpp \
	gongdbvalidresult.cpp \
	gongdbrecord.cpp \
	gongdbrecordext.cpp \
	gongdbrecordlist.cpp \
	gongdbrecordrelation.cpp \
	gongdbrelationdefinition.cpp \
	gongdbresultset.cpp \
	gongdbtabledefinition.cpp \
	gongdbuser.cpp gongdebug.cpp \
	gongdictionary.cpp \
	gongfileutils.cpp \
	gongformatter.cpp \
	gongiconv.cpp \
	gonglibrary.cpp \
	gongmoney.cpp \
	gongdbrecorddatamodel.cpp \
	gongdbrecordlistdatamodel.cpp \
	gongregconfig.cpp \
	gongregexp.cpp \
	gongsettings.cpp \
	gongdbsettings.cpp \
	gongtabledatamodel.cpp \
	gongdbviewdefinition.cpp \
	gongvariant.cpp \
	gongxmlparser.cpp \
	gongxtring.cpp \
	gongdbfieldemail.cpp \
	gongdbmastertable.cpp \
	gongdbfieldimage.cpp \
	gonghttpclient.cpp \
	gongdbresultsetdatamodel.cpp \
	gongdbrecordbehavior.cpp \
	gongdbrecordtimestampbehavior.cpp \
	gongdbcalculator.cpp

nodist_libgonglib_la_HEADERS = \
	gongcsvutils.h \
	gongdate.h \
	gongdbconnection.h \
	gongdbdefinition.h \
	gongdberror.h \
	gongdbfielddefinition.h \
	gongdbfieldlistofvalues.h \
	gongdbfieldreferenceid.h \
	gongdbfieldstyle.h \
	gongdbfieldvalue.h \
	gongdbindexdefinition.h \
	gongdbvalidresult.h gongdbrecord.h gongdbrecordlist.h \
	gongdbrecordrelation.h gongdbrecordpermissions.h gongdbrelationdefinition.h gongdbresultset.h \
	gongdbtabledefinition.h gongdbuser.h gongdebug.h gongdictionary.h \
	gongfileutils.h gongformatter.h gongiconv.h gonglibrary.h \
	gonglist.h gonglist.hpp gongmoney.h \
	gongdbrecorddatamodel.h gongdbrecordlistdatamodel.h gongregconfig.h gongregexp.h \
	gongsettings.h gongdbsettings.h gongdbviewdefinition.h \
	gongtabledatamodel.h gongvariant.h gongxmlparser.h gongxtring.h \
	gonggettext.h gongdbcalculator.h \
	gongdbfieldemail.h gongdbmastertable.h gongdbfieldimage.h \
	gonghttpclient.h gongdbresultsetdatamodel.h \
	gongdbrecordbehavior.h gongdbrecordtimestampbehavior.h


AM_CPPFLAGS = @GONGLIB_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@

gongtest_LDADD = @GONGLIB_LIBS@

bin_PROGRAMS = gongtest
gongtest_SOURCES = main.cpp testdbcommon.cpp \
	testhttpserver.cpp testregexp.cpp testlibrary.cpp testlog.cpp \
	testdate.cpp \
	testdatabase.cpp \
	testdbconnection.cpp testdbdefinition.cpp testdbfieldvalue.cpp testdbrecord.cpp \
	testdbschema.cpp testdbtabledef.cpp testintegrityresult.cpp \
	testsettings.cpp testtabledatamodel.cpp testregconfig.cpp \
	testunicode.cpp testmoney.cpp testcsv.cpp \
	testxmlparser.cpp 
	
#	testlistview.cpp testviewtable.cpp testimportcsv.cpp \
	testdbcalculator.cpp testdbnameslisttable.cpp \
	testlineedit.cpp testfrmbase.cpp testfrmcustom.cpp 

noinst_HEADERS = testdbcommon.h \
	testdatabase.h testdbconnection.h \
	testdbdefinition.h testdbfieldvalue.h testdbrecord.h testdbschema.h testdbtabledef.h \
	testintegrityresult.h testscript.h testsettings.h testtabledatamodel.h \
	testregconfig.h testunicode.h testmoney.h testcsv.h \
	testxmlparser.h 
	
#	testlineedit.h testfrmbase.h testfrmcustom.h \
	testdbcalculator.h testdbnameslisttable.h \
	testviewtable.h testimportcsv.h  \
	testlistview.h 




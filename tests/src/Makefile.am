AM_CPPFLAGS = @GONGLIB_CPPFLAGS@
AM_CXXFLAGS = -std=c++11 @AM_CXXFLAGS@

gongtest_LDADD = @GONGLIB_LIBS@

bin_PROGRAMS = gongtest
gongtest_SOURCES = main.cpp testcommon.cpp testdatabase.cpp \
	testdbconnection.cpp testdbdefinition.cpp testdbfieldvalue.cpp testdbrecord.cpp \
	testdbschema.cpp testdbtabledef.cpp testintegrityresult.cpp testlibrary.cpp \
	testsettings.cpp testtabledatamodel.cpp testregconfig.cpp \
	testdate.cpp testunicode.cpp testmoney.cpp testcsv.cpp \
	testxmlparser.cpp testhttpserver.cpp testregexp.cpp
	
#	testlistview.cpp testviewtable.cpp testimportcsv.cpp \
	testdbcalculator.cpp testdbnameslisttable.cpp \
	testlineedit.cpp testfrmbase.cpp testfrmcustom.cpp 

noinst_HEADERS = testcommon.h testdatabase.h testdbconnection.h \
	testdbdefinition.h testdbfieldvalue.h testdbrecord.h testdbschema.h testdbtabledef.h \
	testintegrityresult.h testlibrary.h testscript.h testsettings.h testtabledatamodel.h \
	testregconfig.h testunicode.h testmoney.h testcsv.h \
	testxmlparser.h 
	
#	testlineedit.h testfrmbase.h testfrmcustom.h \
	testdbcalculator.h testdbnameslisttable.h \
	testviewtable.h testimportcsv.h  \
	testlistview.h 





#include <gonglibrary.h>
#include <limits>
#include <gongdbfieldlistofvalues.hpp>
#include "testmoney.h"
#include "testdatabase.h"
#include "testdbconnection.h"
#include "testdbdefinition.h"
#include "testdbschema.h"
#include "testdbfieldvalue.h"
#include "testdbrecord.h"
#include "testtabledatamodel.h"
#include "testintegrityresult.h"
#include "testsettings.h"
#include "testlibrary.h"
#include "testdbtabledef.h"
#include "testregconfig.h"
#include "testdate.h"
#include "testunicode.h"
#include "testlineedit.h"
#include "testfrmcustom.h"
#include "testcsv.h"
#include "testlistview.h"
#include "testdbcalculator.h"
// #include "testsearchisbn.h"
#include "testhttpclient.h"
#include "testdbnameslisttable.h"

#include <stdarg.h>      // addError

#include <QGraphicsScene>
#include <QGraphicsView>

using namespace gong;


void print_limits()
{
	_GONG_DEBUG_TRACE(0);
	std::numeric_limits<long> nll;
	_GONG_DEBUG_PRINT(0, Xtring::printf("%ld", nll.min()));
	_GONG_DEBUG_PRINT(0, Xtring::printf("%ld", nll.max()));
	std::numeric_limits<long long> nlll;
	_GONG_DEBUG_PRINT(0, Xtring::printf("%lld", nlll.min()));
	_GONG_DEBUG_PRINT(0, Xtring::printf("%lld", nlll.max()));
	std::numeric_limits<double> nld;
	_GONG_DEBUG_PRINT(0, Xtring::printf("%f", nld.min()));
	_GONG_DEBUG_PRINT(0, Xtring::printf("%f", nld.max()));
	std::numeric_limits<long double> nlld;
	_GONG_DEBUG_PRINT(0, Xtring::printf("%f", nlld.min()));
	_GONG_DEBUG_PRINT(0, Xtring::printf("%f", nlld.max()));
}

int main(int argc, char *argv[])
{
	__gong_debug::_gong_debug_level = 4;

#if 0
	TestMoney testmoney;
	testmoney.run();
	TestDate testdate;
	testdate.run();
	TestSettings testsettings;
	testsettings.run();
	TestConnection testconnection;
	testconnection.run();
	TestDatabase testdatabase;
	testdatabase.run();
	TestdbFieldValue testdbfieldvalue;
	testdbfieldvalue.run();
	TestdbDefinition testdbdefinition;
	testdbdefinition.run();
	TestTableDataModel testtabledatamodel;
	testtabledatamodel.run();
	TestdbSchema testdbschema;
	testdbschema.run();
	TestTableDef testtabledef;
	testtabledef.run();
	TestdbRecord testdbrecord;
	testdbrecord.run();
	TestValidResult testvalidresult;
	testvalidresult.run();
	TestLibrary testlibrary;
	testlibrary.run();
	TestRegConfig testregconfig;
	testregconfig.run();
	TestUnicode testunicode;
	testunicode.run();
	TestCSV testcsv;
	testcsv.run();
 	TestdbCalculator testdbcalculator;
 	testdbcalculator.run("4 + PVP");
#endif

	TestdbNamesListTable testdbnameslisttable;
	TestdbNamesListTable.run();

// 	TestSearchISBN testisbnsearch;
// 	testisbnsearch.run();
// 	TestHTTPClient testhttpclient;
// 	testhttpclient.run();
#if 0
#ifdef HAVE_DBAPPMODULE
	gong::GongLibrary l("","GONG-TESTS","", argc, argv);
	gong::GongLibraryInstance = &l;
	gong::GongLibraryInstance->getRegConfig()->setCurrencyFormat( "€", 2, ',' );
	TestFrmCustom testfrmcustom;
	testfrmcustom.run();
	TestLineEdit testlineedit;
	testlineedit.run();
	TestListView tlv;
	tlv.run();
#endif
#endif


}


#ifdef TEST_SVG
	QApplication   app(argc, argv);
	QGraphicsScene scene;
	QGraphicsView  view(&scene);

	QSvgRenderer*     svgRend = new QSvgRenderer(QLatin1String("/home/santilin/.gonglib/lospajaros/tpv/mesas.svg"));
	QGraphicsSvgItem* svg     = new QGraphicsSvgItem();

	if(svgRend->elementExists("layer1"))
	{
		svg->setSharedRenderer(svgRend);
		svg->setElementId(QLatin1String("layer1"));
	}

	scene.addItem(svg);

	view.show();

	return app.exec();
#endif

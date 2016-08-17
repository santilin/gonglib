
#include <iostream>
#include <gongguiapplication.h>
#include <gongdbrecorddatamodel.h>
#include <gongdbviewdefinition.h>
#include <gongviewtable.h>
#include "testcommon.h"
#include "testviewtable.h"


using namespace gong;

TestViewTable::TestViewTable()
{
}


TestViewTable::~TestViewTable()
{
}

void TestViewTable::run()
{
	int argc = 1;
	char *argv[] = { (char *)"gongcommontests", 0 };
	TestCommon t;
	QApplication app(argc, argv, true);
	DerivedRecord contacto(&t.mConnection, t.pTableContactos );
	contacto.setValue("ID",1);
	contacto.setValue("NOMBRE","Santilín");
	contacto.save(false, false);
	contacto.setNew(true);
	contacto.setValue("ID",2);
	contacto.setValue("NOMBRE","Alicilina");
	contacto.save(false, false);
	dbViewDefinition *view = new dbViewDefinition( *t.pTableContactos );
	dbViewDefinitionDict contactos_view;
	contactos_view.insert( "Default", view );
/*	
	ViewTable edittable( &contacto, contactos_view );
	edittable.setView(0);
	edittable.show();
	app.setMainWidget(&edittable);
	app.exec();
*/	
}

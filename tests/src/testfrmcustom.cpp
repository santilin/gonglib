
#include "config.h"

#ifdef HAVE_DBAPPMODULE

#include <iostream>
#include "gongguiapplication.h"
#include "gongguimainwindow.h"
#include <gonglist.hpp>
#include "testfrmcustom.h"


using namespace gong;

FrmCustomForTesting::FrmCustomForTesting(QWidget *parent, const char *name, WidgetFlags f)
		: FrmCustom(parent, name, f)
{
	cb1 = addCheckBox(0, "Check Box 1", false );
	XtringList l;
	l << "Hola" << "Adiós";
	_GONG_DEBUG_PRINT(0, l.join(",") );
	combo1 = addComboBoxXtring(false, 0, "Combo box 1", l );

	XtringList opciones;
	opciones << "Opcion1" << "Opcion2" << "Opcion3";
	gb1 = addGroupBox(0, opciones, "Grupo de opciones", 2);
	gb2 = addGroupBox(0, opciones, "Grupo de opciones", 1);
	gb2->setReadOnly( true );
}

void FrmCustomForTesting::test()
{
	_GONG_DEBUG_ASSERT( gb1->getSelectedPos() == 2 );
	_GONG_DEBUG_ASSERT( gb1->getSelectedString() == "Opcion3" );
	_GONG_DEBUG_ASSERT( !cb1->isOn() );
}


TestFrmCustom::TestFrmCustom()
{
	std::cout << "TestFrmCustom started" << std::endl;
}


TestFrmCustom::~TestFrmCustom()
{
}

void TestFrmCustom::run()
{
	int argc = 1;
	char *argv[] = { (char *)"gongcommontests", 0 };
	GuiApplication app("", "", "", argc, argv);

	FrmCustomForTesting *f = new FrmCustomForTesting(0, "FrmCustomForm test");
	f->show();
	app.setMainWidget(f);
	app.exec();
	f->test();
	delete f;
}

#endif

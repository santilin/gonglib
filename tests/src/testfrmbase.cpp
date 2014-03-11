//
// C++ Implementation: testgongform
//
// Description:
//
//
// Author: Santiago Capel <elcansino@arremolina.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <qapplication.h>
#include <iostream>
#include "testfrmbase.h"
#include "gongguiapplication.h"
#include "gongguimainwindow.h"


using namespace gong;

TestFrmBase::TestFrmBase()
{
	std::cout << "TestFrmBase started" << std::endl;
}


TestFrmBase::~TestFrmBase()
{
}

void TestFrmBase::run()
{
	int argc = 1;
	char *argv[] = { (char *)"gongcommontests", 0 };
	GuiApplication app("", "", "", argc, argv);

	FrmBase *f = new FrmBase(0, "FrmBase test");
	f->show();
	app.setMainWidget(f);
//	app.exec();
	delete f;


}


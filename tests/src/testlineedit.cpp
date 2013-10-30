
#include <iostream>
#include <gongguiapplication.h>
#include <gonglineedit.h>
#include "testfrmbase.h"
#include "testlineedit.h"


using namespace gong;

TestLineEdit::TestLineEdit()
{
}


TestLineEdit::~TestLineEdit()
{
}



void TestLineEdit::run()
{
	test2();
//	test1();
}

void TestLineEdit::test2()
{
	int argc = 1;
	char *argv[] = { (char *)"gongcommontests", 0 };
	GuiApplication app("","","", argc, argv);
	LineEdit *le = new LineEdit(0, "", Variant::tDouble );
	le->setMask("###,###,##0.00000");
	le->setFormat("%$");
	le->setText(0);
	_GONG_DEBUG_ASSERT(le->toDouble() == 0.0 );
}


void TestLineEdit::test1()
{
	int argc = 1;
	char *argv[] = { (char *)"gongcommontests", 0 };
	GuiApplication app("","","", argc, argv);
	app.getRegConfig()->setLocale( "es_ES.utf8");
	_GONG_DEBUG_PRINT(0, app.getRegConfig()->getDateFormat() );
	_GONG_DEBUG_PRINT(0, app.getRegConfig()->getDateMask() );
#if 0
	LineEdit *le = new LineEdit(0, "lineeditmoney");
	le->setValueType( Variant::tMoney );
//	le->setText( "01/02/2007 10:23");
	le->setText( "1.23" );
	le->show();
	app.setMainWidget(le);
	app.exec();
#endif

	FrmBaseForTesting *f = new FrmBaseForTesting(0, "FrmBase test");
    QVBoxLayout *pMainLayout = new QVBoxLayout( f, 0, 6, "pMainLayout");

	QHBoxLayout *pButtonsLayout = new QHBoxLayout( 0, 4, 6, "pButtonsLayout");
	QSpacerItem *spacer1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	pButtonsLayout->addItem( spacer1 );
	pButtonsLayout->addWidget( f->getPushCancelar() );
	pButtonsLayout->addWidget( f->getPushAceptar() );

	QVBoxLayout *vl = new QVBoxLayout( 0, 1, 6, "pFormLayout");

	LineEdit *le = new LineEdit(f, "lineedit");
	le->setValueType( Variant::tDouble );
	le->setAlignment( Qt::AlignRight );
	le->setMask( "###,###,##0.00" );
	le->setFormat( "" );
	le->setMaxLength( 28 );
	le->setText( -1.23 );
	vl->addWidget(le);

/*
	QLineEdit *qle = new QLineEdit(f, "qlineedit" );
	qle->setText(QString::fromUtf8("Atención") );
	vl->addWidget(qle);
*/

	LineEdit *qleint = new LineEdit(f, "qlineeditint" );
	qleint->setValueType( Variant::tInt );
	qleint->setMask( "######" );
//	qleint->setMaxLength(6);
    qleint->setAlignment(Qt::AlignRight);
	qleint->setText(0);
	qleint->setFocus();
	vl->addWidget(qleint);

	LineEdit *qledate = new LineEdit(f, "qlineeditint" );
	qledate->setValueType( Variant::tDate );
	qledate->setMask( "dd/mm/yyyy" );
	qledate->setFormat( "%x" );
//	qledate->setMaxLength(10);
	qledate->setText(Date(2012,4,14));
//	qledate->setFocus();
	vl->addWidget(qledate);

	pMainLayout->addLayout( vl );
    pMainLayout->addLayout( pButtonsLayout );
	f->show();
	app.setMainWidget(f);
	app.exec();
}

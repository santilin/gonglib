#ifndef TESTCUSTOMFORM_H
#define TESTCUSTOMFORM_H

#include "config.h"
#ifdef HAVE_DBAPPMODULE
#include <dbappfrmcustom.h>

using namespace gong;

class FrmCustomForTesting: public FrmCustom
{
public:
    FrmCustomForTesting(QWidget *parent=0,
                        const char *name = 0, WidgetFlags f=0);
	void test();
	QPushButton *getPushAceptar() const { return pushAccept; }
	QPushButton *getPushCancelar() const { return pushCancel; }
private:
	CheckBox *cb1;
	ComboBox<Xtring> *combo1;
	GroupBox *gb1, *gb2;
};


class TestFrmCustom
{
public:
    TestFrmCustom();
    ~TestFrmCustom();

	void run();
};

#endif
#endif
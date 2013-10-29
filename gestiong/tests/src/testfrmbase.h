
#ifndef TESTFRMBASE_H
#define TESTFRMBASE_H

#include <gongfrmbase.h>

using namespace gong;

class FrmBaseForTesting: public FrmBase
{
public:
    FrmBaseForTesting(QWidget *parent=0, const gong::Xtring &name = gong::Xtring(), WidgetFlags f=0)
		: FrmBase(parent, name, f) {}
	QPushButton *getPushAceptar() const { return pushAccept; }
	QPushButton *getPushCancelar() const { return pushCancel; }
};


class TestFrmBase{
public:
    TestFrmBase();
    ~TestFrmBase();
	void run();

};

#endif

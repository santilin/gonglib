#ifndef _TPV_FRMCUSTOM_H
#define _TPV_FRMCUSTOM_H

#include <dbappfrmcustom.h>

namespace gong {
namespace tpv {
	
class TpvFrmCustom: public FrmCustom
{
public:
	TpvFrmCustom(QWidget *parent, const char *name, WidgetFlags f)
		: FrmCustom(parent, name, f)
		{}
 
// 	void paintEvent(QPaintEvent* event);
};

} // namespace tpv
} // namespace gong

#endif // _TPV_FRMCUSTOM_H
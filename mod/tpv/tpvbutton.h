#ifndef _TPV_TPVBUTTON_H
#define _TPV_TPVBUTTON_H

#include <QToolButton>
#include <gongxtring.h>

namespace gong {
namespace tpv {

class TPVButton : public QToolButton
{
public:
	TPVButton(const Xtring &text, QWidget *parent = 0);
	virtual QSize sizeHint() const; // from QWidget
};

} // namespace tpv
} // namespace gong

# endif // _TPV_TPVBUTTON_H

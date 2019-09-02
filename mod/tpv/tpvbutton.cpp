#include <gongguinamespace.h>
#include "tpvbutton.h"

namespace gong {
namespace tpv {

TPVButton::TPVButton(const Xtring &text, QWidget *parent)
	: QToolButton(parent)
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	setText( toGUI( text ) );
}

QSize TPVButton::sizeHint() const
{
	QSize size = QToolButton::sizeHint();
	size.rheight() += 20;
	size.rwidth() = qMax(size.width(), size.height());
	return size;
}

} // namespace tpv
} // namespace gong

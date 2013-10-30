#include "gongpushbutton.h"
#include "gongguicontrols.h"

namespace gong {

PushButton::PushButton(QWidget* parent, const Xtring &text)
    : QPushButton( toGUI( text ), parent)
{
}


} // namespace gong


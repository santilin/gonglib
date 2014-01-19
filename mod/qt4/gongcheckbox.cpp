#include <QKeyEvent>
#include "gongguiapplication.h"
#include "gongcheckbox.h"

namespace gong {

CheckBox::CheckBox( QWidget *parent, const Xtring &name, const Xtring &caption )
    : QCheckBox( parent, name.c_str() ), mMustBeReadOnly(false), mEdited(false),
      mJustEdited(false), mCheckingProgrammatically( false )
{
    setText( toGUI(caption.c_str()) );
}

void CheckBox::keyPressEvent( QKeyEvent *e )
{
	QKeyEvent *tabevent;
    if ( e->state() == 0 || ( e->state() & Qt::KeypadModifier && e->key() == Qt::Key_Enter ) )
        switch ( e->key() ) {
			case Qt::Key_Return:
			case Qt::Key_Enter:
				e->ignore();
				tabevent = new QKeyEvent( e->type(), Qt::Key_Tab,
													'\t', e->state(), "\t",
													e->isAutoRepeat(), e->count() );
				GuiApplication::sendEvent( this, tabevent );
				return;
				// Don't break
			case Qt::Key_Space:
			{
				mJustEdited = true;
				toggle();
				mJustEdited = false;
				mEdited = true;
				// focusNextPrevChild(true); No funciona
				// See QApplication::sendEvent in the qt documentation
				tabevent = new QKeyEvent( e->type(), Qt::Key_Tab,
													'\t', e->state(), "\t",
													e->isAutoRepeat(), e->count() );
				GuiApplication::sendEvent( this, tabevent );
				return;
			}
        }
    QCheckBox::keyPressEvent( e );
}

void CheckBox::nextCheckState()
{
    mEdited = true;
    mJustEdited = true;
    QCheckBox::nextCheckState();
    mJustEdited = false;
}

void CheckBox::setChecked(bool checked)
{
    mCheckingProgrammatically = true;
    QCheckBox::setChecked( checked );
    mCheckingProgrammatically= false;
}



} // namespace gong

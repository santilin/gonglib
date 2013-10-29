#include "gongguiapplication.h"
#include "gongtextedit.h"
#include <qt4/QtCore/qnamespace.h>
#include <QEvent>
#include <QKeyEvent>
#include <QFocusEvent>

namespace gong {

int TextEdit::mGlobalValidating = 0;
int TextEdit::mLastGlobalValidating = 0;

TextEdit::TextEdit(QWidget *parent)
    : QPlainTextEdit(parent), mChanged( false )
{
    mValidating = false;
    mGettingFocus = false;
    connect( this, SIGNAL( textChanged() ), this, SLOT( slotTextChanged() ) );
}

void TextEdit::slotTextChanged()
{
    mChanged = true;
}

/* Redefinida para permitir salir del campo con el tabulador y con otras
  teclas */
void TextEdit::keyPressEvent( QKeyEvent *e )
{
    if ( e->state() == 0 || ( e->state() & Qt::KeypadModifier && e->key() == Qt::Key_Enter ) )
        switch ( e->key() )
        {
        case Qt::Key_Tab:
            focusNextPrevChild(true);
            e->ignore();
            return;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if( !isEdited() ) {
                theGuiApp->sendKeyEvent( this, Qt::Key_Tab, '\t', 0 );
                e->ignore();
                return;
            }
            break;
        case Qt::Key_Down:
        {
#ifdef TODOLATER
            QTextCursor bla bla bla
            int para, index;
            getCursorPosition(&para, &index);
            // comprobar si es la ultima linea
            if( para == paragraphs()-1 && paragraphLength(para) == 0 )
            {
                theGuiApp->sendKeyEvent( this, Qt::Key_Tab, '\t', 0 );
                e->ignore();
                return;
            }
#endif
        }
        break;
        case Qt::Key_Up:
        {
#ifdef TODOLATER
            int para, index;
            getCursorPosition(&para, &index);
            if( para == 0  && index == 0)
            {
                theGuiApp->sendKeyEvent( this, Qt::Key_Backtab, '\t', 0 );
                e->ignore();
                return;
            }
#endif
        }
        break;
        }
    if( e->key() != Qt::Key_F12 && e->text().length() != 0 )
        mJustEdited = true;
    QPlainTextEdit::keyPressEvent(e);
}


void TextEdit::setReadOnly(bool readonly)
{
    if( readonly )
    {
        QPalette pal = palette();
        QColor inactivecolor = pal.color(QPalette::Active, QColorGroup::Button);
        pal.setColor(QPalette::Active, QColorGroup::Base, inactivecolor);
        setPalette(pal);
        setFocusPolicy(Qt::ClickFocus);
    }
    else
    {
        unsetPalette();
        setFocusPolicy(Qt::StrongFocus);
    }
    update();
    QPlainTextEdit::setReadOnly(readonly);
}


void TextEdit::focusOutEvent( QFocusEvent *e)
{
    QPlainTextEdit::focusOutEvent(e);
    if( e->reason() != Qt::MouseFocusReason
            && e->reason() != Qt::TabFocusReason
            && e->reason() != Qt::BacktabFocusReason )
    {
        return;
    }
    if( isReadOnly() )
    {
        return;
    }
    if( mGlobalValidating == mLastGlobalValidating && mGlobalValidating )
    {
        return;
    }
    if( mGettingFocus )
    {
        return;
    }
    if( mValidating )
        return;
    bool isvalid = true;
    mValidating = true;
    mLastGlobalValidating = mGlobalValidating;
    mGlobalValidating++;
    emit validate(this, &isvalid);
    if( !isvalid )
    {
        mGettingFocus = true;
        //    parentWidget()->setFocus();  // Tab changed
        setFocus();
        mGettingFocus = false;
    }
    mLastGlobalValidating = mGlobalValidating;
    mGlobalValidating--;
    mValidating = false;
}

void TextEdit::focusInEvent( QFocusEvent *e)
{
    QPlainTextEdit::focusInEvent(e);
    if( isReadOnly() && (e->reason() == Qt::TabFocusReason) )
    {
        QKeyEvent *tabevent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, '\t', 0);
        QApplication::sendEvent( this, tabevent );
    }
    else if( isReadOnly() && (e->reason() == Qt::BacktabFocusReason) )
    {
        QKeyEvent *backtabevent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backtab, '\t', 0);
        QApplication::sendEvent( this, backtabevent );
    }

}

Xtring TextEdit::toString() const
{
    return fromGUI(toPlainText());
}

void TextEdit::setText(const Xtring &text)
{
    setText( toGUI( text.c_str() ) );
}

void TextEdit::setText(const QString &text)
{
    QPlainTextEdit::setPlainText( text );
    mChanged = mJustEdited = false;
}

bool TextEdit::isEdited() const
{
    return document()->isModified();
}

void TextEdit::setEdited( bool edited )
{
    document()->setModified( edited );
}

QSize TextEdit::sizeHint() const
{
    QSize s = QPlainTextEdit::sizeHint();
    s.setWidth( fontMetrics().width('x') * 80 );
    s.setHeight(fontMetrics().height() * 5 );
    return s;
}

bool TextEdit::isJustEdited() const
{
    return mJustEdited;
}

void TextEdit::setJustEdited(bool justedited)
{
    mJustEdited = justedited;
}


}; // namespace gong

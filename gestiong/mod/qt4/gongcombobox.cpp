#include <gonglist.h>
#include "gongguiapplication.h"
#include "gongcombobox.h"
#include <QKeyEvent>

namespace gong {

template<> void
ComboBox<Xtring>::setText(const Xtring& caption)
{
    bool found = false;
    if( mCaptions.contains( caption ) ) {
        QComboBox::setCurrentText( toGUI(caption) );
        found = true;
    } else if( mValues.contains( caption ) ) {
        setCurrentItemByValue( caption );
        found = true;
    } else if( caption.isEmpty() ) {
        for( int index=0; index < count() && !found; index ++ ) {
            if( text( index ) == "" )
                found = true;
        }
    }
    if( !found ) {
        _GONG_DEBUG_WARNING( "Attempt to set caption '" + caption + "' in combobox with captions: "
                             + mCaptions.join(",") );
    }
}

template<> void
ComboBox<int>::setText(const Xtring& caption)
{
    bool found = false;
    if( mCaptions.contains( caption ) ) {
        QComboBox::setCurrentText( toGUI(caption) );
        found = true;
    } else if( mValues.contains( caption.toInt() ) ) {
        setCurrentItemByValue( caption.toInt() );
        found = true;
    } else if( caption.isEmpty() ) {
        for( int index=0; index < count() && !found; index ++ ) {
            if( text( index ) == "" )
                found = true;
        }
    }
    if( !found ) {
        _GONG_DEBUG_WARNING( "Attempt to set caption '" + caption + "' in combobox with captions: "
                             + mCaptions.join(",") );
    }
}

template<> void
ComboBox<Xtring>::setVisible( bool visible )
{
    QComboBox::setVisible( visible );
    pLabel->setVisible( visible );
}

template<> void
ComboBox<int>::setVisible( bool visible )
{
    QComboBox::setVisible( visible );
    pLabel->setVisible( visible );
}


template<>
void ComboBox<int>::keyPressEvent( QKeyEvent *e )
{
    if ( e->state() == 0 || ( e->state() & Qt::KeypadModifier && e->key() == Qt::Key_Enter ) )
        switch ( e->key() ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            // focusNextPrevChild(true); No funciona
            // See QApplication::sendEvent in the qt documentation
            theGuiApp->sendKeyEvent( this, Qt::Key_Tab, '\t', e->state() );
            return ;
        }
        }
    QComboBox::keyPressEvent( e );
}

template<>
void ComboBox<Xtring>::keyPressEvent( QKeyEvent *e )
{
    if ( e->state() == 0 || ( e->state() & Qt::KeypadModifier && e->key() == Qt::Key_Enter ) )
        switch ( e->key() ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            // focusNextPrevChild(true); No funciona
            // See QApplication::sendEvent in the qt documentation
            theGuiApp->sendKeyEvent( this, Qt::Key_Tab, '\t', e->state() );
            return ;
        }
        }
    QComboBox::keyPressEvent( e );
}

template <>
const int &ComboBox<int>::getItemValue(int i) const
{
    if( i < 0 )
        return mNullValue;
    if( mValues.size() ) {
        if( i < (int)mValues.size() )
            return mValues[i];
        else
            return mNullValue;
    }
    return mNullValue;
}

}

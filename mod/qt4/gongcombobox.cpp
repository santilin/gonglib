#include <gonglist.h>
#include "gongguiapplication.h"
#include "gongcombobox.h"
#include <QKeyEvent>

namespace gong {

ComboBoxInt::ComboBoxInt( const XtringList &captions, const IntList &values,
						QWidget *parent, const Xtring &name, const Xtring &caption, bool horizontal )
    : QComboBox(parent, name.c_str()), mRefCaptions( captions ), mRefValues( values ),
		mHorizontal(horizontal), mMustBeReadOnly(false),
      mEdited(false), mJustEdited( false ), mSettingProgrammatically( false ), mIsRef( true )
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    insertItems();
    if( mHorizontal )
        pLayout = (QBoxLayout *)new QHBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    else
        pLayout = (QBoxLayout *)new QVBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    pLabel = new QLabel( parent, ("label_" + name).c_str() );
    pLabel->setText( toGUI(caption) );
    pLayout->addWidget( pLabel );
    pLayout->addWidget( this );
}

ComboBoxInt::ComboBoxInt( XtringList &captions, IntList &values,
					QWidget *parent, const Xtring &name, const Xtring &caption, bool horizontal )
    : QComboBox(parent, name.c_str()), mCaptions(captions), mValues(values),
    mRefCaptions(mCaptions), mRefValues(mValues),
    mHorizontal(horizontal), mMustBeReadOnly(false),
    mEdited(false), mJustEdited( false ), mSettingProgrammatically( false ), mIsRef( false )
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    insertItems();
    if( mHorizontal )
        pLayout = (QBoxLayout *)new QHBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    else
        pLayout = (QBoxLayout *)new QVBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    pLabel = new QLabel( parent, ("label_" + name).c_str() );
    pLabel->setText( toGUI(caption) );
    pLayout->addWidget( pLabel );
    pLayout->addWidget( this );
}

void ComboBoxInt::setCurrentItemByValue(int value)
{
    mSettingProgrammatically = true;
    for( unsigned int i=0; i<mRefValues.size(); i++ )
        if( mRefValues[i] == value ) {
            setCurrentItem( i );
            mSettingProgrammatically = false;
            return;
        }
    setCurrentItem(-1);
    mSettingProgrammatically = false;
}

bool ComboBoxInt::insertItems()
{
    bool oneempty = false;
    int i = 0;
    for (XtringList::const_iterator itcaptions = mRefCaptions.begin();
            itcaptions != mRefCaptions.end(); itcaptions++ ) {
        QComboBox::insertItem( toGUI( *itcaptions ) );
        if( (*itcaptions).isEmpty() )
            oneempty = true;
		if( !mIsRef ) {
			if( mValues.size() <= (uint)i )
				mValues.push_back( i );
		}
        i++;
    }
    return oneempty;
}

int ComboBoxInt::getItemValue(int i) const
{
	if( i == -1 )
		return 0;
	if( mRefValues.size() == 0 )
		return i;
	if( i<(int)mRefValues.size() )
		return mRefValues[ i ];
	else
		return 0;
}

void ComboBoxInt::insertItem(const Xtring& caption, int value, int index)
{
	if( !mIsRef ) {
		if( index == -1 || (uint)index >= mValues.size() ) {
			mCaptions << caption;
			mValues << value;
		} else {
			mCaptions.insert( mCaptions.begin() + index, caption );
			mValues.insert( mValues.begin() + index, value);
		}
	}
	QComboBox::insertItem( toGUI(caption), index );
}

bool ComboBoxInt::isNewItem() const
{
	if( currentText() == text( currentItem() ) )
		return false;
	else {
		for( int index=0; index < count(); index ++ ) {
			if( text( index ) == currentText() )
				return false;
		}
	}
	return true;
}

void ComboBoxInt::setText(const Xtring& caption)
{
    bool found = false;
    if( mRefCaptions.contains( caption ) ) {
        QComboBox::setCurrentText( toGUI(caption) );
        found = true;
    } else if( mRefValues.contains( caption.toInt() ) ) {
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
                             + mRefCaptions.join(",") );
    }
}

void ComboBoxInt::setVisible( bool visible )
{
    QComboBox::setVisible( visible );
    pLabel->setVisible( visible );
}

void ComboBoxInt::keyPressEvent( QKeyEvent *e )
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

///////////// COMBOBOXXTRING /////////////////

ComboBoxXtring::ComboBoxXtring( const XtringList &captions, const XtringList &values,
						QWidget *parent, const Xtring &name, const Xtring &caption, bool horizontal )
    : QComboBox(parent, name.c_str()), mRefCaptions( captions ), mRefValues( values ),
		mHorizontal(horizontal), mMustBeReadOnly(false),
      mEdited(false), mJustEdited( false ), mSettingProgrammatically( false ), mIsRef( true )
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    insertItems();
    if( mHorizontal )
        pLayout = (QBoxLayout *)new QHBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    else
        pLayout = (QBoxLayout *)new QVBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    pLabel = new QLabel( parent, ("label_" + name).c_str() );
    pLabel->setText( toGUI(caption) );
    pLayout->addWidget( pLabel );
    pLayout->addWidget( this );
}

ComboBoxXtring::ComboBoxXtring( XtringList &captions, XtringList &values,
					QWidget *parent, const Xtring &name, const Xtring &caption, bool horizontal )
    : QComboBox(parent, name.c_str()), mCaptions(captions), mValues(values),
    mRefCaptions(mCaptions), mRefValues(mValues),
    mHorizontal(horizontal), mMustBeReadOnly(false),
    mEdited(false), mJustEdited( false ), mSettingProgrammatically( false ), mIsRef( false )
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    insertItems();
    if( mHorizontal )
        pLayout = (QBoxLayout *)new QHBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    else
        pLayout = (QBoxLayout *)new QVBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    pLabel = new QLabel( parent, ("label_" + name).c_str() );
    pLabel->setText( toGUI(caption) );
    pLayout->addWidget( pLabel );
    pLayout->addWidget( this );
}

void ComboBoxXtring::setCurrentItemByValue(const Xtring &value)
{
    mSettingProgrammatically = true;
    for( unsigned int i=0; i<mRefValues.size(); i++ )
        if( mRefValues[i] == value ) {
            setCurrentItem( i );
            mSettingProgrammatically = false;
            return;
        }
    setCurrentItem(-1);
    mSettingProgrammatically = false;
}

bool ComboBoxXtring::insertItems()
{
	bool oneempty = false;
	for (XtringList::const_iterator itcaptions = mRefCaptions.begin();
			itcaptions != mRefCaptions.end(); itcaptions++ ) {
		QComboBox::insertItem( toGUI( *itcaptions ) );
		if( (*itcaptions).isEmpty() )
			oneempty = true;
	}
	return oneempty;
}

void ComboBoxXtring::insertItem(const Xtring& caption, const Xtring &value, int index)
{
	if( !mIsRef ) {
		if( index == -1 || (uint)index >= mValues.size() ) {
			mCaptions << caption;
			mValues << value;
		} else {
			mCaptions.insert( mCaptions.begin() + index, caption );
			mValues.insert( mValues.begin() + index, value);
		}
	}
	QComboBox::insertItem( toGUI(caption), index );
}


const Xtring &ComboBoxXtring::getItemValue(int i) const
{
	if( i != -1 && i<(int)mRefValues.size() )
		return mRefValues[ i ];
	else
		return Xtring::null;
}

bool ComboBoxXtring::isNewItem() const
{
	if( currentText() == text( currentItem() ) )
		return false;
	else {
		for( int index=0; index < count(); index ++ ) {
			if( text( index ) == currentText() )
				return false;
		}
	}
	return true;
}

void ComboBoxXtring::setText(const Xtring& caption)
{
    bool found = false;
    if( mRefCaptions.contains( caption ) ) {
        QComboBox::setCurrentText( toGUI(caption) );
        found = true;
    } else if( mRefValues.contains( caption ) ) {
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


void ComboBoxXtring::setVisible( bool visible )
{
    QComboBox::setVisible( visible );
    pLabel->setVisible( visible );
}


void ComboBoxXtring::keyPressEvent( QKeyEvent *e )
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

}

#ifndef _GONG_COMBOBOX_H
#define _GONG_COMBOBOX_H

/** @file gongcombobox.h Qt based ComboBox
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <gongguiapplication.h>
#include <gongdbfielddefinition.h>

namespace gong {

template<class ValueT>
class ComboBox: public QComboBox
{
private:
    QLabel *pLabel;
    QBoxLayout *pLayout;
    bool mHorizontal, mMustBeReadOnly, mEdited;
public:
    ComboBox( XtringList &captions, List<ValueT> &values,
              QWidget *parent, const Xtring &name, const Xtring &caption,
              bool horizontal = true );
    void setVisible( bool visible );
    void setText(const Xtring &caption);
    void setCurrentItemByValue(const ValueT &value);
    const ValueT &getCurrentItemValue() const {
        return getItemValue( currentItem() );
    }
    const ValueT &getItemValue(int i) const {
        if( i != -1 && i<(int)mValues.size()) return mValues[ i ];
        else return mNullValue;
    }
    bool isEdited() const {
        return mEdited;
    }
    void setEdited(bool edited) {
        mEdited=edited;
    }
    bool isJustEdited() const {
        return mJustEdited;
    }
    void setJustEdited( bool justedited = true ) {
        mJustEdited = justedited;
    }
    bool insertItems() {
        bool oneempty = false;
        for (XtringList::const_iterator itcaptions = mCaptions.begin();
                itcaptions != mCaptions.end(); itcaptions++ ) {
            QComboBox::insertItem( toGUI( *itcaptions ) );
            if( (*itcaptions).isEmpty() )
                oneempty = true;
        }
        return oneempty;
    }
    void insertItem(const Xtring &caption, const ValueT &value, int index = -1) {
        if( index == -1 || (uint)index >= mValues.size() ) {
            mCaptions << caption;
            mValues << value;
        }
        else {
            mCaptions.insert( mCaptions.begin() + index, caption );
            mValues.insert( mValues.begin() + index, value);
        }
        QComboBox::insertItem( QString::fromUtf8((caption).c_str()), index );
    }
    QBoxLayout *getLayout() const {
        return pLayout;
    }
    QLabel *getLabel() const {
        return pLabel;
    }
    bool mustBeReadOnly() const {
        return mMustBeReadOnly;
    }
    void setMustBeReadOnly(bool must) {
        mMustBeReadOnly = must;
    }
    bool isSettingProgrammatically() const {
        return mSettingProgrammatically;
    }
    Xtring toString() const {
        return fromGUI(currentText());
    }
    bool isNewItem() const {
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
    // Overloaded
    Xtring currentString() const {
        return fromGUI( currentText() );
    }

protected:
    virtual void keyPressEvent( QKeyEvent *e ); // from QWidget

private:
    XtringList mCaptions;
    List<ValueT> mValues;
    ValueT mNullValue;
    bool mJustEdited, mSettingProgrammatically;
};


template<class ValueT>
ComboBox<ValueT>::ComboBox( XtringList &captions, List<ValueT> &values,
                            QWidget *parent, const Xtring &name, const Xtring &caption, bool horizontal )
    : QComboBox(parent, name.c_str()), mHorizontal(horizontal), mMustBeReadOnly(false),
      mEdited(false), mCaptions(captions), mValues(values), mNullValue( ValueT() ),
      mJustEdited( false ), mSettingProgrammatically( false )
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

template<class ValueT>
void ComboBox<ValueT>::setCurrentItemByValue(const ValueT &value)
{
    mSettingProgrammatically = true;
    for( unsigned int i=0; i<mValues.size(); i++ )
        if( mValues[i] == value ) {
            setCurrentItem( i );
            mSettingProgrammatically = false;
            return;
        }
    setCurrentItem(-1);
    mSettingProgrammatically = false;
}

template<>
inline bool ComboBox<int>::insertItems()
{
    bool oneempty = false;
    int i = 0;
    for (XtringList::const_iterator itcaptions = mCaptions.begin();
            itcaptions != mCaptions.end(); itcaptions++ ) {
        QComboBox::insertItem( toGUI( *itcaptions ) );
        if( (*itcaptions).isEmpty() )
            oneempty = true;
        if( mValues.size() <= (uint)i )
            mValues.push_back( i );
        i++;
    }
    return oneempty;
}

} // namespace gong

#endif // _GONG_COMBOBOX_H

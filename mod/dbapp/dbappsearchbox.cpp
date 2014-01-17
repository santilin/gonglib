#include <QBoxLayout>
#include <gongdebug.h>
#include <gongfrmbase.h>
#include "dbappdbapplication.h"
#include "dbappsearchbox.h"

namespace gong {

SearchBox::SearchBox( const Xtring &caption, const Xtring &tablename,
                      const Xtring &fldcode, const Xtring &flddesc,
                      QWidget *parent, enum Flags flags )
    : QPushButton( parent, ("push_" + tablename + "_id_" + fldcode ).c_str() ),
      mFlags(flags), mMustBeReadOnly(false),
      mTableName( tablename ), mFldCode( fldcode ), mFldDesc( flddesc )
{
    setIcon( QIcon::fromTheme("edit-find", QIcon(":/edit-find.png")) );
    pLayout = (QBoxLayout *)new QHBoxLayout( 0, 0, 6, ("layout_" + mTableName).c_str() );
    pLayout->addWidget( this );
    if( mFlags & FlagShowLabels ) {
        pLabelCode = new QLabel( parent, ("label_" + mTableName + "_id_" + mFldCode ).c_str() );
        pLayout->addWidget( pLabelCode );
    } else {
        pLabelCode = 0;
    }
    pEditCode = new LineEdit( parent, ("edit_" + mTableName + "_id_" + mFldCode ).c_str() );
    pLayout->addWidget( pEditCode );
    if( mFlags & FlagShowLabels ) {
        pLabelDesc = new QLabel( parent, ("label_" + mTableName + "_id_" + mFldCode ).c_str() );
        pLayout->addWidget( pLabelDesc );
    } else {
        pLabelDesc = 0;
    }
    pEditDesc = new LineEdit( parent, ("edit" + mTableName + "_id_" + mFldDesc ).c_str() );
    if( !(mFlags & FlagEditableDesc ) )
        pEditDesc->setReadOnly(true);
    pLayout->addWidget( pEditDesc );
    setFocusPolicy( Qt::ClickFocus );
    pRecord = DBAPP->createRecord( mTableName );
}

SearchBox::~SearchBox()
{
    delete pEditCode;
    delete pEditDesc;
    if( pRecord )
        delete pRecord;
}

void SearchBox::setValue(const Variant& value)
{
    pEditCode->setText( value );
    pEditCode->setJustEdited( true );
    bool b;
    pEditCode->emitValidate( &b );
}


Variant SearchBox::getValue() const
{
    return pEditCode->toVariant();
}

void SearchBox::slotClicked()
{
    bool choose_ret;
    if( isMultiple() ) {
        choose_ret = DBAPP->chooseMulti( mRecordIDs, static_cast<FrmEditRec*>(0),
                                         pRecord, 0, dbApplication::editNone, this );
        if( choose_ret ) {
            if( mRecordIDs.size() == 1 ) {
                pEditCode->setText( pRecord->getValue( mFldCode ).toString() );
                pEditDesc->setText( pRecord->getValue( mFldDesc ).toString() );
            } else {
                pEditCode->setText( "" );
                pEditDesc->setText( Xtring::printf(_("%d %s seleccionados"), mRecordIDs.size(),
                                                   pRecord->getTableDefinition()->getDescPlural().c_str() ) );
            }
        }
    } else {
        choose_ret = DBAPP->choose( static_cast<FrmEditRec*>(0),
                                    pRecord, 0, dbApplication::editNone, this );
        if( choose_ret ) {
            pEditCode->setText( pRecord->getValue( mFldCode ).toString() );
            pEditDesc->setText( pRecord->getValue( mFldDesc ).toString() );
        }
    }
    if( !choose_ret ) {
        mRecordIDs.clear();
        pEditCode->setText( "" );
        pEditDesc->setText( "" );
    }
}

void SearchBox::slotValidate(QWidget *sender , bool *isvalid )
{
    if( sender == pEditDesc  )
        return;
    if( !pEditCode->isJustEdited() )
        return;
    bool ret = false;
    Xtring codigo_string = pEditCode->toString().trim();
    if( ( (codigo_string.isEmpty() || codigo_string == "0") ) ) {
        mRecordIDs.clear();
        pEditCode->setText( "" );
        pEditDesc->setText( "" );
        ret = true;
    } else {
        Xtring descripcion;
        descripcion = pEditDesc->toString();
        QWidget *fw = focusWidget(); // El siguiente control, para volver a él si todo va bien
        dbRecordID recid = 0;
        if ( pEditCode->getValueType() == Variant::tInt ) {  // Son dos funciones distintas
            int codigo_int = pEditCode->toInt();
            recid = DBAPP->seekCode( pRecord, this, mFldCode, codigo_int, mFldDesc,
                                     codigo_int == 0 ? descripcion : Xtring::null, Xtring::null,
                                     isMultiple() ? dbApplication::SeekCodeMultiple : dbApplication::SeekCodeNone );
        } else {
            recid = DBAPP->seekCode( pRecord, this, mFldCode, codigo_string, mFldDesc,
                                     codigo_string.isEmpty() ? descripcion : Xtring::null, Xtring::null,
                                     isMultiple() ? dbApplication::SeekCodeMultiple : dbApplication::SeekCodeNone );
        }
        if( recid ) {
            ret = true;
            mRecordIDs = DBAPP->getSeekCodeRecordIDs();
            if( mRecordIDs.size() > 1 ) {
                pEditCode->setText( "" );
                pEditDesc->setText( Xtring::printf( _("%d %s elegidos"),
                                                    mRecordIDs.size(),
                                                    DBAPP->getTableDescPlural( pRecord->getTableName(), "" ).c_str() ) );
            } else {
                mRecordIDs.clear();
                mRecordIDs.push_back( recid );
                pEditCode->setText( pRecord->getValue( mFldCode ) );
                pEditDesc->setText( pRecord->getValue( mFldDesc ) );
            }
            if( fw )
                fw->setFocus();
        } else {
            mRecordIDs.clear();
            FrmBase::msgWarning( 0, Xtring::printf( _("No se ha encontrado %1$s con código \"%2$s\""),
                                                    DBAPP->getTableDescSingular(mTableName, "la" ).c_str(), codigo_string.c_str() ) );
            pEditCode->setText("");
            if( pEditDesc )
                pEditDesc->setText("");
            ret = false;
        }
    }
    if( isvalid )
        *isvalid = ret;
}

void SearchBox::slotSpecialControlKeyPressed(QWidget* , char key)
{
    if( key == 'F' )
        slotClicked();
}

void SearchBox::setVisible(bool visible)
{
	QWidget::setVisible( visible );
	if( pLabelCode )
		pLabelCode->setVisible( visible );
	pEditCode->setVisible( visible );
	if( pLabelDesc )
		pLabelDesc->setVisible( visible );
	pEditDesc->setVisible( visible );
}


} // namespace

#include "dbappfrmeditnameslisttable.h"

namespace gong {

FrmEditNamesListTable::FrmEditNamesListTable(FrmEditRec* parentfrm, dbRecord* master,
        dbRecordDataModel* dm, EditMode editmode,
        dbApplication::EditFlags editflags,
        QWidget* parent, const char* name, WidgetFlags fl)
    : FrmEditRecMaster(parentfrm, master, dm, editmode, editflags, parent, name, fl)
{
    QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
    QHBoxLayout *nombreLayout = new QHBoxLayout(0, 0, 6, "nombreLayout");
    editCodigo = addEditField( pControlsFrame, "TIPOIVA", "Codigo", codigoLayout );
    editNombre = addEditField( pControlsFrame, "TIPOIVA", "Nombre", nombreLayout );
    pControlsLayout->addLayout( codigoLayout );
    pControlsLayout->addLayout( nombreLayout );
    QSpacerItem* spacerfactu = new QSpacerItem ( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pControlsLayout->addItem( spacerfactu );
}

void FrmEditNamesListTable::scatterFields()
{
    editCodigo->setText( getRecord()->getValue("CODIGO").toInt() );
    editNombre->setText( getRecord()->getValue("NOMBRE").toString() );
    if( isInserting() && editCodigo->toInt() == 0 ) {
        editCodigo->setText( getRecord()->selectNextInt( "CODIGO", Xtring::null, true ) );
    } else if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
        if( DBAPP->codeNotFound().toInt() != 0 ) {
            editCodigo->setText( DBAPP->codeNotFound() );
            editCodigo->setJustEdited( true );
        } else {
            editNombre->setText( DBAPP->codeNotFound() );
            editNombre->setJustEdited( true );
        }
    }
    if( pFocusWidget == 0 )
        pFocusWidget = editNombre;
}

void FrmEditNamesListTable::gatherFields()
{
    getRecord()->setValue( "CODIGO", editCodigo->toInt());
    getRecord()->setValue( "NOMBRE", editNombre->toString());
}

void FrmEditNamesListTable::validateFields(QWidget* sender, bool* isvalid, ValidResult *ve)
{
    bool v=true;
    if( !isvalid )
        isvalid = &v;
    ValidResult *validresult = ( ve ? ve : new ValidResult() );
    if( !sender ) {
        *isvalid = pRecord->validate( ValidResult::editing );
		validresult->append( pRecord->getErrors() );
	}
    if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "CODIGO", "NOMBRE" ) )
        if( !sender )
            *isvalid = false;
    if ( !ve ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }

}


} // namespace gong

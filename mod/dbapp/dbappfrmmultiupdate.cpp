#include "config.h"
#include <gonggettext.h>
#include "dbappdbapplication.h"
#include "dbappfrmeditrecmaster.h"
#include "dbappfrmmultiupdate.h"

namespace gong {

FrmMultiUpdate::FrmMultiUpdate(FrmEditRecMaster *theform,
                               QWidget* parent, WidgetFlags fl)
    : FrmCustom(0, "FrmMultiUpdate", fl), pTheForm( theform )
{
    setCaption( toGUI(_("Modificación múltiple")) );
    const dbTableDefinition *tbldef = theform->getRecord()->getTableDefinition();
    XtringList fld_captions;
    IntList fld_pos;
    for ( unsigned int i = 0; i < tbldef->getFieldCount(); i++ ) {
        dbFieldDefinition *fielddef = tbldef->getFieldDefinition ( i );
        if( fielddef->getName() == tbldef->getFldIDName()
                || fielddef->getName().find("_ID") !=  Xtring::npos )
            continue;
        if( pTheForm->findControl( fielddef->getName() ) ) {
            Xtring desc = fielddef->getCaption().isEmpty() ? fielddef->getDescription().isEmpty() ? fielddef->getName() : fielddef->getDescription() : fielddef->getCaption();
            fld_captions.push_back( desc ); // Permite duplicados, pues es el título
            fld_pos.push_back( i );
        }
    }
    pComboField = addComboBoxInt(false, 0, _("Campo a modificar"), fld_captions, fld_pos);
    pValor = addInput(0, _("Nuevo valor"), "", "STRING");
    XtringList rangeoptions;
    rangeoptions << _("Registro actual") << _("Registros seleccionados")
                 << _("Todo");
    int range;
    if( theform->getDataTable()->getSelectedIDs().size() > 1 ) {
        range = 1;
    } else if( !theform->getWholeFilter().isEmpty()
               && theform->getWholeFilter() != theform->getRecord()->getFilter("") ) {
        range = 2;
    } else {
        range = 0;
    }
    pRango = addGroupBox(0, rangeoptions, _("Rango"), range);
    pSupervisar = addCheckBox(0, _("Supervisar registros uno a uno"), true );
}

void FrmMultiUpdate::validate_input( QWidget *sender, bool *isvalid )
{
    FrmCustom::validate_input( sender, isvalid );
}

void FrmMultiUpdate::accept()
{
    List<dbRecordID> ids;
    Xtring where;
    switch( pRango->getSelectedPos() ) {
    case 0:
        ids << pTheForm->getTableRecordID();
        break;
    case 1:
        ids << pTheForm->getDataTable()->getSelectedIDs();
        break;
    case 2:
        FrmBase::msgError( pTheForm, _("Esto aún no funciona" ) );
        return;
    }
    dbRecord *record = pTheForm->getRecord()->duplicate();
    int nupdated = 0;
    for( List<dbRecordID>::const_iterator it = ids.begin(); it != ids.end(); ++ it ) {
        dbRecordID recid = *it;
        bool revisar = pSupervisar->isChecked();
        if( record->read( recid ) ) {
            record->setValue( pComboField->getCurrentItemValue(), pValor->toVariant() );
            record->readRelated( true );
            if( !record->isValid( ValidResult::fixing, 0 ) ) {
                DBAPP->showOSD( _("Modificar múltiple"), _("Este registro contiene errores.") );
                revisar = true;
            }
            if( revisar ) {
                FrmEditRecMaster *editfrm = static_cast<FrmEditRecMaster *>(
                                                DBAPP->createEditForm( 0, record, 0, DataTable::updating, dbApplication::simpleEdition ) );
                if ( editfrm ) {
                    editfrm->setFormFilter( pTheForm->getFormFilter() );  // Copy the form filter
// 					editfrm->setJustEditedAllControls( true );
                    editfrm->showModalFor( this, false, true );
                    if ( !editfrm->isSaved() ) {
                        delete editfrm;
                        List<dbRecordID>::const_iterator itact = it;
                        if( ++itact == ids.end() )
                            break; // Don't ask if this is the last one
                        if( !FrmBase::msgYesNo( this,
                                                _( "Has anulado la modificación de este registro.\n"
                                                   "¿Qieres continuar con los siguientes?" ) ) )  {
                            break;
                        }
                    } else {
                        delete editfrm;
                        nupdated ++;
                    }
                } else {
                    msgOk( this, _( "No se ha podido crear el formulario para modificación múltiple" ) );
                    break;
                }
            } else {
                DBAPP->waitCursor( true );
                try {
                    record->save( false ); // Don't update relations
                    nupdated ++;
                    if( nupdated % 5 == 0 )
                        DBAPP->showOSD( _("Modificando"), Xtring::printf(_("Modificados %d registros"), nupdated ) ); // TODO descplural
                } catch( dbError &e ) {
                    _GONG_DEBUG_WARNING( e.what() );
                    msgError( this, Xtring( _( "Error al guardar el registro:\n" )) + e.what() );
                }
                DBAPP->resetCursor();
            }
        }
    }
    delete record;
    if ( nupdated ) {
        msgOk( this, Xtring::printf( _( "Se han modificado %d registros" ), nupdated ) );
        pTheForm->refresh();
    }
}


} // namespace gong




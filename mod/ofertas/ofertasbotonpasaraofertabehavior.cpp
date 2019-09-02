//*<<<<<MODULE             _INFO*/
// COPYLEFT Behavior para añadir un botón para pasar a oferta un albarán o factura
// TYPE FrmEditRecBehavior ofertas::BotonPasarAOferta Q_OBJECT
/*>>>>>MODULE            _INFO*/

#include <gongpushbutton.h>
#include <dbappfrmeditrecmaster.h>
/*<<<<<BOTONPASARAOFERTABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
/*>>>>>BOTONPASARAOFERTABEHAVIOR_INCLUDES*/
#include "ofertasbotonpasaraofertabehavior.h"
#include "ofertasrecofertacompra.h"
#include "ofertasrecofertaventa.h"
#include "ofertasmodule.h"

namespace gong {
namespace ofertas {


void BotonPasarAOfertaBehavior::initGUI()
{
    /*<<<<<BOTONPASARAOFERTABEHAVIOR_INITGUI*/
//     QWidget *pControlsFrame = getControlsFrame();
//     QVBoxLayout* pControlsLayout = getControlsLayout();
    /*>>>>>BOTONPASARAOFERTABEHAVIOR_INITGUI*/
    pushPasarAOferta = new PushButton(pTheForm, "A oferta");
    getButtonsLayout()->addWidget( pushPasarAOferta );
    connect( pushPasarAOferta, SIGNAL( clicked() ), this, SLOT( pasarAOferta_clicked() ) );
}


void BotonPasarAOfertaBehavior::scatterFields( bool is_pre )
{
    if( !is_pre) return;
/*<<<<<BOTONPASARAOFERTABEHAVIOR_SCATTER*/

/*>>>>>BOTONPASARAOFERTABEHAVIOR_SCATTER*/
}

void BotonPasarAOfertaBehavior::gatherFields()
{
/*<<<<<BOTONPASARAOFERTABEHAVIOR_GATHER*/

/*>>>>>BOTONPASARAOFERTABEHAVIOR_GATHER*/
}

void BotonPasarAOfertaBehavior::validateFields(bool is_pre,
        QWidget *sender, bool *isvalid, ValidResult *ir)
{
    if( !is_pre) return;
    /*<<<<<BOTONPASARAOFERTABEHAVIOR_VALIDATE*/
    bool v=true;
    if( !isvalid )
        isvalid = &v;
    ValidResult *validresult = ( ir ? ir : new ValidResult() );
    /*>>>>>BOTONPASARAOFERTABEHAVIOR_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}


dbRecordList *BotonPasarAOfertaBehavior::getDetalles(dbRecord* record, uint pos)
{
    XtringList det_properties;
    det_properties << "DETALLE";
    dbRecordRelationDict sourcerelations = record->findRelationsBySemanticProperties( det_properties );
    if( pos <= sourcerelations.size() ) {
        return sourcerelations.seq_at(pos)->getRelatedRecordList();
    } else {
        throw std::runtime_error(Xtring::printf("No existen los detalles en pos=%d en la tabla %s", pos, record->getTableName().c_str()));
    }
}


void BotonPasarAOfertaBehavior::pasarAOferta_clicked()
{
    FrmEditRecMaster *frm = static_cast<FrmEditRecMaster *>(pTheForm);
    _GONG_DEBUG_ASSERT(frm);
    dbRecordID id = frm->getTableRecordID();
    if( id == 0 )
        return;
    Xtring tabla_origen = frm->getRecord()->getTableName();
    dbRecord *orig_record = DBAPP->createRecord(tabla_origen);
    if( !orig_record->read(id) ) {
        return;
	}
	DBAPP->waitCursor(true);
    dbRecordList *detalles = getDetalles(orig_record);
    bool escompra = orig_record->hasSemanticProperty("COMPRA");
    RecOfertaCompra *new_oferta_compra = 0;
    RecOfertaVenta *new_oferta_venta = 0;
    if( escompra ) {
        new_oferta_compra = static_cast<RecOfertaCompra *>( DBAPP->createRecord("OFERTACOMPRA") );
        new_oferta_compra->copyRecord( orig_record );
    } else {
        new_oferta_venta = static_cast<RecOfertaVenta *>( DBAPP->createRecord("OFERTAVENTA") );
        new_oferta_venta->copyRecord( orig_record );
    }
    for( dbRecordList::const_iterator it = detalles->begin(); it != detalles->end(); ++it ) {
        dbRecord *detalle = *it;
        if( escompra) {
            RecOfertaCompraDet *new_detalle = static_cast<RecOfertaCompraDet *>( DBAPP->createRecord("OFERTACOMPRADET") );
            new_detalle->copyRecord( detalle );
            new_detalle->setNew( true );
            new_oferta_compra->getListOfertaCompraDet()->addRecord( new_detalle );
        } else {
            RecOfertaVentaDet *new_detalle = static_cast<RecOfertaVentaDet *>( DBAPP->createRecord("OFERTAVENTADET") );
            new_detalle->copyRecord( detalle );
            new_detalle->setNew( true );
            new_oferta_venta->getListOfertaVentaDet()->addRecord( new_detalle );
        }
    }
    FrmEditRec *frmeditoferta = DBAPP->createEditForm(0, (escompra ? static_cast<dbRecord *>(new_oferta_compra) : static_cast<dbRecord *>(new_oferta_venta)),
                                0, DataTable::inserting, dbApplication::simpleEdition);
	DBAPP->resetCursor();
    if( !frmeditoferta->showAndSave() )
        frmeditoferta->showModalFor( 0, true, true );
    if( frmeditoferta->isSaved() ) {
        if (escompra) {
            ofertas::ModuleInstance->copiaCobros(orig_record, new_oferta_compra);
        } else {
            ofertas::ModuleInstance->copiaCobros(orig_record, new_oferta_venta);
        }
        Xtring rec_desc = DBAPP->getTableDescSingular( orig_record->getTableName(), "la" );
        if( FrmBase::msgYesNo("Copiar a oferta",
                              Xtring::printf("La oferta se ha guardado perfectamente.\n¿Quieres borrar %s original?", rec_desc.c_str())) ) {
            if( orig_record->remove() ) {
                DBAPP->showOSD(Xtring::printf("%s original se ha borrado correctamente", rec_desc.c_str()),"");
            } else {
                FrmBase::msgError(Xtring::printf("Error borrando %s original", rec_desc.c_str()), "");
            }
        }
    }
    delete frmeditoferta;
    delete new_oferta_compra;
	delete new_oferta_venta;
    delete orig_record;
}

void BotonPasarAOfertaBehavior::updateStatus()
{
    pushPasarAOferta->hide();
    FrmEditRecMaster *frm = static_cast<FrmEditRecMaster *>(pTheForm);
    _GONG_DEBUG_ASSERT(frm);
    if( !frm->isEditing() && frm->getDataTable()->getDataModel()->getRowCount()> 0) {
        pushPasarAOferta->show();
    }
}



/*<<<<<BOTONPASARAOFERTABEHAVIOR_FIN*/
} // namespace ofertas
} // namespace gong
/*>>>>>BOTONPASARAOFERTABEHAVIOR_FIN*/


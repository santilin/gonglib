/*<<<<<COPYLEFT*/
/** @file ventaswebfrmeditpedidoweb.cpp Fichero de edición de pedidos web
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de pedidos web
// FIELD Contador int - cabecera
// FIELD EstadoPedido comboint - cabecera
// FIELD Fecha date - cabecera
// FIELD Agente_ID Reference(factu::Agente,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera
// FIELD Referencia string - cliente
// FIELD Nombre string - cliente
// FIELD email string - cliente
// FIELD Direccion string - cliente3
// FIELD CP string - cliente3
// FIELD Localidad string - cliente3
// FIELD Provincia string - cliente3
// FIELD PedidoWebDet FrmEditRecDetail
// FIELD Total money - total
// FIELD Telefono text - notas
// FIELD Notas text - notas
// TYPE FrmEditRecMaster ventasweb::PedidoWeb Pedido Venta
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITPEDIDOWEB_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ventaswebfrmeditpedidoweb.h"
/*>>>>>FRMEDITPEDIDOWEB_INCLUDES*/
#include <empresamodule.h>
#include <ventaswebmodule.h>
#include <factufldivadetallado.h>

namespace gong {
namespace ventasweb {

/*<<<<<FRMEDITPEDIDOWEB_CONSTRUCTOR*/
FrmEditPedidoWeb::FrmEditPedidoWeb(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditPedidoWeb" );
/*>>>>>FRMEDITPEDIDOWEB_CONSTRUCTOR*/
    /*<<<<<FRMEDITPEDIDOWEB_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *clienteLayout = new QHBoxLayout(0, 0, 6, "clienteLayout");
	QHBoxLayout *cliente3Layout = new QHBoxLayout(0, 0, 6, "cliente3Layout");
	QHBoxLayout *pedidowebdetLayout = new QHBoxLayout(0, 0, 6, "pedidowebdetLayout");
	QHBoxLayout *totalLayout = new QHBoxLayout(0, 0, 6, "totalLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editContador = addEditField( pControlsFrame, "PEDIDOWEB", "CONTADOR", cabeceraLayout );
	comboEstadoPedido = addComboIntField( pControlsFrame, "PEDIDOWEB", "ESTADOPEDIDO", cabeceraLayout );
	editFecha = addEditField( pControlsFrame, "PEDIDOWEB", "FECHA", cabeceraLayout );

	searchAgenteCodigo = addSearchField( pControlsFrame, "AGENTE_ID", "AGENTE", "CODIGO", "RAZONSOCIAL", cabeceraLayout );
	pushAgenteCodigo = searchAgenteCodigo->getButton();
	connect( pushAgenteCodigo, SIGNAL( clicked() ), this, SLOT( pushAgenteCodigo_clicked() ) );
	editAgenteCodigo = searchAgenteCodigo->getEditCode();
	editAgenteRazonSocial = searchAgenteCodigo->getEditDesc();
	editReferencia = addEditField( pControlsFrame, "PEDIDOWEB", "REFERENCIA", clienteLayout );
	editNombre = addEditField( pControlsFrame, "PEDIDOWEB", "NOMBRE", clienteLayout );
	editemail = addEditField( pControlsFrame, "PEDIDOWEB", "EMAIL", clienteLayout );
	editDireccion = addEditField( pControlsFrame, "PEDIDOWEB", "DIRECCION", cliente3Layout );
	editCP = addEditField( pControlsFrame, "PEDIDOWEB", "CP", cliente3Layout );
	editLocalidad = addEditField( pControlsFrame, "PEDIDOWEB", "LOCALIDAD", cliente3Layout );
	editProvincia = addEditField( pControlsFrame, "PEDIDOWEB", "PROVINCIA", cliente3Layout );

// frmDetails: PedidoWebDet
	QFrame *pedidowebdetFrame = new QFrame(this);
	pedidowebdetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	pedidowebdetFrame->setLineWidth(4);
	QVBoxLayout *pedidowebdetFrameLayout = new QVBoxLayout(pedidowebdetFrame);
	RecPedidoWebDet *pedidowebdet = getRecPedidoWeb()->getRecPedidoWebDet(-1);
	dbViewDefinitionDict pedidowebdetviews;
	DBAPP->getDatabase()->getViewsForTable( pedidowebdet, pedidowebdetviews );
	dbRecordListDataModel *pedidowebdetdm =
		new dbRecordListDataModel(getRecPedidoWeb()->getListPedidoWebDet(), pedidowebdet, pedidowebdetviews, "1=0");
	pFrmPedidoWebDet = static_cast<FrmEditPedidoWebDet *>(
		DBAPP->createEditDetailForm(this,
		-1, pedidowebdet, "PEDIDOWEBDET",pedidowebdetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmPedidoWebDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	pedidowebdetFrameLayout->addWidget( pFrmPedidoWebDet );
	pedidowebdetLayout->addWidget(pedidowebdetFrame);
	editTotal = addEditField( pControlsFrame, "PEDIDOWEB", "TOTAL", totalLayout );
	editTelefono = addTextField( pControlsFrame, "PEDIDOWEB", "TELEFONO", notasLayout );
	editNotas = addTextField( pControlsFrame, "PEDIDOWEB", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( clienteLayout );
	pControlsLayout->addLayout( cliente3Layout );
	pControlsLayout->addLayout( pedidowebdetLayout );
	pControlsLayout->addLayout( totalLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITPEDIDOWEB_INIT_CONTROLS*/
    editAgenteCodigo->setWidthInChars(8);
    editTelefono->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
    editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void FrmEditPedidoWeb::scatterFields()
{
    /*<<<<<FRMEDITPEDIDOWEB_SCATTER*/
	editContador->setText(getRecPedidoWeb()->getValue("CONTADOR").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editContador;
	comboEstadoPedido->setCurrentItemByValue(getRecPedidoWeb()->getValue("ESTADOPEDIDO").toInt());
	editFecha->setText(getRecPedidoWeb()->getValue("FECHA").toDate());
	editReferencia->setText(getRecPedidoWeb()->getValue("REFERENCIA").toString());
	editNombre->setText(getRecPedidoWeb()->getValue("NOMBRE").toString());
	editemail->setText(getRecPedidoWeb()->getValue("EMAIL").toString());
	editDireccion->setText(getRecPedidoWeb()->getValue("DIRECCION").toString());
	editCP->setText(getRecPedidoWeb()->getValue("CP").toString());
	editLocalidad->setText(getRecPedidoWeb()->getValue("LOCALIDAD").toString());
	editProvincia->setText(getRecPedidoWeb()->getValue("PROVINCIA").toString());
	editTotal->setText(getRecPedidoWeb()->getValue("TOTAL").toMoney());
	editTelefono->setText(getRecPedidoWeb()->getValue("TELEFONO").toString());
	editNotas->setText(getRecPedidoWeb()->getValue("NOTAS").toString());
	scatterAgente();
/*>>>>>FRMEDITPEDIDOWEB_SCATTER*/
    if( isInserting() ) {
        if( !isDuplicating() && editFecha->toDate().isNull() )
            editFecha->setText( factu::ModuleInstance->getWorkingDate() );
        editContador->setText( ModuleInstance->getMaxContador( "PEDIDOWEB",
                               getRecord()->getValue("EMPRESA_ID").toInt() ) );
        pFocusWidget = editContador; // Porque no queremos cambiar la fecha
    } else if( isUpdating() ) {
        pFocusWidget = pFrmPedidoWebDet;
    }
    pFrmPedidoWebDet->addDetailIfNeeded();
}

void FrmEditPedidoWeb::gatherFields()
{
    /*<<<<<FRMEDITPEDIDOWEB_GATHER*/
	getRecPedidoWeb()->setValue( "CONTADOR", editContador->toInt());
	getRecPedidoWeb()->setValue( "ESTADOPEDIDO", comboEstadoPedido->getCurrentItemValue());
	getRecPedidoWeb()->setValue( "FECHA", editFecha->toDate());
	getRecPedidoWeb()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecPedidoWeb()->setValue( "REFERENCIA", editReferencia->toString());
	getRecPedidoWeb()->setValue( "NOMBRE", editNombre->toString());
	getRecPedidoWeb()->setValue( "EMAIL", editemail->toString());
	getRecPedidoWeb()->setValue( "DIRECCION", editDireccion->toString());
	getRecPedidoWeb()->setValue( "CP", editCP->toString());
	getRecPedidoWeb()->setValue( "LOCALIDAD", editLocalidad->toString());
	getRecPedidoWeb()->setValue( "PROVINCIA", editProvincia->toString());
	getRecPedidoWeb()->setValue( "TOTAL", editTotal->toMoney());
	getRecPedidoWeb()->setValue( "TELEFONO", editTelefono->toString());
	getRecPedidoWeb()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPEDIDOWEB_GATHER*/
    factu::ModuleInstance->setWorkingDate( editFecha->toDate() );
}


void FrmEditPedidoWeb::scatterAgente()
{
    /*<<<<<FRMEDITPEDIDOWEB_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITPEDIDOWEB_SCATTER_AGENTE*/
}
void FrmEditPedidoWeb::pushAgenteCodigo_clicked()
{
    /*<<<<<FRMEDITPEDIDOWEB_PUSH_AGENTE_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchAgenteCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editAgenteCodigo->setJustEdited( false );
			editAgenteCodigo->setCancelling();
			if( DBAPP->choose(this, getRecAgente(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterAgente();
				editAgenteCodigo->setJustEdited( true );
				setWiseFocus(editAgenteCodigo);
			}
			break;
		case 'M':
			{
				if( getRecAgente()->getRecordID() ) {
					editAgenteCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecAgente(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editAgenteCodigo->setJustEdited( true );
						scatterAgente();
					}
				setWiseFocus(editAgenteCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecAgente()->getRecordID() != 0 ) {
					editAgenteCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecAgente(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				factu::RecAgente *tmprec = static_cast<factu::RecAgente *>(DBAPP->createRecord( "Agente" ));
				editAgenteCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editAgenteCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editAgenteCodigo->setJustEdited( true );
					getRecAgente()->copyRecord( tmprec );
					scatterAgente();
				}
				setWiseFocus(editAgenteCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPEDIDOWEB_PUSH_AGENTE_CODIGO_CLICKED*/
}

void FrmEditPedidoWeb::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITPEDIDOWEB_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editAgenteCodigo )
		pushAgenteCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITPEDIDOWEB_SPECIALACTION*/
}

void FrmEditPedidoWeb::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPEDIDOWEB_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( focusWidget() != pushAgenteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editAgenteCodigo, editAgenteRazonSocial,
		getRecAgente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterAgente();
/*>>>>>FRMEDITPEDIDOWEB_VALIDATE*/
#if 0
    /*<<<<<FRMEDITPEDIDOWEB_CABECERA_VALIDATE*/
	if( sender == editDtoP100 && editDtoP100->isJustEdited() ) {
		editDescuento->setText( 0.0 );
		actTotales();
	}
	if( sender == comboIVADetallado ) {
		if( comboIVADetallado->getCurrentItemValue() == factu::FldIVADetallado::con_recargo ) {
			editRecargoEquivalencia->setVisible( true );
			editRecargoEquivalencia->getLabel()->setVisible( true );
		} else {
			editRecargoEquivalencia->setVisible( false );
			editRecargoEquivalencia->getLabel()->setVisible( false );
		}
		actTotales();
	}
	if( sender == editFecha || !sender ) {
		if( editFecha->toDate().getYear() != factu::ModuleInstance->getEmpresaModule()->getEjercicio() ) {
			validresult->addWarning( Xtring::printf(_("La fecha no es del ejercicio actual: %d"),
				factu::ModuleInstance->getEmpresaModule()->getEjercicio()),
				"FECHA" );
		}
	}
	if( sender == editTotal && editTotal->isJustEdited() ) {
		if( editBaseImponible->toDouble() != 0.0 ) {
			double nuevo_total = editTotal->toDouble();
			editDtoP100->setText( 0.0 );
			editDescuento->setText( 0.0 );
			actTotales();
			double orig_total = editTotal->toDouble();
			if( orig_total != nuevo_total ) {
				double orig_sumaimp = editSumaImportes->toDouble();
				double nuevo_sumaimp =  (nuevo_total * orig_sumaimp) / orig_total;
				double dtop100 = 100 - (100 * nuevo_sumaimp / orig_sumaimp);
				editDtoP100->setText( dtop100 );
				actTotales();
			}
		}
	}
/*>>>>>FRMEDITPEDIDOWEB_CABECERA_VALIDATE*/
    if( !sender || sender == editContador ) {
        if( !validCodeAndDesc( editContador, *validresult, editContador, 0, "CONTADOR", "") )
            if( !sender )
                *isvalid = false;
    }
#endif
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITPEDIDOWEB_CABECERA_UPDATEFROMDETAILS*/
void FrmEditPedidoWeb::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITPEDIDOWEB_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITPEDIDOWEB_CABECERA_NUMERALINEAS*/
void FrmEditPedidoWeb::numeraLineas()
{
	dbRecordList *reclst = getRecPedidoWeb()->getListPedidoWebDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecPedidoWebDet *detalle = static_cast<RecPedidoWebDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITPEDIDOWEB_CABECERA_NUMERALINEAS*/
}

/*<<<<<FRMEDITPEDIDOWEB_CABECERA_ACTTOTALES*/
void FrmEditPedidoWeb::actTotales()
{
	getRecPedidoWeb()->gather( this );
	getRecPedidoWeb()->actTotales();
	getRecPedidoWeb()->scatter( this );
/*>>>>>FRMEDITPEDIDOWEB_CABECERA_ACTTOTALES*/
}

#if 0
/*<<<<<FRMEDITPEDIDOWEB_CABECERA_SCATTER_CLIENTE*/
	if( editClienteCodigo->isJustEdited() ) {
		if( getRecCliente()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecCliente()->getValue("IVADetallado").toInt() );
		if( getRecCliente()->getRecAgente()->getRecordID() != 0 )
			searchAgenteCodigo->setValue( getRecCliente()->getRecAgente()->getValue("CODIGO") );
	}
/*>>>>>FRMEDITPEDIDOWEB_CABECERA_SCATTER_CLIENTE*/
/*<<<<<FRMEDITPEDIDOWEB_CABECERA_GENNUMDOC*/
void FrmEditPedidoWeb::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getMaxContador( "PEDIDOWEB",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITPEDIDOWEB_CABECERA_GENNUMDOC*/
}
#endif

/*<<<<<FRMEDITPEDIDOWEB_FIN*/
} // namespace ventasweb
} // namespace gong
/*>>>>>FRMEDITPEDIDOWEB_FIN*/

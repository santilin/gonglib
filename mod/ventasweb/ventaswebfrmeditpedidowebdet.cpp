/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de detalles de pedidos de venta
// FIELD Cantidad double - cantidad
// FIELD Articulo_ID Reference(factu::Articulo,Codigo,Nombre,dbRecord::SeekCodeInDesc) - cantidad
// FIELD PVP money - precios
// FIELD ImporteConIVA money - precios
// TYPE FrmEditRecDetail ventasweb::PedidoWebDet PedidoDet Venta
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITPEDIDOWEBDET_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ventaswebfrmeditpedidowebdet.h"
/*>>>>>FRMEDITPEDIDOWEBDET_INCLUDES*/
#include <empresamodule.h>
#include "factufrmeditpedidoventa.h"
#include "factufrmeditpresupuestoventa.h"

namespace gong {
namespace ventasweb {

/*<<<<<FRMEDITPEDIDOWEBDET_CONSTRUCTOR*/
FrmEditPedidoWebDet::FrmEditPedidoWebDet(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditPedidoWebDet" );
/*>>>>>FRMEDITPEDIDOWEBDET_CONSTRUCTOR*/
    /*<<<<<FRMEDITPEDIDOWEBDET_INIT_CONTROLS*/
	QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
	QHBoxLayout *preciosLayout = new QHBoxLayout(0, 0, 6, "preciosLayout");
	editCantidad = addEditField( pControlsFrame, "PEDIDOWEBDET", "CANTIDAD", cantidadLayout );

	searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", cantidadLayout );
	pushArticuloCodigo = searchArticuloCodigo->getButton();
	connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
	editArticuloCodigo = searchArticuloCodigo->getEditCode();
	editArticuloNombre = searchArticuloCodigo->getEditDesc();
	editPVP = addEditField( pControlsFrame, "PEDIDOWEBDET", "PVP", preciosLayout );
	editImporteConIVA = addEditField( pControlsFrame, "PEDIDOWEBDET", "IMPORTECONIVA", preciosLayout );
	pControlsLayout->addLayout( cantidadLayout );
	pControlsLayout->addLayout( preciosLayout );
/*>>>>>FRMEDITPEDIDOWEBDET_INIT_CONTROLS*/
}

void FrmEditPedidoWebDet::scatterFields()
{
    /*<<<<<FRMEDITPEDIDOWEBDET_SCATTER*/
	editCantidad->setText(getRecPedidoWebDet()->getValue("CANTIDAD").toDouble());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCantidad;
	editPVP->setText(getRecPedidoWebDet()->getValue("PVP").toMoney());
	editImporteConIVA->setText(getRecPedidoWebDet()->getValue("IMPORTECONIVA").toMoney());
	scatterArticulo();
/*>>>>>FRMEDITPEDIDOWEBDET_SCATTER*/
}

void FrmEditPedidoWebDet::gatherFields()
{
    /*<<<<<FRMEDITPEDIDOWEBDET_GATHER*/
	getRecPedidoWebDet()->setValue( "CANTIDAD", editCantidad->toDouble());
	getRecPedidoWebDet()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
	getRecPedidoWebDet()->setValue( "PVP", editPVP->toMoney());
	getRecPedidoWebDet()->setValue( "IMPORTECONIVA", editImporteConIVA->toMoney());
/*>>>>>FRMEDITPEDIDOWEBDET_GATHER*/
}



void FrmEditPedidoWebDet::scatterArticulo()
{
    /*<<<<<FRMEDITPEDIDOWEBDET_SCATTER_ARTICULO*/
	editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
	editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
/*>>>>>FRMEDITPEDIDOWEBDET_SCATTER_ARTICULO*/
}
void FrmEditPedidoWebDet::pushArticuloCodigo_clicked()
{
    /*<<<<<FRMEDITPEDIDOWEBDET_PUSH_ARTICULO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchArticuloCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editArticuloCodigo->setJustEdited( false );
			editArticuloCodigo->setCancelling();
			if( DBAPP->choose(this, getRecArticulo(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterArticulo();
				editArticuloCodigo->setJustEdited( true );
				setWiseFocus(editArticuloCodigo);
			}
			break;
		case 'M':
			{
				if( getRecArticulo()->getRecordID() ) {
					editArticuloCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecArticulo(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editArticuloCodigo->setJustEdited( true );
						scatterArticulo();
					}
				setWiseFocus(editArticuloCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecArticulo()->getRecordID() != 0 ) {
					editArticuloCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecArticulo(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				factu::RecArticulo *tmprec = static_cast<factu::RecArticulo *>(DBAPP->createRecord( "Articulo" ));
				editArticuloCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editArticuloCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editArticuloCodigo->setJustEdited( true );
					getRecArticulo()->copyRecord( tmprec );
					scatterArticulo();
				}
				setWiseFocus(editArticuloCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPEDIDOWEBDET_PUSH_ARTICULO_CODIGO_CLICKED*/
}
void FrmEditPedidoWebDet::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITPEDIDOWEBDET_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editArticuloCodigo )
		pushArticuloCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITPEDIDOWEBDET_SPECIALACTION*/
}

void FrmEditPedidoWebDet::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPEDIDOWEBDET_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( focusWidget() != pushArticuloCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editArticuloCodigo, editArticuloNombre,
		getRecArticulo(), "CODIGO", "NOMBRE", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::SeekCodeInDesc )) )
		scatterArticulo();
/*>>>>>FRMEDITPEDIDOWEBDET_VALIDATE*/
#if 0
    /*<<<<<FRMEDITPEDIDOWEBDET_DETALLE_VALIDATE*/
	bool actualiza = (sender && (sender == pFocusWidget) );
	double cantidad = editCantidad->toDouble();
	double pvpsiniva = editPVPSinIVA->toDouble();
	double pvpconiva = editPVP->toDouble();
	double dtop100 = editDtoP100->toDouble();
	double importesiniva =  cantidad * pvpsiniva * ( 1 - dtop100 / 100 );
		int tarifacliente = static_cast<FrmEditPedidoWeb *>(getFrmMaster())->getRecCliente()->getValue("TARIFA").toInt();
	if ( sender == editCantidad && editCantidad->isJustEdited() ) {
		actualiza = true;
	}
	if ( sender == editArticuloCodigo && editArticuloCodigo->isJustEdited() ) {
		actualiza = true;
		editTipoIVACodigo->setText( getRecArticulo()->getRecTipoIVA()->getValue( "CODIGO" ) );
		editTipoIVACodigo->setJustEdited( true );
		if ( validSeekCode( editTipoIVACodigo, isvalid, *validresult, editTipoIVACodigo, editTipoIVANombre,
		                    getRecTipoIVA(), "CODIGO", "NOMBRE", Xtring::null ) )
			scatterTipoIVA();
		if( tarifacliente == 0 )
			pvpconiva = getRecArticulo()->getValue( "PVP" ).toDouble();
		else
			pvpconiva = getRecArticulo()->getValue( "PVP" + Xtring::number(tarifacliente) ).toDouble();
		// El precio con IVA se calcula tomando el tipo de iva de este detalle
		pvpsiniva = pvpconiva / ( 1 + getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 );
	}
	if( (sender == editTipoIVACodigo && editTipoIVACodigo->isJustEdited())
		|| (sender == editPVPSinIVA && editPVPSinIVA->isJustEdited()) ) {
		actualiza = true;
		pvpsiniva = editPVPSinIVA->toDouble();
		pvpconiva = pvpsiniva * ( 1 + getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 );
	}
	if ( sender == editPVP && editPVP->isJustEdited() ) {
		actualiza = true;
		pvpconiva = editPVP->toDouble();
		pvpsiniva = pvpconiva / ( 1 + getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 );
	}
	if ( sender == editDtoP100 && editDtoP100->isJustEdited() ) {
		actualiza = true;
		dtop100 = editDtoP100->toDouble();
	}
	if( sender == editImporte && editImporte->isJustEdited() ) {
		importesiniva = editImporte->toDouble();
		if( cantidad != 0.0 && pvpsiniva != 0.0 ) {
			dtop100 = 100 * ( 1 - importesiniva / (cantidad * pvpsiniva) );
			editDtoP100->setText( dtop100 );
		}
		actualiza = true;
	}
	if( sender == editImporteConIVA && editImporteConIVA->isJustEdited() ) {
		if( cantidad != 0.0 && pvpsiniva != 0.0 ) {
			dtop100 = 100 * ( 1 - editImporteConIVA->toDouble() / (cantidad * pvpconiva) );
			editDtoP100->setText( dtop100 );
		}
		actualiza = true;
	}
	if( actualiza ) {
		editPVPSinIVA->setText( pvpsiniva );
		editPVP->setText( pvpconiva );
		int ndec = empresa::ModuleInstance->getDecimalesMoneda();
		if( getFrmMaster()->getControlValue( "IVADETALLADO" ).toInt() == factu::FldIVADetallado::incluido )
			ndec += 2;
		Money importesiniva(cantidad * pvpsiniva * ( 1 - dtop100 / 100 ), ndec);
		Money importeconiva = Money( cantidad * pvpconiva * ( 1 - dtop100 / 100 ),
			empresa::ModuleInstance->getDecimalesMoneda() );
		editImporte->setText( importesiniva );
		editImporteConIVA->setText( importeconiva );
	}
	if (( sender == editArticuloCodigo && editArticuloCodigo->isJustEdited() ) ||
		    ( sender == editCantidad && editCantidad->isJustEdited() ) ) {
		if ( !getRecArticulo()->isNew() ) {
			Xtring stock_msg;
			if( getRecArticulo()->getValue( "USARSTOCKS" ).toBool() )
				stock_msg = getRecArticulo()->getValue( "STOCK" ).toString();
			else
				stock_msg = _("No se usan");
			Xtring precios_msg;
			if( tarifacliente != 0 )
				precios_msg = _( "%s: PVP DEL CLIENTE: %s, Coste: %s, Existencias: %s" );
			else
				precios_msg = _( "%s: PVP: %s, Coste: %s, Existencias: %s" );
			DBAPP->showOSD( getTitle(), Xtring::printf( precios_msg,
		                       getRecArticulo()->getValue( "CODIGO" ).toString().c_str(),
		                       editPVP->toMoney().toString( DBAPP->getRegConfig() ).c_str(),
		                       getRecArticulo()->getValue( "COSTE" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
		                       stock_msg.c_str() ) );
		}
	}
/*>>>>>FRMEDITPEDIDOWEBDET_DETALLE_VALIDATE*/
#endif
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITPEDIDOWEBDET_FIN*/
} // namespace ventasweb
} // namespace gong
/*>>>>>FRMEDITPEDIDOWEBDET_FIN*/



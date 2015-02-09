/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de detalles presupuestos de venta
// FIELD NLinea int - pre
// FIELD TipoIVA_ID Reference(empresa::TipoIVA,Codigo,Nombre) - pre
// FIELD Cantidad double - cantidad
// FIELD Articulo_ID Reference(Articulo,Codigo,Nombre,dbRecord::SeekCodeInDesc) - cantidad
// FIELD PVPSinIVA money - precios
// FIELD PVP money - precios
// FIELD DtoP100 double - precios
// FIELD Importe money - precios
// FIELD ImporteConIVA money - precios
// FIELD Descripcion text
// FIELD Notas text
// TYPE FrmEditRecDetail factu::PresupuestoVentaDet PresupuestoDet Venta
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITPRESUPUESTOVENTADET_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditpresupuestoventadet.h"
/*>>>>>FRMEDITPRESUPUESTOVENTADET_INCLUDES*/
#include <empresamodule.h>
#include "factufrmeditpresupuestoventa.h"
#include "factufldivadetallado.h"
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITPRESUPUESTOVENTADET_CONSTRUCTOR*/
FrmEditPresupuestoVentaDet::FrmEditPresupuestoVentaDet(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditPresupuestoVentaDet" );
/*>>>>>FRMEDITPRESUPUESTOVENTADET_CONSTRUCTOR*/
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_INIT_CONTROLS*/
	QHBoxLayout *preLayout = new QHBoxLayout(0, 0, 6, "preLayout");
	QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
	QHBoxLayout *preciosLayout = new QHBoxLayout(0, 0, 6, "preciosLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNLinea = addEditField( pControlsFrame, "PRESUPUESTOVENTADET", "NLINEA", preLayout );

	searchTipoIVACodigo = addSearchField( pControlsFrame, "TIPOIVA_ID", "TIPOIVA", "CODIGO", "NOMBRE", preLayout );
	pushTipoIVACodigo = searchTipoIVACodigo->getButton();
	connect( pushTipoIVACodigo, SIGNAL( clicked() ), this, SLOT( pushTipoIVACodigo_clicked() ) );
	editTipoIVACodigo = searchTipoIVACodigo->getEditCode();
	editTipoIVANombre = searchTipoIVACodigo->getEditDesc();
	editCantidad = addEditField( pControlsFrame, "PRESUPUESTOVENTADET", "CANTIDAD", cantidadLayout );

	searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", cantidadLayout );
	pushArticuloCodigo = searchArticuloCodigo->getButton();
	connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
	editArticuloCodigo = searchArticuloCodigo->getEditCode();
	editArticuloNombre = searchArticuloCodigo->getEditDesc();
	editPVPSinIVA = addEditField( pControlsFrame, "PRESUPUESTOVENTADET", "PVPSINIVA", preciosLayout );
	editPVP = addEditField( pControlsFrame, "PRESUPUESTOVENTADET", "PVP", preciosLayout );
	editDtoP100 = addEditField( pControlsFrame, "PRESUPUESTOVENTADET", "DTOP100", preciosLayout );
	editImporte = addEditField( pControlsFrame, "PRESUPUESTOVENTADET", "IMPORTE", preciosLayout );
	editImporteConIVA = addEditField( pControlsFrame, "PRESUPUESTOVENTADET", "IMPORTECONIVA", preciosLayout );
	editDescripcion = addTextField( pControlsFrame, "PRESUPUESTOVENTADET", "DESCRIPCION", descripcionLayout );
	editNotas = addTextField( pControlsFrame, "PRESUPUESTOVENTADET", "NOTAS", notasLayout );
	pControlsLayout->addLayout( preLayout );
	pControlsLayout->addLayout( cantidadLayout );
	pControlsLayout->addLayout( preciosLayout );
	pControlsLayout->addLayout( descripcionLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITPRESUPUESTOVENTADET_INIT_CONTROLS*/
    editNLinea->setMustBeReadOnly( true );
    editArticuloCodigo->setWidthInChars(12);

    pushActPrecioArticulo = new QPushButton(this, "pushActPrecioArticulo" );
    pushActPrecioArticulo->setText( toGUI( _( "&Act. PVP artículo" ) ) );
    connect( pushActPrecioArticulo, SIGNAL( clicked() ), this, SLOT( slotActPrecioArticulo_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushActPrecioArticulo );

    pushInsertTable = new QPushButton(this, "pushInsertTable" );
    pushInsertTable->setText( toGUI( _( "Insertar ..." ) ) );
    connect( pushInsertTable, SIGNAL( clicked() ), this, SLOT( slotInsertTable_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushInsertTable );
}

void FrmEditPresupuestoVentaDet::scatterFields()
{
    if( pFocusWidget == 0 )
        pFocusWidget = editCantidad;
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_SCATTER*/
	editNLinea->setText(getRecPresupuestoVentaDet()->getValue("NLINEA").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNLinea;
	editCantidad->setText(getRecPresupuestoVentaDet()->getValue("CANTIDAD").toDouble());
	editPVPSinIVA->setText(getRecPresupuestoVentaDet()->getValue("PVPSINIVA").toMoney());
	editPVP->setText(getRecPresupuestoVentaDet()->getValue("PVP").toMoney());
	editDtoP100->setText(getRecPresupuestoVentaDet()->getValue("DTOP100").toDouble());
	editImporte->setText(getRecPresupuestoVentaDet()->getValue("IMPORTE").toMoney());
	editImporteConIVA->setText(getRecPresupuestoVentaDet()->getValue("IMPORTECONIVA").toMoney());
	editDescripcion->setText(getRecPresupuestoVentaDet()->getValue("DESCRIPCION").toString());
	editNotas->setText(getRecPresupuestoVentaDet()->getValue("NOTAS").toString());
	scatterTipoIVA();
	scatterArticulo();
/*>>>>>FRMEDITPRESUPUESTOVENTADET_SCATTER*/
    if ( isInserting() ) {
        double cli_dto = static_cast<FrmEditPresupuestoVenta *>( getFrmMaster() )->getRecCliente()->getValue( "DTOP100" ).toDouble();
        editDtoP100->setText( cli_dto );
    }
    getRecArticulo()->addFilter( "ARTICULO.ENUSO=1" );
}

void FrmEditPresupuestoVentaDet::gatherFields()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_GATHER*/
	getRecPresupuestoVentaDet()->setValue( "NLINEA", editNLinea->toInt());
	getRecPresupuestoVentaDet()->setValue( "TIPOIVA_ID", getRecTipoIVA()->getRecordID() );
	getRecPresupuestoVentaDet()->setValue( "CANTIDAD", editCantidad->toDouble());
	getRecPresupuestoVentaDet()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
	getRecPresupuestoVentaDet()->setValue( "PVPSINIVA", editPVPSinIVA->toMoney());
	getRecPresupuestoVentaDet()->setValue( "PVP", editPVP->toMoney());
	getRecPresupuestoVentaDet()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecPresupuestoVentaDet()->setValue( "IMPORTE", editImporte->toMoney());
	getRecPresupuestoVentaDet()->setValue( "IMPORTECONIVA", editImporteConIVA->toMoney());
	getRecPresupuestoVentaDet()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecPresupuestoVentaDet()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPRESUPUESTOVENTADET_GATHER*/
}


void FrmEditPresupuestoVentaDet::scatterTipoIVA()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_SCATTER_TIPOIVA*/
	editTipoIVACodigo->setText( getRecTipoIVA()->getValue("CODIGO") );
	editTipoIVANombre->setText( getRecTipoIVA()->getValue("NOMBRE") );
/*>>>>>FRMEDITPRESUPUESTOVENTADET_SCATTER_TIPOIVA*/
}
void FrmEditPresupuestoVentaDet::pushTipoIVACodigo_clicked()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchTipoIVACodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editTipoIVACodigo->setJustEdited( false );
			editTipoIVACodigo->setCancelling();
			if( DBAPP->choose(this, getRecTipoIVA(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterTipoIVA();
				editTipoIVACodigo->setJustEdited( true );
				setWiseFocus(editTipoIVACodigo);
			}
			break;
		case 'M':
			{
				if( getRecTipoIVA()->getRecordID() ) {
					editTipoIVACodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecTipoIVA(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editTipoIVACodigo->setJustEdited( true );
						scatterTipoIVA();
					}
				setWiseFocus(editTipoIVACodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecTipoIVA()->getRecordID() != 0 ) {
					editTipoIVACodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTipoIVA(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				empresa::RecTipoIVA *tmprec = static_cast<empresa::RecTipoIVA *>(DBAPP->createRecord( "TipoIVA" ));
				editTipoIVACodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editTipoIVACodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editTipoIVACodigo->setJustEdited( true );
					getRecTipoIVA()->copyRecord( tmprec );
					scatterTipoIVA();
				}
				setWiseFocus(editTipoIVACodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPRESUPUESTOVENTADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
}

void FrmEditPresupuestoVentaDet::scatterArticulo()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_SCATTER_ARTICULO*/
	editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
	editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
/*>>>>>FRMEDITPRESUPUESTOVENTADET_SCATTER_ARTICULO*/
}
void FrmEditPresupuestoVentaDet::pushArticuloCodigo_clicked()
{
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_PUSH_ARTICULO_CODIGO_CLICKED*/
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
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecArticulo(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecArticulo *tmprec = static_cast<RecArticulo *>(DBAPP->createRecord( "Articulo" ));
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
/*>>>>>FRMEDITPRESUPUESTOVENTADET_PUSH_ARTICULO_CODIGO_CLICKED*/
}
void FrmEditPresupuestoVentaDet::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoIVACodigo )
		pushTipoIVACodigo_clicked();
	if( sender == editArticuloCodigo )
		pushArticuloCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITPRESUPUESTOVENTADET_SPECIALACTION*/
}

void FrmEditPresupuestoVentaDet::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushTipoIVACodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTipoIVACodigo, editTipoIVANombre,
		getRecTipoIVA(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTipoIVA();
	if( focusWidget() != pushArticuloCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editArticuloCodigo, editArticuloNombre,
		getRecArticulo(), "CODIGO", "NOMBRE", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::SeekCodeInDesc )) )
		scatterArticulo();
/*>>>>>FRMEDITPRESUPUESTOVENTADET_VALIDATE*/
    /*<<<<<FRMEDITPRESUPUESTOVENTADET_DETALLE_VALIDATE*/
	bool actualiza = (sender && (sender == pFocusWidget) );
	double cantidad = editCantidad->toDouble();
	double pvpsiniva = editPVPSinIVA->toDouble();
	double pvpconiva = editPVP->toDouble();
	double dtop100 = editDtoP100->toDouble();
	double importesiniva =  cantidad * pvpsiniva * ( 1 - dtop100 / 100 );
		int tarifacliente = static_cast<FrmEditPresupuestoVenta *>(getFrmMaster())->getRecCliente()->getValue("TARIFA").toInt();
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
/*>>>>>FRMEDITPRESUPUESTOVENTADET_DETALLE_VALIDATE*/
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditPresupuestoVentaDet::slotActPrecioArticulo_clicked()
{
    if( ModuleInstance->editPVPsArticulo(this, getRecArticulo(),
                                         static_cast<FrmEditPresupuestoVenta *>(getFrmMaster())->getRecCliente(),
                                         editPVP->toDouble() ) ) {
        searchArticuloCodigo->setValue( getRecArticulo()->getValue("CODIGO").toString() );
    }
}

void FrmEditPresupuestoVentaDet::slotInsertTable_clicked()
{
    ModuleInstance->insertDetails( getFrmMaster(), this );
}

void FrmEditPresupuestoVentaDet::updateStatus( bool callbehaviors )
{
    FrmEditRecDetail::updateStatus(callbehaviors);
    pushInsertTable->setVisible( pMenuRecordAdd->isEnabled() );
    pushActPrecioArticulo->setVisible( isEditing() );
}


/*<<<<<FRMEDITPRESUPUESTOVENTADET_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITPRESUPUESTOVENTADET_FIN*/

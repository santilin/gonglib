/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditofertaventadet.cpp Fichero de edición de detalles de albaranes de venta
 * GongLib project. (L) Santiago Capel Torres, 2003-2011
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de detalles de albaranes de venta
// FIELD NLinea int - pre
// FIELD TipoIVA_ID Reference(empresa::TipoIVA,Codigo,Nombre) - pre
// FIELD Cantidad double - cantidad
// FIELD Articulo_ID Reference(factu::Articulo,Codigo,Nombre,dbRecord::SeekCodeInDesc) - cantidad
// FIELD PVPSinIVA money - precios
// FIELD PVP money - precios
// FIELD DtoP100 double - precios
// FIELD Importe money - precios
// FIELD ImporteConIVA money - precios
// FIELD Descripcion text
// FIELD Notas text
// TYPE FrmEditRecDetail ofertas::OfertaVentaDet AlbaranDet Venta
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITOFERTAVENTADET_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ofertasfrmeditofertaventadet.h"
/*>>>>>FRMEDITOFERTAVENTADET_INCLUDES*/
#include <empresamodule.h>
#include <factufldivadetallado.h>
#include "ofertasfrmeditofertaventa.h"
#include "ofertasmodule.h"

namespace gong {
namespace ofertas {

/*<<<<<FRMEDITOFERTAVENTADET_CONSTRUCTOR*/
FrmEditOfertaVentaDet::FrmEditOfertaVentaDet(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditOfertaVentaDet" );
/*>>>>>FRMEDITOFERTAVENTADET_CONSTRUCTOR*/
    /*<<<<<FRMEDITOFERTAVENTADET_INIT_CONTROLS*/
	QHBoxLayout *preLayout = new QHBoxLayout(0, 0, 6, "preLayout");
	QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
	QHBoxLayout *preciosLayout = new QHBoxLayout(0, 0, 6, "preciosLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNLinea = addEditField( pControlsFrame, "OFERTAVENTADET", "NLINEA", preLayout );

	searchTipoIVACodigo = addSearchField( pControlsFrame, "TIPOIVA_ID", "TIPOIVA", "CODIGO", "NOMBRE", preLayout );
	pushTipoIVACodigo = searchTipoIVACodigo->getButton();
	connect( pushTipoIVACodigo, SIGNAL( clicked() ), this, SLOT( pushTipoIVACodigo_clicked() ) );
	editTipoIVACodigo = searchTipoIVACodigo->getEditCode();
	editTipoIVANombre = searchTipoIVACodigo->getEditDesc();
	editCantidad = addEditField( pControlsFrame, "OFERTAVENTADET", "CANTIDAD", cantidadLayout );

	searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", cantidadLayout );
	pushArticuloCodigo = searchArticuloCodigo->getButton();
	connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
	editArticuloCodigo = searchArticuloCodigo->getEditCode();
	editArticuloNombre = searchArticuloCodigo->getEditDesc();
	editPVPSinIVA = addEditField( pControlsFrame, "OFERTAVENTADET", "PVPSINIVA", preciosLayout );
	editPVP = addEditField( pControlsFrame, "OFERTAVENTADET", "PVP", preciosLayout );
	editDtoP100 = addEditField( pControlsFrame, "OFERTAVENTADET", "DTOP100", preciosLayout );
	editImporte = addEditField( pControlsFrame, "OFERTAVENTADET", "IMPORTE", preciosLayout );
	editImporteConIVA = addEditField( pControlsFrame, "OFERTAVENTADET", "IMPORTECONIVA", preciosLayout );
	editDescripcion = addTextField( pControlsFrame, "OFERTAVENTADET", "DESCRIPCION", descripcionLayout );
	editNotas = addTextField( pControlsFrame, "OFERTAVENTADET", "NOTAS", notasLayout );
	pControlsLayout->addLayout( preLayout );
	pControlsLayout->addLayout( cantidadLayout );
	pControlsLayout->addLayout( preciosLayout );
	pControlsLayout->addLayout( descripcionLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITOFERTAVENTADET_INIT_CONTROLS*/
    editNLinea->setMustBeReadOnly( true );

    pushActPrecioArticulo = new QPushButton(this, "pushActPrecioArticulo" );
    pushActPrecioArticulo->setText( toGUI( _( "&Act. PVP artículo" ) ) );
    connect( pushActPrecioArticulo, SIGNAL( clicked() ), this, SLOT( slotActPrecioArticulo_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushActPrecioArticulo );

    pushInsertTable = new QPushButton(this, "pushInsertTable" );
    pushInsertTable->setText( toGUI( _( "Insertar..." ) ) );
    connect( pushInsertTable, SIGNAL( clicked() ), this, SLOT( slotInsertTable_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushInsertTable );
}

void FrmEditOfertaVentaDet::scatterFields()
{
    if( pFocusWidget == 0 )
        pFocusWidget = editCantidad;
    /*<<<<<FRMEDITOFERTAVENTADET_SCATTER*/
	editNLinea->setText(getRecOfertaVentaDet()->getValue("NLINEA").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNLinea;
	editCantidad->setText(getRecOfertaVentaDet()->getValue("CANTIDAD").toDouble());
	editPVPSinIVA->setText(getRecOfertaVentaDet()->getValue("PVPSINIVA").toMoney());
	editPVP->setText(getRecOfertaVentaDet()->getValue("PVP").toMoney());
	editDtoP100->setText(getRecOfertaVentaDet()->getValue("DTOP100").toDouble());
	editImporte->setText(getRecOfertaVentaDet()->getValue("IMPORTE").toMoney());
	editImporteConIVA->setText(getRecOfertaVentaDet()->getValue("IMPORTECONIVA").toMoney());
	editDescripcion->setText(getRecOfertaVentaDet()->getValue("DESCRIPCION").toString());
	editNotas->setText(getRecOfertaVentaDet()->getValue("NOTAS").toString());
	scatterTipoIVA();
	scatterArticulo();
/*>>>>>FRMEDITOFERTAVENTADET_SCATTER*/
    if ( isInserting() ) {
        double cli_dto = static_cast<FrmEditOfertaVenta *>( getFrmMaster() )->getRecCliente()->getValue( "DTOP100" ).toDouble();
        editDtoP100->setText( cli_dto );
    }
    getRecArticulo()->addFilter( "ARTICULO.ENUSO=1" );
}

void FrmEditOfertaVentaDet::gatherFields()
{
    /*<<<<<FRMEDITOFERTAVENTADET_GATHER*/
	getRecOfertaVentaDet()->setValue( "NLINEA", editNLinea->toInt());
	getRecOfertaVentaDet()->setValue( "TIPOIVA_ID", getRecTipoIVA()->getRecordID() );
	getRecOfertaVentaDet()->setValue( "CANTIDAD", editCantidad->toDouble());
	getRecOfertaVentaDet()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
	getRecOfertaVentaDet()->setValue( "PVPSINIVA", editPVPSinIVA->toMoney());
	getRecOfertaVentaDet()->setValue( "PVP", editPVP->toMoney());
	getRecOfertaVentaDet()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecOfertaVentaDet()->setValue( "IMPORTE", editImporte->toMoney());
	getRecOfertaVentaDet()->setValue( "IMPORTECONIVA", editImporteConIVA->toMoney());
	getRecOfertaVentaDet()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecOfertaVentaDet()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITOFERTAVENTADET_GATHER*/
}

void FrmEditOfertaVentaDet::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITOFERTAVENTADET_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( focusWidget() != pushTipoIVACodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTipoIVACodigo, editTipoIVANombre,
		getRecTipoIVA(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTipoIVA();
	if( focusWidget() != pushArticuloCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editArticuloCodigo, editArticuloNombre,
		getRecArticulo(), "CODIGO", "NOMBRE", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::SeekCodeInDesc )) )
		scatterArticulo();
/*>>>>>FRMEDITOFERTAVENTADET_VALIDATE*/
    /*<<<<<FRMEDITOFERTAVENTADET_DETALLE_VALIDATE*/
	bool actualiza = (sender && (sender == pFocusWidget) );
	double cantidad = editCantidad->toDouble();
	double pvpsiniva = editPVPSinIVA->toDouble();
	double pvpconiva = editPVP->toDouble();
	double dtop100 = editDtoP100->toDouble();
	double importesiniva =  cantidad * pvpsiniva * ( 1 - dtop100 / 100 );
		int tarifacliente = static_cast<FrmEditOfertaVenta *>(getFrmMaster())->getRecCliente()->getValue("TARIFA").toInt();
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
/*>>>>>FRMEDITOFERTAVENTADET_DETALLE_VALIDATE*/
    if( !sender && isvalid && getRecArticulo()->getValue( "PVP" ).toDouble() == 0.0 ) {
        if( editPVP->toDouble() != 0.0 ) {
            getRecArticulo()->setValue("PVPSINIVA", editPVPSinIVA->toMoney() );
            getRecArticulo()->setValue("PVP", editPVP->toMoney() );
            getRecArticulo()->fixMargenYDescuento();
            if( getRecArticulo()->save(true, false) )
                DBAPP->showStickyOSD( getTitle(), Xtring::printf( _( "Se han actualizado los precios de venta del artículo a los de este detalle: sin IVA: %s, con IVA: %s" ),
                                      getRecArticulo()->getValue( "PVPSINIVA" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
                                      getRecArticulo()->getValue( "PVP" ).toMoney().toString( DBAPP->getRegConfig() ).c_str() ) );
        }
    }
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

void FrmEditOfertaVentaDet::scatterTipoIVA()
{
    /*<<<<<FRMEDITOFERTAVENTADET_SCATTER_TIPOIVA*/
	editTipoIVACodigo->setText( getRecTipoIVA()->getValue("CODIGO") );
	editTipoIVANombre->setText( getRecTipoIVA()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTAVENTADET_SCATTER_TIPOIVA*/
}
void FrmEditOfertaVentaDet::pushTipoIVACodigo_clicked()
{
    /*<<<<<FRMEDITOFERTAVENTADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
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
					DBAPP->createClient( DBAPP->createEditForm(this, getRecTipoIVA(),
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
/*>>>>>FRMEDITOFERTAVENTADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
}

void FrmEditOfertaVentaDet::scatterArticulo()
{
    /*<<<<<FRMEDITOFERTAVENTADET_SCATTER_ARTICULO*/
	editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
	editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
/*>>>>>FRMEDITOFERTAVENTADET_SCATTER_ARTICULO*/
}
void FrmEditOfertaVentaDet::pushArticuloCodigo_clicked()
{
    /*<<<<<FRMEDITOFERTAVENTADET_PUSH_ARTICULO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITOFERTAVENTADET_PUSH_ARTICULO_CODIGO_CLICKED*/
}
void FrmEditOfertaVentaDet::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITOFERTAVENTADET_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoIVACodigo )
		pushTipoIVACodigo_clicked();
	if( sender == editArticuloCodigo )
		pushArticuloCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITOFERTAVENTADET_SPECIALACTION*/
}

void FrmEditOfertaVentaDet::slotInsertTable_clicked()
{
    factu::ModuleInstance->insertDetails( getFrmMaster(), this );
}

void FrmEditOfertaVentaDet::updateStatus( bool callbehaviors )
{
    FrmEditRecDetail::updateStatus( callbehaviors );
    pushInsertTable->setVisible( pMenuRecordAdd->isEnabled() );
    pushActPrecioArticulo->setVisible( isEditing() );
}

void FrmEditOfertaVentaDet::slotActPrecioArticulo_clicked()
{
    if( factu::FactuModule::editPVPsArticulo(this, getRecArticulo(),
            static_cast<FrmEditOfertaVenta *>(getFrmMaster())->getRecCliente(),
            editPVP->toDouble() ) ) {
        searchArticuloCodigo->setValue( getRecArticulo()->getValue("CODIGO").toString() );
    }
}


/*<<<<<FRMEDITOFERTAVENTADET_FIN*/
} // namespace ofertas
} // namespace gong
/*>>>>>FRMEDITOFERTAVENTADET_FIN*/

/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de detalles de facturas de venta
// FIELD NLinea int - pre
// FIELD TipoIVA_ID Reference(empresa::TipoIVA,Codigo,Nombre) - pre
// FIELD Cantidad double - cantidad
// FIELD Articulo_ID Reference(Articulo,Codigo,Nombre,dbApplication::FindCodeInDesc) - cantidad
// FIELD PVPSinIVA money - precios
// FIELD PVP money - precios
// FIELD DtoP100 double - precios
// FIELD Importe money - precios
// FIELD ImporteConIVA money - precios
// FIELD Descripcion text
// FIELD Notas text
// TYPE FrmEditRecDetail factu::FacturaVentaDet FacturaDet Venta
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITFACTURAVENTADET_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditfacturaventadet.h"
/*>>>>>FRMEDITFACTURAVENTADET_INCLUDES*/
#include <empresamodule.h>
#include "factufrmeditfacturaventa.h"
#include "factufldivadetallado.h"
#include "facturecpedidoventa.h"
#include "facturecalbaranventa.h"
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITFACTURAVENTADET_CONSTRUCTOR*/
FrmEditFacturaVentaDet::FrmEditFacturaVentaDet(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditFacturaVentaDet" );
/*>>>>>FRMEDITFACTURAVENTADET_CONSTRUCTOR*/
    /*<<<<<FRMEDITFACTURAVENTADET_INIT_CONTROLS*/
	QHBoxLayout *preLayout = new QHBoxLayout(0, 0, 6, "preLayout");
	QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
	QHBoxLayout *preciosLayout = new QHBoxLayout(0, 0, 6, "preciosLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNLinea = addEditField( pControlsFrame, "FACTURAVENTADET", "NLINEA", preLayout );

	searchTipoIVACodigo = addSearchField( pControlsFrame, "TIPOIVA_ID", "TIPOIVA", "CODIGO", "NOMBRE", preLayout );
	pushTipoIVACodigo = searchTipoIVACodigo->getButton();
	connect( pushTipoIVACodigo, SIGNAL( clicked() ), this, SLOT( pushTipoIVACodigo_clicked() ) );
	editTipoIVACodigo = searchTipoIVACodigo->getEditCode();
	editTipoIVANombre = searchTipoIVACodigo->getEditDesc();
	editCantidad = addEditField( pControlsFrame, "FACTURAVENTADET", "CANTIDAD", cantidadLayout );

	searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", cantidadLayout );
	pushArticuloCodigo = searchArticuloCodigo->getButton();
	connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
	editArticuloCodigo = searchArticuloCodigo->getEditCode();
	editArticuloNombre = searchArticuloCodigo->getEditDesc();
	editPVPSinIVA = addEditField( pControlsFrame, "FACTURAVENTADET", "PVPSINIVA", preciosLayout );
	editPVP = addEditField( pControlsFrame, "FACTURAVENTADET", "PVP", preciosLayout );
	editDtoP100 = addEditField( pControlsFrame, "FACTURAVENTADET", "DTOP100", preciosLayout );
	editImporte = addEditField( pControlsFrame, "FACTURAVENTADET", "IMPORTE", preciosLayout );
	editImporteConIVA = addEditField( pControlsFrame, "FACTURAVENTADET", "IMPORTECONIVA", preciosLayout );
	editDescripcion = addTextField( pControlsFrame, "FACTURAVENTADET", "DESCRIPCION", descripcionLayout );
	editNotas = addTextField( pControlsFrame, "FACTURAVENTADET", "NOTAS", notasLayout );
	pControlsLayout->addLayout( preLayout );
	pControlsLayout->addLayout( cantidadLayout );
	pControlsLayout->addLayout( preciosLayout );
	pControlsLayout->addLayout( descripcionLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITFACTURAVENTADET_INIT_CONTROLS*/
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

    pushAddAlbaran = new QPushButton(this, "pushAddAlbaran" );
    pushAddAlbaran->setText( toGUI( _( "&Facturar albarán" ) ) );
    connect( pushAddAlbaran, SIGNAL( clicked() ), this, SLOT( slotAddAlbaran_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushAddAlbaran );
}

void FrmEditFacturaVentaDet::scatterFields()
{
    /*<<<<<FRMEDITFACTURAVENTADET_SCATTER*/
	editNLinea->setText(getRecFacturaVentaDet()->getValue("NLINEA").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNLinea;
	editCantidad->setText(getRecFacturaVentaDet()->getValue("CANTIDAD").toDouble());
	editPVPSinIVA->setText(getRecFacturaVentaDet()->getValue("PVPSINIVA").toMoney());
	editPVP->setText(getRecFacturaVentaDet()->getValue("PVP").toMoney());
	editDtoP100->setText(getRecFacturaVentaDet()->getValue("DTOP100").toDouble());
	editImporte->setText(getRecFacturaVentaDet()->getValue("IMPORTE").toMoney());
	editImporteConIVA->setText(getRecFacturaVentaDet()->getValue("IMPORTECONIVA").toMoney());
	editDescripcion->setText(getRecFacturaVentaDet()->getValue("DESCRIPCION").toString());
	editNotas->setText(getRecFacturaVentaDet()->getValue("NOTAS").toString());
	scatterTipoIVA();
	scatterArticulo();
/*>>>>>FRMEDITFACTURAVENTADET_SCATTER*/
    if ( isInserting() ) {
        double cli_dto = static_cast<FrmEditFacturaVenta *>( getFrmMaster() )->getRecCliente()->getValue( "DTOP100" ).toDouble();
        editDtoP100->setText( cli_dto );
    }
    getRecArticulo()->addFilter( "ARTICULO.ENUSO=1" );
}

void FrmEditFacturaVentaDet::gatherFields()
{
    /*<<<<<FRMEDITFACTURAVENTADET_GATHER*/
	getRecFacturaVentaDet()->setValue( "NLINEA", editNLinea->toInt());
	getRecFacturaVentaDet()->setValue( "TIPOIVA_ID", getRecTipoIVA()->getRecordID() );
	getRecFacturaVentaDet()->setValue( "CANTIDAD", editCantidad->toDouble());
	getRecFacturaVentaDet()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
	getRecFacturaVentaDet()->setValue( "PVPSINIVA", editPVPSinIVA->toMoney());
	getRecFacturaVentaDet()->setValue( "PVP", editPVP->toMoney());
	getRecFacturaVentaDet()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecFacturaVentaDet()->setValue( "IMPORTE", editImporte->toMoney());
	getRecFacturaVentaDet()->setValue( "IMPORTECONIVA", editImporteConIVA->toMoney());
	getRecFacturaVentaDet()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecFacturaVentaDet()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITFACTURAVENTADET_GATHER*/
}

void FrmEditFacturaVentaDet::scatterTipoIVA()
{
    /*<<<<<FRMEDITFACTURAVENTADET_SCATTER_TIPOIVA*/
	editTipoIVACodigo->setText( getRecTipoIVA()->getValue("CODIGO") );
	editTipoIVANombre->setText( getRecTipoIVA()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURAVENTADET_SCATTER_TIPOIVA*/
}
void FrmEditFacturaVentaDet::pushTipoIVACodigo_clicked()
{
    /*<<<<<FRMEDITFACTURAVENTADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITFACTURAVENTADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
}

void FrmEditFacturaVentaDet::scatterArticulo()
{
    /*<<<<<FRMEDITFACTURAVENTADET_SCATTER_ARTICULO*/
	editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
	editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURAVENTADET_SCATTER_ARTICULO*/
}

void FrmEditFacturaVentaDet::pushArticuloCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURAVENTADET_PUSH_ARTICULO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITFACTURAVENTADET_PUSH_ARTICULO_CODIGO_CLICKED*/
}
void FrmEditFacturaVentaDet::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITFACTURAVENTADET_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoIVACodigo )
		pushTipoIVACodigo_clicked();
	if( sender == editArticuloCodigo )
		pushArticuloCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITFACTURAVENTADET_SPECIALACTION*/
}


void FrmEditFacturaVentaDet::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITFACTURAVENTADET_VALIDATE*/
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
		getRecArticulo(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::FindCodeInDesc )) )
		scatterArticulo();
/*>>>>>FRMEDITFACTURAVENTADET_VALIDATE*/
    /*<<<<<FRMEDITFACTURAVENTADET_DETALLE_VALIDATE*/
	bool actualiza = (sender && (sender == pFocusWidget) );
	double cantidad = editCantidad->toDouble();
	double pvpsiniva = editPVPSinIVA->toDouble();
	double pvpconiva = editPVP->toDouble();
	double dtop100 = editDtoP100->toDouble();
	double importesiniva =  cantidad * pvpsiniva * ( 1 - dtop100 / 100 );
		int tarifacliente = static_cast<FrmEditFacturaVenta *>(getFrmMaster())->getRecCliente()->getValue("TARIFA").toInt();
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
/*>>>>>FRMEDITFACTURAVENTADET_DETALLE_VALIDATE*/
    if( !sender && isvalid && getRecArticulo()->getValue( "PVP" ).toDouble() == 0.0 ) {
        if( editPVP->toDouble() != 0.0 ) {
            getRecArticulo()->setValue("PVPSINIVA", editPVPSinIVA->toMoney() );
            getRecArticulo()->setValue("PVP", editPVP->toMoney() );
            getRecArticulo()->fixMargenYDescuento();
            if( getRecArticulo()->save(false) )
                DBAPP->showStickyOSD( getTitle(), Xtring::printf( _( "Se han actualizado los precios de venta del artículo a los de este detalle: sin IVA: %s, con IVA: %s" ),
                                      getRecArticulo()->getValue( "PVPSINIVA" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
                                      getRecArticulo()->getValue( "PVP" ).toMoney().toString( DBAPP->getRegConfig() ).c_str() ) );
        }
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditFacturaVentaDet::slotInsertTable_clicked()
{
    ModuleInstance->insertDetails( getFrmMaster(), this, "ALBARANVENTA" );
}

void FrmEditFacturaVentaDet::slotAddAlbaran_clicked()
{
    RecAlbaranVenta *recalbaran = static_cast<RecAlbaranVenta *>(DBAPP->createRecord( "ALBARANVENTA" ));
    getFrmMaster()->gather();
    RecFacturaVenta *recfact = static_cast<RecFacturaVenta *>(getFrmMaster()->getRecord());
    recalbaran->removeFilter( "ALBARANVENTA.EJERCICIO=" +
                              factu::ModuleInstance->getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
    recalbaran->addStructuralFilter( "FACTURADO=0" );
    if( recfact->getValue( "CLIENTE_ID" ).toInt() )
        recalbaran->addFilter( "CLIENTE_ID=" + recfact->getConnection()->toSQL( recfact->getValue( "CLIENTE_ID" ).toInt() ) );
    DBAPP->choose(this, recalbaran, 0, dbApplication::editNone, this );
    if( recalbaran->getRecordID() != 0 ) {
        DBAPP->waitCursor( true );
        // Comprobar si el albarán ya está en la factura
        for( dbRecordList::const_iterator itfact = recfact->getListFacturaVentaDet()->begin();
                itfact != recfact->getListFacturaVentaDet()->end();
                ++ itfact ) {
            if( (*itfact)->getValue( "ALBARANVENTA_ID" ).toUInt() == recalbaran->getRecordID() ) {
                msgError( this, _("El albarán elegido ya está en la factura" ) );
                delete recalbaran;
                DBAPP->resetCursor();
                return;
            }
        }
        for( dbRecordList::const_iterator it = recalbaran->getListAlbaranVentaDet()->begin();
                it != recalbaran->getListAlbaranVentaDet()->end();
                ++ it ) {
            if( recfact->isEmpty( "EMPRESA_ID,EJERCICIO,FECHA,IVADETALLADO" ) ) {
                recfact->setValue( "TIPODOC_ID", recalbaran->getValue( "TIPODOC_ID" ) );
                recfact->setValue( "CLIENTE_ID", recalbaran->getValue( "CLIENTE_ID" ) );
                recfact->setValue( "AGENTE_ID", recalbaran->getValue( "AGENTE_ID" ) );
                recfact->setValue( "IVADETALLADO", recalbaran->getValue( "IVADETALLADO" ) );
                recfact->readRelated( true );
                getFrmMaster()->scatter();
            }
            unsigned int numrecords = recfact->getListFacturaVentaDet()->size();
            RecFacturaVentaDet *factudet;
            bool add = false;
            if( numrecords > 0  && recfact->getListFacturaVentaDet()->getRecord( numrecords - 1 )->isEmpty() ) {
                factudet = static_cast<RecFacturaVentaDet *>(recfact->getListFacturaVentaDet()->getRecord( numrecords - 1 ));
            } else {
                factudet = static_cast<RecFacturaVentaDet *>(DBAPP->createRecord( "FACTURAVENTADET" ));
                add = true;
            }
            factudet->copyRecord( *it, false, Xtring::null, "ID,ALBARANVENTA_ID,NLINEA" );
            factudet->readRelated( true );
            factudet->setValue( "ALBARANVENTA_ID", recalbaran->getRecordID() );
            if( add )
                recfact->getListFacturaVentaDet()->addRecord( factudet );
        }
        getFrmMaster()->updateFromDetails( this );
        getFrmMaster()->setEdited( true );
        refresh();
        DBAPP->resetCursor();
    }
    delete recalbaran;
}

void FrmEditFacturaVentaDet::updateStatus( bool callbehaviors )
{
    FrmEditRecDetail::updateStatus( callbehaviors );
    pushAddAlbaran->setVisible( pMenuRecordAdd->isEnabled() && isBrowsing() );
    pushInsertTable->setVisible( pMenuRecordAdd->isEnabled() );
    pushActPrecioArticulo->setVisible( isEditing() );
}

bool FrmEditFacturaVentaDet::canBeginEdit(DataTable::EditMode newmode)
{
    dbRecordID alb_id;
    if( (alb_id = getRecFacturaVentaDet()->getValue( "ALBARANVENTA_ID" ).toInt()) != 0 ) {
        if( newmode == DataTable::deleting ) {
            if( msgYesNo(this, "Atención", "¿Estás segura de que quieres eliminar el albarán completo de esta factura") ) {
                // Eliminar todos los detalles de este albarán, del final al inicio para que se borre bien
                for( int i = (int)getDetalles()->size() - 1; i >= 0;  --i ) {
                    dbRecord *r = getDetalles()->getRecord( i );
                    if( (dbRecordID)r->getValue( "ALBARANVENTA_ID" ).toInt() == alb_id ) {
                        getDetalles()->removeRecord(i);
                        delete r;
                    }
                }
                getFrmMaster()->updateFromDetails( this );
                getFrmMaster()->setEdited( true );
                refresh();
            }
            return false;
        } else if( newmode == DataTable::selecting
                   || newmode == DataTable::inserting
                   || newmode == DataTable::duplicating ) {
            return true;
        } else {
            DBAPP->showOSD("Atención", "No se puede modificar este detalle porque proviene de un albarán" );
            return false;
        }
    } else {
        return true;
    }
}

void FrmEditFacturaVentaDet::slotActPrecioArticulo_clicked()
{
	if( ModuleInstance->editPVPsArticulo(this, getRecArticulo(), 
		static_cast<FrmEditFacturaVenta *>(getFrmMaster())->getRecCliente(),
		editPVP->toDouble() ) ) {
		searchArticuloCodigo->setValue( getRecArticulo()->getValue("CODIGO").toString() );
	}
}


/*<<<<<FRMEDITFACTURAVENTADET_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITFACTURAVENTADET_FIN*/

/*<<<<<COPYLEFT*/
/** @file factufrmeditfacturacompradet.cpp Fichero de edición de detalles de factura de compra
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de detalles de factura de compra
// FIELD NLinea int - pre
// FIELD TipoIVA_ID Reference(empresa::TipoIVA,Codigo,Nombre) - pre
// FIELD Cantidad double - cantidad
// FIELD Articulo_ID Reference(Articulo,Codigo,Nombre,dbApplication::FindCodeInDesc) - cantidad
// FIELD CosteSinIVA money - precios
// FIELD Coste money - precios
// FIELD DtoP100 double - precios
// FIELD Importe money - precios
// FIELD ImporteConIVA money - precios
// FIELD Descripcion text
// FIELD Notas text
// TYPE FrmEditRecDetail factu::FacturaCompraDet FacturaDet Compra
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITFACTURACOMPRADET_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditfacturacompradet.h"
/*>>>>>FRMEDITFACTURACOMPRADET_INCLUDES*/
#include <empresamodule.h>
#include "factufrmeditfacturacompra.h"
#include "factufldivadetallado.h"
#include "facturecpedidocompra.h"
#include "facturecalbarancompra.h"
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITFACTURACOMPRADET_CONSTRUCTOR*/
FrmEditFacturaCompraDet::FrmEditFacturaCompraDet(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditFacturaCompraDet" );
/*>>>>>FRMEDITFACTURACOMPRADET_CONSTRUCTOR*/
    /*<<<<<FRMEDITFACTURACOMPRADET_INIT_CONTROLS*/
	QHBoxLayout *preLayout = new QHBoxLayout(0, 0, 6, "preLayout");
	QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
	QHBoxLayout *preciosLayout = new QHBoxLayout(0, 0, 6, "preciosLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNLinea = addEditField( pControlsFrame, "FACTURACOMPRADET", "NLINEA", preLayout );

	searchTipoIVACodigo = addSearchField( pControlsFrame, "TIPOIVA_ID", "TIPOIVA", "CODIGO", "NOMBRE", preLayout );
	pushTipoIVACodigo = searchTipoIVACodigo->getButton();
	connect( pushTipoIVACodigo, SIGNAL( clicked() ), this, SLOT( pushTipoIVACodigo_clicked() ) );
	editTipoIVACodigo = searchTipoIVACodigo->getEditCode();
	editTipoIVANombre = searchTipoIVACodigo->getEditDesc();
	editCantidad = addEditField( pControlsFrame, "FACTURACOMPRADET", "CANTIDAD", cantidadLayout );

	searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", cantidadLayout );
	pushArticuloCodigo = searchArticuloCodigo->getButton();
	connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
	editArticuloCodigo = searchArticuloCodigo->getEditCode();
	editArticuloNombre = searchArticuloCodigo->getEditDesc();
	editCosteSinIVA = addEditField( pControlsFrame, "FACTURACOMPRADET", "COSTESINIVA", preciosLayout );
	editCoste = addEditField( pControlsFrame, "FACTURACOMPRADET", "COSTE", preciosLayout );
	editDtoP100 = addEditField( pControlsFrame, "FACTURACOMPRADET", "DTOP100", preciosLayout );
	editImporte = addEditField( pControlsFrame, "FACTURACOMPRADET", "IMPORTE", preciosLayout );
	editImporteConIVA = addEditField( pControlsFrame, "FACTURACOMPRADET", "IMPORTECONIVA", preciosLayout );
	editDescripcion = addTextField( pControlsFrame, "FACTURACOMPRADET", "DESCRIPCION", descripcionLayout );
	editNotas = addTextField( pControlsFrame, "FACTURACOMPRADET", "NOTAS", notasLayout );
	pControlsLayout->addLayout( preLayout );
	pControlsLayout->addLayout( cantidadLayout );
	pControlsLayout->addLayout( preciosLayout );
	pControlsLayout->addLayout( descripcionLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITFACTURACOMPRADET_INIT_CONTROLS*/
    editNLinea->setMustBeReadOnly( true );
	editArticuloCodigo->setWidthInChars(12);

    pushActPrecioArticulo = new QPushButton(this, "pushActPrecioArticulo" );
    pushActPrecioArticulo->setText( toGUI( _( "Act. coste artículo" ) ) );
    connect( pushActPrecioArticulo, SIGNAL( clicked() ), this, SLOT( slotActPrecioArticulo_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushActPrecioArticulo );

    pushInsertTable = new QPushButton(this, "pushInsertTable" );
    pushInsertTable->setText( toGUI( _( "Insertar..." ) ) );
    connect( pushInsertTable, SIGNAL( clicked() ), this, SLOT( slotInsertTable_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushInsertTable );

    pushAddAlbaran = new QPushButton(this, "pushAddAlbaran" );
    pushAddAlbaran->setText( toGUI( _( "&Facturar albarán" ) ) );
    connect( pushAddAlbaran, SIGNAL( clicked() ), this, SLOT( slotAddAlbaran_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushAddAlbaran );
}

void FrmEditFacturaCompraDet::scatterFields()
{
    /*<<<<<FRMEDITFACTURACOMPRADET_SCATTER*/
	editNLinea->setText(getRecFacturaCompraDet()->getValue("NLINEA").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNLinea;
	editCantidad->setText(getRecFacturaCompraDet()->getValue("CANTIDAD").toDouble());
	editCosteSinIVA->setText(getRecFacturaCompraDet()->getValue("COSTESINIVA").toMoney());
	editCoste->setText(getRecFacturaCompraDet()->getValue("COSTE").toMoney());
	editDtoP100->setText(getRecFacturaCompraDet()->getValue("DTOP100").toDouble());
	editImporte->setText(getRecFacturaCompraDet()->getValue("IMPORTE").toMoney());
	editImporteConIVA->setText(getRecFacturaCompraDet()->getValue("IMPORTECONIVA").toMoney());
	editDescripcion->setText(getRecFacturaCompraDet()->getValue("DESCRIPCION").toString());
	editNotas->setText(getRecFacturaCompraDet()->getValue("NOTAS").toString());
	scatterTipoIVA();
	scatterArticulo();
/*>>>>>FRMEDITFACTURACOMPRADET_SCATTER*/
    if( ModuleInstance->getModuleSetting( "FILTRAARTICULOSPORPROVEEDORA" ).toBool() ) {
        dbRecordID prov_id = getFrmMaster()->getRecord()->getValue("PROVEEDORA_ID").toInt();
        if( prov_id ) {
            Xtring filter = "PROVEEDORA_ID IN(" + getRecArticulo()->getConnection()->toSQL( prov_id );
            int codpro = ModuleInstance->getModuleSetting( "CODIGO_PROVEEDORA_GENERICA" ).toInt();
            if( codpro != 0 ) {
                dbRecordID pro_id = getRecArticulo()->getConnection()->selectInt(
                                        "SELECT ID FROM PROVEEDORA WHERE CODIGO=" + getRecArticulo()->getConnection()->toSQL( codpro ) );
                if( pro_id != 0 )
                    filter += "," + getRecArticulo()->getConnection()->toSQL( pro_id );
            }
            filter += ")";
            getRecArticulo()->addFilter( filter );
        }
    }
    getRecArticulo()->addFilter( "ARTICULO.ENUSO=1" );
}

void FrmEditFacturaCompraDet::gatherFields()
{
    /*<<<<<FRMEDITFACTURACOMPRADET_GATHER*/
	getRecFacturaCompraDet()->setValue( "NLINEA", editNLinea->toInt());
	getRecFacturaCompraDet()->setValue( "TIPOIVA_ID", getRecTipoIVA()->getRecordID() );
	getRecFacturaCompraDet()->setValue( "CANTIDAD", editCantidad->toDouble());
	getRecFacturaCompraDet()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
	getRecFacturaCompraDet()->setValue( "COSTESINIVA", editCosteSinIVA->toMoney());
	getRecFacturaCompraDet()->setValue( "COSTE", editCoste->toMoney());
	getRecFacturaCompraDet()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecFacturaCompraDet()->setValue( "IMPORTE", editImporte->toMoney());
	getRecFacturaCompraDet()->setValue( "IMPORTECONIVA", editImporteConIVA->toMoney());
	getRecFacturaCompraDet()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecFacturaCompraDet()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITFACTURACOMPRADET_GATHER*/
}


void FrmEditFacturaCompraDet::scatterTipoIVA()
{
    /*<<<<<FRMEDITFACTURACOMPRADET_SCATTER_TIPOIVA*/
	editTipoIVACodigo->setText( getRecTipoIVA()->getValue("CODIGO") );
	editTipoIVANombre->setText( getRecTipoIVA()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURACOMPRADET_SCATTER_TIPOIVA*/
}
void FrmEditFacturaCompraDet::pushTipoIVACodigo_clicked()
{
    /*<<<<<FRMEDITFACTURACOMPRADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
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
				editTipoIVACodigo->setFocus();
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
					editTipoIVACodigo->setFocus();
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
				editTipoIVACodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFACTURACOMPRADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
}

void FrmEditFacturaCompraDet::scatterArticulo()
{
    /*<<<<<FRMEDITFACTURACOMPRADET_SCATTER_ARTICULO*/
	editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
	editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
/*>>>>>FRMEDITFACTURACOMPRADET_SCATTER_ARTICULO*/
}
void FrmEditFacturaCompraDet::pushArticuloCodigo_clicked()
{
    /*<<<<<FRMEDITFACTURACOMPRADET_PUSH_ARTICULO_CODIGO_CLICKED*/
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
				editArticuloCodigo->setFocus();
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
					editArticuloCodigo->setFocus();
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
				editArticuloCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFACTURACOMPRADET_PUSH_ARTICULO_CODIGO_CLICKED*/
}
void FrmEditFacturaCompraDet::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITFACTURACOMPRADET_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoIVACodigo )
		pushTipoIVACodigo_clicked();
	if( sender == editArticuloCodigo )
		pushArticuloCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITFACTURACOMPRADET_SPECIALACTION*/
}

void FrmEditFacturaCompraDet::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITFACTURACOMPRADET_VALIDATE*/
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
/*>>>>>FRMEDITFACTURACOMPRADET_VALIDATE*/
    /*<<<<<FRMEDITFACTURACOMPRADET_DETALLE_VALIDATE*/
	bool actualiza = (sender && (sender == pFocusWidget) );
	double cantidad = editCantidad->toDouble();
	double costesiniva = editCosteSinIVA->toDouble();
	double costeconiva = editCoste->toDouble();
	double dtop100 = editDtoP100->toDouble();
	double importesiniva =  cantidad * costesiniva * ( 1 - dtop100 / 100 );
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
		costeconiva = getRecArticulo()->getValue( "Coste" ).toDouble();
		// El precio con IVA se calcula tomando el tipo de iva de este detalle
		costesiniva = costeconiva / ( 1 + getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 );
	}
	if( (sender == editTipoIVACodigo && editTipoIVACodigo->isJustEdited())
		|| (sender == editCosteSinIVA && editCosteSinIVA->isJustEdited()) ) {
		actualiza = true;
		costesiniva = editCosteSinIVA->toDouble();
		costeconiva = costesiniva * ( 1 + getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 );
	}
	if ( sender == editCoste && editCoste->isJustEdited() ) {
		actualiza = true;
		costeconiva = editCoste->toDouble();
		costesiniva = costeconiva / ( 1 + getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 );
	}
	if ( sender == editDtoP100 && editDtoP100->isJustEdited() ) {
		actualiza = true;
		dtop100 = editDtoP100->toDouble();
	}
	if( sender == editImporte && editImporte->isJustEdited() ) {
		importesiniva = editImporte->toDouble();
		if( cantidad != 0.0 && costesiniva != 0.0 ) {
			dtop100 = 100 * ( 1 - importesiniva / (cantidad * costesiniva) );
			editDtoP100->setText( dtop100 );
		}
		actualiza = true;
	}
	if( sender == editImporteConIVA && editImporteConIVA->isJustEdited() ) {
		if( cantidad != 0.0 && costesiniva != 0.0 ) {
			dtop100 = 100 * ( 1 - editImporteConIVA->toDouble() / (cantidad * costeconiva) );
			editDtoP100->setText( dtop100 );
		}
		actualiza = true;
	}
	if( actualiza ) {
		editCosteSinIVA->setText( costesiniva );
		editCoste->setText( costeconiva );
		int ndec = empresa::ModuleInstance->getDecimalesMoneda();
		if( getFrmMaster()->getControlValue( "IVADETALLADO" ).toInt() == factu::FldIVADetallado::incluido )
			ndec += 2;
		Money importesiniva(cantidad * costesiniva * ( 1 - dtop100 / 100 ), ndec);
		Money importeconiva = Money( cantidad * costeconiva * ( 1 - dtop100 / 100 ),
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
			precios_msg = _( "%s: PVP: %s, Coste: %s, Existencias: %s" );
			DBAPP->showOSD( getTitle(), Xtring::printf( precios_msg,
		                       getRecArticulo()->getValue( "CODIGO" ).toString().c_str(),
		                       getRecArticulo()->getValue( "PVP" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
		                       editCoste->toMoney().toString( DBAPP->getRegConfig() ).c_str(),
		                       stock_msg.c_str() ) );
		}
	}
/*>>>>>FRMEDITFACTURACOMPRADET_DETALLE_VALIDATE*/
    if( !sender && isvalid && getRecArticulo()->getValue( "COSTE" ).toDouble() == 0.0 ) {
        if( editCoste->toDouble() != 0.0 ) {
            getRecArticulo()->setValue("COSTESINIVA", editCosteSinIVA->toMoney() );
            getRecArticulo()->setValue("COSTE", editCoste->toMoney() );
            getRecArticulo()->fixMargenYDescuento();
            if( getRecArticulo()->save(false) )
                DBAPP->showStickyOSD( getTitle(), Xtring::printf( _( "Se han actualizado los costes del artículo a los de este detalle: sin IVA: %s, con IVA: %s" ),
                                      getRecArticulo()->getValue( "COSTESINIVA" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
                                      getRecArticulo()->getValue( "COSTE" ).toMoney().toString( DBAPP->getRegConfig() ).c_str() ) );
        }
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditFacturaCompraDet::slotInsertTable_clicked()
{
    ModuleInstance->insertDetails( getFrmMaster(), this, "ALBARANCOMPRA" );
}

void FrmEditFacturaCompraDet::slotAddAlbaran_clicked()
{
    RecAlbaranCompra *recalbaran = static_cast<RecAlbaranCompra *>(DBAPP->createRecord( "ALBARANCOMPRA" ));
    getFrmMaster()->gather();
    RecFacturaCompra *recfact = static_cast<RecFacturaCompra *>(getFrmMaster()->getRecord());
    recalbaran->removeFilter( "ALBARANCOMPRA.EJERCICIO=" +
                              factu::ModuleInstance->getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
    recalbaran->addStructuralFilter( "FACTURADO=0" );
    if( recfact->getValue( "PROVEEDORA_ID" ).toInt() )
        recalbaran->addFilter( "PROVEEDORA_ID=" + recfact->getConnection()->toSQL( recfact->getValue( "PROVEEDORA_ID" ).toInt() ) );
    DBAPP->choose(this, recalbaran, 0, dbApplication::editNone, this );
    if( recalbaran->getRecordID() != 0 ) {
        DBAPP->waitCursor( true );
        // Comprobar si el albarán ya está en la factura
        for( dbRecordList::const_iterator itfact = recfact->getListFacturaCompraDet()->begin();
                itfact != recfact->getListFacturaCompraDet()->end();
                ++ itfact ) {
            if( (*itfact)->getValue( "ALBARANCOMPRA_ID" ).toUInt() == recalbaran->getRecordID() ) {
                msgError( this, _("El albarán elegido ya está en la factura" ) );
                delete recalbaran;
                DBAPP->resetCursor();
                return;
            }
        }
        for( dbRecordList::const_iterator it = recalbaran->getListAlbaranCompraDet()->begin();
                it != recalbaran->getListAlbaranCompraDet()->end();
                ++ it ) {
            if( recfact->isEmpty( "EMPRESA_ID,EJERCICIO,FECHA,IVADETALLADO" ) ) {
                recfact->setValue( "TIPODOC_ID", recalbaran->getValue( "TIPODOC_ID" ) );
                recfact->setValue( "PROVEEDORA_ID", recalbaran->getValue( "PROVEEDORA_ID" ) );
                recfact->setValue( "IVADETALLADO", recalbaran->getValue( "IVADETALLADO" ) );
                recfact->readRelated( true );
                getFrmMaster()->scatter();
            }
            unsigned int numrecords = recfact->getListFacturaCompraDet()->size();
            RecFacturaCompraDet *factudet;
            bool add = false;
            if( numrecords > 0  && recfact->getListFacturaCompraDet()->getRecord( numrecords - 1 )->isEmpty() ) {
                factudet = static_cast<RecFacturaCompraDet *>(recfact->getListFacturaCompraDet()->getRecord( numrecords - 1 ));
            } else {
                factudet = static_cast<RecFacturaCompraDet *>(DBAPP->createRecord( "FACTURACOMPRADET" ));
                add = true;
            }
            factudet->copyRecord( *it, false, Xtring::null, "ID,ALBARANCOMPRA_ID,NLINEA"  );
            factudet->readRelated( true ); // No son de la misma tabla, así que no se copian las relaciones
            factudet->setValue( "ALBARANCOMPRA_ID", recalbaran->getRecordID() );
            if( add )
                recfact->getListFacturaCompraDet()->addRecord( factudet );
        }
        getFrmMaster()->updateFromDetails( this );
        getFrmMaster()->setEdited( true );
        refresh();
        DBAPP->resetCursor();
    }
    delete recalbaran;
}

void FrmEditFacturaCompraDet::updateStatus( bool callbehaviors )
{
    FrmEditRecDetail::updateStatus( callbehaviors );
    pushAddAlbaran->setVisible( pMenuRecordAdd->isEnabled() && isBrowsing() );
    pushInsertTable->setVisible( pMenuRecordAdd->isEnabled() );
    pushActPrecioArticulo->setVisible( isEditing() );
}

bool FrmEditFacturaCompraDet::canBeginEdit(DataTable::EditMode newmode)
{
    dbRecordID alb_id = getRecFacturaCompraDet()->getValue( "ALBARANCOMPRA_ID" ).toInt();
    if( alb_id != 0 ) {
        if( newmode == DataTable::deleting ) {
            if( msgYesNo(this, "Atención", "¿Estás segura de que quieres eliminar el albarán completo de esta factura") ) {
                // Eliminar todos los detalles de este albarán
                for( int i = (int)getDetalles()->size() - 1; i >= 0;  --i ) {
                    dbRecord *r = getDetalles()->getRecord( i );
                    if( (dbRecordID)r->getValue( "ALBARANCOMPRA_ID" ).toInt() == alb_id ) {
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

void FrmEditFacturaCompraDet::slotActPrecioArticulo_clicked()
{
    if( getRecArticulo()->getRecordID() != 0 ) {
        if( editCosteSinIVA->toDouble() != getRecArticulo()->getValue("COSTESINIVA").toDouble() ) {
            getRecArticulo()->setValue("COSTESINIVA", editCosteSinIVA->toMoney() );
            getRecArticulo()->setValue("COSTE", editCoste->toMoney() );
            getRecArticulo()->fixMargenYDescuento();
            if( getRecArticulo()->save(false) ) {
                DBAPP->showOSD( getTitle(), Xtring::printf( _( "Se han actualizado los costes del artículo a los de este detalle: sin IVA: %s, con IVA: %s" ),
                                getRecArticulo()->getValue( "COSTESINIVA" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
                                getRecArticulo()->getValue( "COSTE" ).toMoney().toString( DBAPP->getRegConfig() ).c_str() ) );
                searchArticuloCodigo->setValue( getRecArticulo()->getValue("CODIGO").toString() );
            }
        } else {
            DBAPP->showOSD( getTitle(), _("El coste del artículo coincide con el de este detalle.") );
        }
    }
}


/*<<<<<FRMEDITFACTURACOMPRADET_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITFACTURACOMPRADET_FIN*/

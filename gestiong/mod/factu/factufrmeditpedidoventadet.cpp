/*<<<<<COPYLEFT*/
/** @file factufrmeditpedidoventadet.cpp Fichero de edición de detalles de pedidos de venta
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
// COPYLEFT Fichero de edición de detalles de pedidos de venta
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
// TYPE FrmEditRecDetail factu::PedidoVentaDet PedidoDet Venta
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITPEDIDOVENTADET_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditpedidoventadet.h"
/*>>>>>FRMEDITPEDIDOVENTADET_INCLUDES*/
#include <empresamodule.h>
#include "factufrmeditpedidoventa.h"
#include "factufrmeditpresupuestoventa.h"
#include "factufldivadetallado.h"
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITPEDIDOVENTADET_CONSTRUCTOR*/
FrmEditPedidoVentaDet::FrmEditPedidoVentaDet(FrmEditRecMaster *frmmaster, int ndetail,
        dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
        EditMode editmode, dbApplication::EditFlags editflags,
        QWidget* parent, const char* name, WidgetFlags fl )
    : FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
    if ( !name )
        setName( "FrmEditPedidoVentaDet" );
    /*>>>>>FRMEDITPEDIDOVENTADET_CONSTRUCTOR*/
    /*<<<<<FRMEDITPEDIDOVENTADET_INIT_CONTROLS*/
    QHBoxLayout *preLayout = new QHBoxLayout(0, 0, 6, "preLayout");
    QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
    QHBoxLayout *preciosLayout = new QHBoxLayout(0, 0, 6, "preciosLayout");
    QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
    QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
    editNLinea = addEditField( pControlsFrame, "PEDIDOVENTADET", "NLINEA", preLayout );

    searchTipoIVACodigo = addSearchField( pControlsFrame, "TIPOIVA_ID", "TIPOIVA", "CODIGO", "NOMBRE", preLayout );
    pushTipoIVACodigo = searchTipoIVACodigo->getButton();
    connect( pushTipoIVACodigo, SIGNAL( clicked() ), this, SLOT( pushTipoIVACodigo_clicked() ) );
    editTipoIVACodigo = searchTipoIVACodigo->getEditCode();
    editTipoIVANombre = searchTipoIVACodigo->getEditDesc();
    editCantidad = addEditField( pControlsFrame, "PEDIDOVENTADET", "CANTIDAD", cantidadLayout );

    searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", cantidadLayout );
    pushArticuloCodigo = searchArticuloCodigo->getButton();
    connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
    editArticuloCodigo = searchArticuloCodigo->getEditCode();
    editArticuloNombre = searchArticuloCodigo->getEditDesc();
    editPVPSinIVA = addEditField( pControlsFrame, "PEDIDOVENTADET", "PVPSINIVA", preciosLayout );
    editPVP = addEditField( pControlsFrame, "PEDIDOVENTADET", "PVP", preciosLayout );
    editDtoP100 = addEditField( pControlsFrame, "PEDIDOVENTADET", "DTOP100", preciosLayout );
    editImporte = addEditField( pControlsFrame, "PEDIDOVENTADET", "IMPORTE", preciosLayout );
    editImporteConIVA = addEditField( pControlsFrame, "PEDIDOVENTADET", "IMPORTECONIVA", preciosLayout );
    editDescripcion = addTextField( pControlsFrame, "PEDIDOVENTADET", "DESCRIPCION", descripcionLayout );
    editNotas = addTextField( pControlsFrame, "PEDIDOVENTADET", "NOTAS", notasLayout );
    pControlsLayout->addLayout( preLayout );
    pControlsLayout->addLayout( cantidadLayout );
    pControlsLayout->addLayout( preciosLayout );
    pControlsLayout->addLayout( descripcionLayout );
    pControlsLayout->addLayout( notasLayout );
    /*>>>>>FRMEDITPEDIDOVENTADET_INIT_CONTROLS*/
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

void FrmEditPedidoVentaDet::scatterFields()
{
    /*<<<<<FRMEDITPEDIDOVENTADET_SCATTER*/
    editNLinea->setText(getRecPedidoVentaDet()->getValue("NLINEA").toInt());
    if( isEditing() && (pFocusWidget == 0) )
        pFocusWidget = editNLinea;
    editCantidad->setText(getRecPedidoVentaDet()->getValue("CANTIDAD").toDouble());
    editPVPSinIVA->setText(getRecPedidoVentaDet()->getValue("PVPSINIVA").toMoney());
    editPVP->setText(getRecPedidoVentaDet()->getValue("PVP").toMoney());
    editDtoP100->setText(getRecPedidoVentaDet()->getValue("DTOP100").toDouble());
    editImporte->setText(getRecPedidoVentaDet()->getValue("IMPORTE").toMoney());
    editImporteConIVA->setText(getRecPedidoVentaDet()->getValue("IMPORTECONIVA").toMoney());
    editDescripcion->setText(getRecPedidoVentaDet()->getValue("DESCRIPCION").toString());
    editNotas->setText(getRecPedidoVentaDet()->getValue("NOTAS").toString());
    scatterTipoIVA();
    scatterArticulo();
    /*>>>>>FRMEDITPEDIDOVENTADET_SCATTER*/
    if ( isInserting() ) {
        double cli_dto = static_cast<FrmEditPedidoVenta *>( getFrmMaster() )->getRecCliente()->getValue( "DTOP100" ).toDouble();
        editDtoP100->setText( cli_dto );
    }
    getRecArticulo()->addFilter( "ARTICULO.ENUSO=1" );
}

void FrmEditPedidoVentaDet::gatherFields()
{
    /*<<<<<FRMEDITPEDIDOVENTADET_GATHER*/
    getRecPedidoVentaDet()->setValue( "NLINEA", editNLinea->toInt());
    getRecPedidoVentaDet()->setValue( "TIPOIVA_ID", getRecTipoIVA()->getRecordID() );
    getRecPedidoVentaDet()->setValue( "CANTIDAD", editCantidad->toDouble());
    getRecPedidoVentaDet()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
    getRecPedidoVentaDet()->setValue( "PVPSINIVA", editPVPSinIVA->toMoney());
    getRecPedidoVentaDet()->setValue( "PVP", editPVP->toMoney());
    getRecPedidoVentaDet()->setValue( "DTOP100", editDtoP100->toDouble());
    getRecPedidoVentaDet()->setValue( "IMPORTE", editImporte->toMoney());
    getRecPedidoVentaDet()->setValue( "IMPORTECONIVA", editImporteConIVA->toMoney());
    getRecPedidoVentaDet()->setValue( "DESCRIPCION", editDescripcion->toString());
    getRecPedidoVentaDet()->setValue( "NOTAS", editNotas->toString());
    /*>>>>>FRMEDITPEDIDOVENTADET_GATHER*/
}



void FrmEditPedidoVentaDet::scatterTipoIVA()
{
    /*<<<<<FRMEDITPEDIDOVENTADET_SCATTER_TIPOIVA*/
    editTipoIVACodigo->setText( getRecTipoIVA()->getValue("CODIGO") );
    editTipoIVANombre->setText( getRecTipoIVA()->getValue("NOMBRE") );
    /*>>>>>FRMEDITPEDIDOVENTADET_SCATTER_TIPOIVA*/
}
void FrmEditPedidoVentaDet::pushTipoIVACodigo_clicked()
{
    /*<<<<<FRMEDITPEDIDOVENTADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
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
    /*>>>>>FRMEDITPEDIDOVENTADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
}

void FrmEditPedidoVentaDet::scatterArticulo()
{
    /*<<<<<FRMEDITPEDIDOVENTADET_SCATTER_ARTICULO*/
    editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
    editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
    /*>>>>>FRMEDITPEDIDOVENTADET_SCATTER_ARTICULO*/
}
void FrmEditPedidoVentaDet::pushArticuloCodigo_clicked()
{
    /*<<<<<FRMEDITPEDIDOVENTADET_PUSH_ARTICULO_CODIGO_CLICKED*/
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
    /*>>>>>FRMEDITPEDIDOVENTADET_PUSH_ARTICULO_CODIGO_CLICKED*/
}
void FrmEditPedidoVentaDet::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITPEDIDOVENTADET_SPECIALACTION*/
    mControlKeyPressed = key;
    FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
    if( sender == editTipoIVACodigo )
        pushTipoIVACodigo_clicked();
    if( sender == editArticuloCodigo )
        pushArticuloCodigo_clicked();
    mControlKeyPressed = '\0';
    /*>>>>>FRMEDITPEDIDOVENTADET_SPECIALACTION*/
}

void FrmEditPedidoVentaDet::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPEDIDOVENTADET_VALIDATE*/
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
    /*>>>>>FRMEDITPEDIDOVENTADET_VALIDATE*/
    /*<<<<<FRMEDITPEDIDOVENTADET_DETALLE_VALIDATE*/
    bool actualiza = (sender && (sender == pFocusWidget) );
    double cantidad = editCantidad->toDouble();
    double pvpsiniva = editPVPSinIVA->toDouble();
    double pvpconiva = editPVP->toDouble();
    double dtop100 = editDtoP100->toDouble();
    double importesiniva =  cantidad * pvpsiniva * ( 1 - dtop100 / 100 );
    int tarifacliente = static_cast<FrmEditPedidoVenta *>(getFrmMaster())->getRecCliente()->getValue("TARIFA").toInt();
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
    /*>>>>>FRMEDITPEDIDOVENTADET_DETALLE_VALIDATE*/
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditPedidoVentaDet::slotInsertTable_clicked()
{
    ModuleInstance->insertDetails( getFrmMaster(), this );
}

void FrmEditPedidoVentaDet::updateStatus( bool callbehaviors )
{
    FrmEditRecDetail::updateStatus(callbehaviors);
    pushInsertTable->setVisible( pMenuRecordAdd->isEnabled() );
    pushActPrecioArticulo->setVisible( isEditing() );
}

void FrmEditPedidoVentaDet::slotActPrecioArticulo_clicked()
{
    if( getRecArticulo()->getRecordID() != 0 ) {
        int tarifacliente = static_cast<FrmEditPedidoVenta *>(getFrmMaster())->getRecCliente()->getValue("TARIFA").toInt();
        double pvpconiva = getRecArticulo()->getPVP( tarifacliente );
        if( editPVP->toDouble() != pvpconiva ) {
            if( tarifacliente == 0 ) {
                getRecArticulo()->setValue("PVPSINIVA", editPVPSinIVA->toMoney() );
                getRecArticulo()->setValue("PVP", editPVP->toMoney() );
                getRecArticulo()->fixMargenYDescuento();
                if( getRecArticulo()->save(false) ) {
                    DBAPP->showOSD( getTitle(), Xtring::printf( _( "Se ha actualizado el PVP del artículo: sin IVA: %s, con IVA: %s" ),
                                    getRecArticulo()->getValue( "PVPSINIVA" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
                                    editPVP->toMoney().toString( DBAPP->getRegConfig() ).c_str() ) );
                }
            } else {
                getRecArticulo()->setPVP( tarifacliente, pvpconiva );
                if( getRecArticulo()->save(false) ) {
                    DBAPP->showOSD( getTitle(), Xtring::printf( _( "Se ha actualizado el PVP del artículo PARA ESTE CLIENTE: %s" ),
                                    editPVP->toMoney().toString( DBAPP->getRegConfig() ).c_str() ) );
                }
            }
        } else {
            DBAPP->showOSD( getTitle(), _("El PVP del artículo coincide con el de este detalle.") );
        }
    }
}


/*<<<<<FRMEDITPEDIDOVENTADET_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITPEDIDOVENTADET_FIN*/

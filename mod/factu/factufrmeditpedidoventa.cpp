/*<<<<<COPYLEFT*/
/** @file factufrmeditpedidoventa.cpp Fichero de edición de pedidos de venta
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
// COPYLEFT Fichero de edición de pedidos de venta
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(TipoDoc,Codigo,Nombre) - cabecera
// FIELD Contador int - cabecera
// FIELD Numero string - cabecera
// FIELD Cliente_ID Reference(Cliente,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - cabecera2
// FIELD IVADetallado comboint - cabecera2
// FIELD Agente_ID Reference(Agente,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - cabecera3
// FIELD EstadoPedido comboint - cabecera3
// FIELD PedidoVentaDet FrmEditRecDetail
// FIELD Referencia string - noaddrightReferencia
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD IVA money - noaddrightIVA
// FIELD Total money - noaddrightTotal
// FIELD DesgloseIVA string - desgloseiva
// FIELD Notas text
// TYPE FrmEditRecMaster factu::PedidoVenta Pedido Venta
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITPEDIDOVENTA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditpedidoventa.h"
/*>>>>>FRMEDITPEDIDOVENTA_INCLUDES*/
#include <empresamodule.h>
#include "factumodule.h"
#include "factufldivadetallado.h"


namespace gong {
namespace factu {

/*<<<<<FRMEDITPEDIDOVENTA_CONSTRUCTOR*/
FrmEditPedidoVenta::FrmEditPedidoVenta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
                                       EditMode editmode, dbApplication::EditFlags editflags,
                                       QWidget *parent, const char* name, WidgetFlags fl )
    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
    if ( !name )
        setName( "FrmEditPedidoVenta" );
    /*>>>>>FRMEDITPEDIDOVENTA_CONSTRUCTOR*/
    /*<<<<<FRMEDITPEDIDOVENTA_INIT_CONTROLS*/
    QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
    QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
    QHBoxLayout *cabecera3Layout = new QHBoxLayout(0, 0, 6, "cabecera3Layout");
    QHBoxLayout *pedidoventadetLayout = new QHBoxLayout(0, 0, 6, "pedidoventadetLayout");
    QHBoxLayout *rightReferenciaLayout = new QHBoxLayout(0, 0, 6, "rightReferenciaLayout");
    QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
    QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
    QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
    QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
    QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
    QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
    QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
    QHBoxLayout *desgloseivaLayout = new QHBoxLayout(0, 0, 6, "desgloseivaLayout");
    QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
    editFecha = addEditField( pControlsFrame, "PEDIDOVENTA", "FECHA", cabeceraLayout );

    searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
    pushTipoDocCodigo = searchTipoDocCodigo->getButton();
    connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
    editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
    editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
    editContador = addEditField( pControlsFrame, "PEDIDOVENTA", "CONTADOR", cabeceraLayout );
    editNumero = addEditField( pControlsFrame, "PEDIDOVENTA", "NUMERO", cabeceraLayout );

    searchClienteCodigo = addSearchField( pControlsFrame, "CLIENTE_ID", "CLIENTE", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
    pushClienteCodigo = searchClienteCodigo->getButton();
    connect( pushClienteCodigo, SIGNAL( clicked() ), this, SLOT( pushClienteCodigo_clicked() ) );
    editClienteCodigo = searchClienteCodigo->getEditCode();
    editClienteRazonSocial = searchClienteCodigo->getEditDesc();
    comboIVADetallado = addComboField<int>( pControlsFrame, "PEDIDOVENTA", "IVADETALLADO", cabecera2Layout );

    searchAgenteCodigo = addSearchField( pControlsFrame, "AGENTE_ID", "AGENTE", "CODIGO", "RAZONSOCIAL", cabecera3Layout );
    pushAgenteCodigo = searchAgenteCodigo->getButton();
    connect( pushAgenteCodigo, SIGNAL( clicked() ), this, SLOT( pushAgenteCodigo_clicked() ) );
    editAgenteCodigo = searchAgenteCodigo->getEditCode();
    editAgenteRazonSocial = searchAgenteCodigo->getEditDesc();
    comboEstadoPedido = addComboField<int>( pControlsFrame, "PEDIDOVENTA", "ESTADOPEDIDO", cabecera3Layout );

// frmDetails: PedidoVentaDet
    QFrame *pedidoventadetFrame = new QFrame(this);
    pedidoventadetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    pedidoventadetFrame->setLineWidth(4);
    QVBoxLayout *pedidoventadetFrameLayout = new QVBoxLayout(pedidoventadetFrame);
    RecPedidoVentaDet *pedidoventadet = getRecPedidoVenta()->getRecPedidoVentaDet(-1);
    dbViewDefinitionsList pedidoventadetviews;
    DBAPP->getDatabase()->getViewsForTable( pedidoventadet, pedidoventadetviews );
    dbRecordListDataModel *pedidoventadetdm =
        new dbRecordListDataModel(getRecPedidoVenta()->getListPedidoVentaDet(), pedidoventadet, pedidoventadetviews, "1=0");
    pFrmPedidoVentaDet = static_cast<FrmEditPedidoVentaDet *>(
                             DBAPP->createEditDetailForm(this,
                                     -1, pedidoventadet, "PEDIDOVENTADET",pedidoventadetdm, DataTable::browsing,
                                     static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
                                     pControlsFrame) );
    pFrmPedidoVentaDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
    pedidoventadetFrameLayout->addWidget( pFrmPedidoVentaDet );
    pedidoventadetLayout->addWidget(pedidoventadetFrame);
    editReferencia = addEditField( pControlsFrame, "PEDIDOVENTA", "REFERENCIA", rightReferenciaLayout );
    editSumaImportes = addEditField( pControlsFrame, "PEDIDOVENTA", "SUMAIMPORTES", rightSumaImportesLayout );
    editDtoP100 = addEditField( pControlsFrame, "PEDIDOVENTA", "DTOP100", rightDtoP100Layout );
    editDescuento = addEditField( pControlsFrame, "PEDIDOVENTA", "DESCUENTO", rightDescuentoLayout );
    editBaseImponible = addEditField( pControlsFrame, "PEDIDOVENTA", "BASEIMPONIBLE", rightBaseImponibleLayout );
    editRecargoEquivalencia = addEditField( pControlsFrame, "PEDIDOVENTA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
    editIVA = addEditField( pControlsFrame, "PEDIDOVENTA", "IVA", rightIVALayout );
    editTotal = addEditField( pControlsFrame, "PEDIDOVENTA", "TOTAL", rightTotalLayout );
    editDesgloseIVA = addEditField( pControlsFrame, "PEDIDOVENTA", "DESGLOSEIVA", desgloseivaLayout );
    editNotas = addTextField( pControlsFrame, "PEDIDOVENTA", "NOTAS", notasLayout );
    pControlsLayout->addLayout( cabeceraLayout );
    pControlsLayout->addLayout( cabecera2Layout );
    pControlsLayout->addLayout( cabecera3Layout );
    pControlsLayout->addLayout( pedidoventadetLayout );
    alignLayout( rightReferenciaLayout, false);
    alignLayout( rightSumaImportesLayout, false);
    alignLayout( rightDtoP100Layout, false);
    alignLayout( rightDescuentoLayout, false);
    alignLayout( rightBaseImponibleLayout, false);
    alignLayout( rightRecargoEquivalenciaLayout, false);
    alignLayout( rightIVALayout, false);
    alignLayout( rightTotalLayout, false);
    pControlsLayout->addLayout( desgloseivaLayout );
    pControlsLayout->addLayout( notasLayout );
    /*>>>>>FRMEDITPEDIDOVENTA_INIT_CONTROLS*/

    QGridLayout *gridlayout = new QGridLayout();
    gridlayout->addLayout( rightReferenciaLayout, 0, 0 );
    gridlayout->addLayout( rightSumaImportesLayout, 0, 1 );
    gridlayout->addLayout( rightDtoP100Layout, 0, 2 );
    gridlayout->addLayout( rightDescuentoLayout, 0, 3 );
    gridlayout->addLayout( rightBaseImponibleLayout, 1, 0 );
    gridlayout->addLayout( rightIVALayout, 1, 1 );
    gridlayout->addLayout( rightRecargoEquivalenciaLayout, 1, 2 );
    gridlayout->addLayout( rightTotalLayout, 1, 3 );
    pControlsLayout->addLayout( gridlayout );

    editNumero->setMustBeReadOnly( true ); /// FIXME: Esto debería funcionar del .ddd
    editSumaImportes->setMustBeReadOnly( true );
    editBaseImponible->setMustBeReadOnly( true );
    editRecargoEquivalencia->setMustBeReadOnly( true );
    editIVA->setMustBeReadOnly( true );
    editClienteCodigo->setWidthInChars(8);
    editAgenteCodigo->setWidthInChars(8);
    editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
}

/*<<<<<FRMEDITPEDIDOVENTA_CABECERA_GENNUMDOC*/
void FrmEditPedidoVenta::genNumeroDocumento()
{
    editContador->setText( ModuleInstance->getMaxContador( "PEDIDOVENTA",
                           getRecord()->getValue("EMPRESA_ID").toInt(),
                           ModuleInstance->getEmpresaModule()->getEjercicio(),
                           getRecTipoDoc()->getValue("SERIE").toString() ) );
    editContador->setJustEdited( true );
    validateFields( editContador, 0 );
    /*>>>>>FRMEDITPEDIDOVENTA_CABECERA_GENNUMDOC*/
}

void FrmEditPedidoVenta::scatterFields()
{
    /*<<<<<FRMEDITPEDIDOVENTA_SCATTER*/
    editFecha->setText(getRecPedidoVenta()->getValue("FECHA").toDate());
    if( isEditing() && (pFocusWidget == 0) )
        pFocusWidget = editFecha;
    editContador->setText(getRecPedidoVenta()->getValue("CONTADOR").toInt());
    editNumero->setText(getRecPedidoVenta()->getValue("NUMERO").toString());
    comboIVADetallado->setCurrentItemByValue(getRecPedidoVenta()->getValue("IVADETALLADO").toInt());
    comboEstadoPedido->setCurrentItemByValue(getRecPedidoVenta()->getValue("ESTADOPEDIDO").toInt());
    editReferencia->setText(getRecPedidoVenta()->getValue("REFERENCIA").toString());
    editSumaImportes->setText(getRecPedidoVenta()->getValue("SUMAIMPORTES").toMoney());
    editDtoP100->setText(getRecPedidoVenta()->getValue("DTOP100").toDouble());
    editDescuento->setText(getRecPedidoVenta()->getValue("DESCUENTO").toMoney());
    editBaseImponible->setText(getRecPedidoVenta()->getValue("BASEIMPONIBLE").toMoney());
    editRecargoEquivalencia->setText(getRecPedidoVenta()->getValue("RECARGOEQUIVALENCIA").toMoney());
    editIVA->setText(getRecPedidoVenta()->getValue("IVA").toMoney());
    editTotal->setText(getRecPedidoVenta()->getValue("TOTAL").toMoney());
    editDesgloseIVA->setText(getRecPedidoVenta()->getValue("DESGLOSEIVA").toString());
    editNotas->setText(getRecPedidoVenta()->getValue("NOTAS").toString());
    scatterTipoDoc();
    scatterCliente();
    scatterAgente();
    /*>>>>>FRMEDITPEDIDOVENTA_SCATTER*/
    if( isInserting() ) {
        if( !isDuplicating() && editFecha->toDate().isNull() )
            editFecha->setText( ModuleInstance->getWorkingDate() );
        pFocusWidget = editFecha;
        if( isDuplicating() ) {
            editTipoDocCodigo->setJustEdited( true );
            validateFields( editTipoDocCodigo, 0 );
        }
        comboEstadoPedido->setCurrentItemByValue( FactuModule::PedidoPendiente );
    } else if( isUpdating() ) {
        pFocusWidget = pFrmPedidoVentaDet;
    }
    pFrmPedidoVentaDet->addDetailIfNeeded();
    validateFields( comboIVADetallado, 0 ); // Para mostrar u ocultar el recargo de equivalencia
}

void FrmEditPedidoVenta::gatherFields()
{
    /*<<<<<FRMEDITPEDIDOVENTA_GATHER*/
    getRecPedidoVenta()->setValue( "FECHA", editFecha->toDate());
    getRecPedidoVenta()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
    getRecPedidoVenta()->setValue( "CONTADOR", editContador->toInt());
    getRecPedidoVenta()->setValue( "NUMERO", editNumero->toString());
    getRecPedidoVenta()->setValue( "CLIENTE_ID", getRecCliente()->getRecordID() );
    getRecPedidoVenta()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
    getRecPedidoVenta()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
    getRecPedidoVenta()->setValue( "ESTADOPEDIDO", comboEstadoPedido->getCurrentItemValue());
    getRecPedidoVenta()->setValue( "REFERENCIA", editReferencia->toString());
    getRecPedidoVenta()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
    getRecPedidoVenta()->setValue( "DTOP100", editDtoP100->toDouble());
    getRecPedidoVenta()->setValue( "DESCUENTO", editDescuento->toMoney());
    getRecPedidoVenta()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
    getRecPedidoVenta()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
    getRecPedidoVenta()->setValue( "IVA", editIVA->toMoney());
    getRecPedidoVenta()->setValue( "TOTAL", editTotal->toMoney());
    getRecPedidoVenta()->setValue( "DESGLOSEIVA", editDesgloseIVA->toString());
    getRecPedidoVenta()->setValue( "NOTAS", editNotas->toString());
    /*>>>>>FRMEDITPEDIDOVENTA_GATHER*/
    ModuleInstance->setWorkingDate( editFecha->toDate() );
}

void FrmEditPedidoVenta::scatterTipoDoc()
{
    /*<<<<<FRMEDITPEDIDOVENTA_SCATTER_TIPODOC*/
    editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
    editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
    /*>>>>>FRMEDITPEDIDOVENTA_SCATTER_TIPODOC*/
    if( editTipoDocCodigo->isJustEdited() ) {
        if( editAgenteCodigo->toString().isEmpty() )
            searchAgenteCodigo->setValue( getRecTipoDoc()->getRecAgente()->getValue("CODIGO") );
        if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
            comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
        genNumeroDocumento();
    }
}

void FrmEditPedidoVenta::pushTipoDocCodigo_clicked()
{
    /*<<<<<FRMEDITPEDIDOVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
    char action = mControlKeyPressed;
    if( !isEditing() || searchTipoDocCodigo->mustBeReadOnly() )
        action = 'E';
    switch( action ) {
    case 'F':
    case '\0':
        editTipoDocCodigo->setJustEdited( false );
        editTipoDocCodigo->setCancelling();
        if( DBAPP->choose(this, getRecTipoDoc(), 0, dbApplication::editNone, this ) ) {
            setEdited(true);
            scatterTipoDoc();
            editTipoDocCodigo->setJustEdited( true );
            editTipoDocCodigo->setFocus();
        }
        break;
    case 'M':
    {
        if( getRecTipoDoc()->getRecordID() ) {
            editTipoDocCodigo->setJustEdited( false );
            if( DBAPP->editRecord(this,
                                  getRecTipoDoc(), 0, DataTable::updating,
                                  dbApplication::simpleEdition, this ) ) {
                editTipoDocCodigo->setJustEdited( true );
                scatterTipoDoc();
            }
            editTipoDocCodigo->setFocus();
        }
    }
    break;
    case 'E':
    {
        if( getRecTipoDoc()->getRecordID() != 0 ) {
            editTipoDocCodigo->setJustEdited( false );
            DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTipoDoc(),
                                                  0, DataTable::selecting, dbApplication::simpleEdition, this ) );
        }
    }
    break;
    case 'A':
    {
        RecTipoDoc *tmprec = static_cast<RecTipoDoc *>(DBAPP->createRecord( "TipoDoc" ));
        editTipoDocCodigo->setJustEdited( false );
        tmprec->clear( true ); // set default values
        DBAPP->setCodeNotFound( editTipoDocCodigo->toString() );
        if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
                              dbApplication::simpleEdition, this ) ) {
            editTipoDocCodigo->setJustEdited( true );
            getRecTipoDoc()->copyRecord( tmprec );
            scatterTipoDoc();
        }
        editTipoDocCodigo->setFocus();
        DBAPP->setCodeNotFound( Xtring() );
    }
    break;
    }
    /*>>>>>FRMEDITPEDIDOVENTA_PUSH_TIPODOC_CODIGO_CLICKED*/
}


void FrmEditPedidoVenta::scatterCliente()
{
    /*<<<<<FRMEDITPEDIDOVENTA_SCATTER_CLIENTE*/
    editClienteCodigo->setText( getRecCliente()->getValue("CODIGO") );
    editClienteRazonSocial->setText( getRecCliente()->getValue("RAZONSOCIAL") );
    /*>>>>>FRMEDITPEDIDOVENTA_SCATTER_CLIENTE*/

    /*<<<<<FRMEDITPEDIDOVENTA_CABECERA_SCATTER_CLIENTE*/
    if( editClienteCodigo->isJustEdited() ) {
        if( getRecCliente()->getValue("IVADetallado").toInt() != 0 )
            comboIVADetallado->setCurrentItemByValue( getRecCliente()->getValue("IVADetallado").toInt() );
        if( getRecCliente()->getRecAgente()->getRecordID() != 0 )
            searchAgenteCodigo->setValue( getRecCliente()->getRecAgente()->getValue("CODIGO") );
    }
    /*>>>>>FRMEDITPEDIDOVENTA_CABECERA_SCATTER_CLIENTE*/
}

void FrmEditPedidoVenta::pushClienteCodigo_clicked()
{
    /*<<<<<FRMEDITPEDIDOVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
    char action = mControlKeyPressed;
    if( !isEditing() || searchClienteCodigo->mustBeReadOnly() )
        action = 'E';
    switch( action ) {
    case 'F':
    case '\0':
        editClienteCodigo->setJustEdited( false );
        editClienteCodigo->setCancelling();
        if( DBAPP->choose(this, getRecCliente(), 0, dbApplication::editNone, this ) ) {
            setEdited(true);
            scatterCliente();
            editClienteCodigo->setJustEdited( true );
            editClienteCodigo->setFocus();
        }
        break;
    case 'M':
    {
        if( getRecCliente()->getRecordID() ) {
            editClienteCodigo->setJustEdited( false );
            if( DBAPP->editRecord(this,
                                  getRecCliente(), 0, DataTable::updating,
                                  dbApplication::simpleEdition, this ) ) {
                editClienteCodigo->setJustEdited( true );
                scatterCliente();
            }
            editClienteCodigo->setFocus();
        }
    }
    break;
    case 'E':
    {
        if( getRecCliente()->getRecordID() != 0 ) {
            editClienteCodigo->setJustEdited( false );
            DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCliente(),
                                                  0, DataTable::selecting, dbApplication::simpleEdition, this ) );
        }
    }
    break;
    case 'A':
    {
        RecCliente *tmprec = static_cast<RecCliente *>(DBAPP->createRecord( "Cliente" ));
        editClienteCodigo->setJustEdited( false );
        tmprec->clear( true ); // set default values
        DBAPP->setCodeNotFound( editClienteCodigo->toString() );
        if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
                              dbApplication::simpleEdition, this ) ) {
            editClienteCodigo->setJustEdited( true );
            getRecCliente()->copyRecord( tmprec );
            scatterCliente();
        }
        editClienteCodigo->setFocus();
        DBAPP->setCodeNotFound( Xtring() );
    }
    break;
    }
    /*>>>>>FRMEDITPEDIDOVENTA_PUSH_CLIENTE_CODIGO_CLICKED*/
}

void FrmEditPedidoVenta::scatterAgente()
{
    /*<<<<<FRMEDITPEDIDOVENTA_SCATTER_AGENTE*/
    editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
    editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
    /*>>>>>FRMEDITPEDIDOVENTA_SCATTER_AGENTE*/
}
void FrmEditPedidoVenta::pushAgenteCodigo_clicked()
{
    /*<<<<<FRMEDITPEDIDOVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
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
            editAgenteCodigo->setFocus();
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
            editAgenteCodigo->setFocus();
        }
    }
    break;
    case 'E':
    {
        if( getRecAgente()->getRecordID() != 0 ) {
            editAgenteCodigo->setJustEdited( false );
            DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecAgente(),
                                                  0, DataTable::selecting, dbApplication::simpleEdition, this ) );
        }
    }
    break;
    case 'A':
    {
        RecAgente *tmprec = static_cast<RecAgente *>(DBAPP->createRecord( "Agente" ));
        editAgenteCodigo->setJustEdited( false );
        tmprec->clear( true ); // set default values
        DBAPP->setCodeNotFound( editAgenteCodigo->toString() );
        if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
                              dbApplication::simpleEdition, this ) ) {
            editAgenteCodigo->setJustEdited( true );
            getRecAgente()->copyRecord( tmprec );
            scatterAgente();
        }
        editAgenteCodigo->setFocus();
        DBAPP->setCodeNotFound( Xtring() );
    }
    break;
    }
    /*>>>>>FRMEDITPEDIDOVENTA_PUSH_AGENTE_CODIGO_CLICKED*/
}

void FrmEditPedidoVenta::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITPEDIDOVENTA_SPECIALACTION*/
    mControlKeyPressed = key;
    FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
    if( sender == editTipoDocCodigo )
        pushTipoDocCodigo_clicked();
    if( sender == editClienteCodigo )
        pushClienteCodigo_clicked();
    if( sender == editAgenteCodigo )
        pushAgenteCodigo_clicked();
    mControlKeyPressed = '\0';
    /*>>>>>FRMEDITPEDIDOVENTA_SPECIALACTION*/
}

void FrmEditPedidoVenta::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPEDIDOVENTA_VALIDATE*/
    bool v=true;
    if( !isvalid )
        isvalid = &v;
    ValidResult *validresult = ( ir ? ir : new ValidResult() );
    if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
        *isvalid = false;
    if( focusWidget() != pushTipoDocCodigo) // To avoid triggering the validating if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editTipoDocCodigo, editTipoDocNombre,
                           getRecTipoDoc(), "CODIGO", "NOMBRE", Xtring::null) )
            scatterTipoDoc();
    if( focusWidget() != pushClienteCodigo) // To avoid triggering the validating if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editClienteCodigo, editClienteRazonSocial,
                           getRecCliente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
            scatterCliente();
    if( focusWidget() != pushAgenteCodigo) // To avoid triggering the validating if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editAgenteCodigo, editAgenteRazonSocial,
                           getRecAgente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
            scatterAgente();
    /*>>>>>FRMEDITPEDIDOVENTA_VALIDATE*/
    /*<<<<<FRMEDITPEDIDOVENTA_CABECERA_VALIDATE*/
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
    /*>>>>>FRMEDITPEDIDOVENTA_CABECERA_VALIDATE*/
    if( sender == editContador || (sender == editAgenteCodigo && editAgenteCodigo->isJustEdited()) ) {
        editNumero->setText( RecTipoDoc::formatNumDocumento(
                                 empresa::ModuleInstance->getRecEmpresa()->getValue( "CODIGO" ).toInt(),
                                 empresa::ModuleInstance->getEjercicio(),
                                 editContador->toInt(), editAgenteCodigo->toInt(),
                                 getRecTipoDoc()->getValue("FORMATO").toString()) );
    }
    if( !sender || sender == editContador ) {
        if( !validCodeAndDesc( ( sender == editContador ? editNumero : sender ),
                               *validresult, editNumero, 0, "NUMERO", "") )
            if( !sender )
                *isvalid = false;
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITPEDIDOVENTA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditPedidoVenta::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
    actTotales();
    numeraLineas();
    /*>>>>>FRMEDITPEDIDOVENTA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITPEDIDOVENTA_CABECERA_NUMERALINEAS*/
void FrmEditPedidoVenta::numeraLineas()
{
    dbRecordList *reclst = getRecPedidoVenta()->getListPedidoVentaDet();
    for ( unsigned int i = 0; i < reclst->size(); i++ ) {
        RecPedidoVentaDet *detalle = static_cast<RecPedidoVentaDet *>( reclst->at( i ) );
        if( !detalle->isEmpty() ) // No numerar detalles vacíos
            detalle->setValue( "NLINEA", i+1 );
    }
    /*>>>>>FRMEDITPEDIDOVENTA_CABECERA_NUMERALINEAS*/
}

/*<<<<<FRMEDITPEDIDOVENTA_CABECERA_ACTTOTALES*/
void FrmEditPedidoVenta::actTotales()
{
    getRecPedidoVenta()->gather( this );
    getRecPedidoVenta()->actTotales();
    getRecPedidoVenta()->scatter( this );
    /*>>>>>FRMEDITPEDIDOVENTA_CABECERA_ACTTOTALES*/
}

/*<<<<<FRMEDITPEDIDOVENTA_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITPEDIDOVENTA_FIN*/

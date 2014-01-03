/*<<<<<COPYLEFT*/
/** @file factufrmeditpedidocompra.cpp Fichero de edición de pedidos de compra
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
// COPYLEFT Fichero de edición de pedidos de compra
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(TipoDoc,Codigo,Nombre) - cabecera
// FIELD Contador int - cabecera
// FIELD Numero string - cabecera
// FIELD IVADetallado comboint - cabecera2
// FIELD Proveedora_ID Reference(Proveedora,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - cabecera2
// FIELD EstadoPedido comboint - cabecera2
// FIELD PedidoCompraDet FrmEditRecDetail
// FIELD Referencia string - noaddreferencia
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD IVA money - noaddrightIVA
// FIELD Total money - noaddrightTotal
// FIELD DesgloseIVA string - desgloseiva
// FIELD Notas text
// TYPE FrmEditRecMaster factu::PedidoCompra Pedido Compra
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITPEDIDOCOMPRA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditpedidocompra.h"
/*>>>>>FRMEDITPEDIDOCOMPRA_INCLUDES*/
#include "factumodule.h"
#include "factufldivadetallado.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITPEDIDOCOMPRA_CONSTRUCTOR*/
FrmEditPedidoCompra::FrmEditPedidoCompra(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditPedidoCompra" );
/*>>>>>FRMEDITPEDIDOCOMPRA_CONSTRUCTOR*/
    /*<<<<<FRMEDITPEDIDOCOMPRA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *pedidocompradetLayout = new QHBoxLayout(0, 0, 6, "pedidocompradetLayout");
	QHBoxLayout *referenciaLayout = new QHBoxLayout(0, 0, 6, "referenciaLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
	QHBoxLayout *desgloseivaLayout = new QHBoxLayout(0, 0, 6, "desgloseivaLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editFecha = addEditField( pControlsFrame, "PEDIDOCOMPRA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editContador = addEditField( pControlsFrame, "PEDIDOCOMPRA", "CONTADOR", cabeceraLayout );
	editNumero = addEditField( pControlsFrame, "PEDIDOCOMPRA", "NUMERO", cabeceraLayout );
	comboIVADetallado = addComboIntField( pControlsFrame, "PEDIDOCOMPRA", "IVADETALLADO", cabecera2Layout );

	searchProveedoraCodigo = addSearchField( pControlsFrame, "PROVEEDORA_ID", "PROVEEDORA", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushProveedoraCodigo = searchProveedoraCodigo->getButton();
	connect( pushProveedoraCodigo, SIGNAL( clicked() ), this, SLOT( pushProveedoraCodigo_clicked() ) );
	editProveedoraCodigo = searchProveedoraCodigo->getEditCode();
	editProveedoraRazonSocial = searchProveedoraCodigo->getEditDesc();
	comboEstadoPedido = addComboIntField( pControlsFrame, "PEDIDOCOMPRA", "ESTADOPEDIDO", cabecera2Layout );

// frmDetails: PedidoCompraDet
	QFrame *pedidocompradetFrame = new QFrame(this);
	pedidocompradetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	pedidocompradetFrame->setLineWidth(4);
	QVBoxLayout *pedidocompradetFrameLayout = new QVBoxLayout(pedidocompradetFrame);
	RecPedidoCompraDet *pedidocompradet = getRecPedidoCompra()->getRecPedidoCompraDet(-1);
	dbViewDefinitionsList pedidocompradetviews;
	DBAPP->getDatabase()->getViewsForTable( pedidocompradet, pedidocompradetviews );
	dbRecordListDataModel *pedidocompradetdm =
		new dbRecordListDataModel(getRecPedidoCompra()->getListPedidoCompraDet(), pedidocompradet, pedidocompradetviews, "1=0");
	pFrmPedidoCompraDet = static_cast<FrmEditPedidoCompraDet *>(
		DBAPP->createEditDetailForm(this,
		-1, pedidocompradet, "PEDIDOCOMPRADET",pedidocompradetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmPedidoCompraDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	pedidocompradetFrameLayout->addWidget( pFrmPedidoCompraDet );
	pedidocompradetLayout->addWidget(pedidocompradetFrame);
	editReferencia = addEditField( pControlsFrame, "PEDIDOCOMPRA", "REFERENCIA", referenciaLayout );
	editSumaImportes = addEditField( pControlsFrame, "PEDIDOCOMPRA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "PEDIDOCOMPRA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "PEDIDOCOMPRA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "PEDIDOCOMPRA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "PEDIDOCOMPRA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editIVA = addEditField( pControlsFrame, "PEDIDOCOMPRA", "IVA", rightIVALayout );
	editTotal = addEditField( pControlsFrame, "PEDIDOCOMPRA", "TOTAL", rightTotalLayout );
	editDesgloseIVA = addEditField( pControlsFrame, "PEDIDOCOMPRA", "DESGLOSEIVA", desgloseivaLayout );
	editNotas = addTextField( pControlsFrame, "PEDIDOCOMPRA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( pedidocompradetLayout );
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightTotalLayout, false);
	pControlsLayout->addLayout( desgloseivaLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITPEDIDOCOMPRA_INIT_CONTROLS*/

    QGridLayout *gridlayout = new QGridLayout();
    gridlayout->addLayout( referenciaLayout, 0, 0 );
    gridlayout->addLayout( rightSumaImportesLayout, 0, 1 );
    gridlayout->addLayout( rightDtoP100Layout, 0, 2 );
    gridlayout->addLayout( rightDescuentoLayout, 0, 3 );
    gridlayout->addLayout( rightBaseImponibleLayout, 1, 0 );
    gridlayout->addLayout( rightIVALayout, 1, 1 );
    gridlayout->addLayout( rightRecargoEquivalenciaLayout, 1, 2 );
    gridlayout->addLayout( rightTotalLayout, 1, 3 );
    pControlsLayout->insertLayout(4, gridlayout );

    editNumero->setMustBeReadOnly( true );
    editSumaImportes->setMustBeReadOnly( true );
    editBaseImponible->setMustBeReadOnly( true );
    editRecargoEquivalencia->setMustBeReadOnly( true );
    editIVA->setMustBeReadOnly( true );
    editProveedoraCodigo->setWidthInChars(5);
	editNotas->setHeightInLines(3);
    editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
    if( empresa::ModuleInstance->getRecEmpresa()->getValue("RECARGOEQUIVALENCIA").toBool() == false ) {
        editRecargoEquivalencia->getLabel()->setVisible( false );
        editRecargoEquivalencia->setVisible( false );
    }
}

void FrmEditPedidoCompra::scatterFields()
{
    /*<<<<<FRMEDITPEDIDOCOMPRA_SCATTER*/
	editFecha->setText(getRecPedidoCompra()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editContador->setText(getRecPedidoCompra()->getValue("CONTADOR").toInt());
	editNumero->setText(getRecPedidoCompra()->getValue("NUMERO").toString());
	comboIVADetallado->setCurrentItemByValue(getRecPedidoCompra()->getValue("IVADETALLADO").toInt());
	comboEstadoPedido->setCurrentItemByValue(getRecPedidoCompra()->getValue("ESTADOPEDIDO").toInt());
	editReferencia->setText(getRecPedidoCompra()->getValue("REFERENCIA").toString());
	editSumaImportes->setText(getRecPedidoCompra()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecPedidoCompra()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecPedidoCompra()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecPedidoCompra()->getValue("BASEIMPONIBLE").toMoney());
	editRecargoEquivalencia->setText(getRecPedidoCompra()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editIVA->setText(getRecPedidoCompra()->getValue("IVA").toMoney());
	editTotal->setText(getRecPedidoCompra()->getValue("TOTAL").toMoney());
	editDesgloseIVA->setText(getRecPedidoCompra()->getValue("DESGLOSEIVA").toString());
	editNotas->setText(getRecPedidoCompra()->getValue("NOTAS").toString());
	scatterTipoDoc();
	scatterProveedora();
/*>>>>>FRMEDITPEDIDOCOMPRA_SCATTER*/
    if( isInserting() ) {
        if( !isDuplicating() && editFecha->toDate().isNull() )
            editFecha->setText( ModuleInstance->getWorkingDate() );
        empresa::EmpresaModule *em = ModuleInstance->getEmpresaModule();
        if( em && em->getRecEmpresa() ) {
            if( em->getRecEmpresa()->getValue("RECARGOEQUIVALENCIA").toBool() )
                comboIVADetallado->setCurrentItemByValue( FldIVADetallado::con_recargo );
        }
        if( isDuplicating() ) {
            editTipoDocCodigo->setJustEdited( true );
            validateFields( editTipoDocCodigo, 0 );
        }
        comboEstadoPedido->setCurrentItemByValue( FactuModule::PedidoPendiente );
        pFocusWidget = editFecha;
    } else if( isUpdating() ) {
        pFocusWidget = pFrmPedidoCompraDet;
    }
    pFrmPedidoCompraDet->addDetailIfNeeded();
    validateFields( comboIVADetallado, 0 ); // Para mostrar u ocultar el recargo de equivalencia
}

void FrmEditPedidoCompra::gatherFields()
{
    /*<<<<<FRMEDITPEDIDOCOMPRA_GATHER*/
	getRecPedidoCompra()->setValue( "FECHA", editFecha->toDate());
	getRecPedidoCompra()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecPedidoCompra()->setValue( "CONTADOR", editContador->toInt());
	getRecPedidoCompra()->setValue( "NUMERO", editNumero->toString());
	getRecPedidoCompra()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecPedidoCompra()->setValue( "PROVEEDORA_ID", getRecProveedora()->getRecordID() );
	getRecPedidoCompra()->setValue( "ESTADOPEDIDO", comboEstadoPedido->getCurrentItemValue());
	getRecPedidoCompra()->setValue( "REFERENCIA", editReferencia->toString());
	getRecPedidoCompra()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecPedidoCompra()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecPedidoCompra()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecPedidoCompra()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecPedidoCompra()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecPedidoCompra()->setValue( "IVA", editIVA->toMoney());
	getRecPedidoCompra()->setValue( "TOTAL", editTotal->toMoney());
	getRecPedidoCompra()->setValue( "DESGLOSEIVA", editDesgloseIVA->toString());
	getRecPedidoCompra()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPEDIDOCOMPRA_GATHER*/
    ModuleInstance->setWorkingDate( editFecha->toDate() );
}

/*<<<<<FRMEDITPEDIDOCOMPRA_CABECERA_GENNUMDOC*/
void FrmEditPedidoCompra::genNumeroDocumento()
{
		editContador->setText( ModuleInstance->getMaxContador( "PEDIDOCOMPRA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			ModuleInstance->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITPEDIDOCOMPRA_CABECERA_GENNUMDOC*/
}


void FrmEditPedidoCompra::scatterTipoDoc()
{
    /*<<<<<FRMEDITPEDIDOCOMPRA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITPEDIDOCOMPRA_SCATTER_TIPODOC*/
    if( editTipoDocCodigo->isJustEdited() ) {
        if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
            comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
        genNumeroDocumento();
    }
}

void FrmEditPedidoCompra::pushTipoDocCodigo_clicked()
{
    /*<<<<<FRMEDITPEDIDOCOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITPEDIDOCOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
}


void FrmEditPedidoCompra::scatterProveedora()
{
    /*<<<<<FRMEDITPEDIDOCOMPRA_SCATTER_PROVEEDORA*/
	editProveedoraCodigo->setText( getRecProveedora()->getValue("CODIGO") );
	editProveedoraRazonSocial->setText( getRecProveedora()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITPEDIDOCOMPRA_SCATTER_PROVEEDORA*/
    /*<<<<<FRMEDITPEDIDOCOMPRA_CABECERA_SCATTER_PROVEEDORA*/
	if( editProveedoraCodigo->isJustEdited() ) {
		if( getRecProveedora()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecProveedora()->getValue("IVADetallado").toInt() );
	}
/*>>>>>FRMEDITPEDIDOCOMPRA_CABECERA_SCATTER_PROVEEDORA*/
}

void FrmEditPedidoCompra::pushProveedoraCodigo_clicked()
{
    /*<<<<<FRMEDITPEDIDOCOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchProveedoraCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editProveedoraCodigo->setJustEdited( false );
			editProveedoraCodigo->setCancelling();
			if( DBAPP->choose(this, getRecProveedora(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterProveedora();
				editProveedoraCodigo->setJustEdited( true );
				editProveedoraCodigo->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecProveedora()->getRecordID() ) {
					editProveedoraCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecProveedora(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editProveedoraCodigo->setJustEdited( true );
						scatterProveedora();
					}
					editProveedoraCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecProveedora()->getRecordID() != 0 ) {
					editProveedoraCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecProveedora(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecProveedora *tmprec = static_cast<RecProveedora *>(DBAPP->createRecord( "Proveedora" ));
				editProveedoraCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editProveedoraCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editProveedoraCodigo->setJustEdited( true );
					getRecProveedora()->copyRecord( tmprec );
					scatterProveedora();
				}
				editProveedoraCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPEDIDOCOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
}

void FrmEditPedidoCompra::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITPEDIDOCOMPRA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoDocCodigo )
		pushTipoDocCodigo_clicked();
	if( sender == editProveedoraCodigo )
		pushProveedoraCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITPEDIDOCOMPRA_SPECIALACTION*/
}

void FrmEditPedidoCompra::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPEDIDOCOMPRA_VALIDATE*/
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
	if( focusWidget() != pushProveedoraCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProveedoraCodigo, editProveedoraRazonSocial,
		getRecProveedora(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterProveedora();
/*>>>>>FRMEDITPEDIDOCOMPRA_VALIDATE*/
    if( !sender || (
                (sender == editNumero && !editNumero->toString().isEmpty())
                || (sender == editProveedoraCodigo && !editProveedoraCodigo->toString().isEmpty() ) )
      ) {
        dbRecordID prov_id = getRecProveedora()->getRecordID();
        if( getRecord()->existsAnother(
                    "PROVEEDORA_ID=" + getRecord()->getConnection()->toSQL( prov_id )
                    + " AND NUMERO=" + getRecord()->getConnection()->toSQL( editNumero->toString() ) ) ) {
            *isvalid = false;
            validresult->addError( Xtring::printf(_("Ya existe %s de %s con este valor para '%s'"),
                                                  DBAPP->getTableDescSingular( getRecord()->getTableName(), "una" ).c_str(),
                                                  DBAPP->getTableDescSingular( "PROVEEDORA", "esta" ).c_str(),
                                                  DBAPP->getDatabase()->findFieldDefinition( "PEDIDOCOMPRA.NUMERO" )->getCaption().c_str()),
                                   "NUMERO" );
        }
    }
    /*<<<<<FRMEDITPEDIDOCOMPRA_CABECERA_VALIDATE*/
	if( sender == editDtoP100 && editDtoP100->isJustEdited() ) {
		editDescuento->setText( 0.0 );
		actTotales();
	}
	if( sender == comboIVADetallado ) {
		if( comboIVADetallado->getCurrentItemValue() == factu::FldIVADetallado::con_recargo ) {
			if( empresa::ModuleInstance->getRecEmpresa()->getValue("RECARGOEQUIVALENCIA").toBool() == false )
				validresult->addWarning( Xtring::printf( _("%s no está en el régimen de recargo de equivalencia."),
														 DBAPP->getTableDescSingular("EMPRESA", "La").c_str()),  "IVADETALLADO" );
			editRecargoEquivalencia->setVisible( true );
			editRecargoEquivalencia->getLabel()->setVisible( true );
		} else {
			if( empresa::ModuleInstance->getRecEmpresa()->getValue("RECARGOEQUIVALENCIA").toBool() == true 	)
				validresult->addWarning( Xtring::printf( _("%s está en el régimen de recargo de equivalencia."),
														 DBAPP->getTableDescSingular("EMPRESA", "La").c_str()),  "IVADETALLADO" );
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
/*>>>>>FRMEDITPEDIDOCOMPRA_CABECERA_VALIDATE*/
    if( sender == editContador ) {
        editNumero->setText( RecTipoDoc::formatNumDocumento(
                                 empresa::ModuleInstance->getRecEmpresa()->getValue( "CODIGO" ).toInt(),
                                 empresa::ModuleInstance->getEjercicio(),
                                 editContador->toInt(), 0,
                                 getRecTipoDoc()->getValue("FORMATO").toString()) );
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITPEDIDOCOMPRA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditPedidoCompra::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITPEDIDOCOMPRA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITPEDIDOCOMPRA_CABECERA_NUMERALINEAS*/
void FrmEditPedidoCompra::numeraLineas()
{
	dbRecordList *reclst = getRecPedidoCompra()->getListPedidoCompraDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecPedidoCompraDet *detalle = static_cast<RecPedidoCompraDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITPEDIDOCOMPRA_CABECERA_NUMERALINEAS*/
}

/*<<<<<FRMEDITPEDIDOCOMPRA_CABECERA_ACTTOTALES*/
void FrmEditPedidoCompra::actTotales()
{
	getRecPedidoCompra()->gather( this );
	getRecPedidoCompra()->actTotales();
	getRecPedidoCompra()->scatter( this );
/*>>>>>FRMEDITPEDIDOCOMPRA_CABECERA_ACTTOTALES*/
}

/*<<<<<FRMEDITPEDIDOCOMPRA_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITPEDIDOCOMPRA_FIN*/

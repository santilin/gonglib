/*<<<<<COPYLEFT*/
/** @file pagosfrmeditpago.cpp Fichero de edición de pagos
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
// COPYLEFT Fichero de edición de pagos
// FIELD Factura_ID Reference(Factura,Numero,Fecha) - terceros
// FIELD Tercero_ID Reference(Tercero,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - terceros
// FIELD Automatico bool - numero
// FIELD Numero int - numero
// FIELD NumeroAgrupado string - numero
// FIELD Contador int - numero
// FIELD Descripcion string - desc
// FIELD FechaEmision date - fechas
// FIELD Vencimiento date - fechas
// FIELD FechaValor date - fechas
// FIELD Moneda_ID Reference(empresa::Moneda,Codigo,Nombre) - importes
// FIELD Importe money - importes
// FIELD Resto Money - importes
// FIELD EstadoRecibo comboint - importes
// FIELD FechaPago date - pago
// FIELD DocumentoPago string - pago
// FIELD CuentaPago_ID Reference(contab::Cuenta,Cuenta,Descripcion) - contab MODULE_INCLUDED(Contab)
// FIELD CuentaOrigen string - contab
// FIELD Notas text - notas
// TYPE FrmEditRecMaster pagos::Pago
/*>>>>>MODULE_INFO*/

#include "config.h"
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif
#include <gongdbrecordpermissions.h>
/*<<<<<FRMEDITPAGO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "pagosfrmeditpago.h"
/*>>>>>FRMEDITPAGO_INCLUDES*/
#include "pagosfrmpagarrecibo.h"
#include "pagosmodule.h"
#include "pagosipagablerecord.h"

namespace gong {
namespace pagos {

/*<<<<<FRMEDITPAGO_CONSTRUCTOR*/
FrmEditPago::FrmEditPago(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditPago" );
/*>>>>>FRMEDITPAGO_CONSTRUCTOR*/
    /*<<<<<FRMEDITPAGO_INIT_CONTROLS*/
	QHBoxLayout *tercerosLayout = new QHBoxLayout(0, 0, 6, "tercerosLayout");
	QHBoxLayout *numeroLayout = new QHBoxLayout(0, 0, 6, "numeroLayout");
	QHBoxLayout *descLayout = new QHBoxLayout(0, 0, 6, "descLayout");
	QHBoxLayout *fechasLayout = new QHBoxLayout(0, 0, 6, "fechasLayout");
	QHBoxLayout *importesLayout = new QHBoxLayout(0, 0, 6, "importesLayout");
	QHBoxLayout *pagoLayout = new QHBoxLayout(0, 0, 6, "pagoLayout");
	QHBoxLayout *contabLayout = new QHBoxLayout(0, 0, 6, "contabLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");

	searchFacturaNumero = addSearchField( pControlsFrame, "FACTURA_ID", "FACTURA", "NUMERO", "FECHA", tercerosLayout );
	pushFacturaNumero = searchFacturaNumero->getButton();
	connect( pushFacturaNumero, SIGNAL( clicked() ), this, SLOT( pushFacturaNumero_clicked() ) );
	editFacturaNumero = searchFacturaNumero->getEditCode();
	editFacturaFecha = searchFacturaNumero->getEditDesc();

	searchTerceroCodigo = addSearchField( pControlsFrame, "TERCERO_ID", "TERCERO", "CODIGO", "RAZONSOCIAL", tercerosLayout );
	pushTerceroCodigo = searchTerceroCodigo->getButton();
	connect( pushTerceroCodigo, SIGNAL( clicked() ), this, SLOT( pushTerceroCodigo_clicked() ) );
	editTerceroCodigo = searchTerceroCodigo->getEditCode();
	editTerceroRazonSocial = searchTerceroCodigo->getEditDesc();
	checkAutomatico = addCheckField( pControlsFrame, "PAGO", "AUTOMATICO", numeroLayout );
	editNumero = addEditField( pControlsFrame, "PAGO", "NUMERO", numeroLayout );
	editNumeroAgrupado = addEditField( pControlsFrame, "PAGO", "NUMEROAGRUPADO", numeroLayout );
	editContador = addEditField( pControlsFrame, "PAGO", "CONTADOR", numeroLayout );
	editDescripcion = addEditField( pControlsFrame, "PAGO", "DESCRIPCION", descLayout );
	editFechaEmision = addEditField( pControlsFrame, "PAGO", "FECHAEMISION", fechasLayout );
	editVencimiento = addEditField( pControlsFrame, "PAGO", "VENCIMIENTO", fechasLayout );
	editFechaValor = addEditField( pControlsFrame, "PAGO", "FECHAVALOR", fechasLayout );

	searchMonedaCodigo = addSearchField( pControlsFrame, "MONEDA_ID", "MONEDA", "CODIGO", "NOMBRE", importesLayout );
	pushMonedaCodigo = searchMonedaCodigo->getButton();
	connect( pushMonedaCodigo, SIGNAL( clicked() ), this, SLOT( pushMonedaCodigo_clicked() ) );
	editMonedaCodigo = searchMonedaCodigo->getEditCode();
	editMonedaNombre = searchMonedaCodigo->getEditDesc();
	editImporte = addEditField( pControlsFrame, "PAGO", "IMPORTE", importesLayout );
	editResto = addEditField( pControlsFrame, "PAGO", "RESTO", importesLayout );
	comboEstadoRecibo = addComboIntField( pControlsFrame, "PAGO", "ESTADORECIBO", importesLayout );
	editFechaPago = addEditField( pControlsFrame, "PAGO", "FECHAPAGO", pagoLayout );
	editDocumentoPago = addEditField( pControlsFrame, "PAGO", "DOCUMENTOPAGO", pagoLayout );

#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	searchCuentaPagoCuenta = addSearchField( pControlsFrame, "CUENTAPAGO_ID", "CUENTA", "CUENTA", "DESCRIPCION", contabLayout );
	pushCuentaPagoCuenta = searchCuentaPagoCuenta->getButton();
	connect( pushCuentaPagoCuenta, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCuenta_clicked() ) );
	editCuentaPagoCuenta = searchCuentaPagoCuenta->getEditCode();
	editCuentaPagoDescripcion = searchCuentaPagoCuenta->getEditDesc();
}
#endif
	editCuentaOrigen = addEditField( pControlsFrame, "PAGO", "CUENTAORIGEN", contabLayout );
	editNotas = addTextField( pControlsFrame, "PAGO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( tercerosLayout );
	pControlsLayout->addLayout( numeroLayout );
	pControlsLayout->addLayout( descLayout );
	pControlsLayout->addLayout( fechasLayout );
	pControlsLayout->addLayout( importesLayout );
	pControlsLayout->addLayout( pagoLayout );
	pControlsLayout->addLayout( contabLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITPAGO_INIT_CONTROLS*/
    pTercerosLayout = tercerosLayout;
    editContador->setMustBeReadOnly( true );
    editFechaPago->setMustBeReadOnly(true);
    searchFacturaNumero->setMustBeReadOnly(true);
    searchTerceroCodigo->setMustBeReadOnly(true);
#ifdef HAVE_CONTABMODULE
    searchCuentaPagoCuenta->setMustBeReadOnly(true);
#endif
    editDocumentoPago->setMustBeReadOnly(true);
    pushPagar = new QPushButton( toGUI( _("&Pagar o anular") ), this, "pushPagar" );
    connect ( pushPagar, SIGNAL ( clicked() ), this, SLOT ( pushPagar_clicked() ) );
    pButtonsLayout->addWidget( pushPagar );
    checkAutomatico->setMustBeReadOnly( true );
}

void FrmEditPago::completa(const Xtring& tablafacturas, const Xtring& fldfactcodigo, const Xtring& fldfactdesc,
                           const Xtring& tablaterceros, const Xtring& fldterccodigo, const Xtring& fldtercdesc,
                           const Xtring& descsingular, const Xtring& descplural, bool femenina)
{
    if( tablafacturas != searchFacturaNumero->getTableName()
            || 	tablaterceros != searchTerceroCodigo->getTableName() ) {

        removeControl( searchFacturaNumero );
        if( fldfactcodigo == Xtring::null ) {
            dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablafacturas );
            dbFieldDefinition *flddeffc = tbldef->findFieldByFlags( dbFieldDefinition::CODE );
            if( flddeffc )
                mFldFactCodigo = flddeffc->getName();
            dbFieldDefinition *flddeffd = tbldef->findFieldByFlags( dbFieldDefinition::DESCRIPTION );
            if( flddeffd )
                mFldFactDesc = flddeffd->getName();
        } else {
            mFldFactCodigo = fldfactcodigo;
            mFldFactDesc = fldfactdesc;
        }
        searchFacturaNumero = addSearchField( pControlsFrame, "FACTURA_ID", tablafacturas,
                                              mFldFactCodigo, mFldFactDesc, pTercerosLayout );
        pushFacturaNumero = searchFacturaNumero->getButton();
        connect( pushFacturaNumero, SIGNAL( clicked() ), this, SLOT( pushFacturaNumero_clicked() ) );
        editFacturaNumero = searchFacturaNumero->getEditCode();
        editFacturaFecha = searchFacturaNumero->getEditDesc();

        removeControl( searchTerceroCodigo );
        if( fldterccodigo == Xtring::null ) {
            dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablaterceros );
            dbFieldDefinition *flddeffc = tbldef->findFieldByFlags( dbFieldDefinition::CODE );
            if( flddeffc )
                mFldTercCodigo = flddeffc->getName();
            dbFieldDefinition *flddeffd = tbldef->findFieldByFlags( dbFieldDefinition::DESCRIPTION );
            if( flddeffd )
                mFldTercDesc = flddeffd->getName();
        } else {
            mFldTercCodigo = fldterccodigo;
            mFldTercDesc = fldtercdesc;
        }
        searchTerceroCodigo = addSearchField( pControlsFrame, "TERCERO_ID", tablaterceros,
                                              mFldTercCodigo, mFldTercDesc, pTercerosLayout );
        pushTerceroCodigo = searchTerceroCodigo->getButton();
        connect( pushTerceroCodigo, SIGNAL( clicked() ), this, SLOT( pushTerceroCodigo_clicked() ) );
        editTerceroCodigo = searchTerceroCodigo->getEditCode();
        editTerceroRazonSocial = searchTerceroCodigo->getEditDesc();

        setTabOrder( editFacturaNumero, editTerceroCodigo );
        setTabOrder( editTerceroCodigo, editNumero );

    }
}

void FrmEditPago::scatterFactura()
{
    /*<<<<<FRMEDITPAGO_SCATTER_FACTURA*/
	editFacturaNumero->setText( getRecFactura()->getValue("NUMERO") );
	editFacturaFecha->setText( getRecFactura()->getValue("FECHA") );
/*>>>>>FRMEDITPAGO_SCATTER_FACTURA*/
}

void FrmEditPago::pushFacturaNumero_clicked()
{
    /*<<<<<FRMEDITPAGO_PUSH_FACTURA_NUMERO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchFacturaNumero->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editFacturaNumero->setJustEdited( false );
			editFacturaNumero->setCancelling();
			if( DBAPP->choose(this, getRecFactura(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterFactura();
				editFacturaNumero->setJustEdited( true );
				editFacturaNumero->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecFactura()->getRecordID() ) {
					editFacturaNumero->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecFactura(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editFacturaNumero->setJustEdited( true );
						scatterFactura();
					}
					editFacturaNumero->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecFactura()->getRecordID() != 0 ) {
					editFacturaNumero->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecFactura(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecFactura *tmprec = static_cast<RecFactura *>(DBAPP->createRecord( "Factura" ));
				editFacturaNumero->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editFacturaNumero->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editFacturaNumero->setJustEdited( true );
					getRecFactura()->copyRecord( tmprec );
					scatterFactura();
				}
				editFacturaNumero->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPAGO_PUSH_FACTURA_NUMERO_CLICKED*/
}

#ifdef HAVE_CONTABMODULE
void FrmEditPago::scatterCuentaPago()
{
#define getRecCuenta getRecCuentaPago
    /*<<<<<FRMEDITPAGO_SCATTER_CUENTAPAGO*/
	editCuentaPagoCuenta->setText( getRecCuentaPago()->getValue("CUENTA") );
	editCuentaPagoDescripcion->setText( getRecCuentaPago()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITPAGO_SCATTER_CUENTAPAGO*/
#undef getRecCuenta
}
#endif

void FrmEditPago::pushCuentaPagoCuenta_clicked()
{
#ifdef HAVE_CONTABMODULE
    /*<<<<<FRMEDITPAGO_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaPagoCuenta->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaPagoCuenta->setJustEdited( false );
			editCuentaPagoCuenta->setCancelling();
			if( DBAPP->choose(this, getRecCuentaPago(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaPago();
				editCuentaPagoCuenta->setJustEdited( true );
				editCuentaPagoCuenta->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecCuentaPago()->getRecordID() ) {
					editCuentaPagoCuenta->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaPago(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaPagoCuenta->setJustEdited( true );
						scatterCuentaPago();
					}
					editCuentaPagoCuenta->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaPago()->getRecordID() != 0 ) {
					editCuentaPagoCuenta->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCuentaPago(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				contab::RecCuentaPago *tmprec = static_cast<contab::RecCuentaPago *>(DBAPP->createRecord( "CuentaPago" ));
				editCuentaPagoCuenta->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaPagoCuenta->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaPagoCuenta->setJustEdited( true );
					getRecCuentaPago()->copyRecord( tmprec );
					scatterCuentaPago();
				}
				editCuentaPagoCuenta->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPAGO_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
#endif
}

void FrmEditPago::scatterTercero()
{
    /*<<<<<FRMEDITPAGO_SCATTER_TERCERO*/
	editTerceroCodigo->setText( getRecTercero()->getValue("CODIGO") );
	editTerceroRazonSocial->setText( getRecTercero()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITPAGO_SCATTER_TERCERO*/
}

void FrmEditPago::pushTerceroCodigo_clicked()
{
    /*<<<<<FRMEDITPAGO_PUSH_TERCERO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchTerceroCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editTerceroCodigo->setJustEdited( false );
			editTerceroCodigo->setCancelling();
			if( DBAPP->choose(this, getRecTercero(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterTercero();
				editTerceroCodigo->setJustEdited( true );
				editTerceroCodigo->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecTercero()->getRecordID() ) {
					editTerceroCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecTercero(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editTerceroCodigo->setJustEdited( true );
						scatterTercero();
					}
					editTerceroCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecTercero()->getRecordID() != 0 ) {
					editTerceroCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTercero(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecTercero *tmprec = static_cast<RecTercero *>(DBAPP->createRecord( "Tercero" ));
				editTerceroCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editTerceroCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editTerceroCodigo->setJustEdited( true );
					getRecTercero()->copyRecord( tmprec );
					scatterTercero();
				}
				editTerceroCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPAGO_PUSH_TERCERO_CODIGO_CLICKED*/
}

void FrmEditPago::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITPAGO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editFacturaNumero )
		pushFacturaNumero_clicked();
	if( sender == editTerceroCodigo )
		pushTerceroCodigo_clicked();
	if( sender == editMonedaCodigo )
		pushMonedaCodigo_clicked();
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	if( sender == editCuentaPagoCuenta )
		pushCuentaPagoCuenta_clicked();
}
#endif
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITPAGO_SPECIALACTION*/
}

void FrmEditPago::scatterFields()
{
    if( getRecFactura() == 0 ) {
        throw std::runtime_error( _("El documento origen de este pago no está definido. Intenta volver a generarlo."));
    }
    if( getRecTercero() == 0 ) {
        throw std::runtime_error( _("La entidad origen de este pago no está definida. Intenta volver a generarlo."));
    }
    if( isDuplicating() ) {
        getRecPago()->setValue( "NUMERO", 0 );
        getRecPago()->setValue( "CONTADOR", 0 );
        getRecPago()->setValue( "NUMEROAGRUPADO", 0 );
        getRecPago()->setValue( "ESTADORECIBO", pagos::PagosModule::ReciboPendiente );
        getRecPago()->setValue( "RESTO", Money(0.0) );
        getRecPago()->setValue( "FECHAPAGO", Date() );
        getRecPago()->setValue( "DOCUMENTOPAGO", Xtring::null );
#ifdef HAVE_CONTABMODULE
        getRecPago()->setValue( "CUENTAPAGO_ID", 0 );
        getRecPago()->setValue( "ASIENTO_CARGO_ID", 0 );
        getRecPago()->setValue( "ASIENTO_PAGO_ID", 0 );
#endif
    }
    /*<<<<<FRMEDITPAGO_SCATTER*/
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFacturaNumero;
	checkAutomatico->setChecked(getRecPago()->getValue("AUTOMATICO").toBool());
	editNumero->setText(getRecPago()->getValue("NUMERO").toInt());
	editNumeroAgrupado->setText(getRecPago()->getValue("NUMEROAGRUPADO").toString());
	editContador->setText(getRecPago()->getValue("CONTADOR").toInt());
	editDescripcion->setText(getRecPago()->getValue("DESCRIPCION").toString());
	editFechaEmision->setText(getRecPago()->getValue("FECHAEMISION").toDate());
	editVencimiento->setText(getRecPago()->getValue("VENCIMIENTO").toDate());
	editFechaValor->setText(getRecPago()->getValue("FECHAVALOR").toDate());
	editImporte->setText(getRecPago()->getValue("IMPORTE").toMoney());
	editResto->setText(getRecPago()->getValue("RESTO").toMoney());
	comboEstadoRecibo->setCurrentItemByValue(getRecPago()->getValue("ESTADORECIBO").toInt());
	editFechaPago->setText(getRecPago()->getValue("FECHAPAGO").toDate());
	editDocumentoPago->setText(getRecPago()->getValue("DOCUMENTOPAGO").toString());
	editCuentaOrigen->setText(getRecPago()->getValue("CUENTAORIGEN").toString());
	editNotas->setText(getRecPago()->getValue("NOTAS").toString());
	scatterFactura();
	scatterTercero();
	scatterMoneda();
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	scatterCuentaPago();
}
#endif
/*>>>>>FRMEDITPAGO_SCATTER*/
    if( getRecPago()->getValue("AUTOMATICO").toBool() ) {
        searchFacturaNumero->setMustBeReadOnly(true);
        editFechaEmision->setMustBeReadOnly(true);
        checkAutomatico->setMustBeReadOnly(true);
        searchTerceroCodigo->setMustBeReadOnly(true);
        editNumero->setMustBeReadOnly(true);
        editNumeroAgrupado->setMustBeReadOnly(true);
        comboEstadoRecibo->setMustBeReadOnly(true);
        editImporte->setMustBeReadOnly(true);
        editResto->setMustBeReadOnly(true);
        editCuentaOrigen->setMustBeReadOnly(true);
        searchMonedaCodigo->setMustBeReadOnly(true);
    } else {
        if( editNumero->toInt() == 0 )
            editNumero->setText( getRecPago()->selectNextInt( "NUMERO" ) );
        if( editNumeroAgrupado->toInt() == 0 )
            editNumeroAgrupado->setText( editNumero->toInt() );
    }
    if( getRecPago()->getValue( "ESTADORECIBO").toInt() == PagosModule::ReciboPagado ) {
        editVencimiento->setMustBeReadOnly( true );
        editFechaValor->setMustBeReadOnly(true);
    }
    if( editContador->toInt() == 0 )
        editContador->setText( empresa::ModuleInstance->getMaxContador() );
}

void FrmEditPago::gatherFields()
{
    /*<<<<<FRMEDITPAGO_GATHER*/
	getRecPago()->setValue( "FACTURA_ID", getRecFactura()->getRecordID() );
	getRecPago()->setValue( "TERCERO_ID", getRecTercero()->getRecordID() );
	getRecPago()->setValue( "AUTOMATICO", checkAutomatico->isChecked());
	getRecPago()->setValue( "NUMERO", editNumero->toInt());
	getRecPago()->setValue( "NUMEROAGRUPADO", editNumeroAgrupado->toString());
	getRecPago()->setValue( "CONTADOR", editContador->toInt());
	getRecPago()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecPago()->setValue( "FECHAEMISION", editFechaEmision->toDate());
	getRecPago()->setValue( "VENCIMIENTO", editVencimiento->toDate());
	getRecPago()->setValue( "FECHAVALOR", editFechaValor->toDate());
	getRecPago()->setValue( "MONEDA_ID", getRecMoneda()->getRecordID() );
	getRecPago()->setValue( "IMPORTE", editImporte->toMoney());
	getRecPago()->setValue( "RESTO", editResto->toMoney());
	getRecPago()->setValue( "ESTADORECIBO", comboEstadoRecibo->getCurrentItemValue());
	getRecPago()->setValue( "FECHAPAGO", editFechaPago->toDate());
	getRecPago()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecPago()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
	getRecPago()->setValue( "CUENTAORIGEN", editCuentaOrigen->toString());
	getRecPago()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPAGO_GATHER*/
}

void FrmEditPago::scatterMoneda()
{
    /*<<<<<FRMEDITPAGO_SCATTER_MONEDA*/
	editMonedaCodigo->setText( getRecMoneda()->getValue("CODIGO") );
	editMonedaNombre->setText( getRecMoneda()->getValue("NOMBRE") );
/*>>>>>FRMEDITPAGO_SCATTER_MONEDA*/
}
void FrmEditPago::pushMonedaCodigo_clicked()
{
    /*<<<<<FRMEDITPAGO_PUSH_MONEDA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchMonedaCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editMonedaCodigo->setJustEdited( false );
			editMonedaCodigo->setCancelling();
			if( DBAPP->choose(this, getRecMoneda(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterMoneda();
				editMonedaCodigo->setJustEdited( true );
				editMonedaCodigo->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecMoneda()->getRecordID() ) {
					editMonedaCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecMoneda(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editMonedaCodigo->setJustEdited( true );
						scatterMoneda();
					}
					editMonedaCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecMoneda()->getRecordID() != 0 ) {
					editMonedaCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecMoneda(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				empresa::RecMoneda *tmprec = static_cast<empresa::RecMoneda *>(DBAPP->createRecord( "Moneda" ));
				editMonedaCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editMonedaCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editMonedaCodigo->setJustEdited( true );
					getRecMoneda()->copyRecord( tmprec );
					scatterMoneda();
				}
				editMonedaCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPAGO_PUSH_MONEDA_CODIGO_CLICKED*/
}


void FrmEditPago::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITPAGO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushFacturaNumero) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editFacturaNumero, editFacturaFecha,
		getRecFactura(), "NUMERO", "FECHA", Xtring::null) )
		scatterFactura();
	if( focusWidget() != pushTerceroCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTerceroCodigo, editTerceroRazonSocial,
		getRecTercero(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterTercero();
	if( focusWidget() != pushMonedaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editMonedaCodigo, editMonedaNombre,
		getRecMoneda(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterMoneda();
/*>>>>>FRMEDITPAGO_VALIDATE*/
    if( !sender && isInserting() ) {
        int contador = empresa::ModuleInstance->getMaxContador();
        if( contador > editContador->toInt() ) {
            editContador->setText( contador );
            validresult->addWarning( Xtring::printf(
                                         _("El contador ha cambiado durante la edición de este registro. El nuevo contador es %d"),
                                         contador ), "CONTADOR" );
        }
    }
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

void FrmEditPago::pushPagar_clicked()
{
    if( mMustRead )
        read();
    if( IPagableRecord *pr = dynamic_cast<IPagableRecord *>(getRecFactura() ) ) {
        if( getRecPago()->getValue( "ESTADORECIBO").toInt() == PagosModule::ReciboPendiente ) {
#ifdef HAVE_CONTABMODULE
            bool supervisar = contab::ModuleInstance->getModuleSetting( "SUPERVISAR_ASIENTOS" ).toBool();
#else
            bool supervisar = false;
#endif
            pr->pagarRecibo( this, getRecPago()->getRecordID(), getRecPago(), supervisar );
        } else if( getRecPago()->getValue( "ESTADORECIBO").toInt() == PagosModule::ReciboPagado ) {
            pr->anularPagoRecibo( this, getRecPago()->getRecordID(), getRecPago());
        }
    }
}

bool FrmEditPago::canBeginEdit(EditMode newmode)
{
    if( !FrmEditRecMaster::canBeginEdit( newmode ) )
        return false;
    if( newmode == DataTable::inserting ) {
        if( getRecord()->getValue( "TABLAFACTURAS" ).toString().isEmpty()
                || getRecord()->getValue( "TABLATERCEROS" ).toString().isEmpty() ) {
            msgError( this, _("No se pueden crear pagos directamente.") );
            return false;
        }
    }
    if( newmode == DataTable::deleting ) {
        if( getRecPago()->getValue( "ESTADORECIBO").toInt() == PagosModule::ReciboPagado ) {
            FrmBase::msgError(this,
                              _("No puede borrarse un pago ya pagado. Anula el pago primero" ) );
            return false;
        }
    }
    completa( pRecord->getValue( "TABLAFACTURAS" ).toString(), Xtring::null, Xtring::null,
              pRecord->getValue( "TABLATERCEROS" ).toString(), Xtring::null, Xtring::null,
              Xtring::null, Xtring::null, false );
    return true;
}

void FrmEditPago::updateStatus( bool callbehaviors )
{
    FrmEditRecMaster::updateStatus( callbehaviors );
    pushPagar->setEnabled( pMenuRecordModify->isEnabled() );
}

/*<<<<<FRMEDITPAGO_FIN*/
} // namespace pagos
} // namespace gong
/*>>>>>FRMEDITPAGO_FIN*/



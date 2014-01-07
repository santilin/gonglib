/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de cobros
// FIELD RemesaCobro_ID Reference(RemesaCobro,Numero,Descripcion) - remesaLayout
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
// TYPE FrmEditRecMaster pagos::Cobro
/*>>>>>MODULE_INFO*/

#include "config.h"
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif
#include <gongdbrecordpermissions.h>
/*<<<<<FRMEDITCOBRO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "pagosfrmeditcobro.h"
/*>>>>>FRMEDITCOBRO_INCLUDES*/
#include "pagosmodule.h"
#include "pagosipagablerecord.h"

namespace gong {
namespace pagos {

/*<<<<<FRMEDITCOBRO_CONSTRUCTOR*/
FrmEditCobro::FrmEditCobro(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditCobro" );
/*>>>>>FRMEDITCOBRO_CONSTRUCTOR*/
/*<<<<<FRMEDITCOBRO_INIT_CONTROLS*/
	QHBoxLayout *remesaLayout = new QHBoxLayout(0, 0, 6, "remesaLayout");
	QHBoxLayout *tercerosLayout = new QHBoxLayout(0, 0, 6, "tercerosLayout");
	QHBoxLayout *numeroLayout = new QHBoxLayout(0, 0, 6, "numeroLayout");
	QHBoxLayout *descLayout = new QHBoxLayout(0, 0, 6, "descLayout");
	QHBoxLayout *fechasLayout = new QHBoxLayout(0, 0, 6, "fechasLayout");
	QHBoxLayout *importesLayout = new QHBoxLayout(0, 0, 6, "importesLayout");
	QHBoxLayout *pagoLayout = new QHBoxLayout(0, 0, 6, "pagoLayout");
	QHBoxLayout *contabLayout = new QHBoxLayout(0, 0, 6, "contabLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");

	searchRemesaCobroNumero = addSearchField( pControlsFrame, "REMESACOBRO_ID", "REMESACOBRO", "NUMERO", "DESCRIPCION", remesaLayout );
	pushRemesaCobroNumero = searchRemesaCobroNumero->getButton();
	connect( pushRemesaCobroNumero, SIGNAL( clicked() ), this, SLOT( pushRemesaCobroNumero_clicked() ) );
	editRemesaCobroNumero = searchRemesaCobroNumero->getEditCode();
	editRemesaCobroDescripcion = searchRemesaCobroNumero->getEditDesc();

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
	checkAutomatico = addCheckField( pControlsFrame, "COBRO", "AUTOMATICO", numeroLayout );
	editNumero = addEditField( pControlsFrame, "COBRO", "NUMERO", numeroLayout );
	editNumeroAgrupado = addEditField( pControlsFrame, "COBRO", "NUMEROAGRUPADO", numeroLayout );
	editContador = addEditField( pControlsFrame, "COBRO", "CONTADOR", numeroLayout );
	editDescripcion = addEditField( pControlsFrame, "COBRO", "DESCRIPCION", descLayout );
	editFechaEmision = addEditField( pControlsFrame, "COBRO", "FECHAEMISION", fechasLayout );
	editVencimiento = addEditField( pControlsFrame, "COBRO", "VENCIMIENTO", fechasLayout );
	editFechaValor = addEditField( pControlsFrame, "COBRO", "FECHAVALOR", fechasLayout );

	searchMonedaCodigo = addSearchField( pControlsFrame, "MONEDA_ID", "MONEDA", "CODIGO", "NOMBRE", importesLayout );
	pushMonedaCodigo = searchMonedaCodigo->getButton();
	connect( pushMonedaCodigo, SIGNAL( clicked() ), this, SLOT( pushMonedaCodigo_clicked() ) );
	editMonedaCodigo = searchMonedaCodigo->getEditCode();
	editMonedaNombre = searchMonedaCodigo->getEditDesc();
	editImporte = addEditField( pControlsFrame, "COBRO", "IMPORTE", importesLayout );
	editResto = addEditField( pControlsFrame, "COBRO", "RESTO", importesLayout );
	comboEstadoRecibo = addComboIntField( pControlsFrame, "COBRO", "ESTADORECIBO", importesLayout );
	editFechaPago = addEditField( pControlsFrame, "COBRO", "FECHAPAGO", pagoLayout );
	editDocumentoPago = addEditField( pControlsFrame, "COBRO", "DOCUMENTOPAGO", pagoLayout );

#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	searchCuentaPagoCuenta = addSearchField( pControlsFrame, "CUENTAPAGO_ID", "CUENTA", "CUENTA", "DESCRIPCION", contabLayout );
	pushCuentaPagoCuenta = searchCuentaPagoCuenta->getButton();
	connect( pushCuentaPagoCuenta, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCuenta_clicked() ) );
	editCuentaPagoCuenta = searchCuentaPagoCuenta->getEditCode();
	editCuentaPagoDescripcion = searchCuentaPagoCuenta->getEditDesc();
}
#endif
	editCuentaOrigen = addEditField( pControlsFrame, "COBRO", "CUENTAORIGEN", contabLayout );
	editNotas = addTextField( pControlsFrame, "COBRO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( remesaLayout );
	pControlsLayout->addLayout( tercerosLayout );
	pControlsLayout->addLayout( numeroLayout );
	pControlsLayout->addLayout( descLayout );
	pControlsLayout->addLayout( fechasLayout );
	pControlsLayout->addLayout( importesLayout );
	pControlsLayout->addLayout( pagoLayout );
	pControlsLayout->addLayout( contabLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITCOBRO_INIT_CONTROLS*/
    pTercerosLayout = tercerosLayout;
    editContador->setMustBeReadOnly( true );
    editFechaPago->setMustBeReadOnly(true);
    searchFacturaNumero->setMustBeReadOnly(true);
    editFacturaNumero->setWidthInChars(15);
    searchTerceroCodigo->setMustBeReadOnly(true);
#ifdef HAVE_CONTABMODULE
    searchCuentaPagoCuenta->setMustBeReadOnly(true);
#endif
    editDocumentoPago->setMustBeReadOnly(true);
    pushPagar = new QPushButton( toGUI( _("Co&brar o anular") ), this, "pushPagar" );
    pushPagar->setDefault( true );
    connect ( pushPagar, SIGNAL ( clicked() ), this, SLOT ( pushPagar_clicked() ) );
    pButtonsLayout->addWidget( pushPagar );
    mUsarRemesas = ModuleInstance->getModuleSetting("USAR_REMESAS").toBool();
    checkAutomatico->setMustBeReadOnly( true );
}

void FrmEditCobro::setUsarRemesas(bool usar)
{
    searchRemesaCobroNumero->setVisible( usar );
}


void FrmEditCobro::completa(const Xtring& tablafacturas, const Xtring& fldfactcodigo, const Xtring& fldfactdesc,
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

        setTabOrder( editRemesaCobroNumero, editFacturaNumero );
        setTabOrder( editFacturaNumero, editTerceroCodigo );
        setTabOrder( editTerceroCodigo, editNumero );
    }
}

void FrmEditCobro::scatterFactura()
{
#if 0
/*<<<<<FRMEDITCOBRO_SCATTER_FACTURA*/
	editFacturaNumero->setText( getRecFactura()->getValue("NUMERO") );
	editFacturaFecha->setText( getRecFactura()->getValue("FECHA") );
/*>>>>>FRMEDITCOBRO_SCATTER_FACTURA*/
#endif
    editFacturaNumero->setText( getRecFactura()->getValue( mFldFactCodigo ) );
    editFacturaFecha->setText( getRecFactura()->getValue(mFldFactDesc) );
}

void FrmEditCobro::pushFacturaNumero_clicked()
{
/*<<<<<FRMEDITCOBRO_PUSH_FACTURA_NUMERO_CLICKED*/
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
/*>>>>>FRMEDITCOBRO_PUSH_FACTURA_NUMERO_CLICKED*/
}

#ifdef HAVE_CONTABMODULE
void FrmEditCobro::scatterCuentaPago()
{
#define getRecCuenta getRecCuentaPago
    /*<<<<<FRMEDITCOBRO_SCATTER_CUENTAPAGO*/
	editCuentaPagoCuenta->setText( getRecCuentaPago()->getValue("CUENTA") );
	editCuentaPagoDescripcion->setText( getRecCuentaPago()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITCOBRO_SCATTER_CUENTAPAGO*/
#undef getRecCuenta
}
#endif

void FrmEditCobro::pushCuentaPagoCuenta_clicked()
{
#ifdef HAVE_CONTABMODULE
    /*<<<<<FRMEDITCOBRO_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
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
/*>>>>>FRMEDITCOBRO_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
#endif
}

void FrmEditCobro::scatterTercero()
{
#if 0
    /*<<<<<FRMEDITCOBRO_SCATTER_TERCERO*/
	editTerceroCodigo->setText( getRecTercero()->getValue("CODIGO") );
	editTerceroRazonSocial->setText( getRecTercero()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITCOBRO_SCATTER_TERCERO*/
#endif
    editTerceroCodigo->setText( getRecTercero()->getValue( mFldTercCodigo ) );
    editTerceroRazonSocial->setText( getRecTercero()->getValue(mFldTercDesc ) );
}

void FrmEditCobro::pushTerceroCodigo_clicked()
{
    /*<<<<<FRMEDITCOBRO_PUSH_TERCERO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITCOBRO_PUSH_TERCERO_CODIGO_CLICKED*/
}

void FrmEditCobro::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITCOBRO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editRemesaCobroNumero )
		pushRemesaCobroNumero_clicked();
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
/*>>>>>FRMEDITCOBRO_SPECIALACTION*/
}

void FrmEditCobro::scatterFields()
{
    if( getRecFactura() == 0 ) {
        throw std::runtime_error( _("El documento origen de este pago no está definido. Intenta volver a generarlo."));
    }
    if( getRecTercero() == 0 ) {
        throw std::runtime_error( _("La entidad origen de este pago no está definida. Intenta volver a generarlo."));
    }
    if( isDuplicating() ) {
        getRecCobro()->setValue( "NUMERO", 0 );
        getRecCobro()->setValue( "CONTADOR", 0 );
        getRecCobro()->setValue( "NUMEROAGRUPADO", 0 );
        getRecCobro()->setValue( "ESTADORECIBO", pagos::PagosModule::ReciboPendiente );
        getRecCobro()->setValue( "RESTO", Money(0.0) );
        getRecCobro()->setValue( "FECHAPAGO", Date() );
        getRecCobro()->setValue( "DOCUMENTOPAGO", Xtring::null );
#ifdef HAVE_CONTABMODULE
        getRecCobro()->setValue( "CUENTAPAGO_ID", 0 );
        getRecCobro()->setValue( "ASIENTO_CARGO_ID", 0 );
        getRecCobro()->setValue( "ASIENTO_PAGO_ID", 0 );
#endif
    }
    /*<<<<<FRMEDITCOBRO_SCATTER*/
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editRemesaCobroNumero;
	checkAutomatico->setChecked(getRecCobro()->getValue("AUTOMATICO").toBool());
	editNumero->setText(getRecCobro()->getValue("NUMERO").toInt());
	editNumeroAgrupado->setText(getRecCobro()->getValue("NUMEROAGRUPADO").toString());
	editContador->setText(getRecCobro()->getValue("CONTADOR").toInt());
	editDescripcion->setText(getRecCobro()->getValue("DESCRIPCION").toString());
	editFechaEmision->setText(getRecCobro()->getValue("FECHAEMISION").toDate());
	editVencimiento->setText(getRecCobro()->getValue("VENCIMIENTO").toDate());
	editFechaValor->setText(getRecCobro()->getValue("FECHAVALOR").toDate());
	editImporte->setText(getRecCobro()->getValue("IMPORTE").toMoney());
	editResto->setText(getRecCobro()->getValue("RESTO").toMoney());
	comboEstadoRecibo->setCurrentItemByValue(getRecCobro()->getValue("ESTADORECIBO").toInt());
	editFechaPago->setText(getRecCobro()->getValue("FECHAPAGO").toDate());
	editDocumentoPago->setText(getRecCobro()->getValue("DOCUMENTOPAGO").toString());
	editCuentaOrigen->setText(getRecCobro()->getValue("CUENTAORIGEN").toString());
	editNotas->setText(getRecCobro()->getValue("NOTAS").toString());
	scatterRemesaCobro();
	scatterFactura();
	scatterTercero();
	scatterMoneda();
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	scatterCuentaPago();
}
#endif
/*>>>>>FRMEDITCOBRO_SCATTER*/
    if( isInserting() ) {
        if( ModuleInstance->getRecRemesaCobroPadre( this ) ) {
            getRecRemesaCobro()->copyRecord( ModuleInstance->getRecRemesaCobroPadre( this ) );
            scatterRemesaCobro();
        }
        comboEstadoRecibo->setCurrentItemByValue( ModuleInstance->getModuleSetting( "ESTADORECIBO.PENDIENTE" ).toInt() );
    }
    if( getRecCobro()->getValue("AUTOMATICO").toBool() ) {
        searchFacturaNumero->setMustBeReadOnly(true);
        editFechaEmision->setMustBeReadOnly(true);
        searchTerceroCodigo->setMustBeReadOnly(true);
        checkAutomatico->setMustBeReadOnly(true);
        editNumero->setMustBeReadOnly(true);
        editNumeroAgrupado->setMustBeReadOnly(true);
        editImporte->setMustBeReadOnly(true);
        comboEstadoRecibo->setMustBeReadOnly(true);
        editResto->setMustBeReadOnly(true);
        editCuentaOrigen->setMustBeReadOnly(true);
        searchMonedaCodigo->setMustBeReadOnly(true);
    } else {
        if( editNumero->toInt() == 0 )
            editNumero->setText( getRecCobro()->selectNextInt( "NUMERO",
                                 "REMESACOBRO_ID=" + getRecCobro()->getConnection()->toSQL( getRecRemesaCobro()->getRecordID() ) ) );
        if( editNumeroAgrupado->toInt() == 0 )
            editNumeroAgrupado->setText( editNumero->toInt() );
    }
    if( getRecCobro()->getValue( "ESTADORECIBO").toInt() == PagosModule::ReciboPagado ) {
        editVencimiento->setMustBeReadOnly( true );
        editFechaValor->setMustBeReadOnly(true);
    }
    if( editContador->toInt() == 0 )
        editContador->setText( empresa::ModuleInstance->getMaxContador() );
    if( ModuleInstance->getRecRemesaCobroPadre( this ) && !editRemesaCobroNumero->isReadOnly() )
        pFocusWidget = editRemesaCobroNumero;
    else if( editFacturaNumero->isEnabled() )
        pFocusWidget = editFacturaNumero;
    else
        pFocusWidget = editNumero;
}

void FrmEditCobro::gatherFields()
{
    /*<<<<<FRMEDITCOBRO_GATHER*/
	getRecCobro()->setValue( "REMESACOBRO_ID", getRecRemesaCobro()->getRecordID() );
	getRecCobro()->setValue( "FACTURA_ID", getRecFactura()->getRecordID() );
	getRecCobro()->setValue( "TERCERO_ID", getRecTercero()->getRecordID() );
	getRecCobro()->setValue( "AUTOMATICO", checkAutomatico->isChecked());
	getRecCobro()->setValue( "NUMERO", editNumero->toInt());
	getRecCobro()->setValue( "NUMEROAGRUPADO", editNumeroAgrupado->toString());
	getRecCobro()->setValue( "CONTADOR", editContador->toInt());
	getRecCobro()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecCobro()->setValue( "FECHAEMISION", editFechaEmision->toDate());
	getRecCobro()->setValue( "VENCIMIENTO", editVencimiento->toDate());
	getRecCobro()->setValue( "FECHAVALOR", editFechaValor->toDate());
	getRecCobro()->setValue( "MONEDA_ID", getRecMoneda()->getRecordID() );
	getRecCobro()->setValue( "IMPORTE", editImporte->toMoney());
	getRecCobro()->setValue( "RESTO", editResto->toMoney());
	getRecCobro()->setValue( "ESTADORECIBO", comboEstadoRecibo->getCurrentItemValue());
	getRecCobro()->setValue( "FECHAPAGO", editFechaPago->toDate());
	getRecCobro()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecCobro()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
	getRecCobro()->setValue( "CUENTAORIGEN", editCuentaOrigen->toString());
	getRecCobro()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITCOBRO_GATHER*/
    getRecCobro()->setValue( "RAZONSOCIAL", getRecTercero()->getValue( mFldTercDesc ) );
}

void FrmEditCobro::scatterMoneda()
{
    /*<<<<<FRMEDITCOBRO_SCATTER_MONEDA*/
	editMonedaCodigo->setText( getRecMoneda()->getValue("CODIGO") );
	editMonedaNombre->setText( getRecMoneda()->getValue("NOMBRE") );
/*>>>>>FRMEDITCOBRO_SCATTER_MONEDA*/
}

void FrmEditCobro::pushMonedaCodigo_clicked()
{
/*<<<<<FRMEDITCOBRO_PUSH_MONEDA_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITCOBRO_PUSH_MONEDA_CODIGO_CLICKED*/
}


void FrmEditCobro::scatterRemesaCobro()
{
    /*<<<<<FRMEDITCOBRO_SCATTER_REMESACOBRO*/
	editRemesaCobroNumero->setText( getRecRemesaCobro()->getValue("NUMERO") );
	editRemesaCobroDescripcion->setText( getRecRemesaCobro()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITCOBRO_SCATTER_REMESACOBRO*/
}
void FrmEditCobro::pushRemesaCobroNumero_clicked()
{
    /*<<<<<FRMEDITCOBRO_PUSH_REMESACOBRO_NUMERO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchRemesaCobroNumero->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editRemesaCobroNumero->setJustEdited( false );
			editRemesaCobroNumero->setCancelling();
			if( DBAPP->choose(this, getRecRemesaCobro(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterRemesaCobro();
				editRemesaCobroNumero->setJustEdited( true );
				editRemesaCobroNumero->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecRemesaCobro()->getRecordID() ) {
					editRemesaCobroNumero->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecRemesaCobro(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editRemesaCobroNumero->setJustEdited( true );
						scatterRemesaCobro();
					}
					editRemesaCobroNumero->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecRemesaCobro()->getRecordID() != 0 ) {
					editRemesaCobroNumero->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecRemesaCobro(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecRemesaCobro *tmprec = static_cast<RecRemesaCobro *>(DBAPP->createRecord( "RemesaCobro" ));
				editRemesaCobroNumero->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editRemesaCobroNumero->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editRemesaCobroNumero->setJustEdited( true );
					getRecRemesaCobro()->copyRecord( tmprec );
					scatterRemesaCobro();
				}
				editRemesaCobroNumero->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITCOBRO_PUSH_REMESACOBRO_NUMERO_CLICKED*/
}

void FrmEditCobro::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITCOBRO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushRemesaCobroNumero) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editRemesaCobroNumero, editRemesaCobroDescripcion,
		getRecRemesaCobro(), "NUMERO", "DESCRIPCION", Xtring::null) )
		scatterRemesaCobro();
	if( focusWidget() != pushMonedaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editMonedaCodigo, editMonedaNombre,
		getRecMoneda(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterMoneda();
/*>>>>>FRMEDITCOBRO_VALIDATE*/
// {capel} Eliminar los validSeekCode de Factura y Tercero
    if( focusWidget() != pushFacturaNumero) // To avoid triggering the validating if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editFacturaNumero, editFacturaFecha,
                           getRecFactura(), mFldFactCodigo, mFldFactDesc, Xtring::null) ) {
            getRecord()->setValue( "FACTURA_ID", 0 );
            scatterFactura();
        }
    if( focusWidget() != pushTerceroCodigo) // To avoid triggering the validating if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editTerceroCodigo, editTerceroRazonSocial,
                           getRecTercero(), mFldTercCodigo, mFldTercDesc, Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) ) {
            getRecord()->setValue( "TERCERO_ID", 0 );
            scatterTercero();
        }
    if( sender == editFechaEmision && editFechaValor->toDate().isNull() ) {
        editFechaValor->setText( editFechaEmision->toDate() );
    }
    if( sender == editFechaEmision && editVencimiento->toDate().isNull() ) {
        editVencimiento->setText( editFechaEmision->toDate() );
    }
    if( sender == editImporte && editImporte->isJustEdited() ) {
        if( isInserting() || isDuplicating() ) {
            editResto->setText( editImporte->toMoney() );
        } else {
            Money pagado = editImporte->getPreviousValue().toMoney() - editResto->toMoney();
            editResto->setText( editImporte->toMoney() - pagado );
        }
    }
    if( !sender ) {
        int contador = empresa::ModuleInstance->getMaxContador();
        if( contador > editContador->toInt() ) {
            editContador->setText( contador );
            ir->addWarning( Xtring::printf(
                                _("El contador ha cambiado durante la edición de este registro. El nuevo contador es %d"),
                                contador ), "CONTADOR" );
        }
    }
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

void FrmEditCobro::pushPagar_clicked()
{
    if( mMustRead )
        read();
    if( IPagableRecord *pr = dynamic_cast<IPagableRecord *>(getRecFactura() ) ) {
        if( getRecCobro()->getValue( "ESTADORECIBO" ).toInt() == PagosModule::ReciboPendiente ) {
#ifdef HAVE_CONTABMODULE
            bool supervisar = contab::ModuleInstance->getModuleSetting( "SUPERVISAR_ASIENTOS" ).toBool();
#else
            bool supervisar = false;
#endif
            pr->cobrarRecibo(this, getRecCobro()->getRecordID(), getRecCobro(), supervisar );
        } else if( getRecCobro()->getValue( "ESTADORECIBO" ).toInt() == PagosModule::ReciboPagado ) {
            pr->anularCobroRecibo( this, getRecCobro()->getRecordID(), getRecCobro() );
        }
    }
}

bool FrmEditCobro::canBeginEdit(EditMode newmode)
{
    if( !FrmEditRecMaster::canBeginEdit( newmode ) )
        return false;
    if( newmode == DataTable::inserting || newmode == DataTable::duplicating ) {
        if( getRecord()->getValue( "TABLAFACTURAS" ).toString().isEmpty()
                || getRecord()->getValue( "TABLATERCEROS" ).toString().isEmpty() ) {
            msgError( this, _("No se pueden crear cobros directamente.") );
            return false;
        }
    }
    if( newmode == DataTable::deleting ) {
        if( getRecCobro()->getValue( "ESTADORECIBO").toInt() == PagosModule::ReciboPagado ) {
            FrmBase::msgError(this,
                              _("No puede borrarse un cobro ya cobrado. Anula el cobro primero" ) );
            return false;
        }
    }
    completa( pRecord->getValue( "TABLAFACTURAS" ).toString(), Xtring::null, Xtring::null,
              pRecord->getValue( "TABLATERCEROS" ).toString(), Xtring::null, Xtring::null,
              Xtring::null, Xtring::null, false );
    return true;
}

void FrmEditCobro::updateStatus( bool callbehaviors )
{
    FrmEditRecMaster::updateStatus( callbehaviors );
    pushPagar->setEnabled( pMenuRecordModify->isEnabled() );
}

/*<<<<<FRMEDITCOBRO_FIN*/
} // namespace pagos
} // namespace gong
/*>>>>>FRMEDITCOBRO_FIN*/

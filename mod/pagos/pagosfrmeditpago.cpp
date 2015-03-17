// FIELD Factura_ID Reference(Factura,Numero,Fecha) - terceros
// FIELD Tercero_ID Reference(Tercero,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - terceros
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de pagos
// FIELD Automatico bool - numero
// FIELD Numero int - numero
// FIELD NumeroAgrupado string - numero
// FIELD Contador int - numero
// FIELD Descripcion string - desc
// FIELD FechaEmision date - fechas
// FIELD Vencimiento date - fechas
// FIELD FechaValor date - fechas
// FIELD EstadoRecibo comboint - importes
// FIELD Importe money - importes
// FIELD Resto Money - importes
// FIELD FechaPago date - pago
// FIELD DocumentoPago string - pago
// FIELD Moneda_ID Reference(empresa::Moneda,Codigo,Nombre) - leftMoneda
// FIELD CuentaPago_ID Reference(CuentaTesoreria,Codigo,Nombre) - cuentapago MODULE_INCLUDED(Tesoreria)
// FIELD CuentaPago_ID Reference(contab::Cuenta,Cuenta,Descripcion) - contab MODULE_INCLUDED(Contab)
// FIELD CuentaOrigen string - contab MODULE_INCLUDED(Contab)
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
    searchFacturaNumero = 0;
    searchTerceroCodigo = 0;
    pTercerosLayout = new QHBoxLayout();
    pControlsLayout->addLayout(pTercerosLayout);
    pFacturasLayout = new QHBoxLayout();
    pControlsLayout->addLayout(pFacturasLayout);
/*<<<<<FRMEDITPAGO_INIT_CONTROLS*/
	QHBoxLayout *numeroLayout = new QHBoxLayout(0, 0, 6, "numeroLayout");
	QHBoxLayout *descLayout = new QHBoxLayout(0, 0, 6, "descLayout");
	QHBoxLayout *fechasLayout = new QHBoxLayout(0, 0, 6, "fechasLayout");
	QHBoxLayout *importesLayout = new QHBoxLayout(0, 0, 6, "importesLayout");
	QHBoxLayout *pagoLayout = new QHBoxLayout(0, 0, 6, "pagoLayout");
	QHBoxLayout *leftMonedaLayout = new QHBoxLayout(0, 0, 6, "leftMonedaLayout");
	QHBoxLayout *cuentapagoLayout = new QHBoxLayout(0, 0, 6, "cuentapagoLayout");
	QHBoxLayout *contabLayout = new QHBoxLayout(0, 0, 6, "contabLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	checkAutomatico = addCheckField( pControlsFrame, "PAGO", "AUTOMATICO", numeroLayout );
	editNumero = addEditField( pControlsFrame, "PAGO", "NUMERO", numeroLayout );
	editNumeroAgrupado = addEditField( pControlsFrame, "PAGO", "NUMEROAGRUPADO", numeroLayout );
	editContador = addEditField( pControlsFrame, "PAGO", "CONTADOR", numeroLayout );
	editDescripcion = addEditField( pControlsFrame, "PAGO", "DESCRIPCION", descLayout );
	editFechaEmision = addEditField( pControlsFrame, "PAGO", "FECHAEMISION", fechasLayout );
	editVencimiento = addEditField( pControlsFrame, "PAGO", "VENCIMIENTO", fechasLayout );
	editFechaValor = addEditField( pControlsFrame, "PAGO", "FECHAVALOR", fechasLayout );
	comboEstadoRecibo = addComboIntField( pControlsFrame, "PAGO", "ESTADORECIBO", importesLayout );
	editImporte = addEditField( pControlsFrame, "PAGO", "IMPORTE", importesLayout );
	editResto = addEditField( pControlsFrame, "PAGO", "RESTO", importesLayout );
	editFechaPago = addEditField( pControlsFrame, "PAGO", "FECHAPAGO", pagoLayout );
	editDocumentoPago = addEditField( pControlsFrame, "PAGO", "DOCUMENTOPAGO", pagoLayout );

	searchMonedaCodigo = addSearchField( pControlsFrame, "MONEDA_ID", "MONEDA", "CODIGO", "NOMBRE", leftMonedaLayout );
	pushMonedaCodigo = searchMonedaCodigo->getButton();
	connect( pushMonedaCodigo, SIGNAL( clicked() ), this, SLOT( pushMonedaCodigo_clicked() ) );
	editMonedaCodigo = searchMonedaCodigo->getEditCode();
	editMonedaNombre = searchMonedaCodigo->getEditDesc();

#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	searchCuentaPagoCodigo = addSearchField( pControlsFrame, "CUENTAPAGO_ID", "CUENTATESORERIA", "CODIGO", "NOMBRE", cuentapagoLayout );
	pushCuentaPagoCodigo = searchCuentaPagoCodigo->getButton();
	connect( pushCuentaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCodigo_clicked() ) );
	editCuentaPagoCodigo = searchCuentaPagoCodigo->getEditCode();
	editCuentaPagoNombre = searchCuentaPagoCodigo->getEditDesc();
}
#endif

#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	searchCuentaPagoCuenta = addSearchField( pControlsFrame, "CUENTAPAGO_ID", "CUENTA", "CUENTA", "DESCRIPCION", contabLayout );
	pushCuentaPagoCuenta = searchCuentaPagoCuenta->getButton();
	connect( pushCuentaPagoCuenta, SIGNAL( clicked() ), this, SLOT( pushCuentaPagoCuenta_clicked() ) );
	editCuentaPagoCuenta = searchCuentaPagoCuenta->getEditCode();
	editCuentaPagoDescripcion = searchCuentaPagoCuenta->getEditDesc();
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editCuentaOrigen = addEditField( pControlsFrame, "PAGO", "CUENTAORIGEN", contabLayout );
}
#endif
	editNotas = addTextField( pControlsFrame, "PAGO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( numeroLayout );
	pControlsLayout->addLayout( descLayout );
	pControlsLayout->addLayout( fechasLayout );
	pControlsLayout->addLayout( importesLayout );
	pControlsLayout->addLayout( pagoLayout );
	pControlsLayout->addLayout( leftMonedaLayout );
	alignLayout( leftMonedaLayout, true );
	pControlsLayout->addLayout( cuentapagoLayout );
	pControlsLayout->addLayout( contabLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITPAGO_INIT_CONTROLS*/
    pushPagar = new QPushButton( toGUI( _("&Pagar o anular") ), this, "pushPagar" );
    pushPagar->setDefault( true );
    connect ( pushPagar, SIGNAL ( clicked() ), this, SLOT ( pushPagar_clicked() ) );
    pButtonsLayout->addWidget( pushPagar );
    checkAutomatico->setMustBeReadOnly( true );
	editFechaPago->setMustBeReadOnly(true);
	editDocumentoPago->setMustBeReadOnly(true);
#ifdef HAVE_TESORERIAMODULE
	searchCuentaPagoCodigo->setMustBeReadOnly(true);
#endif
#ifdef HAVE_CONTABMODULE		
	searchCuentaPagoCuenta->setMustBeReadOnly(true);
	editCuentaOrigen->setMustBeReadOnly(true);
#endif	
}

void FrmEditPago::cambiaFields()
{
	Xtring tablafacturas = pRecord->getValue( "TABLAFACTURAS" ).toString();
	Xtring fldfactcodigo, fldfactdesc;
	Xtring tablaterceros = pRecord->getValue( "TABLATERCEROS" ).toString();
	Xtring fldterccodigo, fldtercdesc, descsingular, descplural;
	bool femenina = false;
    if( !searchFacturaNumero || tablafacturas != searchFacturaNumero->getTableName()
            || !searchTerceroCodigo || tablaterceros != searchTerceroCodigo->getTableName() ) {

		if( tablafacturas != Xtring::null ) {
			removeControl( searchFacturaNumero );
			mFldFactCodigo = fldfactcodigo;
			mFldFactDesc = fldfactdesc;
			if( fldfactcodigo == Xtring::null ) {
				dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablafacturas );
				if( tbldef ) {
					mFldFactCodigo = tbldef->getCodeField();
					mFldFactDesc = tbldef->getDescField();
				}
			}
			searchFacturaNumero = addSearchField( pControlsFrame, "FACTURA_ID", tablafacturas,
												mFldFactCodigo, mFldFactDesc, pFacturasLayout );
			alignLayout( pFacturasLayout, true );
			pushFacturaNumero = searchFacturaNumero->getButton();
			connect( pushFacturaNumero, SIGNAL( clicked() ), this, SLOT( pushFacturaNumero_clicked() ), Qt::UniqueConnection );
			editFacturaNumero = searchFacturaNumero->getEditCode();
			connect( editFacturaNumero, SIGNAL( specialControlKeyPressed( QWidget *, char ) ),
					this, SLOT( specialControlKeyPressed( QWidget *, char ) ), Qt::UniqueConnection );
			editFacturaDesc = searchFacturaNumero->getEditDesc();
			editFacturaNumero->setWidthInChars(15);
			if( !isInserting() )
				searchFacturaNumero->setMustBeReadOnly(true);
			setTabOrder( editFacturaNumero, editNumero );
		} else {
			searchFacturaNumero = 0;
			pushFacturaNumero = 0;
		}

        removeControl( searchTerceroCodigo );
		if( tablaterceros != Xtring::null) {
			mFldTercCodigo = fldterccodigo;
			mFldTercDesc = fldtercdesc;
			if( fldterccodigo == Xtring::null ) {
				dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablaterceros );
				if( tbldef ) {
					dbFieldDefinition *flddeffc = tbldef->findFieldByFlags( dbFieldDefinition::CODE );
					if( flddeffc )
						mFldTercCodigo = flddeffc->getName();
					dbFieldDefinition *flddeffd = tbldef->findFieldByFlags( dbFieldDefinition::DESCRIPTION );
					if( flddeffd )
						mFldTercDesc = flddeffd->getName();
				}
			}
			searchTerceroCodigo = addSearchField( pControlsFrame, "TERCERO_ID", tablaterceros,
												mFldTercCodigo, mFldTercDesc, pTercerosLayout );
			alignLayout( pTercerosLayout, true );
			pushTerceroCodigo = searchTerceroCodigo->getButton();
			connect( pushTerceroCodigo, SIGNAL( clicked() ), this, SLOT( pushTerceroCodigo_clicked() ), Qt::UniqueConnection );
			editTerceroCodigo = searchTerceroCodigo->getEditCode();
			connect( editTerceroCodigo, SIGNAL( specialControlKeyPressed( QWidget *, char ) ),
				this, SLOT( specialControlKeyPressed( QWidget *, char ) ), Qt::UniqueConnection );
			editTerceroDesc = searchTerceroCodigo->getEditDesc();
			if( !isInserting() ) 
				searchTerceroCodigo->setMustBeReadOnly(true);
			setTabOrder( editFacturaNumero, editTerceroCodigo );
			setTabOrder( editTerceroCodigo, editNumero );
			editFacturaNumero->setWidthInChars(15);
		} else {
			searchTerceroCodigo = 0;
			pushTerceroCodigo = 0;
		}
    }
}

void FrmEditPago::scatterFactura()
{
    editFacturaNumero->setText( getRecFactura()->getValue( mFldFactCodigo ) );
    editFacturaDesc->setText( getRecFactura()->getValue(mFldFactDesc) );
}

void FrmEditPago::pushFacturaNumero_clicked()
{
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
            setWiseFocus(editFacturaNumero);
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
            setWiseFocus(editFacturaNumero);
        }
    }
    break;
    case 'E':
    {
        if( getRecFactura()->getRecordID() != 0 ) {
            editFacturaNumero->setJustEdited( false );
            DBAPP->createClient( DBAPP->createEditForm(this, getRecFactura(),
                                                  0, DataTable::selecting, dbApplication::simpleEdition, this ) );
        }
    }
    break;
    case 'A':
    {
        RecFactura *tmprec = static_cast<RecFactura *>(DBAPP->createRecord( getRecFactura()->getTableName() ));
        editFacturaNumero->setJustEdited( false );
        tmprec->clear( true ); // set default values
        DBAPP->setCodeNotFound( editFacturaNumero->toString() );
        if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
                              dbApplication::simpleEdition, this ) ) {
            editFacturaNumero->setJustEdited( true );
            getRecFactura()->copyRecord( tmprec );
            scatterFactura();
        }
        setWiseFocus(editFacturaNumero);
        DBAPP->setCodeNotFound( Xtring() );
    }
    break;
    }
}

#ifdef HAVE_CONTABMODULE
void FrmEditPago::scatterCuentaPago()
{
    if( getRecord()->getTableDefinition()->findFieldDefinition("CUENTAPAGO_ID") ) {
/*<<<<<FRMEDITPAGO_SCATTER_CUENTAPAGO*/
	editCuentaPagoCuenta->setText( getRecCuentaPago()->getValue("CUENTA") );
	editCuentaPagoDescripcion->setText( getRecCuentaPago()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITPAGO_SCATTER_CUENTAPAGO*/
    }
}
#elif defined (HAVE_TESORERIAMODULE)
void FrmEditPago::scatterCuentaPago()
{
	editCuentaPagoCodigo->setText( getRecCuentaPago()->getValue("CODIGO") );
	editCuentaPagoNombre->setText( getRecCuentaPago()->getValue("NOMBRE") );
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
				setWiseFocus(editCuentaPagoCuenta);
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
				setWiseFocus(editCuentaPagoCuenta);
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaPago()->getRecordID() != 0 ) {
					editCuentaPagoCuenta->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecCuentaPago(),
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
				setWiseFocus(editCuentaPagoCuenta);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPAGO_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
#endif
}

void FrmEditPago::scatterTercero()
{
    editTerceroCodigo->setText( getRecTercero()->getValue( mFldTercCodigo ) );
    editTerceroDesc->setText( getRecTercero()->getValue(mFldTercDesc ) );
}

void FrmEditPago::pushTerceroCodigo_clicked()
{
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
            setWiseFocus(editTerceroCodigo);
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
            setWiseFocus(editTerceroCodigo);
        }
    }
    break;
    case 'E':
    {
        if( getRecTercero()->getRecordID() != 0 ) {
            editTerceroCodigo->setJustEdited( false );
            DBAPP->createClient( DBAPP->createEditForm(this, getRecTercero(),
                                                  0, DataTable::selecting, dbApplication::simpleEdition, this ) );
        }
    }
    break;
    case 'A':
    {
        RecTercero *tmprec = static_cast<RecTercero *>(DBAPP->createRecord( getRecTercero()->getTableName() ));
        editTerceroCodigo->setJustEdited( false );
        tmprec->clear( true ); // set default values
        DBAPP->setCodeNotFound( editTerceroCodigo->toString() );
        if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
                              dbApplication::simpleEdition, this ) ) {
            editTerceroCodigo->setJustEdited( true );
            getRecTercero()->copyRecord( tmprec );
            scatterTercero();
        }
        setWiseFocus(editTerceroCodigo);
        DBAPP->setCodeNotFound( Xtring() );
    }
    break;
    }
}

void FrmEditPago::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITPAGO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editMonedaCodigo )
		pushMonedaCodigo_clicked();
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	if( sender == editCuentaPagoCodigo )
		pushCuentaPagoCodigo_clicked();
}
#endif
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
	if (isFirstScatter()) {
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
		cambiaFields();
		scatterFactura();
		scatterTercero();
	}    
/*<<<<<FRMEDITPAGO_SCATTER*/
	checkAutomatico->setChecked(getRecPago()->getValue("AUTOMATICO").toBool());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = checkAutomatico;
	editNumero->setText(getRecPago()->getValue("NUMERO").toInt());
	editNumeroAgrupado->setText(getRecPago()->getValue("NUMEROAGRUPADO").toString());
	editContador->setText(getRecPago()->getValue("CONTADOR").toInt());
	editDescripcion->setText(getRecPago()->getValue("DESCRIPCION").toString());
	editFechaEmision->setText(getRecPago()->getValue("FECHAEMISION").toDate());
	editVencimiento->setText(getRecPago()->getValue("VENCIMIENTO").toDate());
	editFechaValor->setText(getRecPago()->getValue("FECHAVALOR").toDate());
	comboEstadoRecibo->setCurrentItemByValue(getRecPago()->getValue("ESTADORECIBO").toInt());
	editImporte->setText(getRecPago()->getValue("IMPORTE").toMoney());
	editResto->setText(getRecPago()->getValue("RESTO").toMoney());
	editFechaPago->setText(getRecPago()->getValue("FECHAPAGO").toDate());
	editDocumentoPago->setText(getRecPago()->getValue("DOCUMENTOPAGO").toString());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editCuentaOrigen->setText(getRecPago()->getValue("CUENTAORIGEN").toString());
}
#endif
	editNotas->setText(getRecPago()->getValue("NOTAS").toString());
	scatterMoneda();
#ifdef HAVE_TESORERIAMODULE
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	scatterCuentaPago();
}
#endif
#endif
#ifdef HAVE_CONTABMODULE
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	scatterCuentaPago();
}
#endif
#endif
/*>>>>>FRMEDITPAGO_SCATTER*/
    if( isInserting() )
        comboEstadoRecibo->setCurrentItemByValue( pagos::PagosModule::ReciboPendiente );
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
        searchMonedaCodigo->setMustBeReadOnly(true);
    } else {
        if( editNumero->toInt() == 0 )
            editNumero->setText( getRecPago()->selectNextInt( "NUMERO" ) );
        if( editNumeroAgrupado->toInt() == 0 )
            editNumeroAgrupado->setText( editNumero->toInt() );
    }
    if( getRecPago()->getValue("ESTADORECIBO").toInt() == PagosModule::ReciboPagado ) {
        editVencimiento->setMustBeReadOnly( true );
        editFechaValor->setMustBeReadOnly(true);
    }
    if( editContador->toInt() == 0 )
        editContador->setText( empresa::ModuleInstance->getMaxContador() );
	if( isInserting() ) 
		editResto->setMustBeReadOnly(true);
	if( editFacturaNumero->isEnabled() )
        pFocusWidget = editFacturaNumero;
    else
        pFocusWidget = editNumero;
}

void FrmEditPago::gatherFields()
{
/*<<<<<FRMEDITPAGO_GATHER*/
	getRecPago()->setValue( "AUTOMATICO", checkAutomatico->isChecked());
	getRecPago()->setValue( "NUMERO", editNumero->toInt());
	getRecPago()->setValue( "NUMEROAGRUPADO", editNumeroAgrupado->toString());
	getRecPago()->setValue( "CONTADOR", editContador->toInt());
	getRecPago()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecPago()->setValue( "FECHAEMISION", editFechaEmision->toDate());
	getRecPago()->setValue( "VENCIMIENTO", editVencimiento->toDate());
	getRecPago()->setValue( "FECHAVALOR", editFechaValor->toDate());
	getRecPago()->setValue( "ESTADORECIBO", comboEstadoRecibo->getCurrentItemValue());
	getRecPago()->setValue( "IMPORTE", editImporte->toMoney());
	getRecPago()->setValue( "RESTO", editResto->toMoney());
	getRecPago()->setValue( "FECHAPAGO", editFechaPago->toDate());
	getRecPago()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
	getRecPago()->setValue( "MONEDA_ID", getRecMoneda()->getRecordID() );
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	getRecPago()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecPago()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecPago()->setValue( "CUENTAORIGEN", editCuentaOrigen->toString());
}
#endif
	getRecPago()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPAGO_GATHER*/
    getRecPago()->setValue( "RAZONSOCIAL", getRecTercero()->getValue( mFldTercDesc ) );
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
				setWiseFocus(editMonedaCodigo);
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
				setWiseFocus(editMonedaCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecMoneda()->getRecordID() != 0 ) {
					editMonedaCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecMoneda(),
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
				setWiseFocus(editMonedaCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPAGO_PUSH_MONEDA_CODIGO_CLICKED*/
}

void FrmEditPago::pushCuentaPagoCodigo_clicked()
{
#ifdef HAVE_TESORERIAMODULE
/*<<<<<FRMEDITPAGO_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaPagoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaPagoCodigo->setJustEdited( false );
			editCuentaPagoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecCuentaPago(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaPago();
				editCuentaPagoCodigo->setJustEdited( true );
				setWiseFocus(editCuentaPagoCodigo);
			}
			break;
		case 'M':
			{
				if( getRecCuentaPago()->getRecordID() ) {
					editCuentaPagoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaPago(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaPagoCodigo->setJustEdited( true );
						scatterCuentaPago();
					}
				setWiseFocus(editCuentaPagoCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaPago()->getRecordID() != 0 ) {
					editCuentaPagoCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecCuentaPago(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecCuentaPago *tmprec = static_cast<RecCuentaPago *>(DBAPP->createRecord( "CuentaPago" ));
				editCuentaPagoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaPagoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaPagoCodigo->setJustEdited( true );
					getRecCuentaPago()->copyRecord( tmprec );
					scatterCuentaPago();
				}
				setWiseFocus(editCuentaPagoCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPAGO_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
#endif
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
	if( focusWidget() != pushMonedaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editMonedaCodigo, editMonedaNombre,
		getRecMoneda(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterMoneda();
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	if( focusWidget() != pushCuentaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaPagoCodigo, editCuentaPagoNombre,
		getRecCuentaPago(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterCuentaPago();
}
#endif
/*>>>>>FRMEDITPAGO_VALIDATE*/
    if( focusWidget() != pushFacturaNumero) // To avoid triggering the validating if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editFacturaNumero, editFacturaDesc,
                           getRecFactura(), mFldFactCodigo, mFldFactDesc, Xtring::null) ) {
            getRecord()->setValue( "FACTURA_ID", 0 );
            scatterFactura();
        }
    if( pushTerceroCodigo && focusWidget() != pushTerceroCodigo) // To avoid triggering the validating if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editTerceroCodigo, editTerceroDesc,
                           getRecTercero(), mFldTercCodigo, mFldTercDesc, Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) ) {
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
    if( !sender && isInserting() ) {
        int contador = empresa::ModuleInstance->getMaxContador();
        if( contador > editContador->toInt() ) {
            editContador->setText( contador );
            validresult->addWarning( Xtring::printf(
                                         _("El contador ha cambiado durante la edición de %s. El nuevo contador es %d"),
                                         DBAPP->getTableDescSingular( pRecord->getTableName(), "esta" ).c_str(), contador ), "CONTADOR" );
        }
    }
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

void FrmEditPago::pushPagar_clicked()
{
	bool refrescar = false;
    if( mMustRead )
        read();
    if( IPagableRecord *pr = dynamic_cast<IPagableRecord *>(getRecFactura() ) ) {
		bool realizado;
        if( getRecPago()->getValue( "ESTADORECIBO").toInt() == PagosModule::ReciboPendiente ) {
#ifdef HAVE_CONTABMODULE
            bool supervisar = contab::ModuleInstance->getModuleSetting( "SUPERVISAR_ASIENTOS" ).toBool();
#elif defined( HAVE_TESORERIAMODULE)
            bool supervisar = tesoreria::ModuleInstance->getModuleSetting( "SUPERVISAR_APUNTES" ).toBool();
#else
            bool supervisar = false;
#endif
            refrescar = pr->pagarRecibo( this, getRecPago()->getRecordID(), getRecPago(), supervisar );
        } else if( getRecPago()->getValue( "ESTADORECIBO").toInt() == PagosModule::ReciboPagado ) {
            refrescar = pr->anularPagoRecibo( this, getRecPago()->getRecordID(), getRecPago());
        }
        if( refrescar )
			refresh();
    }
}

bool FrmEditPago::canBeginEdit(EditMode newmode)
{
    if( !FrmEditRecMaster::canBeginEdit( newmode ) )
        return false;
    if( newmode == DataTable::inserting || newmode == DataTable::duplicating ) {
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


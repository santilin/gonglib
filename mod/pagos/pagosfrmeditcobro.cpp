// Tercero_ID Reference(Tercero,Codigo,Desc,dbRecord::InsertIfNotFound) - terceros
// Factura_ID Reference(Factura,Numero,Fecha) - terceros
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de cobros
// FIELD RemesaCobro_ID Reference(RemesaCobro,Numero,Descripcion) - remesaLayout
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
    searchFacturaNumero = 0;
    searchTerceroCodigo = 0;
    pTercerosLayout = new QHBoxLayout();
    pControlsLayout->addLayout(pTercerosLayout);
    pFacturasLayout = new QHBoxLayout();
    pControlsLayout->addLayout(pFacturasLayout);
/*<<<<<FRMEDITCOBRO_INIT_CONTROLS*/
	QHBoxLayout *remesaLayout = new QHBoxLayout(0, 0, 6, "remesaLayout");
	QHBoxLayout *numeroLayout = new QHBoxLayout(0, 0, 6, "numeroLayout");
	QHBoxLayout *descLayout = new QHBoxLayout(0, 0, 6, "descLayout");
	QHBoxLayout *fechasLayout = new QHBoxLayout(0, 0, 6, "fechasLayout");
	QHBoxLayout *importesLayout = new QHBoxLayout(0, 0, 6, "importesLayout");
	QHBoxLayout *pagoLayout = new QHBoxLayout(0, 0, 6, "pagoLayout");
	QHBoxLayout *leftMonedaLayout = new QHBoxLayout(0, 0, 6, "leftMonedaLayout");
	QHBoxLayout *cuentapagoLayout = new QHBoxLayout(0, 0, 6, "cuentapagoLayout");
	QHBoxLayout *contabLayout = new QHBoxLayout(0, 0, 6, "contabLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");

	searchRemesaCobroNumero = addSearchField( pControlsFrame, "REMESACOBRO_ID", "REMESACOBRO", "NUMERO", "DESCRIPCION", remesaLayout );
	pushRemesaCobroNumero = searchRemesaCobroNumero->getButton();
	connect( pushRemesaCobroNumero, SIGNAL( clicked() ), this, SLOT( pushRemesaCobroNumero_clicked() ) );
	editRemesaCobroNumero = searchRemesaCobroNumero->getEditCode();
	editRemesaCobroDescripcion = searchRemesaCobroNumero->getEditDesc();
	checkAutomatico = addCheckField( pControlsFrame, "COBRO", "AUTOMATICO", numeroLayout );
	editNumero = addEditField( pControlsFrame, "COBRO", "NUMERO", numeroLayout );
	editNumeroAgrupado = addEditField( pControlsFrame, "COBRO", "NUMEROAGRUPADO", numeroLayout );
	editContador = addEditField( pControlsFrame, "COBRO", "CONTADOR", numeroLayout );
	editDescripcion = addEditField( pControlsFrame, "COBRO", "DESCRIPCION", descLayout );
	editFechaEmision = addEditField( pControlsFrame, "COBRO", "FECHAEMISION", fechasLayout );
	editVencimiento = addEditField( pControlsFrame, "COBRO", "VENCIMIENTO", fechasLayout );
	editFechaValor = addEditField( pControlsFrame, "COBRO", "FECHAVALOR", fechasLayout );
	comboEstadoRecibo = addComboIntField( pControlsFrame, "COBRO", "ESTADORECIBO", importesLayout );
	editImporte = addEditField( pControlsFrame, "COBRO", "IMPORTE", importesLayout );
	editResto = addEditField( pControlsFrame, "COBRO", "RESTO", importesLayout );
	editFechaPago = addEditField( pControlsFrame, "COBRO", "FECHAPAGO", pagoLayout );
	editDocumentoPago = addEditField( pControlsFrame, "COBRO", "DOCUMENTOPAGO", pagoLayout );

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
	editCuentaOrigen = addEditField( pControlsFrame, "COBRO", "CUENTAORIGEN", contabLayout );
}
#endif
	editNotas = addTextField( pControlsFrame, "COBRO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( remesaLayout );
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
/*>>>>>FRMEDITCOBRO_INIT_CONTROLS*/
    pushPagar = new QPushButton( toGUI( _("Co&brar o anular") ), this, "pushPagar" );
    pushPagar->setDefault( true );
    connect ( pushPagar, SIGNAL ( clicked() ), this, SLOT ( pushPagar_clicked() ) );
    pButtonsLayout->addWidget( pushPagar );
    mUsarRemesas = ModuleInstance->getModuleSetting("USAR_REMESAS").toBool();
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

void FrmEditCobro::setUsarRemesas(bool usar, bool fijar)
{
    searchRemesaCobroNumero->setVisible( usar );
	searchRemesaCobroNumero->setMustBeReadOnly( fijar );
}


void FrmEditCobro::completa(const Xtring& tablafacturas, const Xtring& fldfactcodigo, const Xtring& fldfactdesc,
                            const Xtring& tablaterceros, const Xtring& fldterccodigo, const Xtring& fldtercdesc,
                            const Xtring& descsingular, const Xtring& descplural, bool femenina)
{
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
			if( !isInserting() )
				searchFacturaNumero->setMustBeReadOnly(true);
			setTabOrder( editRemesaCobroNumero, editFacturaNumero );
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

void FrmEditCobro::scatterFields()
{
	if (isFirstScatter()) {
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
		completa( pRecord->getValue( "TABLAFACTURAS" ).toString(), Xtring::null, Xtring::null,
					pRecord->getValue( "TABLATERCEROS" ).toString(), Xtring::null, Xtring::null,
					Xtring::null, Xtring::null, false );
		scatterFactura();
		scatterTercero();
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
	comboEstadoRecibo->setCurrentItemByValue(getRecCobro()->getValue("ESTADORECIBO").toInt());
	editImporte->setText(getRecCobro()->getValue("IMPORTE").toMoney());
	editResto->setText(getRecCobro()->getValue("RESTO").toMoney());
	editFechaPago->setText(getRecCobro()->getValue("FECHAPAGO").toDate());
	editDocumentoPago->setText(getRecCobro()->getValue("DOCUMENTOPAGO").toString());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editCuentaOrigen->setText(getRecCobro()->getValue("CUENTAORIGEN").toString());
}
#endif
	editNotas->setText(getRecCobro()->getValue("NOTAS").toString());
	scatterRemesaCobro();
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
/*>>>>>FRMEDITCOBRO_SCATTER*/
    if( isInserting() ) {
        if( ModuleInstance->getRecRemesaCobroPadre( this ) ) {
            getRecRemesaCobro()->copyRecord( ModuleInstance->getRecRemesaCobroPadre( this ) );
            scatterRemesaCobro();
        }
        comboEstadoRecibo->setCurrentItemByValue( pagos::PagosModule::ReciboPendiente );
    }
    if( getRecCobro()->getValue("AUTOMATICO").toBool() ) {
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
	if( isInserting() ) 
		editResto->setMustBeReadOnly(true);
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
	getRecCobro()->setValue( "AUTOMATICO", checkAutomatico->isChecked());
	getRecCobro()->setValue( "NUMERO", editNumero->toInt());
	getRecCobro()->setValue( "NUMEROAGRUPADO", editNumeroAgrupado->toString());
	getRecCobro()->setValue( "CONTADOR", editContador->toInt());
	getRecCobro()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecCobro()->setValue( "FECHAEMISION", editFechaEmision->toDate());
	getRecCobro()->setValue( "VENCIMIENTO", editVencimiento->toDate());
	getRecCobro()->setValue( "FECHAVALOR", editFechaValor->toDate());
	getRecCobro()->setValue( "ESTADORECIBO", comboEstadoRecibo->getCurrentItemValue());
	getRecCobro()->setValue( "IMPORTE", editImporte->toMoney());
	getRecCobro()->setValue( "RESTO", editResto->toMoney());
	getRecCobro()->setValue( "FECHAPAGO", editFechaPago->toDate());
	getRecCobro()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
	getRecCobro()->setValue( "MONEDA_ID", getRecMoneda()->getRecordID() );
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	getRecCobro()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecCobro()->setValue( "CUENTAPAGO_ID", getRecCuentaPago()->getRecordID() );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecCobro()->setValue( "CUENTAORIGEN", editCuentaOrigen->toString());
}
#endif
	getRecCobro()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITCOBRO_GATHER*/
    getRecCobro()->setValue( "RAZONSOCIAL", getRecTercero()->getValue( mFldTercDesc ) );
}


void FrmEditCobro::scatterFactura()
{
    editFacturaNumero->setText( getRecFactura()->getValue( mFldFactCodigo ) );
    editFacturaDesc->setText( getRecFactura()->getValue(mFldFactDesc) );
}

void FrmEditCobro::pushFacturaNumero_clicked()
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
void FrmEditCobro::scatterCuentaPago()
{
    if( getRecord()->getTableDefinition()->findFieldDefinition("CUENTAPAGO_ID") ) {
/*<<<<<FRMEDITCOBRO_SCATTER_CUENTAPAGO*/
	editCuentaPagoCuenta->setText( getRecCuentaPago()->getValue("CUENTA") );
	editCuentaPagoDescripcion->setText( getRecCuentaPago()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITCOBRO_SCATTER_CUENTAPAGO*/
    }
}
#elif defined (HAVE_TESORERIAMODULE)
void FrmEditCobro::scatterCuentaPago()
{
	editCuentaPagoCodigo->setText( getRecCuentaPago()->getValue("CODIGO") );
	editCuentaPagoNombre->setText( getRecCuentaPago()->getValue("NOMBRE") );
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
/*>>>>>FRMEDITCOBRO_PUSH_CUENTAPAGO_CUENTA_CLICKED*/
#endif
}

void FrmEditCobro::scatterTercero()
{
    editTerceroCodigo->setText( getRecTercero()->getValue( mFldTercCodigo ) );
    editTerceroDesc->setText( getRecTercero()->getValue(mFldTercDesc ) );
}

void FrmEditCobro::pushTerceroCodigo_clicked()
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

void FrmEditCobro::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITCOBRO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editRemesaCobroNumero )
		pushRemesaCobroNumero_clicked();
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
/*>>>>>FRMEDITCOBRO_SPECIALACTION*/
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
				setWiseFocus(editRemesaCobroNumero);
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
				setWiseFocus(editRemesaCobroNumero);
				}
			}
			break;
		case 'E':
			{
				if( getRecRemesaCobro()->getRecordID() != 0 ) {
					editRemesaCobroNumero->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecRemesaCobro(),
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
				setWiseFocus(editRemesaCobroNumero);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITCOBRO_PUSH_REMESACOBRO_NUMERO_CLICKED*/
}

void FrmEditCobro::pushCuentaPagoCodigo_clicked()
{
#ifdef HAVE_TESORERIAMODULE
/*<<<<<FRMEDITCOBRO_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITCOBRO_PUSH_CUENTAPAGO_CODIGO_CLICKED*/
#endif
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
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	if( focusWidget() != pushCuentaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaPagoCodigo, editCuentaPagoNombre,
		getRecCuentaPago(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterCuentaPago();
}
#endif
/*>>>>>FRMEDITCOBRO_VALIDATE*/
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

void FrmEditCobro::pushPagar_clicked()
{
	bool refrescar = false;
    if( mMustRead )
        read();
    if( IPagableRecord *pr = dynamic_cast<IPagableRecord *>(getRecFactura() ) ) {
        if( getRecCobro()->getValue( "ESTADORECIBO" ).toInt() == PagosModule::ReciboPendiente ) {
#ifdef HAVE_CONTABMODULE
            bool supervisar = contab::ModuleInstance->getModuleSetting( "SUPERVISAR_ASIENTOS" ).toBool();
#elif defined( HAVE_TESORERIAMODULE)
            bool supervisar = tesoreria::ModuleInstance->getModuleSetting( "SUPERVISAR_APUNTES" ).toBool();
#else
            bool supervisar = false;
#endif
            refrescar = pr->cobrarRecibo(this, getRecCobro()->getRecordID(), getRecCobro(), supervisar );
        } else if( getRecCobro()->getValue( "ESTADORECIBO" ).toInt() == PagosModule::ReciboPagado ) {
            refrescar = pr->anularCobroRecibo( this, getRecCobro()->getRecordID(), getRecCobro() );
        }
        if( refrescar )
			refresh();
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

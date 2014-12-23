/*<<<<<MODULE_INFO*/
// COPYLEFT Formulario de edición de formas de pago
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD TipoFormaPago comboint - tipo
// FIELD NumPlazos int - plazos
// FIELD DiasEntrePlazos int - plazos
// FIELD DiasPrimerPlazo int - plazos
// FIELD DtoEnFactura float - dto
// FIELD SubcuentaPago string - sub1 MODULE_INCLUDED(Contab)
// FIELD SubcuentaCobro string - sub2 MODULE_INCLUDED(Contab)
// FIELD CuentaTesoreria_ID Reference(tesoreria::CuentaTesoreria,Codigo,Nombre) - sub1 MODULE_INCLUDED(Tesoreria)
// TYPE FrmEditRecMaster pagos::FormaPago validCodeAndDesc IncCode CodeNotFound
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITFORMAPAGO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "pagosfrmeditformapago.h"
/*>>>>>FRMEDITFORMAPAGO_INCLUDES*/
#include "pagosmodule.h"

namespace gong {
namespace pagos {

/*<<<<<FRMEDITFORMAPAGO_CONSTRUCTOR*/
FrmEditFormaPago::FrmEditFormaPago(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditFormaPago" );
/*>>>>>FRMEDITFORMAPAGO_CONSTRUCTOR*/

    /*<<<<<FRMEDITFORMAPAGO_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *tipoLayout = new QHBoxLayout(0, 0, 6, "tipoLayout");
	QHBoxLayout *plazosLayout = new QHBoxLayout(0, 0, 6, "plazosLayout");
	QHBoxLayout *dtoLayout = new QHBoxLayout(0, 0, 6, "dtoLayout");
	QHBoxLayout *sub1Layout = new QHBoxLayout(0, 0, 6, "sub1Layout");
	QHBoxLayout *sub2Layout = new QHBoxLayout(0, 0, 6, "sub2Layout");
	editCodigo = addEditField( pControlsFrame, "FORMAPAGO", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "FORMAPAGO", "NOMBRE", codigoLayout );
	comboTipoFormaPago = addComboIntField( pControlsFrame, "FORMAPAGO", "TIPOFORMAPAGO", tipoLayout );
	editNumPlazos = addEditField( pControlsFrame, "FORMAPAGO", "NUMPLAZOS", plazosLayout );
	editDiasEntrePlazos = addEditField( pControlsFrame, "FORMAPAGO", "DIASENTREPLAZOS", plazosLayout );
	editDiasPrimerPlazo = addEditField( pControlsFrame, "FORMAPAGO", "DIASPRIMERPLAZO", plazosLayout );
	editDtoEnFactura = addEditField( pControlsFrame, "FORMAPAGO", "DTOENFACTURA", dtoLayout );
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaPago = addEditField( pControlsFrame, "FORMAPAGO", "SUBCUENTAPAGO", sub1Layout );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaCobro = addEditField( pControlsFrame, "FORMAPAGO", "SUBCUENTACOBRO", sub2Layout );
}
#endif

#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	searchCuentaTesoreriaCodigo = addSearchField( pControlsFrame, "CUENTATESORERIA_ID", "CUENTATESORERIA", "CODIGO", "NOMBRE", sub1Layout );
	pushCuentaTesoreriaCodigo = searchCuentaTesoreriaCodigo->getButton();
	connect( pushCuentaTesoreriaCodigo, SIGNAL( clicked() ), this, SLOT( pushCuentaTesoreriaCodigo_clicked() ) );
	editCuentaTesoreriaCodigo = searchCuentaTesoreriaCodigo->getEditCode();
	editCuentaTesoreriaNombre = searchCuentaTesoreriaCodigo->getEditDesc();
}
#endif
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( tipoLayout );
	pControlsLayout->addLayout( plazosLayout );
	pControlsLayout->addLayout( dtoLayout );
	pControlsLayout->addLayout( sub1Layout );
	pControlsLayout->addLayout( sub2Layout );
/*>>>>>FRMEDITFORMAPAGO_INIT_CONTROLS*/
}

void FrmEditFormaPago::scatterCuentaTesoreria()
{
#ifdef HAVE_TESORERIAMODULE
    /*<<<<<FRMEDITFORMAPAGO_SCATTER_CUENTATESORERIA*/
	editCuentaTesoreriaCodigo->setText( getRecCuentaTesoreria()->getValue("CODIGO") );
	editCuentaTesoreriaNombre->setText( getRecCuentaTesoreria()->getValue("NOMBRE") );
/*>>>>>FRMEDITFORMAPAGO_SCATTER_CUENTATESORERIA*/
#endif
}

void FrmEditFormaPago::pushCuentaTesoreriaCodigo_clicked()
{
#ifdef HAVE_TESORERIAMODULE
    /*<<<<<FRMEDITFORMAPAGO_PUSH_CUENTATESORERIA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaTesoreriaCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaTesoreriaCodigo->setJustEdited( false );
			editCuentaTesoreriaCodigo->setCancelling();
			if( DBAPP->choose(this, getRecCuentaTesoreria(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaTesoreria();
				editCuentaTesoreriaCodigo->setJustEdited( true );
				setWiseFocus(editCuentaTesoreriaCodigo);
			}
			break;
		case 'M':
			{
				if( getRecCuentaTesoreria()->getRecordID() ) {
					editCuentaTesoreriaCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaTesoreria(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaTesoreriaCodigo->setJustEdited( true );
						scatterCuentaTesoreria();
					}
				setWiseFocus(editCuentaTesoreriaCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaTesoreria()->getRecordID() != 0 ) {
					editCuentaTesoreriaCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCuentaTesoreria(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				tesoreria::RecCuentaTesoreria *tmprec = static_cast<tesoreria::RecCuentaTesoreria *>(DBAPP->createRecord( "CuentaTesoreria" ));
				editCuentaTesoreriaCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaTesoreriaCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaTesoreriaCodigo->setJustEdited( true );
					getRecCuentaTesoreria()->copyRecord( tmprec );
					scatterCuentaTesoreria();
				}
				setWiseFocus(editCuentaTesoreriaCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITFORMAPAGO_PUSH_CUENTATESORERIA_CODIGO_CLICKED*/
#endif
}

void FrmEditFormaPago::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITFORMAPAGO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	if( sender == editCuentaTesoreriaCodigo )
		pushCuentaTesoreriaCodigo_clicked();
}
#endif
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITFORMAPAGO_SPECIALACTION*/
}

void FrmEditFormaPago::scatterFields()
{
/*<<<<<FRMEDITFORMAPAGO_SCATTER*/
	editCodigo->setText(getRecFormaPago()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecFormaPago()->getValue("NOMBRE").toString());
	comboTipoFormaPago->setCurrentItemByValue(getRecFormaPago()->getValue("TIPOFORMAPAGO").toInt());
	editNumPlazos->setText(getRecFormaPago()->getValue("NUMPLAZOS").toInt());
	editDiasEntrePlazos->setText(getRecFormaPago()->getValue("DIASENTREPLAZOS").toInt());
	editDiasPrimerPlazo->setText(getRecFormaPago()->getValue("DIASPRIMERPLAZO").toInt());
	editDtoEnFactura->setText(getRecFormaPago()->getValue("DTOENFACTURA").toDouble());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaPago->setText(getRecFormaPago()->getValue("SUBCUENTAPAGO").toString());
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaCobro->setText(getRecFormaPago()->getValue("SUBCUENTACOBRO").toString());
}
#endif
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
	if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
		if( DBAPP->codeNotFound().toInt() != 0 ) {
			editCodigo->setText( DBAPP->codeNotFound() );
			editCodigo->setJustEdited( true );
		} else {
		editNombre->setText( DBAPP->codeNotFound() );
		editNombre->setJustEdited( true );
		}
	}
#ifdef HAVE_TESORERIAMODULE
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	scatterCuentaTesoreria();
}
#endif
#endif
/*>>>>>FRMEDITFORMAPAGO_SCATTER*/
    enableControlesPago();
}

void FrmEditFormaPago::gatherFields()
{
    /*<<<<<FRMEDITFORMAPAGO_GATHER*/
	getRecFormaPago()->setValue( "CODIGO", editCodigo->toInt());
	getRecFormaPago()->setValue( "NOMBRE", editNombre->toString());
	getRecFormaPago()->setValue( "TIPOFORMAPAGO", comboTipoFormaPago->getCurrentItemValue());
	getRecFormaPago()->setValue( "NUMPLAZOS", editNumPlazos->toInt());
	getRecFormaPago()->setValue( "DIASENTREPLAZOS", editDiasEntrePlazos->toInt());
	getRecFormaPago()->setValue( "DIASPRIMERPLAZO", editDiasPrimerPlazo->toInt());
	getRecFormaPago()->setValue( "DTOENFACTURA", editDtoEnFactura->toDouble());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecFormaPago()->setValue( "SUBCUENTAPAGO", editSubcuentaPago->toString());
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecFormaPago()->setValue( "SUBCUENTACOBRO", editSubcuentaCobro->toString());
}
#endif
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	getRecFormaPago()->setValue( "CUENTATESORERIA_ID", getRecCuentaTesoreria()->getRecordID() );
}
#endif
/*>>>>>FRMEDITFORMAPAGO_GATHER*/
}

void FrmEditFormaPago::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITFORMAPAGO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
#ifdef HAVE_TESORERIAMODULE
if( ModuleInstance->getTesoreriaModule() ) {
	if( focusWidget() != pushCuentaTesoreriaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaTesoreriaCodigo, editCuentaTesoreriaNombre,
		getRecCuentaTesoreria(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterCuentaTesoreria();
}
#endif
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITFORMAPAGO_VALIDATE*/
    if( sender == comboTipoFormaPago && comboTipoFormaPago->isJustEdited() ) {
        enableControlesPago();
    }
    switch( comboTipoFormaPago->getCurrentItemValue() ) {
    case RecFormaPago::Contado:
    case RecFormaPago::Pendiente:
    case RecFormaPago::SeIgnora:
        break;
    case RecFormaPago::GeneraRecibos:
        if( editNumPlazos->isJustEdited() ) {
            if( editNumPlazos->toInt() == 0 ) {
                validresult->addError( _("El número de plazos tiene que ser mayor que cero"), "NUMPLAZOS" );
                *isvalid = false;
            } else if( editNumPlazos->toInt() > 0 ) {
                if( !sender && editDiasEntrePlazos->toInt() == 0 && editDiasPrimerPlazo->toInt() == 0 ) {
                    validresult->addError( _("El número de días entre plazos o de días hasta el primer plazo tiene que ser mayor que cero"), "NUMPLAZOS" );
                    *isvalid = false;
                }
            }
        }
        break;
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditFormaPago::enableControlesPago()
{
    bool enable;
    switch( comboTipoFormaPago->getCurrentItemValue() ) {
    case RecFormaPago::Contado:
    case RecFormaPago::Pendiente:
    case RecFormaPago::SeIgnora:
        enable = false;
        editNumPlazos->setText( 0 );
        editDiasEntrePlazos->setText( 0 );
        editDiasPrimerPlazo->setText( 0 );
        break;
    case RecFormaPago::GeneraRecibos:
        enable = true;
        break;
    }
    editNumPlazos->setMustBeReadOnly( !enable );
    editDiasEntrePlazos->setMustBeReadOnly( !enable );
    editDiasPrimerPlazo->setMustBeReadOnly( !enable );
}


/*<<<<<FRMEDITFORMAPAGO_FIN*/
} // namespace pagos
} // namespace gong
/*>>>>>FRMEDITFORMAPAGO_FIN*/


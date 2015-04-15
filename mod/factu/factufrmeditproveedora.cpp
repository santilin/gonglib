/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de proveedoras
// FIELD Codigo int - codigo
// FIELD RazonSocial string - codigo
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre,dbRecord::InsertIfNotFound) - formapago
// FIELD NombreAlt string
// FIELD Agente_ID Reference(Agente,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - agente
// FIELD IVADetallado comboint - agente
// FIELD EntidadBanco string tabExtra cuentabanco
// FIELD CuentaBanco string tabExtra cuentabanco
// FIELD Subcuenta string tabExtra leftsubcuentas MODULE_INCLUDED(Contab)
// FIELD GenCodArticulo comboint frameGenCodArt
// FIELD FormatCodArticulo string frameGenCodArt
// FIELD UsaReferencias bool frameGenCodArt
// FIELD Notas text - notas
// TYPE FrmEditRecMaster factu::Proveedora IncCode
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITPROVEEDORA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditproveedora.h"
/*>>>>>FRMEDITPROVEEDORA_INCLUDES*/
#include <contactosfrmeditcontactobehavior.h>
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITPROVEEDORA_CONSTRUCTOR*/
FrmEditProveedora::FrmEditProveedora(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditProveedora" );
/*>>>>>FRMEDITPROVEEDORA_CONSTRUCTOR*/
    QFrame *frameGenCodArt = new QFrame( pControlsFrame );
    QTabWidget *pFrameContactos = new QTabWidget(pControlsFrame);
    (new QVBoxLayout( pFrameContactos ));
    pFrameContactos->setObjectName( "FrameContactos" );
    /*<<<<<FRMEDITPROVEEDORA_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *formapagoLayout = new QHBoxLayout(0, 0, 6, "formapagoLayout");
	QHBoxLayout *nombrealtLayout = new QHBoxLayout(0, 0, 6, "nombrealtLayout");
	QHBoxLayout *agenteLayout = new QHBoxLayout(0, 0, 6, "agenteLayout");
	showTabs(true);
	QWidget *tabExtra = new QWidget( pTabWidget, "tabExtra" );
	QVBoxLayout *tabExtraLayout = new QVBoxLayout(tabExtra, 11, 6, "tabExtraLayout");
	QHBoxLayout *cuentabancoLayout = new QHBoxLayout(0, 0, 6, "cuentabancoLayout");
	QHBoxLayout *leftsubcuentasLayout = new QHBoxLayout(0, 0, 6, "leftsubcuentasLayout");
	QVBoxLayout *frameGenCodArtLayout = new QVBoxLayout(frameGenCodArt, 11, 6, "frameGenCodArtLayout");
	QHBoxLayout *gencodarticuloLayout = new QHBoxLayout(0, 0, 6, "gencodarticuloLayout");
	QHBoxLayout *formatcodarticuloLayout = new QHBoxLayout(0, 0, 6, "formatcodarticuloLayout");
	QHBoxLayout *usareferenciasLayout = new QHBoxLayout(0, 0, 6, "usareferenciasLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "PROVEEDORA", "CODIGO", codigoLayout );
	editRazonSocial = addEditField( pControlsFrame, "PROVEEDORA", "RAZONSOCIAL", codigoLayout );

	searchFormaPagoCodigo = addSearchField( pControlsFrame, "FORMAPAGO_ID", "FORMAPAGO", "CODIGO", "NOMBRE", formapagoLayout );
	pushFormaPagoCodigo = searchFormaPagoCodigo->getButton();
	connect( pushFormaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushFormaPagoCodigo_clicked() ) );
	editFormaPagoCodigo = searchFormaPagoCodigo->getEditCode();
	editFormaPagoNombre = searchFormaPagoCodigo->getEditDesc();
	editNombreAlt = addEditField( pControlsFrame, "PROVEEDORA", "NOMBREALT", nombrealtLayout );

	searchAgenteCodigo = addSearchField( pControlsFrame, "AGENTE_ID", "AGENTE", "CODIGO", "RAZONSOCIAL", agenteLayout );
	pushAgenteCodigo = searchAgenteCodigo->getButton();
	connect( pushAgenteCodigo, SIGNAL( clicked() ), this, SLOT( pushAgenteCodigo_clicked() ) );
	editAgenteCodigo = searchAgenteCodigo->getEditCode();
	editAgenteRazonSocial = searchAgenteCodigo->getEditDesc();
	comboIVADetallado = addComboIntField( pControlsFrame, "PROVEEDORA", "IVADETALLADO", agenteLayout );
	editEntidadBanco = addEditField( tabExtra, "PROVEEDORA", "ENTIDADBANCO", cuentabancoLayout );
	editCuentaBanco = addEditField( tabExtra, "PROVEEDORA", "CUENTABANCO", cuentabancoLayout );
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuenta = addEditField( tabExtra, "PROVEEDORA", "SUBCUENTA", leftsubcuentasLayout );
}
#endif
	comboGenCodArticulo = addComboIntField( frameGenCodArt, "PROVEEDORA", "GENCODARTICULO", gencodarticuloLayout );
	editFormatCodArticulo = addEditField( frameGenCodArt, "PROVEEDORA", "FORMATCODARTICULO", formatcodarticuloLayout );
	checkUsaReferencias = addCheckField( frameGenCodArt, "PROVEEDORA", "USAREFERENCIAS", usareferenciasLayout );
	editNotas = addTextField( pControlsFrame, "PROVEEDORA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( formapagoLayout );
	pControlsLayout->addLayout( nombrealtLayout );
	pControlsLayout->addLayout( agenteLayout );
	tabExtraLayout->addLayout( cuentabancoLayout );
	tabExtraLayout->addLayout( leftsubcuentasLayout );
	alignLayout( leftsubcuentasLayout, true );
	frameGenCodArtLayout->addLayout( gencodarticuloLayout );
	frameGenCodArtLayout->addLayout( formatcodarticuloLayout );
	frameGenCodArtLayout->addLayout( usareferenciasLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITPROVEEDORA_INIT_CONTROLS*/
// {capel} Añadir detrás de addLayout( codigoLayout )
//	pControlsLayout->addWidget( pFrameContactos );
    pEditContactoBehavior = new contactos::FrmEditContactoBehavior( this, pFrameContactos, editRazonSocial );
    addBehavior( pEditContactoBehavior );
    pTabWidget->insertTab( tabExtra, toGUI( _( "&Extra" ) ) );
    pEditContactoBehavior->_initGUI();
    tabExtraLayout->addWidget( frameGenCodArt );
}

void FrmEditProveedora::scatterFields()
{
    if( isDuplicating() )
        getRecProveedora()->setValue( "CODIGO", 0 );
    pEditContactoBehavior->setTabOrders( editRazonSocial, searchFormaPagoCodigo );
    /*<<<<<FRMEDITPROVEEDORA_SCATTER*/
	editCodigo->setText(getRecProveedora()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editRazonSocial->setText(getRecProveedora()->getValue("RAZONSOCIAL").toString());
	editNombreAlt->setText(getRecProveedora()->getValue("NOMBREALT").toString());
	comboIVADetallado->setCurrentItemByValue(getRecProveedora()->getValue("IVADETALLADO").toInt());
	editEntidadBanco->setText(getRecProveedora()->getValue("ENTIDADBANCO").toString());
	editCuentaBanco->setText(getRecProveedora()->getValue("CUENTABANCO").toString());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuenta->setText(getRecProveedora()->getValue("SUBCUENTA").toString());
}
#endif
	comboGenCodArticulo->setCurrentItemByValue(getRecProveedora()->getValue("GENCODARTICULO").toInt());
	editFormatCodArticulo->setText(getRecProveedora()->getValue("FORMATCODARTICULO").toString());
	checkUsaReferencias->setChecked(getRecProveedora()->getValue("USAREFERENCIAS").toBool());
	editNotas->setText(getRecProveedora()->getValue("NOTAS").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
	scatterFormaPago();
	scatterAgente();
/*>>>>>FRMEDITPROVEEDORA_SCATTER*/
    if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
        if( DBAPP->codeNotFound().toInt() != 0 ) {
            editCodigo->setText( DBAPP->codeNotFound() );
            editCodigo->setJustEdited( true );
        } else {
            editRazonSocial->setText( DBAPP->codeNotFound() );
            editRazonSocial->setJustEdited( true );
        }
    }
    pFocusWidget = editRazonSocial;
}

void FrmEditProveedora::gatherFields()
{
    /*<<<<<FRMEDITPROVEEDORA_GATHER*/
	getRecProveedora()->setValue( "CODIGO", editCodigo->toInt());
	getRecProveedora()->setValue( "RAZONSOCIAL", editRazonSocial->toString());
	getRecProveedora()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
	getRecProveedora()->setValue( "NOMBREALT", editNombreAlt->toString());
	getRecProveedora()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecProveedora()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecProveedora()->setValue( "ENTIDADBANCO", editEntidadBanco->toString());
	getRecProveedora()->setValue( "CUENTABANCO", editCuentaBanco->toString());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecProveedora()->setValue( "SUBCUENTA", editSubcuenta->toString());
}
#endif
	getRecProveedora()->setValue( "GENCODARTICULO", comboGenCodArticulo->getCurrentItemValue());
	getRecProveedora()->setValue( "FORMATCODARTICULO", editFormatCodArticulo->toString());
	getRecProveedora()->setValue( "USAREFERENCIAS", checkUsaReferencias->isChecked());
	getRecProveedora()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPROVEEDORA_GATHER*/
}

void FrmEditProveedora::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPROVEEDORA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushFormaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editFormaPagoCodigo, editFormaPagoNombre,
		getRecFormaPago(), "CODIGO", "NOMBRE", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterFormaPago();
	if( focusWidget() != pushAgenteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editAgenteCodigo, editAgenteRazonSocial,
		getRecAgente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterAgente();
/*>>>>>FRMEDITPROVEEDORA_VALIDATE*/
    if( !validCodeAndDesc( sender, *validresult, editCodigo, editRazonSocial, "codigo", "razonsocial" ) )
        if( !sender )
            *isvalid = false;
    if( sender == editRazonSocial && editRazonSocial->isJustEdited()
            && validresult->countErrors() == 0 && pEditContactoBehavior->isCreating() ) {
        if( pEditContactoBehavior->getEditCIF()->toString().isEmpty()
                && pEditContactoBehavior->getEditNombre()->toString().isEmpty() )			if( pEditContactoBehavior->setCIFAndLookForIt( editRazonSocial->toString() ) ) {
                DBAPP->showOSD( fromGUI( windowTitle() ),
                                Xtring::printf( _("Se ha encontrado %s para %s"),
                                                DBAPP->getTableDescSingular("CONTACTO", "una").c_str(),
                                                DBAPP->getTableDescSingular("CLIENTE", "esta").c_str()
                                              ) );
                editRazonSocial->setText( pEditContactoBehavior->getEditNombre()->toString() );
            }
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

#if 0
void FrmEditProveedora::scatterContacto()
{
    /*<<<<<FRMEDITPROVEEDORA_SCATTER_CONTACTO*/
    editContactoCIF->setText( getRecContacto()->getValue("CIF") );
    editContactoNombre->setText( getRecContacto()->getValue("NOMBRE") );
    /*>>>>>FRMEDITPROVEEDORA_SCATTER_CONTACTO*/
}

void FrmEditProveedora::pushContactoCIF_clicked()
{
    /*<<<<<FRMEDITPROVEEDORA_PUSH_CONTACTO_CIF_CLICKED*/
    char action = mControlKeyPressed;
    if( !isEditing() )
        action = 'E';
    switch( action ) {
    case 'F':
    case '\0':
        editContactoCIF->setJustEdited( false );
        editContactoCIF->setCancelling();
        if( DBAPP->choose(this, getRecContacto(), 0, dbApplication::editNone, this ) ) {
            setEdited(true);
            scatterContacto();
            editContactoCIF->setJustEdited( true );
            editContactoCIF->setFocus();
        }
        break;
    case 'M':
    {
        if( getRecContacto()->getRecordID() ) {
            editContactoCIF->setJustEdited( false );
            if( DBAPP->editRecord(this,
                                  getRecContacto(), 0, DataTable::updating,
                                  dbApplication::simpleEdition, this ) ) {
                editContactoCIF->setJustEdited( true );
                scatterContacto();
            }
            editContactoCIF->setFocus();
        }
    }
    break;
    case 'E':
    {
        editContactoCIF->setJustEdited( false );
        DBAPP->createClient( DBAPP->createEditForm(this,
                                              getRecContacto(), 0, DataTable::selecting,
                                              dbApplication::simpleEdition, this ) );
    }
    break;
    case 'A':
    {
        contactos::RecContacto *tmprec = static_cast<contactos::RecContacto *>(DBAPP->createRecord( "Contacto" ));
        editContactoCIF->setJustEdited( false );
        tmprec->clear( true ); // set default values
        DBAPP->setCodeNotFound( editContactoCIF->toString() );
        if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
                              dbApplication::simpleEdition, this ) ) {
            editContactoCIF->setJustEdited( true );
            getRecContacto()->copyRecord( tmprec );
            scatterContacto();
        }
        editContactoCIF->setFocus();
        DBAPP->setCodeNotFound( Xtring() );
    }
    break;
    }
    /*>>>>>FRMEDITPROVEEDORA_PUSH_CONTACTO_CIF_CLICKED*/
}
#endif

void FrmEditProveedora::scatterAgente()
{
    /*<<<<<FRMEDITPROVEEDORA_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITPROVEEDORA_SCATTER_AGENTE*/
}
void FrmEditProveedora::pushAgenteCodigo_clicked()
{
    /*<<<<<FRMEDITPROVEEDORA_PUSH_AGENTE_CODIGO_CLICKED*/
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
				setWiseFocus(editAgenteCodigo);
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
				setWiseFocus(editAgenteCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecAgente()->getRecordID() != 0 ) {
					editAgenteCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecAgente(),
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
				setWiseFocus(editAgenteCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPROVEEDORA_PUSH_AGENTE_CODIGO_CLICKED*/
}
void FrmEditProveedora::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITPROVEEDORA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editFormaPagoCodigo )
		pushFormaPagoCodigo_clicked();
	if( sender == editAgenteCodigo )
		pushAgenteCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITPROVEEDORA_SPECIALACTION*/
}

void FrmEditProveedora::scatterFormaPago()
{
    /*<<<<<FRMEDITPROVEEDORA_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITPROVEEDORA_SCATTER_FORMAPAGO*/
}
void FrmEditProveedora::pushFormaPagoCodigo_clicked()
{
    /*<<<<<FRMEDITPROVEEDORA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchFormaPagoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editFormaPagoCodigo->setJustEdited( false );
			editFormaPagoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecFormaPago(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterFormaPago();
				editFormaPagoCodigo->setJustEdited( true );
				setWiseFocus(editFormaPagoCodigo);
			}
			break;
		case 'M':
			{
				if( getRecFormaPago()->getRecordID() ) {
					editFormaPagoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecFormaPago(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editFormaPagoCodigo->setJustEdited( true );
						scatterFormaPago();
					}
				setWiseFocus(editFormaPagoCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecFormaPago()->getRecordID() != 0 ) {
					editFormaPagoCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecFormaPago(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				pagos::RecFormaPago *tmprec = static_cast<pagos::RecFormaPago *>(DBAPP->createRecord( "FormaPago" ));
				editFormaPagoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editFormaPagoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editFormaPagoCodigo->setJustEdited( true );
					getRecFormaPago()->copyRecord( tmprec );
					scatterFormaPago();
				}
				setWiseFocus(editFormaPagoCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPROVEEDORA_PUSH_FORMAPAGO_CODIGO_CLICKED*/
}

/*<<<<<FRMEDITPROVEEDORA_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITPROVEEDORA_FIN*/



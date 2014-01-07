/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de clientes
// FIELD Codigo int - codigo
// FIELD RazonSocial string - codigo
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre,dbApplication::InsertIfNotFound)
// FIELD NombreAlt string
// FIELD DtoP100 double - leftotros
// FIELD Activo bool - leftotros
// FIELD Tarifa int - leftotros
// FIELD Agente_ID Reference(Agente,Codigo,RazonSocial,dbApplication::InsertIfNotFound) tabExtra
// FIELD IVADetallado comboint tabExtra tipo
// FIELD TipoCliente comboint tabExtra tipo
// FIELD EntidadBanco string tabExtra cuentabanco
// FIELD CuentaBanco string tabExtra cuentabanco
// FIELD Subcuenta string tabExtra leftsubcuentas MODULE_INCLUDED(Contab)
// FIELD Notas text tabExtra
// TYPE FrmEditRecMaster factu::Cliente IncCode
/*>>>>>MODULE_INFO*/
#include <gongpushbutton.h>
/*<<<<<FRMEDITCLIENTE_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditcliente.h"
/*>>>>>FRMEDITCLIENTE_INCLUDES*/
#include <contactosfrmeditcontactobehavior.h>
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITCLIENTE_CONSTRUCTOR*/
FrmEditCliente::FrmEditCliente(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditCliente" );
/*>>>>>FRMEDITCLIENTE_CONSTRUCTOR*/
    QTabWidget *pFrameContactos = new QTabWidget(pControlsFrame);
    new QVBoxLayout( pFrameContactos );
    pFrameContactos->setObjectName( "FrameContactos" );
    /*<<<<<FRMEDITCLIENTE_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *formapago_idLayout = new QHBoxLayout(0, 0, 6, "formapago_idLayout");
	QHBoxLayout *nombrealtLayout = new QHBoxLayout(0, 0, 6, "nombrealtLayout");
	QHBoxLayout *leftotrosLayout = new QHBoxLayout(0, 0, 6, "leftotrosLayout");
	showTabs(true);
	QWidget *tabExtra = new QWidget( pTabWidget, "tabExtra" );
	QVBoxLayout *tabExtraLayout = new QVBoxLayout(tabExtra, 11, 6, "tabExtraLayout");
	QHBoxLayout *agente_idLayout = new QHBoxLayout(0, 0, 6, "agente_idLayout");
	QHBoxLayout *tipoLayout = new QHBoxLayout(0, 0, 6, "tipoLayout");
	QHBoxLayout *cuentabancoLayout = new QHBoxLayout(0, 0, 6, "cuentabancoLayout");
	QHBoxLayout *leftsubcuentasLayout = new QHBoxLayout(0, 0, 6, "leftsubcuentasLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "CLIENTE", "CODIGO", codigoLayout );
	editRazonSocial = addEditField( pControlsFrame, "CLIENTE", "RAZONSOCIAL", codigoLayout );

	searchFormaPagoCodigo = addSearchField( pControlsFrame, "FORMAPAGO_ID", "FORMAPAGO", "CODIGO", "NOMBRE", formapago_idLayout );
	pushFormaPagoCodigo = searchFormaPagoCodigo->getButton();
	connect( pushFormaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushFormaPagoCodigo_clicked() ) );
	editFormaPagoCodigo = searchFormaPagoCodigo->getEditCode();
	editFormaPagoNombre = searchFormaPagoCodigo->getEditDesc();
	editNombreAlt = addEditField( pControlsFrame, "CLIENTE", "NOMBREALT", nombrealtLayout );
	editDtoP100 = addEditField( pControlsFrame, "CLIENTE", "DTOP100", leftotrosLayout );
	checkActivo = addCheckField( pControlsFrame, "CLIENTE", "ACTIVO", leftotrosLayout );
	editTarifa = addEditField( pControlsFrame, "CLIENTE", "TARIFA", leftotrosLayout );

	searchAgenteCodigo = addSearchField( tabExtra, "AGENTE_ID", "AGENTE", "CODIGO", "RAZONSOCIAL", agente_idLayout );
	pushAgenteCodigo = searchAgenteCodigo->getButton();
	connect( pushAgenteCodigo, SIGNAL( clicked() ), this, SLOT( pushAgenteCodigo_clicked() ) );
	editAgenteCodigo = searchAgenteCodigo->getEditCode();
	editAgenteRazonSocial = searchAgenteCodigo->getEditDesc();
	comboIVADetallado = addComboIntField( tabExtra, "CLIENTE", "IVADETALLADO", tipoLayout );
	comboTipoCliente = addComboIntField( tabExtra, "CLIENTE", "TIPOCLIENTE", tipoLayout );
	editEntidadBanco = addEditField( tabExtra, "CLIENTE", "ENTIDADBANCO", cuentabancoLayout );
	editCuentaBanco = addEditField( tabExtra, "CLIENTE", "CUENTABANCO", cuentabancoLayout );
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuenta = addEditField( tabExtra, "CLIENTE", "SUBCUENTA", leftsubcuentasLayout );
}
#endif
	editNotas = addTextField( tabExtra, "CLIENTE", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addWidget( pFrameContactos );
	pControlsLayout->addLayout( formapago_idLayout );
	pControlsLayout->addLayout( nombrealtLayout );
	pControlsLayout->addLayout( leftotrosLayout );
	alignLayout( leftotrosLayout, true );
	tabExtraLayout->addLayout( agente_idLayout );
	tabExtraLayout->addLayout( tipoLayout );
	tabExtraLayout->addLayout( cuentabancoLayout );
	tabExtraLayout->addLayout( leftsubcuentasLayout );
	alignLayout( leftsubcuentasLayout, true );
	tabExtraLayout->addLayout( notasLayout );
/*>>>>>FRMEDITCLIENTE_INIT_CONTROLS*/
// {capel} Añadir detrás de addLayout( codigoLayout )
//	pControlsLayout->addWidget( pFrameContactos );
    pEditContactoBehavior = new contactos::FrmEditContactoBehavior( this, pFrameContactos, editRazonSocial );
    addBehavior( pEditContactoBehavior );
	pEditContactoBehavior->_initGUI();
    pTabWidget->insertTab( tabExtra, toGUI( _( "&Extra" ) ) );
}

void FrmEditCliente::scatterFields()
{
    if( isDuplicating() )
        getRecCliente()->setValue( "CODIGO", 0 );
    pEditContactoBehavior->setTabOrders( editRazonSocial, searchFormaPagoCodigo );
    /*<<<<<FRMEDITCLIENTE_SCATTER*/
	editCodigo->setText(getRecCliente()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editRazonSocial->setText(getRecCliente()->getValue("RAZONSOCIAL").toString());
	editNombreAlt->setText(getRecCliente()->getValue("NOMBREALT").toString());
	editDtoP100->setText(getRecCliente()->getValue("DTOP100").toDouble());
	checkActivo->setChecked(getRecCliente()->getValue("ACTIVO").toBool());
	editTarifa->setText(getRecCliente()->getValue("TARIFA").toInt());
	comboIVADetallado->setCurrentItemByValue(getRecCliente()->getValue("IVADETALLADO").toInt());
	comboTipoCliente->setCurrentItemByValue(getRecCliente()->getValue("TIPOCLIENTE").toInt());
	editEntidadBanco->setText(getRecCliente()->getValue("ENTIDADBANCO").toString());
	editCuentaBanco->setText(getRecCliente()->getValue("CUENTABANCO").toString());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuenta->setText(getRecCliente()->getValue("SUBCUENTA").toString());
}
#endif
	editNotas->setText(getRecCliente()->getValue("NOTAS").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
	scatterFormaPago();
	scatterAgente();
/*>>>>>FRMEDITCLIENTE_SCATTER*/
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

void FrmEditCliente::gatherFields()
{
    /*<<<<<FRMEDITCLIENTE_GATHER*/
	getRecCliente()->setValue( "CODIGO", editCodigo->toInt());
	getRecCliente()->setValue( "RAZONSOCIAL", editRazonSocial->toString());
	getRecCliente()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
	getRecCliente()->setValue( "NOMBREALT", editNombreAlt->toString());
	getRecCliente()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecCliente()->setValue( "ACTIVO", checkActivo->isChecked());
	getRecCliente()->setValue( "TARIFA", editTarifa->toInt());
	getRecCliente()->setValue( "AGENTE_ID", getRecAgente()->getRecordID() );
	getRecCliente()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecCliente()->setValue( "TIPOCLIENTE", comboTipoCliente->getCurrentItemValue());
	getRecCliente()->setValue( "ENTIDADBANCO", editEntidadBanco->toString());
	getRecCliente()->setValue( "CUENTABANCO", editCuentaBanco->toString());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecCliente()->setValue( "SUBCUENTA", editSubcuenta->toString());
}
#endif
	getRecCliente()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITCLIENTE_GATHER*/
}

void FrmEditCliente::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITCLIENTE_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editFormaPagoCodigo )
		pushFormaPagoCodigo_clicked();
	if( sender == editAgenteCodigo )
		pushAgenteCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITCLIENTE_SPECIALACTION*/
}

void FrmEditCliente::scatterFormaPago()
{
    /*<<<<<FRMEDITCLIENTE_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITCLIENTE_SCATTER_FORMAPAGO*/
}
void FrmEditCliente::pushFormaPagoCodigo_clicked()
{
    /*<<<<<FRMEDITCLIENTE_PUSH_FORMAPAGO_CODIGO_CLICKED*/
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
				editFormaPagoCodigo->setFocus();
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
					editFormaPagoCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecFormaPago()->getRecordID() != 0 ) {
					editFormaPagoCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecFormaPago(),
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
				editFormaPagoCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITCLIENTE_PUSH_FORMAPAGO_CODIGO_CLICKED*/
}


void FrmEditCliente::scatterAgente()
{
    /*<<<<<FRMEDITCLIENTE_SCATTER_AGENTE*/
	editAgenteCodigo->setText( getRecAgente()->getValue("CODIGO") );
	editAgenteRazonSocial->setText( getRecAgente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITCLIENTE_SCATTER_AGENTE*/
}
void FrmEditCliente::pushAgenteCodigo_clicked()
{
    /*<<<<<FRMEDITCLIENTE_PUSH_AGENTE_CODIGO_CLICKED*/
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
/*>>>>>FRMEDITCLIENTE_PUSH_AGENTE_CODIGO_CLICKED*/
}

void FrmEditCliente::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITCLIENTE_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushFormaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editFormaPagoCodigo, editFormaPagoNombre,
		getRecFormaPago(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterFormaPago();
	if( focusWidget() != pushAgenteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editAgenteCodigo, editAgenteRazonSocial,
		getRecAgente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterAgente();
/*>>>>>FRMEDITCLIENTE_VALIDATE*/
    if( !validCodeAndDesc( sender, *validresult, editCodigo, editRazonSocial, "codigo", "razonsocial" ) )
        if( !sender )
            *isvalid = false;
    if( sender == editRazonSocial && editRazonSocial->isJustEdited()
            && validresult->countErrors() == 0 && pEditContactoBehavior->isCreating() ) {
        if( pEditContactoBehavior->getEditCIF()->toString().isEmpty()
                && pEditContactoBehavior->getEditNombre()->toString().isEmpty() )		if( pEditContactoBehavior->getEditCIF()->toString().isEmpty() )
                if( pEditContactoBehavior->setCIFAndLookForIt( editRazonSocial->toString() ) ) {
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

bool FrmEditCliente::save()
{
    Xtring tablename;
    tablename = "CONTACTO";
    List<dbRecordID> &recslist = mContactoList;
    if( recslist.size() > 1 ) {
        if( !isInserting() ) {
            msgOk(this,
                  Xtring::printf( _("Sólo se pueden elegir múltiples %s cuando se está añadiendo %s"),
                                  DBAPP->getTableDescPlural( tablename, "<" ).c_str(),
                                  DBAPP->getTableDescPlural( "MIEMBRO", "<" ).c_str() ) );
            return false;
        }
        if( msgYesNo(this,
                     Xtring::printf( _("Has seleccionado %d %s. ¿Estás seguro/a de que quieres añadir %s?"),
                                     recslist.size(),
                                     DBAPP->getTableDescPlural( tablename, "<" ).c_str(),
                                     DBAPP->getTableDescPlural( tablename, "todas las" ).c_str() ) ) ) {
            return saveAllContactos();
        } else {
            return false;
        }
    } else
        return FrmEditRecMaster::save();
}

bool FrmEditCliente::saveAllContactos()
{
    bool ret = true;
#if 0
    Xtring tablename;
    tablename = "CONTACTO";
    List<dbRecordID> &recslist = mContactoList;
    DBAPP->waitCursor( true );
    int cont = 0, errores = 0;
    Xtring report;
    for( List<dbRecordID>::const_iterator it = recslist.begin();
            it != recslist.end(); ++ it ) {
        getRecCliente()->setNew( true );
        gatherFields();
        // Poner el mismo código al cliente que al contacto si es posible.
        getRecCliente()->setValue( "CODIGO", getRecContacto()->getValue( "CODIGO" ) );
        if( getRecCliente()->existsAnother(Xtring::null) )
            getRecCliente()->setValue( "CODIGO", getRecord()->selectNextInt( "CODIGO" ) );
        getRecCliente()->setValue( "CONTACTO_ID", *it );
        getRecCliente()->readRelated( true ); // force
        getRecCliente()->setValue( "RAZONSOCIAL", getRecContacto()->getValue( "NOMBRE" ) );
        try {
            if( !FrmEditRecMaster::save() ) {
                ret = false;
                errores ++;
            } else {
                report += _("Añadido: ") + getRecCliente()->toString( TOSTRING_CODE_AND_DESC ) + "\n";
                cont++;
            }
        } catch( dbError &e ) {
            report += _("Error: ") + getRecCliente()->toString( TOSTRING_CODE_AND_DESC ) + "\n    "
                      + e.what() + "\n";
            errores++;
        }
    }
    report += Xtring::printf( _("\nSe han añadido %d %s. Ha habido %d errores"),
                              cont, DBAPP->getTableDescPlural( tablename, "<" ).c_str(),
                              errores );
    DBAPP->resetCursor();
    msgOkLarge(this, _("Informe"), report );
#endif
    return ret;
}

#if 0
Elección múltiple de contactos
void FrmEditCliente::scatterContacto()
{
    if( mContactoList.size() > 1 ) {
        editContactoCIF->setText( Xtring::null );
        editContactoCIF->setEdited( false );
        editContactoCIF->setJustEdited( false );
        editContactoNombre->setText( Xtring::printf( _("%d %s elegidos"),
                                     mContactoList.size(),
                                     DBAPP->getTableDescPlural( "CONTACTO", "" ).c_str() ) );
    } else {
        /*<<<<<FRMEDITCLIENTE_SCATTER_CONTACTO*/
        editContactoCIF->setText( getRecContacto()->getValue("CIF") );
        editContactoNombre->setText( getRecContacto()->getValue("NOMBRE") );
        /*>>>>>FRMEDITCLIENTE_SCATTER_CONTACTO*/
    }

}

#endif


/*<<<<<FRMEDITCLIENTE_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITCLIENTE_FIN*/




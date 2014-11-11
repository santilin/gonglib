/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de empresas
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD Moneda_ID Reference(Moneda,Codigo,Nombre,dbApplication::InsertIfNotFound) - moneda
// FIELD EntidadBanco string - cuentabanco
// FIELD CuentaBanco string - cuentabanco
// FIELD SoloLectura bool - opciones
// FIELD UsarProyectos bool - opciones
// FIELD Notas text - notas
// FIELD Logo image tabLogo logo
// TYPE FrmEditRecMaster empresa::Empresa validCodeAndDesc IncCode CodeNotFound
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
#include <gongimagebox.h>
/*<<<<<FRMEDITEMPRESA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "empresafrmeditempresa.h"
/*>>>>>FRMEDITEMPRESA_INCLUDES*/
#include <contactosfrmeditcontactobehavior.h>
#include "empresamodule.h"

namespace gong {
namespace empresa {

/*<<<<<FRMEDITEMPRESA_CONSTRUCTOR*/
FrmEditEmpresa::FrmEditEmpresa(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditEmpresa" );
/*>>>>>FRMEDITEMPRESA_CONSTRUCTOR*/
    QTabWidget *pFrameContactos = new QTabWidget(this);
    new QVBoxLayout( pFrameContactos );
    pFrameContactos->setObjectName( "FrameContactos" );
    /*<<<<<FRMEDITEMPRESA_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *monedaLayout = new QHBoxLayout(0, 0, 6, "monedaLayout");
	QHBoxLayout *cuentabancoLayout = new QHBoxLayout(0, 0, 6, "cuentabancoLayout");
	QHBoxLayout *opcionesLayout = new QHBoxLayout(0, 0, 6, "opcionesLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	showTabs(true);
	QWidget *tabLogo = new QWidget( pTabWidget, "tabLogo" );
	QVBoxLayout *tabLogoLayout = new QVBoxLayout(tabLogo, 11, 6, "tabLogoLayout");
	QHBoxLayout *logoLayout = new QHBoxLayout(0, 0, 6, "logoLayout");
	editCodigo = addEditField( pControlsFrame, "EMPRESA", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "EMPRESA", "NOMBRE", codigoLayout );

	searchMonedaCodigo = addSearchField( pControlsFrame, "MONEDA_ID", "MONEDA", "CODIGO", "NOMBRE", monedaLayout );
	pushMonedaCodigo = searchMonedaCodigo->getButton();
	connect( pushMonedaCodigo, SIGNAL( clicked() ), this, SLOT( pushMonedaCodigo_clicked() ) );
	editMonedaCodigo = searchMonedaCodigo->getEditCode();
	editMonedaNombre = searchMonedaCodigo->getEditDesc();
	editEntidadBanco = addEditField( pControlsFrame, "EMPRESA", "ENTIDADBANCO", cuentabancoLayout );
	editCuentaBanco = addEditField( pControlsFrame, "EMPRESA", "CUENTABANCO", cuentabancoLayout );
	checkSoloLectura = addCheckField( pControlsFrame, "EMPRESA", "SOLOLECTURA", opcionesLayout );
	checkUsarProyectos = addCheckField( pControlsFrame, "EMPRESA", "USARPROYECTOS", opcionesLayout );
	editNotas = addTextField( pControlsFrame, "EMPRESA", "NOTAS", notasLayout );
	editLogo = addImageField( tabLogo, "EMPRESA", "LOGO", logoLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addWidget( pFrameContactos );	
	pControlsLayout->addLayout( monedaLayout );
	pControlsLayout->addLayout( cuentabancoLayout );
	pControlsLayout->addLayout( opcionesLayout );
	pControlsLayout->addLayout( notasLayout );
	tabLogoLayout->addLayout( logoLayout );
/*>>>>>FRMEDITEMPRESA_INIT_CONTROLS*/
// {capel} Añadir detrás de codigolayout
//	pControlsLayout->addWidget( pFrameContactos );
    pEditContactoBehavior = new contactos::FrmEditContactoBehavior( this, pFrameContactos, editNombre );
    addBehavior( pEditContactoBehavior );
    pEditContactoBehavior->_initGUI();

    connect( editLogo, SIGNAL( clicked() ), this, SLOT( editLogo_clicked() ) );
    pTabWidget->insertTab( tabLogo, toGUI( _( "&Logo" ) ) );
}

void FrmEditEmpresa::scatterFields()
{
    pEditContactoBehavior->setTabOrders( editNombre, searchMonedaCodigo );
    /*<<<<<FRMEDITEMPRESA_SCATTER*/
	editCodigo->setText(getRecEmpresa()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecEmpresa()->getValue("NOMBRE").toString());
	editEntidadBanco->setText(getRecEmpresa()->getValue("ENTIDADBANCO").toString());
	editCuentaBanco->setText(getRecEmpresa()->getValue("CUENTABANCO").toString());
	checkSoloLectura->setChecked(getRecEmpresa()->getValue("SOLOLECTURA").toBool());
	checkUsarProyectos->setChecked(getRecEmpresa()->getValue("USARPROYECTOS").toBool());
	editNotas->setText(getRecEmpresa()->getValue("NOTAS").toString());
	editLogo->setImageData(getRecEmpresa()->getValue("LOGO"));
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
	if( isInserting() ) {
		getRecMoneda()->setValue( "ID", getRecEmpresa()->getValue("MONEDA_ID") );
		getRecMoneda()->read( getRecMoneda()->getRecordID());
	}	
	
	scatterMoneda();
/*>>>>>FRMEDITEMPRESA_SCATTER*/
    pFocusWidget = editNombre;
    if( pRecord->getConnection()->isRootUser() )
        checkSoloLectura->setMustBeReadOnly( false );
}

void FrmEditEmpresa::gatherFields()
{
    /*<<<<<FRMEDITEMPRESA_GATHER*/
	getRecEmpresa()->setValue( "CODIGO", editCodigo->toInt());
	getRecEmpresa()->setValue( "NOMBRE", editNombre->toString());
	getRecEmpresa()->setValue( "MONEDA_ID", getRecMoneda()->getRecordID() );
	getRecEmpresa()->setValue( "ENTIDADBANCO", editEntidadBanco->toString());
	getRecEmpresa()->setValue( "CUENTABANCO", editCuentaBanco->toString());
	getRecEmpresa()->setValue( "SOLOLECTURA", checkSoloLectura->isChecked());
	getRecEmpresa()->setValue( "USARPROYECTOS", checkUsarProyectos->isChecked());
	getRecEmpresa()->setValue( "NOTAS", editNotas->toString());
	getRecEmpresa()->setValue( "LOGO", editLogo->toData());
/*>>>>>FRMEDITEMPRESA_GATHER*/
    DBAPP->getRegConfig()->setCurrencyFormat(
        getRecMoneda()->getValue( "SIMBOLO" ).toString(),
        getRecMoneda()->getValue( "DECIMALES" ).toInt(),
        getRecMoneda()->getValue( "PUNTODECIMAL" ).toString().isEmpty()
        ? '.' : getRecMoneda()->getValue( "PUNTODECIMAL" ).toString().at( 0 ),
        getRecMoneda()->getValue( "SEPARADORMILES" ).toString().isEmpty()
        ? ',' : getRecMoneda()->getValue( "SEPARADORMILES" ).toString().at( 0 ) );
//	mEditMode = updating;
}

void FrmEditEmpresa::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITEMPRESA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editMonedaCodigo )
		pushMonedaCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITEMPRESA_SPECIALACTION*/
}

void FrmEditEmpresa::scatterMoneda()
{
    /*<<<<<FRMEDITEMPRESA_SCATTER_MONEDA*/
	editMonedaCodigo->setText( getRecMoneda()->getValue("CODIGO") );
	editMonedaNombre->setText( getRecMoneda()->getValue("NOMBRE") );
/*>>>>>FRMEDITEMPRESA_SCATTER_MONEDA*/
}

void FrmEditEmpresa::pushMonedaCodigo_clicked()
{
    /*<<<<<FRMEDITEMPRESA_PUSH_MONEDA_CODIGO_CLICKED*/
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
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecMoneda(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecMoneda *tmprec = static_cast<RecMoneda *>(DBAPP->createRecord( "Moneda" ));
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
/*>>>>>FRMEDITEMPRESA_PUSH_MONEDA_CODIGO_CLICKED*/
}

void FrmEditEmpresa::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITEMPRESA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushMonedaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editMonedaCodigo, editMonedaNombre,
		getRecMoneda(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterMoneda();
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITEMPRESA_VALIDATE*/
    if( sender == editNombre && editNombre->isJustEdited()
            && validresult->countErrors() == 0 && pEditContactoBehavior->isCreating() ) {
        if( pEditContactoBehavior->getEditCIF()->toString().isEmpty()
                && pEditContactoBehavior->getEditNombre()->toString().isEmpty() )
            if( pEditContactoBehavior->setCIFAndLookForIt( editNombre->toString() ) ) {
                DBAPP->showOSD( fromGUI( windowTitle() ),
                                Xtring::printf( _("Se ha encontrado %s para %s"),
                                                DBAPP->getTableDescSingular("CONTACTO", "una").c_str(),
                                                DBAPP->getTableDescSingular("CLIENTE", "esta").c_str()
                                              ) );
                editNombre->setText( pEditContactoBehavior->getEditNombre()->toString() );
            }
    }
    /// TODO: Se podría validar aquí el número de cuenta, pero es un follón, ya se valida al guardar
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditEmpresa::editLogo_clicked()
{
    Xtring fname = GuiApplication::getOpenFileName(
                       Xtring::printf( _( "Elige el logo de %s"), DBAPP->getTableDescSingular("EMPRESA", "la").c_str() ),
                       Xtring::null, _( "Imágenes (*.png *.gif *.jpg)" ), this );
    if ( !fname.isEmpty() ) {
        if ( editLogo->loadFromFile( fname ) )
            editLogo->update();
    }
}


/*<<<<<FRMEDITEMPRESA_FIN*/
} // namespace empresa
} // namespace gong
/*>>>>>FRMEDITEMPRESA_FIN*/




/*<<<<<MODULE_INFO*/
// COPYLEFT Formulario de edición de miembros de la asociación
// FIELD Proyecto_ID Reference(Proyecto,Codigo,Descripcion) - proyectoLayout
// FIELD NumeroSocia int - nsocia
// FIELD FechaAlta date - nsocia
// FIELD SociasEstado comboint - nsocia
// FIELD Cliente_ID Reference(factu::Cliente,Codigo,RazonSocial) - contacto MODULE_INCLUDED(Factu)
// FIELD Contacto_ID Reference(contactos::Contacto,CIF,Nombre) -
// FIELD TipoSocia_ID Reference(TipoSocia,Codigo,Nombre) - tiposocia
// FIELD FormaPago_ID Reference(pagos::FormaPago,Codigo,Nombre) - formapago MODULE_INCLUDED(Pagos)
// FIELD FechaBaja date - baja
// FIELD MotivoBaja string - baja
// FIELD ImporteTotal Money - totales
// FIELD PagosTotal Money - totales
// FIELD RestoTotal Money - totales
// FIELD CuentaBanco String - cuentabanco
// FIELD Notas text - notas
// TYPE FrmEditRecMaster socias::Miembro
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITMIEMBRO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "sociasfrmeditmiembro.h"
/*>>>>>FRMEDITMIEMBRO_INCLUDES*/
#include "sociasfrmeditproyecto.h"
#include "sociasmodule.h"

namespace gong {
namespace socias {

/*<<<<<FRMEDITMIEMBRO_CONSTRUCTOR*/
FrmEditMiembro::FrmEditMiembro(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditMiembro" );
/*>>>>>FRMEDITMIEMBRO_CONSTRUCTOR*/
    searchProyectoCodigo = 0;
    /*<<<<<FRMEDITMIEMBRO_INIT_CONTROLS*/
	QHBoxLayout *proyectoLayout = new QHBoxLayout(0, 0, 6, "proyectoLayout");
	QHBoxLayout *nsociaLayout = new QHBoxLayout(0, 0, 6, "nsociaLayout");
	QHBoxLayout *contactoLayout = new QHBoxLayout(0, 0, 6, "contactoLayout");
	QHBoxLayout *contacto_idLayout = new QHBoxLayout(0, 0, 6, "contacto_idLayout");
	QHBoxLayout *tiposociaLayout = new QHBoxLayout(0, 0, 6, "tiposociaLayout");
	QHBoxLayout *formapagoLayout = new QHBoxLayout(0, 0, 6, "formapagoLayout");
	QHBoxLayout *bajaLayout = new QHBoxLayout(0, 0, 6, "bajaLayout");
	QHBoxLayout *totalesLayout = new QHBoxLayout(0, 0, 6, "totalesLayout");
	QHBoxLayout *cuentabancoLayout = new QHBoxLayout(0, 0, 6, "cuentabancoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");

	searchProyectoCodigo = addSearchField( pControlsFrame, "PROYECTO_ID", "PROYECTO", "CODIGO", "DESCRIPCION", proyectoLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoDescripcion = searchProyectoCodigo->getEditDesc();
	editNumeroSocia = addEditField( pControlsFrame, "MIEMBRO", "NUMEROSOCIA", nsociaLayout );
	editFechaAlta = addEditField( pControlsFrame, "MIEMBRO", "FECHAALTA", nsociaLayout );
	comboSociasEstado = addComboIntField( pControlsFrame, "MIEMBRO", "SOCIASESTADO", nsociaLayout );

#ifdef HAVE_FACTUMODULE
if( ModuleInstance->getFactuModule() ) {
	searchClienteCodigo = addSearchField( pControlsFrame, "CLIENTE_ID", "CLIENTE", "CODIGO", "RAZONSOCIAL", contactoLayout );
	pushClienteCodigo = searchClienteCodigo->getButton();
	connect( pushClienteCodigo, SIGNAL( clicked() ), this, SLOT( pushClienteCodigo_clicked() ) );
	editClienteCodigo = searchClienteCodigo->getEditCode();
	editClienteRazonSocial = searchClienteCodigo->getEditDesc();
}
#endif

	searchContactoCIF = addSearchField( pControlsFrame, "CONTACTO_ID", "CONTACTO", "CIF", "NOMBRE", contacto_idLayout );
	pushContactoCIF = searchContactoCIF->getButton();
	connect( pushContactoCIF, SIGNAL( clicked() ), this, SLOT( pushContactoCIF_clicked() ) );
	editContactoCIF = searchContactoCIF->getEditCode();
	editContactoNombre = searchContactoCIF->getEditDesc();

	searchTipoSociaCodigo = addSearchField( pControlsFrame, "TIPOSOCIA_ID", "TIPOSOCIA", "CODIGO", "NOMBRE", tiposociaLayout );
	pushTipoSociaCodigo = searchTipoSociaCodigo->getButton();
	connect( pushTipoSociaCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoSociaCodigo_clicked() ) );
	editTipoSociaCodigo = searchTipoSociaCodigo->getEditCode();
	editTipoSociaNombre = searchTipoSociaCodigo->getEditDesc();

#ifdef HAVE_PAGOSMODULE
if( ModuleInstance->getPagosModule() ) {
	searchFormaPagoCodigo = addSearchField( pControlsFrame, "FORMAPAGO_ID", "FORMAPAGO", "CODIGO", "NOMBRE", formapagoLayout );
	pushFormaPagoCodigo = searchFormaPagoCodigo->getButton();
	connect( pushFormaPagoCodigo, SIGNAL( clicked() ), this, SLOT( pushFormaPagoCodigo_clicked() ) );
	editFormaPagoCodigo = searchFormaPagoCodigo->getEditCode();
	editFormaPagoNombre = searchFormaPagoCodigo->getEditDesc();
}
#endif
	editFechaBaja = addEditField( pControlsFrame, "MIEMBRO", "FECHABAJA", bajaLayout );
	editMotivoBaja = addEditField( pControlsFrame, "MIEMBRO", "MOTIVOBAJA", bajaLayout );
	editImporteTotal = addEditField( pControlsFrame, "MIEMBRO", "IMPORTETOTAL", totalesLayout );
	editPagosTotal = addEditField( pControlsFrame, "MIEMBRO", "PAGOSTOTAL", totalesLayout );
	editRestoTotal = addEditField( pControlsFrame, "MIEMBRO", "RESTOTOTAL", totalesLayout );
	editCuentaBanco = addEditField( pControlsFrame, "MIEMBRO", "CUENTABANCO", cuentabancoLayout );
	editNotas = addTextField( pControlsFrame, "MIEMBRO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( proyectoLayout );
	pControlsLayout->addLayout( nsociaLayout );
	pControlsLayout->addLayout( contactoLayout );
	pControlsLayout->addLayout( contacto_idLayout );
	pControlsLayout->addLayout( tiposociaLayout );
	pControlsLayout->addLayout( formapagoLayout );
	pControlsLayout->addLayout( bajaLayout );
	pControlsLayout->addLayout( totalesLayout );
	pControlsLayout->addLayout( cuentabancoLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITMIEMBRO_INIT_CONTROLS*/
    editImporteTotal->setMustBeReadOnly( true );
    editPagosTotal->setMustBeReadOnly( true );
    editRestoTotal->setMustBeReadOnly( true );
}

void FrmEditMiembro::scatterFields()
{
/*<<<<<FRMEDITMIEMBRO_SCATTER*/
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editProyectoCodigo;
	editNumeroSocia->setText(getRecMiembro()->getValue("NUMEROSOCIA").toInt());
	editFechaAlta->setText(getRecMiembro()->getValue("FECHAALTA").toDate());
	comboSociasEstado->setCurrentItemByValue(getRecMiembro()->getValue("SOCIASESTADO").toInt());
	editFechaBaja->setText(getRecMiembro()->getValue("FECHABAJA").toDate());
	editMotivoBaja->setText(getRecMiembro()->getValue("MOTIVOBAJA").toString());
	editImporteTotal->setText(getRecMiembro()->getValue("IMPORTETOTAL").toMoney());
	editPagosTotal->setText(getRecMiembro()->getValue("PAGOSTOTAL").toMoney());
	editRestoTotal->setText(getRecMiembro()->getValue("RESTOTOTAL").toMoney());
	editCuentaBanco->setText(getRecMiembro()->getValue("CUENTABANCO").toString());
	editNotas->setText(getRecMiembro()->getValue("NOTAS").toString());
	scatterProyecto();
#ifdef HAVE_FACTUMODULE
#ifdef HAVE_FACTUMODULE
if( ModuleInstance->getFactuModule() ) {
	scatterCliente();
}
#endif
#endif
	scatterContacto();
	scatterTipoSocia();
#ifdef HAVE_PAGOSMODULE
#ifdef HAVE_PAGOSMODULE
if( ModuleInstance->getPagosModule() ) {
	scatterFormaPago();
}
#endif
#endif
/*>>>>>FRMEDITMIEMBRO_SCATTER*/
    if( editProyectoCodigo->toInt() == 0 )
        pFocusWidget = editProyectoCodigo;
    else
        pFocusWidget = editNumeroSocia;
    if( isInserting() || isDuplicating() ) {
        comboSociasEstado->setCurrentItemByValue( SociasModule::activo );
        editFechaAlta->setText( Date::currentDate() );
		editFechaBaja->setText( Xtring::null );
		editMotivoBaja->setText( Xtring::null );
        editFechaBaja->setEnabled( false );
        editMotivoBaja->setEnabled( false );
        if( isDuplicating() ) {
            editNumeroSocia->setText( Xtring::null );
        } else {
            dbRecord *r = pagos::ModuleInstance->getRecProyectoPadre( this );
            if( r ) {
                pFocusWidget = editNumeroSocia;
                editProyectoCodigo->setText( r->getValue( "CODIGO" ) );
				searchProyectoCodigo->setMustBeReadOnly(true);
				searchProyectoCodigo->setEnabled(false);
            }
        }
        if( editProyectoCodigo->toInt() != 0 ) {
            editProyectoCodigo->setJustEdited( true );
            validateFields( editProyectoCodigo, 0 );
        }
    } else {
        editFechaBaja->setEnabled( true );
        editMotivoBaja->setEnabled( true );
	}
}

void FrmEditMiembro::gatherFields()
{
    /*<<<<<FRMEDITMIEMBRO_GATHER*/
	getRecMiembro()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
	getRecMiembro()->setValue( "NUMEROSOCIA", editNumeroSocia->toInt());
	getRecMiembro()->setValue( "FECHAALTA", editFechaAlta->toDate());
	getRecMiembro()->setValue( "SOCIASESTADO", comboSociasEstado->getCurrentItemValue());
#ifdef HAVE_FACTUMODULE
if( ModuleInstance->getFactuModule() ) {
	getRecMiembro()->setValue( "CLIENTE_ID", getRecCliente()->getRecordID() );
}
#endif
	getRecMiembro()->setValue( "CONTACTO_ID", getRecContacto()->getRecordID() );
	getRecMiembro()->setValue( "TIPOSOCIA_ID", getRecTipoSocia()->getRecordID() );
#ifdef HAVE_PAGOSMODULE
if( ModuleInstance->getPagosModule() ) {
	getRecMiembro()->setValue( "FORMAPAGO_ID", getRecFormaPago()->getRecordID() );
}
#endif
	getRecMiembro()->setValue( "FECHABAJA", editFechaBaja->toDate());
	getRecMiembro()->setValue( "MOTIVOBAJA", editMotivoBaja->toString());
	getRecMiembro()->setValue( "IMPORTETOTAL", editImporteTotal->toMoney());
	getRecMiembro()->setValue( "PAGOSTOTAL", editPagosTotal->toMoney());
	getRecMiembro()->setValue( "RESTOTOTAL", editRestoTotal->toMoney());
	getRecMiembro()->setValue( "CUENTABANCO", editCuentaBanco->toString());
	getRecMiembro()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITMIEMBRO_GATHER*/
}

void FrmEditMiembro::scatterProyecto()
{
    /*<<<<<FRMEDITMIEMBRO_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoDescripcion->setText( getRecProyecto()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITMIEMBRO_SCATTER_PROYECTO*/
}

void FrmEditMiembro::pushProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITMIEMBRO_PUSH_PROYECTO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchProyectoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editProyectoCodigo->setJustEdited( false );
			editProyectoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecProyecto(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterProyecto();
				editProyectoCodigo->setJustEdited( true );
				setWiseFocus(editProyectoCodigo);
			}
			break;
		case 'M':
			{
				if( getRecProyecto()->getRecordID() ) {
					editProyectoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecProyecto(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editProyectoCodigo->setJustEdited( true );
						scatterProyecto();
					}
				setWiseFocus(editProyectoCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecProyecto()->getRecordID() != 0 ) {
					editProyectoCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecProyecto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecProyecto *tmprec = static_cast<RecProyecto *>(DBAPP->createRecord( "Proyecto" ));
				editProyectoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editProyectoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editProyectoCodigo->setJustEdited( true );
					getRecProyecto()->copyRecord( tmprec );
					scatterProyecto();
				}
				setWiseFocus(editProyectoCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITMIEMBRO_PUSH_PROYECTO_CODIGO_CLICKED*/
}

void FrmEditMiembro::scatterContacto()
{
    if( mContactoList.size() > 1 ) {
        editContactoNombre->setText( Xtring::printf( _("%d %s elegidos"),
                                     mContactoList.size(),
                                     DBAPP->getTableDescPlural( "CONTACTO", "" ).c_str() ) );
    } else {
        /*<<<<<FRMEDITMIEMBRO_SCATTER_CONTACTO*/
	editContactoCIF->setText( getRecContacto()->getValue("CIF") );
	editContactoNombre->setText( getRecContacto()->getValue("NOMBRE") );
/*>>>>>FRMEDITMIEMBRO_SCATTER_CONTACTO*/
    }
}

void FrmEditMiembro::pushContactoCIF_clicked()
{
    /*<<<<<FRMEDITMIEMBRO_PUSH_CONTACTO_CIF_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchContactoCIF->mustBeReadOnly() )
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
				setWiseFocus(editContactoCIF);
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
				setWiseFocus(editContactoCIF);
				}
			}
			break;
		case 'E':
			{
				if( getRecContacto()->getRecordID() != 0 ) {
					editContactoCIF->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecContacto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
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
				setWiseFocus(editContactoCIF);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITMIEMBRO_PUSH_CONTACTO_CIF_CLICKED*/
}

void FrmEditMiembro::scatterTipoSocia()
{
    /*<<<<<FRMEDITMIEMBRO_SCATTER_TIPOSOCIA*/
	editTipoSociaCodigo->setText( getRecTipoSocia()->getValue("CODIGO") );
	editTipoSociaNombre->setText( getRecTipoSocia()->getValue("NOMBRE") );
/*>>>>>FRMEDITMIEMBRO_SCATTER_TIPOSOCIA*/
}

void FrmEditMiembro::pushTipoSociaCodigo_clicked()
{
    /*<<<<<FRMEDITMIEMBRO_PUSH_TIPOSOCIA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchTipoSociaCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editTipoSociaCodigo->setJustEdited( false );
			editTipoSociaCodigo->setCancelling();
			if( DBAPP->choose(this, getRecTipoSocia(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterTipoSocia();
				editTipoSociaCodigo->setJustEdited( true );
				setWiseFocus(editTipoSociaCodigo);
			}
			break;
		case 'M':
			{
				if( getRecTipoSocia()->getRecordID() ) {
					editTipoSociaCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecTipoSocia(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editTipoSociaCodigo->setJustEdited( true );
						scatterTipoSocia();
					}
				setWiseFocus(editTipoSociaCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecTipoSocia()->getRecordID() != 0 ) {
					editTipoSociaCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTipoSocia(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecTipoSocia *tmprec = static_cast<RecTipoSocia *>(DBAPP->createRecord( "TipoSocia" ));
				editTipoSociaCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editTipoSociaCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editTipoSociaCodigo->setJustEdited( true );
					getRecTipoSocia()->copyRecord( tmprec );
					scatterTipoSocia();
				}
				setWiseFocus(editTipoSociaCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITMIEMBRO_PUSH_TIPOSOCIA_CODIGO_CLICKED*/
}

void FrmEditMiembro::scatterFormaPago()
{
#ifdef HAVE_PAGOSMODULE
    /*<<<<<FRMEDITMIEMBRO_SCATTER_FORMAPAGO*/
	editFormaPagoCodigo->setText( getRecFormaPago()->getValue("CODIGO") );
	editFormaPagoNombre->setText( getRecFormaPago()->getValue("NOMBRE") );
/*>>>>>FRMEDITMIEMBRO_SCATTER_FORMAPAGO*/
#endif
}

void FrmEditMiembro::pushFormaPagoCodigo_clicked()
{
#ifdef HAVE_PAGOSMODULE
    /*<<<<<FRMEDITMIEMBRO_PUSH_FORMAPAGO_CODIGO_CLICKED*/
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
				setWiseFocus(editFormaPagoCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITMIEMBRO_PUSH_FORMAPAGO_CODIGO_CLICKED*/
#endif
}

#ifdef HAVE_FACTUMODULE
void FrmEditMiembro::scatterCliente()
{
    if( mClienteList.size() > 1 ) {
        editClienteRazonSocial->setText( Xtring::printf( _("%d %s elegidos"),
                                         mClienteList.size(),
                                         DBAPP->getTableDescPlural( "CLIENTE", "" ).c_str() ) );
    } else {
        /*<<<<<FRMEDITMIEMBRO_SCATTER_CLIENTE*/
	editClienteCodigo->setText( getRecCliente()->getValue("CODIGO") );
	editClienteRazonSocial->setText( getRecCliente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITMIEMBRO_SCATTER_CLIENTE*/
		if( editClienteCodigo->isJustEdited() ) {
			if( getRecCliente()->getRecFormaPago()->getValue("CODIGO").toInt() != 0 )
				searchFormaPagoCodigo->setValue( getRecCliente()->getRecFormaPago()->getValue("CODIGO") );
		}
		if( editContactoCIF->toString().isEmpty() ) {
			getRecContacto()->copyRecord( getRecCliente()->getRecContacto() );
			scatterContacto();
        }
    }
}
#endif

void FrmEditMiembro::pushClienteCodigo_clicked()
{
#ifdef HAVE_FACTUMODULE
    /*<<<<<FRMEDITMIEMBRO_PUSH_CLIENTE_CODIGO_CLICKED*/
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
				setWiseFocus(editClienteCodigo);
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
				setWiseFocus(editClienteCodigo);
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
				factu::RecCliente *tmprec = static_cast<factu::RecCliente *>(DBAPP->createRecord( "Cliente" ));
				editClienteCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editClienteCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editClienteCodigo->setJustEdited( true );
					getRecCliente()->copyRecord( tmprec );
					scatterCliente();
				}
				setWiseFocus(editClienteCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITMIEMBRO_PUSH_CLIENTE_CODIGO_CLICKED*/
#endif
}


void FrmEditMiembro::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITMIEMBRO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
#ifdef HAVE_FACTUMODULE
if( ModuleInstance->getFactuModule() ) {
	if( sender == editClienteCodigo )
		pushClienteCodigo_clicked();
}
#endif
	if( sender == editContactoCIF )
		pushContactoCIF_clicked();
	if( sender == editTipoSociaCodigo )
		pushTipoSociaCodigo_clicked();
#ifdef HAVE_PAGOSMODULE
if( ModuleInstance->getPagosModule() ) {
	if( sender == editFormaPagoCodigo )
		pushFormaPagoCodigo_clicked();
}
#endif
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITMIEMBRO_SPECIALACTION*/
}

void FrmEditMiembro::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
/*<<<<<FRMEDITMIEMBRO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoDescripcion,
		getRecProyecto(), "CODIGO", "DESCRIPCION", Xtring::null) )
		scatterProyecto();
#ifdef HAVE_FACTUMODULE
if( ModuleInstance->getFactuModule() ) {
	if( focusWidget() != pushClienteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editClienteCodigo, editClienteRazonSocial,
		getRecCliente(), "CODIGO", "RAZONSOCIAL", Xtring::null) )
		scatterCliente();
}
#endif
	if( focusWidget() != pushContactoCIF) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editContactoCIF, editContactoNombre,
		getRecContacto(), "CIF", "NOMBRE", Xtring::null) )
		scatterContacto();
	if( focusWidget() != pushTipoSociaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTipoSociaCodigo, editTipoSociaNombre,
		getRecTipoSocia(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTipoSocia();
#ifdef HAVE_PAGOSMODULE
if( ModuleInstance->getPagosModule() ) {
	if( focusWidget() != pushFormaPagoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editFormaPagoCodigo, editFormaPagoNombre,
		getRecFormaPago(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterFormaPago();
}
#endif
/*>>>>>FRMEDITMIEMBRO_VALIDATE*/
    if( sender == editProyectoCodigo && editProyectoCodigo != 0
            && editProyectoCodigo->isJustEdited() ) {
        editNumeroSocia->setText( getNextNumeroMiembro() );
    }
    if( !validCodeAndDesc( sender, *validresult, editNumeroSocia, 0, "NUMEROSOCIA", "",
                           "PROYECTO_ID=" + pRecord->getConnection()->toSQL( getRecProyecto()->getRecordID() ) ) )
        if( !sender )
            *isvalid = false;
    // Comprobar que el contacto introducido no es ya un miembro de este proyecto
    if( !sender || (sender == editContactoCIF && editContactoCIF->isJustEdited()) ) {
        if( getRecContacto()->getRecordID() != 0 ) {
            Xtring sql = "SELECT COUNT(*) FROM MIEMBRO "
                         " WHERE CONTACTO_ID=" + getRecord()->getConnection()->toSQL( getRecContacto()->getRecordID() )
                         + " AND PROYECTO_ID=" + getRecord()->getConnection()->toSQL( getRecProyecto()->getRecordID() );
            if( getRecord()->getRecordID() != 0 )
                sql += " AND MIEMBRO.ID <> " + getRecord()->getConnection()->toSQL( getRecord()->getRecordID() );
            int existe = getRecord()->getConnection()->selectInt( sql );
            if( existe != 0 ) {
                validresult->addError( Xtring::printf( _("%1$s ya es %2$s de %3$s"),
													   DBAPP->getTableDescSingular("CONTACTO", "Esta").c_str(),
													   DBAPP->getTableDescSingular("MIEMBRO", "").c_str(),
													   DBAPP->getTableDescSingular("PROYECTO", "esta").c_str() ),
													   "CIF");
                *isvalid = false;
            }
        }
    }

    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

int FrmEditMiembro::getNextNumeroMiembro() const
{
    return getRecord()->getConnection()->selectInt(
               Xtring("SELECT MAX(NUMEROSOCIA) FROM MIEMBRO INNER JOIN PROYECTO ON PROYECTO.ID=MIEMBRO.PROYECTO_ID"
                      " WHERE PROYECTO.CODIGO=")
               + getRecord()->getConnection()->toSQL( editProyectoCodigo->toInt() ) ) + 1;
}


bool FrmEditMiembro::save()
{
    Xtring tablename;
#ifndef HAVE_FACTUMODULE
    tablename = "CONTACTO";
    List<dbRecordID> &recslist = mContactoList;
#else
    tablename = "CLIENTE";
    List<dbRecordID> &recslist = mClienteList;
#endif
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

bool FrmEditMiembro::saveAllContactos()
{
    Xtring tablename;
#ifndef HAVE_FACTUMODULE
    tablename = "CONTACTO";
    List<dbRecordID> &recslist = mContactoList;
#else
    tablename = "CLIENTE";
    List<dbRecordID> &recslist = mClienteList;
#endif
    bool ret = true;
    DBAPP->waitCursor( true );
    int cont = 0, errores = 0;
    Xtring report;
    for( List<dbRecordID>::const_iterator it = recslist.begin();
            it != recslist.end(); ++ it ) {
        getRecMiembro()->setNew( true );
        gatherFields();
        getRecMiembro()->setValue( "NUMEROSOCIA", getNextNumeroMiembro() );
        getRecMiembro()->setValue( "CONTACTO_ID", *it );
#ifdef HAVE_FACTUMODULE
        if( ModuleInstance->getFactuModule() )
            getRecMiembro()->setValue( "CLIENTE_ID", *it );
#endif
        getRecMiembro()->readRelated( true ); // force
        try {
            if( !FrmEditRecMaster::save() ) {
                ret = false;
                errores ++;
            } else {
                report += _("Añadido: ") + getRecMiembro()->toString( TOSTRING_CODE_AND_DESC ) + "\n";
                cont++;
            }
        } catch( dbError &e ) {
            report += _("Error: ") + getRecMiembro()->toString( TOSTRING_CODE_AND_DESC ) + "\n    "
                      + e.what() + "\n";
            errores++;
        }
    }
    report += Xtring::printf( _("\nSe han añadido %d %s. Ha habido %d errores"),
                              cont, DBAPP->getTableDescPlural( tablename, "<" ).c_str(),
                              errores );
    DBAPP->resetCursor();
    msgOkLarge(this, "", report );
    return ret;
}

/*<<<<<FRMEDITMIEMBRO_FIN*/
} // namespace socias
} // namespace gong
/*>>>>>FRMEDITMIEMBRO_FIN*/

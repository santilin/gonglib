/*<<<<<MODULE_INFO*/
// COPYLEFT Módulo para la gestión de los editorial
// RECORD Libro FrmEditRecMaster
// RECORD Editorial FrmEditRecMaster Empresa
// MODULE_REQUIRED Empresa
// MODULE_REQUIRED Factu
// MODULE_OPTIONAL Contab
// TYPE GongModule editorial::EditorialModule editorial
/*>>>>>MODULE_INFO*/

/**
 Los editorial se pueden editar como artículos o como editorial. Para eso, el formulario FrmEditLibro
 tiene un constructor con un parámetro que indica si es libro.
 En el createEditForm, si se llama desde el menú de editorial, esLibro = true.
 */

/*<<<<<EDITORIALMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "editorialmodule.h"
#include "editorialfrmeditlibro.h"
#include "editorialfrmediteditorial.h"
/*>>>>>EDITORIALMODULE_INCLUDES*/
#include <contactosmastertable.h>
#include <contactosfldnombrepropio.h>
#include <facturectipodoc.h>
#include "editorialmastertable.h"
#include "editorialfrmeditfamiliabehavior.h"

namespace gong {
namespace editorial {

EditorialModule *ModuleInstance = 0;

EditorialModule::EditorialModule()
        : dbModule( "editorial" )
{
    ModuleInstance = this;
    _GONG_DEBUG_TRACE( 1 );
    pEmpresaModule = static_cast<empresa::EmpresaModule *>( DBAPP->findModule( "empresa" ) );
    _GONG_DEBUG_ASSERT( pEmpresaModule );
/*<<<<<EDITORIALMODULE_PUBLIC_INFO*/
	mModuleRequires << "empresa" << "factu";
	mMasterTables << "LIBRO" << "EDITORIAL";
//	mDetailTables
	pEmpresaModule = static_cast< empresa::EmpresaModule * >(DBAPP->findModule( "Empresa" ));
	pFactuModule = static_cast< factu::FactuModule * >(DBAPP->findModule( "Factu" ));
#ifdef HAVE_CONTABMODULE
	pContabModule = static_cast< contab::ContabModule * >(DBAPP->findModule( "Contab" ));
#endif
/*>>>>>EDITORIALMODULE_PUBLIC_INFO*/
	mMasterTables.remove("LIBRO");
}


EditorialModule::~EditorialModule()
{
    _GONG_DEBUG_TRACE( 1 );
}


bool EditorialModule::login( FrmLogin *frmlogin, const Xtring &plug_version,
							 Xtring &addTitle, bool startingapp )
{
    return true;
}


bool EditorialModule::initDatabase( dbDefinition *db )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    factu::MasterTable *fmt = new factu::MasterTable( db->findTableDefinition( "FAMILIA" ) );
    fmt->addFieldBool( "LIBROS" )->setDefaultValue( "0" );
    delete fmt;

    pFicEditorial = new MasterTable( *pMainDatabase, "EDITORIAL" );
    pFicEditorial->addFieldRecordID();
    pFicEditorial->addFieldEmpresaID();
    pFicEditorial->addFieldIntCode( "CODIGO" );
    pFicEditorial->addField<contactos::FldNombrePropio>( "RAZONSOCIAL" );
    pFicEditorial->addFieldOne2OneRelation( "CONTACTO_ID", "CONTACTO.ID", true );
    pFicEditorial->addFieldNotas();
    pFicEditorial->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicEditorial->getTableDefinition() );

    _GONG_DEBUG_ASSERT( pMainDatabase->findTableDefinition( "ARTICULO" ) ); // Incluir el módulo gong-factu
    pFicLibro = new MasterTable( pMainDatabase->findTableDefinition( "ARTICULO" ) );
    pFicLibro->addFieldIsbn( "ISBN13" )->setUnique( true );
    pFicLibro->addFieldIsbn( "ISBN10" )->setUnique( true );
    pFicLibro->addFieldDepositoLegal()->setUnique( true );
    pFicLibro->addFieldString( "TITULO", 250 );
    pFicLibro->addFieldText( "SUBTITULO" );
    pFicLibro->addFieldText( "AUTORAS" );
    pFicLibro->addFieldString( "LENGUA", 100 );
    pFicLibro->addFieldInt( "ANO" );
    pFicLibro->addFieldString( "COLECCION", 100 );
    pFicLibro->addFieldString( "CDU", 20 );
    pFicLibro->addFieldInt( "PAGINAS" );
    pFicLibro->addFieldString( "EDICION", 30 );
    pFicLibro->addFieldString( "FECHAEDICION", 10 );
    pFicLibro->addFieldString( "FECHAIMPRESION", 10 );
    pFicLibro->addFieldString( "IMPRESOR", 100 );
    pFicLibro->addFieldString( "LUGARIMPRESION", 100 );
    pFicLibro->addFieldString( "ENCUADERNACION", 100 );
    pFicLibro->addFieldString( "LENGUASTRADUCCION", 100 );
    pFicLibro->addFieldString( "TITULOORIGINAL", 100 );
    pFicLibro->addFieldText( "CREDITOS" );
    pFicLibro->addFieldOne2OneRelation( "EDITORIAL_ID", "EDITORIAL.ID", true );

	return true;
}


dbRecord *EditorialModule::createRecord( const Xtring &tablename, dbRecordID recid, dbUser *user )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	if( tablename.upper() == "ARTICULO" )
		return new RecLibro(getConnection(), recid, user);
/*<<<<<EDITORIALMODULE_CREATE_RECORD*/
	if( tablename.upper() == "LIBRO" )
		return new RecLibro(getConnection(), recid, user);
	if( tablename.upper() == "EDITORIAL" )
		return new RecEditorial(getConnection(), recid, user);
/*>>>>>EDITORIALMODULE_CREATE_RECORD*/
    return 0;
}

FrmEditRec *EditorialModule::createEditForm( FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
	if( tablename.upper() == "ARTICULO" )
		return new FrmEditLibro(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*<<<<<EDITORIALMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "LIBRO" )
		return new FrmEditLibro(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "EDITORIAL" )
		return new FrmEditEditorial(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>EDITORIALMODULE_CREATE_EDITFORM*/
    return 0;
}


void EditorialModule::afterCreateEditForm(FrmEditRec* frm, dbRecord* rec)
{
    Xtring tablename = rec->getTableName();
    if ( tablename.upper() == "FAMILIA" )
		frm->addBehavior( new FrmEditFamiliaBehavior( frm ) );
}

FrmEditRecDetail *EditorialModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
/*<<<<<EDITORIALMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>EDITORIALMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

/*<<<<<EDITORIALMODULE_SLOT_EMPRESAEDITORIAL*/
void EditorialModule::slotMenuEmpresaEditorial()
{
	pMainWindow->slotMenuEditRecMaestro( "EDITORIAL" );
}
/*>>>>>EDITORIALMODULE_SLOT_EMPRESAEDITORIAL*/


bool EditorialModule::initMainWindow( MainWindow *mainwin )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( mainwin );
    pMainWindow = mainwin;

    QMenu *pMenuEmpresa = empresa::ModuleInstance->getMenuEmpresa();
	_GONG_DEBUG_ASSERT( pMenuEmpresa );

    /*<<<<<EDITORIALMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("EDITORIAL")->getDescPlural();
		pMenuEmpresaEditorial = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuEmpresaEditorial->setObjectName( "MenuEmpresaEditorial" );
		pMenuEmpresaEditorial->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuEmpresaEditorial->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuEmpresaEditorial, SIGNAL(activated()), this, SLOT(slotMenuEmpresaEditorial()));
		pMenuEmpresaEditorial->addTo(pMenuEmpresa);
	}
/*>>>>>EDITORIALMODULE_INITMAINWINDOW_MENUS*/
    return true;
}

/*<<<<<EDITORIALMODULE_FIN*/
} // namespace editorial
} // namespace gong

/*>>>>>EDITORIALMODULE_FIN*/
/*<<<<<EDITORIALMODULE_INIT_DATABASE*/

/*>>>>>EDITORIALMODULE_INIT_DATABASE*/

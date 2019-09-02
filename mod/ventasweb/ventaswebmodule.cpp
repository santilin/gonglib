/*<<<<<MODULE_INFO*/
// COPYLEFT Module ventasweb
// RECORD PedidoWeb FrmEditRecMaster VentasWeb
// RECORD PedidoWebDet FrmEditRecDetail
// RECORD SemanaPedido FrmEditRecMaster VentasWeb
// MODULE_REQUIRED Empresa
// MODULE_REQUIRED Factu
// TYPE GongModule ventasweb::VentasWebModule
/*>>>>>MODULE_INFO*/

/*<<<<<VENTASWEBMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ventaswebmodule.h"
#include "ventaswebfrmeditpedidoweb.h"
#include "ventaswebfrmeditpedidowebdet.h"
#include "ventaswebfrmeditsemanapedido.h"
/*>>>>>VENTASWEBMODULE_INCLUDES*/
#include <gongdbfieldemail.h>
#include <dbappnameslisttable.h>
#ifdef HAVE_SOCIASMODULE
#include <sociasrecmiembro.h>
#else
#include <factureccliente.h>
#endif
#include "ventaswebfrmsubirbajar.h"
#include "ventaswebfrmeditarticulobehavior.h"
#include "ventaswebfrmeditfamiliabehavior.h"
#include "ventaswebfrmcambiosmasivosarticulos.h"

namespace gong {
namespace ventasweb {

VentasWebModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::String,
        "FTP_HOST",
        _("Host del servidor FTP"),
        ""
    },
    {
        dbModuleSetting::String,
        "FTP_WEBDIR",
        _("Directorio del servidor FTP"),
        ""
    },
    {
        dbModuleSetting::String,
        "REMOTE_DBNAME",
        _("Nombre de la base de datos remota"),
        ""
    },
    {
        dbModuleSetting::String,
        "REMOTE_USER",
        _("Usuaria de la base de datos remota"),
        ""
    },
    {
        dbModuleSetting::Password,
        "REMOTE_PASSWORD",
        _("Contraseña de la usuaria de la base de datos remota"),
        ""
    },
    {
        dbModuleSetting::String,
        "REMOTE_HOST",
        _("Servidor de la base de datos remota"),
        ""
    },
    {dbModuleSetting::None}
};


VentasWebModule::VentasWebModule(int argc, char *argv[])
    : dbModule("ventasweb", argc, argv)
{
    _GONG_DEBUG_TRACE(1);
    ModuleInstance = this;
    pModuleSettings = _settings;
    /*<<<<<VENTASWEBMODULE_PUBLIC_INFO*/
	mModuleRequires << "empresa" << "factu";
	mMasterTables << "PEDIDOWEB" << "SEMANAPEDIDO";
	mDetailTables << "PEDIDOWEBDET";
	pEmpresaModule = static_cast< empresa::EmpresaModule * >(DBAPP->findModule( "Empresa" ));
	pFactuModule = static_cast< factu::FactuModule * >(DBAPP->findModule( "Factu" ));
/*>>>>>VENTASWEBMODULE_PUBLIC_INFO*/
    factu::ModuleInstance->addInsertable("PEDIDOWEB");
}


VentasWebModule::~VentasWebModule()
{
    _GONG_DEBUG_TRACE(1);
}

bool VentasWebModule::login(const Xtring& version,
                            Xtring& addTitle, bool startingapp)
{
    return true;
}

bool VentasWebModule::initDatabase(dbDefinition *db)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    MasterTable *fmt = new MasterTable( db->findTableDefinition( "ARTICULO" ) );
    fmt->addFieldBool( "PUBLICARWEB" )->setDefaultValue("0");
    fmt->addFieldEuro( "PVPWEB" );
    fmt->addFieldInt ( "ORDENWEB" );
    delete fmt;

    fmt = new MasterTable( db->findTableDefinition( "FAMILIA" ) );
    fmt->addFieldBool( "PUBLICARWEB" )->setDefaultValue("0");
    fmt->addFieldText( "DESCRIPCIONWEB" );
    delete fmt;

    fmt = new MasterTable( db->findTableDefinition( "PROVEEDORA" ) );
    fmt->addFieldBool( "PUBLICARWEB" )->setDefaultValue("0");
    fmt->addFieldText( "DESCRIPCIONWEB" );
    delete fmt;

    pFicPedidoWeb = new MasterTable( *pMainDatabase, "PEDIDOWEB" );
    pFicPedidoWeb->addFieldRecordID();
    pFicPedidoWeb->addFieldEmpresaID();
    pFicPedidoWeb->addFieldIntCode( "CONTADOR" );
    pFicPedidoWeb->addFieldFecha();
    pFicPedidoWeb->addField<FldNamesListTable>( "ESTADOPEDIDO" )->setInsertAllowed(false);
    pFicPedidoWeb->addFieldDesc("NOMBRE", 120)->setCanBeNull( false )->setUnique(false);
    pFicPedidoWeb->addFieldDireccion();
    pFicPedidoWeb->addFieldCodigoPostal("CP");
    pFicPedidoWeb->addFieldLocalidad();
    pFicPedidoWeb->addFieldProvincia();
    pFicPedidoWeb->addFieldTelefono("TELEFONO");
    pFicPedidoWeb->addField<dbFieldEmail>("EMAIL");
    pFicPedidoWeb->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicPedidoWeb->addFieldEuro( "TOTAL" );
    pFicPedidoWeb->addFieldString("REFERENCIA", 40 );
    pFicPedidoWeb->addFieldNotas();
//     pFicPedidoWeb->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicPedidoWeb->addRelationDefinition( dbRelationDefinition::one2many, "PEDIDOWEB", "ID", "PEDIDOWEBDET", "PEDIDOWEB_ID" );
    pFicPedidoWeb->addMultipleIndex( "pedidoweb_contador_unico", "EMPRESA_ID,CONTADOR", true );
    pMainDatabase->addTable( pFicPedidoWeb->getTableDefinition() );

    pFicPedidoWebDet = new MasterTable( *pMainDatabase, "PEDIDOWEBDET" );
    pFicPedidoWebDet->addFieldRecordID();
    pFicPedidoWebDet->addFieldReferenceID( "PEDIDOWEB_ID", "PEDIDOWEB.ID" );
    pFicPedidoWebDet->addFieldFloat( "CANTIDAD" );
    pFicPedidoWebDet->addFieldOne2OneRelation( "ARTICULO_ID", "ARTICULO.ID", true );
    pFicPedidoWebDet->addFieldEuro( "PVP" );
    pFicPedidoWebDet->addFieldEuro( "IMPORTECONIVA" );
    pMainDatabase->addTable( pFicPedidoWebDet->getTableDefinition() );

    pFicSemanaPedido = new MasterTable( *pMainDatabase, "SEMANAPEDIDO" );
    pFicSemanaPedido->addFieldRecordID();
    pFicSemanaPedido->addFieldInt( "NUMEROSEMANA" );
    pFicSemanaPedido->addFieldDateTime( "FECHAINICIOPEDIDO" );
    pFicSemanaPedido->addFieldDateTime( "FECHAFINPEDIDO" );
    pFicSemanaPedido->addFieldDate( "FECHARECOGIDA" );
    pFicSemanaPedido->addFieldText( "NOTAS" );
    pMainDatabase->addTable( pFicSemanaPedido->getTableDefinition() );

    return true;
}


Xtring VentasWebModule::getMigrationSQL( uint old_version ) const
{
    Xtring ret;
    if( old_version == 1 ) {
    }
    return ret;
}

dbRecord *VentasWebModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    /*<<<<<VENTASWEBMODULE_CREATE_RECORD*/
	if( tablename.upper() == "PEDIDOWEB" )
		return new RecPedidoWeb(getConnection(), recid, user);
	if( tablename.upper() == "PEDIDOWEBDET" )
		return new RecPedidoWebDet(getConnection(), recid, user);
	if( tablename.upper() == "SEMANAPEDIDO" )
		return new RecSemanaPedido(getConnection(), recid, user);
/*>>>>>VENTASWEBMODULE_CREATE_RECORD*/
    return 0;
}

FrmEditRec *VentasWebModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<VENTASWEBMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "PEDIDOWEB" )
		return new FrmEditPedidoWeb(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "SEMANAPEDIDO" )
		return new FrmEditSemanaPedido(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>VENTASWEBMODULE_CREATE_EDITFORM*/
    return 0;
}


void VentasWebModule::afterCreateEditForm( FrmEditRec *frm, dbRecord *rec )
{
    if( rec->getTableName() == "ARTICULO" ) {
        frm->addBehavior( new FrmEditArticuloBehavior( frm ) );
    }
    if( rec->getTableName() == "FAMILIA" ) {
        frm->addBehavior( new FrmEditFamiliaBehavior( frm ) );
    }
}

FrmEditRecDetail *VentasWebModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<VENTASWEBMODULE_CREATE_EDITFORM_DETAIL*/
	if( tablename.upper() == "PEDIDOWEBDET" )
		return new FrmEditPedidoWebDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
/*>>>>>VENTASWEBMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

/*<<<<<VENTASWEBMODULE_SLOT_VENTASWEBPEDIDOWEB*/
void VentasWebModule::slotMenuVentasWebPedidoWeb()
{
	pMainWindow->slotMenuEditRecMaestro( "PEDIDOWEB" );
}
/*>>>>>VENTASWEBMODULE_SLOT_VENTASWEBPEDIDOWEB*/
/*<<<<<VENTASWEBMODULE_SLOT_VENTASWEBSEMANAPEDIDO*/
void VentasWebModule::slotMenuVentasWebSemanaPedido()
{
	pMainWindow->slotMenuEditRecMaestro( "SEMANAPEDIDO" );
}
/*>>>>>VENTASWEBMODULE_SLOT_VENTASWEBSEMANAPEDIDO*/

void VentasWebModule::slotMenuVentasWebBajar()
{
    if( DBAPP->preventOperationIf( DBAPP->isReadOnly(),
                                   "Bajar ventas", "la base de datos está bloqueada" ) )
        return;
    FrmSubirBajar *fsb = new FrmSubirBajar( false );
    fsb->show();
    DBAPP->createClient ( fsb );
}

void VentasWebModule::slotMenuVentasWebSubir()
{
    FrmSubirBajar *fsb = new FrmSubirBajar( true );
    fsb->show();
    DBAPP->createClient ( fsb );
}

void VentasWebModule::slotMenuVentasWebCambiosMasivosArticulos()
{
    if( DBAPP->preventOperationIf( DBAPP->isReadOnly(),
                                   "Cambios masivos en artículos", "la base de datos está bloqueada" ) )
        return;
    FrmCambiosMasivosArticulos *fcma = new FrmCambiosMasivosArticulos();
    fcma->show();
    DBAPP->createClient ( fcma );
}


bool VentasWebModule::initMainWindow(MainWindow *mainwin)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT(mainwin);
    pMainWindow = mainwin;

    pMenuVentasWeb = new QMenu( pMainWindow );
    pMenuVentasWeb->setObjectName( "VentasWeb" );
    pMainWindow->menuBar()->insertItem( toGUI( _("&VentasWeb")), pMenuVentasWeb );

    /*<<<<<VENTASWEBMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("PEDIDOWEB")->getDescPlural();
		pMenuVentasWebPedidoWeb = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuVentasWebPedidoWeb->setObjectName( "MenuVentasWebPedidoWeb" );
		pMenuVentasWebPedidoWeb->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuVentasWebPedidoWeb->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuVentasWebPedidoWeb, SIGNAL(activated()), this, SLOT(slotMenuVentasWebPedidoWeb()));
		pMenuVentasWebPedidoWeb->addTo(pMenuVentasWeb);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("SEMANAPEDIDO")->getDescPlural();
		pMenuVentasWebSemanaPedido = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuVentasWebSemanaPedido->setObjectName( "MenuVentasWebSemanaPedido" );
		pMenuVentasWebSemanaPedido->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuVentasWebSemanaPedido->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuVentasWebSemanaPedido, SIGNAL(activated()), this, SLOT(slotMenuVentasWebSemanaPedido()));
		pMenuVentasWebSemanaPedido->addTo(pMenuVentasWeb);
	}
/*>>>>>VENTASWEBMODULE_INITMAINWINDOW_MENUS*/
    pMenuVentasWebSubir = new QAction( toGUI("Subir datos a la web..."), 0, pMainWindow,0);
    pMainWindow->connect(pMenuVentasWebSubir, SIGNAL(activated()), this, SLOT(slotMenuVentasWebSubir()));
    pMenuVentasWebSubir->addTo(pMenuVentasWeb);
    pMenuVentasWebBajar = new QAction( toGUI("Bajar datos de la web..."), 0, pMainWindow,0);
    pMainWindow->connect(pMenuVentasWebBajar, SIGNAL(activated()), this, SLOT(slotMenuVentasWebBajar()));
    pMenuVentasWebBajar->addTo(pMenuVentasWeb);
    pMenuVentasWebCreateJoomlaUser = new QAction( toGUI("Crear usuarias Joomla!..."), 0, pMainWindow,0);
    pMainWindow->connect(pMenuVentasWebCreateJoomlaUser, SIGNAL(activated()), this, SLOT(slotMenuVentasWebCreateJoomlaUser()));
    pMenuVentasWebCreateJoomlaUser->addTo(pMenuVentasWeb);

    pMainWindow->disconnect( pMainWindow->findAction("ActionFactuCambiosMasivosArticulos"), SIGNAL( activated() ), 0, 0 );
    pMainWindow->connect( pMainWindow->findAction("ActionFactuCambiosMasivosArticulos"),
                          SIGNAL( activated() ),  this,
                          SLOT( slotMenuVentasWebCambiosMasivosArticulos() ) );

    return true;
}

int VentasWebModule::getMaxContador(const Xtring& table, dbRecordID empresa_id) const
{
    Xtring sql = "SELECT MAX(CONTADOR) FROM " + table;
    if( empresa_id != 0 )
        sql += " WHERE EMPRESA_ID=" + getConnection()->toSQL( empresa_id );
    int numero = getConnection()->selectInt(sql) + 1;
    return numero;
}


void VentasWebModule::slotMenuVentasWebCreateJoomlaUser()
{
#if 0
    Xtring server = "localhost";
    Xtring filename = "/webs/lospajaros.org/web/register_user.php";
#ifdef HAVE_SOCIASMODULE
    socias::RecMiembro *miembro = static_cast<socias::RecMiembro *>(DBAPP->createRecord( "MIEMBRO" ));
    List<dbRecordID> miembros;
    miembro->addFilter( "PROYECTO_ID=1 AND MIEMBRO.SOCIASESTADO=1" );
    if( DBAPP->chooseMulti( miembros, (FrmEditRec*)0, miembro, 0, dbApplication::editNone ) ) {
        for( List<dbRecordID>::const_iterator it = miembros.begin();
                it != miembros.end(); ++ it ) {
            if( miembro->read( *it ) ) {
                if( createJoomlaUser( miembro, server, filename ) ) {
                    DBAPP->showStickyOSD( "Creando usuarios de Joomla!", miembro->toString( TOSTRING_CODE_AND_DESC) );
                }
            }
        }
    }
    delete miembro;
#else
    factu::RecCliente *cliente = static_cast<factu::RecCliente *>(DBAPP->createRecord( "CLIENTE" ));
    List<dbRecordID> clientes;
    cliente->addFilter( "CLIENTE.ACTIVO=1" );
    if( DBAPP->chooseMulti( clientes, (FrmEditRec*)0, cliente, 0, dbApplication::editNone ) ) {
        for( List<dbRecordID>::const_iterator it = clientes.begin();
                it != clientes.end(); ++ it ) {
            if( cliente->read( *it ) ) {
                if( createJoomlaUser( cliente, server, filename ) ) {
                    DBAPP->showStickyOSD( "Creando usuarios de Joomla!", cliente->toString( TOSTRING_CODE_AND_DESC) );
                }
            }
        }
    }
    delete cliente;
#endif
#endif
}

#if 0
#ifdef HAVE_SOCIASMODULE
int VentasWebModule::createJoomlaUser(socias::RecMiembro* miembro,
                                      const Xtring &server, const Xtring &filename ) const
{
    Xtring post = "name=" + uriEncode(miembro->getRecContacto()->getValue("NOMBRE").toString())
                  + "&username=" + uriEncode(miembro->getValue("NUMEROSOCIA").toString())
                  + "&password=" + uriEncode(miembro->getRecContacto()->getValue("CIF").toString())
                  + "&email=" + uriEncode(miembro->getRecContacto()->getValue("EMAIL").toString());
    _GONG_DEBUG_PRINT(0, post );
    char *result = 0;
    XtringList response;
    bool ret = read_http_file( response, &result, server, filename, Xtring::null,
                               "POST", post );
    if( ret ) {
        _GONG_DEBUG_PRINT(0, result );
    }
    return ret;
}
#else
int VentasWebModule::createJoomlaUser(factu::RecCliente* cliente,
                                      const Xtring &server, const Xtring &filename ) const
{
    Xtring post = "name=" + uriEncode(cliente->getRecContacto()->getValue("NOMBRE").toString())
                  + "&username=" + uriEncode(cliente->getValue("NUMEROSOCIA").toString())
                  + "&password=" + uriEncode(cliente->getRecContacto()->getValue("CIF").toString())
                  + "&email=" + uriEncode(cliente->getRecContacto()->getValue("EMAIL").toString());
    _GONG_DEBUG_PRINT(0, post );
    char *result = 0;
    XtringList response;
    bool ret = read_http_file( response, &result, server, filename, Xtring::null,
                               "POST", post );
    if( ret ) {
        _GONG_DEBUG_PRINT(0, result );
    }
    return ret;
}
#endif
#endif

/*<<<<<VENTASWEBMODULE_FIN*/
} // namespace ventasweb
} // namespace gong

/*>>>>>VENTASWEBMODULE_FIN*/
/*<<<<<VENTASWEBMODULE_INIT_DATABASE*/

/*>>>>>VENTASWEBMODULE_INIT_DATABASE*/

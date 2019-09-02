/*<<<<<COPYLEFT*/
/** @file ofertasmodule.cpp Module ofertas
 * GongLib project. (L) Santiago Capel Torres, 2003-2011
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Module ofertas
// RECORD OfertaCompra FrmEditRecMaster Ofertas
// RECORD OfertaCompraDet FrmEditRecDetail
// RECORD OfertaVenta FrmEditRecMaster Ofertas
// RECORD OfertaVentaDet FrmEditRecDetail
// RECORD PagoOferta FrmEditRecMaster Ofertas
// RECORD CobroOferta FrmEditRecMaster Ofertas
// RECORD ApunteOferta FrmEditRecMaster Ofertas
// RECORD CuentaOferta FrmEditRecMaster Ofertas
// MODULE_REQUIRED Empresa
// MODULE_REQUIRED Factu
// MODULE_REQUIRED Pagos
// MODULE_OPTIONAL Tesoreria
// TYPE GongModule ofertas::OfertasModule
/*>>>>>MODULE_INFO*/

#include <gongfileutils.h>
/*<<<<<OFERTASMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ofertasmodule.h"
#include "ofertasfrmeditofertacompra.h"
#include "ofertasfrmeditofertacompradet.h"
#include "ofertasfrmeditofertaventa.h"
#include "ofertasfrmeditofertaventadet.h"
#include "ofertasfrmeditpagooferta.h"
#include "ofertasfrmeditcobrooferta.h"
#include "ofertasfrmeditapunteoferta.h"
#include "ofertasfrmeditcuentaoferta.h"
/*>>>>>OFERTASMODULE_INCLUDES*/
#include <dbappnameslisttable.h>
#include <dbappfrmlogin.h>
#include <dbappfrmcustom.h>
#include <factufldivadetallado.h>
#include <facturecalbarancompra.h>
#include <facturecalbaranventa.h>
#include <facturecfacturacompra.h>
#include <facturecfacturaventa.h>
#include "ofertasdbdoubleconnection.h"
#include "ofertasbotonpasaraofertabehavior.h"

namespace gong {
namespace ofertas {

OfertasModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::String,
        "DBUSER",
        _("Usuaria de la base de datos de ofertas"),
        "usuaria",
        dbModuleSetting::Local
    },
    {
        dbModuleSetting::String,
        "DBPASSWORD",
        _("Contraseña de la base de datos de ofertas"),
        "",
        dbModuleSetting::Local
    },
    {
        dbModuleSetting::String,
        "DBHOST",
        _("Servidor de la base de datos de ofertas"),
        "",
        dbModuleSetting::Local
    },
    {
        dbModuleSetting::Int,
        "DBPORT",
        _("Puerto del servidor de la base de datos de ofertas"),
        "",
        dbModuleSetting::Local
    },
    {
        dbModuleSetting::String,
        "DBOPTIONS",
        _("Opciones para la conexión al servidor de la base de datos de ofertas"),
        "",
        dbModuleSetting::Local
    },
    {dbModuleSetting::None}
};


OfertasModule::OfertasModule(int argc, char *argv[])
    : dbModule("ofertas", argc, argv), mActivo(false)
{
    ModuleInstance = this;
    pModuleSettings = _settings;
    mDescription = "Módulo de ofertas";
    _GONG_DEBUG_TRACE(1);
    /*<<<<<OFERTASMODULE_PUBLIC_INFO*/
	mModuleRequires << "empresa" << "factu" << "pagos";
	mMasterTables << "OFERTACOMPRA" << "OFERTAVENTA" << "PAGOOFERTA" << "COBROOFERTA" << "APUNTEOFERTA" << "CUENTAOFERTA";
	mDetailTables << "OFERTACOMPRADET" << "OFERTAVENTADET";
	pEmpresaModule = static_cast< empresa::EmpresaModule * >(DBAPP->findModule( "Empresa" ));
	pFactuModule = static_cast< factu::FactuModule * >(DBAPP->findModule( "Factu" ));
	pPagosModule = static_cast< pagos::PagosModule * >(DBAPP->findModule( "Pagos" ));
#ifdef HAVE_TESORERIAMODULE
	pTesoreriaModule = static_cast< tesoreria::TesoreriaModule * >(DBAPP->findModule( "Tesoreria" ));
#endif
/*>>>>>OFERTASMODULE_PUBLIC_INFO*/
    pFactuModule->addInsertable( "OFERTACOMPRA" );
    pFactuModule->addInsertable( "OFERTAVENTA" );
    mDBName = "ofertasdb";
    pReplicaDatabase = new dbDefinition( mDBName, "Base de datos de ofertas de GestiONG" );
}

OfertasModule::~OfertasModule()
{
    _GONG_DEBUG_TRACE(1);
}

void OfertasModule::afterCreateEditForm(FrmEditRec* frm, dbRecord* rec)
{
    Xtring tablename = rec->getTableName();
    if ( tablename.upper() == "ALBARANCOMPRA" )
        frm->addBehavior( new BotonPasarAOfertaBehavior( frm ) );
    if ( tablename.upper() == "ALBARANVENTA" )
        frm->addBehavior( new BotonPasarAOfertaBehavior( frm ) );
}

int OfertasModule::getMaxContador() const
{
    int max = 0;
    if( empresa::ModuleInstance->getModuleSetting( "USAR_MAXCONTADOR" ).toBool() == false )
        return max;
    static XtringList contador_tables = XtringList() << "OFERTACOMPRA" << "OFERTAVENTA" << "PAGOOFERTA" << "COBROOFERTA";
    if( contador_tables.size() ) {
        for( XtringList::const_iterator it = contador_tables.begin();
                it != contador_tables.end(); ++it ) {
            try {
                dbRecord *rec = DBAPP->createRecord( *it );
                int tblmax = rec->selectNextInt("CONTADOR", Xtring::null, false); // don't use gaps
                delete rec;
                if( tblmax > max )
                    max = tblmax;
                _GONG_DEBUG_PRINT(4, "Max contador: " + Xtring::number( tblmax ) );
            } catch( dbError &e ) {
                _GONG_DEBUG_WARNING( e.what() );
            }
        }
    }
    return max;
}

bool OfertasModule::login(const Xtring& version, Xtring& addTitle, bool startingapp)
{
	FrmLogin *frmlogin = DBAPP->getFrmLogin();
    pDoubleConnection = new dbDoubleConnection( getConnection() );
    enum dbConnection::SqlDriver conntype;
// 	enum dbConnection::SqlDriver conntype = dbConnection::DRIVER_MYSQL;
    bool forcecreatetables = false;
    Xtring ruta;
    FileUtils::execProcess( "find /media -maxdepth 3 -name \"ofertas.txt\"", ruta );
    if( !ruta.isEmpty() ) {
        conntype = dbConnection::DRIVER_SQLITE3;
        if( !pDoubleConnection->connect( conntype, Xtring::null, Xtring::null, mDBName,
                                         FileUtils::path( ruta.c_str() ), 0,
                                         ModuleInstance->getModuleSetting( "DBOPTIONS" ).toString() ) ) {
            forcecreatetables = true;
            if( !pDoubleConnection->connect( conntype, Xtring::null, Xtring::null, mDBName,
                                             FileUtils::path( ruta.c_str() ), 0,
                                             "CREATEDATABASE;" + ModuleInstance->getModuleSetting( "DBOPTIONS" ).toString() ) ) {
                FrmBase::msgError( frmlogin, Xtring::printf(_("No se ha podido abrir la base de datos del módulo de ofertas (sql3) en\n%s"),
					ruta.c_str()));
                _GONG_DEBUG_WARNING( pDoubleConnection->getLastError().getWholeError() );
                return false;
            }
        }
    } else {
        FileUtils::execProcess( "find /media -name \"ofertas.sql\"", ruta );
        if( !ruta.isEmpty() ) {
            conntype = dbConnection::DRIVER_MYSQL;
            Xtring user = ModuleInstance->getModuleSetting( "DBUSER" ).toString();
            Xtring password = ModuleInstance->getModuleSetting( "DBPASSWORD" ).toString();
            Xtring host = ModuleInstance->getModuleSetting( "DBHOST" ).toString();
            if( !pDoubleConnection->connect( conntype, user, password, mDBName, host,
                                             ModuleInstance->getModuleSetting( "DBPORT" ).toInt(),
                                             ModuleInstance->getModuleSetting( "DBOPTIONS" ).toString() ) ) {
                FrmCustom *frmuserandpass = new FrmCustom( 0, "FrmUserAndPass" );
                frmuserandpass->setTitle( _("Crear base de datos de ofertas") );
                LineEdit *le_user = frmuserandpass->addInput( 0, _("Usuaria para crear la base de datos"), Xtring() );
                LineEdit *le_password = frmuserandpass->addInput( 0, _("Contraseña"), Xtring(), "PASSWORD" );
                frmuserandpass->showModalFor( frmlogin, true, true );
                Xtring crea_user = le_user->toString();
                Xtring crea_passwd = le_password->toString();
                bool cancelled = frmuserandpass->wasCancelled();
                delete frmuserandpass;
                if( cancelled )
                    return false;
                if( !pDoubleConnection->connect( conntype, crea_user, crea_passwd, "",
                                                 ModuleInstance->getModuleSetting( "DBHOST" ).toString(),
                                                 ModuleInstance->getModuleSetting( "DBPORT" ).toInt(),
                                                 ModuleInstance->getModuleSetting( "DBOPTIONS" ).toString() ) ) {
                    FrmBase::msgError( frmlogin, _("No se ha podido conectar al servidor para crear la base de datos del módulo de ofertas") );
                    _GONG_DEBUG_WARNING( pDoubleConnection->getLastError().getWholeError() );
                    return false;
                }
                pReplicaDatabase->setName( mDBName );
                try {
                    frmlogin->addMessage( Xtring::printf(_("Creando la base de datos %s ..."), mDBName.c_str()) );
                    pReplicaDatabase->create( pDoubleConnection, "DEFAULT COLLATE utf8_spanish_ci", true );
                    frmlogin->addMessage ( _("Creando usuario/a...") );
                    pDoubleConnection->createUser( mDBName, user, password, /*host*/Xtring::null, Xtring::null );
                    forcecreatetables = true;
                    pDoubleConnection->selectDatabase( mDBName );
                } catch( dbError &e ) {
                    _GONG_DEBUG_WARNING( e.what() );
                    return false;
                }
            }
        } else {
            return false;
        }
    }
    if( forcecreatetables == true ) {
        frmlogin->addMessage( _("Creando las tablas...") );
        pReplicaDatabase->createTables( pDoubleConnection, Xtring::null, false );
        pReplicaDatabase->dropIndexes( pDoubleConnection, false, true);
        pReplicaDatabase->createIndexes( pDoubleConnection, true );
    } else {
        Xtring diff = DBAPP->upgradeDatabase( pDoubleConnection, pReplicaDatabase, "ofertasdb",
                                              true /*purging*/, false /*silent*/ );
        if( !diff.isEmpty() ) {
            frmlogin->addMessage( Xtring::printf( _("La base de datos de %s no estaba actualizada\n"),
                                                  DBAPP->getPackageString().c_str() ) );
            frmlogin->addMessage( diff );
            DBAPP->processEvents();
        }
    }
    mActivo = true;
	int ejercicio = pEmpresaModule->getEjercicio();
    // Añadir las definiciones de las tablas temporales a la base de datos principal
    dbTableDefinition *tblofertaventa = new dbTableDefinition( *pFicOfertaVenta->getTableDefinition() );
    tblofertaventa->setTemporary(true);
    tblofertaventa->create( getConnection(), false, "", false );
    pDoubleConnection->copyTableToOther( "OFERTAVENTA", "", "EJERCICIO=" + pDoubleConnection->toSQL(ejercicio) );
    pMainDatabase->addTable( tblofertaventa );
    dbTableDefinition *tblofertaventadet = new dbTableDefinition( *pFicOfertaVentaDet->getTableDefinition() );
    tblofertaventadet->setTemporary(true);
    tblofertaventadet->create( getConnection(), false, "", false );
    pDoubleConnection->copyTableToOther( "OFERTAVENTADET", "INNER JOIN OFERTAVENTA ON OFERTAVENTADET.OFERTAVENTA_ID=OFERTAVENTA.ID", "OFERTAVENTA.EJERCICIO=" + pDoubleConnection->toSQL(ejercicio) );
    pMainDatabase->addTable( tblofertaventadet );
    dbTableDefinition *tblofertacompra = new dbTableDefinition( *pFicOfertaCompra->getTableDefinition() );
    tblofertacompra->setTemporary(true);
    tblofertacompra->create( getConnection(), false, "", false );
    pDoubleConnection->copyTableToOther( "OFERTACOMPRA", "", "EJERCICIO=" + pDoubleConnection->toSQL(ejercicio) );
    pMainDatabase->addTable( tblofertacompra );
    dbTableDefinition *tblofertacompradet = new dbTableDefinition( *pFicOfertaCompraDet->getTableDefinition() );
    tblofertacompradet->setTemporary(true);
    tblofertacompradet->create( getConnection(), false, "", false );
    pDoubleConnection->copyTableToOther( "OFERTACOMPRADET", "INNER JOIN OFERTACOMPRA ON OFERTACOMPRADET.OFERTACOMPRA_ID=OFERTACOMPRA.ID", "OFERTACOMPRA.EJERCICIO=" + pDoubleConnection->toSQL(ejercicio) );
    pMainDatabase->addTable( tblofertacompradet );
    dbTableDefinition *tblcobrooferta = new dbTableDefinition( *pFicCobroOferta->getTableDefinition() );
    tblcobrooferta->setTemporary(true);
    tblcobrooferta->create( getConnection(), false, "", false );
    pDoubleConnection->copyTableToOther( "COBROOFERTA", "", "strftime('%Y',FECHAPAGO)=" + pDoubleConnection->toSQL(Xtring::number(ejercicio)) + " OR strftime('%Y',FECHAEMISION)=" + pDoubleConnection->toSQL(Xtring::number(ejercicio)) );
    pMainDatabase->addTable( tblcobrooferta );
    dbTableDefinition *tblpagooferta = new dbTableDefinition( *pFicPagoOferta->getTableDefinition() );
    tblpagooferta->setTemporary(true);
    tblpagooferta->create( getConnection(), false, "", false );
    pDoubleConnection->copyTableToOther( "PAGOOFERTA", "", "strftime('%Y',FECHAPAGO)=" + pDoubleConnection->toSQL(Xtring::number(ejercicio)) + " OR strftime('%Y',FECHAEMISION)=" + pDoubleConnection->toSQL(Xtring::number(ejercicio)) );
    pMainDatabase->addTable( tblpagooferta );
#ifdef HAVE_TESORERIAMODULE
    dbTableDefinition *tblcuentaoferta = new dbTableDefinition( *pFicCuentaOferta->getTableDefinition() );
    tblcuentaoferta->setTemporary(true);
    tblcuentaoferta->create( getConnection(), false, "", false );
    pDoubleConnection->copyTableToOther( "CUENTAOFERTA", Xtring::null, Xtring::null );
    pMainDatabase->addTable( tblcuentaoferta );
    dbTableDefinition *tblapunteoferta = new dbTableDefinition( *pFicApunteOferta->getTableDefinition() );
    tblapunteoferta->setTemporary(true);
    tblapunteoferta->create( getConnection(), false, "", false );
    pDoubleConnection->copyTableToOther( "APUNTEOFERTA", "", "EJERCICIO=" + pDoubleConnection->toSQL(ejercicio) );
    pMainDatabase->addTable( tblapunteoferta );
#endif
	// y ahora las enlazamos
    pDoubleConnection->setLinked( true );
    return true;
}


bool OfertasModule::initDatabase(dbDefinition *db)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    pFicOfertaVenta = new factu::MasterTable( *pReplicaDatabase, "OFERTAVENTA" );
    pFicOfertaVenta->getTableDefinition()->setTemporary(false);
    pFicOfertaVenta->addFieldRecordID();
    pFicOfertaVenta->addFieldEmpresaID();
    pFicOfertaVenta->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicOfertaVenta->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
    pFicOfertaVenta->addFieldInt( "CONTADOR" );
    pFicOfertaVenta->addFieldStringCode( "NUMERO", 20 )->setUnique( false );
    pFicOfertaVenta->addFieldFecha();
    pFicOfertaVenta->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID", false );
    pFicOfertaVenta->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicOfertaVenta->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
    pFicOfertaVenta->addField<factu::FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicOfertaVenta->addFieldsTotalesMaster(true, false); // Añadir entrega a cuentas pero no otros pagos
	pFicOfertaVenta->addFieldDate( "FECHAPAGO" );
	pFicOfertaVenta->addFieldString( "DOCUMENTOPAGO", 50 );
#ifdef HAVE_TESORERIAMODULE
    if ( pTesoreriaModule ) {
		pFicOfertaVenta->addFieldOne2OneRelation( "CUENTAPAGO_ID", "CUENTAOFERTA.ID" )->setCanBeNull(true);
		pFicOfertaVenta->addFieldReferenceID( "APUNTE_ID", "APUNTEOFERTA.ID" );
    }
#endif
    pFicOfertaVenta->addFieldsDesgloseIVA();
    pFicOfertaVenta->addFieldEuro( "COBROS" );
    pFicOfertaVenta->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicOfertaVenta->addFieldNotas();
    pFicOfertaVenta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicOfertaVenta->addRelationDefinition( dbRelationDefinition::one2many, "OFERTAVENTA", "ID", "OFERTAVENTADET", "OFERTAVENTA_ID" );
    pFicOfertaVenta->addMultipleIndex( "ofertaventa_numero_unico", "EMPRESA_ID,EJERCICIO,NUMERO", true );
    pReplicaDatabase->addTable( pFicOfertaVenta->getTableDefinition() );

    pFicOfertaVentaDet = new factu::MasterTable( *pReplicaDatabase, "OFERTAVENTADET" );
    pFicOfertaVentaDet->getTableDefinition()->setTemporary(false);
    pFicOfertaVentaDet->addFieldRecordID();
    pFicOfertaVentaDet->addFieldReferenceID( "OFERTAVENTA_ID", "OFERTAVENTA.ID" );
    pFicOfertaVentaDet->addFieldsComunesDetalles( false /*ventas*/ );
    pFicOfertaVentaDet->addFieldNotas();
    pReplicaDatabase->addTable( pFicOfertaVentaDet->getTableDefinition() );

    pFicOfertaCompra = new factu::MasterTable( *pReplicaDatabase, "OFERTACOMPRA" );
    pFicOfertaCompra->getTableDefinition()->setTemporary(false);
    pFicOfertaCompra->addFieldRecordID();
    pFicOfertaCompra->addFieldEmpresaID();
    pFicOfertaCompra->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicOfertaCompra->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
    pFicOfertaCompra->addFieldInt( "CONTADOR" );
    pFicOfertaCompra->addFieldStringCode( "NUMERO", 20 )->setUnique(false);
    pFicOfertaCompra->addFieldFecha();
    pFicOfertaCompra->addFieldOne2OneRelation( "PROVEEDORA_ID", "PROVEEDORA.ID", false );
    pFicOfertaCompra->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicOfertaCompra->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
    pFicOfertaCompra->addField<factu::FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicOfertaCompra->addFieldsTotalesMaster(true, false); // Añadir entrega a cuentas pero no otros pagos
	pFicOfertaCompra->addFieldDate( "FECHAPAGO" );
	pFicOfertaCompra->addFieldString( "DOCUMENTOPAGO", 50 );
#ifdef HAVE_TESORERIAMODULE
    if ( pTesoreriaModule ) {
		pFicOfertaCompra->addFieldOne2OneRelation( "CUENTAPAGO_ID", "CUENTAOFERTA.ID" )->setCanBeNull(true);
		pFicOfertaCompra->addFieldReferenceID( "APUNTE_ID", "APUNTEOFERTA.ID" );
    }
#endif
	pFicOfertaCompra->addFieldsDesgloseIVA();
    pFicOfertaCompra->addFieldEuro( "PAGOS" );
    pFicOfertaCompra->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicOfertaCompra->addFieldNotas();
    pFicOfertaCompra->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicOfertaCompra->addRelationDefinition( dbRelationDefinition::one2many, "OFERTACOMPRA", "ID", "OFERTACOMPRADET", "OFERTACOMPRA_ID" );
    pFicOfertaCompra->addMultipleIndex( "ofertacompra_proveedora_numero_unico", "EMPRESA_ID,EJERCICIO,PROVEEDORA_ID,NUMERO", true );
    pReplicaDatabase->addTable( pFicOfertaCompra->getTableDefinition() );

    pFicOfertaCompraDet = new factu::MasterTable( *pReplicaDatabase, "OFERTACOMPRADET" );
    pFicOfertaCompraDet->getTableDefinition()->setTemporary(false);
    pFicOfertaCompraDet->addFieldRecordID();
    pFicOfertaCompraDet->addFieldReferenceID( "OFERTACOMPRA_ID", "OFERTACOMPRA.ID" );
    pFicOfertaCompraDet->addFieldsComunesDetalles( true /*compras*/ );
    pFicOfertaCompraDet->addFieldNotas();
    pReplicaDatabase->addTable( pFicOfertaCompraDet->getTableDefinition() );

    pFicCobroOferta= new factu::MasterTable( *pReplicaDatabase, "COBROOFERTA" );
    pFicCobroOferta->getTableDefinition()->setTemporary(false);
    pFicCobroOferta->addFieldRecordID();
    pFicCobroOferta->addFieldEmpresaID();
	// Uso tablafacturas porque todo el código de cobros y pagos está preparado de esta manera
    pFicCobroOferta->addFieldString( "TABLAFACTURAS", 30 );
    pFicCobroOferta->addFieldReferenceID( "FACTURA_ID", "FACTURA.ID" );
    pFicCobroOferta->addFieldString( "TABLATERCEROS", 30 );
    pFicCobroOferta->addFieldReferenceID( "TERCERO_ID", "TERCERO.ID" );
    pFicCobroOferta->addFieldInt( "CONTADOR" );
    pFicCobroOferta->addFieldIntCode( "NUMERO" )->setUnique( false );
    pFicCobroOferta->addFieldString( "NUMEROAGRUPADO", 15 );
    pFicCobroOferta->addFieldBool("AUTOMATICO");
    pFicCobroOferta->addFieldString( "RAZONSOCIAL", 100 );
    pFicCobroOferta->addFieldDesc( "DESCRIPCION" )->setCanBeNull(false);
    pFicCobroOferta->addFieldDate( "FECHAEMISION" )->setCanBeNull(false);
    pFicCobroOferta->addFieldDate( "FECHAVALOR" )->setCanBeNull(false);
    pFicCobroOferta->addFieldDate( "VENCIMIENTO" )->setCanBeNull(false);
    pFicCobroOferta->addFieldEuro( "IMPORTE" );
    pFicCobroOferta->addFieldEuro( "RESTO" );
    pFicCobroOferta->addField<FldNamesListTable>( "ESTADORECIBO" )->setInsertAllowed(false);
    pFicCobroOferta->addFieldDate( "FECHAPAGO" );
    pFicCobroOferta->addFieldString("DOCUMENTOPAGO", 50 );
    pFicCobroOferta->addFieldOne2OneRelation( "MONEDA_ID", "MONEDA.ID" );
    pFicCobroOferta->addFieldFloat( "RATIO_MONEDA" )->setDefaultValue( "1.0" );
#ifdef HAVE_TESORERIAMODULE
    if ( pTesoreriaModule ) {
		pFicCobroOferta->addFieldString( "CUENTAORIGEN", 20 );
		pFicCobroOferta->addFieldOne2OneRelation( "CUENTAPAGO_ID", "CUENTAOFERTA.ID" )->setCanBeNull( true );
		pFicCobroOferta->addFieldReferenceID( "APUNTE_ID", "APUNTEOFERTA.ID" );
    }
#endif
    pFicCobroOferta->addFieldNotas();
    pFicCobroOferta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicCobroOferta->addMultipleIndex( "empresa_numero_unico",
                                       "EMPRESA_ID,NUMERO", true );
    pReplicaDatabase->addTable( pFicCobroOferta->getTableDefinition() );


    pFicPagoOferta= new factu::MasterTable( *pReplicaDatabase, "PAGOOFERTA" );
    pFicPagoOferta->getTableDefinition()->setTemporary(false);
    pFicPagoOferta->addFieldRecordID();
    pFicPagoOferta->addFieldEmpresaID();
	// Uso tablafacturas porque todo el código de cobros y pagos está preparado de esta manera
    pFicPagoOferta->addFieldString( "TABLAFACTURAS", 30 );
    pFicPagoOferta->addFieldReferenceID( "FACTURA_ID", "FACTURA.ID" );
    pFicPagoOferta->addFieldString( "TABLATERCEROS", 30 );
    pFicPagoOferta->addFieldReferenceID( "TERCERO_ID", "TERCERO.ID" );
    pFicPagoOferta->addFieldInt( "CONTADOR" );
    pFicPagoOferta->addFieldIntCode( "NUMERO" )->setUnique( false );
    pFicPagoOferta->addFieldString( "NUMEROAGRUPADO", 15 );
    pFicPagoOferta->addFieldBool("AUTOMATICO");
    pFicPagoOferta->addFieldString( "RAZONSOCIAL", 100 );
    pFicPagoOferta->addFieldDesc( "DESCRIPCION" )->setCanBeNull(false);
    pFicPagoOferta->addFieldDate( "FECHAEMISION" )->setCanBeNull(false);
    pFicPagoOferta->addFieldDate( "FECHAVALOR" )->setCanBeNull(false);
    pFicPagoOferta->addFieldDate( "VENCIMIENTO" )->setCanBeNull(false);
    pFicPagoOferta->addFieldEuro( "IMPORTE" );
    pFicPagoOferta->addFieldEuro( "RESTO" );
    pFicPagoOferta->addField<FldNamesListTable>( "ESTADORECIBO" )->setInsertAllowed(false);
    pFicPagoOferta->addFieldDate( "FECHAPAGO" );
    pFicPagoOferta->addFieldString("DOCUMENTOPAGO", 50 );
    pFicPagoOferta->addFieldOne2OneRelation( "MONEDA_ID", "MONEDA.ID" );
    pFicPagoOferta->addFieldFloat( "RATIO_MONEDA" )->setDefaultValue( "1.0" );
#ifdef HAVE_TESORERIAMODULE
    if ( pTesoreriaModule ) {
		pFicPagoOferta->addFieldString( "CUENTAORIGEN", 20 );
		pFicPagoOferta->addFieldOne2OneRelation( "CUENTAPAGO_ID", "CUENTAOFERTA.ID" )->setCanBeNull( true );
		pFicPagoOferta->addFieldReferenceID( "APUNTE_ID", "APUNTEOFERTA.ID" );
	}
#endif
    pFicPagoOferta->addFieldNotas();
    pFicPagoOferta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicPagoOferta->addMultipleIndex( "empresa_numero_unico",
                                      "EMPRESA_ID,NUMERO", true );
    pReplicaDatabase->addTable( pFicPagoOferta->getTableDefinition() );

#ifdef HAVE_TESORERIAMODULE
	// Parte de tesorería
    // Las cuentas son independientes para cada empresa y ejercicio
    pFicCuentaOferta = new MasterTable( *pReplicaDatabase, "CUENTAOFERTA" );
    pFicCuentaOferta->addFieldRecordID();
    pFicCuentaOferta->addFieldEmpresaID();
    pFicCuentaOferta->addFieldStringCode( "CODIGO", 20 )->setUnique(false);
    pFicCuentaOferta->addFieldDesc( "NOMBRE", 150 );
    pFicCuentaOferta->addFieldEuro( "SALDOINICIAL" );
    pFicCuentaOferta->addFieldDate( "FECHASALDOINICIAL" );
    pFicCuentaOferta->addFieldEuro( "SALDO" )->setReadOnly( true );
    pFicCuentaOferta->addFieldNotas();
    pFicCuentaOferta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicCuentaOferta->addMultipleIndex( "cuenta_codigo_unico", "EMPRESA_ID,CODIGO", true );
    pReplicaDatabase->addTable( pFicCuentaOferta->getTableDefinition() );

    pFicApunteOferta = new MasterTable( *pReplicaDatabase, "APUNTEOFERTA" );
    pFicApunteOferta->addFieldRecordID();
    pFicApunteOferta->addFieldEmpresaID();
    pFicApunteOferta->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicApunteOferta->addFieldOne2OneRelation( "TIPOAPUNTETESORERIA_ID", "TIPOAPUNTETESORERIA.ID" );
    pFicApunteOferta->addFieldInt( "NUMERO" );
    pFicApunteOferta->addFieldDate( "FECHA" );
    pFicApunteOferta->addFieldEuro( "IMPORTE" );
    pFicApunteOferta->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicApunteOferta->addFieldOne2OneRelation( "CUENTAOFERTA_ID", "CUENTAOFERTA.ID" );
    pFicApunteOferta->addFieldListOfValues<Xtring>( false, &tesoreria::ModuleInstance->getTablasTerceros(),
													&tesoreria::ModuleInstance->getTablasTerceros(),
													"TABLATERCEROS" )->setCanBeNull(true);
    pFicApunteOferta->addFieldReferenceID( "TERCERO_ID", "TERCERO.ID" );
    pFicApunteOferta->addFieldDesc( "TERCERO", 200 )->setCanBeNull( true );
    pFicApunteOferta->addFieldListOfValues<Xtring>( false, &tesoreria::ModuleInstance->getTablasConceptos(),
													&tesoreria::ModuleInstance->getTablasConceptos(),
													"TABLACONCEPTOS" )->setCanBeNull(true);
    pFicApunteOferta->addFieldReferenceID( "CONCEPTO_ID", "CONCEPTO.ID" );
    pFicApunteOferta->addFieldDesc( "CONCEPTO", 200 )->setCanBeNull( true );
    pFicApunteOferta->addFieldListOfValues<Xtring>( false, &tesoreria::ModuleInstance->getTablasDocumentos(),
													&tesoreria::ModuleInstance->getTablasDocumentos(),
													"TABLADOCUMENTOS")->setCanBeNull(true);
    pFicApunteOferta->addFieldReferenceID( "DOCUMENTO_ID", "DOCUMENTO.ID" );
    pFicApunteOferta->addFieldDesc( "REFERENCIA", 200 )->setCanBeNull( true );
    pFicApunteOferta->addFieldBool( "AUTOMATICO" )->setDefaultValue( "0" )->setReadOnly( true );
    pFicApunteOferta->addFieldNotas();
    pFicApunteOferta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pReplicaDatabase->addTable( pFicApunteOferta->getTableDefinition() );
#endif

    return true;
}



FrmEditRec *OfertasModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<OFERTASMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "OFERTACOMPRA" )
		return new FrmEditOfertaCompra(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "OFERTAVENTA" )
		return new FrmEditOfertaVenta(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PAGOOFERTA" )
		return new FrmEditPagoOferta(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "COBROOFERTA" )
		return new FrmEditCobroOferta(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>OFERTASMODULE_CREATE_EDITFORM*/
// {capel} Borrar APUNTEOFERTA y CUENTAOFERTA
    return 0;
}

FrmEditRecDetail *OfertasModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<OFERTASMODULE_CREATE_EDITFORM_DETAIL*/
	if( tablename.upper() == "OFERTACOMPRADET" )
		return new FrmEditOfertaCompraDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "OFERTAVENTADET" )
		return new FrmEditOfertaVentaDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
/*>>>>>OFERTASMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

/*<<<<<OFERTASMODULE_SLOT_OFERTASOFERTACOMPRA*/
void OfertasModule::slotMenuOfertasOfertaCompra()
{
	pMainWindow->slotMenuEditRecMaestro( "OFERTACOMPRA" );
}
/*>>>>>OFERTASMODULE_SLOT_OFERTASOFERTACOMPRA*/
/*<<<<<OFERTASMODULE_SLOT_OFERTASOFERTAVENTA*/
void OfertasModule::slotMenuOfertasOfertaVenta()
{
	pMainWindow->slotMenuEditRecMaestro( "OFERTAVENTA" );
}
/*>>>>>OFERTASMODULE_SLOT_OFERTASOFERTAVENTA*/
/*<<<<<OFERTASMODULE_SLOT_OFERTASPAGOOFERTA*/
void OfertasModule::slotMenuOfertasPagoOferta()
{
	pMainWindow->slotMenuEditRecMaestro( "PAGOOFERTA" );
}
/*>>>>>OFERTASMODULE_SLOT_OFERTASPAGOOFERTA*/
/*<<<<<OFERTASMODULE_SLOT_OFERTASCOBROOFERTA*/
void OfertasModule::slotMenuOfertasCobroOferta()
{
	pMainWindow->slotMenuEditRecMaestro( "COBROOFERTA" );
}
/*>>>>>OFERTASMODULE_SLOT_OFERTASCOBROOFERTA*/
#ifdef HAVE_TESORERIAMODULE
/*<<<<<OFERTASMODULE_SLOT_OFERTASAPUNTEOFERTA*/
void OfertasModule::slotMenuOfertasApunteOferta()
{
	pMainWindow->slotMenuEditRecMaestro( "APUNTEOFERTA" );
}
/*>>>>>OFERTASMODULE_SLOT_OFERTASAPUNTEOFERTA*/
/*<<<<<OFERTASMODULE_SLOT_OFERTASCUENTAOFERTA*/
void OfertasModule::slotMenuOfertasCuentaOferta()
{
	pMainWindow->slotMenuEditRecMaestro( "CUENTAOFERTA" );
}
/*>>>>>OFERTASMODULE_SLOT_OFERTASCUENTAOFERTA*/
#endif
bool OfertasModule::initMainWindow(MainWindow *mainwin)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT(mainwin);
    pMainWindow = mainwin;
    if( !mActivo )
        return false;
    pMenuOfertas = new QMenu( pMainWindow );
    pMenuOfertas->setObjectName( "MenuOfertas" );
    pMainWindow->menuBar()->insertItem( toGUI( _( "&Ofertas" ) ), pMenuOfertas);
    /*<<<<<OFERTASMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("OFERTACOMPRA")->getDescPlural();
		pMenuOfertasOfertaCompra = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuOfertasOfertaCompra->setObjectName( "MenuOfertasOfertaCompra" );
		pMenuOfertasOfertaCompra->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuOfertasOfertaCompra->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuOfertasOfertaCompra, SIGNAL(activated()), this, SLOT(slotMenuOfertasOfertaCompra()));
		pMenuOfertasOfertaCompra->addTo(pMenuOfertas);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("OFERTAVENTA")->getDescPlural();
		pMenuOfertasOfertaVenta = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuOfertasOfertaVenta->setObjectName( "MenuOfertasOfertaVenta" );
		pMenuOfertasOfertaVenta->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuOfertasOfertaVenta->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuOfertasOfertaVenta, SIGNAL(activated()), this, SLOT(slotMenuOfertasOfertaVenta()));
		pMenuOfertasOfertaVenta->addTo(pMenuOfertas);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("PAGOOFERTA")->getDescPlural();
		pMenuOfertasPagoOferta = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuOfertasPagoOferta->setObjectName( "MenuOfertasPagoOferta" );
		pMenuOfertasPagoOferta->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuOfertasPagoOferta->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuOfertasPagoOferta, SIGNAL(activated()), this, SLOT(slotMenuOfertasPagoOferta()));
		pMenuOfertasPagoOferta->addTo(pMenuOfertas);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("COBROOFERTA")->getDescPlural();
		pMenuOfertasCobroOferta = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuOfertasCobroOferta->setObjectName( "MenuOfertasCobroOferta" );
		pMenuOfertasCobroOferta->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuOfertasCobroOferta->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuOfertasCobroOferta, SIGNAL(activated()), this, SLOT(slotMenuOfertasCobroOferta()));
		pMenuOfertasCobroOferta->addTo(pMenuOfertas);
	}
/*>>>>>OFERTASMODULE_INITMAINWINDOW_MENUS*/
// {capel} Borrar APUNTEOFERTA y CUENTAOFERTA

    {
        Xtring caption = "Copiar albaranes de compra";
        pMenuOfertasCopiarAlbaranesCompra = new QAction( toGUI( caption ) + "...", pMainWindow );
        pMenuOfertasCopiarAlbaranesCompra->setObjectName( "MenuOfertasCopiarAlbaranesCompra" );
        pMainWindow->connect(pMenuOfertasCopiarAlbaranesCompra, SIGNAL(activated()), this, SLOT(slotMenuOfertasCopiarAlbaranesCompra()));
        pMenuOfertasCopiarAlbaranesCompra->addTo(pMenuOfertas);
    }
    {
        Xtring caption = "Copiar albaranes de venta";
        pMenuOfertasCopiarAlbaranesVenta = new QAction( toGUI( caption ) + "...", pMainWindow );
        pMenuOfertasCopiarAlbaranesVenta->setObjectName( "MenuOfertasCopiarAlbaranesVenta" );
        pMainWindow->connect(pMenuOfertasCopiarAlbaranesVenta, SIGNAL(activated()), this, SLOT(slotMenuOfertasCopiarAlbaranesVenta()));
        pMenuOfertasCopiarAlbaranesVenta->addTo(pMenuOfertas);
    }
    {
        Xtring caption = "Copiar facturas de compra";
        pMenuOfertasCopiarFacturasCompra = new QAction( toGUI( caption ) + "...", pMainWindow );
        pMenuOfertasCopiarFacturasCompra->setObjectName( "MenuOfertasCopiarFacturasCompra" );
        pMainWindow->connect(pMenuOfertasCopiarFacturasCompra, SIGNAL(activated()), this, SLOT(slotMenuOfertasCopiarFacturasCompra()));
        pMenuOfertasCopiarFacturasCompra->addTo(pMenuOfertas);
    }
    {
        Xtring caption = "Copiar facturas de venta";
        pMenuOfertasCopiarFacturasVenta = new QAction( toGUI( caption ) + "...", pMainWindow );
        pMenuOfertasCopiarFacturasVenta->setObjectName( "MenuOfertasCopiarFacturasVenta" );
        pMainWindow->connect(pMenuOfertasCopiarFacturasVenta, SIGNAL(activated()), this, SLOT(slotMenuOfertasCopiarFacturasVenta()));
        pMenuOfertasCopiarFacturasVenta->addTo(pMenuOfertas);
    }
    return true;
}

void OfertasModule::slotMenuOfertasCopiarAlbaranesCompra()
{
    factu::RecAlbaranCompra *orig_record = static_cast<factu::RecAlbaranCompra *>( DBAPP->createRecord("ALBARANCOMPRA") );
    List<dbRecordID> albaran_ids;
    if( !DBAPP->chooseMulti( albaran_ids, static_cast<FrmEditRec *>(0), orig_record, 0, dbApplication::readOnly ) )
        return;
    int resp = FrmBase::NoButton, prev_resp = FrmBase::NoButton;
    for( List<dbRecordID>::const_iterator albit = albaran_ids.begin();
            albit != albaran_ids.end(); albit++) {
        DBAPP->processEvents();
        orig_record->read(*albit);
        dbRecordList *detalles = orig_record->getListAlbaranCompraDet();
        RecOfertaCompra *new_record = static_cast<RecOfertaCompra *>( DBAPP->createRecord("OFERTACOMPRA") );
        new_record->copyRecord( orig_record );
        for( dbRecordList::const_iterator it = detalles->begin();
                it != detalles->end(); ++it ) {
            dbRecord *detalle = *it;
            RecOfertaCompraDet *new_detalle = static_cast<RecOfertaCompraDet *>( DBAPP->createRecord("OFERTACOMPRADET") );
            new_detalle->copyRecord( detalle );
            new_detalle->setNew( true );
            new_record->getListOfertaCompraDet()->addRecord( new_detalle );
        }
        FrmEditRec *frmeditoferta = DBAPP->createEditForm(0,
                                    new_record, 0, DataTable::inserting, dbApplication::simpleEdition);
        if( !frmeditoferta->showAndSave() )
            frmeditoferta->showModalFor( 0, true, true );
        if( frmeditoferta->isSaved() ) {
            copiaCobros(orig_record, new_record);
            if( prev_resp == FrmBase::NoButton )
                resp = FrmBase::msgYesNoAllCancel("Copiar ofertas", "La oferta se ha guardado perfectamente.\n¿Quieres borrar el albarán de compra original?");
            if( resp == FrmBase::Yes || resp == FrmBase::YesToAll ) {
                if( resp == FrmBase::YesToAll )
                    prev_resp = resp;
                if( orig_record->remove() ) {
                    DBAPP->showOSD("El albarán de compra original se ha borrado correctamente","");
                } else {
                    FrmBase::msgError("Error borrando el albarán de compra original", "");
                }
            } else if ( resp == FrmBase::NoToAll ) {
                prev_resp = resp;
            } else if ( resp == FrmBase::Cancel ) {
                break;
            }
        }
        delete frmeditoferta;
        delete new_record;
    }
    delete orig_record;
}

void OfertasModule::slotMenuOfertasCopiarAlbaranesVenta()
{
    factu::RecAlbaranVenta *orig_record = static_cast<factu::RecAlbaranVenta *>( DBAPP->createRecord("ALBARANVENTA") );
    List<dbRecordID> albaran_ids;
    if( !DBAPP->chooseMulti( albaran_ids, static_cast<FrmEditRec *>(0), orig_record, 0, dbApplication::readOnly ) )
        return;
    int resp = FrmBase::NoButton, prev_resp = FrmBase::NoButton;
    for( List<dbRecordID>::const_iterator albit = albaran_ids.begin();
            albit != albaran_ids.end(); albit++) {
        DBAPP->processEvents();
        orig_record->read(*albit);
        dbRecordList *detalles = orig_record->getListAlbaranVentaDet();
        RecOfertaVenta *new_record = static_cast<RecOfertaVenta *>( DBAPP->createRecord("OFERTAVENTA") );
        new_record->copyRecord( orig_record );
        new_record->setValue("CONTADOR", 0);
        new_record->setValue("NUMERO", 0);
        for( dbRecordList::const_iterator it = detalles->begin();
                it != detalles->end(); ++it ) {
            dbRecord *detalle = *it;
            RecOfertaVentaDet *new_detalle = static_cast<RecOfertaVentaDet *>( DBAPP->createRecord("OFERTAVENTADET") );
            new_detalle->copyRecord( detalle );
            new_detalle->setNew( true );
            new_record->getListOfertaVentaDet()->addRecord( new_detalle );
        }
        FrmEditRec *frmeditoferta = DBAPP->createEditForm(0,
                                    new_record, 0, DataTable::inserting, dbApplication::simpleEdition);
        if( !frmeditoferta->showAndSave() )
            frmeditoferta->showModalFor( 0, true, true );
        if( frmeditoferta->isSaved() ) {
            copiaCobros(orig_record, new_record);
            if( prev_resp == FrmBase::NoButton )
                resp = FrmBase::msgYesNoAllCancel("Copiando a ofertas", "La oferta se ha guardado perfectamente.\n¿Quieres borrar el albarán de venta original?");
            if( resp == FrmBase::Yes || resp == FrmBase::YesToAll ) {
                if( resp == FrmBase::YesToAll )
                    prev_resp = resp;
                if( orig_record->remove() ) {
                    DBAPP->showOSD("El albarán de venta original se ha borrado correctamente","");
                } else {
                    FrmBase::msgError("Error borrando el albarán de venta original", "");
                }
            } else if ( resp == FrmBase::NoToAll ) {
                prev_resp = resp;
            } else if ( resp == FrmBase::Cancel ) {
                break;
            }
        }
        delete frmeditoferta;
        delete new_record;
    }
    delete orig_record;
}

void OfertasModule::slotMenuOfertasCopiarFacturasCompra()
{
    factu::RecFacturaCompra *orig_record = static_cast<factu::RecFacturaCompra *>( DBAPP->createRecord("FACTURACOMPRA") );
    List<dbRecordID> factura_ids;
    if( !DBAPP->chooseMulti( factura_ids, static_cast<FrmEditRec *>(0), orig_record, 0, dbApplication::readOnly ) )
        return;
    int resp = FrmBase::NoButton, prev_resp = FrmBase::NoButton;
    for( List<dbRecordID>::const_iterator facit = factura_ids.begin();
            facit != factura_ids.end(); facit++) {
        DBAPP->processEvents();
        orig_record->read(*facit);
        dbRecordList *detalles = orig_record->getListFacturaCompraDet();
        RecOfertaCompra *new_record = static_cast<RecOfertaCompra *>( DBAPP->createRecord("OFERTACOMPRA") );
        new_record->copyRecord( orig_record );
        for( dbRecordList::const_iterator it = detalles->begin();
                it != detalles->end(); ++it ) {
            dbRecord *detalle = *it;
            RecOfertaCompraDet *new_detalle = static_cast<RecOfertaCompraDet *>( DBAPP->createRecord("OFERTACOMPRADET") );
            new_detalle->copyRecord( detalle );
            new_detalle->setNew( true );
            new_record->getListOfertaCompraDet()->addRecord( new_detalle );
        }
        FrmEditRec *frmeditoferta = DBAPP->createEditForm(0,
                                    new_record, 0, DataTable::inserting, dbApplication::simpleEdition);
        if( !frmeditoferta->showAndSave() )
            frmeditoferta->showModalFor( 0, true, true );
        if( frmeditoferta->isSaved() ) {
            copiaCobros(orig_record, new_record);
            if( prev_resp == FrmBase::NoButton )
                resp = FrmBase::msgYesNoAllCancel("Copiando a ofertas", "La oferta se ha guardado perfectamente.\n¿Quieres borrar la factura de compra original?");
            if( resp == FrmBase::Yes || resp == FrmBase::YesToAll ) {
                if( resp == FrmBase::YesToAll )
                    prev_resp = resp;
                if( orig_record->remove() ) {
                    DBAPP->showOSD("La factura de compra original se ha borrado correctamente","");
                } else {
                    FrmBase::msgError("Error borrando la factura de compra original", "");
                }
            } else if ( resp == FrmBase::NoToAll ) {
                prev_resp = resp;
            } else if ( resp == FrmBase::Cancel ) {
                break;
            }
        }
        delete frmeditoferta;
        delete new_record;
    }
    delete orig_record;
}

void OfertasModule::slotMenuOfertasCopiarFacturasVenta()
{
    factu::RecFacturaVenta *orig_record = static_cast<factu::RecFacturaVenta *>( DBAPP->createRecord("FACTURAVENTA") );
    List<dbRecordID> factura_ids;
    if( !DBAPP->chooseMulti( factura_ids, static_cast<FrmEditRec *>(0), orig_record, 0, dbApplication::readOnly ) )
        return;
    int resp = FrmBase::NoButton, prev_resp = FrmBase::NoButton;
    for( List<dbRecordID>::const_iterator facit = factura_ids.begin();
            facit != factura_ids.end(); facit++) {
        DBAPP->processEvents();
        orig_record->read(*facit);
        dbRecordList *detalles = orig_record->getListFacturaVentaDet();
        RecOfertaVenta *new_record = static_cast<RecOfertaVenta *>( DBAPP->createRecord("OFERTAVENTA") );
        new_record->copyRecord( orig_record );
        new_record->setValue("CONTADOR", 0);
        new_record->setValue("NUMERO", 0);
        for( dbRecordList::const_iterator it = detalles->begin();
                it != detalles->end(); ++it ) {
            dbRecord *detalle = *it;
            RecOfertaVentaDet *new_detalle = static_cast<RecOfertaVentaDet *>( DBAPP->createRecord("OFERTAVENTADET") );
            new_detalle->copyRecord( detalle );
            new_detalle->setNew( true );
            new_record->getListOfertaVentaDet()->addRecord( new_detalle );
        }
        FrmEditRec *frmeditoferta = DBAPP->createEditForm(0,
                                    new_record, 0, DataTable::inserting, dbApplication::simpleEdition);
        if( !frmeditoferta->showAndSave() )
            frmeditoferta->showModalFor( 0, true, true );
        if( frmeditoferta->isSaved() ) {
            copiaCobros(orig_record, new_record);
            if( prev_resp == FrmBase::NoButton )
                resp = FrmBase::msgYesNoAllCancel("Copiando a ofertas", "La oferta se ha guardado perfectamente.\n¿Quieres borrar la factura de venta original?");
            if( resp == FrmBase::Yes || resp == FrmBase::YesToAll ) {
                if( resp == FrmBase::YesToAll )
                    prev_resp = resp;
                if( orig_record->remove() ) {
                    DBAPP->showOSD("La factura de venta original se ha borrado correctamente","");
                } else {
                    FrmBase::msgError("Error borrando la factura de venta original", "");
                }
            } else if ( resp == FrmBase::NoToAll ) {
                prev_resp = resp;
            } else if ( resp == FrmBase::Cancel ) {
                break;
            }
        }
        delete frmeditoferta;
        delete new_record;
    }
    delete orig_record;
}

/**
 * @brief Copia los cobros de un albarán o factura de compra o venta a la oferta recién creada
 *
 * @param origen el albarán o factura del que se copiarán los cobros
 * @param nueva la oferta recién creada
 * @return int
 */
int OfertasModule::copiaCobros(dbRecord* origen, dbRecord* nueva)
{
    Xtring destname, cobrostabla, ofertatabla;
    const Xtring &origenname = origen->getTableName();
    if( origenname == "ALBARANCOMPRA" || origenname == "FACTURACOMPRA" ) {
        destname = "PAGOOFERTA";
        cobrostabla = "PAGO";
		ofertatabla = "OFERTACOMPRA";
    } else {
        destname = "COBROOFERTA";
        cobrostabla = "COBRO";
		ofertatabla = "OFERTAVENTA";
    }
    // Borrar todos los pagos o cobros que se han generado nuevos al copiar la oferta
    origen->getConnection()->exec( "DELETE FROM " + destname
                                   + " WHERE " + ofertatabla + "_ID=" + origen->getConnection()->toSQL( nueva->getRecordID() ) );
    // Copiarlos del albarán o factura original
    int copiados = 0;
    dbResultSet *rs = origen->getConnection()->select( "SELECT id FROM " + cobrostabla
                      + " WHERE TABLAFACTURAS=" + origen->getConnection()->toSQL( origenname )
                      + " AND FACTURA_ID=" + origen->getConnection()->toSQL( origen->getRecordID() ) );
    while( rs->next() ) {
        dbRecord *cobroorigen = DBAPP->createRecord( cobrostabla );
        cobroorigen->read( rs->toInt((uint)0) );
        dbRecord *cobrodestino = DBAPP->createRecord( destname );
        cobrodestino->copyRecord( cobroorigen );
        cobrodestino->setValue( ofertatabla + "_ID", nueva->getRecordID());
        cobrodestino->setValue("CONTADOR", 0); // Para que lo genere al guardarlo
        cobrodestino->save(false, false);
        copiados++;
    }
    delete rs;
    return copiados;
}


dbRecord *OfertasModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
#define getConnection() pDoubleConnection
    /*<<<<<OFERTASMODULE_CREATE_RECORD*/
	if( tablename.upper() == "OFERTACOMPRA" )
		return new RecOfertaCompra(getConnection(), recid, user);
	if( tablename.upper() == "OFERTACOMPRADET" )
		return new RecOfertaCompraDet(getConnection(), recid, user);
	if( tablename.upper() == "OFERTAVENTA" )
		return new RecOfertaVenta(getConnection(), recid, user);
	if( tablename.upper() == "OFERTAVENTADET" )
		return new RecOfertaVentaDet(getConnection(), recid, user);
	if( tablename.upper() == "PAGOOFERTA" )
		return new RecPagoOferta(getConnection(), recid, user);
	if( tablename.upper() == "COBROOFERTA" )
		return new RecCobroOferta(getConnection(), recid, user);
/*>>>>>OFERTASMODULE_CREATE_RECORD*/
// {capel} Borrar APUNTEOFERTA y CUENTAOFERTA
    return 0;
}

/*<<<<<OFERTASMODULE_FIN*/
} // namespace ofertas
} // namespace gong

/*>>>>>OFERTASMODULE_FIN*/
/*<<<<<OFERTASMODULE_INIT_DATABASE*/

/*>>>>>OFERTASMODULE_INIT_DATABASE*/

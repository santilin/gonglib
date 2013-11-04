/*<<<<<COPYLEFT*/
/** @file sociasmodule.cpp Plugin de gonglib para la gestión de las socias
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<MODULE_INFO*/
// COPYLEFT Plugin de gonglib para la gestión de las socias
// RECORD Proyecto FrmEditRecMaster Socias
// RECORD Miembro FrmEditRecMaster Socias
// RECORD TipoSocia FrmEditRecMaster Socias
// RECORD PartidaProyecto FrmEditRecMaster Socias
// NAMESLISTTABLE SociasEstado
// NAMESLISTTABLE Periodicidad
// MODULE_REQUIRED Empresa
// MODULE_REQUIRED Contactos
// MODULE_REQUIRED Pagos
// MODULE_OPTIONAL Contab
// MODULE_OPTIONAL Factu
// TYPE GongModule socias::SociasModule
/*>>>>>MODULE_INFO*/
// RECORD Apunte FrmEditRecDetail #ifdef HAVE_CONTABMODULE

/*<<<<<SOCIASMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include <dbappnameslisttable.h>
#include <dbappfrmeditnameslisttable.h>
#include "sociasmodule.h"
#include "sociasfrmeditproyecto.h"
#include "sociasfrmeditmiembro.h"
#include "sociasfrmedittiposocia.h"
#include "sociasfrmeditpartidaproyecto.h"
/*>>>>>SOCIASMODULE_INCLUDES*/
#include "sociasfrmgenerarrecibos.h"
#include <contactosmodule.h>
#include "sociasfrmmailing.h"
#include "sociasfrmeditcobrobehavior.h"
#include "sociasfrmeditremesacobrobehavior.h"
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include "sociasrecapunte.h"
#include "sociasfrmeditapunte.h"
#endif
#ifdef HAVE_PAGOSMODULE
#include "sociascobrobehavior.h"
#endif

namespace gong {
namespace socias {

SociasModule *ModuleInstance = 0;

/* Añadir settings:
 * CODIGO_TIPODOC_RECIBO
 */
SociasModule::SociasModule()
    : dbModule("socias"), pContabModule(0), pFactuModule(0)
{
    ModuleInstance = this;
    _GONG_DEBUG_TRACE(1);
    /*<<<<<SOCIASMODULE_PUBLIC_INFO*/
	mModuleRequires << "empresa" << "contactos" << "pagos";
	mMasterTables << "PROYECTO" << "MIEMBRO" << "TIPOSOCIA" << "PARTIDAPROYECTO" << "SOCIASESTADO" << "PERIODICIDAD";
//	mDetailTables
	pEmpresaModule = static_cast< empresa::EmpresaModule * >(DBAPP->findModule( "Empresa" ));
	pContactosModule = static_cast< contactos::ContactosModule * >(DBAPP->findModule( "Contactos" ));
	pPagosModule = static_cast< pagos::PagosModule * >(DBAPP->findModule( "Pagos" ));
#ifdef HAVE_CONTABMODULE
	pContabModule = static_cast< contab::ContabModule * >(DBAPP->findModule( "Contab" ));
#endif
#ifdef HAVE_FACTUMODULE
	pFactuModule = static_cast< factu::FactuModule * >(DBAPP->findModule( "Factu" ));
#endif
/*>>>>>SOCIASMODULE_PUBLIC_INFO*/
}


SociasModule::~SociasModule()
{
    _GONG_DEBUG_PRINT(1, "Destruyendo module SociasModule...");
}

bool SociasModule::login(FrmLogin *frmlogin, const Xtring& version,
                         Xtring& addTitle, bool startingapp)
{
    return true;
}

void SociasModule::afterLoad()
{
    FldNamesListTable *fldest = static_cast<FldNamesListTable *>(
                                    DBAPP->getDatabase()->findFieldDefinition("MIEMBRO.SOCIASESTADO") );
    if( fldest )
        fldest->fill( *getConnection() );
    if( fldest->getListOfValues()->size() == 0 ) {
        getConnection()->exec( "INSERT INTO SOCIASESTADO (CODIGO,NOMBRE) VALUES "
                               "( 1, 'Activo'),( 2, 'Inactivo' )" );
        fldest->fill( *getConnection() );
    }
    fldest = static_cast<FldNamesListTable *>(
                 DBAPP->getDatabase()->findFieldDefinition("PROYECTO.SOCIASESTADO") );
    if( fldest )
        fldest->fill( *getConnection() );
    FldNamesListTable *fldper = static_cast<FldNamesListTable *>(
                                    DBAPP->getDatabase()->findFieldDefinition("PROYECTO.PERIODICIDAD") );
    if( fldper )
        fldper->fill( *getConnection() );
    if( fldper->getListOfValues()->size() == 0 ) {
        getConnection()->exec( "INSERT INTO PERIODICIDAD (CODIGO,NOMBRE) VALUES "
                               "(" + getConnection()->toSQL( puntual ) + ", 'Puntual'),"
                               "(" + getConnection()->toSQL( diaria ) + ", 'Diaria' ),"
                               "(" + getConnection()->toSQL( semanal ) + ", 'Semanal'),"
                               "(" + getConnection()->toSQL( quincenal ) + ", 'Quincenal' ),"
                               "(" + getConnection()->toSQL( mensual ) + ", 'Mensual' ),"
                               "(" + getConnection()->toSQL( bimensual ) + ", 'Bimensual' ),"
                               "(" + getConnection()->toSQL( trimestral ) + ", 'Trimestral'),"
                               "(" + getConnection()->toSQL( cuatrimestral ) + ", 'Cuatrimestral' ),"
                               "(" + getConnection()->toSQL( semestral ) + ", 'Semestral' ),"
                               "(" + getConnection()->toSQL( anual ) + ", 'Anual' ),"
                               "(" + getConnection()->toSQL( bianual ) + ", 'Bianual')" );
        fldper->fill( *getConnection() );
    }
}

void SociasModule::afterCreateEditForm(FrmEditRec* frm, dbRecord* rec)
{
    if( rec->getTableName() == "COBRO" ) {
        frm->addBehavior( new FrmEditCobroBehavior( frm ) );
    }
    if( rec->getTableName() == "REMESACOBRO" ) {
        frm->addBehavior( new FrmEditRemesaCobroBehavior( frm ) );
    }
}


bool SociasModule::initDatabase(dbDefinition *db)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    _GONG_DEBUG_TRACE(2);
    _GONG_DEBUG_ASSERT( contactos::ModuleInstance->getFicContacto() );

    /*<<<<<SOCIASMODULE_INIT_DATABASE*/
	pFicSociasEstado = new NamesListTable( *pMainDatabase, "SOCIASESTADO" );
	pMainDatabase->addTable( pFicSociasEstado->getTableDefinition() );
	pFicPeriodicidad = new NamesListTable( *pMainDatabase, "PERIODICIDAD" );
	pMainDatabase->addTable( pFicPeriodicidad->getTableDefinition() );
/*>>>>>SOCIASMODULE_INIT_DATABASE*/

    dbTableDefinition *tbldef = getDatabase()->findTableDefinition( "COBRO" );
    if( tbldef ) {
        tbldef->addBehavior( new CobroBehavior() );
    }


    if( !db->findTableDefinition( "PROYECTO" ) ) {
        pFicProyecto = new socias::MasterTable( *db, "PROYECTO" );
        pFicProyecto->addFieldRecordID();
        pFicProyecto->addFieldCodigo()->setCanBeNull(false);
        pFicProyecto->addFieldDesc( "NOMBRE" )->setUnique(true);
        pFicProyecto->addFieldNotas();
        pFicProyecto->addBehavior( DBAPP->getRecordTimestampBehavior() );
    } else {
        pFicProyecto = new socias::MasterTable( db->findTableDefinition( "PROYECTO" ) );
    }
    pFicProyecto->addFieldDate( "FECHAINICIO" )->setCanBeNull(false);
    pFicProyecto->addFieldDate( "FECHAFIN" );
    pFicProyecto->addField<FldNamesListTable>( "SOCIASESTADO" )->setInsertAllowed(true);
    pFicProyecto->addFieldText( "DESCRIPCION" )->setCanBeNull( true );
    pFicProyecto->addFieldEuro( "IMPORTE" );
    pFicProyecto->addField<FldNamesListTable>("PERIODICIDAD");
    pFicProyecto->addFieldInt( "NUMCUOTAS" );
    pFicProyecto->addFieldString( "FORMATONUMRECIBO", 50 );
    pMainDatabase->addTable( pFicProyecto->getTableDefinition() );

    pFicTipoSocia = new socias::MasterTable( *db, "TIPOSOCIA" );
    pFicTipoSocia->addFieldRecordID();
    pFicTipoSocia->addFieldCodigo();
    pFicTipoSocia->addFieldDesc( "NOMBRE" )->setUnique(true);
    pFicTipoSocia->addFieldPercent( "PORCENTAJECUOTA" );
    pFicTipoSocia->addFieldNotas();
    pFicTipoSocia->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicTipoSocia->getTableDefinition() );

    pFicMiembro = new socias::MasterTable( *db, "MIEMBRO" );
    pFicMiembro->addFieldRecordID();
    pFicMiembro->addFieldCodigo( "NUMEROSOCIA" )->setCanBeNull( false )->setUnique( false );
    pFicMiembro->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID" )->setCanBeNull( false );
    pFicMiembro->addFieldOne2OneRelation( "CONTACTO_ID", "CONTACTO.ID" )->setCanBeNull( false );
    pFicMiembro->addMultipleIndex( "proyecto_contacto_unico", "PROYECTO_ID,CONTACTO_ID", true );
#ifdef HAVE_FACTUMODULE
    if( getFactuModule() ) {
        pFicMiembro->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID" )->setCanBeNull( false );
        pFicMiembro->addMultipleIndex( "proyecto_cliente_unico", "PROYECTO_ID,CLIENTE_ID", true );
    }
#endif
    pFicMiembro->addFieldOne2OneRelation( "TIPOSOCIA_ID", "TIPOSOCIA.ID" )->setCanBeNull( false );
    pFicMiembro->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID" )->setCanBeNull( false );
    pFicMiembro->addFieldDate( "FECHAALTA", dbFieldDefinition::DESCRIPTION );
    pFicMiembro->addFieldDate( "FECHABAJA" );
    pFicMiembro->addField<FldNamesListTable>( "SOCIASESTADO" )->setInsertAllowed(true);
    pFicMiembro->addFieldString( "MOTIVOBAJA", 50 );
    pFicMiembro->addField<empresa::FldCuentaBanco>( "CUENTABANCO" );
    pFicMiembro->addFieldEuro( "IMPORTETOTAL" );
    pFicMiembro->addFieldEuro( "RESTOTOTAL" );
    pFicMiembro->addFieldEuro( "PAGOSTOTAL" );
    pFicMiembro->addFieldNotas();
    pFicMiembro->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicMiembro->addMultipleIndex( "proyecto_numerosocia_unico", "PROYECTO_ID,NUMEROSOCIA", true );
    pMainDatabase->addTable( pFicMiembro->getTableDefinition() );

    pFicPartidaProyecto = new socias::MasterTable( *db, "PARTIDAPROYECTO" );
    pFicPartidaProyecto->addFieldRecordID();
    pFicPartidaProyecto->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID" );
    pFicPartidaProyecto->addFieldStringCode( "CODIGO", 15 )->setUnique( false );
    pFicPartidaProyecto->addFieldDesc( "DESCRIPCION" )->setCanBeNull(false);
    // "Tipo (G,I,T)", "Tipo (Gasto,Ingreso,Total)"
    pFicPartidaProyecto->addFieldChar( "TIPO" )->setCanBeNull(false);
    // "Código de la partida madre"
    pFicPartidaProyecto->addFieldString( "MADRE", 15 );
    pFicPartidaProyecto->addFieldEuro( "PRESUPUESTO" );
    pFicPartidaProyecto->addFieldEuro( "IMPORTE" );
    pFicPartidaProyecto->addFieldInt( "ORDEN" );
    pFicPartidaProyecto->addFieldNotas();
    pFicPartidaProyecto->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicPartidaProyecto->addMultipleIndex( "PROYECTO_CODIGO_UNICO", "PROYECTO_ID,CODIGO", true );
    pMainDatabase->addTable( pFicPartidaProyecto->getTableDefinition() );

#ifdef HAVE_CONTABMODULE
    if( contab::ModuleInstance ) {
        contab::MasterTable *ficApuntes = contab::ModuleInstance->getFicheroApuntes();
        if( ficApuntes ) {
            ficApuntes->addFieldOne2OneRelation( "PARTIDAPROYECTO_ID", "PARTIDAPROYECTO.ID" )
            ->setCanBeNull(true);
        }
    }
#endif
    return true;
}


bool SociasModule::initMainWindow(MainWindow *mainwin)
{
    _GONG_DEBUG_ASSERT(  ModuleInstance  ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( mainwin );
    pMainWindow = mainwin;
    _GONG_DEBUG_TRACE(2);
    pMenuSocias = new QMenu( pMainWindow );
    pMenuSocias->setObjectName( "MenuSocias" );
    pMainWindow->menuBar()->insertItem( toGUI( _( "&Socias" ) ), pMenuSocias );

    /*<<<<<SOCIASMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("PROYECTO")->getDescPlural();
		pMenuSociasProyecto = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuSociasProyecto->setObjectName( "MenuSociasProyecto" );
		pMenuSociasProyecto->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuSociasProyecto->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuSociasProyecto, SIGNAL(activated()), this, SLOT(slotMenuSociasProyecto()));
		pMenuSociasProyecto->addTo(pMenuSocias);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("MIEMBRO")->getDescPlural();
		pMenuSociasMiembro = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuSociasMiembro->setObjectName( "MenuSociasMiembro" );
		pMenuSociasMiembro->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuSociasMiembro->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuSociasMiembro, SIGNAL(activated()), this, SLOT(slotMenuSociasMiembro()));
		pMenuSociasMiembro->addTo(pMenuSocias);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("TIPOSOCIA")->getDescPlural();
		pMenuSociasTipoSocia = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuSociasTipoSocia->setObjectName( "MenuSociasTipoSocia" );
		pMenuSociasTipoSocia->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuSociasTipoSocia->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuSociasTipoSocia, SIGNAL(activated()), this, SLOT(slotMenuSociasTipoSocia()));
		pMenuSociasTipoSocia->addTo(pMenuSocias);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("PARTIDAPROYECTO")->getDescPlural();
		pMenuSociasPartidaProyecto = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuSociasPartidaProyecto->setObjectName( "MenuSociasPartidaProyecto" );
		pMenuSociasPartidaProyecto->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuSociasPartidaProyecto->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuSociasPartidaProyecto, SIGNAL(activated()), this, SLOT(slotMenuSociasPartidaProyecto()));
		pMenuSociasPartidaProyecto->addTo(pMenuSocias);
	}
/*>>>>>SOCIASMODULE_INITMAINWINDOW_MENUS*/

#ifdef HAVE_PAGOSMODULE
    pMainWindow->findAction( "MenuPagosCobro" )->addTo( pMenuSocias );
    pMainWindow->findAction( "MenuPagosRemesaCobro" )->addTo( pMenuSocias );
#endif


    pMenuSociasProyecto->setAccel( QKeySequence("CTRL+SHIFT+P") );
    pMenuSociasMiembro->setAccel( QKeySequence("CTRL+SHIFT+M") );
    {
        QString text = QString::fromUtf8( _("Generar recibos...") );
        pMenuGenerarRecibos = new QAction(text, 0, pMainWindow, 0);
        pMenuGenerarRecibos->setStatusTip(text);
        pMenuGenerarRecibos->setWhatsThis(text);
        pMainWindow->connect(pMenuGenerarRecibos, SIGNAL(activated()), this, SLOT(slotMenuGenerarRecibos()));
        pMenuGenerarRecibos->addTo(pMenuSocias);
    }
    QAction *menuContactosMailing = pMainWindow->findAction( "MenuContactosMailing" );
    pMainWindow->disconnect( menuContactosMailing, SIGNAL( activated() ), 0, 0 );
    pMainWindow->connect( menuContactosMailing,
                          SIGNAL( activated() ), this,
                          SLOT( slotMenuSociasFrmMailing() ) );
    return true;
}


void SociasModule::slotMenuSociasFrmMailing()
{
    FrmMailing *frmmailing = new FrmMailing(0);
    frmmailing->show();
    pMainWindow->createClient( frmmailing );
}

dbRecord *SociasModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    /*<<<<<SOCIASMODULE_CREATE_RECORD*/
	if( tablename.upper() == "PROYECTO" )
		return new RecProyecto(getConnection(), recid, user);
	if( tablename.upper() == "MIEMBRO" )
		return new RecMiembro(getConnection(), recid, user);
	if( tablename.upper() == "TIPOSOCIA" )
		return new RecTipoSocia(getConnection(), recid, user);
	if( tablename.upper() == "PARTIDAPROYECTO" )
		return new RecPartidaProyecto(getConnection(), recid, user);
	if( tablename.upper() == "SOCIASESTADO" )
		return new RecNamesListTable("SOCIASESTADO", getConnection(), recid, user);
	if( tablename.upper() == "PERIODICIDAD" )
		return new RecNamesListTable("PERIODICIDAD", getConnection(), recid, user);
/*>>>>>SOCIASMODULE_CREATE_RECORD*/
#ifdef HAVE_CONTABMODULE
    if( tablename.upper() == "APUNTE" )
        return new RecApunte(getConnection(), recid, user);
#endif
    return 0;
}

FrmEditRec *SociasModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<SOCIASMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "PROYECTO" )
		return new FrmEditProyecto(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "MIEMBRO" )
		return new FrmEditMiembro(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TIPOSOCIA" )
		return new FrmEditTipoSocia(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PARTIDAPROYECTO" )
		return new FrmEditPartidaProyecto(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "SOCIASESTADO" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PERIODICIDAD" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>SOCIASMODULE_CREATE_EDITFORM*/
    return 0;
}

FrmEditRecDetail *SociasModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<SOCIASMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>SOCIASMODULE_CREATE_EDITFORM_DETAIL*/
#ifdef HAVE_CONTABMODULE
    if( tablename.upper() == "APUNTE" )
        return new FrmEditApunte(frmmaster, ndetail,
                                 rec, dettablename, dm, editmode, editflags, parent, name, fl);
#endif
    return 0;
}

/*<<<<<SOCIASMODULE_SLOT_SOCIASPROYECTO*/
void SociasModule::slotMenuSociasProyecto()
{
	pMainWindow->slotMenuEditRecMaestro( "PROYECTO" );
}
/*>>>>>SOCIASMODULE_SLOT_SOCIASPROYECTO*/
/*<<<<<SOCIASMODULE_SLOT_SOCIASMIEMBRO*/
void SociasModule::slotMenuSociasMiembro()
{
	pMainWindow->slotMenuEditRecMaestro( "MIEMBRO" );
}
/*>>>>>SOCIASMODULE_SLOT_SOCIASMIEMBRO*/
/*<<<<<SOCIASMODULE_SLOT_SOCIASTIPOSOCIA*/
void SociasModule::slotMenuSociasTipoSocia()
{
	pMainWindow->slotMenuEditRecMaestro( "TIPOSOCIA" );
}
/*>>>>>SOCIASMODULE_SLOT_SOCIASTIPOSOCIA*/
/*<<<<<SOCIASMODULE_SLOT_SOCIASPARTIDAPROYECTO*/
void SociasModule::slotMenuSociasPartidaProyecto()
{
	pMainWindow->slotMenuEditRecMaestro( "PARTIDAPROYECTO" );
}
/*>>>>>SOCIASMODULE_SLOT_SOCIASPARTIDAPROYECTO*/


void SociasModule::slotMenuGenerarRecibos()
{
    FrmGenerarRecibos *frmgenrec= new FrmGenerarRecibos();
    frmgenrec->show();
    pMainWindow->createClient( frmgenrec );
}

bool SociasModule::hasContabilidad() const
{
#ifdef HAVE_CONTABMODULE
    return getContabModule();
#else
    return false;
#endif
}

/*<<<<<SOCIASMODULE_FIN*/
} // namespace socias
} // namespace gong

/*>>>>>SOCIASMODULE_FIN*/

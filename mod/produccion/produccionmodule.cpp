/*<<<<<COPYLEFT*/
/** @file produccionmodule.cpp Module produccion
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Module produccion
// Record Lote FrmEditRecMaster Facturacion
// Record LoteDet FrmEditRecDetail
// Record ArticuloComponente FrmEditRecDetail
// TYPE GongModule produccion::ProduccionModule
/*>>>>>MODULE_INFO*/

/*<<<<<PRODUCCIONMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "produccionmodule.h"
#include "produccionfrmeditlote.h"
#include "produccionfrmeditlotedet.h"
#include "produccionfrmeditarticulocomponente.h"
/*>>>>>PRODUCCIONMODULE_INCLUDES*/
#include "produccionfrmeditarticulobehavior.h"
#include <factumodule.h>

namespace gong {
namespace produccion {

ProduccionModule *ModuleInstance = 0;

ProduccionModule::ProduccionModule()
	: dbModule("produccion")
{
	ModuleInstance = this;
    _GONG_DEBUG_TRACE(1);
/*<<<<<PRODUCCIONMODULE_PUBLIC_INFO*/
//	mModuleRequires
	mMasterTables << "LOTE";
	mDetailTables << "LOTEDET" << "ARTICULOCOMPONENTE";
/*>>>>>PRODUCCIONMODULE_PUBLIC_INFO*/
}


ProduccionModule::~ProduccionModule()
{
	_GONG_DEBUG_TRACE(1);
}


bool ProduccionModule::login(FrmLogin *frmlogin, const Xtring& version,
							 Xtring& addTitle, bool startingapp)
{
	return true;
}

bool ProduccionModule::initDatabase(dbDefinition *db)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT( db );
	pMainDatabase = db;

    pFicLote = new factu::MasterTable( *pMainDatabase, "LOTE" );
    pFicLote->addFieldRecordID();
	pFicLote->addFieldStringCode( "CODIGO" );
	pFicLote->addFieldDesc( "DESCRIPCION" );
    pFicLote->addFieldFloat( "CANTIDAD" );
	pFicLote->addFieldDate( "FECHAFABRICACION" );
	pFicLote->addFieldDate( "FECHACADUCIDAD" );
    pFicLote->addFieldNotas();
    pFicLote->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicLote->addRelationDefinition( dbRelationDefinition::one2many, "LOTE", "ID", "LOTEDET", "LOTE_ID" );
    pMainDatabase->addTable( pFicLote->getTableDefinition() );

	factu::MasterTable *fmt= new factu::MasterTable( db->findTableDefinition( "ARTICULO" ) );
    fmt->addFieldOne2OneRelation( "LOTE_ID", "LOTE.ID", true );
	fmt->addFieldBool( "ESPRODUCTO" );
    fmt->addRelationDefinition( dbRelationDefinition::one2many, "ARTICULO", "ID", "ARTICULOCOMPONENTE", "PRODUCTO_ID" );
	delete fmt;

	pFicLoteDet = new factu::MasterTable( *pMainDatabase, "LOTEDET" );
    pFicLoteDet->addFieldRecordID();
    pFicLoteDet->addFieldReferenceID( "LOTE_ID", "LOTE.ID" );
    pFicLoteDet->addFieldFloat( "CANTIDAD" );
    pFicLoteDet->addFieldOne2OneRelation( "ARTICULO_ID", "ARTICULO.ID", false);
    pFicLoteDet->addFieldNotas();
    pMainDatabase->addTable( pFicLoteDet->getTableDefinition() );

    pFicArticuloComponente = new factu::MasterTable( *pMainDatabase, "ARTICULOCOMPONENTE" );
    pFicArticuloComponente->addFieldRecordID();
    pFicArticuloComponente->addFieldReferenceID( "PRODUCTO_ID", "ARTICULO.ID");
	pFicArticuloComponente->addFieldInt( "ORDEN" );
    pFicArticuloComponente->addFieldFloat( "CANTIDAD" );
    pFicArticuloComponente->addFieldOne2OneRelation( "ARTICULO_ID", "ARTICULO.ID", false);
	pFicArticuloComponente->addFieldEuro4Dec( "COSTE" );
	pFicArticuloComponente->addFieldEuro4Dec( "IMPORTE" );
    pFicArticuloComponente->addFieldNotas();
    pFicArticuloComponente->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicArticuloComponente->getTableDefinition() );

	return true;
}

dbRecord *ProduccionModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
/*<<<<<PRODUCCIONMODULE_CREATE_RECORD*/
	if( tablename.upper() == "LOTE" )
		return new RecLote(getConnection(), recid, user);
	if( tablename.upper() == "LOTEDET" )
		return new RecLoteDet(getConnection(), recid, user);
	if( tablename.upper() == "ARTICULOCOMPONENTE" )
		return new RecArticuloComponente(getConnection(), recid, user);
/*>>>>>PRODUCCIONMODULE_CREATE_RECORD*/
	return 0;
}

void ProduccionModule::afterCreateEditForm(FrmEditRec* frm, dbRecord* rec)
{
	if( rec->getTableName() == "ARTICULO" ) {
		frm->addBehavior( new FrmEditArticuloBehavior( frm ) );
	}
}


FrmEditRec *ProduccionModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<PRODUCCIONMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "LOTE" )
		return new FrmEditLote(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>PRODUCCIONMODULE_CREATE_EDITFORM*/
	return 0;
}

FrmEditRecDetail *ProduccionModule::createEditDetailForm(
	FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<PRODUCCIONMODULE_CREATE_EDITFORM_DETAIL*/
	if( tablename.upper() == "LOTEDET" )
		return new FrmEditLoteDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "ARTICULOCOMPONENTE" )
		return new FrmEditArticuloComponente(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
/*>>>>>PRODUCCIONMODULE_CREATE_EDITFORM_DETAIL*/
	return 0;
}

/*<<<<<PRODUCCIONMODULE_SLOT_FACTURACIONLOTE*/
void ProduccionModule::slotMenuFacturacionLote()
{
	pMainWindow->slotMenuEditRecMaestro( "LOTE" );
}
/*>>>>>PRODUCCIONMODULE_SLOT_FACTURACIONLOTE*/

bool ProduccionModule::initMainWindow(MainWindow *mainwin)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT(mainwin);
	pMainWindow = mainwin;
	QMenu *pMenuFacturacion = factu::ModuleInstance->getMenuFacturacion();
/*<<<<<PRODUCCIONMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("LOTE")->getDescPlural();
		pMenuFacturacionLote = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuFacturacionLote->setObjectName( "MenuFacturacionLote" );
		pMenuFacturacionLote->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuFacturacionLote->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuFacturacionLote, SIGNAL(activated()), this, SLOT(slotMenuFacturacionLote()));
		pMenuFacturacionLote->addTo(pMenuFacturacion);
	}
/*>>>>>PRODUCCIONMODULE_INITMAINWINDOW_MENUS*/
	return true;
}
/*<<<<<PRODUCCIONMODULE_FIN*/
} // namespace produccion
} // namespace gong

/*>>>>>PRODUCCIONMODULE_FIN*/
/*<<<<<PRODUCCIONMODULE_INIT_DATABASE*/

/*>>>>>PRODUCCIONMODULE_INIT_DATABASE*/

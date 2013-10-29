/*<<<<<COPYLEFT*/
/** @file pagosmodule.cpp Module pagos
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
// COPYLEFT Module pagos
// RECORD FormaPago FrmEditRecMaster Empresa
// RECORD RemesaCobro FrmEditRecMaster Pagos
// RECORD Cobro FrmEditRecMaster Pagos
// RECORD Pago FrmEditRecMaster Pagos
// NAMESLISTTABLE TipoFormaPago
// NAMESLISTTABLE EstadoRecibo
// MODULE_REQUIRED Empresa
// MODULE_OPTIONAL Contab
// TYPE GongModule pagos::PagosModule
/*>>>>>MODULE_INFO*/

/*<<<<<PAGOSMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include <dbappnameslisttable.h>
#include <dbappfrmeditnameslisttable.h>
#include "pagosmodule.h"
#include "pagosfrmeditformapago.h"
#include "pagosfrmeditremesacobro.h"
#include "pagosfrmeditcobro.h"
#include "pagosfrmeditpago.h"
/*>>>>>PAGOSMODULE_INCLUDES*/
#include "pagosfrmpagarrecibo.h"

#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include <contabreccuenta.h>
#endif

namespace gong {
namespace pagos {

PagosModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::Int,
        "ESTADORECIBO.PENDIENTE",
        _("Código del estado de recibo PENDIENTE"),
        "1"
    },
    {
        dbModuleSetting::Int,
        "ESTADORECIBO.PAGADO",
        _("Código del estado de recibo PAGADO"),
        "1"
    },
    {
        dbModuleSetting::Bool,
        "USAR_REMESAS",
        _("Si se utilizarán remesas de recibos"),
        "false"
    },
    {dbModuleSetting::None}
};

Date PagosModule::sLastFechaPago(Date::currentDate());
Xtring PagosModule::sLastCuentaPago("57.");
Xtring PagosModule::sLastDocumentoPago;
Xtring PagosModule::sLastMonedaCodigo;

PagosModule::PagosModule()
    : dbModule("pagos")
{
    ModuleInstance = this;
    pModuleSettings = _settings;
    _GONG_DEBUG_TRACE(1);
    /*<<<<<PAGOSMODULE_PUBLIC_INFO*/
    mModuleRequires << "empresa";
    mMasterTables << "FORMAPAGO" << "REMESACOBRO" << "COBRO" << "PAGO" << "TIPOFORMAPAGO" << "ESTADORECIBO";
//	mDetailTables
    pEmpresaModule = static_cast< empresa::EmpresaModule * >(DBAPP->findModule( "Empresa" ));
#ifdef HAVE_CONTABMODULE
    pContabModule = static_cast< contab::ContabModule * >(DBAPP->findModule( "Contab" ));
#endif
    /*>>>>>PAGOSMODULE_PUBLIC_INFO*/
    empresa::ModuleInstance->addContadorTable("COBRO");
    empresa::ModuleInstance->addContadorTable("PAGO");
}


PagosModule::~PagosModule()
{
    _GONG_DEBUG_TRACE(1);
}

bool PagosModule::initDatabase(dbDefinition *db)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    /*<<<<<PAGOSMODULE_INIT_DATABASE*/
    pFicTipoFormaPago = new NamesListTable( *pMainDatabase, "TIPOFORMAPAGO" );
    pMainDatabase->addTable( pFicTipoFormaPago->getTableDefinition() );
    pFicEstadoRecibo = new NamesListTable( *pMainDatabase, "ESTADORECIBO" );
    pMainDatabase->addTable( pFicEstadoRecibo->getTableDefinition() );
    /*>>>>>PAGOSMODULE_INIT_DATABASE*/

    // La tabla FORMAPAGO es común a todas las empresas y ejercicios
    pFicFormaPago = new empresa::MasterTable( *pMainDatabase, "FORMAPAGO" );
    pFicFormaPago->addFieldRecordID();
    pFicFormaPago->addFieldCodigo()->setUnique( true );
    pFicFormaPago->addFieldDesc( "NOMBRE" )->setUnique( true );
    pFicFormaPago->addField<FldNamesListTable>( "TIPOFORMAPAGO" )->setInsertAllowed(false);
    pFicFormaPago->addFieldInt( "NUMPLAZOS" );
    pFicFormaPago->addFieldInt( "DIASENTREPLAZOS" );
    pFicFormaPago->addFieldInt( "DIASPRIMERPLAZO" );
    pFicFormaPago->addFieldPercent( "DTOENFACTURA" );
    pFicFormaPago->addFieldNotas();
#ifdef HAVE_CONTABMODULE
    pFicFormaPago->addField<contab::FldCuenta>( "SUBCUENTACOBRO" );
    pFicFormaPago->addField<contab::FldCuenta>( "SUBCUENTAPAGO" );
#endif
    pFicFormaPago->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicFormaPago->getTableDefinition() );

    pFicRemesaCobro = new pagos::MasterTable( *db, "REMESACOBRO" );
    pFicRemesaCobro->addFieldRecordID();
    pFicRemesaCobro->addFieldEmpresaID();
    pFicRemesaCobro->addFieldIntCode( "NUMERO" );
    pFicRemesaCobro->addFieldDesc( "DESCRIPCION" );
    pFicRemesaCobro->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID" )->setCanBeNull( true );
    pFicRemesaCobro->addFieldDate( "FECHAEMISION" );
    pFicRemesaCobro->addFieldDate( "FECHACARGO" );
    pFicRemesaCobro->addField<empresa::FldCuentaBanco>( "CUENTABANCOABONO" );
    pFicRemesaCobro->addFieldBool( "CERRADA" );
    pFicRemesaCobro->addFieldInt( "NRECIBOS" )->setReadOnly(true);
    pFicRemesaCobro->addFieldEuro( "IMPORTE" )->setReadOnly(true);
    pFicRemesaCobro->addFieldEuro( "RESTO" )->setReadOnly(true);
    pFicRemesaCobro->addFieldNotas();
    pFicRemesaCobro->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicRemesaCobro->getTableDefinition() );

    pFicCobro= new pagos::MasterTable( *db, "COBRO" );
    pFicCobro->addFieldRecordID();
    pFicCobro->addFieldEmpresaID();
    pFicCobro->addFieldOne2OneRelation( "REMESACOBRO_ID", "REMESACOBRO.ID" )->setCanBeNull( true );
    pFicCobro->addFieldString( "TABLAFACTURAS", 30 );
    pFicCobro->addFieldReferenceID( "FACTURA_ID", "FACTURA.ID" );
    pFicCobro->addFieldString( "TABLATERCEROS", 30 );
    pFicCobro->addFieldReferenceID( "TERCERO_ID", "TERCERO.ID" );
    pFicCobro->addFieldInt( "CONTADOR" );
    pFicCobro->addFieldIntCode( "NUMERO" )->setUnique( false );
    pFicCobro->addFieldString( "NUMEROAGRUPADO", 15 );
    pFicCobro->addFieldBool("AUTOMATICO");
    pFicCobro->addFieldString( "RAZONSOCIAL", 100 );
    pFicCobro->addFieldDesc( "DESCRIPCION" )->setCanBeNull(false);
    pFicCobro->addFieldDate( "FECHAEMISION" )->setCanBeNull(false);
    pFicCobro->addFieldDate( "FECHAVALOR" )->setCanBeNull(false);
    pFicCobro->addFieldDate( "VENCIMIENTO" )->setCanBeNull(false);
    pFicCobro->addFieldEuro( "IMPORTE" );
    pFicCobro->addFieldEuro( "RESTO" );
    pFicCobro->addField<FldNamesListTable>( "ESTADORECIBO" )->setInsertAllowed(false);
    pFicCobro->addFieldDate( "FECHAPAGO" );
    pFicCobro->addFieldString("DOCUMENTOPAGO", 50 );
    pFicCobro->addFieldOne2OneRelation( "MONEDA_ID", "MONEDA.ID" );
    pFicCobro->addFieldFloat( "RATIO_MONEDA" )->setDefaultValue( "1.0" );
#ifdef HAVE_CONTABMODULE
    pFicCobro->addFieldString( "CUENTAORIGEN", 20 );
    pFicCobro->addFieldOne2OneRelation( "CUENTAPAGO_ID", "CUENTA.ID" )->setCanBeNull( true );
    pFicCobro->addFieldReferenceID( "ASIENTO_CARGO_ID", "ASIENTO.ID" )->setCanBeNull( true );
    pFicCobro->addFieldReferenceID( "ASIENTO_PAGO_ID", "ASIENTO.ID" )->setCanBeNull( true );
#endif
    pFicCobro->addFieldNotas();
    pFicCobro->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicCobro->addMultipleIndex( "empresa_numero_unico",
                                 "EMPRESA_ID,NUMERO", true );
    pMainDatabase->addTable( pFicCobro->getTableDefinition() );


    pFicPago= new pagos::MasterTable( *db, "PAGO" );
    pFicPago->addFieldRecordID();
    pFicPago->addFieldEmpresaID();
    pFicPago->addFieldString( "TABLAFACTURAS", 30 );
    pFicPago->addFieldReferenceID( "FACTURA_ID", "FACTURA.ID" );
    pFicPago->addFieldString( "TABLATERCEROS", 30 );
    pFicPago->addFieldReferenceID( "TERCERO_ID", "TERCERO.ID" );
    pFicPago->addFieldInt( "CONTADOR" );
    pFicPago->addFieldIntCode( "NUMERO" )->setUnique( false );
    pFicPago->addFieldString( "NUMEROAGRUPADO", 15 );
    pFicPago->addFieldBool("AUTOMATICO");
    pFicPago->addFieldString( "RAZONSOCIAL", 100 );
    pFicPago->addFieldDesc( "DESCRIPCION" )->setCanBeNull(false);
    pFicPago->addFieldDate( "FECHAEMISION" )->setCanBeNull(false);
    pFicPago->addFieldDate( "FECHAVALOR" )->setCanBeNull(false);
    pFicPago->addFieldDate( "VENCIMIENTO" )->setCanBeNull(false);
    pFicPago->addFieldEuro( "IMPORTE" );
    pFicPago->addFieldEuro( "RESTO" );
    pFicPago->addField<FldNamesListTable>( "ESTADORECIBO" )->setInsertAllowed(false);
    pFicPago->addFieldDate( "FECHAPAGO" );
    pFicPago->addFieldString("DOCUMENTOPAGO", 50 );
    pFicPago->addFieldOne2OneRelation( "MONEDA_ID", "MONEDA.ID" );
    pFicPago->addFieldFloat( "RATIO_MONEDA" )->setDefaultValue( "1.0" );
#ifdef HAVE_CONTABMODULE
    pFicPago->addFieldString( "CUENTAORIGEN", 20 );
    pFicPago->addFieldOne2OneRelation( "CUENTAPAGO_ID", "CUENTA.ID" )->setCanBeNull( true );
    pFicPago->addFieldReferenceID( "ASIENTO_CARGO_ID", "ASIENTO.ID" )->setCanBeNull( true );
    pFicPago->addFieldReferenceID( "ASIENTO_PAGO_ID", "ASIENTO.ID" )->setCanBeNull( true );
#endif
    pFicPago->addFieldNotas();
    pFicPago->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicPago->addMultipleIndex( "empresa_numero_unico",
                                "EMPRESA_ID,NUMERO", true );
    pMainDatabase->addTable( pFicPago->getTableDefinition() );

    return true;
}

Xtring PagosModule::getMigrationSQL( uint old_version ) const
{
    Xtring ret;
    switch( old_version ) {
    }
    return ret;
}

void PagosModule::afterLoad()
{
    FldNamesListTable *fldtfp = static_cast<FldNamesListTable *>(
                                    DBAPP->getDatabase()->findFieldDefinition("FORMAPAGO.TIPOFORMAPAGO") );
    if( fldtfp )
        fldtfp->fill( *getConnection() );
    if( fldtfp->getListOfValues()->size() == 0 ) {
        getConnection()->exec( "INSERT INTO TIPOFORMAPAGO (CODIGO,NOMBRE) VALUES "
                               "(" + getConnection()->toSQL( RecFormaPago::Contado ) + ", 'Contado'),"
                               "(" + getConnection()->toSQL( RecFormaPago::GeneraRecibos ) + ", 'Genera recibos'),"
                               "(" + getConnection()->toSQL( RecFormaPago::Pendiente ) + ", 'Pendiente'),"
                               "(" + getConnection()->toSQL( RecFormaPago::SeIgnora ) + ", 'Se ignora')" );
        fldtfp->fill( *getConnection() );
    }
    FldNamesListTable *flder = static_cast<FldNamesListTable *>(
                                   DBAPP->getDatabase()->findFieldDefinition("PAGO.ESTADORECIBO") );
    if( flder )
        flder->fill( *getConnection() );
    if( flder->getListOfValues()->size() == 0 ) {
        getConnection()->exec( "INSERT INTO ESTADORECIBO (CODIGO,NOMBRE) VALUES "
                               "(" + getConnection()->toSQL( PagosModule::ReciboPendiente ) + ", 'Pendiente'),"
                               "(" + getConnection()->toSQL( PagosModule::ReciboPagado) + ", 'Pagado'),"
                               "(" + getConnection()->toSQL( PagosModule::ReciboDevuelto ) + ", 'Devuelto'),"
                               "(" + getConnection()->toSQL( PagosModule::ReciboAnulado ) + ", 'Anulado')" );
        flder->fill( *getConnection() );
    }
    flder = static_cast<FldNamesListTable *>(
                DBAPP->getDatabase()->findFieldDefinition("COBRO.ESTADORECIBO") );
    if( flder )
        flder->fill( *getConnection() );
}

bool PagosModule::login(FrmLogin *frmlogin, const Xtring& version,
                        Xtring& addTitle, bool startingapp)
{
    return true;
}

/*<<<<<PAGOSMODULE_SLOT_EMPRESAFORMAPAGO*/
void PagosModule::slotMenuEmpresaFormaPago()
{
    pMainWindow->slotMenuEditRecMaestro( "FORMAPAGO" );
}
/*>>>>>PAGOSMODULE_SLOT_EMPRESAFORMAPAGO*/
/*<<<<<PAGOSMODULE_SLOT_VENTASCOBRO*/
void PagosModule::slotMenuVentasCobro()
{
    pMainWindow->slotMenuEditRecMaestro( "COBRO" );
}
/*>>>>>PAGOSMODULE_SLOT_VENTASCOBRO*/
/*<<<<<PAGOSMODULE_SLOT_COMPRASPAGO*/
void PagosModule::slotMenuComprasPago()
{
    pMainWindow->slotMenuEditRecMaestro( "PAGO" );
}
/*>>>>>PAGOSMODULE_SLOT_COMPRASPAGO*/
/*<<<<<PAGOSMODULE_SLOT_VENTASREMESACOBRO*/
void PagosModule::slotMenuVentasRemesaCobro()
{
    pMainWindow->slotMenuEditRecMaestro( "REMESACOBRO" );
}
/*>>>>>PAGOSMODULE_SLOT_VENTASREMESACOBRO*/


dbRecord *PagosModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    /*<<<<<PAGOSMODULE_CREATE_RECORD*/
    if( tablename.upper() == "FORMAPAGO" )
        return new RecFormaPago(getConnection(), recid, user);
    if( tablename.upper() == "REMESACOBRO" )
        return new RecRemesaCobro(getConnection(), recid, user);
    if( tablename.upper() == "COBRO" )
        return new RecCobro(getConnection(), recid, user);
    if( tablename.upper() == "PAGO" )
        return new RecPago(getConnection(), recid, user);
    if( tablename.upper() == "TIPOFORMAPAGO" )
        return new RecNamesListTable("TIPOFORMAPAGO", getConnection(), recid, user);
    if( tablename.upper() == "ESTADORECIBO" )
        return new RecNamesListTable("ESTADORECIBO", getConnection(), recid, user);
    /*>>>>>PAGOSMODULE_CREATE_RECORD*/
    return 0;
}
FrmEditRec *PagosModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
                                        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
                                        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<PAGOSMODULE_CREATE_EDITFORM*/
    if( tablename.upper() == "FORMAPAGO" )
        return new FrmEditFormaPago(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
    if( tablename.upper() == "REMESACOBRO" )
        return new FrmEditRemesaCobro(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
    if( tablename.upper() == "COBRO" )
        return new FrmEditCobro(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
    if( tablename.upper() == "PAGO" )
        return new FrmEditPago(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
    if( tablename.upper() == "TIPOFORMAPAGO" )
        return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
    if( tablename.upper() == "ESTADORECIBO" )
        return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
    /*>>>>>PAGOSMODULE_CREATE_EDITFORM*/
    return 0;
}

FrmEditRecDetail *PagosModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<PAGOSMODULE_CREATE_EDITFORM_DETAIL*/

    /*>>>>>PAGOSMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

bool PagosModule::initMainWindow(MainWindow *mainwin)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT(mainwin);
    pMainWindow = mainwin;
    QMenu *pMenuEmpresa = pMainWindow->findMenu( "MenuEmpresa" );
    /*<<<<<PAGOSMODULE_INITMAINWINDOW_MENUS*/
    {
        Xtring caption = DBAPP->getDatabase()->findTableDefinition("FORMAPAGO")->getDescPlural();
        pMenuEmpresaFormaPago = new QAction( toGUI( caption ) + "...", pMainWindow );
        pMenuEmpresaFormaPago->setObjectName( "MenuEmpresaFormaPago" );
        pMenuEmpresaFormaPago->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
        pMenuEmpresaFormaPago->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
        pMainWindow->connect(pMenuEmpresaFormaPago, SIGNAL(activated()), this, SLOT(slotMenuEmpresaFormaPago()));
        pMenuEmpresaFormaPago->addTo(pMenuEmpresa);
    }
    {
        Xtring caption = DBAPP->getDatabase()->findTableDefinition("REMESACOBRO")->getDescPlural();
        pMenuVentasRemesaCobro = new QAction( toGUI( caption ) + "...", pMainWindow );
        pMenuVentasRemesaCobro->setObjectName( "MenuPagosRemesaCobro" );
        pMenuVentasRemesaCobro->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
        pMenuVentasRemesaCobro->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
        pMainWindow->connect(pMenuVentasRemesaCobro, SIGNAL(activated()), this, SLOT(slotMenuVentasRemesaCobro()));
    }
    {
        Xtring caption = DBAPP->getDatabase()->findTableDefinition("COBRO")->getDescPlural();
        pMenuVentasCobro = new QAction( toGUI( caption ) + "...", pMainWindow );
        pMenuVentasCobro->setObjectName( "MenuPagosCobro" );
        pMenuVentasCobro->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
        pMenuVentasCobro->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
        pMainWindow->connect(pMenuVentasCobro, SIGNAL(activated()), this, SLOT(slotMenuVentasCobro()));
    }
    {
        Xtring caption = DBAPP->getDatabase()->findTableDefinition("PAGO")->getDescPlural();
        pMenuComprasPago = new QAction( toGUI( caption ) + "...", pMainWindow );
        pMenuComprasPago->setObjectName( "MenuPagosPago" );
        pMenuComprasPago->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
        pMenuComprasPago->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
        pMainWindow->connect(pMenuComprasPago, SIGNAL(activated()), this, SLOT(slotMenuComprasPago()));
    }
    /*>>>>>PAGOSMODULE_INITMAINWINDOW_MENUS*/
// {capel} eliminar:  pMenuVentasCobro->addTo(pMenuVentas); (esos menús son de factu::) y con las compras igual
    return true;
}

empresa::RecProyecto *PagosModule::getRecProyectoPadre(const FrmEditRec *childfrm) const
{
    while( childfrm ) {
        if( childfrm->getRecord()->getTableName() == "PROYECTO" )
            return static_cast<empresa::RecProyecto*>(childfrm->getRecord());
        childfrm = childfrm->getParentForm();
    }
    return 0;
}

RecRemesaCobro *PagosModule::getRecRemesaCobroPadre(const FrmEditRec *childfrm) const
{
    while( childfrm ) {
        if( childfrm->getRecord()->getTableName() == "REMESACOBRO" )
            return static_cast<RecRemesaCobro*>(childfrm->getRecord());
        childfrm = childfrm->getParentForm();
    }
    return 0;
}


/*<<<<<PAGOSMODULE_FIN*/
} // namespace pagos
} // namespace gong

/*>>>>>PAGOSMODULE_FIN*/

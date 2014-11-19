
/*<<<<<MODULE_INFO*/
// COPYLEFT Módulo de facturación de gonglib
// RECORD Articulo FrmEditRecMaster Facturacion
// RECORD Cliente FrmEditRecMaster Facturacion
// RECORD Proveedora FrmEditRecMaster Facturacion
// RECORD Agente FrmEditRecMaster Facturacion
// RECORD Familia FrmEditRecMaster Facturacion
// RECORD TipoDoc FrmEditRecMaster Facturacion
// RECORD PresupuestoVenta FrmEditRecMaster Ventas
// RECORD PresupuestoVentaDet FrmEditRecDetail
// RECORD PedidoVenta FrmEditRecMaster Ventas
// RECORD PedidoVentaDet FrmEditRecDetail
// RECORD AlbaranVenta FrmEditRecMaster Ventas
// RECORD AlbaranVentaDet FrmEditRecDetail
// RECORD FacturaVenta FrmEditRecMaster Ventas
// RECORD FacturaVentaDet FrmEditRecDetail
// RECORD PedidoCompra FrmEditRecMaster Compras
// RECORD PedidoCompraDet FrmEditRecDetail
// RECORD AlbaranCompra FrmEditRecMaster Compras
// RECORD AlbaranCompraDet FrmEditRecDetail
// RECORD FacturaCompra FrmEditRecMaster Compras
// RECORD FacturaCompraDet FrmEditRecDetail
// NAMESLISTTABLE TipoCliente
// NAMESLISTTABLE EstadoPedido
// MODULE_REQUIRED Empresa
// MODULE_OPTIONAL Tesoreria
// MODULE_REQUIRED Pagos
// MODULE_OPTIONAL Contab
// TYPE GongModule factu::FactuModule
/*>>>>>MODULE_INFO*/

/*<<<<<FACTUMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include <dbappnameslisttable.h>
#include <dbappfrmeditnameslisttable.h>
#include "factumodule.h"
#include "factufrmeditarticulo.h"
#include "factufrmeditcliente.h"
#include "factufrmeditproveedora.h"
#include "factufrmeditagente.h"
#include "factufrmeditfamilia.h"
#include "factufrmedittipodoc.h"
#include "factufrmeditpresupuestoventa.h"
#include "factufrmeditpresupuestoventadet.h"
#include "factufrmeditpedidoventa.h"
#include "factufrmeditpedidoventadet.h"
#include "factufrmeditalbaranventa.h"
#include "factufrmeditalbaranventadet.h"
#include "factufrmeditfacturaventa.h"
#include "factufrmeditfacturaventadet.h"
#include "factufrmeditpedidocompra.h"
#include "factufrmeditpedidocompradet.h"
#include "factufrmeditalbarancompra.h"
#include "factufrmeditalbarancompradet.h"
#include "factufrmeditfacturacompra.h"
#include "factufrmeditfacturacompradet.h"
/*>>>>>FACTUMODULE_INCLUDES*/
#include "facturecarticuloimagen.h"
#include <contactosfldnombrepropio.h>
#include <gongdbfieldstyle.h>
#include <empresamodule.h>
#include "factufrmeditempresabehavior.h"
#include "factufrmestadcompraventa.h"
#include "factufrmgenpedidocompra.h"
#include "factufrmcopiardocumento.h"
#include "factufrmbalanceclipro.h"
#include "factufrmcambiosmasivosarticulos.h"
#include "factufrmedittipodoc.h"
#include "factufrmgencodart.h"

#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include <contabcuenta.h>
#include <contabrecapunte.h>
#include <contabrecasiento.h>
#include "factufrmregenasientos.h"
#endif
#ifdef HAVE_PAGOSMODULE
#include <pagosmodule.h>
#endif
#ifdef HAVE_TESORERIAMODULE
#include "tesoreriaactnombrebehavior.h"
#endif

namespace gong {
namespace factu {

FactuModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::String,
        "IMGARTFILENAME",
        _("Plantilla del nombre del fichero de las imágenes de los artículos"),
        "%s.jpg",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Bool,
        "FILTRARARTICULOSPORPROVEEDORA",
        _("Mostrar solo artículos de la proveedora al dar de alta albaranes, facturas, ..."),
        "false",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Long,
        "CODIGO_PROVEEDORA_GENERICA",
        _("Código de la proveedora genérica (utilizada en los filtros)"),
        "0",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::String,
        "ARTICULO_DESCUENTOS",
        _("Código de los artículos para descuentos"),
        "DESCUENTO",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::String,
        "ARTICULO_RECARGOS",
        _("Código de los artículos para recargos"),
        "RECARGO",
        dbModuleSetting::All
    },
    {dbModuleSetting::None}
};

FactuModule::FactuModule()
    : dbModule( "factu" ), mWorkingDate( Date::currentDate() )
{
    _GONG_DEBUG_TRACE( 1 );
    ModuleInstance = this;
    pModuleSettings = _settings;
    pEmpresaModule = static_cast<empresa::EmpresaModule *>( DBAPP->findModule( "empresa" ) );
    _GONG_DEBUG_ASSERT( pEmpresaModule );
#ifdef HAVE_CONTABMODULE
    pContabModule = static_cast<contab::ContabModule *>( DBAPP->findModule( "contab" ) );
#endif
    mDescription = "Módulo de facturación";
    /*<<<<<FACTUMODULE_PUBLIC_INFO*/
	mModuleRequires << "empresa" << "pagos";
	mMasterTables << "ARTICULO" << "CLIENTE" << "PROVEEDORA" << "AGENTE" << "FAMILIA" << "TIPODOC" << "PRESUPUESTOVENTA" << "PEDIDOVENTA" << "ALBARANVENTA" << "FACTURAVENTA" << "PEDIDOCOMPRA" << "ALBARANCOMPRA" << "FACTURACOMPRA" << "TIPOCLIENTE" << "ESTADOPEDIDO";
	mDetailTables << "PRESUPUESTOVENTADET" << "PEDIDOVENTADET" << "ALBARANVENTADET" << "FACTURAVENTADET" << "PEDIDOCOMPRADET" << "ALBARANCOMPRADET" << "FACTURACOMPRADET";
	pEmpresaModule = static_cast< empresa::EmpresaModule * >(DBAPP->findModule( "Empresa" ));
	pPagosModule = static_cast< pagos::PagosModule * >(DBAPP->findModule( "Pagos" ));
#ifdef HAVE_TESORERIAMODULE
	pTesoreriaModule = static_cast< tesoreria::TesoreriaModule * >(DBAPP->findModule( "Tesoreria" ));
#endif
#ifdef HAVE_CONTABMODULE
	pContabModule = static_cast< contab::ContabModule * >(DBAPP->findModule( "Contab" ));
#endif
/*>>>>>FACTUMODULE_PUBLIC_INFO*/
    mMasterTables << "ARTICULOIMAGEN";
    mInsertables << "PRESUPUESTOVENTA" << "PEDIDOVENTA" << "ALBARANVENTA" << "FACTURAVENTA"
                 << "PEDIDOCOMPRA" << "ALBARANCOMPRA" << "FACTURACOMPRA";
    empresa::ModuleInstance->addContadorTable("FACTURACOMPRA");
    empresa::ModuleInstance->addContadorTable("ALBARANCOMPRA");
}

FactuModule::~FactuModule()
{
    _GONG_DEBUG_TRACE( 1 );
}

bool FactuModule::initDatabase( dbDefinition *db )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    /*<<<<<FACTUMODULE_INIT_DATABASE*/
	pFicTipoCliente = new NamesListTable( *pMainDatabase, "TIPOCLIENTE" );
	pMainDatabase->addTable( pFicTipoCliente->getTableDefinition() );
	pFicEstadoPedido = new NamesListTable( *pMainDatabase, "ESTADOPEDIDO" );
	pMainDatabase->addTable( pFicEstadoPedido->getTableDefinition() );
/*>>>>>FACTUMODULE_INIT_DATABASE*/
// 	pFicDetalleIVA = new NamesListTable( *pMainDatabase, "DETALLEIVA" );
// 	pMainDatabase->addTable( pFicDetalleIVA->getTableDefinition() );

    // Añadir información de facturación a la tabla empresa
    empresa::MasterTable *emt = new empresa::MasterTable( db->findTableDefinition( "EMPRESA" ) );
    emt->addFieldBool( "RECARGOEQUIVALENCIA" );
    delete emt;

    // Las familias son comunes a las empresas y ejercicios
    pFicFamilia = new factu::MasterTable( *pMainDatabase, "FAMILIA" );
    pFicFamilia->addFieldRecordID();
    pFicFamilia->addFieldCodigo()->setUnique( true );
    pFicFamilia->addFieldDesc( "NOMBRE" )->setUnique( true );
    pFicFamilia->addFieldOne2OneRelation( "TIPOIVA_ID", "TIPOIVA.ID", false );
#ifdef HAVE_CONTABMODULE
    if ( pContabModule ) {
        pFicFamilia->addField<contab::FldCuenta>( "SUBCUENTACOMPRAS" );
        pFicFamilia->addField<contab::FldCuenta>( "SUBCUENTAVENTAS" );
    }
#endif
    pFicFamilia->addFieldBool( "ENUSO" )->setDefaultValue("1");
    pFicFamilia->addFieldNotas();
    pFicFamilia->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicFamilia->getTableDefinition() );


    // Las agentes son comunes a las empresas y ejercicios
    pFicAgente = new factu::MasterTable( *pMainDatabase, "AGENTE" );
    pFicAgente->addFieldRecordID();
    pFicAgente->addFieldCodigo()->setUnique( true );
    pFicAgente->addField<contactos::FldNombrePropio>( "RAZONSOCIAL" )->setIsDescription(true)->setUnique( true );
    pFicAgente->addFieldAggregateRelation( "CONTACTO_ID", "CONTACTO.ID", true );
#ifdef HAVE_CONTABMODULE
    if ( pContabModule )
        pFicAgente->addField<contab::FldCuenta>( "SUBCUENTA" );
#endif
    pFicAgente->addFieldNotas();
    pFicAgente->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicAgente->getTableDefinition() );

    // Las clientes son comunes a las empresas y ejercicios
    pFicCliente = new factu::MasterTable( *pMainDatabase, "CLIENTE" );
    pFicCliente->addFieldRecordID();
    pFicCliente->addFieldCodigo()->setUnique( true );
    pFicCliente->addField<contactos::FldNombrePropio>( "RAZONSOCIAL" )->setIsDescription(true)->setUnique( true );
    pFicCliente->addFieldString("NOMBREALT", 150)->setCanBeNull( true );
    pFicCliente->addField<FldNamesListTable>( "TIPOCLIENTE" )->setInsertAllowed(true);
    pFicCliente->addFieldAggregateRelation( "CONTACTO_ID", "CONTACTO.ID", false );
    pFicCliente->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicCliente->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
    pFicCliente->addFieldPercent( "DTOP100" );
    pFicCliente->addField<FldIVADetallado>( "IVADETALLADO" );
    pFicCliente->addFieldBool( "ACTIVO" )->setDefaultValue( "1" );
    pFicCliente->addFieldInt( "TARIFA" )->setDefaultValue( "0" );
    pFicCliente->addFieldString("ENTIDADBANCO", 80);
    pFicCliente->addField<empresa::FldCuentaBanco>( "CUENTABANCO" );
#ifdef HAVE_CONTABMODULE
    if ( pContabModule )
        pFicCliente->addField<contab::FldCuenta>( "SUBCUENTA" );
#endif
    pFicCliente->addFieldNotas();
    pFicCliente->addBehavior( DBAPP->getRecordTimestampBehavior() );
    // Limite de crédito
    // Tarifa
    // Descuento
    pMainDatabase->addTable( pFicCliente->getTableDefinition() );

    // Las proveedoras son comunes a las empresas y ejercicios
    pFicProveedora = new factu::MasterTable( *pMainDatabase, "PROVEEDORA" );
    pFicProveedora->addFieldRecordID();
    pFicProveedora->addFieldCodigo()->setUnique( true );
    pFicProveedora->addField<contactos::FldNombrePropio>( "RAZONSOCIAL" )->setIsDescription(true)->setUnique( true );
    pFicProveedora->addFieldString("NOMBREALT", 150)->setCanBeNull( true );;
    pFicProveedora->addFieldAggregateRelation( "CONTACTO_ID", "CONTACTO.ID", false );
    pFicProveedora->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicProveedora->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
    pFicProveedora->addFieldString("ENTIDADBANCO", 80);
    pFicProveedora->addField<empresa::FldCuentaBanco>( "CUENTABANCO" );
    pFicProveedora->addField<FldIVADetallado>( "IVADETALLADO" );
    pFicProveedora->addFieldListOfValues<int>( false, &RecArticulo::sModalidades,
            &IdentityIntList, "GENCODARTICULO" )->setCanBeNull(true);
    pFicProveedora->addFieldString( "FORMATCODARTICULO", 100 );
    pFicProveedora->addFieldBool( "USAREFERENCIAS" )->setDefaultValue("0");
#ifdef HAVE_CONTABMODULE
    if ( pContabModule )
        pFicProveedora->addField<contab::FldCuenta>( "SUBCUENTA" );
#endif
    pFicProveedora->addFieldNotas();
    pFicProveedora->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicProveedora->getTableDefinition() );

    // Los artículos son comunes a las empresas y ejercicios
    pFicArticulo = new factu::MasterTable( *pMainDatabase, "ARTICULO" );
    pFicArticulo->addFieldRecordID();
    pFicArticulo->addFieldStringCode( "CODIGO", 255 )->setUnique( true );
    pFicArticulo->addFieldDesc( "NOMBRE", 255 )->setUnique( true );
    pFicArticulo->addFieldOne2OneRelation( "FAMILIA_ID", "FAMILIA.ID", false );
    pFicArticulo->addFieldOne2OneRelation( "TIPOIVA_ID", "TIPOIVA.ID", false );
    pFicArticulo->addFieldOne2OneRelation( "PROVEEDORA_ID", "PROVEEDORA.ID", true );
    pFicArticulo->addFieldString( "CODIGOINTERNO", 255 );
    pFicArticulo->addFieldString( "FABRICANTE", 255 );
    pFicArticulo->addFieldEuro4Dec( "COSTESINIVA" );
    pFicArticulo->addFieldEuro( "COSTE" );
    pFicArticulo->addFieldPercent( "MARGENCOMERCIAL" );
    pFicArticulo->addFieldEuro4Dec( "PVPSINIVA" );
    pFicArticulo->addFieldEuro( "PVP" );
    pFicArticulo->addFieldEuro( "PVP1" );
    pFicArticulo->addFieldEuro( "PVP2" );
    pFicArticulo->addFieldEuro( "PVP3" );
    pFicArticulo->addFieldPercent( "DTOCOMERCIAL" );
    pFicArticulo->addFieldBool( "USARSTOCKS" )->setDefaultValue("1");
    pFicArticulo->addFieldFloat( "STOCK" );
    pFicArticulo->addFieldFloat( "STOCKINICIAL" );
    pFicArticulo->addFieldFloat( "STOCKMINIMO" );
    pFicArticulo->addFieldFloat( "STOCKMAXIMO" );
    // Estos campos podrían ir más bien a un módulo de grupos de consumo
    pFicArticulo->addFieldBool( "ENUSO" )->setDefaultValue("1");
    pFicArticulo->addFieldBool( "OFERTA" );
    pFicArticulo->addFieldBool( "NOVEDAD" );
    pFicArticulo->addFieldText( "DESCRIPCION" );
    pFicArticulo->addFieldReferenceID( "ARTICULOBASE_ID", "ARTICULO.ID");
    pFicArticulo->addFieldFloat( "MULTIPLOBASE" );
#ifdef HAVE_CONTABMODULE
    if ( pContabModule )
        pFicArticulo->addField<contab::FldCuenta>( "SUBCUENTA" );
#endif
    pFicArticulo->addFieldAggregateRelation( "ARTICULOIMAGEN_ID", "ARTICULOIMAGEN.ID", true );
    pFicArticulo->addFieldNotas();
    pFicArticulo->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicArticulo->getTableDefinition() );

    pFicArticuloImagen = new factu::MasterTable( *pMainDatabase, "ARTICULOIMAGEN" );
    pFicArticuloImagen->addFieldRecordID();
    pFicArticuloImagen->addField<dbFieldImage>( "IMAGEN" );
    pFicArticuloImagen->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicArticuloImagen->getTableDefinition() );


    // Los tipos de factura comunes a las empresas y ejercicios
    pFicTipoDoc = new factu::MasterTable( *pMainDatabase, "TIPODOC" );
    pFicTipoDoc->addFieldRecordID();
    pFicTipoDoc->addFieldCodigo()->setUnique( false );
    pFicTipoDoc->addFieldDesc( "NOMBRE", 30 );
    pFicTipoDoc->addFieldString( "SERIE", 20);
    pFicTipoDoc->addFieldString( "FORMATO", 30);
    pFicTipoDoc->addFieldBool( "ACTUALIZASTOCKS" )->setDefaultValue( "1" );
    pFicTipoDoc->addFieldBool( "NOGENERAASIENTO" )->setDefaultValue( "0" );
    pFicTipoDoc->addFieldBool( "NOFACTURABLE" )->setDefaultValue( "0" );
    pFicTipoDoc->addFieldNotas();
    pFicTipoDoc->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicTipoDoc->addField<FldIVADetallado>( "IVADETALLADO" );
    pFicTipoDoc->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicTipoDoc->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
    pFicTipoDoc->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pMainDatabase->addTable( pFicTipoDoc->getTableDefinition() );

    // El resto de tablas son únicas para la empresa y para el ejercicio
    // Excepto los presupuestos de venta que son comunes a todos los ejercicios
    // VENTAS
    pFicPresupuestoVenta = new factu::MasterTable( *pMainDatabase, "PRESUPUESTOVENTA" );
    pFicPresupuestoVenta->addFieldRecordID();
    pFicPresupuestoVenta->addFieldEmpresaID();
    pFicPresupuestoVenta->addFieldInt( "CONTADOR" );
    pFicPresupuestoVenta->addFieldStringCode( "NUMERO", 30 )->setUnique( true );
    pFicPresupuestoVenta->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
    pFicPresupuestoVenta->addFieldFecha();
    pFicPresupuestoVenta->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID", false );
    pFicPresupuestoVenta->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicPresupuestoVenta->addField<FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicPresupuestoVenta->addFieldsTotalesMaster( false, false ); // No añadir entrega a cuentas ni otros pagos
    pFicPresupuestoVenta->addFieldsDesgloseIVA();
    pFicPresupuestoVenta->addFieldNotas();
    pFicPresupuestoVenta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicPresupuestoVenta->addRelationDefinition( dbRelationDefinition::one2many, "PRESUPUESTOVENTA", "ID", "PRESUPUESTOVENTADET", "PRESUPUESTOVENTA_ID" );
    pFicPresupuestoVenta->addMultipleIndex( "presupuestoventa_numero_unico", "EMPRESA_ID,NUMERO", true );
    pMainDatabase->addTable( pFicPresupuestoVenta->getTableDefinition() );

    pFicPresupuestoVentaDet = new factu::MasterTable( *pMainDatabase, "PRESUPUESTOVENTADET" );
    pFicPresupuestoVentaDet->addFieldRecordID();
    pFicPresupuestoVentaDet->addFieldReferenceID( "PRESUPUESTOVENTA_ID", "PRESUPUESTOVENTA.ID" );
    pFicPresupuestoVentaDet->addFieldsComunesDetalles( false /*ventas*/ );
    pFicPresupuestoVentaDet->addFieldNotas();
    pMainDatabase->addTable( pFicPresupuestoVentaDet->getTableDefinition() );


    pFicPedidoVenta = new factu::MasterTable( *pMainDatabase, "PEDIDOVENTA" );
    pFicPedidoVenta->addFieldRecordID();
    pFicPedidoVenta->addFieldEmpresaID();
    pFicPedidoVenta->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicPedidoVenta->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
    pFicPedidoVenta->addFieldInt( "CONTADOR" );
    pFicPedidoVenta->addFieldStringCode( "NUMERO", 30 )->setUnique(false);
    pFicPedidoVenta->addFieldFecha();
    pFicPedidoVenta->addField<FldNamesListTable>( "ESTADOPEDIDO" )->setInsertAllowed(false);
    pFicPedidoVenta->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID", false );
    pFicPedidoVenta->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicPedidoVenta->addField<FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicPedidoVenta->addFieldsTotalesMaster(false, false); // No añadir entrega a cuentas ni otros pagos
    pFicPedidoVenta->addFieldsDesgloseIVA();
#ifdef HAVE_CONTABMODULE
    if ( pContabModule ) {
        pFicPedidoVenta->addFieldReferenceID( "ASIENTO_ID", "ASIENTO.ID" );
    }
#endif
    pFicPedidoVenta->addFieldString("REFERENCIA", 40 );
    pFicPedidoVenta->addFieldNotas();
    pFicPedidoVenta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicPedidoVenta->addRelationDefinition( dbRelationDefinition::one2many, "PEDIDOVENTA", "ID", "PEDIDOVENTADET", "PEDIDOVENTA_ID" );
    pFicPedidoVenta->addMultipleIndex( "pedidoventa_numero_unico", "EMPRESA_ID,EJERCICIO,NUMERO", true );
    pMainDatabase->addTable( pFicPedidoVenta->getTableDefinition() );

    pFicPedidoVentaDet = new factu::MasterTable( *pMainDatabase, "PEDIDOVENTADET" );
    pFicPedidoVentaDet->addFieldRecordID();
    pFicPedidoVentaDet->addFieldReferenceID( "PEDIDOVENTA_ID", "PEDIDOVENTA.ID" );
    pFicPedidoVentaDet->addFieldsComunesDetalles( false /*ventas*/ );
    pFicPedidoVentaDet->addFieldNotas();
    pMainDatabase->addTable( pFicPedidoVentaDet->getTableDefinition() );


    pFicAlbaranVenta = new factu::MasterTable( *pMainDatabase, "ALBARANVENTA" );
    pFicAlbaranVenta->addFieldRecordID();
    pFicAlbaranVenta->addFieldEmpresaID();
    pFicAlbaranVenta->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicAlbaranVenta->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
    pFicAlbaranVenta->addFieldInt( "CONTADOR" );
    pFicAlbaranVenta->addFieldStringCode( "NUMERO", 30 )->setUnique( false );
    pFicAlbaranVenta->addFieldFecha();
    pFicAlbaranVenta->addFieldBool( "FACTURADO" )->setDefaultValue( "0" );
    pFicAlbaranVenta->addFieldBool( "NOFACTURABLE" )->setDefaultValue( "0" );
    pFicAlbaranVenta->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID", false );
    pFicAlbaranVenta->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicAlbaranVenta->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
    pFicAlbaranVenta->addField<FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicAlbaranVenta->addFieldsTotalesMaster(true, true); // Añadir entrega a cuentas y otros pagos
    pFicAlbaranVenta->addFieldsDesgloseIVA();
    pFicAlbaranVenta->addFieldEuro( "COBROS" );
#ifdef HAVE_CONTABMODULE
    if ( pContabModule ) {
        pFicAlbaranVenta->addFieldReferenceID( "ASIENTO_ID", "ASIENTO.ID" );
    }
#endif
    pFicAlbaranVenta->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicAlbaranVenta->addFieldNotas();
    pFicAlbaranVenta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicAlbaranVenta->addRelationDefinition( dbRelationDefinition::one2many, "ALBARANVENTA", "ID", "ALBARANVENTADET", "ALBARANVENTA_ID" );
    pFicAlbaranVenta->addMultipleIndex( "albaranventa_numero_unico", "EMPRESA_ID,EJERCICIO,NUMERO", true );
    pMainDatabase->addTable( pFicAlbaranVenta->getTableDefinition() );

    pFicAlbaranVentaDet = new factu::MasterTable( *pMainDatabase, "ALBARANVENTADET" );
    pFicAlbaranVentaDet->addFieldRecordID();
    pFicAlbaranVentaDet->addFieldReferenceID( "ALBARANVENTA_ID", "ALBARANVENTA.ID" );
    pFicAlbaranVentaDet->addFieldsComunesDetalles( false /*ventas*/ );
    pFicAlbaranVentaDet->addFieldNotas();
    pMainDatabase->addTable( pFicAlbaranVentaDet->getTableDefinition() );


    pFicFacturaVenta = new factu::MasterTable( *pMainDatabase, "FACTURAVENTA" );
    pFicFacturaVenta->addFieldRecordID();
    pFicFacturaVenta->addFieldEmpresaID();
    pFicFacturaVenta->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicFacturaVenta->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
    pFicFacturaVenta->addFieldInt( "CONTADOR" );
    pFicFacturaVenta->addFieldStringCode( "NUMERO", 30 )->setUnique( false );
    pFicFacturaVenta->addFieldFecha();
    pFicFacturaVenta->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID", false );
    pFicFacturaVenta->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicFacturaVenta->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
    pFicFacturaVenta->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicFacturaVenta->addField<FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicFacturaVenta->addFieldsTotalesMaster(true, true); // Añadir entrega a cuentas y otros pagos;
    pFicFacturaVenta->addFieldEuro( "ENTREGAALBARANES" );
    pFicFacturaVenta->addFieldEuro( "COBROS" );
#ifdef HAVE_CONTABMODULE
    pFicFacturaVenta->addFieldReferenceID( "ASIENTO_ID", "ASIENTO.ID" );
#endif
    pFicFacturaVenta->addFieldsDesgloseIVA();
    pFicFacturaVenta->addFieldNotas();
    pFicFacturaVenta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicFacturaVenta->addRelationDefinition( dbRelationDefinition::one2many, "FACTURAVENTA", "ID", "FACTURAVENTADET", "FACTURAVENTA_ID" );
    pFicFacturaVenta->addMultipleIndex( "facturaventa_numero_unico", "EMPRESA_ID,EJERCICIO,NUMERO", true );
    pMainDatabase->addTable( pFicFacturaVenta->getTableDefinition() );

    pFicFacturaVentaDet = new factu::MasterTable( *pMainDatabase, "FACTURAVENTADET" );
    pFicFacturaVentaDet->addFieldRecordID();
    pFicFacturaVentaDet->addFieldReferenceID( "FACTURAVENTA_ID", "FACTURAVENTA.ID" );
    pFicFacturaVentaDet->addFieldReferenceID( "ALBARANVENTA_ID", "ALBARANVENTA.ID" );
    pFicFacturaVentaDet->addFieldsComunesDetalles( false /*ventas*/ );
    pFicFacturaVentaDet->addFieldNotas();
    pMainDatabase->addTable( pFicFacturaVentaDet->getTableDefinition() );


    // COMPRAS

    pFicPedidoCompra = new factu::MasterTable( *pMainDatabase, "PEDIDOCOMPRA" );
    pFicPedidoCompra->addFieldRecordID();
    pFicPedidoCompra->addFieldEmpresaID();
    pFicPedidoCompra->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicPedidoCompra->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
    pFicPedidoCompra->addFieldInt( "CONTADOR" );
    pFicPedidoCompra->addFieldStringCode( "NUMERO", 30 )->setUnique( false );
    pFicPedidoCompra->addFieldFecha();
    pFicPedidoCompra->addField<FldNamesListTable>( "ESTADOPEDIDO" )->setInsertAllowed(false);
    pFicPedidoCompra->addFieldOne2OneRelation( "PROVEEDORA_ID", "PROVEEDORA.ID", false );
    pFicPedidoCompra->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicPedidoCompra->addField<FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicPedidoCompra->addFieldsTotalesMaster(false, false); // No añadir entrega a cuentas ni otros pagos
    pFicPedidoCompra->addFieldsDesgloseIVA();
#ifdef HAVE_CONTABMODULE
    if ( pContabModule ) {
        pFicPedidoCompra->addFieldReferenceID( "ASIENTO_ID", "ASIENTO.ID" );
    }
#endif
    pFicPedidoCompra->addFieldString("REFERENCIA", 40 );
    pFicPedidoCompra->addFieldNotas();
    pFicPedidoCompra->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicPedidoCompra->addRelationDefinition( dbRelationDefinition::one2many, "PEDIDOCOMPRA", "ID", "PEDIDOCOMPRADET", "PEDIDOCOMPRA_ID" );
    pFicPedidoCompra->addMultipleIndex( "pedidocompra_proveedora_numero_unico", "EMPRESA_ID,EJERCICIO,PROVEEDORA_ID,NUMERO", true );
    pMainDatabase->addTable( pFicPedidoCompra->getTableDefinition() );

    pFicPedidoCompraDet = new factu::MasterTable( *pMainDatabase, "PEDIDOCOMPRADET" );
    pFicPedidoCompraDet->addFieldRecordID();
    pFicPedidoCompraDet->addFieldReferenceID( "PEDIDOCOMPRA_ID", "PEDIDOCOMPRA.ID" );
    pFicPedidoCompraDet->addFieldsComunesDetalles( true /*compras*/ );
    pFicPedidoCompraDet->addFieldNotas();
    pMainDatabase->addTable( pFicPedidoCompraDet->getTableDefinition() );


    pFicAlbaranCompra = new factu::MasterTable( *pMainDatabase, "ALBARANCOMPRA" );
    pFicAlbaranCompra->addFieldRecordID();
    pFicAlbaranCompra->addFieldEmpresaID();
    pFicAlbaranCompra->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicAlbaranCompra->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
    pFicAlbaranCompra->addFieldInt( "CONTADOR" );
    pFicAlbaranCompra->addFieldStringCode( "NUMERO", 30 )->setUnique(false);
    pFicAlbaranCompra->addFieldFecha();
    pFicAlbaranCompra->addFieldBool( "FACTURADO" )->setDefaultValue( "0" );
    pFicAlbaranCompra->addFieldBool( "NOFACTURABLE" )->setDefaultValue( "0" );
    pFicAlbaranCompra->addFieldOne2OneRelation( "PROVEEDORA_ID", "PROVEEDORA.ID", false );
    pFicAlbaranCompra->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicAlbaranCompra->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
    pFicAlbaranCompra->addField<FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicAlbaranCompra->addFieldsTotalesMaster(true, true); // Añadir entrega a cuentas y otros pagos
    pFicAlbaranCompra->addFieldsDesgloseIVA();
    pFicAlbaranCompra->addFieldEuro( "PAGOS" );
#ifdef HAVE_CONTABMODULE
    if ( pContabModule ) {
        pFicAlbaranCompra->addFieldReferenceID( "ASIENTO_ID", "ASIENTO.ID" );
    }
#endif
#ifdef HAVE_TESORERIAMODULE
    if ( pTesoreriaModule ) {
        pFicAlbaranCompra->addFieldReferenceID( "APUNTE_ID", "APUNTETESORERIA.ID" );
    }
#endif
    pFicAlbaranCompra->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicAlbaranCompra->addFieldNotas();
    pFicAlbaranCompra->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicAlbaranCompra->addRelationDefinition( dbRelationDefinition::one2many, "ALBARANCOMPRA", "ID", "ALBARANCOMPRADET", "ALBARANCOMPRA_ID" );
    pFicAlbaranCompra->addMultipleIndex( "albarancompra_proveedora_numero_unico", "EMPRESA_ID,EJERCICIO,PROVEEDORA_ID,NUMERO", true );
    pMainDatabase->addTable( pFicAlbaranCompra->getTableDefinition() );

    pFicAlbaranCompraDet = new factu::MasterTable( *pMainDatabase, "ALBARANCOMPRADET" );
    pFicAlbaranCompraDet->addFieldRecordID();
    pFicAlbaranCompraDet->addFieldReferenceID( "ALBARANCOMPRA_ID", "ALBARANCOMPRA.ID" );
    pFicAlbaranCompraDet->addFieldsComunesDetalles( true /*compras*/ );
    pFicAlbaranCompraDet->addFieldNotas();
    pMainDatabase->addTable( pFicAlbaranCompraDet->getTableDefinition() );


    pFicFacturaCompra = new factu::MasterTable( *pMainDatabase, "FACTURACOMPRA" );
    pFicFacturaCompra->addFieldRecordID();
    pFicFacturaCompra->addFieldEmpresaID();
    pFicFacturaCompra->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicFacturaCompra->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
    pFicFacturaCompra->addFieldInt( "CONTADOR" );
    pFicFacturaCompra->addFieldStringCode( "NUMERO", 30 )->setUnique(false);
    pFicFacturaCompra->addFieldFecha();
    pFicFacturaCompra->addFieldOne2OneRelation( "PROVEEDORA_ID", "PROVEEDORA.ID", false );
    pFicFacturaCompra->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicFacturaCompra->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
    pFicFacturaCompra->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicFacturaCompra->addField<FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicFacturaCompra->addFieldsTotalesMaster(true, true); // Añadir entrega a cuentas y otros pagos);
    pFicFacturaCompra->addFieldEuro( "ENTREGAALBARANES" );
    pFicFacturaCompra->addFieldEuro( "PAGOS" );
#ifdef HAVE_CONTABMODULE
    if ( pContabModule ) {
        pFicFacturaCompra->addFieldReferenceID( "ASIENTO_ID", "ASIENTO.ID" );
    }
#endif
    pFicFacturaCompra->addFieldsDesgloseIVA();

    pFicFacturaCompra->addFieldNotas();
    pFicFacturaCompra->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicFacturaCompra->addRelationDefinition( dbRelationDefinition::one2many, "FACTURACOMPRA", "ID", "FACTURACOMPRADET", "FACTURACOMPRA_ID" );
    pFicFacturaCompra->addMultipleIndex( "facturacompra_proveedora_numero_unico", "EMPRESA_ID,EJERCICIO,PROVEEDORA_ID,NUMERO", true );
    pMainDatabase->addTable( pFicFacturaCompra->getTableDefinition() );

    pFicFacturaCompraDet = new factu::MasterTable( *pMainDatabase, "FACTURACOMPRADET" );
    pFicFacturaCompraDet->addFieldRecordID();
    pFicFacturaCompraDet->addFieldReferenceID( "FACTURACOMPRA_ID", "FACTURACOMPRA.ID" );
    pFicFacturaCompraDet->addFieldReferenceID( "ALBARANCOMPRA_ID", "ALBARANCOMPRA.ID" );
    pFicFacturaCompraDet->addFieldsComunesDetalles( true /*compras*/ );
    pFicFacturaCompraDet->addFieldNotas();
    pMainDatabase->addTable( pFicFacturaCompraDet->getTableDefinition() );


#ifdef HAVE_TESORERIAMODULE
    pFicAgente->getTableDefinition()->addBehavior( new tesoreria::ActNombreBehavior("RAZONSOCIAL", "APUNTETESORERIA", "TERCERO", "TERCERO_ID") );
    pFicCliente->getTableDefinition()->addBehavior( new tesoreria::ActNombreBehavior("RAZONSOCIAL", "APUNTETESORERIA", "TERCERO", "TERCERO_ID") );
    pFicProveedora->getTableDefinition()->addBehavior( new tesoreria::ActNombreBehavior("RAZONSOCIAL", "APUNTETESORERIA", "TERCERO", "TERCERO_ID") );
    pFicArticulo->getTableDefinition()->addBehavior( new tesoreria::ActNombreBehavior("NOMBRE", "APUNTETESORERIA", "TERCERO", "TERCERO_ID") );
    pFicFamilia->getTableDefinition()->addBehavior( new tesoreria::ActNombreBehavior("NOMBRE", "APUNTETESORERIA", "TERCERO", "TERCERO_ID") );
#endif
    return true;
}

bool FactuModule::login( FrmLogin *frmlogin, const Xtring &version,
                         Xtring &addtitle, bool startingapp )
{
    return true;
}

void FactuModule::afterLoad()
{
    FldIVADetallado *fid = new FldIVADetallado( "foo", "bar" );
    fid->setValuesFromString( ModuleInstance->getModuleSetting( "IVADETALLADO.VALUES" ).toString() );
    delete fid;
#ifdef HAVE_TESORERIAMODULE
    if( pTesoreriaModule && pTesoreriaModule->isEnabled() ) {
        pTesoreriaModule->getTablasTerceros() << "PROVEEDORA" << "CLIENTE" << "AGENTE";
        pTesoreriaModule->getTablasConceptos() << "ARTICULO" << "FAMILIA";
        pTesoreriaModule->getTablasDocumentos() << "FACTURAVENTA" << "ALBARANVENTA"
                                                << "FACTURACOMPRA" << "ALBARANCOMPRA";
    }
#endif
}


Xtring FactuModule::getMigrationSQL( uint old_version ) const
{
    Xtring ret;
    if( old_version == 1 ) {
    }
    return ret;
}

void FactuModule::setWorkingDate( Date dt )
{
    if( !dt.isNull() && dt.isValid() )
        mWorkingDate = dt;
    else
        mWorkingDate = Date::currentDate();
}

Date FactuModule::getWorkingDate()
{
    if( ModuleInstance->getModuleSetting("USEWORKINGDATE") == true )
        return mWorkingDate;
    else
        return Date::currentDate();
}


void FactuModule::afterCreateEditForm( FrmEditRec *frm, dbRecord *rec )
{
    if( rec->getTableName() == "EMPRESA" ) {
        frm->addBehavior( new FrmEditEmpresaBehavior( frm ) );
    }
}

dbRecord *FactuModule::createRecord( const Xtring &tablename, dbRecordID recid, dbUser *user )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    /*<<<<<FACTUMODULE_CREATE_RECORD*/
	if( tablename.upper() == "ARTICULO" )
		return new RecArticulo(getConnection(), recid, user);
	if( tablename.upper() == "CLIENTE" )
		return new RecCliente(getConnection(), recid, user);
	if( tablename.upper() == "PROVEEDORA" )
		return new RecProveedora(getConnection(), recid, user);
	if( tablename.upper() == "AGENTE" )
		return new RecAgente(getConnection(), recid, user);
	if( tablename.upper() == "FAMILIA" )
		return new RecFamilia(getConnection(), recid, user);
	if( tablename.upper() == "TIPODOC" )
		return new RecTipoDoc(getConnection(), recid, user);
	if( tablename.upper() == "PRESUPUESTOVENTA" )
		return new RecPresupuestoVenta(getConnection(), recid, user);
	if( tablename.upper() == "PRESUPUESTOVENTADET" )
		return new RecPresupuestoVentaDet(getConnection(), recid, user);
	if( tablename.upper() == "PEDIDOVENTA" )
		return new RecPedidoVenta(getConnection(), recid, user);
	if( tablename.upper() == "PEDIDOVENTADET" )
		return new RecPedidoVentaDet(getConnection(), recid, user);
	if( tablename.upper() == "ALBARANVENTA" )
		return new RecAlbaranVenta(getConnection(), recid, user);
	if( tablename.upper() == "ALBARANVENTADET" )
		return new RecAlbaranVentaDet(getConnection(), recid, user);
	if( tablename.upper() == "FACTURAVENTA" )
		return new RecFacturaVenta(getConnection(), recid, user);
	if( tablename.upper() == "FACTURAVENTADET" )
		return new RecFacturaVentaDet(getConnection(), recid, user);
	if( tablename.upper() == "PEDIDOCOMPRA" )
		return new RecPedidoCompra(getConnection(), recid, user);
	if( tablename.upper() == "PEDIDOCOMPRADET" )
		return new RecPedidoCompraDet(getConnection(), recid, user);
	if( tablename.upper() == "ALBARANCOMPRA" )
		return new RecAlbaranCompra(getConnection(), recid, user);
	if( tablename.upper() == "ALBARANCOMPRADET" )
		return new RecAlbaranCompraDet(getConnection(), recid, user);
	if( tablename.upper() == "FACTURACOMPRA" )
		return new RecFacturaCompra(getConnection(), recid, user);
	if( tablename.upper() == "FACTURACOMPRADET" )
		return new RecFacturaCompraDet(getConnection(), recid, user);
	if( tablename.upper() == "TIPOCLIENTE" )
		return new RecNamesListTable("TIPOCLIENTE", getConnection(), recid, user);
	if( tablename.upper() == "ESTADOPEDIDO" )
		return new RecNamesListTable("ESTADOPEDIDO", getConnection(), recid, user);
/*>>>>>FACTUMODULE_CREATE_RECORD*/
    if( tablename.upper() == "ARTICULOIMAGEN" )
        return new RecArticuloImagen(getConnection(), recid, user);
    return 0;
}

FrmEditRec *FactuModule::createEditForm( FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        DataTable::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<FACTUMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "ARTICULO" )
		return new FrmEditArticulo(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "CLIENTE" )
		return new FrmEditCliente(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PROVEEDORA" )
		return new FrmEditProveedora(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "AGENTE" )
		return new FrmEditAgente(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "FAMILIA" )
		return new FrmEditFamilia(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TIPODOC" )
		return new FrmEditTipoDoc(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PRESUPUESTOVENTA" )
		return new FrmEditPresupuestoVenta(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PEDIDOVENTA" )
		return new FrmEditPedidoVenta(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "ALBARANVENTA" )
		return new FrmEditAlbaranVenta(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "FACTURAVENTA" )
		return new FrmEditFacturaVenta(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PEDIDOCOMPRA" )
		return new FrmEditPedidoCompra(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "ALBARANCOMPRA" )
		return new FrmEditAlbaranCompra(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "FACTURACOMPRA" )
		return new FrmEditFacturaCompra(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TIPOCLIENTE" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "ESTADOPEDIDO" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>FACTUMODULE_CREATE_EDITFORM*/
    return 0;
}

FrmEditRecDetail *FactuModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    DataTable::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<FACTUMODULE_CREATE_EDITFORM_DETAIL*/
	if( tablename.upper() == "PRESUPUESTOVENTADET" )
		return new FrmEditPresupuestoVentaDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PEDIDOVENTADET" )
		return new FrmEditPedidoVentaDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "ALBARANVENTADET" )
		return new FrmEditAlbaranVentaDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "FACTURAVENTADET" )
		return new FrmEditFacturaVentaDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PEDIDOCOMPRADET" )
		return new FrmEditPedidoCompraDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "ALBARANCOMPRADET" )
		return new FrmEditAlbaranCompraDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "FACTURACOMPRADET" )
		return new FrmEditFacturaCompraDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
/*>>>>>FACTUMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

/*<<<<<FACTUMODULE_SLOT_FACTURACIONFAMILIA*/
void FactuModule::slotMenuFacturacionFamilia()
{
	pMainWindow->slotMenuEditRecMaestro( "FAMILIA" );
}
/*>>>>>FACTUMODULE_SLOT_FACTURACIONFAMILIA*/
/*<<<<<FACTUMODULE_SLOT_FACTURACIONARTICULO*/
void FactuModule::slotMenuFacturacionArticulo()
{
	pMainWindow->slotMenuEditRecMaestro( "ARTICULO" );
}
/*>>>>>FACTUMODULE_SLOT_FACTURACIONARTICULO*/
/*<<<<<FACTUMODULE_SLOT_FACTURACIONAGENTE*/
void FactuModule::slotMenuFacturacionAgente()
{
	pMainWindow->slotMenuEditRecMaestro( "AGENTE" );
}
/*>>>>>FACTUMODULE_SLOT_FACTURACIONAGENTE*/

/*<<<<<FACTUMODULE_SLOT_FACTURACIONCLIENTE*/
void FactuModule::slotMenuFacturacionCliente()
{
	pMainWindow->slotMenuEditRecMaestro( "CLIENTE" );
}
/*>>>>>FACTUMODULE_SLOT_FACTURACIONCLIENTE*/
/*<<<<<FACTUMODULE_SLOT_FACTURACIONPROVEEDORA*/
void FactuModule::slotMenuFacturacionProveedora()
{
	pMainWindow->slotMenuEditRecMaestro( "PROVEEDORA" );
}
/*>>>>>FACTUMODULE_SLOT_FACTURACIONPROVEEDORA*/
/*<<<<<FACTUMODULE_SLOT_FACTURACIONTIPODOC*/
void FactuModule::slotMenuFacturacionTipoDoc()
{
	pMainWindow->slotMenuEditRecMaestro( "TIPODOC" );
}
/*>>>>>FACTUMODULE_SLOT_FACTURACIONTIPODOC*/
/*<<<<<FACTUMODULE_SLOT_VENTASPRESUPUESTOVENTA*/
void FactuModule::slotMenuVentasPresupuestoVenta()
{
	pMainWindow->slotMenuEditRecMaestro( "PRESUPUESTOVENTA" );
}
/*>>>>>FACTUMODULE_SLOT_VENTASPRESUPUESTOVENTA*/
/*<<<<<FACTUMODULE_SLOT_VENTASPEDIDOVENTA*/
void FactuModule::slotMenuVentasPedidoVenta()
{
	pMainWindow->slotMenuEditRecMaestro( "PEDIDOVENTA" );
}
/*>>>>>FACTUMODULE_SLOT_VENTASPEDIDOVENTA*/
/*<<<<<FACTUMODULE_SLOT_VENTASALBARANVENTA*/
void FactuModule::slotMenuVentasAlbaranVenta()
{
	pMainWindow->slotMenuEditRecMaestro( "ALBARANVENTA" );
}
/*>>>>>FACTUMODULE_SLOT_VENTASALBARANVENTA*/
/*<<<<<FACTUMODULE_SLOT_VENTASFACTURAVENTA*/
void FactuModule::slotMenuVentasFacturaVenta()
{
	pMainWindow->slotMenuEditRecMaestro( "FACTURAVENTA" );
}
/*>>>>>FACTUMODULE_SLOT_VENTASFACTURAVENTA*/
/*<<<<<FACTUMODULE_SLOT_COMPRASPEDIDOCOMPRA*/
void FactuModule::slotMenuComprasPedidoCompra()
{
	pMainWindow->slotMenuEditRecMaestro( "PEDIDOCOMPRA" );
}
/*>>>>>FACTUMODULE_SLOT_COMPRASPEDIDOCOMPRA*/
/*<<<<<FACTUMODULE_SLOT_COMPRASALBARANCOMPRA*/
void FactuModule::slotMenuComprasAlbaranCompra()
{
	pMainWindow->slotMenuEditRecMaestro( "ALBARANCOMPRA" );
}
/*>>>>>FACTUMODULE_SLOT_COMPRASALBARANCOMPRA*/
/*<<<<<FACTUMODULE_SLOT_COMPRASFACTURACOMPRA*/
void FactuModule::slotMenuComprasFacturaCompra()
{
	pMainWindow->slotMenuEditRecMaestro( "FACTURACOMPRA" );
}
/*>>>>>FACTUMODULE_SLOT_COMPRASFACTURACOMPRA*/

bool FactuModule::initMainWindow( MainWindow *mainwin )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( mainwin );
    pMainWindow = mainwin;
    pMenuFacturacion = new QMenu( pMainWindow );
    pMenuFacturacion->setObjectName( "MenuFacturacion" );
    pMainWindow->menuBar()->insertItem( toGUI( _( "&Facturación" ) ), pMenuFacturacion );

    pMenuCompras = new QMenu( toGUI( _("Compras") ) );
    pMenuCompras->setObjectName( "MenuFacturacionCompras" );
    pMenuVentas = new QMenu( toGUI( _("Ventas") ) );
    pMenuVentas->setObjectName( "MenuFacturacionVentas" );
    pMenuFacturacion->addMenu( pMenuCompras );
    pMenuFacturacion->addMenu( pMenuVentas );

    /*<<<<<FACTUMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("ARTICULO")->getDescPlural();
		pMenuFacturacionArticulo = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuFacturacionArticulo->setObjectName( "MenuFacturacionArticulo" );
		pMenuFacturacionArticulo->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuFacturacionArticulo->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuFacturacionArticulo, SIGNAL(activated()), this, SLOT(slotMenuFacturacionArticulo()));
		pMenuFacturacionArticulo->addTo(pMenuFacturacion);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("CLIENTE")->getDescPlural();
		pMenuFacturacionCliente = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuFacturacionCliente->setObjectName( "MenuFacturacionCliente" );
		pMenuFacturacionCliente->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuFacturacionCliente->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuFacturacionCliente, SIGNAL(activated()), this, SLOT(slotMenuFacturacionCliente()));
		pMenuFacturacionCliente->addTo(pMenuFacturacion);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("PROVEEDORA")->getDescPlural();
		pMenuFacturacionProveedora = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuFacturacionProveedora->setObjectName( "MenuFacturacionProveedora" );
		pMenuFacturacionProveedora->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuFacturacionProveedora->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuFacturacionProveedora, SIGNAL(activated()), this, SLOT(slotMenuFacturacionProveedora()));
		pMenuFacturacionProveedora->addTo(pMenuFacturacion);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("AGENTE")->getDescPlural();
		pMenuFacturacionAgente = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuFacturacionAgente->setObjectName( "MenuFacturacionAgente" );
		pMenuFacturacionAgente->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuFacturacionAgente->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuFacturacionAgente, SIGNAL(activated()), this, SLOT(slotMenuFacturacionAgente()));
		pMenuFacturacionAgente->addTo(pMenuFacturacion);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("FAMILIA")->getDescPlural();
		pMenuFacturacionFamilia = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuFacturacionFamilia->setObjectName( "MenuFacturacionFamilia" );
		pMenuFacturacionFamilia->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuFacturacionFamilia->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuFacturacionFamilia, SIGNAL(activated()), this, SLOT(slotMenuFacturacionFamilia()));
		pMenuFacturacionFamilia->addTo(pMenuFacturacion);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("TIPODOC")->getDescPlural();
		pMenuFacturacionTipoDoc = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuFacturacionTipoDoc->setObjectName( "MenuFacturacionTipoDoc" );
		pMenuFacturacionTipoDoc->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuFacturacionTipoDoc->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuFacturacionTipoDoc, SIGNAL(activated()), this, SLOT(slotMenuFacturacionTipoDoc()));
		pMenuFacturacionTipoDoc->addTo(pMenuFacturacion);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("PRESUPUESTOVENTA")->getDescPlural();
		pMenuVentasPresupuestoVenta = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuVentasPresupuestoVenta->setObjectName( "MenuVentasPresupuestoVenta" );
		pMenuVentasPresupuestoVenta->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuVentasPresupuestoVenta->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuVentasPresupuestoVenta, SIGNAL(activated()), this, SLOT(slotMenuVentasPresupuestoVenta()));
		pMenuVentasPresupuestoVenta->addTo(pMenuVentas);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("PEDIDOVENTA")->getDescPlural();
		pMenuVentasPedidoVenta = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuVentasPedidoVenta->setObjectName( "MenuVentasPedidoVenta" );
		pMenuVentasPedidoVenta->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuVentasPedidoVenta->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuVentasPedidoVenta, SIGNAL(activated()), this, SLOT(slotMenuVentasPedidoVenta()));
		pMenuVentasPedidoVenta->addTo(pMenuVentas);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("ALBARANVENTA")->getDescPlural();
		pMenuVentasAlbaranVenta = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuVentasAlbaranVenta->setObjectName( "MenuVentasAlbaranVenta" );
		pMenuVentasAlbaranVenta->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuVentasAlbaranVenta->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuVentasAlbaranVenta, SIGNAL(activated()), this, SLOT(slotMenuVentasAlbaranVenta()));
		pMenuVentasAlbaranVenta->addTo(pMenuVentas);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("FACTURAVENTA")->getDescPlural();
		pMenuVentasFacturaVenta = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuVentasFacturaVenta->setObjectName( "MenuVentasFacturaVenta" );
		pMenuVentasFacturaVenta->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuVentasFacturaVenta->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuVentasFacturaVenta, SIGNAL(activated()), this, SLOT(slotMenuVentasFacturaVenta()));
		pMenuVentasFacturaVenta->addTo(pMenuVentas);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("PEDIDOCOMPRA")->getDescPlural();
		pMenuComprasPedidoCompra = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuComprasPedidoCompra->setObjectName( "MenuComprasPedidoCompra" );
		pMenuComprasPedidoCompra->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuComprasPedidoCompra->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuComprasPedidoCompra, SIGNAL(activated()), this, SLOT(slotMenuComprasPedidoCompra()));
		pMenuComprasPedidoCompra->addTo(pMenuCompras);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("ALBARANCOMPRA")->getDescPlural();
		pMenuComprasAlbaranCompra = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuComprasAlbaranCompra->setObjectName( "MenuComprasAlbaranCompra" );
		pMenuComprasAlbaranCompra->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuComprasAlbaranCompra->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuComprasAlbaranCompra, SIGNAL(activated()), this, SLOT(slotMenuComprasAlbaranCompra()));
		pMenuComprasAlbaranCompra->addTo(pMenuCompras);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("FACTURACOMPRA")->getDescPlural();
		pMenuComprasFacturaCompra = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuComprasFacturaCompra->setObjectName( "MenuComprasFacturaCompra" );
		pMenuComprasFacturaCompra->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuComprasFacturaCompra->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuComprasFacturaCompra, SIGNAL(activated()), this, SLOT(slotMenuComprasFacturaCompra()));
		pMenuComprasFacturaCompra->addTo(pMenuCompras);
	}
/*>>>>>FACTUMODULE_INITMAINWINDOW_MENUS*/
    pMenuOperaciones = new QMenu( toGUI( _("Operaciones") ) );
    pMenuFacturacion->addMenu( pMenuOperaciones);
    pMenuFactuCambiosMasivosArticulos = new QAction( toGUI("Cambios masivos de artículos..."), 0, pMainWindow,0);
    pMenuFactuCambiosMasivosArticulos->setObjectName("ActionFactuCambiosMasivosArticulos");
    pMainWindow->connect(pMenuFactuCambiosMasivosArticulos, SIGNAL(activated()), this, SLOT(slotMenuFactuCambiosMasivosArticulos()));
    pMenuFactuCambiosMasivosArticulos->addTo(pMenuOperaciones);
    pMenuFactuCopiarDocumento = new QAction( toGUI("Copiar de otros ejercicios/empresas..."), 0, pMainWindow,0);
    pMainWindow->connect(pMenuFactuCopiarDocumento, SIGNAL(activated()), this, SLOT(slotMenuFactuCopiarDocumento()));
    pMenuFactuCopiarDocumento->addTo(pMenuOperaciones);
    pMenuFactuGenPedidosCompras = new QAction( toGUI("Generar pedidos de compras..."), 0, pMainWindow,0);
    pMainWindow->connect(pMenuFactuGenPedidosCompras, SIGNAL(activated()), this, SLOT(slotMenuFactuGenPedidosCompras()));
    pMenuFactuGenPedidosCompras->addTo(pMenuCompras);
#ifdef HAVE_CONTABMODULE
    pMenuFactuRegenAsientos = new QAction( toGUI("Regenerar asientos de contabilidad..."), 0, pMainWindow,0);
    pMainWindow->connect(pMenuFactuRegenAsientos, SIGNAL(activated()), this, SLOT(slotMenuFactuRegenAsientos()));
    pMenuFactuRegenAsientos->addTo(pMenuOperaciones);
#endif
    pMenuInformes = new QMenu( toGUI( _("Informes") ) );
    pMenuFacturacion->addMenu( pMenuInformes );
    pMenuFactuEstadCompraVenta = new QAction( toGUI("Estadísticas de compras y ventas..."), 0, pMainWindow,0);
    pMainWindow->connect(pMenuFactuEstadCompraVenta, SIGNAL(activated()), this, SLOT(slotMenuFactuEstadCompraVenta()));
    pMenuFactuEstadCompraVenta->addTo(pMenuInformes);
    pMenuFactuBalanceCliPro = new QAction( toGUI("Balances de clientes y proveedoras..."), 0, pMainWindow,0);
    pMainWindow->connect(pMenuFactuBalanceCliPro, SIGNAL(activated()), this, SLOT(slotMenuFactuBalanceCliPro()));
    pMenuFactuBalanceCliPro->addTo(pMenuInformes);

#ifdef HAVE_PAGOSMODULE
    pMainWindow->findAction( "MenuPagosCobro" )->addTo( pMenuVentas );
    if( pagos::ModuleInstance->getModuleSetting("USAR_REMESAS").toBool() )
        pMainWindow->findAction( "MenuPagosRemesaCobro" )->addTo( pMenuVentas );
    pMainWindow->findAction( "MenuPagosPago" )->addTo( pMenuCompras );
#endif

    return true;
}

void FactuModule::slotMenuFactuEstadCompraVenta()
{
    FrmEstadCompraVenta *frmstats = new FrmEstadCompraVenta();
    frmstats->show();
    pMainWindow->createClient ( frmstats );
}

void FactuModule::slotMenuFactuBalanceCliPro()
{
    FrmBalanceCliPro *frmbal = new FrmBalanceCliPro();
    frmbal->show();
    pMainWindow->createClient ( frmbal );
}

void FactuModule::slotMenuFactuCambiosMasivosArticulos()
{
    if( DBAPP->preventOperationIf( DBAPP->isReadOnly(),
                                   "Cambios masivos en artículos", "la base de datos está bloqueada" ) )
        return;
    FrmCambiosMasivosArticulos *frmcamb = new FrmCambiosMasivosArticulos ( 0, "CambiosMasivosArticulos" );
    frmcamb->show();
    pMainWindow->createClient ( frmcamb );
}

void FactuModule::slotMenuFactuGenPedidosCompras()
{
    if( DBAPP->preventOperationIf( DBAPP->isReadOnly(),
                                   "Generar pedidos de compras", "la base de datos está bloqueada" ) )
        return;
    FrmGenPedidoCompra *frmgenpedidocompra = new FrmGenPedidoCompra(0, "GenPedidoCompra");
    frmgenpedidocompra->show();
    pMainWindow->createClient( frmgenpedidocompra );
}

#ifdef HAVE_CONTABMODULE
void FactuModule::slotMenuFactuRegenAsientos()
{
    if( DBAPP->preventOperationIf( DBAPP->isReadOnly(),
                                   "Regenerar asientos", "la base de datos está bloqueada" ) )
        return;
    FrmRegenAsientos *frmregenasientos = new FrmRegenAsientos(0, "RegenAsientos");
    frmregenasientos->show();
    pMainWindow->createClient( frmregenasientos );
}
#else
void FactuModule::slotMenuFactuRegenAsientos()
{
}
#endif

void FactuModule::slotMenuFactuCopiarDocumento()
{
    if( DBAPP->preventOperationIf( DBAPP->isReadOnly(),
                                   "Copiar documentos", "la base de datos está bloqueada" ) )
        return;
    FrmCopiarDocumento *frmcopiardocumento = new FrmCopiarDocumento(0, "CopiarDocumento");
    frmcopiardocumento->show();
    pMainWindow->createClient( frmcopiardocumento );
}


int FactuModule::getMaxContador(const Xtring& table, dbRecordID empresa_id,
                                int ejercicio, const Xtring &serie) const
{
    Xtring sql = "SELECT MAX(CONTADOR) FROM " + table;
    if( !serie.isEmpty() )
        sql += " INNER JOIN TIPODOC ON " + table + ".TIPODOC_ID=TIPODOC.ID";
    Xtring cond;
    if( ejercicio != 0 )
        cond += "EJERCICIO=" + getConnection()->toSQL( ejercicio );
    if( empresa_id != 0 ) {
        if( !cond.isEmpty() )
            cond += " AND ";
        cond += "EMPRESA_ID=" + getConnection()->toSQL( empresa_id );
    }
    if( !serie.isEmpty() ) {
        if( !cond.isEmpty() )
            cond += " AND ";
        cond += "TIPODOC.SERIE=" + getConnection()->toSQL( serie );
    }
    if( !cond.isEmpty() )
        sql += " WHERE " + cond;
    int numero = getConnection()->selectInt(sql) + 1;
    return numero;
}

bool FactuModule::insertDetails(FrmEditRecMaster *masterform, FrmEditRecDetail *detailform,
                                const Xtring &sexcludedtables, const Xtring &sexcludedfields )
{
    XtringList excludedtables;
    sexcludedtables.tokenize( excludedtables, "," );
    dbRecord *dest = masterform->getRecord();
    dbRecordList *dest_detalles = detailform->getDetalles();

    XtringList tables, captions;
    for( XtringList::const_iterator it = mInsertables.begin();
            it != mInsertables.end(); ++it ) {
        if( !excludedtables.contains( *it ) ) {
            tables << *it;
            captions << DBAPP->getTableDescPlural( *it );
        }
    }
    int choosen = FrmBase::msgXtringList( detailform, _("Elige la tabla desde la que insertar"),
                                          captions );
    if( choosen == -1 )
        return false;
    Xtring tablename = tables[choosen];
    dbRecord *source = DBAPP->createRecord( tablename );
    source->removeFilter( source->getTableName()
                          + ".EJERCICIO=" + source->getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
    dbRecordID source_id = DBAPP->choose( detailform, source );
    if( !source_id )
        return false;
    DBAPP->waitCursor( true );
    LineEdit *le_tipodoc = 0;
    if( dest->isEmpty( "EMPRESA_ID,EJERCICIO,FECHA" ) ) {
        // No quiero copiar en profundidad para que no copie los detalles
        dest->copyRecord( source, false, Xtring::null, "ID,EMPRESA_ID,EJERCICIO,FECHA,NUMERO,CONTADOR" );
        le_tipodoc = static_cast<LineEdit *>(masterform->findControl( "TIPODOC.CODIGO" ) );
        if( le_tipodoc )
            le_tipodoc->setJustEdited(true);
    }
    XtringList det_properties;
    det_properties << "DETALLE";
    dbRecordRelationDict sourcerelations = source->findRelationsBySemanticProperties( det_properties );
    if( sourcerelations.size() == 0 ) {
        _GONG_DEBUG_WARNING( "Tabla " + source->getTableName() + " no tiene propiedades " + det_properties.join(",") );
        DBAPP->resetCursor();
        return false;
    }
    dbRecordRelationDict destrelations = dest->findRelationsBySemanticProperties( det_properties );
    if( destrelations.size() == 0 ) {
        DBAPP->resetCursor();
        _GONG_DEBUG_WARNING( "Tabla " + tablename + " no tiene propiedades " + det_properties.join(",") );
        return false;
    }
    dbRecordList *detalles = sourcerelations.seq_at(0)->getRelatedRecordList();
    bool had_something = false;
    unsigned int numrecords = dest_detalles->size();
    for( dbRecordList::const_iterator it = detalles->begin(); it != detalles->end(); ++ it ) {
        dbRecord *detalle_dest, *articulo;
        bool add = false;
        had_something = true;
        if( numrecords > 0 && dest_detalles->getRecord( numrecords - 1 )->isEmpty("ID,NLINEA") ) {
            detalle_dest = dest_detalles->getRecord( numrecords - 1 );
        } else {
            // No puedo usar dest_detalles porque es un dbRecordList que puede estar vacía
            detalle_dest = destrelations.seq_at(0)->getRelatedRecord(-1)->duplicate();
            add = true;
        }
        detalle_dest->copyRecord( *it, true, Xtring::null, "ID,NLINEA," + sexcludedfields  );
        detalle_dest->readRelated( true ); // No son de la misma tabla, así que no se copian las relaciones
        articulo = detalle_dest->findRelatedRecord( "ARTICULO_ID" );
        if( articulo ) {
            bool calc = false;
            if( dest->hasSemanticProperty( "COMPRA" ) && !source->hasSemanticProperty( "COMPRA" ) ) {
                detalle_dest->setValue( "COSTESINIVA", articulo->getValue( "COSTESINIVA") );
                detalle_dest->setValue( "COSTE", articulo->getValue( "COSTE") );
                calc = true;
            } else if( dest->hasSemanticProperty( "VENTA" ) && !source->hasSemanticProperty( "VENTA" ) ) {
                detalle_dest->setValue( "PVPSINIVA", articulo->getValue( "PVPSINIVA") );
                detalle_dest->setValue( "PVP", articulo->getValue( "PVP") );
                calc = true;
            }
            if( calc ) {
                calcImporteDetalle( detalle_dest,
                                    (source->getValue( "IVADETALLADO" ).toInt() == factu::FldIVADetallado::incluido) );
            }
        }
        if( add )
            dest_detalles->addRecord( detalle_dest );
    }
    if( had_something ) {
        masterform->scatterFields();
        masterform->updateFromDetails( detailform );
        masterform->setEdited(true);
        detailform->addDetailIfNeeded();
        if( le_tipodoc )
            le_tipodoc->setJustEdited(false);
    }
    delete source;
    DBAPP->resetCursor();
    return true;
}

/* TODO: Convertir en interface */
void FactuModule::calcImporteDetalle(dbRecord* detalle, bool redondeaimportes)
{
    double preciosiniva, precioconiva;
    double cantidad = detalle->getValue( "CANTIDAD" ).toDouble();
    double dtop100 = detalle->getValue( "DTOP100" ).toDouble();
    if( detalle->hasSemanticProperty( "COMPRA" ) ) {
        preciosiniva = detalle->getValue( "COSTESINIVA" ).toDouble();
        precioconiva = detalle->getValue( "COSTESINIVA" ).toDouble();
    } else {
        preciosiniva = detalle->getValue( "PVPSINIVA" ).toDouble();
        precioconiva = detalle->getValue( "PVP" ).toDouble();
    }
    int ndec = empresa::ModuleInstance->getDecimalesMoneda();
    if( !redondeaimportes )
        ndec += 2;
    Money importesiniva(cantidad * preciosiniva * ( 1 - dtop100 / 100 ), ndec);
    Money importeconiva = Money( cantidad * precioconiva * ( 1 - dtop100 / 100 ),
                                 empresa::ModuleInstance->getDecimalesMoneda() );
    detalle->setValue( "IMPORTE", importesiniva );
    detalle->setValue( "IMPORTECONIVA", importeconiva );
}

bool FactuModule::editPVPsArticulo(FrmBase *parentform,
                                   RecArticulo *articulo, RecCliente *cliente, double pvp, bool canedit)
{
    if( articulo->getRecordID() != 0 ) {
        // Puede ocurrir que el artículo estuviera en la lista de detalles y no esté actualizado
        articulo->read( articulo->getRecordID() );
        int tarifacliente = cliente->getValue("TARIFA").toInt();
        double pvpconiva = articulo->getPVP( tarifacliente );
        if( pvp != pvpconiva ) {
            if( tarifacliente == 0 ) {
                articulo->setValue("PVPSINIVA", articulo->getRecTipoIVA()->menosIVA(pvp));
                articulo->setValue("PVP", pvp );
                articulo->fixMargenYDescuento();
                bool showosd = false;
                if( !canedit || articulo->getValue("COSTESINIVA").toDouble() != 0.0 ) {
                    // Si el coste no se queda a cero, se puede grabar ...
                    showosd = articulo->save(false);
                } else {
                    // ... pero si no, se edita el artículo
                    showosd = DBAPP->editRecord(0, articulo, 0, DataTable::updating,
                                                dbApplication::simpleEdition, parentform);
                }
                if( showosd ) {
                    DBAPP->showOSD( parentform->getTitle(), Xtring::printf( _( "Se ha actualizado el PVP del artículo: sin IVA: %s, con IVA: %s" ),
                                    articulo->getValue( "PVPSINIVA" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
                                    articulo->getValue( "PVP" ).toMoney().toString( DBAPP->getRegConfig() ).c_str() ) );
                }
            } else {
                articulo->setPVP( tarifacliente, pvpconiva );
                if( articulo->save(false) ) {
                    DBAPP->showOSD( parentform->getTitle(), Xtring::printf( _( "Se ha actualizado el PVP del artículo PARA ESTE CLIENTE: %s" ),
                                    Money(pvp).toString( DBAPP->getRegConfig() ).c_str() ) );
                }
            }
            return true;
        } else {
            DBAPP->showOSD( parentform->getTitle(), _("El PVP del artículo no ha cambiado") );
        }
    }
    return false;
}

bool FactuModule::editCostesArticulo(FrmBase *parentform,
                                     RecArticulo *articulo, double costesiniva, bool canedit)
{
    if( articulo->getRecordID() != 0 ) {
        // Puede ocurrir que el artículo estuviera en la lista de detalles y no esté actualizado
        articulo->read( articulo->getRecordID() );
        if( costesiniva != articulo->getValue("COSTESINIVA").toDouble() ) {
            articulo->setValue("COSTESINIVA", costesiniva );
            articulo->setValue("COSTE", articulo->getRecTipoIVA()->masIVA(costesiniva) );
            articulo->fixMargenYDescuento();
            bool showosd = false;
            if( !canedit || articulo->getValue("PVPSINIVA").toDouble() != 0.0 ) {
                // Si el pvp no se queda a cero, se puede grabar ...
                showosd = articulo->save(false);
            } else {
                // ... pero si no, se edita el artículo
                showosd = DBAPP->editRecord(0, articulo, 0, DataTable::updating,
                                            dbApplication::simpleEdition, parentform);
            }
            if( showosd ) {
                DBAPP->showStickyOSD( parentform->getTitle(), Xtring::printf( _( "Se ha actualizado el coste del artículo: sin IVA: %s, con IVA: %s" ),
                                      articulo->getValue( "COSTESINIVA" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
                                      articulo->getValue( "COSTE" ).toMoney().toString( DBAPP->getRegConfig() ).c_str() ) );
            }
            return true;
        } else {
            DBAPP->showOSD( parentform->getTitle(), _("El coste del artículo no ha cambiado") );
        }
    }
    return false;
}

/*<<<<<FACTUMODULE_FIN*/
} // namespace factu
} // namespace gong

/*>>>>>FACTUMODULE_FIN*/

#include "config.h"
#ifdef HAVE_CONTABMODULE
#include <empresarectipoiva.h>
#include <contabmodule.h>
#include "facturectipodoc.h"
#include "facturecalbarancompra.h"
#include "facturecalbaranventa.h"
#include "facturecfacturacompra.h"
#include "facturecfacturaventa.h"
#include "factuiasentables.h"

namespace gong {
namespace factu {

IAsentableFactura::IAsentableFactura(dbRecord* record, IAsentableFactura::Tipo tipo, const Xtring& asiento_id_field)
    : contab::IAsentableRecord( record, asiento_id_field ), mTipo( tipo )
{
}

dbRecord* IAsentableFactura::getRecTercero() const
{
    if( mTipo == albarancompra || mTipo == facturacompra )
        return pRecord->findRelatedRecord("PROVEEDORA_ID");
    else
        return pRecord->findRelatedRecord("CLIENTE_ID");
}


pagos::RecFormaPago* IAsentableFactura::getRecFormaPago() const
{
    return static_cast<pagos::RecFormaPago*>(pRecord->findRelatedRecord("FORMAPAGO_ID"));
}

contab::RecAsiento* IAsentableFactura::creaAsiento(contab::RecAsiento* old_asiento, bool supervisar)
{
    dbRecordID asiento_id = 0;
    contab::RecAsiento *asiento	= 0;
    switch( mTipo ) {
    case albarancompra:
    case albaranventa:
    {
        RecTipoDoc *tipodoc = static_cast<RecTipoDoc *>(pRecord->findRelatedRecord("TIPODOC_ID"));
        if( tipodoc->generaAsientos() ) {
            if( pRecord->getValue( "NOFACTURABLE" ).toBool() == true ) {
                // No es facturable, generar un asiento normal, como de una factura
                if( mTipo == albarancompra ) {
                    RecAlbaranCompra *albarancompra = static_cast<RecAlbaranCompra *>(pRecord);
                    asiento = creaAsientoCompletoCompra( albarancompra->getListAlbaranCompraDet(),
                                                         old_asiento, supervisar );
                } else {
                    RecAlbaranVenta *albaranventa = static_cast<RecAlbaranVenta *>(pRecord);
                    asiento = creaAsientoCompletoVenta( albaranventa->getListAlbaranVentaDet(),
                                                        old_asiento, supervisar );
                }
            } else { // Es facturable, generar solo el asiento del pago
                if( pRecord->getValue( "ENTREGA" ).toDouble() != 0.0 )
                    asiento = creaAsientoSimple( old_asiento, supervisar );
            }
        }
    }
    break;
    case facturacompra:
    case facturaventa:
    {
        if( mTipo == facturacompra ) {
            RecFacturaCompra *facturacompra = static_cast<RecFacturaCompra *>(pRecord);
            asiento = creaAsientoCompletoCompra( facturacompra->getListFacturaCompraDet(),
                                                 old_asiento, supervisar );
        } else {
            RecFacturaVenta *facturaventa = static_cast<RecFacturaVenta *>(pRecord);
            asiento = creaAsientoCompletoVenta( facturaventa->getListFacturaVentaDet(),
                                                old_asiento, supervisar );
        }
    }
    break;
    }
    if( asiento ) {
        DBAPP->showStickyOSD( pRecord->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                              Xtring::printf(_("Se ha generado el asiento nº %s"),
                                             asiento->getValue("NUMASIENTO").toString().c_str() ) );
        asiento_id = asiento->getRecordID();
    }
    pRecord->setValue( "ASIENTO_ID", asiento_id );
    return asiento;
}

contab::RecAsiento *IAsentableFactura::creaAsientoSimple( contab::RecAsiento* old_asiento,
        bool supervisar)
{
    if( pRecord->getValue("CUENTAPAGO_ID").toInt() == 0 )
        pRecord->setValue("CUENTAPAGO_ID",
                          contab::RecCuenta::findCuentaID(
                              pRecord->getConnection(),
                              empresa::ModuleInstance->getRecEmpresa()->getValue("CUENTACAJA").toString() ) );
    Xtring cuenta_anticipo;
    contab::RecAsiento *asiento = 0;
    Money anticipo = pRecord->getValue( "ENTREGA" ).toMoney();
    if( mTipo == facturaventa || mTipo == albaranventa ) {
        cuenta_anticipo = "438"; // TODO: campos en empresa
        Xtring descripcion = "Anticipo " + pRecord->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME );
        asiento = contab::ModuleInstance->creaAsientoSimple(
                      0, old_asiento, pRecord->getValue( "FECHA" ).toDate(),
                      pRecord->getIDSqlCond(), pRecord->getValue( "PROYECTO_ID" ).toInt(), descripcion,
                      pRecord->getValue("DOCUMENTOPAGO").toString(),
                      pRecord->getValue("CUENTAPAGO_ID").toInt(), anticipo,
                      contab::ModuleInstance->creaCuentaTercero(0, getRecTercero(),
                              cuenta_anticipo, supervisar )->getRecordID(),
                      anticipo, supervisar );
    } else {
        cuenta_anticipo = "407";
        Xtring descripcion = "Anticipo " + pRecord->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME );
        asiento = contab::ModuleInstance->creaAsientoSimple(
                      0, old_asiento, pRecord->getValue( "FECHA" ).toDate(),
                      pRecord->getIDSqlCond(), pRecord->getValue( "PROYECTO_ID" ).toInt(),
                      descripcion, pRecord->getValue("DOCUMENTOPAGO").toString(),
                      contab::ModuleInstance->creaCuentaTercero(0, getRecTercero(),
                              cuenta_anticipo, supervisar )->getRecordID(),
                      anticipo, pRecord->getValue("CUENTAPAGO_ID").toInt(), anticipo, supervisar );
    }
    return asiento;
}

contab::RecAsiento* IAsentableFactura::creaAsientoCompletoCompra(
    dbRecordList *detalles, contab::RecAsiento* old_asiento, bool supervisar)
{
    typedef contab::ContabModule::ApunteQuickValues ApunteValues;
    List<ApunteValues> apuntes;
    Dictionary<Money> mercaderias;
    Dictionary<Money> ivas;
    // Ver la cuenta de la proveedora
    Xtring cuentaprov = getRecTercero()->getValue("SUBCUENTA").toString();
    if( cuentaprov.isEmpty() ) {
        cuentaprov = "400." + getRecTercero()->getValue( "CODIGO" ).toString();
    }
    delete contab::RecCuenta::quickCreate( pRecord->getConnection(), cuentaprov,
                                           getRecTercero()->getValue( "RAZONSOCIAL" ).toString() );
    contab::Cuenta cuentaprov_contraida(cuentaprov, contab::ModuleInstance->getMaxNivelGrupo() );
    Xtring concepto;
    if( mTipo == facturacompra )
        concepto = "FacCom:" + pRecord->getValue("NUMERO").toString();
    else
        concepto = "AlbCom:" + pRecord->getValue("NUMERO").toString();
    for( dbRecordList::const_iterator it = detalles->begin(); it != detalles->end(); ++ it ) {
        dbRecord *factudet = *it;
        RecArticulo *recarticulo;
        empresa::RecTipoIVA *rectipoiva;
        if( mTipo == facturacompra ) {
            recarticulo = static_cast<RecFacturaCompraDet *>(*it)->getRecArticulo();
            rectipoiva = static_cast<RecFacturaCompraDet *>(*it)->getRecTipoIVA();
        } else {
            recarticulo = static_cast<RecAlbaranCompraDet *>(*it)->getRecArticulo();
            rectipoiva = static_cast<RecAlbaranCompraDet *>(*it)->getRecTipoIVA();
        }
        Xtring cuenta_mercaderias = recarticulo->getRecFamilia()->getValue( "SUBCUENTACOMPRAS" ).toString();
        if( cuenta_mercaderias.isEmpty() )
            cuenta_mercaderias = "600.";
        else
            delete contab::RecCuenta::quickCreate( factudet->getConnection(), cuenta_mercaderias,
                                                   recarticulo->getRecFamilia()->getValue( "NOMBRE" ).toString() );
        mercaderias.set( cuenta_mercaderias,
                         mercaderias[cuenta_mercaderias] + factudet->getValue("IMPORTE").toMoney() );
        Xtring cuenta_iva = rectipoiva->getValue( "SUBCUENTASOPORTADO" ).toString();
        if( cuenta_iva.isEmpty() )
            cuenta_iva= "472.";
        else
            contab::RecCuenta::quickCreate( factudet->getConnection(), cuenta_iva,
                                            rectipoiva->getValue( "NOMBRE" ).toString() );
        if( dynamic_cast<IIVADesglosable *>(pRecord)->hasRecargo() ) {
            ivas.set( cuenta_iva,
                      ivas[cuenta_iva] + rectipoiva->calcIVARecargo( factudet->getValue("IMPORTE").toMoney() ) );
        } else {
            ivas.set( cuenta_iva,
                      ivas[cuenta_iva] + rectipoiva->calcIVA( factudet->getValue("IMPORTE").toMoney() ) );
        }
    }
    for( Dictionary<Money>::const_iterator itmercad = mercaderias.begin();
            itmercad != mercaderias.end(); ++ itmercad ) {
        if( itmercad->second != 0.0 ) {
            ApunteValues mercaderia = { itmercad->first, "(" + cuentaprov_contraida + ") " + concepto,
                                        round(itmercad->second), 0.0
                                      };
            apuntes.push_back(mercaderia);
        }
    }
    for( Dictionary<Money>::const_iterator itivas = ivas.begin();
            itivas != ivas.end(); ++ itivas ) {
        if( itivas->second != 0.0 ) {
            ApunteValues iva = { itivas->first, "(" + cuentaprov_contraida + ") " + concepto,
                                 round(itivas->second), 0.0
                               };
            apuntes.push_back(iva);
        }
    }
    Money total = pRecord->getValue("TOTAL").toMoney();
    ApunteValues total_apunte_haber = { cuentaprov, concepto, 0.0, total };
    apuntes.push_back(total_apunte_haber);

    // Pagos
    // Decidir si la forma de pago hace que el resto se vaya a una cuenta especial
    Xtring cuentapago = getRecFormaPago()->getValue("SUBCUENTAPAGO").toString();
    if( cuentapago.endsWith(".")) {
        cuentapago += getRecTercero()->getValue("CODIGO").toString();
        contab::RecCuenta::quickCreate( pRecord->getConnection(), cuentapago,
                                        getRecTercero()->getValue( "RAZONSOCIAL" ).toString() );
    }
    Money entrega_albaranes = pRecord->getValue( "ENTREGAALBARANES" ).toMoney();
    Money entrega = pRecord->getValue( "ENTREGA" ).toMoney();
    // Si la forma de pago no tiene cuenta ni plazos y no hay pagos por caja, el asiento acaba aquí
    if( !cuentapago.isEmpty() && getRecFormaPago()->getValue("NUMPLAZOS").toInt() != 0 && total != 0.0 ) {
        ApunteValues total_apunte_debe = { cuentaprov, "Pago " + concepto, total, 0.0 };
        apuntes.push_back(total_apunte_debe);
    } else if( cuentapago.isEmpty() && ( entrega != 0 || entrega_albaranes != 0.0 ) ) {
        ApunteValues total_apunte_debe = { cuentaprov, "Pago " + concepto, entrega + entrega_albaranes, 0.0 };
        apuntes.push_back(total_apunte_debe);
    }
    if( entrega_albaranes != 0.0 ) {
        ApunteValues entrega_albaranes_apunte = {
            contab::ModuleInstance->creaCuentaTercero(0, getRecTercero(), "438", supervisar )->getValue( "CUENTA" ).toString(),
            "Pagos albaranes " + concepto, 0.0, entrega_albaranes
        };
        apuntes.push_back(entrega_albaranes_apunte);
    }
    if( entrega != 0.0 ) {
        ApunteValues entrega_apunte_debe = {
            empresa::ModuleInstance->getRecEmpresa()->getValue( "CUENTACAJA" ).toString(),
            "Pago " + concepto, 0.0, entrega
        };
        apuntes.push_back(entrega_apunte_debe);
    }
    if( !cuentapago.isEmpty() && (total - entrega - entrega_albaranes != 0.0) ) {
        ApunteValues total_apunte = { cuentapago, "Pago " + concepto, 0.0, total };
        apuntes.push_back(total_apunte);
    }
    contab::RecAsiento *asiento = contab::ModuleInstance->creaAsientoMultiple(
                                      0, old_asiento, pRecord->getValue("FECHA").toDate(),
                                      pRecord->getIDSqlCond(), pRecord->getValue("PROYECTO_ID").toInt(),
                                      pRecord->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                                      apuntes, supervisar );
    return asiento;
}

contab::RecAsiento* IAsentableFactura::creaAsientoCompletoVenta(
    dbRecordList* detalles, contab::RecAsiento* old_asiento, bool supervisar)
{
    typedef contab::ContabModule::ApunteQuickValues ApunteValues;
    List<ApunteValues> apuntes;
    Dictionary<Money> mercaderias;
    Dictionary<Money> ivas;
    // Ver la cuenta de la proveedora
    Xtring cuentacli = getRecTercero()->getValue("SUBCUENTA").toString();
    if( cuentacli.isEmpty() ) {
        cuentacli = "430." + getRecTercero()->getValue( "CODIGO" ).toString();
    }
    contab::RecCuenta::quickCreate( pRecord->getConnection(), cuentacli,
                                    getRecTercero()->getValue( "RAZONSOCIAL" ).toString() );
    contab::Cuenta cuentacli_contraida(cuentacli, contab::ModuleInstance->getMaxNivelGrupo() );
    Xtring concepto;
    if( mTipo == facturaventa )
        concepto = "FacVen:" + pRecord->getValue("NUMERO").toString();
    else
        concepto = "AlbVen:" + pRecord->getValue("NUMERO").toString();
    for( dbRecordList::const_iterator it = detalles->begin(); it != detalles->end(); ++ it ) {
        dbRecord *factudet = *it;
        RecArticulo *recarticulo;
        empresa::RecTipoIVA *rectipoiva;
        if( mTipo == facturaventa ) {
            recarticulo = static_cast<RecFacturaVentaDet *>(*it)->getRecArticulo();
            rectipoiva = static_cast<RecFacturaVentaDet *>(*it)->getRecTipoIVA();
        } else {
            recarticulo = static_cast<RecAlbaranVentaDet *>(*it)->getRecArticulo();
            rectipoiva = static_cast<RecAlbaranVentaDet *>(*it)->getRecTipoIVA();
        }
        Xtring cuenta_mercaderias = recarticulo->getRecFamilia()->getValue( "SUBCUENTAVENTAS" ).toString();
        if( cuenta_mercaderias.isEmpty() )
            cuenta_mercaderias = "700.";
        else
            contab::RecCuenta::quickCreate( factudet->getConnection(), cuenta_mercaderias,
                                            recarticulo->getRecFamilia()->getValue( "NOMBRE" ).toString() );
        mercaderias.set( cuenta_mercaderias,
                         mercaderias[cuenta_mercaderias] + factudet->getValue("IMPORTE").toMoney() );
        Xtring cuenta_iva = rectipoiva->getValue( "SUBCUENTAREPERCUTIDO" ).toString();
        if( cuenta_iva.isEmpty() )
            cuenta_iva= "477.";
        else
            contab::RecCuenta::quickCreate( factudet->getConnection(), cuenta_iva,
                                            rectipoiva->getValue( "NOMBRE" ).toString() );
        if( dynamic_cast<IIVADesglosable *>(pRecord)->hasRecargo() ) {
            ivas.set( cuenta_iva,
                      ivas[cuenta_iva] + rectipoiva->calcIVARecargo( factudet->getValue("IMPORTE").toMoney() ) );
        } else {
            ivas.set( cuenta_iva,
                      ivas[cuenta_iva] + rectipoiva->calcIVA( factudet->getValue("IMPORTE").toMoney() ) );
        }
    }
    for( Dictionary<Money>::const_iterator itmercad = mercaderias.begin();
            itmercad != mercaderias.end(); ++ itmercad ) {
        if( itmercad->second != 0.0 ) {
            ApunteValues mercaderia = { itmercad->first, "(" + cuentacli_contraida + ") " + concepto,
                                        0.0, round(itmercad->second)
                                      };
            apuntes.push_back(mercaderia);
        }
    }
    for( Dictionary<Money>::const_iterator itivas = ivas.begin();
            itivas != ivas.end(); ++ itivas ) {
        if( itivas->second != 0.0 ) {
            ApunteValues iva = { itivas->first, "(" + cuentacli_contraida + ") " + concepto,
                                 0.0, round(itivas->second)
                               };
            apuntes.push_back(iva);
        }
    }
    Money total = pRecord->getValue("TOTAL").toMoney();
    ApunteValues total_apunte_haber = { cuentacli, concepto, total, 0.0 };
    apuntes.push_back(total_apunte_haber);

    // Pagos
    // Decidir si la forma de pago hace que el resto se vaya a una cuenta especial
    Xtring cuentapago = getRecFormaPago()->getValue("SUBCUENTAPAGO").toString();
    if( cuentapago.endsWith(".")) {
        cuentapago += getRecTercero()->getValue("CODIGO").toString();
        delete contab::RecCuenta::quickCreate( pRecord->getConnection(), cuentapago,
                                               getRecTercero()->getValue( "RAZONSOCIAL" ).toString() );
    }
    Money entrega_albaranes = pRecord->getValue( "ENTREGAALBARANES" ).toMoney();
    Money entrega = pRecord->getValue( "ENTREGA" ).toMoney();
    // Si la forma de pago no tiene cuenta ni plazos y no hay pagos por caja, el asiento acaba aquí
    if( !cuentapago.isEmpty() && getRecFormaPago()->getValue("NUMPLAZOS").toInt() !=0 && total != 0.0 ) {
        ApunteValues total_apunte_debe = { cuentacli, "Cobro " + concepto, 0.0, total };
        apuntes.push_back(total_apunte_debe);
    } else if( cuentapago.isEmpty() && ( entrega != 0 || entrega_albaranes != 0.0 ) ) {
        ApunteValues total_apunte_debe = { cuentacli, "Cobro " + concepto, 0.0, entrega + entrega_albaranes };
        apuntes.push_back(total_apunte_debe);
    }
    if( entrega_albaranes != 0.0 ) {
        ApunteValues entrega_albaranes_apunte = {
            contab::ModuleInstance->creaCuentaTercero(0, getRecTercero(), "407", supervisar )->getValue( "CUENTA" ).toString(),
            "Cobro albaranes " + concepto, entrega_albaranes, 0.0
        };
        apuntes.push_back(entrega_albaranes_apunte);
    }
    if( entrega != 0.0 ) {
        ApunteValues entrega_apunte_debe = {
            empresa::ModuleInstance->getRecEmpresa()->getValue( "CUENTACAJA" ).toString(),
            "Cobro " + concepto, entrega, 0.0
        };
        apuntes.push_back(entrega_apunte_debe);
    }
    if( !cuentapago.isEmpty() && (total - entrega - entrega_albaranes != 0.0) ) {
        ApunteValues total_apunte = { cuentapago, Xtring::null, total, 0.0 };
        apuntes.push_back(total_apunte);
    }
    contab::RecAsiento *asiento = contab::ModuleInstance->creaAsientoMultiple(
                                      0, old_asiento, pRecord->getValue("FECHA").toDate(),
                                      pRecord->getIDSqlCond(), pRecord->getValue("PROYECTO_ID").toInt(),
                                      pRecord->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                                      apuntes, supervisar );
    return asiento;
}

} // namespace factu
} // namespace gong

#endif // HAVE_CONTABMODULE


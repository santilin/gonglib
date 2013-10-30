#include <dbappdbapplication.h>
#include <dbappmainwindow.h>
#include <empresarecmoneda.h>
#include "pagosmodule.h"
#include "pagosrecformapago.h"
#include "pagosfrmpagarrecibo.h"
#include "pagosrecpago.h"
#include "pagosreccobro.h"
#include "pagosipagablerecord.h"

#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif

namespace gong {
namespace pagos {

IPagableRecord::IPagableRecord(dbRecord* pFactura, Tipo t,
                               const Xtring& total_field, const Xtring& pagos_field,
                               const Xtring& resto_field, const XtringList& other_pagos_fields)
    : pFactura( pFactura ), mTipo( t ), mTotalField( total_field ), mPagosField( pagos_field ),
      mRestoField( resto_field ), mOtherPagosFields( other_pagos_fields )
{
}

dbRecord *IPagableRecord::createRecibo()
{
    if( mTipo == pagos ) {
        return DBAPP->createRecord( "PAGO", 0, pFactura->getUser() );
    } else {
        return DBAPP->createRecord( "COBRO", 0, pFactura->getUser() );
    }
}


Xtring IPagableRecord::getFacturaWhere() const
{
    return "TABLAFACTURAS=" + pFactura->getConnection()->toSQL( pFactura->getTableName() )
           + " AND FACTURA_ID= " + pFactura->getConnection()->toSQL( pFactura->getRecordID() );
}


int IPagableRecord::hasPagos(int estado_si, int estado_no, bool soloautomaticos)
{
    dbRecord *recibo = createRecibo();
    Xtring sql = "SELECT COUNT(*) FROM " + recibo->getTableName() +
                 recibo->getFilter( "WHERE", getFacturaWhere() );
    if( soloautomaticos )
        sql += " AND AUTOMATICO=1";
    if( estado_si != -1 )
        sql += " AND ESTADORECIBO=" + pFactura->getConnection()->toSQL( estado_si );
    if( estado_no != -1 )
        sql += " AND ESTADORECIBO<>" + pFactura->getConnection()->toSQL( estado_no );
    return pFactura->getConnection()->selectInt( sql );
}

bool IPagableRecord::delPagos( bool soloautomaticos )
{
    bool ret = true;
    dbRecord *recibo = createRecibo();
    Xtring id_field = pFactura->getTableName() + "_ID";
    Xtring sql = "SELECT ID FROM " + recibo->getTableName()
                 + recibo->getFilter( "WHERE", getFacturaWhere() )
                 + " AND ESTADORECIBO=" + pFactura->getConnection()->toSQL( PagosModule::ReciboPendiente ) ;
    if( soloautomaticos )
        sql += " AND AUTOMATICO=1";
    dbResultSet *rs = pFactura->getConnection()->select( sql );
    while( rs->next() ) {
        dbRecordID pago_id = rs->toLong( 0 );
        if( recibo->read( pago_id ) ) {
            if( !recibo->remove() )
                ret = false;
        } else {
            ret = false;
        }
    }
    delete rs;
    return ret;
}

void IPagableRecord::actRestoFactura()
{
    Money resto = pFactura->getValue( mTotalField ).toMoney();
    Money pagos = pFactura->getValue( mPagosField ).toMoney();
    resto = resto - pagos;
    for( XtringList::const_iterator it = mOtherPagosFields.begin();
            it != mOtherPagosFields.end(); ++ it ) {
        resto = resto - pFactura->getValue( *it ).toMoney();
    }
    pFactura->setValue( mRestoField, resto );
}


bool IPagableRecord::actPagos()
{
    dbRecord *recibo = createRecibo();
    Xtring id_field = pFactura->getTableName() + "_ID";
    Xtring sql = "SELECT SUM( IMPORTE - RESTO ) FROM " + recibo->getTableName() +
                 recibo->getFilter( "WHERE", getFacturaWhere() );
    Money pagos = pFactura->getConnection()->selectMoney( sql );
    if( pagos != pFactura->getValue( mPagosField ).toDouble() ) {
        pFactura->setValue( mPagosField, pagos );
        actRestoFactura();
        pFactura->save(false);
    }
    return true;
}

int IPagableRecord::genPagos()
{
    dbRecord *formapago = pFactura->findRelatedRecord( "FORMAPAGO_ID" );
    _GONG_DEBUG_ASSERT( formapago );
    if( formapago->getValue( "TIPOFORMAPAGO" ).toInt() != RecFormaPago::GeneraRecibos )
        return 0;
    dbRecord *recibo = createRecibo();
    Xtring informe;
#ifdef HAVE_CONTABMODULE
    Xtring str_cuenta_origen;
    if( mTipo == pagos ) {
        str_cuenta_origen = formapago->getValue( "SUBCUENTAPAGO" ).toString();
    } else {
        str_cuenta_origen = formapago->getValue( "SUBCUENTACOBRO" ).toString();
    }
    if( str_cuenta_origen.isEmpty() )
        str_cuenta_origen = getRecTercero()->getValue( "SUBCUENTA" ).toString();
#endif
    uint nrecibos = formapago->getValue("NUMPLAZOS").toInt();
    if( nrecibos == 0 ) {
        _GONG_DEBUG_WARNING( "La forma de pago tiene 0 plazos" );
        nrecibos = 1;
    }
    uint diasprimerplazo = formapago->getValue("DIASPRIMERPLAZO").toInt();
    uint diasentreplazos = formapago->getValue("DIASENTREPLAZOS").toInt();
    double importe = pFactura->getValue("RESTO").toDouble();
    // Calcular número de recibos según la forma de pago
    if( importe == 0.0 || nrecibos == 0 ) {
        DBAPP->showStickyOSD( pFactura->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                              Xtring::printf( _("%s se considera %s. No se ha generado ningún recibo."),
                                              DBAPP->getTableDescSingular( pFactura->getTableName(), "La" ).c_str(),
                                              pFactura->getTableDefinition()->isFemenina() ? _("pagada") : _("pagado") ) );
        return 0;
    }
    importe /= nrecibos;
    Date vencimiento = pFactura->getValue("FECHA").toDate() + diasprimerplazo;
    Date fechavalor = vencimiento;
    bool nocontinuar = false;
    for( uint nrecibo = 0; nrecibo < nrecibos && nocontinuar == false; nrecibo ++ ) {
        recibo->setNew( true );
        recibo->clear( true );
        // generar el recibo individual
        recibo->setValue( "TABLAFACTURAS", pFactura->getTableName() );
        recibo->setValue( "FACTURA_ID", pFactura->getRecordID() );
        recibo->setValue( "TABLATERCEROS", getRecTercero()->getTableName() );
        recibo->setValue( "TERCERO_ID", getRecTercero()->getRecordID() );
        recibo->setValue( "AUTOMATICO", true );
        recibo->setValue( "RAZONSOCIAL", getRecTercero()->getValue( "RAZONSOCIAL") );
        if( nrecibos == 1 )
            recibo->setValue( "DESCRIPCION", Xtring::printf( "%s %s, %s",
                              pFactura->getTableDefinition()->getDescSingular().c_str(),
                              pFactura->getValue( "NUMERO" ).toString().c_str(),
                              getRecTercero()->getValue( "RAZONSOCIAL").toString().c_str() ) );
        else
            recibo->setValue( "DESCRIPCION", Xtring::printf( "%d/%d: %s %s, %s",
                              nrecibo+1, nrecibos,
                              pFactura->getTableDefinition()->getDescSingular().c_str(),
                              pFactura->getValue( "NUMERO" ).toString().c_str(),
                              getRecTercero()->getValue( "RAZONSOCIAL").toString().c_str() ) );
        recibo->setValue( "MONEDA_ID", empresa::ModuleInstance->getRecEmpresa()->getValue( "MONEDA_ID" ) );
        recibo->setValue( "NUMERO", recibo->selectNextInt("NUMERO") );
        recibo->setValue( "NUMEROAGRUPADO", recibo->getValue("NUMERO") );
        recibo->setValue( "FECHAEMISION", pFactura->getValue("FECHA").toDate() );
        recibo->setValue( "VENCIMIENTO", vencimiento );
        recibo->setValue( "FECHAVALOR", fechavalor );
        recibo->setValue( "IMPORTE", importe );
        recibo->setValue( "RESTO", importe );
        recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPendiente );
#ifdef HAVE_CONTABMODULE
        recibo->setValue( "CUENTAORIGEN", str_cuenta_origen );
#endif
        informe += Xtring::printf("\tImporte: %f,\tFecha de valor: %s\n",
                                  importe, fechavalor.toString().c_str() );
        if( !recibo->existsAnother( getFacturaWhere()
                                    + " AND FECHAVALOR=" + recibo->getConnection()->toSQL( fechavalor )
                                    + " AND IMPORTE=" + recibo->getConnection()->toSQL( importe ) ) ) {
            try {
                recibo->save(false);
            } catch( dbError &e ) {
                informe += Xtring::printf( _("\tError: %s\n"), e.what() );
            }
        } else {
            informe += Xtring::printf(
                           _("Ya existe un recibo con estos datos, por lo que no se generará.\n"));
        }
        fechavalor = fechavalor + diasentreplazos;
        vencimiento = vencimiento + diasentreplazos;
    }
    if( nrecibos )
        DBAPP->showStickyOSD( pFactura->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                              Xtring::printf(_("Se han generado <b>%d</b> recibos"), nrecibos) );
    return nrecibos;
}


void IPagableRecord::pagarRecibo( FrmEditRecMaster *parent, dbRecordID reciboid,
                                  dbRecord *recibo, bool supervisar )
{
    bool pagado = false;
    dbRecordID asientoid = 0;
    Money importe = 0.0;
    Money pago = 0.0;
    bool has_contab = false;
    Xtring que_es;
    Xtring tercero_field = getRecTercero()->getTableName() + "_ID";
    if( mTipo == pagos ) {
        que_es = DBAPP->getTableDescSingular( "PAGO", "" ) + " ";
    } else {
        que_es = DBAPP->getTableDescSingular( "COBRO", "" ) + " ";
    }
    bool accepted = true, pago_multiple = false;
    Xtring numeroagrupado = recibo->getValue( "NUMEROAGRUPADO" ).toString();
    if( numeroagrupado.isEmpty() )
        numeroagrupado = recibo->getValue( "NUMERO" ).toString();
#ifdef HAVE_CONTABMODULE
    dbRecord *cuentapago = recibo->findRelatedRecord( "CUENTAPAGO_ID" );
    has_contab = contab::ModuleInstance->isContabActive();
    dbRecordID cuentapago_id;
    contab::Cuenta cuenta_pago( contab::ModuleInstance->getDigitosTrabajo() );
    contab::Cuenta cuenta_pago_contraida( cuenta_pago );
    contab::RecCuenta *cuenta_origen = 0;
    dbRecordID proyecto_id = 0;
    proyecto_id = getProyectoID();
    if( has_contab ) {
        contab::Cuenta str_cuenta_origen( recibo->getValue( "CUENTAORIGEN" ).toString(),
                                          contab::ModuleInstance->getDigitosTrabajo() );
        str_cuenta_origen.expandir();
        if( str_cuenta_origen.isEmpty() ) {
            cuenta_origen = contab::ModuleInstance->creaCuentaTercero( parent, getRecTercero(),
                            getPrefijoContableTercero(), supervisar );
        } else if( str_cuenta_origen.endsWith(".") ) {
            cuenta_origen = contab::ModuleInstance->creaCuentaTercero( parent, getRecTercero(),
                            str_cuenta_origen, supervisar );
        } else {
            cuenta_origen = static_cast<contab::RecCuenta *>(DBAPP->createRecord("CUENTA"));
            cuenta_origen->readWithFilter( "CUENTA="
                                           + cuenta_origen->getConnection()->toSQL(str_cuenta_origen) );
        }
        if( cuenta_origen->getRecordID() == 0 ) {
            FrmBase::msgError(parent,
                              Xtring::printf( _("No se puede realizar el %s porque la cuenta %s de la forma de pago no existe"),
                                              que_es.lower().c_str(), str_cuenta_origen.c_str()) );
            delete cuenta_origen;
            return;
        }
    }
#endif
    while( reciboid ) {
        recibo->read( reciboid );
        empresa::RecMoneda *recmoneda = static_cast<empresa::RecMoneda *>(recibo->findRelatedRecord( "MONEDA_ID" ));
        _GONG_DEBUG_ASSERT( recmoneda );
        importe = recibo->getValue( "IMPORTE" ).toMoney();
        if( pago == 0.0 )
            pago = importe;
        Date fechapago;
        dbRecordID moneda_id;
        if( !PagosModule::sLastFechaPago.isNull() ) {
            // Si se acaba de borrar un recibo, sLastFechaPago tiene la fecha de pago de ese recibo
            fechapago = PagosModule::sLastFechaPago;
        } else
            fechapago = recibo->getValue( "VENCIMIENTO" ).toDate();
        PagosModule::sLastMonedaCodigo = recmoneda->getValue( "CODIGO" ).toString();
        if( pago_multiple == false ) { // Si no estamos en un pago múltiple, pedir datos
            FrmPagarRecibo *pr = new FrmPagarRecibo( has_contab, pago, fechapago,
                    PagosModule::sLastCuentaPago, PagosModule::sLastDocumentoPago,
                    PagosModule::sLastMonedaCodigo );
            pr->showModalFor( parent, true, true );
            accepted = !pr->wasCancelled();
            pago = pr->getImporte();
#ifdef HAVE_CONTABMODULE
            if( has_contab ) {
                cuenta_pago = contab::Cuenta( pr->getCuentaPago(), contab::ModuleInstance->getDigitosTrabajo() );
                cuenta_pago.expandir();
                cuenta_pago_contraida = cuenta_pago;
                cuenta_pago_contraida.contraer( contab::ModuleInstance->getMaxNivelGrupo() );
                PagosModule::sLastCuentaPago = cuenta_pago;
            }
#endif
            PagosModule::sLastDocumentoPago = pr->getDocumentoPago();
            fechapago = pr->getFechaPago();
            moneda_id = pr->getRecMonedaID();
            delete pr;
        }
        if( accepted ) {
            if( pago == 0.0 || !fechapago.isValid() ) {
                FrmBase::msgError( parent, _("Por favor, rellena el importe y la fecha del pago") );
            } else if( moneda_id == 0 ) {
                FrmBase::msgError( parent, _("Por favor, elige la moneda del pago") );
            } else if( has_contab && PagosModule::sLastCuentaPago.isEmpty() ) {
                FrmBase::msgError( parent, _("Por favor, rellena la cuenta del pago") );
            } else {
#ifdef HAVE_CONTABMODULE
                if( has_contab ) {
                    if( cuentapago_id == 0 ) {
                        cuentapago_id = cuenta_origen->getConnection()->
                                        selectInt( "SELECT ID FROM CUENTA" +  cuenta_origen->getFilter( "WHERE",
                                                   "CUENTA=" + cuenta_origen->getConnection()->toSQL( PagosModule::sLastCuentaPago ) ) );
                    }
                    /// \todo si la cuenta no existe, choose
                }
#endif
                if( pago == importe && asientoid == 0) {
                    // Pagar el recibo
                    if( moneda_id != recibo->getValue( "MONEDA_ID" ).toUInt() ) {
                        recibo->setValue( "MONEDA_ID", moneda_id );
                        recmoneda->read( moneda_id );
                        recibo->setValue( "RATIO_MONEDA", recmoneda->getValue( "RATIO" ).toDouble() );
                    } else {
                        recibo->setValue( "MONEDA_ID", moneda_id );
                        recibo->setValue( "RATIO_MONEDA", 1.0 );
                    }
                    recibo->setValue( "NUMEROAGRUPADO", numeroagrupado ); // Solo este
                    recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPagado );
                    recibo->setValue( "RESTO", Money(0.0) );
                    recibo->setValue( "DOCUMENTOPAGO", PagosModule::sLastDocumentoPago );
#ifdef HAVE_CONTABMODULE
                    recibo->setValue( "CUENTAPAGO_ID", cuentapago_id );
#endif
                    recibo->setValue( "FECHAPAGO", fechapago );
                    recibo->save(false);
                    pagado = true;
#ifdef HAVE_CONTABMODULE
                    if( has_contab ) {
                        // Crear el asiento
                        dbRecord *asiento;
                        if( mTipo == pagos ) {
                            asiento = contab::ModuleInstance->creaAsientoSimple(
                                          parent, 0,
                                          fechapago, recibo->getIDSqlCond(),
                                          proyecto_id, que_es + recibo->getValue( "DESCRIPCION" ).toString(),
                                          PagosModule::sLastDocumentoPago,
                                          cuenta_origen->getValue( "CUENTA").toString(), importe,
                                          PagosModule::sLastCuentaPago, importe,
                                          supervisar );
                        } else {
                            asiento = contab::ModuleInstance->creaAsientoSimple(
                                          parent, 0,
                                          fechapago, recibo->getIDSqlCond(),
                                          proyecto_id, que_es + recibo->getValue( "DESCRIPCION" ).toString(),
                                          PagosModule::sLastDocumentoPago,
                                          PagosModule::sLastCuentaPago, importe,
                                          cuenta_origen->getValue( "CUENTA").toString(), importe,
                                          supervisar );
                        }
                        if( (asientoid = ( asiento->getRecordID() ) ) ) {
                            recibo->setValue( "ASIENTO_PAGO_ID", asientoid );
                            recibo->save(false);
                            pagado = true;
                            DBAPP->showStickyOSD( recibo->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                                                  Xtring::printf(_("Se ha generado el asiento nº %s"),
                                                                 asiento->getValue("NUMASIENTO").toString().c_str() ) );
                        }
                        delete asiento;
                    }
#endif
                    reciboid = 0;
                    pago_multiple = false;
                } else if( pago < importe ) {
                    int answer;
                    Xtring question = Xtring::printf(_("El pago es menor que el importe.\n"
                                                       "Este recibo quedará pagado con este importe de %s\n y se creará un nuevo recibo con un resto de %s"),
                                                     pago.toString().c_str(), (importe-pago).toString().c_str() );
                    if( pago_multiple == true ) {
                        answer = FrmBase::Yes;
                        FrmBase::msgOk( parent, question );
                    } else
                        answer = FrmBase::msgYesNoCancel( parent,
                                                          question + _("\n¿Quieres continuar?"));
                    switch( answer ) {
                    case FrmBase::Yes: {
                        // Pagar este recibo con el importe introducido
                        if( moneda_id != recibo->getValue( "MONEDA_ID" ).toUInt() ) {
                            recibo->setValue( "MONEDA_ID", moneda_id );
                            recmoneda->read( moneda_id );
                            recibo->setValue( "RATIO_MONEDA", recmoneda->getValue( "RATIO" ).toDouble() );
                        } else {
                            recibo->setValue( "MONEDA_ID", moneda_id );
                            recibo->setValue( "RATIO_MONEDA", 1.0 );
                        }
                        recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPagado );
                        recibo->setValue( "IMPORTE", pago );
                        recibo->setValue( "RESTO", 0.0 );
                        recibo->setValue( "DOCUMENTOPAGO", PagosModule::sLastDocumentoPago );
#ifdef HAVE_CONTABMODULE
                        Xtring cuentaorigen = recibo->getValue( "CUENTAORIGEN" ).toString();
                        recibo->setValue( "CUENTAPAGO_ID", cuentapago_id );
                        cuentapago->clear(false);
#endif
                        recibo->setValue( "FECHAPAGO", fechapago );
                        recibo->setValue( "NUMEROAGRUPADO", numeroagrupado );
                        recibo->save(false);
                        pagado = true;
#ifdef HAVE_CONTABMODULE
                        if( has_contab ) {
                            // Crear el asiento
                            dbRecord *asiento;
                            if( asientoid == 0 ) {
                                if( mTipo == pagos ) {
                                    asiento = contab::ModuleInstance->creaAsientoSimple(
                                                  parent, 0, fechapago, recibo->getIDSqlCond(),
                                                  proyecto_id, que_es + recibo->getValue( "DESCRIPCION" ).toString(),
                                                  PagosModule::sLastDocumentoPago,
                                                  cuenta_origen->getValue( "CUENTA" ).toString(), pago,
                                                  PagosModule::sLastCuentaPago, pago,
                                                  supervisar );
                                } else {
                                    asiento = contab::ModuleInstance->creaAsientoSimple(
                                                  parent, 0, fechapago, recibo->getIDSqlCond(),
                                                  proyecto_id, que_es + recibo->getValue( "DESCRIPCION" ).toString(),
                                                  PagosModule::sLastDocumentoPago,
                                                  PagosModule::sLastCuentaPago, pago,
                                                  cuenta_origen->getValue( "CUENTA" ).toString(), pago,
                                                  supervisar );
                                }
                                if( (asientoid = ( asiento->getRecordID() ) ) ) {
                                    recibo->setValue( "ASIENTO_PAGO_ID", asientoid );
                                    recibo->save(false);
                                    pagado = true;
                                    DBAPP->showStickyOSD( recibo->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                                                          Xtring::printf(_("Se ha generado el asiento nº %s"),
                                                                         asiento->getValue("NUMASIENTO").toString().c_str() ) );
                                }
                                delete asiento;
                            } else {
                                contab::RecAsiento *asiento = static_cast<contab::RecAsiento *>( DBAPP->createRecord( "ASIENTO" ) );
                                asiento->read( asientoid );
                                contab::RecApunte *apuntenew = static_cast<contab::RecApunte *>( DBAPP->createRecord( "APUNTE" ) );
                                apuntenew->setValue( "NUMAPUNTE", dbRecordID(asiento->getApuntes()->size() + 1 ) );
                                if( mTipo == pagos ) {
                                    contab::RecApunte *apunte_pago = static_cast<contab::RecApunte *>(asiento->getApuntes()->getRecord( asiento->getApuntes()->size()-1 ) );
                                    apunte_pago->setValue( "HABER", apunte_pago->getValue( "HABER" ).toMoney() + pago );
                                    apuntenew->quickValues( contab::Cuenta(cuenta_origen->getValue( "CUENTA" ).toString(),
                                                                           contab::ModuleInstance->getDigitosTrabajo() ),
                                                            Xtring::null, pago, 0, PagosModule::sLastDocumentoPago,
                                                            cuenta_pago_contraida );
                                    asiento->addApunte( apuntenew, asiento->getApuntes()->size()-1 );
                                } else {
                                    contab::RecApunte *apunte_cobro = static_cast<contab::RecApunte *>(asiento->getApuntes()->getRecord( 0 ) );
                                    apunte_cobro->setValue( "DEBE", apunte_cobro->getValue( "DEBE" ).toMoney() + pago );
                                    apuntenew->quickValues( contab::Cuenta(cuenta_origen->getValue( "CUENTA" ).toString(),
                                                                           contab::ModuleInstance->getDigitosTrabajo() ),
                                                            Xtring::null, 0, pago,
                                                            PagosModule::sLastDocumentoPago, cuenta_pago_contraida );
                                    asiento->addApunte( apuntenew );
                                }
                                for( uint nap = 0; nap < asiento->getApuntes()->size(); ++nap ) {
                                    _GONG_DEBUG_PRINT(0, asiento->getApuntes()->getRecord( nap )->toString( TOSTRING_DEBUG_COMPLETE) );
                                }
                                asiento->save(true);
                                delete asiento;
                            }
                        }
#endif
                        // Generar un nuevo recibo con importe = resto
                        // dont use gaps
                        Xtring lastrecibo = Xtring::number(recibo->selectNextInt( "NUMERO", Xtring::null, false) );
                        recibo->setNew( true );
                        recibo->setValue( "MONEDA_ID", empresa::ModuleInstance->getRecEmpresa()->getValue( "MONEDA_ID" ) );
                        recibo->setValue( "NUMERO", lastrecibo );
                        recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPendiente );
                        recibo->setValue( "IMPORTE", importe - pago );
                        recibo->setValue( "RESTO", importe - pago );
                        recibo->setValue( "FECHAEMISION", fechapago );
                        recibo->setValue( "NUMEROAGRUPADO", lastrecibo );
                        recibo->setNullValue( "FECHAPAGO" );
#ifdef HAVE_CONTABMODULE
                        recibo->setValue( "CUENTAORIGEN", cuentaorigen );
                        recibo->setNullValue( "ASIENTO_PAGO_ID" );
                        recibo->setNullValue( "CUENTAPAGO_ID" );
#endif
                        recibo->setNullValue( "DOCUMENTOPAGO" );
                        recibo->save(false);
                        pagado = true;
                        reciboid = 0;
                        pago = importe - pago;
                    }
                    break;
                    case FrmBase::No:
                        break;
                    case FrmBase::Cancel:
                        reciboid = 0;
                        break;
                    }
                    pago_multiple = false;
                } else { // if( pago > importe || asientoid != 0 )
                    int answer;
                    if( pago_multiple == true )
                        answer = FrmBase::Yes;
                    else
                        answer = FrmBase::msgYesNoCancel( parent,
                                                          Xtring::printf(_("El pago es mayor que el importe.\n"
                                                                  "Si continúas, este recibo quedará pagado y podrás elegir otro recibo para completar el resto de %s.\n"
                                                                  "¿Quieres continuar?"),
                                                                  (pago-importe).toString().c_str() ) );
                    switch( answer ) {
                    case FrmBase::Yes: {
                        // Pagar el recibo
                        if( moneda_id != recibo->getValue( "MONEDA_ID" ).toUInt() ) {
                            recibo->setValue( "MONEDA_ID", moneda_id );
                            recmoneda->read( moneda_id );
                            recibo->setValue( "RATIO_MONEDA", recmoneda->getValue( "RATIO" ).toDouble() );
                        } else {
                            recibo->setValue( "", moneda_id );
                            recibo->setValue( "RATIO_MONEDA", 1.0 );
                        }
                        recibo->setValue( "NUMEROAGRUPADO", numeroagrupado );
                        recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPagado );
                        recibo->setValue( "RESTO", 0 );
                        recibo->setValue( "DOCUMENTOPAGO", PagosModule::sLastDocumentoPago );
#ifdef HAVE_CONTABMODULE
                        recibo->setValue( "CUENTAPAGO_ID", cuentapago_id );
#endif
                        recibo->setValue( "FECHAPAGO", fechapago );
                        recibo->save(false);
                        pagado = true;
#ifdef HAVE_CONTABMODULE
                        if( has_contab ) {
                            // Si no hay asiento creado, crearlo
                            if( asientoid == 0 ) {
                                dbRecord *asiento;
                                if( mTipo == pagos ) {
                                    asiento = contab::ModuleInstance->creaAsientoSimple(
                                                  parent, 0, fechapago, recibo->getIDSqlCond(),
                                                  proyecto_id, que_es + recibo->getValue( "DESCRIPCION" ).toString(),
                                                  PagosModule::sLastDocumentoPago,
                                                  cuenta_origen->getValue( "CUENTA" ).toString(), importe,
                                                  PagosModule::sLastCuentaPago, importe,
                                                  supervisar );
                                } else {
                                    asiento = contab::ModuleInstance->creaAsientoSimple(
                                                  parent, 0, fechapago, recibo->getIDSqlCond(),
                                                  proyecto_id, que_es + recibo->getValue( "DESCRIPCION" ).toString(),
                                                  PagosModule::sLastDocumentoPago,
                                                  PagosModule::sLastCuentaPago, importe,
                                                  cuenta_origen->getValue( "CUENTA" ).toString(), importe,
                                                  supervisar );
                                }
                                if( (asientoid = ( asiento->getRecordID() ) ) ) {
                                    recibo->setValue( "ASIENTO_PAGO_ID", asientoid );
                                    recibo->save(false);
                                    pagado = true;
                                    DBAPP->showStickyOSD( recibo->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                                                          Xtring::printf(_("Se ha generado el asiento nº %s"),
                                                                         asiento->getValue("NUMASIENTO").toString().c_str() ) );
                                }
                                delete asiento;
                            } else { // Añadir el apunte al asiento ya existente
                                contab::RecAsiento *asiento = static_cast<contab::RecAsiento *>( DBAPP->createRecord( "ASIENTO" ) );
                                asiento->read( asientoid );
                                contab::RecApunte *apuntenew = static_cast<contab::RecApunte *>( DBAPP->createRecord( "APUNTE" ) );
                                apuntenew->setValue( "NUMAPUNTE", dbRecordID(asiento->getApuntes()->size() + 1) );
                                if( mTipo == pagos ) {
                                    contab::RecApunte *apunte_pago = static_cast<contab::RecApunte *>(asiento->getApuntes()->getRecord( asiento->getApuntes()->size()-1 ) );
                                    apunte_pago->setValue( "HABER", apunte_pago->getValue( "HABER" ).toMoney() + importe );
                                    apuntenew->quickValues( contab::Cuenta(cuenta_origen->getValue( "CUENTA" ).toString(),
                                                                           contab::ModuleInstance->getDigitosTrabajo() ), Xtring::null,
                                                            importe, 0, PagosModule::sLastDocumentoPago, cuenta_pago_contraida );
                                    asiento->addApunte( apuntenew, asiento->getApuntes()->size()-1 );
                                } else {
                                    contab::RecApunte *apunte_cobro = static_cast<contab::RecApunte *>(asiento->getApuntes()->getRecord( 0 ) );
                                    apunte_cobro->setValue( "DEBE", apunte_cobro->getValue( "DEBE" ).toMoney() + importe );
                                    apuntenew->quickValues( contab::Cuenta(cuenta_origen->getValue( "CUENTA" ).toString(),
                                                                           contab::ModuleInstance->getDigitosTrabajo() ), Xtring::null,
                                                            0, importe, PagosModule::sLastDocumentoPago, cuenta_pago_contraida );
                                    asiento->addApunte( apuntenew );
                                }
                                for( uint nap = 0; nap < asiento->getApuntes()->size(); ++nap ) {
                                    _GONG_DEBUG_PRINT(0, asiento->getApuntes()->getRecord( nap )->toString( TOSTRING_DEBUG_COMPLETE ) );
                                }
                                asiento->save(true);
                                delete asiento;
                            }
                        }
#endif
                        pago = pago - importe;
                        if( pago <= 0.0 ) {
                            reciboid = 0; // break while
                        } else {
                            // Elegir otro recibo para pagar el resto del pago
                            dbViewDefinitionsList views;
                            DBAPP->getDatabase()->getViewsForTable ( recibo->getTableName(), views );
                            dbRecordDataModel dm ( recibo, views,
                                                   "ESTADORECIBO=" + recibo->getConnection()->toSQL( PagosModule::ReciboPendiente )
                                                   + " AND " + getFacturaWhere() );
                            // Para que el choose no sobreescriba nuestro Recibo
                            dbRecord *recibo_tmp = createRecibo();
                            FrmBase::msgOk( _("Pago múltiple"), _("Elige el siguiente recibo para pagar"),
                                            FrmBase::information, parent );
                            reciboid = DBAPP->choose(parent, recibo_tmp, &dm, dbApplication::editNone, parent );
                            delete recibo_tmp;
                            pago_multiple = true;
                        }
                    }
                    break;
                    case FrmBase::No:
                        pago_multiple = false;
                        break;
                    case FrmBase::Cancel:
                        reciboid = 0;
                        pago_multiple = false;
                        break;
                    }
                }
            }
        } else
            reciboid = 0;
    }
    if( pagado ) {
        actRestoFactura();
    }
#ifdef HAVE_CONTABMODULE
    if( has_contab )
        delete cuenta_origen;
#endif
    parent->refresh();
}

void IPagableRecord::anularPagoRecibo( FrmEditRecMaster* parent, dbRecordID reciboid,
                                       dbRecord* recibo )
{
    if( recibo->getValue( "ESTADORECIBO" ).toInt() == PagosModule::ReciboPagado ) {
#ifdef HAVE_CONTABMODULE
        bool has_contab = contab::ModuleInstance->isContabActive();
        dbRecord *cuentapago;
        if( mTipo == pagos ) {
            cuentapago = static_cast<RecPago *>(recibo)->getRecCuentaPago();
        } else {
            cuentapago = static_cast<RecCobro *>(recibo)->getRecCuentaPago();
        }
        if( FrmBase::msgYesNoCancel( parent,
                                     Xtring::printf( _("¿Estás segura de borrar este pago realizado el día %s por un importe de %f,\n"
                                             " por la cuenta '%s' y mediante el documento '%s'?"),
                                             recibo->getValue( "FECHAPAGO" ).toDate().toString().c_str(),
                                             recibo->getValue( "IMPORTE" ).toMoney().toDouble(),
                                             cuentapago->getValue( "DESCRIPCION" ).toString().c_str(),
                                             recibo->getValue( "DOCUMENTOPAGO").toString().c_str() ) ) == FrmBase::Yes ) {
#else
        if( FrmBase::msgYesNoCancel( parent,
                                     Xtring::printf( _("¿Estás segura de borrar este pago realizado el día %s por un importe de %f,\n"
                                             "y mediante el documento '%s'?"),
                                             recibo->getValue( "FECHAPAGO" ).toDate().toString().c_str(),
                                             recibo->getValue( "IMPORTE" ).toMoney().toDouble(),
                                             recibo->getValue( "DOCUMENTOPAGO").toString().c_str() ) ) == FrmBase::Yes ) {
#endif
            bool anulapago = true;
            int npagosagrupados = recibo->getConnection()->selectInt(
                                      "SELECT COUNT(*) FROM " + recibo->getConnection()->nameToSQL(recibo->getTableName()) +
                                      " WHERE " + recibo->getFilter( "", "NUMEROAGRUPADO=" + recibo->getConnection()->toSQL( recibo->getValue("NUMEROAGRUPADO").toInt() ) ) );
            if( npagosagrupados != 1 ) {
                FrmBase::msgOk( parent,
                                Xtring::printf(_("Este es un pago agrupado de %d pagos con el número agrupado %d.\nRecuerda anular el pago de los otros"),
                                               npagosagrupados, recibo->getValue("NUMEROAGRUPADO").toInt() ) );
            }
#ifdef HAVE_CONTABMODULE
            if( has_contab ) {
                dbRecordID asientoid = recibo->getValue( "ASIENTO_PAGO_ID" ).toInt();
                if( asientoid ) {
                    contab::RecAsiento *asiento = static_cast<contab::RecAsiento *>(DBAPP->createRecord( "ASIENTO" ));
                    if( asiento->read( asientoid ) ) {
                        switch( FrmBase::msgYesNoCancel( parent,
                                                         Xtring::printf( _( "Este pago está contabilizado en el asiento %d de fecha %s.\n"
                                                                 "¿Quieres borrar el asiento asociado a este pago?"),
                                                                 asiento->getValue( "NUMASIENTO" ).toInt(),
                                                                 asiento->getValue( "FECHA" ).toString().c_str() ) ) ) {
                        case FrmBase::Yes: {
                            try {
                                if( !asiento->remove() )
                                    anulapago = false;
                                else
                                    DBAPP->getMainWindow()->refreshByName( Xtring::null, Xtring::null );
                            } catch( dbError &e ) {
                                FrmBase::msgError( parent,
                                                   Xtring(e.what()) + _(". Borra el asiento a mano y vuelve a intentarlo."));
                                anulapago = false;
                            }
                            break;
                            case FrmBase::Cancel:
                                return;
                            }
                        }
                    }
                }
            }
#endif
            if( anulapago ) {
                // Anular el pago
                PagosModule::sLastDocumentoPago = recibo->getValue( "DOCUMENTOPAGO" ).toString();
                PagosModule::sLastFechaPago = recibo->getValue( "FECHAPAGO" ).toDate();
                recibo->setValue( "RESTO", recibo->getValue( "IMPORTE" ) );
                recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPendiente );
                /// \todo si el pago es agrupado, avisar y anular el pago completo
                recibo->setNullValue( "NUMEROAGRUPADO" );
                recibo->setNullValue( "FECHAPAGO" );
                recibo->setNullValue( "DOCUMENTOPAGO" );
#ifdef HAVE_CONTABMODULE
                if( mTipo == pagos )
                    PagosModule::sLastCuentaPago = static_cast<RecPago *>(recibo)->getRecCuentaPago()->getValue( "CUENTA" ).toString();
                else
                    PagosModule::sLastCuentaPago = static_cast<RecCobro *>(recibo)->getRecCuentaPago()->getValue( "CUENTA" ).toString();
                recibo->setNullValue( "ASIENTO_PAGO_ID" );
                recibo->setNullValue( "CUENTAPAGO_ID" );
#endif
                try {
                    if( recibo->save(false) )
                        actRestoFactura();
                } catch( dbError &e ) {
                    FrmBase::msgError( parent, e.what() );
                }
                // Refrescar
                parent->refresh();
            }
        }
    }
}


} // namesace pagos
} // namesace gong

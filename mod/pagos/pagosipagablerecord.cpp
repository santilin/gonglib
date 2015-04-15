#include <dbappdbapplication.h>
#include <dbappmainwindow.h>
#include "pagosmodule.h"
#include "pagosrecformapago.h"
#include "pagosfrmpagarrecibo.h"
#include "pagosrecpago.h"
#include "pagosreccobro.h"
#include "pagosipagablerecord.h"

#ifdef HAVE_TESORERIAMODULE
#include <tesoreriaiapuntablerecord.h>
#endif
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif

namespace gong {
namespace pagos {

IPagableRecord::IPagableRecord(dbRecord* pFactura, Tipo t,
                               const Xtring& total_field, const Xtring& pagos_field,
                               const Xtring& resto_field, const XtringList& other_pagos_fields,
                               const Xtring& tablapagos )
    : pFactura( pFactura ), mTipo( t ), mTotalField( total_field ), mPagosField( pagos_field ),
      mRestoField( resto_field ), mOtherPagosFields( other_pagos_fields )
{
    if( tablapagos.isEmpty() ) {
        if( mTipo == IPagableRecord::pagos )
            mTablaPagos = "PAGO";
        else
            mTablaPagos = "COBRO";
    } else
        mTablaPagos = tablapagos;
}

dbRecord *IPagableRecord::createRecibo()
{
    if( mTipo == pagos ) {
        return DBAPP->createRecord( mTablaPagos, 0, pFactura->getUser() );
    } else {
        return DBAPP->createRecord( mTablaPagos, 0, pFactura->getUser() );
    }
}

int IPagableRecord::hasPagos(int estado_si, int estado_no, bool soloautomaticos)
{
    dbRecord *recibo = createRecibo();
    Xtring sql = "SELECT COUNT(*) FROM " + recibo->getConnection()->nameToSQL(recibo->getTableName()) +
                 recibo->getFilter( "WHERE", getFacturaWhere() );
    if( soloautomaticos )
        sql += " AND AUTOMATICO=1";
    if( estado_si != -1 )
        sql += " AND ESTADORECIBO=" + pFactura->getConnection()->toSQL( estado_si );
    if( estado_no != -1 )
        sql += " AND ESTADORECIBO<>" + pFactura->getConnection()->toSQL( estado_no );
    return pFactura->getConnection()->selectInt( sql );
}

/**
 * @brief Borra los pagos de un documento
 *
 * @param borratodos ...
 * @return bool si ha borrado alguno o no
 */
bool IPagableRecord::delPagos( bool borratodos )
{
    int nrecibos = 0;
    dbRecord *recibo = createRecibo();
    Xtring id_field = pFactura->getTableName() + "_ID";
    Xtring sql = "SELECT ID FROM " + recibo->getConnection()->nameToSQL(recibo->getTableName())
                 + recibo->getFilter( "WHERE", getFacturaWhere() );

    if( !borratodos ) {
        sql += " AND ESTADORECIBO=" + pFactura->getConnection()->toSQL( PagosModule::ReciboPendiente )
               + " AND AUTOMATICO=1";
    }
    dbResultSet *rs = pFactura->getConnection()->select( sql );
    while( rs->next() ) {
        dbRecordID pago_id = rs->toLong( 0 );
        if( recibo->read( pago_id ) ) {
            nrecibos++;
            recibo->remove();
        }
    }
    delete rs;
    if( nrecibos ) {
        DBAPP->showStickyOSD( pFactura->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                              Xtring::printf(_("Se han borrado <b>%d</b> recibos"), nrecibos) );
		DBAPP->getMainWindow()->refreshByName( Xtring::null, mTablaPagos );
    }
    return nrecibos;
}

dbRecordList* IPagableRecord::getPagos( PagosModule::EstadoRecibo estado )
{
    dbRecordList *result = 0;
    dbRecord *recibo = createRecibo();
    Xtring id_field = pFactura->getTableName() + "_ID";
    Xtring sql = "SELECT ID FROM " + recibo->getConnection()->nameToSQL(recibo->getTableName())
                 + recibo->getFilter( "WHERE", getFacturaWhere() );
    if( estado != PagosModule::ReciboSinEstado ) {
        sql += " AND ESTADORECIBO=" + pFactura->getConnection()->toSQL( estado );
    }
    dbResultSet *rs = pFactura->getConnection()->select( sql );
    if( rs->getRowCount() ) {
        result = new dbRecordList();
        while( rs->next() ) {
            dbRecordID pago_id = rs->toLong( 0 );
            if( recibo->read( pago_id ) ) {
                result->addRecord( recibo );
            }
        }
    }
    delete rs;
    return result;
}


/**
 * @brief ...
 *
 * @return void
 **/
void IPagableRecord::actRestoFactura()
{
    Money resto;
    dbRecord *formapago = pFactura->findRelatedRecord( "FORMAPAGO_ID" );
    _GONG_DEBUG_ASSERT( formapago );
    if( formapago->getValue( "TIPOFORMAPAGO" ).toInt() != RecFormaPago::SeIgnora ) {
        resto = pFactura->getValue( mTotalField ).toMoney();
        Money pagos = pFactura->getValue( mPagosField ).toMoney();
        resto = resto - pagos;
        for( XtringList::const_iterator it = mOtherPagosFields.begin();
                it != mOtherPagosFields.end(); ++ it ) {
            resto = resto - pFactura->getValue( *it ).toMoney();
        }
    }
    pFactura->setValue( mRestoField, resto );
}

bool IPagableRecord::actPagos()
{
    dbRecord *recibo = createRecibo();
    Xtring id_field = pFactura->getTableName() + "_ID";
    Xtring sql = "SELECT SUM( IMPORTE - RESTO ) FROM " + recibo->getConnection()->nameToSQL(recibo->getTableName()) +
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
    Xtring str_cuenta_origen;
#ifdef HAVE_CONTABMODULE
    if( mTipo == pagos ) {
        str_cuenta_origen = formapago->getValue( "SUBCUENTAPAGO" ).toString();
    } else {
        str_cuenta_origen = formapago->getValue( "SUBCUENTACOBRO" ).toString();
    }
    if( str_cuenta_origen.isEmpty() )
        str_cuenta_origen = getRecTercero()->getValue( "SUBCUENTA" ).toString();
#elif defined(HAVE_TESORERIAMODULE)
	str_cuenta_origen = formapago->getValue("CUENTATESORERIA.NOMBRE").toString();
#endif
    uint nrecibos = formapago->getValue("NUMPLAZOS").toInt();
    if( nrecibos == 0 ) {
        _GONG_DEBUG_WARNING( "La forma de pago tiene 0 plazos" );
        nrecibos = 1;
    }
    uint diasprimerplazo = formapago->getValue("DIASPRIMERPLAZO").toInt();
    uint diasentreplazos = formapago->getValue("DIASENTREPLAZOS").toInt();
    // Si esto es una factura y tiene un albarán del que ya se han generado pagos, se volverían a generar los pagos de ese albarán
    // Hay que detectar esto y restarle al resto de esta factura el importe de los pagos de ese albarán
    // Soluciones:
    // a) modificar el resto de la factura al añadir un albarán: es decir,

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
		setValuesFactura(recibo, pFactura);
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
        recibo->setValue( "NUMERO", recibo->selectNextInt("NUMERO", Xtring::null, false /*dont use gaps*/) );
        recibo->setValue( "NUMEROAGRUPADO", recibo->getValue("NUMERO") );
        recibo->setValue( "FECHAEMISION", pFactura->getValue("FECHA").toDate() );
        recibo->setValue( "VENCIMIENTO", vencimiento );
        recibo->setValue( "FECHAVALOR", fechavalor );
        recibo->setValue( "IMPORTE", importe );
        recibo->setValue( "RESTO", importe );
        recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPendiente );
        recibo->setValue( "CUENTAORIGEN", str_cuenta_origen );
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
    if( nrecibos ) {
        DBAPP->showStickyOSD( pFactura->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                              Xtring::printf(_("Se han generado <b>%d</b> recibos"), nrecibos) );
		DBAPP->getMainWindow()->refreshByName( Xtring::null, mTablaPagos );
	}
    return nrecibos;
}


bool IPagableRecord::pagarRecibo( FrmEditRecMaster *parent, dbRecordID reciboid,
                                  dbRecord *recibo, bool supervisar )
{
    bool pagado = false;
    Money importe = 0.0;
    Money pago = 0.0;
    bool has_contab = false;
    Xtring que_es;
    Xtring tercero_field = getRecTercero()->getTableName() + "_ID";
    if( mTipo == pagos ) {
        que_es = DBAPP->getTableDescSingular( mTablaPagos, "" ) + " ";
    } else {
        que_es = DBAPP->getTableDescSingular( mTablaPagos, "" ) + " ";
    }
    bool accepted = true, pago_multiple = false;
    Xtring numeroagrupado = recibo->getValue( "NUMEROAGRUPADO" ).toString();
    if( numeroagrupado.isEmpty() )
        numeroagrupado = recibo->getValue( "NUMERO" ).toString();
    dbRecordID asientoid = 0, cuentapago_id;
#ifdef HAVE_CONTABMODULE
    has_contab = contab::ModuleInstance->isContabActive()
                 && recibo->getTableDefinition()->findFieldDefinition( "CUENTAPAGO_ID" );
    dbRecord *cuentapago = 0;
    contab::Cuenta cuenta_pago( contab::ModuleInstance->getDigitosTrabajo() );
    contab::Cuenta cuenta_pago_contraida( cuenta_pago );
    contab::RecCuenta *cuenta_origen = 0;
    dbRecordID proyecto_id = 0;
    if( has_contab ) {
        cuentapago = recibo->findRelatedRecord( "CUENTAPAGO_ID" );
        proyecto_id = getProyectoID();
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
                              Xtring::printf( _("No se puede realizar el asiento de %s porque la cuenta de origen '%s' no existe"),
                                              que_es.lower().c_str(), str_cuenta_origen.c_str()) );
            delete cuenta_origen;
            return false;
        }
    }
#endif
#ifdef HAVE_TESORERIAMODULE
    dbRecordID proyecto_id = 0;
    has_contab = DBAPP->findModule("tesoreria") && recibo->getTableDefinition()->findFieldDefinition("CUENTAPAGO_ID");
	if( has_contab ) {
		if( (cuentapago_id = recibo->getValue("CUENTAPAGO_ID").toInt()) != 0 ) {
			PagosModule::sLastCuentaPago = recibo->getValue( "CUENTATESORERIA.NOMBRE" ).toString();
		} else {
			PagosModule::sLastCuentaPago = recibo->getValue( "CUENTAORIGEN" ).toString();
		}
	}
#endif
    while( reciboid ) {
        recibo->read( reciboid );
        importe = recibo->getValue( "IMPORTE" ).toMoney();
        if( pago == 0.0 )
            pago = importe;
        dbRecordID moneda_id;
        Date fechapago;
        fechapago = recibo->getValue( "VENCIMIENTO" ).toDate();
        PagosModule::sLastMonedaCodigo = recibo->getValue( "MONEDA.CODIGO" ).toString();
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
				cuentapago_id = pr->getCuentaPagoID();
            }
#endif
#ifdef HAVE_TESORERIAMODULE
            if( has_contab ) {
				PagosModule::sLastCuentaPago = pr->getCuentaPago();
				cuentapago_id = pr->getCuentaPagoID();
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
				
				// Pagar el recibo normalmente, el pago coincide con el importe del recibo
                if( pago == importe && asientoid == 0) {
					recibo->setValue( "MONEDA_ID", moneda_id );
					recibo->setValue( "RATIO_MONEDA", recibo->getValue("MONEDA.RATIO") );
                    recibo->setValue( "NUMEROAGRUPADO", numeroagrupado ); // Solo este
                    recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPagado );
                    recibo->setValue( "RESTO", Money(0.0) );
                    recibo->setValue( "DOCUMENTOPAGO", PagosModule::sLastDocumentoPago );
#if defined (HAVE_CONTABMODULE) || defined (HAVE_TESORERIAMODULE)
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
#ifdef HAVE_TESORERIAMODULE
					if( has_contab ) {
						if( DBAPP->findModule("tesoreria") ) {
							bool supervisar = tesoreria::ModuleInstance->getModuleSetting( "SUPERVISAR_APUNTES" ).toBool();
							dynamic_cast<tesoreria::IApuntableRecord *>(recibo)->regenApunte( supervisar );
						}
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
                        answer = FrmBase::msgYesNoCancel( parent, question + _("\n¿Quieres continuar?"));
                    switch( answer ) {
                    case FrmBase::Yes: {
                        // Pagar este recibo con el importe introducido
						recibo->setValue( "MONEDA_ID", moneda_id );
						recibo->setValue( "RATIO_MONEDA", recibo->getValue( "MONEDA.RATIO" ).toDouble() );
                        recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPagado );
                        recibo->setValue( "IMPORTE", pago );
                        recibo->setValue( "RESTO", 0.0 );
                        recibo->setValue( "DOCUMENTOPAGO", PagosModule::sLastDocumentoPago );
#if defined (HAVE_CONTABMODULE) || defined (HAVE_TESORERIAMODULE)
                        Xtring cuentaorigen = recibo->getValue( "CUENTAORIGEN" ).toString();
                        recibo->setValue( "CUENTAPAGO_ID", cuentapago_id );
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
                                asiento->save(true);
                                delete asiento;
                            }
                        }
#endif
#ifdef HAVE_TESORERIAMODULE
						if( has_contab ) {
							if( DBAPP->findModule("tesoreria") ) {
								bool supervisar = tesoreria::ModuleInstance->getModuleSetting( "SUPERVISAR_APUNTES" ).toBool();
								dynamic_cast<tesoreria::IApuntableRecord *>(recibo)->regenApunte( supervisar );
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
                        recibo->setValue( "NUMEROAGRUPADO", numeroagrupado );
                        recibo->setNullValue( "FECHAPAGO" );
#ifdef HAVE_CONTABMODULE
                        recibo->setNullValue( "ASIENTO_PAGO_ID" );
#elif defined(HAVE_TESORERIAMODULE)
						recibo->setNullValue( "APUNTE_ID" );
#endif						
#if defined (HAVE_CONTABMODULE) || defined (HAVE_TESORERIAMODULE)
                        recibo->setValue( "CUENTAORIGEN", cuentaorigen );
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
						recibo->setValue( "MONEDA_ID", moneda_id );
						recibo->setValue( "RATIO_MONEDA", recibo->getValue( "MONEDA.RATIO" ).toDouble() );
                        recibo->setValue( "NUMEROAGRUPADO", numeroagrupado );
                        recibo->setValue( "ESTADORECIBO", PagosModule::ReciboPagado );
                        recibo->setValue( "RESTO", 0 );
                        recibo->setValue( "DOCUMENTOPAGO", PagosModule::sLastDocumentoPago );
#if defined (HAVE_CONTABMODULE) || defined (HAVE_TESORERIAMODULE)
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
                                                  we, 0, fechapago, recibo->getIDSqlCond(),
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
#ifdef HAVE_TESORERIAMODULE
						if( has_contab ) {
							if( DBAPP->findModule("tesoreria") ) {
								bool supervisar = tesoreria::ModuleInstance->getModuleSetting( "SUPERVISAR_APUNTES" ).toBool();
								dynamic_cast<tesoreria::IApuntableRecord *>(recibo)->regenApunte( supervisar );
							}
						}
#endif
                        pago = pago - importe;
                        if( pago <= 0.0 ) {
                            reciboid = 0; // break while
                        } else {
                            // Elegir otro recibo para pagar el resto del pago
                            dbViewDefinitionDict views;
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
	parent->refreshRelatedForms();
	if( DBAPP->getMainWindow() )  {
#ifdef HAVE_TESORERIAMODULE
		DBAPP->getMainWindow()->refreshByName( parent->name(), 
			dynamic_cast<tesoreria::IApuntableRecord *>(recibo)->getTablaApunteTesoreria() );
#endif	
		DBAPP->getMainWindow()->refreshByName( parent->name(), pFactura->getTableName() );
		DBAPP->getMainWindow()->refreshByName( parent->name(), mTablaPagos );
	}
	return true;
}

bool IPagableRecord::anularPagoRecibo( FrmEditRecMaster* parent, dbRecordID reciboid,
                                       dbRecord* recibo )
{
    if( recibo->getValue( "ESTADORECIBO" ).toInt() == PagosModule::ReciboPagado ) {
        bool anularpago = false;
        bool pideanularpago = true;
#ifdef HAVE_CONTABMODULE
        bool has_contab = false;
        has_contab = contab::ModuleInstance->isContabActive()
                     && recibo->getTableDefinition()->findFieldDefinition( "CUENTAPAGO_ID" );
        if( has_contab ) {
            dbRecord *cuentapago;
            if( mTipo == pagos ) {
                cuentapago = static_cast<RecPago *>(recibo)->getRecCuentaPago();
            } else {
                cuentapago = static_cast<RecCobro *>(recibo)->getRecCuentaPago();
            }
            anularpago = FrmBase::msgYesNoCancel( parent,
                                                  Xtring::printf( _("¿Estás segura de borrar este pago realizado el día %s por un importe de %f,\n"
                                                          " por la cuenta '%s' y mediante el documento '%s'?"),
                                                          recibo->getValue( "FECHAPAGO" ).toDate().toString().c_str(),
                                                          recibo->getValue( "IMPORTE" ).toMoney().toDouble(),
                                                          cuentapago->getValue( "DESCRIPCION" ).toString().c_str(),
                                                          recibo->getValue( "DOCUMENTOPAGO").toString().c_str() ) ) == FrmBase::Yes;
            pideanularpago = false;
        }
#endif
#ifdef HAVE_TESORERIAMODULE
        bool has_contab = false;
		tesoreria::IApuntableRecord *apuntable = dynamic_cast<tesoreria::IApuntableRecord *>(recibo);
        has_contab = DBAPP->findModule("TESORERIA") && apuntable;
        if( has_contab ) {
            dbRecord *cuentapago;
            if( mTipo == pagos ) {
                cuentapago = static_cast<RecPago *>(recibo)->getRecCuentaPago();
            } else {
                cuentapago = static_cast<RecCobro *>(recibo)->getRecCuentaPago();
            }
            anularpago = FrmBase::msgYesNoCancel( parent,
                                                  Xtring::printf( _("¿Estás segura de borrar este pago realizado el día %s por un importe de %f,\n"
                                                          " por la cuenta '%s' y mediante el documento '%s'?"),
                                                          recibo->getValue( "FECHAPAGO" ).toDate().toString().c_str(),
                                                          recibo->getValue( "IMPORTE" ).toMoney().toDouble(),
                                                          cuentapago->getValue( "NOMBRE" ).toString().c_str(),
                                                          recibo->getValue( "DOCUMENTOPAGO").toString().c_str() ) ) == FrmBase::Yes;
            pideanularpago = false;
        }
#endif
        if( pideanularpago )
            anularpago = FrmBase::msgYesNoCancel( parent,
                                                  Xtring::printf( _("¿Estás segura de borrar este pago realizado el día %s por un importe de %f,\n"
                                                          "y mediante el documento '%s'?"),
                                                          recibo->getValue( "FECHAPAGO" ).toDate().toString().c_str(),
                                                          recibo->getValue( "IMPORTE" ).toMoney().toDouble(),
                                                          recibo->getValue( "DOCUMENTOPAGO").toString().c_str() ) ) == FrmBase::Yes;
        if( anularpago ) {
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
                            } catch( dbError &e ) {
                                FrmBase::msgError( parent,
                                                   Xtring(e.what()) + _(". Borra el asiento a mano y vuelve a intentarlo."));
                                anulapago = false;
                            }
                            break;
                            case FrmBase::Cancel:
                                return false;
                            }
                        }
                    }
                }
            }
#endif
#ifdef HAVE_TESORERIAMODULE
			bool borradoapunte = false;
            if( has_contab ) {
                dbRecordID apunteid = recibo->getValue( apuntable->getRecordApunteIDField() ).toInt();
                if( apunteid ) {
					_GONG_DEBUG_PRINT(0, dynamic_cast<tesoreria::IApuntableRecord *>(recibo)->getTablaApunteTesoreria());
					dbRecord *apunte = DBAPP->createRecord( apuntable->getTablaApunteTesoreria() );
                    if( apunte->read( apunteid ) ) {
						try {
							if( !(borradoapunte = apunte->remove()) )
								anulapago = false;
						} catch( dbError &e ) {
							FrmBase::msgError( parent,
												Xtring(e.what()) + _(". Borra el apunte a mano y vuelve a intentarlo."));
							anulapago = false;
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
                // Puede que dé error de que no existen, pero mejor que lo de a que se quede un valor erróneo
                recibo->setNullValue( "ASIENTO_PAGO_ID" );
                recibo->setNullValue( "CUENTAPAGO_ID" );
                if( has_contab ) {
                    if( mTipo == pagos )
                        PagosModule::sLastCuentaPago = static_cast<RecPago *>(recibo)->getRecCuentaPago()->getValue( "CUENTA" ).toString();
                    else
                        PagosModule::sLastCuentaPago = static_cast<RecCobro *>(recibo)->getRecCuentaPago()->getValue( "CUENTA" ).toString();
                }
#endif
#ifdef HAVE_TESORERIAMODULE
                // Puede que dé error de que no existen, pero mejor que lo de a que se quede un valor erróneo
                recibo->setNullValue( apuntable->getRecordApunteIDField() );
                recibo->setNullValue( apuntable->getRecordCuentaTesoreriaIDField() );
                if( has_contab ) {
                    if( mTipo == pagos )
                        PagosModule::sLastCuentaPago = static_cast<RecPago *>(recibo)->getRecCuentaPago()->getValue( "CODIGO" ).toString();
                    else
                        PagosModule::sLastCuentaPago = static_cast<RecCobro *>(recibo)->getRecCuentaPago()->getValue( "CODIGO" ).toString();
                }
#endif                
                try {
                    if( recibo->save(false) ) {
                        actRestoFactura();
						DBAPP->showStickyOSD( recibo->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                                  Xtring::printf( _("Se ha anulado el pago") ) );
#ifdef HAVE_TESORERIAMODULE
						if (borradoapunte) {
							DBAPP->showStickyOSD( recibo->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                                  Xtring::printf( _("Se ha borrado el apunte en tesorería") ) );
						}
#endif										
					}
                } catch( dbError &e ) {
                    FrmBase::msgError( parent, e.what() );
                }
            }
        }
		parent->refreshRelatedForms();
		DBAPP->getMainWindow()->refreshByName( parent->name(), mTablaPagos );
		DBAPP->getMainWindow()->refreshByName( parent->name(), pFactura->getTableName() );
#ifdef HAVE_TESORERIAMODULE
		DBAPP->getMainWindow()->refreshByName( parent->name(), dynamic_cast<tesoreria::IApuntableRecord *>(recibo)->getTablaApunteTesoreria() );
#endif	
    }
    return true;
}

// Por si en alguna tabla no queremos el lío de tablafacturas
Xtring IPagableRecord::getFacturaWhere() const
{
    return "TABLAFACTURAS=" + pFactura->getConnection()->toSQL( pFactura->getTableName() )
           + " AND FACTURA_ID= " + pFactura->getConnection()->toSQL( pFactura->getRecordID() );
}

void IPagableRecord::setValuesFactura(dbRecord* recibo, const dbRecord* factura)
{
	recibo->setValue( "TABLAFACTURAS", factura->getTableName() );
	recibo->setValue( "FACTURA_ID", factura->getRecordID() );
}


} // namespace pagos
} // namespace gong

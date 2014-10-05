#ifndef _PAGOS_IPAGABLERECORD_H
#define _PAGOS_IPAGABLERECORD_H

#include <gongdbrecord.h>
#include <pagosmodule.h>

namespace gong {

class FrmEditRecMaster;

namespace pagos {

class RecFormaPago;

class IPagableRecord
{
public:
    enum Tipo { pagos, cobros };
    IPagableRecord( dbRecord *factura, IPagableRecord::Tipo t, const Xtring &total_field,
                    const Xtring &pagos_field, const Xtring &resto_field, const XtringList &other_pagos_fields,
                    const Xtring &tablapagos = Xtring::null );
    virtual dbRecordID getProyectoID() const = 0;
    virtual dbRecord *getRecTercero() const = 0;
    dbRecordList *getPagos( PagosModule::EstadoRecibo estado = PagosModule::ReciboSinEstado );
    dbRecordList *getCobros( PagosModule::EstadoRecibo estado = PagosModule::ReciboSinEstado ) {
        return getPagos(estado);
    }
#ifdef HAVE_CONTABMODULE
    virtual Xtring getPrefijoContableTercero() const = 0;
#endif
    void actRestoFactura();
    int hasPagos( int estado_si = -1, int estado_no = -1, bool soloautomaticos = false );
    int hasCobros( int estado_si = -1, int estado_no = -1, bool soloautomaticos = false ) {
        return hasPagos( estado_si, estado_no, soloautomaticos);
    }
    int genPagos();
    int genCobros() {
        return genPagos();
    }
    bool actPagos();
    bool actCobros() {
        return actPagos();
    }
    bool delPagos( bool borratodos );
    bool delCobros( bool borratodos ) {
        return delPagos( borratodos );
    }
    void pagarRecibo( FrmEditRecMaster *parent, dbRecordID reciboid, dbRecord *recibo, bool supervisar );
    void cobrarRecibo( FrmEditRecMaster *parent, dbRecordID reciboid, dbRecord *recibo, bool supervisar ) {
        return pagarRecibo( parent, reciboid, recibo, supervisar );
    }
    void anularPagoRecibo( FrmEditRecMaster* parent, dbRecordID reciboid, dbRecord* recibo );
    void anularCobroRecibo( FrmEditRecMaster* parent, dbRecordID reciboid, dbRecord* recibo ) {
        anularPagoRecibo( parent, reciboid, recibo );
    }
    dbRecord *createRecibo();
    Xtring getFacturaWhere() const;
    const Xtring &getPagosField() const {
        return mPagosField;
    }
    const Xtring &getTotalField() const {
        return mTotalField;
    }
    const Xtring &getRestoField() const {
        return mRestoField;
    }
    const XtringList &getOtherPagosFields() const {
        return mOtherPagosFields;
    }
    void recibo(bool arg1);
protected:
    dbRecord *pFactura;
    Tipo mTipo;
    Xtring mTotalField, mPagosField, mRestoField, mPrefijoContableTercero, mTablaPagos;
    XtringList mOtherPagosFields;
    dbRecord *pRecTercero;
};



} // namesace pagos
} // namesace gong

#endif // _PAGOS_IPAGABLERECORD_H

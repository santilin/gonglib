#include <empresamodule.cpp>
#include <pagosrecformapago.h>
#include "factufldivadetallado.h"
#include "factuiivadesglosable.h"
#include "factuitotalizablerecord.h"

namespace gong {
namespace factu {

XtringList ITotalizableRecord::sTotalFields = XtringList()
        << "TOTAL" << "SUMAIMPORTES" << "BASEIMPONIBLE" << "DTOP100" << "DESCUENTO" << "RECARGOEQUIVALENCIA"
        << "IVA" << "DESGLOSEIVA" << "ENTREGA" << "ENTREGAALBARANES" << "RESTO" << "IVADETALLADO";


ITotalizableRecord::ITotalizableRecord( dbRecord *record, dbRecordList *detalles, Tipo tipo )
    : pRecord( record ), pDetalles( detalles ), mTipo( tipo )
{
}

int ITotalizableRecord::getIvaDetallado() const
{
    return pRecord->getValue("IVADETALLADO").toInt();
}

void ITotalizableRecord::gather( FrmEditRec *frm )
{
    for( XtringList::const_iterator it = sTotalFields.begin();
            it != sTotalFields.end(); ++it )
        pRecord->setValue( *it, frm->getControlValue( *it ) );
}

void ITotalizableRecord::scatter( FrmEditRec *frm )
{
    for( XtringList::const_iterator it = sTotalFields.begin();
            it != sTotalFields.end(); ++it )
        frm->setControlValue( *it, pRecord->getValue( *it ) );
}

void ITotalizableRecord::actTotales()
{
    int ndecmoneda = empresa::ModuleInstance->getDecimalesMoneda();
    int ndecround = ndecmoneda;
    if( getIvaDetallado() == FldIVADetallado::incluido) {
        ndecround = ndecmoneda + 2;
    }
    double sumaimportes = 0.0, baseimponible = 0.0, iva = 0.0, recargo = 0.0, total;
    for ( unsigned int i = 0; i < pDetalles->size(); i++ ) {
        dbRecord *detalle = pDetalles->at( i );
        double precio;
        if( mTipo == venta )
            precio = detalle->getValue("PVPSINIVA").toDouble();
        else
            precio = detalle->getValue("COSTESINIVA").toDouble();
        double importe = round( detalle->getValue("CANTIDAD").toDouble()
                                * precio * ( 1 - detalle->getValue("DTOP100").toDouble() / 100), ndecround);
        sumaimportes += importe;
        baseimponible += importe;
        iva += round( importe * detalle->getValue("TIPOIVA.IVA").toDouble() / 100, ndecround );
        recargo += round( importe *detalle->getValue("TIPOIVA.RECARGO").toDouble() / 100, ndecround );
    }
    pRecord->setValue( "SUMAIMPORTES", Money(sumaimportes, ndecround) );
    double dtop100 = pRecord->getValue( "DTOP100" ).toDouble();
    double descuento = pRecord->getValue( "DESCUENTO" ).toDouble();
    descuento = sumaimportes * dtop100 / 100;
    pRecord->setValue( "DESCUENTO", Money(descuento, 6) );
    pRecord->setValue( "DTOP100", dtop100 );
    if( dtop100 != 0.0 ) {
        iva -= iva * dtop100 / 100;
        recargo -= recargo * dtop100 / 100;
    }
    baseimponible = sumaimportes - descuento;
    if( getIvaDetallado() == FldIVADetallado::sin_iva )
        iva = recargo = 0;
    else if( getIvaDetallado() != FldIVADetallado::con_recargo )
        recargo = 0;
    total = baseimponible + iva + recargo;
    pRecord->setValue( "BASEIMPONIBLE", Money(baseimponible, ndecmoneda) );
    pRecord->setValue( "IVA", Money(iva, ndecmoneda) );
    pRecord->setValue( "RECARGOEQUIVALENCIA", Money(recargo, ndecmoneda) );
    pRecord->setValue( "TOTAL", Money(total, ndecmoneda) );

    double suma_entregas = 0.0;
    Xtring tbl_albaran, fld_pagos;
    if( mTipo == compra ) {
        fld_pagos = "PAGOS";
        tbl_albaran = "ALBARANCOMPRA";
    } else {
        fld_pagos = "COBROS";
        tbl_albaran = "ALBARANVENTA";
    }
    if( pDetalles->size() &&
            pRecord->getTableDefinition()->findFieldDefinition( "ENTREGAALBARANES", false ) ) {
        // TODO: optimizar esto para que sume de una lista de entregas en el formulario para no tener que leerlos cada vez que se actualizan los totales
        dbRecordID old_alb_id = 0, alb_id = 0;
        dbRecord *albaran = DBAPP->createRecord( tbl_albaran );
        for ( unsigned int ii = 0; ii < pDetalles->size(); ii++ ) {
            dbRecord *detalle = pDetalles->at( ii );
            alb_id = detalle->getValue( tbl_albaran + "_ID" ).toInt();
            if( alb_id != 0 && alb_id != old_alb_id ) {
                old_alb_id = alb_id;
                if( albaran->read( alb_id ) ) {
                    suma_entregas += albaran->getValue( "ENTREGA" ).toDouble() + albaran->getValue( fld_pagos ).toDouble();
                }
            }
        }
        delete albaran;
        pRecord->setValue( "ENTREGAALBARANES", Money(suma_entregas) );
    }
    double entrega = 0.0, cobros = 0.0;
    if( pRecord->getTableDefinition()->findFieldDefinition( "ENTREGA" ) )
        entrega = pRecord->getValue( "ENTREGA" ).toDouble();
    suma_entregas += entrega;
    if( pRecord->getTableDefinition()->findFieldDefinition( fld_pagos ) ) {
        cobros = pRecord->getValue( fld_pagos ).toDouble();
        suma_entregas += cobros;
    }
    double resto = total - suma_entregas;
    if( pRecord->getTableDefinition()->findFieldDefinition( "FORMAPAGO_ID" ) ) {
        if( pRecord->getValue( "FORMAPAGO.TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::Contado ) {
            entrega = resto;
            resto = 0.0;
        } else if( pRecord->getValue( "FORMAPAGO.TIPOFORMAPAGO" ).toInt() == pagos::RecFormaPago::SeIgnora ) {
            entrega = 0.0;
            resto = 0.0;
        }
    } else {
        resto -= entrega;
    }
    if( pRecord->getTableDefinition()->findFieldDefinition( "ENTREGA" ) )
        pRecord->setValue( "ENTREGA", Money(entrega, ndecmoneda) );
    if( pRecord->getTableDefinition()->findFieldDefinition( "RESTO" ) )
        pRecord->setValue( "RESTO", Money(resto, ndecmoneda) );
    if( IIVADesglosable *idr = dynamic_cast<IIVADesglosable *>(pRecord) )
        pRecord->setValue( "DESGLOSEIVA", idr->calcDesgloseIVA() );
	_GONG_DEBUG_WARNING( resto );
}

} // namesace factu
} // namesace gong

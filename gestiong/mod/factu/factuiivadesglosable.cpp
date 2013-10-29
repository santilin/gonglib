#include <gongdbrecordlist.h>
#include "factuiivadesglosable.h"
#include "factufldivadetallado.h"
#include "empresarectipoiva.h"

namespace gong {
namespace factu {

Xtring IIVADesglosable::calcDesgloseIVA(const Xtring& format, const Xtring& sep)
{
    Xtring desglose;
    // Sumar los IVA y recargos de la factura
    if( pFactura->getValue( "IVADETALLADO" ).toInt() != FldIVADetallado::sin_iva ) {
        double dto = pFactura->getValue( "DTOP100" ).toDouble();
        Dictionary<Money> ivas;
        Dictionary<Money> recargos;
        for( dbRecordList::const_iterator it = pDetalles->begin();
                it != pDetalles->end(); ++ it ) {
            dbRecord *factudet = *it;
            empresa::RecTipoIVA *reciva = static_cast<empresa::RecTipoIVA *>(
                                              factudet->findRelatedRecord( "TIPOIVA_ID" ));
            Xtring desc_iva = Xtring::printf("%s %s %s",
                                             reciva->getValue( "CODIGO" ).toString().c_str(),
                                             reciva->getValue( "IVA" ).toString().c_str(),
                                             reciva->getValue( "RECARGO" ).toString().c_str() );
            ivas.set( desc_iva,
                      ivas[desc_iva] + reciva->calcIVA( factudet->getValue("IMPORTE").toMoney() * ( 1 - dto/100) ));
            recargos.set( desc_iva,
                          recargos[desc_iva] + reciva->calcRecargo( factudet->getValue("IMPORTE").toMoney() * (1 -dto/100) ));
        }
        Formatter f(*DBAPP->getRegConfig());
        // Escribir los tipos y cantidades
        Dictionary<Money>::const_iterator itrecargos = recargos.begin();
        for( Dictionary<Money>::const_iterator itivas = ivas.begin();
                itivas != ivas.end(); ++ itivas, ++ itrecargos ) {
            if( itivas->second != 0.0 ) {
                XtringList desc_iva_parts;
                itivas->first.tokenize(desc_iva_parts, " ");
                desglose += Xtring::printf( format,
                                            f.format(itivas->second, "", "").c_str(),
                                            desc_iva_parts[1].c_str() );
                if( hasRecargo() ) {
                    desglose += Xtring::printf( format,
                                                f.format(itrecargos->second, "", "").c_str(),
                                                desc_iva_parts[2].c_str() );
                }
                desglose += sep;
            }
        }
    }
    return desglose;
}

bool IIVADesglosable::hasRecargo() const
{
    return pFactura->getValue( "IVADETALLADO" ).toInt() == FldIVADetallado::con_recargo;
}

} // namespace factu
} // namespace gong

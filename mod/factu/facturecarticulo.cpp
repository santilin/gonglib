/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de artículos
// MEMBER isValid
// RELATION Familia
// RELATION empresa::TipoIVA
// RELATION Proveedora
// RELATION ArticuloImagen
// TYPE dbRecord factu::Articulo
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
#include "factumodule.h"
/*<<<<<ARTICULO_INCLUDES*/
#include "facturecarticulo.h"
/*>>>>>ARTICULO_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<ARTICULO_RELATIONS*/
RecFamilia *RecArticulo::getRecFamilia() const
{
	return static_cast<RecFamilia*>(findRelatedRecord("FAMILIA_ID"));
}

empresa::RecTipoIVA *RecArticulo::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

RecProveedora *RecArticulo::getRecProveedora() const
{
	return static_cast<RecProveedora*>(findRelatedRecord("PROVEEDORA_ID"));
}

RecArticuloImagen *RecArticulo::getRecArticuloImagen() const
{
	return static_cast<RecArticuloImagen*>(findRelatedRecord("ARTICULOIMAGEN_ID"));
}

/*>>>>>ARTICULO_RELATIONS*/


double RecArticulo::masIVA( double cantidad ) const
{
    double iva = getValue( "TIPOIVA.IVA" ).toDouble();
    return cantidad * (1 + iva / 100);
}

double RecArticulo::menosIVA(double cantidad) const
{
    double iva = getValue( "TIPOIVA.IVA" ).toDouble();
    if( iva != -100.0 )
        return cantidad / (1 + iva / 100);
    else
        return 0;
}

double RecArticulo::getPVP(int tarifacliente) const
{
    if( tarifacliente == 0 )
        return getValue( "PVP" ).toDouble();
    else
        return getValue( "PVP" + Xtring::number(tarifacliente) ).toDouble();
}

void RecArticulo::setPVP(int tarifacliente, double pvp)
{
    if( tarifacliente == 0 )
        setValue( "PVP", pvp );
    else
        setValue( "PVP" + Xtring::number(tarifacliente), pvp );
}

void RecArticulo::actStocks( bool savingdetail, bool compra, double cantidad )
{
    _GONG_DEBUG_ASSERT( getRecordID() );
    read( getRecordID() ); // El artículo puede haber sido grabado anteriormente desde cualquier otro sitio
    double cantidadoriginal = cantidad;
    if( getValue( "USARSTOCKS" ).toBool() ) {
        if( !compra )
            cantidad = -cantidad;
        if( !savingdetail )
            cantidad = -cantidad;
        if( cantidad != 0 )
            setValue( "STOCK", getValue( "STOCK" ).toDouble() + cantidad );
        if( getValue( "ARTICULOBASE_ID" ).toInt() != 0 ) {
            RecArticulo *base = static_cast<RecArticulo *>(DBAPP->createRecord( this ));
            if( base->read( getValue( "ARTICULOBASE_ID" ).toInt() ) ) {
                cantidadoriginal *= getValue( "MULTIPLOBASE" ).toDouble();
                base->actStocks( savingdetail, compra, cantidadoriginal );
            } else {
                DBAPP->showOSD(_("Atención"),
                               Xtring::printf(_("El artículo base '%s' del artículo '%s' no existe"),
                                              base->getValue( "CODIGO" ).toString().c_str(),
                                              getValue( "CODIGO" ).toString().c_str() ) );
            }
        }
    }
    save( false );
}

void RecArticulo::fixMargenYDescuento()
{
    if( getValue("COSTESINIVA").toDouble() != 0.0 )
        setValue("MARGENCOMERCIAL",
                 (( getValue("PVPSINIVA").toDouble() / getValue("COSTESINIVA").toDouble() ) - 1 ) * 100 );
    else
        setValue("MARGENCOMERCIAL", 0.0 );
    if( getValue("PVPSINIVA").toDouble() != 0.0 )
        setValue("DTOCOMERCIAL",
                 -(( getValue("COSTESINIVA").toDouble() / getValue("PVPSINIVA").toDouble() ) - 1 ) * 100 );
    else
        setValue("DTOCOMERCIAL", 0.0 );
}

void RecArticulo::fixPrecios( const Xtring &fldchanged )
{
    if( fldchanged == "PVP" )
        setValue( "PVPSINIVA", Money() );
    else if( fldchanged == "PVPSINIVA" )
        setValue( "PVP", Money() );
    else if( fldchanged == "COSTE" )
        setValue( "COSTESINIVA", Money() );
    else if( fldchanged == "COSTESINIVA" )
        setValue( "COSTE", Money() );
    else if( fldchanged == "DTOCOMERCIAL" ) {
        setValue( "COSTE", Money() );
        setValue( "COSTESINIVA", Money() );
    } else if( fldchanged == "MARGENCOMERCIAL" ) {
        setValue( "PVP", Money() );
        setValue( "PVPSINIVA", Money() );
    }
    // Calcular los correspondientes precios con o sin IVA
    if( getValue( "PVPSINIVA" ).toDouble() != 0.0 )
        setValue( "PVP", Money( getValue( "PVPSINIVA" ).toDouble()
                                * ( 1 + ( getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 ) ), 2 ) );
    else if( getValue( "PVP" ).toDouble() != 0.0 )
        setValue( "PVPSINIVA", Money( getValue( "PVP" ).toDouble()
                                      / ( 1 + ( getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 ) ), 4 ) );
    if( getValue( "COSTESINIVA" ).toDouble() != 0.0 )
        setValue( "COSTE", Money( getValue( "COSTESINIVA" ).toDouble()
                                  * ( 1 + ( getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 ) ), 2 ) );
    else if( getValue( "COSTE" ).toDouble() != 0.0 )
        setValue( "COSTESINIVA", getRecTipoIVA()->menosIVA( Money(getValue( "COSTE" ).toDouble(), 4) ) );
    // Calcular los márgenes y descuentos si es posible
    if( getValue( "PVP" ).toDouble() != 0.0 && getValue( "COSTE" ).toDouble() != 0.0 ) {
        setValue( "PVPSINIVA", Money( getValue( "PVP" ).toDouble()
                                      / ( 1 + ( getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 ) ), 4 ) );
        setValue( "COSTESINIVA", getRecTipoIVA()->menosIVA( Money(getValue( "COSTE" ).toDouble(), 4 ) ) );
        setValue( "MARGENCOMERCIAL",
                  (( getValue( "PVPSINIVA" ).toDouble() / getValue( "COSTESINIVA" ).toDouble() ) - 1 ) * 100 );
        setValue( "DTOCOMERCIAL",
                  -(( getValue( "COSTESINIVA" ).toDouble() / getValue( "PVPSINIVA" ).toDouble() ) - 1 ) * 100 );
        return;
    }
    if( fldchanged != "MARGENCOMERCIAL" && getValue( "PVPSINIVA" ).toDouble() == 0.0 && getValue( "COSTESINIVA" ).toDouble() != 0.0 ) {
        setValue( "DTOCOMERCIAL", 0.0 );
        setValue( "MARGENCOMERCIAL", 100.0 );
        return;
    }
    if( fldchanged != "DTOCOMERCIAL" && getValue( "PVPSINIVA" ).toDouble() != 0.0 && getValue( "COSTESINIVA" ).toDouble() == 0.0 ) {
        setValue( "DTOCOMERCIAL", 100.0 );
        setValue( "MARGENCOMERCIAL", 0.0 );
        return;
    }
    // Si hay PVP y descuento comercial, calcular los costes
    if( getValue( "PVP" ).toDouble() != 0.0 && getValue( "DTOCOMERCIAL" ).toDouble() != 0.0 ) {
        setValue( "PVPSINIVA", Money( getValue( "PVP" ).toDouble()
                                      / ( 1 + ( getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 ) ), 4 ) );
        setValue( "COSTE", Money(getValue( "PVP" ).toDouble()
                                 * ( 1 - ( getValue( "DTOCOMERCIAL" ).toDouble() / 100 ) ) ) );
        setValue( "COSTESINIVA", getRecTipoIVA()->menosIVA( Money(getValue( "COSTE" ).toDouble(), 4 ) ) );
        setValue( "MARGENCOMERCIAL",
                  (( getValue( "PVPSINIVA" ).toDouble() / getValue( "COSTESINIVA" ).toDouble() ) - 1 ) * 100 );
        return;
    }
    if( getValue( "COSTE" ).toDouble() != 0.0 && getValue( "MARGENCOMERCIAL" ).toDouble() != 0.0 ) {
        setValue( "COSTESINIVA", getRecTipoIVA()->menosIVA( Money(getValue( "COSTE" ).toDouble(), 4) ) );
        setValue( "PVPSINIVA", Money( getValue( "COSTESINIVA" ).toDouble()
                                      * ( 1 + ( getValue( "MARGENCOMERCIAL" ).toDouble() / 100 ) ), 4 ) );
        setValue( "PVP", getRecTipoIVA()->masIVA( getValue( "PVPSINIVA" ).toDouble() ) );
        setValue( "DTOCOMERCIAL",
                  -(( getValue( "COSTESINIVA" ).toDouble() / getValue( "PVPSINIVA" ).toDouble() ) - 1 ) * 100 );
        return;
    }
    if( getValue( "PVPSINIVA" ).toDouble() != 0.0 ) {
        setValue( "COSTESINIVA", Money(getValue( "PVPSINIVA" ).toDouble(), 4) );
        setValue( "COSTE", getValue( "PVP" ).toDouble() );
        setValue( "MARGENCOMERCIAL", 0.0 );
        setValue( "DTOCOMERCIAL", 0.0 );
    } else if( getValue( "COSTESINIVA" ).toDouble() != 0.0 ) {
        setValue( "PVPSINIVA", Money(getValue( "COSTESINIVA" ).toDouble(), 4) );
        setValue( "PVP", getValue( "COSTE" ).toDouble() );
        setValue( "MARGENCOMERCIAL", 0.0 );
        setValue( "DTOCOMERCIAL", 0.0 );
    }
}


XtringList RecArticulo::sModalidades = XtringList()
                                       << _("Según formato")
                                       << _("Siguiente código de la proveedora")
                                       << _("Siguiente código de la familia")
                                       << _("Siguiente código de la familia y la proveedora")
                                       << _("Siguiente código global de los artículos")
									   << _("Código de barras") 
									   << _("ISBN")
									   << _("Código QR");

Xtring RecArticulo::genCodigoArticulo(int modalidad, const Xtring &formato ) const
{
    Xtring codart, last;
	Xtring from, where;
	Xtring buscar_formato = formatCodigoArticulo( "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%", formato );
	// Buscar el último código
	switch( modalidad ) {
	case GenCodArtFijo:
		break;
	case GenCodArtNextProv:
		from = "INNER JOIN PROVEEDORA ON PROVEEDORA.ID=ARTICULO.PROVEEDORA_ID";
		where = getConnection()->toSQLLikeLiteral( "ARTICULO.CODIGO", buscar_formato )
				+ " AND PROVEEDORA.CODIGO=" + getConnection()->toSQL( getValue( "PROVEEDORA.CODIGO" ).toInt() );
		break;
	case GenCodArtNextFam:
		from = "INNER JOIN FAMILIA ON FAMILIA.ID=ARTICULO.FAMILIA_ID";
		where = getConnection()->toSQLLikeLiteral( "ARTICULO.CODIGO", buscar_formato )
				+ " AND FAMILIA.CODIGO=" + getConnection()->toSQL( getValue( "FAMILIA.CODIGO" ).toInt() );
		break;
	case GenCodArtNextProvFam:
		from = "INNER JOIN FAMILIA ON FAMILIA.ID=ARTICULO.FAMILIA_ID"
				" INNER JOIN PROVEEDORA ON PROVEEDORA.ID=ARTICULO.PROVEEDORA_ID";
		where = getConnection()->toSQLLikeLiteral( "ARTICULO.CODIGO", buscar_formato )
				+ " AND FAMILIA.CODIGO=" + getConnection()->toSQL( getValue( "FAMILIA.CODIGO" ).toString() )
				+ " AND PROVEEDORA.CODIGO=" + getConnection()->toSQL( getValue( "PROVEEDORA.CODIGO" ).toInt() );
		break;
	case GenCodArtNextArt:
		break;
	}
	if( modalidad != GenCodArtFijo ) {
		where = getFilter( "WHERE", where );
		Xtring sql = "SELECT MAX(ARTICULO.CODIGO) FROM ARTICULO " + from + where;
		last = getConnection()->selectString( sql );
		if( !last.isEmpty() )
			return Xtring::stringInc( last );
		else
			last = "1";
	}
    return formatCodigoArticulo( last, formato );
}

Xtring RecArticulo::formatCodigoArticulo(const Xtring &last, const Xtring &formato) const
{
    int codempresa = empresa::ModuleInstance->getRecEmpresa()->getValue( "CODIGO" ).toInt();
    int ejercicio = empresa::ModuleInstance->getEjercicio();
    if( formato.isEmpty() && last.isEmpty() )
        return "";
    else if( formato.isEmpty() )
        return last;
    Xtring result;
    for( uint i = 0; i < formato.size(); ++ i ) {
        if( formato[i] == '{' ) {
            ++i;
            Xtring fieldandsize, field, size;
            while( formato[i] != '}' && i < formato.size() )
                fieldandsize += formato[i++];
            fieldandsize.upper().splitIn2( field, size, ":" );
            Xtring bit;
            char padding = size.startsWith('0') ? '0' : ' ';
            if( field == "EJE" || field == "EJERCICIO" ) {
                bit = Xtring::number( ejercicio );
            } else if( field == "EMP" || field == "EMPRESA" || field == "CODEMP" ) {
                bit = Xtring::number( codempresa );
			} else {
				Variant value = getValue(field);
				if( !value.isValid() ) {
					result += fieldandsize;
					continue;
				} else {
					bit = value.toString();
				}
            }
            if( size.toInt() != 0 )
                bit.padL( size.toInt(), padding );
            result += bit;
        } else {
            result += formato[i];
        }
    }
    return result.upper() + "%";
}

/*<<<<<ARTICULO_ISVALID*/
bool RecArticulo::isValid(ValidResult::Context context, ValidResult *result )
{
	bool ret = dbRecord::isValid(context, result);
/*>>>>>ARTICULO_ISVALID*/
    if( context == ValidResult::fixing ) {
        if( getValue( "COSTESINIVA" ).toDouble() != 0.0 )
            fixPrecios( "COSTESINIVA" );
        else if( getValue( "COSTE" ).toDouble() != 0.0 )
            fixPrecios( "COSTE" );
        else if( getValue( "PVP" ).toDouble() != 0.0 )
            fixPrecios( "PVP" );
        else if( getValue( "PVPSINIVA" ).toDouble() != 0.0 )
            fixPrecios( "PVPSINIVA" );
    }
    return ret;
}

} // namespace factu
} // namespace gong


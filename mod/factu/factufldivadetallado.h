/*<<<<<COPYLEFT*/
/** @file factufldivadetallado.h dbFieldDefinition que representa el modo de detallar el iva en las facturas, etc.
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<IVADETALLADO_PREAMBLE*/
#ifndef _FACTU_FACTUFLDIVADETALLADO_H
#define _FACTU_FACTUFLDIVADETALLADO_H
/*>>>>>IVADETALLADO_PREAMBLE*/

/*<<<<<DBFIELD_IVADETALLADO_INCLUDES*/
#include <gongdbfieldlistofvalues.h>
/*>>>>>DBFIELD_IVADETALLADO_INCLUDES*/

namespace gong {
namespace factu {

class FldIVADetallado: public dbFieldListOfValues<int>
{
public:
    enum IvaDetallado { desglosado = 1, con_recargo, sin_iva, incluido };
/*<<<<<DBFIELD_IVADETALLADO_CONSTRUCTOR*/
	FldIVADetallado(const Xtring &tablename, const Xtring &name,
		dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
		const Xtring &defaultvalue = Xtring::null)
/*>>>>>DBFIELD_IVADETALLADO_CONSTRUCTOR*/
        : dbFieldListOfValues<int>( false, &sCaptions, &sValues, tablename, name,
                                    SQLINTEGER, 4, 0, flags, defaultvalue )
    {
        setStyle( "TIPODOCUMENTO" );
    }

/*<<<<<DBFIELD_IVADETALLADO_VALUES$s/mValues/sValues/$*/
public:
	static List<int> sValues;
/*>>>>>DBFIELD_IVADETALLADO_VALUES*/
    static List<Xtring> sCaptions;
    static bool isPreciosConIVA( int ivadetallado );
};

/*<<<<<IVADETALLADO_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFLDIVADETALLADO_H
/*>>>>>IVADETALLADO_POSTAMBLE*/




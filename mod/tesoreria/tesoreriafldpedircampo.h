/*<<<<<PEDIRCAMPO_PREAMBLE*/
#ifndef _GASTOSTIPO_TESORERIAFLDPEDIRCAMPO_H
#define _GASTOSTIPO_TESORERIAFLDPEDIRCAMPO_H
/*>>>>>PEDIRCAMPO_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file tesoreriafldpedircampo.h dbFieldDefinition que representa los valores de PedirCampo
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

/*<<<<<DBFIELD_PEDIRCAMPO_INCLUDES*/
#include <gongdbfieldlistofvalues.h>
/*>>>>>DBFIELD_PEDIRCAMPO_INCLUDES*/

namespace gong {
namespace tesoreria {

class FldPedirCampo: public dbFieldListOfValues<int>
{
public:
    enum PedirCampo { Pedir = 0, FijarValor = 1, FijarOcultar, SugerirValor,
                      DejarVacio, DejarVacioOcultar, Buscar
                    };
    /*<<<<<DBFIELD_PEDIRCAMPO_CONSTRUCTOR*/
	FldPedirCampo(const Xtring &tablename, const Xtring &name,
		dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
		const Xtring &defaultvalue = Xtring::null)
/*>>>>>DBFIELD_PEDIRCAMPO_CONSTRUCTOR*/
        : dbFieldListOfValues<int>( false, &sCaptions, &sValues, tablename, name,
                                    SQLINTEGER, 4, 0, flags, defaultvalue )
    {}

    /*<<<<<DBFIELD_PEDIRCAMPO_VALUES*/
public:
	static List<int> sValues;
/*>>>>>DBFIELD_PEDIRCAMPO_VALUES*/
    static List<Xtring> sCaptions;
};
/*<<<<<PEDIRCAMPO_POSTAMBLE*/
} // namespace gastostipo
} // namespace gong
#endif // GASTOSTIPO_TESORERIAFLDPEDIRCAMPO_H
/*>>>>>PEDIRCAMPO_POSTAMBLE*/

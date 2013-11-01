/*<<<<<COPYLEFT*/
/** @file empresafldperiodicidad.h dbFieldDefinition que representa el estado del pago de un cobro o un pago
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
/*<<<<<PERIODICIDAD_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAFLDPERIODICIDAD_H
#define _EMPRESA_EMPRESAFLDPERIODICIDAD_H
/*>>>>>PERIODICIDAD_PREAMBLE*/

/*<<<<<DBFIELD_PERIODICIDAD_INCLUDES*/
#include <gongdbfieldlistofvalues.h>
/*>>>>>DBFIELD_PERIODICIDAD_INCLUDES*/

namespace gong {
namespace empresa {

class FldPeriodicidad: public dbFieldListOfValues<int>
{
public:
    /*<<<<<DBFIELD_PERIODICIDAD_CONSTRUCTOR*/
	FldPeriodicidad(const Xtring &tablename, const Xtring &name,
		dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
		const Xtring &defaultvalue = Xtring::null)
/*>>>>>DBFIELD_PERIODICIDAD_CONSTRUCTOR*/
        : dbFieldListOfValues<int>( false, mCaptions, mValues,
                                    tablename, name, SQLINTEGER, 2, 0, flags, defaultvalue )
    {}

    /*<<<<<DBFIELD_PERIODICIDAD_VALUES*/
public:
	static List<int> mValues;
/*>>>>>DBFIELD_PERIODICIDAD_VALUES*/
    static List<Xtring> mCaptions;
};

/*<<<<<PERIODICIDAD_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAFLDPERIODICIDAD_H
/*>>>>>PERIODICIDAD_POSTAMBLE*/



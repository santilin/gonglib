/*<<<<<NOMBREPROPIO_PREAMBLE*/
#ifndef _CONTACTOS_CONTACTOSFLDNOMBREPROPIO_H
#define _CONTACTOS_CONTACTOSFLDNOMBREPROPIO_H
/*>>>>>NOMBREPROPIO_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file contactosfldnombrepropio.h dbFieldDefinition que representa un nombre pripio, de persona o asociación
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

/*<<<<<DBFIELD_NOMBREPROPIO_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_NOMBREPROPIO_INCLUDES*/

namespace gong {
namespace contactos {

class FldNombrePropio: public dbFieldDefinition
{
public:
    /*<<<<<DBFIELD_NOMBREPROPIO_CONSTRUCTOR*/
	FldNombrePropio(const Xtring &tablename, const Xtring &name,
		dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
		const Xtring &defaultvalue = Xtring::null)
/*>>>>>DBFIELD_NOMBREPROPIO_CONSTRUCTOR*/
        : dbFieldDefinition(tablename, name, SQLSTRING, 100, 0, flags, defaultvalue)
    {}

    /*<<<<<DBFIELD_NOMBREPROPIO_MEMBERS*/
	virtual bool isValid( dbRecord *r, dbFieldValue *value,
		ValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition
/*>>>>>DBFIELD_NOMBREPROPIO_MEMBERS*/

}; // class

/*<<<<<NOMBREPROPIO_POSTAMBLE*/
} // namespace contactos
} // namespace gong
#endif // CONTACTOS_CONTACTOSFLDNOMBREPROPIO_H
/*>>>>>NOMBREPROPIO_POSTAMBLE*/


/*<<<<<COPYLEFT*/
/** @file contabfldcuenta.h dbFieldDefinition que representa una cuenta
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
/*<<<<<CUENTA_PREAMBLE*/
#ifndef _CONTAB_CONTABFLDCUENTA_H
#define _CONTAB_CONTABFLDCUENTA_H
/*>>>>>CUENTA_PREAMBLE*/

/*<<<<<DBFIELD_CUENTA_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_CUENTA_INCLUDES*/
#include "contabcuenta.h"

namespace gong {
namespace contab {

class FldCuenta: public dbFieldDefinition
{
public:
    /*<<<<<DBFIELD_CUENTA_CONSTRUCTOR*/
	FldCuenta(const Xtring &tablename, const Xtring &name,
		dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
		const Xtring &defaultvalue = Xtring::null)
/*>>>>>DBFIELD_CUENTA_CONSTRUCTOR*/
        : dbFieldDefinition(tablename, name, SQLSTRING, 14, 0, flags, defaultvalue)
    {
        setStyle("CUENTA");
    }

    /*<<<<<DBFIELD_CUENTA_MEMBERS*/
	virtual bool isValid( dbRecord *r, dbFieldValue *value,
		ValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition
/*>>>>>DBFIELD_CUENTA_MEMBERS*/

}; // class

/*<<<<<CUENTA_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABFLDCUENTA_H
/*>>>>>CUENTA_POSTAMBLE*/


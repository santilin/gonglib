/*<<<<<COPYLEFT*/
/** @file empresafldejercicio.h dbFieldDefinition que representa el ejercicio en curso
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<EJERCICIO_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAFLDEJERCICIO_H
#define _EMPRESA_EMPRESAFLDEJERCICIO_H
/*>>>>>EJERCICIO_PREAMBLE*/

/*<<<<<DBFIELD_EJERCICIO_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_EJERCICIO_INCLUDES*/

namespace gong
{
namespace empresa {

class FldEjercicio: public dbFieldDefinition
{
public:
    FldEjercicio( const Xtring &tablename, const Xtring &name,
                  dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                  const Xtring &defaultvalue = Xtring::null )
        : dbFieldDefinition( tablename, name, SQLINTEGER, 4, 0,
                             flags | dbFieldDefinition::Flags( dbFieldDefinition::INVISIBLE | dbFieldDefinition::NOTNULL),
                             defaultvalue ) {}

    /*<<<<<DBFIELD_EJERCICIO_MEMBERS*/
    virtual bool isValid( dbRecord *r, dbFieldValue *value,
                          ValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition
    virtual Variant customDefaultValue() const; // from dbFieldDefinition
    /*>>>>>DBFIELD_EJERCICIO_MEMBERS*/

}; // class

/*<<<<<EJERCICIO_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAFLDEJERCICIO_H
/*>>>>>EJERCICIO_POSTAMBLE*/

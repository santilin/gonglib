/*<<<<<COPYLEFT*/
/** @file empresafldempresaid.h dbFieldDefinition que representa el id de la empresa en curso
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
/*<<<<<EMPRESAID_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAFLDEMPRESAID_H
#define _EMPRESA_EMPRESAFLDEMPRESAID_H
/*>>>>>EMPRESAID_PREAMBLE*/

/*<<<<<DBFIELD_EMPRESAID_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_EMPRESAID_INCLUDES*/

namespace gong
{
namespace empresa {

class FldEmpresaID: public dbFieldDefinition
{
public:
    FldEmpresaID( const Xtring &tablename, const Xtring &name,
                  dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                  const Xtring &reference = "EMPRESA_ID" );
/*<<<<<DBFIELD_EMPRESAID_MEMBERS*/
	virtual bool isValid( dbRecord *r, dbFieldValue *value,
		ValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition
	virtual Variant customDefaultValue() const; // from dbFieldDefinition
/*>>>>>DBFIELD_EMPRESAID_MEMBERS*/
    virtual FldEmpresaID *clone() const { return new FldEmpresaID( *this ); }
}; // class

/*<<<<<EMPRESAID_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAFLDEMPRESAID_H
/*>>>>>EMPRESAID_POSTAMBLE*/



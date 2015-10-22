/*<<<<<COPYLEFT*/
/** @file empresafldcuentabanco.h dbFieldDefinition que representa una cuenta bancaria
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
/*<<<<<CUENTABANCO_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAFLDCUENTABANCO_H
#define _EMPRESA_EMPRESAFLDCUENTABANCO_H
/*>>>>>CUENTABANCO_PREAMBLE*/

/*<<<<<DBFIELD_CUENTABANCO_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_CUENTABANCO_INCLUDES*/

namespace gong
{
namespace empresa {

class FldCuentaBanco: public dbFieldDefinition
{
public:
    FldCuentaBanco( const Xtring &tablename, const Xtring &name,
                    dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                    const Xtring &defaultvalue = Xtring::null )
        : dbFieldDefinition( tablename, name, SQLSTRING, 24, 0, flags, defaultvalue ) {}
    /*<<<<<DBFIELD_CUENTABANCO_MEMBERS*/
	virtual bool isValid( dbRecord *r, dbFieldValue *value,
		ValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition
/*>>>>>DBFIELD_CUENTABANCO_MEMBERS*/
private:
    static int modulo11( const Xtring &digitos );
    static int calcDigitosControl( const Xtring &cuenta );
};

/*<<<<<CUENTABANCO_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAFLDCUENTABANCO_H
/*>>>>>CUENTABANCO_POSTAMBLE*/


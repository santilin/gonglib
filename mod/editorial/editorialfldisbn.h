/*<<<<<COPYLEFT*/
/** @file editorialfldisbn.h dbFieldDefinition que representa un ISBN
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
/*<<<<<ISBN_PREAMBLE*/
#ifndef _EDITORIAL_EDITORIALFLDISBN_H
#define _EDITORIAL_EDITORIALFLDISBN_H
/*>>>>>ISBN_PREAMBLE*/

/*<<<<<DBFIELD_ISBN_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_ISBN_INCLUDES*/

namespace gong {
namespace editorial {
	
class FldIsbn: public dbFieldDefinition
{
public:
	FldIsbn(const Xtring &tablename, const Xtring &name = "ISBN", 
		dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,			
		const Xtring &defaultvalue = Xtring::null)
	: dbFieldDefinition(tablename, name, SQLSTRING, 18, 0,flags, defaultvalue)
	{}
	
/*<<<<<DBFIELD_ISBN_MEMBERS*/
	virtual bool isValid( dbRecord *r, dbFieldValue *value,
		ValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition
/*>>>>>DBFIELD_ISBN_MEMBERS*/

}; // class

/*<<<<<ISBN_POSTAMBLE*/
} // namespace editorial
} // namespace gong
#endif // EDITORIAL_EDITORIALFLDISBN_H
/*>>>>>ISBN_POSTAMBLE*/



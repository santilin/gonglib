/*<<<<<IMAGE_PREAMBLE*/
#ifndef ___GONGDBFIELDIMAGE_H
#define ___GONGDBFIELDIMAGE_H
/*>>>>>IMAGE_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file gongdbfieldimage.h A dbFieldDefinition for Images
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

/*<<<<<DBFIELD_IMAGE_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_IMAGE_INCLUDES*/

namespace gong {

class dbFieldImage: public dbFieldDefinition
{
public:
    /*<<<<<DBFIELD_IMAGE_CONSTRUCTOR*/
	dbFieldImage(const Xtring &tablename, const Xtring &name,
		dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
		const Xtring &defaultvalue = Xtring::null)
/*>>>>>DBFIELD_IMAGE_CONSTRUCTOR*/
        : dbFieldDefinition(tablename, name, SQLBLOB, 0, 0, flags, defaultvalue)
    {}
}; // class

/*<<<<<IMAGE_POSTAMBLE*/
} // namespace gong
#endif // __GONGDBFIELDIMAGE_H
/*>>>>>IMAGE_POSTAMBLE*/




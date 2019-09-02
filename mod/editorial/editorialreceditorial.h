/*<<<<<COPYLEFT*/
/** @file editorialreceditorial.h Registro de editoriales
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
/*<<<<<EDITORIAL_PREAMBLE*/
#ifndef _EDITORIAL_EDITORIALRECEDITORIAL_H
#define _EDITORIAL_EDITORIALRECEDITORIAL_H
/*>>>>>EDITORIAL_PREAMBLE*/

/*<<<<<EDITORIAL_INCLUDES*/
#include <dbappdbapplication.h>

#include "contactosreccontacto.h"
/*>>>>>EDITORIAL_INCLUDES*/

namespace gong {
namespace editorial {
	
/*<<<<<EDITORIAL_CONSTRUCTOR*/
class RecEditorial: public dbRecord
{
public:
	RecEditorial(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("EDITORIAL"), recid, user)
/*>>>>>EDITORIAL_CONSTRUCTOR*/
	{};

/*<<<<<EDITORIAL_RELATIONS*/
	contactos::RecContacto *getRecContacto() const;
/*>>>>>EDITORIAL_RELATIONS*/

};

/*<<<<<EDITORIAL_POSTAMBLE*/
} // namespace editorial
} // namespace gong
#endif // EDITORIAL_EDITORIALRECEDITORIAL_H
/*>>>>>EDITORIAL_POSTAMBLE*/


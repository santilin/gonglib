/*<<<<<COPYLEFT*/
/** @file editorialreclibro.h Registro de libros
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
/*<<<<<LIBRO_PREAMBLE*/
#ifndef _EDITORIAL_EDITORIALRECLIBRO_H
#define _EDITORIAL_EDITORIALRECLIBRO_H
/*>>>>>LIBRO_PREAMBLE*/

/*<<<<<LIBRO_INCLUDES*/
#include <dbappdbapplication.h>

#include "editorialreceditorial.h"
/*>>>>>LIBRO_INCLUDES*/
#include <facturecarticulo.h>

namespace gong {
namespace editorial {

using namespace factu;

/*<<<<<LIBRO_CONSTRUCTOR*/
class RecLibro: public factu::RecArticulo
{
public:
	RecLibro(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: factu::RecArticulo(conn, recid, user)
/*>>>>>LIBRO_CONSTRUCTOR*/
	{};
/*<<<<<LIBRO_MEMBERS*/
	Xtring toString(int format, const Xtring &includedFields = Xtring::null) const;
/*>>>>>LIBRO_MEMBERS*/

/*<<<<<LIBRO_RELATIONS*/
	RecEditorial *getRecEditorial() const;
/*>>>>>LIBRO_RELATIONS*/

};

/*<<<<<LIBRO_POSTAMBLE*/
} // namespace editorial
} // namespace gong
#endif // EDITORIAL_EDITORIALRECLIBRO_H
/*>>>>>LIBRO_POSTAMBLE*/


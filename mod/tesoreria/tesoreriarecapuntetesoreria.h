/*<<<<<APUNTETESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIARECAPUNTETESORERIA_H
#define _TESORERIA_TESORERIARECAPUNTETESORERIA_H
/*>>>>>APUNTETESORERIA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file tesoreriarecapuntetesoreria.h Registro de apuntes de tesorería
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

/*<<<<<APUNTETESORERIA_INCLUDES*/
#include <dbappdbapplication.h>

#include "tesoreriareccuentatesoreria.h"
#include "empresarecproyecto.h"
/*>>>>>APUNTETESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

typedef dbRecord RecTercero;
typedef dbRecord RecConcepto;

/*<<<<<APUNTETESORERIA_CONSTRUCTOR*/
class RecApunteTesoreria: public dbRecord
{
public:
	RecApunteTesoreria(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("APUNTETESORERIA"), recid, user)
/*>>>>>APUNTETESORERIA_CONSTRUCTOR*/
		, pRecTercero(0), pRecConcepto(0)
	{};
    RecTercero *getRecTercero();	
    RecConcepto *getRecConcepto();	
	
/*<<<<<APUNTETESORERIA_RELATIONS*/
	RecCuentaTesoreria *getRecCuentaTesoreria() const;
	empresa::RecProyecto *getRecProyecto() const;
/*>>>>>APUNTETESORERIA_RELATIONS*/

/*<<<<<APUNTETESORERIA_MEMBERS*/
	void init();
/*>>>>>APUNTETESORERIA_MEMBERS*/
private:
	RecConcepto *pRecConcepto;
	RecTercero *pRecTercero;
}; // class

/*<<<<<APUNTETESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIARECAPUNTETESORERIA_H
/*>>>>>APUNTETESORERIA_POSTAMBLE*/

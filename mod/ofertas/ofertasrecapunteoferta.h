/*<<<<<APUNTEOFERTA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASRECAPUNTEOFERTA_H
#define _OFERTAS_OFERTASRECAPUNTEOFERTA_H
/*>>>>>APUNTEOFERTA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file ofertasrecapunteoferta.h Registro de apuntes de ofertas
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

/*<<<<<APUNTEOFERTA_INCLUDES*/
#include <dbappdbapplication.h>

#ifdef HAVE_TESORERIAMODULE
#include "tesoreriarectipoapuntetesoreria.h"
#endif
#include "ofertasreccuentaoferta.h"
#include "empresarecproyecto.h"
/*>>>>>APUNTEOFERTA_INCLUDES*/

namespace gong {
namespace ofertas {

typedef dbRecord RecTercero;
typedef dbRecord RecConcepto;

/*<<<<<APUNTEOFERTA_CONSTRUCTOR*/
class RecApunteOferta: public dbRecord
{
public:
	RecApunteOferta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("APUNTEOFERTA"), recid, user)
/*>>>>>APUNTEOFERTA_CONSTRUCTOR*/
        , pRecConcepto(0), pRecTercero(0)
	{};

/*<<<<<APUNTEOFERTA_RELATIONS*/
#ifdef HAVE_TESORERIAMODULE
	tesoreria::RecTipoApunteTesoreria *getRecTipoApunteTesoreria() const;
#endif
	RecCuentaOferta *getRecCuentaOferta() const;
	empresa::RecProyecto *getRecProyecto() const;
/*>>>>>APUNTEOFERTA_RELATIONS*/
    RecTercero *getRecTercero();
    RecConcepto *getRecConcepto();
/*<<<<<APUNTEOFERTA_MEMBERS*/
	void init();
	virtual bool save(bool validate, bool saverelated)/* throw( dbError ) */; // from dbRecord
	virtual bool remove()/* throw( dbError ) */; // from dbRecord
/*>>>>>APUNTEOFERTA_MEMBERS*/
    void actSaldoCuenta(dbRecordID cuentaoferta_id, const Money &importe, bool saving);
	bool generaContrapartida();
private:
    RecConcepto *pRecConcepto;
    RecTercero *pRecTercero;
}; // end class

/*<<<<<APUNTEOFERTA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASRECAPUNTEOFERTA_H
/*>>>>>APUNTEOFERTA_POSTAMBLE*/


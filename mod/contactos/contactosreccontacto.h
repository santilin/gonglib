/*<<<<<CONTACTO_PREAMBLE*/
#ifndef _CONTACTOS_CONTACTOSRECCONTACTO_H
#define _CONTACTOS_CONTACTOSRECCONTACTO_H
/*>>>>>CONTACTO_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file contactosreccontacto.h Registro de Contactos
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
/*<<<<<CONTACTO_INCLUDES*/
#include <dbappdbapplication.h>
#include <gongdbvalidresult.h>

#include "contactosreccontactoimagen.h"
/*>>>>>CONTACTO_INCLUDES*/

namespace gong {

class FrmBase;
class LineEdit;

namespace contactos {

/*<<<<<CONTACTO_CONSTRUCTOR*/
class RecContacto: public dbRecord
{
public:
	RecContacto(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("CONTACTO"), recid, user)
/*>>>>>CONTACTO_CONSTRUCTOR*/
    {}
    enum Tratamiento { TratamientoNo = 0, TratamientoDona = 1, TratamientoDon,
                       TratamientoEmpresa, TratamientoAsociacion, TratamientoCooperativa,
                       TratamientoColectivo, TratamientoAutonoma
                     };

    static Tratamiento tratamientoFromCIF( const Xtring &cif );
    static Tratamiento tratamientoFromNombre( const Xtring &nombre );
    static Xtring normalizaNombre(const Xtring &nombre, bool force = false);
    static Xtring normalizaEmpresa(const Xtring &nombre, bool force = false);
    static bool findCodigosPostales(const Xtring &cp, const Xtring &barrio,
                                    const Xtring &localidad, XtringList &localidades);
    static Xtring getProvinciaFromCp( const Xtring &cp );
    static bool slotSearchCPClicked( FrmBase *frm, LineEdit *editCP, LineEdit *editBarrio,
                                     LineEdit *editLocalidad, LineEdit *editProvincia );

    /*<<<<<CONTACTO_MEMBERS*/
	virtual bool validate(ValidResult::Context context); // from dbRecord
	Xtring toString(int format, const Xtring &includedFields = Xtring::null) const;
	virtual bool findMatchingRecord(dbRecordID *matchingid) const; // from dbRecord
	virtual bool save(bool validate, bool saverelated) throw( dbError ); // from dbRecord
/*>>>>>CONTACTO_MEMBERS*/
    /*<<<<<CONTACTO_RELATIONS*/
	RecContactoImagen *getRecContactoImagen() const;
/*>>>>>CONTACTO_RELATIONS*/
    static Xtring creaCIFProvisional( dbConnection *conn );
private:
    static Xtring normalizaNombreOEmpresa(int formato, const Xtring &nombre, bool force = false);
    static bool loadLocalidades();
    static Xtring sLocalidadesLineas, sCallesLineas;
    static Dictionary<Xtring> sProvincias;
}; // class

/*<<<<<CONTACTO_POSTAMBLE*/
} // namespace contactos
} // namespace gong
#endif // CONTACTOS_CONTACTOSRECCONTACTO_H
/*>>>>>CONTACTO_POSTAMBLE*/



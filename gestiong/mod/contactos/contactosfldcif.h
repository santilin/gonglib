/*<<<<<CIF_PREAMBLE*/
#ifndef _CONTACTOS_CONTACTOSFLDCIF_H
#define _CONTACTOS_CONTACTOSFLDCIF_H
/*>>>>>CIF_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file contactosfldcif.h dbFieldDefinition que representa un cif
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

/*<<<<<DBFIELD_CIF_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_CIF_INCLUDES*/
#include <dbappfldstringautoinc.h>
#include <dbappdbapplication.h>
#include "contactosreccontacto.h"

namespace gong {
namespace contactos {

class Cif : public Xtring
{
public:
    Cif();
    ~Cif();

    Cif(const Xtring &cif);
    Cif(const char *cif);

    char calcularCharControl() const;
    void eliminarCharControl();
    Xtring validate( RecContacto::Tratamiento tratamiento, bool fixme = false );
};

class FldCif: public FldStringAutoInc
{
public:
    /*<<<<<DBFIELD_CIF_CONSTRUCTOR*/
    FldCif(const Xtring &tablename, const Xtring &name,
           dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
           const Xtring &defaultvalue = Xtring::null)
    /*>>>>>DBFIELD_CIF_CONSTRUCTOR*/
        : FldStringAutoInc(DBAPP->getConnection(), tablename, name, 12, flags, defaultvalue)
    {}
    virtual Variant customDefaultValue() const; // from FldStringAutoInc

    /*<<<<<DBFIELD_CIF_MEMBERS*/
    virtual bool isValid( dbRecord *r, dbFieldValue *value,
                          ValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition
    /*>>>>>DBFIELD_CIF_MEMBERS*/
};

/*<<<<<CIF_POSTAMBLE*/
} // namespace contactos
} // namespace gong
#endif // CONTACTOS_CONTACTOSFLDCIF_H
/*>>>>>CIF_POSTAMBLE*/



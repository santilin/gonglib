/*<<<<<EMAIL_PREAMBLE*/
#ifndef ___GONGDBFIELDEMAIL_H
#define ___GONGDBFIELDEMAIL_H
/*>>>>>EMAIL_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file gongdbfieldemail.h dbFieldDefinition for email addresses
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

/*<<<<<DBFIELD_EMAIL_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_EMAIL_INCLUDES*/

namespace gong {


class EMail: public Xtring
{
public:
    EMail(): Xtring() {}
    EMail(const Xtring &email): Xtring(email) {}

    bool isValid() const;
};


class dbFieldEmail: public dbFieldDefinition
{
public:
    /*<<<<<DBFIELD_EMAIL_CONSTRUCTOR*/
    dbFieldEmail(const Xtring &tablename, const Xtring &name,
                 dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                 const Xtring &defaultvalue = Xtring::null)
    /*>>>>>DBFIELD_EMAIL_CONSTRUCTOR*/
        : dbFieldDefinition(tablename, name, SQLSTRING, 200, 0, flags, defaultvalue)
    {}

    /*<<<<<DBFIELD_EMAIL_MEMBERS*/
    virtual bool isValid( dbRecord *r, dbFieldValue *value,
                          ValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition
    /*>>>>>DBFIELD_EMAIL_MEMBERS*/

}; // class

/*<<<<<EMAIL_POSTAMBLE*/
} // namespace gong
#endif // __GONGDBFIELDEMAIL_H
/*>>>>>EMAIL_POSTAMBLE*/


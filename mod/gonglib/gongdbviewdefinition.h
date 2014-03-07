#ifndef _GONG_DBVIEWDEFINITION_H
#define _GONG_DBVIEWDEFINITION_H

/** @file gongdbviewdefinition.cpp View definition based on dbTableDefinition
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongdbtabledefinition.h"

namespace gong {

class dbViewDefinition: public dbTableDefinition
{
public:
    dbViewDefinition(dbDefinition &db,
                     const Xtring& name, const Xtring &origin,
                     const Xtring& caption,
                     const Xtring& from = Xtring::null,
                     const Xtring& where = Xtring::null,
                     const Xtring& orderby = Xtring::null,
                     const Xtring& groupby = Xtring::null)
        : dbTableDefinition(db, name, from,
                            where, orderby, groupby), mOrigin(origin), mCaption(caption), mModified(false)
    {}

    dbViewDefinition(const dbViewDefinition &other);
    dbViewDefinition(const dbTableDefinition &other);

    /** Creates a dbViewDefinition from a SELECT statement */
    dbViewDefinition(const Xtring &name, dbDefinition &dbdef,
                     const Xtring &select, const Xtring &origin, const Xtring &caption);
    /** Creates a dbViewDefinition from a resultset */
    dbViewDefinition(const Xtring &name, dbResultSet *rs, dbDefinition &dbdef,
                     const Xtring &origin, const Xtring &caption);
    const Xtring &getCaption() const {
        return mCaption;
    }
    void setCaption( const Xtring &caption ) {
        mCaption = caption;
    }
    const Xtring &getOrigin() const {
        return mOrigin;
    }
    void setOrigin( const Xtring &origin ) {
        mOrigin = origin;
    }
    Xtring getFirstFrom() const;
    bool createFromFromFields();
    bool isModified() const {
        return mModified;
    }
    void setModified( bool mod ) {
        mModified = mod;
    }
    Xtring toConfigString() const;

private:
    Xtring mOrigin, mCaption;
    bool mModified;
};


typedef Dictionary<dbViewDefinition *> dbViewDefinitionDict;


/*<<<<<DBTABLEDEFINITION_POSTAMBLE*/
} // namespace gong
#endif // __GONGDBTABLEDEFINITION_H
/*>>>>>DBTABLEDEFINITION_POSTAMBLE*/

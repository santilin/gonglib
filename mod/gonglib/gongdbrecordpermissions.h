#ifndef _GONG_DBRECPERMISSIONS_H
#define _GONG_DBRECPERMISSIONS_H

/** @file gongdbrecpermissions.h Users permissions on database objects
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

namespace gong {

class dbRecordPermissions
{
public:
    dbRecordPermissions(): canView(1), canAdd(1), canDelete(1), canEdit(1), canFilter(1),
        canReorder(1), canPrint(1), canFind(1),
        canMoveFirst(1), canMoveLast(1), canMoveNext(1), canMovePrevious(1) {} ;
    void setAll( bool perm) {
        canView=perm;
        canAdd=perm;
        canDelete=perm;
        canEdit=perm;
        canFilter=perm;
        canReorder=perm;
        canPrint=perm;
        canFind=perm;
        canMoveFirst=perm;
        canMoveLast=perm;
        canMoveNext=perm;
        canMovePrevious=perm;
        canUser1=perm;
        canUser2=perm;
        canUser3=perm;
    }
    int canView:1;
    int canAdd:1;
    int canDelete:1;
    int canEdit:1;
    int canFilter:1;
    int canReorder:1;
    int canPrint:1;
    int canFind:1;
    int canMoveFirst:1;
    int canMoveLast:1;
    int canMoveNext:1;
    int canMovePrevious:1;
    int canUser1:1;
    int canUser2:1;
    int canUser3:1;
};

}; // namespace gong

#endif

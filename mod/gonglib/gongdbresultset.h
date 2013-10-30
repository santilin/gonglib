#ifndef _GONG_DBRESULTSET_H
#define _GONG_DBRESULTSET_H

/** @file gongdbresultset.h A wrapper for sql resultsets
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongdbconnection.h"
#include <mysql/mysql.h>
#ifdef HAVE_SQLITE3
# include <sqlite3.h>
#endif

namespace gong {

class dbConnection;
class dbResultSet
{
public:
    typedef unsigned long long int size_type;

    dbResultSet(dbConnection *dbconn, MYSQL_RES *result);
#ifdef HAVE_SQLITE3
    dbResultSet(dbConnection *dbconn, struct sqlite3_stmt *result);
#endif
    ~dbResultSet();

    size_type getRowCount();
    size_type getRowNumber() const {
        return mRowNumber;
    }
    bool next();
    Variant getValue(unsigned int colnum);
    Variant getValue(size_type tup_num, unsigned int colnum);
    Xtring toString(unsigned colnum) const;
    int toInt(unsigned colnum) const;
    uint toUInt(unsigned colnum) const;
    double toDouble(unsigned colnum) const;
    long toLong(unsigned colnum) const;
    Money toMoney(unsigned colnum) const;
    Date toDate(unsigned colnum) const;
    DateTime toDateTime(unsigned colnum) const;
    Time toTime(unsigned colnum) const;
    bool toBool(unsigned colnum) const;
    bool isNull(unsigned colnum) const;
    Variant toBinary(unsigned colnum) const;

    dbConnection *getConnection() const {
        return pConnection;
    }
    unsigned int getColumnCount() const {
        return mColumnCount;
    }
    SqlColumnType getColumnType( unsigned int colnum ) const;
    const char *getColumnName(unsigned int colnum) const;
    unsigned int getColumnWidth( unsigned int colnum ) const;
    unsigned int getColumnDecimals( unsigned int colnum ) const;

private:
    dbConnection *pConnection;
    size_type mRowNumber;
    size_type mRowCount;
    uint mColumnCount;
    enum { atBeforeStart, atMiddle, atEnd } mPosition;
    union {
        struct {
            ::MYSQL_RES *pResult;
            ::MYSQL_ROW mRow;
            ::MYSQL_FIELD *pFieldDefs;
            unsigned long *pLengths;
        } mysql;
#ifdef HAVE_SQLITE3
        struct {
            sqlite3_stmt *pResult;
            std::vector<Variant>*pRows;
        } sqlite3;
#endif
    } _data;
};

}; // namespace gong

#endif

#ifndef TEST_TESTDBCOMMON_H
#define TEST_TESTDBCOMMON_H

#include <gongdbdefinition.h>
#include <gongdbtabledefinition.h>
#include <gongdbconnection.h>
#include <gongdbrecord.h>

using namespace gong;

dbTableDefinition *create_contactos(dbDefinition *pDatabase, dbConnection *conn);
dbConnection *create_connection();
	
class DerivedRecord: public dbRecord
{
public:
	DerivedRecord(dbConnection *conn, dbTableDefinition *tbldef, dbRecordID recid=0, class dbUser *user=0)
				: dbRecord(conn, tbldef, recid, user) {}
};


#endif
#ifndef TESTCOMMON_H
#define TESTCOMMON_H

#include <gongdbconnection.h>
#include <gongdbdefinition.h>
#include <gongdbrecord.h>

class TestCommon
{
public:
    TestCommon();
    ~TestCommon();
	gong::dbConnection mConnection;
	gong::dbTableDefinition *pTableContactos;
	gong::dbDefinition *pDatabase;
};

class DerivedRecord: public gong::dbRecord
{
	public:
		DerivedRecord(gong::dbConnection *conn, gong::dbTableDefinition *tbldef, gong::dbRecordID recid=0, gong::dbUser *user=0)
			:dbRecord(conn, tbldef, recid, user) {}
};


#endif

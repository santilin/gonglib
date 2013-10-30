#ifndef TESTDBRECORD_H
#define TESTDBRECORD_H

#include <gongdbrecord.h>

class TestdbRecord
{
public:
    TestdbRecord();
    ~TestdbRecord();

	void run();
	int test_normal();
	int test_imagefield();
	int test_create();
	int test_tostringfugit();
};

class DerivedRecord: public gong::dbRecord
{
	public:
		DerivedRecord(gong::dbConnection *conn, gong::dbTableDefinition *tbldef, gong::dbRecordID recid=0, gong::dbUser *user=0)
			:dbRecord(conn, tbldef, recid, user) {}
};


#endif // TESTDBRECORD_H

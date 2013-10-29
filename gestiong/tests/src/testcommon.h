#ifndef TESTCOMMON_H
#define TESTCOMMON_H

#include <gongdbconnection.h>
#include <gongdbdefinition.h>

class TestCommon
{
public:
    TestCommon();
    ~TestCommon();
	gong::dbConnection mConnection;
	gong::dbTableDefinition *pTableContactos;
	gong::dbDefinition *pDatabase;
};

#endif

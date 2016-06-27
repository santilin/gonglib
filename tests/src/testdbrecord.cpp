
#include <gongdbdefinition.h>
#include <gongdbrecord.h>
#include <iostream>
#include "testdbrecord.h"
#include <gongdbfieldimage.h>
#include "passwords.nosvn.h"

using namespace gong;


TestdbRecord::TestdbRecord()

{
}


TestdbRecord::~TestdbRecord()
{
}



void TestdbRecord::run()
{
	_GONG_DEBUG_TRACE(0);
	test_tostringfugit();
 	test_normal();
 	test_create();
 	test_imagefield();
	std::cout << "TestdbRecord finished" << std::endl;
}

int TestdbRecord::test_normal()
{
	_GONG_DEBUG_TRACE(1);
	int ret = 0;

	dbConnection conn;
	_GONG_DEBUG_ASSERT( conn.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, "")  );
	conn.exec("DROP DATABASE santilin_db", true);
	_GONG_DEBUG_ASSERT( conn.getLastError().getNumber() == 0  || conn.getLastError().getNumber() == 1008);
	conn.exec("CREATE DATABASE santilin_db");
	_GONG_DEBUG_ASSERT( conn.getLastError().getNumber() == 0  );
	_GONG_DEBUG_ASSERT( conn.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, DBTEST_DBNAME)  );
	conn.exec("CREATE TABLE test_table (ID INTEGER)");
	_GONG_DEBUG_ASSERT( conn.getLastError().getNumber() == 0  );
	dbDefinition db("TEST", "Test db");
	dbTableDefinition atable(db, "test_table", "test_table to test dbRecord", "test_table");
	atable.addFieldRecordID();
	DerivedRecord r0(&conn, &atable);
	_GONG_DEBUG_ASSERT( !r0.read(1)  );
	_GONG_DEBUG_ASSERT( r0.isNew()  );
	_GONG_DEBUG_ASSERT( !r0.isModified()  );
	_GONG_DEBUG_PRINT(0, r0.getRecordID() );
	_GONG_DEBUG_ASSERT( r0.getRecordID() == 0  );
	_GONG_DEBUG_ASSERT( r0.setValue("id", 1)  );
	_GONG_DEBUG_ASSERT( r0.isNew()  );
	_GONG_DEBUG_ASSERT( r0.isModified()  );
	_GONG_DEBUG_ASSERT( r0.save(false) );

	DerivedRecord r1(&conn, &atable);
	_GONG_DEBUG_ASSERT( r1.read(1)  );
	_GONG_DEBUG_ASSERT( !r1.isNew()  );
	_GONG_DEBUG_ASSERT( !r1.isModified()  );
	_GONG_DEBUG_ASSERT( r1.getRecordID() == 1  );
	_GONG_DEBUG_ASSERT( !r1.setValue("id", 2)  );
	_GONG_DEBUG_ASSERT( !r1.isNew()  );
	_GONG_DEBUG_ASSERT( !r1.isModified()  );
	r1.setNew(true);
	r1.setValue("id", 2);
	_GONG_DEBUG_ASSERT( r1.isModified()  );
	_GONG_DEBUG_ASSERT( r1.save(false)  );

	DerivedRecord r2(&conn, &atable);
	_GONG_DEBUG_ASSERT( r2.read(2)  );
	_GONG_DEBUG_ASSERT( !r2.isNew()  );
	_GONG_DEBUG_ASSERT( !r2.isModified()  );
	_GONG_DEBUG_ASSERT( r2.getRecordID() == 2  );
	_GONG_DEBUG_ASSERT( !r2.setValue("id", 3)  );
	r1.setNew(true);
	_GONG_DEBUG_ASSERT( r1.setValue("id", 3)  );
	_GONG_DEBUG_ASSERT( r2.save(false)  );

	return ret;
}


int TestdbRecord::test_create()
{
	_GONG_DEBUG_TRACE(1);
	int ret = 0;
	dbConnection conn;
	dbDefinition db("TEST", "Test db");
	dbTableDefinition *atable = new dbTableDefinition(db, "test_table", "test_table to test dbRecord", "test_table");
	atable->addFieldRecordID();
	DerivedRecord r(&conn, atable);
	_GONG_DEBUG_ASSERT( r.isNew()  );
	_GONG_DEBUG_ASSERT( !r.isModified()  );
	_GONG_DEBUG_ASSERT( r.getRecordID() == 0  );
	delete atable;
	return ret;

}

int TestdbRecord::test_imagefield()
{
	_GONG_DEBUG_TRACE(1);
	int ret = 0;

	dbConnection conn;
	_GONG_DEBUG_ASSERT( conn.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, "")  );
	conn.exec(Xtring("DROP DATABASE ") + DBTEST_DBNAME, true);
	_GONG_DEBUG_ASSERT( conn.getLastError().getNumber() == 0  || conn.getLastError().getNumber() == 1008);
	conn.exec(Xtring("CREATE DATABASE ") + DBTEST_DBNAME);
	_GONG_DEBUG_ASSERT( conn.getLastError().getNumber() == 0  );
	_GONG_DEBUG_ASSERT( conn.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, DBTEST_DBNAME)  );
	conn.exec("CREATE TABLE test_table (ID INTEGER, IMAGE MEDIUMBLOB)");
	_GONG_DEBUG_ASSERT( conn.getLastError().getNumber() == 0  );
	dbDefinition db("TEST", "Test db");
	dbTableDefinition atable(db, "test_table", "test_table to test dbRecord", "test_table");
	atable.addFieldRecordID();
	dbFieldImage *afieldimage = new dbFieldImage("test_table", "image");
	atable.addField(afieldimage);

	DerivedRecord r0(&conn, &atable);
	_GONG_DEBUG_ASSERT( r0.setValue("image", "value")  );
	_GONG_DEBUG_ASSERT_EQUALS( r0.getValue("image").toString(), "value"  );
	_GONG_DEBUG_ASSERT( r0.setValue("id", 1)  );
	_GONG_DEBUG_ASSERT( r0.save(false)  );

	DerivedRecord r1(&conn, &atable);
	_GONG_DEBUG_ASSERT( r1.read(1)  );
	_GONG_DEBUG_ASSERT_EQUALS( r1.getValue("image").toString(), "value"  );
	_GONG_DEBUG_ASSERT( !r1.isNew()  );

	return ret;
}


int TestdbRecord::test_tostringfugit()
{
	_GONG_DEBUG_TRACE(1);
	int ret = 0;

	dbConnection conn;
	_GONG_DEBUG_ASSERT( conn.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, "")  );
	conn.exec(Xtring("DROP DATABASE ") + DBTEST_DBNAME, true);
	_GONG_DEBUG_ASSERT( conn.getLastError().getNumber() == 0  || conn.getLastError().getNumber() == 1008);
	conn.exec(Xtring("CREATE DATABASE ") + DBTEST_DBNAME);
	_GONG_DEBUG_ASSERT( conn.getLastError().getNumber() == 0  );
	_GONG_DEBUG_ASSERT( conn.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, DBTEST_DBNAME)  );
	conn.exec("CREATE TABLE test_table (ID INTEGER, PRECIO DECIMAL(12,4))");
	dbDefinition db("TEST", "Test db");
	dbTableDefinition atable(db, "test_table", "test_table to test dbRecord", "test_table");
	atable.addFieldRecordID();
	atable.addFieldMoney("PRECIO", 12, 4);

	DerivedRecord r0(&conn, &atable);
	_GONG_DEBUG_ASSERT( r0.setValue("PRECIO", Money(2.3435, 4))  );
	_GONG_DEBUG_ASSERT_EQUALS( r0.getValue("PRECIO").toString(), "2.3435"  );
	_GONG_DEBUG_ASSERT( r0.setValue("id", 1)  );
	_GONG_DEBUG_ASSERT( r0.save(false)  );
	_GONG_DEBUG_PRINT(0, r0.toString( TOSTRING_FUGIT ) );
	return ret;
}

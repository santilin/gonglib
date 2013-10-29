#include <dbappdbapplication.h>
#include <gongdbdefinition.h>
#include <gongdbcalculator.h>
#include <gongdebug.h>
#include "testdbrecord.h"
#include "testdbcalculator.h"
#include "passwords.nosvn.h"

using namespace gong;

#include <iostream>
#include <string>


void calc(dbRecord *record, const Xtring &str )
{
    using boost::spirit::ascii::space;
    typedef std::string::const_iterator iterator_type;
    typedef gong::calculator<iterator_type> calculator;

    calculator calc(record); // Our grammar

    double result;
	std::string::const_iterator iter = str.begin();
	std::string::const_iterator end = str.end();
	bool r = phrase_parse(iter, end, calc, space, result);

	if (r && iter == end)
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing succeeded\n";
		std::cout << "result = " << result << std::endl;
		std::cout << "-------------------------\n";
	}
	else
	{
		std::cout << "-------------------------\n";
		std::cout << "Parsing failed\n";
		std::cout << "-------------------------\n";
	}
}

TestdbCalculator::TestdbCalculator()
{
}

void TestdbCalculator::run( char * expr )
{
	dbRecord *r = createRecord();
	testRecord(r, expr);
	delete r;
}


dbRecord *TestdbCalculator::createRecord()
{
	_GONG_DEBUG_TRACE(0);
	dbConnection conn;
	_GONG_DEBUG_ASSERT(  conn.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, "")  );
	conn.exec("DROP DATABASE test_db", true);
	_GONG_DEBUG_ASSERT(  conn.getLastError().getNumber() == 0  || conn.getLastError().getNumber() == 1008);
	conn.exec("CREATE DATABASE test_db");
	_GONG_DEBUG_ASSERT(  conn.getLastError().getNumber() == 0  );
	_GONG_DEBUG_ASSERT(  conn.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, "test_db")  );
	conn.exec("CREATE TABLE test_table (ID INTEGER, VALUE INTEGER)");
	_GONG_DEBUG_ASSERT(  conn.getLastError().getNumber() == 0  );
	dbDefinition db("TEST", "Test db");
	dbTableDefinition atable(db, "test_table", "test_table to test dbRecord", "test_table");
	atable.addFieldRecordID();
	atable.addFieldInt( "VALUE" );
	DerivedRecord *ret = new DerivedRecord( &conn, &atable );
	return ret;
}

void TestdbCalculator::testRecord(dbRecord* r, char *expr)
{
	_GONG_DEBUG_ASSERT( r );
	calc(r, expr );
}

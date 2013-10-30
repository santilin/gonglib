
#include <gongdbconnection.h>
#include <gongdbresultset.h>
#include <gongdebug.h>
#include "testdbconnection.h"
#include <iostream>
#include "passwords.nosvn.h"

using namespace std;
using namespace gong;

static dbConnection::SqlDriver testdriver = dbConnection::DRIVER_SQLITE3;

TestConnection::TestConnection()

{
}


TestConnection::~TestConnection()
{
}



void TestConnection::run()
{
	testConnection();
	testToSQL();
	testCreateDatabase();
	std::cout << "TestConnection finished" << std::endl;
}


int TestConnection::testConnection()
{
	dbConnection conn;
	unlink( "una base de datos que no existe.sql3" );
	_GONG_DEBUG_ASSERT ( !conn.connect(testdriver, DBTEST_USER, DBTEST_PASSWORD, "una base de datos que no existe" ) );
	_GONG_DEBUG_PRINT ( 0, conn.getLastError().getNumber() );
	_GONG_DEBUG_ASSERT ( conn.getLastError().getNumber() == 1049 );
	_GONG_DEBUG_ASSERT ( strlen(conn.getLastError().what()) );
	if( conn.isMySQL() ) {
		if( conn.getLastError().getNumber() == 2002 )
			std::cout << "please run /etc/init.d/mysql start" << std::endl;
		_GONG_DEBUG_ASSERT ( !conn.connect(testdriver, "un usuario erroneo", "una contraseña errónea", "" ) );
		_GONG_DEBUG_ASSERT ( strlen(conn.getLastError().what()));
		_GONG_DEBUG_ASSERT ( conn.getLastError().getNumber() == 1045 );
		_GONG_DEBUG_ASSERT ( !conn.connect(testdriver, DBTEST_USER, "una contraseña erronea", "db2" ) );
		_GONG_DEBUG_ASSERT ( strlen(conn.getLastError().what()));
		_GONG_DEBUG_ASSERT ( conn.getLastError().getNumber() == 1045 );
		_GONG_DEBUG_ASSERT ( conn.connect(testdriver, DBTEST_USER, DBTEST_PASSWORD, "mysql" ) );
		_GONG_DEBUG_ASSERT ( !strlen(conn.getLastError().what()) );
		_GONG_DEBUG_ASSERT ( conn.getLastError().getNumber() == 0 );
		dbResultSet *rs = conn.select("SHOW TABLES");
		for( unsigned row = 0; row < rs->getRowCount(); row ++)  {
			std::cout << rs->getValue(row,0).toString() << std::endl;
		}
	}
	return 1;
}


int TestConnection::testCreateDatabase()
{
	dbConnection conn;

	if( conn.connect(testdriver, DBTEST_USER, DBTEST_PASSWORD, "testdatabase" ) )
		_GONG_DEBUG_ASSERT( conn.exec( "DROP DATABASE testdbase", true) == 1 );
	else if( conn.isMySQL() &&  conn.getLastError().getNumber() == 2002 ) {
		std::cout << "please run /etc/init.d/mysql start" << std::endl;
		_GONG_DEBUG_ASSERT ( strlen(conn.getLastError().what()) == 0 );
		_GONG_DEBUG_ASSERT ( conn.getLastError().getNumber() == 0 );
		_GONG_DEBUG_ASSERT(  conn.getLastError().getNumber() == 0 || conn.getLastError().getNumber() == 1008 || conn.getLastError().getNumber() == 1146 ); //database doesn't exist
	}
	_GONG_DEBUG_ASSERT( conn.exec("CREATE DATABASE testdbase") );
	dbResultSet *rs = conn.select("SHOW DATABASES");
	_GONG_DEBUG_ASSERT(  rs  );
	bool found = false;
	while( rs->next() ) {
		_GONG_DEBUG_PRINT(0, rs->toString(0) );
		if( rs->toString(0) == "testdbase" )
			found = true;
	}
//	_GONG_DEBUG_ASSERT(  found  );


	_GONG_DEBUG_ASSERT(  conn.selectDatabase( "testdbase" )  );
	_GONG_DEBUG_ASSERT(  conn.select( "SHOW TABLES" )->getRowCount() == 0  );
	conn.exec( "CREATE TABLE contactos ( CONTACTO VARCHAR(50) )" );
	_GONG_DEBUG_ASSERT(  conn.select( "SHOW TABLES" )->getRowCount() == 1  );
	cout << conn.getLastError().what() << endl;
	_GONG_DEBUG_ASSERT(  !conn.selectDatabase( "testdbase2" )  );


	cout << conn.exec( "DROP DATABASE testdbase") << endl;
//	cout << conn.getLastError().what() << endl;
	_GONG_DEBUG_ASSERT(  conn.getLastError().getNumber() == 0  ); //database doesn't exist
	return 1;
}


int TestConnection::testToSQL()
{
	dbConnection conn;

	conn.connect(testdriver, "", "", "" );

	_GONG_DEBUG_ASSERT (conn.toSQL(1) == "1" );
	_GONG_DEBUG_ASSERT (conn.toSQL(-1) == "-1" );
	_GONG_DEBUG_ASSERT_EQUALS( conn.toSQL(1.2), "1.2" );
	_GONG_DEBUG_ASSERT (conn.toSQL(Money(100.34)) == "100.34" );
	_GONG_DEBUG_ASSERT (conn.toSQL(Xtring("Santiago")) == "'Santiago'" );
	_GONG_DEBUG_ASSERT (conn.toSQL("Santiago") == "'Santiago'" );
	_GONG_DEBUG_ASSERT (conn.toSQL(Date(2006,10,20)) == "'2006-10-20'" );
	_GONG_DEBUG_ASSERT (conn.toSQL(Time(22,30,20)) == "'22:30:20'" );
	_GONG_DEBUG_ASSERT (conn.toSQL(DateTime(2006,10,20,10,30,20)) == "'2006-10-20 10:30:20'" );

	if( conn.isMySQL() ) {
		_GONG_DEBUG_ASSERT_EQUALS( conn.toSQLLike( "CUENTA", "57" ), "`CUENTA` LIKE '%57%'" );
		_GONG_DEBUG_ASSERT_EQUALS( conn.toSQLStartLike( "CUENTA", "57" ), "`CUENTA` LIKE '57%'" );
		_GONG_DEBUG_ASSERT (conn.toSQL("O'Bryan") == "'O\\'Bryan'" );
	} else if( conn.isSQLite() ) {
		_GONG_DEBUG_ASSERT_EQUALS( conn.toSQLLike( "CUENTA", "57" ), "\"CUENTA\" LIKE '%57%'" );
		_GONG_DEBUG_ASSERT_EQUALS( conn.toSQLStartLike( "CUENTA", "57" ), "\"CUENTA\" LIKE '57%'" );
		_GONG_DEBUG_ASSERT_EQUALS(conn.toSQL("O'Bryan"), "'O''Bryan'" );
		_GONG_DEBUG_ASSERT_EQUALS(conn.toSQL("O\"Bryan"), "'O\"Bryan'" );
	}


	return 0;
}

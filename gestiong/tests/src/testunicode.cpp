#include "testcommon.h"
#include "passwords.nosvn.h"
#include "testunicode.h"


using namespace gong;


TestUnicode::TestUnicode()
{

}

void TestUnicode::run()
{
	testIConAcentos();
}


void TestUnicode::testIConAcentos()
{
	dbConnection conn;

	_GONG_DEBUG_ASSERT ( conn.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, "mysql" ) );
	_GONG_DEBUG_ASSERT ( !strlen(conn.getLastError().what()) );
	_GONG_DEBUG_ASSERT ( conn.getLastError().getNumber() == 0 );

	conn.exec( "DROP DATABASE `gong-tests`", true );
	conn.exec( "CREATE DATABASE `gong-tests` DEFAULT CHARSET 'latin1'" );
	conn.exec( "USE `gong-tests`" );
	conn.exec( "CREATE TABLE test ( texto CHAR(100) ) ENGINE=MyISAM" );
	conn.exec( "INSERT INTO test VALUES ('á')" );
	Xtring s = conn.selectString("SELECT texto FROM test");
	_GONG_DEBUG_PRINT( 0, s );

}

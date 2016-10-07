#include "testdbcommon.h"

using namespace gong;

dbConnection *create_connection()
{
	dbConnection *conn = new dbConnection();
	conn->connect(dbConnection::DRIVER_SQLITE3, Xtring::null, Xtring::null, "/tmp/test.sql3");
	return conn;
}

dbTableDefinition *create_contactos(dbDefinition *pDatabase, dbConnection *conn)
{
	conn->dropDatabase(pDatabase->getName(), true);
	pDatabase->create(conn);

	dbTableDefinition *pTableContactos = new dbTableDefinition( *pDatabase, "contactos" );
	pTableContactos->addFieldRecordID();
	pTableContactos->addFieldString("NOMBRE", 80);
	pTableContactos->addField("CP", SQLINTEGER, 5, 0);
	pTableContactos->addField("IMPORTE", SQLDECIMAL, 10, 2);
	pTableContactos->addField("NOTAS", SQLTEXT, 0, 0);
	pTableContactos->addField("FECHA", SQLDATE, 0, 0);
	pTableContactos->addField("HORA", SQLTIME, 0, 0);
	pTableContactos->addField("FECHAHORA", SQLDATETIME, 0, 0);
	pTableContactos->addField("BOOLEANO", SQLBOOL, 0, 0);
	pTableContactos->addField("ENTERO", SQLINTEGER, 10, 0);
	pTableContactos->addField("FLOTANTE",  SQLFLOAT, 10, 2 );

	pDatabase->addTable( pTableContactos );
	pDatabase->createTables(conn);
	return pTableContactos;
}

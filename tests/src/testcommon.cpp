#include "testcommon.h"
#include "passwords.nosvn.h"

using namespace gong;


TestCommon::TestCommon()
{
	mConnection.connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, "");

	pDatabase = new dbDefinition("santilin_gestiong", "Tests");
	mConnection.dropDatabase(pDatabase->getName(), true);
	pDatabase->create(&mConnection);

	pTableContactos = new dbTableDefinition( *pDatabase, "contactos" );
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
	pDatabase->createTables(&mConnection);
}


TestCommon::~ TestCommon()
{
	delete pDatabase;
}

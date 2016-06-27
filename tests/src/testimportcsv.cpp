#include <gongdebug.h>
#include <gongcsvutils.h>
#include <gongdbdefinition.h>
#include <gongdbmastertable.h>
#include <testdbrecord.h>
#include "dbappdbapplication.h"
#include "passwords.nosvn.h"
#include "testimportcsv.h"

using namespace gong;

TestImportCSV::TestImportCSV()

{
}


TestImportCSV::~TestImportCSV()
{
}

int TestImportCSV::run()
{
	int argc = 1;
	char *argv[] = { (char *)"gongcommontests", 0 };
	dbApplication app("", "", "", "", argc, argv);
	
	dbConnection *conn = app.getConnection();
	conn->connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, "");
	conn->exec(Xtring("DROP DATABASE ") + DBTEST_DBNAME , true);
	conn->exec(Xtring("CREATE DATABASE ") + DBTEST_DBNAME );
	conn->connect(dbConnection::DRIVER_MYSQL, DBTEST_USER, DBTEST_PASSWORD, DBTEST_DBNAME );
	app.initDatabases();
	dbDefinition *db = app.getDatabase();
	conn->dropDatabase(db->getName(), true);
	db->create(conn);
	dbMasterTable *contacto_t = new dbMasterTable(*db, "CONTACTO", "test_table to test csv import", "CONTACTO");
	contacto_t->addFieldRecordID();
	contacto_t->addFieldInt("CODIGO")->setIsCode(true);
	contacto_t->addFieldString("NOMBRE", 100)->setIsDescription(true);
    contacto_t->addFieldString("TELEFONO",100);
	db->addTable( contacto_t->getTableDefinition() );
	dbMasterTable *cliente_t = new dbMasterTable(*db, "CLIENTE", "test_table to test csv import", "CLIENTE");
	cliente_t->addFieldRecordID();
	cliente_t->addFieldInt("CODIGO")->setIsCode(true);
	cliente_t->addFieldString("RAZONSOCIAL",100)->setIsDescription(true);
    cliente_t->addFieldAggregateRelation( "CONTACTO_ID", "CONTACTO.ID", false );
	db->addTable( cliente_t->getTableDefinition() );
	db->createTables(conn);
	
	Xtring csvfields = 
"CLIENTE.CODIGO,CLIENTE.RAZONSOCIAL,CONTACTO.CODIGO,CONTACTO.NOMBRE,CONTACTO.TELEFONO\n";
	Xtring csvvalues =
"\"1\",\"Santilín\",\"1\",\"Santiago Capel Torres\",\"685334091\"\n";

	DerivedRecord *r = new DerivedRecord( conn, cliente_t->getTableDefinition() );
	r->fromString( csvvalues, TOSTRING_CSV, csvfields );
	r->save(true);
// 	_GONG_DEBUG_WARNING( r->toString( TOSTRING_DEBUG_COMPLETE_WITH_RELATIONS ) );
	return 1;
}



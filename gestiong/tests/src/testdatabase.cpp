
#include "testdatabase.h"
#include <gongdbtabledefinition.h>
#include <gongdbdefinition.h>



using namespace gong;


TestDatabase::TestDatabase()

{
}


TestDatabase::~TestDatabase()
{
}



void TestDatabase::run()
{
	testDatabase();
	std::cout << "TestDatabase finished" << std::endl;
}


int TestDatabase::testDatabase()
{

	dbDefinition GONGdb("gestiongdb", "Base de datos de GestiONG", "3.0.1");

	dbTableDefinition *t = new dbTableDefinition(GONGdb, "contactos", "Contactos", "Contacto");

	t->addFieldRecordID();
	return 1;
	
} 

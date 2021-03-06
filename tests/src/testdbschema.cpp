#include "testdbschema.h"
#include "gongdbconnection.h"
#include "gongdbdefinition.h"
#include "testcommon.h"

using namespace gong;

TestdbSchema::TestdbSchema()
{
}


TestdbSchema::~TestdbSchema()
{
}


void TestdbSchema::run()
{
	_GONG_DEBUG_TRACE(0);
	TestCommon t;
	
	dbDefinition *dbdef = dbDefinition::fromSQLSchema(&t.mConnection, "test_gestiong", XtringList());
	Xtring diff = t.pDatabase->sameSQLSchema( dbdef, &t.mConnection ); 
	std::cout << diff << std::endl;
	_GONG_DEBUG_ASSERT( diff.isEmpty() );
#if 0
	dbFieldDefinition *dbcanbenull = t.pDatabase->findFieldDefinition("contactos.NOMBRE");
	dbcanbenull->setCanBeNull(true);
	diff = t.pDatabase->sameSQLSchema( dbdef, &t.mConnection ); 
	std::cout << diff << std::endl;
	_GONG_DEBUG_ASSERT( !diff.isEmpty() );

	dbFieldDefinition *dbunique = t.pDatabase->findFieldDefinition("contactos.NOMBRE");
	_GONG_DEBUG_ASSERT( dbunique );
	dbunique->setUnique(true);
	diff = t.pDatabase->sameSQLSchema( dbdef, &t.mConnection ); 
	std::cout << diff << std::endl;
	_GONG_DEBUG_ASSERT( diff.isEmpty() );
#endif


}


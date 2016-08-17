
#include "testcommon.h"
#include "testtabledatamodel.h"
#include <gongdebug.h>
#include <gongdbrecorddatamodel.h>
#include "testdbrecord.h"



using namespace gong;


TestTableDataModel::TestTableDataModel()

{
}


TestTableDataModel::~TestTableDataModel()
{
}



void TestTableDataModel::run()
{
	_GONG_DEBUG_TRACE(0);
	testTableDataModel();
	std::cout << "TestTableDataModel finished" << std::endl;
}


int TestTableDataModel::testTableDataModel()
{
	TestCommon t;

	dbViewDefinitionDict views;
	dbViewDefinition *v = new dbViewDefinition(*t.pTableContactos);
	views.insert( "testview", v );

	_GONG_DEBUG_ASSERT(  views.size() == 1  );
	_GONG_DEBUG_ASSERT(  views.seq_at(0)->getName() == "contactos"  );

	DerivedRecord *arecord = new DerivedRecord(&t.mConnection, t.pTableContactos);
	_GONG_DEBUG_ASSERT(  arecord->setValue("NOMBRE", "Santiago")  );
	_GONG_DEBUG_ASSERT(  arecord->setValue("CP", 12345)  );
	_GONG_DEBUG_ASSERT(  arecord->setValue("IMPORTE", 1234.34 )  );
	_GONG_DEBUG_ASSERT(  arecord->setValue("NOTAS", "Observaciones")  );
	_GONG_DEBUG_ASSERT(  arecord->setValue("FECHA", Date::currentDate())  );
	_GONG_DEBUG_ASSERT(  arecord->setValue("HORA", Time(12,13,14))  );
	_GONG_DEBUG_ASSERT(  arecord->setValue("FECHAHORA", DateTime(2006,2,3,4,5,6))  );
	_GONG_DEBUG_ASSERT(  arecord->setValue("BOOLEANO", true ) );
	_GONG_DEBUG_ASSERT(  arecord->setValue("ENTERO", 10) );
	_GONG_DEBUG_ASSERT(  arecord->setValue("FLOTANTE", -343.34 ) );
	_GONG_DEBUG_ASSERT(  arecord->save(false, false)  );
	dbRecordID recid = arecord->getRecordID();

	dbRecordDataModel dm(arecord, views );
	_GONG_DEBUG_ASSERT(  dm.getViewCount() == 1  );
	_GONG_DEBUG_ASSERT(  dm.getCurrentViewIndex() == -1  );
	dm.setViewIndex(0);

	_GONG_DEBUG_ASSERT(  dm.getValue(0,0) == recid  );
	_GONG_DEBUG_ASSERT(  dm.getValue(0,1) == "Santiago"  );
	_GONG_DEBUG_ASSERT(  dm.getValue(0,2) == 12345  );
	_GONG_DEBUG_ASSERT(  dm.getValue(0,3) == 1234.34  );
	_GONG_DEBUG_ASSERT(  dm.getValue(0,4) == "Observaciones"  );

	_GONG_DEBUG_ASSERT(  dm.getValue(0,5) == Date::currentDate()  );
	_GONG_DEBUG_ASSERT(  dm.getValue(0,6) == Time(12,13,14)  );
 	_GONG_DEBUG_ASSERT(  dm.getValue(0,7) == DateTime(2006,2,3,4,5,6)  );
 	_GONG_DEBUG_ASSERT(  dm.getValue(0,8) == true  );
 	_GONG_DEBUG_ASSERT(  dm.getValue(0,9) == 10  );
 	_GONG_DEBUG_ASSERT(  dm.getValue(0,10) == -343.34  );


	delete arecord;
	return 1;

}

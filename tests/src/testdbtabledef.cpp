#include "testcommon.h"
#include "testdbtabledef.h"

using namespace gong;


TestTableDef::TestTableDef()
{
}


TestTableDef::~TestTableDef()
{
}


void TestTableDef::run()
{
	Xtring select, fields, from, where, orderby, groupby;
	XtringList fieldslist;
	bool ret;

	// Sin where ni order ni group
	select = "SELECT MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50 "
		"FROM MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID";
	ret = dbTableDefinition::parseSelectStatement( select, fields, from, where, orderby, groupby);
	fields.tokenize(fieldslist, ",");
	_GONG_DEBUG_ASSERT( ret == true );
	_GONG_DEBUG_ASSERT_EQUALS( fields,
		"MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50" );
	_GONG_DEBUG_ASSERT_EQUALS( from,
		"MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID" );
	_GONG_DEBUG_ASSERT_EQUALS( where, "" );
	_GONG_DEBUG_ASSERT_EQUALS( groupby, "" );
	_GONG_DEBUG_ASSERT_EQUALS( orderby, "" );
	_GONG_DEBUG_ASSERT( fieldslist.size() == 4 );
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[0], "MIEMBRO.ID");
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[1], " PROYECTO.DESCRIPCION|c:Proyecto");

	// Sin where y sin groupby
	select = "SELECT MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50 "
		"FROM MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID "
		"GROUP BY 3";
	ret = dbTableDefinition::parseSelectStatement( select, fields, from, where, orderby, groupby);
	fieldslist.clear();
	fields.tokenize(fieldslist, ",");
	_GONG_DEBUG_ASSERT( ret == true );
	_GONG_DEBUG_ASSERT_EQUALS( fields,
		"MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50" );
	_GONG_DEBUG_ASSERT_EQUALS( from,
		"MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID" );
	_GONG_DEBUG_ASSERT_EQUALS( where, "" );
	_GONG_DEBUG_ASSERT_EQUALS( groupby, "3" );
	_GONG_DEBUG_ASSERT_EQUALS( orderby, "" );
	_GONG_DEBUG_ASSERT( fieldslist.size() == 4 );
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[0], "MIEMBRO.ID");
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[1], " PROYECTO.DESCRIPCION|c:Proyecto");

	// Completa
	select = "SELECT MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50 "
		"FROM MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID "
		"WHERE MIEMBRO LIKE 'santiago' ORDER BY 2,3 GROUP BY 3";
	ret = dbTableDefinition::parseSelectStatement( select, fields, from, where, orderby, groupby);
	fieldslist.clear();
	fields.tokenize(fieldslist, ",");
	_GONG_DEBUG_ASSERT( ret == true );
	_GONG_DEBUG_ASSERT_EQUALS( fields,
		"MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50" );
	_GONG_DEBUG_ASSERT_EQUALS( from,
		"MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID" );
	_GONG_DEBUG_ASSERT_EQUALS( where,
		"MIEMBRO LIKE 'santiago'");
	_GONG_DEBUG_ASSERT_EQUALS( groupby, "3" );
	_GONG_DEBUG_ASSERT_EQUALS( orderby, "2,3" );
	_GONG_DEBUG_ASSERT( fieldslist.size() == 4 );
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[0], "MIEMBRO.ID");
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[1], " PROYECTO.DESCRIPCION|c:Proyecto");

	// Sin where y cambio orden de order y group
	select = "SELECT MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50 "
		"FROM MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID "
		"GROUP BY 3 ORDER BY 2,3";

	ret = dbTableDefinition::parseSelectStatement( select, fields, from, where, orderby, groupby);
	fieldslist.clear();
	fields.tokenize(fieldslist, ",");
	_GONG_DEBUG_ASSERT( ret == true );
	_GONG_DEBUG_ASSERT_EQUALS( fields,
		"MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50" );
	_GONG_DEBUG_ASSERT_EQUALS( from,
		"MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID" );
	_GONG_DEBUG_ASSERT_EQUALS( where, "" );
	_GONG_DEBUG_ASSERT_EQUALS( groupby, "3" );
	_GONG_DEBUG_ASSERT_EQUALS( orderby, "2,3" );
	_GONG_DEBUG_ASSERT( fieldslist.size() == 4 );
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[0], "MIEMBRO.ID");
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[1], " PROYECTO.DESCRIPCION|c:Proyecto");

	// con where y sin orderby
	select = "SELECT MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50 "
		"FROM MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID "
		"WHERE MIEMBRO LIKE 'santiago' ORDER BY 2,3";

	ret = dbTableDefinition::parseSelectStatement( select, fields, from, where, orderby, groupby);
	fieldslist.clear();
	fields.tokenize(fieldslist, ",");
	_GONG_DEBUG_ASSERT( ret == true );
	_GONG_DEBUG_ASSERT_EQUALS( fields,
		"MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50" );
	_GONG_DEBUG_ASSERT_EQUALS( from,
		"MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID" );
	_GONG_DEBUG_ASSERT_EQUALS( where,
		"MIEMBRO LIKE 'santiago'");
	_GONG_DEBUG_ASSERT_EQUALS( groupby, "" );
	_GONG_DEBUG_ASSERT_EQUALS( orderby, "2,3" );
	_GONG_DEBUG_ASSERT( fieldslist.size() == 4 );
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[0], "MIEMBRO.ID");
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[1], " PROYECTO.DESCRIPCION|c:Proyecto");

	// Con where ni order ni group
	select = "SELECT MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50 "
		"FROM MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID "
		"WHERE MIEMBRO LIKE 'santiago'";
	ret = dbTableDefinition::parseSelectStatement( select, fields, from, where, orderby, groupby);
	fieldslist.clear();
	fields.tokenize(fieldslist, ",");
	_GONG_DEBUG_ASSERT( ret == true );
	_GONG_DEBUG_ASSERT_EQUALS( fields,
		"MIEMBRO.ID, PROYECTO.DESCRIPCION|c:Proyecto, MIEMBRO.NUMEROSOCIO, "
		"CONTACTO.NOMBRE|w:50" );
	_GONG_DEBUG_ASSERT_EQUALS( from,
		"MIEMBRO INNER JOIN PROYECTO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID"
		" INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID" );
	_GONG_DEBUG_ASSERT_EQUALS( where, "MIEMBRO LIKE 'santiago'" );
	_GONG_DEBUG_ASSERT_EQUALS( groupby, "" );
	_GONG_DEBUG_ASSERT_EQUALS( orderby, "" );
	_GONG_DEBUG_ASSERT( fieldslist.size() == 4 );
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[0], "MIEMBRO.ID");
	_GONG_DEBUG_ASSERT_EQUALS( fieldslist[1], " PROYECTO.DESCRIPCION|c:Proyecto");


}

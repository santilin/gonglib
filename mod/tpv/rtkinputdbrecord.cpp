#include <empresamodule.h>
#include <gongdbrecordlist.h>
#include "rtkinputdbrecord.h"

namespace gong {


bool InputdbRecord::init()
{
	mRecordNumber = -1;
	return true;
}


bool InputdbRecord::next()
{
	mRecordNumber++;
	if( mRecordNumber == pRecordList->size() )
		return false;
	else
		pDetailRecord = pRecordList->getRecord( mRecordNumber );
	return true;
}

Variant InputdbRecord::getValue(uint sourcepos, Variant::Type type) const
{
	Xtring fullfldname = mInputFields[sourcepos]->source();
	fullfldname.replace("@input.", "").replace("_", ".");
	Xtring tablename, fieldname;
	fullfldname.splitIn2( tablename, fieldname, "." );
	if( tablename == "EMPRESA" ) {
		return empresa::ModuleInstance->getRecEmpresa()->getValue( fieldname );
	} else if( tablename == pDetailRecord->getTableName() || tablename == "ARTICULO" ) {
		return pDetailRecord->getValue( fullfldname );
	} else {
		Variant v(pRecord->getValue( fullfldname ).toString().c_str(), type);
		return v;
	}
}

} // namespace gong

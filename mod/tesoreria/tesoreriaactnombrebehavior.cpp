#include <gongdbrecord.h>
#include <dbappdbapplication.h>
#include "tesoreriaactnombrebehavior.h"

namespace gong {
namespace tesoreria {

	
bool ActNombreBehavior::save(dbRecord *therecord, bool is_pre, bool &result) 
{
    if( !is_pre ) {
		Xtring sql = "UPDATE " + therecord->getConnection()->nameToSQL(mSyncTable)
			+ " SET " + therecord->getConnection()->nameToSQL(mSyncField)
			+ " = " + therecord->getConnection()->toSQL(therecord->getValue(mMyField))
			+ " WHERE " + therecord->getConnection()->nameToSQL(mSyncIdField)
			+ " = " + therecord->getConnection()->toSQL(therecord->getRecordID());
		long affected = therecord->getConnection()->exec( sql, true ); // Ignore errors
		_GONG_DEBUG_WARNING("Se han actualizado " + Xtring::number(affected) + " registros en " + mSyncTable);
    }
    return true;
}


} // namespace tesoreria
} // namespace gong



#include "gongdbtabledefinition.h"
#include "gongdbrecord.h"
#include "gongdbrecordtimestampbehavior.h"

namespace gong {

bool dbRecordTimestampBehavior::addTo(dbTableDefinition* tbldef)
{
    tbldef->addFieldString("REC_ORIGEN", 40)->setCanBeNull( true )->setVisible( false );
    tbldef->addFieldTimestamp("REC_FECHA_MODIF")->setVisible( false );
    tbldef->addFieldReferenceID( "REC_USUAR_MODIF", "USUARIA.ID" )->setVisible( false );
    tbldef->addFieldTimestamp("REC_FECHA_CREA")->setVisible( false );
    tbldef->addFieldReferenceID( "REC_USUAR_CREA", "USUARIA.ID" )->setVisible( false );
    return true;
}

bool dbRecordTimestampBehavior::save(const dbRecord* old_record,
                                     dbRecord* therecord, bool isbefore, bool& result)
{
    if( isbefore ) return true;
    /*
    	In mysql, the first TIMESTAMP field, if null, gets updated with the current date on the server.
    	In postgres, every null TIMESTAMP field gets updated with the current date on the server.
    */
    Xtring now, sql = "UPDATE " + therecord->getTableName();
    if( therecord->getConnection()->isMySQL() )
        now = "NOW()";
    else
        now = "CURRENT_TIMESTAMP";
    sql += " SET REC_FECHA_MODIF=" + now;
    if( therecord->isNew() )
        sql+= ", REC_FECHA_CREA=" + now;
    sql += " WHERE ID=" + therecord->getConnection()->toSQL(therecord->getRecordID());
    result = therecord->getConnection()->exec( sql );
    return true;
}

} // namespace gong


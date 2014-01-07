/*<<<<<MODULE_INFO*/
// COPYLEFT Record of meta data for the database
// TYPE dbRecord MetaDBData
/*>>>>>MODULE_INFO*/

/*<<<<<METADBDATA_INCLUDES*/
#include "dbapprecmetadbdata.h"
/*>>>>>METADBDATA_INCLUDES*/

namespace gong {

bool RecMetaDBData::lock(const Xtring &user, const Xtring &password, const Xtring &desc, Date date)
{
    setValue( "LOCKED", true );
    setValue( "LOCKED_PASSWORD", password );
    setValue( "LOCKED_USER", user );
    setValue( "LOCKED_DESC", desc );
    setValue( "LOCKED_DATE", date );
    return save( false );
}

bool RecMetaDBData::unlock(const Xtring &password)
{
    if( password == getValue( "LOCKED_PASSWORD" ).toString() ) {
        setValue( "LOCKED", false);
        setValue( "LOCKED_PASSWORD", Xtring::null );
        setValue( "LOCKED_USER", Xtring::null  );
        setValue( "LOCKED_DESC", Xtring::null  );
        setNullValue( "LOCKED_DATE" );
        return save( false );
    }
    return false;
}

} // namespace gong

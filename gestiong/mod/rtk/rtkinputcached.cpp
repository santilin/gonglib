#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#include <gongdebug.h>
#include "rtkinputcached.h"
#include "rtkoutputcached.h"
#include "rtkreport.h"

namespace gong {}
namespace RTK = gong;

namespace gong {


InputCached::InputCached(Report &r, const char *basename, const char *name)
    : Input( r, name ), pBaseName ( const_cast<char *>(basename) ), pDB( 0 ), mDeleteDB( false )
{
}

InputCached::InputCached(Report &r, OutputCached *outputcached, const char *name)
    : Input( r, name ), pDB( outputcached->getDB() ), mDeleteDB( false )
{
}


InputCached::~InputCached()
{
    if( mDeleteDB )
        delete pDB;
}

bool InputCached::init()
{
    if( pDB == 0 ) {
        char filename[200];
        if( pBaseName == 0 )
            pBaseName = (char *)"rtkreport.cache"; /// \todo {add} tmp filename
        strcpy( filename, pBaseName );
        strcat( filename, ".key" );
        pDB = dbopen(filename, O_RDWR | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE, DB_BTREE, NULL );
        if( pDB == NULL ) {
            perror("Cache Index");
            return 0;
        }
        mDeleteDB = true;
    }
    return 1;
}

bool InputCached::next()
{
    _GONG_DEBUG_ASSERT( pDB );
    if( !pDB->seq(pDB, &mKey, &mData, R_NEXT ) )
        return true;
    else
        return false;
}

// This function is not called when printing the report.
// Instead, the data is deserialized from mKey and mData in Report::readNext()
Variant InputCached::getValue(uint, Variant::Type ) const
{
    return Variant();
}

} // namespace RTK

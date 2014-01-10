#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#include <gongdebug.h>
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkoutputcached.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

OutputCached::OutputCached(Report &r, const Input *in, CacheType cachetype, const char *basename)
    : Output(r, 0,0), pInput(in), mCacheType(cachetype), pBaseName(basename),
      pDB(0)
{
}

OutputCached::~OutputCached()
{
#ifdef HAVE_BDB
    if( pDB )
        pDB->close(pDB);
#else
	throw std::runtime_error("BDB is not compiled");
#endif
}

int OutputCached::startReport()
{
#ifdef HAVE_BDB
    _GONG_DEBUG_TRACE( 3 );
    char filename[200];
    if( pBaseName == 0 )
        pBaseName = "rtkreport.cache"; // Ojo tmp name
    if( mCacheType == CacheMemory ) {
        pDB = dbopen(0, O_RDWR | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE, DB_BTREE, NULL );
    } else {
        strcpy( filename, pBaseName );
        strcat( filename, ".key" );
        pDB = dbopen(filename, O_RDWR | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE, DB_BTREE, NULL );
    }
    if( pDB == NULL ) {
        perror("Cache Index");
        return 1;
    }
    return 0;
#else
	throw std::runtime_error("BDB is not compiled");
#endif
}


int OutputCached::endReport()
{
#ifdef HAVE_BDB
    _GONG_DEBUG_TRACE( 3 );
    if( pDB )
        if( pDB->sync(pDB, 0) ) // Don't close it as the InputCached might need it open
            perror("Cache Index sync");
    return 0;
#else
	throw std::runtime_error("BDB is not compiled");
#endif
}

Measure OutputCached::endSection(const Section &section)
{
    _GONG_DEBUG_TRACE( 3 );
#ifdef HAVE_BDB
    // if next section is not details, write fields
    if( section.type() == Section::Details ) {
        bool serialize = false;
        Report *r = section.report();
        for( uint ns=0; ns<r->sectionsCount(); ns++ ) {
            if( r->getSection(ns) == &section ) {
                if( ns < r->sectionsCount() - 1 ) {
                    Section *nextsection = r->getSection(ns+1);
                    if( nextsection->type() != Section::Details )
                        serialize = true;
                } else {
                    serialize = true;
                }
                break;
            }
        }
        if( serialize ) {
            _GONG_DEBUG_PRINT(3, Xtring::printf("Caching record %d", section.report()->recordCount() ) );
            int keylen;
            void *key = (void *)r->serializeGroupValues(&keylen);
            int datalen;
            void *data = (void *)r->serializeFields(pInput, &datalen);
            writeRecord( key, keylen, data, datalen );
        }
    }
    return 0.0;
#else
	throw std::runtime_error("BDB is not compiled");
#endif
}

int OutputCached::writeRecord(void *keyptr, int keylen, void *dataptr, int datalen)
{
#ifdef HAVE_BDB
    DBT key = { keyptr, keylen };
    DBT rec = { dataptr, datalen };
    if( pDB->put(pDB, &key, &rec, 0) )
        perror("put key");
    return 0;
#else
	throw std::runtime_error("BDB is not compiled");
#endif
}

} // Namespace RTK

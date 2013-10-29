#include "gongdbrecordlist.h"

namespace gong {

dbRecord *dbRecordList::getRecord(unsigned int i) const
{
    try {
        return at(i);
    } catch( ... ) {
        _GONG_DEBUG_WARNING( "Record not found in recordlist" );
        return 0;
    }
}

void dbRecordList::clear()
{
    for( unsigned int i=0; i<size(); i++ ) {
        delete at(i);
    }
    std::vector<dbRecord *>::clear();
}

dbRecord *dbRecordList::addRecord(const dbRecord *rec, int pos)
{
    if( pos < 0 || (unsigned int)pos >= size() )
        this->push_back(const_cast<dbRecord *>(rec));
    else
        insert( begin() + pos , const_cast<dbRecord *>(rec));
    return const_cast<dbRecord *>(rec);
}

#ifdef _GONG_DEBUG
Xtring dbRecordList::toString(int format) const
{
    Xtring ret = Xtring::printf("RecordList at: %p:, size=%d, ", this, size() );
    if( size() == 0 )
        ret += " Empty\n";
    else {
        ret += getRecord(0)->getTableName() + "\n";
        for( unsigned int i=0; i < size(); i++ ) {
            ret += getRecord(i)->toString( format );
            ret += "\n";
        }
    }
    return ret;
}
#endif

}


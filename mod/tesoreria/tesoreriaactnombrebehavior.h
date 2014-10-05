#ifndef _TESORERIA_ACTNOMBREBEHAVIOR_H
#define _TESORERIA_ACTNOMBREBEHAVIOR_H

#include <gongdbrecordbehavior.h>

namespace gong {
namespace tesoreria {

class ActNombreBehavior: public dbRecordBehavior
{
public:
    ActNombreBehavior(const Xtring &myfield, const Xtring &synctable, const Xtring &syncfield, const Xtring &syncidfield )
        : dbRecordBehavior(), mMyField(myfield), mSyncTable(synctable), mSyncField(syncfield),mSyncIdField(syncidfield) {}
    virtual bool save(dbRecord *therecord, bool is_pre, bool &result);
protected:
    Xtring mMyField, mSyncTable, mSyncField, mSyncIdField;
    Variant mOldValue;
};

} // namespace tesoreria
} // namespace gong

#endif // _TESORERIA_ACTNOMBREBEHAVIOR_H

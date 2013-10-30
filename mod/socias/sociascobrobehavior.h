#ifndef _SOCIAS_COBRO_BEHAVIOR_H
#define _SOCIAS_COBRO_BEHAVIOR_H

#include <gongdbrecordbehavior.h>

namespace gong {
namespace socias {

class CobroBehavior: public dbRecordBehavior
{
public:
    CobroBehavior();
    virtual bool save(dbRecord *therecord, bool is_pre, bool &result);
    virtual bool remove(dbRecord *therecord, bool is_pre);
};

} // namespace socias
} // namespace gong

#endif // _SOCIAS_COBRO_BEHAVIOR_H

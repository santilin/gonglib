#ifndef _SINPAPELES_RECORDBEHAVIOR_H
#define _SINPAPELES_RECORDBEHAVIOR_H

#include <gongdbrecordbehavior.h>

namespace gong {
namespace sinpapeles {

class SinPapelesRecordBehavior: public dbRecordBehavior
{
public:
    SinPapelesRecordBehavior(): dbRecordBehavior() {}
    virtual bool addTo( dbTableDefinition *tbldef );
    virtual bool save(dbRecord *therecord, bool is_pre, const dbRecord *old_record, bool &result);
protected:
    bool copyToAlmacen(const Xtring &source, const Xtring &dest);
};

} // namespace sinpapeles
} // namespace gong

#endif // SINPAPELESRECORDBEHAVIOR_H

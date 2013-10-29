#ifndef _RECCOLOR_RECORDBEHAVIOR_H
#define _RECCOLOR_RECORDBEHAVIOR_H

#include <gongdbrecordbehavior.h>

namespace gong {
namespace reccolor {

class ColorRecordBehavior: public dbRecordBehavior
{
public:
    ColorRecordBehavior(): dbRecordBehavior() {}
    virtual bool addTo( dbTableDefinition *tbldef );
};

} // namespace reccolor
} // namespace gong

#endif // RECCOLORRECORDBEHAVIOR_H

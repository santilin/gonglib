#include <gongdbtabledefinition.h>
#include "reccolorrecordbehavior.h"

namespace gong {
namespace reccolor {

bool ColorRecordBehavior::addTo(dbTableDefinition* tbldef)
{
    if( !tbldef->findFieldDefinition( "REC_COLOR" ) )
        tbldef->addFieldInt("REC_COLOR")->setCanBeNull( true )->setVisible( true );
    return true;
}


} // namespace reccolor
} // namespace gong



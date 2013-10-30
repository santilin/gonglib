/*<<<<<MODULE_INFO*/
// COPYLEFT Table relations definitions
// MEMBER Type Type rwc
// MEMBER LeftTable Xtring rwc
// MEMBER LeftField Xtring rwc
// MEMBER RightTable Xtring rwc
// MEMBER RightField Xtring rwc
// TYPE Class dbRelationDefinition fullconstr
/*>>>>>MODULE_INFO*/

#include "gongdbrelationdefinition.h"

/**
	@class gong::dbRelationDefinition
	@brief Table relations definitions

	Simply holds the definition for a relation between two tables.

	The \a dbRecordRelation class will hold the actual relation usage inside a \a dbRecord.
*/


namespace gong {

Xtring dbRelationDefinition::getName() const
{
    if( mType == many2many )
        return getRightTable() + "." + getRightField();
    else
        return getLeftTable() + "." + getLeftField();
}

Xtring dbRelationDefinition::getFullName() const
{
    return getLeftTable() + "." + getLeftField() + "=>" + getRightTable() + "." + getRightField();
}


#ifdef _GONG_DEBUG
std::ostream &operator<<(std::ostream &out, const dbRelationDefinition &reldef)
{
    out << "\t\t" << reldef.getFullName() << std::endl;
    return out;
}
#endif

}

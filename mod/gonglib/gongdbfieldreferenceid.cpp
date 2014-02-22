/*<<<<<MODULE_INFO*/
// COPYLEFT A dbFieldDefinition for RecordIDs
// MEMBER isValid
// TYPE dbFieldDefinition ReferenceID NoConstr
/*>>>>>MODULE_INFO*/

#include "gonggettext.h"
#include "gongdbdefinition.h"
#include "gongdbtabledefinition.h"
#include "gongdbrecord.h"

/*<<<<<DBFIELD_REFERENCEID_INCLUDES*/
#include "gongdbfieldreferenceid.h"
/*>>>>>DBFIELD_REFERENCEID_INCLUDES*/

namespace gong {

/*<<<<<DBFIELD_REFERENCEID_ISVALID*/
bool dbFieldReferenceID::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
/*>>>>>DBFIELD_REFERENCEID_ISVALID*/
    if (!canBeNull() && ( value->isNull() || value->toVariant().toInt() == 0 ) ) {
        if( integres ) {
			Xtring ref_table = getReference().mid(0, getReference().find("."));
            const dbTableDefinition *tbldef = r->getTableDefinition()->getdbDefinition().findTableDefinition( ref_table );
            if( tbldef ) {
                const char *demostrativo;
				const char *vacio;
                if( tbldef->isFemenina() ) {
                    demostrativo = _("La");
					vacio = "vacía";
				} else {
                    demostrativo = _("El");
					vacio = "vacío";
				}
                integres->addError( Xtring::printf( _("%1$s %2$s no puede estar %3$s."),
                                                    demostrativo, tbldef->getDescSingular().c_str(), vacio ),
                                    getName() );
            } else {
                integres->addError( Xtring::printf( _("%s no puede estar vacío."),
                                                    getCaption().c_str() ),
                                    getName() );
            }
        }
        return false;
    }
    return true;
}

}

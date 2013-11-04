/*<<<<<MODULE_INFO*/
// COPYLEFT A dbFieldDefinition for RecordIDs
// MEMBER isValid
// TYPE dbFieldDefinition ReferenceID NoConstr
/*>>>>>MODULE_INFO*/

#include "gonggettext.h"
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
/// \todo {check} Esto se hacía solo para que los apuntes pudieran tener algunos datos incompletos
        //&& sev != IntegrityResult::valid_for_edition ) {
        if( integres ) {
            const dbTableDefinition *tbldef = getTableDefinition();
            if( !tbldef )
                tbldef = r->getTableDefinition();
            if( tbldef ) {
                const char *demostrativo;
                if( tbldef->isFemenina() )
                    demostrativo = "La";
                else
                    demostrativo = "El";
                integres->addError( Xtring::printf( _("%s %s (%s) no puede estar en blanco."),
                                                    demostrativo, tbldef->getDescSingular().c_str(),
                                                    getCaption().c_str()
                                                  ),
                                    getName() );
            } else {
                integres->addError( Xtring::printf( _("%s no puede estar en blanco."),
                                                    getCaption().c_str() ),
                                    getName() );
            }
        }
        return false;
    }
    return true;
}

}

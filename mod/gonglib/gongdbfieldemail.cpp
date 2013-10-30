/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition for email addresses
// MEMBER isValid
// TYPE dbFieldDefinition Email
/*>>>>>MODULE_INFO*/

#include "gonggettext.h"
/*<<<<<DBFIELD_EMAIL_INCLUDES*/
#include "gongdbfieldemail.h"
/*>>>>>DBFIELD_EMAIL_INCLUDES*/

namespace gong {

bool EMail::isValid() const
{
    if( !isEmpty() )
        return find("@") != Xtring::npos && find(".") != Xtring::npos;
    return true;
}

/*<<<<<DBFIELD_EMAIL_ISVALID*/
bool dbFieldEmail::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
    /*>>>>>DBFIELD_EMAIL_ISVALID*/
    bool ret = dbFieldDefinition::isValid( r, value, context, integres);
    if( ret ) {
        if( !value->isEmpty() && !EMail(value->toString()).isValid() ) {
            if( integres )
                integres->addWarning(
                    _("La dirección de correo electrónico debería contener al menos una '@' y un '.'"),
                    this->getName(), false );
        }
    }
    return ret;
}

}; // Namespace

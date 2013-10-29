
#include <gongdbdefinition.h>
#include "dbappfrmintegrity.h"


namespace gong {

/*
 *  Constructs a FrmIntegrity as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FrmIntegrity::FrmIntegrity(dbDefinition *db,
                           QWidget* parent, const char* name, WidgetFlags fl )
    : FrmCustom( parent, name, fl ),
      pDbDefinition( db )
{
    if ( objectName().isEmpty() )
        setObjectName ( "FrmIngtegrity" );

}

int FrmIntegrity::exec()
{
    showModalFor( getRealParent(), false, true );
    if( !wasCancelled() ) {
    }
    return !wasCancelled();
}


} // namespace gong

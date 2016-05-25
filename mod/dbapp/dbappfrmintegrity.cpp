#include <QListView>
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
FrmIntegrity::FrmIntegrity(dbApplication *dbapp, QWidget* parent, const char* name, WidgetFlags fl )
    : FrmCustom( parent, name, fl ),
      pDbDefinition( dbapp->getDatabase() )
{
    if ( objectName().isEmpty() )
        setObjectName ( "FrmIngtegrity" );
    pListTables = new QListView( this );
    pModelTables = new DBTablesModel( dbapp->getDatabase(), dbapp->getConnection(), this );
    pListTables->setModel( pModelTables );
}



} // namespace gong

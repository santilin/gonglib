#ifndef _FACTU_FACTURECARTICULOIMAGEN_H
#define _FACTU_FACTURECARTICULOIMAGEN_H

#include <dbappdbapplication.h>

namespace gong {
namespace factu {

/*<<<<<ARTICULO_CONSTRUCTOR*/
class RecArticuloImagen: public dbRecord
{
public:
    RecArticuloImagen(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("ARTICULOIMAGEN"), recid, user)
/*>>>>>ARTICULO_CONSTRUCTOR*/
    {}
}; // end class

} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECARTICULOIMAGEN_H

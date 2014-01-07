/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition que representa el modo de detallar el iva en las facturas, etc.
// INHERIT dbFieldListOfValues<int>
// TYPE dbFieldDefinition factu::IVADetallado static
/*>>>>>MODULE_INFO*/

/*<<<<<DBFIELD_IVADETALLADO_INCLUDES*/
#include "factufldivadetallado.h"
/*>>>>>DBFIELD_IVADETALLADO_INCLUDES*/

namespace gong {
namespace factu {

IntList FldIVADetallado::sValues;
XtringList FldIVADetallado::sCaptions;

} // namespace factu
} // namespace gong

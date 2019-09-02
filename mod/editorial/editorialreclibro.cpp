/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de libros
// MEMBER toString
// RELATION Editorial
// INHERIT factu::Articulo
// TYPE dbRecord editorial::Libro
/*>>>>>MODULE_INFO*/

/*<<<<<LIBRO_INCLUDES*/
#include "editorialreclibro.h"
/*>>>>>LIBRO_INCLUDES*/

namespace gong {
namespace editorial {

/*<<<<<LIBRO_RELATIONS*/
RecEditorial *RecLibro::getRecEditorial() const
{
	return static_cast<RecEditorial*>(findRelatedRecord("ARTICULO.EDITORIAL_ID"));
}

/*>>>>>LIBRO_RELATIONS*/
/*<<<<<LIBRO_TOSTRING*/
Xtring RecLibro::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>LIBRO_TOSTRING*/
	if( format == TOSTRING_CODE_AND_DESC )
		result = getValue("CODIGO").toString() + ", " + getValue("NOMBRE").toString();
	else
		result = RecArticulo::toString(format, includedFields);
	return result;
}

} // namespace editorial
} // namespace gong

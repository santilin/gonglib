#ifndef _EDITORIAL_ISBNSEARCH_H
#define _EDITORIAL_ISBNSEARCH_H

#include <gongxtring.h>


namespace gong {
namespace editorial {

struct ISBNInfo {
	Xtring isbn13, isbn10, titulo, anyo, autoras, lengua, edicion, fechaedicion,
		fechaimpresion, editorial, descripcion, encuadernacion, coleccion,
		materias, precio, monografia, lenguastraduccion;
};


Xtring ISBNSearchMCU_ES(const Xtring &isbn, struct ISBNInfo &isbninfo);
Xtring ISBNSearch(const Xtring &engine, const Xtring &isbn, struct ISBNInfo &isbninfo);

} // namespace editorial
} // namespace gong

#endif // _EDITORIAL_ISBNSEARCH_H


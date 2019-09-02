#include <gongdebug.h>
#include <gonggettext.h>
#include <gongiconv.h>
#include "editorialisbnsearch.h"

namespace gong {
namespace editorial {

Xtring getHeaderValue(const XtringList &response, const Xtring &key)
{
	for( XtringList::const_iterator resp_it = response.begin();
	    resp_it != response.end();
	    ++ resp_it ) {
			if( resp_it->startsWith( key + ":" ) ) {
				return resp_it->mid( key.size() + 2 );
			}
		}
	return Xtring::null;
}

Xtring extract_item(const char *source, const char *from, const char *to)
{
	const char *begin = strstr(source, from);
	if( begin )
		begin += strlen(from);
	else
		return Xtring::null;
	const char *end = strstr(begin, to);
	if( end )
		return Xtring(begin, end-begin);
	return Xtring::null;
}


Xtring extract_multiple(const char *key, const char *_div,
                        const char *from, const char *to,
                        const char *globalfrom, const char *globalto,
                        const char *content)
{
	char div[ strlen(_div) + 3 ];
	strcpy( div, "\"" );
	strcat( div, _div );
	strcat( div, "\"");
	const char *divfound = strstr(content, div);
	if( !divfound ) return Xtring::null;
	const char *item = strstr( divfound, key );
	if( !item ) return Xtring::null;
	const char *iterator_start = strstr( item, globalfrom );
	const char *iterator_end = strstr( item, globalto );
	if( !iterator_start || !iterator_end ) return Xtring::null;
	Xtring result;
	while( iterator_start < iterator_end ) {
		Xtring subresult = extract_item(iterator_start, from, to).trim().simplify_white_space();
		if( subresult.size() ) {
			iterator_start = strstr( iterator_start + strlen(from), to ) + strlen(to);
			iterator_start = strstr( iterator_start, from );
			if( result.size() != 0 )
				result += "\n";
			result += subresult;
		}
	}
	return result;
}


Xtring extract_info(const char *key, const char *_div, const char *from, const char *to, const char *content)
{
	char div[ strlen(_div) + 3 ];
	strcpy( div, "\"" );
	strcat( div, _div );
	strcat( div, "\"");
	const char *divfound = strstr(content, div);
	if( !divfound ) return Xtring::null;
	const char *item = strstr( divfound, key );
	if( !item ) return Xtring::null;
	return extract_item(item, from, to);
}


Xtring extract_info(const char *key, const char *_div, const char *content)
{
	char div[ strlen(_div) + 3 ];
	strcpy( div, "\"" );
	strcat( div, _div );
	strcat( div, "\"");
	const char *divfound = strstr(content, div);
	if( !divfound ) return Xtring::null;
	if( !strcmp( key, "SECONDPAGEREF" ) ) {
		const char *secondpageref = strstr( divfound, "href=\"" );
		if( secondpageref )
			return extract_item(secondpageref, "href=\"", "\"");
	} else if( !strcmp(key, "Monografía" ) ) {
		return extract_item(divfound, "[", "]" );
	} else if( !strcmp(key, "Año" ) ) {
		return extract_item(divfound, "(", ")" );
	}
	return Xtring::null;
}



Xtring ISBNSearch(const Xtring &engine, const Xtring &isbn, struct ISBNInfo &isbninfo)
{
	if( engine == "mcu.es" )
		return ISBNSearchMCU_ES( isbn, isbninfo );
	else
		return Xtring::printf( _("El motor de búsqueda %s no está contemplado"), engine.c_str() );
}

int read_http_file(XtringList sl, char **content, const Xtring &host, const Xtring &url,
	const Xtring &method = Xtring::null, const Xtring &extra = Xtring::null, const Xtring &extra2 = Xtring::null)
{
	return 0;
}

Xtring ISBNSearchMCU_ES(const Xtring &isbn, struct ISBNInfo &isbninfo)
{
	XtringList response;
    char *content = 0;
	read_http_file( response, &content,
	               "www.mcu.es",
	               "/webISBN/tituloSimpleFilter.do?cache=init&prev_layout=busquedaisbn&layout=busquedaisbn&language=es" );
	// extraer la cookie de la respuesta
	Xtring cookie, cookie_name, cookie_value;
	cookie = getHeaderValue( response, "Set-Cookie" );
	if( cookie.size() ) {
		XtringList cookie_parts;
		cookie.tokenize(cookie_parts, "=;");
		if( cookie_parts.size() > 1 ) {
			cookie_name = cookie_parts[0];
			cookie_value = cookie_parts[1];
		}
	} else {
		return _("No se ha podido acceder a www.mcu.es. (No se ha obtenido la cabecera 'Set-Cookie')");
	}
	if( content ) {
		delete [] content;
		content = 0;
	}

	read_http_file( response, &content,
	               "www.mcu.es",
	               "/webISBN/tituloSimpleDispatch.do;" + cookie_name.lower() + "=" + cookie_value,
	               "Cookie: " + cookie_name + "=" + cookie_value + "\r\n",
	               "POST",
	               "params.forzaQuery=N&params.cdispo=A&params.cisbnExt="
	               + isbn + "&params.liConceptosExt%5B0%5D.texto=&params.orderByFormId=1&action=Buscar&language=es&prev_layout=busquedaisbn&layout=busquedaisbn" );
	if( content ) {
		Xtring secondpage = extract_info("SECONDPAGEREF", "isbnResultado", content);
		isbninfo.isbn13 = extract_info("ISBN 13", "isbnResultado", "\">", "</a>", content).trim();
		isbninfo.isbn10 = extract_info("ISBN 10", "isbnResultado", "<strong>", "</strong>", content).trim();
		isbninfo.titulo = extract_info("tulo:", "isbnResDescripcion", "\">", "</a>", content).trim();
		isbninfo.anyo = extract_info("Año", "isbnResDescripcion", content).trim();
		isbninfo.monografia = extract_info("Monografía", "isbnResDescripcion", content).trim();
		delete [] content;
		content = 0;
		if( !secondpage.isEmpty() ) {
			read_http_file( response, &content,
			           "www.mcu.es",
			           secondpage.replace(";" + cookie_name.lower() + "=" + cookie_value, "" )
				           .replace("&amp;","&").c_str(),
			           "Cookie: " + cookie_name + "=" + cookie_value + "\r\n"
				       "Referer: http://" + "www.mcu.es" + "/webISBN/tituloSimpleDispatch.do;" + cookie_name.lower() + "=" + cookie_value + "\r\n");
		}
		if( !content )
			return _("Imposible acceder a la segunda página de www.mcu.es");
		Xtring charset, content_type;
		content_type = getHeaderValue( response, "Content-Type" );
		if( content_type.size() ) {
			XtringList contenttypeparts;
			content_type.tokenize( contenttypeparts, ";" );
			for( XtringList::const_iterator charset_it = contenttypeparts.begin();
			    charset_it != contenttypeparts.end();
			    ++charset_it) {
					if( charset_it->startsWith( "charset=" ) ) {
						charset = charset_it->mid( 8 ).upper();
						break;
					}
				}
		}
		Xtring againtitulo = extract_info("tulo:", "fichaISBN", "<strong>", "</strong>", content).trim();
		if( againtitulo != isbninfo.titulo ) {
			isbninfo.titulo = againtitulo;
			_GONG_DEBUG_WARNING("El título ha cambiado" );
		}
		isbninfo.autoras = extract_multiple("Autor/es:", "fichaISBN",
		                                         "<span>", "<", "<td>", "</td>", content).trim();
		isbninfo.lengua = extract_info("Lengua de", "fichaISBN", "<span>", "</span>", content).trim();
		isbninfo.edicion = extract_info("Edici", "fichaISBN", "<td>", "</td>", content).trim();
		isbninfo.fechaedicion = extract_info("Fecha Edici", "fichaISBN", "<td>", "</td>", content).trim();
		isbninfo.fechaimpresion = extract_info("Fecha Impresi", "fichaISBN", "<td>", "</td>", content).trim();
		isbninfo.editorial = extract_info("Publicaci", "fichaISBN", "\">", "</a>", content).trim();
		isbninfo.descripcion = extract_info("Descripci", "fichaISBN", "<td>", "</td>", content).trim();
		isbninfo.encuadernacion = extract_info("Encuadernaci", "fichaISBN", "<td>", "</td>", content).trim();
		isbninfo.coleccion = extract_info("Colecci", "fichaISBN", "<span>", "<a", content).trim();
		isbninfo.lenguastraduccion = extract_info("Lengua/s de traduc", "fichaISBN", "<span>", "</span>", content).trim();
		isbninfo.materias = extract_info("Materia/s", "fichaISBN", "<span>", "</span>", content).trim();
		isbninfo.precio = extract_info("Precio", "fichaISBN", "<td>", "</td>", content).trim();
		delete [] content;
		charset = IConv::canonicalCodeName(charset.c_str());
		if( charset.size() && charset != "UTF-8" && charset != "UTF8" ) {
			IConv icv( "UTF-8", charset );
			isbninfo.titulo = icv.convert( isbninfo.titulo );
			isbninfo.autoras = icv.convert( isbninfo.autoras );
			isbninfo.lengua = icv.convert( isbninfo.lengua );
			isbninfo.editorial = icv.convert( isbninfo.editorial );
			isbninfo.edicion = icv.convert( isbninfo.edicion );
			isbninfo.descripcion = icv.convert( isbninfo.descripcion );
			isbninfo.encuadernacion = icv.convert( isbninfo.encuadernacion );
			isbninfo.coleccion = icv.convert( isbninfo.coleccion );
			isbninfo.lenguastraduccion = icv.convert( isbninfo.lenguastraduccion );
			isbninfo.materias = icv.convert( isbninfo.materias );
			isbninfo.precio = icv.convert( isbninfo.precio );
		}
		return Xtring();
	}
	return _("No se ha podido obtener la búsqueda");
}

} // namespace editorial
} // namespace gong

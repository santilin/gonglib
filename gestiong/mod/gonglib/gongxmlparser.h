#ifndef _GONG_XMLPARSER_H
#define _GONG_XMLPARSER_H

/** @file gongxmlparser.h A SAX2 parser for XML based on libxml2
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#ifdef HAVE_LIBXML2

#include <libxml/parser.h>
#include <libxml/xmlstring.h>


namespace gong {

class XmlParser
{
public:
    XmlParser();
    virtual ~XmlParser();
    bool parseFile(const char *filename);
    bool parseString(const char *string);

    virtual void characters(const xmlChar *text, unsigned int len);
    virtual void comment(const xmlChar *text);
    virtual void startDocument();
    virtual void endDocument();
    virtual void startElement(const xmlChar *name, const xmlChar **attributes);
    virtual void endElement(const xmlChar *name);
    virtual void error ( const char *msg, ... );
    virtual void warning ( const char *msg, ... );
    virtual void fatalError ( const char *msg, ... );
    const xmlChar *getAttrValue( const char *name, const xmlChar **attributes);

protected:
    void stopParser(bool status);
    const char *pFilename;
    xmlParserCtxtPtr pXmlCtxtPtr;
private:
    bool mParseStatus;
};

}; // namespace gong

#endif // ifdef HAVE_LIBXML2

#endif // _GONG_XMLPARSER_H

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


#ifdef HAVE_POCOLIB

#include <Poco/SAX/SAXParser.h>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/SAXException.h>
#include <Poco/SAX/DefaultHandler.h>
#include <gongfileutils.h>

using namespace Poco::XML;

namespace gong {

class XmlParser: public SAXParser, public DefaultHandler
{
public:
	XmlParser() {
		setContentHandler( this );
		setErrorHandler( this );
	}
    bool parseFile(const char *filename) {
		Xtring content = FileUtils::readFile( filename );
		return parseString( content.c_str() );
	};
    bool parseString(const char *string) {
		try {
			parseMemoryNP( string, strlen(string) );
			return true;
		} catch( SAXException &exc ) {
			return false;
		}
	}
    void stopParser(bool status)
	{
		throw new SAXException();
	}

	// virtual void startDocument
	// virtual void endDocument
	virtual void startElement( const Xtring &name, const Attributes &attrList ) {}
	virtual void endElement( const Xtring &name ) {}
	virtual void characters( const Xtring &data ) {}
	virtual void comment( const Xtring &data ) {}
	virtual void error( const Xtring &message ) {}
	virtual void warning( const Xtring &message ) {}
	virtual void fatalError( const Xtring &message ) {}
    const Xtring getAttrValue( const Xtring &name, const Attributes &attrList) {
		return attrList.getValue( Xtring::null, name );
	}

private:
	// DocumentHandler implementation
	virtual void startElement( const XMLString & uri, const XMLString & localName, const XMLString & qname, const Attributes & attrList )
	{
		startElement( localName, attrList );
	}
	virtual void endElement( const XMLString & uri, const XMLString & localName, const XMLString & qname )
	{
		endElement( localName );
	}
	virtual void characters( const XMLChar ch[], int start, int length )
	{
		characters( Xtring( ch, start, length ) );
	}
	virtual void ignorableWhitespace( const XMLChar ch[], int start, int length )
	{
		comment( Xtring( ch, start, length ) );
	}
	// ErrorHandler implementation
	virtual void error( const SAXException & exc )
	{
		_GONG_DEBUG_WARNING( exc.displayText() );
		error( exc.what() );
	}
	virtual void fatalError( const SAXException & exc )
	{
		_GONG_DEBUG_WARNING( exc.displayText() );
		error( exc.what() );
	}
	virtual void warning( const SAXException & exc )
	{
		_GONG_DEBUG_WARNING( exc.displayText() );
		error( exc.what() );
	}
};

}; // namespace gong

#elif defined (HAVE_LIBXML2)

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

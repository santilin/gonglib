#include <gongdebug.h>
#include <gongfileutils.h>
#include <gongvariant.h>
#include <gongxmlparser.h>
#include <Poco/SAX/SAXParser.h>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/SAXException.h>
#include <Poco/SAX/DefaultHandler.h>
#include "testxmlparser.h"

using namespace gong;
using namespace Poco::XML;

class PocoXmlParser: public SAXParser, public DefaultHandler
{
public:
	PocoXmlParser() {
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
	virtual void characters( const Xtring &characters ) {}
	virtual void comment( const Xtring &characters ) {}
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



TestXMLParser::TestXMLParser()
{
}

const char *xml1 = "<TAG1 attr1='value1' attr2='value2'>Texto</TAG1>";
const char *xml2 = "<TAG1 attr1='value1' attr2='value2'>Texto</TAG2>";

void TestXMLParser::run()
{
	class simpleXmlParser: public PocoXmlParser
	{
		public:
			simpleXmlParser() {}
			virtual void startDocument() { _GONG_DEBUG_TRACE(0); }
			virtual void endDocument() { _GONG_DEBUG_TRACE(0); }
			virtual void startElement( const Xtring &name, const Attributes &attributes )
			{
				_GONG_DEBUG_PRINT(0, name );
				_GONG_DEBUG_PRINT(0, getAttrValue( "attr1", attributes ) );
				_GONG_DEBUG_PRINT(0, getAttrValue( "attr2", attributes ) );
			}
			virtual void endElement( const Xtring &name )
			{
				_GONG_DEBUG_PRINT(0, name );
			}
			virtual void characters( const Xtring &characters )
			{
				_GONG_DEBUG_PRINT(0, characters );
			}

	};
	simpleXmlParser theparser;
	theparser.parseString( xml1 );
	theparser.parseString( xml2 );
}


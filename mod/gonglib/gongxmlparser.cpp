#include "config.h"

#ifdef HAVE_POCOLIB

#elif defined( HAVE_LIBXML2 )
#include "gonggettext.h"

#include <fstream>
#include <sys/errno.h>

#include <libxml/xmlversion.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlIO.h>

#include "gongdebug.h"
#include "gongfileutils.h"
#include "gongxtring.h"
#include "gongxmlparser.h"


/*
	The xml2 library converts all the strings to utf-8
*/

namespace gong
{

extern "C"
{

    static void saxCharacters(XmlParser *xml, const xmlChar *text, int unsigned len)
    {
        xml->characters(text, len);
    }

    static void saxComment(XmlParser *xml, const xmlChar *text)
    {
        xml->comment(text);
    }

    static void saxStartDocument(XmlParser *xml)
    {
        xml->startDocument();
    }

    static void saxEndDocument(XmlParser *xml)
    {
        xml->endDocument();
    }

    static void saxStartElement(XmlParser *xml, const xmlChar *name, const xmlChar **attributes)
    {
        xml->startElement(name, attributes);
    }

    static void saxEndElement(XmlParser *xml, xmlChar *name)
    {
        xml->endElement(name);
    }

    static void saxWarning(XmlParser *xml, const char *msg, ...)
    {
        va_list args;
        va_start(args, msg);
        _GONG_DEBUG_WARNING( Xtring::printf(msg, args) );
        xml->warning(msg, args);
        va_end(args);
    }

    static void saxError(XmlParser *xml, const char *msg, ...)
    {
        va_list args;
        va_start(args, msg);
        xml->error(msg, args);
        va_end(args);
    }

    static void saxFatalError(XmlParser *xml, const char *msg, ...)
    {
        va_list args;
        va_start(args, msg);
        _GONG_DEBUG_WARNING( Xtring::printf(msg, args) );
        xml->fatalError(msg, args);
        va_end(args);
    }



}; // extern "C"


XmlParser::XmlParser()
    : pXmlCtxtPtr(0)
{
}


XmlParser::~XmlParser()
{}



bool XmlParser::parseFile(const char *filename)
{
    bool ret = false;
    xmlSAXHandler sax;
    mParseStatus = true;
    char buffer[1024];
    int res;

    if(!filename)
        return false;
    if( FileUtils::isDir( filename ) ) {
        error(_("The chosen file '%s' is a directory"), filename );
        return false;
    }
    std::ifstream *pFile = new std::ifstream(filename);
    if( !pFile->is_open() ) {
        error(_("Error reading file %s: %s"), filename, strerror(errno));
        delete pFile;
        return false;
    }
    memset(&sax, 0, sizeof(sax));
    sax.startDocument = (startDocumentSAXFunc)&saxStartDocument;
    sax.endDocument = (endDocumentSAXFunc)&saxEndDocument;
    sax.startElement = (startElementSAXFunc)&saxStartElement;
    sax.endElement = (endElementSAXFunc)&saxEndElement;
    sax.characters = (charactersSAXFunc)&saxCharacters;
    sax.comment = (commentSAXFunc)&saxComment;
    sax.warning = (warningSAXFunc)&saxWarning;
    sax.error = (errorSAXFunc)&saxError;
    sax.fatalError = (errorSAXFunc)&saxFatalError;


    pXmlCtxtPtr = xmlCreatePushParserCtxt(&sax, this, NULL, 0, filename);

    if(!pXmlCtxtPtr)
        return false;

    while( (res = pFile->readsome(buffer, 1024)) )
        xmlParseChunk(pXmlCtxtPtr, buffer, res, 0);

    if(pXmlCtxtPtr->wellFormed && mParseStatus == true)
        ret = true;

    pXmlCtxtPtr->sax = NULL;
    xmlFreeParserCtxt(pXmlCtxtPtr);
    return ret;
}

bool XmlParser::parseString(const char *xmlstring)
{
    bool ret = false;
    xmlSAXHandler sax;
    mParseStatus = true;

    if( !xmlstring || !*xmlstring )
        return false;

    memset(&sax, 0, sizeof(sax));
    sax.startDocument = (startDocumentSAXFunc)&saxStartDocument;
    sax.endDocument = (endDocumentSAXFunc)&saxEndDocument;
    sax.startElement = (startElementSAXFunc)&saxStartElement;
    sax.endElement = (endElementSAXFunc)&saxEndElement;
    sax.characters = (charactersSAXFunc)&saxCharacters;
    sax.comment = (commentSAXFunc)&saxComment;

    pXmlCtxtPtr = xmlCreatePushParserCtxt(&sax, this, (const char *)xmlstring,
                                          strlen((const char *)xmlstring), 0);
    if(!pXmlCtxtPtr)
        return false;
    xmlParseChunk(pXmlCtxtPtr, (const char *)xmlstring, strlen((const char *)xmlstring), 1);
    if(pXmlCtxtPtr->wellFormed && mParseStatus == true)
        ret = true;
    pXmlCtxtPtr->sax = NULL;
    xmlFreeParserCtxt(pXmlCtxtPtr);
    return ret;
}


void XmlParser::characters(const xmlChar *text, unsigned int len)
{
}

void XmlParser::comment(const xmlChar *text)
{
}

void XmlParser::startDocument()
{
}

void XmlParser::endDocument()
{
}

void XmlParser::startElement(const xmlChar *name, const xmlChar **attributes)
{
}

void XmlParser::endElement(const xmlChar *name)
{
}

void XmlParser::error ( const char *msg, ... )
{
    va_list args;
    va_start(args, msg);
    _GONG_DEBUG_WARNING( Xtring::printf(msg, args) );
    va_end(args);
}

void XmlParser::warning ( const char *msg, ... )
{
    va_list args;
    va_start(args, msg);
    _GONG_DEBUG_WARNING( Xtring::printf(msg, args) );
    va_end(args);
}

void XmlParser::fatalError ( const char *msg, ... )
{
    va_list args;
    va_start(args, msg);
    _GONG_DEBUG_WARNING( Xtring::printf(msg, args) );
    va_end(args);
}


const xmlChar *XmlParser::getAttrValue( const char *name, const xmlChar **attributes)
{
    _GONG_DEBUG_ASSERT(name);
    if( attributes ) {
        const xmlChar *attrname;
        const xmlChar **attrptr = attributes;
        while( *attrptr ) {
            attrname = *attrptr++;
            if( strcasecmp((const char *)attrname, name) == 0 )
                return *attrptr;
            attrptr++;
        }
    }
    return (const xmlChar *)"";
}

void XmlParser::stopParser(bool status)
{
    xmlStopParser( pXmlCtxtPtr );
    mParseStatus = status;
}

}; // namespace


#endif

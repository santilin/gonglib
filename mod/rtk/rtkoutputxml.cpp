#include "rtkobject.h"
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkoutputxml.h"


#define XML_HEADER "<?xml version=\"1.0\""
#define XML_HEADER_ENCODING " encoding=\""

namespace gong {}
namespace RTK = gong;

namespace gong {

int OutputXml::startReport()
{
    Output::startReport();
    pFile = new std::ofstream(fileName());
    if( !pFile->is_open() ) {
        perror(fileName());
        delete pFile;
        pFile = 0;
        return 1;
    }
    (*pFile) << XML_HEADER << std::endl;
    if( !strempty(encoding()) ) {
        (*pFile) << XML_HEADER_ENCODING << std::endl;
        (*pFile) << encoding() << std::endl;
        (*pFile) << '"' << std::endl;
    }
    (*pFile) << "?>" << std::endl << '<' << mReport.title() << '>' << std::endl;
    return 0;
}

int OutputXml::endReport()
{
    if ( pFile ) {
        (*pFile) << "</" << mReport.title() << ">" << std::endl;
        pFile->close();
        delete pFile;
    }
    pFile = 0;
    return 0;
}

Measure OutputXml::startSection( const Section &section )
{
    Xtring buffer;
    for( uint i=0; i<=section.groupLevel(); i++ )
        buffer += '\t';
    buffer += "<";
    buffer += section.name();
    buffer += ">\n";
    (*pFile) << buffer << std::endl;
    return 0.0;
}


Measure OutputXml::endSection( const Section &section )
{
    Xtring buffer;
    for( uint i=0; i<=section.groupLevel(); i++ )
        buffer += '\t';
    buffer += "</";
    buffer += section.name();
    buffer += ">\n";
    (*pFile) << buffer << std::endl;
    return 0.0;
}


Measure OutputXml::printObject( const Object &object )
{
    if( !object.isObject() )
        return 0.0;
    Xtring buffer;
    for( uint i=0; i<=(static_cast<Section *>(object.parent()))->groupLevel()+1; i++ )
        buffer += '\t';
    buffer += "<";
    buffer += object.name();
    buffer += ">";
    (*pFile) << buffer << std::endl;

    Xtring text( object.formattedText().XMLProtect() );
    (*pFile) << text << std::endl;
    buffer = "</";
    buffer += object.name();
    buffer += ">\n";
    (*pFile) << buffer << std::endl;
    return 0.0;
}


} // namespace

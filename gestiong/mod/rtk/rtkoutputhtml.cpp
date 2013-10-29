#include "rtkobject.h"
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkoutputhtml.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

#define HTML_HEADER "<HTML><BODY><TABLE>"
#define HTML_FOOTER "</BODY></HTML></TABLE>"
#define HTML_SECTION_START "<TR>"
#define HTML_SECTION_END "</TR>"
#define HTML_OBJECT_START "<TD>"
#define HTML_OBJECT_END "</TD>"

int OutputHtml::startReport()
{
    pFile = new std::ofstream(fileName());
    if( !pFile->is_open() ) {
        perror(fileName());
        delete pFile;
        pFile = 0;
        return 1;
    }
    return 0;
}

int OutputHtml::endReport()
{
    if ( pFile ) {
        pFile->close();
        delete pFile;
    }
    pFile = 0;
    return 0;
}

Measure OutputHtml::startPage()
{
    mCurrX = marginLeft();
    mCurrY = marginTop();
    (*pFile) << HTML_HEADER << std::endl;
    return sizeY();
}


Measure OutputHtml::endPage()
{
    (*pFile) << HTML_FOOTER << std::endl;
    return 0;
}

Measure OutputHtml::startSection( const Section & /*section*/ )
{
    (*pFile) << HTML_SECTION_START << std::endl;
    return sizeY() - mCurrY;
}


Measure OutputHtml::endSection( const Section &section )
{
    (*pFile) << HTML_SECTION_END << std::endl;
    mCurrY += section.sizeY();
    mCurrX = marginLeft();
    return sizeY() - mCurrY;
}


Measure OutputHtml::printObject( const Object &object )
{
    (*pFile) << HTML_OBJECT_START << std::endl;
    Xtring text( object.formattedText() );
    const char *c_text = text.c_str();
    if( c_text && strlen(c_text) )
        (*pFile) << c_text << std::endl;
    (*pFile) << HTML_OBJECT_END << std::endl;
    return sizeY() - mCurrY;
}

} // Namespace RTK

#include "rtkobject.h"
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkoutputtext.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

/*
  This format is a plain text file
  The writing is done on a page buffer and each page is writed to the file
  The coordinates are in columns and lines, so a cast must be done from
    Measure to int in every Measure of Output
*/

int OutputText::startReport()
{
    Output::startReport();
    mIntSizeX = static_cast<int>(sizeX());
    mIntSizeY = static_cast<int>(sizeY());
    mPageBuf = new char[ ( ( mIntSizeX + 1 ) * mIntSizeY ) + 1 ];
    pFile = new std::ofstream(fileName());
    if( !pFile->is_open() ) {
        perror(fileName());
        delete pFile;
        pFile = 0;
        return 1;
    }
    return 0;
}

int OutputText::endReport()
{
    if ( mPageBuf ) {
        delete [] mPageBuf;
    }
    if ( pFile ) {
        pFile->close();
        delete pFile;
    }
    pFile = 0;
    mPageBuf = 0;
    return 0;
}

Measure OutputText::startPage()
{
    mCurrX = marginLeft();
    mCurrY = marginTop();
    memset( mPageBuf, ' ', ( ( mIntSizeX + 1 ) * mIntSizeY ) );
    for ( int i = 1; i < mIntSizeY; i++ )
        mPageBuf[ i * ( mIntSizeX + 1 ) - 1 ] = '\n';
    mPageBuf[ ( mIntSizeX + 1 ) * mIntSizeY ] = '\0';
    return sizeY();
}


Measure OutputText::endPage()
{
    if ( pFile )
        pFile->write( mPageBuf, ( ( mIntSizeX + 1 ) * mIntSizeY ) );
    return 0;
}

Measure OutputText::startSection( const Section & /*section*/ )
{
    return sizeY() - mCurrY;
}


Measure OutputText::endSection( const Section &section )
{
    mCurrY += section.sizeY();
    mCurrX = marginLeft();
    return sizeY() - mCurrY;
}



Measure OutputText::printObject( const Object &object )
{
    internal_print( object.text().c_str(),
                    static_cast<int>( object.posX() + mCurrX ),
                    static_cast<int>( object.posY() + mCurrY ),
                    static_cast<int>( object.sizeX() ) );
    return sizeY() - mCurrY;
}

void OutputText::internal_print( const char *text, int x, int y, int len )
{
    if ( !text )
        return ;
    char *pbuff = mPageBuf + ( ( ( static_cast<int>(mCurrY) + y )
                                 * ( mIntSizeX + 1 ) ) + ( static_cast<int>(mCurrX) + x ) );
    const char *ptext;
    for ( ptext = text;
            ( *ptext != 0 ) && ( mCurrX + x + marginRight() < sizeX() ) && ( len > 0 );
            ptext++, pbuff++, x++, len-- )
        *pbuff = *ptext;
}

} // namespace

#include "rtkobject.h"
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkoutputcsv.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

int OutputCsv::startReport()
{
    Output::startReport();
    if( !strempty(fileName()) ) {
        pFile = new std::ofstream(fileName());
        if( !pFile->is_open() ) {
            perror(fileName());
            delete pFile;
            pFile = 0;
            return 1;
        }
        mStdCout = false;
    } else {
        mStdCout = true;
    }
    return 0;
}

int OutputCsv::endReport()
{

    if ( pFile && !mStdCout ) {
        pFile->close();
        delete pFile;
    }
    pFile = 0;
    return 0;
}

Measure OutputCsv::startSection( const Section &/*section*/ )
{
    mLine.clear();
    return 0;
}

Measure OutputCsv::endSection( const Section &/*section*/ )
{
    if( !mStdCout )
        (*pFile) << mLine << std::endl;
    else
        std::cout << mLine << std::endl;
    return 0;
}

Measure OutputCsv::printObject( const Object &object )
{
    if( !object.isSection() ) {
        if( mLine.size() )
            mLine += delimiters();
        mLine += quote();
        mLine += object.text();
        mLine += quote();
    }
    return 1;
}


} // namespace

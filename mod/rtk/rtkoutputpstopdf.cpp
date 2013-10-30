#include <cerrno>
#include <gongfileutils.h>
#include "rtkreport.h"
#include "rtkoutputpstopdf.h"

using namespace std;

namespace gong {}
namespace RTK = gong;

namespace gong {

OutputPStoPDF::OutputPStoPDF(const char *pathps2pdf, class Report &r, const char *filename,
                             Measure unitsperinchx, Measure unitsperinchy, PaperSize papersize,
                             PageOrientation pageorientation, Measure sizex, Measure sizey,
                             Measure marginleft, Measure marginright, Measure margintop, Measure marginbottom)
    : OutputPS(r, filename, unitsperinchx, unitsperinchy,
               papersize, pageorientation, sizex,
               sizey, marginleft, marginright,
               margintop, marginbottom), pPathPSToPDF( pathps2pdf)
{
    // Creates a temp .ps file
    mFileNamePDF = fileName();
    propFileName.set( (mFileNamePDF + ".~~~").c_str() );
}



int OutputPStoPDF::endReport()
{
    OutputPS::endReport();
    Xtring cmd2pdf = Xtring("ps2pdf \"") + fileName() + "\" \"" + mFileNamePDF + "\"";
    Xtring output;
    if( FileUtils::execProcess( cmd2pdf.c_str(), output) != 0 )  {
        mReport.addError( Error::OpenCommand, __FUNCTION__, 0, cmd2pdf.c_str(), strerror(errno) );
        if( !output.isEmpty() )
            mReport.addError( Error::OpenCommand, __FUNCTION__, 0, cmd2pdf.c_str(), output.c_str() );
    }
    unlink( fileName() );
    return 0;
}

} // namespace RTK

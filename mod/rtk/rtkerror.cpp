/*
	I have deleted all the declarations of throw in functions. See:
	http://www.icce.rug.nl/documents/cplusplus/cplusplus08.html#l140
*/

#include "rtkerror.h"

namespace gong {}
namespace RTK = gong;

namespace gong {


Error::AbortType Error::sErrorAbort = AbortNever;

Error::Error(ErrorType type, ErrorCode code, const char *loc, const char *message)
    : std::runtime_error(message), mCode(code), mType(type)
{
    if( !strempty(loc) )
        pLocation = strdup(loc);
    else
        pLocation = 0;
}

Error::~Error() throw()
{
    if( pLocation )
        delete pLocation;
}

const char *Error::getFormat(int code)
{
    switch( code ) {
    case InvalidArgument:
        return "'%s': Invalid argument in '%s'";

    case InternalError:
        return "Internal error: %s";

    case OpenFile:
        return "Unable to open file '%s': '%s'";
    case OpenCommand:
        return "Unable to open command '%s': '%s'";

    case ImageFileNotFound:
        return "Image file '%s' not found";
    case ImageNotDefined:
        return "Image '%s' not defined in this report";
    case ImageFormat:
        return "Invalid image format in object '%s', recordcount: %d";

    case FeatureNotSupported:
        return "Feature '%s' not supported";
    case FeatureNotCompiled:
        return "RTK has been compiled without '%s' support";


    case ParseReportOpenBracket:
        return "%s:%d,%d: expected '{' or '=' after '%s' in %s '%s'";
    case ParseReportProperty:
        return "%s:%d,%d: Invalid property '%s' in %s '%s'";
    case ParseReportValue:
        return "%s:%d,%d: Invalid value '%s' for property '%s' in %s '%s'";
    case ParseReportValueMissing:
        return "%s:%d,%d: Missing value for property '%s' in %s '%s'";
    case ParseReportElement:
        return "%s:%d,%d: Invalid element '%s' in %s";
    case ParseReportCloseBracket:
        return "%s:%d,%d: expected '}' in %s '%s'";
    case ParseReportSemicolon:
        return "%s:%d,%d: expected ';' after '%s' in %s '%s'";
    case ParseReportName:
        return "%s:%d,%d: Name not defined in %s";
    case ParseReportInputDriverName:
        return "%s:%d,%d: Property 'driver' must be the first one in INPUT: %s found";
    case ParseReportInputDriverNotRegistered:
        return "%s:%d,%d: Driver '%s' has not been registered";
    case ParseReportColon:
        return "%s:%d,%d: expected ':' or '='";
    case ParseReportIncludeFileNotFound:
        return "%s:%d,%d: Included file '%s' not found in %s";
    case ParseReportPreprocessor:
        return "%s:%d,%d: Directive %s not recognized";

    case ParseXml:
        return "Malformed XML file '%s'";

    case SqlConnect:
        return "%s";
    case SqlQuery:
        return "%s\n%s";

    case FormulaInputField:
        return "Input Field '%s' not found in formla:\n%s";

    case FormulaCxxNotFound:
        return "Cxx Formula: '%s' not found in object: '%s'";

    case FormulaSyntaxError:
        return "Cxx Formula: Syntax error in '%s'";

    case FormulaLibraryNotFound:
        return "Unable to open formula library '%s': '%s'";
    default:
        return 0;
    }
}

} // namespace RTK

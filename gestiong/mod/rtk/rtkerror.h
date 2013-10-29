/*<<<<<COPYLEFT*/
/** @file rtkerror.h Class to store errors
 * RTK The report toolkit. Copyright (C) 2003-2013 Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
#ifndef RTKERROR_H
#define RTKERROR_H

#include <stdexcept>
#include <gongxtring.h>

namespace gong {}
namespace RTK = gong;

namespace gong {

/**
@author Francisco Santiago Capel Torres
*/
class Error: public std::runtime_error
{
public:
    enum ErrorType { EError, EWarning, ENotice };
    enum AbortType { ThrowOnError, AbortOnError, AbortOnWarning, AbortAlways, AbortNever };
    enum ErrorCode {
        NoError = 0,

        InternalError,

        InvalidArgument,

        FeatureNotCompiled,
        FeatureNotSupported,

        ParseReportOpenBracket,
        ParseReportElement,
        ParseReportName,
        ParseReportProperty,
        ParseReportValue,
        ParseReportValueMissing,
        ParseReportSemicolon,
        ParseReportCloseBracket,
        ParseReportInputDriverName,
        ParseReportInputDriverNotRegistered,
        ParseReportColon,
        ParseReportIncludeFileNotFound,
        ParseReportPreprocessor,

        ParseXml,
        ParseXmlTypeAttr,

        OpenFile,
        OpenCommand,
        ImageFileNotFound,
        ImageNotDefined,
        ImageFormat,

        SqlConnect,
        SqlQuery,

        FormulaLibraryNotFound,
        FormulaInputField,
        FormulaCxxNotFound,
        FormulaSyntaxError


    };

    Error(ErrorType type, ErrorCode code, const char *loc, const char *message);
    virtual ~Error() throw();
    bool isWarning() const {
        return mType == EWarning;
    }
    bool isError() const {
        return mType == EError;
    }
    bool isNotice() const {
        return mType == ENotice;
    }
    ErrorType type() const {
        return mType;
    }
    ErrorCode code() const {
        return mCode;
    }
    const char *location() const {
        return pLocation;
    }
    const char *message() const {
        return what();
    }
    static AbortType errorAbort() {
        return sErrorAbort;
    }
    static const char *getFormat(int code);
    static AbortType sErrorAbort;

private:
    ErrorCode mCode;
    ErrorType mType;
    const char *pLocation;
};

} // namespace RTK

#endif // RTKERROR_H

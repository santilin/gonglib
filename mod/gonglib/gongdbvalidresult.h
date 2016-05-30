#ifndef _GONG_DBVALIDRESULT_H
#define _GONG_DBVALIDRESULT_H

/** @file gongdbintegresult.h Validation messages container
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <vector>
#include <gongdbconnection.h>

namespace gong {

class ValidResult
{
public:
    enum ErrorCode { ok = 0, warning, error };
    enum Context { saving = 0, editing, checking, fixing };
    struct MessageInfo
    {
        Xtring message;
        Xtring wrongfield;
        ErrorCode code;
        bool fixable;
    };

    ValidResult(Context ctxt = saving) : mContext(ctxt) {}
    uint count() const { return mMessages.size(); }
    uint countErrors() const;
    uint countWarnings() const;
	std::vector<MessageInfo> getMessages() const { return mMessages; }
	void addMessage(const MessageInfo &mi);
	void addMessage(ErrorCode code, const Xtring &message, const Xtring &fld, bool fixable=false);
    void addError(const Xtring &message, const Xtring &fld, bool fixable=false) {
        addMessage( error, message, fld, fixable);
    }
    void addWarning(const Xtring &message, const Xtring &fld, bool fixable=false) {
        addMessage( warning, message, fld, fixable);
    }
    MessageInfo getMessageInfo(uint i) const;
    Context getContext() const { return mContext; }
    void clear() { mMessages.clear(); }
    const Xtring &getFirstErrorMessage() const;
	const Xtring &getMessage(uint i) const;
	const ValidResult &append( const ValidResult &other);
private:
    std::vector<MessageInfo> mMessages;
    Context mContext;
};

}; // namespace gong

#endif

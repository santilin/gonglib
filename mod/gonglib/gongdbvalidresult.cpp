#include "gongdebug.h"
#include "gongdbvalidresult.h"

/**
	@file gongdbintegresult.h Validation messages container
	@class gong::ValidResult
	@brief Validation messages container

	A container for field and table validation messages that can be errors or warnings.

	@see dbFieldDefinition::isValid, dbRecord::isValid
*/


namespace gong {

ValidResult::MessageInfo ValidResult::getMessageInfo(unsigned int i) const
{
    if( i < mMessages.size() )
        return mMessages[i];
    else {
        MessageInfo mi;
        return mi;
    }
}

const Xtring &ValidResult::getFirstErrorMessage() const
{
    for( std::vector<MessageInfo>::const_iterator it = mMessages.begin();
            it != mMessages.end(); ++it ) {
        if( it->code == error )
			return it->message;
	}
	return Xtring::null;
}

const Xtring &ValidResult::getMessage(uint i) const
{
    if( i < mMessages.size() )
        return mMessages[i].message;
	else
		return Xtring::null;
}

void ValidResult::addMessage(ErrorCode code, const Xtring &message,
                             const Xtring &fld, bool fixable)
{
    MessageInfo mes = { message, fld.upper(), code, fixable };
    bool found = false;
    for( std::vector<MessageInfo>::const_iterator it = mMessages.begin();
            it != mMessages.end(); ++it )
        if( it->code == mes.code
                && it->message == mes.message
                && it->wrongfield.upper() == mes.wrongfield
                && it->code == mes.code )
            found = true;
    if( !found )
        mMessages.push_back(mes);
}

void ValidResult::addMessage(const MessageInfo &mi)
{
	mMessages.push_back(mi);
}


uint ValidResult::countErrors() const
{
    uint count = 0;
    for( std::vector<MessageInfo>::const_iterator it = mMessages.begin();
            it != mMessages.end(); ++it ) {
        if( it->code == error )
            count++;
    }
    return count;
}

uint ValidResult::countWarnings() const
{
    uint count = 0;
    for( std::vector<MessageInfo>::const_iterator it = mMessages.begin();
            it != mMessages.end(); ++it ) {
        if( it->code == warning )
            count++;
    }
    return count;
}

const ValidResult &ValidResult::append( const ValidResult &other)
{
    for( std::vector<MessageInfo>::const_iterator it = other.getMessages().begin();
            it != other.getMessages().end(); ++it ) {
		this->addMessage( *it );
	}
	return *this;
}



} // namespace gong

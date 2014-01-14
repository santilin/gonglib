#include <iostream>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <gongdebug.h>

#include "contactossmtpmailsender.h"

using namespace std;
using namespace Poco::Net;
using namespace Poco;

namespace gong {
namespace contactos {

SMTPMailSender::SMTPMailSender(const Xtring& smtphost, UInt16 port,
							   const Xtring& smtpuser, const Xtring& smtpassword)
	: pSession(0), mHost( smtphost ), mUser( smtpuser ), mPassword( smtpassword ), mPort( port )
{
}

SMTPMailSender::~SMTPMailSender()
{
	if( pSession )
		delete pSession;
}


void SMTPMailSender::setError(const Xtring &error)
{
	mError = error;
}

void SMTPMailSender::clearError()
{
	mError = Xtring::null;
}

const Xtring& SMTPMailSender::getError() const
{
	return mError;
}

int SMTPMailSender::open()
{
	int ret = 0;
	clearError();
    try {
        pSession = new SMTPClientSession(mHost, mPort);
        pSession->open();
        try {
            pSession->login(SMTPClientSession::AUTH_LOGIN, mUser, mPassword);
			ret = 1;
        } catch (SMTPException &e) {
            cerr << "SMTPException: " << e.displayText() << endl;
			setError( e.displayText() );
            ret = 0;
        }
    } catch (NetException &e) {
		setError( e.displayText() );
        ret = 0;
    }
	return ret;
}

int SMTPMailSender::send(const Xtring& from, const Xtring& to, const Xtring &subject, const Xtring& content)
{
	int ret = 0;
	clearError();
    Xtring enc_subject = MailMessage::encodeWord(subject, "UTF-8");
    MailMessage message;
    message.setSender(from);
    message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
    message.setSubject(enc_subject);
    message.setContentType("text/plain; charset=UTF-8");
    message.setContent(content, MailMessage::ENCODING_8BIT);
    try {
		pSession->sendMessage(message);
		ret = 1;
	} catch (SMTPException &e) {
		cerr << "SMTPException: " << e.displayText() << endl;
		setError( e.displayText() );
		ret = 0;
	} catch( ConnectionResetException &e ) {
		cerr << "SMTPException: " << e.displayText() << endl;
		setError( e.displayText() );
		ret = 0;
	}
    return ret;
}

int SMTPMailSender::sendHTML(const Xtring& from, const Xtring& to, const Xtring& subject, const Xtring& content)
{

	const char *c = content.c_str();
	const char *n = 0, *r = 0;
	while( *c ) {
		if( *c == '\n' )
			n = c;
		if( *c == '\r' ) {
			r = c;
			if( n + 1 != r ) {
				_GONG_DEBUG_PRINT(0, "Bare LF!");
				_GONG_DEBUG_PRINT(0, n );
			}
		}
		c++;
	}

	int ret = 0;
	clearError();
    Xtring enc_subject = MailMessage::encodeWord(subject, "UTF-8");
    MailMessage message;
    message.setSender(from);
    message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
    message.setSubject(enc_subject);
    message.setContentType("text/html; charset=UTF-8");
	Xtring lf_content(content);
	lf_content.replace("\x0a", "\x0d\x0a");
    message.setContent(lf_content, MailMessage::ENCODING_7BIT);
    try {
		pSession->sendMessage(message);
		ret = 1;
	} catch (SMTPException &e) {
		cerr << "SMTPException: " << e.displayText() << endl;
		setError( e.displayText() );
		ret = 0;
	} catch( ConnectionResetException &e ) {
		cerr << "SMTPException: " << e.displayText() << endl;
		setError( e.displayText() );
		open();
		ret = 0;
	}
    return ret;

}


int SMTPMailSender::close()
{
	int ret = 0;
    try {
		pSession->close();
		pSession = 0;
		ret = 1;
    } catch (NetException &e) {
		setError( e.displayText() );
        ret = 0;
    }
    return ret;
}


} // namespace contactos
} // namespace gong

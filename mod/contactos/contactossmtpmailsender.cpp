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

// 	std::string logo(reinterpret_cast<const char*>(PocoLogo), sizeof(PocoLogo));
// 	message.addContent(new StringPartSource(content));
// 	message.addAttachment("logo", new StringPartSource(logo, "image/gif"));

MailMessage *SMTPMailSender::createMessage(const Xtring& from, const Xtring& to, const Xtring& subject,
						   const Xtring &content, bool html)
{
	clearError();
    Xtring enc_subject = MailMessage::encodeWord(subject, "UTF-8");
    MailMessage *message = new MailMessage();
    message->setSender(from);
	if( !to.isEmpty() )
		message->addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
    message->setSubject(enc_subject);
	if( html )
		message->setContentType("text/html; charset=UTF-8");
	else
		message->setContentType("text/plain; charset=UTF-8");
    message->setContent(content, MailMessage::ENCODING_8BIT);
	return message;
}

int SMTPMailSender::sendMessage( MailMessage *message )
{
	int ret = 0;
    try {
		pSession->sendMessage(*message);
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


int SMTPMailSender::close()
{
	int ret = 0;
    try {
		pSession->close();
		ret = 1;
    } catch (NetException &e) {
		setError( e.displayText() );
        ret = 0;
    }
    return ret;
}


} // namespace contactos
} // namespace gong

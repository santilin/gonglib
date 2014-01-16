#ifndef DBAPPSMTPMAILSENDER_H
#define DBAPPSMTPMAILSENDER_H

#include <gongxtring.h>
#include <Poco/Types.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>

namespace gong {

class SMTPMailSender
{
public:
	SMTPMailSender( const Xtring &smtphost, Poco::UInt16 port,
					const Xtring &smtpuser, const Xtring &smtpassword );
	~SMTPMailSender();
	int open();
	Poco::Net::MailMessage *createMessage( const Xtring &from, const Xtring &to,
								const Xtring &subject, const Xtring &body, bool html = false );
	int sendMessage( Poco::Net::MailMessage *message );
	int close();
	const Xtring &getError() const;
private:
	void setError( const Xtring &error );
	void clearError();

	Poco::Net::SMTPClientSession *pSession;
	Xtring mHost, mUser, mPassword;
	Poco::UInt16 mPort;
	Xtring mError;
};

} // namespace gong

#endif // DBAPPSMTPMAILSENDER_H

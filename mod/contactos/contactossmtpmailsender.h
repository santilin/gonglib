#ifndef CONTACTOSSMTPMAILSENDER_H
#define CONTACTOSSMTPMAILSENDER_H

#include <gongxtring.h>
#include <Poco/Types.h>
#include <Poco/Net/SMTPClientSession.h>

namespace gong {
namespace contactos {

class SMTPMailSender
{
public:
	SMTPMailSender( const Xtring &smtphost, Poco::UInt16 port, const Xtring &smtpuser, const Xtring &smtpassword );
	~SMTPMailSender();
	int open();
	int send( const Xtring &from, const Xtring &to, const Xtring &subject, const Xtring &body);
	int sendHTML( const Xtring &from, const Xtring &to, const Xtring &subject, const Xtring &body);
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

} // namespace contactos
} // namespace gong

#endif // CONTACTOSSMTPMAILSENDER_H

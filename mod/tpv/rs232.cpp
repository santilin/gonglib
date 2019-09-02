/* Better port reading program
   v1.0
   23-10-96
   
   This test program uses quasi-POSIX compliant UNIX functions to
   open the ABU port and read.
   
   Uses termio functions to initialise the port to 9600 baud, at
   8 data bits, no parity, no hardware flow control,
   and features character buffering.
   The 20ms delay after the port read indicates that characters are
   buffered if a button is pressed many times.
   
   This program was derived from instructions at
    http://www.easysw.com/~mike/serial/
*/

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <gongdebug.h>
#include "rs232.h"

namespace gong {
	
RS232::RS232(const Xtring &com_port)
	: mComPort(com_port), mFileHandle(0)
{
}

bool RS232::open()
{
	mFileHandle = ::open(mComPort.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	if (mFileHandle == -1)
	{                                              /* Could not open the port */
		fprintf(stderr, "open_port: Unable to open %s - %s\n",
				mComPort.c_str(), strerror(errno));
	}
	return (mFileHandle != -1);
}

/* Close the serial port */
int RS232::close()
{
	return ::close(mFileHandle);
}

int RS232::setup(int speed, short int bits, 
		  char parity, short int stopbits, bool hard)
{
	struct termios options;
	fcntl(mFileHandle, F_SETFL, FNDELAY);              /* Configure port reading */
									/* Get the current options for the port */
	tcgetattr(mFileHandle, &options);
	cfsetispeed(&options, B9600);             /* Set the baud rates to 9600 */
	cfsetospeed(&options, B9600);

								  /* Enable the receiver and set local mode */
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB; /* Mask the character size to 8 bits, no parity */
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |=  CS8;                          /* Select 8 data bits */
	options.c_cflag &= ~CRTSCTS;           /* Disable hardware flow control */  

								/* Enable data to be processed as raw input */
	options.c_lflag &= ~(ICANON | ECHO | ISIG);

									    /* Set the new options for the port */
	return tcsetattr(mFileHandle, TCSANOW, &options);						
}

Xtring RS232::read(int maxlen, int timeout)
{
	char chout;
	Xtring ret;
	while (timeout > 0 && maxlen > 0)
	{
		int r = ::read(mFileHandle, &chout, sizeof(chout));
		if( r == -1 ) 
			return ret;
		if( r == sizeof(chout) ) {
			--maxlen;
			ret += chout;
		} else {
			timeout--;
			usleep(1000);
			continue;
		}
	}
	return ret;
}

Xtring RS232::readLine()
{
	char chout;
	Xtring ret;
	while (true )
	{
		int r = ::read(mFileHandle, &chout, sizeof(chout));
		if( r == -1 || r == 0) 
			return ret;
		if( r == sizeof(chout) ) {
			if( chout == '\n' || chout == '\r' ) {
				return ret;
			} else {
				ret += chout;
			}
		}
	}
	return ret;
}


int RS232::write(const Xtring &text)
{
	return ::write(mFileHandle, text.c_str(), text.size());          /* Read character from ABU */
}

} // namespace

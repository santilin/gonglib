#ifndef TVP_RS232_H
#define TVP_RS232_H

#include <gongxtring.h>

namespace gong {
	
class RS232
{
public:
	RS232(const Xtring &port_name);
	int setup(int speed, short int bits = 8, 
				 char parity = 'N', short int stopbits = 1, bool hardflowcontrol = false);
	bool open();
	int close();
	int write(const Xtring &text);
	Xtring read(int maxlen, int timeout);
	Xtring readLine();
private:
	Xtring mComPort;
	int mFileHandle;
};

} // namespace

#endif // TVP_RS232_H
	
	
	

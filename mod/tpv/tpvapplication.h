#include <getopt.h>
#include <dbappdbapplication.h>


using namespace gong;

class TPVApplication: public dbApplication
{
public:
	TPVApplication(const char *dbversion, const char *datadir,
                   const char *packagename, const char *packageversion,
                   int &argc, char **argv )
		: dbApplication(dbversion, datadir, packagename, packageversion, argc, argv), mIsTpvScreen(false)
	{
		int c;
		Xtring errmsg;
		optind = 1;
		while ( 1 ) {
			c = getopt( argc, argv, "t");
			if ( c == -1 )
				break;

			switch ( c ) {
			case 't':
				mIsTpvScreen = true;
				break;
			}
		}
	}
	bool isTpvScreen() const { return mIsTpvScreen; }
private:
	bool mIsTpvScreen;
};

extern TPVApplication *TPVAPP;

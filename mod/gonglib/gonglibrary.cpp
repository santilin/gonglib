#include "config.h"
#include <cstdio>
#include <cstdlib> // getenv
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <cerrno>
#include <netinet/in.h>
#include <getopt.h>
extern int optind;

#include "gongdebug.h"
#include "gongfileutils.h"
#include "gonggettext.h"
#include "gonglibrary.h"


extern "C" {
    void __stack_chk_fail(void)
    {
        std::cerr << "__stack_chk_fail" << std::endl;
        exit( 0 );
    }
    void terminate_handler()
    {
        std::cerr << "terminate_handler" << std::endl;
    }

    void unexpected_handler()
    {
        std::cerr << "Unexpected exception"  << std::endl;
    }
}

namespace gong {

const List<int> NullIntList;
const XtringList NullXtringList;
List<int> IdentityIntList;

GongLibrary *GongLibraryInstance = 0;

GongLibrary::GongLibrary( const char *datadir, const char *packagename,
                          const char *packageversion, int argc, char *argv[] )
    : mGlobalDataDir( datadir ), pPackageName( packagename ), pPackageVersion( packageversion )
{
    _GONG_DEBUG_TRACE( 1 );
    std::set_unexpected( unexpected_handler );
    std::set_terminate (__gnu_cxx::__verbose_terminate_handler);
    GongLibraryInstance = this;
	if( mGlobalDataDir.isEmpty() )
		throw new std::runtime_error("GongLibrary::datadir can not be empty");
    FileUtils::addSeparator( mGlobalDataDir );
    mGonglibDataDir = mGlobalDataDir + "gonglib/";
    mGlobalDataDir += Xtring(packagename).lower() + "/";
    mLocalDataDir = Xtring( getenv( "HOME" ) ) + "/.gonglib/" + Xtring(pPackageName).lower() + "/";
    Xtring packagename_save(pPackageName), localdadadir_save = mLocalDataDir;
    parseArguments( argc, argv ); // Here datadir can have changed
    if( packagename_save != pPackageName && localdadadir_save == mLocalDataDir ) {
        // If packagename has changed but localdatadir has not, rebuild localdatadir
        mLocalDataDir = Xtring( getenv( "HOME" ) ) + "/.gonglib/" + Xtring(pPackageName).lower() + "/";
    }
    FileUtils::addSeparator( mLocalDataDir);
    FileUtils::addSeparator( mGlobalDataDir);
    if ( !FileUtils::exists( mLocalDataDir.c_str() ) )
        FileUtils::makePath( mLocalDataDir );

#ifdef _GONG_DEBUG
    char pathname[256];
    char *s = getcwd( pathname, 255 );
    if( s ) {
        _GONG_DEBUG_PRINT( 0, Xtring::printf(
                               "CWD=%s\nGonglibDataDir=%s\nGlobalDataDir=%s\nLocalDataDir=%s",
                               pathname, getGonglibDataDir().c_str(),  getGlobalDataDir().c_str(),
                               getLocalDataDir().c_str() ) );
    }
#ifdef ENABLE_NLS
    _GONG_DEBUG_PRINT( 0, Xtring::printf(
                           "LOCALEDIR=%s, locale=\"%s\", textdomain=\"%s\"",
                           LOCALEDIR, setlocale( LC_MESSAGES, 0 ), textdomain( NULL ) ) );
#endif
#endif
}

void GongLibrary::init()
{
#ifdef ENABLE_NLS
    bindtextdomain( mPackageName.c_str(), LOCALEDIR );
    bind_textdomain_codeset( mPackageName.c_str(), "UTF-8" );
    textdomain( mPackageName.c_str() );
#endif
}


GongLibrary::~GongLibrary()
{
    _GONG_DEBUG_TRACE( 1 );
}

Xtring GongLibrary::getHostIP( const Xtring &host ) const
{
    char           buf[BUFSIZ];
    int            err;
    int            ret;
    struct hostent he;
    struct hostent *hp = 0;
    struct utsname ut;

    if ( uname( &ut ) < 0 ) {
        return Xtring();
    }

    if ( *ut.nodename ) {
        printf( "Node name:  %s\n", ut.nodename );
    }

    ret = gethostbyname_r( ut.nodename, &he, buf, sizeof buf, &hp, &err );

    if ( hp == NULL || ret != 0 ) {
        return Xtring();
    }

    if ( hp->h_length > ( int ) sizeof( struct in_addr ) ) {
        hp->h_length = sizeof( struct in_addr );
    }
    Xtring retvalue;
    while ( *hp->h_addr_list ) {
        printf( "  %s\n", inet_ntoa( * (( struct in_addr* ) *hp->h_addr_list ) ) );
        retvalue = inet_ntoa( * (( struct in_addr* ) * hp->h_addr_list ) );
        hp->h_addr_list++;
    }
    return retvalue;
}


Xtring GongLibrary::getInterfaceIP( const Xtring &ifname ) const
{
    struct ifreq ifr;
    struct sockaddr_in *sin = ( struct sockaddr_in * ) & ifr.ifr_addr;
    int sockfd;

    bzero( &ifr, sizeof( ifr ) );

    if (( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
        perror( "socket()" );
        return ( Xtring::null );
    }

    strcpy( ifr.ifr_name, ifname.c_str() );
    sin->sin_family = AF_INET;

    if ( ioctl( sockfd, SIOCGIFADDR, &ifr ) == 0 ) {
        printf( "\n%s : [%s]\n", ifr.ifr_name, inet_ntoa( sin->sin_addr ) );
        return inet_ntoa( sin->sin_addr );
    } else
        return Xtring();
}

Xtring GongLibrary::getUsage() const
{
    return Xtring::printf( _( Xtring("Gong Library options\n"
                                     "  -v, --version\t\tPrints the version of ") + getPackageName() + " and exits\n"
                              "  -l, --local-cfg-path path\tConfiguration path (%s)\n"
                              "  -g, --global-cfg-path path\tConfiguration path (%s)\n"
                              "  -d, --debug debuglevel\tLevel of debug output\n" ),
                           mLocalDataDir.c_str(), mGlobalDataDir.c_str() );
}

int GongLibrary::parseArguments( int argc, char **argv )
{
    int c;
    Xtring errmsg;

    while ( 1 ) {
        int option_index = 0;
        static struct option long_options[] = {
            {"version", no_argument, 0, 'v'},
            {"local-cfg-path", required_argument, 0, 'l'},
            {"global-cfg-path", required_argument, 0, 'g'},
            {"package-name", required_argument, 0, 'p'},
            {"debug", required_argument, 0, 'd'},
            {0, 0, 0, 0}
        };
        // Do not forget to add here the parameter letters and colons
        c = getopt_long( argc, argv, "vl:g:d:p:", long_options, &option_index );
        if ( c == -1 )
            break;

        switch ( c ) {
        case 'v':
            fprintf( stdout, "%s\n", getPackageVersion() );
            exit( 0 );
            break;
        case 'l':
            mLocalDataDir = optarg;
            _GONG_DEBUG_PRINT( 1, Xtring::printf( "ArgLocalDataDir=%s", optarg ) );
            break;
        case 'g':
            mGlobalDataDir = optarg;
            _GONG_DEBUG_PRINT( 1, Xtring::printf( "ArgGlobalDataDir=%s", optarg ) );
            break;
        case 'p':
            pPackageName = optarg;
            _GONG_DEBUG_PRINT( 1, Xtring::printf( "PackageName=%s", optarg ) );
            break;
        case 'd':
#ifdef _GONG_DEBUG
            ::__gong_debug::_gong_debug_level = Xtring( optarg ).toInt();
            _GONG_DEBUG_PRINT( 1, Xtring::printf( "ArgDebug=%s", optarg ) );
#else
            _GONG_DEBUG_WARNING( "--debug option passed but GongLib was not compiled with debug support" );
#endif
            break;
        }
    }
    return 1;
}

}; // namespace


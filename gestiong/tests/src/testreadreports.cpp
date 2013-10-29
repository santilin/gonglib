
#include <basertkreport.h>
#include <cstdlib>
#include "testreadreports.h"



using namespace gong;
using namespace RTK;

TestReadReports::TestReadReports()
{
	system( "pwd" );
}


TestReadReports::~TestReadReports()
{
}



void TestReadReports::run()
{
	RTK::Error::sErrorAbort = RTK::Error::AbortNever;

	Xtring path = "../../../share/gestiong/informes/";
	XtringList reports;
	reports << "miembros.rtk" << "cuentas.rtk" << "miembros.rtk";
	for( XtringList::iterator it = reports.begin();
	        it != reports.end();
	        ++it)
	{
		_GONG_DEBUG_PRINT(4, Xtring::printf("Informe: %s%s", path.c_str(), (*it).c_str()) );
		RTK::Report * r = new RTK::Report();
		if( r->readRtkFile( Xtring(path + "/" + (*it)).c_str()  ) )
		{
			stReportInfo ri;
			r->fixParameters(RTK::ParametersList(), "");
			ri.repPath = path + (*it);
			ri.repName = r->name();
			ri.repTitle = r->title();
			ri.repGroup = r->tag();
		} 
		else {
	        for( int e=0; e < r->errorsCount(); e++ ) {
	            std::cerr << (r->getError(e)->isWarning()? "Warning: ":"Error: ")
    	            << r->getError(e)->location() << ": " << r->getError(e)->message() << std::endl;
			}
		}
		delete r;
	}
}



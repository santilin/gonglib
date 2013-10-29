//
// C++ Implementation: testintegrityresult
//
// Description: 
//
//
// Author: Santiago Capel <elcansino@arremolina.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <gongdebug.h>
#include "testintegrityresult.h"
#include <gongdbintegresult.h>
#include <iostream>

using namespace std;
using namespace gong;


TestIntegrityResult::TestIntegrityResult()
{
}


TestIntegrityResult::~TestIntegrityResult()
{
}

bool showValidMessages(bool *isvalid, const IntegrityResult &validresult)
{
    Xtring messagesA;
    if( !(*isvalid) )
    {
		_GONG_DEBUG_PRINT(2,validresult.getMessageInfo(0).wrongfield);
        // Mostrar todos los mensajes de error, no los avisos
#if 0
        for( int nm=0; nm<validresult.count(); nm++ )
            if( validresult.getMessageInfo(nm).sev >= IntegrityResult::invalid )
                messages += validresult.getMessageInfo(nm).message + "\n";
//		Xtring caption = "Error " + getTitle();
#endif
		messagesA = "Mensaje";
		_GONG_DEBUG_PRINT(2,validresult.getMessageInfo(0).wrongfield);
		_GONG_DEBUG_PRINT(2,validresult.getMessageInfo(0).wrongfield);
		_GONG_DEBUG_PRINT(2,validresult.getMessageInfo(0).wrongfield);
    }
	return true;
}


void TestIntegrityResult::run()
{
	IntegrityResult r;
	r.addMessage( INTEG_INVALID, IntegrityResult::invalid, 
		"Elige 'En Local' o 'En calle'", "ENLOCAL" );
	bool isvalid = false;
	_GONG_DEBUG_ASSERT(  showValidMessages( &isvalid, r )  );
}

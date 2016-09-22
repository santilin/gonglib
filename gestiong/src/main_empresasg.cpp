/*<<<<<COPYLEFT*/
/* GestiONG Gestión para ONGs y Asociaciones sin ánimo de lucro
 * Copyright (C) 2003 Francisco Santiago Capel Torres
 *
 * main_empresas.cpp función main()
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/*>>>>>COPYLEFT*/

#include "config.h"

#include <exception>
#include <iostream>
#include <gongdebug.h>
#include <gonglist.hpp>
#include <gongdbfieldlistofvalues.hpp>
#include <dbappdbapplication.h>

#ifdef HAVE_CONTACTOSMODULE
#	include <contactosmodule.h>
#endif
#ifdef HAVE_EMPRESAMODULE
#	include <empresamodule.h>
#endif
#ifdef HAVE_TESORERIAMODULE
#	include <tesoreriamodule.h>
#endif
#ifdef HAVE_PAGOSMODULE
#	include <pagosmodule.h>
#endif
#ifdef HAVE_FACTUMODULE
#	include <factumodule.h>
#endif
#ifdef HAVE_SINPAPELESMODULE
#	include <sinpapelesmodule.h>
#endif
#ifdef HAVE_RECCOLORMODULE
#	include <reccolormodule.h>
#endif

using namespace gong;

int main( int argc, char *argv[] )
{
    int ret = 0;
    try {
        DBAPP = new dbApplication( VERSION ".1", PACKAGE_DATA_DIR,
                                   "EmpresasG", PACKAGE_VERSION, argc, argv );
// 		bool warn_language = DBAPP->getSetting("WARN_LANGUAGE", true).toBool();
// 		char *language = getenv("LANGUAGE");
// 		if( language && *language && !DBAPP->getRegConfig()->setLanguage( language ) ) {
// 			if( warn_language )
// 				FrmBase::msgOk( DBAPP->getPackageString(),
// 								Xtring::printf("El entorno de tu sistema (LANGUAGE) está configurado para presentar mensajes en el lenguaje '%s', pero tu sistema no tiene este lenguaje disponible.\nSe mostrarán los mensajes en el lenguaje '%s'",
// 									language?language:"", setlocale(LC_MESSAGES, NULL) ), FrmBase::information );
// 		}
        DBAPP->init();
#ifdef HAVE_CONTACTOSMODULE
        DBAPP->addModule( new contactos::ContactosModule(argc, argv) );
#endif
        DBAPP->readSettings();
        DBAPP->initDatabases();
        if( DBAPP->login(PACKAGE_VERSION) ) {
            DBAPP->initMainWindow();
            DBAPP->exec();
            DBAPP->writeSettings();
        }
    } catch( std::runtime_error e) {
        _GONG_DEBUG_WARNING( e.what() );
        return 1;
    }
    delete DBAPP;
    return ret;
}

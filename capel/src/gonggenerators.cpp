#include <fstream>
#include "gonggenerators.h"

using namespace gong;

void capel::getConditionalCode( const Xtring &moreprops, Xtring &csh, Xtring &cscpp, Xtring &ceh, Xtring &cecpp )
{
	bool module_included = false, module_not_included = false;
	if( moreprops.startsWith("#if") ) {
		cscpp = moreprops + "\n";
		cscpp.replace("|", " ");
		cecpp= "#endif\n";
		csh = cscpp;
		ceh = cecpp;
	} else if( moreprops.startsWith("if") ) {
		cscpp = moreprops + "{\n";
		cscpp.replace("|", " ");
		cecpp = "}\n";
		// No se hace nada en el .h
	} else if( moreprops.startsWith("MODULE_INCLUDED") ) {
		module_included = true;
	} else if( moreprops.startsWith("MODULE_NOT_INCLUDED") ) {
		module_not_included = true;
	}
	if( module_included || module_not_included ) {
		XtringList modparts;
		moreprops.tokenize(modparts, "(,)");
		if( modparts.size() < 2 ) {
			std::cout << moreprops << std::endl;
			exit(2);
		}
		Xtring Module = modparts[1];
		Xtring MODULE = Module.upper();
		cscpp = "#ifdef HAVE_" + MODULE + "MODULE\n";
		if( module_included ) {
			cscpp += "if( ModuleInstance->get" + Module + "Module() ) {\n";
			cecpp = "}\n#endif\n";
		}
		if( module_not_included ) {
			cscpp += "if( !ModuleInstance->get" + Module + "Module() ) {\n";
			cscpp += "#else\n";
			cscpp += "if( true ) {\n";
			cecpp += "#endif\n}\n";
		}
		if( module_included ) {
			csh = "#ifdef HAVE_" + MODULE + "MODULE\n";
		} else {
			csh = "#ifndef HAVE_" + MODULE + "MODULE\n";
		}
		ceh = "#endif\n";
	}
}

bool createFileWithContent(const char *filename, const Xtring &content)
{
	std::ofstream pFile(filename);
    if( !pFile ) {
		perror(filename);
		return false;
	} else {
		_GONG_DEBUG_PRINT(0, Xtring::printf( "capel: file: %s,  %d bytes",
			filename, content.length() ) );
        pFile.write(content.c_str(), content.length());
		if( pFile.bad() ) {
			_GONG_DEBUG_WARNING(Xtring::printf( "capel: file: %s, error writing %d bytes",
				filename, content.length() ) );
			return false;
		}
		pFile.close();
	}
	return true;
}

void capel::genMakefileCvs()
{
	Xtring content =
"default: all\n"
"\n"
"all:\n"
"	aclocal -I m4\n"
"	autoheader\n"
"	libtoolize --force -c\n"
"	automake --add-missing\n"
"	autoconf\n";

	createFileWithContent( "Makefile.cvs", content );
}

void capel::genSrc_MakefileAm(const Xtring &type, const Xtring &args)
{
	Xtring modulename = args.lower();
	Xtring content;

	if( type == "PROJECT" ) {
		content =
"AM_CPPFLAGS = @AM_CPPFLAGS@ -DPACKAGE_LOCALE_DIR=\\\"\"$(prefix)/$(DATADIRNAME)/locale\"\\\" \\\n"
"	-DPACKAGE_SRC_DIR=\\\"\"$(srcdir)\"\\\" \\\n"
"	-DPACKAGE_DATA_DIR=\\\"\"$(datadir)\"\\\"\n"
"INCLUDES = @GONGLIB_CPPFLAGS@\n"
"\n"
"bin_PROGRAMS = " + modulename + "\n"
"" + modulename + "_LDADD = @GONGLIB_LIBS@\n"
"\n"
"" + modulename + "_SOURCES = main.cpp " + modulename + "module.cpp\n"
"noinst_HEADERS = " + modulename + "module.h\n";
	} else {
		content =
"AM_CPPFLAGS = @GONGLIB_CPPFLAGS@\n"
"\n"
"lib_LTLIBRARIES = libgong" + modulename + ".la\n"
"libgong" + modulename + "_ladir = $(includedir)/gonglib\n"
"\n"
"libgong" + modulename + "_la_LDADD = @GONGLIB_LIBS@\n"
"\n"
"libgong" + modulename + "_la_SOURCES = " + modulename + "module.cpp\n"
"libgong" + modulename + "_la_HEADERS = " + modulename + "module.h\n";
	}

	createFileWithContent( "src/Makefile.am", content );
}

void capel::genModuleCpp(const Xtring &args, const XtringList &modules)
{
	Xtring modulename = args;
	// Generate modulename-module.cpp
	Xtring filename = "src/" + modulename.lower() + "module.cpp";
	if( !gong::FileUtils::exists( filename.c_str() ) ) {
		CppModule *module_cpp = new CppModule( filename );

		Xtring content =
"// TYPE GongModule " + modulename.lower() + "::" + modulename + "Module\n";

		module_cpp->insert_extrusion("MODULE_INFO", content);
		module_cpp->writeIfModified();
		delete module_cpp;
	}

	// Generate main.cpp
	filename = "src/main.cpp";
	CppModule *main_cpp = new CppModule( filename );

	Xtring before_includes =
"#include \"config.h\"\n"
"#include <gongdebug.h>\n"
"#include <dbappdbapplication.h>\n"
"#include <dbappdbmodule.h>\n";
	Xtring after_includes =
"#include <gongdbfieldlistofvalues.hpp>\n"
"using namespace gong;\n";

	Xtring modules_includes, modules_load;

	for( XtringList::const_iterator it = modules.begin();
		it != modules.end(); ++ it ) {
		const Xtring &module = *it;
		if( module.startsWith("rtk") || module.startsWith("qt4") || module == "dbapp" )
			continue;
		modules_includes +=
"#ifdef HAVE_" + module.upper() + "MODULE\n"
"#\tinclude <" + module.lower() + "module.h>\n"
"#endif\n";
		modules_load +=
"#ifdef HAVE_" + module.upper() + "MODULE\n"
"\t\tDBAPP->addModule( new " + module.lower() + "::" + module + "Module() );\n"
"#endif\n";
	}
	main_cpp->insert_extrusion( "MODULES_INCLUDES", modules_includes, after_includes, before_includes);

	Xtring before_load_modules =
"int main( int argc, char *argv[] )\n"
"{\n"
"\tint ret = 0;\n"
"\ttry {\n"
"\t\tDBAPP = new dbApplication(PACKAGE_VERSION, PACKAGE_DATA_DIR, PACKAGE_NAME,\n"
"\t\t\t\t\tPACKAGE_VERSION, argc, argv );\n"
"\t\tDBAPP->init();\n";

	Xtring after_load_modules =
"\t\tDBAPP->readSettings();\n"
"\t\tDBAPP->initDatabases();\n"
"\t\tif( DBAPP->login(PACKAGE_VERSION) ) {\n"
"\t\t\tDBAPP->initMainWindow();\n"
"\t\t\tDBAPP->exec();\n"
"\t\t\tDBAPP->writeSettings();\n"
"\t\t}\n"
"\t} catch( std::runtime_error e) {\n"
"\t\t_GONG_DEBUG_WARNING( e.what() );\n"
"\t\treturn 1;\n"
"\t}\n"
"\tdelete DBAPP;\n"
"\treturn ret;\n"
"}\n";

	main_cpp->insert_extrusion( "LOAD_MODULES", modules_load, after_load_modules, before_load_modules);
	main_cpp->writeIfModified();
	delete main_cpp;
}

void capel::genModuleConfigure_ac(CapelModule *cpm, const XtringList &modules, const Xtring &args)
{
	XtringList props;
	args.tokenize( props, " ");
	Xtring modulename = props[0];
	Xtring gonglib_path;
	bool isproject = props.remove( "PROJECT" );
	bool ismodule = props.remove( "MODULE" );
	if( props.size() > 1 )
		gonglib_path = props[1];
	else
		gonglib_path = "../..";

	Xtring ext_prefix = "GONGLIB_MODULE_";

	Xtring ac_init =
"AC_CONFIG_MACRO_DIR([m4])\n"
"AC_CONFIG_HEADERS([config.h])\n"
"AC_PROG_CXX\n"
"AM_PROG_LIBTOOL\n"
"AM_INIT_AUTOMAKE\n"
"\n"
"AC_MSG_NOTICE( [" + modulename + " Custom configuration] )\n"
"\n"
"AC_ARG_ENABLE(debug,\n"
"  AS_HELP_STRING([--enable-debug],\n"
"				  [enable debugging, default: no]),\n"
"  [case \"${enableval}\" in\n"
"			  yes)\n"
"				AM_CPPFLAGS=\"$AM_CPPFLAGS -D_GONG_DEBUG\"\n"
"				AM_CXXFLAGS=\"$AM_CXXFLAGS -g -g3 -O0\"\n"
"               AC_SUBST(AM_CPPFLAGS)\n"
"               AC_SUBST(AM_CXXFLAGS)\n"
"				;;\n"
"			  no)\n"
"				AM_CXXFLAGS=\"$AM_CXXFLAGS -Wall -Werror\"\n"
"               AC_SUBST(AM_CXXFLAGS)\n"
"				;;\n"
"			  *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;\n"
"  esac],\n"
"  [debug=false])\n"
"\n"
"AC_ARG_WITH( [poco],\n"
"	AS_HELP_STRING([--with-poco], [Use the poco library]),\n"
"	[ac_cv_use_poco=$withval],\n"
"	[ac_cv_use_poco=yes] )\n"
"AM_CONDITIONAL( [COMPILE_WITH_POCOLIB], [ test \"$ac_cv_use_poco\" = yes ])\n"
"if test \"$ac_cv_use_poco\" = yes ; then\n"
"        GONG_CHECK_POCO\n"
"	if test \"x$POCO_CPPFLAGS\" = x ; then\n"
"		AC_MSG_ERROR([[No se han detectado las bibliotecas de desarrollo de Poco. Si las has instalado en un lugar no estandard, usa la opción --with-poco=PFX. Para instalar las bibliotecas de MySQL: (Mandriva: libpoco-devel) (Debian/K/Ubuntu: libpoco-dev) ]])\n"
"	fi\n"
"fi\n"
"\n"
"AC_ARG_WITH( [mysql], \n"
"	AS_HELP_STRING([--with-mysql], [Support for input from MySQL (default is YES)]), \n"
"	[ac_cv_use_mysql=$withval],\n"
"	[ac_cv_use_mysql=yes] )\n"
"AM_CONDITIONAL( [COMPILE_WITH_MYSQLLIB], [ test \"$ac_cv_use_mysql\" = yes ])\n"
"if test \"$ac_cv_use_mysql\" = yes ; then\n"
"        GONG_CHECK_MYSQL\n"
"\tif test \"x$MYSQL_CPPFLAGS\" = x ; then\n"
"\t\tAC_MSG_ERROR([[No se han detectado las bibliotecas de desarrollo de MySQL. Si las has instalado en un lugar no estandard, usa la opción --with-mysql-prefix=PFX. Para instalar las bibliotecas de MySQL: (Mandriva: libmysql-devel) (Debian/K/Ubuntu: libmysqlclient15-dev) ]])\n"
"\tfi\n"
"fi\n"
"AX_LIB_SQLITE3\n"
"\n"
// "AC_ARG_WITH( [libxml2], \n"
// "			AS_HELP_STRING([--with-libxml2], [Support for XML input and output (default is YES)]), \n"
// "			[ac_cv_use_libxml2=$withval],\n"
// "			[ac_cv_use_libxml2=no] )\n"
// "AM_CONDITIONAL( [COMPILE_WITH_LIBXML2], [ test \"$ac_cv_use_libxml2\" = yes ])\n"
// "if test \"$ac_cv_use_libxml2\" = yes ; then\n"
// "\tAC_DEFINE( [HAVE_LIBXML2], [], [Define to 1 if you have the Xml2 library.] )\n"
// "\tm4_ifdef([AM_PATH_XML2], [AM_PATH_XML2([2.5.0])], [] )\n"
// "\tif test \"x$XML_CPPFLAGS\" = x ; then\n"
// "\t\tAC_MSG_ERROR([[No se han detectado las bibliotecas de desarrollo de XML2 (versión 2.5.0 o superior). Si las has instalado en un lugar no estandard, usa la opción --with-xml-prefix=PFX. Para instalar XML2 (Mandriva: libxml2-devel) (Debian/K/Ubuntu: libxml2-dev) ]])\n"
// "\tfi\n"
// "fi\n"
// "\n"
"AC_ARG_WITH( [boost],\n"
"            AS_HELP_STRING([--with-boost], [Support for the Boost library (default is YES)]),\n"
"            [ac_cv_use_boost=$withval],\n"
"            [ac_cv_use_boost=yes] )\n"
"if test \"$ac_cv_use_boost\" = yes ; then\n"
"		AX_BOOST_BASE(1.0)\n"
"		AC_DEFINE( [HAVE_boost], [], [Define to 1 if you have the Boost library.] )\n"
"fi\n"
"\n";

	cpm->insert_extrusion( ext_prefix + "INIT",
	                      ac_init, Xtring::null,
			"AC_INIT([" + modulename + "], [0.1], [scapel@users.sourceforge.net], [" + modulename.lower() + "])\n");

	Xtring ac_gonglib;
	Xtring ac_inc_path = gonglib_path;
	Xtring ac_lib_path = (isproject ? ".." : "../.." );
	ac_gonglib +=
"GONGLIB_CPPFLAGS=\"-I" + ac_inc_path + "/gonglib $MYSQL_CPPFLAGS $SQLITE3_CFLAGS $POCO_CPPFLAGS $BOOST_CPPFLAGS \"\n"
"GONGLIB_LIBS=\"-L" + ac_lib_path + "/gonglib -lgonglib $MYSQL_LIBS $SQLITE3_LDFLAGS $POCO_LIBS $BOOST_LDFLAGS \"\n";
	cpm->insert_extrusion( ext_prefix + "GONGLIB",
	                      ac_gonglib);


	Xtring module_substs, ac_outputs;
	for( XtringList::const_iterator it = modules.begin(); it != modules.end(); ++it ) {
		Xtring module = *it;
		XtringList module_info;
		module.tokenize( module_info, " " );
		Xtring modulename = module_info[0];
		Xtring module_extra_check;
		if( module_info.size() > 1 )
			module_extra_check = module_info[1];
		if( !module_substs.isEmpty() )
			module_substs += "\n";
		module_substs +=
			"AC_ARG_WITH( [gong-" + modulename.lower() + "],\n"
			"AS_HELP_STRING([--with-gong-" + modulename.lower() +
				"], [Support for the gong " + modulename.lower() + " module (default is YES)]), \n"
			"[ac_cv_use_gong_" + modulename.lower() + "=$withval],\n"
			"[ac_cv_use_gong_" + modulename.lower() + "=yes] )\n"
			"AM_CONDITIONAL( [COMPILE_WITH_GONG_" + modulename.upper()
				+ "], [ test \"$ac_cv_use_gong_" + modulename.lower() + "\" = yes ])\n"
			"if test \"$ac_cv_use_gong_" + modulename.lower() + "\" = yes ; then\n";
		if( !module_extra_check.isEmpty() )
			module_substs += "\n\t" + module_extra_check + "\n\n";
		module_substs +=
			"\tAC_DEFINE( [HAVE_" + modulename.upper() + "MODULE], [1], [Define to 1 if you have the " + modulename + " module.] )\n"
			"\tGONG" + modulename.upper() + "_CPPFLAGS=\"-I" + ac_inc_path + "/" + modulename.lower();
		if( modulename.upper() == "QT4" )
			module_substs += " $QT_CXXFLAGS";
		module_substs += "\"\n"
			"\tGONG" + modulename.upper() + "_LIBS=\"-L" + ac_lib_path + "/" + modulename.lower() + " -lgong" + modulename.lower();
		if( modulename.upper() == "RTK" )
			module_substs += " -ljpeg -lpng $BDB_LDADD";
		if( modulename.upper() == "QT4" )
			module_substs += " $QT_LDADD -lQt3Support";
		module_substs += "\"\n";
		module_substs += "\tGONGLIB_CPPFLAGS=\"$GONGLIB_CPPFLAGS $GONG" + modulename.upper() + "_CPPFLAGS\"\n"
						 "\tGONGLIB_LIBS=\"$GONG" + modulename.upper() + "_LIBS $GONGLIB_LIBS\"\n";
		module_substs +=
			"else\n"
			"\tGONG" + modulename.upper() + "_CPPFLAGS=\n"
			"\tGONG" + modulename.upper() + "_LIBS=\n"
			"fi\n"
			"AC_SUBST(GONG" + modulename.upper() + "_CPPFLAGS)\n"
			"AC_SUBST(GONG" + modulename.upper() + "_LIBS)\n";
		ac_outputs += " " + modulename.lower() + "/Makefile";
	}
	if( isproject ) {
		cpm->insert_extrusion( ext_prefix + "_MODULES", module_substs +
			"AC_SUBST(AM_CXXFLAGS)\n"
			"AC_SUBST(AM_CPPFLAGS)\n",
			"AC_SUBST(GONGLIB_CPPFLAGS)\n"
			"AC_SUBST(GONGLIB_LIBS)\n"
			"AC_CONFIG_FILES( Makefile src/Makefile\n",
							  "\t\tsrc/Makefile )\nAC_OUTPUT" );
	} else {
		cpm->insert_extrusion( ext_prefix + "_MODULES", module_substs,
			"AC_CONFIG_FILES( Makefile gonglib/Makefile " + ac_outputs + ")\n",
							  "\t\tsrc/Makefile )\nAC_OUTPUT" );
	}

}

void capel::genMakefileAm(const Xtring &filename, const XtringList &modules, const Xtring &args)
{
	XtringList props;
	args.upper().tokenize( props, " ");
	Xtring modulename = props[0];

	bool project = props.contains( "PROJECT" );

	Xtring ext_prefix = "GONGLIB";
	Xtring first_content =
"ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}\n"
"\n";
	CapelModule *makefile_am = new CapelModule( filename, "<<<<<", ">>>>>", "# ", "\n" );
	makefile_am->insert_extrusion( ext_prefix + "_M4", first_content, "SUBDIRS=#put here your subdirs");
	makefile_am->writeIfModified();
	delete makefile_am;
}


#include <gongfileutils.h>
#include "gonggenerators.h"

using namespace gong;

namespace capel {


void addModuleDefinition(CppModule *cpcpp, CppModule *cph,
		const Xtring &modulename, const XtringList &records,
		const XtringList &modules_required, const XtringList &modules_optional,
		const XtringList &nameslisttables)
{
	Xtring classname = cph->extractClassname(modulename);
	Xtring _namespace = cph->extractNamespace(modulename);
	Xtring ext_prefix = classname.upper() + "_";
	Xtring fnameprefix = FileUtils::basename(  cph->getFilename() );
	fnameprefix = fnameprefix.left( fnameprefix.length() - 6 );
	Xtring namespaces = "\nnamespace gong {\n" + ( _namespace == "" ? "" : "namespace " + _namespace + " {\n") + "\n";

	XtringList allmodules = modules_required;
	allmodules << modules_optional;
	Xtring mod_public, mod_private, load_module;
	Xtring hmodules_required, hmodules_required_includes, public_info_mod_required;
	for( XtringList::const_iterator it = allmodules.begin(); it != allmodules.end(); ++it ) {
		Xtring require = *it;
		hmodules_required_includes += "#ifdef HAVE_" + require.upper() + "MODULE\n"
						"#include <" + require.lower() + "module.h>\n";
		if( modules_required.contains( require ) ) {
			mod_public += "\t" + require.lower() + "::" + require + "Module *get" + require + "Module() const { return p" + require + "Module; }\n";
			mod_private += "\t" + require.lower() + "::" + require + "Module *p" + require + "Module;\n";
			hmodules_required_includes += "#else\n"
							"#error El módulo '" + modulename + "' requiere el módulo '" + require + "'\n";
			if( !public_info_mod_required.isEmpty() )
				public_info_mod_required += " << ";
			public_info_mod_required += "\"" + require.lower() + "\"";
			load_module += "\tp" + require + "Module = static_cast< " + require.lower() + "::" + require + "Module * >(DBAPP->findModule( \"" + require + "\" ));\n";
		} else { // opcional
			mod_public += "#ifdef HAVE_" + require.upper() + "MODULE\n";
			mod_public += "\t" + require.lower() + "::" + require + "Module *get" + require + "Module() const { return p" + require + "Module; }\n";
			mod_public += "#endif\n";
			mod_private += "#ifdef HAVE_" + require.upper() + "MODULE\n";
			mod_private += "\t" + require.lower() + "::" + require + "Module *p" + require + "Module;\n";
			mod_private += "#endif\n";
			load_module += "#ifdef HAVE_" + require.upper() + "MODULE\n";
			load_module += "\tp" + require + "Module = static_cast< " + require.lower() + "::" + require + "Module * >(DBAPP->findModule( \"" + require + "\" ));\n";
			load_module += "#endif\n";
		}
		hmodules_required_includes += "#endif\n";
	}
	if( allmodules.size() > 0 ) {
		hmodules_required = "public:\n" + mod_public + "private:\n" + mod_private;
	}


	// .H
	Xtring h_includes =
		"#include \"config.h\"\n"
		"#include <qobject.h>\n"
		"#include <dbappdbmodule.h>\n"
		"#include \"" + fnameprefix.lower() + "mastertable.h\"\n";
	h_includes += "\n"
		"class QAction;\n"
		"class QMenu;\n";
	if( nameslisttables.size() > 0 )
		h_includes += "\nclass NamesListTable;\n";
	if( !hmodules_required_includes.isEmpty() )
		h_includes += hmodules_required_includes;
	cph->insert_extrusion( ext_prefix + "INCLUDES", h_includes, namespaces );

	cph->insert_extrusion( ext_prefix + "CLASS_DEFINITION",
		"class " + classname + ": public QObject, public dbModule\n"
		"{\n"
		"\tQ_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto\n"
		"public:\n"
    	"\t" + classname + "();\n"
    	"\tvirtual ~" + classname + "();\n"
    	"\tvirtual bool initMainWindow(class MainWindow *win);\n"
    	"\tvirtual bool initDatabase(dbDefinition *dbdef);\n"
    	"\tvirtual bool login(QDialog *frmlogin, const Xtring &version, Xtring &addTitle, bool startingapp = true);\n"
    	"\tvirtual dbRecord *createRecord(const Xtring &tablename, dbRecordID recid=0, dbUser *user=0);\n"
    	"\tvirtual FrmEditRec *createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm = 0,\n"
        "\t\tFrmEditRec::EditMode editmode = DataTable::defaulteditmode,\n"
        "\t\tdbApplication::EditFlags editflags = dbApplication::editNone,\n"
        "\t\tQWidget *parent = 0, const char* name = 0,\n"
        "\t\tWidgetFlags fl = WidgetFlags(0) );\n"
    	"\tvirtual FrmEditRecDetail *createEditDetailForm(\n"
		"\t\tFrmEditRecMaster *frmmaster, int ndetail,\n"
		"\t\tdbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm = 0,\n"
        "\t\tFrmEditRec::EditMode editmode = DataTable::defaulteditmode,\n"
        "\t\tdbApplication::EditFlags editflags = dbApplication::editNone,\n"
        "\t\tQWidget *parent = 0, const char* name = 0,\n"
        "\t\tWidgetFlags fl = WidgetFlags(0) );\n",
		"};\n"
		"\n"
		"extern " + classname + " *ModuleInstance;\n" );

	Xtring public_record_defs, private_record_defs, menu_defs, slot_defs,
		public_info_records, public_info_records_detail;
	for( XtringList::const_iterator it = records.begin(); it != records.end(); ++it ) {
		XtringList proplist;
		(*it).tokenize( proplist, " " );
		Xtring recname = proplist.size() ? proplist[0] : "";
		Xtring frmtype = proplist.size()>1 ? proplist[1] : "";
		Xtring menuname = proplist.size()>2 ? proplist[2] : "";
		Xtring inherit = proplist.size()>3 ? proplist[3] : recname;
		if( menuname.startsWith("~") )
			menuname = menuname.mid(1);
		public_record_defs += "\t" + fnameprefix.lower() + "::MasterTable *getFic" + recname + "() const { return pFic" + recname + "; }\n";
		private_record_defs += "\t" + fnameprefix.lower() + "::MasterTable *pFic" + recname + ";\n";
		if( menuname != "-" && !menuname.isEmpty() ) {
			menuname += recname;
			menu_defs += "\tQAction *pMenu" + menuname + ";\n";
			slot_defs += "\tvoid slotMenu"  + menuname + "();\n";
		}
		Xtring path = FileUtils::path(cph->getFilename());
		if( !FileUtils::exists( (path + fnameprefix + "rec" + recname.lower() + ".cpp").c_str() ) ) {
			CppModule cpprec( path + fnameprefix + "rec" + recname.lower() + ".cpp" );
			cpprec.insert_extrusion("MODULE_INFO",
				"// COPYLEFT Registro de \n"
				"// TYPE dbRecord " + _namespace + "::" + recname + "\n");
			cpprec.write();
		}
		if( !FileUtils::exists( (path + fnameprefix + "frmedit" + recname.lower() + ".cpp").c_str() ) ) {
			CppModule cppfrm( path + fnameprefix + "frmedit" + recname.lower() + ".cpp" );
			cppfrm.insert_extrusion("MODULE_INFO",
				"// COPYLEFT Fichero de edición de \n"
				"// FIELD \n"
				"// TYPE FrmEditRecMaster " + _namespace + "::" + recname + "\n");
			cppfrm.write();
		}
	}


	// .CPP
	Xtring includes, create_records, create_editform, create_editform_detail, init_menus;
	for( XtringList::const_iterator it = records.begin(); it != records.end(); ++it ) {
		XtringList proplist;
		(*it).tokenize( proplist, " " );
		Xtring recname = proplist.size() ? proplist[0] : "";
		Xtring frmtype = proplist.size()>1 ? proplist[1] : "";
		bool addtobar = true;
		Xtring mainmenuname = proplist.size()>2 ? proplist[2] : "";
		if(mainmenuname.startsWith("~") ) {
			mainmenuname = mainmenuname.mid(1);
			addtobar = false;
		}
		Xtring inherit = proplist.size()>3 ? proplist[3] : recname;
		includes += "#include \"" + fnameprefix + "frmedit" + recname.lower() + ".h\"\n";
		create_records += "\tif( tablename.upper() == \"" + inherit.upper() + "\" )\n"
				  "\t\treturn new Rec" + recname + "(getConnection(), recid, user);\n";
		if( frmtype == "FrmEditRecMaster" ) {
			if( !public_info_records.isEmpty() )
				public_info_records += " << ";
			public_info_records += "\"" + recname.upper() + "\"";
			create_editform +=
				"\tif( tablename.upper() == \"" + inherit.upper() + "\" )\n"
				"\t\treturn new FrmEdit" + recname + "(parentfrm, rec, dm, editmode, editflags, parent, name, fl);\n";
		} else if( frmtype == "FrmEditRecDetail" ) {
			if( !public_info_records_detail.isEmpty() )
				public_info_records_detail += " << ";
			public_info_records_detail += "\"" + recname.upper() + "\"";
			create_editform_detail +=
				"\tif( tablename.upper() == \"" + recname.upper() + "\" )\n"
				"\t\treturn new FrmEdit" + recname + "(frmmaster, ndetail,\n"
				"\t\t\trec, dettablename, dm, editmode, editflags, parent, name, fl);\n";
		}
		if( !mainmenuname.isEmpty() && mainmenuname != "-" ) {
			Xtring menuname =  mainmenuname + recname;
			cpcpp->insert_extrusion( ext_prefix + "SLOT_" + menuname.upper(),
				"void " + classname + "::slotMenu" + menuname + "()\n"
				"{\n"
				"\tpMainWindow->slotMenuEditRecMaestro( \"" + inherit.upper() + "\" );\n"
				"}\n");
			init_menus +=
				"\t{\n"
				"\t\tXtring caption = DBAPP->getDatabase()->findTableDefinition(\"" + inherit.upper() + "\")->getDescPlural();\n"
				"\t\tpMenu" + menuname + " = new QAction( toGUI( caption ) + \"...\", pMainWindow );\n"
				"\t\tpMenu" + menuname + "->setObjectName( \"Menu" + menuname + "\" );\n"
				"\t\tpMenu" + menuname + "->setStatusTip( toGUI( Xtring::printf( _(\"Fichero de %s\"), caption.c_str() ) ) );\n"
				"\t\tpMenu" + menuname + "->setWhatsThis( toGUI( Xtring::printf( _(\"Abre el fichero de \"), caption.c_str() ) ) );\n"
				"\t\tpMainWindow->connect(pMenu" + menuname + ", SIGNAL(activated()), this, SLOT(slotMenu" + menuname + "()));\n";
				if( addtobar )
					init_menus += "\t\tpMenu" + menuname + "->addTo(pMenu" + mainmenuname + ");\n";
				init_menus += "\t}\n";
		}
	}

	Xtring unixname = classname.lower();
	unixname.replace("module","");
	Xtring cpp_includes =
		"#include <QMenu>\n"
		"#include <QMenuBar>\n"
	    "#include <gonggettext.h>\n"
		"#include <dbappmainwindow.h>\n"
        "#include <dbappdbapplication.h>\n";
	if( nameslisttables.size() > 0 ) {
		cpp_includes += "#include <dbappnameslisttable.h>\n"
			"#include <dbappfrmeditnameslisttable.h>\n";
	}
	cpp_includes += "#include \"" + classname.lower() + ".h\"\n" + includes;

	cpcpp->insert_extrusion_at( cpcpp->position_after_extrusion( "MODULE_INFO"),
		 ext_prefix + "INCLUDES", cpp_includes,
// Constructores y destructor añadidos tras los includes
		namespaces
	    + classname + " *ModuleInstance = 0;\n"
"\n");

	Xtring h_init_databases, cpp_init_databases;
	for( XtringList::const_iterator it = nameslisttables.begin(); it != nameslisttables.end(); ++it ) {
		Xtring nltable = *it;
		public_info_records += " << \"" + nltable.upper() + "\"";
		public_record_defs += "\tNamesListTable *pFic" + nltable + ";\n";
		cpp_init_databases += "\tpFic" + nltable + " = new NamesListTable( *pMainDatabase, \"" + nltable.upper() + "\" );\n"
			"\tpMainDatabase->addTable( pFic" + nltable + "->getTableDefinition() );\n";
		create_records += "\tif( tablename.upper() == \"" + nltable.upper() + "\" )\n"
			"\t\treturn new RecNamesListTable(\"" + nltable.upper() + "\", getConnection(), recid, user);\n";
		create_editform += "\tif( tablename.upper() == \"" + nltable.upper() + "\" )\n"
			"\t\treturn new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);\n";
	}

	Xtring public_info;
	if( public_info_mod_required.isEmpty() )
		public_info += "//\tmModuleRequires\n";
	else
		public_info += "\tmModuleRequires << " + public_info_mod_required + ";\n";
	if( public_info_records.isEmpty() )
		public_info += "//\tmMasterTables\n";
	else
		public_info += "\tmMasterTables << " + public_info_records + ";\n";
	if( public_info_records_detail.isEmpty() )
		public_info += "//\tmDetailTables\n";
	else
		public_info += "\tmDetailTables << " + public_info_records_detail + ";\n";
	public_info += load_module;
	cpcpp->insert_extrusion_at( cpcpp->position_after_extrusion( ext_prefix + "INCLUDES"),
		 ext_prefix + "PUBLIC_INFO",
		public_info,
"}\n"
"\n"
"\n"
"" + classname + "::~" + classname + "()\n"
"{\n"
"	_GONG_DEBUG_TRACE(1);\n"
"}\n"
"\n\n"
"bool " + classname + "::initDatabase(dbDefinition *db)\n"
"{\n"
"\t_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application\n"
"\t_GONG_DEBUG_ASSERT( db );\n"
"\tpMainDatabase = db;\n"
"\n"
"\treturn true;\n"
"}\n",
classname + "::" + classname + "()\n"
"	: dbModule(\"" + unixname + "\")\n"
"{\n"
"	ModuleInstance = this;\n"
"    _GONG_DEBUG_TRACE(1);\n"
 );


	if( !public_record_defs.isEmpty() )
		cph->insert_extrusion( ext_prefix + "RECORD_DEFINITIONS",
	                      "public:\n" + public_record_defs
							+ "\nprivate:\n" + private_record_defs );
	if( !slot_defs.isEmpty() )
		cph->insert_extrusion( ext_prefix + "MENU_DEFINITIONS",  "private slots:\n" + slot_defs + "protected:\n" + menu_defs);
	if( !hmodules_required.isEmpty() )
		cph->insert_extrusion( ext_prefix + "REQUIRES", hmodules_required );
	cph->addHeaderProtect( modulename );

	cpcpp->insert_extrusion( ext_prefix + "INIT_DATABASE", cpp_init_databases );

	cpcpp->insert_extrusion( ext_prefix + "CREATE_RECORD", create_records,
		"\treturn 0;\n}\n",
		"dbRecord *" + classname + "::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)\n"
		"{\n"
		"\t_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application\n");

	cpcpp->insert_extrusion( ext_prefix + "CREATE_EDITFORM", create_editform,
		"\treturn 0;\n}\n\n",
		"FrmEditRec *" + classname + "::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,\n"
		"\tFrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,\n"
        "\tQWidget *parent, const char* name, WidgetFlags fl )\n"
		"{\n"
		"\t_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application\n"
		"\tXtring tablename = rec->getTableName();\n" );

	cpcpp->insert_extrusion( ext_prefix + "CREATE_EDITFORM_DETAIL", create_editform_detail,
		"\treturn 0;\n}\n\n",
		"FrmEditRecDetail *" + classname + "::createEditDetailForm(\n"
		"\tFrmEditRecMaster *frmmaster, int ndetail,\n"
		"\tdbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,\n"
		"\tFrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,\n"
        "\tQWidget *parent, const char* name, WidgetFlags fl )\n"
		"{\n"
		"\t_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application\n"
		"\tXtring tablename = rec->getTableName();\n" );

	cpcpp->insert_extrusion( ext_prefix + "INITMAINWINDOW_MENUS", init_menus,
		"\treturn true;\n"
		"}\n",
"bool " + classname + "::initMainWindow(MainWindow *mainwin)\n"
"{\n"
"\t_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application\n"
"\t_GONG_DEBUG_ASSERT(mainwin);\n"
"\tpMainWindow = mainwin;\n" );


	cpcpp->insert_extrusion( ext_prefix + "FIN",
	    "} // namespace " + _namespace + "\n" +
		"} // namespace gong\n"
		"\n");
}


}; // namespace



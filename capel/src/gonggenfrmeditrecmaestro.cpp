#include <gongfileutils.h>
#include <iostream>
#include "gonggenerators.h"
#include "gonggenfrmeditrecmaestro.h"

using namespace gong;

namespace capel {

void addFrmEditRecMaster(CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
                          const Xtring &fullrecname, const XtringList &fields,
                          const Xtring &props, const Xtring &_inherit, const XtringList &interfaces)
{
	Xtring recname = cpcpp->extractClassname( fullrecname );
	Xtring _namespace = cpcpp->extractNamespace( fullrecname );
	Xtring fullfrmname;
	Xtring frmname = "FrmEdit" + recname;
	if( _namespace.isEmpty() == false ) {
		fullfrmname = _namespace + "::" + frmname;
	} else {
		fullfrmname = frmname;
	}
	Xtring inherit, inherit_namespace;
	if( !_inherit.isEmpty() ) {
		inherit_namespace = cpcpp->extractNamespace( _inherit );
		if( !inherit_namespace.isEmpty() )
			inherit_namespace += "::";
		inherit = cpcpp->extractClassname( _inherit );
	}
	Xtring fullfrmtype = frmtype;
	if( !inherit_namespace.isEmpty() )
		fullfrmtype = inherit_namespace + "FrmEdit" + inherit;

	Xtring ext_prefix = frmname.upper() + "_";

	Xtring fnameprefix = FileUtils::basename(  cph->getFilename() );
	fnameprefix = fnameprefix.left( fnameprefix.find("frm") );
	Xtring namespaces = "\nnamespace gong {\n" + ( _namespace == "" ? "" : "namespace " + _namespace + " {\n") + "\n";

	// .CPP
	Xtring includes =
		"#include <dbappmainwindow.h>\n"
		"#include <dbappdbapplication.h>\n";
/*
	if( frmname == "FrmEditRecMaster" || frmname == "FrmEditRecDetails" )
		includes += "#include \"" + fnameprefix + frmname.lower() + ".h\"\n";
	else
*/
	includes += "#include \"" + ( _namespace.isEmpty() ? Xtring("gong") : _namespace )
		+ frmname.lower() + ".h\"\n";

	cpcpp->insert_extrusion_at(cpcpp->position_after_extrusion( "MODULE_INFO"),
		ext_prefix + "INCLUDES", includes, namespaces, "\n");

	Xtring frm_constructor;
	if( frmtype == "FrmEditRecDetail" ) {
		frm_constructor +=
frmname + "::" + frmname + "(FrmEditRecMaster *frmmaster, int ndetail,\n"
		"\tdbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,\n"
"\tEditMode editmode, dbApplication::EditFlags editflags,\n"
"\tQWidget* parent, const char* name, WidgetFlags fl )\n"
"\t\t: " + fullfrmtype + "( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )\n";
	} else {
		frm_constructor +=
frmname + "::" + frmname + "(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,\n"
"\t                               EditMode editmode, dbApplication::EditFlags editflags,\n"
"\t                               QWidget *parent, const char* name, WidgetFlags fl )\n"
"\t    : " + fullfrmtype + "( parentfrm, master, dm, editmode, editflags, parent, name, fl )\n";
	}
	frm_constructor +=
"{\n"
"\tif ( !name )\n"
"\t    setName( \"" + frmname + "\" );\n";

	cpcpp->insert_extrusion(ext_prefix + "CONSTRUCTOR", frm_constructor);


	// .H
	cph->addHeaderPreamble( fullfrmname );
	Xtring h_frm_constructor;
	Xtring h_class =
"class " + frmname + ": public " + fullfrmtype + "\n"
"{\n"
"\tQ_OBJECT\n"
"\n"
"public:\n";
	if( frmtype == "FrmEditRecDetail" ) {
		h_class +=
"\t" + frmname +"(FrmEditRecMaster *frmmaster, int ndetail,\n"
"\t\tdbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,\n"
"\t\tEditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,\n"
"\t\tQWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );\n";
	} else {
		h_class +=
"\t" + frmname +"(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,\n"
"\t\tEditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,\n"
"\t\tQWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );\n";
	}

	if( _inherit.isEmpty() || (inherit != recname) ) {
		h_class +=
"\tRec" + recname +" *getRec" + recname +"() const { return (Rec" + recname +" *)getRecord(); }\n";
	}
	cph->insert_extrusion( ext_prefix + "CLASS", h_class );

	cph->insert_extrusion( ext_prefix + "VIRTUALS_FROM_FRMEDITREC",
"protected:\n"
"\tvirtual void scatterFields(); // From FrmEditRec\n"
"\tvirtual void gatherFields();\n"
"\tvirtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);\n");

	XtringList propslist;
	props.upper().tokenize( propslist, " " );
	if( propslist.contains( "PRESUPUESTO" )
		|| propslist.contains( "PEDIDO" )
		|| propslist.contains( "ALBARAN" )
		|| propslist.contains( "FACTURA" ) )
		insert_cabeceras(propslist, cpcpp, cph, frmtype, recname, ext_prefix );
	if( propslist.contains( "PRESUPUESTODET" )
		|| propslist.contains( "PEDIDODET" )
		|| propslist.contains( "ALBARANDET" )
		|| propslist.contains( "FACTURADET" ) )
		insert_detalles(propslist, cpcpp, cph, frmtype, recname, ext_prefix );
}




void addFrmEditRecMasterFields(CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
	const Xtring &fullrecname, const XtringList &fields, const XtringList &members,
	const Xtring &props, const Xtring &_inherit)
{
	Xtring recname = cpcpp->extractClassname( fullrecname );
	Xtring _namespace = cpcpp->extractNamespace( fullrecname );
	Xtring frmname = "FrmEdit" + recname;
	Xtring ext_prefix = frmname.upper() + "_";
	Xtring fnameprefix = FileUtils::basename(  cph->getFilename() );
	fnameprefix = fnameprefix.left( fnameprefix.find("frm") );
	Xtring namespaces = "\nnamespace gong {\n" + ( _namespace == "" ? "" : "namespace " + _namespace + " {\n") + "\n";
	Xtring scatter, scatterrels, frmscatter;
	Xtring gather, hcontrols;
	Xtring special_action, init_controls, validate, layouts_start, layouts_end;
	Xtring hscatters, hslots, hincludes, hgetrecords;
	Xtring codefld, descfld; // For ValidCodeAndDesc
	XtringList layoutslist, parentslist;
	Xtring inherit, inherit_namespace;
	if( !_inherit.isEmpty() ) {
		inherit_namespace = cpcpp->extractNamespace( _inherit );
		if( !inherit_namespace.isEmpty() )
			inherit_namespace += "::";
		inherit = cpcpp->extractClassname( _inherit );
	} else {
		inherit = recname;
	}
	Xtring fullfrmtype = frmtype;
	if( !inherit_namespace.isEmpty() )
		fullfrmtype = inherit_namespace + "FrmEdit" + inherit;

	bool firstfield = true, hasnombre = false, hasintcode = false, hastabs = false;
	for( XtringList::const_iterator it = fields.begin();
		it != fields.end();
		++ it ) {
		XtringList proplist;
		(*it).tokenize( proplist, " " );
		Xtring fldname = proplist.size() ? proplist[0] : "";
		Xtring type = proplist.size()>1 ? proplist[1] : "";
		Xtring parent = proplist.size()>2 ? proplist[2] : "pControlsFrame";
		Xtring layout = proplist.size()>3 ? proplist[3] : "";
		Xtring moreprops = proplist.size()>4 ? proplist[4] : "";
		if( (fldname.upper() == "CODIGO" || fldname.upper() == "CODE")
			&& type.upper() == "INT" ) {
			hasintcode = true;
			codefld = fldname;
		}
		if( fldname.upper() == "NOMBRE" ) {
			hasnombre = true;
			descfld = fldname;
		}
		Xtring csh, cscpp, ceh, cecpp; // conditional_start, conditional_end;
		getConditionalCode( moreprops, csh, cscpp, ceh, cecpp );
		if( parent == "-" )
			parent = "pControlsFrame";
		if( layout == "-" )
			layout = "pControlsLayout";
		if( layout.isEmpty() )
			layout = fldname.lower() + "Layout";
		if( !layout.endsWith( "Layout") )
			layout = layout + "Layout";
		if( parent != "pControlsFrame" && !parentslist.contains( parent ) ) {
			parentslist << parent;
			if( parent.startsWith( "tab" ) ) {
				if( !hastabs )
					layouts_start += "\tshowTabs(true);\n";
				hastabs = true;
				layouts_start += "\tQWidget *" + parent + " = new QWidget( pTabWidget, \"" + parent + "\" );\n";
			}
			layouts_start += "\tQVBoxLayout *" + parent + "Layout = new QVBoxLayout(" + parent + ", 11, 6, \"" + parent + "Layout\");\n";
		}
		// Los elementos de un tab sin layout adoptan el layout del tab
		bool noadd_layout = false;
		if( layout.startsWith("noadd") ) {
			noadd_layout = true;
			layout = layout.mid(5);
		}
		if( parent.startsWith( "tab" ) && layout == "pControlsLayout" )
			layout = parent + "Layout";
		else if( layout != "pControlsLayout" && !layoutslist.contains( layout ) ) {
			layoutslist << layout;
			layouts_start += "\tQHBoxLayout *" + layout + " = new QHBoxLayout(0, 0, 6, \"" + layout + "\");\n";
			if( !noadd_layout ) {
				if( parent == "pControlsFrame" )
					layouts_end += "\tpControlsLayout->addLayout( " + layout + " );\n";
				else if( parent + "Layout" != layout )
					layouts_end += "\t" + parent + "Layout->addLayout( " + layout + " );\n";
			}
			if( layout.startsWith("left") )
				layouts_end += "\talignLayout( " + layout + ", true );\n";
			if( layout.startsWith("right") )
				layouts_end += "\talignLayout( " + layout + ", false);\n";
		}

		// Code and desc for validation
		if( codefld.isEmpty() )
			codefld = fldname;
		else if( descfld.isEmpty() )
			descfld = fldname;
		bool isgenerated = false;
		Xtring ctrltype, ctrlname, fldnamereplaced = fldname;
		fldnamereplaced.replace(".", "_");
		if( type.upper() == "DATETIME" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toDateTime());\n" + cecpp;
			gather += cscpp + "\tgetRec" + recname + "()->setValue( \"" +
				fldname.upper() + "\", " + ctrlname + "->toDateTime());\n" + cecpp;
		} else if( type.upper() == "DATE" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toDate());\n" + cecpp;
			gather += cscpp + "\tgetRec" + recname + "()->setValue( \""
					+ fldname.upper() + "\", " + ctrlname + "->toDate());\n" + cecpp;
		} else if( type.upper() == "TIME" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toTime());\n" + cecpp;
			gather += cscpp +"\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toTime());\n" + cecpp;
		} else if( type.upper() == "STRING" || type.upper() == "TEXT" || type.upper() == "RICHTEXT") {
			ctrlname = "edit" + fldnamereplaced;
			if( type.upper() == "STRING" )
				ctrltype = "gong::EditBox";
			else if( type.upper() == "RICHTEXT" )
				ctrltype = "gong::RichTextBox";
			else
				ctrltype = "gong::TextBox";
			scatter += cscpp + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toString());\n" + cecpp;
			gather += cscpp + "\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toString());\n" + cecpp;
		} else if( type.upper() == "STRING" || type.upper() == "TEXT" || type.upper() == "RICHTEXT") {
			ctrlname = "edit" + fldnamereplaced;
			if( type.upper() == "STRING" )
				ctrltype = "gong::EditBox";
			else if( type.upper() == "RICHTEXT" )
				ctrltype = "gong::RichTextBox";
			else
				ctrltype = "gong::TextBox";
			scatter += cscpp + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toString());\n" + cscpp;
			gather += cscpp + "\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toString());\n" + cecpp;
		} else if( type.upper() == "INT" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toInt());\n" + cecpp;
			gather += cscpp + "\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toInt());\n" + cecpp;
		} else if( type.upper() == "MONEY" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toMoney());\n" + cecpp;
			gather += cscpp + "\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toMoney());\n" + cecpp;
		} else if( type.upper() == "DOUBLE" || type.upper() == "FLOAT" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toDouble());\n" + cecpp;
			gather += cscpp + "\tgetRec" + recname + "()->setValue( \""
				+  fldname.upper() + "\", " + ctrlname + "->toDouble());\n" + cecpp;
		} else if( type.upper() == "BOOL" ) {
			ctrltype = "gong::CheckBox";
			ctrlname = "check" + fldnamereplaced;
			scatter += cscpp  + "\t" + ctrlname + "->setChecked(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toBool());\n" + cecpp;
			gather += cscpp  + "\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->isChecked());\n" + cecpp;
		} else if( type.upper() == "COMBOSTRING" ) {
			ctrltype = "gong::ComboBoxXtring";
			ctrlname = "combo" + fldnamereplaced;
			scatter += cscpp  + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toString());\n" + cecpp;
			gather += cscpp  + "\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toString());\n" + cecpp;
		} else if( type.upper() == "COMBOINT" ) {
			ctrltype = "gong::ComboBoxInt";
			ctrlname = "combo" + fldnamereplaced;
			scatter += cscpp  + "\t" + ctrlname + "->setCurrentItemByValue(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toInt());\n" + cecpp;
			gather += cscpp  + "\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->getCurrentItemValue());\n" + cecpp;
		} else if( type.upper() == "IMAGE" ) {
			ctrltype = "gong::ImageBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp  + "\t" + ctrlname + "->setImageData(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\"));\n" + cecpp;
			gather += cscpp  + "\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toData());\n" + cecpp;
		} else if( type.upper().startsWith("REFERENCE") || type.upper().startsWith("MULTIPLEREFERENCE") ) {
			bool multiplereference = type.upper().startsWith("MULTIPLEREFERENCE");
			// FIELD Proyecto_ID Reference(Proyecto,Codigo,Descripcion) - nasientolayout
			isgenerated = true;
			Xtring referredfield = fldname.left(fldname.find_last_of("_"));
			Xtring referredfieldcontrolname = referredfield;
			referredfieldcontrolname.replace(".","_");
			if( referredfield.find( "." ) != Xtring::npos )
				referredfield = referredfield.mid( referredfield.find( "." ) + 1 );
			XtringList codedesc;
			type.tokenize( codedesc, "(,)" );
			Xtring referredtablecomplete = codedesc[1];
			Xtring referredtablenamespace = CppModule::extractNamespace( referredtablecomplete );
			if( !referredtablenamespace.isEmpty() )
				referredtablenamespace += "::";
			Xtring referredtable = CppModule::extractClassname( referredtablecomplete );

			Xtring fldcode = codedesc[2];
			Xtring flddesc = codedesc[3];
			Xtring applicationflags;
			if( codedesc.size() > 4 )
				applicationflags = codedesc[4];
			scatterrels += cscpp + "\tscatter" + referredfieldcontrolname + "();\n" + cecpp;
			if( firstfield && _inherit.isEmpty() ) {
				scatter += "\tif( isEditing() && (pFocusWidget == 0) )\n";
				scatter += "\t\tpFocusWidget = edit" + referredfieldcontrolname + fldcode + ";\n";
				firstfield = false;
			}
			gather += cscpp  + "\tgetRec" + recname + "()->setValue( \""
				+  fldname.upper() + "\", getRec" + referredfield + "()->getRecordID() );\n" + cecpp;
			hcontrols += csh +
				"\tgong::SearchBox *search" + referredfieldcontrolname + fldcode + ";\n"
				"\tQPushButton *push" + referredfieldcontrolname + fldcode + ";\n"
				"\tgong::LineEdit *edit" + referredfieldcontrolname + fldcode + ";\n"
				"\tgong::LineEdit *edit" + referredfieldcontrolname + flddesc + ";\n" + ceh;

			init_controls += "\n" + cscpp +
"\tsearch" + referredfieldcontrolname + fldcode + " = addSearchField( " + parent + ", \""
+ fldname.upper() + "\", \""  + referredtable.upper() + "\", \"" + fldcode.upper() + "\", \"" + flddesc.upper() + "\", " + layout + " );\n"
"\tpush" + referredfieldcontrolname + fldcode + " = search" + referredfieldcontrolname + fldcode + "->getButton();\n"
"\tconnect( push" + referredfieldcontrolname + fldcode + ", SIGNAL( clicked() ), this, SLOT( push" + referredfield + fldcode + "_clicked() ) );\n"
"\tedit" + referredfieldcontrolname + fldcode + " = search" + referredfieldcontrolname + fldcode + "->getEditCode();\n"
"\tedit" + referredfieldcontrolname + flddesc + " = search" + referredfieldcontrolname + fldcode + "->getEditDesc();\n" + cecpp;

			Xtring relatedscatter;
			Xtring specialaction;
			Xtring ctrlcode =  "edit" + referredfieldcontrolname + fldcode;
			Xtring ctrldesc =  "edit" + referredfieldcontrolname + flddesc;

			if( referredfield + fldcode != "CuentaPagoCuenta" ) { // Siempre tengo que andar quitándolo
				validate += cscpp +
"\tif( focusWidget() != push" + referredfieldcontrolname + fldcode + ") // To avoid triggering the validating if the button is pressed\n"
"\tif( validSeekCode(" + (multiplereference ? Xtring("m" + referredtable + "List,") : Xtring("") ) + " sender, isvalid, *validresult, " + ctrlcode + ", " + ctrldesc + ",\n"
"\t\tgetRec" + referredfield + "(), \"" + fldcode.upper() + "\", \"" + flddesc.upper() + "\", Xtring::null";
				if( multiplereference ) {
					if( applicationflags.isEmpty() == false )
						applicationflags += " + ";
					applicationflags += "dbApplication::SeekCodeMultiple";
				}
				if( !applicationflags.isEmpty() )
					validate += ", dbApplication::SeekCodeFlags( " + applicationflags + " )";
				validate +=") )\n"
"\t\tscatter" + referredfieldcontrolname + "();\n" + cecpp;
			}
			// scatterPrincesa
			relatedscatter += "\t" + ctrlcode +
				"->setText( getRec" + referredfield + "()->getValue(\"" + fldcode.upper() + "\") );\n";
			relatedscatter += "\t" + ctrldesc +
				"->setText( getRec" + referredfield + "()->getValue(\"" + flddesc.upper() + "\") );\n";

			cpcpp->insert_extrusion(ext_prefix + "SCATTER_" + referredfieldcontrolname.upper(), relatedscatter, "}\n",
				"\nvoid " + frmname + "::scatter" + referredfieldcontrolname + "()\n{\n");

			Xtring pushrefclicked =
"\tchar action = mControlKeyPressed;\n"
"\tif( !isEditing() || " + Xtring(ctrlcode).replace("edit","search") + "->mustBeReadOnly() )\n"
"\t\taction = 'E';\n"
"\tswitch( action ) {\n"
"\t\tcase 'F':\n"
"\t\tcase '\\0':\n"
"\t\t\t" + ctrlcode + "->setJustEdited( false );\n"
"\t\t\t" + ctrlcode + "->setCancelling();\n"
"\t\t\tif( DBAPP->choose" + (multiplereference ? Xtring("Multi( m" + referredtable + "List, " ) : Xtring("(") ) + "this, getRec" + referredfield + "(), 0, dbApplication::editNone, this ) ) {\n"
"\t\t\t\tsetEdited(true);\n"
"\t\t\t\tscatter" + referredfieldcontrolname + "();\n";
		if( !multiplereference )
			pushrefclicked += "\t\t\t\t" + ctrlcode + "->setJustEdited( true );\n";
		pushrefclicked +=
"\t\t\t\tsetWiseFocus(" + ctrlcode + ");\n"
"\t\t\t}\n"
"\t\t\tbreak;\n"
"\t\tcase 'M':\n"
"\t\t\t{\n"
"\t\t\t\tif( getRec" + referredfield + "()->getRecordID() ) {\n"
"\t\t\t\t\t" + ctrlcode + "->setJustEdited( false );\n"
"\t\t\t\t\tif( DBAPP->editRecord(this,\n"
"\t\t\t\t\t\t\tgetRec" + referredfield + "(), 0, DataTable::updating,\n"
"\t\t\t\t\t\t\tdbApplication::simpleEdition, this ) ) {\n"
"\t\t\t\t\t\t" + ctrlcode + "->setJustEdited( true );\n"
"\t\t\t\t\t\tscatter" + referredfieldcontrolname + "();\n"
"\t\t\t\t\t}\n"
"\t\t\t\tsetWiseFocus(" + ctrlcode + ");\n"
"\t\t\t\t}\n"
"\t\t\t}\n"
"\t\t\tbreak;\n"
"\t\tcase 'E':\n"
"\t\t\t{\n"
"\t\t\t\tif( getRec" + referredfield + "()->getRecordID() != 0 ) {\n"
"\t\t\t\t\t" + ctrlcode + "->setJustEdited( false );\n"
"\t\t\t\t\tDBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRec" + referredfield + "(),\n"
"\t\t\t\t\t\t0, DataTable::selecting, dbApplication::simpleEdition, this ) );\n"
"\t\t\t\t}\n"
"\t\t\t}\n"
"\t\t\tbreak;\n"
"\t\tcase 'A':\n"
"\t\t\t{\n"
"\t\t\t\t" + referredtablenamespace + "Rec" + referredfield + " *tmprec = static_cast<" + referredtablenamespace + "Rec" + referredfield + " *>(DBAPP->createRecord( \"" + referredfield + "\" ));\n"
"\t\t\t\t" + ctrlcode + "->setJustEdited( false );\n"
"\t\t\t\ttmprec->clear( true ); // set default values\n"
"\t\t\t\tDBAPP->setCodeNotFound( " + ctrlcode + "->toString() );\n"
"\t\t\t\tif( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,\n"
"\t\t\t\t\tdbApplication::simpleEdition, this ) ) {\n"
"\t\t\t\t\t" + ctrlcode + "->setJustEdited( true );\n"
"\t\t\t\t\tgetRec" + referredfield + "()->copyRecord( tmprec );\n"
"\t\t\t\t\tscatter" + referredfieldcontrolname + "();\n"
"\t\t\t\t}\n"
"\t\t\t\tsetWiseFocus(" + ctrlcode + ");\n"
"\t\t\t\tDBAPP->setCodeNotFound( Xtring() );\n"
"\t\t\t}\n"
"\t\t\tbreak;\n"
"\t}\n";

			cpcpp->insert_extrusion(ext_prefix + "PUSH_" + referredfield.upper() + "_" + fldcode.upper() +  "_CLICKED",
									pushrefclicked, "}\n",
"void " + frmname + "::push" + referredfieldcontrolname + fldcode + "_clicked()\n"
"{\n");

			special_action += cscpp +
				"\tif( sender == " + ctrlcode + " )\n"
				"\t\tpush" + referredfieldcontrolname + fldcode + "_clicked();\n" + cecpp;

//			hincludes += "#include \"" + fnameprefix + "rec" + referredfield.lower() + ".h\"\n";
			hscatters += "\tvoid scatter" + referredfieldcontrolname + "();\n";
// Los slots no tienen condicional porque el moc no los incluye
			hslots += "\tvoid push" + referredfieldcontrolname + fldcode + "_clicked();\n";
			hgetrecords += csh + "\t" + referredtablenamespace + "Rec" + referredfield +"* getRec" + referredfield +"() const\n"
				"\t\t{ return static_cast<Rec" + recname
				+ "*>(getRecord())->getRec" + referredfield +"(); }\n" + ceh;
		} else if( type.upper() == "FRMEDITRECMASTER" || type.upper() == "FRMEDITRECDETAIL") {
			isgenerated = true;
			hincludes += "#include \"" + fnameprefix + "frmedit" + fldname.lower() + ".h\"\n";
			hcontrols += "\tFrmEdit" + fldname + " *pFrm" + fldname + ";\n";
			if( type.upper() == "FRMEDITRECMASTER" ) {
				init_controls +=
					"\n" + cscpp +
					"\tRec" + fldname + " *" + fldname.lower() + " = static_cast<Rec"
						+ fldname + "*>(DBAPP->createRecord(\"" + fldname.upper() + "\"));\n"
					"\tdbViewDefinitionDict " + fldname.lower() + "views;\n"
					"\tDBAPP->getDatabase()->getViewsForTable( " + fldname.lower() + ", " + fldname.lower() + "views );\n"
					"\tdbRecordDataModel *" + fldname.lower() + "dm = new dbRecordDataModel("
						+ fldname.lower() + ", " + fldname.lower() + "views);\n"
					"\tpFrm" + fldname + " = static_cast<FrmEdit" + fldname + " *>(\n"
					"\t\tDBAPP->createEditForm(this, " + fldname.lower() + ", " + fldname.lower() + "dm, DataTable::browsing,\n"
					+"\t\tstatic_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),\n"
					"\t\t" + parent + ") );\n"
					"\t" + layout + "->addWidget(pFrm" + fldname + ");\n";
					"\n" + cecpp;
			} else {
				init_controls += cscpp +
					"\n// frmDetails: " + fldname + "\n"
					"\tQFrame *" + fldname.lower() + "Frame = new QFrame(this);\n"
					"\t" + fldname.lower() + "Frame->setFrameStyle(QFrame::Sunken | QFrame::Panel);\n"
					"\t" + fldname.lower() + "Frame->setLineWidth(4);\n"
    				"\tQVBoxLayout *" + fldname.lower() + "FrameLayout = new QVBoxLayout(" + fldname.lower() + "Frame);\n"
					"\tRec" + fldname + " *" + fldname.lower() + " = getRec" + recname + "()->getRec" + fldname + "(-1);\n"
					"\tdbViewDefinitionDict " + fldname.lower() + "views;\n"
					"\tDBAPP->getDatabase()->getViewsForTable( " + fldname.lower() + ", " + fldname.lower() + "views );\n"
					"\tdbRecordListDataModel *" + fldname.lower() + "dm =\n"
						"\t\tnew dbRecordListDataModel(getRec" + recname + "()->getList" + fldname + "(), "
					    + fldname.lower() + ", " + fldname.lower() + "views, \"1=0\");\n"
					"\tpFrm" + fldname + " = static_cast<FrmEdit" + fldname + " *>(\n"
					"\t\tDBAPP->createEditDetailForm(this,\n"
					"\t\t-1, " + fldname.lower() + ", \"" + fldname.upper() + "\"," + fldname.lower() + "dm, DataTable::browsing,\n"
					+"\t\tstatic_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),\n"
					"\t\t" + parent + ") );\n"
					"\tpFrm" + fldname + "->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);\n"
					"\t" + fldname.lower() + "FrameLayout->addWidget( pFrm" + fldname + " );\n"
					"\t" + layout + "->addWidget(" + fldname.lower() + "Frame);\n" + cecpp;
			}
			frmscatter += 	"\tif( !isInserting() )\n"
				"\t\tpFrm" + fldname + "->setFilter( // Pon el filtro aquí\n";
			// No gather
		} else if( type.upper() == "HSPACER" ) {
				init_controls +=
					"\tQSpacerItem* " + fldname + " = new QSpacerItem ( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);\n"
					"\t" + layout + "->addItem ( " + fldname + ");\n";
					isgenerated = true;
		} else if( type.upper() == "VSPACER" ) {
				init_controls +=
					"\tQSpacerItem* " + fldname + " = new QSpacerItem ( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);\n"
					"\t" + layout + "->addItem ( " + fldname + ");\n";
					isgenerated = true;
		} else  {
			ctrltype = type;
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp  + "\t" + ctrlname + "->setText(getRec" + recname + "()->getValue(\"" + fldname.upper() + "\").toString());\n" + cecpp;
			gather += cscpp  + "\tgetRec" + recname + "()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toString());\n" + cecpp;
		}
		if( !isgenerated ) {
			if( firstfield && _inherit.isEmpty() ) {
				scatter += "\tif( isEditing() && (pFocusWidget == 0) )\n";
				scatter += "\t\tpFocusWidget = " + ctrlname + ";\n";
				firstfield = false;
			}
			hcontrols += csh + "\t" + ctrltype + " *" + ctrlname + ";\n" + ceh;
			Xtring tableoffield = inherit;
			Xtring fieldoffield = fldname;
			if( fldname.find(".") != Xtring::npos ) {
				fldname.splitIn2( tableoffield, fieldoffield, "." );
			}
			if( ctrltype == "gong::ComboBoxInt" ) {
				init_controls += cscpp +
	"\t" + ctrlname + " = addComboIntField( " /* values" + fldname + ", " */ + parent + ", \""
		+ tableoffield.upper() + "\", \"" + fieldoffield.upper() + "\", " + layout + " );\n" + cecpp;
			} else if( ctrltype == "gong::ComboBoxXtring" ) {
				init_controls += cscpp +
	"\t" + ctrlname + " = addComboXtringField( " /* values" + fldname + ", "  */ + parent + ", \""
		+ tableoffield.upper() + "\", \"" + fieldoffield.upper() + "\", " + layout + " );\n" + cecpp;
			} else {
				init_controls += cscpp +
	"\t" + ctrlname + " = add" + ctrltype.replace("Box", "Field").replace("gong::","") + "( " + parent
	+ ", \"" + tableoffield.upper() + "\", \"" + fieldoffield.upper() + "\", " + layout;
				if( layout.startsWith( "horiz" ) )
					init_controls += ", true ";
				init_controls += " );\n" + cecpp;
			}
			if( moreprops == "editable" )
				init_controls += "\t" + ctrlname + "->setEditable( true );\n";
		}
	}

	// MEMBERS
	Xtring hmembers;
	for( XtringList::const_iterator it = members.begin();
		it != members.end();
		++ it ) {
		if( *it == "actTotales" ) {
			hmembers += "\tvoid childUpdated(FrmEditRec *child);\n"
						"\tvoid actTotales();\n";
			cpcpp->insert_extrusion(ext_prefix + "ACTTOTALES",
"void " + frmname + "::actTotales()\n"
"{\n",
"\tdbRecordList *reclst = p" + frmname + "Detalle->getDetalles();\n"
"\tuint i;\n"
"\tfor ( i = 0; i < reclst->size(); i++ ) {\n"
"\t\tRec" + recname + "Detalle *detail = static_cast<Rec" + recname + "Detalle *>( reclst->at( i ) );\n"
"\t}\n"
"}\n");
			cpcpp->insert_extrusion(ext_prefix + "CHILDUPDATED",
"void " + frmname + "::childUpdated(FrmEditRec * child)\n"
"{\n", "\tactTotales();\n}\n" );
		}
	}

	if( props.upper().find("INCCODE") != Xtring::npos ) {
		scatter += "\tif( isInserting() && edit" + codefld + "->toInt() == 0 ) {\n"
				   "\t\tedit" + codefld + "->setText( getRecord()->selectNextInt( \""
						+ codefld.upper() + "\" ) );\n"
				   "\t}\n";
	}
	bool usecodenotfound = ( props.upper().find("CODENOTFOUND") != Xtring::npos);
	if( usecodenotfound ) {
		Xtring newdescfld = descfld;
		if( descfld == "Contacto_ID" )
			newdescfld = "ContactoCIF";
		scatter += "\tif( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {\n";
		if( hasintcode )
			scatter += "\t\tif( DBAPP->codeNotFound().toInt() != 0 ) {\n"
				"\t\t\teditCodigo->setText( DBAPP->codeNotFound() );\n"
				"\t\t\teditCodigo->setJustEdited( true );\n"
				"\t\t} else {\n";
		scatter += "\t\tedit" + newdescfld + "->setText( DBAPP->codeNotFound() );\n"
				   "\t\tedit" + newdescfld + "->setJustEdited( true );\n";
		if( hasintcode )
			scatter += "\t\t}\n";
		scatter += "\t}\n";
	}
	if( !_inherit.isEmpty() )
		scatter = "\t" + inherit_namespace + "FrmEdit" + inherit + "::scatterFields();\n" + scatter;
	cpcpp->insert_extrusion(ext_prefix + "SCATTER", scatter + scatterrels, frmscatter + "}\n",
		"\nvoid " + frmname + "::scatterFields()\n{\n");
	if( !_inherit.isEmpty() )
		gather = "\t" + inherit_namespace + "FrmEdit" + inherit + "::gatherFields();\n" + gather;
	cpcpp->insert_extrusion(ext_prefix + "GATHER", gather, "}\n",
		"\nvoid " + frmname + "::gatherFields()\n{\n");
	cpcpp->insert_extrusion_at(cpcpp->position_after_extrusion( ext_prefix + "CONSTRUCTOR" ),
		ext_prefix + "INIT_CONTROLS", layouts_start + init_controls + layouts_end, "}\n");

	if( !special_action.isEmpty() ) {
		cpcpp->insert_extrusion(ext_prefix + "SPECIALACTION",
								"\tmControlKeyPressed = key;\n"
								"\t" + fullfrmtype + "::specialControlKeyPressed(sender,key); // calls the behaviors\n"
								+ special_action +
								"\tmControlKeyPressed = '\\0';\n",
						  "}\n",
			"void " + frmname + "::specialControlKeyPressed(QWidget *sender, char key)\n{\n");
		cph->append_text_to_extrusion(ext_prefix + "VIRTUALS_FROM_FRMEDITREC",
		"\tvoid specialControlKeyPressed(QWidget *sender, char key);\n" );
	}

	if( props.upper().find("VALIDCODEANDDESC") != Xtring::npos )
		validate += "\tif( !validCodeAndDesc( sender, *validresult, edit"
			+ codefld + ", edit" + descfld + ", \""
			+ codefld.lower() + "\", \"" + descfld.lower() + "\" ) )\n"
			"\t\tif( !sender )\n"
			"\t\t\t*isvalid = false;\n";
	else if( props.upper().find("VALIDCODE") != Xtring::npos )
		validate += "\tif( !validCodeAndDesc( sender, *validresult, edit"
			+ codefld + ", 0, \""
			+ codefld.lower() + "\", \"\" ) )\n"
			"\t\tif( !sender )\n"
			"\t\t\t*isvalid = false;\n";

	Xtring whole_validate =
		"\tbool v=true;\n"
		"\tif( !isvalid )\n"
		"\t\tisvalid = &v;\n";
	whole_validate +=
"\tValidResult *validresult = ( ir ? ir : new ValidResult() );\n";

	if( !_inherit.isEmpty() ) {
		whole_validate +=
"\t" + inherit_namespace + "FrmEdit" + inherit + "::validateFields(sender, isvalid, validresult);\n";
	} else {
		whole_validate +=
"\tif( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )\n"
"\t\t\t*isvalid = false;\n";
	}
	whole_validate += validate;

	cpcpp->insert_extrusion(ext_prefix + "VALIDATE", whole_validate,
	    "\tif( !ir ) {\n"
	    "\t\tshowValidMessages(isvalid, *validresult, sender);\n"
	    "\t\tdelete validresult;\n"
	    "\t}\n"
        "}\n\n",
		"\nvoid " + frmname + "::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)\n"
		"{\n");

	Xtring fin =
"} // namespace " + _namespace + "\n"
"} // namespace gong\n";
	cpcpp->insert_extrusion( ext_prefix + "FIN", fin, "\n" );

	// .H
	Xtring ext_includes =
		"#include <gongfrmbase.h>\n";
 	if( frmtype == "FrmEditRecMaster" || frmtype == "FrmEditRecDetail" )
		ext_includes += "#include <dbapp" + frmtype.lower() + ".h>\n";
	if( _inherit.isEmpty() || (inherit != recname) )
		ext_includes += "#include \"" + fnameprefix + + "rec" + recname.lower() + ".h\"\n" + hincludes;
	cph->insert_extrusion_at(cph->position_after_extrusion( ext_prefix + "PREAMBLE"),
		ext_prefix + "INCLUDES", ext_includes, namespaces );

	if( !hscatters.isEmpty() || !hgetrecords.isEmpty() || !hslots.isEmpty() )
		cph->insert_extrusion(ext_prefix + "SCATTERS_AND_SLOTS",
		"protected:\n" + hscatters + "\nprivate slots:\n" + hslots + "\npublic:\n" + hgetrecords, "\n");
	if( !hmembers.isEmpty() ) {
		cph->insert_extrusion(ext_prefix + "MEMBERS", "protected:\n" + hmembers, "\n");
	}
	cph->insert_extrusion(ext_prefix + "CONTROLS", "protected:\n" + hcontrols, "};\n");
	cph->addHeaderPostamble( _namespace + "::" + frmname );
}


void insert_cabeceras(const XtringList &props, CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
	const gong::Xtring &recname, const Xtring &ext_prefix)
{
	bool escompra = props.contains("COMPRA");
	bool esfactura = props.contains("FACTURA");
	bool esalbaran = props.contains("ALBARAN");

	cph->insert_extrusion( ext_prefix + "CABECERA_DECL",
"protected:\n"
"\tvoid updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster\n"
"\tvoid actTotales();\n"
"\tvoid numeraLineas();\n"
"\tvoid genNumeroDocumento();\n"
"public:\n"
"\tint getIvaDetallado() const { return comboIVADetallado->getCurrentItemValue(); }\n",
						   "\n");

	Xtring cabecera_scatter_cliente =
"\tif( editClienteCodigo->isJustEdited() ) {\n";
	if( esalbaran || esfactura ) {
		cabecera_scatter_cliente +=
"\t\tif( getRecCliente()->getRecFormaPago()->getValue(\"CODIGO\").toInt() != 0 )\n"
"\t\t\tsearchFormaPagoCodigo->setValue( getRecCliente()->getRecFormaPago()->getValue(\"CODIGO\") );\n";
	}
	cabecera_scatter_cliente +=
"\t\tif( getRecCliente()->getValue(\"IVADetallado\").toInt() != 0 )\n"
"\t\t\tcomboIVADetallado->setCurrentItemByValue( getRecCliente()->getValue(\"IVADetallado\").toInt() );\n";

	if( escompra ) {
		cabecera_scatter_cliente.replace( "Cliente", "Proveedora" );
		cabecera_scatter_cliente += "\t}\n";
		cpcpp->insert_extrusion( ext_prefix + "CABECERA_SCATTER_PROVEEDORA",
							 cabecera_scatter_cliente );
	} else {
		cabecera_scatter_cliente +=
"\t\tif( getRecCliente()->getRecAgente()->getRecordID() != 0 )\n"
"\t\t\tsearchAgenteCodigo->setValue( getRecCliente()->getRecAgente()->getValue(\"CODIGO\") );\n";
		cabecera_scatter_cliente += "\t}\n";

		cpcpp->insert_extrusion( ext_prefix + "CABECERA_SCATTER_CLIENTE",
							 cabecera_scatter_cliente );
	}

	Xtring cabecera_validate =
"\tif( sender == editDtoP100 && editDtoP100->isJustEdited() ) {\n"
"\t\teditDescuento->setText( 0.0 );\n"
"\t\tactTotales();\n"
"\t}\n"
"\tif( sender == comboIVADetallado ) {\n"
"\t\tif( comboIVADetallado->getCurrentItemValue() == factu::FldIVADetallado::con_recargo ) {\n"
"\t\t\teditRecargoEquivalencia->setVisible( true );\n"
"\t\t\teditRecargoEquivalencia->getLabel()->setVisible( true );\n"
"\t\t} else {\n"
"\t\t\teditRecargoEquivalencia->setVisible( false );\n"
"\t\t\teditRecargoEquivalencia->getLabel()->setVisible( false );\n"
"\t\t}\n"
"\t\tactTotales();\n"
"\t}\n"
"\tif( sender == editFecha || !sender ) {\n"
"\t\tif( editFecha->toDate().getYear() != factu::ModuleInstance->getEmpresaModule()->getEjercicio() ) {\n"
"\t\t\tvalidresult->addWarning( Xtring::printf(_(\"La fecha no es del ejercicio actual: %d\"),\n"
"\t\t\t\tfactu::ModuleInstance->getEmpresaModule()->getEjercicio()),\n"
"\t\t\t\t\"FECHA\" );\n"
"\t\t}\n"
"\t}\n"
"\tif( sender == editTotal && editTotal->isJustEdited() ) {\n"
"\t\tif( editBaseImponible->toDouble() != 0.0 ) {\n"
"\t\t\tdouble nuevo_total = editTotal->toDouble();\n"
"\t\t\teditDtoP100->setText( 0.0 );\n"
"\t\t\teditDescuento->setText( 0.0 );\n"
"\t\t\tactTotales();\n"
"\t\t\tdouble orig_total = editTotal->toDouble();\n"
"\t\t\tif( orig_total != nuevo_total ) {\n"
"\t\t\t\tdouble orig_sumaimp = editSumaImportes->toDouble();\n"
"\t\t\t\tdouble nuevo_sumaimp =  (nuevo_total * orig_sumaimp) / orig_total;\n"
"\t\t\t\tdouble dtop100 = 100 - (100 * nuevo_sumaimp / orig_sumaimp);\n"
"\t\t\t\teditDtoP100->setText( dtop100 );\n"
"\t\t\t\tactTotales();\n"
"\t\t\t}\n"
"\t\t}\n"
"\t}\n"
;

	if( props.contains( "FACTURA" ) || props.contains( "ALBARAN") )
		cabecera_validate +=
	"\tif( sender == editEntrega )\n"
	"\t\tactTotales();\n";

	cpcpp->insert_extrusion( ext_prefix + "CABECERA_VALIDATE", cabecera_validate );

	cpcpp->insert_extrusion( ext_prefix + "CABECERA_UPDATEFROMDETAILS",
"void FrmEdit" + recname + "::updateFromDetails( class FrmEditRecDetail *detailfrm )\n"
"{\n"
"	actTotales();\n"
"	numeraLineas();\n", "}\n" );

	cpcpp->insert_extrusion( ext_prefix + "CABECERA_NUMERALINEAS",
"void FrmEdit" + recname + "::numeraLineas()\n"
"{\n"
"	dbRecordList *reclst = getRec" + recname + "()->getList" + recname + "Det();\n"
"	for ( unsigned int i = 0; i < reclst->size(); i++ ) {\n"
"		Rec" + recname + "Det *detalle = static_cast<Rec" + recname + "Det *>( reclst->at( i ) );\n"
"		if( !detalle->isEmpty() ) // No numerar detalles vacíos \n"
"			detalle->setValue( \"NLINEA\", i+1 );\n"
"	}\n", "}\n");

	cpcpp->insert_extrusion( ext_prefix + "CABECERA_GENNUMDOC",
"void FrmEdit" + recname + "::genNumeroDocumento()\n"
"{\n"
"\t\teditContador->setText( ModuleInstance->getMaxContador( \"" + recname.upper()  + "\",\n"
"\t\t\tgetRecord()->getValue(\"EMPRESA_ID\").toInt(),\n"
"\t\t\tModuleInstance->getEmpresaModule()->getEjercicio(),\n"
"\t\t\tgetRecTipoDoc()->getValue(\"SERIE\").toString() ) );\n"
"\t\teditContador->setJustEdited( true );\n"
"\t\tvalidateFields( editContador, 0 );\n"
, "}\n");

	Xtring act_totales =
"void FrmEdit" + recname + "::actTotales()\n"
"{\n"
"\tgetRec" + recname + "()->gather( this );\n"
"\tgetRec" + recname + "()->actTotales();\n"
"\tgetRec" + recname + "()->scatter( this );\n";

	cpcpp->insert_extrusion( ext_prefix + "CABECERA_ACTTOTALES",
							 act_totales, "}\n");

}

void insert_detalles(const XtringList &props, CppModule *cpcpp, CppModule *cph,
					 const Xtring &frmtype,	const gong::Xtring &recname,
					 const Xtring &ext_prefix)
{
	bool escompra = props.contains("COMPRA");

	Xtring s =
"\tbool actualiza = (sender && (sender == pFocusWidget) );\n"
"\tdouble cantidad = editCantidad->toDouble();\n"
"\tdouble pvpsiniva = editPVPSinIVA->toDouble();\n"
"\tdouble pvpconiva = editPVP->toDouble();\n"
"\tdouble dtop100 = editDtoP100->toDouble();\n"
"\tdouble importesiniva =  cantidad * pvpsiniva * ( 1 - dtop100 / 100 );\n";
	if( !escompra ) {
		s +=
"\t\tint tarifacliente = static_cast<FrmEdit" + Xtring(recname).replace("Det","") + " *>(getFrmMaster())->getRecCliente()->getValue(\"TARIFA\").toInt();\n";
	}
	s +=
"\tif ( sender == editCantidad && editCantidad->isJustEdited() ) {\n"
"\t\tactualiza = true;\n"
"\t}\n"
"\tif ( sender == editArticuloCodigo && editArticuloCodigo->isJustEdited() ) {\n"
"\t\tactualiza = true;\n"
"\t\teditTipoIVACodigo->setText( getRecArticulo()->getRecTipoIVA()->getValue( \"CODIGO\" ) );\n"
"\t\teditTipoIVACodigo->setJustEdited( true );\n"
"\t\tif ( validSeekCode( editTipoIVACodigo, isvalid, *validresult, editTipoIVACodigo, editTipoIVANombre,\n"
"\t\t                    getRecTipoIVA(), \"CODIGO\", \"NOMBRE\", Xtring::null ) )\n"
"\t\t\tscatterTipoIVA();\n";
	if( !escompra ) {
		s +=
"\t\tif( tarifacliente == 0 )\n"
"\t\t\tpvpconiva = getRecArticulo()->getValue( \"PVP\" ).toDouble();\n"
"\t\telse\n"
"\t\t\tpvpconiva = getRecArticulo()->getValue( \"PVP\" + Xtring::number(tarifacliente) ).toDouble();\n";
	} else {
		s += "\t\tpvpconiva = getRecArticulo()->getValue( \"PVP\" ).toDouble();\n";
	}
	s +=
"\t\t// El precio con IVA se calcula tomando el tipo de iva de este detalle\n"
"\t\tpvpsiniva = pvpconiva / ( 1 + getRecTipoIVA()->getValue( \"IVA\" ).toDouble() / 100 );\n"
"\t}\n"
"\tif( (sender == editTipoIVACodigo && editTipoIVACodigo->isJustEdited())\n"
"\t\t|| (sender == editPVPSinIVA && editPVPSinIVA->isJustEdited()) ) {\n"
"\t\tactualiza = true;\n"
"\t\tpvpsiniva = editPVPSinIVA->toDouble();\n"
"\t\tpvpconiva = pvpsiniva * ( 1 + getRecTipoIVA()->getValue( \"IVA\" ).toDouble() / 100 );\n"
"\t}\n"
"\tif ( sender == editPVP && editPVP->isJustEdited() ) {\n"
"\t\tactualiza = true;\n"
"\t\tpvpconiva = editPVP->toDouble();\n"
"\t\tpvpsiniva = pvpconiva / ( 1 + getRecTipoIVA()->getValue( \"IVA\" ).toDouble() / 100 );\n"
"\t}\n"
"\tif ( sender == editDtoP100 && editDtoP100->isJustEdited() ) {\n"
"\t\tactualiza = true;\n"
"\t\tdtop100 = editDtoP100->toDouble();\n"
"\t}\n"
"\tif( sender == editImporte && editImporte->isJustEdited() ) {\n"
"\t\timportesiniva = editImporte->toDouble();\n"
"\t\tif( cantidad != 0.0 && pvpsiniva != 0.0 ) {\n"
"\t\t\tdtop100 = 100 * ( 1 - importesiniva / (cantidad * pvpsiniva) );\n"
"\t\t\teditDtoP100->setText( dtop100 );\n"
"\t\t}\n"
"\t\tactualiza = true;\n"
"\t}\n"
"\tif( sender == editImporteConIVA && editImporteConIVA->isJustEdited() ) {\n"
"\t\tif( cantidad != 0.0 && pvpsiniva != 0.0 ) {\n"
"\t\t\tdtop100 = 100 * ( 1 - editImporteConIVA->toDouble() / (cantidad * pvpconiva) );\n"
"\t\t\teditDtoP100->setText( dtop100 );\n"
"\t\t}\n"
"\t\tactualiza = true;\n"
"\t}\n"
"\tif( actualiza ) {\n"
"\t\teditPVPSinIVA->setText( pvpsiniva );\n"
"\t\teditPVP->setText( pvpconiva );\n"
"\t\tint ndec = empresa::ModuleInstance->getDecimalesMoneda();\n"
"\t\tif( getFrmMaster()->getControlValue( \"IVADETALLADO\" ).toInt() == factu::FldIVADetallado::incluido )\n"
"\t\t\tndec += 2;\n"
"\t\tMoney importesiniva(cantidad * pvpsiniva * ( 1 - dtop100 / 100 ), ndec);\n"
"\t\tMoney importeconiva = Money( cantidad * pvpconiva * ( 1 - dtop100 / 100 ),\n"
"\t\t\tempresa::ModuleInstance->getDecimalesMoneda() );\n"
"\t\teditImporte->setText( importesiniva );\n"
"\t\teditImporteConIVA->setText( importeconiva );\n"
"\t}\n"
"\tif (( sender == editArticuloCodigo && editArticuloCodigo->isJustEdited() ) ||\n"
"\t\t    ( sender == editCantidad && editCantidad->isJustEdited() ) ) {\n"
"\t\tif ( !getRecArticulo()->isNew() ) {\n"
"\t\t\tXtring stock_msg;\n"
"\t\t\tif( getRecArticulo()->getValue( \"USARSTOCKS\" ).toBool() )\n"
"\t\t\t\tstock_msg = getRecArticulo()->getValue( \"STOCK\" ).toString();\n"
"\t\t\telse\n"
"\t\t\t\tstock_msg = _(\"No se usan\");\n"
"\t\t\tXtring precios_msg;\n";
	if( escompra ) {
		s.replace( "PVPSINIVA", "COSTESINIVA" );
		s.replace( "PVP", "Coste" );
		s.replace( "pvpsiniva", "costesiniva" );
		s.replace( "pvpconiva", "costeconiva" );
	}
	if( !escompra ) {
		s +=
"\t\t\tif( tarifacliente != 0 )\n"
"\t\t\t\tprecios_msg = _( \"%s: PVP DEL CLIENTE: %s, Coste: %s, Existencias: %s\" );\n"
"\t\t\telse\n"
"\t";
	}
	if( escompra ) {
		s +=
"\t\t\tprecios_msg = _( \"%s: PVP: %s, Coste: %s, Existencias: %s\" );\n"
"\t\t\tDBAPP->showOSD( getTitle(), Xtring::printf( precios_msg,\n"
"\t\t                       getRecArticulo()->getValue( \"CODIGO\" ).toString().c_str(),\n"
"\t\t                       getRecArticulo()->getValue( \"PVP\" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),\n"
"\t\t                       editCoste->toMoney().toString( DBAPP->getRegConfig() ).c_str(),\n"
"\t\t                       stock_msg.c_str() ) );\n"
"\t\t}\n"
"\t}\n";
	} else {
		s +=
"\t\t\tprecios_msg = _( \"%s: PVP: %s, Coste: %s, Existencias: %s\" );\n"
"\t\t\tDBAPP->showOSD( getTitle(), Xtring::printf( precios_msg,\n"
"\t\t                       getRecArticulo()->getValue( \"CODIGO\" ).toString().c_str(),\n"
"\t\t                       editPVP->toMoney().toString( DBAPP->getRegConfig() ).c_str(),\n"
"\t\t                       getRecArticulo()->getValue( \"COSTE\" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),\n"
"\t\t                       stock_msg.c_str() ) );\n"
"\t\t}\n"
"\t}\n";
	}

	cpcpp->insert_extrusion( ext_prefix + "DETALLE_VALIDATE", s );

	cpcpp->append_text_to_extrusion( ext_prefix + "INIT_CONTROLS",
	    "\teditNLinea->setMustBeReadOnly( true );");
}


}; // Namespace capel

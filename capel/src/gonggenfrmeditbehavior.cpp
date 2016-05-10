#include <iostream>
#include <gongfileutils.h>
#include "gonggenerators.h"
#include "gonggenfrmeditrecmaestro.h"

using namespace gong;

namespace capel {

void addFrmEditBehavior(CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
                          const Xtring &fullrecname, const XtringList &fields,
                          const Xtring &props, const Xtring &_inherit)
{
	Xtring recname = cpcpp->extractClassname( fullrecname );
	Xtring _namespace = cpcpp->extractNamespace( fullrecname );
	Xtring fullfrmname;
	Xtring frmname = "FrmEdit" + recname + "Behavior";
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
		"#include <dbappmainwindow.h>\n";
		"#include <dbappfrmeditrec.h>\n";
/*
	if( frmname == "FrmEditRecMaster" || frmname == "FrmEditRecDetails" )
		includes += "#include \"" + fnameprefix + frmname.lower() + ".h\"\n";
	else
*/
	includes += "#include \"" + ( _namespace.isEmpty() ? Xtring("gong") : _namespace )
		+ frmname.lower() + ".h\"\n";

	cpcpp->insert_extrusion_at(cpcpp->position_after_extrusion( "MODULE_INFO"),
		ext_prefix + "INCLUDES", includes, namespaces, "\n");

#if 0
	Xtring frm_constructor;
	if( frmtype == "FrmEditRecDetail" ) {
		frm_constructor +=
frmname + "::" + frmname + "(FrmEditRecMaster *frmmaster, dbRecordList *details, int ndetail,\n"
		"\tdbRecord *detail, dbRecordDataModel *dm,\n"
"\tEditMode editmode, dbApplication::EditFlags editflags,\n"
"\tQWidget* parent, const char* name, WidgetFlags fl )\n"
"\t\t: " + fullfrmtype + "( frmmaster, details, ndetail, detail, dm, editmode, editflags, parent, name, fl )\n";
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
#endif

	XtringList proplist;
	props.tokenize( proplist, " " );

	// .H
	cph->addHeaderPreamble( fullfrmname );
	Xtring h_frm_constructor;
	Xtring h_class =
"class " + frmname + ": public " + fullfrmtype + "\n"
"{\n";
	if( proplist.contains( "Q_OBJECT" ) )
		h_class += "\tQ_OBJECT\n";
	h_class += "public:\n";
#if 0
	if( frmtype == "FrmEditRecDetail" ) {
		h_class +=
"\t" + frmname +"(FrmEditRecMaster *frmmaster, dbRecordList *details, int ndetail,\n"
"\t\tdbRecord *detail, dbRecordDataModel *dm=0,\n"
"\t\tEditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,\n"
"\t\tQWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );\n";
	} else {
		h_class +=
"\t" + frmname +"(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,\n"
"\t\tEditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,\n"
"\t\tQWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );\n";
	}
#endif
	h_class += "\tvirtual void initGUI();\n";

	if( !proplist.contains("NoCreateRecord") ) {
		if( _inherit.isEmpty() || (inherit != recname) ) {
			h_class +=
	"\tRec" + recname +" *getRec" + recname +"() const { return static_cast<Rec" + recname +" *>(pTheForm->getRecord()); }\n";
		}
	}
	h_class += "\t" + frmname + "( FrmEditRec *theform )\n"
		"\t\t: FrmEditRecBehavior( theform )\n";
	cph->insert_extrusion( ext_prefix + "CLASS", h_class, "\t{}\n" );

	cph->insert_extrusion( ext_prefix + "VIRTUALS_FROM_FRMEDITRECBEHAVIOR",
"protected:\n"
"\tvirtual void scatterFields( bool is_pre );\n"
"\tvirtual void gatherFields();\n"
"\tvirtual void validateFields( bool is_pre,\n"
"\t\tQWidget *sender, bool *isvalid, ValidResult *ir = 0);\n",
	"",
"\t// virtual void updateStatus();\n"
"\t// virtual bool canClose();\n"
	);
}

void addFrmEditBehaviorFields(CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
	const Xtring &fullrecname, const XtringList &fields, const XtringList &members,
	const Xtring &props, const Xtring &_inherit)
{
	Xtring recname = cpcpp->extractClassname( fullrecname );
	Xtring _namespace = cpcpp->extractNamespace( fullrecname );
	Xtring frmname = "FrmEdit" + recname + "Behavior";
	Xtring ext_prefix = frmname.upper() + "_";
	Xtring fnameprefix = FileUtils::basename(  cph->getFilename() );
	fnameprefix = fnameprefix.left( fnameprefix.find("frm") );
	Xtring namespaces = "\nnamespace gong {\n" + ( _namespace == "" ? "" : "namespace " + _namespace + " {\n") + "\n";
	Xtring scatter, frmscatter;
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
	layouts_start =
"\tQWidget *pControlsFrame = getControlsFrame();\n"
"\tQVBoxLayout* pControlsLayout = getControlsLayout();\n";

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
		if( parent != "pControlsFrame" && !parentslist.contains( parent ) ) {
			parentslist << parent;
			if( parent.startsWith( "tab" ) ) {
				hastabs = true;
				layouts_start +=
					"\tQWidget *" + parent + " = getOrCreateTab( \"" + parent + "\" );\n";
			}
			layouts_start += "\tQVBoxLayout *" + parent + "Layout = static_cast<QVBoxLayout *>( " + parent + "->layout() );\n";
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
			ctrlname = "edit" + fldname;
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toDateTime());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \"" +
				fldname.upper() + "\", " + ctrlname + "->toDateTime());\n" + cecpp;
		} else if( type.upper() == "DATE" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toDate());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
					+ fldname.upper() + "\", " + ctrlname + "->toDate());\n" + cecpp;
		} else if( type.upper() == "TIME" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toTime());\n" + cecpp;
			gather += cscpp +"\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toTime());\n" + cecpp;
		} else if( type.upper() == "STRING" || type.upper() == "TEXT" || type.upper() == "RICHTEXT") {
			ctrlname = "edit" + fldnamereplaced;
			if( type.upper() == "STRING" )
				ctrltype = "gong::EditBox";
			else if( type.upper() == "RICHTEXT" )
				ctrltype = "gong::RichTextBox";
			else
				ctrltype = "gong::TextBox";
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toString());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toString());\n" + cecpp;
		} else if( type.upper() == "STRING" || type.upper() == "TEXT" || type.upper() == "RICHTEXT") {
			ctrlname = "edit" + fldname;
			if( type.upper() == "STRING" )
				ctrltype = "gong::EditBox";
			else if( type.upper() == "RICHTEXT" )
				ctrltype = "gong::RichTextBox";
			else
				ctrltype = "gong::TextBox";
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toString());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toString());\n" + cecpp;
		} else if( type.upper() == "INT" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toInt());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toInt());\n" + cecpp;
		} else if( type.upper() == "MONEY" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toMoney());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toMoney());\n" + cecpp;
		} else if( type.upper() == "DOUBLE" || type.upper() == "FLOAT" ) {
			ctrltype = "gong::EditBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toDouble());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+  fldname.upper() + "\", " + ctrlname + "->toDouble());\n" + cecpp;
		} else if( type.upper() == "BOOL" ) {
			ctrltype = "gong::CheckBox";
			ctrlname = "check" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setChecked( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toBool());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", check" + fldname + "->isChecked());\n" + cecpp;
		} else if( type.upper() == "COMBOSTRING" ) {
			ctrltype = "gong::ComboBoxXtring";
			ctrlname = "combo" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toString());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", combo" + fldnamereplaced + "->toString());\n" + cecpp;
		} else if( type.upper() == "COMBOINT" ) {
			ctrltype = "gong::ComboBoxInt";
			ctrlname = "combo" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setCurrentItemByValue( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toInt());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", combo" + fldnamereplaced + "->getCurrentItemValue());\n" + cecpp;
		} else if( type.upper() == "IMAGE" ) {
			ctrltype = "gong::ImageBox";
			ctrlname = "edit" + fldnamereplaced;
			scatter += cscpp + "\t" + ctrlname + "->setImageData( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\"));\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toData());\n" + cecpp;
		} else if( type.upper().startsWith("REFERENCE") || type.upper().startsWith("MULTIPLEREFERENCE") ) {
			bool multiplereference = type.upper().startsWith("MULTIPLEREFERENCE");
			// FIELD Proyecto_ID Reference(Proyecto,Codigo,Descripcion) - nasientolayout
			isgenerated = true;
			Xtring referredfield = fldname.left(fldname.find_last_of("_"));
			XtringList codedesc;
			type.tokenize( codedesc, "(,)" );
			Xtring referredtablecomplete = codedesc[1];
			Xtring referredtablenamespace = CppModule::extractNamespace( referredtablecomplete );
			if( !referredtablenamespace.isEmpty() )
				referredtablenamespace += "::";
			Xtring referredtable = CppModule::extractClassname( referredtablecomplete );

			Xtring fldcode = codedesc[2];
			Xtring flddesc = codedesc[3];
			Xtring seekcodeflags;
			if( codedesc.size()>3 )
				seekcodeflags = codedesc[4];
			scatter += cscpp + "\tscatter" + referredfield + "();\n" + cecpp;
			if( firstfield && _inherit.isEmpty() ) {
				firstfield = false;
			}
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+  fldname.upper() + "\", getRec" + referredfield + "()->getRecordID() );\n" + cecpp;
			hcontrols += csh +
				"\tgong::SearchBox *search" + referredfield + fldcode + ";\n"
				"\tQPushButton *push" + referredfield + fldcode + ";\n"
				"\tgong::LineEdit *edit" + referredfield + fldcode + ";\n"
				"\tgong::LineEdit *edit" + referredfield + flddesc + ";\n" + ceh;

			init_controls += "\n" + cscpp +
"\tsearch" + referredfield + fldcode + " = pTheForm->addSearchField( " + parent + ", \""
+ fldname.upper() + "\", \"" + referredtable.upper() + "\", \"" + fldcode.upper() + "\", \"" + flddesc.upper() + "\", " + layout + " );\n"
"\tpush" + referredfield + fldcode + " = search" + referredfield + fldcode + "->getButton();\n"
"\tconnect( push" + referredfield + fldcode + ", SIGNAL( clicked() ), this, SLOT( push" + referredfield + fldcode + "_clicked() ) );\n"
"\tedit" + referredfield + fldcode + " = search" + referredfield + fldcode + "->getEditCode();\n"
"\tedit" + referredfield + flddesc + " = search" + referredfield + fldcode + "->getEditDesc();\n" + cecpp;

			Xtring relatedscatter;
			Xtring ctrlcode =  "edit" + referredfield + fldcode;
			Xtring ctrldesc =  "edit" + referredfield + flddesc;

			validate += cscpp +
"\tif( pTheForm->focusWidget() != push" + referredfield + fldcode + ") // To avoid triggering the validating if the button is pressed\n"
"\tif( pTheForm->validSeekCode(" + (multiplereference ? Xtring("m" + referredtable + "List,") : Xtring("") ) + " sender, isvalid, *validresult, " + ctrlcode + ", " + ctrldesc + ",\n"
"\t\tgetRec" + referredfield + "(), \"" + fldcode.upper() + "\", \"" + flddesc.upper() + "\", Xtring::null";
			if( multiplereference ) {
				if( seekcodeflags.isEmpty() == false )
					seekcodeflags += " + ";
				seekcodeflags += "dbApplication::SeekCodeMultiple";
			}
			if( !seekcodeflags.isEmpty() )
				validate += ", dbRecord::SeekCodeFlags( " + seekcodeflags + " )";
			validate +=") )\n"
		"\t\tscatter" + referredfield + "();\n" + cecpp;

			// scatterPrincesa
			relatedscatter += "\t" + ctrlcode +
				"->setText( getRec" + referredfield + "()->getValue(\"" + fldcode.upper() + "\") );\n";
			relatedscatter += "\t" + ctrldesc +
				"->setText( getRec" + referredfield + "()->getValue(\"" + flddesc.upper() + "\") );\n";

			cpcpp->insert_extrusion(ext_prefix + "SCATTER_" + referredfield.upper(), relatedscatter, "}\n",
				"\nvoid " + frmname + "::scatter" + referredfield + "()\n{\n");

			Xtring pushrefclicked =
"\tchar action = getControlKeyPressed();\n"
"\tif( !pTheForm->isEditing() || " + Xtring(ctrlcode).replace("edit","search") + "->mustBeReadOnly() )\n"
"\t\taction = 'E';\n"
"\tswitch( action ) {\n"
"\t\tcase 'F':\n"
"\t\tcase '\\0':\n"
"\t\t\t" + ctrlcode + "->setJustEdited( false );\n"
"\t\t\t" + ctrlcode + "->setCancelling();\n"
"\t\t\tif( DBAPP->choose" + (multiplereference ? Xtring("Multi( m" + referredtable + "List, " ) : Xtring("(") ) + "pTheForm, getRec" + referredfield + "(), 0, dbApplication::editNone, pTheForm ) ) {\n"
"\t\t\t\tpTheForm->setEdited(true);\n"
"\t\t\t\tscatter" + referredfield + "();\n";
		if( !multiplereference )
			pushrefclicked += "\t\t\t\t" + ctrlcode + "->setJustEdited( true );\n";
		pushrefclicked +=
"\t\t\t\tpTheForm->setWiseFocus(" + ctrlcode + ");\n"
"\t\t\t}\n"
"\t\t\tbreak;\n"
"\t\tcase 'M':\n"
"\t\t\t{\n"
"\t\t\t\tif( getRec" + referredfield + "()->getRecordID() ) {\n"
"\t\t\t\t\t" + ctrlcode + "->setJustEdited( false );\n"
"\t\t\t\t\tif( DBAPP->editRecord(pTheForm,\n"
"\t\t\t\t\t\t\tgetRec" + referredfield + "(), 0, DataTable::updating,\n"
"\t\t\t\t\t\t\t\tdbApplication::simpleEdition, pTheForm ) ) {\n"
"\t\t\t\t\t\t" + ctrlcode + "->setJustEdited( true );\n"
"\t\t\t\t\t\tscatter" + referredfield + "();\n"
"\t\t\t\t\t}\n"
"\t\t\t\t\tpTheForm->setWiseFocus(" + ctrlcode + ");\n"
"\t\t\t\t}\n"
"\t\t\t}\n"
"\t\t\tbreak;\n"
"\t\tcase 'E':\n"
"\t\t\t{\n"
"\t\t\t\t" + ctrlcode + "->setJustEdited( false );\n"
"\t\t\t\tDBAPP->createClient( DBAPP->createEditForm(static_cast<FrmEditRec *>(pTheForm),\n"
"\t\t\t\t\tgetRec" + referredfield + "(), 0, DataTable::selecting,\n"
"\t\t\t\tdbApplication::simpleEdition, pTheForm ) );\n"
"\t\t\t}\n"
"\t\t\tbreak;\n"
"\t\tcase 'A':\n"
"\t\t\t{\n"
"\t\t\t\t" + referredtablenamespace + "Rec" + referredfield + " *tmprec = static_cast<" + referredtablenamespace + "Rec" + referredfield + " *>(DBAPP->createRecord( \"" + referredfield + "\" ));\n"
"\t\t\t\t" + ctrlcode + "->setJustEdited( false );\n"
"\t\t\t\ttmprec->clear( true ); // set default values\n"
"\t\t\t\tDBAPP->setCodeNotFound( " + ctrlcode + "->toString() );\n"
"\t\t\t\tif( DBAPP->editRecord(pTheForm, tmprec, 0, DataTable::inserting,\n"
"\t\t\t\t\tdbApplication::simpleEdition, pTheForm ) ) {\n"
"\t\t\t\t\t" + ctrlcode + "->setJustEdited( true );\n"
"\t\t\t\t\tgetRec" + referredfield + "()->copyRecord( tmprec );\n"
"\t\t\t\t\tscatter" + referredfield + "();\n"
"\t\t\t\t}\n"
"\t\t\t\tpTheForm->setWiseFocus(" + ctrlcode + ");\n"
"\t\t\t\tDBAPP->setCodeNotFound( Xtring() );\n"
"\t\t\t}\n"
"\t\t\tbreak;\n"
"\t}\n";

			cpcpp->insert_extrusion(ext_prefix + "PUSH_" + referredfield.upper() + "_" + fldcode.upper() +  "_CLICKED",
									pushrefclicked, "}\n",
"void " + frmname + "::push" + referredfield + fldcode + "_clicked()\n"
"{\n");

			special_action += cscpp +
				"\tif( sender == " + ctrlcode + " )\n"
				"\t\tpush" + referredfield + fldcode + "_clicked();\n" + cecpp;

//			hincludes += "#include \"" + fnameprefix + "rec" + referredfield.lower() + ".h\"\n";
			hscatters += "\tvoid scatter" + referredfield + "();\n";
// Los slots no tienen condicional porque el moc no los incluye
			hslots += "\tvoid push" + referredfield + fldcode + "_clicked();\n";
#if 0
			hgetrecords += cs + "\t" + referredtablenamespace + "Rec" + referredfield +"* getRec" + referredfield +"() const\n"
				"\t\t{ return static_cast<Rec" + recname
				+ "*>(getRecord())->getRec" + referredfield +"(); }\n" + ce;
#endif
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
					"\t\tDBAPP->createEditForm(pTheForm, " + fldname.lower() + ", " + fldname.lower() + "dm, DataTable::browsing,\n"
					+"\t\tstatic_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),\n"
					"\t\t" + parent + ") );\n"
					"\t" + layout + "->addWidget(pFrm" + fldname + ");\n";
					"\n" + cecpp;
			} else {
				init_controls += cscpp +
					"\n// frmDetails: " + fldname + "\n"
					"\tQFrame *" + fldname.lower() + "Frame = new QFrame(pTheForm);\n"
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
					"\t\tDBAPP->createEditDetailForm( static_cast<FrmEditRecMaster *>(pTheForm),\n"
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
			ctrlname = "edit" + fldnamereplaced;
			ctrltype = type;
			scatter += cscpp + "\t" + ctrlname + "->setText( pTheForm->getRecord()->getValue(\"" + fldname.upper() + "\").toString());\n" + cecpp;
			gather += cscpp + "\tpTheForm->getRecord()->setValue( \""
				+ fldname.upper() + "\", " + ctrlname + "->toString());\n" + cecpp;
		}
		if( !isgenerated ) {
			if( firstfield && _inherit.isEmpty() ) {
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
	"\t" + ctrlname + " = pTheForm->addComboIntField( " /*values" + fldname + ", " */+ parent + ", \""
		+ tableoffield.upper() + "\", \"" + fieldoffield.upper() + "\", " + layout + " );\n" + cecpp;
			} else if( ctrltype == "gong::ComboBoxXtring" ) {
				init_controls += cscpp +
	"\t" + ctrlname + " = pTheForm->addComboXtringField( " /*values" + fldname + ", " */ + parent + ", \""
		+ tableoffield.upper() + "\", \"" + fieldoffield.upper() + "\", " + layout + " );\n" + cecpp;
			} else {
				init_controls += cscpp +
	"\t" + ctrlname + " = pTheForm->add" + ctrltype.replace("Box", "Field").replace("gong::","") + "( " + parent
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
	}

	if( !_inherit.isEmpty() )
		scatter = "\t" + inherit_namespace + "FrmEdit" + inherit + "::scatterFields( bool is_pre );\n" + scatter;
	cpcpp->insert_extrusion(ext_prefix + "SCATTER", scatter, frmscatter + "}\n",
		"\nvoid " + frmname + "::scatterFields( bool is_pre )\n{\n"
		"\tif( !is_pre) return;\n" );
	if( !_inherit.isEmpty() )
		gather = "\t" + inherit_namespace + "FrmEdit" + inherit + "::gatherFields();\n" + gather;
	cpcpp->insert_extrusion(ext_prefix + "GATHER", gather, "}\n",
		"\nvoid " + frmname + "::gatherFields()\n{\n" );
	Xtring initgui_pre =
		"void " + frmname + "::initGUI()\n{\n";
	cpcpp->insert_extrusion_at(cpcpp->position_after_extrusion( ext_prefix + "INCLUDES" ),
		ext_prefix + "INITGUI", layouts_start + init_controls + layouts_end, "}\n", initgui_pre);

	if( !special_action.isEmpty() ) {
		cpcpp->insert_extrusion(ext_prefix + "SPECIALACTION", special_action,
			"}\n",
			"void " + frmname + "::specialControlKeyPressed(QWidget *sender, char key)\n{\n");
		cph->append_text_to_extrusion(ext_prefix + "VIRTUALS_FROM_FRMEDITRECBEHAVIOR",
		"\tvirtual void specialControlKeyPressed(QWidget *sender, char key);\n" );
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
"\t" + inherit_namespace + "FrmEdit" + inherit + "::validateFields(bool is_pre,\n"
			"\t\tsender, isvalid, validresult);\n";
	} else {
	}
	whole_validate += validate;

	cpcpp->insert_extrusion(ext_prefix + "VALIDATE", whole_validate,
	    "\tif( !ir ) {\n"
	    "\t\tshowValidMessages(isvalid, *validresult, sender);\n"
	    "\t\tdelete validresult;\n"
	    "\t}\n"
        "}\n\n",
		"\nvoid " + frmname + "::validateFields(bool is_pre,\n"
		"\t\tQWidget *sender, bool *isvalid, ValidResult *ir)\n"
		"{\n"
		"\tif( !is_pre) return;"
	);

	Xtring fin =
"} // namespace " + _namespace + "\n"
"} // namespace gong\n";
	cpcpp->insert_extrusion( ext_prefix + "FIN", fin, "\n" );

	// .H
	Xtring ext_includes =
		"#include <dbappfrmeditrec.h>\n"
		"#include <dbappfrmeditrecbehavior.h>\n";
// 	if( _inherit.isEmpty() || (inherit != recname) )
// 		ext_includes += "#include \"" + fnameprefix + + "rec" + recname.lower() + ".h\"\n" + hincludes;
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


}; // Namespace capel


#include <stdexcept>
#include "gonglib_inc.h"
#include "gonggenerators.h"

using namespace gong;

namespace capel {

void addRecordDefinition(CppModule *cpcpp, CppModule *cph, const Xtring &fullrecname,
                         const Xtring &props, const XtringList &members,
                         const gong::XtringList &relations, const Xtring &_inherit,
						 const gong::XtringList &_interfaces )
{
	Xtring recname = cph->extractClassname( fullrecname );
	Xtring _namespace = cph->extractNamespace( fullrecname );
	Xtring ext_prefix = recname.upper() + "_";
	bool hayvalid = false, haytostring = false;
	Xtring fnameprefix = FileUtils::basename(  cph->getFilename() );
	fnameprefix = fnameprefix.left( fnameprefix.find("rec") );
	Xtring inherit, inherit_namespace;
	if( !_inherit.isEmpty() ) {
		inherit_namespace = cpcpp->extractNamespace( _inherit );
		if( !inherit_namespace.isEmpty() )
			inherit_namespace += "::";
		inherit = cpcpp->extractClassname( _inherit );
	}
	Xtring baserecordtype = inherit.isEmpty() ? "dbRecord" :
		(inherit_namespace.isEmpty() ? inherit : Xtring(inherit_namespace + "Rec" + inherit));
	Xtring constructor;
	Xtring namespaces = "\nnamespace gong {\n" + ( _namespace == "" ? "" : "namespace " + _namespace + " {\n") + "\n";

	Xtring interfaces;
	if( _interfaces.size() ) {
		for( XtringList::const_iterator it = _interfaces.begin();
			it != _interfaces.end(); ++ it ) {
			XtringList interface_props;
			it->tokenize( interface_props, " ");
			_GONG_DEBUG_ASSERT( interface_props.size() > 1 );
			bool is_last = (it + 1 ) == _interfaces.end();
			if( interface_props.size() == 2 ) {
				interfaces += "\t" + *it + "";
				if( !is_last )
					interfaces += ",";
				interfaces += "\n";
			} else {
				interfaces += interface_props[2].replace("|"," ") + "\n\t"
					+ interface_props[0] + " " + interface_props[1];
				if( !is_last )
					interfaces += ",";
				interfaces += "\n"
					"#endif\n";
			}
		}
		interfaces = ",\n" + interfaces;
	}
	constructor =
"class Rec" + recname + ": public " + baserecordtype + interfaces + "\n"
"{\n"
"public:\n"
"\tRec" + recname + "(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)\n"
"\t\t: " + baserecordtype + "(conn, ";
	if( inherit.isEmpty() )
		constructor +=  "DBAPP->getDatabase()->findTableDefinition(\""
			+ recname.upper() + "\"), ";
	constructor += "recid, user)\n";

	cph->insert_extrusion(ext_prefix + "CONSTRUCTOR", constructor,
	                      "\t{};\n}; // end class\n\n");

	Xtring cppincludes =
	"#include \"" + fnameprefix + "rec" + recname.lower() + ".h\"\n";
	Xtring hincludes;
	Xtring hrelations, cpprelations;
	for( XtringList::const_iterator it = relations.begin();
		it != relations.end();
		++ it ) {
		Xtring relationnamecomplete = (*it).left( (*it).find_first_of( " \t" ) );
		Xtring relationnamespace = cph->extractNamespace( relationnamecomplete );
		Xtring rnms_include = fnameprefix;
		if( !relationnamespace.isEmpty() ) {
			rnms_include = relationnamespace;
			relationnamespace += "::";
		}
		Xtring relationname = cph->extractClassname( relationnamecomplete );
		Xtring relationprops = (*it).mid( (*it).find_first_of( " \t" ) + 1 );
		XtringList proplist;
		relationprops.tokenize( proplist, " " );
		bool containsdetalles = false;
		if( proplist.contains("Detalles") )
			containsdetalles = true;
		proplist.remove( "Detalles" );
		Xtring moreprops = proplist.size() ? proplist[0] : "";
		Xtring csh, cscpp, ceh, cecpp; // conditional_start, conditional_end;
		getConditionalCode( moreprops, csh, cscpp, ceh, cecpp );
		if( containsdetalles) {
			hrelations += csh;
			cpprelations += cscpp;
			hrelations += "\t" + relationnamespace + "Rec" + relationname + " *getRec" + relationname + "( int n" + relationname.lower() + " = -1 ) const;\n";
			cpprelations +=
				"Rec" + relationname + " *Rec" + recname + "::getRec" + relationname + "( int " + relationname.lower() + " ) const\n"
				"{\n"
				"\treturn static_cast<Rec" + relationname + "*>(findRelationByRelatedTable(\"" + relationname.upper() + "\" )->getRelatedRecord( " + relationname.lower() + "));\n"
				"}\n\n";
			hrelations += "\tdbRecordList *getList" + relationname + "() const;\n";
			cpprelations += "dbRecordList *Rec" + recname + "::getList" + relationname + "() const\n"
				"{\n"
				"\treturn findRelationByRelatedTable( \"" + relationname.upper() + "\" )->getRelatedRecordList();\n"
				"}\n";
			hrelations += ceh;
			cpprelations += cecpp;
		} else {
			hrelations += csh;
			cpprelations += cscpp;
			hrelations += "\t" + relationnamespace + "Rec" + relationname + " *getRec" + relationname + "() const;\n";
			cpprelations +=
				relationnamespace + "Rec" + relationname + " *Rec" + recname + "::getRec" + relationname + "() const\n"
				"{\n"
				"\treturn static_cast<" + relationnamespace + "Rec" + relationname + "*>(findRelatedRecord(\"";
			// No incluir el nombre de la tabla para que funcionen los registros derivados
			if( !inherit.isEmpty() )
				cpprelations += inherit.upper() + ".";
			cpprelations += relationname.upper() + "_ID\"));\n"
				"}\n\n";
			hrelations += ceh;
			cpprelations += cecpp;
		}
		hincludes += csh + "#include \"" + rnms_include.lower() + "rec" + relationname.lower() + ".h\"\n" + ceh;
	}

	Xtring hmembers;
	cpcpp->insert_extrusion_at(cph->position_after_extrusion("PREAMBLE"),
		ext_prefix + "INCLUDES", cppincludes, namespaces );
	if( relations.size() )
		cpcpp->insert_extrusion(ext_prefix + "RELATIONS", cpprelations);

	for( XtringList::const_iterator it = members.begin();
		it != members.end();
		++ it ) {
		Xtring membername = (*it);
		if( membername == "clear" ) {
			hmembers += "\tvirtual void clear( bool setcustomvalues = true );\n";
			cpcpp->insert_extrusion(ext_prefix + "CLEAR",
"void Rec" + recname + "::clear( bool setcustomvalues )\n"
"{\n", "\n}\n");
		} else if( membername == "init" ) {
			hmembers += "\tvoid init();\n";
			cpcpp->insert_extrusion(ext_prefix + "INIT",
"void Rec" + recname + "::init()\n"
"{\n", "\n}\n");
		} else if( membername == "toString" ) {
			haytostring = true;
			hmembers +=
				"\tXtring toString(int format, const RegExp &includedFields=RegExp()) const;\n";
			cpcpp->insert_extrusion(ext_prefix + "TOSTRING",
				"Xtring Rec" + recname + "::toString(int format, const RegExp &includedFields) const\n"
				"{\n"
				"\tXtring result;\n",
				"\tresult = " + baserecordtype + "::toString(format, includedFields);\n"
				"\treturn result;\n}\n");
		} else if( membername == "isValid" ) {
			hayvalid = true;
			hmembers += "\tvirtual bool isValid(ValidResult::Context context, ValidResult *result=0); // from dbRecord\n";
			cpcpp->insert_extrusion(ext_prefix + "ISVALID",
"bool Rec" + recname + "::isValid(ValidResult::Context context, ValidResult *result )\n"
"{\n"
"\tbool ret = " + baserecordtype + "::isValid(context, result);", "\treturn ret;\n}\n");
		} else if( membername == "isEmpty" ) {
			hmembers += "\tbool isEmpty( const Xtring &nocheck_fields = Xtring::null ) const; // from dbRecord\n";
			cpcpp->insert_extrusion(ext_prefix + "ISEMPTY",
"bool Rec" + recname + "::isEmpty( const Xtring &nocheck_fields ) const\n"
"{\n", "\treturn false;\n}\n");
		} else if( membername == "save" ) {
			hmembers += "\tvirtual bool save(bool saverelated) throw( dbError ); // from dbRecord\n";
			cpcpp->insert_extrusion(ext_prefix + "SAVE",
"bool Rec" + recname + "::save(bool saverelated) throw( dbError )\n"
"{\n",
				"\tbool ret = " + baserecordtype + "::save(saverelated);\n\treturn ret;\n}\n");
		} else if( membername == "remove" ) {
			hmembers += "\tvirtual bool remove() throw( dbError ); // from dbRecord\n";
			cpcpp->insert_extrusion(ext_prefix + "REMOVE",
"bool Rec" + recname + "::remove() throw( dbError )\n"
"{\n",
				"\tbool ret = " + baserecordtype + "::remove();\n\treturn ret;\n}\n");
        } else if( membername == "read" ) {
			hmembers += "\tvirtual int read(const Xtring &where, dbRecord *master=0); // from dbRecord\n";
			cpcpp->insert_extrusion(ext_prefix + "READ",
"int Rec" + recname + "::read(const Xtring &where, dbRecord *master)\n"
"{\n",
				"\tint ret = " + baserecordtype + "::read(where, master);\n\treturn ret;\n}\n");
        } else if( membername == "beforeSaveOrDeleteRelated" ) {
			hmembers += "\tvirtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving); // from dbRecord\n";
			cpcpp->insert_extrusion(ext_prefix + "BEFORESAVEORDELETERELATED",
"void Rec" + recname + "::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)\n"
"{\n",	"}\n");
        } else if( membername == "findMatchingRecord" ) {
			haytostring = true;
			hmembers +=
				"\tvirtual bool findMatchingRecord(dbRecordID *matchingid) const; // from dbRecord\n";
			cpcpp->insert_extrusion(ext_prefix + "FINDMATCHINGRECORD",
				"bool Rec" + recname + "::findMatchingRecord(dbRecordID *matchingid) const\n"
				"{\n"
				"\tbool found;\n",
				"\tfound = " + baserecordtype + "::findMatchingRecord(matchingid);\n"
				"\treturn found;\n}\n");
		} else if( membername == "getDetalles" ) {
			hmembers +=
			 	"\tclass Rec" + recname + "Detalle *getRecDetalle(uint ndetail) const;\n"
				"\tvoid addDetalle(const class Rec" + recname + "Detalle *detail);\n"
				"\tdbRecordList *getDetalles() const;\n";
			cpcpp->insert_extrusion(ext_prefix + "GETDETALLES",
				"dbRecordList *Rec" + recname + "::getDetalles() const\n"
				"{\n"
				"\treturn findRelationByRelatedTable( \"" + recname.upper() + "DETALLE\" )->getRelRecordList( this );\n"
				"}\n"
				"\n"
				"void Rec" + recname + "::addDetalle(const Rec" + recname + "Detalle * detail)\n"
				"{\n"
				"\tgetDetalles()->addRecord( detail );\n"
				"}\n"
				"\n"
				"Rec" + recname + "Detalle *Rec" + recname + "::getRecDetalle(uint ndetail) const\n"
				"{\n"
				"\treturn static_cast<Rec" + recname + "Detalle *>(findRelationByRelatedTable( \""
					+ recname.upper() + "DETALLE\" )->getRelRecord(this, ndetail));\n"
				"}\n");
		}
	}
	cph->insert_extrusion_at(cph->position_before_extrusion( ext_prefix + "CONSTRUCTOR"),
		ext_prefix + "INCLUDES",
		"#include <dbappdbapplication.h>\n"
		+ Xtring(hayvalid?"#include <gongdbvalidresult.h>\n":"")
		+ Xtring(haytostring?"#include <gongregexp.h>":"")
		+ "\n" + hincludes, namespaces );
	if( relations.size() )
		cph->insert_extrusion(ext_prefix + "RELATIONS", hrelations);
	if( members.size() )
		cph->insert_extrusion(ext_prefix + "MEMBERS", hmembers, "\n}; // class\n" );
	cph->addHeaderProtect( fullrecname );
}


void adddbFieldDefinition(CppModule *cpcpp, CppModule *cph, const Xtring &fullfldname,
                          const XtringList &members, const Xtring &inherit, const Xtring &_args)
{
	Xtring fldname = cph->extractClassname( fullfldname );
	Xtring _namespace = cph->extractNamespace( fullfldname );
	Xtring ext_prefix = "DBFIELD_" + fldname.upper() + "_";
	Xtring fnameprefix = FileUtils::basename(  cph->getFilename() );
	fnameprefix = fnameprefix.left( fnameprefix.find("dbfld") );
	Xtring namespaces = "\nnamespace gong {\n" + ( _namespace == "" ? "" : "namespace " + _namespace + " {\n") + "\n";
	bool staticValues = (_args.lower().find("static") != Xtring::npos);
	bool notConstr = (_args.lower().find("noconstr") != Xtring::npos);
	bool fullConstr = (_args.lower().find("fullconstr") != Xtring::npos);
	Xtring parent = (inherit.isEmpty() ? "dbFieldDefinition" : inherit );
	Xtring preclassname = ((_namespace == "" || _namespace == "gong" ) ? "dbField" : "Fld" );

	Xtring::size_type ltpos = parent.find("<");
	Xtring includes;
	if( ltpos != Xtring::npos )
		includes = "#include <gong" + parent.lower().left(ltpos) + ".h>\n";
	else
		includes = "#include <gong" + parent.lower() + ".h>\n";
	cph->insert_extrusion(ext_prefix + "INCLUDES", includes,
	                      namespaces +
	                      "\n"
						  "class " + preclassname + fldname + ": public " + parent + "\n"
						  "{\n"
						  "public:\n" );

	if( !notConstr ) {
		if( fullConstr ) {
			cph->insert_extrusion(ext_prefix + "CONSTRUCTOR",
"\t" + preclassname + fldname + "(const Xtring &tablename, const Xtring &name, SqlColumnType sqltype,\n"
"\t\tunsigned short int width, unsigned short int decimals = 0,\n"
"\t\tdbFieldDefinition::Flags flags = dbFieldDefinition::NONE, const Xtring &defaultvalue = Xtring::null)\n"
"\t: " + parent + "(tablename, name, sqltype, width, decimals, flags, defaultvalue)\n"
"\t{}\n\n");
		} else {
			cph->insert_extrusion(ext_prefix + "CONSTRUCTOR",
"\t" + preclassname + fldname + "(const Xtring &tablename, const Xtring &name,\n"
"\t\tdbFieldDefinition::Flags flags = dbFieldDefinition::NONE,\n"
"\t\tconst Xtring &defaultvalue = Xtring::null)\n",
"\t: " + parent + "(tablename, name, sqltype, width, decimals, flags, defaultvalue)\n"
"\t{}\n\n");
		}
	}

	Xtring hmembers;
	cpcpp->insert_extrusion(ext_prefix + "INCLUDES",
	                        "#include \"" + fnameprefix + ".h\"\n",
	                        namespaces );
	for( XtringList::const_iterator it = members.begin();
		it != members.end();
		++ it ) {
		Xtring membername = (*it);
		if( membername == "isValid" ) {
			hmembers += "\tvirtual bool isValid( dbRecord *r, dbFieldValue *value,\n"
						"\t\tValidResult::Context context, ValidResult *integres) const; // From dbFieldDefinition\n";
			cpcpp->insert_extrusion(ext_prefix + "ISVALID",
"bool " + preclassname + fldname + "::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const\n"
"{\n", "\treturn true;\n}\n");
		} else if( membername == "customDefaultValue" ) {
			hmembers += "\tvirtual Variant customDefaultValue() const; // from dbFieldDefinition \n";
			cpcpp->insert_extrusion(ext_prefix + "CUSTOMDEFAULTVALUE",
"Variant " + preclassname + fldname + "::customDefaultValue() const\n"
"{\n",
			                        "\tVariant defaultvalue;\n"
									"\treturn defaultvalue;\n}\n");
        }
	}
	Xtring values;
	if( parent.startsWith( "dbFieldListOfValues" ) ) {
		XtringList tokens;
		parent.tokenize( tokens, "<>" );
		if( tokens.size() < 1 )
			throw std::runtime_error( "dbFieldListOfValues: missing <ValueT>" );
		Xtring type = tokens[1];
		values += "public:\n\t";
		if( staticValues )
			values += "static ";
		values +="List<" + type + "> mValues;\n";
	}
	if( !values.isEmpty() )
		cph->insert_extrusion( ext_prefix + "VALUES", values );

	if( !hmembers.isEmpty() )
		cph->insert_extrusion(ext_prefix + "MEMBERS", hmembers, "\n}; // class\n" );

	cph->addHeaderProtect( fullfldname );
}


}; // Namespace capel

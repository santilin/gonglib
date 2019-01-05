#include <gongdebug.h>
#include <gongfileutils.h>
#include "capelrtkmodule.h"

using namespace gong;

namespace capel {

extern const char *lgpllicense;

void RtkModule::addCopyLeft(const Xtring &project, const Xtring &desc,
	const Xtring &license)
{
	Xtring text;
	text += "// ";
	text += "@file " + gong::FileUtils::filename(mFilename) + " " + desc + "\n";
	text += "// " + project + "\n";
	if( license.length() == 0 )
		text += lgpllicense;
	else
		text += license;
	insert_extrusion_at(0, "COPYLEFT", text);
}


void RtkModule::addDefinition(const Xtring &_type, const Xtring &_repname,
                              const Xtring &_reptitle, const XtringList &fields,
                              const Xtring &args)
{
	insert_extrusion( "REPORT_DEFINITION",
		"\tTitle = \"" + _reptitle + "\";\n"
		"\tUnits = chars;\n"
		"\tSizeX = 100;\n"
		"\tSizeY = 60;\n" );
	Xtring headers[10], footers[10];
	Xtring details[10], groupvalues[10], input;
	Xtring pageheader, pagefooter;

	double posx = 0.0;
	int oldlevel = -1, maxlevels = 0;
	Xtring oldsection;
	bool nopageheader = args.upper().find("NOPAGEHEADER") != Xtring::npos;
	for( XtringList::const_iterator it = fields.begin();
		it != fields.end();
		++ it ) {
		XtringList props;
		bool isheader, isfooter;
		(*it).tokenize( props, "|" );
		_GONG_DEBUG_ASSERT( props.size() == 4 || props.size() == 5);
		Xtring fullfldname = props[0];
		Xtring fldcaption = props[1];
		if( fldcaption.isEmpty() )
			fldcaption = "= const @DDD." + Xtring(fullfldname).replace(".","_")  + "_CAPTION;";
		Xtring fldtype = props[2];
		double fldsize = props[3].toDouble();
		Xtring slevel = (props.size() > 4 ) ? props[4] : "d0";
		if( slevel != oldsection ) {
			posx = 0;
			oldsection = slevel;
		}
		int level = slevel.mid(1).toInt();
		bool ispageheader = slevel == "ph";
		bool ispagefooter = slevel == "pf";
		isheader = (slevel[0] == 'h');
		isfooter = (slevel[0] == 'f');
		if( level != oldlevel )
			posx = 0.0;
		oldlevel = level;
		if( level > maxlevels ) {
			maxlevels = level;
			posx = 0.0;
			groupvalues[level] = fullfldname;
		}
		Xtring tablename;
		Xtring fldname = fullfldname;
		Xtring::size_type pos = fullfldname.find(".");
		Xtring object_name = Xtring(fullfldname).replace(".", "_");
		if( pos != Xtring::npos ) {
			fldname = fullfldname.mid(pos + 1);
			tablename = fullfldname.left( pos );
		}

		input += "\t\tINPUTFIELD " + object_name + " {\n"
			"\t\t\tSource = " + fullfldname + ";\n"
			"\t\t\tType = " + fldtype + ";\n"
			"\t\t}\n";
		if( isheader ) {
			headers[level] += "\t\tOBJECT header_" + object_name + " {\n"
					"\t\t\tvalue = \"" + fldcaption + "\";\n"
				  	"\t\t\tposx = " + Xtring::number(posx) + ";\n"
				  	"\t\t\tsizex = " + Xtring::number(fldsize) + ";\n";
			if( fldtype.lower() == "money" || fldtype.lower() == "double" || fldtype.lower() == "int" )
				headers[level] += "\t\t\thalignment = \"right\";\n";
			headers[level] += "\t\t}\n";
		}
		if( isfooter ) {
			footers[level] += "\t\tOBJECT footer_" + object_name + " {\n"
					"\t\t\tvalue = \"" + fldcaption + "\";\n"
				  	"\t\t\tposx = " + Xtring::number(posx) + ";\n"
				  	"\t\t\tsizex = " + Xtring::number(fldsize) + ";\n";
			if( fldtype.lower() == "money" || fldtype.lower() == "double" || fldtype.lower() == "int" )
				footers[level] += "\t\t\thalignment = \"right\";\n";
			footers[level] += "\t\t}\n";
		}
		if( ispageheader ) {
			pageheader += "\t\tOBJECT pageheader_" + object_name + " {\n"
					"\t\t\tvalue = \"" + fldcaption + "\";\n"
				  	"\t\t\tposx = " + Xtring::number(posx) + ";\n"
				  	"\t\t\tsizex = " + Xtring::number(fldsize) + ";\n";
			if( fldtype.lower() == "money" || fldtype.lower() == "double" || fldtype.lower() == "int" )
				pageheader += "\t\t\thalignment = \"right\";\n";
			pageheader += "\t\t}\n";
		}
		if( ispagefooter ) {
			pagefooter += "\t\tOBJECT pagefooter_" + object_name + " {\n"
					"\t\t\tvalue = \"" + fldcaption + "\";\n"
				  	"\t\t\tposx = " + Xtring::number(posx) + ";\n"
				  	"\t\t\tsizex = " + Xtring::number(fldsize) + ";\n";
			if( fldtype.lower() == "money" || fldtype.lower() == "double" || fldtype.lower() == "int" )
				pagefooter += "\t\t\thalignment = \"right\";\n";
			pagefooter += "\t\t}\n";
		}
		if( !ispageheader && !ispagefooter ) {
			details[level] += "\t\tOBJECT " + object_name + " {\n";
			if( fldtype.lower() == "money" || fldtype.lower() == "double" || fldtype.lower() == "int" )
				details[level] += "\t\t\thalignment = \"right\";\n";
			details[level] +=
					  "\t\t\tvalue = \"=input." + Xtring(fullfldname).replace(".", "_") + ";\";\n"
					  "\t\t\tposx = " + Xtring::number(posx) + ";\n"
					  "\t\t\tsizex = " + Xtring::number(fldsize) + ";\n"
					  "\t\t}\n";
		}
		posx += fldsize + 1;
	}
	insert_extrusion( "INPUT_DEFAULT", input, "\t}\n", "\tINPUT default {\n\t\tdriver = ;\n");
	if( ! nopageheader ) {
		if( pageheader.isEmpty() ) {
			insert_extrusion( "PAGE_HEADER", "\tInclude = \"default_pageheader.inc\";", "}\n");
		} else {
			insert_extrusion( "PAGE_HEADER",
				"\tPAGEHEADER page_header {\n"
				"\t\tsizey = 4;\n"
				"\t\tstyle = pageheader;\n"
				"\t\tbackgroundimage = logo;\n"
			    "\n" + pageheader + "\n", "\t}\n");
		}
	}
	if( maxlevels == 0 ) {
		if( ! headers[0].isEmpty() ) {
			insert_extrusion( "PAGE_HEADER_0",
				"\tPAGEHEADER page_header_0 {\n"
				"\t\tsizey=1;\n"
				"\t\tfontweight = bold;\n"
#if 0
				"\t\tbordertopstyle = solid ;\n"
				"\t\tbordertopcolor = black ;\n"
				"\t\tbordertopwidth = 1;\n"
#endif
				"\t\tborderbottomstyle = solid ;\n"
				"\t\tborderbottomcolor = black ;\n"
				"\t\tborderbottomwidth = 2;\n" + headers[0], "\t}\n\n");
		}
		if( ! footers[0].isEmpty() ) {
			insert_extrusion( "PAGE_FOOTER_0",
				"\tPAGEFOOTER page_footer_0 {\n"
				"\t\tsizey=1;\n"
				"\t\tfontweight = bold;\n"
#if 0
				"\t\tbordertopstyle = solid ;\n"
				"\t\tbordertopcolor = black ;\n"
				"\t\tbordertopwidth = 1;\n"
#endif
				"\t\tborderbottomstyle = solid ;\n"
				"\t\tborderbottomcolor = black ;\n"
				"\t\tborderbottomwidth = 2;\n" + footers[0], "\t}\n\n");
		}
		insert_extrusion( "DETAILS",
			"\tDETAILS details {\n"
			"\t\tstyle = detail;\n"
			"\t\tsizey = 1;\n"
			"\t\n"
			+ details[0], "\t}\n\n" );
	} else for( int nl = maxlevels; nl >= 0; --nl ) {
		if( nl == 0 ) { // Cuando hay más de un grupo, los detalles no se indentan
			if( !headers[nl].isEmpty() ) {
				insert_extrusion( "GROUP_HEADER_CAPTIONS_" + Xtring::number(nl),
					"\tGROUPHEADER group_header_captions_" + Xtring::number(nl) + " {\n"
					"\t\tposx = " + Xtring::number( maxlevels * 2 ) + ";\n"
					"\t\tsizey=1;\n"
					"\t\tfontweight = bold;\n"
#if 0
					"\t\tbordertopstyle = solid ;\n"
					"\t\tbordertopcolor = black ;\n"
					"\t\tbordertopwidth = 1;\n"
#endif
					"\t\tborderbottomstyle = solid ;\n"
					"\t\tborderbottomcolor = black ;\n"
					"\t\tborderbottomwidth = 2;\n"
//					"\t\tvalue= \"=input." + Xtring(groupvalues[nl]).replace(".", "_") + ";\";\n"
					"\t\tlevel = 1;\n"
					+ headers[nl], "\t}\n\n" );
			}
			if( !footers[nl].isEmpty() ) {
				insert_extrusion( "GROUP_FOOTER_CAPTIONS_" + Xtring::number(nl),
					"\tGROUPFOOTER group_footer_captions_" + Xtring::number(nl) + " {\n"
					"\t\tposx = " + Xtring::number( maxlevels * 2 ) + ";\n"
					"\t\tsizey=1;\n"
					"\t\tfontweight = bold;\n"
#if 0
					"\t\tbordertopstyle = solid ;\n"
					"\t\tbordertopcolor = black ;\n"
					"\t\tbordertopwidth = 1;\n"
#endif
					"\t\tborderbottomstyle = solid ;\n"
					"\t\tborderbottomcolor = black ;\n"
					"\t\tborderbottomwidth = 2;\n"
//					"\t\tvalue= \"=input." + Xtring(groupvalues[nl]).replace(".", "_") + ";\";\n"
					"\t\tlevel = 1;\n"
					+ footers[nl], "\t}\n\n" );
			}
			insert_extrusion( "DETAILS_" + Xtring::number(nl),
				"\tDETAILS details {\n"
				"\t\tposx = " + Xtring::number( maxlevels * 2 ) + ";\n"
				"\t\tlevel = " + Xtring::number(nl) + ";\n"
				"\t\tstyle = detail;\n"
				"\t\tsizey = 1;\n"
				"\n"
				+ details[nl], "\t}\n\n" );
		} else {
			if( !headers[nl].isEmpty() ) {
				insert_extrusion( "GROUP_HEADER_CAPTIONS_" + Xtring::number(nl),
					"\tGROUPHEADER group_header_captions_" + Xtring::number(nl) + " {\n"
					"\t\tposx = " + Xtring::number( (maxlevels - nl) * 2 ) + ";\n"
					"\t\tsizey=1;\n"
					"\t\tfontweight = bold;\n"
#if 0
					"\t\tbordertopstyle = solid ;\n"
					"\t\tbordertopcolor = black ;\n"
					"\t\tbordertopwidth = 1;\n"
#endif
					"\t\tborderbottomstyle = solid ;\n"
					"\t\tborderbottomcolor = black ;\n"
					"\t\tborderbottomwidth = 2;\n"
					"\n"
					"\t\tvalue= \"=input." + Xtring(groupvalues[nl]).replace(".", "_") + ";\";\n"
					"\t\tlevel=" + Xtring::number( nl ) + ";\n"
					+ headers[nl], "\t}\n\n" );
			}
			if( !footers[nl].isEmpty() ) {
				insert_extrusion( "GROUP_FOOTER_CAPTIONS_" + Xtring::number(nl),
					"\tGROUPFOOTER group_footer_captions_" + Xtring::number(nl) + " {\n"
					"\t\tposx = " + Xtring::number( (maxlevels - nl) * 2 ) + ";\n"
					"\t\tsizey=1;\n"
					"\t\tfontweight = bold;\n"
#if 0
					"\t\tbordertopstyle = solid ;\n"
					"\t\tbordertopcolor = black ;\n"
					"\t\tbordertopwidth = 1;\n"
#endif
					"\t\tborderbottomstyle = solid ;\n"
					"\t\tborderbottomcolor = black ;\n"
					"\t\tborderbottomwidth = 2;\n"
					"\n"
					"\t\tvalue= \"=input." + Xtring(groupvalues[nl]).replace(".", "_") + ";\";\n"
					"\t\tlevel=" + Xtring::number( nl ) + ";\n"
					+ footers[nl], "\t}\n\n" );
			}
			insert_extrusion( "GROUP_DETAILS_"+ Xtring::number(nl),
				"\tGROUPHEADER group_details_" + Xtring::number(nl) + " {\n"
				"\t\tstyle = detail;\n"
				"\t\tsizey = 1;\n"
				"\t\tposx = " + Xtring::number( (maxlevels - nl) * 2 ) + ";\n"
				"\n"
				"\t\tvalue= \"=input." + Xtring(groupvalues[nl]).replace(".", "_") + ";\";\n"
				"\t\tlevel=" + Xtring::number(nl) + ";\n"
				+ details[nl], "\t}\n\n" );
		}
	}
	insert_extrusion( "PAGE_FOOTER",
		"\tPAGEFOOTER page_footer {\n"
		"\t\tsizey = 1;\n"
		"\t\tstyle = pagefooter;\n"
		"\n" + pagefooter + "\n", "\t}\n");

}



}


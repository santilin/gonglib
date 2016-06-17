#include <iostream>
#include <cstdlib>
#include <gongdebug.h>
#include <gongfileutils.h>
#include <gongdbrecord.h>
#include <gongdbfieldlistofvalues.hpp>
#include "capelrtkmodule.h"
#include "gonggenfrmeditrecmaestro.h"
#include "gonggenfrmeditbehavior.h"
#include "gonggenrecords.h"
#include "gonggenerators.h"
#include "gonggendbmodule.h"

using namespace capel;
using namespace gong;


void genCppClass( CapelModule *cppm, CapelModule *hm, const Xtring &classname, const Xtring &members )
{}

#define GONGLIB_COPYRIGHT "Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres"

void test()
{
	CppModule modcpp("/tmp/capel.cpp");
	modcpp.insert_extrusion_at(-1, "INICIO", "El texto del fin");
	modcpp.insert_extrusion_at(-1, "FIN", "El texto del fin");
	modcpp.insert_extrusion_at(-1, "DESPUESFIN", "El texto después del fin");
	modcpp.write();
	exit(0);
}

gong::dbRecord* gong::dbRecord::duplicate()
{
	throw std::runtime_error( "dbRecord::duplicate");
}


namespace capel {


extern const char *shortlgpllicense;

void genrtk();
#if HAS_VENTADIRECTA == 1
void genventadirecta();
#endif
}

/**
 * Generates code from files with CAPEL extrusions.
 * Files recognized are cpp and rtk.
 *
 * @param argv[] List of files to process
 *
 * Each input file must have a MODULE_INFO extrusion.
 * Each line in this section has a special meaning:
 *   COPYLEFT text
 *      Creates a COPYLEFT extrusion in both the .h and .cpp files, with text as description
 *
 *   TYPE FrmEditRecMaster|FrmEditRecDetails|class|dbRecord|GongModule|dbFieldDefinition|FrmEditRecBehavior
 			[namespace::]classname flags
 *      Must be the last line in the extrusion due to the way in which this main function reads it
 *
 *   TYPE GongModule modulename namespace
 *      * RECORD [namespace::]recname [FrmEditRecMaster] [[~]menuname|-] inherit
 *        If menuname starts with ~, it is not added to the main menu bar
 *      * MODULE_REQUIRED modulename
 *      * MODULE_OPTIONAL modulename
 * 	    * NAMESLISTTABLE tablename
 *
 * 	 TYPE dbRecord namespace::recname flags
 *  	* MEMBER   init|clear|toString|fromString|validate|isEmpty|save|remove|read|findMatchingRecord|getDetalles
 *		* RELATION relname [Detalles] [conditional]
 *		* INTERFACE ifacename [Detalles] [conditional]
 *		* INHERIT  recname
 *
 *   TYPE FrmEditRecMaster|FrmEditRecDetail|FrmEditRec(parent) \
 			[namespace::]recname [INCCODE|VALIDCODEANDDESC|VALIDCODE|CODENOTFOUND]
 *		* FIELD nombrecampo tipo [parent|- => pControlsFrame] [layout|- => pControlsLayout] [FLAGS] [conditional]
 *        * TIPO: DATETIME|DATE|TIME|STRING|INT|MONEY|DOUBLE|BOOL|COMBOSTRING|COMBOINT|REFERENCE
 					|FRMEDITRECMASTER|FRMEDITRRECDETAIL|HSPACER|VSPACER
 *			* REFERENCE(TABLENAME,FLDCODE,FLDDESC,SEEKCODEFLAGS)
 *        * FLAGS: EDITABLE - for COMBOSTRING and COMOBOINT
 *      * MEMBER [actTotales]
 *		* INTERFACE ifacename [Detalles]
 *
 *   TYPE FrmEditRecBehavior [namespace::]recname
 *		* FIELD nombrecampo tipo [parent|- => pControlsFrame] [layout|- => pControlsLayout] [FLAGS]
 *        * TIPO: DATETIME|DATE|TIME|STRING|INT|MONEY|DOUBLE|BOOL|COMBOSTRING|COMBOINT|REFERENCE
 					|FRMEDITRECMASTER|FRMEDITRRECDETAIL|HSPACER|VSPACER
 *			* REFERENCE(TABLENAME,FLDCODE,FLDDESC,SEEKCODEFLAGS)
 *        * FLDFLAGS: EDITABLE - for COMBOSTRING and COMOBOINT
 *      * FLAGS: NoCreateRecord
 *
 *	 TYPE dbFieldDefinition name [NoConstr|FullConstr|Static]
 *      * MEMBER isValid|customDefaultValue
 *      * INHERIT dbFieldClass

 *   TYPE SimpleReport [NoPageHeader]
 *      * FIELD TABLENAME.FIELD | caption | type | width | section_type
 *        * TYPE: int|date|money|string|datetime
 *        * SECTIONTYPE: ph->pageheader, h[0-9] -> header, f[0-9] -> footer, d-> detail
 *
 * CONFIGURE.AC
 *
 *   TYPE Module modulename [AC_CHECK_MACRO]
 *   TYPE Project appname [path-to-gonglib]
 */

int main( int argc, char *argv[] )
{
#ifdef _GONG_DEBUG
	::__gong_debug::_gong_debug_level = 0;
#endif
	if( argc == 2 && strcasecmp(argv[1], "rtk") == 0 ) {
		capel::genrtk();
		return 0;
	}
#ifdef HAS_VENTADIRECTA
	if( argc == 2 && strcasecmp(argv[1], "ventadirecta") == 0 ) {
		capel::genventadirecta();
		return 0;
	}
#endif
	if ( argc < 2 ) {
		std::cout << "Use: capel filename" << std::endl;
		return 0;
	}
	//	std::cout << "capel: working on " << FileUtils::path(argv[1]) << std::endl;
	for ( int nfile = 1; nfile < argc ; nfile ++ ) {
		const char *filename = argv[nfile]; //Xtring(argv[ nfile ]).replace("file://","").c_str();
/****************************
	RTK FILES
*****************************/
		if ( FileUtils::extension( filename ) == "rtk" ) {
			RtkModule modrtk( filename );
			Xtring mod_info = modrtk.get_extrusion_text( "MODULE_INFO" );
			XtringList info_instructions, rtkfields;
			Xtring reptitle;
			mod_info.tokenize( info_instructions, "\n" );
			for ( XtringList::const_iterator it = info_instructions.begin();
					it != info_instructions.end();
					++ it ) {
				Xtring line = ( *it ).mid( 3 ).trim(); // Skip "// "
				Xtring instruction = line.left( line.find_first_of( " \t" ) ).upper();
				Xtring args, type;
				if ( line.find_first_of( " \t" ) != Xtring::npos )
					args = line.mid( line.find_first_of( " \t" ) + 1 ).trim();
				if ( instruction == "COPYLEFT" ) {
					reptitle = args;
					modrtk.addCopyLeft( GONGLIB_COPYRIGHT, args );
				} else if ( instruction == "TYPE" ) {
					type = args.left( args.find_first_of( " \t" ) );
					args = args.mid( args.find_first_of( " \t" ) + 1 );
					if ( type == "SimpleReport" ) {
						Xtring repname = args.left( args.find_first_of( " \t" ) );
						args = args.mid( args.find_first_of( " \t" ) + 1 );
						modrtk.addDefinition( type, repname, reptitle, rtkfields, args);
					} else {
						_GONG_DEBUG_WARNING( Xtring( "Tipo no reconocido " + type ) );
					}
				} else if ( instruction == "FIELD" ) {
					rtkfields.push_back( args );
				}
			}
			modrtk.writeIfModified();
		} else if ( FileUtils::extension( filename ) == "ac" ) {
/***************************
	CONFIGURE.AC
***************************/
			CapelModule *configure_ac = new CapelModule( filename, "<<<<<", ">>>>>", "dnl ", "\n" );
			Xtring mod_info = configure_ac->get_extrusion_text( "MODULE_INFO" );
			if ( mod_info.isEmpty() ) {
				std::cout << "capel: " << filename << " does not contain a MODULE_INFO extrusion" << std::endl;
			} else {
				XtringList info_instructions, members;
				Xtring copyleft_text, type;
				mod_info.tokenize( info_instructions, "\n" );
				for ( XtringList::const_iterator it = info_instructions.begin();
				        it != info_instructions.end();
				        ++ it ) {
					Xtring line = ( *it ).mid( 4 ).trim(); // Skip "dnl "
					Xtring instruction = line.left( line.find_first_of( " \t" ) ).upper();
					Xtring args;
					if ( line.find_first_of( " \t" ) != Xtring::npos )
						args = line.mid( line.find_first_of( " \t" ) + 1 ).trim();
					if ( instruction == "COPYLEFT" ) {
						copyleft_text = args;
					} else if ( instruction == "MODULE" ) {
						members.push_back( args );
					} else if ( instruction == "TYPE" ) {
						type = args.left( args.find_first_of( " \t" ) ).upper();
						args = args.mid( args.find_first_of( " \t" ) + 1 );
						if ( type == "PROJECT" ) {
							genModuleConfigure_ac(configure_ac, members, args + " PROJECT");
							configure_ac->writeIfModified();
							genMakefileAm("Makefile.am", members, args + " PROJECT");
							if( !FileUtils::exists("Makefile.cvs") ) {
								system("mkdir m4 src po; touch NEWS README AUTHORS ChangeLog");
								genMakefileCvs();
							}
							if( !FileUtils::exists("src/Makefile.am") )
								genSrc_MakefileAm(type, args);
							genModuleCpp(args, members);
						} else if ( type == "MODULE" ) {
							genModuleConfigure_ac(configure_ac, members, args + " MODULE");
							configure_ac->writeIfModified();
							genMakefileAm("Makefile.am", members, args + " MODULE");
							if( !FileUtils::exists("Makefile.cvs") ) {
								system("mkdir m4 src po; touch NEWS README AUTHORS ChangeLog");
								genMakefileCvs();
								genSrc_MakefileAm(type, args);
								genModuleCpp(args, members);
							}
						}

					}
				}
			}
			delete configure_ac;
		} else {
/****************************
	REST OF FILES
*****************************/
			CppModule *modcpp = new CppModule( filename );
			Xtring mod_info = modcpp->get_extrusion_text( "MODULE_INFO" );
			delete modcpp;
			if ( mod_info.isEmpty() ) {
				std::cout << "capel: " << filename << " does not contain a MODULE_INFO extrusion" << std::endl;
			} else {
				Xtring copyleft_text, type, recname, classname, classprops, inherit;
				XtringList members, methods, info_instructions, records, fields, implementations,
					tests, relations, modules_required, modules_optional, interfaces, nameslisttables;
				mod_info.tokenize( info_instructions, "\n" );
				for ( XtringList::const_iterator it = info_instructions.begin();
				        it != info_instructions.end();
				        ++ it ) {
					Xtring line = ( *it ).mid( 3 ).trim(); // Skip "// "
					Xtring instruction = line.left( line.find_first_of( " \t" ) ).upper();
					Xtring args;
					if ( line.find_first_of( " \t" ) != Xtring::npos )
						args = line.mid( line.find_first_of( " \t" ) + 1 ).trim();
					if ( instruction == "COPYLEFT" ) {
						copyleft_text = args;
					} else if ( instruction == "FIELD" ) {
						fields.push_back( args );
					} else if ( instruction == "MEMBER" ) {
						members.push_back( args );
					} else if ( instruction == "MODULE_REQUIRED" ) {
						modules_required.push_back( args );
					} else if ( instruction == "MODULE_OPTIONAL" ) {
						modules_optional.push_back( args );
					} else if ( instruction == "NAMESLISTTABLE" ) {
						nameslisttables.push_back( args );
					} else if ( instruction == "RELATION" ) {
						relations.push_back( args );
					} else if ( instruction == "INHERIT" ) {
						inherit = args;
					} else if ( instruction == "INTERFACE" ) {
						interfaces.push_back( args );
					} else if ( instruction == "METHOD" ) {
						methods.push_back( args );
					} else if ( instruction == "RECORD" ) {
						records.push_back( args );
					} else if ( instruction == "TEST" ) {
						tests.push_back( args );
					} else if ( instruction == "IMPLEMENTATION" ) {
						implementations.push_back( args );
					} else if ( instruction == "TYPE" ) {
						type = args.left( args.find_first_of( " \t" ) );
						args = args.mid( args.find_first_of( " \t" ) + 1 );
						if ( type == "FrmEditRecBehavior" ) {
							recname = args.left( args.find_first_of( " \t" ) );
							classprops = args.mid( args.find_first_of( " \t" ) + 1 );
							CppModule modcpp( filename );
// 							modcpp.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text );
							CppModule modh( FileUtils::path( filename ) + FileUtils::basename( filename ) + ".h" );
							modh.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text, shortlgpllicense );
							addFrmEditBehavior( &modcpp, &modh, type, recname, fields, classprops, inherit );
							addFrmEditBehaviorFields( &modcpp, &modh, type, recname, fields, members, classprops, inherit);
							modcpp.writeIfModified();
							modh.writeIfModified();
						} else if ( type.startsWith("FrmEdit") ) {
							recname = args.left( args.find_first_of( " \t" ) );
							classprops = args.mid( args.find_first_of( " \t" ) + 1 );
							CppModule modcpp( filename );
// 							modcpp.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text );
							CppModule modh( FileUtils::path( filename ) + FileUtils::basename( filename ) + ".h" );
							modh.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text, shortlgpllicense );
							addFrmEditRecMaster( &modcpp, &modh, type, recname, fields, classprops, inherit, interfaces );
							addFrmEditRecMasterFields( &modcpp, &modh, type, recname, fields, members, classprops, inherit);
							modcpp.writeIfModified();
							modh.writeIfModified();
						} else if ( type == "Class" ) {
							classname = args.left( args.find_first_of( " \t" ) );
							classprops = args.mid( args.find_first_of( " \t" ) + 1 );
							CppModule modcpp( filename );
// 							modcpp.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text );
							CppModule modh( FileUtils::path( filename ) + FileUtils::basename( filename ) + ".h" );
							modh.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text, shortlgpllicense );
							modh.addClassDefinition( classname, members, classprops );
							modcpp.writeIfModified();
							modh.writeIfModified();
						} else if ( type == "dbRecord" ) {
							classname = args.left( args.find_first_of( " \t" ) );
							classprops = args.mid( args.find_first_of( " \t" ) + 1 );
							CppModule modcpp( filename );
// 							modcpp.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text );
							CppModule modh( FileUtils::path( filename ) + FileUtils::basename( filename ) + ".h" );
							modh.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text, shortlgpllicense );
							addRecordDefinition( &modcpp, &modh, classname, classprops, members, relations, inherit, interfaces );
							modcpp.writeIfModified();
							modh.writeIfModified();
						} else if ( type == "GongModule" ) {
							classname = args.left( args.find_first_of( " \t" ) );
							classprops = args.mid( args.find_first_of( " \t" ) + 1 );
							CppModule modcpp( filename );
// 							modcpp.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text );
							CppModule modh( FileUtils::path( filename ) + FileUtils::basename( filename ) + ".h" );
							modh.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text, shortlgpllicense );
							addModuleDefinition( &modcpp, &modh, classname, records,
												 modules_required, modules_optional, nameslisttables );
							modcpp.writeIfModified();
							modh.writeIfModified();
						} else if ( type == "dbFieldDefinition" ) {
							Xtring fieldname = args.left( args.find_first_of( " \t" ) );
							args = args.mid( args.find_first_of( " \t" ) + 1 );
							CppModule modcpp( filename );
// 							modcpp.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text );
							CppModule modh( FileUtils::path( filename ) + FileUtils::basename( filename ) + ".h" );
							modh.addCopyLeft( GONGLIB_COPYRIGHT, copyleft_text, shortlgpllicense );
							adddbFieldDefinition( &modcpp, &modh, fieldname, members, inherit, args );
							modcpp.writeIfModified();
							modh.writeIfModified();
						} else {
							std::cout << "Tipo no reconocido " << type << std::endl;
						}
					}
				}
			}
		}
	}
}

#if 0
typedef std::vector PageCollection;
typedef int Page;
typedef double VisualRank;

VisualRank GenerateVisualRank( double visualfactor, PageCollection pages )
{
	VisualRank vr = 0;
	for( Page p = 0; p < pages.size(); ++p ) {
		PageCollection relatedPages = pages.GetRelatedPages( p );
		vr += GenerateVisualRank( relatedPages ) + (visualfactor * VisualRank( p ));
	}
}

#endif
#ifndef _CAPEL_GENERALGENERATOR_H
#define _CAPEL_GENERALGENERATOR_H

/**
 * @author santilin<santi@programaciosocial.net>
 */
#include <gongdbtabledefinition.h>

using namespace gong;

namespace capel {

class GeneralGenerator
{
public:
	
	enum CppFlags {
		NONE					= 0x0000,
		NO_SET					= 0x0001,
		NO_SET_IMPL             = 0x0002,
		NO_GET					= 0x0004,
		NO_GET_IMPL             = 0x0008,
		CONSTRUCTOR_ARG			= 0x0010,
		SET_CONST_REFERENCE     = 0x0020,
		GET_CONST_REFERENCE     = 0x0040,
		CONST_REFERENCE         = SET_CONST_REFERENCE | GET_CONST_REFERENCE,
		NULLABLE                = 0x0080,
		PROPERTY_NONULL 		= 0x0100,
		PROPERTY 				= PROPERTY_NONULL | NULLABLE
	};

	enum FrameWork { Yii };
	enum Language { CPP, PHP };
	enum TipoModulo { MVCModel, MVCController, MVCView };

	typedef struct {
		const char *pModuleName;
		Language mLanguage;
		TipoModulo mTipo;
		const char *pRecordClass;
		const char *pTableName;
		bool mUsesDatabase;
		const char *pFormFields;
	} ModuleDefinition;	

	typedef struct {
		FrameWork mFrameWork;
		const char *pName;
		const char *pDestPath;
		const char *pDatabaseName;
		const char *pDatabaseUser;
		const char *pDatabasePassword;
		const char *pDatabaseHost;
		const char *pPackageDocBlock;
		ModuleDefinition *mModulos;
	} ProgramDefinition;

	GeneralGenerator( const ProgramDefinition &pd );
	int generate();
	int generateAllRelations();
	int generateMVC( const ModuleDefinition &md );
	int generateYiiMVC( const ModuleDefinition &md, const dbTableDefinition &tbldef);
	int generateTablaTipoViews( const ModuleDefinition &md );
	int generateInformedRecordViews( const ModuleDefinition &md, const dbTableDefinition &tbldef );
	
protected:
	const ProgramDefinition &mProgramDefinition;
	dbConnection *pConn;
	dbDefinition *pDatabase;
	Dictionary<dbRelationDefinition *> mAllRelations;
};

}
#endif

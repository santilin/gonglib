#include <gongdebug.h>
#include <gongdbconnection.h>
#include <gongdbfielddefinition.h>
#include <gongdbtabledefinition.h>
#include <gongdbdefinition.h>
#include <gongfileutils.h>
#include "capelphpmodule.h"
#include "generalgenerator.h"

/**
 * @todo extraer las relaciones del mysql y generarlas aquí
 */

using namespace gong;

namespace capel {

int GeneralGenerator::generateAllRelations()
{
	if( mAllRelations.size() )
		return mAllRelations.size();
	for( dbTableDefinitionDict::const_iterator it = pDatabase->getTables().begin();
		it != pDatabase->getTables().end(); ++it ) {
		dbTableDefinition *tbldef = it->second;
		for( dbRelationDefinitionDict::const_iterator it = tbldef->getRelationDefinitions().begin();
			it!=tbldef->getRelationDefinitions().end(); ++ it ) {
			dbRelationDefinition *reldef = it->second;
//  			_GONG_DEBUG_PRINT( 0, reldef->getFullName() );
// 			_GONG_DEBUG_PRINT( 0, reldef->getName() );
			if( reldef->getType() == dbRelationDefinition::many2many ) {
				// pluralizar relationname
				;
			} else {
				Xtring className = PhpModule::modelize( reldef->getLeftTable() );
				Xtring refTable = reldef->getRightTable(); // Table name that current fk references to
				Xtring refKey= reldef->getRightField();   // Key in that table being referenced
				Xtring refClassName = PhpModule::modelize( refTable );
				Xtring relationname( reldef->getLeftField() );
				if( relationname.startsWith("id_") ) 
					relationname = relationname.mid(3);
				// Añade la relación para esta tabla
				relationname = PhpModule::modelize( relationname ).unproper();
				std::map<Xtring, Xtring> relationcodes = mAllRelations[ tbldef->getName() ];
				relationcodes[relationname] = "array(self::BELONGS_TO, '" + refClassName + "', '" + reldef->getLeftField() + "')";
//				_GONG_DEBUG_PRINT(0, relationname + " => " + relationcodes[ relationname ] );
				mAllRelations[ tbldef->getName() ] = relationcodes;
				// Añade la relación para la otra tabla
				dbRelationDefinition::Type t = tbldef->getFldIDName() == reldef->getLeftField() 
					? dbRelationDefinition::one2one : dbRelationDefinition::one2many;

				relationname = reldef->getLeftTable();
				if( relationname.startsWith("id_") ) 
					relationname = relationname.mid(3);
				relationname = PhpModule::modelize( relationname ).unproper();
				if( t == dbRelationDefinition::one2many )
					relationname.append("s");
				relationcodes = mAllRelations[reldef->getRightTable() ];
				relationcodes[relationname] = Xtring( "array(self::" )
					+ (t == dbRelationDefinition::one2one ? "HAS_ONE" : "HAS_MANY" ) 
					+ ", '" + className + "', '" + reldef->getLeftField() + "')";
// 				_GONG_DEBUG_PRINT(0, "Relations[" + reldef->getRightTable() + "] = '" + relationname + "' => " + relationcodes[ relationname ] );
				mAllRelations[reldef->getRightTable()] = relationcodes;
			}
		}
	}
	return mAllRelations.size();
}
	
	
const char *record_search_begin =
"\t/**\n"
"\t * Retrieves a list of models based on the current search/filter conditions.\n"
"\t *\n"
"\t * Typical usecase:\n"
"\t * - Initialize the model fields with values from filter form.\n"
"\t * - Execute this method to get CActiveDataProvider instance which will filter\n"
"\t * models according to data in model fields.\n"
"\t * - Pass data provider to CGridView, CListView or any similar widget.\n"
"\t *\n"
"\t * @return CActiveDataProvider the data provider that can return the models\n"
"\t * based on the search/filter conditions.\n"
"\t */\n"
"\tpublic function search()\n"
"\t{\n"
"\t	// @todo Please modify the following code to remove attributes that should not be searched.\n"
"\t\n"
"\t	$criteria=new CDbCriteria;\n"
"\t\n";
const char *record_search_end =
"\t\n"
"\t	return new CActiveDataProvider($this, array(\n"
"\t		'criteria'=>$criteria,\n"
"\t	));\n"
"\t}\n"
"\n";

	
int GeneralGenerator::generateYiiMVC(const GeneralGenerator::ModuleDefinition& md, const dbTableDefinition& tbldef)
{
	Xtring fields_comments, relations_comments, relations_code;
	Xtring search_rules, field_labels, required_fields, integer_fields, safe_fields,
		nullable_fields, numerical_fields, unique_fields, criteria_compare;
	Dictionary< Xtring >lengths;
	
	for( uint i = 0; i < tbldef.getFieldCount(); ++i ) {
		const dbFieldDefinition *flddef = tbldef.getFieldDefinition(i);
		// class comments
		Xtring phpfldtype = PhpModule::getPHPTypeFromMysqlType( flddef->getSqlColumnType() );
		fields_comments += " * @property " + phpfldtype + " $" + flddef->getName() + "\n";
		if( flddef->getName() == "created_at" || flddef->getName() == "updated_at"
			|| flddef->getName() == "created_by" || flddef->getName() == "updated_by" )
			continue;
		// rules
		if( !search_rules.isEmpty() )
			search_rules += ", ";
		search_rules += flddef->getName();
		if( !flddef->isSequence() ) {
			if( flddef->isUnique() )
				unique_fields.appendWithSeparator( flddef->getName(), ",");
			bool required = !flddef->canBeNull() && flddef->getDefaultValue().isEmpty();
			if( required ) {
// 				_GONG_DEBUG_PRINT(0, flddef->getFullName() + " is required");
				required_fields.appendWithSeparator( flddef->getName(), "," );
			}
			else if( flddef->canBeNull() /*&& flddef->getDefaultValue() == "NULL"*/ ) {
				nullable_fields.appendWithSeparator( flddef->getName(), ",");
			}
			if( phpfldtype == "integer" ) {
				integer_fields.appendWithSeparator( flddef->getName(), ",");
			} else if( phpfldtype == "double" ) {
				numerical_fields.appendWithSeparator( flddef->getName(), ",");
			} else if( phpfldtype == "string" && flddef->getSqlWidth() > 0 ) {
				Xtring slength = Xtring::number( flddef->getSqlWidth() );
				if( lengths[ slength ].isEmpty() )
					lengths.insert( slength, flddef->getName()  );
				else
					lengths.set( slength, lengths[slength] + "," + flddef->getName());
			} else if( !flddef->isPrimaryKey() ) {
				safe_fields.appendWithSeparator( flddef->getName(), ",");
			}
		} 
			
		Xtring human_label = flddef->getName();
		if( human_label.startsWith("id_") ) {
			Xtring model = PhpModule::modelize( human_label.mid(3) );
			human_label = model + "::getStaticDescSingular()";
		} else 
			human_label = "'" + human_label.proper().replace("_"," ") + "'";
		
		
		field_labels += "\t\t\t'" + flddef->getName() + "' => " + human_label + ",\n";
		criteria_compare += "\t\t$criteria->compare('" + flddef->getName() + "',$this->" + flddef->getName();
		if( phpfldtype == "string" )
			criteria_compare += ",true";
		criteria_compare += ");\n";
	}

	std::map<Xtring, Xtring> relations = mAllRelations[tbldef.getName()];
	for( Dictionary<Xtring>::const_iterator relit = relations.begin();
		relit != relations.end(); ++relit ) {
		Xtring relation_code = relit->second;  // $array(self::BELONG_TO, 'Cliente', 'id_cliente_conyuge')
		XtringList rc_parts;
		relation_code.tokenize( rc_parts, "'" );
		Xtring relation_model = rc_parts[1];
		if( relation_code.find("HAS_MANY") != Xtring::npos )
			relations_comments += " * @property " + relation_model + "[] $" + relit->first + "\n";
		else
			relations_comments += " * @property " + relation_model + " $" + relit->first + "\n";
		relations_code += "\t\t\t'" + relit->first + "' => " + relit->second + ",\n";
	}

	if( !relations_comments.isEmpty() )
		relations_comments = "\n * Relaciones del modelo:\n" + relations_comments;

	Xtring class_comments = 
"/* Columnas disponibles en la tabla '" + Xtring(md.pTableName) + "':\n";
	Xtring real_info = 
"\t\t'desc_singular' => '" + Xtring(md.pModuleName) + "',\n"
"\t\t'desc_plural' => '" + md.pModuleName + "s',\n"
"\t\t'code_field' => 'id',\n"
"\t\t'desc_field' => 'nombre',\n"
"\t\t'order_field' => 'nombre',\n"
"\t\t'female' => 'false',\n"
"\t);\n"
"\n";

	if( md.mTipo & MVCModel ) {
		PhpModule *phpmodel = new PhpModule( Xtring(mProgramDefinition.pDestPath) + "/protected/models/" + md.pModuleName + ".php");

		phpmodel->insert_extrusion_at(0, "CLASS_DEFINITION", 
								   class_comments + fields_comments + relations_comments + " */\n"  +
"class " + md.pModuleName + " extends hmbasemvc\\" + md.pRecordClass + "\n"
"{\n"
"/* Rellena este array con información del registro, las claves son:\n"
"\tdesc_singular, desc_plural, desc_field, code_field, order_field, female, title, subtitle */\n"
"\tstatic protected $real_info = Array(\n",
// after
real_info,
// before								   
"<?php\n"
"/**\n"
" * Clase modelo para la tabla \"" + Xtring(md.pTableName) + "\".\n"
+ mProgramDefinition.pPackageDocBlock +
" */\n"
"\n" ); 
	
	Xtring construction = 
"\t/**\n"
"\t * @return string el nombre de la tabla asociada de la base de datos\n"
"\t */\n"
"\tpublic function tableName()\n"
"\t{\n"
"\t\treturn '" + Xtring(md.pTableName) + "';\n"
"\t}\n"
"\n"
"\t/**\n"
"\t * Returns the static model of the specified AR class.\n"
"\t * Please note that you should have this exact method in all your CActiveRecord descendants!\n"
"\t * @param string $className active record class name.\n"
"\t * @return " + Xtring(md.pModuleName) + " the static model class\n"
"\t */\n"
"\tpublic static function model($className=__CLASS__)\n"
"\t{\n"
"\t	return parent::model('" + Xtring(md.pModuleName) + "');\n"
"\t}\n"
"\n"
+ record_search_begin + criteria_compare + record_search_end;

	phpmodel->insert_extrusion( "CONSTRUCTION", construction );

	phpmodel->insert_extrusion_at( phpmodel->position_after_extrusion("CONSTRUCTION"),
								   "RELATIONS", relations_code,
								   "\t\t);\n"
								   "\t\t// Añade o modifica aquí las relaciones\n"
								   "\t}\n",
"\n"
"\t/**\n"
"\t * @return array relaciones del modelo.\n"
"\t */\n"
"\tpublic function relations()\n"
"\t{\n"
"\t\treturn array(\n" );
	
	Xtring rules = 
"\t/**\n"
"\t * @return array reglas de validación para los atributos del modelo.\n"
"\t */\n"
"\tpublic function rules()\n"
"\t{\n"
"\t\t// ATENCIÓN: Define reglas solo para los atributos que recibirán datos por parte del usuario.\n"
"\t\t$ret = array(\n";

	if( !required_fields.isEmpty() )
		required_fields = "\t\t\tarray('" + required_fields + "', 'required'),\n";
	if( !nullable_fields.isEmpty() )
		nullable_fields = "\t\t\tarray('" + nullable_fields + "', 'default', 'setOnEmpty' => true, 'value' => null),\n";
	if( !integer_fields.isEmpty() )
		integer_fields = "\t\t\tarray('" + integer_fields + "', 'numerical', 'integerOnly'=>true),\n";
	if( !numerical_fields.isEmpty() )
		numerical_fields = "\t\t\tarray('" + numerical_fields + "', 'required'),\n";
	if( !unique_fields.isEmpty() )
		unique_fields = "\t\t\tarray('" + unique_fields + "', 'unique'),\n";
	
	Xtring length_rules;
	for( Dictionary< Xtring >::const_iterator it = lengths.begin();
		it != lengths.end(); ++ it ) {
		length_rules += Xtring("\t\t\tarray('") + it->second + "', 'length', 'max'=>" + it->first + "),\n";
	}
	if( !safe_fields.isEmpty() )
		safe_fields = "\t\t\tarray('" + safe_fields + "', 'safe'),\n";
		
	phpmodel->insert_extrusion( "RULES", 
					rules + unique_fields + required_fields + integer_fields + numerical_fields 
					+ length_rules + nullable_fields + safe_fields +
"\t\t\t// La siguiente regla la usa search().\n"
"\t\t\t// @todo Elimina los atributos por los que no se quiere buscar.\n"
"\t\t\tarray('" + search_rules +" ', 'safe', 'on'=>'search'),\n",
"\t\t);\n"
"\t\t// Añade o modifica tus reglas aquí\n"
"\t\treturn $ret;\n"
"\t}\n"
"\n");

	phpmodel->insert_extrusion( "LABELS", 							 
"\t/**\n"
"\t * @return array etiquetas personalizadas de los atributos (name=>label)\n"
"\t */\n"
"\tpublic function attributeLabels()\n"
"\t{\n"
"\t\t$ret = array(\n"
	+ Xtring(field_labels) +
"\t	);\n",
"\t\t/* Modifica las etiquetas a tu gusto aquí */\n"
"\t\treturn $ret;\n"
"\t}\n"
"} // class\n"
"\n");
		phpmodel->writeIfModified();
	}
	
	if( md.mTipo & MVCController ) {
		PhpModule *phpcontroller = new PhpModule( Xtring(mProgramDefinition.pDestPath) + "/protected/controllers/" + md.pModuleName + "Controller.php");
		phpcontroller->insert_extrusion_at( 0, "BASIC",
Xtring(
"class ") + Xtring(md.pModuleName) + "Controller extends \\hmbasemvc\\AdminController\n"
"{\n"
"\tpublic function loadModel($id = 0, $search = NULL, $id_master = 0)\n"
"\t{\n"
"\t\tif ($id == 0) {\n"
"\t\t\tif ($search) {\n"
"\t\t\t\t$model = new " + Xtring(md.pModuleName) + "($search);\n"
"\t\t\t} else {\n"
"\t\t\t\t$model = new " + Xtring(md.pModuleName) + ";\n"
"\t\t\t}\n"
"\t\t} else {\n"
"\t\t\t$model = " + Xtring(md.pModuleName) + "::model()->findByPk((int)$id);\n"
"\t\t}\n"
"\t\tif($model===null) {\n"
"\t\t\tthrow new CHttpException(404,'No se pudo crear el modelo " + Xtring(md.pModuleName) + "');\n"
"\t\t}\n",

"\t\treturn $model;\n"
"\t}\n"
"}\n"
"\n", 

"<?php\n"
"/**\n"
" * " + Xtring(md.pModuleName) + "Controller.php hmbasemvc\\AdminController para el modelo " + md.pModuleName + "\n"
+ mProgramDefinition.pPackageDocBlock +
" */\n"
"\n" ); 
	
		phpcontroller->writeIfModified();
	}
	
	if( md.mTipo & MVCView ) {
		if( Xtring(md.pRecordClass) == "TablaTipoRecord" ) {
			generateTablaTipoViews( md );
		} else if( Xtring(md.pRecordClass) == "InformedRecord" ) {
			generateInformedRecordViews( md, tbldef );
		}
	}
}
	
	
int GeneralGenerator::generateTablaTipoViews(const GeneralGenerator::ModuleDefinition& md)
{
	Xtring mv_path = Xtring(mProgramDefinition.pDestPath) + "/protected/views/" + Xtring(md.pModuleName).unproper();
	if( !FileUtils::isDir( mv_path.c_str() ) )
		FileUtils::makePath( mv_path );
	PhpModule *phpviewadmin = new PhpModule( mv_path + "/_admin.php");

	phpviewadmin->insert_extrusion_at( 0, "BASIC",
"$this->widget('hmbasemvc.widgets.grid.GridView', array(\n"
"\t'id'=> get_class($model) . '-grid',\n"
"\t'dataProvider'=>$model->search(),\n"
"\t'filter'=>$model,\n"
"\t'columns'=>array(\n"
"\t\tarray( 'name' => 'nombre', 'class' => 'MainColumn' ),\n",
	   
"\t\t// Añade el resto de columnas aquí\n"
"\t\tarray( 'class'=>'ButtonColumn' ),\n"
"\t)\n"
"));\n",

"<?php\n"
"/**\n"
" * " + Xtring(md.pModuleName).unproper() + "/_admin.php vista de administración para el modelo " + md.pModuleName + "\n"
+ mProgramDefinition.pPackageDocBlock +
" */\n" 
"\n" ); 
	phpviewadmin->writeIfModified();
	
	PhpModule *phpviewformfields = new PhpModule( mv_path + "/_form_fields.php");
	phpviewformfields->insert_extrusion_at( 0, "BASIC",
"\techo $form->textControl(\"nombre\");\n", 
	   Xtring::null, 
"<?php\n"
"/**\n"
" * " + Xtring(md.pModuleName).unproper() + "/_form_fields.php formulario para el modelo " + md.pModuleName + "\n"
+ mProgramDefinition.pPackageDocBlock + 
" */\n" 
"\n" ); 		
	phpviewformfields->writeIfModified();
}
	

int GeneralGenerator::generateInformedRecordViews(const GeneralGenerator::ModuleDefinition& md, 
												  const dbTableDefinition &tbldef )
{
	
	XtringList form_fields;
	Xtring admin_fields;
	Xtring md_form_fields( md.pFormFields );
	md_form_fields.tokenize( form_fields, "," );
	Dictionary<Xtring> controls_code;
	for( uint i = 0; i < tbldef.getFieldCount(); ++i ) {
		const dbFieldDefinition *flddef = tbldef.getFieldDefinition(i);
		Xtring fldname( flddef->getName() );
		if( !form_fields.contains( fldname ) )
			continue;
		Xtring control_code;
		Xtring phpfldtype( PhpModule::getPHPTypeFromMysqlType( flddef->getSqlColumnType() ) );
		if( flddef->isReference() || flddef->getName().startsWith("id_tipo_") ) {
			dbRelationDefinition *reldef = flddef->findRelationDefinition( tbldef );
			Xtring relation_model;
			if( reldef )
				relation_model = PhpModule::modelize( reldef->getRightTable() );
			else
				relation_model = PhpModule::modelize( flddef->getName().mid(3) );
			// 1:1 relations
			control_code += 
"\techo $form->listControl('" + flddef->getName() + "',\n"
"\t\tCHtml::listData(" + relation_model + "::model()->findAll(array('order' => " + relation_model + "::getOrderField())), \n"
"\t\t\t'id', " + relation_model + "::getDescField()));\n";
		} else {
			if( flddef->getOrigDDL().startsWith("enum") ) {
				control_code += "\techo $form->enumControl('" + flddef->getName() + "');\n";
			} else if( flddef->getOrigDDL().upper().startsWith("TEXT") ) {
				control_code += "\techo $form->textAreaControl('" + flddef->getName() + "');\n";
			} else if( phpfldtype == "string" || phpfldtype == "integer" ) {
				control_code += "\techo $form->textControl('" + flddef->getName() + "');\n";
			} else if( phpfldtype == "bool" ) {
				control_code += "\techo $form->checkControl('" + flddef->getName() + "');\n";
			} else if( phpfldtype == "date" ) {
				control_code += "\techo $form->dateControl('" + flddef->getName() + "');\n";
			} else if( phpfldtype == "datetime" ) {
				control_code += "\techo $form->dateTimeControl('" + flddef->getName() + "');\n";
			} else if( phpfldtype == "float" ) {
				if( flddef->getOrigDDL().upper().startsWith("DECIMAL") )
					control_code += "\techo $form->eurosControl('" + flddef->getName() + "');\n";
				else
					control_code += "\techo $form->floatControl('" + flddef->getName() + "');\n";
			} else {
				_GONG_DEBUG_WARNING( phpfldtype + " no se reconoce como tipo de campo" );
			}
		}
		// Almaceno el código indexado por nombre del campo para luego sacarlo en orden
		controls_code.insert( fldname, control_code );
		admin_fields.appendWithSeparator( "\t\t'" + fldname + "'", ",\n" );
	}
	// Sacar el código en orden
	Xtring all_controls_code;
	for( XtringList::const_iterator form_fld_it = form_fields.begin();
		form_fld_it!=form_fields.end(); ++form_fld_it ) {
		all_controls_code += controls_code[*form_fld_it];
	}
	
	
	Xtring mv_path = Xtring(mProgramDefinition.pDestPath) + "/protected/views/" + Xtring(md.pModuleName).unproper();
	if( !FileUtils::isDir( mv_path.c_str() ) )
		FileUtils::makePath( mv_path );
	PhpModule *phpviewadmin = new PhpModule( mv_path + "/_admin.php");

	phpviewadmin->insert_extrusion_at( 0, "BASIC",
"$this->widget('hmbasemvc.widgets.grid.GridView', array(\n"
"\t'id'=> get_class($model) . '-grid',\n"
"\t'dataProvider'=>$model->search(),\n"
"\t'filter'=>$model,\n"
"\t'columns'=>array(\n",
"\t\t// Añade o elimina el resto de columnas aquí\n"
+ admin_fields + ",\n"
"\t\tarray( 'class'=>'ButtonColumn' ),\n"
"\t)\n"
"));\n",

"<?php\n"
"/**\n"
" * " + Xtring(md.pModuleName).unproper() + "/_admin.php vista de administración para el modelo " + md.pModuleName + "\n"
+ mProgramDefinition.pPackageDocBlock +
" */\n" 
"\n" ); 
	phpviewadmin->writeIfModified();

	
	mv_path = Xtring(mProgramDefinition.pDestPath) + "/protected/views/" + Xtring(md.pModuleName).unproper();
	if( !FileUtils::isDir( mv_path.c_str() ) )
		FileUtils::makePath( mv_path );
	PhpModule *phpviewformfields = new PhpModule( mv_path + "/_form_fields.php");
	phpviewformfields->insert_extrusion( "BASIC", all_controls_code );
	phpviewformfields->writeIfModified();
}
	
	
int GeneralGenerator::generateMVC(const GeneralGenerator::ModuleDefinition& md)
{
	if( md.mUsesDatabase ) {
		dbTableDefinition *tbldef = pDatabase->findTableDefinition( md.pTableName );
		if( !tbldef )
			return 0;
		generateYiiMVC( md, *tbldef );
	}
}
	
int GeneralGenerator::generate()
{
	if( strlen(mProgramDefinition.pDatabaseName) != 0 ) {
		pConn = new dbConnection();
		if( !pConn->connect( dbConnection::DRIVER_MYSQL, mProgramDefinition.pDatabaseUser, 
				mProgramDefinition.pDatabasePassword, mProgramDefinition.pDatabaseName, mProgramDefinition.pDatabaseHost ) ) {
			_GONG_DEBUG_WARNING( "No se puede conectar con la base de datos" );
		}
		pDatabase = dbDefinition::fromSQLSchema( pConn, mProgramDefinition.pDatabaseName );
		generateAllRelations();
	}
	ModuleDefinition *md = mProgramDefinition.mModulos;
	while( md->pTableName ) {
		generateMVC( *md );
		md++;
	}
}

GeneralGenerator::GeneralGenerator(const GeneralGenerator::ProgramDefinition& pd)
	: mProgramDefinition( pd )
{
}

	
}


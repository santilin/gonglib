/*<<<<<MODULE_INFO*/
// COPYLEFT Root of the data dictionary
// MEMBER Name Xtring rwc
// MEMBER Description Xtring rwc
// MEMBER Encoding Xtring rwc 0
// MEMBER Collation Xtring rwc 0
// MEMBER Tables dbTableDefinitionsList rw
// MEMBER Views dbViewDefinitionsList rw
// MEMBER FieldStyles dbFieldStylesList rw
// TYPE Class dbDefinition
/*>>>>>MODULE_INFO*/

/**
	@class gong::dbDefinition
	@brief Root of the data dictionary

	The dbDefinition, dbTableDefinition, dbFieldDefinition, dbFieldStyle, dbIndexDefinition,
	dbRelationDefinition and dbViewDefinition classes form the data dictionary for your application.
	The data dictionary is an in-memory tree structure that mirrors that of your database storage engine.

	You define your data dictionary by creating a dbDefinition object and populating it with table, view
	and field style definitions. This can be achieved in several ways
	- Create dbTableDefinitions and dbViewDefinitions and add them with addTable
	- Read dbViewDefinitions from a file with addViewsFromPath or addViewsFromFile (obsolete)
    - Read dbViewDefinitions from the application config with addViewsFromConfig
	- Create the data dictionary out of an existing database in a SQL server with fromSQLSchema.

	Note that you can not create a data dictionary directly from a text file. It must by created
	programmatically. Nevertheless, you can complete the locale dependent information, as table and fields
	captions, styles and descriptions from a file, using readDescriptionsFromPath and readDescriptionsFromFile.

	Once you have created your data dictionary, you can:
	- Look for the different elements with get and find accessor functions.
	- Create the SQL database with create, createTables and createIndexes
	- Compare your in-memory definition with another one and get a set of SQL statements to convert
		one to the other
	- Drop the database or indexes with drop and dropIndexes
	- Work with your database engine creating dbRecord objects

	@see dbTableDefinition dbFieldStyle dbViewDefinition dbRecord
*/

#include <fstream>
#include <memory>

#include "gonggettext.h"
#include "gongdebug.h"
#include "gongfileutils.h"
#include "gongdbconnection.h"
#include "gongdbdefinition.h"
#include "gongdbrecord.h"

namespace gong {

// create database cauac_db default charset utf8 collate utf8_spanish_ci
dbDefinition::dbDefinition(const Xtring& name, const Xtring& description,
                           const Xtring& encoding, const Xtring& collation)
    : mName(name), mDescription(description),
      mEncoding(IConv::canonicalCodeName(encoding.c_str())), mCollation(collation)
{
}


dbDefinition *dbDefinition::fromSQLSchema( dbConnection *conn, const Xtring &dbname,
        const XtringList &tables )
{
    _GONG_DEBUG_ASSERT( conn );
    Xtring tblname;
    dbDefinition *dbdef = 0;

	dbdef = new dbDefinition( dbname, "Generated from Schema" );
	std::auto_ptr<dbResultSet> rsTables(conn->select( "SHOW TABLES" ));
	while( rsTables->next() ) {
		tblname = rsTables->toString(0);
		dbTableDefinition *tbldef = dbTableDefinition::fromSQLSchema( conn, *dbdef, tblname );
		dbdef->addTable( tbldef );
	}
    return dbdef;
}

bool dbDefinition::create( dbConnection *conn, const Xtring &extraargs,
                           bool ignoreerrors )
{
    try {
        conn->exec( "CREATE DATABASE " + getName() +
                    (mEncoding.isEmpty() ? "" : " CHARACTER SET '" + (mEncoding == "UTF-8" ? "UTF8" : mEncoding) + "'")
                    + " " + extraargs );
        conn->selectDatabase( getName() );
    } catch( dbError dbe ) {
        if( ignoreerrors && conn->getLastError().getNumber() == 1007 ) {
            conn->selectDatabase( getName() );
            return true;
        }
        if( !ignoreerrors )
            throw;
        return false;
    }
    return true;
}

bool dbDefinition::createTables( dbConnection *conn,
                                 const Xtring &extraargs, bool ifnotexists, bool ignoreerrors )
{
    for( dbTableDefinitionsList::const_iterator it = mTables.begin();
            it != mTables.end();
            ++it ) {
        dbTableDefinition *table = (*it).second;
        try {
            table->create( conn, ifnotexists, extraargs, ignoreerrors );
            table->createIndexes( conn, ignoreerrors );
        } catch ( dbError e) {
            _GONG_DEBUG_ERROR(e.what());
            if( !ignoreerrors )
                throw;
        }
    }
    return true;
}

bool dbDefinition::createIndexes( dbConnection *conn, bool ignoreerrors )
{
    for( dbTableDefinitionsList::const_iterator it = mTables.begin();
            it != mTables.end();
            ++it )
    {
        dbTableDefinition *table = (*it).second;
        try {
            table->createIndexes( conn, ignoreerrors );
        } catch ( dbError e) {
            _GONG_DEBUG_ERROR(e.what());
            if( !ignoreerrors )
                throw;
        }
    }
    return true;
}

bool dbDefinition::dropIndexes( dbConnection *conn, bool removeall, bool ignoreerrors )
{
    for( dbTableDefinitionsList::const_iterator it = mTables.begin();
            it != mTables.end();
            ++it )
    {
        dbTableDefinition *table = (*it).second;
        try {
            table->dropIndexes( conn, removeall, ignoreerrors );
        } catch ( dbError e) {
            _GONG_DEBUG_ERROR(e.what());
            if( !ignoreerrors )
                throw;
        }
    }
    return true;
}


/**
	Comprueba si esta definición de base de datos coincide (está incluida exactamente) en la otra.
	Normalmente, se crea esta definición de la base de datos con código c++ y la otra se lee del servidor SQL
	con fromSQLSchema().

		pDatabase = new dbDefinition();
		...
		... completar la base de datos que se desea usar en el programa
		...
		dbDefinition *oldschema = dbDefinition::fromSQLSchema( getConnection(), pDatabase->getName() );
		Xtring diff = pDatabase->sameSQLSchema( oldschema, getConnection() );

	El valor devuelto, diff, contiene una cadena con las instrucciones SQL necesarias para igualar la otra base de datos,
	la que está en el servidor, con la que hemos definido en la aplicación.

	Para actualizar la base de datos del servidor, se puede usar:

		XtringList querys;
		diff.tokenize( querys, ";\n" );
		getConnection()->exec( querys );

	De este modo, cuando actualizamos el programa, o añadimos algn plugin, la estructura de la base de datos puede ser actualizada automáticamente, aunque no se actualiza ningn dato

	Cuando una tabla que hemos definido en el programa no existe en la base de datos, la cadena devuelta contiene
	# CREATE TABLE new_table
	como comentario para que no se ejecute si actualizamos. Deberemos crear las tablas que faltan llamando a createTables.

 */

Xtring dbDefinition::sameSQLSchema( const dbDefinition *other, dbConnection *conn, bool purging ) const
{
    Xtring ret;
    for( unsigned int nt = 0; nt < getTables().size(); nt++ ) {
        const dbTableDefinition *othertabledef = other->findTableDefinition( getTableDefinition(nt)->getName() );
        if( othertabledef )
            ret += getTableDefinition(nt)->sameSQLSchema( othertabledef, conn, purging );
        else
            ret += "# CREATE TABLE " + getTableDefinition(nt)->getName() + "\n";
    }
    return ret;
}


dbFieldDefinition *dbDefinition::findFieldDefinition(const Xtring &tablename,
        const Xtring &fldname, bool warning )
{
    dbTableDefinition *tbldef = findTableDefinition( tablename, warning );
    if( tbldef ) {
        if( fldname.find(".") != Xtring::npos ) {
            Xtring fname, tname;
            fldname.splitIn2( tname, fname, ".");
            return findFieldDefinition( tname, fname, warning );
        } else
            return tbldef->findFieldDefinition( fldname, warning );
    } else {
        if( warning ) {
            _GONG_DEBUG_WARNING(Xtring::printf("Table for field '%s.%s' not found",
                                               tablename.c_str(), fldname.c_str()));
        }
        return 0;
    }
}

const dbFieldDefinition *dbDefinition::findFieldDefinition(const Xtring &tablename,
        const Xtring &fldname,
        bool warning ) const
{
    const dbTableDefinition *tbldef = findTableDefinition( tablename, warning );
    if( tbldef ) {
        if( fldname.find(".") != Xtring::npos ) {
            Xtring fname, tname;
            fldname.splitIn2( tname, fname, ".");
            return findFieldDefinition( tname, fname, warning );
        } else
            return tbldef->findFieldDefinition( fldname, warning );
    } else {
        if( warning ) {
            _GONG_DEBUG_WARNING(Xtring::printf("Table for field '%s.%s' not found",
                                               tablename.c_str(), fldname.c_str()));
        }
        return 0;
    }
}

dbFieldDefinition *dbDefinition::findFieldDefinition(const Xtring &fullfldname, bool warning)
{
    Xtring tablename = fullfldname.left( fullfldname.find('.') );
    dbTableDefinition *tbldef = findTableDefinition( tablename );
    if( tbldef ) {
        Xtring fldname = fullfldname.mid( fullfldname.find('.') + 1 );
        return tbldef->findFieldDefinition( fldname, warning );
    } else {
        if( warning ) {
            _GONG_DEBUG_WARNING(Xtring::printf("Table not found for field '%s'",
                                               fullfldname.c_str()));
        }
        return 0;
    }
}

const dbFieldDefinition *dbDefinition::findFieldDefinition(const Xtring &fullfldname,
        bool warning) const
{
    Xtring tablename = fullfldname.left( fullfldname.find('.') );
    const dbTableDefinition *tbldef = findTableDefinition( tablename.trim() );
    if( tbldef ) {
        Xtring fldname = fullfldname.mid( fullfldname.find('.') + 1 );
        return tbldef->findFieldDefinition( fldname.trim(), warning );
    } else {
        if( warning ) {
            _GONG_DEBUG_WARNING(Xtring::printf("Table not found for field '%s'",
                                               fullfldname.c_str()));
        }
        return 0;
    }
}

dbFieldStyle *dbDefinition::findFieldStyle( const Xtring &stylename) const
{
    if( stylename.isEmpty() )
        return 0;
    else {
        _GONG_DEBUG_PRINT(5, "Searching style " + stylename + " in " + mFieldStyles.toString() );
        return mFieldStyles[stylename];
    }
}


/**
	Comprueba si un registro es o no usado en una tabla relacionada con esta
	Se usa para comprobar a la hora de borrar que es seguro borrar
 */
bool dbDefinition::isRecordUsed(dbRecord *rec, Xtring *usingtable)
{
    Xtring tablename = rec->getTableName();
    dbRecordID recid = rec->getRecordID();
    for( unsigned int nt=0; nt < mTables.size(); nt ++ ) {
        // Recorrer las tablas de la base de datos
        dbTableDefinition *tbldef = mTables[nt];
        if( tbldef->getName().upper() != tablename.upper() ) {
            // Recorrer las relaciones de esa tabla
            for( unsigned int nrel=0; nrel < tbldef->getRelationDefinitions().size(); nrel++ ) {
                dbRelationDefinition *reldef = tbldef->getRelationDefinitions()[nrel];
                if( reldef->getRightTable().upper() == tablename.upper() ) { // \ojo Comprobar si enabled?
                    if( rec->getConnection()->selectInt("SELECT ID FROM " + rec->getConnection()->nameToSQL( reldef->getLeftTable() ) +
                                                        " WHERE " + rec->getConnection()->nameToSQL( reldef->getLeftField() ) + " = " + rec->getConnection()->toSQL( recid ) ) != 0 ) {
                        *usingtable = reldef->getLeftTable();
                        return true;
                    }
                }
            }
        }
    }
    usingtable->clear();
    return false;
}

int dbDefinition::readDescriptionsFromPath( const Xtring &language, const Xtring &paths, const Xtring &globalpath )
{
    _GONG_DEBUG_PRINT(2, "paths=" + paths );
    int ndds = 0;
    XtringList ddspaths;
    paths.tokenize( ddspaths, ":" );
    for( XtringList::const_iterator itpath = ddspaths.begin();
            itpath != ddspaths.end();
            ++ itpath ) {
        if( !(*itpath).isEmpty() ) {
            Xtring path;
            if( !(*itpath).startsWith("/") && !globalpath.isEmpty() )
                path = globalpath + *itpath;
            else
                path = *itpath;
            XtringList dds = FileUtils::getDirEntries( path );
            // First time, non-language ddd
            for( XtringList::iterator itdd = dds.begin();
                    itdd != dds.end();
                    ++itdd) {
                if( !((*itdd).startsWith("_")) && FileUtils::extension( *itdd ) == "ddd" ) {
                    _GONG_DEBUG_PRINT(4, Xtring::printf("DataDictionary: %s%s", path.c_str(), (*itdd).c_str()) );
                    ndds += readDescriptionsFromFile( path + "/" + (*itdd) );
                }
            }
            // Second time, language ddd
            for( XtringList::iterator itdd = dds.begin();
                    itdd != dds.end();
                    ++itdd) {
                if( (*itdd).startsWith( "_" + language + "_" ) && FileUtils::extension( *itdd ) == "ddd" ) {
                    _GONG_DEBUG_PRINT(4, Xtring::printf("DataDictionary: %s%s", path.c_str(), (*itdd).c_str()) );
                    ndds += readDescriptionsFromFile( path + "/" + (*itdd) );
                }
            }
        }
    }
    return ndds;
}

int dbDefinition::readDescriptionsFromFile( const Xtring &filename )
{
    std::ifstream input;
    input.open(filename.c_str(), std::ios_base::binary | std::ios_base::in);
    if ( !input.is_open() || !input.good() ) {
        perror( filename.c_str() );
        return 0;
    }

    Xtring inputString, wholeline;
    int count = 0;
    while(getline(input, inputString) != NULL)
    {
        if( inputString.endsWith("\\") )
            wholeline += inputString.mid( 0, inputString.length() - 1 );
        else {
            wholeline += inputString;
            if( !wholeline.trim().isEmpty() && !wholeline.startsWith( '#' ) ) {
                Xtring wholefldname = wholeline.left( wholeline.find( "=" ) ).trim().upper();
                Xtring value = wholeline.mid( wholeline.find("=") + 1 ).trim();
                XtringList names;
                wholefldname.tokenize( names, "." );
                if( names.size() == 3 ) {
                    Xtring &tablename = names[0];
                    Xtring &fieldname = names[1];
                    Xtring &propname = names[2];
                    dbFieldDefinition *flddef = findFieldDefinition( tablename, fieldname);
                    if( flddef ) {
                        if( propname == "CAPTION" ) {
                            flddef->setCaption( value );
                        } else if( propname == "DESCRIPTION" ) {
                            flddef->setDescription( value );
                        } else if( propname == "DEFAULT" || propname == "DEFAULTVALUE") {
                            flddef->setDefaultValue( value );
                        } else if( propname == "STYLE" ) {
                            flddef->setStyle( value );
                        } else
                            _GONG_DEBUG_WARNING(Xtring::printf("%s: property %s not found",
                                                               filename.c_str(), propname.c_str()) );
                    } else
                        _GONG_DEBUG_WARNING(Xtring::printf("%s: field %s.%s not found",
                                                           filename.c_str(), tablename.c_str(), fieldname.c_str()) );
                } else if( names.size() == 2 ) {
                    Xtring &tablename = names[0];
                    Xtring &propname = names[1];
                    dbTableDefinition *tbldef = findTableDefinition(tablename);
                    if( tbldef ) {
                        if( propname == "DESC_PLURAL" ) {
                            tbldef->setDescPlural( value );
                        } else if( propname == "DESC_SINGULAR" ) {
                            tbldef->setDescSingular( value );
                        } else if( propname == "FEMENINA") {
                            tbldef->setFemenina( value.toBool() );
                        } else
                            _GONG_DEBUG_WARNING(Xtring::printf("%s: property %s not found",
                                                               filename.c_str(), propname.c_str()) );
                    } else
                        _GONG_DEBUG_WARNING(Xtring::printf("%s: table %s not found",
                                                           filename.c_str(), tablename.c_str()) );

                } else
                    _GONG_DEBUG_WARNING(Xtring::printf("%s: missing table name, field name or property name %s",
                                                       filename.c_str(), wholefldname.c_str()) );
                count ++;
            }
            wholeline.clear();
        }
    }
    input.close();
    return count;
}

/// TODO: it might not be necessary the caption field, we can use mDescSingular
int dbDefinition::addViewFromString( const Xtring &caption, const Xtring &viewstr, const Xtring &origin)
{
    dbViewDefinition *viewdef = new dbViewDefinition(Xtring::null, *this, viewstr, origin, caption );
    _GONG_DEBUG_PRINT(4, Xtring::printf("Adding view %s from %s, caption %s:\n %s",
                                        viewdef->getName().c_str(), origin.c_str(),
                                        caption.c_str(), viewstr.c_str() ) );
    if( !caption.isEmpty() )
        viewdef->setCaption( Xtring(viewdef->getDescSingular() + " " + Xtring(caption).replace("_"," ")).proper());
    mViews.insert( viewdef->getName(), viewdef );
    return mViews.size();
}

int dbDefinition::addViewsFromPath( const Xtring &language, const Xtring &paths, const Xtring &globalpath )
{
    _GONG_DEBUG_PRINT(2, "paths=" + paths );
    int nviews = 0;
    XtringList viewspaths;
    paths.tokenize( viewspaths, ":" );
    for( XtringList::const_iterator itpath = viewspaths.begin();
            itpath != viewspaths.end();
            ++ itpath ) {
        if( !(*itpath).isEmpty() ) {
            Xtring path;
            if( !(*itpath).startsWith("/") && !globalpath.isEmpty() )
                path = globalpath + *itpath;
            else
                path = *itpath;
            XtringList views = FileUtils::getDirEntries( path );
            // First time, non-language views
            for( XtringList::iterator itview = views.begin();
                    itview != views.end();
                    ++itview) {
                if( !((*itview).startsWith("_")) && FileUtils::extension( *itview ) == "view" ) {
                    _GONG_DEBUG_PRINT(4, Xtring::printf("View: %s%s", path.c_str(), (*itview).c_str()) );
                    nviews += addViewsFromFile( path + "/" + (*itview) );
                }
            }
            // Second time, language views
            for( XtringList::iterator itview = views.begin();
                    itview != views.end();
                    ++itview) {
                if( (*itview).startsWith( "_" + language + "_" ) && FileUtils::extension( *itview ) == "view" ) {
                    _GONG_DEBUG_PRINT(4, Xtring::printf("View: %s%s", path.c_str(), (*itview).c_str()) );
                    nviews += addViewsFromFile( path + "/" + (*itview) );
                }
            }
        }
    }
    return nviews;
}

int dbDefinition::addViewsFromFile( const Xtring &filename )
{
    std::ifstream input;
    input.open(filename.c_str(), std::ios_base::binary | std::ios_base::in);
    if ( !input.is_open() || !input.good() ) {
        perror( filename.c_str() );
        return 0;
    }

    Xtring inputString, wholeline, origin;
    origin = _("From file ") + filename;
    int count = 0;
    while(getline(input, inputString) != NULL)
    {
        if( inputString.endsWith("\\") )
            wholeline += inputString.mid( 0, inputString.length() - 1 );
        else {
            wholeline += inputString;
            if( !wholeline.trim().isEmpty() && !wholeline.startsWith( '#' ) ) {
                Xtring::size_type equalpos = wholeline.find( " = " );
                dbViewDefinition *newview;
                if( equalpos == Xtring::npos )
                    newview = new dbViewDefinition(Xtring::null, *this, wholeline, origin, "Unnamed" );
                else
                    newview = new dbViewDefinition(wholeline.mid(0, equalpos ), *this,
                                                   wholeline.mid( equalpos + 3 ), origin, "Unnamed" );
                mViews.insert( newview->getName(), newview );
                count ++;
            }
            wholeline.clear();
        }
    }
    input.close();
    return count;
}

dbTableDefinitionsList &dbDefinition::addTable(const dbTableDefinition *tabledef)
{
    mTables.insert(tabledef->getName(), const_cast<dbTableDefinition *>(tabledef) );
    return mTables;
}

dbViewDefinitionsList &dbDefinition::addView( const dbViewDefinition *_view )
{
    /// \todo {refactor} Why do we need const_cast here?
    mViews.insert( _view->getName(), const_cast<dbViewDefinition *>(_view) );
    return mViews;
}

dbFieldStylesList &dbDefinition::addFieldStyle( const dbFieldStyle *_style )
{
    mFieldStyles.insert( _style->getName(), const_cast<dbFieldStyle *>(_style) );
    return mFieldStyles;
}

void dbDefinition::addStyleFromString(const Xtring& name, const Xtring& styledef )
{
    if( name.isEmpty() )
        return;
    int width = 0;
    Xtring mask, format;
    bool visible = true;
    dbFieldStyle::Alignment halign = dbFieldStyle::AlignAuto,
                            valign = dbFieldStyle::AlignAuto;
    XtringList stlattrs;
    (styledef).tokenize( stlattrs, "|" );
    for( unsigned int i = 0; i < stlattrs.size(); i++ ) {
        if( stlattrs[i].startsWith( "w:" ) ) { // Width
            width =  stlattrs[i].mid(2).toInt();
        } else if( stlattrs[i].startsWith( "f:" ) ) { // format
            // The format for bool values are separated by '|' in Formatter::format( const Variant &avalue
            // as are these values, so in the config file we use '~'
            format = stlattrs[i].mid(2);
            format = format.replace("~", "|");
        } else if( stlattrs[i].startsWith( "m:" ) ) { // mask
            mask = stlattrs[i].mid(2);
        } else if( stlattrs[i].startsWith( "v:" ) ) { // visible
            visible = stlattrs[i].mid(2).toBool();
        } else if( stlattrs[i].startsWith( "a:" ) ) { // horizontal alignment
            halign = dbFieldStyle::fromString(stlattrs[i].mid(2));
        } else if( stlattrs[i].startsWith( "va:" ) ) { // vertical alignment
            valign = dbFieldStyle::fromString(stlattrs[i].mid(3));
        }
    }
    _GONG_DEBUG_PRINT(5, Xtring::printf("Adding style %s with width=%d, format=%s, mask=%s",
                                        name.c_str(), width, format.c_str(), mask.c_str() ) );
    dbFieldStyle *fldstyle = findFieldStyle( name );
    if( fldstyle ) {
        fldstyle->setWidth( width );
        fldstyle->setVisible( visible );
        fldstyle->setFormat( format );
        fldstyle->setMask( mask );
        fldstyle->setHAlignment( halign );
        fldstyle->setVAlignment( valign );
    } else {
        addFieldStyle( new dbFieldStyle(name, width, visible, format, mask, halign, valign) );
    }
}


int dbDefinition::getViewsForTable(const Xtring &tablename, dbViewDefinitionsList &container)
{
    int count = 0;
    _GONG_DEBUG_PRINT(3, "Table: " + tablename );
    for( unsigned int i=0; i<getViews().size(); i++ ) {
        // Startswith, because the from clause may contain JOIN
        if( getViews()[i]->getFirstFrom().upper() == tablename.upper() ) {
            _GONG_DEBUG_PRINT(4, "FROM of the view: " + getViews()[i]->getFirstFrom().upper() );
            if( !getViews()[i]->getName().upper().startsWith( tablename.upper() + "._" ) ) { // Some views are hidden
                container.insert( getViews()[i]->getName(), getViews()[i] );
                count++;
            } else {
                _GONG_DEBUG_PRINT(1, "Skipping hidden view: " + getViews()[i]->getName() );
            }
        }
    }
    return count;
}

int dbDefinition::getViewsForTable(const dbRecord *r, dbViewDefinitionsList &container)
{
    return getViewsForTable( r->getTableName(), container );
}

int dbDefinition::getViewsByName(const Xtring &viewname, dbViewDefinitionsList &container)
{
    int count = 0;
    _GONG_DEBUG_PRINT(3, "Looking for viewname: " + viewname );
    for( unsigned int i=0; i<getViews().size(); i++ ) {
        _GONG_DEBUG_PRINT(4, "Comparing with viewname: " + getViews()[i]->getName() );
        if( getViews()[i]->getName().upper().startsWith(viewname.upper()) ) {
            container.insert( getViews()[i]->getName(), getViews()[i] );
            count++;
        }
    }
    return count;
}

dbTableDefinition *dbDefinition::findTableDefinition(const Xtring &tablename, bool warning) const
{
    dbTableDefinition *tbldef = mTables[tablename];
#ifdef _GONG_DEBUG
    if( !tbldef && warning)
        _GONG_DEBUG_WARNING( Xtring::printf("Table %s not found in database",
                                            tablename.c_str() ) );
#endif
    return tbldef;
}

dbTableDefinition *dbDefinition::getTableDefinition(unsigned int i)
{
    dbTableDefinition *tbldef = mTables[i];
#ifdef _GONG_DEBUG
    if( !tbldef )
        _GONG_DEBUG_WARNING( Xtring::printf("Table with index %d not found in database", i ) );
#endif
    return tbldef;
}

const dbTableDefinition *dbDefinition::getTableDefinition(unsigned int i) const
{
    dbTableDefinition *tbldef = mTables[i];
#ifdef _GONG_DEBUG
    if( !tbldef )
        _GONG_DEBUG_WARNING( Xtring::printf("Table with index %d not found in database", i ) );
#endif
    return tbldef;
}


#ifdef _GONG_DEBUG
std::ostream &operator<<(std::ostream &out, const dbDefinition &dbdef)
{
    out << dbdef.getName() << '\t' << dbdef.getDescription() << std::endl;
    for( unsigned int i = 0; i < dbdef.getTables().size(); i++ )
        out << *(const_cast<dbDefinition &>(dbdef).getTables()[i]);
    return out;
}
#endif


}; // namespace





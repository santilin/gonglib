#include "gongdbdefinition.h"
#include "gongdbresultset.h"
#include "gongdbviewdefinition.h"

namespace gong {

dbViewDefinition::dbViewDefinition(const dbTableDefinition &other)
    : dbTableDefinition(other),
      mOrigin("DEFAULT:" + other.getName()),
      mCaption( other.getDescPlural() ), mModified(false)
{
    setFrom( other.getFrom().isEmpty() ? other.getName() : other.getFrom() );
}

dbViewDefinition::dbViewDefinition(const dbViewDefinition &other)
    : dbTableDefinition( other ),
      mOrigin( other.mOrigin ), mCaption( other.mCaption ),
      mModified(false)
{
}

// Warning: The fields in the select clause must be separated by ", "
dbViewDefinition::dbViewDefinition(const Xtring &name, dbDefinition &dbdef,
                                   const Xtring &select, const Xtring &origin, const Xtring& caption)
    : dbTableDefinition(dbdef, name),
      mOrigin(origin), mCaption(caption), mModified(false)
{
    Xtring fields, from, where, orderby, groupby;
    if( parseSelectStatement(select, fields, from, where, orderby, groupby) ) {
        XtringList fieldslist;
        Xtring firstfrom;
        if( !from.isEmpty() ) {
            XtringList fromtables = dbConnection::extractSqlTables( "FROM " + from );
            if( fromtables.size() != 0 )
                firstfrom = fromtables[0];
            else
                firstfrom = from;
        } else { //  Give a chance to the recorddatamodel to add its tablename
            _GONG_DEBUG_WARNING( "View " + name + " lacks FROM" );
        }
        dbTableDefinition *firsttbldef = dbdef.findTableDefinition(firstfrom);
        if( !firsttbldef ) {
            _GONG_DEBUG_WARNING( "Table '" + firstfrom + "' not found in view '" + name + "'" );
        } else {
            setDescSingular( firsttbldef->getDescPlural() );
            setDescPlural( getDescSingular() );
        }
        // Tokenize the fields string by ", "
        // Add the ID field even if there is one in the view
        fieldslist.push_back( firstfrom + ".ID" );
        Xtring::size_type pos=0, commapos = fields.find( ", " );
        while( commapos != Xtring::npos ) {
            fieldslist.push_back( fields.mid(pos, commapos - pos) );
            pos = commapos + 2;
            commapos = fields.find( ", ", pos );
        }
        fieldslist.push_back( fields.mid(pos) );
        setFrom( from );
        setWhere( where );
        setOrderBy( orderby );
        setGroupBy( groupby );
        for( XtringList::iterator it = fieldslist.begin();
                it != fieldslist.end();
                ++ it ) {
            XtringList fldattrs;
            (*it).tokenize( fldattrs, "|" );
            Xtring fldname = fldattrs[0];
            const dbFieldDefinition *flddeforig;
            _GONG_DEBUG_PRINT(6, "Buscando campo " + firstfrom + "." + fldname);
            if( fldname.find(".") != Xtring::npos )
                flddeforig = dbdef.findFieldDefinition( fldname );
            else
                flddeforig = dbdef.findFieldDefinition( firstfrom + "." + fldname );
            if( flddeforig ) {
                // Copy the field definition as it will be owned by the view
                dbFieldDefinition *flddef = flddeforig->clone();
                for( unsigned int i = 1; i < fldattrs.size(); i++ ) {
                    if( fldattrs[i].startsWith( "w:" ) ) { // Width
                        flddef->setDisplayWidth( fldattrs[i].mid(2).toInt() );
                    } else if( fldattrs[i].startsWith( "d:" ) ) { // Description
                        flddef->setDescription( fldattrs[i].mid(2) );
                    } else if( fldattrs[i].startsWith( "c:" ) ) { // Caption
                        flddef->setCaption( fldattrs[i].mid(2) );
                    } else if( fldattrs[i].startsWith( "s:" ) ) { // Caption
                        flddef->setStyle( fldattrs[i].mid(2) );
                    }
                }
                _GONG_DEBUG_PRINT(5, Xtring::printf("Añadiendo campo %p,%s con caption=%s y width=%d a la vista %p,%s",
                                                    flddef, flddef->getFullName().c_str(), flddef->getCaption().c_str(), flddef->getDisplayWidth(), this, getName().c_str() ) );
                addField ( flddef );
            } else {
                _GONG_DEBUG_WARNING( Xtring::printf("Campo %s de la vista %s no encontrado",
                                                    fldname.c_str(), origin.c_str() ) );
            }
            if( mCaption.isEmpty() )
                mCaption = "DEFAULT";
            if( name.isEmpty() )
                setName( firstfrom + "." + mCaption.replace(" ", "_") );
            else
                setName( name );
        }
    } else {
        _GONG_DEBUG_WARNING( "Error in view select clause: " + select );
    }
}


dbViewDefinition::dbViewDefinition(const Xtring &name, dbResultSet *rs, dbDefinition &dbdef,
                                   const Xtring &origin, const Xtring &caption)
    : dbTableDefinition(dbdef, name), mOrigin( origin ), mCaption( caption ), mModified( false )
{
    if( dbdef.findTableDefinition(name) )
        setFrom( name );
    for( dbResultSet::size_type col = 0; col < rs->getColumnCount(); col++)
    {
        Xtring fldname = rs->getColumnName(col);
        Xtring sfield = dbFieldDefinition::extractFieldName( fldname );
        Xtring stable = dbFieldDefinition::extractTableName( fldname );
        if( getFrom().isEmpty() && !stable.isEmpty() )
            setFrom(stable);
        const dbFieldDefinition *flddeforig;
        if( fldname.find(".") != Xtring::npos )
            flddeforig = dbdef.findFieldDefinition( fldname );
        else
            flddeforig = dbdef.findFieldDefinition( getFrom() + "." + fldname );
        int w, d;
        SqlColumnType t;
        dbFieldDefinition::Flags tmpflags;
        dbFieldDefinition *flddef;
        if( flddeforig ) {
            flddef = flddeforig->clone();
        } else {
            w = rs->getColumnWidth(col);
            d = rs->getColumnDecimals(col);
            t = rs->getColumnType(col);
            tmpflags = dbFieldDefinition::NONE;
            flddef = new dbFieldDefinition(
                stable, sfield, t, w, d,
                tmpflags, // null, primarykey, ...
                Xtring::null // default value
            );
            flddef->setCaption( fldname.proper() );
            flddef->setDescription( stable + "." + sfield );
        }
        addField( flddef );
    }
}

Xtring dbViewDefinition::toConfigString() const
{
    Xtring result;
    for( unsigned int i=0; i<getFieldDefinitions().size(); i++) {
        dbFieldDefinition *flddef = getFieldDefinition(i);

        if( flddef->getName() == "ID" || !flddef->isVisible() )
            continue;
        if( !result.isEmpty() )
            result += ", ";
        result += flddef->getFullName();
        _GONG_DEBUG_PRINT(10, Xtring::printf("field %s: caption=%s, style=%s, width=%d",
                                             flddef->getFullName().c_str(),
                                             flddef->getCaption().c_str(),
                                             flddef->getStyle().c_str(),
                                             flddef->getDisplayWidth() ) );
        // Do not add values that are the same as the table definitions of the field being added
        dbFieldDefinition *origflddef = getdbDefinition().findFieldDefinition( flddef->getFullName() );
        _GONG_DEBUG_PRINT(10, Xtring::printf("field %s: caption=%s, style=%s, width=%d",
                                             origflddef->getFullName().c_str(),
                                             origflddef->getCaption().c_str(),
                                             origflddef->getStyle().c_str(),
                                             origflddef->getDisplayWidth() ) );
        if( !flddef->getCaption().isEmpty() && origflddef && origflddef->getCaption() != flddef->getCaption() )
            result+="|c:" + flddef->getCaption();
        if( !flddef->getDescription().isEmpty() && origflddef && origflddef->getDescription() != flddef->getDescription() )
            result+="|d:" + flddef->getDescription();
        if( flddef->getDisplayWidth() != 0 && origflddef && origflddef->getDisplayWidth() != flddef->getDisplayWidth() )
            result+="|w:" + Xtring::number(flddef->getDisplayWidth());
        if( !flddef->getStyle().isEmpty() && origflddef && origflddef->getStyle() != flddef->getStyle() )
            result+="|s:" + flddef->getStyle();
    }
    result += " FROM " + ( getFrom().isEmpty() ? getName() : getFrom() ) ;
    if( !getWhere().isEmpty() )
        result += " WHERE " + getWhere();
    if( !getGroupBy().isEmpty() )
        result += " GROUPBY " + getGroupBy();
    if( !getOrderBy().isEmpty() )
        result += " ORDER BY " + getOrderBy();
    return result;
}

Xtring dbViewDefinition::getFirstFrom() const
{
    XtringList fromtables = dbConnection::extractSqlTables( "FROM " + getFrom() );
    if( fromtables.size() != 0 )
        return fromtables[0];
    else
        return getFrom();
}

bool dbViewDefinition::createFromFromFields()
{
    XtringList needed_tables, found_tables;
    Xtring firstfrom = getFirstFrom();
    if( !getdbDefinition().findTableDefinition( firstfrom ) )
        firstfrom.clear();
    // Extract all the tables from the fields
    Xtring result;
    for( dbFieldDefinitionsList::const_iterator flddefit= getFieldDefinitions().begin();
            flddefit != getFieldDefinitions().end();
            ++flddefit ) {
        Xtring tablename = dbFieldDefinition::extractTableName( flddefit->second->getFullName() );
        if( firstfrom.isEmpty() ) {
            firstfrom = tablename;
        } else if( !needed_tables.contains( tablename ) && tablename != firstfrom ) {
            needed_tables << tablename;
        }
    }
    setFrom( firstfrom );
    found_tables << firstfrom;
    Xtring needed_table;
    bool has_found_one = true;
    // Iterate tables until we relate them all
    while( needed_tables.size() ) {
        has_found_one = false;
        for( XtringList::iterator needed_it = needed_tables.begin();
                needed_it != needed_tables.end();
                ++ needed_it ) {
            needed_table = *needed_it;
            bool found = false;
            // Look for a table that relates to needed_table
            for( XtringList::const_iterator found_it = found_tables.begin();
                    found_it != found_tables.end() && !found;
                    ++found_it ) {
                dbTableDefinition *tbldef = getdbDefinition().findTableDefinition( *found_it );
                for( dbRelationDefinitionsList::const_iterator relit = tbldef->getRelationDefinitions().begin();
                        relit != tbldef->getRelationDefinitions().end() && !found;
                        ++relit ) {
                    const dbRelationDefinition *reldef = relit->second;
                    if( reldef->getLeftTable() == *found_it
                            && reldef->getRightTable() == needed_table ) {
                        dbFieldDefinition *flddef = getdbDefinition().findFieldDefinition(*found_it + "." + reldef->getLeftField());
                        if( flddef && flddef->canBeNull() )
                            result += " LEFT";
                        else
                            result += " INNER";
                        result += " JOIN " + needed_table
                                  + " ON " + *found_it + "." + reldef->getLeftField()
                                  + " = " + needed_table + "." + reldef->getRightField();
                        found = true;
                    }
                }
                if( !found ) {
                    // look for a relation the other way round
                    tbldef = getdbDefinition().findTableDefinition( needed_table );
                    for( dbRelationDefinitionsList::const_iterator relit = tbldef->getRelationDefinitions().begin();
                            relit != tbldef->getRelationDefinitions().end() && !found;
                            ++relit ) {
                        const dbRelationDefinition *reldef = relit->second;
                        if( reldef->getLeftTable() == needed_table
                                && reldef->getRightTable() == *found_it ) {
                            dbFieldDefinition *flddef = getdbDefinition().findFieldDefinition( needed_table + "." + reldef->getLeftField());
                            if( flddef && flddef->canBeNull() )
                                result += " LEFT";
                            else
                                result += " INNER";
                            result += " JOIN " + needed_table
                                      + " ON " + needed_table + "." + reldef->getLeftField()
                                      + " = " + *found_it + "." + reldef->getRightField();
                            found = true;
                        }
                    }
                }
            }
            if( found ) {
                has_found_one = true;
                found_tables.push_back( *needed_it );
                needed_tables.erase( needed_it );
                break;
            }
        }
        if( !has_found_one ) {
            _GONG_DEBUG_WARNING( "Impossible to find a relation for table " + needed_table );
            break;
        }
    }
    setFrom( getFrom() + result );
    return has_found_one;
}

} // namespace gong

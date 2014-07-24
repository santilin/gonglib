#include "gonggettext.h"
#include "gongdbfieldlistofvalues.h"
#include "gongdbmastertable.h"

namespace gong {

dbFieldDefinition *dbMasterTable::addFieldOne2OneRelation(const Xtring &name,
        const Xtring &referredField, bool canbenull)
{
    Xtring reftable = dbFieldDefinition::extractTableName( referredField );
    Xtring reffield = dbFieldDefinition::extractFieldName( referredField );
    pTableDefinition->addRelationDefinition( dbRelationDefinition::one2one, pTableDefinition->getName(), name, reftable, reffield );
    dbFieldReferenceID *fld = new dbFieldReferenceID(pTableDefinition->getName(), name, referredField,
            canbenull ? dbFieldDefinition::NONE : dbFieldDefinition::NOTNULL);
    return pTableDefinition->addField(fld);
}

dbFieldDefinition *dbMasterTable::addFieldAggregateRelation(const Xtring &name,
        const Xtring &referredField, bool canbenull)
{
    Xtring reftable = dbFieldDefinition::extractTableName( referredField );
    Xtring reffield = dbFieldDefinition::extractFieldName( referredField );
    pTableDefinition->addRelationDefinition( dbRelationDefinition::aggregate, pTableDefinition->getName(), name, reftable, reffield );
    dbFieldReferenceID *fld = new dbFieldReferenceID(pTableDefinition->getName(), name, referredField,
            canbenull ? dbFieldDefinition::NONE : dbFieldDefinition::NOTNULL);
    return pTableDefinition->addField(fld);
}

dbFieldDefinition *dbMasterTable::addFieldOne2ManyRelation(const Xtring &name,
        const Xtring &referredField, bool canbenull)
{
    Xtring reftable = dbFieldDefinition::extractTableName( referredField );
    Xtring reffield = dbFieldDefinition::extractFieldName( referredField );
    pTableDefinition->addRelationDefinition( dbRelationDefinition::one2many, pTableDefinition->getName(), name, reftable, reffield );
    dbFieldReferenceID *fld = new dbFieldReferenceID(pTableDefinition->getName(), name, referredField,
            canbenull ? dbFieldDefinition::NONE : dbFieldDefinition::NOTNULL);
    return pTableDefinition->addField(fld);
}

// Adds a list of values field whose values and captions are pointers, so no copy is made
template<>
dbFieldDefinition *dbMasterTable::addFieldListOfValues(bool insertallowed,
        XtringList *captions, List<Xtring> *values, const Xtring &name)
{
    /// \todo {0.3.4} Calcular la longitud máxima de los captions?
    return pTableDefinition->addField(new dbFieldListOfValues<Xtring>(
		insertallowed, captions, values, pTableDefinition->getName(), name, SQLSTRING, 100, 0,
		dbFieldDefinition::NOTNULL));
}

// Adds a list of values field whose values and captions are references, so a copy is made
template<>
dbFieldDefinition *dbMasterTable::addFieldListOfValues(bool insertallowed,
        XtringList &captions, List<Xtring> &values, const Xtring &name)
{
    /// \todo {0.3.4} Calcular la longitud máxima de los captions?
    return pTableDefinition->addField(new dbFieldListOfValues<Xtring>(
		insertallowed, captions, values, pTableDefinition->getName(), name, SQLSTRING, 100, 0,
		dbFieldDefinition::NOTNULL));
}

template<>
dbFieldDefinition *dbMasterTable::addFieldListOfValues<int>(bool insertallowed,
        XtringList *captions, List<int> *values, const Xtring &name)
{
    return pTableDefinition->addField(new dbFieldListOfValues<int>(
                                          insertallowed, captions, values, pTableDefinition->getName(), name, SQLINTEGER, 10, 0,
                                          dbFieldDefinition::NOTNULL));
}

template<>
dbFieldDefinition *dbMasterTable::addFieldListOfValues<int>(bool insertallowed,
        XtringList &captions, List<int> &values, const Xtring &name)
{
    return pTableDefinition->addField(new dbFieldListOfValues<int>(
                                          insertallowed, captions, values, pTableDefinition->getName(), name, SQLINTEGER, 10, 0,
                                          dbFieldDefinition::NOTNULL));
}

}

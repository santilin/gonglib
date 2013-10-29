#include "gonglibrary.h"
#include "gongdbrecordlist.h"
#include "gongdbrecordrelation.h"

/*
	Do not confuse this class with dbRelationDefinition (which simply defines a relation,
	that is, a set of left and right fields an tables to be added to a dbTableDefinition
	usually with addOne2OneRelation or addOne2ManyRelation),
	while this class is the actual relation created on a dbrecord in its constructor, based
	on the definition of its dbTableDefinition.
*/

namespace gong {

// We should make here the lazy read, but here we don't have the left record
// dbRecordList* dbRecordRelation::getRelRecordList() const

dbRecordRelation::dbRecordRelation(dbRelationDefinition* reldef, bool enabled, dbRecord* relrecord, dbRecordList* relrecordlist)
    : pRelationDefinition(reldef), mEnabled(enabled), pRelatedRecord(relrecord), pRelatedRecordList(relrecordlist)
{
    // If we have a record list, create the orig record list
    if( pRelatedRecordList )
        pRelatedRecordListOrig = new dbRecordList();
}

dbRecordRelation::~dbRecordRelation()
{
    if( pRelatedRecordListOrig )
        delete pRelatedRecordListOrig;
}

dbRecord *dbRecordRelation::addRelatedRecord( const dbRecord *record, int pos )
{
    return pRelatedRecordList->addRecord( record, pos );
}

dbRecord *dbRecordRelation::getRelatedRecord(int index)
{
    if( index == -1 ) {
        if( pRelatedRecord == 0 )
            pRelatedRecord = GongLibraryInstance->createRecord ( pRelationDefinition->getRightTable() );
        return pRelatedRecord;
    } else {
        return pRelatedRecordList->getRecord( index );
    }
}

bool dbRecordRelation::isRelatedRecordCreated(int index) const
{
    if( index == -1 ) {
        return pRelatedRecord != 0;
    } else {
        return pRelatedRecordList->at(index) != 0; // getRecord( index ) != 0;
    }
}

}

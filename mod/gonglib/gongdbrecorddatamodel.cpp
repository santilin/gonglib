#include "gongdbrecord.h"
#include "gongdbrecorddatamodel.h"

namespace gong
{

dbRecordDataModel::dbRecordDataModel(
    dbRecord *record, const dbViewDefinitionsList &vlist, const Xtring &filter)
    : TableDataModel(vlist, filter),
      pRecord(record), pResultSet(0)
{
    _GONG_DEBUG_ASSERT( record );
    _GONG_DEBUG_PRINT(4, Xtring::printf("record: %s, %d views, filter='%s'", record->getTableName().c_str(), vlist.size(), filter.c_str() ) );
    mReadOnly = true;
    mHasFetchedAllRows = false;
    mCurrentViewIndex = -1;
}

bool dbRecordDataModel::setViewIndex( int nview )
{
    _GONG_DEBUG_ASSERT(  pRecord  );
    dbViewDefinition *viewformat = 0;
    mFieldPos.clear();
    if( nview >= (int)getViewCount() )
        nview = 0;
    viewformat = getView( nview );
    // Why this? getView allways must return a view
    while( !viewformat && nview < (int)getViewCount() ) {
        _GONG_DEBUG_WARNING( "getView did not return a view for record " + pRecord->getTableName() );
        nview ++;
        viewformat = getView( nview );
    }
    if( !viewformat ) {
        nview = 0;
        while( !viewformat && nview < (int)getViewCount() ) {
            nview++;
            viewformat = getView( nview );
        }
    }
    mCurrentViewIndex = nview;
    // Build the query out of the view. If there is no view, build a general query from the dbRecord
    Xtring fields, from, where, orderby, groupby, join;
    if( viewformat ) {
        // The record ID is allways the first field, regardless of its position in the table
        for( unsigned int i=0; i<viewformat->getFieldCount(); i++ ) {
            dbFieldDefinition *flddef = viewformat->getFieldDefinition(i);
            if( !fields.isEmpty() )
                fields += ",";
            if( i == 0 && flddef->getName() == "ID" && viewformat->getFieldCount() > 1 )
                orderby = "1";
            if( flddef->getTableName().isEmpty() ) {
                fields += getRecord()->getConnection()->nameToSQL(pRecord->getTableName())
                          + "." + getRecord()->getConnection()->nameToSQL(flddef->getName());
                mFieldPos.insert( flddef->getName(), i);
            } else {
                fields += getRecord()->getConnection()->nameToSQL(flddef->getTableName())
                          + "." + getRecord()->getConnection()->nameToSQL(flddef->getName());
                mFieldPos.insert( flddef->getTableName() + "." + flddef->getName(), i );
            }
        }
        if( !viewformat->getOrderBy().isEmpty() )
            orderby = viewformat->getOrderBy();
        from = viewformat->getFrom();
        where = viewformat->getWhere();
        groupby = viewformat->getGroupBy();
    }
    if( fields.isEmpty() )
        fields = "*";
    if( from.isEmpty() )
        from = pRecord->getTableName();
    /* Build the global filter
    	= structural filter from the record
    	+ view filter (view.where)
    	+ datamodel filter
    */
// 	if( mFilter.find( where ) == Xtring::npos ) {
// 		if( !mFilter.isEmpty() )
// 			mFilter += " AND ";
// 		mFilter += where;
// 	}
    where = getFilter( "WHERE", pRecord->getFilter("", where ) );
    pResultSet = pRecord->getConnection()->select( "SELECT " + fields + " FROM " + from
                 + where
                 + (groupby.isEmpty()?"":" GROUP BY " + groupby)
                 + (orderby.isEmpty()?"":" ORDER BY " + orderby) );
    mColCount = pResultSet->getColumnCount();
    mHasFetchedAllRows = true;
    mGroupCount = 0;
    return true;
}

unsigned int dbRecordDataModel::getRowCount() const
{
    if( !pResultSet )
        return 0;
    return pResultSet->getRowCount();
}


Variant dbRecordDataModel::getValue(unsigned int row, unsigned int col) const
{
    if( row >=0 && col >= 0 && row < getRowCount() && col < getColCount() ) {
        return pResultSet->getValue(row, col);
    } else return Variant();
}

} // namespace

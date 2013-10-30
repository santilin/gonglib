#include "gongdbresultsetdatamodel.h"

namespace gong {

dbResultSetDataModel::dbResultSetDataModel( dbResultSet *resultset, dbRecord *record)
    : dbRecordDataModel(record, dbViewDefinitionsList(), Xtring::null)
{
    _GONG_DEBUG_ASSERT( resultset );
    _GONG_DEBUG_ASSERT( record );
    pRecord = record;
    pResultSet = resultset;
    mColCount = resultset->getColumnCount();
    mReadOnly = true;
    mHasFetchedAllRows = true;
    mGroupCount = 0;
    pViewDefinition = new dbViewDefinition(record->getTableName(), pResultSet,
                                           record->getTableDefinition()->getdbDefinition(),
                                           "From resultset", Xtring::null);
    mViewDefinitionsList.insert( pViewDefinition->getName(), const_cast<dbViewDefinition *>(pViewDefinition) );
}


dbResultSetDataModel::~dbResultSetDataModel()
{
}

bool dbResultSetDataModel::setValue( unsigned int row, unsigned int col, const Variant &value )
{
    return false;
}

bool dbResultSetDataModel::setViewIndex( int nview )
{
    if ( nview == getCurrentViewIndex() )
        return true; // No need to do anything as the data is in the model
    _GONG_DEBUG_ASSERT( pRecord );
    dbViewDefinition *viewformat = 0;
    mFieldPos.clear();
    if ( nview >= ( int )getViewCount() )
        nview = 0;
    viewformat = getView( nview );
    while ( !viewformat && nview < ( int )getViewCount() ) {
        nview ++;
        viewformat = getView( nview );
    }
    if ( !viewformat ) {
        nview = 0;
        while ( !viewformat && nview < ( int )getViewCount() ) {
            nview++;
            viewformat = getView( nview );
        }
    }
    mCurrentViewIndex = nview;
    if ( viewformat ) {
        // El ID del registro es siempre el primer campo, independientemente de la posición real que ocupe
        mFieldPos.clear();
        for ( unsigned int i = 0; i < viewformat->getFieldCount(); i++ ) {
            dbFieldDefinition *flddef = viewformat->getFieldDefinition( i );
            if ( flddef->getTableName().isEmpty() ) {
                mFieldPos.insert( flddef->getName(), i );
            } else {
                mFieldPos.insert( flddef->getFullName(), i );
            }
        }
    }
    return true;
}


Variant dbResultSetDataModel::getValue( unsigned int row, unsigned int col ) const
{
    Variant ret;
    if ( row < mColCount )
        ret = pResultSet->getValue( row, col );
    _GONG_DEBUG_PRINT( 11, Xtring::printf( "row=%d, col=%d, fldname=%s, value=%s", row, col, getColName( col ).c_str(), ret.toString().c_str() ) );
    return ret;
}

} // namespace

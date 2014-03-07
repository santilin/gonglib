#include "gongdbrecordlistdatamodel.h"

/**
	@file gongdbrecordlistdatamodel.cpp A data model between record lists and GUI table widgets
	This datamodel inherits dbRecordDataModel because it uses the same views, but it doesn't
	make use of a dbResultSet but a dbRecordList
*/

namespace gong {

dbRecordListDataModel::dbRecordListDataModel( dbRecordList *reclist, dbRecord *record,
        const dbViewDefinitionDict &vlist, const Xtring &filter)
    : dbRecordDataModel(record, vlist, filter)
{
    _GONG_DEBUG_ASSERT( reclist );
    _GONG_DEBUG_ASSERT( record );
    pRecord = record;
    pRecordList = reclist;
    mColCount = record->getFieldCount();
    mReadOnly = false;
    mHasFetchedAllRows = true;
}


dbRecordListDataModel::~dbRecordListDataModel()
{}


bool dbRecordListDataModel::setViewIndex(int nview)
{
    _GONG_DEBUG_ASSERT(  pRecord  );
    dbViewDefinition *viewformat = 0;
    mFieldPos.clear();
    if( nview >= (int)getViewCount() )
        nview = 0;
    viewformat = getView( nview );
    while( !viewformat && nview < (int)getViewCount() )
    {
        nview ++;
        viewformat = getView( nview );
    }
    if( !viewformat )
    {
        nview = 0;
        while( !viewformat && nview < (int)getViewCount() )
        {
            nview++;
            viewformat = getView( nview );
        }
    }
    mCurrentViewIndex = nview;
    if( viewformat )
    {
        // El ID del registro es siempre el primer campo, independientemente de la posición real que ocupe
        mFieldPos.clear();
        for( unsigned int i=0; i<viewformat->getFieldCount(); i++ )
        {
            dbFieldDefinition *flddef = viewformat->getFieldDefinition(i);
            if( flddef->getTableName().isEmpty() )
            {
                mFieldPos.insert( flddef->getName(), i);
            }
            else
            {
                mFieldPos.insert( flddef->getFullName(), i );
            }
        }
    }
    return true;
}


unsigned int dbRecordListDataModel::getRowCount() const
{
    return pRecordList->size();
}


Variant dbRecordListDataModel::getValue(unsigned int row, unsigned int col) const
{
    Variant ret;
    if( row < pRecordList->size() )
        ret = pRecordList->getRecord(row)->getValue(getColName(col));
    _GONG_DEBUG_PRINT(11, pRecordList->getRecord(row)->toString( TOSTRING_DEBUG_COMPLETE ) );
    _GONG_DEBUG_PRINT(11, Xtring::printf("row=%d, col=%d, fldname=%s, value=%s", row, col, getColName(col).c_str(), ret.toString().c_str()) );
    return ret;
}

bool dbRecordListDataModel::setValue(unsigned int row, unsigned int col, const Variant &value)
{
    if( row < pRecordList->size() )
        return pRecordList->getRecord(row)->setValue(col, value);
    else
        return false;
}


} // namespace

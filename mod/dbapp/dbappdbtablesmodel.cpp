#include <gongxtring.h>
#include <gongguinamespace.h>
#include <gongdbconnection.h>
#include "dbappdbtablesmodel.h"

namespace gong {

DBTablesModel::DBTablesModel(dbDefinition* db, dbConnection *conn, QObject* parent)
    : QAbstractListModel( parent )
{
    mTables = conn->selectStringList( 1, "SHOW TABLES" );
    for( uint i=0; i < mTables.size(); ++ i )
        mCheckedTables.push_back( false );
}

int DBTablesModel::rowCount(const QModelIndex& parent) const
{
    return mTables.size();
}

QVariant DBTablesModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    else if( role == Qt::DisplayRole )
        return QVariant( toGUI( mTables[ index.row() ]) );
    else if( role == Qt::CheckStateRole )
        return mCheckedTables[ index.row() ] ? Qt::Checked : Qt::Unchecked;
    return QVariant();
}

bool DBTablesModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if( index.row() < 0 || index.row() >= rowCount() || !index.isValid() )
        return false;
    if( role == Qt::CheckStateRole ) {
        mCheckedTables[ index.row() ] = static_cast<Qt::CheckState>(value.toUInt()) == Qt::Checked;
        return true;
    }
    return false;
}

Qt::ItemFlags DBTablesModel::flags( const QModelIndex& index ) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags( index );
    if ( index.column() == 0 )
        defaultFlags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    return defaultFlags;
}

void DBTablesModel::selectAll()
{
    for( uint i=0; i < mTables.size(); ++ i )
        mCheckedTables[i] = true;
}

void DBTablesModel::unselectAll()
{
    for( uint i=0; i < mTables.size(); ++ i )
        mCheckedTables[i] = false;
}

void DBTablesModel::getCheckedTables(XtringList& tableslist)
{
    for( uint i=0; i < mCheckedTables.size(); ++ i )
        if( mCheckedTables[i] )
            tableslist << mTables[i];
}



} // namespace gong

#include <QVariant>
#include <qnamespace.h>
#include <gongdbrecord.h>
#include <gongguinamespace.h>
#include "dbapprecordcompleter.h"


namespace gong {

RecordCompleterModel::RecordCompleterModel(dbRecord *record, const Xtring& fldname, QObject* parent)
    : QAbstractListModel( parent )
{
    pResultSet = record->getConnection()->select( "SELECT " + record->getConnection()->nameToSQL( fldname )
                 + " FROM " + record->getTableName() + record->getFilter("WHERE")
                 + " ORDER BY 1" );
}

RecordCompleterModel::~RecordCompleterModel()
{
    delete pResultSet;
}

int RecordCompleterModel::rowCount(const QModelIndex& parent) const
{
    return pResultSet->getRowCount();
}

QVariant RecordCompleterModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    else if( role == Qt::EditRole || role == Qt::DisplayRole ) {
        _GONG_DEBUG_PRINT( 0, pResultSet->getValue( index.row(), 0 ).toString() );
        return QVariant( toGUI( pResultSet->getValue( index.row(), 0 ).toString() ) );
    }
    return QVariant();
}

RecordCompleter::RecordCompleter(dbRecord* record, const Xtring &fldname, QObject* parent)
    : QCompleter(parent)
{
    setCaseSensitivity( Qt::CaseInsensitive );
    setModelSorting( QCompleter::CaseInsensitivelySortedModel );
    setCompletionMode( QCompleter::PopupCompletion );
    pRecCompModel = new RecordCompleterModel( record, fldname, parent );
    setModel( pRecCompModel );
}

RecordCompleter::~RecordCompleter()
{
    delete pRecCompModel;
}

} // namespace gong



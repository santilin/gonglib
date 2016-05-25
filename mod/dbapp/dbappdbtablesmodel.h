#ifndef _DBAPP_DBTABLESMODEL_H
#define _DBAPP_DBTABLESMODEL_H

#include <QAbstractListModel>

namespace gong {

class dbConnection;
class dbDefinition;

class DBTablesModel: public QAbstractListModel
{
    Q_OBJECT
public:
    DBTablesModel( dbDefinition *db, dbConnection *conn, QObject *parent = 0 );
    int rowCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    void selectAll();
    void unselectAll();
    void getCheckedTables( XtringList &tableslist );
private:
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    XtringList mTables;
    List<bool> mCheckedTables;
};

} // namespace gong

#endif // _DBAPP_DBTABLESMODEL_H

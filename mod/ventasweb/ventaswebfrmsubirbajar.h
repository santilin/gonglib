#ifndef VENTASWEB_FRMSUBIRBAJAR_H
#define VENTASWEB_FRMSUBIRBAJAR_H

#include <dbappfrmcustom.h>
#include <QAbstractListModel>

class QListView;
class QTreeView;

namespace gong {
namespace ventasweb {

class DatabaseTablesModel: public QAbstractListModel
{
    Q_OBJECT
public:
    DatabaseTablesModel( dbDefinition *db, QObject *parent = 0 );
    int rowCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    void selectAll();
    void unselectAll();
    void selectVentasWeb( bool subir );
    void getCheckedTables( XtringList &tableslist );
private:
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    XtringList mTables;
    List<bool> mCheckedTables;
};

class FrmSubirBajar: public FrmCustom
{
    Q_OBJECT
public:
    FrmSubirBajar( bool subir, QWidget* parent = 0, WidgetFlags fl = 0 );
    virtual void accept();
protected slots:
    void slotSelectAll();
    void slotUnselectAll();
    void slotSelectVentasWeb();

private:
    bool mSubir;
    LineEdit *pRemoteDbName, *pRemoteUser, *pRemotePassword, *pRemoteHost;
    QPushButton *pushSelectAll, *pushUnselectAll, *pushSelectVentasWeb;
    QCheckBox *pCheckSaveSettings;
    QListView *pListTables;
    DatabaseTablesModel *pModelTables;
};

} // namespace ventasweb
} // namespace gong

#endif // VENTASWEB_FRMSUBIRBAJAR_H


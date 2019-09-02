#include <QListView>
#include <gongfileutils.h>
#include <gonggettext.h>
#include <dbappfrmdatabasetools.h>
#include "ventaswebfrmsubirbajar.h"
#include "ventaswebmodule.h"

namespace gong {
namespace ventasweb {

DatabaseTablesModel::DatabaseTablesModel(dbDefinition* db, QObject* parent)
    : QAbstractListModel( parent )
{
    dbConnection *conn = DBAPP->getConnection();
    mTables = conn->selectStringList( 1, "SHOW TABLES" );
    for( uint i=0; i < mTables.size(); ++ i )
        mCheckedTables.push_back( false );
}

int DatabaseTablesModel::rowCount(const QModelIndex& parent) const
{
    return mTables.size();
}

QVariant DatabaseTablesModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    else if( role == Qt::DisplayRole )
        return QVariant( toGUI( mTables[ index.row() ]) );
    else if( role == Qt::CheckStateRole )
        return mCheckedTables[ index.row() ] ? Qt::Checked : Qt::Unchecked;
    return QVariant();
}

bool DatabaseTablesModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if( index.row() < 0 || index.row() >= rowCount() || !index.isValid() )
        return false;
    if( role == Qt::CheckStateRole ) {
        mCheckedTables[ index.row() ] = static_cast<Qt::CheckState>(value.toUInt()) == Qt::Checked;
        return true;
    }
    return false;
}

Qt::ItemFlags DatabaseTablesModel::flags( const QModelIndex& index ) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags( index );
    if ( index.column() == 0 )
        defaultFlags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    return defaultFlags;
}

void DatabaseTablesModel::selectAll()
{
    for( uint i=0; i < mTables.size(); ++ i )
        mCheckedTables[i] = true;
}

void DatabaseTablesModel::selectVentasWeb(bool subir)
{
    for( uint i=0; i < mTables.size(); ++ i ) {
        mCheckedTables[i] = false;
        if( subir ) {
            if( mTables[i] == "ARTICULO" || mTables[i] == "FAMILIA"
                    || mTables[i] == "PROVEEDORA" || mTables[i] == "CLIENTE"
                    || mTables[i] == "CONTACTO" )
                mCheckedTables[i] = true;
        } else {
            if( mTables[i] == "PEDIDOWEB" || mTables[i] == "PEDIDOWEBDET" )
                mCheckedTables[i] = true;
        }
    }
}

void DatabaseTablesModel::unselectAll()
{
    for( uint i=0; i < mTables.size(); ++ i )
        mCheckedTables[i] = false;
}

void DatabaseTablesModel::getCheckedTables(XtringList& tableslist)
{
    for( uint i=0; i < mCheckedTables.size(); ++ i )
        if( mCheckedTables[i] )
            tableslist << mTables[i];
}



FrmSubirBajar::FrmSubirBajar(bool subir, QWidget* parent, WidgetFlags fl)
    : FrmCustom( parent, 0, fl ), mSubir( subir )
{
    if( subir )
        setTitle( _("Subir datos a la web") );
    else
        setTitle( _("Bajar datos de la web") );
    pushSelectAll = new QPushButton( this );
    pushSelectAll->setText( _("Seleccionar todo") );
    connect( pushSelectAll, SIGNAL( clicked() ), this, SLOT( slotSelectAll() ) );
    pushUnselectAll = new QPushButton( this );
    pushUnselectAll->setText( _("Deseleccionar todo") );
    connect( pushUnselectAll, SIGNAL( clicked() ), this, SLOT( slotUnselectAll() ) );
    pushSelectVentasWeb = new QPushButton( this );
    pushSelectVentasWeb->setText( _("Seleccionar ventas web") );
    connect( pushSelectVentasWeb, SIGNAL( clicked() ), this, SLOT( slotSelectVentasWeb() ) );
    pListTables = new QListView( this );
    pModelTables = new DatabaseTablesModel( DBAPP->getDatabase(), this );
    pModelTables->selectVentasWeb( mSubir );
    pListTables->setModel( pModelTables );
    pControlsLayout->addWidget(pListTables);
    pControlsLayout->addWidget(pushSelectAll);
    pControlsLayout->addWidget(pushUnselectAll);
    pControlsLayout->addWidget(pushSelectVentasWeb);
    pRemoteDbName = addInput( this, _("Nombre de la base de datos remota"),
                              ModuleInstance->getModuleSetting( "REMOTE_DBNAME") );
    pRemoteUser = addInput( this, _("Nombre de la usuaria remota"),
                            ModuleInstance->getModuleSetting( "REMOTE_USER") );
    pRemotePassword = addInput( this, _("Contraseña de la usuaria remota"),
                                ModuleInstance->getModuleSetting( "REMOTE_PASSWORD"),
                                "PASSWORD" );
    pRemoteHost = addInput( this, _("Nombre del servidor remoto"),
                            ModuleInstance->getModuleSetting( "REMOTE_HOST") );
    pCheckSaveSettings = addCheckBox( this, _("Guardar datos de conexión"), true );
}

void FrmSubirBajar::slotSelectAll()
{
    pModelTables->selectAll();
    pListTables->repaint();
}

void FrmSubirBajar::slotUnselectAll()
{
    pModelTables->unselectAll();
    pListTables->repaint();
}

void FrmSubirBajar::slotSelectVentasWeb()
{
    pModelTables->selectVentasWeb( mSubir );
    pListTables->repaint();
}

void FrmSubirBajar::accept()
{
    XtringList checkedtables;
    pModelTables->getCheckedTables( checkedtables );
    if( checkedtables.size() == 0 ) {
        msgError(this, _("Marca las tablas a subir o bajar de la web") );
        return;
    }
    Xtring database = pRemoteDbName->toString();
    if( database.isEmpty() ) {
        msgError(this, _("Introduce el nombre de la base de datos remota") );
        pRemoteDbName->setFocus();
        return;
    }
    Xtring user = pRemoteUser->toString();
    if( user.isEmpty() ) {
        msgError(this, _("Introduce la usuaria remota") );
        pRemoteUser->setFocus();
        return;
    }
    Xtring password = pRemotePassword->toString();
    if( password.isEmpty() ) {
        msgError(this, _("Introduce la contraseña remota") );
        pRemotePassword->setFocus();
        return;
    }
    Xtring host = pRemoteHost->toString();
    Xtring errors;
    if( mSubir ) {
        FrmDatabaseTools::dump( "/tmp/subir.sql", DBAPP->getDatabase()->getName(), DBAPP->getDbUser(), DBAPP->getDbHost(),
                                DBAPP->getDbUserPassword(), checkedtables, errors );
        if( errors.isEmpty() )
            FrmDatabaseTools::load( "/tmp/subir.sql", database, user, host, password, errors );
    } else {
        FrmDatabaseTools::dump( "/tmp/subir.sql", database, user, host, password, checkedtables, errors );
        if( errors.isEmpty() )
            FrmDatabaseTools::load( "/tmp/subir.sql", DBAPP->getDatabase()->getName(), DBAPP->getDbUser(), DBAPP->getDbHost(),
                                    DBAPP->getDbUserPassword(), errors );
    }
    if( !errors.isEmpty() )
        msgError( this, errors );
    else {
        if( pCheckSaveSettings->isChecked() ) {
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.REMOTE_DBNAME", database );
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.REMOTE_USER", user );
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.REMOTE_PASSWORD", password );
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.REMOTE_HOST", host );
        } else {
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.REMOTE_DBNAME", "" );
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.REMOTE_USER", "" );
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.REMOTE_PASSWORD", "" );
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.REMOTE_HOST", "" );
        }
        msgOk( this, _("La operación se ha completado con éxito") );
        FrmCustom::accept();
    }
}

} // namespace ventasweb
} // namespace gong

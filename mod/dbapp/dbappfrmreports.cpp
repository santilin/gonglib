#include <gonggettext.h>
#include "dbappdbapplication.h"
#include "dbappfrmsqlselect.h"
#include "dbappfrmreports.h"

/// \todo {0.3.4} Al leer los informes, leer reversed settings combined y no machacar

namespace gong {

bool FrmReports::mReportsRead = false;

FrmReports::FrmReports( QWidget* parent, const char* name, WidgetFlags fl )
    : GuiFrmReports( parent, name, fl )
{
    DBAPP->waitCursor(true);
    // signals and slots connections
    connect( pushFilter, SIGNAL( clicked() ), this, SLOT( pushFilter_clicked() ) );
    connect( treeReports, SIGNAL( currentChanged(Q3ListViewItem*) ),
             this, SLOT( slotCurrentChanged(Q3ListViewItem*) ) );

    if( !mReportsRead ) {
        readAllReports();
        mReportsRead = true;
    }

    // create the tree view out of the DOM
    ReportsTreeItem *groupItem, *newItem;
    ReportsList rplst = AppReport::getReportsList();
    _GONG_DEBUG_PRINT(4, Xtring::printf("Rellenando %d informes", rplst.size() ) );
    for( ReportsList::iterator it = rplst.begin();
            it != rplst.end();
            ++it) {
        _GONG_DEBUG_PRINT(5, Xtring::printf("Titulo: %s, grupo:%s",
                                            (*it).repTitle.c_str(), (*it).repGroup.c_str() ) );
        groupItem = static_cast<ReportsTreeItem *>(treeReports->findItem(
                        QString::fromUtf8((*it).repGroup.c_str()), 0));
        if( !groupItem )
            groupItem = new ReportsTreeItem(treeReports, *it,
                                            QString::fromUtf8((*it).repGroup.c_str()) );
        groupItem->setOpen(true);
        newItem = new ReportsTreeItem( groupItem, groupItem, *it,
                                       QString::fromUtf8((*it).repTitle.c_str()) );
        newItem->setText(1, toGUI((*it).repPath) );
    }
    DBAPP->resetCursor();
}

void FrmReports::slotCurrentChanged(Q3ListViewItem*it)
{
    mSelectedItem = static_cast<ReportsTreeItem *>(it);
    txtTitulo->setText( mSelectedItem->getValue().repTitle );

    Xtring filter; // = mSelectedItem->getValue().repWhere; \todo {check} Esto no es necesario
    for( XtringList::const_iterator it = mSelectedItem->getValue().repFrom.begin();
            it != mSelectedItem->getValue().repFrom.end();
            ++ it ) {
        if( DBAPP->getDatabase()->findTableDefinition( *it ) ) {
            dbRecord *r = DBAPP->createRecord( *it );
            filter = r->getFilter( "", filter );
            delete r;
        } else {
            _GONG_DEBUG_WARNING( Xtring::printf("Table %s used by from in report %s not found",
                                                (*it).c_str(), mSelectedItem->getValue().repTitle.c_str() ) );
        }
    }
    txtFilter->setText( filter );
    /// \todo {0.3.4} Añadir un tostring(string pre, string post) a List
    if( mSelectedItem->getValue().repOrderBy.size() )
        txtOrder->setText( mSelectedItem->getValue().repOrderBy[0] );
    setCaption( toGUI(Xtring::printf( _("Informes de %s - %s"),
                                      DBAPP->getPackageString().c_str(),
                                      txtTitulo->toString().c_str() ) ) );
}

void FrmReports::pushOrder_clicked()
{
}

void FrmReports::pushFilter_clicked()
{
    FrmSQLSelect *sel = new FrmSQLSelect(DBAPP->getConnection(), DBAPP->getDatabase(),
                                         0, "filtrar");
    if( sel->exec(mSelectedItem->getValue().repFrom, txtFilter->toString() ) == QDialog::Accepted )
        txtFilter->setText(sel->getSqlExpression());
    delete sel;
}

void FrmReports::show()
{
    mSelectedItem = static_cast<ReportsTreeItem *>(treeReports->firstChild());
    if( mSelectedItem ) {
        if( mSelectedItem->firstChild() )
            mSelectedItem = static_cast<ReportsTreeItem *>(mSelectedItem->firstChild());
    }
    if( mSelectedItem )
        treeReports->setSelected(mSelectedItem, true);
    QWidget::show(); // FrmBase no tiene show
}

void FrmReports::show(const dbRecord *fic, const Xtring &filter)
{
    txtFilter->setText(filter);
    mSelectedItem = static_cast<ReportsTreeItem *>(treeReports->firstChild());
    if( mSelectedItem ) {
        stReportInfo repinfo;
        /// \todo {0.3.4} repinfo.repGroup = GongDatabase::getGroup(fic->getTableName());
        mSelectedItem = static_cast<ReportsTreeItem *>(treeReports->findItem(toGUI(repinfo.repGroup), 0));
    }
    if( mSelectedItem ) {
        mSelectedItem->setOpen(true);
        if( mSelectedItem->firstChild() )
            mSelectedItem = static_cast<ReportsTreeItem *>(mSelectedItem->firstChild());
    }
    if( mSelectedItem )
        treeReports->setSelected(mSelectedItem, true);
    QWidget::show(); // FrmBase no tiene show
}

void FrmReports::readAllReports()
{
    AppReport::readAllReports();
}

void FrmReports::refresh()
{
    readAllReports();
    mReportsRead = true;
}

void FrmReports::accept()
{
    if( mSelectedItem ) {
        AppReport *report = new AppReport(*DBAPP, DBAPP->getConnection());
        report->readFile(mSelectedItem->getValue().repPath.c_str(), chkResumido->isChecked() ? "RESUMIDO":"");
        Dictionary<Xtring> properties;
        properties.insert( "TITLE", txtTitulo->toString() );
        report->print( static_cast<RTK_Output_Type>(cmbSalida->currentItem()),
                       properties, txtFilter->toString(), txtOrder->toString(),
                       chkApaisado->isChecked() ? Landscape : DefaultOrientation,
                       chkAskParameters->isChecked());
        delete report;
    }
}

}



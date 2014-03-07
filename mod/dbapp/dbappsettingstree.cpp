#include <QtGui>
#include "dbappdbapplication.h"
#include "dbappsettingstree.h"

namespace gong {

class NoEditDelegate: public QStyledItemDelegate
{
public:
    NoEditDelegate(QObject* parent=0): QStyledItemDelegate(parent) {}
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        return 0;
    }
};


SettingsTree::SettingsTree(QWidget *parent)
    : QTreeWidget(parent)
{
    QStringList labels;
    labels << toGUI(_("Clave")) << toGUI(_("Por omisión")) << toGUI(_("Local")) << toGUI( _("Gobal") );
    setHeaderLabels(labels);
    header()->resizeSection(0, fontMetrics().width("W") * 20);
    header()->resizeSection(1, fontMetrics().width("W") * 15);
    header()->resizeSection(2, fontMetrics().width("W") * 15);
//	header()->setResizeMode(0, QHeaderView::Stretch);
//	header()->setResizeMode(2, QHeaderView::Stretch);
    setItemDelegateForColumn(0, new NoEditDelegate(this) );
    setItemDelegateForColumn(1, new NoEditDelegate(this) );
    if( DBAPP->isReadOnly() )
        setItemDelegateForColumn(2, new NoEditDelegate(this) );
    mRefreshTimer.setInterval(2000);
    autoRefresh = false;

    mGroupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    mGroupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    mKeyIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    connect(&mRefreshTimer, SIGNAL(timeout()), this, SLOT(maybeRefresh()));
    refresh();
}

QSize SettingsTree::sizeHint() const
{
    return QSize(900, 600);
}

void SettingsTree::setAutoRefresh(bool autoRefresh)
{
    this->autoRefresh = autoRefresh;
    if (autoRefresh) {
        maybeRefresh();
        mRefreshTimer.start();
    } else {
        mRefreshTimer.stop();
    }
}

void SettingsTree::setFallbacksEnabled(bool enabled)
{
    refresh();
}

void SettingsTree::maybeRefresh()
{
    if (state() != EditingState)
        refresh();
}

void SettingsTree::refresh()
{
    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(updateSetting(QTreeWidgetItem*)));

    updateChildItems(0);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateSetting(QTreeWidgetItem*)));
}

bool SettingsTree::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate) {
        if (isActiveWindow() && autoRefresh)
            maybeRefresh();
    }
    return QTreeWidget::event(event);
}

void SettingsTree::updateChildItems(QTreeWidgetItem *parent)
{
    int dividerIndex = 0;
    const SettingsDict &alluserlocalsettings = DBAPP->getMachineSettings()->allSettings();
    Dictionary<QTreeWidgetItem *> tables_items;
    QTreeWidgetItem *item_ddd = createItem("Diccionario de datos", 0, -1 );
    QTreeWidgetItem *item_styles = createItem("Estilos", item_ddd, -1 );
    QTreeWidgetItem *item_tables = createItem("Ficheros", item_ddd, -1 );
    QTreeWidgetItem *item_views = createItem("Vistas", item_ddd, -1 );
    QTreeWidgetItem *item_gui = createItem("Interfaz de la usuaria", 0, -1 );
    QTreeWidgetItem *item_rtk = createItem("Informes", 0, -1 );
    QTreeWidgetItem *item_modules = createItem("Módulos", 0, -1 );
    QTreeWidgetItem *item_system = createItem("Sistema", 0, -1 );
    QTreeWidgetItem *cur_group;
    for( SettingsDict::const_reverse_iterator it = alluserlocalsettings.rbegin();
            it != alluserlocalsettings.rend(); ++ it ) {
        Xtring key = it->first;
        if( key.startsWith( "DBDEF.VIEW." ) )
            cur_group = item_views;
        else if( key.startsWith( "DBDEF.STYLE." ) )
            cur_group = item_styles;
        else if( key.startsWith( "DBDEF.TABLE." ) || key.startsWith( "DBDEF.FIELD." ) ) {
            Xtring tablename = key.mid( 12 );
            tablename = tablename.left( tablename.find(".") );
            cur_group = tables_items[tablename];
            if( !cur_group ) {
                dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition(tablename);
                cur_group = createItem( tbldef ? tbldef->getDescPlural() : tablename, item_tables, -1);
                tables_items.insert( tablename, cur_group );
            }
        }
        else if( key.startsWith( "GUI." ) )
            cur_group = item_gui;
        else if( key.startsWith( "RTK." ) )
            cur_group = item_rtk;
        else if( key.startsWith( "MODULE." ) )
            cur_group = item_modules;
        else if( key.startsWith( "SYSTEM." ) )
            cur_group = item_system;
        else
            cur_group = 0;
        QTreeWidgetItem * newitem = createItem(key, cur_group, dividerIndex );
        newitem->setText( 1, toGUI(it->second.toString()) );
        Variant vlocalsetting = DBAPP->getUserLocalSetting( key );
        if( vlocalsetting.isValid() )
            newitem->setText( 2, toGUI(vlocalsetting.toString()) );
        Variant vglobalsetting = DBAPP->getGlobalSetting( key );
        if( vglobalsetting.isValid() )
            newitem->setText( 3, toGUI(vglobalsetting.toString()) );
    }
}

QTreeWidgetItem *SettingsTree::createItem(const Xtring &text,
        QTreeWidgetItem *parent, int index)
{
    QTreeWidgetItem *after = 0;
    if (index != 0)
        after = childAt(parent, index - 1);

    QTreeWidgetItem *item;
    if (parent)
        item = new QTreeWidgetItem(parent, after);
    else
        item = new QTreeWidgetItem(this, after);

    item->setText(0, toGUI(text) );
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    return item;
}

void SettingsTree::updateSetting(QTreeWidgetItem *item)
{
    Xtring key = fromGUI(item->text(0));
    if( item->text(3).length() == 0 )
        DBAPP->getGlobalSettings()->removeSetting( key );
    else
        DBAPP->getGlobalSettings()->setValue( key, fromGUI(item->text(3)) );
    if( item->text(2).length() == 0 )
        DBAPP->getUserLocalSettings()->removeSetting( key );
    else
        DBAPP->getUserLocalSettings()->setValue(key, fromGUI(item->text(2)));
    if (autoRefresh)
        refresh();
}


QTreeWidgetItem *SettingsTree::childAt(QTreeWidgetItem *parent, int index)
{
    if (parent)
        return parent->child(index);
    else
        return topLevelItem(index);
}

int SettingsTree::childCount(QTreeWidgetItem *parent)
{
    if (parent)
        return parent->childCount();
    else
        return topLevelItemCount();
}

int SettingsTree::findChild(QTreeWidgetItem *parent, const Xtring &text,
                            int startIndex)
{
    for (int i = startIndex; i < childCount(parent); ++i) {
        if (childAt(parent, i)->text(0) == toGUI(text))
            return i;
    }
    return -1;
}

void SettingsTree::moveItemForward(QTreeWidgetItem *parent, int oldIndex,
                                   int newIndex)
{
    for (int i = 0; i < oldIndex - newIndex; ++i)
        delete childAt(parent, newIndex);
}

QTreeWidgetItem *SettingsTree::findText( const Xtring &text, uint column, QTreeWidgetItem *start )
{
	QList<QTreeWidgetItem*> founditems = findItems( toGUI(text),
		static_cast<Qt::MatchFlags>(Qt::MatchContains + Qt::MatchRecursive), 0 );
	for( QList<QTreeWidgetItem*>::const_iterator it = founditems.begin();
		it != founditems.end(); ++it ) {
		if( *it == start ) {
			++it;
			if( it == founditems.end() )
				return 0;
			else
				return *it;
		}
	}
	return 0;
}

} // namespace gong

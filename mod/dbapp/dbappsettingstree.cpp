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
    labels << toGUI(_("Clave")) << toGUI(_("Valor por omisión")) << toGUI(_("Local")) << toGUI( _("Gobal")) << toGUI(_("Clave interna"));
    setHeaderLabels(labels);
    header()->resizeSection(0, fontMetrics().width("W") * 25); // Clave
    header()->resizeSection(1, fontMetrics().width("W") * 15); // Valor por omisión
    header()->resizeSection(2, fontMetrics().width("W") * 15); // Valor local
    header()->resizeSection(3, fontMetrics().width("W") * 25); // Valor Global
    setItemDelegateForColumn(0, new NoEditDelegate(this) );
    setItemDelegateForColumn(1, new NoEditDelegate(this) );
    if( DBAPP->isReadOnly() )  // No modificar la base de datos
        setItemDelegateForColumn(3, new NoEditDelegate(this) );
    setItemDelegateForColumn(4, new NoEditDelegate(this) );
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
        Xtring key = it->first, desc(key);
        if( key.startsWith( "DBDEF.VIEW." ) )
            cur_group = item_views;
        else if( key.startsWith( "DBDEF.STYLE." ) )
            cur_group = item_styles;
        else if( key.startsWith( "DBDEF.TABLE." ) ) {
            Xtring tablename = key.mid( 12 );
            tablename = tablename.left( tablename.find(".") );
            cur_group = tables_items[tablename];
            if( !cur_group ) {
                dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition(tablename);
                cur_group = createItem( tbldef ? tbldef->getDescPlural() : tablename, item_tables, -1);
                tables_items.insert( tablename, cur_group );
            }
        } else if( key.startsWith( "DBDEF.FIELD." ) ) {
			XtringList parts;
			key.tokenize(parts, ".");
			Xtring fullfldname = parts[2] + "." + parts[3];
            cur_group = tables_items[parts[2]];
			dbFieldDefinition *fld = DBAPP->getDatabase()->findFieldDefinition(fullfldname);
			if (fld) {
				desc = key.mid(12).replace(fullfldname, fld->getCaption());
				desc = desc.replace(".CANBENULL", _(".Puede estar vacío"));
				desc = desc.replace(".CAPTION",_(".Nombre"));
				desc = desc.replace(".DEFAULTVALUE",_(".Valor por defecto"));
				desc = desc.replace(".DESCRIPTION",_(".Descripción"));
				desc = desc.replace(".READONLY",_(".No modificar"));
				desc = desc.replace(".STYLE",_(".Estilo"));
				desc = desc.replace(".VISIBLE",_(".Visible"));
				desc = desc.replace(".WIDTH",_(".Longitud"));
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
        QTreeWidgetItem *newitem = createItem(desc, cur_group, dividerIndex );
		newitem->setText( 1, toGUI(it->second.toString()) );
        Variant vlocalsetting = DBAPP->getUserLocalSetting( key );
        if( vlocalsetting.isValid() )
            newitem->setText( 2, toGUI(vlocalsetting.toString()) );
        Variant vglobalsetting = DBAPP->getGlobalSetting( key );
        if( vglobalsetting.isValid() )
            newitem->setText( 3, toGUI(vglobalsetting.toString()) );
        newitem->setText( 4, key.c_str() ); // Oculto?
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
	bool changed = false;
    Xtring key = fromGUI(item->text(4));
	Xtring gvalue = fromGUI(item->text(3)).trim();
	if (DBAPP->getGlobalSettings()->getValue(key).toString().trim() != gvalue ) {
        DBAPP->getGlobalSettings()->setValue( key, gvalue );
		changed = true;
	}
	Xtring lvalue = fromGUI(item->text(2)).trim();
	if (DBAPP->getUserLocalSettings()->getValue(key).toString().trim() != lvalue ) {
        DBAPP->getUserLocalSettings()->setValue(key, lvalue );
		changed = true;
	}
	if( key.startsWith("DBDEF.") && changed ) {
		DBAPP->setDDDFromConfig(0); // TODO
		DBAPP->setStylesFromConfig(0);
	}
    if (autoRefresh && changed)
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
	QString search = toGUI(text).upper();
    for (int i = startIndex; i < childCount(parent); ++i) {
		_GONG_DEBUG_PRINT(0, fromGUI(childAt(parent, i)->text(0)));
        if (childAt(parent, i)->text(0).upper() == search)
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
	bool has_started = (start == 0);
    for( QList<QTreeWidgetItem*>::const_iterator it = founditems.begin();
            it != founditems.end(); ++it ) {
        if( !has_started ) 
			has_started = *it > start;
		if (has_started )
            return *it;
    }
    return 0;
}

} // namespace gong

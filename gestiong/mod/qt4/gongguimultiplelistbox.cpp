#include "gongguiapplication.h"
#include "gongguimultiplelistbox.h"


namespace gong {

MultipleListBoxItem::MultipleListBoxItem( long id, const Xtring &text,
        QListWidget *parent, bool checked )
    : QListWidgetItem( toGUI(text), parent ), mId( Xtring::number(id) )
{
    if ( checked )
        setCheckState( Qt::Checked );
    else
        setCheckState( Qt::Unchecked );
}

MultipleListBoxItem::MultipleListBoxItem( const Xtring &id, const Xtring &text,
        QListWidget *parent, bool checked )
    : QListWidgetItem( toGUI(text), parent ), mId(id)
{
    if ( checked )
        setCheckState( Qt::Checked );
    else
        setCheckState( Qt::Unchecked );
}

bool MultipleListBoxItem::isChecked() const
{
    return checkState() == Qt::Checked;
}

MultipleListBox::MultipleListBox(QWidget* parent): QListWidget(parent)
{
}

int MultipleListBox::getCheckedIds(XtringList& idslist) const
{
    idslist.clear();
    for(int row = 0; row < count(); row++)
    {
        MultipleListBoxItem *mlb = static_cast<MultipleListBoxItem *>( item(row) );
        if( mlb->isChecked() )
            idslist << mlb->getId();
    }
    return idslist.size();
}

int MultipleListBox::getCheckedIds(List<long>& idslist) const
{
    idslist.clear();
    for(int row = 0; row < count(); row++)
    {
        MultipleListBoxItem *mlb = static_cast<MultipleListBoxItem *>( item(row) );
        if( mlb->isChecked() )
            idslist << mlb->getId().toLong();
    }
    return idslist.size();
}

void MultipleListBox::addItem( long id, const Xtring &text, bool checked )
{
    new MultipleListBoxItem(id, text, this, checked );
}

void MultipleListBox::addItem( const Xtring &id, const Xtring &text, bool checked)
{
    new MultipleListBoxItem(id, text, this, checked );
}

} // namespace gong

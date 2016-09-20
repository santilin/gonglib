#include <gonggettext.h>
#include "dbappdbapplication.h"
#include "dbappdaterangebox.h"

namespace gong {

DateRangeBox::DateRangeBox( QWidget *parent, const Xtring& name, const Xtring& caption,
                            Variant::Type type, const DateTime date_from, const DateTime date_to,
                            const Xtring &caption_from, const Xtring &caption_to,
                            QBoxLayout *layout )
{
    if( !(pLayout = layout) ) {
        pLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        pLayout->setObjectName( toGUI("layout_" + name) );
    }
    QLabel *label = new QLabel( parent );
    label->setText ( toGUI ( caption.c_str() ) );
    pLayout->addWidget ( label );
    QLabel *label_from = new QLabel( parent );
    if( caption_from.isEmpty() )
        label_from->setText ( _("del") );
    else
        label_from->setText ( toGUI ( caption_from.c_str() ) );
    pLayout->addWidget ( label_from );
    pEditDateFrom = new LineEdit( parent, "Edit" + name + "FechaIni", type );
    pEditDateFrom->setMask( DBAPP->getRegConfig()->getDateMask() );
    pEditDateFrom->setFormat("%x");
    pEditDateFrom->setText( (Date)date_from );
    pLayout->addWidget( pEditDateFrom);
    QLabel *label_to = new QLabel( parent );
    if( caption_to.isEmpty() )
        label_to->setText ( _("al") );
    else
        label_to->setText ( toGUI ( caption_to.c_str() ) );
    pLayout->addWidget ( label_to );
    pEditDateTo = new LineEdit( parent, "Edit" + name + "FechaFin", type );
    pEditDateTo->setMask( DBAPP->getRegConfig()->getDateMask() );
    pEditDateTo->setFormat("%x");
    pEditDateTo->setText( (Date)date_to );
    pLayout->addWidget( pEditDateTo);
    QSpacerItem *spacer1 = new QSpacerItem ( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pLayout->addItem ( spacer1 );
}

Date DateRangeBox::getDateFrom() const
{
    return pEditDateFrom->toDate();
}

DateTime DateRangeBox::getDateTimeFrom() const
{
    return DateTime( getDateFrom(), Time(0,0,0) );
}

Date DateRangeBox::getDateTo() const
{
    return pEditDateTo->toDate() ;
}

DateTime DateRangeBox::getDateTimeTo() const
{
    return DateTime( getDateTo(), Time(23,59,59) );
}


} // namespace gong

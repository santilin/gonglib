#ifndef DBAPP_DATERANGEBOX_H
#define DBAPP_DATERANGEBOX_H

#include <gongguicontrols.h>

namespace gong {

class DateRangeBox
{
public:
    DateRangeBox( QWidget *parent, const Xtring &name, const Xtring &caption,
                  Variant::Type type, const DateTime date_from, const DateTime date_to,
                  const Xtring &caption_from, const Xtring &caption_to,
                  QBoxLayout *layout = 0);
    Date getDateFrom() const;
    DateTime getDateTimeFrom() const;
    Date getDateTo() const;
    DateTime getDateTimeTo() const;
    QBoxLayout *getLayout() const {
        return pLayout;
    }
    LineEdit *getEditDateFrom() const {
        return pEditDateFrom;
    }
    LineEdit *getEditDateTo() const {
        return pEditDateTo;
    }
private:
    LineEdit *pEditDateFrom, *pEditDateTo;
    QBoxLayout *pLayout;
};

} // namespace gong

#endif

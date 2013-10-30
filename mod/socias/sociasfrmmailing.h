#ifndef GONG_SOCIAS_MAILING_H
#define GONG_SOCIAS_MAILING_H

#include <contactosfrmmailing.h>

namespace gong {
namespace socias {

class FrmMailing: public contactos::FrmMailing
{
public:
    FrmMailing( QWidget* parent = 0, WidgetFlags fl = 0 );
    virtual int getEmailsList( XtringList &list, bool include_names ) const;
protected:
    SearchBox *pSearchMiembro;
};

} // namespace socias
} // namespace gong

#endif // GONG_SOCIAS_MAILING_H

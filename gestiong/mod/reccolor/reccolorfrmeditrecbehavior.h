#ifndef _RECCOLOR_FRMEDITRECBEHAVIOR_H
#define _RECCOLOR_FRMEDITRECBEHAVIOR_H

#include <QObject>
#include <dbappfrmeditrecbehavior.h>

class QMenu;

namespace gong {
namespace reccolor {

class ColorFrmEditRecBehavior: public FrmEditRecBehavior
{
    Q_OBJECT
public:
    ColorFrmEditRecBehavior( FrmEditRec *theform ): FrmEditRecBehavior( theform ) {}
    virtual void initGUI();
    virtual void scatterFields( bool is_pre );
    virtual void gatherFields( bool is_pre );
protected slots:
    virtual void menuRecColor_clicked();
private:
    QMenu *pMenuRecColor;
};

} // namespace reccolor
} // namespace gong

#endif // RECCOLOR_FRMEDITRECBEHAVIOR_H

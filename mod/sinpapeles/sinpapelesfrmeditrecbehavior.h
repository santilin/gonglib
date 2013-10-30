#ifndef _SINPAPELES_FRMEDITRECBEHAVIOR_H
#define _SINPAPELES_FRMEDITRECBEHAVIOR_H

#include <QObject>
#include <dbappfrmeditrecbehavior.h>

namespace gong {
namespace sinpapeles {

class SinPapelesFrmEditRecBehavior: public FrmEditRecBehavior
{
    Q_OBJECT
public:
    SinPapelesFrmEditRecBehavior( FrmEditRec *theform ): FrmEditRecBehavior( theform ) {}
    virtual void initGUI();
    virtual void scatterFields(bool is_pre );
    virtual void gatherFields(bool is_pre );
protected slots:
    virtual void slot_fnbox_clicked();
};

} // namespace sinpapeles
} // namespace gong

#endif // SINPAPELES_FRMEDITRECBEHAVIOR_H

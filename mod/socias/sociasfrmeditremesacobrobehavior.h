/*<<<<<COPYLEFT*/
/** @file sociasfrmeditremesacobrobehavior.h Behavior para editar cobros del módulo de socias
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<FRMEDITREMESACOBROBEHAVIOR_PREAMBLE*/
#ifndef _SOCIAS_SOCIASFRMEDITREMESACOBROBEHAVIOR_H
#define _SOCIAS_SOCIASFRMEDITREMESACOBROBEHAVIOR_H
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_PREAMBLE*/
/*<<<<<FRMEDITREMESACOBROBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_INCLUDES*/
#include <pagosrecremesacobro.h>

namespace gong {
namespace socias {

typedef pagos::RecRemesaCobro RecRemesaCobro;

/*<<<<<FRMEDITREMESACOBROBEHAVIOR_CLASS*/
class FrmEditRemesaCobroBehavior: public FrmEditRecBehavior
{
	Q_OBJECT
public:
	virtual void initGUI();
	RecRemesaCobro *getRecRemesaCobro() const { return static_cast<RecRemesaCobro *>(pTheForm->getRecord()); }
	FrmEditRemesaCobroBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_CLASS*/
    {}
    virtual void updateStatus();
    // virtual bool canClose();
    /*<<<<<FRMEDITREMESACOBROBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
    /*<<<<<FRMEDITREMESACOBROBEHAVIOR_CONTROLS*/
protected:
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_CONTROLS*/
    QPushButton *pushGenerarRecibo;
protected slots:
    void slotGenerarRecibo_clicked();
};
/*<<<<<FRMEDITREMESACOBROBEHAVIOR_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASFRMEDITREMESACOBROBEHAVIOR_H
/*>>>>>FRMEDITREMESACOBROBEHAVIOR_POSTAMBLE*/


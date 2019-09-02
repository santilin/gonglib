/*<<<<<COPYLEFT*/
/** @file ventaswebfrmeditarticulobehavior.h Behavior del formulario de artículos
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
/*<<<<<FRMEDITARTICULOBEHAVIOR_PREAMBLE*/
#ifndef _VENTASWEB_VENTASWEBFRMEDITARTICULOBEHAVIOR_H
#define _VENTASWEB_VENTASWEBFRMEDITARTICULOBEHAVIOR_H
/*>>>>>FRMEDITARTICULOBEHAVIOR_PREAMBLE*/
/*<<<<<FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITARTICULOBEHAVIOR_INCLUDES*/

#include <QObject>
#include <gongguicontrols.h>
#include <dbappfrmeditrec.h>
#include <facturecarticulo.h>

namespace gong {
namespace ventasweb {

typedef factu::RecArticulo RecArticulo;


/*<<<<<FRMEDITARTICULOBEHAVIOR_CLASS*/
class FrmEditArticuloBehavior: public FrmEditRecBehavior
{
	Q_OBJECT
public:
	virtual void initGUI();
	RecArticulo *getRecArticulo() const { return static_cast<RecArticulo *>(pTheForm->getRecord()); }
	FrmEditArticuloBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITARTICULOBEHAVIOR_CLASS*/
    {}
    /*<<<<<FRMEDITARTICULOBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITARTICULOBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
    /*<<<<<FRMEDITARTICULOBEHAVIOR_CONTROLS*/
protected:
	gong::CheckBox *checkPublicarWeb;
	gong::EditBox *editPVPWeb;
	gong::EditBox *editOrdenWeb;
/*>>>>>FRMEDITARTICULOBEHAVIOR_CONTROLS*/
    QPushButton *pushSubirImagenWeb;
protected slots:
    void slot_SubirImagenWeb();
};

/*<<<<<FRMEDITARTICULOBEHAVIOR_POSTAMBLE*/
} // namespace ventasweb
} // namespace gong
#endif // VENTASWEB_VENTASWEBFRMEDITARTICULOBEHAVIOR_H
/*>>>>>FRMEDITARTICULOBEHAVIOR_POSTAMBLE*/


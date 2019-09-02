/*<<<<<COPYLEFT*/
/** @file produccionfrmeditarticulobehavior.h Behavior del formulario de familias
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
#ifndef _PRODUCCION_PRODUCCIONFRMEDITARTICULOBEHAVIOR_H
#define _PRODUCCION_PRODUCCIONFRMEDITARTICULOBEHAVIOR_H
/*>>>>>FRMEDITARTICULOBEHAVIOR_PREAMBLE*/

/*<<<<<FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include "produccionfrmeditarticulocomponente.h"
#include "produccionrecarticulo.h"

namespace gong {
namespace produccion {

class RecLote;

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
	virtual void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITARTICULOBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
	virtual void updateFromDetails( class FrmEditRecDetail *frmdetail );
	RecLote *getRecLote() { return getRecArticulo()->getRecLote(); }
/*<<<<<FRMEDITARTICULOBEHAVIOR_SCATTERS_AND_SLOTS*/
protected:
	void scatterLote();

private slots:
	void pushLoteCodigo_clicked();

public:
/*>>>>>FRMEDITARTICULOBEHAVIOR_SCATTERS_AND_SLOTS*/

/*<<<<<FRMEDITARTICULOBEHAVIOR_CONTROLS*/
protected:
	gong::SearchBox *searchLoteCodigo;
	QPushButton *pushLoteCodigo;
	gong::LineEdit *editLoteCodigo;
	gong::LineEdit *editLoteDescripcion;
	FrmEditArticuloComponente *pFrmArticuloComponente;
/*>>>>>FRMEDITARTICULOBEHAVIOR_CONTROLS*/
};
/*<<<<<FRMEDITARTICULOBEHAVIOR_POSTAMBLE*/
} // namespace produccion
} // namespace gong
#endif // PRODUCCION_PRODUCCIONFRMEDITARTICULOBEHAVIOR_H
/*>>>>>FRMEDITARTICULOBEHAVIOR_POSTAMBLE*/


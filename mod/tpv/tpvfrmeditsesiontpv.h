/*<<<<<COPYLEFT*/
/** @file tpvfrmeditsesiontpv.h Fichero de edición de sesiones de TPV
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
/*<<<<<FRMEDITSESIONTPV_PREAMBLE*/
#ifndef _TPV_TPVFRMEDITSESIONTPV_H
#define _TPV_TPVFRMEDITSESIONTPV_H
/*>>>>>FRMEDITSESIONTPV_PREAMBLE*/
/*<<<<<FRMEDITSESIONTPV_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "tpvrecsesiontpv.h"
/*>>>>>FRMEDITSESIONTPV_INCLUDES*/

namespace gong {

namespace factu {
	class RecCliente;
}

namespace tpv {

typedef factu::RecCliente RecCliente;

/*<<<<<FRMEDITSESIONTPV_CLASS*/
class FrmEditSesionTPV: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditSesionTPV(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecSesionTPV *getRecSesionTPV() const { return (RecSesionTPV *)getRecord(); }
/*>>>>>FRMEDITSESIONTPV_CLASS*/
	virtual ~FrmEditSesionTPV();

/*<<<<<FRMEDITSESIONTPV_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITSESIONTPV_VIRTUALS_FROM_FRMEDITREC*/
	void scatterCliente();
	void updateStatus( bool callbehaviors );
	RecCliente* getRecCliente() const { return pRecCliente; }
	void specialControlKeyPressed(QWidget *sender, char key);
protected slots:
	void slotPushReabrir();
	void slotPushCerrar();
	void slotPushZ();
	void pushClienteCodigo_clicked();
/*<<<<<FRMEDITSESIONTPV_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNumeroTPV;
	gong::EditBox *editInicio;
	gong::EditBox *editFin;
	gong::EditBox *editCajaInicial;
	gong::EditBox *editMesasSVG;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITSESIONTPV_CONTROLS*/
	gong::SearchBox *searchClienteCodigo;
	QPushButton *pushClienteCodigo;
	gong::LineEdit *editClienteCodigo;
	gong::LineEdit *editClienteRazonSocial;
	class PushButton *pushReabrir, *pushCerrar, *pushZ;
	RecCliente *pRecCliente;
};
/*<<<<<FRMEDITSESIONTPV_POSTAMBLE*/
} // namespace tpv
} // namespace gong
#endif // TPV_TPVFRMEDITSESIONTPV_H
/*>>>>>FRMEDITSESIONTPV_POSTAMBLE*/


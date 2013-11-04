/*<<<<<FRMEDITREMESACOBRO_PREAMBLE*/
#ifndef _PAGOS_PAGOSFRMEDITREMESACOBRO_H
#define _PAGOS_PAGOSFRMEDITREMESACOBRO_H
/*>>>>>FRMEDITREMESACOBRO_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file pagosfrmeditremesacobro.h Fichero de edición de remesas de recibos de proyectos
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

/*<<<<<FRMEDITREMESACOBRO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "pagosrecremesacobro.h"
#include "pagosfrmeditcobro.h"
/*>>>>>FRMEDITREMESACOBRO_INCLUDES*/

namespace gong {
namespace pagos {

/*<<<<<FRMEDITREMESACOBRO_CLASS*/
class FrmEditRemesaCobro: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditRemesaCobro(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecRemesaCobro *getRecRemesaCobro() const { return (RecRemesaCobro *)getRecord(); }
/*>>>>>FRMEDITREMESACOBRO_CLASS*/
    /*<<<<<FRMEDITREMESACOBRO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITREMESACOBRO_VIRTUALS_FROM_FRMEDITREC*/
    bool usarProyecto() const;
    /*<<<<<FRMEDITREMESACOBRO_SCATTERS_AND_SLOTS*/
protected:
	void scatterProyecto();

private slots:
	void pushProyectoCodigo_clicked();

public:
	empresa::RecProyecto* getRecProyecto() const
		{ return static_cast<RecRemesaCobro*>(getRecord())->getRecProyecto(); }
/*>>>>>FRMEDITREMESACOBRO_SCATTERS_AND_SLOTS*/
    /*<<<<<FRMEDITREMESACOBRO_CONTROLS*/
protected:
	gong::EditBox *editNumero;
	gong::EditBox *editDescripcion;
	gong::SearchBox *searchProyectoCodigo;
	QPushButton *pushProyectoCodigo;
	gong::LineEdit *editProyectoCodigo;
	gong::LineEdit *editProyectoNombre;
	gong::CheckBox *checkCerrada;
	gong::EditBox *editFechaEmision;
	gong::EditBox *editFechaCargo;
	gong::EditBox *editCuentaBancoAbono;
	gong::EditBox *editNRecibos;
	gong::EditBox *editImporte;
	gong::EditBox *editResto;
	gong::TextBox *editNotas;
	FrmEditCobro *pFrmCobro;
/*>>>>>FRMEDITREMESACOBRO_CONTROLS*/
    QPushButton *pushGenerarCB19;
    QWidget *pTabRecibos;
private slots:
    void pushGenerarCB19_clicked();
protected:
    virtual void updateStatus(); // from FrmEditRecMaster
    void actTotales();
    virtual void refresh(); // From FrmEditRecMaster
};

/*<<<<<FRMEDITREMESACOBRO_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSFRMEDITREMESACOBRO_H
/*>>>>>FRMEDITREMESACOBRO_POSTAMBLE*/



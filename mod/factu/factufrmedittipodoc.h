/*<<<<<COPYLEFT*/
/** @file factufrmedittipodoc.h Fichero de edición de tipos de facturas
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
/*<<<<<FRMEDITTIPODOC_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITTIPODOC_H
#define _FACTU_FACTUFRMEDITTIPODOC_H
/*>>>>>FRMEDITTIPODOC_PREAMBLE*/

#include "config.h"

/*<<<<<FRMEDITTIPODOC_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturectipodoc.h"
/*>>>>>FRMEDITTIPODOC_INCLUDES*/
#include <empresarecproyecto.h>

namespace gong {
namespace factu {

/*<<<<<FRMEDITTIPODOC_CLASS*/
class FrmEditTipoDoc: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditTipoDoc(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecTipoDoc *getRecTipoDoc() const { return (RecTipoDoc *)getRecord(); }
/*>>>>>FRMEDITTIPODOC_CLASS*/
    /*<<<<<FRMEDITTIPODOC_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITTIPODOC_VIRTUALS_FROM_FRMEDITREC*/

    /*<<<<<FRMEDITTIPODOC_SCATTERS_AND_SLOTS*/
protected:
	void scatterFormaPago();
	void scatterAgente();
	void scatterProyecto();

private slots:
	void pushFormaPagoCodigo_clicked();
	void pushAgenteCodigo_clicked();
	void pushProyectoCodigo_clicked();

public:
	pagos::RecFormaPago* getRecFormaPago() const
		{ return static_cast<RecTipoDoc*>(getRecord())->getRecFormaPago(); }
	RecAgente* getRecAgente() const
		{ return static_cast<RecTipoDoc*>(getRecord())->getRecAgente(); }
	empresa::RecProyecto* getRecProyecto() const
		{ return static_cast<RecTipoDoc*>(getRecord())->getRecProyecto(); }
/*>>>>>FRMEDITTIPODOC_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITTIPODOC_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::EditBox *editFormato;
	gong::EditBox *editSerie;
	gong::ComboBoxInt *comboIVADetallado;
	gong::SearchBox *searchFormaPagoCodigo;
	QPushButton *pushFormaPagoCodigo;
	gong::LineEdit *editFormaPagoCodigo;
	gong::LineEdit *editFormaPagoNombre;
	gong::SearchBox *searchAgenteCodigo;
	QPushButton *pushAgenteCodigo;
	gong::LineEdit *editAgenteCodigo;
	gong::LineEdit *editAgenteRazonSocial;
	gong::SearchBox *searchProyectoCodigo;
	QPushButton *pushProyectoCodigo;
	gong::LineEdit *editProyectoCodigo;
	gong::LineEdit *editProyectoNombre;
	gong::CheckBox *checkActualizaStocks;
#ifdef HAVE_CONTABMODULE
	gong::CheckBox *checkNoGeneraASiento;
#endif
	gong::CheckBox *checkNoFacturable;
/*>>>>>FRMEDITTIPODOC_CONTROLS*/
};
/*<<<<<FRMEDITTIPODOC_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITTIPODOC_H
/*>>>>>FRMEDITTIPODOC_POSTAMBLE*/


/*<<<<<FRMEDITAPUNTETESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIAFRMEDITAPUNTETESORERIA_H
#define _TESORERIA_TESORERIAFRMEDITAPUNTETESORERIA_H
/*>>>>>FRMEDITAPUNTETESORERIA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file tesoreriafrmeditapuntetesoreria.h Fichero de edición de
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

/*<<<<<FRMEDITAPUNTETESORERIA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "tesoreriarecapuntetesoreria.h"
/*>>>>>FRMEDITAPUNTETESORERIA_INCLUDES*/
#include <empresarecproyecto.h>

namespace gong {
namespace tesoreria {

/*<<<<<FRMEDITAPUNTETESORERIA_CLASS*/
class FrmEditApunteTesoreria: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditApunteTesoreria(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecApunteTesoreria *getRecApunteTesoreria() const { return (RecApunteTesoreria *)getRecord(); }
/*>>>>>FRMEDITAPUNTETESORERIA_CLASS*/
    /*<<<<<FRMEDITAPUNTETESORERIA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITAPUNTETESORERIA_VIRTUALS_FROM_FRMEDITREC*/
    void changeTipoApunte();

    /*<<<<<FRMEDITAPUNTETESORERIA_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoApunteTesoreria();
	void scatterCuentaTesoreria();
	void scatterProyecto();

private slots:
	void pushTipoApunteTesoreriaCodigo_clicked();
	void pushCuentaTesoreriaCodigo_clicked();
	void pushProyectoCodigo_clicked();

public:
	RecTipoApunteTesoreria* getRecTipoApunteTesoreria() const
		{ return static_cast<RecApunteTesoreria*>(getRecord())->getRecTipoApunteTesoreria(); }
	RecCuentaTesoreria* getRecCuentaTesoreria() const
		{ return static_cast<RecApunteTesoreria*>(getRecord())->getRecCuentaTesoreria(); }
	empresa::RecProyecto* getRecProyecto() const
		{ return static_cast<RecApunteTesoreria*>(getRecord())->getRecProyecto(); }
/*>>>>>FRMEDITAPUNTETESORERIA_SCATTERS_AND_SLOTS*/
    RecTercero* getRecTercero() const
    {
        return static_cast<RecApunteTesoreria*>(getRecord())->getRecTercero();
    }
    RecConcepto* getRecConcepto() const
    {
        return static_cast<RecApunteTesoreria*>(getRecord())->getRecConcepto();
    }
protected:
    void scatterTercero();
    void scatterConcepto();
private slots:
    void pushTerceroCodigo_clicked();
    void pushConceptoCodigo_clicked();


/*<<<<<FRMEDITAPUNTETESORERIA_CONTROLS*/
protected:
	gong::EditBox *editNumero;
	gong::CheckBox *checkAutomatico;
	gong::SearchBox *searchTipoApunteTesoreriaCodigo;
	QPushButton *pushTipoApunteTesoreriaCodigo;
	gong::LineEdit *editTipoApunteTesoreriaCodigo;
	gong::LineEdit *editTipoApunteTesoreriaNombre;
	gong::CheckBox *checkCargo;
	gong::EditBox *editFecha;
	gong::EditBox *editImporte;
	gong::SearchBox *searchCuentaTesoreriaCodigo;
	QPushButton *pushCuentaTesoreriaCodigo;
	gong::LineEdit *editCuentaTesoreriaCodigo;
	gong::LineEdit *editCuentaTesoreriaNombre;
	gong::ComboBoxXtring *comboTablaTerceros;
	gong::EditBox *editTercero;
	gong::ComboBoxXtring *comboTablaConceptos;
	gong::EditBox *editConcepto;
	gong::EditBox *editReferencia;
	gong::SearchBox *searchProyectoCodigo;
	QPushButton *pushProyectoCodigo;
	gong::LineEdit *editProyectoCodigo;
	gong::LineEdit *editProyectoNombre;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITAPUNTETESORERIA_CONTROLS*/
    QBoxLayout *pTercerosLayout, *pConceptosLayout;
    gong::SearchBox *searchTerceroCodigo;
    QPushButton *pushTerceroCodigo;
    gong::LineEdit *editTerceroCodigo;
    gong::LineEdit *editTerceroNombre;
    gong::SearchBox *searchConceptoCodigo;
    QPushButton *pushConceptoCodigo;
    gong::LineEdit *editConceptoCodigo;
    gong::LineEdit *editConceptoNombre;
    Xtring mFldConcCodigo, mFldConcDesc, mFldTercCodigo, mFldTercDesc;
};

/*<<<<<FRMEDITAPUNTETESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIAFRMEDITAPUNTETESORERIA_H
/*>>>>>FRMEDITAPUNTETESORERIA_POSTAMBLE*/


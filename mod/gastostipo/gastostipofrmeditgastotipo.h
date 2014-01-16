/*<<<<<COPYLEFT*/
/** @file gastostipofrmeditgastotipo.h Fichero de edición de definiciones de gastos e ingresos
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
/*<<<<<FRMEDITGASTOTIPO_PREAMBLE*/
#ifndef _GASTOSTIPO_GASTOSTIPOFRMEDITGASTOTIPO_H
#define _GASTOSTIPO_GASTOSTIPOFRMEDITGASTOTIPO_H
/*>>>>>FRMEDITGASTOTIPO_PREAMBLE*/
/*<<<<<FRMEDITGASTOTIPO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "gastostiporecgastotipo.h"
/*>>>>>FRMEDITGASTOTIPO_INCLUDES*/



namespace gong {
namespace factu {
	class RecCliente;
	class RecProveedora;
	class RecArticulo;
}
namespace contab {
	class RecCuenta;
	typedef RecCuenta RecCuentaTercero;
	typedef RecCuenta RecCuentaConcepto;
}
namespace gastostipo {

/*<<<<<FRMEDITGASTOTIPO_CLASS*/
class FrmEditGastoTipo: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditGastoTipo(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecGastoTipo *getRecGastoTipo() const { return (RecGastoTipo *)getRecord(); }
/*>>>>>FRMEDITGASTOTIPO_CLASS*/
/*<<<<<FRMEDITGASTOTIPO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITGASTOTIPO_VIRTUALS_FROM_FRMEDITREC*/
	void updateStatus( bool callbehaviors );
	bool hasFactu() const;
	bool hasContab() const;

protected slots:
	virtual void slotUtilizar();

/*<<<<<FRMEDITGASTOTIPO_SCATTERS_AND_SLOTS*/
protected:
	void scatterCliente();
	void scatterProveedora();
	void scatterCuentaTercero();
	void scatterArticulo();
	void scatterCuentaConcepto();

private slots:
	void pushClienteCodigo_clicked();
	void pushProveedoraCodigo_clicked();
	void pushCuentaTerceroCuenta_clicked();
	void pushArticuloCodigo_clicked();
	void pushCuentaConceptoCuenta_clicked();

public:
	factu::RecCliente* getRecCliente() const;
	factu::RecProveedora* getRecProveedora() const;
	contab::RecCuentaTercero* getRecCuentaTercero() const;
	factu::RecArticulo* getRecArticulo() const;
	contab::RecCuentaConcepto* getRecCuentaConcepto() const;
/*>>>>>FRMEDITGASTOTIPO_SCATTERS_AND_SLOTS*/
// {capel} Borrar implementación de los getRecXXXX

/*<<<<<FRMEDITGASTOTIPO_CONTROLS*/
protected:
	gong::CheckBox *checkEsGasto;
	gong::EditBox *editNombre;
	gong::CheckBox *checkRevisar;
	gong::ComboBoxInt *comboCategoriaGasto;
	gong::ComboBoxXtring *comboTableName;
	gong::ComboBoxInt *comboPedirTercero;
	gong::SearchBox *searchClienteCodigo;
	QPushButton *pushClienteCodigo;
	gong::LineEdit *editClienteCodigo;
	gong::LineEdit *editClienteRazonSocial;
	gong::SearchBox *searchProveedoraCodigo;
	QPushButton *pushProveedoraCodigo;
	gong::LineEdit *editProveedoraCodigo;
	gong::LineEdit *editProveedoraRazonSocial;
	gong::SearchBox *searchCuentaTerceroCuenta;
	QPushButton *pushCuentaTerceroCuenta;
	gong::LineEdit *editCuentaTerceroCuenta;
	gong::LineEdit *editCuentaTerceroDescripcion;
	gong::ComboBoxInt *comboPedirConcepto;
	gong::SearchBox *searchArticuloCodigo;
	QPushButton *pushArticuloCodigo;
	gong::LineEdit *editArticuloCodigo;
	gong::LineEdit *editArticuloNombre;
	gong::SearchBox *searchCuentaConceptoCuenta;
	QPushButton *pushCuentaConceptoCuenta;
	gong::LineEdit *editCuentaConceptoCuenta;
	gong::LineEdit *editCuentaConceptoDescripcion;
	gong::ComboBoxInt *comboPedirImporte;
	gong::EditBox *editImporte;
	gong::ComboBoxInt *comboPedirDescripcion;
	gong::EditBox *editDescripcion;
	gong::ComboBoxInt *comboPedirNotas;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITGASTOTIPO_CONTROLS*/
	QPushButton *pushUtilizar;
};
/*<<<<<FRMEDITGASTOTIPO_POSTAMBLE*/
} // namespace gastostipo
} // namespace gong
#endif // GASTOSTIPO_GASTOSTIPOFRMEDITGASTOTIPO_H
/*>>>>>FRMEDITGASTOTIPO_POSTAMBLE*/


/*<<<<<COPYLEFT*/
/** @file factufrmeditalbaranventa.h Fichero de edición de albaranes de venta
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
/*<<<<<FRMEDITALBARANVENTA_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITALBARANVENTA_H
#define _FACTU_FACTUFRMEDITALBARANVENTA_H
/*>>>>>FRMEDITALBARANVENTA_PREAMBLE*/
/*<<<<<FRMEDITALBARANVENTA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturecalbaranventa.h"
#include "factufrmeditalbaranventadet.h"
/*>>>>>FRMEDITALBARANVENTA_INCLUDES*/
#include <empresarecempresa.h>

#ifdef HAVE_CONTABMODULE
#include <contabreccuenta.h>
typedef gong::contab::RecCuenta RecCuentaPago;
#endif

namespace gong {
namespace factu {

/*<<<<<FRMEDITALBARANVENTA_CLASS*/
class FrmEditAlbaranVenta: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditAlbaranVenta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecAlbaranVenta *getRecAlbaranVenta() const { return (RecAlbaranVenta *)getRecord(); }
/*>>>>>FRMEDITALBARANVENTA_CLASS*/

    /*<<<<<FRMEDITALBARANVENTA_CABECERA_DECL*/
protected:
	void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
	void actTotales();
	void numeraLineas();
	void genNumeroDocumento();
public:
	int getIvaDetallado() const { return comboIVADetallado->getCurrentItemValue(); }
/*>>>>>FRMEDITALBARANVENTA_CABECERA_DECL*/
    virtual bool canBeginEdit( EditMode newmode); // from FrmEditRecMaster

    /*<<<<<FRMEDITALBARANVENTA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITALBARANVENTA_VIRTUALS_FROM_FRMEDITREC*/

protected slots:
    void slotCobrar();

    /*<<<<<FRMEDITALBARANVENTA_CONTROLS*/
protected:
	gong::EditBox *editFecha;
	gong::SearchBox *searchTipoDocCodigo;
	QPushButton *pushTipoDocCodigo;
	gong::LineEdit *editTipoDocCodigo;
	gong::LineEdit *editTipoDocNombre;
	gong::EditBox *editContador;
	gong::EditBox *editNumero;
	gong::SearchBox *searchClienteCodigo;
	QPushButton *pushClienteCodigo;
	gong::LineEdit *editClienteCodigo;
	gong::LineEdit *editClienteRazonSocial;
	gong::ComboBoxInt *comboIVADetallado;
	gong::CheckBox *checkFacturado;
	gong::SearchBox *searchFormaPagoCodigo;
	QPushButton *pushFormaPagoCodigo;
	gong::LineEdit *editFormaPagoCodigo;
	gong::LineEdit *editFormaPagoNombre;
	gong::SearchBox *searchAgenteCodigo;
	QPushButton *pushAgenteCodigo;
	gong::LineEdit *editAgenteCodigo;
	gong::LineEdit *editAgenteRazonSocial;
	FrmEditAlbaranVentaDet *pFrmAlbaranVentaDet;
	gong::CheckBox *checkNoFacturable;
	gong::EditBox *editSumaImportes;
	gong::EditBox *editDtoP100;
	gong::EditBox *editDescuento;
	gong::EditBox *editBaseImponible;
	gong::EditBox *editRecargoEquivalencia;
	gong::EditBox *editIVA;
	gong::EditBox *editTotal;
	gong::EditBox *editCobros;
	gong::EditBox *editEntrega;
	gong::EditBox *editResto;
	gong::SearchBox *searchProyectoCodigo;
	QPushButton *pushProyectoCodigo;
	gong::LineEdit *editProyectoCodigo;
	gong::LineEdit *editProyectoNombre;
	gong::EditBox *editDesgloseIVA;
	gong::EditBox *editDocumentoPago;
	gong::EditBox *editFechaPago;
#ifdef HAVE_CONTABMODULE
	gong::SearchBox *searchCuentaPagoCuenta;
	QPushButton *pushCuentaPagoCuenta;
	gong::LineEdit *editCuentaPagoCuenta;
	gong::LineEdit *editCuentaPagoDescripcion;
#endif
	gong::TextBox *editNotas;
/*>>>>>FRMEDITALBARANVENTA_CONTROLS*/
    QPushButton *pushCobrar;
protected:
    bool mHasCobros;
    Money mOldTotal;

    /*<<<<<FRMEDITALBARANVENTA_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoDoc();
	void scatterCliente();
	void scatterFormaPago();
	void scatterAgente();
	void scatterProyecto();
	void scatterCuentaPago();

private slots:
	void pushTipoDocCodigo_clicked();
	void pushClienteCodigo_clicked();
	void pushFormaPagoCodigo_clicked();
	void pushAgenteCodigo_clicked();
	void pushProyectoCodigo_clicked();
	void pushCuentaPagoCuenta_clicked();

public:
	RecTipoDoc* getRecTipoDoc() const
		{ return static_cast<RecAlbaranVenta*>(getRecord())->getRecTipoDoc(); }
	RecCliente* getRecCliente() const
		{ return static_cast<RecAlbaranVenta*>(getRecord())->getRecCliente(); }
	pagos::RecFormaPago* getRecFormaPago() const
		{ return static_cast<RecAlbaranVenta*>(getRecord())->getRecFormaPago(); }
	RecAgente* getRecAgente() const
		{ return static_cast<RecAlbaranVenta*>(getRecord())->getRecAgente(); }
	empresa::RecProyecto* getRecProyecto() const
		{ return static_cast<RecAlbaranVenta*>(getRecord())->getRecProyecto(); }
#ifdef HAVE_CONTABMODULE
	RecCuentaPago* getRecCuentaPago() const
		{ return static_cast<RecAlbaranVenta*>(getRecord())->getRecCuentaPago(); }
#endif
/*>>>>>FRMEDITALBARANVENTA_SCATTERS_AND_SLOTS*/
};

/*<<<<<FRMEDITALBARANVENTA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITALBARANVENTA_H
/*>>>>>FRMEDITALBARANVENTA_POSTAMBLE*/



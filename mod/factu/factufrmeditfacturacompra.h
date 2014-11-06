/*<<<<<COPYLEFT*/
/** @file factufrmeditfacturacompra.h Fichero de edición de facturas de compra
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
/*<<<<<FRMEDITFACTURACOMPRA_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITFACTURACOMPRA_H
#define _FACTU_FACTUFRMEDITFACTURACOMPRA_H
/*>>>>>FRMEDITFACTURACOMPRA_PREAMBLE*/
/*<<<<<FRMEDITFACTURACOMPRA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturecfacturacompra.h"
#include "factufrmeditfacturacompradet.h"
/*>>>>>FRMEDITFACTURACOMPRA_INCLUDES*/

#ifdef HAVE_CONTABMODULE
#include <contabreccuenta.h>
typedef gong::contab::RecCuenta RecCuentaPago;
#include "factuiasentables.h"
#endif

namespace gong {
namespace factu {

/*<<<<<FRMEDITFACTURACOMPRA_CLASS*/
class FrmEditFacturaCompra: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditFacturaCompra(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecFacturaCompra *getRecFacturaCompra() const { return (RecFacturaCompra *)getRecord(); }
/*>>>>>FRMEDITFACTURACOMPRA_CLASS*/

    bool canBeginEdit(EditMode newmode);

    /*<<<<<FRMEDITFACTURACOMPRA_CABECERA_DECL*/
protected:
	void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
	void actTotales();
	void numeraLineas();
	void genNumeroDocumento();
public:
	int getIvaDetallado() const { return comboIVADetallado->getCurrentItemValue(); }
/*>>>>>FRMEDITFACTURACOMPRA_CABECERA_DECL*/

    /*<<<<<FRMEDITFACTURACOMPRA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITFACTURACOMPRA_VIRTUALS_FROM_FRMEDITREC*/

protected slots:
    void slotPagar();

    /*<<<<<FRMEDITFACTURACOMPRA_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoDoc();
	void scatterProveedora();
	void scatterFormaPago();
	void scatterProyecto();
	void scatterCuentaPago();

private slots:
	void pushTipoDocCodigo_clicked();
	void pushProveedoraCodigo_clicked();
	void pushFormaPagoCodigo_clicked();
	void pushProyectoCodigo_clicked();
	void pushCuentaPagoCuenta_clicked();

public:
	RecTipoDoc* getRecTipoDoc() const
		{ return static_cast<RecFacturaCompra*>(getRecord())->getRecTipoDoc(); }
	RecProveedora* getRecProveedora() const
		{ return static_cast<RecFacturaCompra*>(getRecord())->getRecProveedora(); }
	pagos::RecFormaPago* getRecFormaPago() const
		{ return static_cast<RecFacturaCompra*>(getRecord())->getRecFormaPago(); }
	empresa::RecProyecto* getRecProyecto() const
		{ return static_cast<RecFacturaCompra*>(getRecord())->getRecProyecto(); }
#ifdef HAVE_CONTABMODULE
	RecCuentaPago* getRecCuentaPago() const
		{ return static_cast<RecFacturaCompra*>(getRecord())->getRecCuentaPago(); }
#endif
/*>>>>>FRMEDITFACTURACOMPRA_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITFACTURACOMPRA_CONTROLS*/
protected:
	gong::EditBox *editFecha;
	gong::SearchBox *searchTipoDocCodigo;
	QPushButton *pushTipoDocCodigo;
	gong::LineEdit *editTipoDocCodigo;
	gong::LineEdit *editTipoDocNombre;
	gong::EditBox *editNumero;
	gong::ComboBoxInt *comboIVADetallado;
	gong::SearchBox *searchProveedoraCodigo;
	QPushButton *pushProveedoraCodigo;
	gong::LineEdit *editProveedoraCodigo;
	gong::LineEdit *editProveedoraRazonSocial;
	gong::SearchBox *searchFormaPagoCodigo;
	QPushButton *pushFormaPagoCodigo;
	gong::LineEdit *editFormaPagoCodigo;
	gong::LineEdit *editFormaPagoNombre;
	gong::EditBox *editContador;
	FrmEditFacturaCompraDet *pFrmFacturaCompraDet;
	gong::EditBox *editSumaImportes;
	gong::EditBox *editDtoP100;
	gong::EditBox *editDescuento;
	gong::EditBox *editBaseImponible;
	gong::EditBox *editRecargoEquivalencia;
	gong::EditBox *editIVA;
	gong::EditBox *editTotal;
	gong::EditBox *editPagos;
	gong::EditBox *editEntregaAlbaranes;
	gong::EditBox *editResto;
	gong::EditBox *editEntrega;
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
/*>>>>>FRMEDITFACTURACOMPRA_CONTROLS*/
    QPushButton *pushPagar;
protected:
    bool mHasPagos;
    Money mOldTotal;
};
/*<<<<<FRMEDITFACTURACOMPRA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITFACTURACOMPRA_H
/*>>>>>FRMEDITFACTURACOMPRA_POSTAMBLE*/

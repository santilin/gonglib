/*<<<<<COPYLEFT*/
/** @file pagosfrmeditpago.h Fichero de edición de pagos
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
/*<<<<<FRMEDITPAGO_PREAMBLE*/
#ifndef _PAGOS_PAGOSFRMEDITPAGO_H
#define _PAGOS_PAGOSFRMEDITPAGO_H
/*>>>>>FRMEDITPAGO_PREAMBLE*/

/*<<<<<FRMEDITPAGO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "pagosrecpago.h"
/*>>>>>FRMEDITPAGO_INCLUDES*/

#ifdef HAVE_CONTABMODULE
#include <contabreccuenta.h>
namespace gong {
namespace contab {
typedef RecCuenta RecCuentaPago;
}
}
#else
typedef gong::dbRecord RecCuentaPago;
#endif

namespace gong {
namespace pagos {

typedef dbRecord RecFactura;
typedef dbRecord RecTercero;

/*<<<<<FRMEDITPAGO_CLASS*/
class FrmEditPago: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditPago(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecPago *getRecPago() const { return (RecPago *)getRecord(); }
/*>>>>>FRMEDITPAGO_CLASS*/
protected:
    void completa(const Xtring &tablafacturas, const Xtring &fldfactcodigo, const Xtring &fldfactdesc,
                  const Xtring &tablaterceros, const Xtring &fldterccodigo, const Xtring &fldtercdesc,
                  const Xtring &descsingular, const Xtring &descplural, bool femenina );
    virtual void updateStatus( bool callbehaviors ); // From FrmEditRec
    virtual bool canBeginEdit(EditMode newmode); // From FrmEditRec

    /*<<<<<FRMEDITPAGO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITPAGO_VIRTUALS_FROM_FRMEDITREC*/

protected slots:
    virtual void pushPagar_clicked();
    void pushFacturaNumero_clicked();
    void pushTerceroCodigo_clicked();
protected:
    void scatterFactura();
    void scatterTercero();

    /*<<<<<FRMEDITPAGO_SCATTERS_AND_SLOTS*/
protected:
	void scatterMoneda();
	void scatterCuentaPago();

private slots:
	void pushMonedaCodigo_clicked();
	void pushCuentaPagoCuenta_clicked();

public:
	empresa::RecMoneda* getRecMoneda() const
		{ return static_cast<RecPago*>(getRecord())->getRecMoneda(); }
#ifdef HAVE_CONTABMODULE
	contab::RecCuentaPago* getRecCuentaPago() const
		{ return static_cast<RecPago*>(getRecord())->getRecCuentaPago(); }
#endif
/*>>>>>FRMEDITPAGO_SCATTERS_AND_SLOTS*/
public:
    RecFactura *getRecFactura() const
    {
        return static_cast<RecPago*>(getRecord())->getRecFactura();
    }
    RecTercero *getRecTercero() const
    {
        return static_cast<RecPago*>(getRecord())->getRecTercero();
    }

    gong::SearchBox *getSearchFacturaNumero() const {
        return searchFacturaNumero;
    }
    gong::SearchBox *getSearchTerceroCodigo() const {
        return searchTerceroCodigo;
    }

    /*<<<<<FRMEDITPAGO_CONTROLS*/
protected:
	gong::CheckBox *checkAutomatico;
	gong::EditBox *editNumero;
	gong::EditBox *editNumeroAgrupado;
	gong::EditBox *editContador;
	gong::EditBox *editDescripcion;
	gong::EditBox *editFechaEmision;
	gong::EditBox *editVencimiento;
	gong::EditBox *editFechaValor;
	gong::SearchBox *searchMonedaCodigo;
	QPushButton *pushMonedaCodigo;
	gong::LineEdit *editMonedaCodigo;
	gong::LineEdit *editMonedaNombre;
	gong::EditBox *editImporte;
	gong::EditBox *editResto;
	gong::ComboBoxInt *comboEstadoRecibo;
	gong::EditBox *editFechaPago;
	gong::EditBox *editDocumentoPago;
#ifdef HAVE_CONTABMODULE
	gong::SearchBox *searchCuentaPagoCuenta;
	QPushButton *pushCuentaPagoCuenta;
	gong::LineEdit *editCuentaPagoCuenta;
	gong::LineEdit *editCuentaPagoDescripcion;
#endif
	gong::EditBox *editCuentaOrigen;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITPAGO_CONTROLS*/
    QPushButton *pushPagar;
    QHBoxLayout *pTercerosLayout;
    gong::SearchBox *searchFacturaNumero;
    QPushButton *pushFacturaNumero;
    gong::LineEdit *editFacturaNumero, *editFacturaDesc;
    gong::SearchBox *searchTerceroCodigo;
    QPushButton *pushTerceroCodigo;
    gong::LineEdit *editTerceroCodigo, *editTerceroDesc;
    Xtring mFldFactCodigo, mFldFactDesc, mFldTercCodigo, mFldTercDesc;
};
/*<<<<<FRMEDITPAGO_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSFRMEDITPAGO_H
/*>>>>>FRMEDITPAGO_POSTAMBLE*/


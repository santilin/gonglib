/*<<<<<COPYLEFT*/
/** @file pagosfrmeditcobro.h Fichero de edición de cobros
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
/*<<<<<FRMEDITCOBRO_PREAMBLE*/
#ifndef _PAGOS_PAGOSFRMEDITCOBRO_H
#define _PAGOS_PAGOSFRMEDITCOBRO_H
/*>>>>>FRMEDITCOBRO_PREAMBLE*/

/*<<<<<FRMEDITCOBRO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "pagosreccobro.h"
/*>>>>>FRMEDITCOBRO_INCLUDES*/

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

/*<<<<<FRMEDITCOBRO_CLASS*/
class FrmEditCobro: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditCobro(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecCobro *getRecCobro() const { return (RecCobro *)getRecord(); }
/*>>>>>FRMEDITCOBRO_CLASS*/
    void setUsarRemesas( bool usar );
    void completa(const Xtring &tablafacturas, const Xtring &fldfactcodigo, const Xtring &fldfactdesc,
                  const Xtring &tablaterceros, const Xtring &fldterccodigo, const Xtring &fldtercdesc,
                  const Xtring &descsingular, const Xtring &descplural, bool femenina );
protected:
    virtual void updateStatus( bool callbehaviors );
    virtual bool canBeginEdit(EditMode newmode); // From FrmEditRec

    /*<<<<<FRMEDITCOBRO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITCOBRO_VIRTUALS_FROM_FRMEDITREC*/

protected slots:
    virtual void pushPagar_clicked();
    void pushFacturaNumero_clicked();
    void pushTerceroCodigo_clicked();
protected:
    void scatterFactura();
    void scatterTercero();

    /*<<<<<FRMEDITCOBRO_SCATTERS_AND_SLOTS*/
protected:
	void scatterRemesaCobro();
	void scatterMoneda();
	void scatterCuentaPago();

private slots:
	void pushRemesaCobroNumero_clicked();
	void pushMonedaCodigo_clicked();
	void pushCuentaPagoCuenta_clicked();

public:
	RecRemesaCobro* getRecRemesaCobro() const
		{ return static_cast<RecCobro*>(getRecord())->getRecRemesaCobro(); }
	empresa::RecMoneda* getRecMoneda() const
		{ return static_cast<RecCobro*>(getRecord())->getRecMoneda(); }
#ifdef HAVE_CONTABMODULE
	contab::RecCuentaPago* getRecCuentaPago() const
		{ return static_cast<RecCobro*>(getRecord())->getRecCuentaPago(); }
#endif
/*>>>>>FRMEDITCOBRO_SCATTERS_AND_SLOTS*/
public:
    RecFactura *getRecFactura() const
    {
        return static_cast<RecCobro*>(getRecord())->getRecFactura();
    }
    RecTercero *getRecTercero() const
    {
        return static_cast<RecCobro*>(getRecord())->getRecTercero();
    }

    gong::SearchBox *getSearchRemesaCobroNumero() const {
        return searchRemesaCobroNumero;
    }
    gong::SearchBox *getSearchFacturaNumero() const {
        return searchFacturaNumero;
    }
    gong::SearchBox *getSearchTerceroCodigo() const {
        return searchTerceroCodigo;
    }

    /*<<<<<FRMEDITCOBRO_CONTROLS*/
protected:
	gong::SearchBox *searchRemesaCobroNumero;
	QPushButton *pushRemesaCobroNumero;
	gong::LineEdit *editRemesaCobroNumero;
	gong::LineEdit *editRemesaCobroDescripcion;
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
/*>>>>>FRMEDITCOBRO_CONTROLS*/
    QPushButton *pushPagar;
    QHBoxLayout *pTercerosLayout;
    gong::SearchBox *searchFacturaNumero;
    QPushButton *pushFacturaNumero;
    gong::LineEdit *editFacturaNumero, *editFacturaDesc;
    gong::SearchBox *searchTerceroCodigo;
    QPushButton *pushTerceroCodigo;
    gong::LineEdit *editTerceroCodigo, *editTerceroDesc;
    Xtring mFldFactCodigo, mFldFactDesc, mFldTercCodigo, mFldTercDesc;
    bool mUsarRemesas;
};
/*<<<<<FRMEDITCOBRO_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSFRMEDITCOBRO_H
/*>>>>>FRMEDITCOBRO_POSTAMBLE*/


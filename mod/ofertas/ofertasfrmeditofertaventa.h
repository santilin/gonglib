/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditofertaventa.h Fichero de edición de albaranes de venta
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
/*<<<<<FRMEDITOFERTAVENTA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASFRMEDITOFERTAVENTA_H
#define _OFERTAS_OFERTASFRMEDITOFERTAVENTA_H
/*>>>>>FRMEDITOFERTAVENTA_PREAMBLE*/
/*<<<<<FRMEDITOFERTAVENTA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "ofertasrecofertaventa.h"
#include "ofertasfrmeditofertaventadet.h"
/*>>>>>FRMEDITOFERTAVENTA_INCLUDES*/

namespace gong {

class PushButton;

namespace ofertas {

#ifdef HAVE_TESORERIAMODULE
typedef RecCuentaOferta RecCuentaPago;
#endif

/*<<<<<FRMEDITOFERTAVENTA_CLASS*/
class FrmEditOfertaVenta: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditOfertaVenta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecOfertaVenta *getRecOfertaVenta() const { return (RecOfertaVenta *)getRecord(); }
/*>>>>>FRMEDITOFERTAVENTA_CLASS*/

    /*<<<<<FRMEDITOFERTAVENTA_CABECERA_DECL*/
protected:
	void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
	void actTotales();
	void numeraLineas();
	void genNumeroDocumento();
public:
	int getIvaDetallado() const { return comboIVADetallado->getCurrentItemValue(); }
/*>>>>>FRMEDITOFERTAVENTA_CABECERA_DECL*/
    virtual bool canBeginEdit( EditMode newmode); // from FrmEditRecMaster

    /*<<<<<FRMEDITOFERTAVENTA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITOFERTAVENTA_VIRTUALS_FROM_FRMEDITREC*/
    virtual void updateStatus( bool callbehaviors = true ); // from FrmEditRecMaster

protected slots:
    void slotCobrar();
    void slotPasarAAlbaran();

/*<<<<<FRMEDITOFERTAVENTA_SCATTERS_AND_SLOTS*/
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
	void pushCuentaPagoCodigo_clicked();

public:
	factu::RecTipoDoc* getRecTipoDoc() const
		{ return static_cast<RecOfertaVenta*>(getRecord())->getRecTipoDoc(); }
	factu::RecCliente* getRecCliente() const
		{ return static_cast<RecOfertaVenta*>(getRecord())->getRecCliente(); }
	pagos::RecFormaPago* getRecFormaPago() const
		{ return static_cast<RecOfertaVenta*>(getRecord())->getRecFormaPago(); }
	factu::RecAgente* getRecAgente() const
		{ return static_cast<RecOfertaVenta*>(getRecord())->getRecAgente(); }
	empresa::RecProyecto* getRecProyecto() const
		{ return static_cast<RecOfertaVenta*>(getRecord())->getRecProyecto(); }
#ifdef HAVE_TESORERIAMODULE
	RecCuentaPago* getRecCuentaPago() const
		{ return static_cast<RecOfertaVenta*>(getRecord())->getRecCuentaPago(); }
#endif
/*>>>>>FRMEDITOFERTAVENTA_SCATTERS_AND_SLOTS*/
/*<<<<<FRMEDITOFERTAVENTA_CONTROLS*/
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
	gong::SearchBox *searchFormaPagoCodigo;
	QPushButton *pushFormaPagoCodigo;
	gong::LineEdit *editFormaPagoCodigo;
	gong::LineEdit *editFormaPagoNombre;
	gong::SearchBox *searchAgenteCodigo;
	QPushButton *pushAgenteCodigo;
	gong::LineEdit *editAgenteCodigo;
	gong::LineEdit *editAgenteRazonSocial;
	FrmEditOfertaVentaDet *pFrmOfertaVentaDet;
	gong::EditBox *editSumaImportes;
	gong::EditBox *editDtoP100;
	gong::EditBox *editDescuento;
	gong::EditBox *editBaseImponible;
	gong::EditBox *editCobros;
	gong::EditBox *editRecargoEquivalencia;
	gong::EditBox *editIVA;
	gong::EditBox *editEntrega;
	gong::EditBox *editResto;
	gong::EditBox *editTotal;
	gong::SearchBox *searchProyectoCodigo;
	QPushButton *pushProyectoCodigo;
	gong::LineEdit *editProyectoCodigo;
	gong::LineEdit *editProyectoNombre;
	gong::EditBox *editDesgloseIVA;
	gong::EditBox *editDocumentoPago;
	gong::EditBox *editFechaPago;
#ifdef HAVE_TESORERIAMODULE
	gong::SearchBox *searchCuentaPagoCodigo;
	QPushButton *pushCuentaPagoCodigo;
	gong::LineEdit *editCuentaPagoCodigo;
	gong::LineEdit *editCuentaPagoNombre;
#endif
	gong::TextBox *editNotas;
/*>>>>>FRMEDITOFERTAVENTA_CONTROLS*/
    PushButton *pushCobrar;
    PushButton *pushPasarAAlbaran;
protected:
    bool mHasCobros;
    Money mOldTotal;
};
/*<<<<<FRMEDITOFERTAVENTA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASFRMEDITOFERTAVENTA_H
/*>>>>>FRMEDITOFERTAVENTA_POSTAMBLE*/


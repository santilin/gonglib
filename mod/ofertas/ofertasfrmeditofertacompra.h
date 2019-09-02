/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditofertacompra.h Fichero de edición de albaranes de compra
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
/*<<<<<FRMEDITOFERTACOMPRA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASFRMEDITOFERTACOMPRA_H
#define _OFERTAS_OFERTASFRMEDITOFERTACOMPRA_H
/*>>>>>FRMEDITOFERTACOMPRA_PREAMBLE*/
/*<<<<<FRMEDITOFERTACOMPRA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "ofertasrecofertacompra.h"
#include "ofertasfrmeditofertacompradet.h"
/*>>>>>FRMEDITOFERTACOMPRA_INCLUDES*/

namespace gong {

class PushButton;

namespace ofertas {

#ifdef HAVE_TESORERIAMODULE
typedef RecCuentaOferta RecCuentaPago;
#endif

/*<<<<<FRMEDITOFERTACOMPRA_CLASS*/
class FrmEditOfertaCompra: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditOfertaCompra(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecOfertaCompra *getRecOfertaCompra() const { return (RecOfertaCompra *)getRecord(); }
/*>>>>>FRMEDITOFERTACOMPRA_CLASS*/

/*<<<<<FRMEDITOFERTACOMPRA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITOFERTACOMPRA_VIRTUALS_FROM_FRMEDITREC*/
    virtual void updateStatus( bool callbehaviors = true ); // from FrmEditRecMaster

/*<<<<<FRMEDITOFERTACOMPRA_CABECERA_DECL*/
protected:
	void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
	void actTotales();
	void numeraLineas();
	void genNumeroDocumento();
public:
	int getIvaDetallado() const { return comboIVADetallado->getCurrentItemValue(); }
/*>>>>>FRMEDITOFERTACOMPRA_CABECERA_DECL*/
    virtual bool canBeginEdit( EditMode newmode ); // from FrmEditRecMaster

/*<<<<<FRMEDITOFERTACOMPRA_SCATTERS_AND_SLOTS$s/RecCuentaPago/RecCuentaOferta/$*/
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
	void pushCuentaPagoCodigo_clicked();

public:
	factu::RecTipoDoc* getRecTipoDoc() const
		{ return static_cast<RecOfertaCompra*>(getRecord())->getRecTipoDoc(); }
	factu::RecProveedora* getRecProveedora() const
		{ return static_cast<RecOfertaCompra*>(getRecord())->getRecProveedora(); }
	pagos::RecFormaPago* getRecFormaPago() const
		{ return static_cast<RecOfertaCompra*>(getRecord())->getRecFormaPago(); }
	empresa::RecProyecto* getRecProyecto() const
		{ return static_cast<RecOfertaCompra*>(getRecord())->getRecProyecto(); }
#ifdef HAVE_TESORERIAMODULE
	RecCuentaOferta* getRecCuentaOferta() const
		{ return static_cast<RecOfertaCompra*>(getRecord())->getRecCuentaOferta(); }
#endif
/*>>>>>FRMEDITOFERTACOMPRA_SCATTERS_AND_SLOTS*/
protected slots:
    void slotPagar();
    void slotPasarAAlbaran();

    /*<<<<<FRMEDITOFERTACOMPRA_CONTROLS*/
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
	FrmEditOfertaCompraDet *pFrmOfertaCompraDet;
	gong::EditBox *editSumaImportes;
	gong::EditBox *editDtoP100;
	gong::EditBox *editDescuento;
	gong::EditBox *editBaseImponible;
	gong::EditBox *editPagos;
	gong::EditBox *editRecargoEquivalencia;
	gong::EditBox *editIVA;
	gong::EditBox *editEntrega;
	gong::EditBox *editResto;
	gong::EditBox *editTotal;
	gong::EditBox *editDesgloseIVA;
	gong::SearchBox *searchProyectoCodigo;
	QPushButton *pushProyectoCodigo;
	gong::LineEdit *editProyectoCodigo;
	gong::LineEdit *editProyectoNombre;
#ifdef HAVE_TESORERIAMODULE
	gong::SearchBox *searchCuentaPagoCodigo;
	QPushButton *pushCuentaPagoCodigo;
	gong::LineEdit *editCuentaPagoCodigo;
	gong::LineEdit *editCuentaPagoNombre;
#endif
	gong::EditBox *editDocumentoPago;
	gong::EditBox *editFechaPago;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITOFERTACOMPRA_CONTROLS*/
    PushButton *pushPagar;
    PushButton *pushPasarAAlbaran;
protected:
    bool mHasPagos;
    Money mOldTotal;
};

/*<<<<<FRMEDITOFERTACOMPRA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASFRMEDITOFERTACOMPRA_H
/*>>>>>FRMEDITOFERTACOMPRA_POSTAMBLE*/

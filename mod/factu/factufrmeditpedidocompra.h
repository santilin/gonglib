/*<<<<<COPYLEFT*/
/** @file factufrmeditpedidocompra.h Fichero de edición de pedidos de compra
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<FRMEDITPEDIDOCOMPRA_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITPEDIDOCOMPRA_H
#define _FACTU_FACTUFRMEDITPEDIDOCOMPRA_H
/*>>>>>FRMEDITPEDIDOCOMPRA_PREAMBLE*/
/*<<<<<FRMEDITPEDIDOCOMPRA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturecpedidocompra.h"
#include "factufrmeditpedidocompradet.h"
/*>>>>>FRMEDITPEDIDOCOMPRA_INCLUDES*/

namespace gong
{
namespace factu {


/*<<<<<FRMEDITPEDIDOCOMPRA_CLASS*/
class FrmEditPedidoCompra: public FrmEditRecMaster
{
    Q_OBJECT

public:
    FrmEditPedidoCompra(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                        EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                        QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecPedidoCompra *getRecPedidoCompra() const {
        return (RecPedidoCompra *)getRecord();
    }
    /*>>>>>FRMEDITPEDIDOCOMPRA_CLASS*/

    /*<<<<<FRMEDITPEDIDOCOMPRA_CABECERA_DECL*/
protected:
    void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
    void actTotales();
    void numeraLineas();
    void genNumeroDocumento();
public:
    int getIvaDetallado() const {
        return comboIVADetallado->getCurrentItemValue();
    }
    /*>>>>>FRMEDITPEDIDOCOMPRA_CABECERA_DECL*/

    /*<<<<<FRMEDITPEDIDOCOMPRA_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    void specialControlKeyPressed(QWidget *sender, char key);
    /*>>>>>FRMEDITPEDIDOCOMPRA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITPEDIDOCOMPRA_SCATTERS_AND_SLOTS*/
protected:
    void scatterTipoDoc();
    void scatterProveedora();

private slots:
    void pushTipoDocCodigo_clicked();
    void pushProveedoraCodigo_clicked();

public:
    RecTipoDoc* getRecTipoDoc() const
    {
        return static_cast<RecPedidoCompra*>(getRecord())->getRecTipoDoc();
    }
    RecProveedora* getRecProveedora() const
    {
        return static_cast<RecPedidoCompra*>(getRecord())->getRecProveedora();
    }
    /*>>>>>FRMEDITPEDIDOCOMPRA_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITPEDIDOCOMPRA_CONTROLS*/
protected:
    gong::EditBox *editFecha;
    gong::SearchBox *searchTipoDocCodigo;
    QPushButton *pushTipoDocCodigo;
    gong::LineEdit *editTipoDocCodigo;
    gong::LineEdit *editTipoDocNombre;
    gong::EditBox *editContador;
    gong::EditBox *editNumero;
    gong::ComboBox<int> *comboIVADetallado;
    gong::SearchBox *searchProveedoraCodigo;
    QPushButton *pushProveedoraCodigo;
    gong::LineEdit *editProveedoraCodigo;
    gong::LineEdit *editProveedoraRazonSocial;
    gong::ComboBox<int> *comboEstadoPedido;
    FrmEditPedidoCompraDet *pFrmPedidoCompraDet;
    gong::EditBox *editReferencia;
    gong::EditBox *editSumaImportes;
    gong::EditBox *editDtoP100;
    gong::EditBox *editDescuento;
    gong::EditBox *editBaseImponible;
    gong::EditBox *editRecargoEquivalencia;
    gong::EditBox *editIVA;
    gong::EditBox *editTotal;
    gong::EditBox *editDesgloseIVA;
    gong::TextBox *editNotas;
    /*>>>>>FRMEDITPEDIDOCOMPRA_CONTROLS*/
};
/*<<<<<FRMEDITPEDIDOCOMPRA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITPEDIDOCOMPRA_H
/*>>>>>FRMEDITPEDIDOCOMPRA_POSTAMBLE*/


/*<<<<<COPYLEFT*/
/** @file factufrmeditpresupuestoventadet.h Fichero de edición de detalles presupuestos de venta
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
/*<<<<<FRMEDITPRESUPUESTOVENTADET_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITPRESUPUESTOVENTADET_H
#define _FACTU_FACTUFRMEDITPRESUPUESTOVENTADET_H
/*>>>>>FRMEDITPRESUPUESTOVENTADET_PREAMBLE*/
/*<<<<<FRMEDITPRESUPUESTOVENTADET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "facturecpresupuestoventadet.h"
/*>>>>>FRMEDITPRESUPUESTOVENTADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<FRMEDITPRESUPUESTOVENTADET_CLASS*/
class FrmEditPresupuestoVentaDet: public FrmEditRecDetail
{
    Q_OBJECT

public:
    FrmEditPresupuestoVentaDet(FrmEditRecMaster *frmmaster, int ndetail,
                               dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
                               EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                               QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecPresupuestoVentaDet *getRecPresupuestoVentaDet() const {
        return (RecPresupuestoVentaDet *)getRecord();
    }
    /*>>>>>FRMEDITPRESUPUESTOVENTADET_CLASS*/
protected slots:
    void slotActPrecioArticulo_clicked();

    /*<<<<<FRMEDITPRESUPUESTOVENTADET_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    void specialControlKeyPressed(QWidget *sender, char key);
    /*>>>>>FRMEDITPRESUPUESTOVENTADET_VIRTUALS_FROM_FRMEDITREC*/
    void updateStatus( bool callbehaviors );

protected slots:
    void slotInsertTable_clicked();

    /*<<<<<FRMEDITPRESUPUESTOVENTADET_SCATTERS_AND_SLOTS*/
protected:
    void scatterTipoIVA();
    void scatterArticulo();

private slots:
    void pushTipoIVACodigo_clicked();
    void pushArticuloCodigo_clicked();

public:
    empresa::RecTipoIVA* getRecTipoIVA() const
    {
        return static_cast<RecPresupuestoVentaDet*>(getRecord())->getRecTipoIVA();
    }
    RecArticulo* getRecArticulo() const
    {
        return static_cast<RecPresupuestoVentaDet*>(getRecord())->getRecArticulo();
    }
    /*>>>>>FRMEDITPRESUPUESTOVENTADET_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITPRESUPUESTOVENTADET_CONTROLS*/
protected:
    gong::EditBox *editNLinea;
    gong::SearchBox *searchTipoIVACodigo;
    QPushButton *pushTipoIVACodigo;
    gong::LineEdit *editTipoIVACodigo;
    gong::LineEdit *editTipoIVANombre;
    gong::EditBox *editCantidad;
    gong::SearchBox *searchArticuloCodigo;
    QPushButton *pushArticuloCodigo;
    gong::LineEdit *editArticuloCodigo;
    gong::LineEdit *editArticuloNombre;
    gong::EditBox *editPVPSinIVA;
    gong::EditBox *editPVP;
    gong::EditBox *editDtoP100;
    gong::EditBox *editImporte;
    gong::EditBox *editImporteConIVA;
    gong::TextBox *editDescripcion;
    gong::TextBox *editNotas;
    /*>>>>>FRMEDITPRESUPUESTOVENTADET_CONTROLS*/
    QPushButton *pushInsertTable, *pushActPrecioArticulo;
};
/*<<<<<FRMEDITPRESUPUESTOVENTADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITPRESUPUESTOVENTADET_H
/*>>>>>FRMEDITPRESUPUESTOVENTADET_POSTAMBLE*/



/*<<<<<COPYLEFT*/
/** @file factufrmeditalbarancompradet.h Fichero de edición de detalles de albaranes de compra
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
/*<<<<<FRMEDITALBARANCOMPRADET_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITALBARANCOMPRADET_H
#define _FACTU_FACTUFRMEDITALBARANCOMPRADET_H
/*>>>>>FRMEDITALBARANCOMPRADET_PREAMBLE*/
/*<<<<<FRMEDITALBARANCOMPRADET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "facturecalbarancompradet.h"
/*>>>>>FRMEDITALBARANCOMPRADET_INCLUDES*/

namespace gong {
namespace factu {


/*<<<<<FRMEDITALBARANCOMPRADET_CLASS*/
class FrmEditAlbaranCompraDet: public FrmEditRecDetail
{
    Q_OBJECT

public:
    FrmEditAlbaranCompraDet(FrmEditRecMaster *frmmaster, int ndetail,
                            dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
                            EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                            QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecAlbaranCompraDet *getRecAlbaranCompraDet() const {
        return (RecAlbaranCompraDet *)getRecord();
    }
    /*>>>>>FRMEDITALBARANCOMPRADET_CLASS*/
    /*<<<<<FRMEDITALBARANCOMPRADET_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    void specialControlKeyPressed(QWidget *sender, char key);
    /*>>>>>FRMEDITALBARANCOMPRADET_VIRTUALS_FROM_FRMEDITREC*/

protected slots:
    void slotInsertTable_clicked();
    void slotActPrecioArticulo_clicked();

    /*<<<<<FRMEDITALBARANCOMPRADET_SCATTERS_AND_SLOTS*/
protected:
    void scatterTipoIVA();
    void scatterArticulo();

private slots:
    void pushTipoIVACodigo_clicked();
    void pushArticuloCodigo_clicked();

public:
    empresa::RecTipoIVA* getRecTipoIVA() const
    {
        return static_cast<RecAlbaranCompraDet*>(getRecord())->getRecTipoIVA();
    }
    RecArticulo* getRecArticulo() const
    {
        return static_cast<RecAlbaranCompraDet*>(getRecord())->getRecArticulo();
    }
    /*>>>>>FRMEDITALBARANCOMPRADET_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITALBARANCOMPRADET_CONTROLS*/
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
    gong::EditBox *editCosteSinIVA;
    gong::EditBox *editCoste;
    gong::EditBox *editDtoP100;
    gong::EditBox *editImporte;
    gong::EditBox *editImporteConIVA;
    gong::TextBox *editDescripcion;
    gong::TextBox *editNotas;
    /*>>>>>FRMEDITALBARANCOMPRADET_CONTROLS*/
    virtual void updateStatus( bool callbehaviors );
    QPushButton *pushInsertTable, *pushActPrecioArticulo;
};

/*<<<<<FRMEDITALBARANCOMPRADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITALBARANCOMPRADET_H
/*>>>>>FRMEDITALBARANCOMPRADET_POSTAMBLE*/

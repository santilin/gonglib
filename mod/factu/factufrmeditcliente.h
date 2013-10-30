/*<<<<<COPYLEFT*/
/** @file factufrmeditcliente.h Fichero de edición de clientes
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
/*<<<<<FRMEDITCLIENTE_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITCLIENTE_H
#define _FACTU_FACTUFRMEDITCLIENTE_H
/*>>>>>FRMEDITCLIENTE_PREAMBLE*/

#include "config.h"
/*<<<<<FRMEDITCLIENTE_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "factureccliente.h"
/*>>>>>FRMEDITCLIENTE_INCLUDES*/

namespace gong {
class PushButton;
namespace contactos {
class FrmEditContactoBehavior;
}
namespace factu {

/*<<<<<FRMEDITCLIENTE_CLASS*/
class FrmEditCliente: public FrmEditRecMaster
{
    Q_OBJECT

public:
    FrmEditCliente(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                   EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                   QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecCliente *getRecCliente() const {
        return (RecCliente *)getRecord();
    }
    /*>>>>>FRMEDITCLIENTE_CLASS*/
    virtual bool save(); // from FrmEditRec
    bool saveAllContactos();

    /*<<<<<FRMEDITCLIENTE_SCATTERS_AND_SLOTS*/
protected:
    void scatterFormaPago();
    void scatterAgente();

private slots:
    void pushFormaPagoCodigo_clicked();
    void pushAgenteCodigo_clicked();

public:
    pagos::RecFormaPago* getRecFormaPago() const
    {
        return static_cast<RecCliente*>(getRecord())->getRecFormaPago();
    }
    RecAgente* getRecAgente() const
    {
        return static_cast<RecCliente*>(getRecord())->getRecAgente();
    }
    /*>>>>>FRMEDITCLIENTE_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITCLIENTE_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    void specialControlKeyPressed(QWidget *sender, char key);
    /*>>>>>FRMEDITCLIENTE_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITCLIENTE_CONTROLS*/
protected:
    gong::EditBox *editCodigo;
    gong::EditBox *editRazonSocial;
    gong::SearchBox *searchFormaPagoCodigo;
    QPushButton *pushFormaPagoCodigo;
    gong::LineEdit *editFormaPagoCodigo;
    gong::LineEdit *editFormaPagoNombre;
    gong::EditBox *editNombreAlt;
    gong::EditBox *editDtoP100;
    gong::CheckBox *checkActivo;
    gong::EditBox *editTarifa;
    gong::SearchBox *searchAgenteCodigo;
    QPushButton *pushAgenteCodigo;
    gong::LineEdit *editAgenteCodigo;
    gong::LineEdit *editAgenteRazonSocial;
    gong::ComboBox<int> *comboIVADetallado;
    gong::ComboBox<int> *comboTipoCliente;
    gong::EditBox *editEntidadBanco;
    gong::EditBox *editCuentaBanco;
#ifdef HAVE_CONTABMODULE
    gong::EditBox *editSubcuenta;
#endif
    gong::TextBox *editNotas;
    /*>>>>>FRMEDITCLIENTE_CONTROLS*/
    List<dbRecordID> mContactoList;
    class contactos::FrmEditContactoBehavior *pEditContactoBehavior;
    class QTabWidget *pFrameContactos;
};
/*<<<<<FRMEDITCLIENTE_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITCLIENTE_H
/*>>>>>FRMEDITCLIENTE_POSTAMBLE*/



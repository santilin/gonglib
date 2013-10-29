/*<<<<<COPYLEFT*/
/** @file contabfrmeditcuenta.h Fichero de edición de Cuentas
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
/*<<<<<FRMEDITCUENTA_PREAMBLE*/
#ifndef _CONTAB_CONTABFRMEDITCUENTA_H
#define _CONTAB_CONTABFRMEDITCUENTA_H
/*>>>>>FRMEDITCUENTA_PREAMBLE*/

/*<<<<<FRMEDITCUENTA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "contabreccuenta.h"
/*>>>>>FRMEDITCUENTA_INCLUDES*/
#include "contactosreccontacto.h"

namespace gong  {
namespace contab {

/*<<<<<FRMEDITCUENTA_CLASS*/
class FrmEditCuenta: public FrmEditRecMaster
{
    Q_OBJECT

public:
    FrmEditCuenta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                  EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                  QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecCuenta *getRecCuenta() const {
        return (RecCuenta *)getRecord();
    }
    /*>>>>>FRMEDITCUENTA_CLASS*/
protected:
    virtual bool remove(); // From frmEditRec

    /*<<<<<FRMEDITCUENTA_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    void specialControlKeyPressed(QWidget *sender, char key);
    /*>>>>>FRMEDITCUENTA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITCUENTA_SCATTERS_AND_SLOTS*/
protected:
    void scatterContacto();

private slots:
    void pushContactoCIF_clicked();

public:
    contactos::RecContacto* getRecContacto() const
    {
        return static_cast<RecCuenta*>(getRecord())->getRecContacto();
    }
    /*>>>>>FRMEDITCUENTA_SCATTERS_AND_SLOTS*/
    /*<<<<<FRMEDITCUENTA_CONTROLS*/
protected:
    gong::EditBox *editDebe;
    gong::EditBox *editHaber;
    gong::EditBox *editSaldo;
    gong::SearchBox *searchContactoCIF;
    QPushButton *pushContactoCIF;
    gong::LineEdit *editContactoCIF;
    gong::LineEdit *editContactoNombre;
    gong::EditBox *editCuenta;
    gong::EditBox *editDescripcion;
    gong::CheckBox *checkManual;
    gong::TextBox *editNotas;
    /*>>>>>FRMEDITCUENTA_CONTROLS*/
    class FrmEditAsiento *pFrmAsiento;
};

/*<<<<<FRMEDITCUENTA_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABFRMEDITCUENTA_H
/*>>>>>FRMEDITCUENTA_POSTAMBLE*/


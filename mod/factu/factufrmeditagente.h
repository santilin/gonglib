/*<<<<<COPYLEFT*/
/** @file factufrmeditagente.h Fichero de edición de agentes
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
/*<<<<<FRMEDITAGENTE_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITAGENTE_H
#define _FACTU_FACTUFRMEDITAGENTE_H
/*>>>>>FRMEDITAGENTE_PREAMBLE*/

/*<<<<<FRMEDITAGENTE_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturecagente.h"
/*>>>>>FRMEDITAGENTE_INCLUDES*/
#include <contactosreccontacto.h>

namespace gong {
class PushButton;
namespace contactos {
class FrmEditContactoBehavior;
}
namespace factu {

/*<<<<<FRMEDITAGENTE_CLASS*/
class FrmEditAgente: public FrmEditRecMaster
{
    Q_OBJECT

public:
    FrmEditAgente(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                  EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                  QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecAgente *getRecAgente() const {
        return (RecAgente *)getRecord();
    }
    /*>>>>>FRMEDITAGENTE_CLASS*/

    /*<<<<<FRMEDITAGENTE_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    /*>>>>>FRMEDITAGENTE_VIRTUALS_FROM_FRMEDITREC*/
    class contactos::FrmEditContactoBehavior *pEditContactoBehavior;

    /*<<<<<FRMEDITAGENTE_CONTROLS*/
protected:
    gong::EditBox *editCodigo;
    gong::EditBox *editRazonSocial;
    gong::TextBox *editNotas;
    /*>>>>>FRMEDITAGENTE_CONTROLS*/
};

/*<<<<<FRMEDITAGENTE_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITAGENTE_H
/*>>>>>FRMEDITAGENTE_POSTAMBLE*/


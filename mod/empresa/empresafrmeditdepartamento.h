/*<<<<<COPYLEFT*/
/** @file empresafrmeditdepartamento.h Fichero de edición de departamentos
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
/*<<<<<FRMEDITDEPARTAMENTO_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAFRMEDITDEPARTAMENTO_H
#define _EMPRESA_EMPRESAFRMEDITDEPARTAMENTO_H
/*>>>>>FRMEDITDEPARTAMENTO_PREAMBLE*/

/*<<<<<FRMEDITDEPARTAMENTO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "empresarecdepartamento.h"
/*>>>>>FRMEDITDEPARTAMENTO_INCLUDES*/

namespace gong
{
namespace empresa {

/*<<<<<FRMEDITDEPARTAMENTO_CLASS*/
class FrmEditDepartamento: public FrmEditRecMaster
{
    Q_OBJECT

public:
    FrmEditDepartamento(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                        EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                        QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecDepartamento *getRecDepartamento() const {
        return (RecDepartamento *)getRecord();
    }
    /*>>>>>FRMEDITDEPARTAMENTO_CLASS*/
    /*<<<<<FRMEDITDEPARTAMENTO_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    /*>>>>>FRMEDITDEPARTAMENTO_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITDEPARTAMENTO_CONTROLS*/
protected:
    gong::EditBox *editCodigo;
    gong::EditBox *editNombre;
    gong::TextBox *editNotas;
    /*>>>>>FRMEDITDEPARTAMENTO_CONTROLS*/
};
/*<<<<<FRMEDITDEPARTAMENTO_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAFRMEDITDEPARTAMENTO_H
/*>>>>>FRMEDITDEPARTAMENTO_POSTAMBLE*/


/*<<<<<COPYLEFT*/
/** @file empresafrmeditempresa.h Fichero de edición de empresas
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
/*<<<<<FRMEDITEMPRESA_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAFRMEDITEMPRESA_H
#define _EMPRESA_EMPRESAFRMEDITEMPRESA_H
/*>>>>>FRMEDITEMPRESA_PREAMBLE*/
/*<<<<<FRMEDITEMPRESA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "empresarecempresa.h"
/*>>>>>FRMEDITEMPRESA_INCLUDES*/

namespace gong {

namespace contactos {
class FrmEditContactoBehavior;
}

namespace empresa {

/*<<<<<FRMEDITEMPRESA_CLASS*/
class FrmEditEmpresa: public FrmEditRecMaster
{
    Q_OBJECT

public:
    FrmEditEmpresa(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                   EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                   QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecEmpresa *getRecEmpresa() const {
        return (RecEmpresa *)getRecord();
    }
    /*>>>>>FRMEDITEMPRESA_CLASS*/

    /*<<<<<FRMEDITEMPRESA_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    void specialControlKeyPressed(QWidget *sender, char key);
    /*>>>>>FRMEDITEMPRESA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITEMPRESA_SCATTERS_AND_SLOTS*/
protected:
    void scatterMoneda();

private slots:
    void pushMonedaCodigo_clicked();

public:
    RecMoneda* getRecMoneda() const
    {
        return static_cast<RecEmpresa*>(getRecord())->getRecMoneda();
    }
    /*>>>>>FRMEDITEMPRESA_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITEMPRESA_CONTROLS*/
protected:
    gong::EditBox *editCodigo;
    gong::EditBox *editNombre;
    gong::SearchBox *searchMonedaCodigo;
    QPushButton *pushMonedaCodigo;
    gong::LineEdit *editMonedaCodigo;
    gong::LineEdit *editMonedaNombre;
    gong::EditBox *editEntidadBanco;
    gong::EditBox *editCuentaBanco;
    gong::CheckBox *checkSoloLectura;
    gong::CheckBox *checkUsarProyectos;
    gong::TextBox *editNotas;
    gong::ImageBox *editLogo;
    /*>>>>>FRMEDITEMPRESA_CONTROLS*/
    class contactos::FrmEditContactoBehavior *pEditContactoBehavior;
    class QTabWidget *pFrameContactos;

private slots:
    void editLogo_clicked();

};
/*<<<<<FRMEDITEMPRESA_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAFRMEDITEMPRESA_H
/*>>>>>FRMEDITEMPRESA_POSTAMBLE*/


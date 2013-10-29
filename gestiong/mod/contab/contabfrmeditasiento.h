/*<<<<<COPYLEFT*/
/** @file contabfrmeditasiento.h Fichero de edición de asientos
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
/*<<<<<FRMEDITASIENTO_PREAMBLE*/
#ifndef _CONTAB_CONTABFRMEDITASIENTO_H
#define _CONTAB_CONTABFRMEDITASIENTO_H
/*>>>>>FRMEDITASIENTO_PREAMBLE*/

/*<<<<<FRMEDITASIENTO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "contabrecasiento.h"
#include "contabfrmeditapunte.h"
/*>>>>>FRMEDITASIENTO_INCLUDES*/

#ifdef HAVE_SOCIASMODULE
#include "sociasrecproyecto.h"
#endif

namespace gong {
namespace contab {

/*<<<<<FRMEDITASIENTO_CLASS*/
class FrmEditAsiento: public FrmEditRecMaster
{
    Q_OBJECT

public:
    FrmEditAsiento(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                   EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                   QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecAsiento *getRecAsiento() const {
        return (RecAsiento *)getRecord();
    }
    /*>>>>>FRMEDITASIENTO_CLASS*/
    dbRecordList *getApuntes() const;
    void actSaldos();
    Xtring getLastConcepto() const;
protected:
    virtual bool remove();

    /*<<<<<FRMEDITASIENTO_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    void specialControlKeyPressed(QWidget *sender, char key);
    /*>>>>>FRMEDITASIENTO_VIRTUALS_FROM_FRMEDITREC*/
public:
    int getLastNumAsiento() const;
    virtual bool save(); // from FrmEditRec
    virtual void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
    /*<<<<<FRMEDITASIENTO_SCATTERS_AND_SLOTS*/
protected:
    void scatterProyecto();

private slots:
    void pushProyectoCodigo_clicked();

public:
    empresa::RecProyecto* getRecProyecto() const
    {
        return static_cast<RecAsiento*>(getRecord())->getRecProyecto();
    }
    /*>>>>>FRMEDITASIENTO_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITASIENTO_CONTROLS*/
protected:
    gong::EditBox *editNumAsiento;
    gong::EditBox *editFecha;
    gong::EditBox *editDescripcion;
    gong::CheckBox *checkAutomatico;
    gong::ComboBox<int> *comboTipoAsiento;
    gong::SearchBox *searchProyectoCodigo;
    QPushButton *pushProyectoCodigo;
    gong::LineEdit *editProyectoCodigo;
    gong::LineEdit *editProyectoNombre;
    gong::EditBox *editContador;
    FrmEditApunte *pFrmApunte;
    gong::EditBox *editDebe;
    gong::EditBox *editHaber;
    gong::EditBox *editSaldo;
    gong::TextBox *editNotas;
    /*>>>>>FRMEDITASIENTO_CONTROLS*/
};
/*<<<<<FRMEDITASIENTO_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABFRMEDITASIENTO_H
/*>>>>>FRMEDITASIENTO_POSTAMBLE*/


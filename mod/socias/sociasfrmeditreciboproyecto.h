/*<<<<<FRMEDITRECIBOPROYECTO_PREAMBLE*/
#ifndef _SOCIAS_SOCIASFRMEDITRECIBOPROYECTO_H
#define _SOCIAS_SOCIASFRMEDITRECIBOPROYECTO_H
/*>>>>>FRMEDITRECIBOPROYECTO_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file sociasfrmeditreciboproyecto.h Fichero de edición de
 * Proyecto GestiONG. (C) Santiago Capel Torres, 2003-2007
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*>>>>>COPYLEFT*/

/*<<<<<FRMEDITRECIBOPROYECTO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "sociasrecreciboproyecto.h"
/*>>>>>FRMEDITRECIBOPROYECTO_INCLUDES*/
#ifdef HAVE_PAGOSMODULE
#include <pagosrecformapago.h>
#endif

#include "sociasrecmiembro.h"
#include "sociasrecremesaproyecto.h"


namespace gong {
namespace socias {

/*<<<<<FRMEDITRECIBOPROYECTO_CLASS*/
class FrmEditReciboProyecto: public FrmEditRecMaster
{
    Q_OBJECT

public:
    FrmEditReciboProyecto(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                          EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                          QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    RecReciboProyecto *getRecReciboProyecto() const {
        return (RecReciboProyecto *)getRecord();
    }
    /*>>>>>FRMEDITRECIBOPROYECTO_CLASS*/

    virtual void menuRecordPrint_clicked(); // From FrmEditRecMaster
    RecRemesaProyecto *getRecRemesaProyectoPadre() const;
    /** Partida del proyecto para la generación automática del asiento */
    void setPartidaProyectoID(dbRecordID id) {
        mPartidaProyectoID = id;
    }

public:
    virtual bool save(); // From FrmEditRec
    virtual bool remove(); // From FrmEditRec
    virtual void updateStatus(); // From FrmEditRecMaster
    /*<<<<<FRMEDITRECIBOPROYECTO_VIRTUALS_FROM_FRMEDITREC*/
protected:
    virtual void scatterFields(); // From FrmEditRec
    virtual void gatherFields();
    virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
    void specialControlKeyPressed(QWidget *sender, char key);
    /*>>>>>FRMEDITRECIBOPROYECTO_VIRTUALS_FROM_FRMEDITREC*/
    virtual void pushPagar_clicked();
    virtual void pushAnularPago_clicked();

    /*<<<<<FRMEDITRECIBOPROYECTO_CONTROLS*/
protected:
    gong::SearchBox *searchRemesaProyectoNumero;
    QPushButton *pushRemesaProyectoNumero;
    gong::LineEdit *editRemesaProyectoNumero;
    gong::LineEdit *editRemesaProyectoDescripcion;
    gong::EditBox *editNumero;
    gong::EditBox *editDescripcion;
    gong::SearchBox *searchMiembroNumeroSocia;
    QPushButton *pushMiembroNumeroSocia;
    gong::LineEdit *editMiembroNumeroSocia;
    gong::LineEdit *editMiembroContacto_Nombre;
    gong::EditBox *editFechaEmision;
    gong::EditBox *editFechaCargo;
    gong::EditBox *editFechaValor;
    gong::EditBox *editImporte;
    gong::EditBox *editResto;
    gong::ComboBox<int> *comboEstado;
    gong::EditBox *editFechaPago;
    gong::EditBox *editDocumentoPago;
#ifdef HAVE_CONTABMODULE
    gong::SearchBox *searchCuentaPagoCuenta;
    QPushButton *pushCuentaPagoCuenta;
    gong::LineEdit *editCuentaPagoCuenta;
    gong::LineEdit *editCuentaPagoDescripcion;
#endif
    gong::TextBox *editNotas;
    /*>>>>>FRMEDITRECIBOPROYECTO_CONTROLS*/
    QPushButton *pushPagar;
    QPushButton *pushAnularPago;
    /*<<<<<FRMEDITRECIBOPROYECTO_SCATTERS_AND_SLOTS*/
protected:
    void scatterRemesaProyecto();
    void scatterMiembro();
    void scatterCuentaPago();

private slots:
    void pushRemesaProyectoNumero_clicked();
    void pushMiembroNumeroSocia_clicked();
    void pushCuentaPagoCuenta_clicked();

public:
    RecRemesaProyecto* getRecRemesaProyecto() const
    {
        return static_cast<RecReciboProyecto*>(getRecord())->getRecRemesaProyecto();
    }
    RecMiembro* getRecMiembro() const
    {
        return static_cast<RecReciboProyecto*>(getRecord())->getRecMiembro();
    }
#ifdef HAVE_CONTABMODULE
    contab::RecCuentaPago* getRecCuentaPago() const
    {
        return static_cast<RecReciboProyecto*>(getRecord())->getRecCuentaPago();
    }
#endif
    /*>>>>>FRMEDITRECIBOPROYECTO_SCATTERS_AND_SLOTS*/
    void setSupervisando(bool theValue) {
        mSupervisando = theValue;
    }

private:
#ifdef HAVE_CONTABMODULE
    dbRecordID creaAsiento(dbRecordID asientoid, const Xtring &origen,
                           dbRecordID tipodocumentoid, const Xtring &documento,
                           const Xtring &cuentadebe, Money debe,
                           const Xtring &cuentahaber, Money haber,
                           const Xtring &concepto, dbRecordID partidaid);
    Xtring creaCuentaSocia();
#endif
    dbRecordID mPartidaProyectoID;
    bool mSupervisando;
    static Xtring sLastCuentaPago, sLastDocumentoPago;
    static Date sLastFechaPago;
};
/*<<<<<FRMEDITRECIBOPROYECTO_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASFRMEDITRECIBOPROYECTO_H
/*>>>>>FRMEDITRECIBOPROYECTO_POSTAMBLE*/


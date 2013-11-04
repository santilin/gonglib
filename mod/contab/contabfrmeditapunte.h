/*<<<<<COPYLEFT*/
/** @file contabfrmeditapunte.h Fichero de edición de apuntes
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<FRMEDITAPUNTE_PREAMBLE*/
#ifndef _CONTAB_CONTABFRMEDITAPUNTE_H
#define _CONTAB_CONTABFRMEDITAPUNTE_H
/*>>>>>FRMEDITAPUNTE_PREAMBLE*/

/*<<<<<FRMEDITAPUNTE_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "contabrecapunte.h"
/*>>>>>FRMEDITAPUNTE_INCLUDES*/
#include <dbappfrmeditrecdetail.h>

namespace gong {
namespace contab {

typedef RecCuenta RecContrapartida;

/*<<<<<FRMEDITAPUNTE_CLASS*/
class FrmEditApunte: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditApunte(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecApunte *getRecApunte() const { return (RecApunte *)getRecord(); }
/*>>>>>FRMEDITAPUNTE_CLASS*/
    virtual ~FrmEditApunte();
    virtual bool save(); // From frmEditRecDetalle

    /*<<<<<FRMEDITAPUNTE_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITAPUNTE_VIRTUALS_FROM_FRMEDITREC*/

protected:
    void addContrapartida();
    void scatterContrapartida();
    Money calcSaldo() const;
    RecContrapartida* getRecContrapartida()
    {
        return pRecContrapartida;
    }
    class FrmEditAsiento *getFrmAsiento() const;

private slots:
    void pushContrapartidaCuenta_clicked();
    void pushCuadrar_clicked();

    /*<<<<<FRMEDITAPUNTE_SCATTERS_AND_SLOTS*/
protected:
	void scatterCuenta();

private slots:
	void pushCuentaCuenta_clicked();

public:
	RecCuenta* getRecCuenta() const
		{ return static_cast<RecApunte*>(getRecord())->getRecCuenta(); }
/*>>>>>FRMEDITAPUNTE_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITAPUNTE_CONTROLS*/
protected:
	gong::EditBox *editNumApunte;
	gong::SearchBox *searchCuentaCuenta;
	QPushButton *pushCuentaCuenta;
	gong::LineEdit *editCuentaCuenta;
	gong::LineEdit *editCuentaDescripcion;
	gong::EditBox *editDebe;
	gong::EditBox *editHaber;
	gong::EditBox *editConcepto;
	gong::EditBox *editContrapartida;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITAPUNTE_CONTROLS*/
    SearchBox *searchContrapartidaCuenta;
    QPushButton *pushContrapartidaCuenta;
    LineEdit *editContrapartidaCuenta;
    LineEdit *editContrapartidaDescripcion;
    RecContrapartida *pRecContrapartida;
};

/*<<<<<FRMEDITAPUNTE_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABFRMEDITAPUNTE_H
/*>>>>>FRMEDITAPUNTE_POSTAMBLE*/


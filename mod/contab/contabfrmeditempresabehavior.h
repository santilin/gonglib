/*<<<<<FRMEDITEMPRESABEHAVIOR_PREAMBLE*/
#ifndef _CONTAB_CONTABFRMEDITEMPRESABEHAVIOR_H
#define _CONTAB_CONTABFRMEDITEMPRESABEHAVIOR_H
/*>>>>>FRMEDITEMPRESABEHAVIOR_PREAMBLE*/
/*<<<<<FRMEDITEMPRESABEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITEMPRESABEHAVIOR_INCLUDES*/

namespace gong {
namespace contab {


/*<<<<<COPYLEFT*/
/** @file contabfrmeditempresabehavior.h Fichero de edición de empresas adaptado por el plugin factu
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
/*<<<<<FRMEDITEMPRESA_PREAMBLE*/
#ifndef _CONTAB_CONTABFRMEDITEMPRESA_H
#define _CONTAB_CONTABFRMEDITEMPRESA_H
/*>>>>>FRMEDITEMPRESA_PREAMBLE*/
/*<<<<<FRMEDITEMPRESA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
/*>>>>>FRMEDITEMPRESA_INCLUDES*/
#include "empresafrmeditempresa.h"

namespace gong {
namespace contab {


/*<<<<<FRMEDITEMPRESA_CLASS*/
class FrmEditEmpresa: public empresa::FrmEditEmpresa
{
	Q_OBJECT

public:
	FrmEditEmpresa(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
/*>>>>>FRMEDITEMPRESA_CLASS*/
    /*<<<<<FRMEDITEMPRESA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITEMPRESA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITEMPRESA_CONTROLS*/
protected:
	gong::CheckBox *checkContabilidad;
	gong::EditBox *editInicioEjercicio;
	gong::EditBox *editFinEjercicio;
	gong::EditBox *editFechaContable;
	gong::EditBox *editDigitosSubcuentas;
	gong::EditBox *editCuentaCaja;
	gong::EditBox *editMaxNivelGrupo;
/*>>>>>FRMEDITEMPRESA_CONTROLS*/
};
/*<<<<<FRMEDITEMPRESA_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABFRMEDITEMPRESA_H
/*>>>>>FRMEDITEMPRESA_POSTAMBLE*/

/*<<<<<FRMEDITEMPRESABEHAVIOR_CLASS*/
class FrmEditEmpresaBehavior: public empresa::FrmEditEmpresa
{
public:
	virtual void initGUI();
	FrmEditEmpresaBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITEMPRESABEHAVIOR_CLASS*/
	{}
	// virtual void updateStatus();
	// virtual bool canClose();
/*<<<<<FRMEDITEMPRESABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITEMPRESABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
/*<<<<<FRMEDITEMPRESABEHAVIOR_CONTROLS*/
protected:
	gong::CheckBox *checkContabilidad;
	gong::EditBox *editInicioEjercicio;
	gong::EditBox *editFinEjercicio;
	gong::EditBox *editFechaContable;
	gong::EditBox *editDigitosSubcuentas;
	gong::EditBox *editCuentaCaja;
	gong::EditBox *editMaxNivelGrupo;
/*>>>>>FRMEDITEMPRESABEHAVIOR_CONTROLS*/
};
/*<<<<<FRMEDITEMPRESABEHAVIOR_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABFRMEDITEMPRESABEHAVIOR_H
/*>>>>>FRMEDITEMPRESABEHAVIOR_POSTAMBLE*/


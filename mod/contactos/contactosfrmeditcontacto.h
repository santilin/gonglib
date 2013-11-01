/*<<<<<FRMEDITCONTACTO_PREAMBLE*/
#ifndef _CONTACTOS_CONTACTOSFRMEDITCONTACTO_H
#define _CONTACTOS_CONTACTOSFRMEDITCONTACTO_H
/*>>>>>FRMEDITCONTACTO_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file contactosfrmeditcontacto.h Formulario de edición de contactos
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

/*<<<<<FRMEDITCONTACTO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "contactosreccontacto.h"
/*>>>>>FRMEDITCONTACTO_INCLUDES*/

class QToolButton;
namespace gong {
namespace contactos {

/*<<<<<FRMEDITCONTACTO_CLASS*/
class FrmEditContacto: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditContacto(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecContacto *getRecContacto() const { return (RecContacto *)getRecord(); }
/*>>>>>FRMEDITCONTACTO_CLASS*/

    /*<<<<<FRMEDITCONTACTO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITCONTACTO_VIRTUALS_FROM_FRMEDITREC*/
    bool mUsarTratamiento;

protected slots:
    void slot_searchCP_clicked();
    void editContactoImagen_clicked();

/*<<<<<FRMEDITCONTACTO_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNacionalidad;
	gong::EditBox *editFechaNacimiento;
	gong::EditBox *editCIF;
	gong::ComboBoxInt *comboTratamientoContacto;
	gong::EditBox *editNombre;
	gong::EditBox *editNombreAlt;
	gong::EditBox *editemail;
	gong::EditBox *editDireccion;
	gong::EditBox *editApdoCorreos;
	gong::EditBox *editCP;
	gong::EditBox *editLocalidad;
	gong::EditBox *editBarrio;
	gong::EditBox *editProvincia;
	gong::EditBox *editPais;
	gong::TextBox *editTelefono;
	gong::EditBox *editFax;
	gong::EditBox *editweb;
	gong::EditBox *editClave;
	gong::TextBox *editNotas;
	gong::ImageBox *editContactoImagen_Imagen;
/*>>>>>FRMEDITCONTACTO_CONTROLS*/
    QFrame *pFrameContactos, *pFrameDir, *pFrameResto;
    QToolButton *pSearchCP;
};

/*<<<<<FRMEDITCONTACTO_POSTAMBLE*/
} // namespace contactos
} // namespace gong
#endif // CONTACTOS_CONTACTOSFRMEDITCONTACTO_H
/*>>>>>FRMEDITCONTACTO_POSTAMBLE*/


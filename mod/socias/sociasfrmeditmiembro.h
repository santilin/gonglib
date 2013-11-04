/*<<<<<FRMEDITMIEMBRO_PREAMBLE*/
#ifndef _SOCIAS_SOCIASFRMEDITMIEMBRO_H
#define _SOCIAS_SOCIASFRMEDITMIEMBRO_H
/*>>>>>FRMEDITMIEMBRO_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file sociasfrmeditmiembro.h Formulario de edición de miembros de la asociación
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

/*<<<<<FRMEDITMIEMBRO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "sociasrecmiembro.h"
/*>>>>>FRMEDITMIEMBRO_INCLUDES*/
#include <contactosreccontacto.h>
#include "sociasrecproyecto.h"
#include "sociasrectiposocia.h"

namespace gong {
namespace socias {

/*<<<<<FRMEDITMIEMBRO_CLASS*/
class FrmEditMiembro: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditMiembro(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecMiembro *getRecMiembro() const { return (RecMiembro *)getRecord(); }
/*>>>>>FRMEDITMIEMBRO_CLASS*/
protected:
    virtual bool save(); // from FrmEditRec
    bool saveAllContactos();
    int getNextNumeroMiembro() const;

    /*<<<<<FRMEDITMIEMBRO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITMIEMBRO_VIRTUALS_FROM_FRMEDITREC*/

    /*<<<<<FRMEDITMIEMBRO_SCATTERS_AND_SLOTS*/
protected:
	void scatterProyecto();
	void scatterCliente();
	void scatterContacto();
	void scatterTipoSocia();
	void scatterFormaPago();

private slots:
	void pushProyectoCodigo_clicked();
	void pushClienteCodigo_clicked();
	void pushContactoCIF_clicked();
	void pushTipoSociaCodigo_clicked();
	void pushFormaPagoCodigo_clicked();

public:
	RecProyecto* getRecProyecto() const
		{ return static_cast<RecMiembro*>(getRecord())->getRecProyecto(); }
#ifdef HAVE_FACTUMODULE
	factu::RecCliente* getRecCliente() const
		{ return static_cast<RecMiembro*>(getRecord())->getRecCliente(); }
#endif
	contactos::RecContacto* getRecContacto() const
		{ return static_cast<RecMiembro*>(getRecord())->getRecContacto(); }
	RecTipoSocia* getRecTipoSocia() const
		{ return static_cast<RecMiembro*>(getRecord())->getRecTipoSocia(); }
#ifdef HAVE_PAGOSMODULE
	pagos::RecFormaPago* getRecFormaPago() const
		{ return static_cast<RecMiembro*>(getRecord())->getRecFormaPago(); }
#endif
/*>>>>>FRMEDITMIEMBRO_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITMIEMBRO_CONTROLS*/
protected:
	gong::SearchBox *searchProyectoCodigo;
	QPushButton *pushProyectoCodigo;
	gong::LineEdit *editProyectoCodigo;
	gong::LineEdit *editProyectoDescripcion;
	gong::EditBox *editNumeroSocia;
	gong::EditBox *editFechaAlta;
	gong::ComboBoxInt *comboSociasEstado;
#ifdef HAVE_FACTUMODULE
	gong::SearchBox *searchClienteCodigo;
	QPushButton *pushClienteCodigo;
	gong::LineEdit *editClienteCodigo;
	gong::LineEdit *editClienteRazonSocial;
#endif
	gong::SearchBox *searchContactoCIF;
	QPushButton *pushContactoCIF;
	gong::LineEdit *editContactoCIF;
	gong::LineEdit *editContactoNombre;
	gong::SearchBox *searchTipoSociaCodigo;
	QPushButton *pushTipoSociaCodigo;
	gong::LineEdit *editTipoSociaCodigo;
	gong::LineEdit *editTipoSociaNombre;
#ifdef HAVE_PAGOSMODULE
	gong::SearchBox *searchFormaPagoCodigo;
	QPushButton *pushFormaPagoCodigo;
	gong::LineEdit *editFormaPagoCodigo;
	gong::LineEdit *editFormaPagoNombre;
#endif
	gong::EditBox *editFechaBaja;
	gong::EditBox *editMotivoBaja;
	gong::EditBox *editImporteTotal;
	gong::EditBox *editPagosTotal;
	gong::EditBox *editRestoTotal;
	gong::EditBox *editCuentaBanco;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITMIEMBRO_CONTROLS*/
    bool mResumido;
    List<dbRecordID> mContactoList;
#ifdef HAVE_FACTUMODULE
    List<dbRecordID> mClienteList;
#endif
};

/*<<<<<FRMEDITMIEMBRO_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASFRMEDITMIEMBRO_H
/*>>>>>FRMEDITMIEMBRO_POSTAMBLE*/


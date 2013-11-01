/*<<<<<FRMEDITCONTACTOBEHAVIOR_PREAMBLE*/
#ifndef _CONTACTOS_CONTACTOSFRMEDITCONTACTOBEHAVIOR_H
#define _CONTACTOS_CONTACTOSFRMEDITCONTACTOBEHAVIOR_H
/*>>>>>FRMEDITCONTACTOBEHAVIOR_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file contactosfrmeditcontactobehavior.h Behavior para añadir edición de contactos a otros formularios
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

#include <dbappfrmeditrec.h>
/*<<<<<FRMEDITCONTACTOBEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITCONTACTOBEHAVIOR_INCLUDES*/
#include "contactosreccontacto.h"

namespace gong {
namespace contactos {

/*<<<<<FRMEDITCONTACTOBEHAVIOR_CLASS*/
class FrmEditContactoBehavior: public FrmEditRecBehavior
{
	Q_OBJECT
public:
	virtual void initGUI();
	FrmEditContactoBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITCONTACTOBEHAVIOR_CLASS*/
        , pFrameContactos(0), editRazonSocial(0)
        , mSetContactoIDNoNullAgain(false), mCreating(true), mSearching(0) {}
	virtual void _initGUI();
    RecContacto *getRecContacto() const;
    FrmEditContactoBehavior( FrmEditRec *theform, QTabWidget *holder, gong::LineEdit *razonsocial ):
        FrmEditRecBehavior( theform ), pFrameContactos( holder ), editRazonSocial( razonsocial ),
        mSetContactoIDNoNullAgain(false), mCreating(false), mSearching(0) {};
    LineEdit *getEditCIF() const {
        return editContactoCIF;
    }
    LineEdit *getEditNombre() const {
        return editContactoNombre;
    }
    bool setCIFAndLookForIt( const Xtring &cif );
    void setTabOrders( QWidget *pre, QWidget *post );
    bool isCreating() const {
        return mCreating;
    }

protected slots:
    void slotSearchCP_clicked();
    void slotLimpiaDatos_clicked();
    void slotARazonSocial_clicked();
    void slotEditFull_clicked();

    /*<<<<<FRMEDITCONTACTOBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	virtual void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITCONTACTOBEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
    dbRecordID findDupCIFOrName( QWidget *sender,
                                 bool codecanbenull, const Xtring &cond = Xtring::null );
    virtual bool save();
    bool mUsarTratamiento;
    /*<<<<<FRMEDITCONTACTOBEHAVIOR_SCATTERS_AND_SLOTS*/
protected:
	void scatterContacto();

private slots:
	void pushContactoCIF_clicked();

public:
/*>>>>>FRMEDITCONTACTOBEHAVIOR_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITCONTACTOBEHAVIOR_CONTROLS*/
protected:
	gong::SearchBox *searchContactoCIF;
	QPushButton *pushContactoCIF;
	gong::LineEdit *editContactoCIF;
	gong::LineEdit *editContactoNombre;
	gong::ComboBoxInt *comboContacto_TratamientoContacto;
	gong::EditBox *editContacto_Direccion;
	gong::EditBox *editContacto_CP;
	gong::EditBox *editContacto_Barrio;
	gong::EditBox *editContacto_Localidad;
	gong::EditBox *editContacto_Provincia;
	gong::EditBox *editContacto_email;
	gong::TextBox *editContacto_Telefono;
	gong::EditBox *editContacto_Codigo;
	gong::EditBox *editContacto_Nacionalidad;
	gong::EditBox *editContacto_FechaNacimiento;
	gong::EditBox *editContacto_NombreAlt;
	gong::EditBox *editContacto_Pais;
	gong::EditBox *editContacto_ApdoCorreos;
	gong::EditBox *editContacto_Fax;
	gong::EditBox *editContacto_web;
/*>>>>>FRMEDITCONTACTOBEHAVIOR_CONTROLS*/
    QTabWidget *pFrameContactos;
    gong::LineEdit *editRazonSocial;
    QPushButton *pushLimpiaDatos, *pushSearchCP, *pushARazonSocial, *pushEditFull;
private:
    bool mSetContactoIDNoNullAgain, mCreating;
    int mSearching;
};
/*<<<<<FRMEDITCONTACTOBEHAVIOR_POSTAMBLE*/
} // namespace contactos
} // namespace gong
#endif // CONTACTOS_CONTACTOSFRMEDITCONTACTOBEHAVIOR_H
/*>>>>>FRMEDITCONTACTOBEHAVIOR_POSTAMBLE*/


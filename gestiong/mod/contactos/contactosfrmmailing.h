#ifndef GONG_CONTACTOS_FRMMAILING_H
#define GONG_CONTACTOS_FRMMAILING_H

/** @file contactosfrmmailing.h Formulario para envío masivo de emails
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gonglist.h>
#include "dbappfrmcustom.h"

namespace gong {

class PushButton;

namespace contactos {

class FrmMailing: public FrmCustom
{
public:
    FrmMailing( QWidget* parent = 0, WidgetFlags fl = 0 );
    virtual int getEmailsList( XtringList &list,bool include_names ) const;
protected:
    void addEmailToList( XtringList &list, const Xtring &email,
                         const Xtring &nombre, bool include_names) const;
    virtual void validate_input( QWidget *sender, bool *isvalid ); // from FrmCustom
private:
    QTabWidget *tabFrameEdit;
    QWidget *tabSeleccion, *tabContenido, *tabConfiguracion;
    LineEdit *pSubject;
    class RichTextBox *pBody;
    LineEdit *pHost, *pUser, *pPassword, *pPort;
    List<SearchBox *> mSearchBoxes;
    PushButton *pushShowEMails, *pushShowEMailsAndNames;
};

} // namespace contactos
} // namespace gong

#endif // GONG_CONTACTOS_FRMMAILING_H

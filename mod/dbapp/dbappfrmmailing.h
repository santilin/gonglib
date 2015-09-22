#ifndef GONG_DBAPP_FRMMAILING_H
#define GONG_DBPPP_FRMMAILING_H

/** @file dbappfrmmailing.h Formulario para envío masivo de emails
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

class FrmMailing: public FrmCustom
{
public:
    FrmMailing( const Xtring &tabla_contactos, const Xtring &campo_id_contactos, QWidget* parent = 0, WidgetFlags fl = 0 );
    virtual int getEmailsList( XtringList &list,bool include_names ) const;
	void setAttachement(const Xtring &filename, const Xtring &mimetype);
protected:
    void addEmailToList( XtringList &list, const Xtring &email,
                         const Xtring &nombre, bool include_names) const;
    virtual void validate_input( QWidget *sender, bool *isvalid ); // from FrmCustom
    void accept(); // from FrmBase
    void addMessage( TextEdit *dest, const Xtring &message );
private:
    QTabWidget *tabFrameEdit;
    QWidget *tabSeleccion, *tabContenido, *tabMailsExtra, *tabConfiguracion, *tabResultado;
    LineEdit *pFrom, *pSubject;
    class RichTextBox *pHTMLBody;
    class FileNameBox *pAttachment;
    LineEdit *pHost, *pUser, *pPassword, *pPort, *pGrouping;
    List<SearchBox *> mSearchBoxes;
    PushButton *pushShowEMails, *pushShowEMailsAndNames;
    TextEdit *pMailsExtra, *pBody, *pResultado, *pOks, *pErrors;
    QCheckBox *pCheckSaveSettings;
    QLabel *lblProgreso;
	Xtring mTablaContactos, mCampoIdContactos, mMimeType;
};

} // namespace gong

#endif // GONG_DBAPP_FRMMAILING_H

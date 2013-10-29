#ifndef _GONG_DBAPPFRMEXPORT_GUI_H
#define _GONG_DBAPPFRMEXPORT_GUI_H

/** @file dbappfrmexport_gui.h A Form to export records (gui part)
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class Q3ListView;
class Q3ListViewItem;
class Q3Frame;
class QLabel;
class QPushButton;


#include <gongfrmbase.h>

namespace gong {

class TextEdit;
class LineEdit;

class GuiFrmExport : public FrmBase
{
    Q_OBJECT

public:
    GuiFrmExport( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );

    Q3ListView* treeExport;
    Q3Frame* frame3;
    QLabel* lblResumen;
    TextEdit* txtResumen;
    QLabel* lblFicheroDestino;
    LineEdit* txtDestino;
    QPushButton* pushBuscarFichero;

protected slots:
    virtual void slotCurrentChanged(Q3ListViewItem *);
    virtual void slotBuscarFicheroClicked();
    virtual void languageChange();

protected:
    Q3HBoxLayout* FrmExportLayout;
    Q3VBoxLayout* layout12;
    Q3VBoxLayout* layoutAceptarCancelar;
    Q3HBoxLayout* layout10;
    Q3HBoxLayout* frame3Layout;
    Q3VBoxLayout* layout9;
    Q3HBoxLayout* layout8;
    Q3HBoxLayout* accept_cancel_layout;
};

} // namespace gong
#endif // _GONG_DBAPPFRMEXPORT_H

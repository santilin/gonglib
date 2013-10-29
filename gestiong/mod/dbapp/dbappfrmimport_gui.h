#ifndef _GONG_DBAPPFRMIMPORT_GUI_H
#define _GONG_DBAPPFRMIMPORT_GUI_H

/** @file dbappfrmimport_gui.h A Form to import a records (gui part)
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
#include <q3wizard.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class Q3ButtonGroup;
class QComboBox;
class QLabel;
class Q3ListView;
class Q3ListViewItem;
class QPushButton;
class QRadioButton;
class QWidget;

#include <gongguicontrols.h>
#include <gongfrmbase.h>

namespace gong {

class GuiFrmImport : public Q3Wizard
{
    Q_OBJECT

public:
    GuiFrmImport( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );

    QWidget* page;
    QLabel *labelDescripcion;
    LineEdit *editDescripcion;
    QLabel* labelFicheroOrigen;
    LineEdit* editFicheroFugit;
    QPushButton* pushExaminarFugit;
    Q3ButtonGroup* groupFicherosGestiong;
    Q3ListView* treeImport;
    QWidget* page_2;
    QLabel* textLabel1;
    QComboBox* comboActualizar;
    QLabel* textLabel1_2_3_2;
    QComboBox* comboOtroEjercicio;
    QWidget* page_3;
    TextEdit* editResumen;

public slots:
    virtual void pushExaminarFugitClicked();

protected:
    Q3VBoxLayout* pageLayout;
    Q3VBoxLayout* layout14;
    Q3HBoxLayout* layout11;
    Q3HBoxLayout* layout12;
    Q3VBoxLayout* groupFicherosGestiongLayout;
    Q3VBoxLayout* pageLayout_2;
    Q3VBoxLayout* layout14_2;
    Q3HBoxLayout* layout54;
    Q3HBoxLayout* layout11_2_2;
    Q3HBoxLayout* layout11_2_2_2;
    Q3VBoxLayout* pageLayout_3;

private:
    void languageChange();
};

} // namespace gong
#endif // _GONG_DBAPPFRMIMPORT_GUI_H

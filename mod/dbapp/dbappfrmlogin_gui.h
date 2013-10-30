#ifndef _GONG_DBAPPFRMLOGIN_GUI_H
#define _GONG_DBAPPFRMLOGIN_GUI_H

/** @file dbappfrmimport.h The form to log in to the application (gui part)
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
#include <qpixmap.h>
#include <q3wizard.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QWidget;
class QLabel;
class Q3GroupBox;
class QCheckBox;
class QComboBox;
class QPushButton;
class Q3ButtonGroup;
class QRadioButton;

#include <gongguiapplication.h>
#include <gonglineedit.h>
#include <gongtextedit.h>

namespace gong {

class GuiWizardLogin : public Q3Wizard
{
    Q_OBJECT

public:
    GuiWizardLogin( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );

    QWidget* pageConexion;
    QLabel* textWelcome;
    QLabel* PixmapGestiong;
    TextEdit* textDetailed;
    Q3GroupBox* groupBox2;
    QLabel* labelUser;
    LineEdit* editUser;
    QLabel* labelPassword;
    LineEdit* editPassword;
    QCheckBox* chkSavePassword;
    QCheckBox* chkAutoLogin;
    Q3GroupBox* groupBox1;
    QLabel* labelDriver_2;
    QComboBox* comboDriver;
    QLabel* labelHost_3;
    LineEdit* editHost;
    QLabel* labelDatabaseName;
    LineEdit* editDatabaseName;
    QLabel* labelPort_2;
    LineEdit* editPort;
    QPushButton* pushCreateDatabase;
    QWidget* pageCreaDB;
    QLabel* textCreateDBWelcome;
    QLabel* PixmapGestiong_3;
    TextEdit* textCreateDBDetailed;
    Q3GroupBox* groupBox3;
    QLabel* labelCreateUserUser_2;
    LineEdit* editCreateUserUser;
    QLabel* labelCreateUserPassword_2;
    LineEdit* editCreateUserPassword;
    QLabel* labelCreateDBName;
    LineEdit* editCreateDBName;
    Q3GroupBox* groupBox4;
    QLabel* labelCreateDBDriver;
    QComboBox* comboCreateDBDriver;
    QLabel* labelCreateDBPort;
    LineEdit* editCreateDBPort;
    QLabel* labelCreateDBHost;
    LineEdit* editCreateDBHost;
    QLabel* labelCreateDBUser;
    LineEdit* editCreateDBUser;
    QLabel* labelCreateDBPassword;
    LineEdit* editCreateDBPassword;
    QWidget* page;

protected:
    Q3VBoxLayout* pageConexionLayout;
    Q3VBoxLayout* layout96;
    Q3VBoxLayout* layout82;
    Q3HBoxLayout* layout81;
    Q3VBoxLayout* layout80;
    QSpacerItem* spacer16;
    Q3HBoxLayout* layout95;
    Q3VBoxLayout* groupBox2Layout;
    Q3VBoxLayout* layout94;
    Q3HBoxLayout* layout63_2;
    Q3HBoxLayout* layout64_2;
    Q3HBoxLayout* layout67_2;
    Q3HBoxLayout* layout72_2;
    Q3VBoxLayout* groupBox1Layout;
    Q3VBoxLayout* layout46;
    Q3HBoxLayout* layout11_2;
    Q3HBoxLayout* layout62_2;
    Q3HBoxLayout* layout39_2;
    Q3VBoxLayout* pageCreaDBLayout;
    Q3VBoxLayout* layout109;
    Q3VBoxLayout* layout75;
    Q3HBoxLayout* layout74;
    Q3VBoxLayout* layout73;
    QSpacerItem* spacer14;
    Q3HBoxLayout* layout108;
    Q3VBoxLayout* groupBox3Layout;
    Q3VBoxLayout* layout70;
    Q3HBoxLayout* layout81_2;
    Q3HBoxLayout* layout82_2;
    Q3HBoxLayout* layout83_2;
    Q3HBoxLayout* layout84_2;
    Q3VBoxLayout* groupBox4Layout;
    Q3VBoxLayout* layout77_3;
    Q3HBoxLayout* layout59_2;
    Q3HBoxLayout* layout78;
    Q3HBoxLayout* layout77;
    Q3HBoxLayout* layout79;
    Q3HBoxLayout* layout60_2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
};

} // namespace gong
#endif // _GONG_DBAPPFORMLOGIN_GUI_H

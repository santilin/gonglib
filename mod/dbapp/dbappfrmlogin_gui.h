#ifndef _GONG_DBAPPFRMLOGIN_GUI_H
#define _GONG_DBAPPFRMLOGIN_GUI_H

/** @file dbappfrmlogin.h The form to log in to the application (gui part)
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QDialog>
#include <gongguinamespace.h>

class QSpacerItem;
class QWidget;
class QLabel;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QPushButton;
class QRadioButton;
class QTabWidget;
class QHBoxLayout;
class QVBoxLayout;

namespace gong {

class TextEdit;
class LineEdit;

class GuiFrmLogin: public QDialog
{
    Q_OBJECT

public:
    GuiFrmLogin( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );

	QTabWidget *pTabWidget;
    QWidget* pageConexion;
    QLabel* textWelcome;
    QLabel* PixmapGestiong;
    TextEdit* textDetailed;
    QGroupBox* groupBox2;
    QLabel* labelUser;
    LineEdit* editUser;
    QLabel* labelPassword;
    LineEdit* editPassword;
    QCheckBox* chkSavePassword;
    QCheckBox* chkAutoLogin;
    QGroupBox* groupBox1;
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
    QGroupBox* groupBox3;
    QLabel* labelCreateUserUser_2;
    LineEdit* editCreateUserUser;
    QLabel* labelCreateUserPassword_2;
    LineEdit* editCreateUserPassword;
    QLabel* labelCreateDBName;
    LineEdit* editCreateDBName;
    QGroupBox* groupBox4;
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

protected:
    QVBoxLayout* pageConexionLayout;
    QVBoxLayout* layout96;
    QVBoxLayout* layout82;
    QHBoxLayout* layout81;
    QVBoxLayout* layout80;
    QSpacerItem* spacer16;
    QHBoxLayout* layout95;
    QVBoxLayout* groupBox2Layout;
    QVBoxLayout* layout94;
    QHBoxLayout* layout63_2;
    QHBoxLayout* layout64_2;
    QHBoxLayout* layout67_2;
    QHBoxLayout* layout72_2;
    QVBoxLayout* groupBox1Layout;
    QVBoxLayout* layout46;
    QHBoxLayout* layout11_2;
    QHBoxLayout* layout62_2;
    QHBoxLayout* layout39_2;
    QVBoxLayout* pageCreaDBLayout;
    QVBoxLayout* layout109;
    QVBoxLayout* layout75;
    QHBoxLayout* layout74;
    QVBoxLayout* layout73;
    QSpacerItem* spacer14;
    QHBoxLayout* layout108;
    QVBoxLayout* groupBox3Layout;
    QVBoxLayout* layout70;
    QHBoxLayout* layout81_2;
    QHBoxLayout* layout82_2;
    QHBoxLayout* layout83_2;
    QHBoxLayout* layout84_2;
    QVBoxLayout* groupBox4Layout;
    QVBoxLayout* layout77_3;
    QHBoxLayout* layout59_2;
    QHBoxLayout* layout78;
    QHBoxLayout* layout77;
    QHBoxLayout* layout79;
    QHBoxLayout* layout60_2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
};

} // namespace gong
#endif // _GONG_DBAPPFORMLOGIN_GUI_H

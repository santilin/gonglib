#ifndef _DBAPP_FRMREPORTS_GUI_H
#define _DBAPP_FRMREPORTS_GUI_H

/** @file dbappfrmreports.h A form to show the reports of the application (gui part)
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
#include <QVBoxLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListView;
class QListViewItem;
class QFrame;
class QLabel;
class QComboBox;
class QCheckBox;
class QPushButton;

#include <q3listview.h>
#include <gongfrmbase.h>

namespace gong {

class GuiFrmReports : public FrmBase
{
    Q_OBJECT

public:
    GuiFrmReports( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    ~GuiFrmReports();

    Q3ListView* treeReports;
    QFrame* frame3;
    QLabel* lblSalida;
    QComboBox* cmbSalida;
    QPushButton* pushFilter;
    QPushButton* pushOrder;
    TextEdit *txtOrder;
    TextEdit* txtFilter;
    QLabel* lblTitulo;
    LineEdit* txtTitulo;
    QCheckBox* chkResumido;
    QCheckBox* chkApaisado;
    QCheckBox* chkAlternateColores;
    QCheckBox* chkAskParameters;
    QLabel* lblEstilo;
    QComboBox* cmbEstilo;

protected:
    QVBoxLayout* FrmReportsLayout;
    QHBoxLayout* layoutMain;
    QVBoxLayout* frame3Layout;
    QVBoxLayout* layout15;
    QHBoxLayout* layout9_2;
    QHBoxLayout* layout14;
    QHBoxLayout* layout9;
    QHBoxLayout* layoutAceptarCancelar;

protected slots:
    virtual void languageChange();
};

}

#endif // _DBAPP_FRMREPORTS_GUI_H


#ifndef _GONG_FRMSQLSELECT_H
#define _GONG_FRMSQLSELECT_H

/** @file dbappfrmsqlselect.h A form to choose a filter for a FrmEditRecMaster
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <vector>
#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QLabel;
class Q3ListBox;
class Q3ListBoxItem;
class QPushButton;

#include "gongfrmbase.h"
#include "gongguiapplication.h"

namespace gong {

class dbRecord;
class dbTableDefinition;
class dbFieldDefinition;
class LineEdit;
class TextEdit;
class dbConnection;
class dbDefinition;

class FrmSQLSelect : public FrmBase
{
    Q_OBJECT

public:
    FrmSQLSelect ( dbConnection *conn, dbDefinition *db,
                   QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    ~FrmSQLSelect();

    QLabel* labelCampoDecha;
    Q3ListBox* lbCamposIzda;
    QLabel* labelOperador;
    Q3ListBox* lbOperador;
    QLabel* labelValorComparado;
    LineEdit* textValorComparado;
    QLabel* labelCampoComparado;
    Q3ListBox* lbCamposDecha;
    QLabel* labelSQLText;
    TextEdit* textSQL;
    QPushButton *pushEliminarFiltro;
    QPushButton *pushAnadirO;
    QPushButton *pushAnadirY;
    QPushButton *pushRellenarCampo;

public slots:
    virtual void lbCamposDecha_highlighted ( int index );
    virtual void pushAnadirY_clicked();
    virtual void pushAnadirO_clicked();
    virtual void pushRellenarCampo_clicked();
    virtual void lbCamposIzda_highlighted ( int index );
    virtual void pushEliminarFiltro_clicked();
    virtual void validateValorComparado ( QWidget *, bool * );
protected:
    Q3VBoxLayout* FrmSQLSelectLayout;
    Q3VBoxLayout* layout12;
    Q3VBoxLayout* layout11;
    Q3HBoxLayout* layout9;
    Q3VBoxLayout* layout6;
    Q3VBoxLayout* layout8;
    Q3VBoxLayout* layout7;
    Q3VBoxLayout* layout11_2;
    Q3HBoxLayout* layout9_2;
    Q3HBoxLayout* layoutAceptarCancelar;

protected slots:
    virtual void languageChange();

public:
    int exec ( QWidget *parent, const dbRecord *record, const Xtring &currentfilter=Xtring::null );
    int exec ( QWidget *parent, const XtringList &tables, const Xtring &currentfilter=Xtring::null );
    int exec ( QWidget *parent, const List<dbTableDefinition *> &tabledefs, const Xtring &currentfilter=Xtring::null );
    Xtring getSqlExpression();

private:
    dbDefinition *pDatabase;
    dbConnection *pConnection;

    Variant chooseValue(dbConnection *conn,
                        const dbTableDefinition *tbldef, const dbFieldDefinition *flddef,
                        QWidget *parent, const char* name = 0, WidgetFlags fl = WidgetFlags(0) );
    enum aggop { none, sumof, countof, maxof, minof, yearof, monthof, dayof };
    struct item_info
    {
        const dbTableDefinition *tabledef;
        const dbFieldDefinition *fielddef;
        aggop operation;
    };
    std::vector<item_info> mItemsInfo;

    Xtring getCondition();
    Xtring getSubCondition ( item_info iileft, int operindex );
    Xtring getFldExpression ( const item_info &ii ) const;
    int addFieldToListBoxes ( const Xtring &text, item_info &iteminfo );

};

}

#endif // _GONG_FRMSQLSELECT_H


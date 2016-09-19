#ifndef _GONG_DBAPP_FRMCUSTOM_H
#define _GONG_DBAPP_FRMCUSTOM_H

/** @file dbappfrmcustom.h The base of all non edit forms
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QVBoxLayout>
#include <QFrame>
#include <gongfrmbase.h>
#include <gonglineedit.h>
#include <gongrichtextedit.h>
#include <gongpushbutton.h>
#include "dbappdaterangebox.h"
#include "dbappsearchbox.h"
#include "dbappdbapplication.h"


namespace gong {

class FrmCustom: public FrmBase
{
    Q_OBJECT
public:
    FrmCustom( QWidget *parent=0, const char *name=0, WidgetFlags fl = 0 );
    virtual void accept(); // from FrmBase
    // Tab widget
    void showTabs(bool show = true );
    QWidget *insertTab(QWidget *tab, const Xtring &label, const char *name = 0, int index = -1);
    void setTabTitle( QWidget *tab, const Xtring &title );

    // Independent controls. The name is necessary in these controls
    QLabel *addLabel( QWidget *parent, const Xtring &caption, const Variant &value,
                      const Xtring &style = Xtring::null, const char *name = 0, QBoxLayout *layout = 0 );
    LineEdit *addInput ( QWidget *parent, const Xtring &caption, const Variant &value,
                         const Xtring &style = Xtring::null, const char *name = 0, QBoxLayout *layout = 0 );
    TextEdit *addTextEditBox( QWidget *parent, const Xtring &caption, const Xtring &value,
                              const char *name, QBoxLayout *layout = 0 );
    CheckBox *addCheckBox( QWidget *parent, const Xtring &caption,
                           bool value, const char *name = 0, QBoxLayout *layout = 0 );
    ComboBoxXtring *addComboBoxXtring( bool byref, QWidget *parent, const Xtring &caption,
                                       const XtringList &captions, const XtringList &values, const Xtring &empty = Xtring::null,
                                       const char *name = 0, QBoxLayout * layout = 0, bool horiz = true );
    ComboBoxXtring *addComboBoxXtring( bool byref, QWidget *parent, const Xtring &caption,
                                       const XtringList &captions_values, const Xtring &empty = Xtring::null,
                                       const char *name = 0, QBoxLayout * layout = 0, bool horiz = true );
    ComboBoxInt *addComboBoxInt( bool byref, QWidget *parent, const Xtring &caption,
                                 const XtringList &captions, const IntList &values, const Xtring &empty = Xtring::null,
                                 const char *name = 0, QBoxLayout * layout = 0, bool horiz = true );
    GroupBox *addGroupBox( QWidget *parent, XtringList &options, const Xtring &caption,
                           int selected, QBoxLayout *layout = 0, bool horiz = true );
    DateRangeBox *addDateRangeBox( QWidget *parent, const Xtring &caption,
                                   const DateTime date_from, const DateTime date_to,
                                   const Xtring &caption_from = Xtring::null,
                                   const Xtring &caption_to = Xtring::null,
                                   const char *name = 0, QBoxLayout *layout = 0 );
    PushButton *addButton( QWidget *parent, const Xtring &caption,
                           const char *name = 0, QBoxLayout *layout = 0 );
    RichTextBox *addRichTextBox( QWidget *parent, const Xtring &caption,
                                 const char *name = 0, QBoxLayout *layout = 0, bool horiz = true );
    FileNameBox *addFileNameBox( QWidget *parent, const Xtring &caption,
                                 const char *name = 0, QBoxLayout *layout = 0, bool horiz = true );
    FileNameBox *addDirNameBox( QWidget *parent, const Xtring &caption,
                                const char *name = 0, QBoxLayout *layout = 0, bool horiz = true);
	QLabel *addImage( QWidget *parent, QPixmap &pixmap, const char *name = 0, QBoxLayout *layout = 0);

    // Controls bound to fields
    LineEdit *addInputField ( QWidget *parent, const Xtring &caption, const Xtring &tablename,
                              const Xtring &fldname, const Variant &value, QBoxLayout *layout = 0 );
    CheckBox *addCheckField ( QWidget *parent, const Xtring &caption,
                              const Xtring &tablename, const Xtring &fldname, bool value,
                              const char *name = 0, QBoxLayout *layout = 0  );
    ComboBoxInt *addComboIntField( QWidget *parent, const Xtring &caption, const Xtring &tablename,
                                   const Xtring &fldnamecaptions, const Xtring &fldnamevalues,
                                   const Xtring &empty = Xtring::null,
                                   const char *name = 0, QBoxLayout * layout = 0 );
    ComboBoxXtring *addComboXtringField(QWidget *parent, const Xtring &tablename,
                                        const Xtring &fldname, QBoxLayout *layout = 0, bool horiz = true);
    SearchBox *addSearchField( QWidget *parent, const Xtring &tablename,
                               const Xtring &fldnamecodigo, const Xtring &fldnamenombre, QBoxLayout *layout = 0,
                               SearchBox::Flags flags = SearchBox::FlagHorizontal );
    SearchBox *addMultipleSearchField( QWidget *parent, const Xtring &tablename,
                                       const Xtring &fldnamecodigo, const Xtring &fldnamenombre, QBoxLayout *layout = 0 );
    SearchBox *addLabeledSearchField( QWidget *parent, const Xtring &tablename,
                                      const Xtring &fldnamecodigo, const Xtring &fldnamenombre, QBoxLayout *layout = 0 );
protected slots:
    virtual void validate_input( QWidget *, bool * );
    virtual bool validate() {
        return true;
    }
    void combo_activated( int );
    void button_clicked();

protected:
    QTabWidget *pTabWidget;
    QWidget *pControlsFrame;
    QVBoxLayout *pFormLayout, *pMainLayout, *pControlsLayout;
    QHBoxLayout *pButtonsLayout;
};

} // namespace gong

#endif // _GONG_DBAPP_FRMCUSTOM_H

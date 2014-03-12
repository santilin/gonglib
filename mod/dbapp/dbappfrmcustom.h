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
#include "dbappsearchbox.h"
#include "dbappdbapplication.h"

namespace gong {

class DateRangeBox
{
public:
    DateRangeBox( QWidget *parent, const Xtring &name, const Xtring &caption,
                  Variant::Type type, const DateTime date_from, const DateTime date_to,
                  const Xtring &caption_from, const Xtring &caption_to,
                  QBoxLayout *layout = 0);
    Date getDateFrom() const;
    DateTime getDateTimeFrom() const;
    Date getDateTo() const;
    DateTime getDateTimeTo() const;
    QBoxLayout *getLayout() const {
        return pLayout;
    }
    LineEdit *getEditDateFrom() const {
        return pEditDateFrom;
    }
    LineEdit *getEditDateTo() const {
        return pEditDateTo;
    }
private:
    LineEdit *pEditDateFrom, *pEditDateTo;
    QBoxLayout *pLayout;
};

class FrmCustom: public FrmBase
{
    Q_OBJECT
public:
    FrmCustom( QWidget *parent=0, const char *name=0, WidgetFlags fl = 0 );
	// Tab widget
    void showTabs(bool show = true );
    QWidget *insertTab(QWidget *tab, const Xtring &label, int index = -1);
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
                           int selected, bool horiz = true, QBoxLayout *layout = 0 );
    DateRangeBox *addDateRangeBox( QWidget *parent, const Xtring &caption,
                                   const DateTime date_from, const DateTime date_to,
                                   const Xtring &caption_from = Xtring::null,
                                   const Xtring &caption_to = Xtring::null,
                                   const char *name = 0, QBoxLayout *layout = 0 );
    PushButton *addButton( QWidget *parent, const Xtring &caption,
                           const char *name = 0, QBoxLayout *layout = 0 );
    RichTextBox *addRichTextBox( QWidget *parent, const Xtring &caption,
                                 const char *name = 0, QBoxLayout *layout = 0 );
    FileNameBox *addFileNameBox( QWidget *parent, const Xtring &caption,
                                 bool horiz = true, QBoxLayout *layout = 0);
    FileNameBox *addDirNameBox( QWidget *parent, const Xtring &caption,
                                bool horiz = true, QBoxLayout *layout = 0);
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
    ComboBoxXtring *addComboXtringField( QWidget *parent, const Xtring &caption, const Xtring &tablename,
                                     const Xtring &fldnamecaptions, const Xtring &fldnamevalues,
                                     const Xtring &empty = Xtring::null,
                                     const char *name = 0, QBoxLayout * layout = 0 );
    SearchBox *addSearchField( QWidget *parent, const Xtring &tablename,
                               const Xtring &fldnamecodigo, const Xtring &fldnamenombre, QBoxLayout *layout = 0,
                               SearchBox::Flags flags = SearchBox::FlagHorizontal );
    SearchBox *addMultipleSearchField( QWidget *parent, const Xtring &tablename,
                                       const Xtring &fldnamecodigo, const Xtring &fldnamenombre, QBoxLayout *layout = 0 );
    SearchBox *addLabeledSearchField( QWidget *parent, const Xtring &tablename,
                                      const Xtring &fldnamecodigo, const Xtring &fldnamenombre, QBoxLayout *layout = 0 );
	virtual void accept(); // from FrmBase
protected slots:
    virtual void validate_input( QWidget *, bool * );
    virtual bool validate() { return true; }
    void combo_activated( int );

protected:
    QTabWidget *pTabWidget;
    QWidget* pControlsFrame;
    QVBoxLayout *pFormLayout, *pMainLayout, *pControlsLayout;
    QHBoxLayout *pButtonsLayout;

private:	
    void button_clicked();
};

} // namespace gong

#endif // _GONG_DBAPP_FRMCUSTOM_H

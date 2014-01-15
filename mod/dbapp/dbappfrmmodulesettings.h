#ifndef _GONG_FRMMODULESETTINGS_H
#define _GONG_FRMMODULESETTINGS_H

/** @file dbappfrmmodulesettings.h A form to edit the settings of the modules
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
#include "dbappfrmcustom.h"
#include "dbappdbmodule.h"

namespace gong {
class dbModule;

class FrmModuleSettings: public FrmCustom
{
	Q_OBJECT
public:
	typedef struct {
		QWidget *w;
		bool edited, resetted;
		Xtring modulename;
		const dbModuleSetting *settinginfo;
		QPushButton *resetButton;
	} ControlInfo;
	typedef std::vector<ControlInfo> ControlsList;
    FrmModuleSettings( dbModuleSetting::Scope type, QWidget* parent = 0, WidgetFlags fl = 0 );

protected slots:
	void resetButton_clicked();

protected:
    virtual void accept(); // from FrmBase
    virtual bool canClose(); // from FrmBase
    virtual void validate_input( QWidget *, bool * ); // from FrmCustom
	void setControlValue( QWidget *control, const Variant &value );
	void setControlColor( ControlInfo *ci, const Variant &value );
    void addModuleSettings( dbModule *module, QVBoxLayout *layout );
    void gather();
    void scatter();

private:
    QTabWidget *pTabWidget;
    ControlsList mControls;
    dbModuleSetting::Scope mSettingsScope;
    Settings *pSettings;
};

}

#endif


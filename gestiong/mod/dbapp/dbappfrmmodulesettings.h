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
typedef struct {
    QWidget *w;
    bool edited;
    const Xtring *modulename;
    const dbModuleSetting *settinginfo;
} SettingsControlInfo;
typedef std::vector<SettingsControlInfo> SettingsControlsList;
class dbModule;

class FrmModuleSettings: public FrmCustom
{
public:
    enum SettingsType { Machine = 0, Global, GlobalUser, LocalUser };
    FrmModuleSettings( SettingsType type, QWidget* parent = 0, WidgetFlags fl = 0 );

protected:
    virtual void accept(); // from FrmBase
    virtual bool canClose(); // from FrmBase
    virtual void validate_input( QWidget *, bool * ); // from FrmCustom
    void addModuleSettings( dbModule *module, QVBoxLayout *layout );
    void gather();
    void scatter();

private:
    QTabWidget *pTabWidget;
    SettingsControlsList mControls;
    SettingsType mSettingsType;
    Settings *pSettings;
};

}

#endif


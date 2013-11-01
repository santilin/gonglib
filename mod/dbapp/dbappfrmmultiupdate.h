#ifndef GONG_DBAPP_MULTIUPDATE_H
#define GONG_DBAPP_MULTIUPDATE_H

/** @file dbappfrmmultiupdate.h A form to update multiple records from a FrmEditRecMaster
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongcombobox.h>
#include "dbappfrmcustom.h"

namespace gong {

class FrmEditRecMaster;
class EditLine;

class FrmMultiUpdate: public FrmCustom
{
public:
    FrmMultiUpdate( FrmEditRecMaster *theform,
                    QWidget* parent = 0, WidgetFlags fl = 0 );
protected:
    virtual void accept(); // from FrmBase
    virtual void validate_input( QWidget *sender, bool *isvalid ); // from FrmCustom
    QVBoxLayout *pFrmMultiUpdateLayout;
    QHBoxLayout *pMainLayout;
    LineEdit *pValor;
    GroupBox *pRango;
    CheckBox *pSupervisar;
    ComboBoxInt *pComboField;

private:
    FrmEditRecMaster *pTheForm;
};

} // namespace gong

#endif // MULTIUPDATE_H

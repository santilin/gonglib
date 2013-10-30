#ifndef GONG_DBAPP_ADVANCEDPRINT_H
#define GONG_DBAPP_ADVANCEDPRINT_H

/** @file dbappfrmadvancedprint.h Advancedprint form for frmeditrec
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

namespace gong {

class FrmEditRecMaster;

class FrmAdvancedPrint: public FrmCustom
{
public:
    FrmAdvancedPrint( FrmEditRecMaster *theform,
                      QWidget* parent = 0, WidgetFlags fl = 0 );
protected:
    virtual void accept(); // from FrmBase
    virtual void validate_input( QWidget *sender, bool *isvalid ); // from FrmCustom
    Xtring mCondition;
    QVBoxLayout *pFrmAdvancedPrintLayout;
    QHBoxLayout *pMainLayout;
    GroupBox *pRange;
    FileNameBox *pReportFileName;
    LineEdit *pTitle;
    CheckBox *pSummary, *pIncFilterInHeader, *pIncLogo, *pLandscape, *pShowTemplate,
             *pIncTotals, *pOneByOne;
    ComboBox<Xtring> *pComboTemplates;

private:
    FrmEditRecMaster *pTheForm;
};

} // namespace gong

#endif // ADVANCEDPRINT_H

#ifndef GONG_DBAPP_FRMCSVPARAMS_H
#define GONG_DBAPP_FRMCSVPARAMS_H

/** @file dbappfrmcsvparams.h A form to ask for csv parameters
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

class FrmCsvParams: public FrmCustom
{
public:
    enum Modes { importing, exporting };
	enum Ranges { current, selected, all };
    FrmCsvParams( Modes mode, QWidget *parent, const Xtring &title,
		const Xtring &separator = ",", const Xtring &delimiter = "\"", 
		const Xtring &encoding = "UTF-8" );
    Xtring getSeparator() const {
        return pEditSeparator->toString();
    }
    Xtring getDelimiter() const {
        return pEditDelimiter->toString();
    }
    Xtring getEncoding() const {
        return pEditEncoding->toString();
    }
    Xtring getExportFilename() const {
        return pEditExportFilename->getFileName();
    }
    Xtring getImportFilename() const {
        return pEditImportFilename->getFileName();
    }
    int getRange() const {
		return pRange->getSelectedPos();
	}
private:
    virtual bool validate();

    FileNameBox *pEditExportFilename, *pEditImportFilename;
    GroupBox *pRange;
    LineEdit *pEditSeparator, *pEditDelimiter, *pEditEncoding;
	PushButton *pPushShowTemplate;
};

} // namespace

#endif // GONG_DBAPP_FRMCSVPARAMS_H
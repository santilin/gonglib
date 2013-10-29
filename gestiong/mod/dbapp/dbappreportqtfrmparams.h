#ifndef DBAPP_REPORTQTFRMPARAMS_H
#define DBAPP_REPORTQTFRMPARAMS_H

/** @file dbappreportqtfrmparams.h A form to ask for parameters for a report
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
#include <qdialog.h>
#include <qwidget.h>
#include <gongguiapplication.h>

namespace gong {

class ReportQtFrmParams: public QDialog
{
public:
    ReportQtFrmParams(class Report &r, QWidget *parent=0, const char *name=0, WidgetFlags f=0);
    ~ReportQtFrmParams();

    virtual void accept(); // from QDialog

private:
    class Report &mReport;
    QPushButton* pushCancelar;
    QPushButton* pushAceptar;
    std::vector<QWidget *>mControls;
};

} // namespace RTK

#endif // DBAPP_REPORTQTFRMPARAMS_H


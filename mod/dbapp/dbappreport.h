#ifndef _DBAPP_REPORT_H
#define _DBAPP_REPORT_H

/** @file dbappreport.h A wrapper for RTK::Report
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongtreeitem.h>
#include <gongxtring.h>
#include <gonglist.h>
#include <gongdictionary.h>
#include <rtkreport.h>

namespace gong {

class dbApplication;
class ReportViewer;
class ReportInput;
class dbViewDefinition;

struct stReportInfo {
    Xtring repGroup;
    Xtring repName;
    Xtring repPath;
    Xtring repTitle;
    XtringList repFrom;
    Xtring repWhere;
    XtringList repOrderBy;
    XtringList repFields;
};
typedef std::vector<stReportInfo> ReportsList;
enum RTK_Output_Type { RTK_Screen = 0, RTK_Printer_Without_Dialog, RTK_Printer_With_Dialog, RTK_PDF, RTK_OpenOffice_Calc, RTK_PostScript, RTK_Text, RTK_GNumeric, RTK_CSV };

class AppReport : public RTK::Report
{
public:
    AppReport(const dbApplication &dbapp);
    ~AppReport();
    bool readFile(const Xtring &rtkfilename, const Xtring &initdefines = Xtring(), Input *input = 0);
    bool readString(const Xtring &rtkfilename, const Xtring &initdefines = Xtring(), Input *input = 0);
    Xtring fromViewDefinition( const dbViewDefinition *viewdef, bool totals = true) const;
    int readAndPrint( RTK_Output_Type tiposalida, const Xtring &rtkfilename,
                      const Xtring &filter = Xtring::null, const Xtring &initdefines = Xtring::null,
                      const Dictionary<Xtring> &properties = Dictionary<Xtring> (),
                      const Xtring &order = Xtring::null, PageOrientation po = DefaultOrientation,
                      bool askforparameters = false );
    int print(RTK_Output_Type tiposalida, const Dictionary<Xtring> &properties,
              const Xtring &filter, const Xtring &order, PageOrientation po, bool askforparameters);
    void clearErrors() {
        return RTK::Report::clearErrors();
    }
    static void readAllReports( bool force = false );
    static int readReportsFromPath(const Xtring &pathorig, ReportsList &reportslist = mReportsList );
    static const ReportsList &getReportsList() { return mReportsList; }
    static void clearList() { mReportsList.clear(); }
    static Xtring getReportSubTitle() { return sReportSubTitle; }
    static void setReportSubTitle( const Xtring &title ) { sReportSubTitle = title; }
    static Xtring getReportAuthor() { return sReportAuthor; }
    static void setReportAuthor( const Xtring &title ) { sReportAuthor = title; }
    static Xtring getReportCompany() { return sReportCompany; }
    static void setReportCompany( const Xtring &title ) { sReportCompany = title; }

    /* Looks for a property in the whole report, usually called from formulae */
    virtual Variant getExternalPropertyValue(const Xtring &propname) const; // from Report
    virtual Variant callExternalFunction(const Xtring &function, Variant &value1, Variant &value2); // from Report

private:
    ReportViewer *mViewer;
    static ReportsList mReportsList;
    RTK::Input *pGongInput;
    const dbApplication &mdbApp;
    static Xtring sReportAuthor, sReportCompany, sReportSubTitle;
};

} // namespace gong

#endif // _DBAPP_REPORT_H

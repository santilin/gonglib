#include "config.h"
#include <gonggettext.h>
#include <gongfileutils.h>
#include "dbappdbapplication.h"
#include "dbappfrmeditrecmaster.h"
#ifdef HAVE_RTKMODULE
#include "dbappreport.h"
#endif
#include "dbappfrmadvancedprint.h"

namespace gong {


FrmAdvancedPrint::FrmAdvancedPrint(FrmEditRecMaster *theform,
                                   QWidget* parent, WidgetFlags fl)
    : FrmCustom(parent, "FrmAdvancedPrint", fl), pTheForm( theform )
{
    setCaption( toGUI(_("Impresión avanzada")) );
    Xtring reporttitle = theform->getDataTable()->getDataModel()->getCurrentView()->getCaption();
    pTitle = addInput(0, _("Título"), reporttitle, "STRING");
    Xtring viewname = theform->getDataTable()->getDataModel()->getCurrentView()->getName();
    viewname.replace(".","_");
    XtringList rangeoptions;
    rangeoptions << _("Registro actual") << _("Registros seleccionados")
                 << _("Filtro actual") << _("Todo");
    int range;
    if( theform->getDataTable()->getSelectedIDs().size() > 1 ) {
        range = 1;
    } else if( !theform->getWholeFilter().isEmpty()
               && theform->getWholeFilter() != theform->getRecord()->getFilter("") ) {
        range = 2;
    } else {
        range = 0;
    }
    pRange = addGroupBox(0, rangeoptions, _("Rango"), range);


    XtringList templates_captions;
    templates_captions << _("Tabla: ") + theform->getRecord()->getTableName() + ".rtk"
                       << _("Vista: ") + viewname
                       << _("Crearlo a partir de la vista actual")
                       << _("Buscar otro fichero");
    // add reports
#ifdef HAVE_RTKMODULE
    AppReport::readAllReports();
    ReportsList rplst = AppReport::getReportsList();
    for( ReportsList::iterator it = rplst.begin();
            it != rplst.end(); 	++it) {
        _GONG_DEBUG_PRINT(0, Xtring::printf("Titulo: %s, repFrom:%s",
                                            (*it).repTitle.c_str(), (*it).repFrom.join(",").c_str() ) );
        if( (*it).repFrom.contains( pTheForm->getRecord()->getTableName() ) ) {
            templates_captions << (*it).repPath;
        }
    }
#endif

    pComboTemplates = addComboBoxXtring(false, 0, _("Elige un informe:"), templates_captions );

    pReportFileName = addFileNameBox(0, _("Buscar otro informe") );
    pReportFileName->setFileName( theform->getRecord()->getTableName() + ".rtk" );
    pReportFileName->setEnabled( false );
    pSummary = addCheckBox(0, _("Resumido"), false );
//	pSummary->setVisible( false );
    pIncTotals = addCheckBox(0, _("Incluir totales"), true );
    pOneByOne = addCheckBox(0, _("Imprimir un informe por cada registro"), false );
    pIncFilterInHeader = addCheckBox(0, _("Incluir filtro en la cabecera"), false );
    pIncLogo = addCheckBox(0, _("Incluir logo"), false );
    pLandscape = addCheckBox(0, _("Apaisado"), DBAPP->getAppSetting("RTK.LANDSCAPE").toBool() );
    pShowTemplate = addCheckBox(0, _("Mostrar la plantilla antes de imprimir"), false );
    pShowTemplate->setEnabled( false );
}

void FrmAdvancedPrint::validate_input( QWidget *sender, bool *isvalid )
{
    FrmCustom::validate_input( sender, isvalid );
#ifdef HAVE_RTKMODULE
    if( sender == pComboTemplates ) {
        Xtring reportname;
        switch( pComboTemplates->currentIndex() ) {
        case 0: // Tabla
            reportname = pTheForm->getRecord()->getTableName() + ".rtk";
            pReportFileName->setEnabled( false );
            pShowTemplate->setChecked( false );
            pShowTemplate->setEnabled( false );
            break;
        case 1: // Vista
            reportname = pTheForm->getDataTable()->getDataModel()->getCurrentView()->getName();
            reportname.replace(".","_");
            reportname += ".rtk";
            pReportFileName->setEnabled( false );
            pShowTemplate->setChecked( false );
            pShowTemplate->setEnabled( false );
            break;
        case 2: // Crear desde la vista
            reportname = Xtring::null;
            pReportFileName->setEnabled( false );
            pShowTemplate->setEnabled( true );
            break;
        case 3: // Elegir otro
            pReportFileName->setEnabled( true );
            pReportFileName->setFocus();
            pShowTemplate->setChecked( false );
            pShowTemplate->setEnabled( false );
            break;
        default:
            reportname = pComboTemplates->getCurrentItemValue();
        }
        pReportFileName->setFileName( reportname );
    }
#endif
}


void FrmAdvancedPrint::accept()
{
#ifdef HAVE_RTKMODULE
    if( pComboTemplates->currentIndex() == 3 && pReportFileName->getFileName().isEmpty() ) {
        msgError(this, _("Por favor, elige un informe") );
        return;
    }
    mCondition.clear();
    switch( pRange->getSelectedPos() ) {
    case 0: // Registro actual
        mCondition += pTheForm->getRecord()->getIDSqlCond( pTheForm->getTableRecordID() );
        break;
    case 1: // Registros seleccionados
        mCondition += pTheForm->getRecord()->getIDSqlCond( pTheForm->getDataTable()->getSelectedIDs() );
        break;
    case 2: // Filtro actual
        mCondition += pTheForm->getWholeFilter();
        break;
    case 3: // Todos
        mCondition = pTheForm->getRecord()->getFilter("");
        break;
    }
    Xtring defines;
    Dictionary<Xtring> properties;
    properties.insert( "TITLE", pTitle->toString() );
    if( pSummary->isOn() ) {
        defines += "RESUMIDO,";
// 		properties.insert( "DETAILS.SUPRESSED", "true" );
    }
    if( pIncLogo->isOn() )
        defines += "INCLOGO,";
    if( pIncFilterInHeader->isOn() )
        defines += "INCFILTER,";
    Xtring reportfilename;
    bool isafilename;
    AppReport *report = 0;
    if( pComboTemplates->currentIndex() == 2 ) { // Crear a partir de la vista
        report = new AppReport( *DBAPP );
        Xtring rtk = report->fromViewDefinition(
                         pTheForm->getDataTable()->getDataModel()->getCurrentView(),
                         pIncTotals->isOn() );
        if( pShowTemplate->isOn() ) {
            msgOkLarge(this,
                       _("Esta es la plantilla generada a partir de la vista actual. Puedes guardarla como un fichero .rtk"),
                       rtk);
        }
        reportfilename = rtk;
        isafilename = false;
    } else {
        reportfilename = pReportFileName->getFileName();
        isafilename = true;
    }
    if( pOneByOne->isOn() ) {
        if( pRange->getSelectedPos() == 1 ) {
            for( List<dbRecordID>::const_iterator idit = pTheForm->getDataTable()->getSelectedIDs().begin();
                    idit != pTheForm->getDataTable()->getSelectedIDs().end(); ++ idit ) {
                pTheForm->printReport(reportfilename, properties,
                                      pTheForm->getRecord()->getIDSqlCond( *idit ),
                                      pLandscape->isOn() ? Landscape : DefaultOrientation,
                                      defines, isafilename, false /*resorttofilename*/ );
            }
        } else {
            msgOk( this, _("Imprimir uno por uno solo se puede hacer cuando se seleccionan registros uno por uno") );
            return;
        }
    } else {
        pTheForm->printReport(reportfilename, properties, mCondition,
                              pLandscape->isOn() ? Landscape : DefaultOrientation,
                              defines, isafilename, false /*resorttofilename*/ );
    }
    if( report )
        delete report;

#endif
}


} // namespace gong




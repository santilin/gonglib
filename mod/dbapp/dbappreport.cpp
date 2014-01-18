#include "config.h"
#include <unistd.h>
#include <stdio.h> // tmpnam
#include <errno.h>
#include <gonggettext.h>
#include <gongfileutils.h>
#include <gongdbfielddefinition.h>
#include <dbappreportqtoutput.h>
#include <rtkoutputodfcalc1.h>
#include <rtkoutputgnumeric.h>
#include <rtkoutputps.h>
#include <rtkoutputtext.h>
#include <rtkoutputcsv.h>
#include "dbappmainwindow.h"
#include "dbappdbapplication.h"
#include "dbappdbmodule.h"
#include "dbappreportqtfrmparams.h"
#include "dbappreportinput.h"
#include "dbappreport.h"
#include "dbappreportqtviewer.h"

namespace gong {

ReportsList AppReport::mReportsList;
Xtring AppReport::sReportSubTitle;
Xtring AppReport::sReportCompany;
Xtring AppReport::sReportAuthor;

// Subclase de QReportViewer para poder cerrarla con la tecla escape
class ReportViewer: public ReportQtViewer
{
public:
    ReportViewer( Output *output, bool autodelete = false,
                  QWidget *parent = 0, const char *name = 0, WidgetFlags f = 0 )
        : ReportQtViewer( output, autodelete, parent, name, f) {}
private:
    void keyPressEvent( QKeyEvent *ke );
};

void ReportViewer::keyPressEvent( QKeyEvent *ke )
{
	_GONG_DEBUG_PRINT(0, ke->key() );
    if( !(ke->state() & (Qt::ControlModifier | Qt::ShiftModifier | Qt::MetaModifier | Qt::AltModifier )) )
        if( ke->key() == Qt::Key_Escape ) {
            close();
            return;
        }
    ReportQtViewer::keyPressEvent( ke );
}

AppReport::AppReport( const dbApplication &dbapp, dbConnection *conn )
    : gong::Report ( dbapp.getRegConfig() ), pConnection(conn), pGongInput ( 0 ),
      mdbApp( dbapp )
{
    Xtring paths = DBAPP->getReportsPath( true );
    setStylesPath ( paths );
    setImagesPath ( paths );
    setIncludePath( paths );
}

AppReport::~AppReport()
{
    if ( pGongInput != defaultInput() )
        delete pGongInput;
}

bool AppReport::readFile( const Xtring &rtkfilename, const Xtring &initdefines, Input *input )
{
    pGongInput = input;
    if( !pGongInput )
        pGongInput = new ReportInput ( pConnection, *this, 0 );
    return readRtkFile ( rtkfilename.c_str(), 0, pGongInput, initdefines );
}

bool AppReport::readString( const Xtring &rtkcode, const Xtring &initdefines, Input *input )
{
    pGongInput = input;
    if( !pGongInput )
        pGongInput = new ReportInput ( pConnection, *this, 0 );
    return readRtkString ( rtkcode.c_str(), 0, "From String", pGongInput, initdefines );
}



/// \todo {refactor} {0.3.4} Función genérica para pedir un fichero y comprobar si se va a sobreescribir
int AppReport::print( RTK_Output_Type tiposalida, const Dictionary<Xtring> &properties,
                      const Xtring &filter, const Xtring &order,
                      PageOrientation po, bool askforparameters )
{
    DBAPP->waitCursor ( true );
    int ret = 0;
    bool goon = true;
    Xtring title = properties["TITLE"];
    if( title.isEmpty() )
        title = propTitle.getOrig();
    if( title.isEmpty() )
        title = _("Informe");
    if( !filter.isEmpty() ) /// TODO: mezclar con report.filter
        setParameterValue( "report_filter", filter );
	setParameterValue( "COMPANY", sReportCompany );
	setParameterValue( "AUTHOR", sReportAuthor );
	setParameterValue( "SUBTITLE", sReportSubTitle );
    if ( askforparameters && getParametersCount() )  // Is there any parameter?
    {
        ReportQtFrmParams *frmparam = new ReportQtFrmParams ( *this );
        frmparam->exec();
        if ( frmparam->result() == QDialog::Rejected )
            goon = false;
    }
    if ( goon )
    {
        Xtring fname;
        Xtring fn;
        Output *salida = 0;
        switch ( tiposalida )
        {
        case RTK_Screen:
        case RTK_Printer_With_Dialog:
        case RTK_Printer_Without_Dialog:
            salida = new ReportQtOutput ( *this, 0.0, 0.0, A4, po, 0.0, 0.0,
                                          DBAPP->getAppSetting("RTK.OUTPUT.SCREEN.MARGINLEFT").toDouble(),
                                          DBAPP->getAppSetting("RTK.OUTPUT.SCREEN.MARGINRIGHT").toDouble(),
                                          DBAPP->getAppSetting("RTK.OUTPUT.SCREEN.MARGINTOP").toDouble(),
                                          DBAPP->getAppSetting("RTK.OUTPUT.SCREEN.MARGINBOTTOM").toDouble() );
            break;
        case RTK_OpenOffice_Calc:
            fn = theGuiApp->getSaveFileName ( title,
                                              Xtring::null,
                                              _( "Hoja de cálculo de Open/LibreOffice (*.ods)" ),
                                              DBAPP->getMainWindow() );
            if ( !fn.isEmpty() )
            {
                fname = fn ;
                if ( FileUtils::extension ( fname ).isEmpty() )
                    fname += ".ods";
                salida = new OutputODFCalc1 ( *this, fname.c_str(),
                                              "UTF-8", Xtring(DBAPP->getGlobalDataDir() + "/opendocument/calc/template").c_str(),
                                              OutputODFCalc1::CREATE, 72, 72, gong::A4, po );
            }
            break;
        case RTK_GNumeric:
            fn = theGuiApp->getSaveFileName ( title,
                                              Xtring::null,
                                              _( "Hoja de cálculo de GNUmeric (*.gnumeric)" ),
                                              DBAPP->getMainWindow() );
            if ( !fn.isEmpty() )
            {
                fname = fn;
                if ( FileUtils::extension ( fname ).isEmpty() )
                    fname += ".gnumeric";
                salida = new OutputGNumeric ( *this, fname.c_str(), "UTF-8",
                                              OutputGNumeric::CREATE, 72, 72, gong::A4, po );
            }
            break;
        case RTK_PostScript:
            fn = theGuiApp->getSaveFileName ( title,
                                              Xtring::null,
                                              _( "Fichero PostScript (*.ps)" ),
                                              DBAPP->getMainWindow() );
            if ( !fn.isEmpty() )
            {
                fname = fn;
                if ( FileUtils::extension ( fname ).isEmpty() )
                    fname += ".ps";
                salida = new OutputPS ( *this, fname.c_str(),
                                        72, 72, gong::A4, po, 0, 0, 20, 20, 20, 20 );
            }
            break;
        case RTK_PDF:
            fn = theGuiApp->getSaveFileName ( title,
                                              Xtring::null,
                                              _( "Fichero Portable Document Format (*.pdf)" ),
                                              DBAPP->getMainWindow() );
            if ( !fn.isEmpty() )
            {
                fname = fn;
                if ( FileUtils::extension ( fname ).isEmpty() )
                    fname += ".pdf";
                salida = new OutputPS ( *this, ( fname + ".~~~" ).c_str(),
                                        72, 72, gong::A4, po, 0, 0,
                                        DBAPP->getAppSetting("RTK.OUTPUT.PDF.MARGINLEFT").toDouble(),
                                        DBAPP->getAppSetting("RTK.OUTPUT.PDF.MARGINRIGHT").toDouble(),
                                        DBAPP->getAppSetting("RTK.OUTPUT.PDF.MARGINTOP").toDouble(),
                                        DBAPP->getAppSetting("RTK.OUTPUT.PDF.MARGINBOTTOM").toDouble() );
            }
            break;
        case RTK_Text:
            fn = theGuiApp->getSaveFileName ( title,
                                              Xtring::null,
                                              _( "Fichero de texto (*.txt)" ),
                                              DBAPP->getMainWindow() );
            if ( !fn.isEmpty() ) {
                fname = fn;
                if ( FileUtils::extension ( fname ).isEmpty() )
                    fname += ".txt";
                salida = new OutputText ( *this, fname.c_str(), 72, 72, gong::A4, po );
            }
            break;
        case RTK_CSV:
            fn = theGuiApp->getSaveFileName ( title,
                                              Xtring::null,
                                              _( "Fichero de valores separados por comas (*.csv)" ),
                                              DBAPP->getMainWindow() );
            if ( !fn.isEmpty() )
            {
                fname = fn;
                if ( FileUtils::extension ( fname ).isEmpty() )
                    fname += ".csv";
                salida = new OutputCsv ( *this, fname.c_str() );
            }
            break;

        }
        if( !salida ) {
            DBAPP->resetCursor();
            return 0;
        }
        if( !filter.isEmpty() )
            pGongInput->setFilter( filter );
#if 0
        if( !filter.isEmpty() ) {
            if( strempty( pGongInput->where() ) )
                pGongInput->setOrigWhere( filter.c_str() );
            else
                pGongInput->setOrigWhere(
                    (Xtring( "(" ) + pGongInput->propWhere().get() + ")AND(" + filter + ")").c_str() );
        }
#endif
        setOrigOrder ( order.c_str() );
        for( Dictionary<Xtring>::const_iterator propit = properties.begin();
                propit != properties.end(); ++ propit ) {
            if( !setPropOrigValue( propit->first, propit->second ) )
				if( !setParameterValue( propit->first, propit->second) )
                _GONG_DEBUG_WARNING( "Neither property nor parameter '" + propit->first + "'  found in report" );
        }
        try {
            ret = Report::print ( pGongInput, salida );
        } catch ( const dbError &e ) {
            addError ( RTK::Error::InternalError, "RtkGongInput", e.what() );
        } catch ( std::exception &e ) {
            addError ( RTK::Error::InternalError, 0, e.what() );
        }
        if ( tiposalida == RTK_Screen || tiposalida == 	RTK_Printer_With_Dialog
                || tiposalida == RTK_Printer_Without_Dialog ) {
            if ( ret == true ) {
                // No borrar salida, que ya la borrará el viewer
                mViewer = new ReportViewer ( salida, true, DBAPP->getMainWindow()->getViewport() );
                mViewer->setObjectName( tmpnam(0) );
                mViewer->setCaption ( toGUI( Xtring::printf( _( "Informe: %s" ), title.c_str() ) ) );
                mViewer->setPageDimensions ( ( int ) ( salida->sizeX() ), ( int ) ( salida->getFolioSizeY() ) );
                mViewer->setPageCollection ( static_cast<ReportQtOutput *> ( salida )->getPageCollection() );
                if( tiposalida == RTK_Screen ) {
                    DBAPP->getMainWindow()->createClient ( mViewer );
                    mViewer->slotFirstPage();
                } else if( tiposalida == RTK_Printer_With_Dialog ) {
                    mViewer->show();
                    mViewer->printWithDialog();
                    delete mViewer;
                } else {
                    mViewer->show();
                    mViewer->printWithoutDialog();
                    delete mViewer;
                }
            }
        } else {
            if ( ret == true && tiposalida == RTK_PDF ) {
                Xtring cmd2pdf = "ps2pdf \"" + fname + ".~~~\" \"" + fname + "\"";
                ret = FileUtils::execProcess ( cmd2pdf.c_str() );
                unlink ( ( fname + ".~~~" ).c_str() );
            }
            if ( ret == true )
                FrmBase::msgOk ( DBAPP->getPackageString(),
                                 Xtring::printf ( "El informe se ha guardado correctamente en %s",
                                                  fname.c_str() ), FrmBase::information );
            delete salida;
        }
        if ( !ret || errorsCount() ) {
            Xtring errores;
            int e, maxerrs = 10; // Do not show more that this number of errors
            for ( e = 0; e < errorsCount() && maxerrs > 0; e++ ) {
                if ( !getError(e) ->isWarning() ) {
                    maxerrs --;
                    errores += Xtring::printf ( "Error: %s: %s\n",
                                                getError(e)->location(), getError(e) ->message() );
                }
            }
            if( maxerrs < 10 ) {
                for ( e = 0; e < errorsCount() && --maxerrs > 0; e++ ) {
                    if ( getError(e) ->isWarning() ) {
                        maxerrs --;
                        errores += Xtring::printf ( "Warning: %s: %s\n",
                                                    getError(e)->location(), getError(e) ->message() );
                    }
                }
            } else {
                for ( e = 0; e < errorsCount() && --maxerrs > 0; e++ ) {
                    if ( getError(e) ->isWarning() ) {
                        maxerrs --;
                        _GONG_DEBUG_WARNING( Xtring::printf ( "Warning: %s: %s",
                                                              getError(e)->location(), getError(e) ->message() ) );
                    }
                }
            }
            if( !errores.isEmpty() )
                FrmBase::msgError( DBAPP->getPackageString(), errores );
        }
    }
    DBAPP->resetCursor();
    return ret;
}


/**
 * @brief prints a rtk filename in just one call
 *        Can't have an input, as the input needs a report in order to be created
 *
 * @param tiposalida ...
 * @param rtkfilename ...
 * @param initdefines ...
 * @param properties ...
 * @param filter ...
 * @param order ...
 * @param po ...
 * @param askforparameters ...
 * @return int
 **/
int AppReport::readAndPrint(RTK_Output_Type tiposalida, const Xtring& rtkfilename,
                            const Xtring& filter, const Xtring& initdefines,
                            const Dictionary< Xtring >& properties,
                            const Xtring& order, PageOrientation po, bool askforparameters )
{
    Xtring paths = DBAPP->getReportsPath( true );
    DBAPP->waitCursor( true );
    int ret = readFile( FileUtils::findInPath( paths, rtkfilename ), initdefines );
    if( ret ) {
        ret = print( tiposalida, properties, filter, Xtring::null,
                     po, askforparameters );
        if( !ret ) {
            if ( errorsCount() != 0 ) {
                FrmBase::msgError( DBAPP->getPackageString(), Xtring::printf(
                                       _( "En el informe %.100s:\n%s" ),
                                       rtkfilename.c_str(), lastError()->message() ) );
            }
        }
    } else {
        FrmBase::msgError( DBAPP->getPackageString(), Xtring::printf(
                               _( "Imposible encontrar el informe %s.\nBuscando en %s" ),
                               rtkfilename.c_str(),
                               paths.replace( ":", "\n" ).c_str() ) );
    }
    DBAPP->resetCursor();
    return ret;
}

void AppReport::readAllReports( bool force )
{
    if( force )
        AppReport::clearList();
    if( AppReport::getReportsList().size() )
        return;
    XtringList reportpaths;
    Xtring paths = DBAPP->getReportsPath( true );
    paths.tokenize( reportpaths, ":" );
    for( XtringList::const_iterator it = reportpaths.begin();
            it != reportpaths.end();
            ++ it ) {
        Xtring path;
        if( !(*it).startsWith("/") && !DBAPP->getGlobalDataDir().isEmpty() )
            path = DBAPP->getGlobalDataDir() + *it;
        else
            path = *it;
        _GONG_DEBUG_PRINT(3, "Reading reports from " + path );
        AppReport::readReportsFromPath( path );
    }
}

int AppReport::readReportsFromPath ( const Xtring &pathorig, ReportsList &reportslist )
{
    RTK::Error::sErrorAbort = RTK::Error::AbortNever;
    _GONG_DEBUG_PRINT ( 3, "Leyendo informes desde " + pathorig );
    XtringList reports = FileUtils::getDirEntries ( pathorig );
    for ( XtringList::iterator it = reports.begin();
            it != reports.end();
            ++it ) {
        if ( FileUtils::extension ( *it ) == "rtk" ) {
            _GONG_DEBUG_PRINT ( 4, Xtring::printf ( "Informe: %s%s", pathorig.c_str(), ( *it ).c_str() ) );
            RTK::Report * r = new RTK::Report();
            ReportInput *inp = new ReportInput ( DBAPP->getConnection(), *r, 0 );
            if ( r->readRtkFile ( Xtring ( pathorig + "/" + ( *it ) ).c_str(), 0, inp ) )
            {
                stReportInfo ri;
                r->fixParameters ( RTK::ParametersList(), "" );
                ri.repPath = pathorig + "/" + ( *it );
                ri.repName = r->name();
                ri.repTitle = r->title();
                ri.repGroup = r->tag();
                if( inp->getOrigFrom() )
                    ri.repFrom = dbConnection::extractSqlTables ( Xtring ( "FROM " ) + inp->getOrigFrom() );
                ri.repWhere = inp->getOrigWhere();
                ri.repOrderBy << inp->getOrigOrderBy();
                reportslist.push_back ( ri );
            } else {
                for ( int e=0; e < r->errorsCount(); e++ ) {
                    _GONG_DEBUG_WARNING ( Xtring::printf ( "%s: %s: %s",
                                                           r->getError(e)->isWarning() ? "Warning":"Error",
                                                           r->getError(e)->location(),
                                                           r->getError(e)->message() ) );
                }
            }
            delete r;
        }
    }
    return reportslist.size();
}

/* the grammar only admits one . in propname, so we use module.modname_tablename_fldname */
Variant AppReport::getExternalPropertyValue( const Xtring &propname ) const
{
    _GONG_DEBUG_PRINT(4, propname );
    Variant ret;
    Xtring object, property;
    propname.upper().splitIn2( object, property, ".");
    if ( object == "DDD" )
    {
        XtringList names;
        property.tokenize ( names, "_" );
        if ( names.size() == 3 )
        {
            Xtring &tablename = names[0];
            Xtring &fieldname = names[1];
            Xtring &propname = names[2];
            dbFieldDefinition *flddef =
                DBAPP->getDatabase()->findFieldDefinition ( tablename, fieldname );
            if ( flddef )
            {
                if ( propname == "CAPTION" )
                {
                    return flddef->getCaption();
                }
                else if ( propname == "DESCRIPTION" )
                {
                    return flddef->getDescription();
                }
                else if ( propname == "WIDTH" || propname == "SIZE" )
                {
                    return flddef->getStyleWidth( DBAPP->getDatabase() );
                }
                else if ( propname == "DEFAULT" || propname == "DEFAULTVALUE" )
                {
                    return flddef->getDefaultValue();
                }
                else if ( propname == "STYLE" )
                {
                    return flddef->getStyle();
                }
                else
                    _GONG_DEBUG_WARNING ( Xtring::printf ( "property not found %s", propname.c_str() ) );
            }
            else
                _GONG_DEBUG_WARNING ( Xtring::printf ( "field not found %s.%s",
                                                       tablename.c_str(), fieldname.c_str() ) );
        }
        else if ( names.size() == 2 )
        {
            Xtring &tablename = names[0];
            Xtring &propname = names[1];
            dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition ( tablename );
            if ( tbldef )
            {
                if ( propname == "DESCPLURAL" )
                {
                    return tbldef->getDescPlural();
                }
                else if ( propname == "DESCSINGULAR" )
                {
                    return tbldef->getDescSingular();
                }
                else if ( propname == "FEMENINA" )
                {
                    return tbldef->isFemenina();
                }
                else
                    _GONG_DEBUG_WARNING ( Xtring::printf ( "property not found %s",
                                                           propname.c_str() ) );
            }
            else
                _GONG_DEBUG_WARNING ( Xtring::printf ( "table not found %s",
                                                       tablename.c_str() ) );
        }
        else
            _GONG_DEBUG_WARNING ( Xtring::printf ( "missing table name, field name or property name %s",
                                                   propname.c_str() ) );
    } else if( object == "MODULE" ) {
        Xtring modname, fullfldname;
        property.splitIn2( modname, fullfldname, "_" );
        dbModule *dbmod = DBAPP->findModule(modname.upper());
        if( dbmod ) {
            ret = dbmod->getGlobalProperty(fullfldname.replace("_","."));
        } else {
            _GONG_DEBUG_WARNING("Module " + modname + " not found.");
        }
    }
    if ( !ret.isValid() )
        ret = RTK::Report::getExternalPropertyValue( propname );
    return ret;
}

Variant AppReport::callExternalFunction(const Xtring& _function, Variant& value1, Variant &value2)
{
	const char *function = _function.c_str();
	if( strcasecmp( function, "getDisplayValue") == 0 ) {
		const dbFieldDefinition *flddef = DBAPP->getDatabase()->findFieldDefinition( value1.toString() );
		return flddef->getDisplayValue( value2 );
	}
	_GONG_DEBUG_WARNING( "Function " + _function + " not found" );
	return Report::callExternalFunction( _function, value1, value2 );
}

Xtring AppReport::fromViewDefinition( const dbViewDefinition* viewdef, bool totals ) const
{
    const int report_size = 100;

    static Xtring page_header =
        "\tPAGEHEADER page_header {\n"
        "\t\tsizey = 4;\n"
        "\t\tstyle = pageheader;\n"
        "\t\tbackgroundimage = logo;\n"
        "\t\tOBJECT subtitle {\n"
        "\t\t\tstyle = footer ;\n"
        "\t\t\tvalue=\"###subtitle###\";\n"
        "\t\t\thalignment = left;\n"
        "\t\t\tfontweight = bold;\n"
        "\t\t\tsizey = 1;\n"
        "\t\t\tposx = 0;\n"
        "\t\t}\n"
        "\t\tOBJECT reportdate {\n"
        "\t\t\tstyle = pagenumber;\n"
        "\t\t\tvalue=\"= const @report.Date\";\n"
        "\t\t\tformat=\"%d/%m/%Y %H:%M\";\n"
        "\t\t\tsizey = 1;\n"
        "\t\t\tposx = " + Xtring::number(report_size - 28) + ";\n"
        "\t\t\tsizex = 16;\n"
        "\t\t}\n"
        "\t\tOBJECT npagina_text {\n"
        "\t\t\tstyle = pagenumber;\n"
        "\t\t\thalignment = right;\n"
        "\t\t\tvalue=\"Pág.\";\n"
        "\t\t\tsizey = 1;\n"
        "\t\t\tposx = " + Xtring::number(report_size - 10) + ";\n"
        "\t\t\tsizex = 4 ;\n"
        "\t\t}\n"
        "\t\tOBJECT npagina {\n"
        "\t\t\tstyle = pagenumber;\n"
        "\t\t\tsizey = 1;\n"
        "\t\t\tposx = " + Xtring::number(report_size - 5) + ";\n"
        "\t\t\thalignment = right;\n"
        "\t\t\tvalue=\"= @report.PageNumber\";\n"
        "\t\t\tsizex = 5;\n"
        "\t\t}\n"
        "\t\tOBJECT titulo {\n"
        "\t\t\tstyle = title;\n"
        "\t\t\tvalue = \"= const @report.Title\";\n"
        "\t\t\tposy = 1;\n"
        "\t\t\tsizey = 2;\n"
        "\t\t\thalignment = center;\n"
        "\t\t\tfontsize = 14;\n"
        "\t\t\tfontweight = bold;\n"
        "\t\t}\n"
        "\t\tOBJECT selection {\n"
        "\t\t\tfontsize = 8;\n"
        "\t\t\thalignment = right;\n"
        "\t\t\tadjustment = none;\n"
        "\t\t\tvalue=\"###selection###\";\n"
        "\t\t\tposx = 0;\n"
        "\t\t\tposy = 3;\n"
        "\t\t\tsizey = 1;\n"
        "\t\t}\n"
        "\t}\n";

    Xtring inputfields, header, details, footer;
    int totalwidth = 0, posx = 0;
    double scale = 1.0;
    // Calculate total width of the report
    for( unsigned int i=0; i<viewdef->getFieldDefinitions().size(); i++) {
        dbFieldDefinition *flddef = viewdef->getFieldDefinitions()[i];
        if( flddef->getName() == "ID" || !flddef->isVisible() )
            continue;
        dbFieldDefinition *origflddef = mdbApp.getDatabase()->findFieldDefinition( flddef->getFullName() );
        int width = flddef->getStyleWidth( DBAPP->getDatabase() );
        _GONG_DEBUG_PRINT( 4, Xtring::printf("fld: %s, viewwitdh= %d", flddef->getName().c_str(), width )  );
        if( width == 0 )
            width = origflddef->getStyleWidth( DBAPP->getDatabase() );
        _GONG_DEBUG_PRINT( 4, Xtring::printf("fld: %s, origwitdh= %d", flddef->getName().c_str(), width )  );
        if( width > 50 )
            width = 50;
        if( width == 0 )
            width = 10;
        totalwidth += width;
    }
    if( totalwidth != 0 )
        scale = report_size / (double)totalwidth;
    for( unsigned int i=0; i<viewdef->getFieldDefinitions().size(); i++) {
        dbFieldDefinition *flddef = viewdef->getFieldDefinitions()[i];
        if( flddef->getName() == "ID" || !flddef->isVisible() )
            continue;
        dbFieldDefinition *origflddef = mdbApp.getDatabase()->findFieldDefinition( flddef->getFullName() );
        int width = flddef->getStyleWidth( DBAPP->getDatabase() );
        if( width == 0 )
            width = origflddef->getStyleWidth( DBAPP->getDatabase() );
        if( width > 50 )
            width = 50;
        if( width == 0 )
            width = 10;
        width *= scale;
        Xtring source = flddef->getFullName();
        Xtring rtkfldname = source.upper();
        rtkfldname.replace(".","_");
        // Insert a dummy field so that the order of the view is correct
        if( inputfields.isEmpty() ) {
            inputfields += "\t\tINPUTFIELD " + rtkfldname  + "_dummy {\n"
                           "\t\t\tSource = " + source + ";\n"
                           "\t\t\tType = " + Variant::typeToName( flddef->getVariantType() ) + ";\n"
                           "\t\t}\n";
        }
        inputfields += "\t\tINPUTFIELD " + rtkfldname + " {\n"
                       "\t\t\tSource = " + source + ";\n"
                       "\t\t\tType = " + Variant::typeToName( flddef->getVariantType() ) + ";\n"
                       "\t\t}\n";
        header += "\t\tOBJECT header_" + rtkfldname + " {\n"
                  "\t\t\tvalue = \"" +  flddef->getCaption() + "\";\n"
                  "\t\t\tposx = " + Xtring::number( posx ) + ";\n"
                  "\t\t\tsizex = " + Xtring::number( width ) + ";\n"
                  "\t\t}\n";
        Xtring salign = "left";
        dbFieldStyle *fldstyle = mdbApp.getDatabase()->findFieldStyle( flddef->getStyle() );
        if( fldstyle ) {
            switch( fldstyle->getHAlignment() ) {
            case dbFieldStyle::AlignRight:
                salign = "right";
                break;
            case dbFieldStyle::AlignHCenter:
                salign = "center";
                break;
            default:
                break;
            }
        } else {
            if( flddef->getVariantType() == Variant::tBool )
                salign = "center";
            else if( Variant::isNumeric( flddef->getVariantType() ) ) {
                salign = "right";
            }
        }
        details += "\t\tOBJECT " + rtkfldname + " {\n"
                   "\t\t\tvalue = \"=input." +  rtkfldname + "\";\n"
                   "\t\t\thalignment = \"" + salign + "\";\n"
                   "\t\t\tposx = " + Xtring::number( posx ) + ";\n"
                   "\t\t\tsizex = " + Xtring::number( width ) + ";\n";
        if( flddef->getVariantType() == Variant::tBool )
            details+="\t\t\tformat=\"No|Sí\";\n";
        details += "\t\t}\n";

        if( totals ) {
            if( Variant::isNumeric( flddef->getVariantType() ) && !flddef->isCode()
                    && flddef->getVariantType() != Variant::tBool ) {
                footer += "\t\tOBJECT suma_" + rtkfldname + " {\n"
                          "\t\t\tvalue = \"=input." +  rtkfldname + "\";\n"
                          "\t\t\thalignment = \"" + salign + "\";\n"
                          "\t\t\tposx = " + Xtring::number( posx ) + ";\n"
                          "\t\t\tsizex = " + Xtring::number( width ) + ";\n"
                          "\t\t\taggregate = sum;\n"
                          "\t\t}\n";
            }
        }
        posx += (width + 1);
    }
    Xtring report_return = "REPORT " + Xtring(viewdef->getName()).replace(".","_") + "\n"
                           "{\n"
                           "\tTitle = \"" + viewdef->getCaption() + "\";\n"
                           "\tUnits = chars;\n"
                           "\tSizeX = " + Xtring::number(report_size) + ";\n"
                           "\tSizeY = 60;\n"
                           "\tPARAMETER subtitle {\n"
                           "\t\ttype = text;\n"
                           "\t\tvalue = \"" + sReportSubTitle + "\";\n"
                           "\t}\n"
                           "\tPARAMETER selection {\n"
                           "\t\ttype = text;\n"
                           "\t\tvalue = \"\";\n"
                           "\t}\n"
                           "\n"
                           "\tINPUT default {\n"
                           "\t\tdriver = mysql;\n"
                           "\t\tfrom = \"" + viewdef->getFrom() + "\";\n"
                           "\t\torderby = \"" + viewdef->getOrderBy() + "\";\n"
                           "\n" + inputfields + "\n"
                           "\t}\n"
                           "\n" + page_header + "\n"
                           "\tPAGEHEADER header_captions {\n"
                           "\t\tsizey=1;\n"
                           "\t\tfontweight = bold;\n"
                           "\t\tborderbottomstyle = solid;\n"
                           "\t\tborderbottomcolor = black;\n"
                           "\t\tborderbottomwidth = 1;\n"
                           "\n" + header + " \n"
                           "\t}\n"
                           "#ifndef RESUMIDO\n"
                           "\tDETAILS details {\n"
                           "\t\tposx = 0;\n"
                           "\t\tlevel = 0;\n"
                           "\t\tstyle = detail;\n"
                           "\t\tsizey = 1;\n"
                           "\t\tbackcolor = \"gray&white\";\n"
                           "\n"
                           "\n" + details + " \n"
                           "\t}\n"
                           "#endif\n"
                           "\tREPORTFOOTER page_footer {\n"
                           "\t\tsizey = 1.4;\n"
                           "\t\tbordertopstyle = solid;\n"
                           "\t\tpaddingtop = 0.4;\n"
                           "\t\tstyle = pagefooter;\n"
                           "\n"
                           "\n" + footer + " \n"
                           "\t}\n"
                           "\n";
    return report_return + "}\n";
}


} // namespace gong




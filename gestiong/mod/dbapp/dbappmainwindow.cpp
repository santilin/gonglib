#include <iostream>
#include <sstream>
#include <qt4/QtCore/qstring.h>

#include <QAction>
#include <QMenu>
#include <gonggettext.h>
#include "dbappnameslisttable.h"
#include "dbappfrmeditrec.h"
#include "dbappmainwindow.h"
#include "dbappfrmsettingsedit.h"
#include "dbappfrmmodulesettings.h"
#include "dbappdbapplication.h"
#include "dbappfrmdatabasetools.h"
#include "dbappfrmintegrity.h"
#include "dbappfrmimport.h"
#include "dbappfrmexport.h"
#ifdef HAVE_RTKMODULE
#include "dbappfrmreports.h"
#endif
#include "dbappfrmabout.h"
namespace gong {

QMdiSubWindow *MainWindow::createClient(QWidget *widget, bool force )
{
    QMdiSubWindow *ret = 0;
    DBAPP->waitCursor(true);
    try {
        removeUnusedSubWindows();
        if( ret = findClient( widget ) ) {
            DBAPP->resetCursor();
            if( !force ) {
                widget->setFocus();
                return ret;
            } else {
                widget->setObjectName( Xtring::stringInc( Xtring(objectName()) ).c_str() );
                return createClient( widget, true );
            }
        } else if( FrmBase *asfrm = dynamic_cast<FrmBase *>(widget) ) {
            ret = GuiMainWindow::createClient(widget);
            Xtring savedsize = DBAPP->getAppSetting("GUI.WORKSPACE." + Xtring(widget->name()) + ".GEOMETRY").toString();
            if( !savedsize.isEmpty() )
                asfrm->setGeometry( savedsize );
        } else {
            ret = GuiMainWindow::createClient(widget);
        }
    } catch( std::exception &e ) {
        _GONG_DEBUG_WARNING( e.what() );
    }
    DBAPP->resetCursor();
    return ret;
}

void MainWindow::initGUI()
{
    GuiMainWindow::initGUI();
    QString text = toGUI( _("Muestra el asistente de conexión a la base de datos para cambiar de asociación") );
    pMenuSystemLogin = new QAction( toGUI( _("&Asistente de acceso...") ), 0, this, 0);
    pMenuSystemLogin->setStatusTip(text);
    pMenuSystemLogin->setWhatsThis( text );
    connect(pMenuSystemLogin, SIGNAL(activated()), this, SLOT(slotMenuSystemLogin()));
    pMenuSystemLogin->addTo(pMenuSystem);

    text = toGUI( _("Todos los informes") );
    pMenuSystemReports = new QAction(toGUI( _("&Informes...") ), 0, this, 0);
    pMenuSystemReports->setStatusTip(text);
    pMenuSystemReports->setWhatsThis(text);
    connect(pMenuSystemReports, SIGNAL(activated()), this, SLOT(slotMenuSystemReports()));
    pMenuSystemReports->addTo(pMenuSystem);

    text = toGUI( _("Herramientas de la base de datos...") );
    pMenuSystemDatabaseTools = new QAction( text, 0, this, 0);
    pMenuSystemDatabaseTools->setStatusTip( text );
    pMenuSystemDatabaseTools->setWhatsThis( text );
    connect(pMenuSystemDatabaseTools, SIGNAL(activated()), this, SLOT(slotMenuSystemDatabaseTools()));
    pMenuSystemDatabaseTools->addTo(pMenuSystem);

    pMenuSystemConfig = new QMenu( toGUI( _("Configuración") ) );
    pMenuSystemConfig->setObjectName( "MenuSystemConfig" );
    pMenuSystem->addMenu( pMenuSystemConfig );

    text = toGUI( _("Abre el editor de la configuración") );
    pMenuSystemConfigEditor = new QAction( toGUI( _("Todo") ), 0, this, 0);
    pMenuSystemConfigEditor->setStatusTip( text );
    pMenuSystemConfigEditor->setWhatsThis( text );
    connect(pMenuSystemConfigEditor, SIGNAL(activated()), this, SLOT(slotMenuSystemConfigEditor()));
    pMenuSystemConfigEditor->addTo(pMenuSystemConfig);

    text = toGUI( _("Abre el editor de la configuración para este ordenador") );
    pMenuSystemModuleLocalUserSettingsEditor = new QAction( toGUI( _("local a este ordenador") ), 0, this, 0);
    pMenuSystemModuleLocalUserSettingsEditor->setStatusTip( text );
    pMenuSystemModuleLocalUserSettingsEditor->setWhatsThis( text );
    connect(pMenuSystemModuleLocalUserSettingsEditor, SIGNAL(activated()), this, SLOT(slotMenuSystemModuleLocalUserSettingsEditor()));
    pMenuSystemModuleLocalUserSettingsEditor->addTo(pMenuSystemConfig);

    text = toGUI( _("Abre el editor de la configuración global para todos los ordenadores") );
    pMenuSystemModuleGlobalSettingsEditor = new QAction( toGUI( _("común a todos los ordenadores") ), 0, this, 0);
    pMenuSystemModuleGlobalSettingsEditor->setStatusTip( text );
    pMenuSystemModuleGlobalSettingsEditor->setWhatsThis( text );
    connect(pMenuSystemModuleGlobalSettingsEditor, SIGNAL(activated()), this, SLOT(slotMenuSystemModuleGlobalSettingsEditor()));
    pMenuSystemModuleGlobalSettingsEditor->addTo(pMenuSystemConfig);


    text = toGUI( _("Asistente para importar datos") );
    pMenuSystemImport = new QAction( toGUI( _("Importar") ), 0, this, 0);
    pMenuSystemImport->setStatusTip( text );
    pMenuSystemImport->setWhatsThis( text );
    connect(pMenuSystemImport, SIGNAL(activated()), this, SLOT(slotMenuSystemImport()));
    pMenuSystemImport->addTo(pMenuSystem);

    text = toGUI( _("Asistente para exportar datos") );
    pMenuSystemExport = new QAction( toGUI( _("Exportar") ), 0, this, 0);
    pMenuSystemExport->setStatusTip( text );
    pMenuSystemExport->setWhatsThis( text );
    connect(pMenuSystemExport, SIGNAL(activated()), this, SLOT(slotMenuSystemExport()));
    pMenuSystemExport->addTo(pMenuSystem);

    text = toGUI( _("Otras tablas") );
    pMenuOtherTables = new QAction(toGUI( _("&Otras tablas...") ), 0, this, 0);
    pMenuOtherTables->setStatusTip(text);
    pMenuOtherTables->setWhatsThis(text);
    connect(pMenuOtherTables, SIGNAL(activated()), this, SLOT(slotMenuOtherTables()));
    pMenuOtherTables->addTo(pMenuSystem);

    text = toGUI( _("Comprobar integridad...") );
    pMenuSystemIntegrity = new QAction( text, 0, this, 0);
    pMenuSystemIntegrity->setStatusTip( text );
    pMenuSystemIntegrity->setWhatsThis( text );
    connect(pMenuSystemIntegrity, SIGNAL(activated()), this, SLOT(slotMenuSystemIntegrity()));
// TODO    pMenuSystemIntegrity->addTo(pMenuSystem);



    pMenuSystemLogin->setAccel( QKeySequence("CTRL+SHIFT+L") );
    pMenuSystemReports->setAccel( QKeySequence("CTRL+SHIFT+I") );


}

void MainWindow::finishGUI()
{
    GuiMainWindow::finishGUI();
    QString text = toGUI( _( "Recordar posición" ) ) ;
    pMenuWindowSaveGeometry = new QAction( text, this );
    pMenuWindowSaveGeometry->setStatusTip( text );
    pMenuWindowSaveGeometry->setWhatsThis( text );
    connect( pMenuWindowSaveGeometry, SIGNAL( triggered() ), this, SLOT( slotMenuWindowSaveGeometry() ) );
    pMenuWindowSaveGeometry->addTo(pMenuWindow);

    text = toGUI( _( "Olvidar posición" ) ) ;
    pMenuWindowRemoveGeometry = new QAction( text, this );
    pMenuWindowRemoveGeometry->setStatusTip( text );
    pMenuWindowRemoveGeometry->setWhatsThis( text );
    connect( pMenuWindowRemoveGeometry, SIGNAL( triggered() ), this, SLOT( slotMenuWindowRemoveGeometry() ) );
    pMenuWindowRemoveGeometry->addTo(pMenuWindow);
}

/// TODO: Move to Xtring
Xtring operator-(const Xtring &other, const Xtring &minus)
{
    Xtring ret;
    for( Xtring::const_iterator ch = other.begin(); ch != other.end(); ++ch ) {
        if( minus.find( *ch ) == Xtring::npos )
            ret.push_back( *ch );
    }
    return ret;
}

// Slot genérico para hacer browse de un fichero
void MainWindow::slotMenuEditRecMaestro(const Xtring &tablename, dbRecord *record, bool deleterecord )
{
    Xtring frmname = "browse_Rec" + tablename;
    if( record ) {
        // TODO: Add getLastFilter() a dbRecord
        // TODO: Add getFilter(void) return getFilter("");
        frmname += "_" + record->getFilter("") - "()= ";
    }
    FrmEditRec * frm = static_cast<FrmEditRec*>( findWindow( frmname ) );
    if (frm ) {
        _GONG_DEBUG_PRINT(6, Xtring::printf("Setting focus to %s", frm->name() ) );
        frm->setFocus();
        return;
    }
    DBAPP->waitCursor( true );
    try {
        if( !record ) {
            deleterecord = true;
            record = DBAPP->createRecord( tablename );
        }
        if( !record )
            throw std::runtime_error("Imposible crear un registro de la tabla " + tablename);
        frm = DBAPP->browse( 0, record, 0, DataTable::defaulteditmode,
                             deleterecord ? dbApplication::deleteRecord : dbApplication::editNone,
                             0, frmname.c_str() );
        createClient( frm );
    } catch( gong::dbError &e ) {
        DBAPP->resetCursor();
        _GONG_DEBUG_WARNING( e.what() );
        if( !DBAPP->manageDbError( tablename, e ) )
            throw;
    } catch( std::runtime_error &e ) {
        DBAPP->resetCursor();
        _GONG_DEBUG_WARNING( e.what() );
        if( FrmBase::msgYesNo( this, Xtring(e.what()) + _("\n\n¿Quieres salir del programa?") ) )
            throw;
    }
    DBAPP->resetCursor();
}

bool MainWindow::editRecMaestroModal(const Xtring& tablename, dbRecord* record,
                                     bool deleterecord, FrmBase* parentfrm)
{
    Xtring frmname = "browse_RecModal" + tablename;
    try {
        if( !record ) {
            deleterecord = true;
            record = DBAPP->createRecord( tablename );
        }
        if( !record )
            throw std::runtime_error("Imposible crear un registro de la tabla " + tablename);
        frmname += "_" + record->getFilter("") - "()= ";
        FrmBase *frm = DBAPP->browse( 0, record, 0, DataTable::defaulteditmode,
                                      deleterecord ? dbApplication::deleteRecord : dbApplication::editNone,
                                      0, frmname.c_str() );
        frm->showModalFor( parentfrm, true, true );
        return !frm->wasCancelled();
    } catch( gong::dbError &e ) {
        _GONG_DEBUG_WARNING( e.what() );
        if( !DBAPP->manageDbError( tablename, e ) )
            throw;
    } catch( std::exception &e ) {
        _GONG_DEBUG_WARNING( e.what() );
        if( FrmBase::msgYesNo( this, Xtring(e.what()) + _("\n\n¿Quieres salir del programa?") ) )
            throw;
    }
    return false;
}

void MainWindow::slotMenuSystemLogin()
{
    if ( getNumClients() > 0 )
        FrmBase::msgError( _( "Seleccionar asociación" ),
                           _( "Debes cerrar todas las ventanas antes de volver a ingresar en el programa" ) );
    else if( !DBAPP->login( DBAPP->getPackageVersion(), false ) ) // No es la primera vez que hacemos login
        DBAPP->exit(0);
}

void MainWindow::slotMenuSystemDatabaseTools()
{
    if( getNumClients() > 0 ) {
        FrmBase::msgError( _( "Herramientas de la base de datos" ),
                           _( "Debes cerrar todas las ventanas antes de utilizar las herramientas de la base de datos" ) );
    } else {
        FrmDatabaseTools *frmdbtools = new FrmDatabaseTools();
        frmdbtools->show();
        createClient( frmdbtools );
    }
}

void MainWindow::slotMenuSystemReports()
{
#ifdef HAVE_RTKMODULE
    FrmReports *frminformes = new FrmReports( 0, "Informes" );
    frminformes->show();
    createClient( frminformes );
#else
#endif
}

void MainWindow::slotMenuSystemModuleLocalUserSettingsEditor()
{
    FrmModuleSettings *modulesettingseditor = new FrmModuleSettings( FrmModuleSettings::LocalUser );
    modulesettingseditor->show();
    createClient( modulesettingseditor);
}

void MainWindow::slotMenuSystemModuleGlobalSettingsEditor()
{
    FrmModuleSettings *modulesettingseditor = new FrmModuleSettings( FrmModuleSettings::Global );
    modulesettingseditor->show();
    createClient( modulesettingseditor);
}

void MainWindow::slotMenuSystemModuleGlobalUserSettingsEditor()
{
    FrmModuleSettings *modulesettingseditor = new FrmModuleSettings( FrmModuleSettings::GlobalUser );
    modulesettingseditor->show();
    createClient( modulesettingseditor);
}

void MainWindow::slotMenuSystemConfigEditor()
{
    FrmSettingsEdit *settingseditor = new FrmSettingsEdit();
    settingseditor->show();
    createClient( settingseditor );
}

void MainWindow::slotMenuWindowSaveGeometry()
{
    if( FrmBase *asfrm = dynamic_cast<FrmBase *>(activeClient()) ) {
        Xtring name = asfrm->name();
        if( !name.isEmpty() ) {
            DBAPP->setUserLocalSetting( "GUI.WORKSPACE." + name + ".GEOMETRY",
                                        asfrm->getGeometry() );
        }
    }
}

void MainWindow::slotMenuWindowRemoveGeometry()
{
    if( activeClient() ) {
        Xtring name = activeClient()->name();
        if( !name.isEmpty() ) {
            DBAPP->setUserLocalSetting( "GUI.WORKSPACE." + name + ".GEOMETRY", "");
        }
    }
}

void MainWindow::slotMenuSystemIntegrity()
{
    FrmIntegrity *frminteg = new FrmIntegrity( DBAPP->getDatabase() );
    frminteg->show();
    createClient( frminteg );
}

void MainWindow::slotMenuSystemExport()
{
    FrmExport *frmexport = new FrmExport();
    frmexport->show();
    createClient( frmexport );
}

void MainWindow::slotMenuSystemImport()
{
    FrmImport *frmimport = new FrmImport();
    frmimport->show();
    createClient( frmimport );
}

void MainWindow::slotMenuOtherTables()
{
    XtringList namestables;
    // Look for all the namestables fields in the database
    for( dbTableDefinitionsList::iterator ittb = DBAPP->getDatabase()->getTables().begin();
            ittb != DBAPP->getDatabase()->getTables().end(); ++ittb ) {
        dbTableDefinition *tbldef = (*ittb).second;
        for( unsigned int nf=0; nf<tbldef->getFieldCount(); nf++ ) {
            FldNamesListTable *flddef = dynamic_cast<FldNamesListTable *>( tbldef->getFieldDefinition( nf ) );
            if( flddef )
                namestables << flddef->getName();
        }
    }
    int choosen = FrmBase::msgXtringList( 0, _("Elige el fichero"), namestables );
    if( choosen != -1 ) {
        Xtring choosen_table = namestables[choosen];
        if( editRecMaestroModal( choosen_table ) ) {
            // Fill the values of the fields
            for( dbTableDefinitionsList::iterator ittb = DBAPP->getDatabase()->getTables().begin();
                    ittb != DBAPP->getDatabase()->getTables().end(); ++ittb ) {
                dbTableDefinition *tbldef = (*ittb).second;
                for( unsigned int nf=0; nf<tbldef->getFieldCount(); nf++ ) {
                    FldNamesListTable *flddef = dynamic_cast<FldNamesListTable *>( tbldef->getFieldDefinition( nf ) );
                    if( flddef && flddef->getTableName() == choosen_table )
                        flddef->fill( *DBAPP->getConnection() );
                    break;
                }
            }
        }
    }
}

void MainWindow::slotMenuHelpAbout()
{
    FrmAbout *frmabout = new FrmAbout();
    createClient( frmabout, true );
}

};

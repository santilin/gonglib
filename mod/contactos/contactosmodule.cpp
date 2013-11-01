/*<<<<<MODULE_INFO*/
// COPYLEFT Plugin para los contactos de gestiong
// RECORD Contacto FrmEditRecMaster ~Contactos
// NAMESLISTTABLE TratamientoContacto
// TYPE GongModule contactos::ContactosModule
/*>>>>>MODULE_INFO*/

#include "config.h"
/*<<<<<CONTACTOSMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include <dbappnameslisttable.h>
#include <dbappfrmeditnameslisttable.h>
#include "contactosmodule.h"
#include "contactosfrmeditcontacto.h"
/*>>>>>CONTACTOSMODULE_INCLUDES*/

#include "contactosfldcif.h"
#include "contactosfrmmailing.h"

namespace gong {
namespace contactos {

ContactosModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::Int,
        "FORMATO_NOMBRE",
        _("Formato de los nombres"),
        "0",
        _("No formatear|Nombre Apellidos|Apellidos, Nombre|NOMBRE APELLIDOS|APELLIDOS, NOMBRE")
    },
    {
        dbModuleSetting::Bool,
        "USAR_TRATAMIENTOS",
        _("Usar tratamientos de los contactos"),
        "0"
    },
    {
        dbModuleSetting::Int,
        "FORMATO_EMPRESA",
        _("Formato de las empresas"),
        "0",
        _("No formatear|Programación Social, S.Coop|PROGRAMACIÓN SOCIAL, S.Coop")
    },
    {
        dbModuleSetting::Text,
        "FORMATO_DIRECCION",
        _("Formato de la dirección postal"),
        "{direccion}, {barrio}\n{cp} {localidad}, {provincia}. {pais}\nTel: {telefono}, Fax: {fax}\n{email}"
    },
    {dbModuleSetting::None}
};

ContactosModule::ContactosModule()
    : dbModule("contactos"), pFicContacto(0), pMenuContactosContacto(0)
{
    _GONG_DEBUG_TRACE(1);
    ModuleInstance = this;
    pModuleSettings = _settings;
    mDescription = "Datos personales de los contactos";
    mExportTables << "CONTACTO" << "TRATAMIENTOCONTACTO";
    mImportTables << "CONTACTO" << "TRATAMIENTOCONTACTO";
    /*<<<<<CONTACTOSMODULE_PUBLIC_INFO*/
//	mModuleRequires
	mMasterTables << "CONTACTO" << "TRATAMIENTOCONTACTO";
//	mDetailTables
/*>>>>>CONTACTOSMODULE_PUBLIC_INFO*/
    mMasterTables << "CONTACTOIMAGEN";
}

ContactosModule::~ContactosModule()
{
    _GONG_DEBUG_TRACE(1);
}

bool ContactosModule::login(FrmLogin *frmlogin, const Xtring &version,
                            Xtring &addtitle, bool startingapp)
{
    return true;
}

void ContactosModule::afterLoad()
{
    FldNamesListTable *fldtrat = static_cast<FldNamesListTable *>(
		DBAPP->getDatabase()->findFieldDefinition("CONTACTO.TRATAMIENTOCONTACTO") );
    if( fldtrat )
        fldtrat->fill( *getConnection() );
    if( fldtrat->getListOfValues().size() == 0 ) {
        getConnection()->exec( "INSERT INTO TRATAMIENTOCONTACTO (CODIGO,NOMBRE) VALUES "
                               "(" + getConnection()->toSQL( RecContacto::TratamientoDona ) + ", 'Doña'),("
                               + getConnection()->toSQL( RecContacto::TratamientoDon ) + ", 'Don'), ("
                               + getConnection()->toSQL( RecContacto::TratamientoEmpresa ) + ", 'Empresa'), ("
                               + getConnection()->toSQL( RecContacto::TratamientoAsociacion ) + ", 'Asociación'), ("
                               + getConnection()->toSQL( RecContacto::TratamientoCooperativa ) + ", 'Cooperativa'), ("
                               + getConnection()->toSQL( RecContacto::TratamientoAutonoma ) + ", 'Autónomo'), ("
                               + getConnection()->toSQL( RecContacto::TratamientoColectivo) + ", 'Colectivo')" );
        fldtrat->fill( *getConnection() );
    }
}

bool ContactosModule::initDatabase(dbDefinition *db)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    /*<<<<<CONTACTOSMODULE_INIT_DATABASE*/
	pFicTratamientoContacto = new NamesListTable( *pMainDatabase, "TRATAMIENTOCONTACTO" );
	pMainDatabase->addTable( pFicTratamientoContacto->getTableDefinition() );
/*>>>>>CONTACTOSMODULE_INIT_DATABASE*/

    pFicContacto = new MasterTable( *pMainDatabase, "CONTACTO");
    pFicContacto->addFieldRecordID();
    pFicContacto->addField<FldCif>("CIF")->setIsCode(false)->setCanBeNull(true);
    pFicContacto->addFieldInt("CODIGO")->setIsCode(true)->setCanBeNull(false)->setUnique(true);
    pFicContacto->addFieldDesc("NOMBRE", 120)->setCanBeNull( false )->setUnique(true);
    pFicContacto->addFieldDesc("NOMBREALT", 120)->setCanBeNull(true);
    pFicContacto->addField<FldNamesListTable>( "TRATAMIENTOCONTACTO" )->setInsertAllowed(false);
    pFicContacto->addFieldString( "NOMBRECOMPLETO", 120);
    pFicContacto->addFieldDireccion();
    pFicContacto->addFieldCodigoPostal("CP");
    pFicContacto->addFieldString("APDOCORREOS", 20);
    pFicContacto->addFieldString("BARRIO", 50);
    pFicContacto->addFieldLocalidad();
    pFicContacto->addFieldProvincia();
    pFicContacto->addFieldPais("PAIS");
    pFicContacto->addFieldPais("NACIONALIDAD");
    pFicContacto->addFieldTelefono("TELEFONO");
    pFicContacto->addFieldTelefono( "FAX");
    pFicContacto->addField<dbFieldEmail>("EMAIL");
    pFicContacto->addFieldString("WEB", 200);
    pFicContacto->addFieldDate("FECHANACIMIENTO");
    pFicContacto->addFieldString("CLAVE", 100);
    pFicContacto->addFieldAggregateRelation( "CONTACTOIMAGEN_ID", "CONTACTOIMAGEN.ID", true );
    pFicContacto->addFieldNotas();
    pFicContacto->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicContacto->getTableDefinition() );

    pFicContactoImagen = new contactos::MasterTable( *pMainDatabase, "CONTACTOIMAGEN" );
    pFicContactoImagen->addFieldRecordID();
    pFicContactoImagen->addField<dbFieldImage>( "IMAGEN" );
    pFicContactoImagen->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicContactoImagen->getTableDefinition() );


    return true;
}


/*<<<<<CONTACTOSMODULE_SLOT_CONTACTOSCONTACTO*/
void ContactosModule::slotMenuContactosContacto()
{
	pMainWindow->slotMenuEditRecMaestro( "CONTACTO" );
}
/*>>>>>CONTACTOSMODULE_SLOT_CONTACTOSCONTACTO*/

void ContactosModule::slotMenuContactosMailing()
{
    FrmMailing *frmmailing = new FrmMailing(0);
    frmmailing->show();
    pMainWindow->createClient( frmmailing );
}

dbRecord *ContactosModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    /*<<<<<CONTACTOSMODULE_CREATE_RECORD*/
	if( tablename.upper() == "CONTACTO" )
		return new RecContacto(getConnection(), recid, user);
	if( tablename.upper() == "TRATAMIENTOCONTACTO" )
		return new RecNamesListTable("TRATAMIENTOCONTACTO", getConnection(), recid, user);
/*>>>>>CONTACTOSMODULE_CREATE_RECORD*/
    if( tablename.upper() == "CONTACTOIMAGEN" )
        return new RecContactoImagen(getConnection(), recid, user);
    return 0;
}

FrmEditRec *ContactosModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        DataTable::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<CONTACTOSMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "CONTACTO" )
		return new FrmEditContacto(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TRATAMIENTOCONTACTO" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>CONTACTOSMODULE_CREATE_EDITFORM*/
    return 0;
}

FrmEditRecDetail *ContactosModule::createEditDetailForm(
    FrmEditRecMaster *frmmaestro, int ndetalle,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    DataTable::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<CONTACTOSMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>CONTACTOSMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

bool ContactosModule::initMainWindow(MainWindow *mainwin)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT(mainwin);
    pMainWindow = mainwin;
//	QMenu *pMenuSystem = mainwin->getMenuSystem();

    /*<<<<<CONTACTOSMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("CONTACTO")->getDescPlural();
		pMenuContactosContacto = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuContactosContacto->setObjectName( "MenuContactosContacto" );
		pMenuContactosContacto->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuContactosContacto->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuContactosContacto, SIGNAL(activated()), this, SLOT(slotMenuContactosContacto()));
	}
/*>>>>>CONTACTOSMODULE_INITMAINWINDOW_MENUS*/
    pMenuContactosMailing = new QAction( toGUI(_("Envío de emails")), 0, pMainWindow, 0 );
    pMenuContactosMailing->setObjectName( "MenuContactosMailing" );
    pMainWindow->connect(pMenuContactosMailing, SIGNAL(activated()), this, SLOT(slotMenuContactosMailing()) );
//	pMenuContactosMailing->addTo( pMenuSystem );
//	pMenuContactosContacto->addTo( pMenuSystem );
    pMenuContactosContacto->setAccel( QKeySequence("CTRL+SHIFT+C") );
    return true;
}


/*<<<<<CONTACTOSMODULE_FIN*/
} // namespace contactos
} // namespace gong

/*>>>>>CONTACTOSMODULE_FIN*/


/*<<<<<MODULE_INFO*/
// COPYLEFT Formulario de edición de contactos
// FIELD Codigo int pFrameContactos preLayout
// FIELD Nacionalidad string pFrameResto preLayout
// FIELD FechaNacimiento Date pFrameResto preLayout
// FIELD CIF string pFrameContactos nombreLayout
// FIELD TratamientoContacto comboint pFrameContactos nombreLayout
// FIELD Nombre string pFrameContactos nombreLayout
// FIELD NombreAlt string pFrameContactos altLayout
// FIELD email string pFrameContactos emailLayout
// FIELD Direccion string pFrameDir dirLayout
// FIELD ApdoCorreos string pFrameDir dirLayout
// FIELD CP string pFrameDir cpLayout
// FIELD Localidad string pFrameDir cpLayout
// FIELD Barrio string pFrameDir provLayout
// FIELD Provincia string pFrameDir provLayout
// FIELD Pais string pFrameContactos provLayout
// FIELD Telefono text pFrameResto telLayout
// FIELD Fax string pFrameResto faxLayout
// FIELD web string pFrameResto webLayout
// FIELD Clave string pFrameResto claveLayout
// FIELD Notas text pFrameResto notasLayout
// FIELD ContactoImagen.Imagen image tabImagen imagenLayout
// TYPE FrmEditRecMaster contactos::Contacto validCode
/*>>>>>MODULE_INFO*/

#include <QToolButton>
#include <gonggettext.h>
/*<<<<<FRMEDITCONTACTO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "contactosfrmeditcontacto.h"
/*>>>>>FRMEDITCONTACTO_INCLUDES*/
#include <gongimagebox.h>
#include "contactosmodule.h"
#include "contactosfldcif.h"

namespace gong {
namespace contactos {

/*<<<<<FRMEDITCONTACTO_CONSTRUCTOR*/
FrmEditContacto::FrmEditContacto(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
                                 EditMode editmode, dbApplication::EditFlags editflags,
                                 QWidget *parent, const char* name, WidgetFlags fl )
    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
    if ( !name )
        setName( "FrmEditContacto" );
    /*>>>>>FRMEDITCONTACTO_CONSTRUCTOR*/

    pFrameContactos = new QFrame( this );
    pFrameContactos->setObjectName( "pFrameContactos" );
    pControlsLayout->addWidget( pFrameContactos );
    pFrameContactos = new QFrame( this );
    pFrameContactos->setObjectName( "pFrameContactos" );
    pControlsLayout->addWidget( pFrameContactos );
    pFrameDir = new QFrame( this );
    pFrameDir->setObjectName( "pFrameDir" );
    pControlsLayout->addWidget( pFrameDir );
    pFrameResto = new QFrame( this );
    pFrameResto->setObjectName( "pFrameResto" );
    pControlsLayout->addWidget( pFrameResto );
    /*<<<<<FRMEDITCONTACTO_INIT_CONTROLS*/
    QVBoxLayout *pFrameContactosLayout = new QVBoxLayout(pFrameContactos, 11, 6, "pFrameContactosLayout");
    QHBoxLayout *preLayout = new QHBoxLayout(0, 0, 6, "preLayout");
    QVBoxLayout *pFrameRestoLayout = new QVBoxLayout(pFrameResto, 11, 6, "pFrameRestoLayout");
    QHBoxLayout *nombreLayout = new QHBoxLayout(0, 0, 6, "nombreLayout");
    QHBoxLayout *altLayout = new QHBoxLayout(0, 0, 6, "altLayout");
    QHBoxLayout *emailLayout = new QHBoxLayout(0, 0, 6, "emailLayout");
    QVBoxLayout *pFrameDirLayout = new QVBoxLayout(pFrameDir, 11, 6, "pFrameDirLayout");
    QHBoxLayout *dirLayout = new QHBoxLayout(0, 0, 6, "dirLayout");
    QHBoxLayout *cpLayout = new QHBoxLayout(0, 0, 6, "cpLayout");
    QHBoxLayout *provLayout = new QHBoxLayout(0, 0, 6, "provLayout");
    QHBoxLayout *telLayout = new QHBoxLayout(0, 0, 6, "telLayout");
    QHBoxLayout *faxLayout = new QHBoxLayout(0, 0, 6, "faxLayout");
    QHBoxLayout *webLayout = new QHBoxLayout(0, 0, 6, "webLayout");
    QHBoxLayout *claveLayout = new QHBoxLayout(0, 0, 6, "claveLayout");
    QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
    showTabs(true);
    QWidget *tabImagen = new QWidget( pTabWidget, "tabImagen" );
    QVBoxLayout *tabImagenLayout = new QVBoxLayout(tabImagen, 11, 6, "tabImagenLayout");
    QHBoxLayout *imagenLayout = new QHBoxLayout(0, 0, 6, "imagenLayout");
    editCodigo = addEditField( pFrameContactos, "CONTACTO", "CODIGO", preLayout );
    editNacionalidad = addEditField( pFrameResto, "CONTACTO", "NACIONALIDAD", preLayout );
    editFechaNacimiento = addEditField( pFrameResto, "CONTACTO", "FECHANACIMIENTO", preLayout );
    editCIF = addEditField( pFrameContactos, "CONTACTO", "CIF", nombreLayout );
    comboTratamientoContacto = addComboField<int>( pFrameContactos, "CONTACTO", "TRATAMIENTOCONTACTO", nombreLayout );
    editNombre = addEditField( pFrameContactos, "CONTACTO", "NOMBRE", nombreLayout );
    editNombreAlt = addEditField( pFrameContactos, "CONTACTO", "NOMBREALT", altLayout );
    editemail = addEditField( pFrameContactos, "CONTACTO", "EMAIL", emailLayout );
    editDireccion = addEditField( pFrameDir, "CONTACTO", "DIRECCION", dirLayout );
    editApdoCorreos = addEditField( pFrameDir, "CONTACTO", "APDOCORREOS", dirLayout );
    editCP = addEditField( pFrameDir, "CONTACTO", "CP", cpLayout );
    editLocalidad = addEditField( pFrameDir, "CONTACTO", "LOCALIDAD", cpLayout );
    editBarrio = addEditField( pFrameDir, "CONTACTO", "BARRIO", provLayout );
    editProvincia = addEditField( pFrameDir, "CONTACTO", "PROVINCIA", provLayout );
    editPais = addEditField( pFrameContactos, "CONTACTO", "PAIS", provLayout );
    editTelefono = addTextField( pFrameResto, "CONTACTO", "TELEFONO", telLayout );
    editFax = addEditField( pFrameResto, "CONTACTO", "FAX", faxLayout );
    editweb = addEditField( pFrameResto, "CONTACTO", "WEB", webLayout );
    editClave = addEditField( pFrameResto, "CONTACTO", "CLAVE", claveLayout );
    editNotas = addTextField( pFrameResto, "CONTACTO", "NOTAS", notasLayout );
    editContactoImagen_Imagen = addImageField( tabImagen, "CONTACTOIMAGEN", "IMAGEN", imagenLayout );
    pFrameContactosLayout->addLayout( preLayout );
    pFrameContactosLayout->addLayout( nombreLayout );
    pFrameContactosLayout->addLayout( altLayout );
    pFrameContactosLayout->addLayout( emailLayout );
    pFrameDirLayout->addLayout( dirLayout );
    pFrameDirLayout->addLayout( cpLayout );
    pFrameDirLayout->addLayout( provLayout );
    pFrameRestoLayout->addLayout( telLayout );
    pFrameRestoLayout->addLayout( faxLayout );
    pFrameRestoLayout->addLayout( webLayout );
    pFrameRestoLayout->addLayout( claveLayout );
    pFrameRestoLayout->addLayout( notasLayout );
    tabImagenLayout->addLayout( imagenLayout );
    /*>>>>>FRMEDITCONTACTO_INIT_CONTROLS*/
    connect( editContactoImagen_Imagen, SIGNAL( clicked() ), this, SLOT( editContactoImagen_clicked() ) );
    pTabWidget->insertTab( tabImagen, toGUI( _( "&Imagen" ) ) );
    pSearchCP = new QToolButton( pFrameDir );
    pSearchCP->setText( _("Buscar CP") );
    cpLayout->addWidget( pSearchCP );
    connect( pSearchCP, SIGNAL( clicked() ), this, SLOT( slot_searchCP_clicked() ) );
    mUsarTratamiento = ModuleInstance->getModuleSetting( "USAR_TRATAMIENTOS" ).toBool();
    comboTratamientoContacto->setVisible( mUsarTratamiento );
}

void FrmEditContacto::scatterFields()
{
    /*<<<<<FRMEDITCONTACTO_SCATTER*/
    editCodigo->setText(getRecContacto()->getValue("CODIGO").toInt());
    if( isEditing() && (pFocusWidget == 0) )
        pFocusWidget = editCodigo;
    editNacionalidad->setText(getRecContacto()->getValue("NACIONALIDAD").toString());
    editFechaNacimiento->setText(getRecContacto()->getValue("FECHANACIMIENTO").toDate());
    editCIF->setText(getRecContacto()->getValue("CIF").toString());
    comboTratamientoContacto->setCurrentItemByValue(getRecContacto()->getValue("TRATAMIENTOCONTACTO").toInt());
    editNombre->setText(getRecContacto()->getValue("NOMBRE").toString());
    editNombreAlt->setText(getRecContacto()->getValue("NOMBREALT").toString());
    editemail->setText(getRecContacto()->getValue("EMAIL").toString());
    editDireccion->setText(getRecContacto()->getValue("DIRECCION").toString());
    editApdoCorreos->setText(getRecContacto()->getValue("APDOCORREOS").toString());
    editCP->setText(getRecContacto()->getValue("CP").toString());
    editLocalidad->setText(getRecContacto()->getValue("LOCALIDAD").toString());
    editBarrio->setText(getRecContacto()->getValue("BARRIO").toString());
    editProvincia->setText(getRecContacto()->getValue("PROVINCIA").toString());
    editPais->setText(getRecContacto()->getValue("PAIS").toString());
    editTelefono->setText(getRecContacto()->getValue("TELEFONO").toString());
    editFax->setText(getRecContacto()->getValue("FAX").toString());
    editweb->setText(getRecContacto()->getValue("WEB").toString());
    editClave->setText(getRecContacto()->getValue("CLAVE").toString());
    editNotas->setText(getRecContacto()->getValue("NOTAS").toString());
    editContactoImagen_Imagen->setImageData(getRecContacto()->getValue("CONTACTOIMAGEN.IMAGEN"));
    /*>>>>>FRMEDITCONTACTO_SCATTER*/
    if( isInserting() ) {
        if( !DBAPP->codeNotFound().isEmpty() ) {
            if( DBAPP->codeNotFound().toInt() != 0 )
                editCIF->setText(DBAPP->codeNotFound() );
            else
                editNombre->setText(DBAPP->codeNotFound());
        }
        editCodigo->setText( getRecord()->selectNextInt("CODIGO", Xtring::null, true ) );
    }
    pFocusWidget = editCIF;
}

void FrmEditContacto::gatherFields()
{
    /*<<<<<FRMEDITCONTACTO_GATHER*/
    getRecContacto()->setValue( "CODIGO", editCodigo->toInt());
    getRecContacto()->setValue( "NACIONALIDAD", editNacionalidad->toString());
    getRecContacto()->setValue( "FECHANACIMIENTO", editFechaNacimiento->toDate());
    getRecContacto()->setValue( "CIF", editCIF->toString());
    getRecContacto()->setValue( "TRATAMIENTOCONTACTO", comboTratamientoContacto->getCurrentItemValue());
    getRecContacto()->setValue( "NOMBRE", editNombre->toString());
    getRecContacto()->setValue( "NOMBREALT", editNombreAlt->toString());
    getRecContacto()->setValue( "EMAIL", editemail->toString());
    getRecContacto()->setValue( "DIRECCION", editDireccion->toString());
    getRecContacto()->setValue( "APDOCORREOS", editApdoCorreos->toString());
    getRecContacto()->setValue( "CP", editCP->toString());
    getRecContacto()->setValue( "LOCALIDAD", editLocalidad->toString());
    getRecContacto()->setValue( "BARRIO", editBarrio->toString());
    getRecContacto()->setValue( "PROVINCIA", editProvincia->toString());
    getRecContacto()->setValue( "PAIS", editPais->toString());
    getRecContacto()->setValue( "TELEFONO", editTelefono->toString());
    getRecContacto()->setValue( "FAX", editFax->toString());
    getRecContacto()->setValue( "WEB", editweb->toString());
    getRecContacto()->setValue( "CLAVE", editClave->toString());
    getRecContacto()->setValue( "NOTAS", editNotas->toString());
    getRecContacto()->setValue( "CONTACTOIMAGEN.IMAGEN", editContactoImagen_Imagen->toData());
    /*>>>>>FRMEDITCONTACTO_GATHER*/
}

void FrmEditContacto::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITCONTACTO_VALIDATE*/
    bool v=true;
    if( !isvalid )
        isvalid = &v;
    ValidResult *validresult = ( ir ? ir : new ValidResult() );
    if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
        *isvalid = false;
    if( !validCodeAndDesc( sender, *validresult, editCodigo, 0, "codigo", "" ) )
        if( !sender )
            *isvalid = false;
    /*>>>>>FRMEDITCONTACTO_VALIDATE*/
    bool pais_espana = editPais->toString().isEmpty() || editPais->toString().upper() == _("ESPAÑA");
    bool nac_espana = editNacionalidad->toString().isEmpty() || editNacionalidad->toString().upper() == _("ESPAÑOLA");
    dbFieldDefinition *cifdef = DBAPP->getDatabase()->findFieldDefinition("CONTACTO.CIF");
    // Comprobamos y reparamos el cif antes de validar si está duplicado
    if( !sender ) {
        if( editCIF->toString().isEmpty() && nac_espana && cifdef && !cifdef->canBeNull() ) {
            if( msgYesNo( this,
                          Xtring::printf( _("'%s' no puede estar vacío. ¿Quieres crear un valor provisional?"),
                                          cifdef->getCaption().c_str() ) ) ) {
                editCIF->setText( cifdef->customDefaultValue() );
            }
            *isvalid = false;
            return;
        }
    }
    Xtring cif_error;
    if( (!sender || (sender == editCIF ) ) && nac_espana ) {
        Cif cif = editCIF->toString().trim();
        if( !cif.isEmpty() || !cifdef->canBeNull() ) {
            cif_error += cif.validate( RecContacto::Tratamiento(comboTratamientoContacto->getCurrentItemValue()),
                                       /*fix if */sender == editCIF );
            // Dejar a la función validate que dé el error de que está vacío
        }
        if( cif_error.isEmpty() && sender == editCIF ) {
            editCIF->setText( cif );
            editCIF->setJustEdited( true );
        }
    }
    if( mUsarTratamiento && (!sender || (sender == editNombre && editNombre->isJustEdited())) ) {
        if( comboTratamientoContacto->getCurrentItemValue() == contactos::RecContacto::TratamientoDona
                || comboTratamientoContacto->getCurrentItemValue() == contactos::RecContacto::TratamientoDon ) {
            Xtring nombre = editNombre->toString();
            int fn = contactos::ModuleInstance->getModuleSetting( "FORMATO_NOMBRE" ).toInt();
            switch( fn ) {
            case 1: // Nombre Apellidos
            case 3: // NOMBRE APELLIDOS
                if( nombre.find(",") != Xtring::npos ) {
                    validresult->addWarning( Xtring::printf(
                                                 _("Según la configuración, no puede haber comas entre el nombre y los apellidos")
                                             ),"NOMBRE");
                }
                if( fn == 3 )
                    nombre = nombre.upper();
                break;
            case 2: // Apellidos, Nombre
            case 4: // APELLIDOS, NOMBRE
                if( nombre.find(",") == Xtring::npos ) {
                    validresult->addWarning( Xtring::printf(
                                                 _("Según la configuración, tienes que escribir 'apellidos, nombre'")
                                             ),"NOMBRE");
                }
                if( fn == 4 )
                    nombre = nombre.upper();
                break;
            }
            editNombre->setText( nombre );
            editNombre->setJustEdited( true );
        }
    }
    // Normalizar nombre
    if( sender == editNombre && editNombre->isJustEdited() && nac_espana ) {
        // Adivinar el tipo de contacto
        if( comboTratamientoContacto->getCurrentItemValue() == 0 )
            comboTratamientoContacto->setCurrentItemByValue( RecContacto::tratamientoFromNombre( editNombre->toString() ) );
        if( mUsarTratamiento ) {
            if( comboTratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoDona
                    || comboTratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoDon )
                editNombre->setText( RecContacto::normalizaNombre( editNombre->toString() ) );
            else if( comboTratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoCooperativa
                     || comboTratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoEmpresa
                     || comboTratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoAsociacion )
                editNombre->setText( RecContacto::normalizaEmpresa( editNombre->toString() ) );
        } else {
            editNombre->setText( RecContacto::normalizaNombre( editNombre->toString() ) );
            editNombre->setText( RecContacto::normalizaEmpresa( editNombre->toString() ) );
        }
        editNombre->setJustEdited( true );
    }
    // El CIF y el nombre están ahora normalizados, ahora se comprueban los duplicados
    validCodeAndDesc( sender, *validresult, editCIF, editNombre,
                      "cif", "nombre", Xtring::null, true /*codecanbenull*/ );
    if( (!sender || sender == editCIF ) && nac_espana && cif_error.isEmpty() ) {
        if( comboTratamientoContacto->getCurrentItemValue() == 0 )
            comboTratamientoContacto->setCurrentItemByValue( RecContacto::tratamientoFromCIF( editCIF->toString() ) );
    }
    if( !cif_error.isEmpty() ) {
        validresult->addWarning( Xtring::printf( _("%s erróneo: %s"),
                                 cifdef->getName().c_str(), cif_error.c_str()), "CIF");
    }
    if( pais_espana && sender == editCP && editCP->isJustEdited() ) {
        slot_searchCP_clicked();
        if( editProvincia->toString().isEmpty() )
            editProvincia->setText( RecContacto::normalizaNombre(
                                        RecContacto::getProvinciaFromCp( editProvincia->toString() ) ) );
    }
    if( !sender || sender == editemail ) {
        if( getRecContacto()->existsAnother(
                    "EMAIL=" + getRecContacto()->getConnection()->toSQL( editemail->toString() ) ) ) {
            validresult->addWarning( Xtring::printf( _("Ya existe %s con el mismo email: %s"),
                                     DBAPP->getTableDescSingular( "CONTACTO", "una").c_str(),
                                     editemail->toString().c_str() ), "EMAIL" );
        }
    }
    if (!ir) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}
void FrmEditContacto::slot_searchCP_clicked()
{
    if( RecContacto::slotSearchCPClicked( this, editCP, editBarrio, editLocalidad, editProvincia) )
        editTelefono->setFocus();
    else
        editCP->setFocus();
}

void FrmEditContacto::editContactoImagen_clicked()
{
    Xtring fname = GuiApplication::getOpenFileName(
                       Xtring::printf( _( "Elige una imagen para %s"),
                                       DBAPP->getTableDescSingular("CONTACTO", "esta" ).c_str() ),
                       Xtring::null, _( "Imágenes (*.png *.gif *.jpg)" ),
                       this );
    if ( !fname.isEmpty() ) {
        if ( editContactoImagen_Imagen->loadFromFile( fname ) )
            editContactoImagen_Imagen->update();
    }
}

/*<<<<<FRMEDITCONTACTO_FIN*/
} // namespace contactos
} // namespace gong
/*>>>>>FRMEDITCONTACTO_FIN*/


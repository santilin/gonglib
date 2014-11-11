/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior para añadir edición de contactos a otros formularios
// FIELD Contacto_ID Reference(contactos::Contacto,CIF,Nombre,dbApplication::AllowNotFound) tabContacto contactoLayout
// FIELD Contacto.TratamientoContacto comboint tabContacto dirLayout
// FIELD Contacto.Direccion string tabContacto dirLayout
// FIELD Contacto.CP string tabContacto dirLayout
// FIELD Contacto.Barrio string tabContacto locLayout
// FIELD Contacto.Localidad string tabContacto locLayout
// FIELD Contacto.Provincia string tabContacto emailLayout
// FIELD Contacto.email string tabContacto emailLayout
// FIELD Contacto.Telefono text tabContacto horiztelLayout
// FIELD Contacto.Codigo int tabContactoMore codLayout
// FIELD Contacto.Nacionalidad string tabContactoMore codLayout
// FIELD Contacto.FechaNacimiento Date tabContactoMore codLayout
// FIELD Contacto.NombreAlt string tabContactoMore altLayout
// FIELD Contacto.Pais string tabContactoMore paisLayout
// FIELD Contacto.ApdoCorreos string tabContactoMore paisLayout
// FIELD Contacto.Fax string tabContactoMore faxLayout
// FIELD Contacto.web string tabContactoMore webLayout
// FIELD Contacto.clave string tabContactoMore claveLayout
// FIELD noname vspacer tabContactoMore
// TYPE FrmEditRecBehavior contactos::Contacto Q_OBJECT NoCreateRecord
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITCONTACTOBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "contactosfrmeditcontactobehavior.h"
/*>>>>>FRMEDITCONTACTOBEHAVIOR_INCLUDES*/
#include "contactosmodule.h"

namespace gong {
namespace contactos {

RecContacto* FrmEditContactoBehavior::getRecContacto() const
{
    return static_cast<RecContacto *>(pTheForm->getRecord()->findRelatedRecord("CONTACTO_ID"));
}

void FrmEditContactoBehavior::initGUI()
{
}

/**
 * @brief Esta función se llama desde el constructor del formulario maestro.
 * Si los controles se añaden desde initGUI no funciona bien porque el initGUI se llama después de
 * haberse creado el formulario maestro y los layouts no se redibujan bien
 *
 * @return void
 **/

void FrmEditContactoBehavior::_initGUI()
{
    // {capel} añadir a addSearchField de ContactoCIF
    // static_cast<SearchBox::Flags>(SearchBox::FlagShowLabels | SearchBox::FlagEditableDesc ));
    QHBoxLayout *pushLayout = new QHBoxLayout();

    /*<<<<<FRMEDITCONTACTOBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QWidget *tabContacto = getOrCreateTab( "tabContacto" );
	QVBoxLayout *tabContactoLayout = static_cast<QVBoxLayout *>( tabContacto->layout() );
	QHBoxLayout *contactoLayout = new QHBoxLayout(0, 0, 6, "contactoLayout");
	QHBoxLayout *dirLayout = new QHBoxLayout(0, 0, 6, "dirLayout");
	QHBoxLayout *locLayout = new QHBoxLayout(0, 0, 6, "locLayout");
	QHBoxLayout *emailLayout = new QHBoxLayout(0, 0, 6, "emailLayout");
	QHBoxLayout *horiztelLayout = new QHBoxLayout(0, 0, 6, "horiztelLayout");
	QWidget *tabContactoMore = getOrCreateTab( "tabContactoMore" );
	QVBoxLayout *tabContactoMoreLayout = static_cast<QVBoxLayout *>( tabContactoMore->layout() );
	QHBoxLayout *codLayout = new QHBoxLayout(0, 0, 6, "codLayout");
	QHBoxLayout *altLayout = new QHBoxLayout(0, 0, 6, "altLayout");
	QHBoxLayout *paisLayout = new QHBoxLayout(0, 0, 6, "paisLayout");
	QHBoxLayout *faxLayout = new QHBoxLayout(0, 0, 6, "faxLayout");
	QHBoxLayout *webLayout = new QHBoxLayout(0, 0, 6, "webLayout");
	QHBoxLayout *claveLayout = new QHBoxLayout(0, 0, 6, "claveLayout");
	QHBoxLayout *nonameLayout = new QHBoxLayout(0, 0, 6, "nonameLayout");

	searchContactoCIF = pTheForm->addSearchField( tabContacto, "CONTACTO_ID", "CONTACTO", "CIF", "NOMBRE", contactoLayout,
		static_cast<SearchBox::Flags>(SearchBox::FlagShowLabels | SearchBox::FlagEditableDesc ));
	pushContactoCIF = searchContactoCIF->getButton();
	connect( pushContactoCIF, SIGNAL( clicked() ), this, SLOT( pushContactoCIF_clicked() ) );
	editContactoCIF = searchContactoCIF->getEditCode();
	editContactoNombre = searchContactoCIF->getEditDesc();
	comboContacto_TratamientoContacto = pTheForm->addComboIntField( tabContacto, "CONTACTO", "TRATAMIENTOCONTACTO", dirLayout );
	editContacto_Direccion = pTheForm->addEditField( tabContacto, "CONTACTO", "DIRECCION", dirLayout );
	editContacto_CP = pTheForm->addEditField( tabContacto, "CONTACTO", "CP", dirLayout );
	editContacto_Barrio = pTheForm->addEditField( tabContacto, "CONTACTO", "BARRIO", locLayout );
	editContacto_Localidad = pTheForm->addEditField( tabContacto, "CONTACTO", "LOCALIDAD", locLayout );
	editContacto_Provincia = pTheForm->addEditField( tabContacto, "CONTACTO", "PROVINCIA", emailLayout );
	editContacto_email = pTheForm->addEditField( tabContacto, "CONTACTO", "EMAIL", emailLayout );
	editContacto_Telefono = pTheForm->addTextField( tabContacto, "CONTACTO", "TELEFONO", horiztelLayout, true  );
	editContacto_Codigo = pTheForm->addEditField( tabContactoMore, "CONTACTO", "CODIGO", codLayout );
	editContacto_Nacionalidad = pTheForm->addEditField( tabContactoMore, "CONTACTO", "NACIONALIDAD", codLayout );
	editContacto_FechaNacimiento = pTheForm->addEditField( tabContactoMore, "CONTACTO", "FECHANACIMIENTO", codLayout );
	editContacto_NombreAlt = pTheForm->addEditField( tabContactoMore, "CONTACTO", "NOMBREALT", altLayout );
	editContacto_Pais = pTheForm->addEditField( tabContactoMore, "CONTACTO", "PAIS", paisLayout );
	editContacto_ApdoCorreos = pTheForm->addEditField( tabContactoMore, "CONTACTO", "APDOCORREOS", paisLayout );
	editContacto_Fax = pTheForm->addEditField( tabContactoMore, "CONTACTO", "FAX", faxLayout );
	editContacto_web = pTheForm->addEditField( tabContactoMore, "CONTACTO", "WEB", webLayout );
	editContacto_clave = pTheForm->addEditField( tabContactoMore, "CONTACTO", "CLAVE", claveLayout );
	QSpacerItem* noname = new QSpacerItem ( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	nonameLayout->addItem ( noname);
	tabContactoLayout->addLayout( contactoLayout );
	tabContactoLayout->addLayout( dirLayout );
	tabContactoLayout->addLayout( locLayout );
	tabContactoLayout->addLayout( emailLayout );
	tabContactoLayout->addLayout( horiztelLayout );
	tabContactoMoreLayout->addLayout( codLayout );
	tabContactoMoreLayout->addLayout( altLayout );
	tabContactoMoreLayout->addLayout( paisLayout );
	tabContactoMoreLayout->addLayout( faxLayout );
	tabContactoMoreLayout->addLayout( webLayout );
	tabContactoMoreLayout->addLayout( claveLayout );
	tabContactoMoreLayout->addLayout( nonameLayout );
/*>>>>>FRMEDITCONTACTOBEHAVIOR_INITGUI*/
    (void)pControlsFrame;
    (void)pControlsLayout;
    editContacto_Telefono->setHeightInLines(2);
    editContacto_Provincia->setWidthInChars(20);
    pFrameContactos->insertTab( tabContacto, toGUI( _( "&Contacto" ) ) );
    pFrameContactos->insertTab( tabContactoMore, toGUI( _( "&Más" ) ) );

    dbTableDefinition *contactosdef = DBAPP->getDatabase()->findTableDefinition("CONTACTO");
    pushSearchCP = new QPushButton( pFrameContactos );
    pushSearchCP->setIcon( QIcon::fromTheme("edit-find", QIcon(":/edit-find.png")) );
    pushSearchCP->setText( toGUI(contactosdef->findFieldDefinition("CP")->getCaption().c_str() ) );
    pushLayout->addWidget( pushSearchCP );
    pushSearchCP->setFocusPolicy( Qt::ClickFocus );
    connect( pushSearchCP, SIGNAL( clicked() ), this, SLOT( slotSearchCP_clicked() ) );
    pushLimpiaDatos = new QPushButton( pFrameContactos );
    pushLimpiaDatos->setText( toGUI(_("Limpiar")) );
    pushLimpiaDatos->setIcon( QIcon::fromTheme("edit-clear", QIcon(":/edit-clear.png")) );
    connect( pushLimpiaDatos, SIGNAL( clicked() ), this, SLOT( slotLimpiaDatos_clicked() ) );
    pushLimpiaDatos->setFocusPolicy( Qt::ClickFocus );
    pushARazonSocial = new QPushButton( pFrameContactos );
    pushARazonSocial->setText( toGUI(_("A razón social")) );
    pushARazonSocial->setFocusPolicy( Qt::ClickFocus );
    pushARazonSocial->setIcon( QIcon::fromTheme("go-up", QIcon(":/go-up.png")) );
    connect( pushARazonSocial, SIGNAL( clicked() ), this, SLOT( slotARazonSocial_clicked() ) );
    pushEditFull = new QPushButton( pFrameContactos );
    pushEditFull->setText( toGUI(_("Editar")) );
    pushEditFull->setFocusPolicy( Qt::ClickFocus );
    connect( pushEditFull, SIGNAL( clicked() ), this, SLOT( slotEditFull_clicked() ) );
    pushLayout->insertWidget( 0, pushLimpiaDatos );
    pushLayout->insertWidget( 0, pushARazonSocial );
    pushLayout->insertWidget( 0, searchContactoCIF );
    pushLayout->insertWidget( 0, pushEditFull);
    pushEditFull->setVisible( false ); /// TODO
    tabContactoLayout->insertLayout(0, pushLayout );
    mUsarTratamiento = ModuleInstance->getModuleSetting( "USAR_TRATAMIENTOS" ).toBool();
    comboContacto_TratamientoContacto->setVisible( mUsarTratamiento );
}

void FrmEditContactoBehavior::setTabOrders(QWidget* pre, QWidget* post)
{
// 	pTheForm->setTabOrder( pre, searchContactoCIF );
// 	pTheForm->setTabOrder( searchContactoCIF, editContactoCIF );
    pTheForm->setTabOrder( pre, editContactoCIF );
    pTheForm->setTabOrder( editContactoCIF, editContactoNombre );
    pTheForm->setTabOrder( editContactoNombre, comboContacto_TratamientoContacto );
    pTheForm->setTabOrder( comboContacto_TratamientoContacto, editContacto_Direccion );
    pTheForm->setTabOrder( editContacto_Direccion, editContacto_CP );
    pTheForm->setTabOrder( editContacto_CP, editContacto_Barrio );
    pTheForm->setTabOrder( editContacto_Barrio, editContacto_Localidad );
    pTheForm->setTabOrder( editContacto_Localidad, editContacto_Provincia );
    pTheForm->setTabOrder( editContacto_Provincia, editContacto_email );
    pTheForm->setTabOrder( editContacto_email, editContacto_Telefono );
    pTheForm->setTabOrder( editContacto_Telefono, post );
}

void FrmEditContactoBehavior::scatterContacto()
{
    /*<<<<<FRMEDITCONTACTOBEHAVIOR_SCATTER_CONTACTO*/
	editContactoCIF->setText( getRecContacto()->getValue("CIF") );
	editContactoNombre->setText( getRecContacto()->getValue("NOMBRE") );
/*>>>>>FRMEDITCONTACTOBEHAVIOR_SCATTER_CONTACTO*/
    scatterFields( false ); // Mucho ojo, solo el resto de campos, evitar recursividad
}

void FrmEditContactoBehavior::pushContactoCIF_clicked()
{
    /*<<<<<FRMEDITCONTACTOBEHAVIOR_PUSH_CONTACTO_CIF_CLICKED*/
	char action = getControlKeyPressed();
	if( !pTheForm->isEditing() || searchContactoCIF->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editContactoCIF->setJustEdited( false );
			editContactoCIF->setCancelling();
			if( DBAPP->choose(pTheForm, getRecContacto(), 0, dbApplication::editNone, pTheForm ) ) {
				pTheForm->setEdited(true);
				scatterContacto();
				editContactoCIF->setJustEdited( true );
				pTheForm->setWiseFocus(editContactoCIF);
			}
			break;
		case 'M':
			{
				if( getRecContacto()->getRecordID() ) {
					editContactoCIF->setJustEdited( false );
					if( DBAPP->editRecord(pTheForm,
							getRecContacto(), 0, DataTable::updating,
								dbApplication::simpleEdition, pTheForm ) ) {
						editContactoCIF->setJustEdited( true );
						scatterContacto();
					}
					pTheForm->setWiseFocus(editContactoCIF);
				}
			}
			break;
		case 'E':
			{
				editContactoCIF->setJustEdited( false );
				DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(static_cast<FrmEditRecMaster *>(pTheForm),
					getRecContacto(), 0, DataTable::selecting,
				dbApplication::simpleEdition, pTheForm ) );
			}
			break;
		case 'A':
			{
				contactos::RecContacto *tmprec = static_cast<contactos::RecContacto *>(DBAPP->createRecord( "Contacto" ));
				editContactoCIF->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editContactoCIF->toString() );
				if( DBAPP->editRecord(pTheForm, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, pTheForm ) ) {
					editContactoCIF->setJustEdited( true );
					getRecContacto()->copyRecord( tmprec );
					scatterContacto();
				}
				pTheForm->setWiseFocus(editContactoCIF);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITCONTACTOBEHAVIOR_PUSH_CONTACTO_CIF_CLICKED*/
}

void FrmEditContactoBehavior::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITCONTACTOBEHAVIOR_SPECIALACTION*/
	if( sender == editContactoCIF )
		pushContactoCIF_clicked();
/*>>>>>FRMEDITCONTACTOBEHAVIOR_SPECIALACTION*/
}

void FrmEditContactoBehavior::scatterFields( bool is_pre )
{
    if( !is_pre ) {
        mSearching = (pTheForm->getRecord()->getValue( "CONTACTO_ID" ).toUInt() == 0);
    }
    if( is_pre ) // Solo hacer scatterContacto si is_pre, porque cuando no, es para el resto de campos
        /*<<<<<FRMEDITCONTACTOBEHAVIOR_SCATTER*/
	scatterContacto();
	comboContacto_TratamientoContacto->setCurrentItemByValue( pTheForm->getRecord()->getValue("CONTACTO.TRATAMIENTOCONTACTO").toInt());
	editContacto_Direccion->setText( pTheForm->getRecord()->getValue("CONTACTO.DIRECCION").toString());
	editContacto_CP->setText( pTheForm->getRecord()->getValue("CONTACTO.CP").toString());
	editContacto_Barrio->setText( pTheForm->getRecord()->getValue("CONTACTO.BARRIO").toString());
	editContacto_Localidad->setText( pTheForm->getRecord()->getValue("CONTACTO.LOCALIDAD").toString());
	editContacto_Provincia->setText( pTheForm->getRecord()->getValue("CONTACTO.PROVINCIA").toString());
	editContacto_email->setText( pTheForm->getRecord()->getValue("CONTACTO.EMAIL").toString());
	editContacto_Telefono->setText( pTheForm->getRecord()->getValue("CONTACTO.TELEFONO").toString());
	editContacto_Codigo->setText( pTheForm->getRecord()->getValue("CONTACTO.CODIGO").toInt());
	editContacto_Nacionalidad->setText( pTheForm->getRecord()->getValue("CONTACTO.NACIONALIDAD").toString());
	editContacto_FechaNacimiento->setText( pTheForm->getRecord()->getValue("CONTACTO.FECHANACIMIENTO").toDate());
	editContacto_NombreAlt->setText( pTheForm->getRecord()->getValue("CONTACTO.NOMBREALT").toString());
	editContacto_Pais->setText( pTheForm->getRecord()->getValue("CONTACTO.PAIS").toString());
	editContacto_ApdoCorreos->setText( pTheForm->getRecord()->getValue("CONTACTO.APDOCORREOS").toString());
	editContacto_Fax->setText( pTheForm->getRecord()->getValue("CONTACTO.FAX").toString());
	editContacto_web->setText( pTheForm->getRecord()->getValue("CONTACTO.WEB").toString());
	editContacto_clave->setText( pTheForm->getRecord()->getValue("CONTACTO.CLAVE").toString());
/*>>>>>FRMEDITCONTACTOBEHAVIOR_SCATTER*/
    if(is_pre && pTheForm->isInserting() && editContacto_Codigo->toInt() == 0 )
        editContacto_Codigo->setText( getRecContacto()->selectNextInt("CODIGO", Xtring::null, true ) );
}

void FrmEditContactoBehavior::gatherFields()
{
    /*<<<<<FRMEDITCONTACTOBEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "CONTACTO_ID", getRecContacto()->getRecordID() );
	pTheForm->getRecord()->setValue( "CONTACTO.TRATAMIENTOCONTACTO", comboContacto_TratamientoContacto->getCurrentItemValue());
	pTheForm->getRecord()->setValue( "CONTACTO.DIRECCION", editContacto_Direccion->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.CP", editContacto_CP->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.BARRIO", editContacto_Barrio->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.LOCALIDAD", editContacto_Localidad->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.PROVINCIA", editContacto_Provincia->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.EMAIL", editContacto_email->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.TELEFONO", editContacto_Telefono->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.CODIGO", editContacto_Codigo->toInt());
	pTheForm->getRecord()->setValue( "CONTACTO.NACIONALIDAD", editContacto_Nacionalidad->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.FECHANACIMIENTO", editContacto_FechaNacimiento->toDate());
	pTheForm->getRecord()->setValue( "CONTACTO.NOMBREALT", editContacto_NombreAlt->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.PAIS", editContacto_Pais->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.APDOCORREOS", editContacto_ApdoCorreos->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.FAX", editContacto_Fax->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.WEB", editContacto_web->toString());
	pTheForm->getRecord()->setValue( "CONTACTO.CLAVE", editContacto_clave->toString());
/*>>>>>FRMEDITCONTACTOBEHAVIOR_GATHER*/
    pTheForm->getRecord()->setValue( "CONTACTO.CIF", editContactoCIF->toString() );
    pTheForm->getRecord()->setValue( "CONTACTO.NOMBRE", editContactoNombre->toString() );
    if( getRecContacto()->getValue( "CODIGO" ).toInt() == 0 )
        getRecContacto()->setValue( "CODIGO", getRecContacto()->selectNextInt("CODIGO", Xtring::null, true ) );
}


bool FrmEditContactoBehavior::setCIFAndLookForIt(const Xtring& cif)
{
    editContactoCIF->setText( cif );
    editContactoCIF->setJustEdited( true );
    ValidResult v;
    if( pTheForm->validSeekCode( editContactoCIF, 0, v,
                                 editContactoCIF, editContactoNombre,
                                 getRecContacto(), "CIF", "NOMBRE", Xtring::null,
                                 dbApplication::SeekCodeFlags( dbApplication::AllowNotFound + dbApplication::DontShowBrowse )) ) {
        scatterContacto();
        mSearching = 0;
        return true;
    } else {
        mSearching = 1;
        getRecContacto()->setNew( true );
        getRecContacto()->clear( true );
        getRecContacto()->setValue( "NOMBRE", cif );
        getRecContacto()->setValue( "CIF", Xtring::null ); // Delete the provisional CIF
        getRecContacto()->setValue( "CODIGO", getRecContacto()->selectNextInt("CODIGO", Xtring::null, true ) );
        scatterContacto();
        return false;
    }
}

void FrmEditContactoBehavior::validateFields( bool is_pre, QWidget *sender, bool *isvalid, ValidResult *ir)
{
    if( is_pre ) {
        bool v=true;
        if( !isvalid )
            isvalid = &v;
        ValidResult *validresult = ( ir ? ir : new ValidResult() );
        if( !sender && !getRecContacto()->isValid(ValidResult::editing, validresult) )
            *isvalid = false;
        // Comprobamos y reparamos el cif antes de validar si está duplicado
        dbFieldDefinition *cifdef = DBAPP->getDatabase()->findFieldDefinition("CONTACTO.CIF");
        dbFieldDefinition *nombredef = DBAPP->getDatabase()->findFieldDefinition("CONTACTO.NOMBRE");
        bool cifcanbenull = (cifdef && cifdef->canBeNull());
        bool pais_espana = (editContacto_Pais->toString().upper() == _("ESPAÑA")
                            || editContacto_Pais->toString().isEmpty() );
        bool nac_espana = ( editContacto_Nacionalidad->toString().isEmpty()
                            || editContacto_Nacionalidad->toString().upper() == _("ESPAÑOLA")
                            || editContacto_Nacionalidad->toString().upper() == _("ESPAÑOL") );
        if( !sender ) {
            if( editContactoCIF->toString().isEmpty() && nac_espana && !cifcanbenull ) {
                if( FrmBase::msgYesNo( pTheForm,
                                       Xtring::printf( _("'%s' no puede estar vacío. ¿Quieres crear un valor provisional?"),
                                                       cifdef->getCaption().c_str() ) ) ) {
                    editContactoCIF->setText( cifdef->customDefaultValue() );
                    *isvalid = true;
                } else {
                    *isvalid = false;
                    validresult->addError( Xtring::printf( _("'%s' no puede estar vacío."),
                                                           cifdef->getCaption().c_str() ), "CIF" );
                }
                return;
            }
        }
        Xtring cif_error;
        if( (!sender || sender == editContactoCIF ) && nac_espana ) {
            contactos::Cif cif = editContactoCIF->toString().trim();
            if( !cif.isEmpty() || !cifcanbenull ) {
                cif_error += cif.validate( RecContacto::Tratamiento(comboContacto_TratamientoContacto->getCurrentItemValue()),
                                           /*fix if */sender == editContactoCIF );
                // Dejar a la función validate que dé el error de que está vacío
            }
            if( cif_error.isEmpty() && sender == editContactoCIF ) {
                editContactoCIF->setText( cif );
                editContactoCIF->setJustEdited( true );
            }
        }
        if( mUsarTratamiento && (!sender || (sender == editContactoNombre && editContactoNombre->isJustEdited())) ) {
            if( comboContacto_TratamientoContacto->getCurrentItemValue() == contactos::RecContacto::TratamientoDona
                    || comboContacto_TratamientoContacto->getCurrentItemValue() == contactos::RecContacto::TratamientoDon ) {
                Xtring nombre = editContactoNombre->toString();
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
                editContactoNombre->setText( nombre );
                editContactoNombre->setJustEdited( true );
            }
        }
        // Normalizar nombre
        if( sender == editContactoNombre && nac_espana && editContactoNombre->isJustEdited() ) {
            // Adivinar el tipo de contacto
            if( comboContacto_TratamientoContacto->getCurrentItemValue() == 0 )
                comboContacto_TratamientoContacto->setCurrentItemByValue( RecContacto::tratamientoFromNombre( editContactoNombre->toString() ) );
            if( mUsarTratamiento ) {
                if( comboContacto_TratamientoContacto->getCurrentItemValue() == contactos::RecContacto::TratamientoDona
                        || comboContacto_TratamientoContacto->getCurrentItemValue() == contactos::RecContacto::TratamientoDon )
                    editContactoNombre->setText( contactos::RecContacto::normalizaNombre( editContactoNombre->toString() ) );
                else if( comboContacto_TratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoCooperativa
                         || comboContacto_TratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoEmpresa
                         || comboContacto_TratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoAsociacion )
                    editContactoNombre->setText( contactos::RecContacto::normalizaEmpresa( editContactoNombre->toString() ) );
            } else {
                editContactoNombre->setText( RecContacto::normalizaNombre( editContactoNombre->toString() ) );
                editContactoNombre->setText( RecContacto::normalizaEmpresa( editContactoNombre->toString() ) );
            }
            editContactoNombre->setJustEdited( true );
        }
        // Si no se está creando un nuevo contacto llama a validSeekCode para comprobar existencia, duplicados, etc.
        if( pTheForm->focusWidget() != pushContactoCIF) { // To avoid triggering the validating if the button is pressed
            // Si está vacío, no buscar ni dar error ni nada
            if( (sender == editContactoCIF && !editContactoCIF->toString().isEmpty() )
                    || (sender == editContactoNombre && !editContactoNombre->toString().isEmpty() ) ) {
                if( false /*mSearching == 1*/ ) {
                    if( pTheForm->validSeekCode( sender, isvalid, *validresult, editContactoCIF, editContactoNombre,
                                                 getRecContacto(), "CIF", "NOMBRE", Xtring::null,
                                                 dbApplication::SeekCodeFlags( dbApplication::AllowNotFound + dbApplication::AskIfFoundOne )) ) {
                        scatterContacto();
                        mSearching = 0;
                        cif_error = Xtring::null;
                    }
                } else {
                    bool copied = false;
                    // Buscar duplicidades, teniendo en cuenta que ya había un contacto en edición
                    dbRecordID otherid = 0;
                    if( sender == editContactoCIF ) {
                        dbRecordID otherid = findDupCIFOrName( editContactoCIF, true );
                        if( otherid ) {
                            if( pTheForm->msgYesNo( pTheForm, Xtring::printf( _("Existe %s con %s '%s'.\n¿Quieres copiar sus datos aqui?"),
                                                    DBAPP->getTableDescSingular("CONTACTO", "una").c_str(),
                                                    cifdef->getCaption().c_str(),
                                                    editContactoCIF->toString().c_str(),
                                                    nombredef->getCaption().lower().c_str(),
                                                    editContactoNombre->toString().c_str() ) ) ) {
                                getRecContacto()->read( otherid );
                                scatterContacto();
                                mSearching = 0;
                                cif_error = Xtring::null;
                                copied = true;
                            } else {
                                pTheForm->msgOk( pTheForm, Xtring::printf( _("Cuando se guarde %s su %s estará duplicado"),
                                                 DBAPP->getTableDescSingular("CONTACTO", "esta").c_str(),
                                                 cifdef->getCaption().c_str() ) );
                            }
                        }
                    }
                    if( !copied && sender == editContactoNombre && editContactoNombre->isJustEdited() ) {
                        otherid = findDupCIFOrName( editContactoNombre, true );
                        if( otherid ) {
                            if( pTheForm->msgYesNo( pTheForm, Xtring::printf( _("Existe %s con %s '%s'.\n¿Quieres copiar sus datos aqui?"),
                                                    DBAPP->getTableDescSingular("CONTACTO", "una").c_str(),
                                                    nombredef->getCaption().lower().c_str(),
                                                    editContactoNombre->toString().c_str() ) ) ) {
                                getRecContacto()->read( otherid );
                                scatterContacto();
                                mSearching = 0;
                                cif_error = Xtring::null;
                                copied = true;
                            }
                        }
                    }
                }
            }
        }
        // Comprobar al final, cuando el CIF ya es correcto, si es apropiado para el tipo de contacto
        if( mUsarTratamiento && (!sender || sender == editContactoCIF) && nac_espana && cif_error.isEmpty() ) {
            if( comboContacto_TratamientoContacto->getCurrentItemValue() == 0 )
                comboContacto_TratamientoContacto->setCurrentItemByValue( RecContacto::tratamientoFromCIF( editContactoCIF->toString() ) );
        }
        if( !cif_error.isEmpty() ) {
            validresult->addWarning( Xtring::printf( _("%s erróneo: %s"),
                                     cifdef->getName().c_str(), cif_error.c_str()), "CIF");
        }
        // Cuando valide el formulario maestro, evitar el error de que el contacto
        // del registro maestro no puede estar vacío
        dbFieldDefinition *fld = DBAPP->getDatabase()->findFieldDefinition(
                                     pTheForm->getRecord()->getTableName() + ".CONTACTO_ID" );
        if( fld ) {
            if ( !fld->canBeNull() ) {
                mSetContactoIDNoNullAgain = true;
                fld->setCanBeNull(true);
            }
        }
        // Comprobar que finalmente el nombre no está vacío y que no se duplica tanto el CIF como el nombre
        if( !sender ) {
            if( !editContactoNombre->toString().isEmpty() && findDupCIFOrName( 0, true ) ) {
                dbFieldDefinition *nombredef = DBAPP->getDatabase()->findFieldDefinition("CONTACTO.NOMBRE");
                validresult->addError( Xtring::printf( _("Ya existe %s con %s '%s' y %s '%s'."),
                                                       DBAPP->getTableDescSingular("CONTACTO", "una").c_str(),
                                                       cifdef->getCaption().c_str(),
                                                       editContactoCIF->toString().c_str(),
                                                       nombredef->getCaption().lower().c_str(),
                                                       editContactoNombre->toString().c_str() ), "CIF" );
                *isvalid = false;
            }
        }
        if( pais_espana && sender == editContacto_CP && editContacto_CP->isJustEdited() ) {
            slotSearchCP_clicked();
            if( editContacto_Provincia->toString().isEmpty() )
                editContacto_Provincia->setText( RecContacto::normalizaNombre(RecContacto::getProvinciaFromCp( editContacto_CP->toString() ) ) );
        }
        if( !sender || sender == editContacto_email ) {
            if( getRecContacto()->existsAnother(
                        "EMAIL=" + getRecContacto()->getConnection()->toSQL( editContacto_email->toString() ) ) ) {
                validresult->addWarning( Xtring::printf( _("Ya existe %s con el mismo email: %s"),
                                         DBAPP->getTableDescSingular( "CONTACTO", "una").c_str(),
                                         editContacto_email->toString().c_str() ), "EMAIL" );
            }
        }
    } else {
        // Dejar como estaba el canbenull del contacto del registro maestro
        if( mSetContactoIDNoNullAgain ) {
            mSetContactoIDNoNullAgain = false;
            dbFieldDefinition *fld = DBAPP->getDatabase()->findFieldDefinition(
                                         pTheForm->getRecord()->getTableName() + ".CONTACTO_ID" );
            fld->setCanBeNull(false);
        }
    }
}

dbRecordID FrmEditContactoBehavior::findDupCIFOrName( QWidget *sender,
        bool codecanbenull, const Xtring &cond )
{
    if( pTheForm->focusWidget() == pTheForm->getAcceptButton() && sender )
        return 0; // This avoids checking twice when pressing pushAccept while in one of the fields
    Xtring bigcond;
    if( sender == editContactoCIF || !sender ) {
        // If value is empty and codecanbenull, do not check dups
        if( !(codecanbenull && editContactoCIF->toString().trim().isEmpty() ) )
            bigcond += "CIF=" + DBAPP->getConnection()->toSQL( editContactoCIF->toString() );
    }
    if( sender == editContactoNombre || !sender ) {
        // editContactoCIF puede estar vacío si solo queremos validar el código
        if( !bigcond.isEmpty() )
            bigcond += " AND ";
        bigcond += "NOMBRE=" + DBAPP->getConnection()->toSQL( editContactoNombre->toString() );
    }
    if( !cond.isEmpty() )
        bigcond = "(" + bigcond + ")AND(" + cond + ")";
    return getRecContacto()->existsAnother( bigcond );
}

bool FrmEditContactoBehavior::save()
{
    bool wasCreating = getRecContacto()->isNew();
    if( wasCreating ) {
        if( getRecContacto()->getValue( "CODIGO" ).toInt() == 0 )
            getRecContacto()->setValue( "CODIGO", getRecContacto()->selectNextInt("CODIGO", Xtring::null, true ) );
    }
    bool modified = getRecContacto()->isModified();
    if( !getRecContacto()->save( false ) ) {
        FrmBase::msgError( pTheForm, _("No se ha podido grabar el contacto") );
        return false;
    }
    if( modified ) {
        if( wasCreating ) {
            DBAPP->showOSD( pTheForm->getRecord()->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                            Xtring::printf( _("Se ha creado un nuevo contacto: %s"), getRecContacto()->toString( TOSTRING_CODE_AND_DESC ).c_str() ) );
        } else {
            DBAPP->showOSD( pTheForm->getRecord()->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                            Xtring::printf( _("Se ha actualizado el contacto: %s"), getRecContacto()->toString( TOSTRING_CODE_AND_DESC ).c_str() ) );
        }
    }
    pTheForm->getRecord()->setValue( "CONTACTO_ID", getRecContacto()->getRecordID() );
    return true;
}


void FrmEditContactoBehavior::slotLimpiaDatos_clicked()
{
    pTheForm->getRecord()->setValue("CONTACTO_ID", 0 );
    getRecContacto()->setNew( true );
    getRecContacto()->clear( true );
    getRecContacto()->setValue( "CIF", Xtring::null );
    getRecContacto()->setValue( "CODIGO", getRecContacto()->selectNextInt("CODIGO", Xtring::null, true ) );
    scatterContacto(); // Todos los campos
    mSearching = 1;
}

void FrmEditContactoBehavior::slotSearchCP_clicked()
{
    if( RecContacto::slotSearchCPClicked( pTheForm, editContacto_CP, editContacto_Barrio,
                                          editContacto_Localidad, editContacto_Provincia) )
        editContacto_email->setFocus();
    else
        editContacto_CP->setFocus();
}

void FrmEditContactoBehavior::slotARazonSocial_clicked()
{
    Xtring nombre = editContactoNombre->toString();
    if( !nombre.isEmpty() ) {
// 		if( comboContacto_TratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoDon
// 			|| comboContacto_TratamientoContacto->getCurrentItemValue() == RecContacto::TratamientoDona ) {
// 			switch( ModuleInstance->getModuleSetting( "FORMATO_NOMBRE" ).toInt() ) {
// 				case 2:
// 				case 4: { // apellidos, nombre
// 					Xtring _nombre, _apellidos;
// 					nombre.splitIn2( _apellidos, _nombre, "," );
// 					_GONG_DEBUG_PRINT( 0, _apellidos );
// 					_GONG_DEBUG_PRINT( 0, _nombre );
// 					editRazonSocial->setText( Xtring(_nombre.trim() + " " + _apellidos.trim()).trim() );
// 					break;
// 				}
// 				default:
// 					editRazonSocial->setText( nombre );
// 					break;
// 			}
// 		} else {
        editRazonSocial->setText( nombre );
// 		}
    }
}

void FrmEditContactoBehavior::slotEditFull_clicked()
{

}


/*<<<<<FRMEDITCONTACTOBEHAVIOR_FIN*/
} // namespace contactos
} // namespace gong
/*>>>>>FRMEDITCONTACTOBEHAVIOR_FIN*/

#if 0
/*<<<<<FRMEDITCONTACTOBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( pTheForm->focusWidget() != pushContactoCIF) // To avoid triggering the validating if the button is pressed
	if( pTheForm->validSeekCode( sender, isvalid, *validresult, editContactoCIF, editContactoNombre,
		getRecContacto(), "CIF", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::dbApplication::AllowNotFound )) )
		scatterContacto();
/*>>>>>FRMEDITCONTACTOBEHAVIOR_VALIDATE*/
#endif

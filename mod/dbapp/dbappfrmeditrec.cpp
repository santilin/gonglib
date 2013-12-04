#include <QTabWidget>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QShowEvent>
#include <gonggettext.h>
#include <gongdbfieldlistofvalues.h>
#include <gongguicontrols.h>
#include <gongimagebox.h>
#include <gongosdwidget.h>
#include <gongguimainwindow.h>
#include "dbappdbapplication.h"
#include "dbappfrmeditrec.h"
#include "dbappfrmeditrecbehavior.h"

namespace gong {

FrmEditRec::FrmEditRec( FrmEditRec *parentfrm, dbRecord *arecord,
                        EditMode editmode, dbApplication::EditFlags editflags,
                        QWidget *parent, const char* name, WidgetFlags fl )
    : FrmBase( parent, name, fl ),
      pMenuBar( 0 ), pMenuRecord( 0 ), pMenuTable(0), pParentForm( parentfrm),
      pOrigRecord( arecord ), mEditMode( editmode ), mEditFlags( editflags ),
      mEditStatus( never_shown ), mLastID( 0 ), mEdited( false ), mControlKeyPressed('\0')
{
    _GONG_DEBUG_ASSERT(  arecord  );
    pRecord = DBAPP->createRecord( arecord->getTableName(), 0, arecord->getUser() );
    pRecord->copyRecord( arecord ); // Detail records must be preserved if cancelled

    pFormLayout = new QVBoxLayout(this);
    pFormLayout->setObjectName( "FormLayout" );
    pMainLayout = new QVBoxLayout();
//   	pMainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    pMainLayout->setObjectName( "MainLayout" );

    pMenuBar = new QMenuBar( this, "MainMenuBar" );
    pMenuRecord = new QMenu( this );
    pMenuRecord->setObjectName( "MenuRecord" );
    pMenuBar->insertItem( QString::fromUtf8(_( "&Registro" )), pMenuRecord );
    pMenuTable = new QMenu( this );
    pMenuTable->setObjectName( "MenuTable" );
    pMenuBar->insertItem( QString::fromUtf8(_( "&Fichero" )), pMenuTable );
    // http://lists.trolltech.com/qt-interest/2004-04/thread00216-0.html
    pMenuBar->setNativeMenuBar(false);
    QPalette palette = pMenuBar->palette();
    palette.setColor(QPalette::Button, this->palette().color(QPalette::Window));
    pMenuBar->setPalette(palette);
    layout()->setMenuBar( pMenuBar );

    pTabWidget = new QTabWidget(this);
    pControlsFrame = new QWidget(pTabWidget);
    pControlsFrame->setObjectName("ControlsFrame");
    pControlsLayout = new QVBoxLayout(pControlsFrame);
    pControlsLayout->setObjectName("ControlsLayout");
//   	pControlsLayout->setSizeConstraint(QLayout::SetFixedSize);
    pTabWidget->addTab( pControlsFrame, "&General" );
    showTabs( false );
    pMainLayout->addWidget(pTabWidget);

    pButtonsLayout = new QHBoxLayout();
    pButtonsLayout->setObjectName("ButtonsLayout");
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    pButtonsLayout->addItem(horizontalSpacer);
    pButtonsLayout->addWidget(pushCancel);
    pButtonsLayout->addWidget(pushAccept);
    pMainLayout->addLayout(pButtonsLayout);
    pFormLayout->addLayout(pMainLayout);
}

/*
 *  Destroys the object and frees any allocated resources
 */
FrmEditRec::~FrmEditRec()
{
    delete pRecord;
    if( (mEditFlags & dbApplication::deleteRecord) && pOrigRecord )
        delete pOrigRecord;
}

void FrmEditRec::setReadOnly(bool ro)
{
    if( ro ) {
        mEditFlags = static_cast<dbApplication::EditFlags>( mEditFlags | dbApplication::readOnly );
    } else {
        mEditFlags = static_cast<dbApplication::EditFlags>( mEditFlags & ~dbApplication::readOnly );
    }
}

void FrmEditRec::showTabs(bool show)
{
    QTabBar *tb = pTabWidget->findChild<QTabBar *>();
    if( tb )
        tb->setVisible( show );
}

QWidget *FrmEditRec::insertTab(QWidget* tab, const Xtring& label, int index)
{
    if( !tab )
        tab = new QWidget( this );
    pTabWidget->insertTab( index, tab, toGUI(label) );
    return tab;
}

/**
 * @brief Sets the label text of a tab widget. There is no easy way of doing this from the widget itself
 *
 * @param tab ...
 * @param title ...
 * @return void
 **/
void FrmEditRec::setTabTitle(QWidget* tab, const Xtring& title)
{
    for( int index = 0; index < pTabWidget->count(); ++index ) {
        if( pTabWidget->widget( index ) == tab ) {
            pTabWidget->setTabText( index, toGUI( title ) );
            break;
        }
    }
}

void FrmEditRec::initMenus()
{
}

/**
    Inicializa el formulario para comenzar la edición del registro

	Esta funcion, a pesar de ser virtual, no es llamada desde la clase derivada FrmEditRecMaster
 */
void FrmEditRec::justShown( bool firsttime )
{
    if( firsttime || mEditStatus == never_shown ) {
        initMenus();
        for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
                bit != mBehaviors.end();
                ++ bit ) {
            (*bit)->initGUI();
        }
        mEditStatus = not_saved;
        scatter();
        fixFocusWidgetText();
        setInitialFocus();
        addSpacersToTabs();
    }
    updateStatus();
}

void FrmEditRec::specialControlKeyPressed(QWidget* sender, char key)
{
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end(); ++ bit )
        (*bit)->specialControlKeyPressed( sender, key );
}

void FrmEditRec::validateFields( QWidget * /*sender*/, bool *isvalid, ValidResult *ir )
{
    if( isvalid )
        *isvalid = true;
    return;
}


// reimplementada de FrmBase
// La clase FrmEditRecMaster no usa esta funcion como base,
// sino que la redefine completamente.
void FrmEditRec::accept()
{
    bool isvalid = true;
    mEditStatus = not_saved;
    try {
        switch ( mEditMode ) {
        case DataTable::inserting:
        case DataTable::duplicating:
        case DataTable::updating:
            gather();
            validate( NULL, &isvalid );
            if ( isvalid ) {
                if ( save() )
                    mEditStatus = saved;
            } else
                return ;
            break;
        case DataTable::deleting:
            if ( remove() )
                mEditStatus = saved;
            break;
        case DataTable::selecting:
            mEditStatus = saved;
            break;
        default:
            _GONG_DEBUG_WARNING( "Unknown editmode: " + Xtring::number( mEditMode ) );
        }
        if ( mEditStatus == saved ) {
            mLastID = getRecord()->getRecordID();
            mEdited = false;
            if ( mEditFlags & dbApplication::editContinuous ) {
                mEditMode = DataTable::inserting;
                mEditStatus = not_saved;
            } else {
                FrmBase::accept();
                return;
            }
        }
    } catch ( dbError & err ) {
        _GONG_DEBUG_PRINT(0, err.getNumber() );
        msgError( this,
                  Xtring::printf( _("No se ha podido %1s %2s.\n%3s"),
                                  mEditMode == DataTable::deleting ? _("borrar") : _("grabar"),
                                  DBAPP->getTableDescSingular(pRecord->getTableName(), "la" ).c_str(),
                                  err.what() ) );
    }
    updateStatus();
}


/** reimplementada de FrmBase
  Si se han editado los datos, muestra un mensaje de confirmación
  Atencion: frmEditRecMaestro no llama a esta funcion
  */
bool FrmEditRec::canClose()
{
    if ( wasCancelled() )
        mEditStatus = cancelled;
    bool willclose = false;
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end();
            ++ bit ) {
        if( !(*bit)->canClose() )
            return false;
    }
    if ( isEnabled() ) {
        if ( mEditStatus == saved ) {
            willclose = true;
        } else if ( !isEditing() || !edited()
                    || msgYesNo( this,
                                 Xtring::printf(_( "¿Estás segura de cancelar la edición de %s?\n\n" ),
                                                DBAPP->getTableDescSingular(pRecord->getTableName(), "esta" ).c_str()),
                                 false ) ) {
            mEdited = false;
            mEditStatus = cancelled;
            willclose = true;
        }
    }
    return willclose;
}

bool FrmEditRec::showAndSave()
{
    show();
    accept();
    return !wasCancelled();
}

/**
 * @brief Updates the form controls based on the internal state of the record
 *
 * @return void
 **/
void FrmEditRec::updateStatus(bool callbehaviors)
{
    Xtring caption;
    switch ( mEditMode ) {
    case DataTable::inserting:
        caption += _("Añadiendo ");
        enableEditControls( true );
        pushAccept->setText( _("&Grabar") );
        break;
    case DataTable::updating:
        caption += _("Modificando ");
        enableEditControls( true );
        pushAccept->setText( _("&Grabar") );
        break;
    case DataTable::deleting:
        caption += _("Borrando ");
        enableEditControls( false );
        break;
    case DataTable::selecting:
        caption += _("Examinando ");
        enableEditControls( false );
        break;
    case DataTable::duplicating:
        caption += _("Duplicando ");
        pushAccept->setText( _("&Grabar") );
        enableEditControls( true );
        break;
    case DataTable::finding:
        caption += _("Buscando ");
        enableEditControls( true );
        break;
    case DataTable::filtering:
        caption += _("Filtrando ");
        enableEditControls( true );
        break;
    case DataTable::defaulteditmode:
        _GONG_DEBUG_WARNING( "Editmode not set" );
        break;
    case DataTable::cancelling:
    case DataTable::accepting:
        break;
    case DataTable::choosing:
    case DataTable::browsing:
        return ;
    }
    caption += pRecord->getTableDefinition()->getDescSingular();
    if ( mEditMode != DataTable::inserting )
        caption += ": " + getRecord()->toString(TOSTRING_CODE_AND_DESC);
    if( isReadOnly() )
        caption = "(R) " + caption;
    if ( !(mEditFlags & dbApplication::embedded) )
        setTitle( caption );
    if( callbehaviors ) {
        for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
                bit != mBehaviors.end();
                ++ bit ) {
            (*bit)->updateStatus();
        }
    }
}


/**
  Las funciones save, remove, read y clear
  se redefinen en los formularios que contienen más de un registro
  para manejar todos los registros.
  */
/**
  Lee un registro de la base de datos y vuelca los campos sobre
  los controles del formulario.
  Si el registro no se encuentra en la base de datos, se vacía.
  */
bool FrmEditRec::read(dbRecordID id )
{
    bool ret;
    if ( id == 0 )
        _GONG_DEBUG_WARNING( __FUNCTION__ + getRecord()->getTableName() + ".ID = 0" );
    ret = getRecord()->read( id );
    if ( !ret ) {
        getRecord()->setNew();
    } else {
        for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
                bit != mBehaviors.end();
                ++ bit ) {
            ret = (*bit)->read( id );
        }
    }
    pOrigRecord->copyRecord( getRecord());
    mEdited = false;
    return ret;
}

/**
  Guarda el registro en la base de datos. Si es nuevo se inserta, y si no se modifica
  */
bool FrmEditRec::save()
{
    _GONG_DEBUG_ASSERT( getRecord() );
    _GONG_DEBUG_ASSERT( !getRecord()->getTableName().isEmpty() );
    bool ret;
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end();
            ++ bit ) {
        ret = (*bit)->save();
        if( !ret )
            break;
    }
    if( (ret = getRecord()->save( true )) ) {
        pOrigRecord->copyRecord( getRecord(), true ); // deep copy
        if( pParentForm )
            pParentForm->refresh();
        DBAPP->showStickyOSD( getRecord()->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                              _("Se ha grabado correctamente")  );
        if( DBAPP->getMainWindow() ) /// TODO: Refrescar solo los browses de esta tabla y las relacionadas
            DBAPP->getMainWindow()->refreshByName(name(), "");
    }
    return ret;
}

/**
  Borra permanentemente el registro de la base de datos
  */
bool FrmEditRec::remove()
{
    Xtring usingtable;
    if ( DBAPP->getDatabase()->isRecordUsed( getRecord(), &usingtable ) ) {
        msgOk( this,
               Xtring::printf( _("%s '%s'\nestá en uso en el fichero de %s"),
                               DBAPP->getTableDescSingular(pRecord->getTableName(), "esta" ).proper().c_str(),
                               pRecord->toString( TOSTRING_CODE_AND_DESC ).c_str(),
                               DBAPP->getDatabase()->findTableDefinition( usingtable )->getDescPlural().c_str() ) );
        return false;
    }
    bool ret;
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end();
            ++ bit ) {
        ret = (*bit)->remove();
        if( !ret )
            break;
    }
    if( (ret = getRecord()->remove()) ) {
        pOrigRecord->copyRecord( getRecord(), true ); // deep copy
        if( pParentForm )
            pParentForm->refresh();
        DBAPP->showStickyOSD( getRecord()->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                              _("Se ha borrado correctamente")  );
    }
    return ret;
}

/**
  Vacía los campos del registro del formulario y lo pone como nuevo,
  preparado para añadir.
  */
void FrmEditRec::clearRecord( bool duplicating )
{
    getRecord()->setNew( true );
    if ( !duplicating )
        getRecord()->clear( true ); // set default custom values
}


void FrmEditRec::msgInputError( const Xtring &errorText )
{
    msgError ( this, errorText );
}

void FrmEditRec::showValidMessages( bool *isvalid, const ValidResult &validresult, QWidget *sender )
{
    Xtring messages;
    if( !isvalid )
        return;
    if( !( *isvalid ) ) {
        // Show only the error messages
        for ( uint nm = 0; nm < validresult.count(); nm++ )
            if ( validresult.getMessageInfo( nm ).code == ValidResult::error )
                messages += validresult.getMessageInfo( nm ).message + "\n";
        if( !messages.isEmpty() ) {
            if( !sender )
                msgError( this, messages );
            else
                theGuiApp->showOSD( _("Error "), messages);
        }
    } else {
        // Show the warning messages
        if ( validresult.count() > 0 ) {
            for ( uint nm = 0; nm < validresult.count(); nm++ )
                messages += validresult.getMessageInfo( nm ).message + "\n";
            if( !sender ) {
                messages += Xtring::printf(_("\n¿Estás segura de grabar %s?"),
                                           DBAPP->getTableDescSingular(pRecord->getTableName(), "esta" ).c_str());
                if( !FrmBase::msgYesNo( this, messages ) )
                    *isvalid = false;
            } else {
                theGuiApp->showOSD( _("Aviso "), messages);
            }
        }
    }
/// \todo {qt} Este set focus causa conflictos con el del choose
    if ( (messages.isEmpty() || validresult.count()) && !sender ) {
//		if( focusWidget() == pushAccept || focusWidget() == pushCancel ) {
        QWidget *focusfield = findControl( validresult.getMessageInfo( 0 ).wrongfield );
        if ( focusfield )
            focusfield->setFocus();
//		}
    }
    return;
}

EditBox *FrmEditRec::addEditField( QWidget * parent, const Xtring & tablename,
                                   const Xtring & fldname, QBoxLayout * layout, bool horizontal )
{
    EditBox *edit = new EditBox( parent ? parent : pControlsFrame, tablename + "_" + fldname, "", horizontal );
    const dbFieldDefinition *flddef = DBAPP->getDatabase()->findFieldDefinition( tablename, fldname );
    applyFieldStyle( edit->getLabel(), flddef );
    applyFieldStyle( edit, flddef );
    connect( edit, SIGNAL( validate( QWidget *, bool * ) ), this, SLOT( validate( QWidget *, bool * ) ) );
    if ( layout ) {
        layout->addWidget( edit->getLabel() );
        layout->setAlignment( edit->getLabel(), Qt::AlignRight );
        layout->addWidget( edit );
        if( edit->sizePolicy().horizontalPolicy() == QSizePolicy::Fixed )
            layout->setAlignment( edit, Qt::AlignLeft );
    } else {
        pControlsLayout->addLayout( edit->getLayout() );
        edit->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    }
    mEditControls.push_back( edit );
    if( flddef ) {
        edit->setMustBeReadOnly( flddef->isReadOnly() );
        if( flddef->isVisible() == false ) {
            edit->setVisible( false );
            edit->getLabel()->setVisible( false );
        }
    }
    return edit;
}

TextBox *FrmEditRec::addTextField( QWidget * parent, const Xtring & tablename,
                                   const Xtring & fldname, QBoxLayout * layout, bool horizontal )
{
    TextBox * edit = new TextBox( parent ? parent : pControlsFrame,
                                  tablename + "_" + fldname, "", horizontal );
    if ( layout ) {
        if( horizontal ) {
            layout->addWidget( edit->getLabel() );
            layout->setAlignment( edit->getLabel(), Qt::AlignRight );
            layout->addWidget( edit );
            if( edit->maximumWidth() != 16777215 )
                layout->setAlignment( edit, Qt::AlignLeft );
        } else {
            QHBoxLayout *tmplay = new QHBoxLayout();
            layout->addLayout( tmplay );
            tmplay->addLayout( edit->getLayout() );
        }
    } else
        pControlsLayout->addLayout( edit->getLayout() );
    const dbFieldDefinition *flddef = DBAPP->getDatabase()->findFieldDefinition( tablename, fldname );
    applyFieldStyle( edit->getLabel(), flddef );
    applyFieldStyle( edit, flddef );
    // Solo despues de aplicar el estilo podemos saber si el control es readonly o no
    connect( edit, SIGNAL( validate( QWidget *, bool * ) ), this, SLOT( validate( QWidget *, bool * ) ) );
    mEditControls.push_back( edit );
    if( flddef ) {
        edit->setMustBeReadOnly( flddef->isReadOnly() );
        if( flddef->isVisible() == false ) {
            edit->setVisible( false );
            edit->getLabel()->setVisible( false );
        }
    }
//	edit->setHeightInLines(4);
    return edit;
}

RichTextBox *FrmEditRec::addRichTextField( QWidget *parent, const Xtring &tablename,
        const Xtring &fldname, QBoxLayout *layout, bool horizontal )
{
    RichTextBox *rich = new RichTextBox( parent ? parent : pControlsFrame,
                                         tablename + "_" + fldname, "", horizontal );
    rich->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    if ( layout )
        layout->addLayout( rich->getLayout() );
    else
        pControlsLayout->addLayout( rich->getLayout() );
    const dbFieldDefinition *flddef = DBAPP->getDatabase()->findFieldDefinition( tablename, fldname );
    applyFieldStyle( rich->getLabel(), flddef );
    applyFieldStyle( rich, flddef );
    if( rich->mustBeReadOnly() )
        rich->setReadOnly(true);
    mEditControls.push_back( rich );
    if( flddef ) {
        rich->setMustBeReadOnly( flddef->isReadOnly() );
        if( flddef->isVisible() == false ) {
            rich->setVisible( false );
            rich->getLabel()->setVisible( false );
        }
    }
    return rich;
}

SearchBox *FrmEditRec::addSearchField( QWidget *parent, const Xtring &fldname,
                                       const Xtring &reftablename, const Xtring &fldnamecodigo, const Xtring &flddnamedesc,
                                       QBoxLayout *layout, SearchBox::Flags flags )
{
    const bool showlabels = (flags & SearchBox::FlagShowLabels);
    SearchBox * search = new SearchBox( "", reftablename, fldnamecodigo, flddnamedesc,
                                        parent ? parent : pControlsFrame, flags );
    if ( layout ) {
        layout->addWidget( search->getButton() );
        if( showlabels ) layout->addWidget( search->getLabelCode() );
        layout->addWidget( search->getEditCode() );
        if( showlabels ) layout->addWidget( search->getLabelDesc() );
        layout->addWidget( search->getEditDesc() );
    } else
        pControlsLayout->addLayout( search->getLayout() );
    dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( reftablename );
    if( tbldef ) {
        dbFieldDefinition *flddefdesc = tbldef->findFieldDefinition( flddnamedesc, true );
        search->getButton()->setText( toGUI( tbldef->getDescSingular().c_str()) );
        if( flddefdesc ) {
		    // 	applyFieldStyle( search->getButton(), flddefdesc );
		    // Set the caption to the ddd field caption of the reference if appropiate
		    dbFieldDefinition *reference_id_fld = getRecord()->getTableDefinition()->findFieldDefinition( fldname );
		    if( reference_id_fld ) {
		        Xtring caption = reference_id_fld->getCaption();
		        if( caption.upper() != fldname.upper() ) {
		            search->getButton()->setText( toGUI( caption ) );
		        }
		    }
		    int nombrewidth = flddefdesc->getDisplayWidth(), savewidth = nombrewidth;
		    if( nombrewidth > 15 )
		        nombrewidth = nombrewidth * 0.3;
		    if( nombrewidth > 15 )
		        nombrewidth = 15;
		    flddefdesc->setDisplayWidth( nombrewidth );
			applyFieldStyle( search->getEditCode(), flddefdesc );
	 		search->getEditCode()->setMaxLength( savewidth );
    	    flddefdesc->setDisplayWidth( savewidth );
		    applyFieldStyle( search->getEditDesc(), flddefdesc );
		}
        search->setMustBeReadOnly( search->getEditCode()->isReadOnly() );
        if( showlabels ) {
            dbFieldDefinition *flddefcode = tbldef->findFieldDefinition( fldnamecodigo, true );
            applyFieldStyle( search->getLabelCode(), flddefcode );
			if( flddefdesc )
            	applyFieldStyle( search->getLabelDesc(), flddefdesc );
        }
        search->getEditCode()->setName( ( reftablename + "_" + fldnamecodigo ).c_str() );
        search->getEditDesc()->setName( ( reftablename + "_" + flddnamedesc ).c_str() );
        connect( search->getEditCode(), SIGNAL( specialControlKeyPressed( QWidget *, char ) ),
                 this, SLOT( specialControlKeyPressed( QWidget *, char ) ) );
        connect( search->getEditCode(), SIGNAL( validate( QWidget *, bool * ) ),
                 this, SLOT( validate( QWidget *, bool * ) ) );
        if( flags & SearchBox::FlagEditableDesc )
            connect( search->getEditDesc(), SIGNAL( validate( QWidget *, bool * ) ),
                     this, SLOT( validate( QWidget *, bool * ) ) );
    }
    mEditControls.push_back( search );
    return search;
}


CheckBox *FrmEditRec::addCheckField( QWidget * parent, const Xtring & tablename,
                                     const Xtring & fldname, QBoxLayout * layout )
{
    CheckBox * check = new CheckBox( parent ? parent : pControlsFrame, tablename + "_" + fldname, "" );
    dbFieldDefinition *flddef = DBAPP->getDatabase()->findFieldDefinition( tablename, fldname );
    applyFieldStyle( check, flddef );
    if ( layout )
        layout->addWidget( check );
    else
        pControlsLayout->addWidget( check );
    mEditControls.push_back( check );
    connect( check, SIGNAL( stateChanged( int ) ), this, SLOT( validateComboOrCheck( int ) ) );
    if( flddef ) {
        check->setMustBeReadOnly( flddef->isReadOnly() );
        if( flddef->isVisible() == false )
            check->setVisible( false );
    }
    return check;
}

ComboBoxXtring *FrmEditRec::addComboXtringField( QWidget * parent, const Xtring & tablename,
        const Xtring & fldname, QBoxLayout * layout, bool horizontal )
{
    ComboBoxXtring *combo = 0;
    dbFieldListOfValues<Xtring> *flddef = static_cast<dbFieldListOfValues<Xtring> *>
                                          (DBAPP->getDatabase()->findFieldDefinition( tablename, fldname ) );
    if( flddef ) {
        combo = new ComboBoxXtring(
            flddef->getListOfCaptions(), flddef->getListOfValues(),
            parent ? parent : pControlsFrame, tablename + "_" + fldname, Xtring(), horizontal );
        applyFieldStyle( combo, flddef );
        applyFieldStyle( combo->getLabel(), flddef );
        if ( layout ) {
            layout->addWidget( combo->getLabel() );
            layout->setAlignment( combo->getLabel(), Qt::AlignRight );
            layout->addWidget( combo );
            if( combo->maximumWidth() != 16777215 )
                layout->setAlignment( combo, Qt::AlignLeft );
        } else
            pControlsLayout->addLayout( combo->getLayout() );
        // Insert one blank value only if it don't exists yet
        mEditControls.push_back( combo );
        connect( combo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( validateComboOrCheck( int ) ) );
        combo->setMustBeReadOnly( flddef->isReadOnly() );
        if( flddef->isVisible() == false ) {
            combo->setVisible( false );
            combo->getLabel()->setVisible( false );
        }
        if( !flddef->isReadOnly() && flddef->canBeNull() )
            combo->insertItem( "", "" );
    } else {
        _GONG_DEBUG_WARNING( Xtring::printf( "Field %s.%s not exists",
                                             tablename.c_str(), fldname.c_str() ) );
    }
    return combo;
}

ComboBoxInt *FrmEditRec::addComboIntField( QWidget * parent, const Xtring & tablename,
        const Xtring & fldname, QBoxLayout * layout, bool horizontal )
{
    ComboBoxInt *combo = 0;
    dbFieldListOfValues<int> *flddef = static_cast<dbFieldListOfValues<int> *>
                                       (DBAPP->getDatabase()->findFieldDefinition( tablename, fldname ) );
    if( flddef ) {
        combo = new ComboBoxInt( flddef->getListOfCaptions(), flddef->getListOfValues(),
            parent ? parent : pControlsFrame, tablename + "_" + fldname, Xtring(), horizontal );
        applyFieldStyle( combo, flddef );
        applyFieldStyle( combo->getLabel(), flddef );
        if ( layout ) {
            layout->addWidget( combo->getLabel() );
            layout->setAlignment( combo->getLabel(), Qt::AlignRight );
            layout->addWidget( combo );
            if( combo->maximumWidth() != 16777215 )
                layout->setAlignment( combo, Qt::AlignLeft );
        } else
            pControlsLayout->addLayout( combo->getLayout() );
        combo->setMustBeReadOnly( flddef->isReadOnly() );
        if( flddef->isVisible() == false ) {
            combo->setVisible( false );
            combo->getLabel()->setVisible( false );
        }
        if( !flddef->isReadOnly() && flddef->canBeNull() )
            combo->insertItem( "", 0 );
        mEditControls.push_back( combo );
        connect( combo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( validateComboOrCheck( int ) ) );
    } else {
        combo = 0;
        _GONG_DEBUG_WARNING( Xtring::printf( "Field %s.%s not exists",
                                             tablename.c_str(), fldname.c_str() ) );
    }
    return combo;
}

ImageBox *FrmEditRec::addImageField( QWidget *parent, const Xtring &tablename,
                                     const Xtring &fldname, QBoxLayout * layout,
                                     bool horizontal)
{
    ImageBox *image = new ImageBox( parent ? parent : pControlsFrame,
                                    tablename + "_" + fldname, "", horizontal);
    dbFieldDefinition *flddef = DBAPP->getDatabase()->findFieldDefinition( tablename, fldname );
    applyFieldStyle( static_cast<QPushButton *>(image), flddef );
    image->setText( image->text() + "..." );
    if ( layout ) {
        layout->addLayout( image->getLayout() );
    } else
        pControlsLayout->addLayout( image->getLayout() );
    mEditControls.push_back( image );
    if( flddef ) {
        image->setMustBeReadOnly( flddef->isReadOnly() );
        if( flddef->isVisible() == false )
            image->setVisible( false );
    }
    return image;
}

FileNameBox* FrmEditRec::addFileNameField(QWidget* parent, const Xtring& tablename,
        const Xtring& fldname, QBoxLayout* layout,
        bool horizontal)
{
    FileNameBox *edit = new FileNameBox( parent ? parent : pControlsFrame,
                                         tablename + "_" + fldname, "", false /*choosedir*/, horizontal );
    if ( layout ) {
        if( horizontal ) {
            layout->addWidget( edit );
            layout->setAlignment( edit, Qt::AlignRight );
            layout->addWidget( edit->getEditFileName() );
            if( edit->maximumWidth() != 16777215 )
                layout->setAlignment( edit, Qt::AlignLeft );
        } else {
            QHBoxLayout *tmplay = new QHBoxLayout();
            layout->addLayout( tmplay );
            tmplay->addLayout( edit->getLayout() );
        }
    } else
        pControlsLayout->addLayout( edit->getLayout() );
    const dbFieldDefinition *flddef = DBAPP->getDatabase()->findFieldDefinition( tablename, fldname );
    applyFieldStyle( edit, flddef );
    applyFieldStyle( edit->getEditFileName(), flddef );
    // Solo despues de aplicar el estilo podemos saber si el control es readonly o no
    connect( edit->getEditFileName(), SIGNAL( validate( QWidget *, bool * ) ), this, SLOT( validate( QWidget *, bool * ) ) );
    mEditControls.push_back( edit );
    if( flddef ) {
        edit->setMustBeReadOnly( flddef->isReadOnly() );
        if( flddef->isVisible() == false ) {
            edit->setVisible( false );
        }
    }
    return edit;
}


QHBoxLayout *FrmEditRec::addHBoxLayout( QBoxLayout * parent )
{
    QHBoxLayout * l = new QHBoxLayout( 0, 0, 6 );
    if( parent )
        parent->addLayout( l );
    else
        pControlsLayout->addLayout( l );
    return l;
}

QVBoxLayout *FrmEditRec::addVBoxLayout( QBoxLayout * parent )
{
    QVBoxLayout * l = new QVBoxLayout( 0, 0, 6 );
    if( parent )
        parent->addLayout( l );
    else
        pControlsLayout->addLayout( l );
    return l;
}

bool FrmEditRec::addBehavior(FrmEditRecBehavior* behavior)
{
    _GONG_DEBUG_ASSERT( behavior );
    if( !mBehaviors.contains( behavior ) )
        mBehaviors.push_back( behavior );
    return true;
}

/**
 * @brief Gets the value of a control by the name of the field
 *
 * @param fieldname ...
 * @return Variant
 **/
Variant FrmEditRec::getControlValue(const Xtring& fieldname) const
{
    QWidget *control = findControl( fieldname );
    if( control ) {
        if ( EditBox * edit = dynamic_cast<EditBox *>( control ) ) {
            return edit->toVariant();
        } else if ( TextBox * edit = dynamic_cast<TextBox *>( control ) ) {
            return edit->toString();
        } else if ( RichTextBox * edit = dynamic_cast<RichTextBox *>( control ) ) {
            return edit->toString();
        } else if ( SearchBox * search = dynamic_cast<SearchBox *>( control ) ) {
            return search->getEditCode()->toString();
        } else if ( CheckBox * check = dynamic_cast<CheckBox *>( control ) ) {
            return check->isChecked();
        } else if ( ComboBoxInt *combo = dynamic_cast<ComboBoxInt *>( control ) ) {
            return combo->getCurrentItemValue();
        } else if ( ComboBoxXtring *combo = dynamic_cast<ComboBoxXtring *>( control ) ) {
            return combo->getCurrentItemValue();
        } else if ( ImageBox *image = dynamic_cast<ImageBox *>( control ) ) {
            return image->toData();
        } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( control ) ) {
            return fnbox->getEditFileName()->toVariant();
        }
    }
    return Variant();
}


/**
 * @brief Sets the value of a control by the name of a field
 *
 * @param fieldname ...
 * @return Variant
 **/
bool FrmEditRec::setControlValue(const Xtring& fieldname, const Variant &value )
{
    QWidget *control = findControl( fieldname );
    if( control ) {
        if ( EditBox * edit = dynamic_cast<EditBox *>( control ) ) {
            edit->setText( value );
        } else if ( TextBox * edit = dynamic_cast<TextBox *>( control ) ) {
            edit->setText( value.toString() );
        } else if ( RichTextBox * edit = dynamic_cast<RichTextBox *>( control ) ) {
            edit->setText( value.toString() );
        } else if ( SearchBox * search = dynamic_cast<SearchBox *>( control ) ) {
            search->getEditCode()->setText( value );
        } else if ( CheckBox * check = dynamic_cast<CheckBox *>( control ) ) {
            check->setChecked( value.toBool() );
        } else if ( ComboBoxInt *combo = dynamic_cast<ComboBoxInt *>( control ) ) {
            combo->setCurrentItemByValue( value.toInt() );
        } else if ( ComboBoxXtring *combo = dynamic_cast<ComboBoxXtring *>( control ) ) {
            combo->setCurrentItemByValue( value.toString() );
        } else if ( ImageBox *image = dynamic_cast<ImageBox *>( control ) ) {
            image->setImageData( value.toString() );
        } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( control ) ) {
            fnbox->getEditFileName()->setText( value.toString() );
        }
    }
    return false;
}

/**
 * @brief Looks for the control bound to a field of the record
 *
 * @param _fieldname name or fullname of the field of the record
 * @return QWidget*
 **/
QWidget *FrmEditRec::findControl( const Xtring & _fieldname ) const
{
    if( _fieldname.isEmpty() )
        return 0;
    Xtring fieldname = _fieldname.upper();
    fieldname.replace(".", "_");
    for ( EditControlsList::const_iterator it = mEditControls.begin();
            it != mEditControls.end();
            ++ it ) {
		_GONG_DEBUG_PRINT( 0, Xtring::printf( "Field:%s, comparando con %s", fieldname.c_str(), (*it)->name() ) );
        if ( SearchBox * search = dynamic_cast<SearchBox *>( *it ) ) {
			_GONG_DEBUG_PRINT( 0, Xtring::printf( "Field:%s, comparando con %s", fieldname.c_str(), search->getEditCode()->name() ) );
			_GONG_DEBUG_PRINT( 0, Xtring::printf( "Field:%s, comparando con %s", fieldname.c_str(), search->getEditDesc()->name() ) );
            if ( Xtring( search->getEditCode()->name() ).upper() == fieldname ) {
                return search->getEditCode();
            } else if ( Xtring( search->getEditDesc()->name() ).upper() ==  fieldname ) {
                return search->getEditDesc();
            }
        }
        if( Xtring( ( *it )->name() ).upper() == fieldname )
            return *it;
        else if ( ( Xtring( ( *it )->name() ).upper() ).endsWith( "_" + fieldname.upper() ) )
            return *it;
    }
    _GONG_DEBUG_PRINT(1, Xtring::printf( "%s: EditField not found", fieldname.c_str() ) );
    return 0;
}

/**
 * @brief Checks if any of the controls in a form is edited
 *
 * @return bool
 **/
bool FrmEditRec::edited() const
{
    if( mEdited )
        return true;
    for ( EditControlsList::const_iterator it = mEditControls.begin();
            it != mEditControls.end();
            ++ it ) {
        if ( EditBox * edit = dynamic_cast<EditBox *>( *it ) ) {
            if ( edit->edited() )
                return true;
        } else if ( TextBox * edit = dynamic_cast<TextBox *>( *it ) ) {
            if ( edit->isEdited() )
                return true;
        } else if ( RichTextBox * edit = dynamic_cast<RichTextBox *>( *it ) ) {
            if ( edit->isEdited() )
                return true;
        } else if ( SearchBox * search = dynamic_cast<SearchBox *>( *it ) ) {
            if ( search->getEditCode()->edited() || search->getEditDesc()->edited() )
                return true;
        } else if ( CheckBox * check = dynamic_cast<CheckBox *>( *it ) ) {
            if ( check->isEdited() )
                return true;
        } else if ( ComboBoxInt *combo = dynamic_cast<ComboBoxInt *>( *it ) ) {
            if ( combo->isEdited() )
                return true;
        } else if ( ComboBoxXtring *combo = dynamic_cast<ComboBoxXtring *>( *it ) ) {
            if ( combo->isEdited() )
                return true;
        } else if ( ImageBox *image = dynamic_cast<ImageBox *>( *it ) ) {
            if ( image->isEdited() )
                return true;
        } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( *it ) ) {
            if( fnbox->getEditFileName()->isEdited() )
                return true;
        }
    }
    return false;
}


/**
 * @brief Sets or resets the edited flag of all the controls of the form
 *
 * @param edited ...
 * @return void
 **/
void FrmEditRec::setEdited( bool edited )
{
    mEdited = edited;
    for ( EditControlsList::const_iterator it = mEditControls.begin();
            it != mEditControls.end();
            ++ it ) {
        if ( EditBox * edit = dynamic_cast<EditBox *>( *it ) ) {
            edit->setEdited( edited );
        } else if ( TextBox * edit = dynamic_cast<TextBox *>( *it ) ) {
            edit->setEdited( edited );
        } else if ( RichTextBox * edit = dynamic_cast<RichTextBox *>( *it ) ) {
            edit->setEdited( edited );
        } else if ( SearchBox * search = dynamic_cast<SearchBox *>( *it ) ) {
            search->getEditCode()->setEdited( edited );
            search->getEditDesc()->setEdited( edited );
        } else if ( CheckBox * check = dynamic_cast<CheckBox *>( *it ) ) {
            check->setEdited( edited );
        } else if ( ComboBoxInt * combo = dynamic_cast<ComboBoxInt *>( *it ) ) {
            combo->setEdited( edited );
        } else if ( ComboBoxXtring * combo = dynamic_cast<ComboBoxXtring *>( *it ) ) {
            combo->setEdited( edited );
        } else if ( ImageBox *image = dynamic_cast<ImageBox *>( *it ) ) {
            image->setEdited( edited );
        } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( *it ) ) {
            fnbox->getEditFileName()->setEdited( edited );
        }
    }
}

/**
 * @brief enables or disables all the controls in a form
 *
 * @param enabled ...
 * @return void
 **/
void FrmEditRec::enableEditControls( bool enabled )
{
    for ( EditControlsList::const_iterator it = mEditControls.begin();
            it != mEditControls.end();
            ++ it ) {
        if ( EditBox * edit = dynamic_cast<EditBox *>( *it ) ) {
            if ( !enabled || edit->mustBeReadOnly() )
                edit->setReadOnly( true );
            else
                edit->setReadOnly( false );
        } else if ( TextBox * edit = dynamic_cast<TextBox *>( *it ) ) {
            if ( !enabled || edit->mustBeReadOnly() )
                edit->setReadOnly( true );
            else
                edit->setReadOnly( false );
        } else if ( RichTextBox * edit = dynamic_cast<RichTextBox *>( *it ) ) {
            if ( !enabled || edit->mustBeReadOnly() )
                edit->setReadOnly( true );
            else
                edit->setReadOnly( false );
        } else if ( SearchBox * search = dynamic_cast<SearchBox *>( *it ) ) {
            if ( !enabled || search->mustBeReadOnly() ) {
                search->getEditCode()->setReadOnly( true );
                search->getEditDesc()->setReadOnly( true );
                search->getButton()-> setEnabled (true);
            } else {
                search->getEditCode()->setReadOnly( false );
                search->getEditDesc()->setReadOnly( !(search->getFlags() & SearchBox::FlagEditableDesc) );
                search->getButton()-> setEnabled (true);
            }
        } else if ( CheckBox * check = dynamic_cast<CheckBox *>( *it ) ) {
            if ( !enabled || check->mustBeReadOnly() )
                check->setEnabled( false );
            else
                check->setEnabled( true );
        } else if ( ComboBoxInt * combo = dynamic_cast<ComboBoxInt *>( *it ) ) {
            if ( !enabled || combo->mustBeReadOnly() )
                combo->setEnabled( false );
            else
                combo->setEnabled( true );
        } else if ( ComboBoxXtring * combo = dynamic_cast<ComboBoxXtring *>( *it ) ) {
            if ( !enabled || combo->mustBeReadOnly() )
                combo->setEnabled( false );
            else
                combo->setEnabled( true );
        } else if ( ImageBox * img = dynamic_cast<ImageBox *>( *it ) ) {
            if ( !enabled || img->mustBeReadOnly() )
                img->setEnabled( false );
            else
                img->setEnabled( true );
        } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( *it ) ) {
            if ( !enabled || fnbox->mustBeReadOnly() )
                fnbox->getEditFileName()->setEnabled( false );
            else
                fnbox->getEditFileName()->setEnabled( true );
        }
    }
}


/**
 * @brief Enables or disables search boxes. Used to enable them while examining a record
 *
 * @param enabled ...
 * @return void
 **/
void FrmEditRec::enableSearchBoxes( bool enabled )
{
    for ( EditControlsList::const_iterator it = mEditControls.begin();
            it != mEditControls.end();
            ++ it ) {
        if ( SearchBox * search = dynamic_cast<SearchBox *>( *it ) ) {
            if ( !enabled || search->mustBeReadOnly() ) {
                search->getEditCode()->setReadOnly( true );
                search->getEditDesc()->setReadOnly( true );
                search->getButton()-> setEnabled (false);
            } else {
                search->getEditCode()->setReadOnly( true );
                search->getEditDesc()->setReadOnly( true );
                search->getButton()->setEnabled (true);
                search->getButton()->setFocusPolicy( Qt::TabFocus );
            }
        }
    }
}

/**
 * @brief Validates all the justEdited controls in the form.
 *
 * @param justedited If true, forces all controls to be validated by setting justEdited = true
 * @return bool
 **/
bool FrmEditRec::validateControls(bool justedited)
{
    bool isvalid;
    for ( EditControlsList::const_iterator it = mEditControls.begin();
            it != mEditControls.end();
            ++ it ) {
        if ( EditBox *edit = dynamic_cast<EditBox *>( *it ) ) {
            if( !justedited )
                edit->setJustEdited(true);
            if( edit->isJustEdited() )
                validate( *it, &isvalid );
        } else if ( TextBox *edit = dynamic_cast<TextBox *>( *it ) ) {
            if( !justedited )
                edit->setJustEdited(true);
            if( edit->isJustEdited() )
                validate( *it, &isvalid );
        } else if ( dynamic_cast<RichTextBox *>( *it ) ) {
            if( !justedited ) // TODO
                validate( *it, &isvalid );
        } else if ( SearchBox * search = dynamic_cast<SearchBox *>( *it ) ) {
            if( !justedited )
                search->getEditCode()->setJustEdited(true);
            if( search->getEditCode()->isJustEdited() )
                validate( search->getEditCode(), &isvalid );
        } else if ( CheckBox *cb = dynamic_cast<CheckBox *>( *it ) ) {
            if( !justedited )
                cb->setJustEdited( true );
            if( cb->isJustEdited() )
                validate( *it, &isvalid );
        } else if ( ComboBoxInt * combo = dynamic_cast<ComboBoxInt *>( *it ) ) {
            if( !justedited )
                combo->setJustEdited(true);
            if( combo->isJustEdited() )
                validate( *it, &isvalid );
        } else if ( ComboBoxXtring * combo = dynamic_cast<ComboBoxXtring *>( *it ) ) {
            if( !justedited )
                combo->setJustEdited(true);
            if( combo->isJustEdited() )
                validate( *it, &isvalid );
        } else if ( ImageBox *ib = dynamic_cast<ImageBox *>( *it ) ) {
            if( !justedited )
                ib->setJustEdited(true);
            if( ib->isJustEdited() )
                validate( *it, &isvalid );
        } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( *it ) ) {
            if( !justedited )
                fnbox->getEditFileName()->setJustEdited(true);
            if( fnbox->getEditFileName()->isJustEdited() )
                validate( fnbox->getEditFileName(), &isvalid );
        }
    }
    return isvalid;
}

bool FrmEditRec::removeControl( QWidget *control )
{
    for ( EditControlsList::iterator it = mEditControls.begin();
            it != mEditControls.end(); ++ it ) {
        if ( EditBox *edit = dynamic_cast<EditBox *>( *it ) ) {
            if( dynamic_cast<EditBox *>(control) && *it == control) {
                delete edit;
                mEditControls.erase( it );
                return true;
            }
        } else if ( TextBox * edit = dynamic_cast<TextBox *>( *it ) ) {
            if( dynamic_cast<TextBox *>(control) && *it == control) {
                delete edit;
                mEditControls.erase( it );
                return true;
            }
        } else if ( RichTextBox *edit = dynamic_cast<RichTextBox *>( *it ) ) {
            if( dynamic_cast<RichTextBox *>(control) && edit == control) {
                delete edit;
                mEditControls.erase( it );
                return true;
            }
        } else if ( SearchBox *search = dynamic_cast<SearchBox *>( *it ) ) {
            _GONG_DEBUG_PRINT(0, search->name() );
            if( dynamic_cast<SearchBox *>(control) && search == control) {
                delete search;
                mEditControls.erase( it );
                return true;
            }
        } else if ( CheckBox * check = dynamic_cast<CheckBox *>( *it ) ) {
            if( dynamic_cast<CheckBox *>(control) && *it == control) {
                delete check;
                mEditControls.erase( it );
                return true;
            }
        } else if ( ComboBoxInt * combo = dynamic_cast<ComboBoxInt *>( *it ) ) {
            if( dynamic_cast<ComboBoxInt*>(control) && *it == control) {
                delete combo;
                mEditControls.erase( it );
                return true;
            }
        } else if ( ComboBoxXtring * combo = dynamic_cast<ComboBoxXtring *>( *it ) ) {
            if( dynamic_cast<ComboBoxXtring*>(control) && *it == control) {
                delete combo;
                mEditControls.erase( it );
                return true;
            }
        } else if ( ImageBox *image = dynamic_cast<ImageBox *>( *it ) ) {
            if( dynamic_cast<ImageBox*>(control) && *it == control) {
                delete image;
                mEditControls.erase( it );
                return true;
            }
        } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( *it ) ) {
            if( dynamic_cast<FileNameBox *>(control) && *it == control) {
                delete fnbox;
                mEditControls.erase( it );
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Checks if the code and/or description supplied are unique for the record of this form
 *
 * @param sender The control thats sends the validate event
 * @param validresult a holder for the error messsage
 * @param editCode the control that contains the code
 * @param editDesc the control that contains the description
 * @param fldcodename the name of the code field
 * @param flddescname the name of the desc field
 * @param cond any extra condition for the records
 * @param codecanbenull If value is empty and codecanbenull, do not check dups
 * @return bool
 **/
bool FrmEditRec::validCodeAndDesc( QWidget *sender, ValidResult &validresult,
                                   LineEdit *editCode, LineEdit *editDesc,
                                   const Xtring &fldcodename, const Xtring &flddescname,
                                   const Xtring &cond, bool codecanbenull )
{
    _GONG_DEBUG_ASSERT( editCode );
    if( focusWidget() == pushAccept && sender )
        return true; // This avoids checking twice when pressing pushAccept while in one of the fields
    if( sender == editCode || !sender ) {
        // If value is empty and codecanbenull, do not check dups
        if( !(codecanbenull && editCode->toString().trim().isEmpty() ) ) {
            Xtring bigcond;
            if( editCode->getValueType() == Variant::tString )
                bigcond = fldcodename + "=" + DBAPP->getConnection()->toSQL( editCode->toString() );
            else
                bigcond = fldcodename + "=" + DBAPP->getConnection()->toSQL( editCode->toInt() );
            if( !cond.isEmpty() )
                bigcond = "(" + bigcond + ")AND(" + cond + ")";
            if( !getFormFilter().isEmpty() )
                bigcond = "(" + bigcond + ")AND(" + getFormFilter() + ")";
            if( dbRecordID another_id = getRecord()->existsAnother( bigcond ) ) {
                validresult.addError( Xtring::printf( _("Ya existe %s con el valor '%s' para '%s'"),
                                                      DBAPP->getTableDescSingular(pRecord->getTableName(), "una").c_str(),
                                                      editCode->toString().trim().c_str(),
                                                      getRecord()->findFieldDefinition( fldcodename )->getCaption().c_str()), fldcodename);
                DBAPP->setAnotherRecordID( another_id );
                return false;
            } else {
                DBAPP->setAnotherRecordID( 0 );
            }
        }
        if( editCode->toString().trim().isEmpty() && !codecanbenull ) {
            validresult.addError( Xtring::printf( _("'%s' no puede estar en blanco."),
                                                  getRecord()->findFieldDefinition(fldcodename)->getCaption().c_str()),
                                  fldcodename );
            return false;
        }
    }
    if( editDesc && (sender == editDesc || !sender ) ) {
        // editDesc puede estar vacío si solo queremos validar el código
        dbFieldDefinition *flddef = const_cast<dbFieldDefinition *>(getRecord()->findFieldDefinition(flddescname));
        if( flddef && flddef->isUnique() ) {
            Xtring bigcond = flddescname + "=" + DBAPP->getConnection()->toSQL( editDesc->toString() );
            if( !getFormFilter().isEmpty() )
                bigcond = "(" + bigcond + ")AND(" + getFormFilter() + ")";
            if( dbRecordID another_id = getRecord()->existsAnother( bigcond )  )
            {
                validresult.addError( Xtring::printf( _("Ya existe %s con el valor '%s' para '%s'"),
                                                      DBAPP->getTableDescSingular(pRecord->getTableName(), "una").c_str(),
                                                      editDesc->toString().c_str(),
                                                      getRecord()->findFieldDefinition( flddescname )->getCaption().c_str()), flddescname);
                DBAPP->setAnotherRecordID( another_id );
                return false;
            } else {
                DBAPP->setAnotherRecordID( 0 );
            }
        }
        if( flddef && !flddef->canBeNull() ) {
            if( editDesc->toString().trim().isEmpty()  ) {
                validresult.addError( Xtring::printf( _("'%s' no puede estar en blanco."),
                                                      getRecord()->findFieldDefinition(flddescname)->getCaption().c_str()),
                                      flddescname );
                return false;
            }
        }
    }
    return true;
}

void FrmEditRec::validateComboOrCheck( int activated )
{
    bool isvalid = true;
    CheckBox * check = const_cast<CheckBox *>( dynamic_cast<const CheckBox *>( sender() ) );
    if( check ) {
        if( !check->isCheckingProgrammatically() ) {
            check->setEdited( true );
            check->setJustEdited( true );
            validate( check, &isvalid );
        }
    } else {
        ComboBoxInt *comboint = dynamic_cast<ComboBoxInt *>( sender() );
        if( comboint ) {
            if( !comboint->isSettingProgrammatically() ) {
                comboint->setEdited( true );
                comboint->setJustEdited( true );
                validate( comboint, &isvalid );
            }
        } else {
            ComboBoxXtring *comboxtr = dynamic_cast<ComboBoxXtring *>( sender() );
            if( comboxtr ) {
                if( !comboxtr->isSettingProgrammatically() ) {
                    comboxtr->setEdited( true );
                    comboxtr->setJustEdited( true );
                    validate( comboxtr, &isvalid );
                }
            }
        }
    }
}

bool FrmEditRec::fieldHasChangedAfterSaving(const Xtring & fldname)
{
    return pRecord->getValue(fldname) != pOrigRecord->getValue(fldname);
}

bool FrmEditRec::validSeekCode(List< dbRecordID >& rids, QWidget* sender, bool* isvalid,
                               ValidResult& validresult, LineEdit* editCode,
                               LineEdit* editDesc, dbRecord* record,
                               const Xtring& fldcodename, const Xtring& flddescname,
                               const Xtring& cond, dbApplication::SeekCodeFlags flags,
                               LineEdit *editAlt, const Xtring &fldaltname )
{
    bool ret = validSeekCode( sender, isvalid, validresult, editCode, editDesc,
                              record, fldcodename, flddescname, cond,
                              static_cast<dbApplication::SeekCodeFlags> (flags | dbApplication::SeekCodeMultiple),
                              editAlt, fldaltname );
    if( ret )
        rids = DBAPP->getSeekCodeRecordIDs();
    return ret;
}

/**
 * @brief Validates a pair of code/desc fields. The desc is optional
 *
 * @param sender The control that is being validated
 * @param isvalid The result of the validation
 * @param validresult The messages of the validation
 * @param editCode The control that stores the code
 * @param editDesc The control that stores the description
 * @param record The record
 * @param fldcodename The name of the field that stores the code
 * @param flddescname The name of the field that stores the description
 * @param cond Any extra condition for the record
 * @param flags \sa dbApplication::SeekCodeFlags
 * @return bool
 **/
bool FrmEditRec::validSeekCode( QWidget * sender, bool *isvalid, ValidResult & validresult,
                                LineEdit * editCode, LineEdit * editDesc, dbRecord * record,
                                const Xtring & fldcodename, const Xtring & flddescname,
                                const Xtring &cond, dbApplication::SeekCodeFlags flags,
                                LineEdit *editAlt, const Xtring &fldaltname )
{
    bool ret = false;
    if( sender == editDesc && editDesc->isReadOnly() )
        return false;
    // First of all, check that the field has just been edited.
    if ( ( (!sender || sender == editCode) && editCode->isJustEdited() )
            || ( editDesc && sender == editDesc && editDesc->isJustEdited() ) ) {
        QWidget *fw = focusWidget(); // The next focus control, to return to it if everything is ok
        Xtring codigo_string = editCode->toString().trim();
        Xtring descripcion;
        if( editDesc )
            descripcion = editDesc->toString().trim();
        dbFieldDefinition *fldcode = DBAPP->getDatabase()->findFieldDefinition( record->getTableName(), fldcodename );
        // If the field has been cleared, clear the record
        if( !sender && (codigo_string.isEmpty() || codigo_string == "0") && descripcion.isEmpty() ) {
            record->clear( true ); // set default custom values
            ret = true;
        } else if ( (sender == editCode && (codigo_string.isEmpty() || codigo_string == "0") )
                    || ( editDesc && sender == editDesc && descripcion.isEmpty() ) ) {
            record->clear( true ); // set default custom values
            ret = true;
        } else {
            dbRecordID recid = 0;
            if ( sender == editCode || !sender) {
                if ( editCode->getValueType() == Variant::tInt ) {  // Son dos funciones distintas
                    int codigo_int = editCode->toInt();
                    recid = DBAPP->seekCode( record, this, fldcodename, codigo_int, flddescname,
                                             codigo_int == 0 ? descripcion : Xtring::null, cond, flags );
                } else
                    recid = DBAPP->seekCode( record, this, fldcodename, codigo_string, flddescname,
                                             codigo_string.isEmpty() ? descripcion : Xtring::null, cond, flags );
                if( recid )  {
                    ret = true;
                    if( fw )
                        fw->setFocus();
                } else {
                    validresult.addError( Xtring::printf( _("No se ha encontrado %1$s con %2$s \"%3$s\""),
                                                          DBAPP->getTableDescSingular(record->getTableName(), "la" ).c_str(),
                                                          fldcode ? fldcode->getCaption().c_str() : fldcodename.c_str(),
                                                          codigo_string.c_str() ), fldcodename );
                    if( !(flags & dbApplication::AllowNotFound) ) {
                        if( editDesc )
                            editDesc->setText("");
                        ret = false;
                        if( isvalid )
                            *isvalid = false;
                    } else {
                        if( fw )
                            fw->setFocus();
                    }
                }
            } else if ( sender == editDesc && editDesc ) {
                if ( ( recid = DBAPP->seekCode( record, this, fldcodename, Xtring(), flddescname,
                                                descripcion, cond, flags ) ) ) {
                    ret = true;
                    if( fw )
                        fw->setFocus();
                } else {
                    dbFieldDefinition *flddesc = DBAPP->getDatabase()->findFieldDefinition( record->getTableName(), flddescname );
                    validresult.addError( Xtring::printf( _("No se ha encontrado %1$s con %2$s \"%3$s\""),
                                                          DBAPP->getTableDescSingular(record->getTableName(), "la" ).c_str(),
                                                          flddesc ? flddesc->getCaption().c_str() : flddescname.c_str(),
                                                          descripcion.c_str() ), flddescname );
                    if( !(flags & dbApplication::AllowNotFound) ) {
                        ret = false;
                        if( isvalid )
                            *isvalid = false;
                    } else {
                        if( fw )
                            fw->setFocus();
                    }
                }
            }
        }
    }
    return ret;
}

void FrmEditRec::applyBasicStyle(LineEdit *edit, const Xtring &stylename )
{
    if( edit->getValueType() == Variant::tInt ) {
        edit->setAlignment(Qt::AlignRight);
    } else if( edit->getValueType() == Variant::tDouble ) {
        edit->setMask(GongLibraryInstance->getRegConfig()->getNumberMask());
        edit->setAlignment(Qt::AlignRight);
    } else if( edit->getValueType() == Variant::tMoney ) {
        edit->setMask(GongLibraryInstance->getRegConfig()->getCurrencyMask());
        edit->setAlignment(Qt::AlignRight);
    } else if( edit->getValueType() == Variant::tDate ) {
        edit->setMask(GongLibraryInstance->getRegConfig()->getDateMask());
        edit->setFormat(GongLibraryInstance->getRegConfig()->getDateFormat());
        edit->setAlignment(Qt::AlignLeft);
    } else if( edit->getValueType() == Variant::tDateTime ) {
        edit->setMask(GongLibraryInstance->getRegConfig()->getDateTimeMask());
        edit->setFormat(GongLibraryInstance->getRegConfig()->getDateTimeFormat());
        edit->setAlignment(Qt::AlignLeft);
    } else if( edit->getValueType() == Variant::tTime ) {
        edit->setMask(GongLibraryInstance->getRegConfig()->getTimeMask());
        edit->setFormat(GongLibraryInstance->getRegConfig()->getTimeFormat());
        edit->setAlignment(Qt::AlignLeft);
    }
    dbFieldStyle *styledef = DBAPP->getDatabase()->findFieldStyle ( stylename );
    if( styledef )
    {
        if( !styledef->getMask().isEmpty() )
            edit->setMask(styledef->getMask());
        if( !styledef->getFormat().isEmpty() )
            edit->setFormat(styledef->getFormat());
        if( styledef->getHAlignment() != dbFieldStyle::AlignAuto )
            edit->setAlignment(toGUI(styledef->getHAlignment()));
    }
    if( stylename == "PASSWORD" )
        edit->setEchoMode( QLineEdit::Password );
}


/**
  Establece el estilo de edición de los controles de edición
  */
void FrmEditRec::applyFieldStyle(LineEdit *edit, const dbFieldDefinition *fielddef)
{
    _GONG_DEBUG_ASSERT(  edit  );
    if( !fielddef )
        return;
    Variant::Type valuetype = fielddef->getVariantType();
    if( valuetype != Variant::tInvalid )
    {
        edit->setValueType(valuetype);
    }
    else
    {
        _GONG_DEBUG_WARNING("No existe el valuetype");
        edit->setValueType(Variant::tString);
    }
    applyBasicStyle(edit, fielddef->getStyle());
    unsigned int fldwidth = fielddef->getStyleWidth( DBAPP->getDatabase() );
    bool hasmaskorstyle = false;
    _GONG_DEBUG_PRINT( 4, Xtring::printf("%s=>size:%d, mask=%s", edit->name(), fldwidth, edit->getMask().c_str() ) );
    if( fldwidth < edit->getMask().size() ) {
        fldwidth = edit->getMask().size();
        hasmaskorstyle = true;
    }
    if( !hasmaskorstyle ) {
        hasmaskorstyle = true;
        switch( edit->getValueType() ) {
        case Variant::tDouble:
            if( fldwidth == 0 )
                fldwidth = GongLibraryInstance->getRegConfig()->getNumberMask().size();
            fldwidth += 1; // for the sign
            break;
        case Variant::tMoney:
            if( fldwidth == 0 )
                fldwidth = GongLibraryInstance->getRegConfig()->getCurrencyMask().size();
            fldwidth += 3; // for the currency symbol and sign
            break;
        case Variant::tInt:
            break;
        case Variant::tDate:
            if( fldwidth == 0 )
                fldwidth = GongLibraryInstance->getRegConfig()->getDateMask().size();
            break;
        case Variant::tTime:
            if( fldwidth == 0 )
                fldwidth = GongLibraryInstance->getRegConfig()->getTimeMask().size();
            break;
        case Variant::tDateTime:
            if( fldwidth == 0 )
                fldwidth = GongLibraryInstance->getRegConfig()->getDateTimeMask().size();
            break;
        default:
            if( fldwidth == 0 )
                fldwidth = 100;
            else
                edit->setMaxLength( fldwidth );
            hasmaskorstyle = false;
            break;
        }
    }
    // Esto está hecho a ojo, basándome en el formulario de contactos
    if( edit->getValueType() == Variant::tString ) {
        if( fldwidth <= 15 ) {
            edit->setMinimumWidth( fldwidth * edit->fontMetrics().width("W") );
            edit->setMaximumWidth( 15 * edit->fontMetrics().width("W") );
            edit->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
        } else if (fldwidth <= 25 ) {
            edit->setMinimumWidth( fldwidth * edit->fontMetrics().width("W") );
            edit->setMaximumWidth( 25 * edit->fontMetrics().width("W") );
            edit->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
        } else {
            edit->setMinimumWidth( (fldwidth / 3 > 25 ? 25 : fldwidth/3) * edit->fontMetrics().width("W") );
            edit->setMaximumWidth( (fldwidth > 150 ? 150 : fldwidth) * edit->fontMetrics().width("W") );
            edit->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
        }
    } else {
        edit->setMinimumWidth( (fldwidth) * edit->fontMetrics().width("9") );
        edit->setMaximumWidth( (fldwidth + 2) * edit->fontMetrics().width("9") );
        edit->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    }
    if( fielddef->getDescription() != fielddef->getCaption() ) {
        edit->setToolTip( toGUI(fielddef->getDescription()) );
    }
    _GONG_DEBUG_PRINT( 10, Xtring::printf("%s=>size:%d, mask=%s", edit->name(), fldwidth, edit->getMask().c_str() ) );
}

void FrmEditRec::applyFieldStyle(TextEdit *edit, const dbFieldDefinition *fielddef)
{
    _GONG_DEBUG_ASSERT(  edit  );
    if( !fielddef )
        return;
}

void FrmEditRec::applyFieldStyle(QLabel *label, const dbFieldDefinition *fielddef)
{
    if( fielddef ) {
        if( fielddef->getCaption() != "~" )
            label->setText( toGUI(fielddef->getCaption() ) );
    }
}

void FrmEditRec::applyFieldStyle(QPushButton *button, const dbFieldDefinition *fielddef)
{
    _GONG_DEBUG_ASSERT(  button  );
    if( fielddef ) {
        if( fielddef->getCaption() != "~" )
            button->setText( toGUI(fielddef->getCaption()) );
    }
}

void FrmEditRec::applyFieldStyle(QCheckBox *check, const dbFieldDefinition *fielddef)
{
    _GONG_DEBUG_ASSERT(  check  );
    if( fielddef ) {
        if( fielddef->getCaption() != "~" )
            check->setText(toGUI(fielddef->getCaption().c_str()));
    }
}

void FrmEditRec::applyFieldStyle(QComboBox *combo, const dbFieldDefinition *fielddef)
{
    _GONG_DEBUG_ASSERT( combo );
    if( fielddef ) {
        combo->setEnabled(!fielddef->isReadOnly());
        combo->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
    }
}

void FrmEditRec::fixFocusWidgetText()
{
    setEdited(false);
    if( !mFocusWidgetText.isEmpty() ) {
        if( LineEdit *le = dynamic_cast<LineEdit *>(pFocusWidget) ) {
            le->setText( mFocusWidgetText == "-" ? "-0" : mFocusWidgetText );
            setEdited(true);
            le->setJustEdited(true);
        } else if( TextEdit *te = dynamic_cast<TextEdit *>(pFocusWidget) ) {
            te->setText( mFocusWidgetText == "-" ? "-0" : mFocusWidgetText );
            setEdited(true);
            te->setJustEdited(true);
            //move the cursor to the end of the text
            QTextCursor cursor(te->textCursor());
            cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
            te->setTextCursor(cursor);
        }
        mFocusWidgetText = "";
    }
}

void FrmEditRec::addSpacersToTabs()
{
    for( int index = 0; index < pTabWidget->count(); ++index ) {
        QWidget *tab = pTabWidget->widget( index );
        if( tab->layout() ) {
            // Find a control with vertical expanding
            bool found = false;
            QList<QWidget *>allwidgets = tab->findChildren<QWidget *>();
            for( QList<QWidget *>::const_iterator wit = allwidgets.begin();
                    wit != allwidgets.end(); ++wit ) {
                QSizePolicy sp = (*wit)->sizePolicy();
                if( sp.verticalPolicy() == QSizePolicy::Maximum
                        || sp.verticalPolicy() == QSizePolicy::Expanding
                        || sp.verticalPolicy() == QSizePolicy::MinimumExpanding ) {
                    found = true;
                    break;
                }
            }
            if( !found ) {
                QSpacerItem *vertSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
                tab->layout()->addItem(vertSpacer);
            }
        }
    }
}

void FrmEditRec::gather()
{
    gatherFields();
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end();
            ++ bit ) {
        (*bit)->gatherFields();
    }
}

void FrmEditRec::scatter()
{
    QTabBar *tb = pTabWidget->findChild<QTabBar *>();
    if( tb && tb->isVisible() )
        tb->setCurrentIndex( 0 );
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end();
            ++ bit ) {
        (*bit)->scatterFields( BEHAVIOR_PRE );
    }
    scatterFields();
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end();
            ++ bit ) {
        (*bit)->scatterFields( BEHAVIOR_POST );
    }
}

void FrmEditRec::validate(QWidget* sender, bool* isvalid, ValidResult* ir)
{
    // If ir == 0, we want to show the messages, create a temporary validresult to hold
    // al the messages.
    ValidResult *validresult = ( ir ? ir : new ValidResult() );
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end();
            ++ bit ) {
        (*bit)->validateFields( BEHAVIOR_PRE, sender, isvalid, validresult);
    }
    validateFields(sender, isvalid, validresult);
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end();
            ++ bit ) {
        (*bit)->validateFields( BEHAVIOR_POST, sender, isvalid, validresult);
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

bool FrmEditRec::canBeginEdit(EditMode newmode)
{
    DBAPP->waitCursor();
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end(); ++ bit ) {
        if( !(*bit)->canBeginEdit( newmode ) ) {
            DBAPP->resetCursor();
            return false;
        }
    }
    DBAPP->resetCursor();
    return true;
}

} // namespace gong

#include <cerrno>
#include <config.h>

#include <QMenu>
#include <QMenuBar>
#include <QFocusEvent>
#include <QMdiSubWindow>
#include <gonggettext.h>
#include <gongdbrecorddatamodel.h>
#include <gongcsvutils.h>
#include <gongdbrecordpermissions.h>
#include <gongfileutils.h>
#include "dbappmainwindow.h"
#include "dbappdbapplication.h"
#include "dbappfrmsqlselect.h"
#include "dbappfrmviewedit.h"
#include "dbappfrmeditrecmaster.h"
#include "dbappfrmeditrecdetail.h"
#include "dbappfrmcustom.h"
#include "dbappfrmmultiupdate.h"
#include "dbappfrmeditrecbehavior.h"
#include "dbappfrmadvancedprint.h"
#include "dbappfrmcsvparams.h"
#include "../gonglib/gongxtring.h"

#ifdef HAVE_RTKMODULE
#include <dbappreport.h>
#endif

namespace gong {

FrmEditRecMaster::FrmEditRecMaster( FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm,
                                    EditMode editmode, dbApplication::EditFlags editflags,
                                    QWidget *parent, const char* name, WidgetFlags fl )
    : FrmEditRec( parentfrm, maestro, editmode, editflags, parent, name, fl ),
      pMenuRecordAdd( 0 ), pMenuRecordDelete( 0 ), pMenuRecordModify( 0 ), pMenuRecordDup( 0 ),
      pMenuRecordView( 0 ), pMenuRecordPrint( 0 ), pMenuRecordReopen( 0 ), pMenuRecordInfo( 0 ),
      pMenuTableFilter( 0 ), pMenuTableRemoveFilter( 0 ), pMenuTablePrint(0),
      pMenuTableImport(0), pMenuTableExport( 0 ), pMenuTableSelectAll( 0 ), pMenuTableUpdate(0),
      pMenuView( 0 ), pMenuViewReorder( 0 ), pMenuViewAdd( 0 ), pMenuViewModify( 0 ), pMenuViewDup( 0 ), pMenuViewDelete( 0 ),
      pDataTable( 0 ), mBrowsing( false ), mChoosing( false ), mWasBrowsing( false ),
      mMustRead( false ), mMustRefresh( false ), mRefreshing( false ), mMultipleDeleting(false),
      pDataModel( dm )
{
    _GONG_DEBUG_ASSERT( maestro );
    if ( !name )
        setName(( "FrmEditRec" + maestro->getTableName() ).c_str() );

    _GONG_DEBUG_PRINT( 1, Xtring::printf( "Creating FrmEditRecMaster %s...", this->name() ) );

    // DataTable
    if ( !( mEditFlags & dbApplication::simpleEdition ) ) {
        pDataTable = new DataTable( DBAPP->getDatabase(), this, "pDataTable" );
        pDataTable->hide();
        if ( mEditFlags & dbApplication::tableEditEvent )
            pDataTable->setTableEditMode( DataTable::Inline );
        else
            pDataTable->setTableEditMode( DataTable::None );
        pMainLayout->insertWidget( 1, pDataTable );
        connect( pDataTable, SIGNAL( currentChanged( int, int ) ), this, SLOT( tableCurrentChanged( int, int ) ) );
        connect( pDataTable, SIGNAL( beginEditSignal( DataTable *, DataTable::EditMode, const Xtring& ) ),
                 this, SLOT( slotBeginEdit( DataTable *, DataTable::EditMode, const Xtring& ) ) );
        setTabOrder( pDataTable, pushAccept );
        setTabOrder( pushAccept, pushCancel );
    }
    pushMiddle = new QPushButton( this, "pushMiddle" );
    connect( pushMiddle, SIGNAL( clicked() ), this, SLOT( pushMiddle_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushMiddle );
    pushMiddle->setVisible( false );

    pushRemoveFilter = new QPushButton( this, "pushRemoveFilter" );
    pushRemoveFilter->setIcon( QIcon::fromTheme("edit-clear", QIcon(":/edit-clear.png")) );
    pushRemoveFilter->setText( toGUI( _("Quitar filtro") ) );
    connect( pushRemoveFilter, SIGNAL( clicked() ), this, SLOT( menuTableRemoveFilter_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushRemoveFilter );
    pushRemoveFilter->setVisible( false );
}

/*
 *  Destroys the object and frees any allocated resources
 */
FrmEditRecMaster::~FrmEditRecMaster()
{
    if (( mEditFlags & dbApplication::deleteDataModel ) && pDataModel )
        delete pDataModel;
}

/**
 * @brief Reads the form record with the passed id. If id == 0, reads the current row record
 *
 * @param id ...
 * @return bool
 **/
bool FrmEditRecMaster::read(dbRecordID id)
{
    if( id == 0 )
        id = getTableRecordID();
    if( FrmEditRec::read(id) ) {
        mMustRead = false;
    } else
        mMustRead = true;
    return !mMustRead;
}

/**
 * @brief Called when a detail is saved or moved from a FrmEditRecDetail form
 *
 * @param frmdetail ...
 * @return void
 **/
void FrmEditRecMaster::updateFromDetails( class FrmEditRecDetail *frmdetail )
{
    for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
            bit != mBehaviors.end();
            ++ bit ) {
        (*bit)->updateFromDetails(frmdetail);
    }
}

Xtring FrmEditRecMaster::getWholeFilter() const
{
    Xtring wf = getFormFilter();
    _GONG_DEBUG_PRINT( 4, "formfilter: " + wf );
    // The datamodel filter does not contain the record filter, as set in pDataTable->setDataModel()
    wf = pRecord->getFilter( "", wf );
    _GONG_DEBUG_PRINT( 4, "+ pRecord->getFilter: " + wf );
    wf = pDataModel->getFilter("", wf );
    _GONG_DEBUG_PRINT( 4, "+ pDataModel->getFilter: " + wf );
    return wf;
}


Xtring FrmEditRecMaster::getDataModelFilter() const
{
    if( pDataModel )
        return pDataModel->getFilter( "" );
    else
        return Xtring::null;
}

/**
 * Sets or resets a filter for the current view, preserving the base form filter
 * @param filter
 */
void FrmEditRecMaster::applyFilter( const Xtring &filter )
{
    _GONG_DEBUG_ASSERT( pDataTable );
    if( !pDataModel )
        return;
    DBAPP->waitCursor( true );
    Xtring f;
    if ( !mFormFilter.isEmpty() ) {
        if( !mFormFilter.startsWith("(") )
            f += "(" + mFormFilter + ")";
        else
            f += mFormFilter;
    }
    if ( !filter.isEmpty() ) {
        if ( !f.isEmpty() )
            f += "AND";
        if( !filter.startsWith("(") )
            f += "(" + filter + ")";
        else
            f += filter;
    }
    pDataModel->setFilter( f );
    try {
        setView( pDataModel->getCurrentViewIndex(), true ); // force
    } catch ( dbError e ) {
        _GONG_DEBUG_WARNING( e.what() );
        msgError( this, e.what() );
    }
    updateStatus();
    DBAPP->resetCursor();
}


/**
 * @brief Sets the datamodel of the given datatable. Catches any error.
 *
 * @param datatable ...
 * @param dm If null, tries to either find views from the config or to create a default view
 * @return void
 **/
void FrmEditRecMaster::setTableDataModel()
{
    _GONG_DEBUG_ASSERT( pDataTable );
    if ( pDataModel && pDataModel->getViewCount() ) {
        try {
            pDataModel->addFilter( mFormFilter ); // TODO: Adding this filter to the datamodel makes it redundant later
            pDataTable->setDataModel( pDataModel );
            return;
        } catch ( dbError &e ) {
            if( !DBAPP->manageDbError( getTitle(), e) )
                throw;
        } catch ( std::runtime_error &e ) {
            _GONG_DEBUG_WARNING( e.what() );
            FrmBase::msgError( this, Xtring::printf( _( "Error en la vista proporcionada al formulario %s:%s" ),
                               name(), Xtring( e.what() ).c_str() ) );
        }
    }

    dbViewDefinitionDict views;
    if ( DBAPP->getDatabase()->getViewsForTable( getRecord()->getTableName(), views ) ) {
        try {
            if ( !pDataModel ) {
                pDataModel = new dbRecordDataModel( getRecord(), views, mFormFilter );
                mEditFlags = dbApplication::EditFlags( mEditFlags | dbApplication::deleteDataModel );
            }
            pDataModel->setViews( views );
            pDataTable->setDataModel( pDataModel );
            return;
        } catch ( dbError &e ) {
            if( DBAPP->manageDbError( getTitle(), e) )
                return;
            FrmBase::msgError( this,
                               Xtring::printf( _( "Error en la vista definida para esta tabla:\n%s\n\nSe creará una vista por defecto con todos los campos." ),
                                               e.what() ) );
        }
    }
    try {
        _GONG_DEBUG_WARNING( "No se han encontrado vistas para la tabla " +
                             getRecord()->getTableDefinition()->getDescSingular() );
        views.clear();
        dbViewDefinition *v = new dbViewDefinition( *getRecord()->getTableDefinition() );
        v->setCaption( v->getDescPlural() + _(" por defecto") );
        if( v->getOrderBy().isEmpty() ) {
            for( uint i = 0; i < v->getFieldCount(); ++ i ) {
                if( v->getFieldDefinition(i)->isCode() ) {
                    v->setOrderBy( Xtring::number(i) );
                    break;
                }
            }
        }
        views.insert( getRecord()->getTableName() + "_default", v );
        if ( !pDataModel ) {
            pDataModel = new dbRecordDataModel( getRecord(), views, mFormFilter );
            mEditFlags = dbApplication::EditFlags( mEditFlags | dbApplication::deleteDataModel );
        }
        pDataModel->setViews( views );
        pDataTable->setDataModel( pDataModel );
        return;
    } catch ( dbError &e ) {
        if( DBAPP->manageDbError( getTitle(), e) )
            return;
        FrmBase::msgError( this, Xtring::printf(
                               _( "Imposible generar la vista por defecto de la tabla %s:\n" ),
                               getRecord()->getTableDefinition()->getDescSingular().c_str() )
                           + Xtring( e.what() ) );
    }
    close();
}

/**
*/
/**
 * @brief Inicializa los menus y otros elementos del formulario una vez que se ha mostrado la primera vez
 * No llama a la función FrmEditRec::justShown()
 * 
 * @param firsttime ...
 * @return void
 */
void FrmEditRecMaster::justShown( bool firsttime )
{
    DBAPP->waitCursor(true);
    if ( firsttime || mEditStatus == never_shown ) {
        setUpdatesEnabled(false);
        try {
            initMenus();
            for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
                    bit != mBehaviors.end();
                    ++ bit ) {
                (*bit)->initGUI();
            }
            addSpacersToTabs();
            if( pDataTable ) { // This can be an embedded form without datatable
                setTableDataModel();
                updateViewMenus(true);
            }
            if ( mEditFlags & dbApplication::simpleEdition ) {
                mEditStatus = not_saved;
                mEdited = false;
                scatter();
                updateFromDetails( 0 ); // to update values from details if there are details
                fixFocusWidgetText();
            } else {
                if ( mEditMode == DataTable::browsing || ( mChoosing = ( mEditMode == DataTable::choosing ) ) ) {
                    mMustRead = true;
                    pFocusWidget = pDataTable;
                    ensureBrowseView();
                    restoreBrowseStatus();
                } else {
                    if ( pDataModel->getRowCount() == 0 )
                        mEditMode = DataTable::inserting;
                    ensureEditView();
                    if( !isReadOnly() )
                        validateControls( true ); // only just edited ones
                }
                if ( !isInserting() )
                    pDataTable->sync( pRecord->getRecordID() );
                mEditStatus = not_saved;
            }
        } catch( ... ) {
            setUpdatesEnabled( true );
            DBAPP->resetCursor();
            throw;
        }
        setUpdatesEnabled( true );
        if( mBrowsing == true ) {
            // ensure cell visible here once updates have been enabled
            pDataTable->scrollToCurrent();
        }
        setInitialFocus();
    }
    updateStatus();
    DBAPP->resetCursor();
}

/**
  Reimplementada de FrmEditRec
  TODO Información, si es nuevo, poner simplemente que es nuevo
  */
/**
 * @brief Updates the form controls based on the internal state of the record
 *
 * Calls the parent updateStatus
 *
 * @return void
 **/
void FrmEditRecMaster::updateStatus( bool callbehaviors )
{
    if ( mEditStatus == never_shown )
        return;
    Xtring aceptar_text;
    if ( mEditFlags & dbApplication::simpleEdition ) {
        FrmEditRec::updateStatus( false );
        pMenuRecordAdd->setEnabled( false );
        pMenuRecordDup->setEnabled( false );
        pMenuRecordDelete->setEnabled( false );
        pMenuRecordModify->setEnabled( false );
        pMenuRecordView->setEnabled( false );
        pMenuRecordPrint->setEnabled( false );
        pMenuViewReorder->setEnabled( false );
        pMenuTableFilter->setEnabled( false );
        pMenuTableRemoveFilter->setEnabled( false );
        pMenuTableExport->setEnabled( false );
        pMenuTableSelectAll->setEnabled( false );
        pMenuTable->setEnabled( false );
        pMenuView->setEnabled( false );
    } else {
        pushRemoveFilter->setVisible( false );
        dbRecordPermissions r = getPermissions();
        if (( mEditMode == DataTable::browsing || mEditMode == DataTable::choosing ) ) {
            pDataTable->setReadOnly( isReadOnly() );
            Xtring title;
            if( isReadOnly() )
                title += "(R) ";
            if ( mEditMode == DataTable::browsing )
                title = _( "Hojeando " ) + Xtring::number( pDataTable->numRows() ) + " ";
            else {
                title += _( "Eligiendo " ) + Xtring::number( pDataTable->numRows() ) + " ";
            }
            if ( pDataModel->getCurrentView() )
                title += pDataModel->getCurrentView()->getCaption();
            else {
                title += getRecord()->getTableDefinition()->getDescPlural();
            }
            if ( !pDataModel->getFilter("").isEmpty() ) {
                title += _( " [Filtrado]" );
                pushRemoveFilter->setVisible( true );
            }
            setTitle( title );
            if ( mChoosing && mEditMode == DataTable::choosing ) {
                if ( pDataModel->getRowCount() == 0 && r.canAdd )
                    aceptar_text = _( "&Añadir" );
                else
                    aceptar_text = _( "&Elegir" );
                pushCancel->setText( toGUI( _( "&Cerrar" ) ) );
            } else if ( mBrowsing && mEditMode == DataTable::browsing ) {
                pushCancel->setText( toGUI( _( "&Cerrar" ) ) );
                if ( r.canEdit ) {
                    aceptar_text = _( "&Modificar" );
                } else if ( r.canAdd ) {
                    aceptar_text = _( "&Añadir" );
                } else if ( r.canView ) {
                    aceptar_text = _( "&Examinar" );
                }
            }
            pMenuView->setEnabled( true );
            pMenuTable->setEnabled( true );
            pMenuViewReorder->setEnabled( true );
            pMenuTableFilter->setEnabled( true );
            pMenuTableRemoveFilter->setEnabled( true );
            pMenuTableExport->setEnabled( true );
            pMenuTableSelectAll->setEnabled( true );
        } else {
            r.canAdd = r.canEdit = r.canDelete = r.canView = false;
            FrmEditRec::updateStatus( false );
            if ( mEditMode == DataTable::selecting )
                aceptar_text = _( "&Cerrar" );
            else
                aceptar_text = _( "&Grabar" );
            pushCancel->setText( toGUI( _( "&Cancelar" ) ) );
            pMenuViewReorder->setEnabled( false );
            pMenuTableFilter->setEnabled( false );
            pMenuTableRemoveFilter->setEnabled( false );
            pMenuTableExport->setEnabled( false );
            pMenuTableSelectAll->setEnabled( false );
            pMenuView->setEnabled( false );
        }
        pMenuRecordAdd->setEnabled( r.canAdd );
        pMenuRecordDup->setEnabled( r.canAdd && r.canEdit );
        pMenuRecordDelete->setEnabled( r.canDelete );
        pMenuRecordModify->setEnabled( r.canEdit );
        pMenuRecordView->setEnabled( r.canView );
        pMenuRecordPrint->setEnabled( r.canView && isBrowsing() );
        pMenuViewReorder->setEnabled( isBrowsing() || isChoosing() );
        pMenuTableImport->setEnabled( r.canAdd );
        if (( mEditFlags & dbApplication::embedded ) && isBrowsing() ) {
            pushCancel->hide();
        } else {
            pushAccept->show();
            pushCancel->show();
        }
        pushAccept->setText( toGUI( aceptar_text + " " + pRecord->getTableDefinition()->getDescSingular() ) );
        pushMiddle->setVisible( false );
        if ( mEditMode == DataTable::choosing ) {
            if ( pMenuRecordModify->isEnabled() && DBAPP->codeNotFound().isEmpty() ) {
                pushMiddle->setVisible( true );
                pushMiddle->setText( toGUI( _( "&Modificar" ) ) );
            } else if ( pMenuRecordAdd->isEnabled() && aceptar_text != "&Añadir") {
                pushMiddle->setVisible( true );
                pushMiddle->setText( toGUI( _( "&Añadir" ) ) );
            }
        } else if ( mEditMode == DataTable::choosing && pMenuRecordAdd->isEnabled()  ) {
            pushMiddle->setVisible( true );
            pushMiddle->setText( toGUI( _( "&Añadir" ) ) );
//         } else if ( mEditMode == DataTable::browsing && pMenuRecordAdd->isEnabled() && fromGUI(pushAccept->text()) != _( "&Añadir" ) ) {
// 			pushMiddle->setVisible( true );
// 			pushMiddle->setText( toGUI( _( "&Añadir" ) ) );
		}
    }
    if( callbehaviors ) {
        for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
                bit != mBehaviors.end();
                ++ bit ) {
            (*bit)->updateStatus();
        }
    }
}


dbRecordPermissions FrmEditRecMaster::getPermissions() const
{
    dbRecordPermissions r = DBAPP->getRecordPermissions( getRecord(), getRecord()->getUser() );
    if (( mEditFlags & dbApplication::simpleEdition ) ) {
        r.setAll( true );
        return r;
    } else if (( mEditStatus != saved && mEditMode != DataTable::browsing
                 && mEditMode != DataTable::choosing && mEditMode != DataTable::deleting ) ) {
        // deleting .- if continuous and we have cancelled the last one
        r.setAll( false );
        return r;
    }
    if ( pDataTable->currentRow() < 0
            || pDataTable->currentRow() >= ( int ) pDataModel->getRowCount() ) {
        r.canMoveFirst = ( false );
        r.canMoveLast = ( false );
        r.canMoveNext = ( false );
        r.canMovePrevious = ( false );
        /// \todo {check} No hace falta realmente
        r.canReorder = ( false );
        // Si esta filtrado, permitir que se elimine el filtro
        if ( pDataModel->getFilter("").isEmpty() )
            r.canFilter = ( false );
        r.canPrint = ( false );
        r.canFind = ( false );
        r.canEdit = ( false );
        r.canDelete = ( false );
    } else {
        if ( pDataTable->currentRow() == 0 ) {
            r.canMovePrevious = ( false );
            r.canMoveFirst = ( false );
        }
        if (( int )( pDataTable->getDataModel()->getRowCount() - 1 ) == pDataTable->currentRow() ) {
            r.canMoveNext = ( false );
            r.canMoveLast = ( false );
        }
    }
    if ( (mEditFlags & dbApplication::onlyBrowse) || pDataTable->getDataModel()->getRowCount() == 0 ) {
        r.canPrint = false;
        r.canFind = false;
        r.canEdit = false;
        r.canDelete = false;
        r.canView = false;
    }
    if ( ( DBAPP->isReadOnly() || (mEditFlags & dbApplication::readOnly) )
            && !getRecord()->getConnection()->isRootUser() ) {
        r.canEdit = false;
        r.canDelete = false;
        r.canAdd = false;
    }
    return r;
}


void FrmEditRecMaster::tableCurrentChanged( int /*row*/, int /*col*/ )
{
    mMustRead = true;
    if ( !mRefreshing ) {
        mLastID = getTableRecordID();
        updateStatus();
    }
}

/**
  Reimplementado de FrmEditRec.
  Accept se llama al pulsar el botón push_aceptar, pero si estamos en browse puede ser añadir, modificar o ver y en choose puede ser elegir
  */
void FrmEditRecMaster::accept()
{
    if ( mChoosing && ( mEditMode == DataTable::choosing ) ) {
        if ( pDataTable->numRows() > 0 ) {
            // Si estamos eligiendo, leer el registro seleccionado si hace falta y cerrar
            if ( mMustRead )
                read( getTableRecordID() );
            mEditStatus = saved;
            FrmBase::accept();
            return;
        } else {
            beginEdit( pDataTable, DataTable::inserting );
        }
    } else if ( mBrowsing && mEditMode == DataTable::browsing ) {
        beginEdit( pDataTable, DataTable::defaulteditmode );
    } else {
        DBAPP->waitCursor( true );
        try {
            bool isvalid = true;
            mEditStatus = not_saved;
            switch ( mEditMode ) {
            case DataTable::inserting:
            case DataTable::duplicating:
            case DataTable::updating:
                gather();
                validate( NULL, &isvalid );
                if ( isvalid ) {
                    // Disable the form so that if it triggers other forms while saving,
                    // the user can not interact with this
                    setEnabled( false );
                    if ( save() ) {
                        mEditStatus = saved;
                        mMustRefresh = true;
                        mLastID = getRecord()->getRecordID();
                    }
                    setEnabled( true );
                } else {
                    DBAPP->resetCursor();
                    return;
                }
                break;
            case DataTable::deleting: {
                if ( pDataTable ) { // Puede ser un formulario embebido clonado
                    int currow = pDataTable->currentRow();
                    // En una vista con maestro/detalles, el ID puede estar repetido. Moverse al siguiente o anterior
                    dbRecordID nextid = pDataTable->getDataModel()->getRowID( currow );
                    while ( currow < pDataTable->numRows() && nextid == mLastID )
                        nextid = pDataTable->getDataModel()->getRowID( currow++ );
                    if ( nextid == mLastID ) {
                        while ( currow > 0 && nextid == mLastID )
                            nextid = pDataTable->getDataModel()->getRowID( --currow );
                    }
                    if ( remove() ) {
                        mLastID = nextid;
                        mEditStatus = saved;
                        mMustRefresh = true;
                    }
                } else {
                    remove();
                    mEditStatus = saved;
                    mMustRefresh = true;
                }
            }
            break;
            case DataTable::selecting:
                mEditStatus = saved;
                break;
            default:
                throw std::exception();
                break;
            }
            setEdited( false );
            if ( mEditStatus == saved ) {
                DBAPP->setAnotherRecordID( 0 );
                mMustRead = false;
                if ( !( mEditFlags & dbApplication::simpleEdition ) ) {
                    if ( mChoosing == true ) {
                        // Después de grabar, si estábamos eligiendo, no tenemos que seguir añadiendo continuamente
                        mEditMode = DataTable::choosing;
                    }
                    if (( mEditFlags & dbApplication::editContinuous ) && isInserting() && !isDuplicating() ) {
                        mBrowsing = mWasBrowsing;
                        beginEdit( pDataTable, DataTable::inserting );
                    } else if( ( mEditFlags & dbApplication::editContinuous ) && isDeleting() ) {
                    } else {
                        if ( mWasBrowsing )
                            ensureBrowseView();
                        else
                            ensureEditView();
                    }
                } else if (( mEditFlags & dbApplication::simpleEdition ) && mEditStatus == saved && ( mEditFlags & dbApplication::editContinuous ) ) {
                    pRecord->setNew( true );
                    pRecord->clear( true ); // set custom default values
					mIsFirstScatter = true;
                    scatter();
                    fixFocusWidgetText();
                    setInitialFocus();
                } else if (( mEditFlags & dbApplication::simpleEdition ) && mEditStatus == saved ) {
                    FrmBase::accept();
                    DBAPP->resetCursor();
                    return; // Skip updatestatus on closed form
                }
            }
        } catch ( std::overflow_error &err ) {
            setEnabled( true );
            msgError( this, _("Una cantidad ha sobrepasado el límite aceptable para este ordenador") );
        } catch ( std::runtime_error &err ) {
            setEnabled( true );
            msgError( this, err.what() );
        } catch ( std::exception &e ) {
            setEnabled( true );
            _GONG_DEBUG_WARNING( e.what() );
            msgError( this, e.what() );
        }
        DBAPP->resetCursor();
    }
    updateStatus();
}

/**
  Reimplementado de FrmEditRec. No llama a FrmEditRec::canClose()
  */
bool FrmEditRecMaster::canClose()
{
    bool willclose = false;
    try {
        if ( !isEnabled() ) // Estamos debajo de un formulario de elegir
            return false;
        for( FrmEditRecBehaviorsList::const_iterator bit = mBehaviors.begin();
                bit != mBehaviors.end();
                ++ bit ) {
            if( !(*bit)->canClose() )
                return false;
        }
        if (( mBrowsing && mEditMode == DataTable::browsing ) || ( mChoosing && mEditMode == DataTable::choosing ) ) {
            // browsing or choosing always closes
            if( wasCancelled() ) // FIXME does not work
                pDataTable->getSelectedIDs().clear();
            willclose = true;
        } else if ( !isEditing() || isSaved() ) {
            if ( !( mEditFlags & dbApplication::simpleEdition ) && !wasCancelled() ) {
                if ( pDataModel->getRowCount() != 0 ) {
                    if ( mMustRead )
                        read( getTableRecordID() );
                } else {
                    getRecord()->setNew();
                }
            }
            willclose = true;
        } else if ( !edited() || msgYesNo( this,
                                           Xtring::printf( _( "¿Estás segura de cancelar la edición de %s?\n\n" ),
                                                   DBAPP->getTableDescSingular( pRecord->getTableName(), "esta" ).c_str() ),
                                           false ) ) {
            mMustRead = true;
            setEdited( false );
            willclose = true;
            mEditFlags = static_cast<dbApplication::EditFlags>( mEditFlags & !dbApplication::editContinuous);
        }
        if ( !( mEditFlags & dbApplication::simpleEdition ) && willclose ) {
            // If the form was cancelled after raising an existing record error, try to sync to the existing record
            if( DBAPP->getAnotherRecordID() ) {
                mLastID = DBAPP->getAnotherRecordID();
                mMustRefresh = true;
                DBAPP->setAnotherRecordID( 0 );
            }
            if( pDataTable && !( mEditMode == DataTable::browsing || mEditMode == DataTable::choosing ) ) {
                pFocusWidget = 0; // Para que se calcule el focuswidget otra vez
                ensureBrowseView();
                if ( theGuiApp->getMainWindow() ) // Unless the main window is closing, we dont want to close the window
                    if ( !theGuiApp->getMainWindow()->amIClosing() )
                        willclose = false;
            }
        }
    } catch ( std::runtime_error e ) {
        _GONG_DEBUG_WARNING( e.what() );
        willclose = FrmBase::msgYesNo( this, Xtring( e.what() ) + "\n\n¿Quieres cerrar este formulario?" );
    }
    if ( willclose && ( mEditFlags & dbApplication::embedded ) ) { // Si está embebido, cerrar el padre
        QObject *padre = parent();
        while ( padre ) {
            if ( FrmEditRec *editrec = dynamic_cast<FrmEditRec *>( padre ) ) {
                willclose = editrec->canClose();
                break;
            }
            padre = padre->parent();
        }
    }
    if( willclose )
        saveBrowseStatus();
    return willclose;
}

void FrmEditRecMaster::ensureEditView()
{
    if ( !pTabWidget->isVisible() ) {
        // Si estos show/hide se hacen al revés, cuando la ventana está maximizada se va al fondo
        pDataTable->hide();
        pTabWidget->show(); // Esto puede lanzar eventos lostfocus si se hace antes del scatter
        pFocusWidget = 0;
        if ( mEditFlags & dbApplication::embedded )
            pushAccept->setFocusPolicy( Qt::StrongFocus );
// 		DBAPP->postEvent( this->parent(), new QResizeEvent( size() + QSize(100,100), size() ) );
// 		adjustSize();
    }
    if ( mMustRead ) {
        if ( pDataTable->currentRow() != -1 && pDataTable->currentRow() < ( int ) pDataModel->getRowCount() ) {
            read( getTableRecordID() );
			mIsFirstScatter = true;
        }
        mMustRead = false;
    }
    scatter();
    fixFocusWidgetText();
    setInitialFocus();
    setEdited( false );
    mBrowsing = false;
    updateStatus();
}

void FrmEditRecMaster::ensureBrowseView()
{
    if (( isVisible() && pTabWidget->isVisible() ) || !mBrowsing ) {
        mEditMode = mChoosing ? DataTable::choosing : DataTable::browsing;
        pTabWidget->setVisible( false );
        pDataTable->setVisible( true );
        mBrowsing = true;
        if ( mEditStatus != never_shown ) {
            setWiseFocus(pDataTable);
            updateStatus();
        }
        if ( mEditFlags & dbApplication::embedded )
            pushAccept->setFocusPolicy( Qt::ClickFocus );
    }
    if( mMustRefresh )
        refresh();
    else
        pDataTable->scrollToCurrent();
}

void FrmEditRecMaster::refresh()
{
    if ( !pDataTable || !pDataTable->getDataModel() )
        return;
    if ( mRefreshing || (!mBrowsing & !mChoosing) )
        return;
    mRefreshing = true;
    dbRecordID actid = mLastID ? mLastID : getTableRecordID();
    setWiseFocus(pDataTable);
    setView( pDataTable->getDataModel()->getCurrentViewIndex(), true );
    if( actid != mLastID  )
        pDataTable->sync( actid );
    mRefreshing = false;
    mMustRefresh = false;
    updateStatus();
}


/**
 * @brief Sets the view of this form
 *
 * It is set from:
 *      * in SetDataModel
 * 		* The menu views
 *      * in menuTableReorder_clicked
 *      * Pressing Apply from FrmViewEdit
 *      * in Refresh
 *
 * @param newviewindex Index of the view to set
 * @param force Set it even if the index is the same
 * @return bool
 **/
bool FrmEditRecMaster::setView( int newviewindex, bool force )
{
    bool ret = true;
    if ( newviewindex >= ( int ) pDataModel->getViewCount() )
        newviewindex = 0;
    if ( force || newviewindex != pDataModel->getCurrentViewIndex() ) {
        DBAPP->waitCursor( true );
        try {
            if( mLastID == 0 )
                mLastID = getTableRecordID();
            pDataTable->setView( newviewindex );
            if( mLastID != 0 )
                pDataTable->sync( mLastID );
            updateViewMenus( false );
        } catch ( std::runtime_error e ) {
            ret = false;
            FrmBase::msgOk( this, e.what() );
        }
        DBAPP->resetCursor();
    }
    return ret;
}

dbRecordID FrmEditRecMaster::getTableRecordID() const
{
    return pDataModel->getRowID( pDataTable->currentRow() );
}

void FrmEditRecMaster::initMenus()
{
    QString text = toGUI( _( "&Añadir" ) );
    pMenuRecordAdd = new QAction( text, QKeySequence( "CTRL+A" ), this, 0);
    pMenuRecordAdd->setObjectName("MenuRecordAdd");
    pMenuRecordAdd->setStatusTip( text );
    pMenuRecordAdd->setWhatsThis( toGUI( _( "Añade un registro al fichero" ) ) );
    connect( pMenuRecordAdd, SIGNAL( activated() ), this, SLOT( menuRecordAdd_clicked() ) );
    pMenuRecordAdd->addTo( pMenuRecord );

    text = toGUI( _( "&Borrar" ) );
    pMenuRecordDelete = new QAction( text, QKeySequence( "CTRL+B" ), this, 0 );
    pMenuRecordDelete->setStatusTip( text );
    pMenuRecordDelete->setWhatsThis( toGUI( _( "Borra el registro seleccionado" ) ) );
    connect( pMenuRecordDelete, SIGNAL( activated() ), this, SLOT( menuRecordDelete_clicked() ) );
    pMenuRecordDelete->addTo( pMenuRecord );

    text = toGUI( _( "&Modificar" ) );
    pMenuRecordModify = new QAction( text, QKeySequence( "CTRL+M" ), this, 0 );
    pMenuRecordModify->setStatusTip( text );
    pMenuRecordModify->setWhatsThis( toGUI( _( "Borra el registro seleccionado" ) ) );
    connect( pMenuRecordModify, SIGNAL( activated() ), this, SLOT( menuRecordModify_clicked() ) );
    pMenuRecordModify->addTo( pMenuRecord );

    text = toGUI( _( "Im&primir" ) );
    pMenuRecordPrint = new QAction( text, QKeySequence( "CTRL+P" ), this, 0 );
    pMenuRecordPrint->setStatusTip( text );
    pMenuRecordPrint->setWhatsThis( toGUI( _( "Imprime el registro seleccionado" ) ) );
    connect( pMenuRecordPrint, SIGNAL( activated() ), this, SLOT( menuRecordPrint_clicked() ) );
    pMenuRecordPrint->addTo( pMenuRecord );

    text = toGUI( _( "&Examinar" ) );
    pMenuRecordView = new QAction( text, QKeySequence( "CTRL+E" ), this, 0 );
    pMenuRecordView->setStatusTip( text );
    pMenuRecordView->setWhatsThis( toGUI( _( "Muestra el registro seleccionado" ) ) );
    connect( pMenuRecordView, SIGNAL( activated() ), this, SLOT( menuRecordView_clicked() ) );
    pMenuRecordView->addTo( pMenuRecord );

    text = toGUI( _( "&Duplicar" ) );
    pMenuRecordDup = new QAction( text, QKeySequence( "CTRL+D" ), this, 0 );
    pMenuRecordDup->setStatusTip( text );
    pMenuRecordDup->setWhatsThis( toGUI( _( "Duplica el registro seleccionado" ) ) );
    connect( pMenuRecordDup, SIGNAL( activated() ), this, SLOT( menuRecordDup_clicked() ) );
    pMenuRecordDup->addTo( pMenuRecord );

    text = toGUI( _( "&Información" ) );
    pMenuRecordInfo = new QAction( text, QKeySequence( "CTRL+I" ), this, 0 );
    pMenuRecordInfo->setStatusTip( text );
    pMenuRecordInfo->setWhatsThis( toGUI( _( "Muestra información sobre este fichero" ) ) );
    connect( pMenuRecordInfo, SIGNAL( activated() ), this, SLOT( menuRecordInfo_clicked() ) );
    pMenuRecordInfo->addTo( pMenuRecord );

    text = toGUI( _( "&Reabrir" ) );
    pMenuRecordReopen = new QAction( text, QKeySequence( "CTRL+2" ), this, 0 );
    pMenuRecordReopen->setStatusTip( text );
    pMenuRecordReopen->setWhatsThis( toGUI( _( "Abre una nueva ventana de este fichero" ) ) );
    connect( pMenuRecordReopen, SIGNAL( activated() ), this, SLOT( menuRecordReopen_clicked() ) );
    pMenuRecordReopen->addTo( pMenuRecord );


    // Opciones del menú Tabla
    text = toGUI( _( "&Filtrar" ) );
    pMenuTableFilter = new QAction( text, QKeySequence( "CTRL+F" ), this, 0 );
    pMenuTableFilter->setStatusTip( text );
    pMenuTableFilter->setWhatsThis( toGUI( _( "Filtra los registros de este fichero" ) ) );
    connect( pMenuTableFilter, SIGNAL( activated() ), this, SLOT( menuTableFilter_clicked() ) );
    pMenuTableFilter->addTo( pMenuTable );

    text = toGUI( _( "Quitar fi&ltro" ) );
    pMenuTableRemoveFilter = new QAction( text, QKeySequence( "CTRL+L" ), this, 0 );
    pMenuTableRemoveFilter->setStatusTip( text );
    pMenuTableRemoveFilter->setWhatsThis( toGUI( _( "Elimina el filtro de este fichero" ) ) );
    connect( pMenuTableRemoveFilter, SIGNAL( activated() ), this, SLOT( menuTableRemoveFilter_clicked() ) );
    pMenuTableRemoveFilter->addTo( pMenuTable );

    text = toGUI( _( "&Modificar múltiple" ) );
    pMenuTableUpdate = new QAction( text, QKeySequence(), this, 0 );
    pMenuTableUpdate->setStatusTip( text );
    pMenuTableUpdate->setWhatsThis( toGUI( _( "Modifica varios registros a la vez" ) ) );
    connect( pMenuTableUpdate, SIGNAL( activated() ), this, SLOT( menuTableUpdate_clicked() ) );
    pMenuTableUpdate->addTo( pMenuTable );

    text = toGUI( _( "Im&primir" ) );
    pMenuTablePrint = new QAction( text, QKeySequence(), this, 0 );
    pMenuTablePrint->setStatusTip( text );
    pMenuTablePrint->setWhatsThis( toGUI( _( "Imprime la vista como un informe" ) ) );
    connect( pMenuTablePrint, SIGNAL( activated() ), this, SLOT( menuTablePrint_clicked() ) );
    pMenuTablePrint->addTo( pMenuTable );

    text = toGUI( _( "&Importar" ) );
    pMenuTableImport = new QAction( text, QKeySequence( "CTRL+<" ), this, 0 );
    pMenuTableImport->setStatusTip( "Importar" );
    pMenuTableImport->setWhatsThis( toGUI( _( "Importa registros a este fichero" ) ) );
    connect( pMenuTableImport, SIGNAL( activated() ), this, SLOT( menuTableImport_clicked() ) );
    pMenuTableImport->addTo( pMenuTable );

    text = toGUI( _( "&Exportar" ) );
    pMenuTableExport = new QAction( text, QKeySequence( "CTRL+SHIFT+<" ), this, 0 );
    pMenuTableExport->setStatusTip( "Exportar" );
    pMenuTableExport->setWhatsThis( toGUI( _( "Exporta los registros de este fichero" ) ) );
    connect( pMenuTableExport, SIGNAL( activated() ), this, SLOT( menuTableExport_clicked() ) );
    pMenuTableExport->addTo( pMenuTable );

    text = toGUI( _( "&Seleccionar todo" ) );
    pMenuTableSelectAll = new QAction( text, QKeySequence( "CTRL+S" ), this, 0 );
    pMenuTableSelectAll->setStatusTip( "Seleccionar todo" );
    pMenuTableSelectAll->setWhatsThis( toGUI( _( "Selecciona todos los registros de este fichero" ) ) );
    connect( pMenuTableSelectAll, SIGNAL( activated() ), this, SLOT( menuTableSelectAll_clicked() ) );
    pMenuTableSelectAll->addTo( pMenuTable );

    pMenuView = new QMenu( this );
    pMenuBar->insertItem( QString::fromUtf8( _( "&Vistas" ) ), pMenuView );

    text = toGUI( _( "&Reordenar" ) );
    pMenuViewReorder = new QAction( text, QKeySequence( "CTRL+R" ), this, 0 );
    pMenuViewReorder->setStatusTip( text );
    pMenuViewReorder->setWhatsThis( toGUI( _( "Cambia el orden de los registros" ) ) );
    connect( pMenuViewReorder, SIGNAL( activated() ), this, SLOT( menuTableReorder_clicked() ) );
    pMenuViewReorder->addTo( pMenuView );
    text = toGUI( _( "&Añadir" ) );
    pMenuViewAdd = new QAction( text, this );
    pMenuViewAdd->setStatusTip( text );
    pMenuViewAdd->setWhatsThis( toGUI( _( "Añade una vista para este fichero" ) ) );
    connect( pMenuViewAdd, SIGNAL( activated() ), this, SLOT( menuViewAdd_clicked() ) );
    pMenuViewAdd->addTo( pMenuView );
    text = toGUI( _( "&Modificar" ) );
    pMenuViewModify = new QAction( text, this );
    pMenuViewModify->setStatusTip( text );
    pMenuViewModify->setWhatsThis( toGUI( _( "Modifica la vista actual" ) ) );
    connect( pMenuViewModify, SIGNAL( activated() ), this, SLOT( menuViewModify_clicked() ) );
    pMenuViewModify->addTo( pMenuView );
    text = toGUI( _( "&Duplicar" ) );
    pMenuViewDup = new QAction( text, this );
    pMenuViewDup->setStatusTip( text );
    pMenuViewDup->setWhatsThis( toGUI( _( "Duplica la vista actual" ) ) );
    connect( pMenuViewDup, SIGNAL( activated() ), this, SLOT( menuViewDup_clicked() ) );
    pMenuViewDup->addTo( pMenuView );
    text = toGUI( _( "&Borrar" ) );
    pMenuViewDelete = new QAction( text, this );
    pMenuViewDelete->setStatusTip( text );
    pMenuViewDelete->setWhatsThis( toGUI( _( "Borra la vista actual" ) ) );
    connect( pMenuViewDelete, SIGNAL( activated() ), this, SLOT( menuViewDelete_clicked() ) );
    pMenuViewDelete->addTo( pMenuView );
    pMenuView->insertSeparator();
};

void FrmEditRecMaster::updateViewMenus( bool refill )
{
    QList<QAction *> allMenus = pMenuView->findChildren<QAction *>();
    if( refill ) {
        int i;
        for ( i = allMenus.count() -1; i>0; --i ) {
            QAction *qa = allMenus[i];
            if( qa->isCheckable() )
                pMenuView->removeAction(qa);
        }
        // Dont use the iterator, as the order is not the same
        for( uint nv = 0; nv < pDataModel->getViews().size(); ++ nv ) {
            dbViewDefinition *viewdef = pDataModel->getViews().seq_at(nv);
            QAction *action  = pMenuView->addAction( toGUI( Xtring::number( nv ) + ". " + viewdef->getCaption() ) );
            action->setCheckable( true );
            if( pDataModel->getCurrentViewIndex() != -1 )
                action->setChecked( viewdef == pDataModel->getCurrentView() );
            connect( action, SIGNAL( triggered() ), this, SLOT( slotViewChanged() ) );
        }
    } else {
        Xtring curcaption = pDataModel->getCurrentView()->getCaption();
        for ( int i = allMenus.count() -1; i>0; --i ) {
            QAction *qa = allMenus[i];
            if( qa->isCheckable() ) {
                Xtring menu_caption = fromGUI( allMenus[i]->text() );
                Xtring without_number = menu_caption.mid( menu_caption.find(".") + 2);
                if( without_number == curcaption )
                    qa->setChecked( true );
                else
                    qa->setChecked( false );
            }
        }
    }
}

void FrmEditRecMaster::pushMiddle_clicked()
{
    if ( fromGUI( pushMiddle->text() )  == _( "&Modificar" ) )
        menuRecordModify_clicked();
    else
        menuRecordAdd_clicked();
}

void FrmEditRecMaster::menuRecordView_clicked()
{
    beginEdit( pDataTable, DataTable::selecting );
}

void FrmEditRecMaster::menuRecordAdd_clicked()
{
    beginEdit( pDataTable, DataTable::inserting );
}

void FrmEditRecMaster::menuRecordDup_clicked()
{
    beginEdit( pDataTable, DataTable::duplicating );
}


void FrmEditRecMaster::menuRecordDelete_clicked()
{
    if( pDataTable->getSelectedIDs().size() > 1 ) {
        if( msgYesNoCancel(this,
                           Xtring::printf(_("Has seleccionado %d registros.\n¿Estás seguro/a de que quieres borrarlos todos?"),
                                          pDataTable->getSelectedIDs().size() ) )
                == FrmBase::Yes ) {
            for( uint nrec = 0; nrec < pDataTable->getSelectedIDs().size(); nrec++ ) {
                read( pDataTable->getSelectedIDs()[nrec] );
                beginEdit( pDataTable, DataTable::deleting, dbApplication::editContinuous );
                DBAPP->processEvents();
                if( !(mEditFlags & dbApplication::editContinuous) ) // cancelled
                    break;
            }
            mMultipleDeleting = false;
            if( mEditStatus == FrmEditRec::saved ) // We have deleted up to the last one
                ensureBrowseView();
            refresh();
        }
    } else {
        beginEdit( pDataTable, DataTable::deleting );
    }
}


void FrmEditRecMaster::menuRecordModify_clicked()
{
    beginEdit( pDataTable, DataTable::updating );
}

// This is the slot signaled by the datatable
void FrmEditRecMaster::slotBeginEdit(DataTable *dt, DataTable::EditMode editmode,
                                     const Xtring &fldvalue)
{
    beginEdit( dt, editmode, dbApplication::editNone, fldvalue );
}

/// \todo {0.4} Mensaje cuando no haya permisos de la usuaria
void FrmEditRecMaster::beginEdit( DataTable *dt, DataTable::EditMode newmode,
                                  dbApplication::EditFlags editflags, const Xtring &fldvalue )
{
    DBAPP->waitCursor( true );
    try {
        dbRecordPermissions r = getPermissions();
        if ( newmode == DataTable::defaulteditmode ) {
            // Borrar nunca está por defecto
            if ( mChoosing ) {
                accept();
                DBAPP->resetCursor();
                return;
            } else if ( r.canEdit )
                newmode = DataTable::updating;
            else if ( r.canAdd )
                newmode = DataTable::inserting;
            else newmode = DataTable::selecting;
        }
        mEditFlags = static_cast<dbApplication::EditFlags>(mEditFlags | editflags );
        if (( mEditFlags & dbApplication::embedded ) ) {
            // Clonar el formulario. Atención, el nuevo formulario es masterfrm, no this
            if ( newmode == DataTable::inserting && r.canAdd ) {
                pRecord->setNew();
                pRecord->clear( true ); // set custom default values
            } else if (( newmode == DataTable::updating && r.canEdit )
                       || ( newmode == DataTable::duplicating && r.canAdd )
                       || ( newmode == DataTable::deleting && r.canDelete ) ) {
                read( dt->getDataModel()->getRowID( dt->currentRow() ) );
                if ( newmode == DataTable::duplicating )
                    clearRecord( true /*duplicating*/ );
            } else {
                DBAPP->resetCursor();
                return;
            }
            FrmEditRecMaster *masterfrm = static_cast<FrmEditRecMaster *>(
                                              DBAPP->createEditForm( pParentForm, pRecord,
                                                      static_cast<dbRecordDataModel *>( dt->getDataModel() ), newmode,
                                                      dbApplication::EditFlags(dbApplication::simpleEdition | editflags) ) );
            if ( masterfrm ) {
                masterfrm->setFormFilter( getFormFilter() );  // Copy the form filter
                if ( newmode == DataTable::deleting ) {
                    masterfrm->show();
                    if( msgYesNo( masterfrm,
                                  Xtring::printf( _( "¿Estás segura de borrar %s?\n" ),
                                                  DBAPP->getTableDescSingular( pRecord->getTableName(), "esta" ).c_str() ), false ) ) {
                        // Find the id where to go when this record is deleted
                        int currow = pDataTable->currentRow();
                        // En una vista con maestro/detalles, el ID puede estar repetido. Moverse al siguiente o anterior
                        dbRecordID nextid = pDataTable->getDataModel()->getRowID( currow );
                        while ( currow < pDataTable->numRows() && nextid == mLastID )
                            nextid = pDataTable->getDataModel()->getRowID( currow++ );
                        if ( nextid == mLastID ) {
                            while ( currow > 0 && nextid == mLastID )
                                nextid = pDataTable->getDataModel()->getRowID( --currow );
                        }
                        masterfrm->accept();
                        mLastID = nextid;
                        refresh();
                    } else if ( mBrowsing )
                        ensureBrowseView();
                } else {
                    _GONG_DEBUG_ASSERT( dt );
                    _GONG_DEBUG_ASSERT( dt->getDataModel() );
                    Xtring fldname;
                    if( dt->currentColumn() != -1 )
                        fldname = dt->getDataModel()->getColName( dt->indexOf( dt->currentColumn() ) );
                    if ( newmode == DataTable::inserting )
                        masterfrm->setEdited( true );  // to get all fields validated (default values)
                    else if ( !fldname.isEmpty() ) {
                        masterfrm->pFocusWidget = masterfrm->findControl( Xtring( fldname ).replace( ".", "_" ) );
                        if ( masterfrm->pFocusWidget ) {
                            if ( !fldvalue.isEmpty() ) {
                                masterfrm->getRecord()->setValue( fldname, fldvalue );
                            }
                        }
                    }
                    masterfrm->showModalFor( this, true, true );  // Centered
                    if ( masterfrm->isSaved() ) {
                        mLastID = masterfrm->getRecord()->getRecordID();
                        refresh();
                    }
                }
                delete masterfrm;
            }
        } else if( !(mEditFlags & dbApplication::onlyBrowse) ) {
            if ( newmode == DataTable::inserting && r.canAdd ) {
                mMustRead = false;
                clearRecord();
                if( canBeginEdit( DataTable::inserting ) ) {
                    setEdited( true );  // to get all fields validated (default values)
                    mWasBrowsing = mBrowsing;
                    mEditMode = DataTable::inserting;
                    mEditFlags = static_cast<dbApplication::EditFlags>(mEditFlags | dbApplication::editContinuous );
                    mEditStatus = not_saved;
                    ensureEditView();
                }
            } else if ( newmode == DataTable::duplicating && r.canAdd ) {
                if( mMustRead )
                    read( getTableRecordID() );
                if( canBeginEdit( DataTable::duplicating ) ) {
                    mWasBrowsing = mBrowsing;
                    mEditMode = DataTable::duplicating;
                    mEditStatus = not_saved;
                    ensureEditView();
                    // Trick: first we copy data to form, then make the record new
                    clearRecord( true /*duplicating*/ );
                    // 				setJustEditedAllControls( true );
                }
            } else if ( newmode == DataTable::updating && r.canEdit ) {
                if( mMustRead )
                    read( getTableRecordID() );
                if( canBeginEdit( DataTable::updating ) ) {
                    mWasBrowsing = mBrowsing;
                    mEditMode = DataTable::updating;
                    mEditStatus = not_saved;
                    ensureEditView();
                }
            } else if ( newmode == DataTable::deleting && r.canDelete ) {
                if( mMustRead )
                    read( getTableRecordID() );
                if( canBeginEdit( DataTable::deleting ) ) {
                    mWasBrowsing = mBrowsing;
                    bool wasbrowsing = mBrowsing;
                    mEditMode = DataTable::deleting;
                    mEditStatus = not_saved;
                    try {
                        ensureEditView(); // scatter might throw
                    } catch( ... ) {
                    }
                    int yes_no_cancel = FrmBase::Yes;
                    if( !mMultipleDeleting ) {
                        if( mEditFlags & dbApplication::editContinuous ) {
                            yes_no_cancel = msgYesNoAllCancel( this,
                                                               Xtring::printf( _( "¿Estás segura de borrar %s?\n" ),
                                                                       DBAPP->getTableDescSingular( pRecord->getTableName(), "esta" ).c_str() ), false );
                        } else {
                            yes_no_cancel = msgYesNoCancel( this,
                                                            Xtring::printf( _( "¿Estás segura de borrar %s?\n" ),
                                                                    DBAPP->getTableDescSingular( pRecord->getTableName(), "esta" ).c_str() ), false );
                        }
                    }
                    switch( yes_no_cancel ) {
                    case FrmBase::Yes:
                        mEditMode = DataTable::deleting;
                        accept();
                        break;
                    case FrmBase::YesToAll:
                        mMultipleDeleting = true;
                        mEditMode = DataTable::deleting;
                        accept();
                        break;
                    case FrmBase::No:
                        break;
                    case FrmBase::Cancel:
                        mEditFlags = static_cast<dbApplication::EditFlags>(mEditFlags & ~dbApplication::editContinuous );
                        break;
                    }
                    if( mEditStatus == not_saved ||
                            (wasbrowsing && !( mEditFlags & dbApplication::editContinuous ) ) )
                        ensureBrowseView();
                }
            } else if ( newmode == DataTable::selecting && r.canView ) {
                if ( !mBrowsing ) {
                    mWasBrowsing = mBrowsing;
                    ensureBrowseView();
                } else {
                    if( mMustRead )
                        read( getTableRecordID() );
                    if( canBeginEdit( mEditMode ) ) {
                        mWasBrowsing = mBrowsing;
                        mEditMode = DataTable::selecting;
                        ensureEditView();
                        enableSearchBoxes( true );
                    }
                }
            }
        }
    } catch( gong::dbError &e ) {
        _GONG_DEBUG_WARNING( e.what() );
        if( !DBAPP->manageDbError( getRecord()->getTableName(), e ) ) {
            DBAPP->resetCursor();
            throw;
        } else
            msgError( this, e.what() );
    } catch( std::runtime_error &e ) {
        msgError( this, e.what() );
        _GONG_DEBUG_WARNING( e.what() );
    }
    DBAPP->resetCursor();
}

void FrmEditRecMaster::menuTableFilter_clicked()
{
    FrmSQLSelect *sel = new FrmSQLSelect( DBAPP->getConnection(), DBAPP->getDatabase(),
                                          this, "filtrar" );
    XtringList joinedtables = dbConnection::extractSqlTables(
                                  "FROM " + getDataTable()->getDataModel()->getCurrentView()->getFrom() );
    // The whole filter minus the record filter
    Xtring filter = getFormFilter();
    filter = pDataModel->getFilter("", filter );
    if ( sel->exec( this, joinedtables, filter ) )
        applyFilter( sel->getSqlExpression() );
    delete sel;
}

#ifdef HAVE_RTKMODULE

void FrmEditRecMaster::menuRecordPrint_clicked()
{
    Xtring cond;
    if( getDataTable()->getSelectedIDs().size() > 1 ) {
        cond = getRecord()->getIDSqlCond( getDataTable()->getSelectedIDs() ) ;
    } else {
        cond = getRecord()->getIDSqlCond( getTableRecordID() ) ;
    }
    Dictionary<Variant> empty_properties;
    printReport( Xtring(pDataModel->getCurrentView()->getName()).replace(".","_"),
                 empty_properties, cond,
                 DBAPP->getAppSetting( "RTK.LANDSCAPE", false ).toBool() ? Landscape : DefaultOrientation,
                 Xtring::null, true, true );
}

void FrmEditRecMaster::menuTablePrint_clicked()
{
    FrmAdvancedPrint *advprt = new FrmAdvancedPrint( this );
    advprt->showModalFor( this, false, true );
}

void FrmEditRecMaster::printReport( const Xtring &reportname, const Dictionary<Variant> &properties,
                                    const Xtring &filter, PageOrientation po, const Xtring &defines,
                                    bool isafilename, bool resorttotablename )
{
    DBAPP->waitCursor(true);
    try {
        bool ret;
        AppReport *report = new AppReport( *DBAPP, getRecord()->getConnection() );
        if( isafilename ) {
            _GONG_DEBUG_PRINT(2, "Printing report " + reportname );
            ret = report->readFile( reportname, defines );
        } else {
            ret = report->readString( reportname.c_str(), defines );
        }
        if( !ret && resorttotablename ) {
            report->clearErrors();
            ret = report->readFile( FileUtils::findInPath( report->includePath(), getRecord()->getTableName() + ".rtk", defines ).c_str() );
        }
        if ( ret ) {
            if ( mMustRead )
                read( getTableRecordID() );
            ret = report->print( RTK_Screen, properties, filter, Xtring::null, po, false ); // no pedir parámetros
        }
        if( !ret ) {
            if ( report->errorsCount() == 0 ) {
                FrmBase::msgError( this,
                                   Xtring::printf(
                                       _( "Imposible encontrar el informe \n%.100s\npara la tabla de %s.\nBuscando en %s" ),
                                       reportname.c_str(),
                                       getRecord()->getTableDefinition()->getDescPlural().c_str(),
                                       Xtring(report->includePath()).replace( ":", "\n" ).c_str() ) );
            } else {
                FrmBase::msgError( this,
                                   Xtring::printf(
                                       _( "En el informe %.100s para la tabla de %s.\n%s" ),
                                       reportname.c_str(),
                                       getRecord()->getTableDefinition()->getDescPlural().c_str(),
                                       report->lastError()->message() ) );
            }
        }
        delete report;
    } catch ( RTK::Error &e ) {
        _GONG_DEBUG_WARNING( e.what() );
        FrmBase::msgError( this,
                           Xtring::printf( _( "Imprimiendo el informe %.100s para la tabla de %s.\n%s" ),
                                           getRecord()->getTableName().c_str(),
                                           getRecord()->getTableDefinition()->getDescPlural().c_str(),
                                           e.what() ) );
    }
    DBAPP->resetCursor();
}

#else

void FrmEditRecMaster::menuRecordPrint_clicked()
{
    FrmBase::msgError( this,
                       _( PACKAGE " no ha sido compilado con la posibilidad de hacer informes" ) );
}

void FrmEditRecMaster::menuTablePrint_clicked()
{
    menuRecordPrint_clicked();
}

#endif // HAVE_RTKMODULE

void FrmEditRecMaster::menuTableUpdate_clicked()
{
    FrmMultiUpdate *fmu = new FrmMultiUpdate( this );
    fmu->showModalFor( this, false, true );
    delete fmu;
}

void FrmEditRecMaster::menuTableReorder_clicked()
{
    setView( pDataModel->getCurrentViewIndex() + 1, false );
}

void FrmEditRecMaster::slotViewChanged()
{
    QAction *action = static_cast<QAction *>(sender());
    Xtring text = fromGUI( action->text() );
    int index = text.toInt();
    setView( index, false );
}

void FrmEditRecMaster::menuTableRemoveFilter_clicked()
{
    // Remove adhoc record filters
    for( int i = (int)pRecord->getFilters().size() -1; i >= 0; --i ) {
        Xtring f = pRecord->getFilters()[i];
        if( !pRecord->isStructuralFilter( f ) ) {
            pRecord->removeFilter( f );
        }
    }
    applyFilter( Xtring::null );
}

void FrmEditRecMaster::menuTableImport_clicked()
{
    class FrmImport: public FrmCsvParams
    {
    public:
        FrmImport(dbRecord *r, QWidget *parent, const char *name, WidgetFlags f = 0)
            : FrmCsvParams( FrmCsvParams::importing, parent, _("Importación de registros") ),
              pRecord(r)
        {
            XtringList mOpcionesExisteCaptions;
            IntList mOpcionesExisteValues;
            mOpcionesExisteCaptions << "Actualizar el registro" << "Ignorar el registro" << "Cancelar la importación";
            mOpcionesExisteValues << 0 << 1 << 2;
            comboExiste = addComboBoxInt( true, 0, _("Si el registro ya existe..."),
                                          mOpcionesExisteCaptions, mOpcionesExisteValues );
            checkRevisar = addCheckBox( 0, _("Revisar los registros uno por uno"), true );
            pShowTemplate = addButton( this, _("Mostrar plantilla para importar") );
        }
        bool getRevisar() const {
            return checkRevisar->isChecked();
        }
        int getSiExiste() const {
            return comboExiste->currentIndex();
        }
    private:
        virtual void validate_input(QWidget *w, bool *) {
            if( w!=pShowTemplate )
                return;
            // show a template for importing
            Xtring descriptions;
            Xtring _template;
            Xtring blankline;
            const dbTableDefinition *tbldef = pRecord->getTableDefinition();
            addDescription( tbldef, descriptions, _template, blankline );
            for( dbRelationDefinitionDict::const_iterator it = tbldef->getRelationDefinitions().begin();
                    it != tbldef->getRelationDefinitions().end(); ++it ) {
                if( it->second->getType() == dbRelationDefinition::aggregate ) {
                    dbTableDefinition *reltbldef = DBAPP->getDatabase()->findTableDefinition(it->second->getRightTable());
                    addDescription( reltbldef, descriptions, _template, blankline );
                } else if( it->second->getType() == dbRelationDefinition::one2one ) {
                    dbTableDefinition *reltbldef = DBAPP->getDatabase()->findTableDefinition(it->second->getRightTable());
                    dbFieldDefinition *codefld = reltbldef->findFieldByFlags( dbFieldDefinition::CODE );
                    if( codefld ) {
                        descriptions += codefld->getFullName() + ": ";
                        if( codefld->getDescription().isEmpty() )
                            descriptions += codefld->getCaption() + "\n";
                        else
                            descriptions += codefld->getDescription() + "\n";
                        _template.appendWithSeparator( codefld->getFullName(), getSeparator() );
                        blankline.appendWithSeparator( getDelimiter()+getDelimiter(), getSeparator() );
                    }
                }
            }
            FrmBase::msgOkLarge( this,
                                 Xtring::printf(_("Plantilla para importar %s"), DBAPP->getTableDescPlural( pRecord->getTableName() ).c_str() ),
                                 descriptions
                                 + _( "\nPuedes copiar y pegar el texto entre las dos rayas directamente a una hoja de cálculo:\n")
                                 + "--------------------------\n"
                                 + _template + "\n"
                                 + blankline + "\n\n"
                                 + "--------------------------\n" );
        }
        void addDescription( const dbTableDefinition *tbldef, Xtring &descriptions, Xtring &_template, Xtring &blankline ) {
            for( uint i = 0; i < tbldef->getFieldCount(); ++i ) {
                dbFieldDefinition *flddef = tbldef->getFieldDefinition(i);
                if( flddef->isReference() || flddef->isPrimaryKey() )
                    continue;
                if( flddef->getName().startsWith("REC_") )
                    continue;
                descriptions += flddef->getFullName() + ": ";
                if( flddef->getDescription().isEmpty() )
                    descriptions += flddef->getCaption() + "\n";
                else
                    descriptions += flddef->getDescription() + "\n";
                _template.appendWithSeparator( flddef->getFullName(), getSeparator() );
                blankline.appendWithSeparator( getDelimiter()+getDelimiter(), getSeparator() );
            }
        }

        dbRecord *pRecord;
        ComboBoxInt *comboExiste;
        CheckBox *checkRevisar;
        PushButton *pShowTemplate;
    };

    FrmImport *frmimport = new FrmImport(pRecord, 0, "FrmImport_FrmEditRecMaster" );
    frmimport->showModalFor( this, false, true );
    if( !frmimport->wasCancelled() ) {
        bool revisarorig = frmimport->getRevisar();
        int siexiste = frmimport->getSiExiste();
        Xtring fname = frmimport->getImportFilename();
        int nimported = 0;
        Xtring headersline;
        XtringList records, headers;
        if ( !CsvUtils::readCSVFile( fname, records, headersline, true, '\"' ) ) {
            FrmBase::msgError( this,
                               Xtring::printf( _( "Error en el fichero %s\n%s" ), fname.c_str(), strerror( errno ) ) );
            return;
        }
        if ( records.size() == 0 ) {
            FrmBase::msgWarning( this,
                                 Xtring::printf( _( "El fichero %s no contiene registros." ), fname.c_str() ) );
            return;
        }
        if ( !FrmBase::msgYesNoLarge( this,
                                      Xtring::printf( _( "Se importarán %d registros. ¿Quieres continuar?" ), records.size() ),
                                      headersline + "\n" + records[0] ) ) {
            return;
        }
        DBAPP->waitCursor( true );
        CsvUtils::tokenize( headers, headersline, '\"', ',' );
        Xtring unknownfields;
        for ( XtringList::const_iterator headersit = headers.begin();
                headersit != headers.end(); ++ headersit ) {
            Xtring foo = *headersit;
            if( !(foo.isEmpty())
                    && !DBAPP->getDatabase()->findFieldDefinition( *headersit, false )
                    && !DBAPP->getDatabase()->findFieldDefinition( getRecord()->getTableName()+"."+*headersit, false ) ) {
                unknownfields.appendWithSeparator( *headersit, ", " );
                headersline.appendWithSeparator( "\"" + *headersit + "\"", "," );
            }
        }
        if ( !unknownfields.isEmpty() ) {
            if ( msgYesNo( this,
                           Xtring::printf( _( "Las columnas\n%s\n no existen en la tabla de %s. ¿Quieres continuar?" ),
                                           unknownfields.c_str(), getRecord()->getTableDefinition()->getDescPlural().c_str() ) ) == false ) {
                DBAPP->resetCursor();
                return;
            }
        }
        dbRecord *r = DBAPP->createRecord( getRecord()->getTableName() );
        for ( XtringList::const_iterator linesit = records.begin();
                linesit != records.end(); ++linesit ) {
            bool revisar = revisarorig;
            r->clear( true ); // set custom default values
            r->setNew( true );
            r->fromString( *linesit, TOSTRING_CSV, headersline );
            _GONG_DEBUG_PRINT(0, r->toString( TOSTRING_DEBUG_COMPLETE ) );
            dbRecordID existing_id = 0;
            r->findMatchingRecord(&existing_id);
            if( existing_id ) {
                if( siexiste == 0) { // Actualizar
                    r->read( existing_id );
                    _GONG_DEBUG_PRINT(0, r->toString( TOSTRING_DEBUG_COMPLETE ) );
                    r->fromString( *linesit, TOSTRING_CSV, headersline );
                } else if( siexiste == 1 ) { // Ignorar
                    continue; // Leer el siguiente
                } else {
                    msgError( this, _("Importación cancelada porque un registro ya existía") );
                    break;
                }
            }
            _GONG_DEBUG_PRINT(0, r->toString( TOSTRING_DEBUG_COMPLETE ) );
            r->readRelated( true );
            _GONG_DEBUG_PRINT(0, r->toString( TOSTRING_DEBUG_COMPLETE ) );
            bool revisaerroneo = false;
            if( !r->isValid( ValidResult::fixing, 0 ) ) {
                DBAPP->showOSD( _("Importar"), _("Este registro contiene errores.") );
                DBAPP->processEvents();
                revisaerroneo = true;
            }
            if( !revisar && !revisaerroneo ) {
                try {
                    r->save( false ); // Don't update relations
                    nimported ++;
                    if( nimported % 5 == 0 ) {
                        DBAPP->showOSD( _("Importando"), Xtring::printf(_("Importados %d registros"), nimported ) ); // TODO descplural
                        DBAPP->processEvents();
                    }
                } catch( dbError &e ) {
                    _GONG_DEBUG_WARNING( e.what() );
                    msgError( this, Xtring( _( "Error al guardar el registro:\n" )) + e.what() );
                    revisaerroneo = true;
                }
            }
            if( revisar || revisaerroneo ) {
                revisaerroneo = false;
                FrmEditRecMaster *editfrm = static_cast<FrmEditRecMaster *>(
                                                DBAPP->createEditForm( this, r, 0, DataTable::updating, dbApplication::simpleEdition ) );
                if ( editfrm ) {
                    editfrm->setFormFilter( getFormFilter() );  // Copy the form filter
                    editfrm->showModalFor( this, false, true );
                    if ( !editfrm->isSaved() ) {
                        delete editfrm;
                        XtringList::const_iterator linesact = linesit;
                        if( ++linesact == records.end() )
                            break; // Don't ask if this is the last line
                        if( !FrmBase::msgYesNo( this,
                                                _( "Has anulado la importación de este registro.\n"
                                                   "¿Qieres continuar con los siguientes?" ) ) )  {
                            break;
                        }
                    } else {
                        delete editfrm;
                        nimported ++;
                    }
                } else {
                    msgError( this, _( "No se ha podido crear el formulario para la importación" ) );
                    break;
                }
            } else {
            }
        }
        delete r;
        if ( nimported ) {
            msgOk( this, Xtring::printf( _( "Se han importado %d registros" ), nimported ) );
            refresh();
        }
        DBAPP->resetCursor();
    }
}

void addFieldValueToCSV(dbRecord *r, const dbTableDefinition *tbldef,
                        const Xtring &delimiter, const Xtring &separator,
                        Xtring &csvline, Xtring &headers, bool headercompleted )
{
    for( uint i = 0; i < tbldef->getFieldCount(); ++i ) {
        dbFieldDefinition *flddef = tbldef->getFieldDefinition(i);
        if( flddef->isReference() || flddef->isPrimaryKey() )
            continue;
        if( flddef->getName().startsWith("REC_") )
            continue;
        csvline.appendWithSeparator( delimiter + r->getValue(flddef->getFullName()).toString() + delimiter, separator );
        if( !headercompleted )
            headers.appendWithSeparator( flddef->getFullName(), separator );
    }
}

void FrmEditRecMaster::menuTableExport_clicked()
{
    FrmCsvParams *frmcsvparams = new FrmCsvParams( FrmCsvParams::exporting, this, _("Exportar") );
    frmcsvparams->showModalFor( this, false, true );
    if( !frmcsvparams->wasCancelled() ) {
        Xtring delimiter = frmcsvparams->getDelimiter();
        Xtring separator = frmcsvparams->getSeparator();
        Xtring exporttext;
        int range = frmcsvparams->getRange();
        dbRecord *r = 0;
        if( frmcsvparams->getFields() == FrmCsvParams::allFields )
            r = pRecord->duplicate();
        Xtring csvheader;
        bool headercompleted = false;
        for ( int row = 0; row < pDataTable->numRows(); row ++ ) {
            if( range == FrmCsvParams::current ) {
                if( row != pDataTable->currentRow() )
                    continue; // skip
                else
                    break; // no more
            } else if( range == FrmCsvParams::selected ) {
                if( !pDataTable->isRowSelected(row) ) {
                    continue; // skip
                }
            }
            Xtring csvline;
            if( frmcsvparams->getFields() == FrmCsvParams::allFields ) {
                r->read( pDataTable->getDataModel()->getRowID(row) );
                const dbTableDefinition *tbldef = pRecord->getTableDefinition();
                addFieldValueToCSV(r, tbldef, delimiter, separator, csvline, csvheader, headercompleted);
                for( dbRelationDefinitionDict::const_iterator it = tbldef->getRelationDefinitions().begin();
                        it != tbldef->getRelationDefinitions().end(); ++it ) {
                    if( it->second->getType() == dbRelationDefinition::aggregate ) {
                        dbTableDefinition *reltbldef = DBAPP->getDatabase()->findTableDefinition(it->second->getRightTable());
                        addFieldValueToCSV(r, reltbldef, delimiter, separator, csvline, csvheader, headercompleted);
                    } else if( it->second->getType() == dbRelationDefinition::one2one ) {
                        dbTableDefinition *reltbldef = DBAPP->getDatabase()->findTableDefinition(it->second->getRightTable());
                        dbFieldDefinition *codefld = reltbldef->findFieldByFlags( dbFieldDefinition::CODE );
                        if( codefld ) {
                            csvline.appendWithSeparator( r->getValue( codefld->getFullName() ).toString(), separator );
                            if( !headercompleted )
                                csvheader.appendWithSeparator( codefld->getFullName(), separator );
                        }
                    }
                }
            } else {
                for ( int col = 1; col < pDataTable->numCols(); col ++ ) {
                    if( !headercompleted )
                        csvheader.appendWithSeparator( pDataTable->getFldInfo( col )->getFullName(), separator );
                    csvline.appendWithSeparator( delimiter, separator );
                    if( pDataTable->getFldInfo( col )->getSqlColumnType() == SQLDECIMAL
                            || pDataTable->getFldInfo( col )->getSqlColumnType() == SQLFLOAT ) {
                        csvline += Xtring::number(pDataTable->getDataModel()->getValue(row, col).toDouble()).replace(",",".");
                    } else {
                        csvline += CsvUtils::dupQuotes( fromGUI(pDataTable->text( row, col ) ), delimiter[0] ); // .replace( "\n", " " );
                    }
                    csvline += delimiter;
                }
            }
            exporttext += csvline + "\n";
            headercompleted = true;
        }
        if( r ) delete r;
        Xtring csvname = frmcsvparams->getExportFilename();
        if( csvname.isEmpty() ) {
            FrmBase::msgOkLarge( this,
                                 _( "Puedes copiar y pegar este texto directamente a una hoja de cálculo." ), csvheader + "\n" + exporttext );
        } else {
            GuiApplication::writeFile( getTitle(), csvname, csvheader + "\n" + exporttext );
        }
    }
}

void FrmEditRecMaster::menuTableSelectAll_clicked()
{
    getDataTable()->selectCells( 0, 0, getDataTable()->numRows()-1,
                                 getDataTable()->numCols()-1);
}

void FrmEditRecMaster::menuRecordInfo_clicked()
{
    Xtring info;
    if( !isBrowsing() && !isSelecting() && getRecord()->isNew() ) {
        info = _("Nuevo registro");
    } else {
        if( mMustRead )
            read();
        info = Xtring::printf(
                   "ID: %d\n"
                   "Fecha de creación: %s\n"
                   "Fecha de última modificación: %s\n"
                   "Usuaria que lo ha creado: %d\n"
                   "Usuaria que lo ha modificado por última vez: %d\n\n"
                   "Número de registros del fichero: %d\n\n",
                   getRecordID(),
                   getRecord()->getValue( "REC_FECHA_CREA" ).toDate().toString().c_str(),
                   getRecord()->getValue( "REC_FECHA_MODIF" ).toDate().toString().c_str(),
                   getRecord()->getValue( "REC_USUAR_CREA" ).toInt(),
                   getRecord()->getValue( "REC_USUAR_MODIF" ).toInt(),
                   getRecord()->getConnection()->selectInt( Xtring( "SELECT COUNT(*) FROM " ) + getRecord()->getTableName() ) );
    }
    FrmBase::msgOk( _( "Información del registro" ), info, FrmBase::information, this  );
}

void FrmEditRecMaster::menuRecordReopen_clicked()
{
    DBAPP->waitCursor( true );
    try {
        dbRecord *r = DBAPP->createRecord( getRecord()->getTableName() );
        DBAPP->getMainWindow()->createClient( DBAPP->browse( 0, r, 0,
                                              DataTable::defaulteditmode,	dbApplication::deleteRecord  ), true /* force reopen */ );
    } catch ( std::runtime_error e ) {
        _GONG_DEBUG_WARNING( e.what() );
        if ( FrmBase::msgYesNo( this, Xtring( e.what() ) + _( "\n\n¿Quieres cerrar el programa?" ) ) ) {
            DBAPP->resetCursor();
            throw;
        }
    }
    DBAPP->resetCursor();
}

/**
 * @brief Adds a new view to the datamodel and class FrmViewEdit to edit it
 *
 * @param newviewdef A newly created view
 * @return void
 *
 * If the view is successfully edited, it remains in the datamodel
 *    If not, the view is deleted
 **/
void FrmEditRecMaster::editNewView( dbViewDefinition *newviewdef )
{
    newviewdef->setName( "newview" );
    pDataModel->addView( newviewdef );
    pDataModel->setViewIndex( pDataModel->getViewCount() - 1 );
    refresh();
    newviewdef->setCaption( "" );
    newviewdef->setOrigin( "NEW:" + newviewdef->getName() );
    FrmViewEdit *vedit = new FrmViewEdit( DataTable::inserting,
                                          getRecord(), newviewdef, this);
    vedit->exec();
    if( vedit->wasCancelled() ) {
        pDataModel->deleteView( newviewdef );
        pDataModel->setViewIndex( 0 );
    } else {
        // refresh view, at least the title of this form
        pDataModel->setViewIndex( pDataModel->getViewCount() - 1 );
    }
    refresh();
    updateViewMenus(true);
    delete vedit;
}

void FrmEditRecMaster::menuViewAdd_clicked()
{
    editNewView( new dbViewDefinition( *getRecord()->getTableDefinition() ) );
}

void FrmEditRecMaster::menuViewDup_clicked()
{
    editNewView( new dbViewDefinition( *pDataModel->getCurrentView() ) );
}

void FrmEditRecMaster::menuViewModify_clicked()
{
    if( pDataModel->getCurrentView()->getOrigin().upper().startsWith( "DEFAULT:" ) ) {
        msgWarning( this, Xtring::printf( _("Esta tabla no tiene vistas definidas. Se creará una nueva vista.")  ) );
        menuViewAdd_clicked();
        return;
    }
    FrmViewEdit *vedit = new FrmViewEdit( DataTable::updating,
                                          getRecord(), pDataModel->getCurrentView(), this );
    vedit->exec();
    delete vedit;
    refresh();
    updateViewMenus(true);
    updateStatus();
}

void FrmEditRecMaster::menuViewDelete_clicked()
{
    if( pDataModel->getCurrentView()->getOrigin().upper().startsWith( "DEFAULT:" ) ) {
        msgError( this, _("No se puede borrar la vista por defecto") );
        return;
    }
    bool borrar = true;
    bool global = true;
    Xtring caption = pDataModel->getCurrentView()->getCaption();
    Xtring cfgkey = pDataModel->getCurrentView()->getOrigin();
    if( pDataModel->getViews().size() == 1 ) {
        if( ! msgYesNo( this, getTitle(), _("¿Estás segura de borrar la única vista de este fichero?") ) )
            borrar = false;
    }
    if( borrar ) {
        switch( msgYesNoCancel(this, Xtring::printf(
                                   _("¿Quieres borrar la vista '%s' de la configuración global también?"),
                                   caption.c_str() ) ) ) {
        case FrmBase::Yes:
            global = true;
            break;
        case FrmBase::No:
            global = false;
            break;
        default:
            borrar = false;
        }
    }
    if( borrar ) {
        _GONG_DEBUG_PRINT(10, pDataModel->getCurrentView()->getOrigin() );
        if( cfgkey.startsWith("CONFIG:") )
            cfgkey = cfgkey.mid( 7 );
        DBAPP->removeSetting( cfgkey, global );
        DBAPP->setViewsFromConfig( DBAPP->getDatabase() );
        pDataModel->deleteView( pDataModel->getCurrentView() );
        refresh();
        updateViewMenus(true);
        msgOk( this, Xtring::printf( _("Se ha borrado la vista %s de la tabla de %s.\nOrigen: %s"),
                                     caption.c_str(),
                                     getRecord()->getTableDefinition()->getDescPlural().c_str(),
                                     cfgkey.c_str() ) );
    }
}

void FrmEditRecMaster::restoreBrowseStatus()
{
    _GONG_DEBUG_ASSERT( pDataModel );
    if( pDataTable ) {
        unsigned int lastid = 0;
        int nview = 0;
        Xtring browseStatus = DBAPP->getAppSetting("GUI.WORKSPACE." + Xtring(name()) + ".BROWSESTATUS").toString();
        if( !browseStatus.isEmpty() )
            sscanf( browseStatus.c_str(), "%d, %d", &lastid, &nview);
        if( !pDataTable->getDataModel() )
            setView( nview, true );
        else if( nview != pDataTable->getDataModel()->getCurrentViewIndex() )
            setView( nview, false );
        mLastID = getTableRecordID();
        if( mChoosing ) {
            pDataTable->sync( mLastID );
        } else if( lastid != 0 && lastid != mLastID ) {
            mLastID = lastid;
            pDataTable->sync( mLastID );
        } else if( lastid == 0 ) {
            if( pDataTable->numRows() != 0 ) {
                pDataTable->clearSelection();
                pDataTable->selectRow( 0 );
                pDataTable->setCurrentCell( 0, 1 );
            }
        }
    }
}

void FrmEditRecMaster::saveBrowseStatus() const
{
    if( pDataTable ) {
        dbRecordID recid = getTableRecordID();
        DBAPP->setUserLocalSetting("GUI.WORKSPACE." + Xtring(name()) + ".BROWSESTATUS",
                                   Xtring::printf("%d, %d", recid, pDataModel->getCurrentViewIndex() ) );
    }
}

void FrmEditRecMaster::focusInEvent(QFocusEvent* event)
{
    if( ( isBrowsing() || isChoosing() ) && pDataTable && focusWidget() != pDataTable )
        setWiseFocus(pDataTable);
    else
        QWidget::focusInEvent( event );
}

}; // Namespace

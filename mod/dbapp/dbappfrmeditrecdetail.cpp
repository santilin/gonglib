#include <QAction>
#include <QMenu>
#include <gonggettext.h>
#include <gongdatatable.h>
#include <gongdbrecordpermissions.h>
#include "dbappmainwindow.h"
#include "dbappdbapplication.h"
#include "dbappfrmeditrecdetail.h"

namespace gong {

FrmEditRecDetail::FrmEditRecDetail(
    FrmEditRecMaster *frmmaster, int ndetalle,
    dbRecord *detail, const Xtring &detailstablename, dbRecordDataModel *dm,
    EditMode editmode, dbApplication::EditFlags editflags,
    QWidget* parent, const char* name, WidgetFlags fl)
    : FrmEditRecMaster(frmmaster, detail, dm, editmode, editflags, parent, name, fl ),
      mDetallesTableName( detailstablename ), mNumDetalle( ndetalle )
{
    _GONG_DEBUG_ASSERT( frmmaster->getRecord()->findRelationByRelatedTable( detailstablename ) );
    pDetalles = frmmaster->getRecord()->findRelationByRelatedTable( detailstablename )->getRelatedRecordList();
    if( pDataTable ) {
        pDataTable->setVerticalHeader(true);
        connect( pDataTable, SIGNAL( focusChanged(DataTable*,QFocusEvent*)),
                 this, SLOT( slotFocusChanged(DataTable*,QFocusEvent*) ) );
    }
    pButtonDetailUp = new QPushButton( this, "pushDetailUp" );
    connect( pButtonDetailUp, SIGNAL( clicked() ), this, SLOT( slotDetailUpClicked() ) );
    pButtonsLayout->insertWidget( 3, pButtonDetailUp );
    pButtonDetailUp->setText( _("Subir") );
    pButtonDetailDown = new QPushButton( this, "pushDetailUp" );
    connect( pButtonDetailDown, SIGNAL( clicked() ), this, SLOT( slotDetailDownClicked() ) );
    pButtonsLayout->insertWidget( 3, pButtonDetailDown );
    pButtonDetailDown->setText( _("Bajar") );
}

FrmEditRecDetail::~FrmEditRecDetail()
{
    _GONG_DEBUG_PRINT( 3, Xtring( "Deleting " ) + name() );
}

// All this is to avoid this form to get the CTRL+M key press of another field
void FrmEditRecDetail::slotFocusChanged(DataTable* dt, QFocusEvent* ev)
{
    if( ev->reason() == Qt::PopupFocusReason
            || ev->reason() == Qt::MenuBarFocusReason )
        return;
    if( ev->gotFocus() ) {
        pMenuRecord->setEnabled( true );
        pMenuView->setEnabled(true);
        updateStatus(); // To restore the enabled menus
    } else {
        pMenuRecord->setEnabled(false);
        // This would be enough, but the shortcuts still work
        pMenuRecordAdd->setEnabled(false);
        pMenuRecordModify->setEnabled(false);
        pMenuRecordDelete->setEnabled(false);
        pMenuRecordPrint->setEnabled(false);
        pMenuRecordView->setEnabled(false);
        pMenuRecordDup->setEnabled(false);
        pMenuRecordReopen->setEnabled(false);
        pMenuView->setEnabled(false);
    }
}

void FrmEditRecDetail::slotDetailDownClicked()
{
    int row = pDataTable->currentRow();
    uint col = pDataTable->currentColumn();
    if( row < pDataTable->numRows() - 1  ) {
        dbRecord *tmp = pDetalles->at(row);
        (*pDetalles)[row] = pDetalles->at(row + 1);
        (*pDetalles)[row + 1] = tmp;
        refresh();
        pDataTable->setCurrentCell( row + 1, col );
        getFrmMaster()->updateFromDetails( this );
    }
}

void FrmEditRecDetail::slotDetailUpClicked()
{
    int row = pDataTable->currentRow();
    uint col = pDataTable->currentColumn();
    if( row > 0  ) {
        dbRecord *tmp = pDetalles->at(row);
        (*pDetalles)[row] = pDetalles->at(row - 1);
        (*pDetalles)[row - 1] = tmp;
        refresh();
        pDataTable->setCurrentCell( row - 1, col );
        getFrmMaster()->updateFromDetails( this );
    }
}

void FrmEditRecDetail::updateStatus( bool callbehaviors )
{
    FrmEditRecMaster::updateStatus( callbehaviors );
    pMenuTable->setEnabled( false );
    pushRemoveFilter->setVisible( false );
    if( pDataTable && pDataTable->isVisible() && !isReadOnly() ) {
        bool down = false;
        bool up = false;
        if( pDataTable->currentRow() > 0 )
            up = true;
        if( pDataTable->currentRow() < pDataTable->numRows() - 1 )
            down = true;
        pButtonDetailDown->setEnabled( down );
        pButtonDetailUp->setEnabled( up );
        pButtonDetailDown->setVisible( true );
        pButtonDetailUp->setVisible( true );
    } else {
        pButtonDetailDown->setVisible( false );
        pButtonDetailUp->setVisible( false );
    }
}


/**
 * @brief Reimplemented to add a detail if the datamodel is empty.
 *
 * This could be done in the derived edit form, but if it is on a QTabWidget, it is shown
 * when the tab is shown, not at constructor or scatter time, but at the show event.
 * This is the only place inside the justShown where this can be done.
 *
 * Note that this is not necessary for master records, only in details where a detail is
 * needed to start the edition.
 *
 * @return void
 **/
void FrmEditRecDetail::restoreBrowseStatus()
{
    FrmEditRecMaster::restoreBrowseStatus();
    if( getFrmMaster() )
        setReadOnly( getFrmMaster()->isReadOnly() );
    addDetailIfNeeded( false );
}

void FrmEditRecDetail::initMenus()
{
    FrmEditRecMaster::initMenus();
    if ( fromGUI(pMenuRecord->title()) != getRecord()->getTableDefinition()->getDescSingular() )
        pMenuRecord->setTitle( toGUI ( getRecord()->getTableDefinition()->getDescPlural().c_str() ) );
    pMenuRecordReopen->setEnabled( false );
}

void FrmEditRecDetail::clearRecord(bool duplicating)
{
    getRecord()->setNew(true);
}

// El id es en realidad el número de columna
bool FrmEditRecDetail::read(dbRecordID id)
{
    if( (int)id == -1 || id >= pDetalles->size() )
        return false;
    else {
        pDetalles->getRecord( id )->readRelated( false );
        getRecord()->copyRecord( pDetalles->getRecord( id ) );
        return true;
    }
}

bool FrmEditRecDetail::remove()
{
    if( pDetalles->size() ) {
        getDetalles()->erase( getDetalles()->begin() + mNumDetalle );
        return true;
    }
    else
        return false;
}

/*
	Como estamos editando un dbRecordList, los registros no tienen ID cuando se crean nuevos,
	por lo tanto, su ID va a ser la posición en la tabla
 */
dbRecordID FrmEditRecDetail::getTableRecID() const
{
    return mNumDetalle;
}


/**
	Details are not saved to the database, they will be saved by the master form
*/
bool FrmEditRecDetail::save()
{
    pDetalles->getRecord( mNumDetalle )->copyRecord ( getRecord() );
    return true;
}

// A pesar de ser virtual, esta función no llama a la clase base
void FrmEditRecDetail::beginEdit(DataTable *dt, EditMode newmode,
                                 dbApplication::EditFlags ediflags, const Xtring &fldtext)
{
    DBAPP->waitCursor(true);
    if( newmode != DataTable::deleting && newmode != DataTable::selecting )
        addDetailIfNeeded( false );
    int row = dt->currentRow();
    if( mMustRead )
        read( row );
    dbRecordPermissions r = getPermissions();
    if( newmode == DataTable::defaulteditmode )
    {
        // Borrar nunca está por defecto
        if( mChoosing )
            accept();
        else if ( r.canAdd && row >= 0 && pDetalles->getRecord(row)->isEmpty() )
            // Empty details can be treated as new, but they are not new by default
            newmode = DataTable::inserting;
        else if( r.canEdit )
            newmode = DataTable::updating;
        else if ( r.canAdd )
            newmode = DataTable::inserting;
        else
            newmode = DataTable::selecting;
    }
    if( getFrmMaster()->isSelecting() ) {
        pMenuRecordAdd->setEnabled(false);
        pMenuRecordDup->setEnabled(false);
        pMenuRecordModify->setEnabled(false);
        pMenuRecordDelete->setEnabled(false);
        if( newmode == DataTable::deleting )
			DBAPP->showOSD( _("No se puede borrar detalles al visualizar"), getTitle() );
        else if( newmode == DataTable::inserting )
			DBAPP->showOSD( _("No se puede añadir detalles al visualizar"), getTitle() );
        else if( newmode == DataTable::updating )
			DBAPP->showOSD( _("No se puede modificar detalles al visualizar"), getTitle() );
        r.canDelete = false;
        r.canAdd = false;
        r.canEdit = false;
        newmode = DataTable::selecting;
    }
    // Clonar el formulario. Atención, el nuevo formulario es detailfrm, no this
    if( canBeginEdit( newmode ) ) {
        if( newmode == DataTable::inserting )
            mNumDetalle = row = getDetalles()->size() -1 ;
        FrmEditRecDetail *detailfrm = static_cast<FrmEditRecDetail *>(
                                          DBAPP->createEditDetailForm(getFrmMaster(), row, pDetalles->getRecord(row),
                                                  mDetallesTableName, pDataModel, newmode, dbApplication::simpleEdition) );
        if( detailfrm ) {
            Xtring fldname;
            detailfrm->setFormFilter( getFormFilter() ); // Copy the form filter
            if( newmode == DataTable::deleting ) {
                detailfrm->show();
                if( msgYesNo( detailfrm,
                              Xtring::printf(_("¿Estás seguro/a de borrar %s?\n"),
                                             DBAPP->getTableDescSingular(pRecord->getTableName(), "esta").c_str()), false) )
                    detailfrm->accept();
                else
                    detailfrm->cancel();
            } else {
                fldname = dt->getDataModel()->getColName( dt->indexOf(dt->currentColumn()) );
                if( !fldname.isEmpty() ) {
                    detailfrm->pFocusWidget = detailfrm->findControl( Xtring(fldname).replace(".", "_" ) );
                    if( detailfrm->pFocusWidget ) {
                        if( !fldtext.isEmpty() )
                            detailfrm->setFocusWidgetText( fldtext );
                    }
                }
                getFrmMaster()->gather(); // So that the detail form can access the master record's fields
                DBAPP->resetCursor();
                detailfrm->showModalFor(getFrmMaster(), true, true ); // Centered
                DBAPP->waitCursor();
            }
            setWiseFocus(dt);
            int c = dt->currentColumn();
            if( detailfrm->isSaved() ) {
                getFrmMaster()->setEdited( true );
                addDetailIfNeeded( detailfrm->mMustRefresh );
                getFrmMaster()->updateFromDetails( this );
				if( newmode == DataTable::inserting )
					row = dt->numRows() - 1;
				if( newmode == DataTable::updating )
					row ++; // Ir a la siguiente para facilitar modificación hacia abajo
            }
            if (row >= dt->numRows() ) row = dt->numRows() - 1;
			if( row < 0 ) row = 0;
            dt->setCurrentCell( row, c );
            dt->ensureCellVisible( row + 1, c );
            delete detailfrm;
        }
    }
    DBAPP->resetCursor();
}


void FrmEditRecDetail::addDetailIfNeeded( bool forcerefresh )
{
    if ( pDataTable->getDataModel() == 0 )
        return;
    int r = pDataTable->currentRow(), c = pDataTable->currentColumn();
    bool add = !isReadOnly();
    add &= getFrmMaster()->isEditing();
    if( add && pDetalles->size() > 0 )
        add = !pDetalles->getRecord( pDataModel->getRowCount() -1 )->isEmpty();
    if( add ) {
        dbRecord *rec = DBAPP->createRecord( getRecord()->getTableName(), 0, getRecord()->getUser() );
        pDetalles->addRecord( rec );
        pDataTable->refresh( DataTable::RefreshData );
        pDataTable->setCurrentCell( pDetalles->size()-1, 1 );
    } else if( forcerefresh ) {
        pDataTable->refresh( DataTable::RefreshData );
        pDataTable->setCurrentCell( r, c );
    }
}

void FrmEditRecDetail::keyPressEvent( QKeyEvent* e )
{
    if ( e->key() == Qt::Key_F12 && isBrowsing() ) {
        if( getFrmMaster() ) {
            getFrmMaster()->keyPressEvent( e );
            return;
        }
    }
    FrmBase::keyPressEvent( e );
}

}

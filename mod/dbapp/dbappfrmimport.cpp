#include <gonggettext.h>
#include "dbappmainwindow.h"
#include "dbappdbapplication.h"
#include "dbappdbmodule.h"
#include "dbappfrmimport.h"

namespace gong {

FrmImport::FrmImport(QWidget* parent, const char* name, WidgetFlags fl)
    : GuiFrmImport( parent, name, fl), mWorking(false)
{
    editFicheroFugit->setText( getenv("HOME") );
    ImportViewItem *groupItem, *newItem;
    // Recorrer la lista de fin a principio, para que salga ordenada de principio a fin
    for( XtringList::reverse_iterator it = DBAPP->getMasterTables().rbegin();
            it != DBAPP->getMasterTables().rend();
            ++it ) {
        dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( *it );
        if( tbldef ) {
            groupItem = static_cast<ImportViewItem *>(treeImport->findItem(toGUI(*it), 0));
            if( !groupItem ) {
                groupItem = new ImportViewItem ( treeImport, groupItem, *it, QString::fromUtf8(tbldef->getDescPlural().c_str()) );
                groupItem->setOn(true);
            }
            newItem = new ImportViewItem ( groupItem, *it, toGUI( _("Todos los campos")) );
            newItem->setOn(true);
            for( unsigned int nf=0; nf<tbldef->getFieldCount(); nf++ )
            {
                dbFieldDefinition *fldef = tbldef->getFieldDefinition( nf );
                Xtring fldname = fldef->getName().upper();
                Xtring flddesc = fldef->getDescription();
                if( !flddesc.isEmpty() && !fldef->isCalculated() && fldname != "ID" ) {
                    newItem = new ImportViewItem ( groupItem, newItem, fldname,
                                                   QString::fromUtf8(flddesc.c_str()) );
                    newItem->setOn(true);
                }
            }
        }
    }
}


void FrmImport::next()
{
    switch( indexOf( currentPage() ) )
    {
    case 0:  // Tipo de fichero
        if( editFicheroFugit->text().isEmpty() )
        {
            FrmBase::msgError(this,
                              _("Por favor, elija el fichero FUGIT que contiene los datos a Import"));
            return;
        }
        Q3Wizard::next();
        break;
    case 1:  // Opciones de importacion
        Q3Wizard::next();
        editResumen->clear();
        DBAPP->processEvents();
        struct FugitImporter::ImportOptions io = getImportOptions();
        FugitImporter fi( getImportTablesList(), editDescripcion->toString(), 1, io, editResumen );
        DBAPP->waitCursor(true);
        cancelButton()->setEnabled( false );
        backButton()->setEnabled( false );
        mWorking = true;
        if( !fi.parseFile(editFicheroFugit->toString().c_str()) )
        {
            FrmBase::msgError(this,
                              _("Lea los mensajes para ver el error"));
            finishButton()->setEnabled(false);
        }
        else
        {
            addMessage( _("\n=== Comprobación finalizada. Pulse el botón 'Terminar' para realizar la importación definitivamente\n") );
            finishButton()->setEnabled(true);
        }
        mWorking = false;
        cancelButton()->setEnabled( true );
        backButton()->setEnabled( true );
        break;
    }
    DBAPP->resetCursor();
}

void FrmImport::reject()
{
    if( !mWorking )
        QDialog::reject();
}

void FrmImport::accept()
{
    editResumen->clear();
    struct FugitImporter::ImportOptions io = getImportOptions();
    FugitImporter fi( getImportTablesList(), editDescripcion->toString(), 2, io, editResumen );
    DBAPP->waitCursor();
    cancelButton()->setEnabled( false );
    backButton()->setEnabled( false );
    mWorking = true;
    if( fi.parseFile(editFicheroFugit->toString().c_str()) )
    {
        FrmBase::msgOk(this,
                       _("El proceso de importación ha terminado correctamente"));
        cancelButton()->setText( "&Cerrar" );
        finishButton()->setEnabled(false);
        backButton()->setEnabled(false);
		if( DBAPP->getMainWindow() ) 
			DBAPP->getMainWindow()->refreshByName( Xtring::null, Xtring::null );
    }
    else
    {
        FrmBase::msgError(this,
                          Xtring::printf("El fichero FUGIT %s no se ha encontrado. Pulse el boton Atras y elija otro fichero",
                                         (const char *)editFicheroFugit->text()) );
    }
    mWorking = false;
    cancelButton()->setEnabled( true );
    backButton()->setEnabled( true );
    DBAPP->resetCursor();
}

void FrmImport::addMessage(const Xtring &message)
{
    static int retardo = 0;
    editResumen->appendPlainText(toGUI(message));
    if( retardo++ == 100 )
    {
        retardo = 0;
        DBAPP->processEvents();
    }
}


FugitImporter::ImportOptions FrmImport::getImportOptions()
{
    FugitImporter::ImportOptions impopt;
    impopt.mExiste = FugitImporter::ImportOptions::EXISTE_IMPORTAR;
    if( comboOtroEjercicio->currentItem() == 0 )
    {
        impopt.mOtroEjercicio = FugitImporter::ImportOptions::OTROEJER_ABORTAR;
    }
    else if( comboOtroEjercicio->currentItem() == 1 )
    {
        impopt.mOtroEjercicio = FugitImporter::ImportOptions::OTROEJER_IMPORTAR;
    }
    else
    {
        impopt.mOtroEjercicio = FugitImporter::ImportOptions::OTROEJER_IGNORAR;
    } /// \todo {0.3.4} PREGUNTAR
    return impopt;
}


XtringList FrmImport::getImportTablesList() const
{
    XtringList list;
    ImportViewItem *current = static_cast<ImportViewItem *>(treeImport->firstChild());
    while( current )
    {
        if( current->isOn() )
        {
            list << current->getValue();
        }
        current = static_cast<ImportViewItem *>(current->nextSibling());
    }
    return list;
}

} // namespace gong

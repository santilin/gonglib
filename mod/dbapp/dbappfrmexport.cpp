
#include <fstream>
#include <cerrno>
#include <memory>
#include <gonggettext.h>
#include <gonglist.h>
#include "dbappdbapplication.h"
#include "dbappfrmexport.h"

///\todo {0.4} Mostrar proceso de exportación, número de registros por tabla

#define FUGIT_VERSION "0.1"

namespace gong {

FrmExport::FrmExport( QWidget* parent, const char* name, WidgetFlags fl )
    : GuiFrmExport( parent, name, fl )
{
    ExportViewItem *groupItem;
    _GONG_DEBUG_PRINT(0, DBAPP->getMasterTables().join("\n") );
    // Recorrer la lista de fin a principio, para que salga ordenada de principio a fin
    for( XtringList::reverse_iterator it = DBAPP->getMasterTables().rbegin();
            it != DBAPP->getMasterTables().rend();
            ++it ) {
        _GONG_DEBUG_PRINT(0, *it);
        dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( *it );
        if( tbldef ) {
            groupItem = static_cast<ExportViewItem *>(treeExport->findItem(toGUI(*it), 0));
            if( !groupItem ) {
                groupItem = new ExportViewItem( treeExport, groupItem, *it, toGUI(tbldef->getDescPlural().c_str()) );
                groupItem->setOn(true);
            }
        }
    }
}

void FrmExport::accept()
{
    Xtring filename = txtDestino->toString();
    std::ifstream in(filename.c_str());
    if( in ) {
        if( !FrmBase::msgYesNo(this,
                               Xtring::printf( _("El fichero %s ya existe. ¿Desea sobreescribirlo?"),
                                               filename.c_str() ) ) ) {
            in.close();
            return;
        }
        in.close();
    }
    std::ofstream out(filename.c_str());
    if( !out ) {
        FrmBase::msgError(this, strerror( errno ) );
        return;
    } else {
        DBAPP->waitCursor(true);
        out << "<?xml version=\"1.0\" encoding = \"UTF-8\"?>\n"
            "<!DOCTYPE FUGIT>\n"
            "<FUGIT version='" << FUGIT_VERSION << "' generator='gonglib " << FUGIT_VERSION <<
            "' date='" << Date::currentDate().toString() << "'>\n";
        ExportViewItem *current = static_cast<ExportViewItem *>(treeExport->firstChild());
        while( current )
        {
            if( current->isOn() )
            {
                doExport( current, out, current->getValue() );
            }
            current = static_cast<ExportViewItem *>(current->nextSibling());
        }
        out << "</FUGIT>\n";
        out.close();
        FrmBase::msgOk(this, _("La exportación ha finalizado correctamente") );
        DBAPP->resetCursor();
    }
}


void FrmExport::doExport(ExportViewItem *current, std::ofstream &stream, const Xtring &tablename)
{
    dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablename );
    if( tbldef ) {
        stream << "<FICHERO-" << tablename.upper() << ">\n";
        dbFieldDefinition *fldcode = tbldef->findFieldByFlags( dbFieldDefinition::CODE );
        if( !fldcode )
            fldcode = tbldef->getFieldDefinition(1);
        Xtring selectall = "SELECT ID FROM " + tablename;
        Xtring orderby = tbldef->getFullFldName(fldcode->getName());
        dbRecord *r = DBAPP->createRecord(tablename);
        selectall += r->getFilter( "WHERE" );
        selectall += " ORDER BY " + orderby;
        std::auto_ptr<dbResultSet> rs( r->getConnection()->select( selectall ) );
        while( rs->next() ) {
            r->read( rs->toInt(0) );
            stream << r->toString(TOSTRING_FUGIT) << "\n";
        }
        delete r;
#if 0
        // Lista de campos a Export
        Xtring flds;
        ExportViewItem *flditem = static_cast<ExportViewItem *>(current->firstChild());
        if( !flditem->isChecked() )
        {
            for( flditem = static_cast<ExportViewItem *>(flditem->nextSibling());
                    flditem;
                    flditem = static_cast<ExportViewItem *>(flditem->nextSibling()) )
            {
                if( flditem->isChecked() )
                {
                    if( !flds.isEmpty() )
                        flds += "|";
                    flds += flditem->getValue().lower();
                }
            }
        }
        flds = stringReplace(flds, ".", "\\.");
        QRegExp reg = flds;
        reg.setCaseSensitive(false);
        while( q.next() )
        {
            GSqlRecord *record = GongDatabase::getTable(tablename)->createRecord(&q);
            if( tablename == "proyecto" )
                static_cast<RecProyecto *>(record)->addMiembroRelation();
            record->readRelated(&qrel);
            stream << record->toString(GRECORDSTRINGFORMAT_FUGIT, reg) << "\n";
            delete record;
        }
#endif
    }
    stream << "</FICHERO-" << tablename.upper() << ">\n\n";
}

} // namespace gong

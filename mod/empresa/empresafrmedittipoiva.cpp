
/*<<<<<COPYLEFT*/
/** @file empresafrmedittipoiva.cpp Fichero de edición de tipos de IVA
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de tipos de IVA
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD IVA double - leftvalor
// FIELD Recargo double - leftvalor
// FIELD Notas text
// TYPE FrmEditRecMaster empresa::TipoIVA validCodeAndDesc IncCode CodeNotFound
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITTIPOIVA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "empresafrmedittipoiva.h"
/*>>>>>FRMEDITTIPOIVA_INCLUDES*/

namespace gong {
namespace empresa {

/*<<<<<FRMEDITTIPOIVA_CONSTRUCTOR*/
FrmEditTipoIVA::FrmEditTipoIVA(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
                               EditMode editmode, dbApplication::EditFlags editflags,
                               QWidget *parent, const char* name, WidgetFlags fl )
    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
    if ( !name )
        setName( "FrmEditTipoIVA" );
    /*>>>>>FRMEDITTIPOIVA_CONSTRUCTOR*/
    /*<<<<<FRMEDITTIPOIVA_INIT_CONTROLS*/
    QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
    QHBoxLayout *leftvalorLayout = new QHBoxLayout(0, 0, 6, "leftvalorLayout");
    QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
    editCodigo = addEditField( pControlsFrame, "TIPOIVA", "CODIGO", codigoLayout );
    editNombre = addEditField( pControlsFrame, "TIPOIVA", "NOMBRE", codigoLayout );
    editIVA = addEditField( pControlsFrame, "TIPOIVA", "IVA", leftvalorLayout );
    editRecargo = addEditField( pControlsFrame, "TIPOIVA", "RECARGO", leftvalorLayout );
    editNotas = addTextField( pControlsFrame, "TIPOIVA", "NOTAS", notasLayout );
    pControlsLayout->addLayout( codigoLayout );
    pControlsLayout->addLayout( leftvalorLayout );
    alignLayout( leftvalorLayout, true );
    pControlsLayout->addLayout( notasLayout );
    /*>>>>>FRMEDITTIPOIVA_INIT_CONTROLS*/
}

void FrmEditTipoIVA::scatterFields()
{
    /*<<<<<FRMEDITTIPOIVA_SCATTER*/
    editCodigo->setText(getRecTipoIVA()->getValue("CODIGO").toInt());
    if( isEditing() && (pFocusWidget == 0) )
        pFocusWidget = editCodigo;
    editNombre->setText(getRecTipoIVA()->getValue("NOMBRE").toString());
    editIVA->setText(getRecTipoIVA()->getValue("IVA").toDouble());
    editRecargo->setText(getRecTipoIVA()->getValue("RECARGO").toDouble());
    editNotas->setText(getRecTipoIVA()->getValue("NOTAS").toString());
    if( isInserting() && editCodigo->toInt() == 0 ) {
        editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
    }
    if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
        if( DBAPP->codeNotFound().toInt() != 0 ) {
            editCodigo->setText( DBAPP->codeNotFound() );
            editCodigo->setJustEdited( true );
        } else {
            editNombre->setText( DBAPP->codeNotFound() );
            editNombre->setJustEdited( true );
        }
    }
    /*>>>>>FRMEDITTIPOIVA_SCATTER*/

}
void FrmEditTipoIVA::gatherFields()
{
    /*<<<<<FRMEDITTIPOIVA_GATHER*/
    getRecTipoIVA()->setValue( "CODIGO", editCodigo->toInt());
    getRecTipoIVA()->setValue( "NOMBRE", editNombre->toString());
    getRecTipoIVA()->setValue( "IVA", editIVA->toDouble());
    getRecTipoIVA()->setValue( "RECARGO", editRecargo->toDouble());
    getRecTipoIVA()->setValue( "NOTAS", editNotas->toString());
    /*>>>>>FRMEDITTIPOIVA_GATHER*/
}
void FrmEditTipoIVA::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITTIPOIVA_VALIDATE*/
    bool v=true;
    if( !isvalid )
        isvalid = &v;
    ValidResult *validresult = ( ir ? ir : new ValidResult() );
    if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
        *isvalid = false;
    if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
        if( !sender )
            *isvalid = false;
    /*>>>>>FRMEDITTIPOIVA_VALIDATE*/
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITTIPOIVA_FIN*/
} // namespace empresa
} // namespace gong
/*>>>>>FRMEDITTIPOIVA_FIN*/


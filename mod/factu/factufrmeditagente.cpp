/*<<<<<COPYLEFT*/
/** @file factufrmeditagente.cpp Fichero de edición de agentes
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
// COPYLEFT Fichero de edición de agentes
// FIELD Codigo int - codigo
// FIELD RazonSocial string - codigo
// FIELD Notas text - notas
// TYPE FrmEditRecMaster factu::Agente IncCode
/*>>>>>MODULE_INFO*/
#include <gongpushbutton.h>
/*<<<<<FRMEDITAGENTE_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditagente.h"
/*>>>>>FRMEDITAGENTE_INCLUDES*/
#include <contactosfrmeditcontactobehavior.h>

namespace gong {
namespace factu {

/*<<<<<FRMEDITAGENTE_CONSTRUCTOR*/
FrmEditAgente::FrmEditAgente(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
                             EditMode editmode, dbApplication::EditFlags editflags,
                             QWidget *parent, const char* name, WidgetFlags fl )
    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
    if ( !name )
        setName( "FrmEditAgente" );
    /*>>>>>FRMEDITAGENTE_CONSTRUCTOR*/
    QTabWidget *pFrameContactos = new QTabWidget(pControlsFrame);
    new QVBoxLayout( pFrameContactos );
    pFrameContactos->setObjectName( "FrameContactos" );
    /*<<<<<FRMEDITAGENTE_INIT_CONTROLS*/
    QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
    QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
    editCodigo = addEditField( pControlsFrame, "AGENTE", "CODIGO", codigoLayout );
    editRazonSocial = addEditField( pControlsFrame, "AGENTE", "RAZONSOCIAL", codigoLayout );
    editNotas = addTextField( pControlsFrame, "AGENTE", "NOTAS", notasLayout );
    pControlsLayout->addLayout( codigoLayout );
    pControlsLayout->addWidget( pFrameContactos );
    pControlsLayout->addLayout( notasLayout );
    /*>>>>>FRMEDITAGENTE_INIT_CONTROLS*/
// {capel} Añadir detrás de addLayout( codigoLayout )
//	pControlsLayout->addWidget( pFrameContactos );
    pEditContactoBehavior = new contactos::FrmEditContactoBehavior( this, pFrameContactos, editRazonSocial );
    addBehavior( pEditContactoBehavior );
}

void FrmEditAgente::scatterFields()
{
    // Quitar el valor por defecto del cif del contacto
    if( isDuplicating() )
        getRecAgente()->setValue( "CODIGO", 0 );
    pEditContactoBehavior->setTabOrders( editRazonSocial, editNotas);
    /*<<<<<FRMEDITAGENTE_SCATTER*/
    editCodigo->setText(getRecAgente()->getValue("CODIGO").toInt());
    if( isEditing() && (pFocusWidget == 0) )
        pFocusWidget = editCodigo;
    editRazonSocial->setText(getRecAgente()->getValue("RAZONSOCIAL").toString());
    editNotas->setText(getRecAgente()->getValue("NOTAS").toString());
    if( isInserting() && editCodigo->toInt() == 0 ) {
        editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
    }
    /*>>>>>FRMEDITAGENTE_SCATTER*/
    if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
        if( DBAPP->codeNotFound().toInt() != 0 ) {
            editCodigo->setText( DBAPP->codeNotFound() );
            editCodigo->setJustEdited( true );
        } else {
            editRazonSocial->setText( DBAPP->codeNotFound() );
            editRazonSocial->setJustEdited( true );
        }
    }
    pFocusWidget = editRazonSocial;
}

void FrmEditAgente::gatherFields()
{
    /*<<<<<FRMEDITAGENTE_GATHER*/
    getRecAgente()->setValue( "CODIGO", editCodigo->toInt());
    getRecAgente()->setValue( "RAZONSOCIAL", editRazonSocial->toString());
    getRecAgente()->setValue( "NOTAS", editNotas->toString());
    /*>>>>>FRMEDITAGENTE_GATHER*/
}
void FrmEditAgente::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITAGENTE_VALIDATE*/
    bool v=true;
    if( !isvalid )
        isvalid = &v;
    ValidResult *validresult = ( ir ? ir : new ValidResult() );
    if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
        *isvalid = false;
    /*>>>>>FRMEDITAGENTE_VALIDATE*/
    if( !validCodeAndDesc( sender, *validresult, editCodigo, editRazonSocial, "codigo", "razonsocial" ) )
        if( !sender )
            *isvalid = false;
    if( sender == editRazonSocial && editRazonSocial->isJustEdited()
            && validresult->countErrors() == 0 && pEditContactoBehavior->isCreating() ) {
        if( pEditContactoBehavior->getEditCIF()->toString().isEmpty()
                && pEditContactoBehavior->getEditNombre()->toString().isEmpty() )
            if( pEditContactoBehavior->setCIFAndLookForIt( editRazonSocial->toString() ) ) {
                DBAPP->showOSD( fromGUI( windowTitle() ),
                                Xtring::printf( _("Se ha encontrado %s para %s"),
                                                DBAPP->getTableDescSingular("CONTACTO", "una").c_str(),
                                                DBAPP->getTableDescSingular("CLIENTE", "esta").c_str()
                                              ) );
                editRazonSocial->setText( pEditContactoBehavior->getEditNombre()->toString() );
            }
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITAGENTE_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITAGENTE_FIN*/

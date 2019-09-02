/*<<<<<COPYLEFT*/
/** @file produccionfrmeditlotedet.cpp Fichero de edición de detalles de dotes
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de detalles de dotes
// FIELD Cantidad double
// FIELD Articulo_ID Reference(factu::Articulo,Codigo,Nombre,dbApplication::FindCodeInDesc)
// TYPE FrmEditRecDetail produccion::LoteDet
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITLOTEDET_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "produccionfrmeditlotedet.h"
/*>>>>>FRMEDITLOTEDET_INCLUDES*/

namespace gong {
namespace produccion {

/*<<<<<FRMEDITLOTEDET_CONSTRUCTOR*/
FrmEditLoteDet::FrmEditLoteDet(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditLoteDet" );
/*>>>>>FRMEDITLOTEDET_CONSTRUCTOR*/
/*<<<<<FRMEDITLOTEDET_INIT_CONTROLS*/
	QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
	QHBoxLayout *articulo_idLayout = new QHBoxLayout(0, 0, 6, "articulo_idLayout");
	editCantidad = addEditField( pControlsFrame, "LOTEDET", "CANTIDAD", cantidadLayout );

	searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", articulo_idLayout );
	pushArticuloCodigo = searchArticuloCodigo->getButton();
	connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
	editArticuloCodigo = searchArticuloCodigo->getEditCode();
	editArticuloNombre = searchArticuloCodigo->getEditDesc();
	pControlsLayout->addLayout( cantidadLayout );
	pControlsLayout->addLayout( articulo_idLayout );
/*>>>>>FRMEDITLOTEDET_INIT_CONTROLS*/
}

void FrmEditLoteDet::scatterFields()
{
/*<<<<<FRMEDITLOTEDET_SCATTER*/
	editCantidad->setText(getRecLoteDet()->getValue("CANTIDAD").toDouble());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCantidad;
	scatterArticulo();
/*>>>>>FRMEDITLOTEDET_SCATTER*/
}

void FrmEditLoteDet::gatherFields()
{
/*<<<<<FRMEDITLOTEDET_GATHER*/
	getRecLoteDet()->setValue( "CANTIDAD", editCantidad->toDouble());
	getRecLoteDet()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
/*>>>>>FRMEDITLOTEDET_GATHER*/
}

void FrmEditLoteDet::scatterArticulo()
{
/*<<<<<FRMEDITLOTEDET_SCATTER_ARTICULO*/
	editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
	editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
/*>>>>>FRMEDITLOTEDET_SCATTER_ARTICULO*/
}
void FrmEditLoteDet::pushArticuloCodigo_clicked()
{
/*<<<<<FRMEDITLOTEDET_PUSH_ARTICULO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchArticuloCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editArticuloCodigo->setJustEdited( false );
			editArticuloCodigo->setCancelling();
			if( DBAPP->choose(this, getRecArticulo(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterArticulo();
				editArticuloCodigo->setJustEdited( true );
				setWiseFocus(editArticuloCodigo);
			}
			break;
		case 'M':
			{
				if( getRecArticulo()->getRecordID() ) {
					editArticuloCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecArticulo(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editArticuloCodigo->setJustEdited( true );
						scatterArticulo();
					}
				setWiseFocus(editArticuloCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecArticulo()->getRecordID() != 0 ) {
					editArticuloCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecArticulo(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				factu::RecArticulo *tmprec = static_cast<factu::RecArticulo *>(DBAPP->createRecord( "Articulo" ));
				editArticuloCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editArticuloCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editArticuloCodigo->setJustEdited( true );
					getRecArticulo()->copyRecord( tmprec );
					scatterArticulo();
				}
				setWiseFocus(editArticuloCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITLOTEDET_PUSH_ARTICULO_CODIGO_CLICKED*/
}
void FrmEditLoteDet::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITLOTEDET_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editArticuloCodigo )
		pushArticuloCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITLOTEDET_SPECIALACTION*/
}


void FrmEditLoteDet::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITLOTEDET_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( focusWidget() != pushArticuloCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editArticuloCodigo, editArticuloNombre,
		getRecArticulo(), "CODIGO", "NOMBRE", Xtring::null, dbRecord::SeekCodeFlags( dbApplication::FindCodeInDesc )) )
		scatterArticulo();
/*>>>>>FRMEDITLOTEDET_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITLOTEDET_FIN*/
} // namespace produccion
} // namespace gong
/*>>>>>FRMEDITLOTEDET_FIN*/



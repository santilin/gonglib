/*<<<<<COPYLEFT*/
/** @file produccionfrmeditarticulocomponente.cpp Fichero de edición de productos de artículos
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
// COPYLEFT Fichero de edición de productos de artículos
// FIELD Orden int - pre
// FIELD Cantidad double - cantidad
// FIELD Articulo_ID Reference(Articulo,Codigo,Nombre,dbApplication::FindCodeInDesc) - cantidad
// FIELD Coste money - precios
// FIELD Importe money - precios
// FIELD Notas text - notas
// TYPE FrmEditRecDetail produccion::ArticuloComponente
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITARTICULOCOMPONENTE_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "produccionfrmeditarticulocomponente.h"
/*>>>>>FRMEDITARTICULOCOMPONENTE_INCLUDES*/
#include "produccionrecarticulo.h"
#include <empresamodule.h>

namespace gong {
namespace produccion {

/*<<<<<FRMEDITARTICULOCOMPONENTE_CONSTRUCTOR*/
FrmEditArticuloComponente::FrmEditArticuloComponente(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditArticuloComponente" );
/*>>>>>FRMEDITARTICULOCOMPONENTE_CONSTRUCTOR*/
/*<<<<<FRMEDITARTICULOCOMPONENTE_INIT_CONTROLS*/
	QHBoxLayout *preLayout = new QHBoxLayout(0, 0, 6, "preLayout");
	QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
	QHBoxLayout *preciosLayout = new QHBoxLayout(0, 0, 6, "preciosLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editOrden = addEditField( pControlsFrame, "ARTICULOCOMPONENTE", "ORDEN", preLayout );
	editCantidad = addEditField( pControlsFrame, "ARTICULOCOMPONENTE", "CANTIDAD", cantidadLayout );

	searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", cantidadLayout );
	pushArticuloCodigo = searchArticuloCodigo->getButton();
	connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
	editArticuloCodigo = searchArticuloCodigo->getEditCode();
	editArticuloNombre = searchArticuloCodigo->getEditDesc();
	editCoste = addEditField( pControlsFrame, "ARTICULOCOMPONENTE", "COSTE", preciosLayout );
	editImporte = addEditField( pControlsFrame, "ARTICULOCOMPONENTE", "IMPORTE", preciosLayout );
	editNotas = addTextField( pControlsFrame, "ARTICULOCOMPONENTE", "NOTAS", notasLayout );
	pControlsLayout->addLayout( preLayout );
	pControlsLayout->addLayout( cantidadLayout );
	pControlsLayout->addLayout( preciosLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITARTICULOCOMPONENTE_INIT_CONTROLS*/
}

void FrmEditArticuloComponente::scatterFields()
{
/*<<<<<FRMEDITARTICULOCOMPONENTE_SCATTER*/
	editOrden->setText(getRecArticuloComponente()->getValue("ORDEN").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editOrden;
	editCantidad->setText(getRecArticuloComponente()->getValue("CANTIDAD").toDouble());
	editCoste->setText(getRecArticuloComponente()->getValue("COSTE").toMoney());
	editImporte->setText(getRecArticuloComponente()->getValue("IMPORTE").toMoney());
	editNotas->setText(getRecArticuloComponente()->getValue("NOTAS").toString());
	scatterArticulo();
/*>>>>>FRMEDITARTICULOCOMPONENTE_SCATTER*/
	// No mostrar el artículo maestro
	getRecArticulo()->addFilter( "ARTICULO.ID<>"
		+ getRecArticulo()->getConnection()->toSQL( getFrmMaster()->getRecord()->getRecordID() ) );
	if( isDuplicating() || (editOrden->toInt() == 0 && isInserting()) ) {
		editOrden->setText( (int)mNumDetalle + 1 );
		pFocusWidget = editCantidad;
	}
}

void FrmEditArticuloComponente::gatherFields()
{
/*<<<<<FRMEDITARTICULOCOMPONENTE_GATHER*/
	getRecArticuloComponente()->setValue( "ORDEN", editOrden->toInt());
	getRecArticuloComponente()->setValue( "CANTIDAD", editCantidad->toDouble());
	getRecArticuloComponente()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
	getRecArticuloComponente()->setValue( "COSTE", editCoste->toMoney());
	getRecArticuloComponente()->setValue( "IMPORTE", editImporte->toMoney());
	getRecArticuloComponente()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITARTICULOCOMPONENTE_GATHER*/
}

void FrmEditArticuloComponente::scatterArticulo()
{
/*<<<<<FRMEDITARTICULOCOMPONENTE_SCATTER_ARTICULO*/
	editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
	editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
/*>>>>>FRMEDITARTICULOCOMPONENTE_SCATTER_ARTICULO*/
}
void FrmEditArticuloComponente::pushArticuloCodigo_clicked()
{
/*<<<<<FRMEDITARTICULOCOMPONENTE_PUSH_ARTICULO_CODIGO_CLICKED*/
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
				RecArticulo *tmprec = static_cast<RecArticulo *>(DBAPP->createRecord( "Articulo" ));
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
/*>>>>>FRMEDITARTICULOCOMPONENTE_PUSH_ARTICULO_CODIGO_CLICKED*/
}

void FrmEditArticuloComponente::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITARTICULOCOMPONENTE_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editArticuloCodigo )
		pushArticuloCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITARTICULOCOMPONENTE_SPECIALACTION*/
}

void FrmEditArticuloComponente::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITARTICULOCOMPONENTE_VALIDATE*/
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
/*>>>>>FRMEDITARTICULOCOMPONENTE_VALIDATE*/
	// Comprobar que el artículo añadido no contiene al artículo original en ninguno de sus componentes
	dbRecordID master_art_id = getFrmMaster()->getRecord()->getValue("ARTICULO.ID").toInt();
	if( getRecArticulo()->hasComponente( master_art_id ) ) {
		validresult->addError( _("Este componente o alguno de sus componentes ya incluye el artículo principal, por lo tanto no se puede añadir"),
									"CODIGO" );
		*isvalid = false;
	}
	if( *isvalid ) {
		bool actualiza = (sender && (sender == pFocusWidget) );
		double cantidad = editCantidad->toDouble();
		double coste = editCoste->toDouble();
		if ( sender == editCantidad && editCantidad->isJustEdited() ) {
			actualiza = true;
		}
		if ( sender == editArticuloCodigo && editArticuloCodigo->isJustEdited() ) {
			actualiza = true;
			coste = getRecArticulo()->getValue( "COSTE" ).toDouble();
		}
		if ( sender == editCoste && editCoste->isJustEdited() ) {
			actualiza = true;
			coste = editCoste->toDouble();
		}
		if( actualiza ) {
			editCoste->setText( coste );
			Money importe = Money( cantidad * coste,
				empresa::ModuleInstance->getDecimalesMoneda() );
			editImporte->setText( importe );
		}
	}
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}


/*<<<<<FRMEDITARTICULOCOMPONENTE_FIN*/
} // namespace produccion
} // namespace gong
/*>>>>>FRMEDITARTICULOCOMPONENTE_FIN*/

/*<<<<<COPYLEFT*/
/** @file produccionfrmeditarticulobehavior.cpp Behavior del formulario de familias
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
// COPYLEFT Behavior del formulario de familias
// FIELD Lote_ID Reference(Lote,Codigo,Descripcion,readOnly) tabProduccion
// FIELD ArticuloComponente FrmEditRecDetail tabProduccion
// TYPE FrmEditRecBehavior produccion::Articulo Q_OBJECT
/*>>>>>MODULE_INFO*/

#include <gongdbrecord.h>
/*<<<<<FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "produccionfrmeditarticulobehavior.h"
/*>>>>>FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include <factufrmeditarticulo.h>
#include "produccionreclote.h"

namespace gong {
namespace produccion {

void FrmEditArticuloBehavior::initGUI()
{
/*<<<<<FRMEDITARTICULOBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QWidget *tabProduccion = getOrCreateTab( "tabProduccion" );
	QVBoxLayout *tabProduccionLayout = static_cast<QVBoxLayout *>( tabProduccion->layout() );
	QHBoxLayout *lote_idLayout = new QHBoxLayout(0, 0, 6, "lote_idLayout");
	QHBoxLayout *articulocomponenteLayout = new QHBoxLayout(0, 0, 6, "articulocomponenteLayout");

	searchLoteCodigo = pTheForm->addSearchField( tabProduccion, "LOTE_ID", "LOTE", "CODIGO", "DESCRIPCION", lote_idLayout );
	pushLoteCodigo = searchLoteCodigo->getButton();
	connect( pushLoteCodigo, SIGNAL( clicked() ), this, SLOT( pushLoteCodigo_clicked() ) );
	editLoteCodigo = searchLoteCodigo->getEditCode();
	editLoteDescripcion = searchLoteCodigo->getEditDesc();

// frmDetails: ArticuloComponente
	QFrame *articulocomponenteFrame = new QFrame(pTheForm);
	articulocomponenteFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	articulocomponenteFrame->setLineWidth(4);
	QVBoxLayout *articulocomponenteFrameLayout = new QVBoxLayout(articulocomponenteFrame);
	RecArticuloComponente *articulocomponente = getRecArticulo()->getRecArticuloComponente(-1);
	dbViewDefinitionDict articulocomponenteviews;
	DBAPP->getDatabase()->getViewsForTable( articulocomponente, articulocomponenteviews );
	dbRecordListDataModel *articulocomponentedm =
		new dbRecordListDataModel(getRecArticulo()->getListArticuloComponente(), articulocomponente, articulocomponenteviews, "1=0");
	pFrmArticuloComponente = static_cast<FrmEditArticuloComponente *>(
		DBAPP->createEditDetailForm( static_cast<FrmEditRecMaster *>(pTheForm),
		-1, articulocomponente, "ARTICULOCOMPONENTE",articulocomponentedm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		tabProduccion) );
	pFrmArticuloComponente->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	articulocomponenteFrameLayout->addWidget( pFrmArticuloComponente );
	articulocomponenteLayout->addWidget(articulocomponenteFrame);
	tabProduccionLayout->addLayout( lote_idLayout );
	tabProduccionLayout->addLayout( articulocomponenteLayout );
/*>>>>>FRMEDITARTICULOBEHAVIOR_INITGUI*/
	(void)pControlsFrame; (void)pControlsLayout;
	pTheForm->setTabTitle( tabProduccion, _("Producción") );
}

void FrmEditArticuloBehavior::scatterFields( bool is_pre )
{
	if( is_pre ) return;
	pFrmArticuloComponente->addDetailIfNeeded();
/*<<<<<FRMEDITARTICULOBEHAVIOR_SCATTER*/
	scatterLote();
/*>>>>>FRMEDITARTICULOBEHAVIOR_SCATTER*/
}

void FrmEditArticuloBehavior::gatherFields()
{
/*<<<<<FRMEDITARTICULOBEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "LOTE_ID", getRecLote()->getRecordID() );
/*>>>>>FRMEDITARTICULOBEHAVIOR_GATHER*/
}

void FrmEditArticuloBehavior::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITARTICULOBEHAVIOR_SPECIALACTION*/
	if( sender == editLoteCodigo )
		pushLoteCodigo_clicked();
/*>>>>>FRMEDITARTICULOBEHAVIOR_SPECIALACTION*/
}

void FrmEditArticuloBehavior::updateFromDetails(FrmEditRecDetail* frmdetail)
{
	if( !frmdetail )
		return;
	Money coste = 0;
	dbRecordList *lcomp = pFrmArticuloComponente->getDetalles();
	for( dbRecordList::const_iterator it = lcomp->begin(); it != lcomp->end(); ++ it ) {
		RecArticuloComponente *componente = static_cast<RecArticuloComponente *>(*it);
		coste += componente->getValue( "COSTE" ).toMoney() * componente->getValue("CANTIDAD").toDouble();
	}
	LineEdit *editCoste = static_cast<LineEdit *>(pTheForm->findControl("ARTICULO.COSTESINIVA"));
	if( coste != editCoste->toMoney() ) {
// 		editCoste->setText( coste );
// 		static_cast<factu::FrmEditArticulo *>(pTheForm)->fixPrecios( "COSTESINIVA" );
		DBAPP->showOSD( pTheForm->getRecord()->toString( TOSTRING_CODE_AND_DESC ),
						Xtring::printf( _("El coste sin IVA a partir de los productos sería de %s"),
										coste.toString().c_str() ) );
	}
}

void FrmEditArticuloBehavior::validateFields(bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir)
{
	if( !is_pre ) return;
/*<<<<<FRMEDITARTICULOBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( pTheForm->focusWidget() != pushLoteCodigo) // To avoid triggering the validating if the button is pressed
	if( pTheForm->validSeekCode( sender, isvalid, *validresult, editLoteCodigo, editLoteDescripcion,
		getRecLote(), "CODIGO", "DESCRIPCION", Xtring::null, dbRecord::SeekCodeFlags( readOnly )) )
		scatterLote();
/*>>>>>FRMEDITARTICULOBEHAVIOR_VALIDATE*/
	if( !sender ) {
		dbRecordID articulo_id = pTheForm->getRecord()->getRecordID();
		if( articulo_id != 0 ) {
			dbRecordList *lcomp = pFrmArticuloComponente->getDetalles();
			for( dbRecordList::const_iterator it = lcomp->begin(); it != lcomp->end(); ++ it ) {
				dbRecord *componente = *it;
				if( componente->getRecordID()!= 0 && (componente->getRecordID() == articulo_id) ) {
					validresult->addError( _("No se puede incluir el artículo en sus componentes"),
						"LOTE_ID" );
					*isvalid = false;
				}
			}
		}
	}
	if( !sender && *isvalid ) {
		bool esproducto = pFrmArticuloComponente->getDetalles()->size() > 1
			|| ( pFrmArticuloComponente->getDetalles()->size() == 1
				&& !pFrmArticuloComponente->getDetalles()->getRecord(0)->isEmpty() );
		pTheForm->getRecord()->setValue( "ESPRODUCTO", esproducto );
		pTheForm->gatherFields();
	}
}

void FrmEditArticuloBehavior::scatterLote()
{
/*<<<<<FRMEDITARTICULOBEHAVIOR_SCATTER_LOTE*/
	editLoteCodigo->setText( getRecLote()->getValue("CODIGO") );
	editLoteDescripcion->setText( getRecLote()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITARTICULOBEHAVIOR_SCATTER_LOTE*/
}

void FrmEditArticuloBehavior::pushLoteCodigo_clicked()
{
/*<<<<<FRMEDITARTICULOBEHAVIOR_PUSH_LOTE_CODIGO_CLICKED*/
	char action = getControlKeyPressed();
	if( !pTheForm->isEditing() || searchLoteCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editLoteCodigo->setJustEdited( false );
			editLoteCodigo->setCancelling();
			if( DBAPP->choose(pTheForm, getRecLote(), 0, dbApplication::editNone, pTheForm ) ) {
				pTheForm->setEdited(true);
				scatterLote();
				editLoteCodigo->setJustEdited( true );
				pTheForm->setWiseFocus(editLoteCodigo);
			}
			break;
		case 'M':
			{
				if( getRecLote()->getRecordID() ) {
					editLoteCodigo->setJustEdited( false );
					if( DBAPP->editRecord(pTheForm,
							getRecLote(), 0, DataTable::updating,
								dbApplication::simpleEdition, pTheForm ) ) {
						editLoteCodigo->setJustEdited( true );
						scatterLote();
					}
					pTheForm->setWiseFocus(editLoteCodigo);
				}
			}
			break;
		case 'E':
			{
				editLoteCodigo->setJustEdited( false );
				DBAPP->createClient( DBAPP->createEditForm(static_cast<FrmEditRec *>(pTheForm),
					getRecLote(), 0, DataTable::selecting,
				dbApplication::simpleEdition, pTheForm ) );
			}
			break;
		case 'A':
			{
				RecLote *tmprec = static_cast<RecLote *>(DBAPP->createRecord( "Lote" ));
				editLoteCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editLoteCodigo->toString() );
				if( DBAPP->editRecord(pTheForm, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, pTheForm ) ) {
					editLoteCodigo->setJustEdited( true );
					getRecLote()->copyRecord( tmprec );
					scatterLote();
				}
				pTheForm->setWiseFocus(editLoteCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITARTICULOBEHAVIOR_PUSH_LOTE_CODIGO_CLICKED*/
// {capel} Quitar case 'A' y case 'M'
}

/*<<<<<FRMEDITARTICULOBEHAVIOR_FIN*/
} // namespace produccion
} // namespace gong
/*>>>>>FRMEDITARTICULOBEHAVIOR_FIN*/

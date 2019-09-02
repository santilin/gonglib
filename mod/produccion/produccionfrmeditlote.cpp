/*<<<<<COPYLEFT*/
/** @file produccionfrmeditlote.cpp Fichero de edición de lotes
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
// COPYLEFT Fichero de edición de lotes
// FIELD Codigo string
// FIELD Descripcion string
// FIELD FechaFabricacion date
// FIELD FechaCaducidad date
// FIELD LoteDet FrmEditRecDetail
// FIELD Cantidad double
// TYPE FrmEditRecMaster produccion::Lote
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITLOTE_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "produccionfrmeditlote.h"
/*>>>>>FRMEDITLOTE_INCLUDES*/

namespace gong {
namespace produccion {

/*<<<<<FRMEDITLOTE_CONSTRUCTOR*/
FrmEditLote::FrmEditLote(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditLote" );
/*>>>>>FRMEDITLOTE_CONSTRUCTOR*/
/*<<<<<FRMEDITLOTE_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *fechafabricacionLayout = new QHBoxLayout(0, 0, 6, "fechafabricacionLayout");
	QHBoxLayout *fechacaducidadLayout = new QHBoxLayout(0, 0, 6, "fechacaducidadLayout");
	QHBoxLayout *lotedetLayout = new QHBoxLayout(0, 0, 6, "lotedetLayout");
	QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
	editCodigo = addEditField( pControlsFrame, "LOTE", "CODIGO", codigoLayout );
	editDescripcion = addEditField( pControlsFrame, "LOTE", "DESCRIPCION", descripcionLayout );
	editFechaFabricacion = addEditField( pControlsFrame, "LOTE", "FECHAFABRICACION", fechafabricacionLayout );
	editFechaCaducidad = addEditField( pControlsFrame, "LOTE", "FECHACADUCIDAD", fechacaducidadLayout );

// frmDetails: LoteDet
	QFrame *lotedetFrame = new QFrame(this);
	lotedetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	lotedetFrame->setLineWidth(4);
	QVBoxLayout *lotedetFrameLayout = new QVBoxLayout(lotedetFrame);
	RecLoteDet *lotedet = getRecLote()->getRecLoteDet(-1);
	dbViewDefinitionDict lotedetviews;
	DBAPP->getDatabase()->getViewsForTable( lotedet, lotedetviews );
	dbRecordListDataModel *lotedetdm =
		new dbRecordListDataModel(getRecLote()->getListLoteDet(), lotedet, lotedetviews, "1=0");
	pFrmLoteDet = static_cast<FrmEditLoteDet *>(
		DBAPP->createEditDetailForm(this,
		-1, lotedet, "LOTEDET",lotedetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmLoteDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	lotedetFrameLayout->addWidget( pFrmLoteDet );
	lotedetLayout->addWidget(lotedetFrame);
	editCantidad = addEditField( pControlsFrame, "LOTE", "CANTIDAD", cantidadLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( descripcionLayout );
	pControlsLayout->addLayout( fechafabricacionLayout );
	pControlsLayout->addLayout( fechacaducidadLayout );
	pControlsLayout->addLayout( lotedetLayout );
	pControlsLayout->addLayout( cantidadLayout );
/*>>>>>FRMEDITLOTE_INIT_CONTROLS*/
}

void FrmEditLote::scatterFields()
{
	getRecord()->readRelated( false );
/*<<<<<FRMEDITLOTE_SCATTER*/
	editCodigo->setText(getRecLote()->getValue("CODIGO").toString());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editDescripcion->setText(getRecLote()->getValue("DESCRIPCION").toString());
	editFechaFabricacion->setText(getRecLote()->getValue("FECHAFABRICACION").toDate());
	editFechaCaducidad->setText(getRecLote()->getValue("FECHACADUCIDAD").toDate());
	editCantidad->setText(getRecLote()->getValue("CANTIDAD").toDouble());
/*>>>>>FRMEDITLOTE_SCATTER*/
	if( editFechaFabricacion->toDate().isNull() )
		editFechaFabricacion->setText( Date::currentDate() );
	if( isUpdating() ) {
		pFocusWidget = pFrmLoteDet;
	}
	pFrmLoteDet->addDetailIfNeeded();
}

void FrmEditLote::gatherFields()
{
/*<<<<<FRMEDITLOTE_GATHER*/
	getRecLote()->setValue( "CODIGO", editCodigo->toString());
	getRecLote()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecLote()->setValue( "FECHAFABRICACION", editFechaFabricacion->toDate());
	getRecLote()->setValue( "FECHACADUCIDAD", editFechaCaducidad->toDate());
	getRecLote()->setValue( "CANTIDAD", editCantidad->toDouble());
/*>>>>>FRMEDITLOTE_GATHER*/
}

void FrmEditLote::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITLOTE_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
/*>>>>>FRMEDITLOTE_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITLOTE_FIN*/
} // namespace produccion
} // namespace gong
/*>>>>>FRMEDITLOTE_FIN*/


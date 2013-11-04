/*<<<<<COPYLEFT*/
/** @file pagosfrmeditformapago.cpp Formulario de edición de formas de pago
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
// COPYLEFT Formulario de edición de formas de pago
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD TipoFormaPago comboint - tipo
// FIELD NumPlazos int - plazos
// FIELD DiasEntrePlazos int - plazos
// FIELD DiasPrimerPlazo int - plazos
// FIELD DtoEnFactura float - dto
// FIELD SubcuentaPago string - sub1 MODULE_INCLUDED(Contab)
// FIELD SubcuentaCobro string - sub2 MODULE_INCLUDED(Contab)
// FIELD Notas text - notas
// TYPE FrmEditRecMaster pagos::FormaPago validCodeAndDesc IncCode CodeNotFound
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITFORMAPAGO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "pagosfrmeditformapago.h"
/*>>>>>FRMEDITFORMAPAGO_INCLUDES*/
#include "pagosmodule.h"

namespace gong {
namespace pagos {

/*<<<<<FRMEDITFORMAPAGO_CONSTRUCTOR*/
FrmEditFormaPago::FrmEditFormaPago(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditFormaPago" );
/*>>>>>FRMEDITFORMAPAGO_CONSTRUCTOR*/

    /*<<<<<FRMEDITFORMAPAGO_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *tipoLayout = new QHBoxLayout(0, 0, 6, "tipoLayout");
	QHBoxLayout *plazosLayout = new QHBoxLayout(0, 0, 6, "plazosLayout");
	QHBoxLayout *dtoLayout = new QHBoxLayout(0, 0, 6, "dtoLayout");
	QHBoxLayout *sub1Layout = new QHBoxLayout(0, 0, 6, "sub1Layout");
	QHBoxLayout *sub2Layout = new QHBoxLayout(0, 0, 6, "sub2Layout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "FORMAPAGO", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "FORMAPAGO", "NOMBRE", codigoLayout );
	comboTipoFormaPago = addComboIntField( pControlsFrame, "FORMAPAGO", "TIPOFORMAPAGO", tipoLayout );
	editNumPlazos = addEditField( pControlsFrame, "FORMAPAGO", "NUMPLAZOS", plazosLayout );
	editDiasEntrePlazos = addEditField( pControlsFrame, "FORMAPAGO", "DIASENTREPLAZOS", plazosLayout );
	editDiasPrimerPlazo = addEditField( pControlsFrame, "FORMAPAGO", "DIASPRIMERPLAZO", plazosLayout );
	editDtoEnFactura = addEditField( pControlsFrame, "FORMAPAGO", "DTOENFACTURA", dtoLayout );
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaPago = addEditField( pControlsFrame, "FORMAPAGO", "SUBCUENTAPAGO", sub1Layout );
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaCobro = addEditField( pControlsFrame, "FORMAPAGO", "SUBCUENTACOBRO", sub2Layout );
}
#endif
	editNotas = addTextField( pControlsFrame, "FORMAPAGO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( tipoLayout );
	pControlsLayout->addLayout( plazosLayout );
	pControlsLayout->addLayout( dtoLayout );
	pControlsLayout->addLayout( sub1Layout );
	pControlsLayout->addLayout( sub2Layout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITFORMAPAGO_INIT_CONTROLS*/
}

void FrmEditFormaPago::scatterFields()
{
    /*<<<<<FRMEDITFORMAPAGO_SCATTER*/
	editCodigo->setText(getRecFormaPago()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecFormaPago()->getValue("NOMBRE").toString());
	comboTipoFormaPago->setCurrentItemByValue(getRecFormaPago()->getValue("TIPOFORMAPAGO").toInt());
	editNumPlazos->setText(getRecFormaPago()->getValue("NUMPLAZOS").toInt());
	editDiasEntrePlazos->setText(getRecFormaPago()->getValue("DIASENTREPLAZOS").toInt());
	editDiasPrimerPlazo->setText(getRecFormaPago()->getValue("DIASPRIMERPLAZO").toInt());
	editDtoEnFactura->setText(getRecFormaPago()->getValue("DTOENFACTURA").toDouble());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaPago->setText(getRecFormaPago()->getValue("SUBCUENTAPAGO").toString());
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuentaCobro->setText(getRecFormaPago()->getValue("SUBCUENTACOBRO").toString());
}
#endif
	editNotas->setText(getRecFormaPago()->getValue("NOTAS").toString());
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
/*>>>>>FRMEDITFORMAPAGO_SCATTER*/
    enableControlesPago();
}

void FrmEditFormaPago::gatherFields()
{
    /*<<<<<FRMEDITFORMAPAGO_GATHER*/
	getRecFormaPago()->setValue( "CODIGO", editCodigo->toInt());
	getRecFormaPago()->setValue( "NOMBRE", editNombre->toString());
	getRecFormaPago()->setValue( "TIPOFORMAPAGO", comboTipoFormaPago->getCurrentItemValue());
	getRecFormaPago()->setValue( "NUMPLAZOS", editNumPlazos->toInt());
	getRecFormaPago()->setValue( "DIASENTREPLAZOS", editDiasEntrePlazos->toInt());
	getRecFormaPago()->setValue( "DIASPRIMERPLAZO", editDiasPrimerPlazo->toInt());
	getRecFormaPago()->setValue( "DTOENFACTURA", editDtoEnFactura->toDouble());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecFormaPago()->setValue( "SUBCUENTAPAGO", editSubcuentaPago->toString());
}
#endif
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecFormaPago()->setValue( "SUBCUENTACOBRO", editSubcuentaCobro->toString());
}
#endif
	getRecFormaPago()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITFORMAPAGO_GATHER*/
}

void FrmEditFormaPago::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITFORMAPAGO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITFORMAPAGO_VALIDATE*/
    if( sender == comboTipoFormaPago && comboTipoFormaPago->isJustEdited() ) {
        enableControlesPago();
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditFormaPago::enableControlesPago()
{
    bool enable;
    switch( comboTipoFormaPago->getCurrentItemValue() ) {
    case RecFormaPago::Contado:
    case RecFormaPago::Pendiente:
    case RecFormaPago::SeIgnora:
        enable = false;
        editNumPlazos->setText( 0 );
        editDiasEntrePlazos->setText( 0 );
        editDiasPrimerPlazo->setText( 0 );
        break;
    case RecFormaPago::GeneraRecibos:
        enable = true;
        break;
    }
    editNumPlazos->setReadOnly( !enable );
    editDiasEntrePlazos->setReadOnly( !enable );
    editDiasPrimerPlazo->setReadOnly( !enable );
}


/*<<<<<FRMEDITFORMAPAGO_FIN*/
} // namespace pagos
} // namespace gong
/*>>>>>FRMEDITFORMAPAGO_FIN*/

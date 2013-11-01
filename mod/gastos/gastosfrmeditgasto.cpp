/*<<<<<COPYLEFT*/
/** @file gastosfrmeditgasto.cpp Fichero de edición de
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
// COPYLEFT Fichero de edición de gastos e ingresos
// FIELD TipoGasto comboint
// FIELD CategoriaGasto comboint
// FIELD Numero int
// FIELD Fecha date
// FIELD Importe money
// FIELD Descripcion string
// FIELD CuentaPago string
// FIELD DocumentoPago string
// FIELD GenAlbaran bool
// FIELD NumeroAlbaran string
// FIELD Notas text
// TYPE FrmEditRecMaster gastos::Gasto
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITGASTO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "gastosfrmeditgasto.h"
/*>>>>>FRMEDITGASTO_INCLUDES*/

namespace gong {
namespace gastos {

/*<<<<<FRMEDITGASTO_CONSTRUCTOR*/
FrmEditGasto::FrmEditGasto(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditGasto" );
/*>>>>>FRMEDITGASTO_CONSTRUCTOR*/
/*<<<<<FRMEDITGASTO_INIT_CONTROLS*/
	QHBoxLayout *tipogastoLayout = new QHBoxLayout(0, 0, 6, "tipogastoLayout");
	QHBoxLayout *categoriagastoLayout = new QHBoxLayout(0, 0, 6, "categoriagastoLayout");
	QHBoxLayout *numeroLayout = new QHBoxLayout(0, 0, 6, "numeroLayout");
	QHBoxLayout *fechaLayout = new QHBoxLayout(0, 0, 6, "fechaLayout");
	QHBoxLayout *importeLayout = new QHBoxLayout(0, 0, 6, "importeLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *cuentapagoLayout = new QHBoxLayout(0, 0, 6, "cuentapagoLayout");
	QHBoxLayout *documentopagoLayout = new QHBoxLayout(0, 0, 6, "documentopagoLayout");
	QHBoxLayout *genalbaranLayout = new QHBoxLayout(0, 0, 6, "genalbaranLayout");
	QHBoxLayout *numeroalbaranLayout = new QHBoxLayout(0, 0, 6, "numeroalbaranLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	comboTipoGasto = addComboField<int>( pControlsFrame, "GASTO", "TIPOGASTO", tipogastoLayout );
	comboCategoriaGasto = addComboField<int>( pControlsFrame, "GASTO", "CATEGORIAGASTO", categoriagastoLayout );
	editNumero = addEditField( pControlsFrame, "GASTO", "NUMERO", numeroLayout );
	editFecha = addEditField( pControlsFrame, "GASTO", "FECHA", fechaLayout );
	editImporte = addEditField( pControlsFrame, "GASTO", "IMPORTE", importeLayout );
	editDescripcion = addEditField( pControlsFrame, "GASTO", "DESCRIPCION", descripcionLayout );
	editCuentaPago = addEditField( pControlsFrame, "GASTO", "CUENTAPAGO", cuentapagoLayout );
	editDocumentoPago = addEditField( pControlsFrame, "GASTO", "DOCUMENTOPAGO", documentopagoLayout );
	checkGenAlbaran = addCheckField( pControlsFrame, "GASTO", "GENALBARAN", genalbaranLayout );
	editNumeroAlbaran = addEditField( pControlsFrame, "GASTO", "NUMEROALBARAN", numeroalbaranLayout );
	editNotas = addTextField( pControlsFrame, "GASTO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( tipogastoLayout );
	pControlsLayout->addLayout( categoriagastoLayout );
	pControlsLayout->addLayout( numeroLayout );
	pControlsLayout->addLayout( fechaLayout );
	pControlsLayout->addLayout( importeLayout );
	pControlsLayout->addLayout( descripcionLayout );
	pControlsLayout->addLayout( cuentapagoLayout );
	pControlsLayout->addLayout( documentopagoLayout );
	pControlsLayout->addLayout( genalbaranLayout );
	pControlsLayout->addLayout( numeroalbaranLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITGASTO_INIT_CONTROLS*/
}

void FrmEditGasto::scatterFields()
{
/*<<<<<FRMEDITGASTO_SCATTER*/
	comboTipoGasto->setCurrentItemByValue(getRecGasto()->getValue("TIPOGASTO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = comboTipoGasto;
	comboCategoriaGasto->setCurrentItemByValue(getRecGasto()->getValue("CATEGORIAGASTO").toInt());
	editNumero->setText(getRecGasto()->getValue("NUMERO").toInt());
	editFecha->setText(getRecGasto()->getValue("FECHA").toDate());
	editImporte->setText(getRecGasto()->getValue("IMPORTE").toMoney());
	editDescripcion->setText(getRecGasto()->getValue("DESCRIPCION").toString());
	editCuentaPago->setText(getRecGasto()->getValue("CUENTAPAGO").toString());
	editDocumentoPago->setText(getRecGasto()->getValue("DOCUMENTOPAGO").toString());
	checkGenAlbaran->setChecked(getRecGasto()->getValue("GENALBARAN").toBool());
	editNumeroAlbaran->setText(getRecGasto()->getValue("NUMEROALBARAN").toString());
	editNotas->setText(getRecGasto()->getValue("NOTAS").toString());
/*>>>>>FRMEDITGASTO_SCATTER*/
}

void FrmEditGasto::gatherFields()
{
/*<<<<<FRMEDITGASTO_GATHER*/
	getRecGasto()->setValue( "TIPOGASTO", comboTipoGasto->getCurrentItemValue());
	getRecGasto()->setValue( "CATEGORIAGASTO", comboCategoriaGasto->getCurrentItemValue());
	getRecGasto()->setValue( "NUMERO", editNumero->toInt());
	getRecGasto()->setValue( "FECHA", editFecha->toDate());
	getRecGasto()->setValue( "IMPORTE", editImporte->toMoney());
	getRecGasto()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecGasto()->setValue( "CUENTAPAGO", editCuentaPago->toString());
	getRecGasto()->setValue( "DOCUMENTOPAGO", editDocumentoPago->toString());
	getRecGasto()->setValue( "GENALBARAN", checkGenAlbaran->isChecked());
	getRecGasto()->setValue( "NUMEROALBARAN", editNumeroAlbaran->toString());
	getRecGasto()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITGASTO_GATHER*/
}

void FrmEditGasto::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITGASTO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
/*>>>>>FRMEDITGASTO_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITGASTO_FIN*/
} // namespace gastos
} // namespace gong
/*>>>>>FRMEDITGASTO_FIN*/


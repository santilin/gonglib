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
// COPYLEFT Fichero de edición de
// FIELD Tipo comboint
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
	QHBoxLayout *tipoLayout = new QHBoxLayout(0, 0, 6, "tipoLayout");
	comboTipo = addComboField<int>( pControlsFrame, "GASTO", "TIPO", tipoLayout );
	pControlsLayout->addLayout( tipoLayout );
/*>>>>>FRMEDITGASTO_INIT_CONTROLS*/
}

void FrmEditGasto::scatterFields()
{
/*<<<<<FRMEDITGASTO_SCATTER*/
	comboTipo->setCurrentItemByValue(getRecGasto()->getValue("TIPO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = comboTipo;
/*>>>>>FRMEDITGASTO_SCATTER*/
}

void FrmEditGasto::gatherFields()
{
/*<<<<<FRMEDITGASTO_GATHER*/
	getRecGasto()->setValue( "TIPO", comboTipo->getCurrentItemValue());
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


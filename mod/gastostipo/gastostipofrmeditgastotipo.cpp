/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de definiciones de gastos e ingresos
// FIELD TipoGasto comboint
// FIELD CategoriaGasto comboint
// FIELD PedirImporte comboint
// FIELD Importe money
// FIELD PedirDescripcion comboint
// FIELD Descripcion string
// FIELD PedirNotas comboint
// FIELD Notas text
// TYPE FrmEditRecMaster gastos::GastoDef
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITGASTODEF_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "gastosfrmeditgastodef.h"
/*>>>>>FRMEDITGASTODEF_INCLUDES*/

namespace gong {
namespace gastos {

/*<<<<<FRMEDITGASTODEF_CONSTRUCTOR*/
FrmEditGastoDef::FrmEditGastoDef(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditGastoDef" );
/*>>>>>FRMEDITGASTODEF_CONSTRUCTOR*/
/*<<<<<FRMEDITGASTODEF_INIT_CONTROLS*/
	QHBoxLayout *tipogastoLayout = new QHBoxLayout(0, 0, 6, "tipogastoLayout");
	QHBoxLayout *categoriagastoLayout = new QHBoxLayout(0, 0, 6, "categoriagastoLayout");
	QHBoxLayout *pedirimporteLayout = new QHBoxLayout(0, 0, 6, "pedirimporteLayout");
	QHBoxLayout *importeLayout = new QHBoxLayout(0, 0, 6, "importeLayout");
	QHBoxLayout *pedirdescripcionLayout = new QHBoxLayout(0, 0, 6, "pedirdescripcionLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *pedirnotasLayout = new QHBoxLayout(0, 0, 6, "pedirnotasLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	comboTipoGasto = addComboIntField( pControlsFrame, "GASTODEF", "TIPOGASTO", tipogastoLayout );
	comboCategoriaGasto = addComboIntField( pControlsFrame, "GASTODEF", "CATEGORIAGASTO", categoriagastoLayout );
	comboPedirImporte = addComboIntField( pControlsFrame, "GASTODEF", "PEDIRIMPORTE", pedirimporteLayout );
	editImporte = addEditField( pControlsFrame, "GASTODEF", "IMPORTE", importeLayout );
	comboPedirDescripcion = addComboIntField( pControlsFrame, "GASTODEF", "PEDIRDESCRIPCION", pedirdescripcionLayout );
	editDescripcion = addEditField( pControlsFrame, "GASTODEF", "DESCRIPCION", descripcionLayout );
	comboPedirNotas = addComboIntField( pControlsFrame, "GASTODEF", "PEDIRNOTAS", pedirnotasLayout );
	editNotas = addTextField( pControlsFrame, "GASTODEF", "NOTAS", notasLayout );
	pControlsLayout->addLayout( tipogastoLayout );
	pControlsLayout->addLayout( categoriagastoLayout );
	pControlsLayout->addLayout( pedirimporteLayout );
	pControlsLayout->addLayout( importeLayout );
	pControlsLayout->addLayout( pedirdescripcionLayout );
	pControlsLayout->addLayout( descripcionLayout );
	pControlsLayout->addLayout( pedirnotasLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITGASTODEF_INIT_CONTROLS*/
}

void FrmEditGastoDef::scatterFields()
{
/*<<<<<FRMEDITGASTODEF_SCATTER*/
	comboTipoGasto->setCurrentItemByValue(getRecGastoDef()->getValue("TIPOGASTO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = comboTipoGasto;
	comboCategoriaGasto->setCurrentItemByValue(getRecGastoDef()->getValue("CATEGORIAGASTO").toInt());
	comboPedirImporte->setCurrentItemByValue(getRecGastoDef()->getValue("PEDIRIMPORTE").toInt());
	editImporte->setText(getRecGastoDef()->getValue("IMPORTE").toMoney());
	comboPedirDescripcion->setCurrentItemByValue(getRecGastoDef()->getValue("PEDIRDESCRIPCION").toInt());
	editDescripcion->setText(getRecGastoDef()->getValue("DESCRIPCION").toString());
	comboPedirNotas->setCurrentItemByValue(getRecGastoDef()->getValue("PEDIRNOTAS").toInt());
	editNotas->setText(getRecGastoDef()->getValue("NOTAS").toString());
/*>>>>>FRMEDITGASTODEF_SCATTER*/
}

void FrmEditGastoDef::gatherFields()
{
/*<<<<<FRMEDITGASTODEF_GATHER*/
	getRecGastoDef()->setValue( "TIPOGASTO", comboTipoGasto->getCurrentItemValue());
	getRecGastoDef()->setValue( "CATEGORIAGASTO", comboCategoriaGasto->getCurrentItemValue());
	getRecGastoDef()->setValue( "PEDIRIMPORTE", comboPedirImporte->getCurrentItemValue());
	getRecGastoDef()->setValue( "IMPORTE", editImporte->toMoney());
	getRecGastoDef()->setValue( "PEDIRDESCRIPCION", comboPedirDescripcion->getCurrentItemValue());
	getRecGastoDef()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecGastoDef()->setValue( "PEDIRNOTAS", comboPedirNotas->getCurrentItemValue());
	getRecGastoDef()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITGASTODEF_GATHER*/
}

void FrmEditGastoDef::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITGASTODEF_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
/*>>>>>FRMEDITGASTODEF_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITGASTODEF_FIN*/
} // namespace gastos
} // namespace gong
/*>>>>>FRMEDITGASTODEF_FIN*/


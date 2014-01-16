/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de definiciones de gastos e ingresos
// FIELD CategoriaGasto comboint
// FIELD PedirImporte comboint
// FIELD Importe money
// FIELD PedirDescripcion comboint
// FIELD Descripcion string
// FIELD PedirNotas comboint
// FIELD Notas text
// TYPE FrmEditRecMaster gastostipo::GastoTipo
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITGASTOTIPO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "gastostipofrmeditgastotipo.h"
/*>>>>>FRMEDITGASTOTIPO_INCLUDES*/

namespace gong {
namespace gastostipo {

/*<<<<<FRMEDITGASTOTIPO_CONSTRUCTOR*/
FrmEditGastoTipo::FrmEditGastoTipo(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditGastoTipo" );
/*>>>>>FRMEDITGASTOTIPO_CONSTRUCTOR*/
/*<<<<<FRMEDITGASTOTIPO_INIT_CONTROLS*/
	QHBoxLayout *categoriagastoLayout = new QHBoxLayout(0, 0, 6, "categoriagastoLayout");
	QHBoxLayout *pedirimporteLayout = new QHBoxLayout(0, 0, 6, "pedirimporteLayout");
	QHBoxLayout *importeLayout = new QHBoxLayout(0, 0, 6, "importeLayout");
	QHBoxLayout *pedirdescripcionLayout = new QHBoxLayout(0, 0, 6, "pedirdescripcionLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *pedirnotasLayout = new QHBoxLayout(0, 0, 6, "pedirnotasLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	comboCategoriaGasto = addComboIntField( pControlsFrame, "GASTOTIPO", "CATEGORIAGASTO", categoriagastoLayout );
	comboPedirImporte = addComboIntField( pControlsFrame, "GASTOTIPO", "PEDIRIMPORTE", pedirimporteLayout );
	editImporte = addEditField( pControlsFrame, "GASTOTIPO", "IMPORTE", importeLayout );
	comboPedirDescripcion = addComboIntField( pControlsFrame, "GASTOTIPO", "PEDIRDESCRIPCION", pedirdescripcionLayout );
	editDescripcion = addEditField( pControlsFrame, "GASTOTIPO", "DESCRIPCION", descripcionLayout );
	comboPedirNotas = addComboIntField( pControlsFrame, "GASTOTIPO", "PEDIRNOTAS", pedirnotasLayout );
	editNotas = addTextField( pControlsFrame, "GASTOTIPO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( categoriagastoLayout );
	pControlsLayout->addLayout( pedirimporteLayout );
	pControlsLayout->addLayout( importeLayout );
	pControlsLayout->addLayout( pedirdescripcionLayout );
	pControlsLayout->addLayout( descripcionLayout );
	pControlsLayout->addLayout( pedirnotasLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITGASTOTIPO_INIT_CONTROLS*/
}

void FrmEditGastoTipo::scatterFields()
{
/*<<<<<FRMEDITGASTOTIPO_SCATTER*/
	comboCategoriaGasto->setCurrentItemByValue(getRecGastoTipo()->getValue("CATEGORIAGASTO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = comboCategoriaGasto;
	comboPedirImporte->setCurrentItemByValue(getRecGastoTipo()->getValue("PEDIRIMPORTE").toInt());
	editImporte->setText(getRecGastoTipo()->getValue("IMPORTE").toMoney());
	comboPedirDescripcion->setCurrentItemByValue(getRecGastoTipo()->getValue("PEDIRDESCRIPCION").toInt());
	editDescripcion->setText(getRecGastoTipo()->getValue("DESCRIPCION").toString());
	comboPedirNotas->setCurrentItemByValue(getRecGastoTipo()->getValue("PEDIRNOTAS").toInt());
	editNotas->setText(getRecGastoTipo()->getValue("NOTAS").toString());
/*>>>>>FRMEDITGASTOTIPO_SCATTER*/
}

void FrmEditGastoTipo::gatherFields()
{
/*<<<<<FRMEDITGASTOTIPO_GATHER*/
	getRecGastoTipo()->setValue( "CATEGORIAGASTO", comboCategoriaGasto->getCurrentItemValue());
	getRecGastoTipo()->setValue( "PEDIRIMPORTE", comboPedirImporte->getCurrentItemValue());
	getRecGastoTipo()->setValue( "IMPORTE", editImporte->toMoney());
	getRecGastoTipo()->setValue( "PEDIRDESCRIPCION", comboPedirDescripcion->getCurrentItemValue());
	getRecGastoTipo()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecGastoTipo()->setValue( "PEDIRNOTAS", comboPedirNotas->getCurrentItemValue());
	getRecGastoTipo()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITGASTOTIPO_GATHER*/
}

void FrmEditGastoTipo::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITGASTOTIPO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
/*>>>>>FRMEDITGASTOTIPO_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

/*<<<<<FRMEDITGASTOTIPO_FIN*/
} // namespace gastostipo
} // namespace gong
/*>>>>>FRMEDITGASTOTIPO_FIN*/


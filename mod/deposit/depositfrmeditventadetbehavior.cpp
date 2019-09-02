/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior para añadir campos a los detalles de las ventas
// FIELD ImporteLiquidacion money - ventadet
// FIELD ImporteLiquidacionIVA money - ventadet
// TYPE FrmEditRecBehavior deposit::VentaDet
/*>>>>>MODULE_INFO*/

#include <gongdbrecordlist.h>
#include <dbappfrmeditrec.h>
#include <facturecarticulo.h>
#include "depositrecliquidacioncompra.h"
#include "depositrecliquidacioncompradet.h"
/*<<<<<FRMEDITVENTADETBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "depositfrmeditventadetbehavior.h"
/*>>>>>FRMEDITVENTADETBEHAVIOR_INCLUDES*/

namespace gong {
namespace deposit {

void FrmEditVentaDetBehavior::initGUI()
{
	pEditCantidad = pEditArticuloCodigo = 0;
	QHBoxLayout *ventadetlay = new QHBoxLayout();
	editImporteLiquidacion = pTheForm->addEditField( getControlsFrame(),
													pTheForm->getRecord()->getTableName(),
													"IMPORTELIQUIDACION", ventadetlay );
	editImporteLiquidacionIVA = pTheForm->addEditField( getControlsFrame(),
													pTheForm->getRecord()->getTableName(),
													"IMPORTELIQUIDACIONIVA", ventadetlay );
	pCheckLiquidada = pTheForm->addCheckField( getControlsFrame(),
		pTheForm->getRecord()->getTableName(), "LIQUIDACIONCOMPRA_ID", ventadetlay );
 	pCheckLiquidada->setText(_("Liquidada"));
	pCheckLiquidada->setVisible( true ); // Como es una referencia, por defecto no es visible
	getControlsLayout()->addLayout( ventadetlay );
#if 0
/*<<<<<FRMEDITVENTADETBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QHBoxLayout *ventadet = new QHBoxLayout(0, 0, 6, "ventadet");
	editImporteLiquidacion = pTheForm->addEditField( pControlsFrame, "VENTADET", "IMPORTELIQUIDACION", ventadet );
	editImporteLiquidacionIVA = pTheForm->addEditField( pControlsFrame, "VENTADET", "IMPORTELIQUIDACIONIVA", ventadet );
	pControlsLayout->addLayout( ventadet );
/*>>>>>FRMEDITVENTADETBEHAVIOR_INITGUI*/
#endif
}

void FrmEditVentaDetBehavior::scatterFields( bool is_pre )
{
	if( is_pre ) {
		if( pTheForm->isDuplicating() ) {
			pTheForm->getRecord()->setValue( "IMPORTELIQUIDACION", Money() );
			pTheForm->getRecord()->setValue( "IMPORTELIQUIDACIONIVA", Money() );
			pTheForm->getRecord()->setNullValue( "LIQUIDACIONCOMPRA_ID" );
		}
		return;
	}
/*<<<<<FRMEDITVENTADETBEHAVIOR_SCATTER*/
	editImporteLiquidacion->setText( pTheForm->getRecord()->getValue("IMPORTELIQUIDACION").toMoney());
	editImporteLiquidacionIVA->setText( pTheForm->getRecord()->getValue("IMPORTELIQUIDACIONIVA").toMoney());
/*>>>>>FRMEDITVENTADETBEHAVIOR_SCATTER*/
	pCheckLiquidada->setChecked( pTheForm->getRecord()->getValue( "LIQUIDACIONCOMPRA_ID" ).toInt() != 0 );
}

void FrmEditVentaDetBehavior::gatherFields()
{
/*<<<<<FRMEDITVENTADETBEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "IMPORTELIQUIDACION", editImporteLiquidacion->toMoney());
	pTheForm->getRecord()->setValue( "IMPORTELIQUIDACIONIVA", editImporteLiquidacionIVA->toMoney());
/*>>>>>FRMEDITVENTADETBEHAVIOR_GATHER*/
	if( !pCheckLiquidada->isOn() )
		pTheForm->getRecord()->setNullValue( "LIQUIDACIONCOMPRA_ID" );
}

void FrmEditVentaDetBehavior::validateFields( bool is_pre, QWidget* sender,
											 bool* isvalid, ValidResult *ir)
{
	if( is_pre ) return;
/*<<<<<FRMEDITVENTADETBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITVENTADETBEHAVIOR_VALIDATE*/
	(void)validresult;
	if( pEditCantidad == 0 ) {
		Xtring sfldcantidad = pTheForm->getRecord()->getTableName() + ".CANTIDAD";
		pEditCantidad = static_cast<EditBox *>(pTheForm->findControl(sfldcantidad));
	}
	_GONG_DEBUG_ASSERT( pEditCantidad );
	if( pEditArticuloCodigo == 0 ) {
		Xtring sfldarticulocodigo = "ARTICULO.CODIGO";
		pEditArticuloCodigo = static_cast<EditBox *>(pTheForm->findControl(sfldarticulocodigo));
	}
	_GONG_DEBUG_ASSERT( pEditArticuloCodigo );
	factu::RecArticulo *recarticulo = static_cast<factu::RecArticulo *>(pTheForm->getRecord()->findRelatedRecord("ARTICULO_ID") );
	// Si la cantidad cambia, recalcular el importe de la liquidación
	if( pEditCantidad->isJustEdited() || pEditArticuloCodigo->isJustEdited() ) {
		double liq = pEditCantidad->toDouble() * recarticulo->getValue("COSTESINIVA").toDouble();
		editImporteLiquidacion->setText( liq );
		editImporteLiquidacionIVA->setText( recarticulo->masIVA( liq ) );
	}
	if( sender == editImporteLiquidacion && editImporteLiquidacion->isJustEdited() )
		editImporteLiquidacionIVA->setText( recarticulo->masIVA( editImporteLiquidacion->toDouble() ) );
	if( sender == editImporteLiquidacionIVA && editImporteLiquidacionIVA->isJustEdited() )
		editImporteLiquidacion->setText( recarticulo->menosIVA( editImporteLiquidacionIVA->toDouble() ) );
}


/*<<<<<FRMEDITVENTADETBEHAVIOR_FIN*/
} // namespace deposit
} // namespace gong
/*>>>>>FRMEDITVENTADETBEHAVIOR_FIN*/


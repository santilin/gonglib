/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior del formulario de artículos para el módulo ecotienda
// FIELD Aval string frameEcotienda
// FIELD PesoNeto string frameEcotienda
// FIELD UnidadesCaja double frameEcotienda
// TYPE FrmEditRecBehavior ecotienda::Articulo Q_OBJECT
/*>>>>>MODULE_INFO*/

#include <dbappfrmeditrecmaster.h>
/*<<<<<FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "ecotiendafrmeditarticulobehavior.h"
/*>>>>>FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include "ecotiendamodule.h"

namespace gong {
namespace ecotienda {

void FrmEditArticuloBehavior::initGUI()
{
    QWidget *tabExtra = getOrCreateTab( "tabExtra" );
    QVBoxLayout *tabExtraLayout = static_cast<QVBoxLayout *>( tabExtra->layout() );
	QFrame *frameEcotienda = new QFrame( tabExtra );
	frameEcotienda->setObjectName( "FrameArticuloEcotienda" );
	new QVBoxLayout( frameEcotienda );

/*<<<<<FRMEDITARTICULOBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QVBoxLayout *frameEcotiendaLayout = static_cast<QVBoxLayout *>( frameEcotienda->layout() );
	QHBoxLayout *avalLayout = new QHBoxLayout(0, 0, 6, "avalLayout");
	QHBoxLayout *pesonetoLayout = new QHBoxLayout(0, 0, 6, "pesonetoLayout");
	QHBoxLayout *unidadescajaLayout = new QHBoxLayout(0, 0, 6, "unidadescajaLayout");
	editAval = pTheForm->addEditField( frameEcotienda, "ARTICULO", "AVAL", avalLayout );
	editPesoNeto = pTheForm->addEditField( frameEcotienda, "ARTICULO", "PESONETO", pesonetoLayout );
	editUnidadesCaja = pTheForm->addEditField( frameEcotienda, "ARTICULO", "UNIDADESCAJA", unidadescajaLayout );
	frameEcotiendaLayout->addLayout( avalLayout );
	frameEcotiendaLayout->addLayout( pesonetoLayout );
	frameEcotiendaLayout->addLayout( unidadescajaLayout );
/*>>>>>FRMEDITARTICULOBEHAVIOR_INITGUI*/
	(void)pControlsFrame; (void)pControlsLayout;
	tabExtraLayout->addWidget( frameEcotienda );
}

void FrmEditArticuloBehavior::scatterFields( bool is_pre )
{
	if( !is_pre ) return;
/*<<<<<FRMEDITARTICULOBEHAVIOR_SCATTER*/
	editAval->setText( pTheForm->getRecord()->getValue("AVAL").toString());
	editPesoNeto->setText( pTheForm->getRecord()->getValue("PESONETO").toString());
	editUnidadesCaja->setText( pTheForm->getRecord()->getValue("UNIDADESCAJA").toDouble());
/*>>>>>FRMEDITARTICULOBEHAVIOR_SCATTER*/
}

void FrmEditArticuloBehavior::gatherFields()
{
/*<<<<<FRMEDITARTICULOBEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "AVAL", editAval->toString());
	pTheForm->getRecord()->setValue( "PESONETO", editPesoNeto->toString());
	pTheForm->getRecord()->setValue( "UNIDADESCAJA", editUnidadesCaja->toDouble());
/*>>>>>FRMEDITARTICULOBEHAVIOR_GATHER*/
}

void FrmEditArticuloBehavior::validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir)
{
	if( !is_pre ) return;
/*<<<<<FRMEDITARTICULOBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITARTICULOBEHAVIOR_VALIDATE*/
	(void)validresult;
}


/*<<<<<FRMEDITARTICULOBEHAVIOR_FIN*/
} // namespace ecotienda
} // namespace gong
/*>>>>>FRMEDITARTICULOBEHAVIOR_FIN*/


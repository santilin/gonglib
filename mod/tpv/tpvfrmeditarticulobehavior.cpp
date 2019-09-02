/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior del formulario de artículos para el módulo TPV
// FIELD ParaTPV bool frameTPV tpvlayout
// FIELD OrdenTPV int frameTPV tpvlayout
// FIELD PVPTPV money frameTPV tpvlayout
// FIELD Variante1 string frameTPV var1
// FIELD PVPVariante1 money frameTPV var1
// FIELD Variante2 string frameTPV var2
// FIELD PVPVariante2 money frameTPV var2
// TYPE FrmEditRecBehavior tpv::Articulo Q_OBJECT
/*>>>>>MODULE_INFO*/

#include <gongimagebox.h>
#include <dbappfrmeditrecmaster.h>
/*<<<<<FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "tpvfrmeditarticulobehavior.h"
/*>>>>>FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include "tpvmodule.h"

namespace gong {
namespace tpv {

void FrmEditArticuloBehavior::initGUI()
{
    QWidget *tabExtra = getOrCreateTab( "tabExtra" );
    QVBoxLayout *tabExtraLayout = static_cast<QVBoxLayout *>( tabExtra->layout() );
	QFrame *frameTPV = new QFrame( tabExtra );
	frameTPV->setObjectName( "FrameArticuloTPV" );
	new QVBoxLayout( frameTPV );

/*<<<<<FRMEDITARTICULOBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QVBoxLayout *frameTPVLayout = static_cast<QVBoxLayout *>( frameTPV->layout() );
	QHBoxLayout *tpvlayout = new QHBoxLayout(0, 0, 6, "tpvlayout");
	QHBoxLayout *var1 = new QHBoxLayout(0, 0, 6, "var1");
	QHBoxLayout *var2 = new QHBoxLayout(0, 0, 6, "var2");
	checkParaTPV = pTheForm->addCheckField( frameTPV, "ARTICULO", "PARATPV", tpvlayout );
	editOrdenTPV = pTheForm->addEditField( frameTPV, "ARTICULO", "ORDENTPV", tpvlayout );
	editPVPTPV = pTheForm->addEditField( frameTPV, "ARTICULO", "PVPTPV", tpvlayout );
	editVariante1 = pTheForm->addEditField( frameTPV, "ARTICULO", "VARIANTE1", var1 );
	editPVPVariante1 = pTheForm->addEditField( frameTPV, "ARTICULO", "PVPVARIANTE1", var1 );
	editVariante2 = pTheForm->addEditField( frameTPV, "ARTICULO", "VARIANTE2", var2 );
	editPVPVariante2 = pTheForm->addEditField( frameTPV, "ARTICULO", "PVPVARIANTE2", var2 );
	frameTPVLayout->addLayout( tpvlayout );
	frameTPVLayout->addLayout( var1 );
	frameTPVLayout->addLayout( var2 );
/*>>>>>FRMEDITARTICULOBEHAVIOR_INITGUI*/
	(void)pControlsFrame; (void)pControlsLayout;
	tabExtraLayout->addWidget( frameTPV );

	pushOptimizeTPVImage = new QPushButton( pTheForm, "pushOptimizeTPVImage" );
    pushOptimizeTPVImage->setText( toGUI( _("Optimizar para TPV") ) );
    connect( pushOptimizeTPVImage, SIGNAL( clicked() ), this, SLOT( slotOptimizeTPVImage_clicked() ) );
	QLayout *tabimagenlayout = static_cast<QWidget *>(pTheForm->child("tabImagen"))->layout();
	tabimagenlayout->addWidget( pushOptimizeTPVImage );
}

void FrmEditArticuloBehavior::scatterFields( bool is_pre )
{
	if( !is_pre ) return;
/*<<<<<FRMEDITARTICULOBEHAVIOR_SCATTER*/
	checkParaTPV->setChecked( pTheForm->getRecord()->getValue("PARATPV").toBool());
	editOrdenTPV->setText( pTheForm->getRecord()->getValue("ORDENTPV").toInt());
	editPVPTPV->setText( pTheForm->getRecord()->getValue("PVPTPV").toMoney());
	editVariante1->setText( pTheForm->getRecord()->getValue("VARIANTE1").toString());
	editPVPVariante1->setText( pTheForm->getRecord()->getValue("PVPVARIANTE1").toMoney());
	editVariante2->setText( pTheForm->getRecord()->getValue("VARIANTE2").toString());
	editPVPVariante2->setText( pTheForm->getRecord()->getValue("PVPVARIANTE2").toMoney());
/*>>>>>FRMEDITARTICULOBEHAVIOR_SCATTER*/
}

void FrmEditArticuloBehavior::gatherFields()
{
/*<<<<<FRMEDITARTICULOBEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "PARATPV", checkParaTPV->isChecked());
	pTheForm->getRecord()->setValue( "ORDENTPV", editOrdenTPV->toInt());
	pTheForm->getRecord()->setValue( "PVPTPV", editPVPTPV->toMoney());
	pTheForm->getRecord()->setValue( "VARIANTE1", editVariante1->toString());
	pTheForm->getRecord()->setValue( "PVPVARIANTE1", editPVPVariante1->toMoney());
	pTheForm->getRecord()->setValue( "VARIANTE2", editVariante2->toString());
	pTheForm->getRecord()->setValue( "PVPVARIANTE2", editPVPVariante2->toMoney());
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

void FrmEditArticuloBehavior::slotOptimizeTPVImage_clicked()
{
	ImageBox *imgbox = static_cast<ImageBox *>( pTheForm->findControl( "ARTICULOIMAGEN.IMAGEN" ) );
	_GONG_DEBUG_ASSERT( imgbox );
	const QPixmap *p = imgbox->getImage();
	if( !p )
		return;
	int w = ModuleInstance->getModuleSetting( "ANCHO_IMAGENES" ).toInt();
	int h = ModuleInstance->getModuleSetting( "ALTO_IMAGENES" ).toInt();
	QPixmap ps( p->scaled( w, h ) );
	imgbox->setImage( &ps );
}

/*<<<<<FRMEDITARTICULOBEHAVIOR_FIN*/
} // namespace tpv
} // namespace gong
/*>>>>>FRMEDITARTICULOBEHAVIOR_FIN*/


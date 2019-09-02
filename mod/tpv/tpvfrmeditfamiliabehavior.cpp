/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior del formulario de familias para el módulo TPV
// FIELD ParaTPV bool tabTPV tpvlayout
// FIELD OrdenTPV int tabTPV tpvlayout
// FIELD ArticuloImagen.Imagen image tabTPV imagenlayout
// TYPE FrmEditRecBehavior tpv::Familia Q_OBJECT
/*>>>>>MODULE_INFO*/

#include <gongimagebox.h>
/*<<<<<FRMEDITFAMILIABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "tpvfrmeditfamiliabehavior.h"
/*>>>>>FRMEDITFAMILIABEHAVIOR_INCLUDES*/
#include "tpvmodule.h"

namespace gong {
namespace tpv {

void FrmEditFamiliaBehavior::initGUI()
{
/*<<<<<FRMEDITFAMILIABEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QWidget *tabTPV = getOrCreateTab( "tabTPV" );
	QVBoxLayout *tabTPVLayout = static_cast<QVBoxLayout *>( tabTPV->layout() );
	QHBoxLayout *tpvlayout = new QHBoxLayout(0, 0, 6, "tpvlayout");
	QHBoxLayout *imagenlayout = new QHBoxLayout(0, 0, 6, "imagenlayout");
	checkParaTPV = pTheForm->addCheckField( tabTPV, "FAMILIA", "PARATPV", tpvlayout );
	editOrdenTPV = pTheForm->addEditField( tabTPV, "FAMILIA", "ORDENTPV", tpvlayout );
	editArticuloImagen_Imagen = pTheForm->addImageField( tabTPV, "ARTICULOIMAGEN", "IMAGEN", imagenlayout );
	tabTPVLayout->addLayout( tpvlayout );
	tabTPVLayout->addLayout( imagenlayout );
/*>>>>>FRMEDITFAMILIABEHAVIOR_INITGUI*/
	(void)pControlsFrame; (void)pControlsLayout;
	pPushOptimizeImage = new QPushButton( pTheForm );
	pPushOptimizeImage->setText( toGUI( _("Optimizar imagen para TPV") ) );
	connect( pPushOptimizeImage, SIGNAL( clicked() ), pPushOptimizeImage, SLOT( slotPushOptimizeImage() ) );
	pPushOptimizeImage->setVisible( false );
	pTheForm->setTabTitle( tabTPV, _("TPV") );
}

void FrmEditFamiliaBehavior::scatterFields( bool is_pre )
{
	if( !is_pre ) return;
/*<<<<<FRMEDITFAMILIABEHAVIOR_SCATTER*/
	checkParaTPV->setChecked( pTheForm->getRecord()->getValue("PARATPV").toBool());
	editOrdenTPV->setText( pTheForm->getRecord()->getValue("ORDENTPV").toInt());
	editArticuloImagen_Imagen->setImageData( pTheForm->getRecord()->getValue("ARTICULOIMAGEN.IMAGEN"));
/*>>>>>FRMEDITFAMILIABEHAVIOR_SCATTER*/
}

void FrmEditFamiliaBehavior::gatherFields()
{
/*<<<<<FRMEDITFAMILIABEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "PARATPV", checkParaTPV->isChecked());
	pTheForm->getRecord()->setValue( "ORDENTPV", editOrdenTPV->toInt());
	pTheForm->getRecord()->setValue( "ARTICULOIMAGEN.IMAGEN", editArticuloImagen_Imagen->toData());
/*>>>>>FRMEDITFAMILIABEHAVIOR_GATHER*/
}

void FrmEditFamiliaBehavior::validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir)
{
	if( !is_pre ) return;
/*<<<<<FRMEDITFAMILIABEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITFAMILIABEHAVIOR_VALIDATE*/
	(void)validresult;
}

void FrmEditFamiliaBehavior::slotPushOptimizeImage()
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


/*<<<<<FRMEDITFAMILIABEHAVIOR_FIN*/
} // namespace tpv
} // namespace gong
/*>>>>>FRMEDITFAMILIABEHAVIOR_FIN*/


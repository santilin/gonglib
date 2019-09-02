/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior del formulario de artículos
// FIELD PublicarWeb bool tabExtra descweb
// FIELD PVPWeb Money tabExtra descweb
// FIELD OrdenWeb int tabExtra descweb
// TYPE FrmEditRecBehavior ventasweb::Articulo Q_OBJECT
/*>>>>>MODULE_INFO*/

#include <gongimagebox.h>
#include <gongfileutils.h>
#include <dbappfrmcustom.h>
/*<<<<<FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "ventaswebfrmeditarticulobehavior.h"
/*>>>>>FRMEDITARTICULOBEHAVIOR_INCLUDES*/
#include "ventaswebmodule.h"

namespace gong {
namespace ventasweb {

void FrmEditArticuloBehavior::initGUI()
{
    /*<<<<<FRMEDITARTICULOBEHAVIOR_INITGUI*/
	QWidget *pControlsFrame = getControlsFrame();
	QVBoxLayout* pControlsLayout = getControlsLayout();
	QWidget *tabExtra = getOrCreateTab( "tabExtra" );
	QVBoxLayout *tabExtraLayout = static_cast<QVBoxLayout *>( tabExtra->layout() );
	QHBoxLayout *descweb = new QHBoxLayout(0, 0, 6, "descweb");
	checkPublicarWeb = pTheForm->addCheckField( tabExtra, "ARTICULO", "PUBLICARWEB", descweb );
	editPVPWeb = pTheForm->addEditField( tabExtra, "ARTICULO", "PVPWEB", descweb );
	editOrdenWeb = pTheForm->addEditField( tabExtra, "ARTICULO", "ORDENWEB", descweb );
	tabExtraLayout->addLayout( descweb );
/*>>>>>FRMEDITARTICULOBEHAVIOR_INITGUI*/
    (void)pControlsFrame;
    (void)pControlsLayout;
    QHBoxLayout *subirweb = new QHBoxLayout();
    pushSubirImagenWeb = new QPushButton( pTheForm, "subirimagenweb" );
    pushSubirImagenWeb->setText( _("Subir imagen a la web") );
    subirweb->addWidget( pushSubirImagenWeb );
    QWidget *tabImagen = getOrCreateTab( "tabImagen" );
    static_cast<QHBoxLayout*>(tabImagen->layout())->addLayout( subirweb );
    connect( pushSubirImagenWeb, SIGNAL( clicked() ), this, SLOT( slot_SubirImagenWeb() ) );
}

void FrmEditArticuloBehavior::scatterFields( bool is_pre )
{
    if( !is_pre) return;
    /*<<<<<FRMEDITARTICULOBEHAVIOR_SCATTER*/
	checkPublicarWeb->setChecked( pTheForm->getRecord()->getValue("PUBLICARWEB").toBool());
	editPVPWeb->setText( pTheForm->getRecord()->getValue("PVPWEB").toMoney());
	editOrdenWeb->setText( pTheForm->getRecord()->getValue("ORDENWEB").toInt());
/*>>>>>FRMEDITARTICULOBEHAVIOR_SCATTER*/
}

void FrmEditArticuloBehavior::gatherFields()
{
    /*<<<<<FRMEDITARTICULOBEHAVIOR_GATHER*/
	pTheForm->getRecord()->setValue( "PUBLICARWEB", checkPublicarWeb->isChecked());
	pTheForm->getRecord()->setValue( "PVPWEB", editPVPWeb->toMoney());
	pTheForm->getRecord()->setValue( "ORDENWEB", editOrdenWeb->toInt());
/*>>>>>FRMEDITARTICULOBEHAVIOR_GATHER*/
}

void FrmEditArticuloBehavior::validateFields(bool is_pre,
        QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITARTICULOBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITARTICULOBEHAVIOR_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

void FrmEditArticuloBehavior::slot_SubirImagenWeb()
{
    ImageBox *imgbox = static_cast<ImageBox *>(pTheForm->findControl("IMAGEN"));
    if( !imgbox || !imgbox->getImage() ) {
        FrmBase::msgError( pTheForm, _("No hay imagen") );
        return;
    }
    FrmCustom *FrmSubirImagenWeb = new FrmCustom( pTheForm );
    FrmSubirImagenWeb->setObjectName( "SubirImagenWeb" );
    FrmSubirImagenWeb->setTitle( _("Subir imagen") );
    LineEdit *pFTPName = FrmSubirImagenWeb->addInput( pTheForm, _("Nombre de la imagen"),
                         imgbox->getProposedFileName() );
    LineEdit *pFTPHost = FrmSubirImagenWeb->addInput( pTheForm, _("Servidor ftp"),
                         ModuleInstance->getModuleSetting( "FTP_HOST").toString() );
    LineEdit *pFTPWebDir = FrmSubirImagenWeb->addInput( pTheForm, _("Directorio de destino"),
                           ModuleInstance->getModuleSetting( "FTP_WEBDIR").toString(), "/httpdocs/images/gongcarrito/articulos" );
    CheckBox *pCheckSaveSettings = FrmSubirImagenWeb->addCheckBox( pTheForm, _("Guardar datos de conexión"), true );
    FrmSubirImagenWeb->showModalFor( pTheForm, true, true );
    if( !FrmSubirImagenWeb->wasCancelled() ) {
        DBAPP->waitCursor( true );
        imgbox->getImage()->save( ("/tmp/" + pFTPName->toString()).c_str() );
        Xtring command = "ncftpput -m -f " + pFTPHost->toString()
                         + " " + pFTPWebDir->toString() + " /tmp/" + pFTPName->toString();
        Xtring messages, errors;
        (void)FileUtils::execProcess( command, messages, errors );
        if( pCheckSaveSettings->isOn() ) {
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.FTP_HOST", pFTPHost->toString() );
            DBAPP->setUserLocalSetting( "MODULE.VENTASWEB.FTP_WEBDIR", pFTPWebDir->toString() );
        }
        if( !errors.isEmpty() )
            pTheForm->msgError( FrmSubirImagenWeb, errors );
        else
            pTheForm->msgOk( FrmSubirImagenWeb, _("La imagen se ha subido correctamente") );
        DBAPP->resetCursor();
    }
    delete FrmSubirImagenWeb;
}

/*<<<<<FRMEDITARTICULOBEHAVIOR_FIN*/
} // namespace ventasweb
} // namespace gong
/*>>>>>FRMEDITARTICULOBEHAVIOR_FIN*/


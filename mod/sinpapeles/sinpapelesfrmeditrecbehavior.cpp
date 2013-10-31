#include <dbappfrmeditrec.h>
#include "sinpapelesmodule.h"
#include "sinpapelesfrmeditrecbehavior.h"

namespace gong {
namespace sinpapeles {

void SinPapelesFrmEditRecBehavior::initGUI()
{
    FileNameBox *fnbox = pTheForm->addFileNameField( getControlsFrame(),
                         pTheForm->getRecord()->getTableName(), "SINPAPELES_FILENAME");
    connect( fnbox, SIGNAL(clicked()), this, SLOT( slot_fnbox_clicked() ) );
}

void SinPapelesFrmEditRecBehavior::scatterFields( bool is_pre )
{
    if( is_pre ) return;
    LineEdit *le_filename = 0;
    for ( EditControlsList::const_iterator it = getEditControlsList().begin();
            it != getEditControlsList().end();
            ++ it ) {
        if( Xtring((*it)->name()).endsWith( pTheForm->getRecord()->getTableName() + "_SINPAPELES_FILENAME" ) ) {
            le_filename = static_cast<FileNameBox *>(*it)->getEditFileName();
            le_filename->setText( pTheForm->getRecord()->getValue( "SINPAPELES_FILENAME").toString() );
            break;
        }
    }
}

void SinPapelesFrmEditRecBehavior::gatherFields( bool is_pre )
{
    if( is_pre ) return;
    LineEdit *le_filename = 0;
    for ( EditControlsList::const_iterator it = getEditControlsList().begin();
            it != getEditControlsList().end();
            ++ it ) {
        if( Xtring((*it)->name()).endsWith( pTheForm->getRecord()->getTableName() + "_SINPAPELES_FILENAME" ) ) {
            le_filename = static_cast<FileNameBox *>(*it)->getEditFileName();
            pTheForm->getRecord()->setValue( "SINPAPELES_FILENAME", le_filename->toString() );
            break;
        }
    }
}

void SinPapelesFrmEditRecBehavior::slot_fnbox_clicked()
{
    if( FileNameBox *fnbox = dynamic_cast<FileNameBox *>(sender()) ) {
        if( (fnbox->getEditFileName()->isReadOnly() || !fnbox->getEditFileName()->isEnabled() )
                && !fnbox->getEditFileName()->toString().isEmpty() ) {
            Xtring command = ModuleInstance->getModuleSetting( "OPEN_COMMAND" ).toString();
            Xtring path = ModuleInstance->getModuleSetting( "PATH" ).toString();
            if( command.find("{filename}") != Xtring::npos ) {
                command.replace("{filename}", path + "/" + fnbox->getEditFileName()->toString() );
                (void)system( command.c_str() );
            }
        }
    }
}

} // namespace sinpapeles
} // namespace gong

#include <gonggettext.h>
#include "dbappfrmsettingsedit.h"

namespace gong {

FrmSettingsEdit::FrmSettingsEdit( QWidget* parent, WidgetFlags fl )
    : FrmBase ( parent, "FrmSettingsEdit", fl )
{
    setTitle( "Editor de configuración" );
    mainLayout = new QVBoxLayout(this);
    settingsLayout = new QVBoxLayout();

    SettingsTree *st = new SettingsTree( this );
    settingsLayout->addWidget( st );
    mainLayout->addLayout(settingsLayout);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(pushCancel);
    pushCancel->setText( _("Cerrar") );
    pushAccept->setVisible( false );
    mainLayout->addLayout(buttonsLayout);
}

int FrmSettingsEdit::exec()
{
    return !wasCancelled();
}

bool FrmSettingsEdit::canClose()
{
    return FrmBase::canClose();
}

}


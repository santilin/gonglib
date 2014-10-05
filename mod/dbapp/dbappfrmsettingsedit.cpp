#include <gonggettext.h>
#include <gongpushbutton.h>
#include "dbappfrmsettingsedit.h"

namespace gong {

FrmSettingsEdit::FrmSettingsEdit( QWidget* parent, WidgetFlags fl )
    : FrmBase ( parent, "FrmSettingsEdit", fl ), mLastMatchItem(0)
{
    setTitle( "Editor de configuración" );
    mainLayout = new QVBoxLayout(this);
    settingsLayout = new QVBoxLayout();

    pSettingsTree = new SettingsTree( this );
    settingsLayout->addWidget( pSettingsTree );
    mainLayout->addLayout(settingsLayout);

    PushButton *pushSearch = new PushButton( this, _("Buscar") );
    connect( pushSearch, SIGNAL( clicked() ), this, SLOT( slotSearch() ) );
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(pushCancel);
    buttonsLayout->addWidget(pushSearch);
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

void FrmSettingsEdit::slotSearch()
{
    Xtring search_text;
    if( FrmBase::inputBoxString( search_text, _("Texto:"),
                                 _("Buscar en la configuración"), this ) ) {
        mLastMatchItem = pSettingsTree->findText( search_text, 0, mLastMatchItem );
        if( mLastMatchItem != 0 ) {
            pSettingsTree->setCurrentItem( mLastMatchItem, 0, QItemSelectionModel::Select);
        }
    }
}

} // namespace gong


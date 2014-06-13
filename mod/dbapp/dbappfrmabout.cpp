#include "dbappfrmabout.h"

namespace gong {

FrmAbout::FrmAbout(QWidget* parent, WidgetFlags fl)
{
    setTitle( _( "Acerca de " + GongLibraryInstance->getPackageString() ) );
    QHBoxLayout *descLayout = new QHBoxLayout();
    QLabel *descLabel = new QLabel( this );
    descLabel->setText( toGUI( _( "Una bonita aplicación desarrollada por Santilín (gestiong@programacionsocial.net).\n\n"
                                  "\nVersión de construcción del 10/4/2014"	) ) );
    descLayout->addWidget( descLabel );
    pControlsLayout->addLayout( descLayout );

    QLabel *iconthemeLabel = new QLabel( this );
    iconthemeLabel->setText( _("Tema de los iconos: ") + QIcon::themeName() );
    pControlsLayout->addWidget( iconthemeLabel );

    QHBoxLayout *dlsLayout = new QHBoxLayout();
    QLabel *dlsLabel = new QLabel( this );
    dlsLabel->setText( toGUI(_("Datos locales de la aplicación:") + DBAPP->getLocalDataDir() ) );
    dlsLayout->addWidget( dlsLabel );
    pushDirLocalSettings = addButton( this, "Abrir", 0, dlsLayout );
    alignLayout( dlsLayout, true );
    pControlsLayout->addLayout( dlsLayout );

    QHBoxLayout *dgsLayout = new QHBoxLayout();
    QLabel *dgsLabel = new QLabel( this );
    dgsLabel->setText( toGUI(_("Datos globales de la aplicación:") + DBAPP->getGlobalDataDir() ) );
    dgsLayout->addWidget( dgsLabel );
    pushDirGlobalSettings = addButton( this, "Abrir", 0, dgsLayout );
    alignLayout( dgsLayout, true );
    pControlsLayout->addLayout( dgsLayout );

    QHBoxLayout *licenceLayout = new QHBoxLayout();
    QLabel *licenceLabel = new QLabel( this );
    licenceLabel->setText( toGUI( _(
                                      "This package is free software; you can redistribute it and/or\n"
                                      " modify it under the terms of the GNU General Public\n"
                                      " License as published by the Free Software Foundation; either\n"
                                      " version 2 of the License, or (at your option) any later version.\n"
                                      " .\n"
                                      " This package is distributed in the hope that it will be useful,\n"
                                      " but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                                      " MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
                                      " Lesser General Public License for more details\n"
                                  ) ) );
    licenceLayout->addWidget( licenceLabel );
    pControlsLayout->addLayout( licenceLayout );
}


void FrmAbout::button_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender() );
    if( button == pushDirLocalSettings ) {
        (void)system( ("xdg-open "  + DBAPP->getLocalDataDir()).c_str() );
    } else if( button == pushDirGlobalSettings) {
        (void)system( ("xdg-open "  + DBAPP->getGlobalDataDir()).c_str() );
    }
}


} // namespace



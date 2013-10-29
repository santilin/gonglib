#include <qvariant.h>
#include <q3buttongroup.h>
#include <qcombobox.h>
#include <q3header.h>
#include <qlabel.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include <gonggettext.h>
#include "dbappmainwindow.h"
#include "dbappdbapplication.h"

#include "dbappfrmimport_gui.h"

namespace gong {

/*
 *  Constructs a FugitImporter as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The wizard will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal wizard.
 */
GuiFrmImport::GuiFrmImport( QWidget* parent, const char* name, WidgetFlags fl )
    : Q3Wizard( parent, name, fl )

{
    if ( !name )
        setName( "GuiFrmImport" );

    page = new QWidget( this, "page" );
    pageLayout = new Q3VBoxLayout( page, 11, 6, "pageLayout");

    layout14 = new Q3VBoxLayout( 0, 0, 6, "layout14");

    layout12 = new Q3HBoxLayout( 0, 0, 6, "layout12");
    labelDescripcion = new QLabel( page, "Descripción" );
    layout12->addWidget( labelDescripcion );
    editDescripcion = new LineEdit( page, "editDescripcion", Variant::tString );
    layout12->addWidget( editDescripcion );
    layout14->addLayout( layout12 );

    layout11 = new Q3HBoxLayout( 0, 0, 6, "layout11");

    labelFicheroOrigen = new QLabel( page, "labelFicheroOrigen" );
    layout11->addWidget( labelFicheroOrigen );

    editFicheroFugit = new LineEdit( page, "editFicheroFugit" );
    layout11->addWidget( editFicheroFugit );

    pushExaminarFugit = new QPushButton( page, "pushExaminarFugit" );
    layout11->addWidget( pushExaminarFugit );
    layout14->addLayout( layout11 );


    groupFicherosGestiong = new Q3ButtonGroup( page, "groupFicherosGestiong" );
    groupFicherosGestiong->setColumnLayout(0, Qt::Vertical );
    groupFicherosGestiong->layout()->setSpacing( 6 );
    groupFicherosGestiong->layout()->setMargin( 11 );
    groupFicherosGestiongLayout = new Q3VBoxLayout( groupFicherosGestiong->layout() );
    groupFicherosGestiongLayout->setAlignment( Qt::AlignTop );

    treeImport = new Q3ListView( groupFicherosGestiong, "treeImport" );
    treeImport->addColumn( _("Fichero" ) );
    treeImport->setSorting( -1 );
    treeImport->setRootIsDecorated( false );
    groupFicherosGestiongLayout->addWidget( treeImport );
    layout14->addWidget( groupFicherosGestiong );
    pageLayout->addLayout( layout14 );
    addPage( page, "" );

    page_2 = new QWidget( this, "page_2" );
    pageLayout_2 = new Q3VBoxLayout( page_2, 11, 6, "pageLayout_2");

    layout14_2 = new Q3VBoxLayout( 0, 0, 6, "layout14_2");

    layout54 = new Q3HBoxLayout( 0, 0, 6, "layout54");

    textLabel1 = new QLabel( page_2, "textLabel1" );
    layout54->addWidget( textLabel1 );

    comboActualizar = new QComboBox( FALSE, page_2, "comboActualizar" );
    layout54->addWidget( comboActualizar );
    layout14_2->addLayout( layout54 );


    layout11_2_2_2 = new Q3HBoxLayout( 0, 0, 6, "layout11_2_2_2");

    textLabel1_2_3_2 = new QLabel( page_2, "textLabel1_2_3_2" );
    layout11_2_2_2->addWidget( textLabel1_2_3_2 );

    comboOtroEjercicio = new QComboBox( FALSE, page_2, "comboOtroEjercicio" );
    layout11_2_2_2->addWidget( comboOtroEjercicio );
    layout14_2->addLayout( layout11_2_2_2 );
    QSpacerItem* spacer = new QSpacerItem( 20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout14_2->addItem( spacer );
    pageLayout_2->addLayout( layout14_2 );
    addPage( page_2, "" );

    page_3 = new QWidget( this, "page_3" );
    pageLayout_3 = new Q3VBoxLayout( page_3, 11, 6, "pageLayout_3");

    editResumen = new TextEdit( page_3 );
    editResumen->setObjectName( "editResumen" );
    editResumen->setReadOnly( TRUE );
    pageLayout_3->addWidget( editResumen );
    addPage( page_3, "" );
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( pushExaminarFugit, SIGNAL( clicked() ), this, SLOT( pushExaminarFugitClicked() ) );
    languageChange();

}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void GuiFrmImport::languageChange()
{
    setCaption( toGUI(_( "Importación de datos" ) ) );
    labelFicheroOrigen->setText( toGUI(_("Fichero de origen" ) ) );
    labelDescripcion->setText( toGUI(_("Descripción de la importación") ) );
    pushExaminarFugit->setText( toGUI(_("Examinar..." ) ) );
    groupFicherosGestiong->setTitle(
        toGUI( _("Seleccione los ficheros a los que desea importar los datos" ) ) );

    setTitle( page, toGUI(_("Origen y destino de los datos" ) ) );
    textLabel1->setText( toGUI(_("Si los datos a importar ya existen" ) ) );
    comboActualizar->clear();
    comboActualizar->insertItem( toGUI( _( "Cancelar la importación" ) ) );
    comboActualizar->insertItem( toGUI( _( "Actualizar" ) ) );
    comboActualizar->insertItem( toGUI( _( "Ignorar los datos" ) ) );
    comboActualizar->insertItem( toGUI( _( "Preguntar en cada caso" ) ) );
    textLabel1_2_3_2->setText( toGUI( _("Si los datos no pertenecen a este ejercicio" ) ) );
    comboOtroEjercicio->clear();
    comboOtroEjercicio->insertItem( toGUI( _("Cancelar la importación" ) ) );
    comboOtroEjercicio->insertItem( toGUI(_("Importarlos a este ejercicio" ) ) );
    comboOtroEjercicio->insertItem( toGUI(_("Ignorarlos" ) ) );
    setTitle( page_2, toGUI(_( "Opciones de la importación" ) ) );
    setTitle( page_3, toGUI(_( "Resumen de la importación" ) ) );
}

void GuiFrmImport::pushExaminarFugitClicked()
{
    Xtring fn = DBAPP->getOpenFileName( _("Elige el fichero a importar" ),
                                        fromGUI( editFicheroFugit->text() ),
                                        _("Ficheros FUGIT (*.fugit)" ), this );
    if( !fn.isEmpty() )
        editFicheroFugit->setText( fn );
}

} // namespace gong

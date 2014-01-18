
#include <qpushbutton.h>
#include <q3header.h>
#include <q3listview.h>
#include <q3frame.h>
#include <qlabel.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <gongtextedit.h>
#include <gonglineedit.h>

#include <gonggettext.h>
#include "dbappdbapplication.h"
#include "dbappfrmexport_gui.h"

namespace gong {

GuiFrmExport::GuiFrmExport( QWidget* parent, const char* name, WidgetFlags fl )
    : FrmBase( parent, name, fl )
{
    if ( !name )
        setName( "FrmExportar" );
    FrmExportLayout = new Q3HBoxLayout( this, 11, 6, "FrmExportLayout");

    layout12 = new Q3VBoxLayout( 0, 0, 6, "layout12");

    layoutAceptarCancelar = new Q3VBoxLayout( 0, 0, 6, "layoutAceptarCancelar");

    layout10 = new Q3HBoxLayout( 0, 0, 6, "layout10");

    treeExport = new Q3ListView( this, "treeExportar" );
    layout10->addWidget( treeExport );

    frame3 = new Q3Frame( this, "frame3" );
    frame3->setMinimumSize( QSize( 100, 0 ) );
    frame3->setFrameShape( Q3Frame::StyledPanel );
    frame3->setFrameShadow( Q3Frame::Raised );
    frame3Layout = new Q3HBoxLayout( frame3, 11, 6, "frame3Layout");

    layout9 = new Q3VBoxLayout( 0, 0, 6, "layout9");

    lblResumen = new QLabel( frame3, "lblResumen" );
    layout9->addWidget( lblResumen );

    txtResumen = new TextEdit( frame3 );
    txtResumen->setObjectName( "txtResumen" );
    layout9->addWidget( txtResumen );
    frame3Layout->addLayout( layout9 );
    layout10->addWidget( frame3 );
    layoutAceptarCancelar->addLayout( layout10 );

    layout8 = new Q3HBoxLayout( 0, 0, 6, "layout8");

    lblFicheroDestino = new QLabel( this, "lblFicheroDestino" );
    layout8->addWidget( lblFicheroDestino );

    txtDestino = new LineEdit( this, "txtDestino" );
    layout8->addWidget( txtDestino );

    pushBuscarFichero = new QPushButton( this, "pushBuscarFichero" );
    layout8->addWidget( pushBuscarFichero );
    layoutAceptarCancelar->addLayout( layout8 );
    layout12->addLayout( layoutAceptarCancelar );

    accept_cancel_layout = new Q3HBoxLayout( 0, 0, 6, "accept_cancel_layout");
    layout12->addLayout( accept_cancel_layout );
    FrmExportLayout->addLayout( layout12 );
    languageChange();

    // signals and slots connections
    connect( pushBuscarFichero, SIGNAL( clicked() ), this, SLOT( slotBuscarFicheroClicked() ) );
    /*>>>>>UIC_CONSTRUCTOR*/

    txtDestino->setValueType(Variant::tString);
    accept_cancel_layout->addWidget(pushAccept);
    accept_cancel_layout->addWidget(pushCancel);

    // div. configuration of the list view
    treeExport->addColumn( toGUI( _("Ficheros") ) );
    treeExport->setSorting( -1 );
    treeExport->setRootIsDecorated( TRUE );
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void GuiFrmExport::languageChange()
{
    setCaption( toGUI( _( "\x45\x78\x70\x6f\x72\x74\x61\x63\x69\xc3\xb3\x6e\x20\x64\x65\x20\x64\x61\x74\x6f\x73\x20\x64\x65\x20\x47\x65\x73\x74\x69\x4f\x4e\x47" ) ) );
    lblResumen->setText( toGUI( _( "\x52\x65\x73\x75\x6d\x65\x6e\x20\x64\x65\x20\x6c\x61\x20\x65\x78\x70\x6f\x72\x74\x61\x63\x69\xc3\xb3\x6e" ) ) );
    lblFicheroDestino->setText( toGUI( _( "Destino:" ) ) );
    pushBuscarFichero->setText( toGUI( _( "Examinar..." ) ) );
}

void GuiFrmExport::slotCurrentChanged(Q3ListViewItem*)
{
    _GONG_DEBUG_WARNING( "FrmExportar::slotCurrentChanged(QListViewItem*): Not implemented yet" );
}

void GuiFrmExport::slotBuscarFicheroClicked()
{
    Xtring fn = DBAPP->getSaveFileName( _("Elija el fichero donde guardar la exportación"),
                                        Xtring::null,
                                        _("Ficheros FUGIT (*.fugit);;Todos (*)" ), this );
    if( !fn.isEmpty() )
        txtDestino->setText( fn );
}

} // namespace gong

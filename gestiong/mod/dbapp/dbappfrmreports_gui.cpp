#include <gonggettext.h>
#include <QComboBox>
#include "dbappfrmreports_gui.h"

namespace gong {

GuiFrmReports::GuiFrmReports( QWidget* parent, const char* name, WidgetFlags fl )
    : FrmBase( parent, name, fl )
{
    if ( !name )
        setName( "GuiFrmReports" );
    FrmReportsLayout = new QVBoxLayout( this, 11, 6, "FrmReportsLayout");

    layoutMain = new QHBoxLayout( 0, 0, 6, "layoutMain");

    treeReports = new Q3ListView( this, "treeReports" );
    layoutMain->addWidget( treeReports );

    frame3 = new QFrame( this, "frame3" );
    frame3->setMinimumSize( QSize( 250, 0 ) );
    frame3->setFrameShape( QFrame::StyledPanel );
    frame3->setFrameShadow( QFrame::Raised );
    frame3Layout = new QVBoxLayout( frame3, 11, 6, "frame3Layout");

    layout15 = new QVBoxLayout( 0, 0, 6, "layout15");

    layout9_2 = new QHBoxLayout( 0, 0, 6, "layout9_2");

    lblSalida = new QLabel( frame3, "lblSalida" );
    layout9_2->addWidget( lblSalida );

    cmbSalida = new QComboBox( FALSE, frame3, "cmbSalida" );
    layout9_2->addWidget( cmbSalida );
    layout15->addLayout( layout9_2 );

    pushFilter = new QPushButton( frame3, "pushFilter" );
    layout15->addWidget( pushFilter );

    txtFilter = new TextEdit( frame3 );
    layout15->addWidget( txtFilter );

    pushOrder = new QPushButton( frame3, "pushOrder" );
    layout15->addWidget( pushOrder );

    txtOrder = new TextEdit( frame3 );
    layout15->addWidget( txtOrder );

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14");

    lblTitulo = new QLabel( frame3, "lblTitulo" );
    layout14->addWidget( lblTitulo );

    txtTitulo = new LineEdit( frame3, "txtTitulo" );
    layout14->addWidget( txtTitulo );
    layout15->addLayout( layout14 );

    chkResumido = new QCheckBox( frame3, "chkResumido" );
    layout15->addWidget( chkResumido );

    chkAskParameters = new QCheckBox( frame3, "chkAskParameters" );
    layout15->addWidget( chkAskParameters );

    chkAlternateColores = new QCheckBox( frame3, "chkAlternateColores" );
    layout15->addWidget( chkAlternateColores );

    chkApaisado = new QCheckBox( frame3, "chkApaisado" );
    layout15->addWidget( chkApaisado );

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9");

    lblEstilo = new QLabel( frame3, "lblEstilo" );
    layout9->addWidget( lblEstilo );

    cmbEstilo = new QComboBox( FALSE, frame3, "cmbEstilo" );
    layout9->addWidget( cmbEstilo );
    layout15->addLayout( layout9 );
    frame3Layout->addLayout( layout15 );
    layoutMain->addWidget( frame3 );
    FrmReportsLayout->addLayout( layoutMain );

    layoutAceptarCancelar = new QHBoxLayout( 0, 0, 6, "layoutAceptarCancelar");
    FrmReportsLayout->addLayout( layoutAceptarCancelar );
    languageChange();
    resize( QSize(600, 482).expandedTo(minimumSizeHint()) );
    setAttribute( Qt::WA_WState_Polished );

    pushAccept->setText( toGUI( _("Imprimir") ) );
    layoutAceptarCancelar->addWidget(pushAccept);
    layoutAceptarCancelar->addWidget(pushCancel);

    // div. configuration of the list view
    treeReports->addColumn( toGUI( _("Informes" ) ) );
    treeReports->addColumn( toGUI( _("Localización") ) );
    treeReports->setSorting( -1 );
    treeReports->setRootIsDecorated( TRUE );
}





/*
 *  Destroys the object and frees any allocated resources
 */
GuiFrmReports::~GuiFrmReports()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void GuiFrmReports::languageChange()
{
    setCaption( toGUI( _( "Informes" ) ) ) ;
    lblSalida->setText( toGUI( _( "Salida" ) ) ) ;
    cmbSalida->clear();
    cmbSalida->insertItem( toGUI( _( "Pantalla" ) ) ) ;
    cmbSalida->insertItem( toGUI( _( "PDF" ) ) ) ;
    cmbSalida->insertItem( toGUI( _( "OpenOffice Calc" ) ) ) ;
    cmbSalida->insertItem( toGUI( _( "Postscript" ) ) ) ;
    cmbSalida->insertItem( toGUI( _( "Fichero de texto" ) ) ) ;
    cmbSalida->insertItem( toGUI( _( "GNumeric" ) ) ) ;
//        cmbSalida->insertItem( toGUI( _( "HTML 1 página" ) ) ) ;
//        cmbSalida->insertItem( toGUI( _( "HTML multipágina" ) ) ) ;
//        cmbSalida->insertItem( toGUI( _( "PDF" ) ) ) ;
//        cmbSalida->insertItem( toGUI( _( "XML" ) ) ) ;
//        cmbSalida->insertItem( toGUI( _( "Impresora" ) ) ) ;
    pushFilter->setText( toGUI( _( "Filtro..." ) ) ) ;
    pushOrder->setText( toGUI( _( "Orden..." ) ) ) ;
    lblTitulo->setText( toGUI( _( "Título" ) ) ) ;
    chkResumido->setText( toGUI( _( "Resumido" ) ) ) ;
    chkAskParameters->setText( toGUI( _( "Preguntar parámetros adicionales" ) ) ) ;
    chkAlternateColores->setText( toGUI( _( "Alternar colores en las líneas" ) ) ) ;
    lblEstilo->setText( toGUI( _( "Estilo" ) ) ) ;
    chkApaisado->setText( toGUI( _( "Apaisado" ) ) ) ;
    cmbEstilo->clear();
    cmbEstilo->insertItem( toGUI( _( "Normal" ) ) ) ;
}

}

#include <qpushbutton.h>
#include <q3frame.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include <rtkparameter.h>
#include <rtkreport.h>
#include "dbappreportqtfrmparams.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

ReportQtFrmParams::ReportQtFrmParams(Report &r, QWidget *parent, const char *name, WidgetFlags f)
    :QDialog(parent, name, f), mReport(r)
{
    if( !name )
        name = "ReportQtFrmParams";

    Q3VBoxLayout *pFormLayout = new Q3VBoxLayout( this, 2, 0, "pFormLayout");
    Q3VBoxLayout *pMainLayout = new Q3VBoxLayout( 0, 0, 0, "pMainLayout" );

    pushAceptar = new QPushButton( trUtf8("&Aceptar" ), this, "pushAceptar" );
    pushAceptar->setDefault( true );
    pushCancelar = new QPushButton( trUtf8("&Cancelar" ), this, "pushCancelar" );

    // Parameters controls frame
    Q3VBoxLayout *pControlsFrameLayout = new Q3VBoxLayout( 0, 0, 0, "pControlsFrameLayout" );
    Q3Frame *pControlsFrame = new Q3Frame( this, "pControlsFrame" );
    pControlsFrameLayout->addWidget( pControlsFrame );
    pControlsFrame->setSizePolicy( QSizePolicy( QSizePolicy::Expanding,
                                   QSizePolicy::Expanding, 0, 0,
                                   pControlsFrame->sizePolicy().hasHeightForWidth() ) );
    pControlsFrame->setPaletteForegroundColor( QColor( 0, 0, 127 ) );
    pControlsFrame->setFrameShape( Q3Frame::StyledPanel );
    pControlsFrame->setFrameShadow( Q3Frame::Plain );
    // Layout donde se incluirán los controles de edición
    Q3VBoxLayout *pControlsLayout = new Q3VBoxLayout( pControlsFrame, 10, 6, "pControlsLayout" );

    // Botones
    Q3HBoxLayout *pButtonsLayout = new Q3HBoxLayout( 0, 5, 6, "pButtonsLayout" );
    QSpacerItem *spacer1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pButtonsLayout->addItem( spacer1 );
    pButtonsLayout->addWidget( pushCancelar );
    pButtonsLayout->addWidget( pushAceptar );
    // signals and slots connections
    connect( pushAceptar, SIGNAL( clicked() ), this, SLOT( accept() ));
    connect( pushCancelar, SIGNAL( clicked() ), this, SLOT( reject() ));


    // Add parameter controls to controls frame
    for( uint i=0; i < r.getParametersCount(); i++ ) {
        Parameter *p = r.getParameter( i );
        switch( p->type() ) {
        case Variant::tString:
        default: /// TODO
            Q3HBoxLayout *playout = new Q3HBoxLayout( 0, 5, 6, (Xtring(p->name()) + "_layout").c_str() );
            QLabel *lbl = new QLabel(
                QString::fromLocal8Bit(strempty(p->caption()) ? p->name():p->caption()), this );
            QLineEdit *edit = new QLineEdit(QString::fromLocal8Bit(p->value()), this, p->name());
            playout->addWidget( lbl );
            playout->addWidget( edit );
            pControlsLayout->addLayout(playout);
            mControls.push_back(edit);
            break;
        }
    }

    setCaption( QString::fromLocal8Bit( "Parámetros de " ) + QString::fromLocal8Bit(r.name() ) );

    pMainLayout->addLayout( pControlsFrameLayout );
    pMainLayout->addLayout( pButtonsLayout );

    pFormLayout->addLayout( pMainLayout );
    setMinimumSize( QSize(800, 300) );
}


ReportQtFrmParams::~ReportQtFrmParams()
{
}

void ReportQtFrmParams::accept()
{
    for( uint i=0; i < mReport.getParametersCount(); i++ ) {
        Parameter *p = mReport.getParameter( i );
        switch( p->type() ) {
        case Variant::tString:
        default: /// TODO
            QLineEdit *edit = static_cast<QLineEdit *>(mControls[i]);
            p->setValue( edit->text().local8Bit() );
            break;
        }
    }
    QDialog::accept();
}

} // namespace gong


#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QImage>
#include <QPixmap>
#include <QBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>

#include <gonggettext.h>
#include <gongtextedit.h>
#include <gonglineedit.h>
#include <gongdate.h>
#include "dbappdbapplication.h"
#include "dbappfrmlogin_gui.h"

namespace gong {

GuiFrmLogin::GuiFrmLogin( QWidget* parent, const char* name, WidgetFlags fl )
    : QDialog( parent, name, true, fl )
{
    if ( !name )
        setName( "GuiFrmLogin" );
    if( !image0.load( toGUI(DBAPP->getGonglibDataDir() + Xtring(DBAPP->getPackageName()).lower() + "logo.jpg") ) )
        image0.load( toGUI(DBAPP->getGonglibDataDir() + "dbapp/logogestiong.jpg") );

    QVBoxLayout *formLayout = new QVBoxLayout( this );

    pTabWidget = new QTabWidget( this );

    pageConexion = new QWidget( this, "pageConexion" );
    pageConexionLayout = new QVBoxLayout( pageConexion );

    layout96 = new QVBoxLayout();

    layout82 = new QVBoxLayout();

    textWelcome = new QLabel( pageConexion, "textWelcome" );
    textWelcome->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, textWelcome->sizePolicy().hasHeightForWidth() ) );
    QFont textWelcome_font(  textWelcome->font() );
    textWelcome_font.setPointSize( 16 );
    textWelcome->setFont( textWelcome_font );
    layout82->addWidget( textWelcome );

    layout81 = new QHBoxLayout();

    layout80 = new QVBoxLayout();

    PixmapGestiong = new QLabel( pageConexion, "PixmapGestiong" );
    PixmapGestiong->setEnabled( TRUE );
    PixmapGestiong->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PixmapGestiong->sizePolicy().hasHeightForWidth() ) );
    PixmapGestiong->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    PixmapGestiong->setFrameShape( QFrame::StyledPanel );
    PixmapGestiong->setFrameShadow( QLabel::Sunken );
    PixmapGestiong->setLineWidth( 3 );
    PixmapGestiong->setPixmap( image0 );
    PixmapGestiong->setScaledContents( FALSE );
    PixmapGestiong->setAlignment( int( Qt::AlignTop | Qt::AlignHCenter ) );
    layout80->addWidget( PixmapGestiong );
    spacer16 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout80->addItem( spacer16 );
    layout81->addLayout( layout80 );

    textDetailed = new TextEdit( pageConexion );
    textDetailed->setObjectName( "textDetailed" );
#ifdef TODO
    textDetailed->setResizePolicy( QTextEdit::Manual );
    textDetailed->setWordWrap( QTextEdit::WidgetWidth );
#endif
    textDetailed->setReadOnly( TRUE );
    layout81->addWidget( textDetailed );
    layout82->addLayout( layout81 );
    layout96->addLayout( layout82 );

    layout95 = new QHBoxLayout();

    groupBox2 = new QGroupBox( pageConexion, "groupBox2" );
//     groupBox2->setColumnLayout(0, Qt::Vertical );
//     groupBox2->layout()->setSpacing( 6 );
//     groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QVBoxLayout( groupBox2 );
    groupBox2Layout->setAlignment( Qt::AlignTop );

    layout94 = new QVBoxLayout();

    layout63_2 = new QHBoxLayout();

    labelUser = new QLabel( groupBox2, "labelUser" );
    labelUser->setMinimumSize( QSize( 120, 0 ) );
    labelUser->setAlignment( int( Qt::AlignVCenter | Qt::AlignRight ) );
    layout63_2->addWidget( labelUser );

    editUser = new LineEdit( groupBox2, "editUser" );
    layout63_2->addWidget( editUser );
    layout94->addLayout( layout63_2 );

    layout64_2 = new QHBoxLayout( 0, 0, 6, "layout64_2");

    labelPassword = new QLabel( groupBox2, "labelPassword" );
    labelPassword->setMinimumSize( QSize( 120, 0 ) );
    layout64_2->addWidget( labelPassword );

    editPassword = new LineEdit( groupBox2, "editPassword" );
    editPassword->setEchoMode( QLineEdit::Password );
    layout64_2->addWidget( editPassword );
    layout94->addLayout( layout64_2 );

    chkSavePassword = new QCheckBox( groupBox2, "chkSavePassword" );
    layout94->addWidget( chkSavePassword );

    chkAutoLogin = new QCheckBox( groupBox2, "chkAutoLogin" );
    layout94->addWidget( chkAutoLogin );
    groupBox2Layout->addLayout( layout94 );
    layout95->addWidget( groupBox2 );

    groupBox1 = new QGroupBox( pageConexion, "groupBox1" );
//     groupBox1->setColumnLayout(0, Qt::Vertical );
//     groupBox1->layout()->setSpacing( 6 );
//     groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QVBoxLayout( groupBox1 );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    layout46 = new QVBoxLayout();

    layout11_2 = new QHBoxLayout();

    labelDriver_2 = new QLabel( groupBox1, "labelDriver_2" );
    labelDriver_2->setMinimumSize( QSize( 80, 0 ) );
    layout11_2->addWidget( labelDriver_2 );

    comboDriver = new QComboBox( FALSE, groupBox1, "comboDriver" );
    comboDriver->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, comboDriver->sizePolicy().hasHeightForWidth() ) );
    comboDriver->setMinimumSize( QSize( 150, 0 ) );
    layout11_2->addWidget( comboDriver );
    layout46->addLayout( layout11_2 );

    QHBoxLayout *layoutdbname = new QHBoxLayout( 0, 0, 6, "layoutdbname");

    labelDatabaseName = new QLabel( groupBox1, "labelDatabaseName" );
    labelDatabaseName->setMinimumSize( QSize( 80, 0 ) );
    layoutdbname->addWidget( labelDatabaseName );

    editDatabaseName = new LineEdit( groupBox1, "editDatabaseName" );
    layoutdbname->addWidget( editDatabaseName );
    layout46->addLayout( layoutdbname );


    layout62_2 = new QHBoxLayout();

    labelHost_3 = new QLabel( groupBox1 );
    labelHost_3->setMinimumSize( QSize( 80, 0 ) );
    layout62_2->addWidget( labelHost_3 );

    editHost = new LineEdit( groupBox1, "editHost" );
    layout62_2->addWidget( editHost );
    layout46->addLayout( layout62_2 );

    layout39_2 = new QHBoxLayout();

    labelPort_2 = new QLabel( groupBox1 );
    labelPort_2->setMinimumSize( QSize( 80, 0 ) );
    layout39_2->addWidget( labelPort_2 );

    editPort = new LineEdit( groupBox1, "editPort" );
    layout39_2->addWidget( editPort );
    layout46->addLayout( layout39_2 );

    pushCreateDatabase = new QPushButton( groupBox1, "pushCreateDatabase" );
    pushCreateDatabase->setEnabled( true );
    layout46->addWidget( pushCreateDatabase );
    groupBox1Layout->addLayout( layout46 );

    layout95->addWidget( groupBox1 );
    layout96->addLayout( layout95 );
    pageConexionLayout->addLayout( layout96 );
    pTabWidget->addTab( pageConexion, toGUI(_("Conexión") ) );

    pageCreaDB = new QWidget( this, "pageCreaDB" );
    pageCreaDBLayout = new QVBoxLayout( pageCreaDB );

    layout109 = new QVBoxLayout();

    layout75 = new QVBoxLayout();

    textCreateDBWelcome = new QLabel( pageCreaDB, "textCreateDBWelcome" );
    textCreateDBWelcome->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, textCreateDBWelcome->sizePolicy().hasHeightForWidth() ) );
    QFont textCreateDBWelcome_font(  textCreateDBWelcome->font() );
    textCreateDBWelcome_font.setPointSize( 16 );
    textCreateDBWelcome->setFont( textCreateDBWelcome_font );
    layout75->addWidget( textCreateDBWelcome );

    layout74 = new QHBoxLayout();

    layout73 = new QVBoxLayout();

    PixmapGestiong_3 = new QLabel( pageCreaDB, "PixmapGestiong_3" );
    PixmapGestiong_3->setEnabled( TRUE );
    PixmapGestiong_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PixmapGestiong_3->sizePolicy().hasHeightForWidth() ) );
    PixmapGestiong_3->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    PixmapGestiong_3->setFrameShape( QFrame::StyledPanel );
    PixmapGestiong_3->setFrameShadow( QLabel::Sunken );
    PixmapGestiong_3->setLineWidth( 3 );
    PixmapGestiong_3->setPixmap( image0 );
    PixmapGestiong_3->setScaledContents( FALSE );
    PixmapGestiong_3->setAlignment( int( Qt::AlignTop | Qt::AlignHCenter ) );
    layout73->addWidget( PixmapGestiong_3 );
    spacer14 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout73->addItem( spacer14 );
    layout74->addLayout( layout73 );

    textCreateDBDetailed = new TextEdit( pageCreaDB );
    textCreateDBDetailed->setObjectName( "textCreateDBDetailed" );
    textCreateDBDetailed->setReadOnly( TRUE );
    layout74->addWidget( textCreateDBDetailed );
    layout75->addLayout( layout74 );
    layout109->addLayout( layout75 );

    layout108 = new QHBoxLayout( 0, 0, 6, "layout108");

    groupBox3 = new QGroupBox( pageCreaDB, "groupBox3" );
//     groupBox3->setColumnLayout(0, Qt::Vertical );
//     groupBox3->layout()->setSpacing( 6 );
//     groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QVBoxLayout( groupBox3 );
    groupBox3Layout->setAlignment( Qt::AlignTop );


    layout70 = new QVBoxLayout();

    layout83_2 = new QHBoxLayout();

    labelCreateUserUser_2 = new QLabel( groupBox3 );
    labelCreateUserUser_2->setMinimumSize( QSize( 120, 0 ) );
    layout83_2->addWidget( labelCreateUserUser_2 );

    editCreateUserUser = new LineEdit( groupBox3, "editCreateUserUser" );
    layout83_2->addWidget( editCreateUserUser );
    layout70->addLayout( layout83_2 );

    layout84_2 = new QHBoxLayout();
    labelCreateUserPassword_2 = new QLabel( groupBox3 );
    labelCreateUserPassword_2->setMinimumSize( QSize( 120, 0 ) );
    layout84_2->addWidget( labelCreateUserPassword_2 );
    editCreateUserPassword = new LineEdit( groupBox3, "editCreateUserPassword" );
    editCreateUserPassword->setEchoMode( QLineEdit::Password );
    layout84_2->addWidget( editCreateUserPassword );
    layout70->addLayout( layout84_2 );

    groupBox3Layout->addLayout( layout70 );
    layout108->addWidget( groupBox3 );

    groupBox4 = new QGroupBox( pageCreaDB, "groupBox4" );
//     groupBox4->setColumnLayout(0, Qt::Vertical );
//     groupBox4->layout()->setSpacing( 6 );
//     groupBox4->layout()->setMargin( 11 );
    groupBox4Layout = new QVBoxLayout( groupBox4 );
    groupBox4Layout->setAlignment( Qt::AlignTop );

    layout77_3 = new QVBoxLayout();

    layout59_2 = new QHBoxLayout();

    labelCreateDBDriver = new QLabel( groupBox4, "labelCreateDBDriver" );
    labelCreateDBDriver->setAlignment( int( Qt::AlignVCenter | Qt::AlignRight ) );
    layout59_2->addWidget( labelCreateDBDriver );

    comboCreateDBDriver = new QComboBox( FALSE, groupBox4, "comboCreateDBDriver" );
    comboCreateDBDriver->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, comboCreateDBDriver->sizePolicy().hasHeightForWidth() ) );
    comboCreateDBDriver->setMinimumSize( QSize( 150, 0 ) );
    layout59_2->addWidget( comboCreateDBDriver );

    labelCreateDBPort = new QLabel( groupBox4, "labelCreateDBPort" );
    labelCreateDBPort->setAlignment( int( Qt::AlignVCenter | Qt::AlignRight ) );
    layout59_2->addWidget( labelCreateDBPort );

    editCreateDBPort = new LineEdit( groupBox4, "editCreateDBPort" );
    layout59_2->addWidget( editCreateDBPort );
    layout77_3->addLayout( layout59_2 );

    layout78 = new QHBoxLayout( 0, 0, 6, "layout78");

    labelCreateDBHost = new QLabel( groupBox4, "labelCreateDBHost" );
    labelCreateDBHost->setMinimumSize( QSize( 80, 0 ) );
    labelCreateDBHost->setAlignment( int( Qt::AlignVCenter | Qt::AlignRight ) );
    layout78->addWidget( labelCreateDBHost );

    editCreateDBHost = new LineEdit( groupBox4, "editCreateDBHost" );
    layout78->addWidget( editCreateDBHost );
    layout77_3->addLayout( layout78 );


    QHBoxLayout *layoutcreatedbname = new QHBoxLayout();
    labelCreateDBName = new QLabel( groupBox4 );
    labelCreateDBName->setMinimumSize( QSize( 120, 0 ) );
    layoutcreatedbname->addWidget( labelCreateDBName );
    editCreateDBName = new LineEdit( groupBox4, "editCreateDBName" );
    layoutcreatedbname->addWidget( editCreateDBName );
    layout77_3->addLayout( layoutcreatedbname );

    layout77 = new QHBoxLayout( 0, 0, 6, "layout77");

    labelCreateDBUser = new QLabel( groupBox4 );
    labelCreateDBUser->setMinimumSize( QSize( 80, 0 ) );
    layout77->addWidget( labelCreateDBUser );

    editCreateDBUser = new LineEdit( groupBox4, "CreateDBUser" );
    layout77->addWidget( editCreateDBUser );
    layout77_3->addLayout( layout77 );

    layout79 = new QHBoxLayout();

    labelCreateDBPassword = new QLabel( groupBox4 );
    labelCreateDBPassword->setMinimumSize( QSize( 80, 0 ) );
    layout79->addWidget( labelCreateDBPassword );

    editCreateDBPassword = new LineEdit( groupBox4, "CreateDBPassword" );
    editCreateDBPassword->setEchoMode( QLineEdit::Password );
    layout79->addWidget( editCreateDBPassword );
    layout77_3->addLayout( layout79 );

    groupBox4Layout->addLayout( layout77_3 );
    layout108->addWidget( groupBox4 );
    layout109->addLayout( layout108 );
    pageCreaDBLayout->addLayout( layout109 );
    pTabWidget->addTab( pageCreaDB, toGUI( _("Crear") ) );

    pTabWidget->setTabEnabled( 1, false );

    languageChange();
    resize( QSize(694, 523).expandedTo(minimumSizeHint()) );

    formLayout->addWidget( pTabWidget );
    QHBoxLayout *pButtonsLayout = new QHBoxLayout();
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this );
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    pButtonsLayout->addWidget( buttonBox );
    formLayout->addLayout( pButtonsLayout );

    editPort->setValueType( Variant::tInt );
    editPort->setMask("00000");
    editPort->setAlignment(Qt::AlignLeft);
    editCreateDBPort->setValueType( Variant::tInt );
    editCreateDBPort->setMask("00000");
    editCreateDBPort->setAlignment(Qt::AlignLeft);
    editCreateDBPort->setText(0);
#if 0
    editNumeroDigitos->setValueType( Variant::tInt );
    editNumeroDigitos->setMask("00");
    editNumeroDigitos->setAlignment(Qt::AlignLeft);
    editNumeroDigitos->setText(10);
#endif
}

/*<<<<<UIC_REST*/

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void GuiFrmLogin::languageChange()
{
//     setWindowTitle( toGUI( _("Conexión con la base de datos" ) ) );
    textWelcome->setText( QString::null );
    groupBox2->setTitle( toGUI( _( "Acceso" ) ) );
    labelUser->setText( toGUI( _( "Usuaria" ) ) );
    labelPassword->setText( toGUI( _("Contraseña" ) ) );
    chkSavePassword->setText( toGUI( _("Guardar contraseña") ) );
    chkAutoLogin->setText( toGUI( _( "Conectar sin preguntar" ) ) );
    groupBox1->setTitle( toGUI( _("Localización de la base de datos" ) ) );
    labelDriver_2->setText( toGUI( _( "Tipo de base de datos" ) ) );
    labelDatabaseName->setText( toGUI( _( "Nombre de la base de datos" ) ) );
    labelHost_3->setText( toGUI(_( "Dirección del servidor" ) ) );
    labelPort_2->setText( toGUI( _( "Puerto" ) ) );
    pushCreateDatabase->setText( toGUI( _( "C&rear la base de datos" ) ) );
    textCreateDBWelcome->setText( QString::null );
    textCreateDBDetailed->setText( QString::null );
    groupBox3->setTitle( toGUI( _("Acceso a la base de datos" ) ) );
    labelCreateUserUser_2->setText( toGUI( _( "Usuaria" ) ) );
    labelCreateUserPassword_2->setText( toGUI( _("Contraseña" ) ) );
    groupBox4->setTitle( toGUI( _("Localización de la base de datos" ) ) );
    labelCreateDBDriver->setText( toGUI( _( "Tipo" ) ) );
    labelCreateDBPort->setText( toGUI( _( "Puerto" ) ) );
    labelCreateDBHost->setText( toGUI( _( "Dirección del servidor" ) ) );
    labelCreateDBUser->setText( toGUI( _( "Nombre de la superusuaria" ) ) );
    labelCreateDBPassword->setText( toGUI( _("Contraseña de la superusuaria" ) ) );
    labelCreateDBName->setText( toGUI( _( "Nombre de la base de datos" ) ) );
}
/*>>>>>UIC_REST*/

} // namespace gong

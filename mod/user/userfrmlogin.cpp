#include <dbappdbapplication.h>
#include "usermodule.h"
#include "userfrmlogin.h"

namespace gong {
namespace user {

FrmLogin::FrmLogin(QWidget* parent)
    : QDialog(parent)
{

    pFormLayout = new QVBoxLayout(this);
    pFormLayout->setObjectName( "FormLayout" );
    pMainLayout = new QVBoxLayout();
    pMainLayout->setObjectName( "MainLayout" );

    pControlsFrame = new QWidget(pTabWidget);
    pControlsFrame->setObjectName("ControlsFrame");
    pControlsLayout = new QVBoxLayout(pControlsFrame);
    pControlsLayout->setObjectName("ControlsLayout");
    pMainLayout->addWidget(pTabWidget);

//  	QSpacerItem* spacer = new QSpacerItem ( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
//  	pControlsFrameLayout->addItem ( spacer );

    pButtonsLayout = new QHBoxLayout();
    pButtonsLayout->setObjectName( "ButtonsLayout" );
    pButtonsLayout->addWidget ( pushAccept );
    pButtonsLayout->addWidget ( pushCancel );

    pMainLayout->addLayout ( pButtonsLayout );
    pFormLayout->addLayout ( pMainLayout );


    setCaption( toGUI("Acceso a " + DBAPP->getPackageString() ) );
	editUser = addInput( 0, Xtring::null, "USUARIA", "LOGIN");
	editPassword = addInput( 0, Xtring::null, "USUARIA", "PASSWORD");
    editUser->setText(DBAPP->getAppSetting ( "USER" ).toString() );
	editPassword->setText(DBAPP->getAppSetting ( "PASSWORD" ).toString());
	checkSavePassword = addCheckBox( this, _("Guardar contraseña"),
									   DBAPP->getAppSetting ( "SAVEPASSWORD", false ).toBool());
	checkAutoLogin = addCheckBox( this, _("Entrar sin preguntar"),
									DBAPP->getAppSetting ( "AUTOLOGIN", false ).toBool());
}

LineEdit *FrmLogin::addInput ( QWidget *parent, const Xtring &caption,
                                const Variant &value, const Xtring &style,
                                const char *name, QBoxLayout *layout )
{
    LineEdit *edit = new LineEdit ( parent ? parent : pControlsFrame,
                                    "edit_" + Xtring(name), value.type() );
    connect ( edit, SIGNAL ( validate ( QWidget *, bool * ) ),
              this, SLOT ( validate_input ( QWidget *, bool * ) ) );
    if ( !style.isEmpty() )
        FrmEditRec::applyBasicStyle ( edit, style );
    if ( value.toString() == "today()" )
        edit->setText ( DateTime::currentDateTime() );
    else if ( value.type() == Variant::tDate || value.type() == Variant::tDateTime ) {
        if ( value.isEmpty() )
            edit->setText ( DateTime::currentDateTime() );
    }
    else
        edit->setText ( value );
    QLabel *label = new QLabel ( parent ? parent : pControlsFrame,
                                 ( "label_" + Xtring(name) ).c_str() );
    label->setText ( toGUI ( caption ) );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( label );
    hlay->addWidget ( edit );
    if( layout )
        layout->addLayout( hlay );
    else
        pControlsLayout->addLayout( hlay);
    if ( !pFocusWidget )
        pFocusWidget = edit;
    return edit;
}

CheckBox* FrmLogin::addCheckBox(QWidget *parent, const Xtring& caption,
                                 bool value, const char *name, QBoxLayout *layout)
{
    CheckBox *chkbox = new CheckBox( parent ? parent : pControlsFrame, name, caption );
    chkbox->setChecked( value );
    if( !layout ) {
        layout = new QHBoxLayout();
        pControlsLayout->addLayout ( layout );
    }
    layout->addWidget( chkbox );
    chkbox->setEdited( false );
    connect( chkbox, SIGNAL( stateChanged( int ) ), this, SLOT( combo_activated( int ) ) );
    return chkbox;
}



} // namespace user
} // namespace gong

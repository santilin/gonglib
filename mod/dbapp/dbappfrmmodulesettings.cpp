#include <gongimagebox.h>
#include "dbappfrmmodulesettings.h"
#include <QGraphicsBlurEffect>

namespace gong {

FrmModuleSettings::FrmModuleSettings(dbModuleSetting::Scope scope, QWidget* parent, WidgetFlags fl)
    : FrmCustom( parent, "FrmModuleSettings", fl ), mSettingsScope( scope )
{
	switch( mSettingsScope ) {
    case dbModuleSetting::Machine:
        pSettings = DBAPP->getMachineSettings();
        break;
    case dbModuleSetting::Local:
        pSettings = DBAPP->getUserLocalSettings();
        break;
//     case FrmModuleSettings::GlobalUser:
//         pSettings = DBAPP->getUserGlobalSettings();
//         break;
    case dbModuleSetting::Global:
        pSettings = DBAPP->getGlobalSettings();
        break;
	default:
		throw std::runtime_error( "Wrong parameter scope in FrmModuleSettings" );
    }
    setTitle( pSettings->getDescription() );
    pTabWidget = new QTabWidget( pFrameEdit );
    pInputsLayout->addWidget(pTabWidget);
	QLabel *lbl1 = new QLabel( this );
	lbl1->setStyleSheet( "color:black" );
	lbl1->setText( toGUI( _("Valores en negro = Valores por defecto") ) );
	QLabel *lbl2 = new QLabel( this );
	lbl2->setStyleSheet( "color:green" );
	lbl2->setText( toGUI( _("Valores en verde = cambiados y se usarán") ) );
	QLabel *lbl3 = new QLabel( this );
	lbl3->setStyleSheet( "color:red" );
	lbl3->setText( toGUI( _("Valores en rojo = cambiados pero no se usarán porque tienen preferencia otros valores") ) );
	pInputsLayout->addWidget( lbl1 );
	pInputsLayout->addWidget( lbl2 );
	pInputsLayout->addWidget( lbl3 );
    scatter();
}

void FrmModuleSettings::addModuleSettings(dbModule* module, QVBoxLayout* layout)
{
    const dbModuleSetting *pms;
	if( module )
		pms = module->getModuleSettings();
	else
		pms = DBAPP->getModuleSettings();
    while( pms && pms->type != dbModuleSetting::None ) {
		if( !(pms->scope & mSettingsScope) ) {
			pms++;
			continue;
		}
		QHBoxLayout *hboxlayout = new QHBoxLayout();
		layout->addLayout( hboxlayout );
        ControlInfo scinfo;
		scinfo.resetButton = new QPushButton( this );
		scinfo.resetButton->setFocusPolicy( Qt::ClickFocus );
		scinfo.resetButton->setIcon( QIcon::fromTheme("edit-clear-locationbar-ltr", QIcon(":/edit-clear-locationbar-ltr.png")) );
		scinfo.resetButton->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
		connect( scinfo.resetButton, SIGNAL( clicked() ), this, SLOT( resetButton_clicked() ) );

		hboxlayout->addWidget( scinfo.resetButton );
        scinfo.settinginfo = pms;
		if( module )
			scinfo.modulename = "MODULE." + module->getUnixName() + ".";
		else
			scinfo.modulename = "";
        scinfo.edited = false;
        Variant value = pSettings->getValue( scinfo.modulename.upper() + pms->key, Variant() );
		if( value.type() == Variant::tInvalid )
			scinfo.resetted = true;
		else
			scinfo.resetted = false;
        switch( pms->type ) {
        case dbModuleSetting::Bool:
            scinfo.w = addCheckBox(this, pms->description, value.toBool(), 0, hboxlayout );
            break;
        case dbModuleSetting::Int:
            if( pms->values ) {
                XtringList captions;
                Xtring(pms->values).tokenize( captions, "|" );
                IntList il;
                ComboBoxInt *cb = addComboBoxInt( false, this, pms->description, captions, il, Xtring::null, 0, hboxlayout );
                cb->setCurrentItemByValue( value.toInt() );
                scinfo.w = cb;
            } else {
                scinfo.w = addInput(this, pms->description, value.toInt(), Xtring::null, 0, hboxlayout );
            }
            break;
        case dbModuleSetting::Long:
            scinfo.w = addInput(this, pms->description, value.toLong(), Xtring::null, 0, hboxlayout );
            break;
        case dbModuleSetting::Double:
            scinfo.w = addInput(this, pms->description, value.toDouble(), Xtring::null, 0, hboxlayout );
            break;
        case dbModuleSetting::String:
            if( pms->values ) {
                XtringList captions;
                Xtring(pms->values).tokenize( captions, "|" );
                ComboBoxXtring *cb = addComboBoxXtring( false, this, pms->description, captions, captions, Xtring::null, 0, hboxlayout );
                cb->setCurrentItemByValue( value.toString() );
                scinfo.w = cb;
            } else {
                scinfo.w = addInput(this, pms->description, value.toString(), Xtring::null, 0, hboxlayout );
            }
            break;
        case dbModuleSetting::Text:
            scinfo.w = addTextEditBox(this, pms->description, value.toString(), 0, hboxlayout );
            break;
        case dbModuleSetting::Date:
            scinfo.w = addInput(this, pms->description, value.toDate(), Xtring::null, 0, hboxlayout );
            break;
        case dbModuleSetting::DateTime:
            scinfo.w = addInput(this, pms->description, value.toDateTime(), Xtring::null, 0, hboxlayout );
            break;
        case dbModuleSetting::Time:
            scinfo.w = addInput(this, pms->description, value.toTime(), Xtring::null, 0, hboxlayout );
            break;
        case dbModuleSetting::Money:
            scinfo.w = addInput(this, pms->description, value.toMoney(), Xtring::null, 0, hboxlayout );
            break;
        case dbModuleSetting::Password:
            scinfo.w = addInput(this, pms->description, value.toString(), "PASSWORD", 0, hboxlayout );
            break;
        case dbModuleSetting::FileName:
        {
            FileNameBox *fnbox = addFileNameBox(this, pms->description, 0, hboxlayout );
            fnbox->setFileName( value.toString() );
            scinfo.w = fnbox;
        }
        break;
        case dbModuleSetting::DirName:
        {
            FileNameBox *fnbox = addDirNameBox(this, pms->description, 0, hboxlayout );
            fnbox->setFileName( value.toString() );
            scinfo.w = fnbox;
        }
        break;
        case dbModuleSetting::StringList:
        case dbModuleSetting::dbRecordID:
        case dbModuleSetting::Image: /// TODO
        case dbModuleSetting::None:
            break;
        }
        setControlColor( &scinfo, value );
        mControls.push_back( scinfo );
        pms++;
    }
    QSpacerItem *spacer1 = new QSpacerItem ( 40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addItem ( spacer1 );
}

void FrmModuleSettings::resetButton_clicked()
{
	QPushButton *rb = static_cast< QPushButton *>(sender() );
	for( ControlsList::const_iterator it = mControls.begin(); it != mControls.end(); ++ it ) {
		ControlInfo ci = *it;
		if( ci.resetButton == rb ) {
			setControlValue( ci.w, ci.settinginfo->defaultvalue );
			ci.w->setStyleSheet( "" );
			ci.resetted = true;
		}
	}
}


bool FrmModuleSettings::canClose()
{
    return FrmCustom::canClose();
}

void FrmModuleSettings::scatter()
{
	// DBAPP
	QWidget *moduleWidget = new QWidget(pTabWidget);
	moduleWidget->setObjectName("ControlsFrameDBAPP");
	QVBoxLayout *moduleLayout = new QVBoxLayout( moduleWidget );
	pTabWidget->addTab( moduleWidget, _("Sistema") );
	addModuleSettings( 0, moduleLayout );
	// MODULES
	for ( unsigned int i=0; i< DBAPP->getModules().size(); i++ ) {
        dbModule *module = DBAPP->getModules()[i];
        if( module->getModuleSettings() ) {
            QWidget *moduleWidget = new QWidget(pTabWidget);
            moduleWidget->setObjectName( ("ControlsFrame" + module->getUnixName()).c_str() );
            QVBoxLayout *moduleLayout = new QVBoxLayout( moduleWidget );
            pTabWidget->addTab( moduleWidget, toGUI(module->getUnixName()) );
            addModuleSettings( module, moduleLayout );
        }
    }
}

void FrmModuleSettings::gather()
{
    for( ControlsList::const_iterator it = mControls.begin();
            it != mControls.end(); ++ it ) {
        ControlInfo scinfo = *it;
        if( !scinfo.edited )
            continue;
        Variant old_value = pSettings->getValue(
                                scinfo.modulename.upper() + scinfo.settinginfo->key,
                                scinfo.settinginfo->defaultvalue );
        Variant new_value;
        QWidget *control = scinfo.w;
        if ( LineEdit * edit = dynamic_cast<LineEdit *>( control ) ) {
            new_value = edit->toVariant();
        } else if ( TextEdit * edit = dynamic_cast<TextEdit *>( control ) ) {
            new_value = edit->toString();
        } else if ( RichTextBox * edit = dynamic_cast<RichTextBox *>( control ) ) {
            new_value = edit->toString();
        } else if ( SearchBox * search = dynamic_cast<SearchBox *>( control ) ) {
            new_value = search->getEditCode()->toString();
        } else if ( CheckBox * check = dynamic_cast<CheckBox *>( control ) ) {
            new_value = check->isChecked();
        } else if ( ComboBoxInt *combo = dynamic_cast<ComboBoxInt *>( control ) ) {
            new_value = combo->getCurrentItemValue();
        } else if ( ComboBoxXtring *combo = dynamic_cast<ComboBoxXtring *>( control ) ) {
            new_value = combo->getCurrentItemValue();
        } else if ( ImageBox *image = dynamic_cast<ImageBox *>( control ) ) {
            new_value = image->toData();
        } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( control ) ) {
            new_value = fnbox->getEditFileName()->toVariant();
        } else {
            _GONG_DEBUG_WARNING( Xtring("Control ") + control->name() + " not found" );
        }
        if( new_value.toString() != old_value.toString() ) {
            pSettings->setValue(
                scinfo.modulename.upper() + scinfo.settinginfo->key, new_value );
        }
    }
}

void FrmModuleSettings::validate_input(QWidget *control , bool* )
{
    bool edited = false;
	Variant value;
    if ( LineEdit * edit = dynamic_cast<LineEdit *>( control ) ) {
        if ( edit->edited() ) {
            edited = true;
			value = edit->toString();
		}
    } else if ( TextEdit * edit = dynamic_cast<TextEdit *>( control ) ) {
        if ( edit->isEdited() ) {
            edited = true;
			value = edit->toString();
		}
    } else if ( RichTextBox * edit = dynamic_cast<RichTextBox *>( control ) ) {
        if ( edit->isEdited() ) {
            edited = true;
			value = edit->toString();
		}
    } else if ( SearchBox * search = dynamic_cast<SearchBox *>( control ) ) {
        if ( search->getEditCode()->edited() || search->getEditDesc()->edited() ) {
            edited = true;
			value = search->getEditCode()->toString();
		}
    } else if ( CheckBox * check = dynamic_cast<CheckBox *>( control ) ) {
        if ( check->isEdited() ) {
            edited = true;
			value = check->isOn();
		}
    } else if ( ComboBoxInt *combo = dynamic_cast<ComboBoxInt *>( control ) ) {
        if ( combo->isEdited() ) {
            edited = true;
			value = combo->getCurrentItemValue();
		}
    } else if ( ComboBoxXtring *combo = dynamic_cast<ComboBoxXtring *>( control ) ) {
        if ( combo->isEdited() ) {
            edited = true;
			value = combo->getCurrentItemValue();
		}
    } else if ( ImageBox *image = dynamic_cast<ImageBox *>( control ) ) {
        if ( image->isEdited() ) {
            edited = true;
			value = image->toData();
		}
    } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( control ) ) {
        if( fnbox->getEditFileName()->isEdited() ) {
            edited = true;
			value = fnbox->getFileName();
		}
    }
    if( edited ) {
        for( ControlsList::iterator it = mControls.begin();
                it != mControls.end(); ++ it ) {
            if( it->w == control ) {
				setControlColor( &(*it), value );
                it->edited = true;
				it->resetted = false;
                return;
            }
        }
    }
}

void FrmModuleSettings::setControlValue(QWidget* control, const Variant& value)
{
	bool edited;
    if ( LineEdit * edit = dynamic_cast<LineEdit *>( control ) ) {
		edit->setText( value );
		edited = true;
    } else if ( TextEdit * edit = dynamic_cast<TextEdit *>( control ) ) {
		edit->setText( value.toString() );
		edited = true;
    } else if ( RichTextBox * edit = dynamic_cast<RichTextBox *>( control ) ) {
		edit->setText( value.toString() );
		edited = true;
    } else if ( SearchBox * search = dynamic_cast<SearchBox *>( control ) ) {
		search->getEditCode()->setText( value );
		edited = true;
    } else if ( CheckBox * check = dynamic_cast<CheckBox *>( control ) ) {
		check->setChecked( value.toBool() );
		edited = true;
    } else if ( ComboBoxInt *combo = dynamic_cast<ComboBoxInt *>( control ) ) {
		combo->setCurrentItemByValue( value.toInt() );
		edited = true;
    } else if ( ComboBoxXtring *combo = dynamic_cast<ComboBoxXtring *>( control ) ) {
		combo->setCurrentItemByValue( value.toString() );
		edited = true;
    } else if ( ImageBox *image = dynamic_cast<ImageBox *>( control ) ) {
		image->setImageData( value.toString() );
		edited = true;
    } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( control ) ) {
		fnbox->setFileName( value.toString() );
		edited = true;
    }
    if( edited ) {
        for( ControlsList::iterator it = mControls.begin(); it != mControls.end(); ++ it ) {
			setControlColor( &(*it), value );
            if( it->w == control ) {
                it->edited = true;
                return;
            }
        }
    }
}

/**
 * @brief Si el valor es el valor por defecto, lo pone en negro.
 *        Si es la configuración global y no es por defecto, lo pone en verde
 *        Si es la configuración local y no es por defecto, entonces miramos si la global está por defecto,
 *            en cuyo caso lo ponemos en verde y en caso contrario, en rojo
 *
 * @param ci ...
 * @param value ...
 * @return void
 **/
void FrmModuleSettings::setControlColor(FrmModuleSettings::ControlInfo* ci, const Variant& value)
{
	if( ci->resetted )
		/*|| value.toString().isEmpty()*/
		/*|| value.toString() == ci->settinginfo->defaultvalue */
		ci->w->setStyleSheet( "color:black" );
	else if( mSettingsScope == dbModuleSetting::Local )
		ci->w->setStyleSheet( "color:green" );
	else {
		Xtring s( DBAPP->getUserLocalSetting( ci->modulename.upper() + ci->settinginfo->key, Variant() ).toString() );
		if( s.isEmpty() || ci->settinginfo->defaultvalue == s )
			ci->w->setStyleSheet( "color:green" );
		else
			ci->w->setStyleSheet( "color:red" );
	}
}

void FrmModuleSettings::accept()
{
    gather();
    FrmBase::accept();
}


} // namespace

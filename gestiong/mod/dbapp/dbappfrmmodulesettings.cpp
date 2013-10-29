#include <gongimagebox.h>
#include "dbappfrmmodulesettings.h"

namespace gong {

FrmModuleSettings::FrmModuleSettings(SettingsType type, QWidget* parent, WidgetFlags fl)
    : FrmCustom( parent, "FrmModuleSettings", fl ), mSettingsType( type )
{
    setObjectName( (name() + Xtring::number( type )).c_str() );
    switch( mSettingsType ) {
    case FrmModuleSettings::Machine:
        pSettings = DBAPP->getMachineSettings();
        break;
    case FrmModuleSettings::LocalUser:
        pSettings = DBAPP->getUserLocalSettings();
        break;
    case FrmModuleSettings::GlobalUser:
        pSettings = DBAPP->getUserGlobalSettings();
        break;
    case FrmModuleSettings::Global:
        pSettings = DBAPP->getGlobalSettings();
        break;
    }
    setTitle( pSettings->getDescription() );
    pTabWidget = new QTabWidget( pFrameEdit );
    pInputsLayout->addWidget(pTabWidget);
    scatter();
}

void FrmModuleSettings::addModuleSettings(dbModule* module, QVBoxLayout* layout)
{
    const dbModuleSetting *pms = module->getModuleSettings();
    while( pms && pms->type != dbModuleSetting::None ) {
        SettingsControlInfo scinfo;
        scinfo.settinginfo = pms;
        scinfo.modulename = &module->getUnixName();
        scinfo.edited = false;
        Variant value = pSettings->getValue( "MODULE." + scinfo.modulename->upper() + "." + pms->key,
                                             scinfo.settinginfo->defaultvalue);
        if( value.toString() == scinfo.settinginfo->defaultvalue )
            value = Variant();
        switch( pms->type ) {
        case dbModuleSetting::Bool:
            scinfo.w = addCheckBox(this, pms->description, value.toBool(), 0, layout );
            break;
        case dbModuleSetting::Int:
            if( pms->values ) {
                XtringList captions;
                Xtring(pms->values).tokenize( captions, "|" );
                IntList il;
                ComboBox<int> *cb = addComboBox( this, pms->description, captions, il, Xtring::null, 0, layout );
                cb->setCurrentItemByValue( value.toInt() );
                scinfo.w = cb;
            } else {
                scinfo.w = addInput(this, pms->description, value.toInt(), Xtring::null, 0, layout );
            }
            break;
        case dbModuleSetting::Long:
            scinfo.w = addInput(this, pms->description, value.toLong(), Xtring::null, 0, layout );
            break;
        case dbModuleSetting::Double:
            scinfo.w = addInput(this, pms->description, value.toDouble(), Xtring::null, 0, layout );
            break;
        case dbModuleSetting::String:
            if( pms->values ) {
                XtringList captions;
                Xtring(pms->values).tokenize( captions, "|" );
                ComboBox<Xtring> *cb = addComboBox( this, pms->description, captions, captions, Xtring::null, 0, layout );
                cb->setCurrentItemByValue( value.toString() );
                scinfo.w = cb;
            } else {
                scinfo.w = addInput(this, pms->description, value.toString(), Xtring::null, 0, layout );
            }
            break;
        case dbModuleSetting::Text:
            scinfo.w = addTextEditBox(this, pms->description, value.toString(), 0, layout );
            break;
        case dbModuleSetting::Date:
            scinfo.w = addInput(this, pms->description, value.toDate(), Xtring::null, 0, layout );
            break;
        case dbModuleSetting::DateTime:
            scinfo.w = addInput(this, pms->description, value.toDateTime(), Xtring::null, 0, layout );
            break;
        case dbModuleSetting::Time:
            scinfo.w = addInput(this, pms->description, value.toTime(), Xtring::null, 0, layout );
            break;
        case dbModuleSetting::Money:
            scinfo.w = addInput(this, pms->description, value.toMoney(), Xtring::null, 0, layout );
            break;
        case dbModuleSetting::Password:
            scinfo.w = addInput(this, pms->description, value.toString(), "PASSWORD", 0, layout );
            break;
        case dbModuleSetting::FileName:
        {
            FileNameBox *fnbox = addFileNameBox(this, pms->description, 0, layout );
            fnbox->setFileName( value.toString() );
            scinfo.w = fnbox;
        }
        break;
        case dbModuleSetting::DirName:
        {
            FileNameBox *fnbox = addDirNameBox(this, pms->description, 0, layout );
            fnbox->setFileName( value.toString() );
            scinfo.w = fnbox;
        }
        break;
        case dbModuleSetting::None:
            break;
        }
        mControls.push_back( scinfo );
        pms++;
    }
    QSpacerItem *spacer1 = new QSpacerItem ( 40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addItem ( spacer1 );
}

bool FrmModuleSettings::canClose()
{
    return FrmCustom::canClose();
}

void FrmModuleSettings::scatter()
{
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
    for( SettingsControlsList::const_iterator it = mControls.begin();
            it != mControls.end(); ++ it ) {
        SettingsControlInfo scinfo = *it;
        if( !scinfo.edited )
            continue;
        Variant old_value = pSettings->getValue(
                                "MODULE." + scinfo.modulename->upper() + "." + scinfo.settinginfo->key,
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
        } else if ( ComboBox<int> *combo = dynamic_cast<ComboBox<int> *>( control ) ) {
            new_value = combo->getCurrentItemValue();
        } else if ( ComboBox<Xtring> *combo = dynamic_cast<ComboBox<Xtring> *>( control ) ) {
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
                "MODULE." + scinfo.modulename->upper() + "." +scinfo.settinginfo->key, new_value );
        }
    }
}

void FrmModuleSettings::validate_input(QWidget *control , bool* )
{
    bool edited = false;
    if ( LineEdit * edit = dynamic_cast<LineEdit *>( control ) ) {
        if ( edit->edited() )
            edited = true;
    } else if ( TextEdit * edit = dynamic_cast<TextEdit *>( control ) ) {
        if ( edit->isEdited() )
            edited = true;
    } else if ( RichTextBox * edit = dynamic_cast<RichTextBox *>( control ) ) {
        if ( edit->isEdited() )
            edited = true;
    } else if ( SearchBox * search = dynamic_cast<SearchBox *>( control ) ) {
        if ( search->getEditCode()->edited() || search->getEditDesc()->edited() )
            edited = true;
    } else if ( CheckBox * check = dynamic_cast<CheckBox *>( control ) ) {
        if ( check->isEdited() )
            edited = true;
    } else if ( ComboBox<int> *combo = dynamic_cast<ComboBox<int> *>( control ) ) {
        if ( combo->isEdited() )
            edited = true;
    } else if ( ComboBox<Xtring> *combo = dynamic_cast<ComboBox<Xtring> *>( control ) ) {
        if ( combo->isEdited() )
            edited = true;
    } else if ( ImageBox *image = dynamic_cast<ImageBox *>( control ) ) {
        if ( image->isEdited() )
            edited = true;
    } else if ( FileNameBox *fnbox = dynamic_cast<FileNameBox *>( control ) ) {
        if( fnbox->getEditFileName()->isEdited() )
            edited = true;
    }
    if( edited ) {
        for( SettingsControlsList::iterator it = mControls.begin();
                it != mControls.end(); ++ it ) {
            if( it->w == control ) {
                it->edited = true;
                return;
            }
        }
    }
}

void FrmModuleSettings::accept()
{
    gather();
    FrmBase::accept();
}


} // namespace

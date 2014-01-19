#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <gonggettext.h>
#include <gongfileutils.h>
#include <gongvariant.h>
#include <gongpushbutton.h>
#include "dbappsearchbox.h"
#include "dbappdbapplication.h"
#include "dbappfrmeditrec.h"
#include "dbappfrmcustom.h"
#include "dbappnameslisttable.h"

namespace gong {

DateRangeBox::DateRangeBox( QWidget *parent, const Xtring& name, const Xtring& caption,
                            Variant::Type type, const DateTime date_from, const DateTime date_to,
                            const Xtring &caption_from, const Xtring &caption_to,
                            QBoxLayout *layout )
{
    if( !(pLayout = layout) ) {
        pLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        pLayout->setObjectName( toGUI("layout_" + name) );
    }
    QLabel *label = new QLabel( parent );
    label->setText ( toGUI ( caption.c_str() ) );
    pLayout->addWidget ( label );
    QLabel *label_from = new QLabel( parent );
    if( caption_from.isEmpty() )
        label_from->setText ( _("del") );
    else
        label_from->setText ( toGUI ( caption_from.c_str() ) );
    pLayout->addWidget ( label_from );
    pEditDateFrom = new LineEdit( parent, "Edit" + name + "FechaIni", type );
    pEditDateFrom->setMask( DBAPP->getRegConfig()->getDateMask() );
    pEditDateFrom->setFormat("%x");
    pEditDateFrom->setText( (Date)date_from );
    pLayout->addWidget( pEditDateFrom);
    QLabel *label_to = new QLabel( parent );
    if( caption_to.isEmpty() )
        label_to->setText ( _("al") );
    else
        label_to->setText ( toGUI ( caption_to.c_str() ) );
    pLayout->addWidget ( label_to );
    pEditDateTo = new LineEdit( parent, "Edit" + name + "FechaFin", type );
    pEditDateTo->setMask( DBAPP->getRegConfig()->getDateMask() );
    pEditDateTo->setFormat("%x");
    pEditDateTo->setText( (Date)date_to );
    pLayout->addWidget( pEditDateTo);
    QSpacerItem *spacer1 = new QSpacerItem ( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pLayout->addItem ( spacer1 );
}

Date DateRangeBox::getDateFrom() const
{
    return pEditDateFrom->toDate();
}

DateTime DateRangeBox::getDateTimeFrom() const
{
    return DateTime( getDateFrom(), Time(0,0,0) );
}

Date DateRangeBox::getDateTo() const
{
    return pEditDateTo->toDate() ;
}

DateTime DateRangeBox::getDateTimeTo() const
{
    return DateTime( getDateTo(), Time(23,59,59) );
}


FrmCustom::FrmCustom ( QWidget *parent, const char *name, WidgetFlags f )
    : FrmBase ( parent, name, f )
{
    pFrmCustomLayout = new QVBoxLayout( this );
    pFormLayout = new QVBoxLayout();

    pFrameEdit = new QFrame( this );
    pFrameEdit->setObjectName( "FrameEdit" );
    pFrameEdit->setFrameShape ( QFrame::StyledPanel );
    pFrameEdit->setFrameShadow ( QFrame::Raised );

    pFrameEditLayout = new QVBoxLayout( pFrameEdit );
    pInputsLayout = new QVBoxLayout();
    pInputsLayout->setObjectName( "InputsLayout" );

    // controles de edicion, se generan despues

    // Resto de layouts
    pFrameEditLayout->addLayout ( pInputsLayout );
    pFormLayout->addWidget ( pFrameEdit );

//  	QSpacerItem* spacer = new QSpacerItem ( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
//  	pFrameEditLayout->addItem ( spacer );

    pButtonsLayout = new QHBoxLayout();
    pButtonsLayout->setObjectName( "ButtonsLayout" );
    pButtonsLayout->addWidget ( pushAccept );
    pButtonsLayout->addWidget ( pushCancel );

    pFormLayout->addLayout ( pButtonsLayout );
    pFrmCustomLayout->addLayout ( pFormLayout );
}

LineEdit *FrmCustom::addInput ( QWidget *parent, const Xtring &caption,
                                const Variant &value, const Xtring &style,
                                const char *name, QBoxLayout *layout )
{
    LineEdit *edit = new LineEdit ( parent ? parent : pFrameEdit,
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
    QLabel *label = new QLabel ( parent ? parent : pFrameEdit,
                                 ( "label_" + Xtring(name) ).c_str() );
    label->setText ( toGUI ( caption ) );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( label );
    hlay->addWidget ( edit );
    if( layout )
        layout->addLayout( hlay );
    else
        pInputsLayout->addLayout( hlay);
    if ( !pFocusWidget )
        pFocusWidget = edit;
    return edit;
}

QLabel *FrmCustom::addLabel( QWidget *parent, const Xtring &caption,
                             const Variant &value, const Xtring &style,
                             const char *name, QBoxLayout *layout )
{
    QLabel *label = new QLabel( toGUI( value.toString() ), parent ? parent : pFrameEdit );
    QLabel *cap_label = new QLabel ( toGUI( caption ), parent ? parent : pFrameEdit );
    if( name )
        label->setObjectName( name );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( cap_label );
    hlay->addWidget ( label );
    if( layout )
        layout->addLayout( hlay );
    else
        pInputsLayout->addLayout( hlay);
    return label;
}

TextEdit* FrmCustom::addTextEditBox(QWidget* parent, const Xtring& caption,
                                    const Xtring& value, const char* name, QBoxLayout* layout)
{
    TextBox *textbox = new TextBox( parent ? parent : pFrameEdit,
                                    name ? (Xtring("editTextBox_") + name).c_str() : "", "", false);
    connect ( textbox, SIGNAL ( validate ( QWidget *, bool * ) ),
              this, SLOT ( validate_input ( QWidget *, bool * ) ) );
    textbox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    textbox->setText( value );
    QLabel *label = new QLabel ( parent ? parent : pFrameEdit,
                                 ( "label_" + Xtring(name) ).c_str() );
    label->setText( toGUI(caption) );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( label );
    hlay->addLayout( textbox->getLayout() );
    if( layout )
        layout->addLayout( hlay );
    else
        pInputsLayout->addLayout ( hlay );
    if ( !pFocusWidget )
        pFocusWidget = textbox;
    return textbox;
}


RichTextBox* FrmCustom::addRichTextBox(QWidget* parent, const Xtring& caption,
                                       const char* name, QBoxLayout *layout )
{
    RichTextBox *rich = new RichTextBox( parent ? parent : pFrameEdit,
                                         name ? (Xtring("editRich_") + name).c_str() : "", "", false);
    rich->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    connect( rich, SIGNAL ( validate ( QWidget *, bool * ) ),
              this, SLOT ( validate_input ( QWidget *, bool * ) ) );    QLabel *label = new QLabel ( parent ? parent : pFrameEdit,
                                 ( "label_" + Xtring(name) ).c_str() );
    label->setText ( toGUI(caption) );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( label );
    hlay->addLayout( rich->getLayout() );
    if( layout )
        layout->addLayout( hlay );
    else
        pInputsLayout->addLayout ( hlay );
    if ( !pFocusWidget )
        pFocusWidget = rich;
    return rich;
}

CheckBox* FrmCustom::addCheckBox(QWidget *parent, const Xtring& caption,
                                 bool value, const char *name, QBoxLayout *layout)
{
    CheckBox *chkbox = new CheckBox( parent ? parent : pFrameEdit, name, caption );
    chkbox->setChecked( value );
    if( !layout ) {
        layout = new QHBoxLayout();
        pInputsLayout->addLayout ( layout );
    }
    layout->addWidget( chkbox );
    chkbox->setEdited( false );
    connect( chkbox, SIGNAL( stateChanged( int ) ), this, SLOT( combo_activated( int ) ) );
    return chkbox;
}

PushButton* FrmCustom::addButton(QWidget* parent, const Xtring& caption,
                                 const char *name, QBoxLayout* layout)
{
    PushButton *button = new PushButton( parent ? parent : pFrameEdit, caption );
    button->setObjectName( name );
    if( !layout ) {
        layout = new QHBoxLayout();
        pInputsLayout->addLayout ( layout );
    }
    layout->addWidget( button );
    connect( button, SIGNAL( clicked() ), this, SLOT( button_clicked() ) );
    return button;
}

void FrmCustom::button_clicked()
{
    bool is_valid = true;
    validate_input( static_cast<QWidget *>(sender()), &is_valid );
}

ComboBoxXtring *FrmCustom::addComboBoxXtring(bool byref, QWidget* parent, const Xtring& caption,
        const XtringList& captions, const XtringList& values,
        const Xtring& empty, const char *_name, QBoxLayout* layout)
{
    Xtring name = _name;
    if( name.isEmpty() )
        name = "combo_" + Xtring(caption).replace(" ", "_");
    ComboBoxXtring *combo;
	if( byref )
		combo = new ComboBoxXtring( captions, values, parent ? parent : pFrameEdit, name.c_str(), caption );
	else
		combo = new ComboBoxXtring( const_cast<XtringList &>(captions), const_cast<XtringList&>(values), parent ? parent : pFrameEdit, name.c_str(), caption );
    if( !empty.isEmpty() ) {
        combo->insertItem( empty, "", 0 );
        combo->setCurrentIndex( 0 );
    }
    if ( !layout )
        layout = pInputsLayout;
// 		layout->addWidget( combo->getLabel() );
// 		layout->setAlignment( combo->getLabel(), Qt::AlignRight );
// 		layout->addWidget( combo );
// 		if( combo->maximumWidth() != 16777215 )
// 			layout->setAlignment( combo, Qt::AlignLeft );
    layout->addLayout( combo->getLayout() );
    connect( combo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( combo_activated( int ) ) );
    return combo;
}


ComboBoxXtring *FrmCustom::addComboBoxXtring( bool byref, QWidget *parent, const Xtring &caption,
        const XtringList &captions_values, const Xtring &empty,
        const char *name, QBoxLayout* layout )
{
    return addComboBoxXtring( byref, parent, caption, captions_values, captions_values, empty, name, layout );
}


ComboBoxInt *FrmCustom::addComboBoxInt( bool byref, QWidget *parent, const Xtring &caption,
	const XtringList &captions, const IntList &values, const Xtring &empty,
	const char *name, QBoxLayout * layout )
{
	ComboBoxInt *combo;
	if( byref )
		combo = new ComboBoxInt( captions, values, parent ? parent : pFrameEdit, name, caption );
	else
		combo = new ComboBoxInt( const_cast<XtringList &>(captions), const_cast<IntList &>(values), parent ? parent : pFrameEdit, name, caption );
    if( !empty.isEmpty() ) {
        combo->insertItem( empty, -1, 0 );
        combo->setCurrentIndex( 0 );
    }
    if ( layout ) {
        layout->addWidget( combo->getLabel() );
        layout->setAlignment( combo->getLabel(), Qt::AlignRight );
        layout->addWidget( combo );
        if( combo->maximumWidth() != 16777215 )
            layout->setAlignment( combo, Qt::AlignLeft );
    } else
        pInputsLayout->addLayout( combo->getLayout() );
    connect( combo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( combo_activated( int ) ) );
    return combo;
}


GroupBox* FrmCustom::addGroupBox(QWidget* parent, XtringList& options, const Xtring& caption,
                                 int selected, bool horiz, QBoxLayout* layout)
{
    GroupBox *grpbox = new GroupBox( options, caption,
                                     parent ? parent : pFrameEdit, horiz );
    grpbox->setSelectedPos( selected );
    if( !layout )
        layout = new QHBoxLayout();
    layout->addWidget( grpbox );
    pInputsLayout->addLayout ( layout );
    connect( grpbox, SIGNAL( radio_clicked( int ) ), this, SLOT( combo_activated( int ) ) );
    return grpbox;
}

FileNameBox* FrmCustom::addFileNameBox(QWidget* parent, const Xtring& caption, bool horiz,
                                       QBoxLayout* layout)
{
    FileNameBox *fnbox= new FileNameBox( parent ? parent : pFrameEdit, "", caption, false /*choosedir*/, horiz );
    if ( !layout )
        layout = pInputsLayout;
    layout->addLayout( fnbox->getLayout() );
    return fnbox;
}


FileNameBox* FrmCustom::addDirNameBox(QWidget* parent, const Xtring& caption, bool horiz,
                                      QBoxLayout* layout)
{
    FileNameBox *fnbox= new FileNameBox( parent ? parent : pFrameEdit, "", caption, true /*choosedir*/, horiz );
    if ( !layout )
        layout = pInputsLayout;
    layout->addLayout( fnbox->getLayout() );
    return fnbox;
}


DateRangeBox* FrmCustom::addDateRangeBox( QWidget *parent, const Xtring& caption,
        const DateTime date_from, const DateTime date_to,
        const Xtring &caption_from, const Xtring &caption_to,
        const char *name, QBoxLayout *layout )
{
    DateRangeBox *drbox = new DateRangeBox( parent ? parent : pFrameEdit, name, caption,
                                            Variant::tDate, date_from, date_to,
                                            caption_from, caption_to, layout );
    pInputsLayout->addLayout ( drbox->getLayout() );
    return drbox;
}

LineEdit *FrmCustom::addInputField( QWidget *parent, const Xtring &acaption,
                                    const Xtring &tablename, const Xtring &fldname,
                                    const Variant &value, QBoxLayout *layout )
{
    Xtring name = "input_" + tablename + "_" + fldname;
    if( !layout )
        layout = pInputsLayout;
    QHBoxLayout* anInputLayout = new QHBoxLayout();
    LineEdit *edit = new LineEdit ( pFrameEdit, "edit_" + name, value.type() );
    dbFieldDefinition *flddef = DBAPP->getDatabase()->findFieldDefinition( tablename, fldname );
    if( flddef ) {
        Xtring caption = acaption;
        if( caption.isEmpty() )
            caption = flddef->getCaption();
        if ( !flddef->getStyle().isEmpty() )
            FrmEditRec::applyBasicStyle ( edit, flddef->getStyle() );
        QLabel *lblField = new QLabel( toGUI( caption), pFrameEdit, ( "label_" + name ).c_str() );
        anInputLayout->addWidget ( lblField );
    }
    if ( value == "today()" )
        edit->setText ( DateTime::currentDateTime() );
    else
        edit->setText ( value );
    connect ( edit, SIGNAL ( validate ( QWidget *, bool * ) ), this, SLOT ( validate_input ( QWidget *, bool * ) ) );
    anInputLayout->addWidget ( edit );
    layout->addLayout ( anInputLayout );
    if ( !pFocusWidget )
        pFocusWidget = edit;
    return edit;
}

CheckBox* FrmCustom::addCheckField(QWidget *parent, const Xtring& caption,
                                   const Xtring& tablename, const Xtring& fldname,
                                   bool value, const char *name, QBoxLayout *layout)
{
    return addCheckBox(parent, caption, value, name, layout);
}

ComboBoxInt *FrmCustom::addComboIntField(QWidget* parent, const Xtring& _caption,
                                        const Xtring& tablename, const Xtring &fldnamecaptions, const Xtring &fldnamevalues,
                                        const Xtring& empty, const char* _name, QBoxLayout* layout)
{
    FldNamesListTable *flddef = dynamic_cast<FldNamesListTable *>(
                                    DBAPP->getDatabase()->findFieldDefinition(tablename, fldnamecaptions) );
    Xtring name = _name, caption = _caption;
    if( name.isEmpty() )
        name = "combo_" + tablename + "_" + fldnamevalues;
    if( flddef ) {
        ComboBoxInt *combo = addComboBoxInt( true, parent, caption, flddef->getListOfCaptions(),
                                            flddef->getListOfValues(), empty, name.c_str(), layout );
        FrmEditRec::applyFieldStyle( combo, flddef );
        FrmEditRec::applyFieldStyle( combo->getLabel(), flddef );
        if( !caption.isEmpty() )
            combo->getLabel()->setText( toGUI(caption) );
        return combo;
    } else {
        XtringList lc;
        IntList li;
        return addComboBoxInt( false, parent, caption, lc, li, empty, name.c_str(), layout );
    }
}

SearchBox *FrmCustom::addSearchField( QWidget *parent, const Xtring &tablename,
                                      const Xtring &_fldnamecodigo, const Xtring &_fldnamedesc,
                                      QBoxLayout *layout, SearchBox::Flags flags )
{
    Xtring fldnamecodigo = _fldnamecodigo, fldnamedesc = _fldnamedesc;
    dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablename );
    if( !tbldef ) {
        _GONG_DEBUG_WARNING( Xtring::printf( "Table '%s' not found",tablename.c_str() ) );
        return 0;
    }
    dbFieldDefinition *codefld = tbldef->findFieldDefinition( fldnamecodigo );
    if( !codefld ) {
        codefld = tbldef->findFieldByFlags( dbFieldDefinition::CODE );
        if( codefld == 0 ) {
            _GONG_DEBUG_WARNING( "Code Field not found in table " + tablename );
        } else {
            fldnamecodigo = codefld->getName();
        }
    }
    dbFieldDefinition *namefld = tbldef->findFieldDefinition( fldnamedesc );
    if( fldnamedesc.isEmpty() ) {
        namefld = tbldef->findFieldByFlags( dbFieldDefinition::DESCRIPTION );
        if( namefld == 0 ) {
            _GONG_DEBUG_WARNING( "Description Field not found in table " + tablename );
        } else {
            fldnamedesc = namefld->getName();
        }
    }
    SearchBox *search = new SearchBox( "", tablename, fldnamecodigo,
                                       fldnamedesc, parent ? parent : pFrameEdit, flags );
    if ( !layout )
        layout = pInputsLayout;
    layout->addLayout( search->getLayout() );
    FrmEditRec::applyFieldStyle( search->getButton(), namefld );
    // Change the text into the table description instead of the field caption
    if( flags & SearchBox::FlagMultiple )
        search->getButton()->setText( toGUI( tbldef->getDescPlural() ) );
    else
        search->getButton()->setText( toGUI( tbldef->getDescSingular() ) );
    if( flags & SearchBox::FlagShowLabels ) {
        FrmEditRec::applyFieldStyle( search->getLabelCode(), codefld );
        FrmEditRec::applyFieldStyle( search->getLabelDesc(), namefld );
    }
    search->setMustBeReadOnly( search->getEditCode()->isReadOnly() );
    connect( search, SIGNAL( clicked() ), search, SLOT( slotClicked() ) );
    connect( search->getEditCode(), SIGNAL( validate( QWidget *, bool * ) ),
             search, SLOT( slotValidate( QWidget *, bool * ) ) );
    if( flags & SearchBox::FlagEditableDesc ) {
        connect( search->getEditCode(), SIGNAL( validate( QWidget *, bool * ) ),
                 search, SLOT( slotValidate( QWidget *, bool * ) ) );
    }
    connect( search->getEditCode(), SIGNAL( specialControlKeyPressed ( QWidget *, char ) ),
             search, SLOT ( slotSpecialControlKeyPressed( QWidget *, char ) ) );
    return search;
}

SearchBox* FrmCustom::addMultipleSearchField(QWidget* parent, const Xtring& tablename,
        const Xtring& fldnamecodigo, const Xtring& fldnamedesc,
        QBoxLayout* layout)
{
    return addSearchField( parent, tablename, fldnamecodigo, fldnamedesc, layout,
                           static_cast<SearchBox::Flags>(SearchBox::FlagMultiple|SearchBox::FlagHorizontal) );
}

SearchBox* FrmCustom::addLabeledSearchField(QWidget* parent, const Xtring& tablename,
        const Xtring& fldnamecodigo, const Xtring& fldnamedesc,
        QBoxLayout* layout)
{
    return addSearchField( parent, tablename, fldnamecodigo, fldnamedesc, layout,
                           static_cast<SearchBox::Flags>(SearchBox::FlagShowLabels|SearchBox::FlagHorizontal|SearchBox::FlagEditableDesc) );
}

void FrmCustom::validate_input( QWidget *sender, bool *is_valid )
{
}

void FrmCustom::combo_activated( int )
{
    if( ComboBoxInt *cbi = dynamic_cast<ComboBoxInt *>( sender() ) ) {
        if( !cbi->isSettingProgrammatically() ) {
            cbi->setEdited( true );
            cbi->setJustEdited( true );
            validate_input( static_cast<QWidget *>(cbi), 0 );
        }
    } else if( ComboBoxXtring *cbs = dynamic_cast<ComboBoxXtring *>( sender() ) ) {
        if( !cbs->isSettingProgrammatically() ) {
            cbs->setEdited( true );
            cbs->setJustEdited( true );
            validate_input( static_cast<QWidget *>(cbs), 0 );
        }
    } else if( CheckBox *chk = dynamic_cast<CheckBox *>( sender() ) ) {
        if( !chk->isCheckingProgrammatically() ) {
            chk->setEdited( true );
            chk->setJustEdited( true );
            validate_input( static_cast<QWidget *>(chk), 0 );
        }
    }
}


#ifdef HAVE_RTKMODULE
Xtring FrmCustom::readRTK( const Xtring &_template )
{
    Xtring paths = DBAPP->getReportsPath( true );
    Xtring filename = FileUtils::findInPath( paths, _template );
    Xtring rtkstring = FileUtils::readFile( filename );
    if( rtkstring.isEmpty() ) {
        FrmBase::msgError(this, Xtring::printf(_("Imposible encontrar el fichero %s en\n%s"),
                                               _template.c_str(), paths.c_str() ));
        return Xtring();
    }
    return rtkstring;
}
#endif

}

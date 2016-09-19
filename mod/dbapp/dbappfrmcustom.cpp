#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTabWidget>
#include <gonggettext.h>
#include <gongfileutils.h>
#include <gongvariant.h>
#include <gongpushbutton.h>
#include <gongguicontrols.h>
#include "dbappsearchbox.h"
#include "dbappdbapplication.h"
#include "dbappfrmeditrec.h"
#include "dbappfrmcustom.h"
#include "dbappnameslisttable.h"
#include "dbappdaterangebox.h"

namespace gong {


FrmCustom::FrmCustom ( QWidget *parent, const char *name, WidgetFlags f )
    : FrmBase ( parent, name, f )
{
    pFormLayout = new QVBoxLayout(this);
    pFormLayout->setObjectName( "FormLayout" );
    pMainLayout = new QVBoxLayout();
    pMainLayout->setObjectName( "MainLayout" );

    pTabWidget = new QTabWidget(this);
    pControlsFrame = new QWidget(pTabWidget);
    pControlsFrame->setObjectName("ControlsFrame");
    pControlsLayout = new QVBoxLayout(pControlsFrame);
    pControlsLayout->setObjectName("ControlsLayout");
    pTabWidget->setFocusPolicy( Qt::ClickFocus );
    pTabWidget->addTab( pControlsFrame, "&General" );
    showTabs( false );
    pMainLayout->addWidget(pTabWidget);

//  	QSpacerItem* spacer = new QSpacerItem ( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
//  	pControlsFrameLayout->addItem ( spacer );

    pButtonsLayout = new QHBoxLayout();
    pButtonsLayout->setObjectName( "ButtonsLayout" );
    pButtonsLayout->addWidget ( pushAccept );
    pButtonsLayout->addWidget ( pushCancel );

    pMainLayout->addLayout ( pButtonsLayout );
    pFormLayout->addLayout ( pMainLayout );
}

void FrmCustom::showTabs(bool show)
{
    QTabBar *tb = pTabWidget->findChild<QTabBar *>();
    if( tb )
        tb->setVisible( show );
}

QWidget *FrmCustom::insertTab(QWidget* tab, const Xtring& label, const char *name, int index)
{
    if( !tab )
        tab = new QWidget( this, name );
    pTabWidget->insertTab( index, tab, toGUI(label) );
    return tab;
}

/**
 * @brief Sets the label text of a tab widget. There is no easy way of doing this from the widget itself
 *
 * @param tab ...
 * @param title ...
 * @return void
 **/
void FrmCustom::setTabTitle(QWidget* tab, const Xtring& title)
{
    for( int index = 0; index < pTabWidget->count(); ++index ) {
        if( pTabWidget->widget( index ) == tab ) {
            pTabWidget->setTabText( index, toGUI( title ) );
            break;
        }
    }
}

LineEdit *FrmCustom::addInput ( QWidget *parent, const Xtring &caption,
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

QLabel *FrmCustom::addLabel( QWidget *parent, const Xtring &caption,
                             const Variant &value, const Xtring &style,
                             const char *name, QBoxLayout *layout )
{
    QLabel *label = new QLabel( toGUI( value.toString() ), parent ? parent : pControlsFrame );
    QLabel *cap_label = new QLabel ( toGUI( caption ), parent ? parent : pControlsFrame );
    if( name )
        label->setObjectName( name );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( cap_label );
    hlay->addWidget ( label );
    if( layout )
        layout->addLayout( hlay );
    else
        pControlsLayout->addLayout( hlay);
    return label;
}

TextEdit* FrmCustom::addTextEditBox(QWidget* parent, const Xtring& caption,
                                    const Xtring& value, const char* name, QBoxLayout* layout)
{
    TextBox *textbox = new TextBox( parent ? parent : pControlsFrame,
                                    name ? (Xtring("editTextBox_") + name).c_str() : "", "", false);
    connect ( textbox, SIGNAL ( validate ( QWidget *, bool * ) ),
              this, SLOT ( validate_input ( QWidget *, bool * ) ) );
    textbox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    textbox->setText( value );
    QLabel *label = new QLabel ( parent ? parent : pControlsFrame,
                                 ( "label_" + Xtring(name) ).c_str() );
    label->setText( toGUI(caption) );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( label );
    hlay->addLayout( textbox->getLayout() );
    if( layout )
        layout->addLayout( hlay );
    else
        pControlsLayout->addLayout ( hlay );
    if ( !pFocusWidget )
        pFocusWidget = textbox;
    return textbox;
}


/**
 * @brief ...
 *
 * @param parent ...
 * @param caption ...
 * @param name ...
 * @param layout ...
 * @param horiz
 * @return RichTextBox*
 */
RichTextBox* FrmCustom::addRichTextBox(QWidget* parent, const Xtring& caption,
                                       const char* name, QBoxLayout *layout, bool horiz)
{
    RichTextBox *rich = new RichTextBox( parent ? parent : pControlsFrame,
                                         name ? (Xtring("editRich_") + name).c_str() : "", "", horiz);
    rich->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    connect( rich, SIGNAL ( validate ( QWidget *, bool * ) ),
             this, SLOT ( validate_input ( QWidget *, bool * ) ) );
    QLabel *label = new QLabel ( parent ? parent : pControlsFrame,
                                 ( "label_" + Xtring(name) ).c_str() );
    label->setText ( toGUI(caption) );
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget ( label );
    hlay->addLayout( rich->getLayout() );
    if( layout )
        layout->addLayout( hlay );
    else
        pControlsLayout->addLayout ( hlay );
    if ( !pFocusWidget )
        pFocusWidget = rich;
    return rich;
}

CheckBox* FrmCustom::addCheckBox(QWidget *parent, const Xtring& caption,
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

PushButton* FrmCustom::addButton(QWidget* parent, const Xtring& caption,
                                 const char *name, QBoxLayout* layout)
{
    PushButton *button = new PushButton( parent ? parent : pControlsFrame, caption );
    button->setObjectName( name );
    if( !layout ) {
        layout = new QHBoxLayout();
        pControlsLayout->addLayout ( layout );
    }
    layout->addWidget( button );
    connect( button, SIGNAL( clicked() ), this, SLOT( button_clicked() ) );
    return button;
}


QLabel* FrmCustom::addImage(QWidget* parent, QPixmap& pixmap, const char *name, QBoxLayout* layout)
{
	QLabel *label = new QLabel( parent, name );
    label->setEnabled( TRUE );
    label->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, label->sizePolicy().hasHeightForWidth() ) );
    label->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    label->setFrameShape( QFrame::StyledPanel );
    label->setFrameShadow( QLabel::Sunken );
    label->setLineWidth( 3 );
    label->setPixmap( pixmap );
    label->setScaledContents( FALSE );
    label->setAlignment( int( Qt::AlignTop | Qt::AlignHCenter ) );
    if( !layout ) {
        layout = new QHBoxLayout();
        pControlsLayout->addLayout(layout);
    }
    layout->addWidget(label);
}

ComboBoxXtring *FrmCustom::addComboBoxXtring( bool byref, QWidget *parent, const Xtring &caption,
        const XtringList &captions_values, const Xtring &empty,
        const char *name, QBoxLayout* layout, bool horiz )
{
    return addComboBoxXtring( byref, parent, caption, captions_values, captions_values,
                              empty, name, layout, horiz );
}

ComboBoxXtring* FrmCustom::addComboBoxXtring(bool byref, QWidget* parent, const Xtring& caption, const XtringList& captions, const XtringList& values, const Xtring& empty, const char* name, QBoxLayout* layout, bool horiz)
{
    ComboBoxXtring *combo;
    if( byref )
        combo = new ComboBoxXtring( captions, values, parent ? parent : pControlsFrame, name, caption, horiz );
    else
        combo = new ComboBoxXtring( const_cast<XtringList &>(captions), const_cast<XtringList &>(values),
                                 parent ? parent : pControlsFrame, name, caption, horiz );
    if( !empty.isEmpty() ) {
        combo->insertItem( empty, Xtring::null, 0 );
        combo->setCurrentIndex( 0 );
    }
    if ( !layout )
        layout = pControlsLayout;
    layout->addLayout( combo->getLayout() );
    connect( combo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( combo_activated( int ) ) );
    return combo;
}


ComboBoxInt *FrmCustom::addComboBoxInt( bool byref, QWidget *parent, const Xtring &caption,
                                        const XtringList &captions, const IntList &values, const Xtring &empty,
                                        const char *name, QBoxLayout * layout, bool horiz )
{
    ComboBoxInt *combo;
    if( byref )
        combo = new ComboBoxInt( captions, values, parent ? parent : pControlsFrame, name, caption, horiz );
    else
        combo = new ComboBoxInt( const_cast<XtringList &>(captions), const_cast<IntList &>(values),
                                 parent ? parent : pControlsFrame, name, caption, horiz );
    if( !empty.isEmpty() ) {
        combo->insertItem( empty, -1, 0 );
        combo->setCurrentIndex( 0 );
    }
    if ( !layout )
        layout = pControlsLayout;
    layout->addLayout( combo->getLayout() );
    connect( combo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( combo_activated( int ) ) );
    return combo;
}


GroupBox* FrmCustom::addGroupBox(QWidget* parent, XtringList& options, const Xtring& caption,
                                 int selected, QBoxLayout* layout, bool horiz)
{
    GroupBox *grpbox = new GroupBox( options, caption,
                                     parent ? parent : pControlsFrame, horiz );
    grpbox->setSelectedPos( selected );
    if( !layout )
        layout = new QHBoxLayout();
    layout->addWidget( grpbox );
    pControlsLayout->addLayout ( layout );
    connect( grpbox, SIGNAL( radio_clicked( int ) ), this, SLOT( combo_activated( int ) ) );
    return grpbox;
}

FileNameBox* FrmCustom::addFileNameBox(QWidget* parent, const Xtring& caption, const char *name,
                                       QBoxLayout* layout, bool horiz)
{
    FileNameBox *fnbox= new FileNameBox( parent ? parent : pControlsFrame, name, caption, false /*choosedir*/, horiz );
    if ( !layout )
        layout = pControlsLayout;
    layout->addLayout( fnbox->getLayout() );
    return fnbox;
}


FileNameBox* FrmCustom::addDirNameBox(QWidget* parent, const Xtring& caption, const char *name,
                                      QBoxLayout* layout, bool horiz)
{
    FileNameBox *fnbox= new FileNameBox( parent ? parent : pControlsFrame, name, caption, true /*choosedir*/, horiz );
    if ( !layout )
        layout = pControlsLayout;
    layout->addLayout( fnbox->getLayout() );
    return fnbox;
}


DateRangeBox* FrmCustom::addDateRangeBox( QWidget *parent, const Xtring& caption,
        const DateTime date_from, const DateTime date_to,
        const Xtring &caption_from, const Xtring &caption_to,
        const char *name, QBoxLayout *layout )
{
    DateRangeBox *drbox = new DateRangeBox( parent ? parent : pControlsFrame, name, caption,
                                            Variant::tDate, date_from, date_to,
                                            caption_from, caption_to, layout );
    pControlsLayout->addLayout ( drbox->getLayout() );
    return drbox;
}


/// FIELDS


LineEdit *FrmCustom::addInputField( QWidget *parent, const Xtring &acaption,
                                    const Xtring &tablename, const Xtring &fldname,
                                    const Variant &value, QBoxLayout *layout )
{
    Xtring name = "input_" + tablename + "_" + fldname;
    if( !layout )
        layout = pControlsLayout;
    QHBoxLayout* anInputLayout = new QHBoxLayout();
    LineEdit *edit = new LineEdit ( pControlsFrame, "edit_" + name, value.type() );
    dbFieldDefinition *flddef = DBAPP->getDatabase()->findFieldDefinition( tablename, fldname );
    if( flddef ) {
        Xtring caption = acaption;
        if( caption.isEmpty() )
            caption = flddef->getCaption();
        if ( !flddef->getStyle().isEmpty() )
            FrmEditRec::applyBasicStyle ( edit, flddef->getStyle() );
        QLabel *lblField = new QLabel( toGUI( caption), pControlsFrame, ( "label_" + name ).c_str() );
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
                                       fldnamedesc, parent ? parent : pControlsFrame, flags );
    if ( !layout )
        layout = pControlsLayout;
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

void FrmCustom::accept()
{
    if( validate() )
        FrmBase::accept();
}

void FrmCustom::button_clicked()
{
    bool is_valid = true;
    validate_input( static_cast<QWidget *>(sender()), &is_valid );
}

void FrmCustom::validate_input( QWidget *sender, bool *is_valid )
{
    if (is_valid)
        *is_valid = true;
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

}

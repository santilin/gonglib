#include <QScrollArea>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QRadioButton>
#include "gonggettext.h"
#include "gongguiapplication.h"
#include "gongguimainwindow.h"
#include "gongguicontrols.h"
#include "gongdbfieldstyle.h"
#include "gongrichtextedit.h"

namespace gong {

/************************************************************
 * EDITION CONTROLS
 ************************************************************/

EditBox::EditBox( QWidget *parent, const Xtring &name, const Xtring &caption, bool horizontal )
    : LineEdit(parent, name), mHorizontal(horizontal)
{
    if( mHorizontal )
        pLayout = (QBoxLayout *)new QHBoxLayout();
    else
        pLayout = (QBoxLayout *)new QVBoxLayout();
    pLabel = new QLabel( parent, ("label_" + name).c_str() );
    pLabel->setText( QString::fromUtf8(caption.c_str()) );
    pLayout->addWidget( pLabel );
    pLayout->addWidget( this );
}

void EditBox::setVisible(bool visible)
{
    pLabel->setVisible( visible );
    LineEdit::setVisible( visible );
}

TextBox::TextBox(QWidget *parent, const Xtring &name,
                 const Xtring &caption, bool horizontal)
    : TextEdit(parent), mHorizontal(horizontal)
{
    setObjectName( name.c_str() );
    if( mHorizontal )
        pLayout = (QBoxLayout *)new QHBoxLayout();
    else
        pLayout = (QBoxLayout *)new QVBoxLayout();
    this->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
    pLabel = new QLabel( parent, ("label_" + name).c_str() );
    pLabel->setText( QString::fromUtf8(caption.c_str()) );
    pLayout->addWidget( pLabel );
    pLayout->addWidget( this );
}

void TextBox::setHeightInLines(int lines)
{
    if( lines > 1 )
        setMinimumHeight( fontMetrics().height() * (lines - 1));
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::MinimumExpanding );
    setMaximumHeight( fontMetrics().height() * (lines + 1));
}

RichTextBox::RichTextBox(QWidget *parent, const Xtring &name,
                         const Xtring &caption, bool horizontal )
    : QLabel(parent, ("richlabel_" + name).c_str()),
      mHorizontal(horizontal), mMustBeReadOnly( false ), mIsReadOnly( false ),
      pEditRich( 0 ), pEditRichHTML( 0 )
{
    setWordWrap( true );
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if( mHorizontal )
        pLayout = (QBoxLayout *)new QHBoxLayout();
    else
        pLayout = (QBoxLayout *)new QVBoxLayout();
    pLabel = new QLabel( parent, ("label_" + name).c_str() );
    pLabel->setText( toGUI(caption) );
    QBoxLayout *labelandbutton = new QHBoxLayout();
    labelandbutton->addWidget( pLabel );
    pEditRich = new QPushButton( this, ("editbutton_" + name).c_str() );
    pEditRich->setText( toGUI( _("Editar") ) );
    connect( pEditRich, SIGNAL( clicked() ), this, SLOT( slotBeginEdit() ) );
    pEditRichHTML = new QPushButton( this, ("editbutton_HTML_" + name).c_str() );
    pEditRichHTML->setText( toGUI( _("Editar HTML") ) );
    connect( pEditRichHTML, SIGNAL( clicked() ), this, SLOT( slotBeginEditHTML() ) );
    QSpacerItem *spacer16 = new QSpacerItem( 20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum );
    labelandbutton->addItem( spacer16 );
    labelandbutton->addWidget( pEditRich );
    labelandbutton->addWidget( pEditRichHTML );
    QScrollArea *scrollarea = new QScrollArea;
    scrollarea->setWidget(this);
//	scrollarea->ensureWidgetVisible( this );
    scrollarea->setWidgetResizable(true);
    pLayout->addLayout( labelandbutton );
    pLayout->addWidget( scrollarea );
}

void RichTextBox::setReadOnly(bool readonly)
{
    mIsReadOnly = readonly;
    if( readonly )
        pEditRich->setEnabled(false);
    else
        pEditRich->setEnabled(true);
}

void RichTextBox::slotBeginEdit()
{
    Xtring text = getText();
    if( FrmBase::inputBoxText(text, _("Texto"), fromGUI(getLabel()->text()), this) )
        setText( text );
}

void RichTextBox::slotBeginEditHTML()
{
    RichTextEdit *te = new RichTextEdit( this, static_cast<QWidget *>( parent() ), true /* modal */);
    te->setCustomStyles( mCustomStyles );
    theGuiApp->getMainWindow()->createClient( te );
    adjustSize();
}


Xtring RichTextBox::getText() const
{
    return fromGUI(text());
}

void RichTextBox::setText( const Xtring &text)
{
    QLabel::setText( toGUI( text ) );
    adjustSize();
}

QSize RichTextBox::sizeHint() const
{
    QSize s;
    s.setWidth( fontMetrics().width('x') * 80 );
    s.setHeight(fontMetrics().height() * 7 );
    return s;
}

FileNameBox::FileNameBox( QWidget *parent, const Xtring &name, const Xtring &caption,
                          bool choosedir, bool horizontal )
    : QPushButton( parent, name.c_str() ), mHorizontal(horizontal),
      mMustBeReadOnly(false), mChooseDir( choosedir )
{
    pLayout = (QBoxLayout *)new QHBoxLayout();
    pLayout->addWidget( this );
    setText( toGUI( caption ) );
    pEditFileName = new LineEdit( parent, (name + "_FName").c_str() );
    pLayout->addWidget( pEditFileName );
    connect( this, SIGNAL( clicked() ), this, SLOT( slot_button_clicked() ) );
}

Xtring FileNameBox::getFileName() const
{
    _GONG_DEBUG_ASSERT( pEditFileName );
    return pEditFileName->toString();
}

void FileNameBox::setFileName(const Xtring& filename)
{
    _GONG_DEBUG_ASSERT( pEditFileName );
    pEditFileName->setText( filename );
}

void FileNameBox::slot_button_clicked()
{
    if( !pEditFileName->isReadOnly() && pEditFileName->isEnabled() ) {
        Xtring fname;
        if( !mChooseDir ) {
            fname = theGuiApp->getOpenFileName( _("Elige el documento"), "",
                                                _("Todos los archivos (*)" ), this);
        } else {
            fname = theGuiApp->getExistingDirName( _("Elige el directorio"),
                                                   pEditFileName->toString(), this );
        }
        if( !fname.isEmpty() ) {
            pEditFileName->setText( fname );
        }
    }
}

void FileNameBox::setEnabled(bool enabled)
{
    pEditFileName->setEnabled( enabled );
    QPushButton::setEnabled( enabled );
}


GroupBox::GroupBox(XtringList& options, const Xtring& caption, QWidget* parent, bool horizontal)
    : QGroupBox( toGUI( caption ) ), mMustBeReadOnly( false ), mIsReadOnly( false ), mIsEdited( false )
{
    QHBoxLayout *hlay;
    QVBoxLayout *vlay;
    if( horizontal )
        hlay = new QHBoxLayout();
    else
        vlay = new QVBoxLayout();
    for( XtringList::const_iterator it = options.begin();
            it != options.end(); ++it ) {
        QRadioButton *rb = new QRadioButton( toGUI( *it ) );
        connect( rb, SIGNAL( clicked(bool) ), this, SLOT( radio_clicked(bool) ) );
        if( horizontal )
            hlay->addWidget( rb );
        else
            vlay->addWidget( rb );
    }
    if( horizontal )
        setLayout( hlay );
    else
        setLayout( vlay );
}

void GroupBox::radio_clicked(bool checked)
{
    emit radio_clicked( getSelectedPos() );
}


int GroupBox::getSelectedPos() const
{
    QListIterator<QObject *> i = children();
    int pos = -1;
    while (i.hasNext()) {
        QObject *obj = i.next();
        QRadioButton *rb = dynamic_cast<QRadioButton *>(obj);
        if( rb ) {
            pos++;
            if( rb->isOn() )
                break;
        }
    }
    return pos;
}

Xtring GroupBox::getSelectedString() const
{
    Xtring ret;
    QListIterator<QObject *> i = children();
    while (i.hasNext()) {
        QObject *obj = i.next();
        QRadioButton *rb = dynamic_cast<QRadioButton *>(obj);
        if( rb ) {
            if( rb->isOn() ) {
                ret = fromGUI( rb->text() );
                break;
            }
        }
    }
    return ret;
}

void GroupBox::setSelectedPos(int sel)
{
    QListIterator<QObject *> i = children();
    int pos = -1;
    while (i.hasNext()) {
        QObject *obj = i.next();
        QRadioButton *rb = dynamic_cast<QRadioButton *>(obj);
        if( rb ) {
            pos++;
            if( pos == sel ) {
                rb->setChecked( true );
                break;
            }
        }
    }
}

void GroupBox::setSelectedString(const Xtring& value)
{
    QListIterator<QObject *> i = children();
    while (i.hasNext()) {
        QObject *obj = i.next();
        QRadioButton *rb = dynamic_cast<QRadioButton *>(obj);
        if( rb ) {
            if( fromGUI( rb->text() ) == value ) {
                rb->setChecked( true );
                break;
            }
        }
    }
}

}; // Namespace gong

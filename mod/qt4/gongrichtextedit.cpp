#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QMdiSubWindow>

#include <algorithm>
#include <gonggettext.h>
#include "gongfrmbase.h"
#include "gongguimainwindow.h"
#include "gongguiapplication.h"
#include "gongrichtextedit.h"

namespace gong {

RichTextEdit::RichTextEdit(Xtring &text, QWidget *parent)
    : QMainWindow(parent), sourceTextLabel(0), mPlainText( toGUI(text) ),
      mIsModal( false ), mWasCancelled( true )
{
    setup();
    setText( mPlainText );
    textEdit->setFocus();
}

RichTextEdit::RichTextEdit(QLabel *txtlabel, QWidget *parent, bool modal)
    : QMainWindow(parent), sourceTextLabel( txtlabel ),
      mIsModal( modal ), mWasCancelled( true )
{
    setup();
    setText( txtlabel->text() );
    if( modal ) {
        QWidget *p = txtlabel->parentWidget();
        while( p && !dynamic_cast<FrmBase *>(p) )
            p = p->parentWidget();
        if( p )
            p->setEnabled(false);
    }
    textEdit->setFocus();
}

void RichTextEdit::setup()
{
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setupFileActions();
    setupEditActions();
    setupTextActions();

    QFrame *mainFrame = new QFrame( this );
    QVBoxLayout *mainLayout = new QVBoxLayout( mainFrame );

    textEdit = new QTextEdit(mainFrame);
    connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(textHasChanged()));

    mainLayout->addWidget( textEdit );

    pushAccept = new QPushButton ( toGUI ( _ ( "&Aceptar" ) ), this, "pushAccept" );
    pushAccept->setIcon( pushAccept->style()->standardIcon( QStyle::SP_DialogYesButton ) );
    pushAccept->setDefault ( true );
    pushCancel = new QPushButton ( toGUI ( _ ( "&Cancelar" ) ), this, "pushCancel" );
    pushCancel->setIcon( pushAccept->style()->standardIcon( QStyle::SP_DialogCancelButton ) );
    connect ( pushAccept, SIGNAL ( clicked() ), this, SLOT ( accept_clicked() ) );
    connect ( pushCancel, SIGNAL ( clicked() ), this, SLOT ( cancel_clicked() ) );
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget( pushAccept );
    buttonsLayout->addWidget( pushCancel );

    mainLayout->addLayout( buttonsLayout );

    setCentralWidget(mainFrame);
    textEdit->setFocus();
    setCurrentFileName(QString());

    fontChanged(textEdit->font());
    colorChanged(textEdit->textColor());
    alignmentChanged(textEdit->alignment());

    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            actionSave, SLOT(setEnabled(bool)));
    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(setWindowModified(bool)));
    connect(textEdit->document(), SIGNAL(undoAvailable(bool)),
            actionUndo, SLOT(setEnabled(bool)));
    connect(textEdit->document(), SIGNAL(redoAvailable(bool)),
            actionRedo, SLOT(setEnabled(bool)));

    setWindowModified(textEdit->document()->isModified());
    actionSave->setEnabled(textEdit->document()->isModified());
    actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
    actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

    connect(actionUndo, SIGNAL(triggered()), textEdit, SLOT(undo()));
    connect(actionRedo, SIGNAL(triggered()), textEdit, SLOT(redo()));

    actionCut->setEnabled(false);
    actionCopy->setEnabled(false);

    connect(actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));
    connect(actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));
    connect(actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));

    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));

#ifndef QT_NO_CLIPBOARD
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
#endif

    fileNew();
}

void RichTextEdit::accept_clicked()
{
    _GONG_DEBUG_TRACE(0);
    bool isvalid = true;
    emit validate( this, &isvalid );
    if( isvalid ) {
        mWasCancelled = false;
        close();
    } else {
        mWasCancelled = true; // For if later on the form is closed from the window close button
    }
}

void RichTextEdit::cancel_clicked()
{
    mWasCancelled = true;
    close();
}

void RichTextEdit::setText(const QString &text)
{
    textEdit->setText( text );
    mJustEdited = false;
}

QString RichTextEdit::getText() const
{
    return mPlainText;
}

void RichTextEdit::closeEvent(QCloseEvent *e)
{
    if (maybeSave()) {
        mPlainText = textEdit->document()->toHtml();
        if( sourceTextLabel && !mWasCancelled ) {
            sourceTextLabel->setText( mPlainText );
            if( RichTextBox *rte = dynamic_cast<RichTextBox *>(sourceTextLabel) )
                rte->setEdited( true );
        }
        if( mIsModal && sourceTextLabel ) {
            QWidget *p = sourceTextLabel->parentWidget();
            while( p && !dynamic_cast<FrmBase *>(p) )
                p = p->parentWidget();
            if( p )
                p->setEnabled(true);
        }
        if( theGuiApp->getMainWindow() ) {
            QMdiSubWindow *pw = theGuiApp->getMainWindow()->findClient( this );
            if( pw ) {
                pw->deleteLater();
                pw->close();
            }
        }
        e->accept();
    } else
        e->ignore();
}

void RichTextEdit::setupFileActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(toGUI( _("Acciones sobre ficheros")));
    addToolBar(tb);

    QMenu *menu = new QMenu(toGUI( _("&Fichero")), this);
    menuBar()->addMenu(menu);

    QAction *a;

    QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/filenew.png"));
    a = new QAction( newIcon, toGUI( _("&Nuevo")), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::New);
    connect(a, SIGNAL(triggered()), this, SLOT(fileNew()));
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(QIcon::fromTheme("document-open", QIcon(":/fileopen.png")),
                    toGUI( _("&Abrir...")), this);
    a->setShortcut(QKeySequence::Open);
    connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();

    actionSave = a = new QAction(QIcon::fromTheme("document-save", QIcon(":/filesave.png")),
                                 toGUI( _("&Guardar")), this);
    a->setShortcut(QKeySequence::Save);
    connect(a, SIGNAL(triggered()), this, SLOT(fileSave()));
    a->setEnabled(false);
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(toGUI( _("Guardar &como...")), this);
    a->setPriority(QAction::LowPriority);
    connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    menu->addAction(a);
    menu->addSeparator();

#ifndef QT_NO_PRINTER
    a = new QAction(QIcon::fromTheme("document-print", QIcon( ":/fileprint.png")),
                    toGUI( _("Im&primir...")), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Print);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrint()));
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(QIcon::fromTheme("fileprint", QIcon(":/fileprint.png")),
                    toGUI( _("Previsualizar...")), this);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
    menu->addAction(a);

    a = new QAction(QIcon::fromTheme("exportpdf",  QIcon(":/exportpdf.png")),
                    toGUI( _("&Exportar PDF...")), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(Qt::CTRL + Qt::Key_D);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrintPdf()));
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();
#endif

    a = new QAction(toGUI( _("&Salir")), this);
    a->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(a, SIGNAL(triggered()), this, SLOT(close()));
    menu->addAction(a);

}

void RichTextEdit::setupEditActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle( toGUI( _("Acciones de edición")));
    addToolBar(tb);
    QMenu *menu = new QMenu(toGUI( _("&Edición")), this);
    menuBar()->addMenu(menu);

    QAction *a;
    a = actionUndo = new QAction(QIcon::fromTheme("edit-undo", QIcon(":/editundo.png")),
                                 toGUI( _("&Deshacer")), this);
    a->setShortcut(QKeySequence::Undo);
    tb->addAction(a);
    menu->addAction(a);
    a = actionRedo = new QAction(QIcon::fromTheme("edit-redo", QIcon( ":/editredo.png")),
                                 toGUI( _("&Rehacer")), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Redo);
    tb->addAction(a);
    menu->addAction(a);
    menu->addSeparator();
    a = actionCut = new QAction(QIcon::fromTheme("edit-cut", QIcon( ":/editcut.png")),
                                toGUI( _("Cor&tar")), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Cut);
    tb->addAction(a);
    menu->addAction(a);
    a = actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/editcopy.png")),
                                 toGUI( _("&Copiar")), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Copy);
    tb->addAction(a);
    menu->addAction(a);
    a = actionPaste = new QAction(QIcon::fromTheme("edit-paste", QIcon(":/editpaste.png")),
                                  toGUI( _("&Pegar")), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Paste);
    tb->addAction(a);
    menu->addAction(a);
#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());
#endif
}

void RichTextEdit::setupTextActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(toGUI( _("Acciones de formato")));
    addToolBar(tb);

    QMenu *menu = new QMenu(toGUI( _("F&ormato")), this);
    menuBar()->addMenu(menu);

    actionTextBold = new QAction(QIcon::fromTheme("format-text-bold", QIcon(":/textbold.png")),
                                 toGUI( _("&Negrilla")), this);
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
    tb->addAction(actionTextBold);
    menu->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    actionTextItalic = new QAction(QIcon::fromTheme("format-text-italic", QIcon(":/textitalic.png")),
                                   toGUI( _("&Cursiva")), this);
    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
    tb->addAction(actionTextItalic);
    menu->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    actionTextUnderline = new QAction(QIcon::fromTheme("format-text-underline", QIcon(":/textunder.png")),
                                      toGUI( _("&Subrayado")), this);
    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    connect(actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
    tb->addAction(actionTextUnderline);
    menu->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    menu->addSeparator();

    QActionGroup *grp = new QActionGroup(this);
    connect(grp, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));

    // Make sure the alignLeft  is always left of the alignRight
    if (QApplication::isLeftToRight()) {
        actionAlignLeft = new QAction(QIcon::fromTheme("format-justify-left", QIcon(":/textleft.png")),
                                      toGUI( _("&Izquierda")), grp);
        actionAlignCenter = new QAction(QIcon::fromTheme("format-justify-center", QIcon( ":/textcenter.png")), toGUI( _("C&entrado")), grp);
        actionAlignRight =  new QAction(QIcon::fromTheme("format-justify-right",  QIcon( ":/textright.png")), toGUI( _("De&recha")), grp);
    } else {
        actionAlignRight = new QAction(QIcon::fromTheme("format-justify-right", QIcon(":/textright.png")), toGUI( _("De&recha")), grp);
        actionAlignCenter = new QAction(QIcon::fromTheme("format-justify-center", QIcon(":/textcenter.png")), toGUI( _("C&entrado")), grp);
        actionAlignLeft = new QAction(QIcon::fromTheme("format-justify-left", QIcon(":/textleft.png")), toGUI( _("&Izquierda")), grp);
    }
    actionAlignJustify = new QAction(QIcon::fromTheme("format-justify-fill", QIcon(":/textjustify.png")), toGUI( _("&Justificado")), grp);

    actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);
    actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);
    actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);
    actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);

    tb->addActions(grp->actions());
    menu->addActions(grp->actions());

    menu->addSeparator();

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = new QAction(pix, toGUI( _("&Color...")), this);
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
    tb->addAction(actionTextColor);
    menu->addAction(actionTextColor);

    tb = new QToolBar(this);
    tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    tb->setWindowTitle(toGUI( _("Acciones de formato")));
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(tb);

    comboStyle = new QComboBox(tb);
    tb->addWidget(comboStyle);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Bullet List (Disc)");
    comboStyle->addItem("Bullet List (Circle)");
    comboStyle->addItem("Bullet List (Square)");
    comboStyle->addItem("Lista ordenada (Decimal)");
    comboStyle->addItem("Lista ordenada (Alpha lower)");
    comboStyle->addItem("Lista ordenada (Alpha upper)");
    comboStyle->addItem("Lista ordenada (Roman lower)");
    comboStyle->addItem("Lista ordenada (Roman upper)");
    connect(comboStyle, SIGNAL(activated(int)),
            this, SLOT(textStyle(int)));

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, SIGNAL(activated(QString)),
            this, SLOT(textFamily(QString)));

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);

    comboCustomStyle = new QComboBox(tb);
    comboCustomStyle->setObjectName("comboCustomStyle");
    tb->addWidget(comboCustomStyle);
    comboCustomStyle->addItem( toGUI(_("Borrar formato")));
    comboCustomStyle->setEditable(false);
    connect(comboCustomStyle, SIGNAL(activated(int)),
            this, SLOT(textCustomStyle(int)));

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)),
            this, SLOT(textSize(QString)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font()
                               .pointSize())));
}

bool RichTextEdit::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str)) {
        textEdit->setHtml(str);
    } else {
        str = QString::fromLocal8Bit(data);
        textEdit->setPlainText(str);
    }
    setCurrentFileName(f);
    return true;
}

bool RichTextEdit::maybeSave()
{
    return true;
    if (!textEdit->document()->isModified())
        return true;
    if (fileName.startsWith(QLatin1String(":/")))
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, toGUI( _("Application")),
                               toGUI( _("The document has been modified.\n"
                                        "Do you want to save your changes?")),
                               QMessageBox::Save | QMessageBox::Discard
                               | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void RichTextEdit::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    textEdit->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(toGUI( _("%1[*] - %2")).arg(shownName).arg( _("Rich Text")));
    setWindowModified(false);
}

void RichTextEdit::fileNew()
{
    if (maybeSave()) {
        textEdit->clear();
        textEdit->setFocus();
        setCurrentFileName(QString());
    }
}

void RichTextEdit::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, toGUI( _("Open File...")),
                 QString(),
                 toGUI( _("HTML-Files (*.htm *.html);;All Files (*)")));
    if (!fn.isEmpty())
        load(fn);
}

bool RichTextEdit::fileSave()
{
    if (fileName.isEmpty())
        return fileSaveAs();

    QTextDocumentWriter writer(fileName);
    bool success = writer.write(textEdit->document());
    if (success)
        textEdit->document()->setModified(false);
    return success;
}

bool RichTextEdit::fileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, toGUI( _("Save as...")),
                 QString(),
                 toGUI( _("ODF files (*.odt);;HTML-Files (*.htm *.html);;All Files (*)")));
    if (fn.isEmpty())
        return false;
    if (! (fn.endsWith(".odt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive)) )
        fn += ".odt"; // default
    setCurrentFileName(fn);
    return fileSave();
}

void RichTextEdit::filePrint()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (textEdit->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(toGUI( _("Print Document")));
    if (dlg->exec() == QDialog::Accepted) {
        textEdit->print(&printer);
    }
    delete dlg;
#endif
}

void RichTextEdit::filePrintPreview()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    preview.exec();
#endif
}

void RichTextEdit::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    textEdit->print(printer);
#endif
}

void RichTextEdit::filePrintPdf()
{
#ifndef QT_NO_PRINTER
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                       QString(), "*.pdf");
    if (!fileName.isEmpty()) {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        textEdit->document()->print(&printer);
    }
#endif
}

void RichTextEdit::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void RichTextEdit::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void RichTextEdit::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void RichTextEdit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void RichTextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void RichTextEdit::textStyle(int styleIndex)
{
    QTextCursor cursor = textEdit->textCursor();

    if (styleIndex != 0) {
        QTextListFormat::Style style = QTextListFormat::ListDisc;

        switch (styleIndex) {
        default:
        case 1:
            style = QTextListFormat::ListDisc;
            break;
        case 2:
            style = QTextListFormat::ListCircle;
            break;
        case 3:
            style = QTextListFormat::ListSquare;
            break;
        case 4:
            style = QTextListFormat::ListDecimal;
            break;
        case 5:
            style = QTextListFormat::ListLowerAlpha;
            break;
        case 6:
            style = QTextListFormat::ListUpperAlpha;
            break;
        case 7:
            style = QTextListFormat::ListLowerRoman;
            break;
        case 8:
            style = QTextListFormat::ListUpperRoman;
            break;
        }

        cursor.beginEditBlock();

        QTextBlockFormat blockFmt = cursor.blockFormat();

        QTextListFormat listFmt;

        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }

        listFmt.setStyle(style);

        cursor.createList(listFmt);

        cursor.endEditBlock();
    } else {
        // ####
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

void RichTextEdit::textColor()
{
    QColor col = QColorDialog::getColor(textEdit->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void RichTextEdit::textAlign(QAction *a)
{
    if (a == actionAlignLeft)
        textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == actionAlignCenter)
        textEdit->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
        textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == actionAlignJustify)
        textEdit->setAlignment(Qt::AlignJustify);
}

void RichTextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void RichTextEdit::cursorPositionChanged()
{
    alignmentChanged(textEdit->alignment());
}

void RichTextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());
#endif
}

void RichTextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

void RichTextEdit::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void RichTextEdit::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}

void RichTextEdit::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft) {
        actionAlignLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
        actionAlignCenter->setChecked(true);
    } else if (a & Qt::AlignRight) {
        actionAlignRight->setChecked(true);
    } else if (a & Qt::AlignJustify) {
        actionAlignJustify->setChecked(true);
    }
}

QSize RichTextEdit::sizeHint() const
{
    QSize s = QMainWindow::sizeHint();
    s.setWidth( std::max(s.width(), fontMetrics().width('M') * 80) );
    s.setHeight( std::max(s.height(), fontMetrics().height() * 8) );
    return s;
}

void RichTextEdit::setCustomStyles(const XtringList& customstyles)
{
    for( XtringList::const_iterator it = customstyles.begin();
            it != customstyles.end(); ++ it ) {
        comboCustomStyle->addItem( toGUI(*it) );
    }
}

void RichTextEdit::textCustomStyle(int customStyleIndex)
{
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::BlockUnderCursor);
    QTextBlockFormat bfmt;
    cursor.setBlockFormat(bfmt);
    QTextCharFormat cfmt;
    cursor.setCharFormat(cfmt);
    textEdit->setTextCursor( cursor );
}

void RichTextEdit::textHasChanged()
{
    mJustEdited = true;
}

bool RichTextEdit::isEdited() const
{
    return textEdit->document()->isModified();
}

bool RichTextEdit::isJustEdited() const
{
    return mJustEdited;
}

void RichTextEdit::setEdited(bool e)
{
    textEdit->document()->setModified( e );
}

void RichTextEdit::focusInEvent(QFocusEvent* event)
{
    QWidget::focusInEvent(event);
    mJustEdited = false;
}

void RichTextEdit::focusOutEvent(QFocusEvent* event)
{
    QWidget::focusOutEvent(event);
}

} // namespace

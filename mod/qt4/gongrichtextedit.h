#ifndef _GONG_RICHTEXTEDIT_H
#define _GONG_RICHTEXTEDIT_H

/** @file gongrichtextedit.h Qt based Rich Text Edit
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */


#include <QMainWindow>
#include <QMap>
#include <QPointer>

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QFontComboBox)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QTextCharFormat)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QToolBar)

#include "gongguicontrols.h"


namespace gong {

class RichTextEdit : public QMainWindow
{
    Q_OBJECT

public:
    RichTextEdit(Xtring &text, QWidget *parent = 0);
    RichTextEdit(QLabel *txtlabel, QWidget *parent = 0, bool modal = false);

    void setCustomStyles( const XtringList &customstyles );
    QString getText() const;
    void setText( const QString &text );

protected:
    virtual void closeEvent(QCloseEvent *e);
    virtual QSize sizeHint() const; // from QWidget
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

private:
    void setup();
    void setupFileActions();
    void setupEditActions();
    void setupTextActions();
    bool load(const QString &f);
    bool maybeSave();
    void setCurrentFileName(const QString &fileName);

private slots:
    void fileNew();
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();
    void filePrint();
    void filePrintPreview();
    void filePrintPdf();

    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textStyle(int styleIndex);
    void textCustomStyle(int customStyleIndex);
    void textColor();
    void textAlign(QAction *a);

    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();

    void clipboardDataChanged();
    void printPreview(QPrinter *);

private:
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);

    QAction *actionSave,
            *actionTextBold,
            *actionTextUnderline,
            *actionTextItalic,
            *actionTextColor,
            *actionAlignLeft,
            *actionAlignCenter,
            *actionAlignRight,
            *actionAlignJustify,
            *actionUndo,
            *actionRedo,
            *actionCut,
            *actionCopy,
            *actionPaste;

    QComboBox *comboStyle, *comboCustomStyle;
    QFontComboBox *comboFont;
    QComboBox *comboSize;

    QToolBar *tb;
    QString fileName;
    QTextEdit *textEdit;
    QLabel *sourceTextLabel;
    QString mPlainText;
    bool mIsModal;
};

}; // namespace

#endif  // _GONG_RICHTEXTEDIT_H

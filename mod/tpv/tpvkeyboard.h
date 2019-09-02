#ifndef _TPV_KEYBOARD_H
#define _TPV_KEYBOARD_H

#include <QDialog>
#include <QToolButton>
#include <gongguinamespace.h>

namespace gong {

class LineEdit;

namespace tpv {

class KeyButton : public QToolButton
{
	Q_OBJECT

public:
	KeyButton(const Xtring &text, QWidget *parent = 0);

	QSize sizeHint() const;
};


class KeyBoard : public QDialog
{
	Q_OBJECT

public:
	KeyBoard(const Xtring &title, QWidget *parent = 0);
	void setValue( const Xtring &value );
	Xtring getValue() const;

private slots:
	void keyClicked();
	void backspaceClicked();
	void clear();

private:
	KeyButton *createButton(const Xtring &text, const char *member);
	LineEdit *display;

	KeyButton *keyButtons[200];
};

} // namespace tpv
} // namespace gong

#endif // _TPV_KEYBOARD_H
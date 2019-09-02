#include <QGridLayout>
#include <gonggettext.h>
#include <gonglineedit.h>
#include <dbappdbapplication.h>
#include "tpvkeyboard.h"

namespace gong {
namespace tpv {


KeyButton::KeyButton(const Xtring &text, QWidget *parent)
	: QToolButton(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	setText( toGUI( text ) );
}

QSize KeyButton::sizeHint() const
{
	QSize size = QToolButton::sizeHint();
	size.rheight() += 20;
	size.rwidth() = qMax(size.width(), size.height());
	return size;
}


KeyBoard::KeyBoard(const Xtring &title, QWidget *parent)
     : QDialog(parent)
{
	display = new LineEdit(this, "");
	display->setReadOnly(true);

	const char *rows[3] = {
		"QWERTYUIOP[]789*",
		"ASDFGHIJKL{}456+",
		"ZXCVBNM,.-/0123="
	};

	QFont font = display->font();
	font.setPointSize(font.pointSize() + 8);
	display->setFont(font);

	uint count = 0, r, k;
	for(r = 0; r < 3; ++r) {
		for( k = 0; k < strlen( rows[r] ); ++k ) {
			keyButtons[count++] = createButton(Xtring(1, rows[r][k] ), SLOT(keyClicked()));
		}
	}

	KeyButton *backspaceButton = createButton(_("Borra"), SLOT(backspaceClicked()));
	KeyButton *clearAllButton = createButton(_("Limpia todo"), SLOT(clear()));
	KeyButton *cancelButton = createButton( _("Cancelar"), SLOT( close() ) );
	KeyButton *acceptButton = createButton( _("Aceptar"), SLOT( accept() ) );

	QGridLayout *mainLayout = new QGridLayout;
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_SIMULATOR)
	mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
#else
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
#endif
	mainLayout->addWidget( display, 0, 0, 1, 16 );
	count = 0;
	for( r = 0; r < 3; ++r) {
		for( k = 0; k < strlen( rows[r] ); ++k ) {
			mainLayout->addWidget(keyButtons[ count++ ], r + 1, k);
		}
	}
	mainLayout->addWidget(backspaceButton, 4, 0);
	mainLayout->addWidget(clearAllButton, 4, 1);
	mainLayout->addWidget(acceptButton, 4, 2 );
	mainLayout->addWidget(cancelButton, 4, 3 );

	setLayout(mainLayout);

	setWindowTitle( toGUI( title ) );
}

Xtring KeyBoard::getValue() const
{
	return display->toString();
}

void KeyBoard::setValue(const Xtring &value)
{
	display->setText( value );
}


void KeyBoard::keyClicked()
{
	KeyButton *clickedButton = qobject_cast<KeyButton *>(sender());
	Xtring value = fromGUI( clickedButton->text() );
	display->setText(display->toString() + value);
}

void KeyBoard::backspaceClicked()
{
	Xtring text = display->toString();
	if( text.size() > 1 )
		text = text.mid( 0, text.size()-1 );
	else
		text = "";
	display->setText(text);
}

void KeyBoard::clear()
{
	display->setText("");
}

KeyButton *KeyBoard::createButton(const Xtring &text, const char *member)
{
	KeyButton *button = new KeyButton( text );
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}


} // namespace tpv
} // namespace gong


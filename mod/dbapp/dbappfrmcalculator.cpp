#include <math.h>
#include <QGridLayout>
#include <gonggettext.h>
#include <gonglineedit.h>
#include <dbappdbapplication.h>
#include "dbappfrmcalculator.h"

namespace gong {

CalcButton::CalcButton(const Xtring &text, QWidget *parent)
    : QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setText( toGUI( text ) );
}

QSize CalcButton::sizeHint() const
{
    QSize size = QToolButton::sizeHint();
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}


FrmCalculator::FrmCalculator(bool simple, bool embedded, QWidget *parent)
    : FrmBase(parent, "FrmCalculator")
{

    sumInMemory = 0.0;
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    waitingForOperand = true;

    display = new LineEdit(this, "0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);

    for (int i = 0; i < NumDigitButtons; ++i) {
        digitButtons[i] = createButton(Xtring::number(i), SLOT(digitClicked()));
    }

    CalcButton *pointButton = createButton( Xtring(1, DBAPP->getRegConfig()->getDecimalPoint()), SLOT(pointClicked()));
    CalcButton *changeSignButton = createButton( _("\xC2\xB1"), SLOT(changeSignClicked()));

    CalcButton *backspaceButton = createButton(_("Borra dígito"), SLOT(backspaceClicked()));
    CalcButton *clearButton = createButton(_("Borra"), SLOT(clear()));
    CalcButton *clearAllButton = createButton(_("Pone a 0"), SLOT(clearAll()));
    CalcButton *clearMemoryButton, *readMemoryButton, *setMemoryButton, *addToMemoryButton;
    if( !simple ) {
        clearMemoryButton = createButton(_("MC"), SLOT(clearMemory()));
        readMemoryButton = createButton(_("MR"), SLOT(readMemory()));
        setMemoryButton = createButton(_("MS"), SLOT(setMemory()));
        addToMemoryButton = createButton(_("M+"), SLOT(addToMemory()));
    }

    CalcButton *divisionButton = createButton(_("\xc3\xb7"), SLOT(multiplicativeOperatorClicked()));
    CalcButton *timesButton = createButton(_("\xc3\x97"), SLOT(multiplicativeOperatorClicked()));
    CalcButton *minusButton = createButton( DBAPP->getRegConfig()->getNegativeSign(), SLOT(additiveOperatorClicked()));
    CalcButton *plusButton = createButton(_("+"), SLOT(additiveOperatorClicked()));

    CalcButton *squareRootButton, *powerButton, *reciprocalButton, *equalButton;
    if( !simple ) {
        squareRootButton = createButton(_("Raíz"), SLOT(unaryOperatorClicked()));
        powerButton = createButton(_("x\xc2\xb2"), SLOT(unaryOperatorClicked()));
        reciprocalButton = createButton(_("1/x"), SLOT(unaryOperatorClicked()));
        equalButton = createButton(_("="), SLOT(equalClicked()));
    }
    CalcButton *cancelButton, *acceptButton;
    if( !embedded ) {
        cancelButton = createButton( _("Cancelar"), SLOT( close() ) );
        acceptButton = createButton( _("Aceptar"), SLOT( accept() ) );
    }

    pGridLayout = new QGridLayout;
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_SIMULATOR)
    pGridLayout->setSizeConstraint(QLayout::SetNoConstraint);
#else
    pGridLayout->setSizeConstraint(QLayout::SetFixedSize);
#endif
    if( !embedded )
        pGridLayout->addWidget(display, 0, 0, 1, 6);
    else
        pGridLayout->addWidget(display, 1, 6);

    pGridLayout->addWidget(backspaceButton, 2, 6);
    pGridLayout->addWidget(clearAllButton, 3, 6);

    if( !simple ) {
        pGridLayout->addWidget(clearButton, 2, 6);
        pGridLayout->addWidget(clearMemoryButton, 3, 6);
        pGridLayout->addWidget(readMemoryButton, 4, 6);
        pGridLayout->addWidget(setMemoryButton, 5, 6);
        pGridLayout->addWidget(addToMemoryButton, 6, 6);
    }

    for (int i = 1; i < NumDigitButtons; ++i) {
        int row = ((9 - i) / 3) + 1;
        int column = ((i - 1) % 3) + 1;
        pGridLayout->addWidget(digitButtons[i], row, column);
    }

    pGridLayout->addWidget(changeSignButton, 4, 1);
    pGridLayout->addWidget(digitButtons[0], 4, 2);
    pGridLayout->addWidget(pointButton, 4, 3);

    pGridLayout->addWidget(divisionButton, 1, 4);
    pGridLayout->addWidget(timesButton, 2, 4);
    pGridLayout->addWidget(minusButton, 3, 4);
    pGridLayout->addWidget(plusButton, 4, 4);

    if( !simple ) {
        pGridLayout->addWidget(squareRootButton, 1, 5);
        pGridLayout->addWidget(powerButton, 2, 5);
        pGridLayout->addWidget(reciprocalButton, 3, 5);
        pGridLayout->addWidget(equalButton, 4, 5);
    }
    if( !embedded ) {
        pGridLayout->addWidget(acceptButton, 6, 1 );
        pGridLayout->addWidget(cancelButton, 6, 3 );
    }

    setLayout(pGridLayout);

    setWindowTitle(_("FrmCalculator"));
}

double FrmCalculator::getValue() const
{
    return display->toDouble();
}

void FrmCalculator::setValue(double value)
{
    display->setText( value );
    waitingForOperand = false;
}


void FrmCalculator::digitClicked()
{
    CalcButton *clickedButton = dynamic_cast<CalcButton *>(sender());
    int digitValue = clickedButton->text().toInt();
    if (display->toString() == "0" && digitValue == 0 )
        return;
    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }
    display->setText(display->toString() + Xtring::number(digitValue));
}

void FrmCalculator::unaryOperatorClicked()
{
    CalcButton *clickedButton = dynamic_cast<CalcButton *>(sender());
    Xtring clickedOperator = fromGUI(clickedButton->text());
    double operand = display->toString().toDouble();
    double result = 0.0;

    if (clickedOperator == _("Raíz")) {
        if (operand < 0.0) {
            abortOperation();
            return;
        }
        result = sqrt(operand);
    } else if (clickedOperator == _("x\xc2\xb2")) {
        result = pow(operand, 2.0);
    } else if (clickedOperator == _("1/x")) {
        if (operand == 0.0) {
            abortOperation();
            return;
        }
        result = 1.0 / operand;
    }
    display->setText(Xtring::number(result));
    waitingForOperand = true;
}

void FrmCalculator::additiveOperatorClicked()
{
    CalcButton *clickedButton = dynamic_cast<CalcButton *>(sender());
    Xtring clickedOperator = fromGUI(clickedButton->text());
    double operand = display->toString().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        display->setText(Xtring::number(factorSoFar));
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }

    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        display->setText(Xtring::number(sumSoFar));
    } else {
        sumSoFar = operand;
    }

    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}

void FrmCalculator::multiplicativeOperatorClicked()
{
    CalcButton *clickedButton = dynamic_cast<CalcButton *>(sender());
    Xtring clickedOperator = fromGUI(clickedButton->text());
    double operand = display->toString().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        display->setText(Xtring::number(factorSoFar));
    } else {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;
}

void FrmCalculator::equalClicked()
{
    double operand = display->toString().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        pendingAdditiveOperator.clear();
    } else {
        sumSoFar = operand;
    }

    display->setText(Xtring::number(sumSoFar));
    sumSoFar = 0.0;
    waitingForOperand = true;
}

void FrmCalculator::pointClicked()
{
    if (waitingForOperand)
        display->setText("0");
    if (display->toString().find( DBAPP->getRegConfig()->getDecimalPoint() ) == Xtring::npos )
        display->setText(display->toString() + DBAPP->getRegConfig()->getDecimalPoint() );
    waitingForOperand = false;
}

void FrmCalculator::changeSignClicked()
{
    Xtring text = display->toString();
    double value = text.toDouble();

    if (value > 0.0) {
        text = DBAPP->getRegConfig()->getNegativeSign() + text;
    } else if (value < 0.0) {
        text = text.mid(1);
    }
    display->setText(text);
}

void FrmCalculator::backspaceClicked()
{
    if (waitingForOperand)
        return;

    Xtring text = display->toString();
    if( text.size() > 1 )
        text = text.mid( 0, text.size()-1 );
    else
        text = "";
    if (text.isEmpty()) {
        text = "0";
        waitingForOperand = true;
    }
    display->setText(text);
}

void FrmCalculator::clear()
{
    if (waitingForOperand)
        return;

    display->setText("0");
    waitingForOperand = true;
}

void FrmCalculator::clearAll()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    display->setText("0");
    waitingForOperand = true;
}

void FrmCalculator::clearMemory()
{
    sumInMemory = 0.0;
}

void FrmCalculator::readMemory()
{
    display->setText(Xtring::number(sumInMemory));
    waitingForOperand = true;
}

void FrmCalculator::setMemory()
{
    equalClicked();
    sumInMemory = display->toString().toDouble();
}

void FrmCalculator::addToMemory()
{
    equalClicked();
    sumInMemory += display->toString().toDouble();
}

CalcButton *FrmCalculator::createButton(const Xtring &text, const char *member)
{
    CalcButton *button = new CalcButton( text );
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void FrmCalculator::abortOperation()
{
    clearAll();
    display->setText(_("####"));
}

bool FrmCalculator::calculate(double rightOperand, const Xtring &pendingOperator)
{
    if (pendingOperator == _("+")) {
        sumSoFar += rightOperand;
    } else if (pendingOperator == DBAPP->getRegConfig()->getNegativeSign() ) {
        sumSoFar -= rightOperand;
    } else if (pendingOperator == _("\xc3\x97")) {
        factorSoFar *= rightOperand;
    } else if (pendingOperator == _("\xc3\xb7")) {
        if (rightOperand == 0.0)
            return false;
        factorSoFar /= rightOperand;
    }
    return true;
}

} // namespace


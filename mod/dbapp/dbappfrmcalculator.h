#ifndef _GONG_DBAPPFRMCALCULATOR_H
#define _GONG_DBAPPFRMCALCULATOR_H

/** @file dbappfrmcalculator.h A simple calculator form
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QToolButton>
#include <gongfrmbase.h>
#include <gongguinamespace.h>

namespace gong {

class LineEdit;

class CalcButton : public QToolButton
{
public:
    CalcButton(const Xtring &text, QWidget *parent = 0);

    QSize sizeHint() const;
};


class FrmCalculator : public FrmBase
{
    Q_OBJECT

public:
    FrmCalculator(bool simple, bool embedded, QWidget *parent = 0);
    void setValue( double value );
    double getValue() const;
    QGridLayout *getGridLayout() const {
        return pGridLayout;
    }
public slots:
    void clearAll();

private slots:
    void digitClicked();
    void unaryOperatorClicked();
    void additiveOperatorClicked();
    void multiplicativeOperatorClicked();
    void equalClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearMemory();
    void readMemory();
    void setMemory();
    void addToMemory();

private:
    CalcButton *createButton(const Xtring &text, const char *member);
    void abortOperation();
    bool calculate(double rightOperand, const Xtring &pendingOperator);

    double sumInMemory;
    double sumSoFar;
    double factorSoFar;
    Xtring pendingAdditiveOperator;
    Xtring pendingMultiplicativeOperator;
    bool waitingForOperand;

    LineEdit *display;

    enum { NumDigitButtons = 10 };
    CalcButton *digitButtons[NumDigitButtons];
    QGridLayout *pGridLayout;
};

} // namespace gong

#endif

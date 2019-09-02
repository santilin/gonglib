#ifndef _TPV_TPVFRMTICKETCAMBIO_H
#define _TPV_TPVFRMTICKETCAMBIO_H

#include <dbappfrmcustom.h>
#include "tpvbutton.h"

namespace gong {
namespace tpv {

class FrmTpvScreen;

class FrmTicketCambio: public FrmCustom
{
	Q_OBJECT
public:
	FrmTicketCambio( FrmTpvScreen *parent, Money total );
	void validate_input(QWidget *sender , bool *is_valid);
	Money getEntrega() const;
	Money getCambio() const { return getEntrega() - mTotal; }
	bool mustPrint() const { return mMustPrint; }

protected slots:
	void slotImprimir();
	void slotEuros();

private:
	FrmTpvScreen *pFrmTpvScreen;
	Money mTotal;
	LineEdit *pEditEntrega;
	QLabel *pLabelCambio;
	PushButton *pushImprimir;
	TPVButton *push1euro, *push2euros, *push5euros,
		*push10euros, *push20euros, *push50euros;
	bool mCanReset, mMustPrint;
};

} // namespace tpv
} // namespace gong

# endif // _TPV_TPVFRMTICKETCAMBIO_H

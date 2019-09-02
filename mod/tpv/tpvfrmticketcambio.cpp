#include "tpvfrmtpvscreen.h"
#include "tpvfrmticketcambio.h"

namespace gong {
namespace tpv {

FrmTicketCambio::FrmTicketCambio(FrmTpvScreen* parent, Money total):
	FrmCustom( parent, "FrmTicketCambio" ), mTotal( total ),
	mCanReset( true ), mMustPrint( false )
{
	setCaption( _("Cerrar y guardar") );
	addLabel(0, _("Total"), mTotal );
	pEditEntrega = addInput(0, _("Entrega"), total );
	pEditEntrega->setSelectedOnEntry( true );
	pEditEntrega->setFocus();
	pLabelCambio = addLabel(0, _("Cambio"), 0.0 );

	QGridLayout *botonesLayout = new QGridLayout();

	pushImprimir = new PushButton( this , _("CON Ticket") );
	pushImprimir->setIcon( QIcon::fromTheme("printer", QIcon(":/printer.png")) );
	connect( pushImprimir, SIGNAL( clicked() ), this, SLOT( slotImprimir() ) );
	push1euro = new TPVButton( _("1 €" ), this );
	connect( push1euro, SIGNAL( clicked() ), this, SLOT( slotEuros() ) );
	push2euros = new TPVButton( _("2 €" ), this );
	connect( push2euros, SIGNAL( clicked() ), this, SLOT( slotEuros() ) );
	push5euros = new TPVButton( _("5 €" ), this );
	connect( push5euros, SIGNAL( clicked() ), this, SLOT( slotEuros() ) );
	push10euros = new TPVButton( _("10 €" ), this );
	connect( push10euros, SIGNAL( clicked() ), this, SLOT( slotEuros() ) );
	push20euros = new TPVButton( _("20 €" ), this );
	connect( push20euros, SIGNAL( clicked() ), this, SLOT( slotEuros() ) );
	push50euros = new TPVButton( _("50 €" ), this );
	connect( push50euros, SIGNAL( clicked() ), this, SLOT( slotEuros() ) );

	pushAccept->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	pushAccept->setText( toGUI( _("SIN Ticket") ) );
	pushCancel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	botonesLayout->addWidget(pushAccept, 0, 0, 2, 2 );
	botonesLayout->addWidget(pushImprimir, 0, 2, 2, 2 );
	botonesLayout->addWidget(pushCancel, 0, 4, 2, 2 );
	botonesLayout->addWidget(push1euro, 2, 0 );
	botonesLayout->addWidget(push2euros, 2, 1 );
	botonesLayout->addWidget(push5euros, 2, 2 );
	botonesLayout->addWidget(push10euros, 2, 3 );
	botonesLayout->addWidget(push20euros, 2, 4 );
	botonesLayout->addWidget(push50euros, 2, 5 );
	pControlsLayout->addLayout( botonesLayout );
}

void FrmTicketCambio::validate_input(QWidget* sender, bool* is_valid)
{
	FrmCustom::validate_input(sender, is_valid);
	if( sender == pEditEntrega ) {
		pLabelCambio->setText( toGUI( (pEditEntrega->toMoney() - mTotal).toString( DBAPP->getRegConfig() ) ) );
	}
}

Money FrmTicketCambio::getEntrega() const
{
	return pEditEntrega->toMoney(); /*< mTotal ? pEditEntrega->toMoney() : mTotal;*/
}

void FrmTicketCambio::slotImprimir()
{
	mMustPrint = true;
	accept();
}

void FrmTicketCambio::slotEuros()
{
	double cantidad;
	if( sender() == push1euro )
		cantidad = 1.0;
	else if( sender() == push2euros )
		cantidad = 2.0;
	else if( sender() == push5euros )
		cantidad = 5.0;
	else if( sender() == push10euros )
		cantidad = 10.0;
	else if( sender() == push20euros )
		cantidad = 20.0;
	else if( sender() == push50euros )
		cantidad = 50.0;
	if( pEditEntrega->isJustEdited() || !mCanReset )
		pEditEntrega->setText( pEditEntrega->toDouble() + cantidad );
	else
		pEditEntrega->setText( cantidad );
	mCanReset = false;
	validate_input( pEditEntrega, 0 );
	pEditEntrega->setJustEdited( false );
}


} // namespace tpv
} // namespace gong

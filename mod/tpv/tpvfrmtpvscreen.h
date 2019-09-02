#ifndef _TPV_TPVFRMEDITSCREEN_H
#define _TPV_TPVFRMEDITSCREEN_H

#include "config.h"
#include <QGraphicsItem>
#include <QTableView>
#include <QToolButton>
#include <gongdbrecordlist.h>
#include <dbappfrmcustom.h>
#include <facturecalbaranventa.h>
#ifdef HAVE_SOCIASMODULE
#include <sociasrecmiembro.h>
#endif

class QToolButton;
class QSplitter;
class QTimer;

namespace gong {

class SearchBox;
class RichTextBox;
class PushButton;
class FrmCalculator;
class RS232;

namespace tpv {

class TPVButton;
class ArticulosGallery;
class FamiliasGallery;
class FavoritosGallery;
class FrmChooseMesa;
class FrmChooseByImage;
class RecSesionTPV;
class FrmTpvScreen;

typedef struct { const char *mesaname;
				factu::RecAlbaranVenta *albaranventa;
				QRectF coords;
				int ronda;
#ifdef HAVE_SOCIASMODULE
				socias::RecMiembro *pRecMiembro;
#endif
} MesaInfo;
typedef Dictionary<MesaInfo> DictMesas;

class TicketItemsModel: public QAbstractTableModel
{
public:
	TicketItemsModel( FrmTpvScreen *tpvscreen, DictMesas &mesas );
	int rowCount( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount( const QModelIndex & parent = QModelIndex() ) const;
	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	void reset() { QAbstractTableModel::reset(); }
	void refresh(int row);
	void addDetalle( factu::RecAlbaranVentaDet *detalle );
	void delDetalle( int row );
	const Xtring &getCurrentMesaName() const { return mCurrentMesaName; }
	MesaInfo &getCurrentMesa() const;
	MesaInfo &setCurrentMesa( const Xtring &current_index );
	void setRonda( int ronda );
	int getRonda() const;
#ifdef HAVE_SOCIASMODULE
	socias::RecMiembro *getRecMiembro() const;
#endif
private:
	enum Columns { cantidad = 0, pvp, nombrearticulo, importeconiva };
	DictMesas &mMesas;
	dbTableDefinition *pAlbVentaDetDefinition;
	Xtring mCurrentMesaName;
	FrmTpvScreen *pFrmTpvScreen;
	bool mUpperCase;
};

class FrmTpvScreen: public FrmCustom
{
	Q_OBJECT
public:
	enum ModalidadTPV {
		ModNada =   0x0000,
		ModBar =    0x0001,
		ModTienda = 0x0002,
		ModRondas = 0x0004,
		ModCajon =  0x0008,
		ModSocias = 0x0010,
		ModFilaFavoritos = 0x0020,
		ModBalanza = 0x0040,
	};

	FrmTpvScreen( ModalidadTPV modalidad, RecSesionTPV *recsesiontpv, QWidget* parent = 0, WidgetFlags fl = 0 );
	~FrmTpvScreen();

	bool isBar() const { return mModalidad & ModBar; }
	bool isTienda() const { return mModalidad & ModTienda; }
	bool isRondas() const { return mModalidad & ModRondas; }
	bool isBalanza() const { return mModalidad & ModBalanza; }
	bool isSocias() const { return mModalidad & ModSocias; }
	bool isCajon() const { return mModalidad & ModCajon; }
	ModalidadTPV getModalidad() const { return mModalidad ; }

	void accept(); // from FrmCustom
	bool printToPrinter(Money entrega, Money cambio, Money resto = Money(),
		const Xtring &desc_formapago = Xtring::null)
		{ return print(true, entrega, cambio, resto, desc_formapago); }
	void openDrawer();
	void clear( factu::RecAlbaranVenta *albaranventa );
	MesaInfo &getCurrentMesa() const { return pTicketItemsModel->getCurrentMesa(); }
	MesaInfo &setCurrentMesa( const Xtring &mesaname );

protected:
	bool save();
	bool printToScreen() { return print(false, Money(), Money(), Money(), Xtring::null ); }
	bool print(bool toprinter, Money entrega, Money cambio, Money resto, const Xtring &desc_formapago);
	void gather();
	void actualizaTotal();
	int fillCuentas( DictMesas &mesas, int ncuentas = 10 );
	Xtring getMesaName() const;
	void clearCurrentMesa();
	factu::RecAlbaranVentaDet* findArticuloInDetalles( factu::RecAlbaranVenta *albaranventa,
							factu::RecArticulo* recart, const Xtring &variante, Money pvp,
							int &row, int ronda );
	bool chooseVariante( factu::RecArticulo *articulo, Xtring &variante, Money &pvp );
	bool canClose();
	void saveEstado();
	void restoreEstado();
	Xtring getGeometry() const; // from FrmCustom
	void setGeometry( const Xtring &geometry ); // from FrmCustom

protected slots:
	void validate_input( QWidget *, bool * );
	void slotSave();
	void slotOpenDrawer();
	void slotClear();
	void slotDeleteLine();
	void slotPrintTicket();
	void slotChooseAgente();
	void slotChooseCliente();
	void slotElegirMesa();
	void slotElegirCuenta();
	void slotCambiarDeMesa();
	void slotHistorico();
	void slotImageFamilia( QGraphicsItem *);
	void slotImageArticulo( QGraphicsItem *);
	void slotEditCantidadOPVP();
	void slotGuardar();
	void slotTimerShot();
	void slotNuevaRonda();
	void slotBalanza();
	void slotFijarPrecioArticulo();
	void slotAddDescuentoRecargo();

protected:
	int getCurrentRow() const;
	void setCurrentRow( uint row );
	void updateTitle();
	void updateAgenteClienteLabel( const MesaInfo &mesa );
	int buscaCliente( const Xtring &nombrecliente );

	ModalidadTPV mModalidad;
	RecSesionTPV *pRecSesionTPV;
	factu::RecArticulo *pRecArticulo;
	factu::RecFamilia *pRecFamilia;
	factu::RecAgente *pDefRecAgente;
	factu::RecCliente *pDefRecCliente;
#ifdef HAVE_SOCIASMODULE
	dbRecordID mProyectoID;
#endif
	TicketItemsModel *pTicketItemsModel;
	QTableView *pTableTicket;
	SearchBox *pSearchArticulo;
	QLabel *pLabelTotal, *pLabelAgente, *pLabelCliente, *pLabelMesa;
	TPVButton *pushSave, *pushOpenDrawer, *pushClear, *pushDeleteLine, *pushPrintTicket,
		*pushEditCantidad, *pushEditPVP, *pushChooseAgente, *pushCambiarDeMesa;
	TPVButton *pushHistorico, *pushElegirMesa, *pushChooseCliente, *pushElegirCuenta,
		*pushGuardar, *pushNuevaRonda, *pushBalanza, *pushFijarPrecioArticulo;
	TPVButton *pushAddDescuentoRecargo;
	QTimer *pSaveEstadoTimer;
	ArticulosGallery *pArticulosGallery;
	FamiliasGallery *pFamiliasGallery;
	FavoritosGallery *pFavoritosGallery;
	dbRecordID mTipoDocID, mFormaPagoID;
	DictMesas mDictMesas;
	FrmChooseMesa *pFrmElegirMesa;
	FrmChooseByImage *pFrmChooseAgente;
	Xtring mLastEstado, mVendedoraName;
	QSplitter *pSplitterLeft, *pSplitterRight;
	bool mMainWindowWasMaximized;
	QTimer *pTimer;
	FrmCalculator *pCalculator;
	RS232 *pPuertoBalanza;
};

} // namespace tpv
} // namespace gong

# endif // _TPV_TPVFRMEDITSCREEN_H

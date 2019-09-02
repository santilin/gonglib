#include <cerrno>
#include <QSplitter>
#include <QTimer>
#include <QHeaderView>
#include <gongfileutils.h>
#include <gongguimainwindow.h>
#include <gongregexp.h>
#include <dbappreport.h>
#include <dbappfrmcalculator.h>
#include <factufldivadetallado.h>
#include <facturecalbaranventa.h>
#include <rtkinputdbrecord.h>
#include "tpvmodule.h"
#include "tpvbutton.h"
#include "tpvkeyboard.h"
#include "tpvfrmticketcambio.h"
#include "tpvfrmchoosebyimage.h"
#include "tpvfrmchoosemesa.h"
#include "tpvfrmtpvscreen.h"
#include "tpvimagesgallery.h"
#include "tpvrecsesiontpv.h"
#include "tpvfrmcustom.h"
#include "factumodule.h"
#include "rs232.h"
#ifdef HAVE_SOCIASMODULE
#include <sociasrecmiembro.h>
#endif

namespace gong {
namespace tpv {

class ArticulosGallery: public ImagesGallery
{
public:
	ArticulosGallery( QWidget *parent, factu::RecArticulo *ra, int sizex, int sizey,
					  int columns, int rows )
		: ImagesGallery( parent, sizex, sizey, columns, rows), pRecArticulo(ra)
		{
			setShowNames(true);
			showPagination(true);
			reload( Xtring::null );
		}
	void reload( const Xtring &familicode ) {
		clear();
		if( !familicode.isEmpty() ) {
			dbResultSet *rs = pRecArticulo->getConnection()->select(
				"SELECT IMAGEN, ARTICULO.CODIGO, ARTICULO.NOMBRE, ARTICULO.ID, ARTICULOIMAGEN_ID"
				" FROM ARTICULO LEFT JOIN ARTICULOIMAGEN"
				"   ON ARTICULO.ARTICULOIMAGEN_ID=ARTICULOIMAGEN.ID"
				" INNER JOIN FAMILIA ON FAMILIA.ID=ARTICULO.FAMILIA_ID"
				" WHERE ARTICULO.PARATPV=1 AND FAMILIA.CODIGO=" + pRecArticulo->getConnection()->toSQL( familicode )
				+ " ORDER BY ARTICULO.ORDENTPV, ARTICULO.NOMBRE, ARTICULOIMAGEN_ID DESC");
			if( rs->getRowCount() > mPreferredRows * mColumns )
				showPagination(true);
			else
				showPagination(false);
			while( rs->next() ) {
				QGraphicsItem *item = addImage( rs->toBinary(uint(0)), rs->toString( 2 ) );
				item->setData(0, toGUI( rs->toString( 1 ) ) );
				item->setData(1, toGUI( rs->toString( 2 ) ) );
			}
			delete rs;
		}
		nextPage();
	}

private:
	factu::RecArticulo *pRecArticulo;
};

class FamiliasGallery: public ImagesGallery
{
public:
	FamiliasGallery( QWidget *parent, factu::RecFamilia *rf, int sizex, int sizey,
					 int columns, int rows )
		: ImagesGallery( parent, sizex, sizey, columns, rows ), pRecFamilia(rf)
		{
			setShowNames(true);
// 			setSelectable(true);
			addImages();
		}
	void addImages() {
		dbResultSet *rs = pRecFamilia->getConnection()->select(
			"SELECT IMAGEN, FAMILIA.CODIGO, FAMILIA.NOMBRE, FAMILIA.ID, FAMILIAIMAGEN_ID"
			" FROM FAMILIA LEFT JOIN ARTICULOIMAGEN"
			"   ON FAMILIA.FAMILIAIMAGEN_ID=ARTICULOIMAGEN.ID"
			" WHERE FAMILIA.PARATPV=1 ORDER BY FAMILIA.ORDENTPV, FAMILIA.CODIGO");
		if( rs->getRowCount() > mPreferredRows * mColumns )
			showPagination(true);
		else
			showPagination(false );
		while( rs->next() ) {
			Xtring familycode = rs->toString(1);
			Xtring familyname = rs->toString(2);
			QGraphicsItem *item = addImage( rs->toBinary(uint(0)), familyname );
			item->setData(0, toGUI(familycode) );
			item->setData(1, toGUI(familyname) );
		}
		delete rs;
	}

private:
	factu::RecFamilia *pRecFamilia;
};

class FavoritosGallery: public ImagesGallery
{
public:
	FavoritosGallery( QWidget *parent, factu::RecArticulo *ra, int sizex, int sizey,
					 int columns, int rows )
		: ImagesGallery( parent, sizex, sizey, columns, rows ), pRecArticulo(ra)
		{
			setShowNames(true);
			addImages();
		}
	void addImages() {
		dbResultSet *rs = pRecArticulo->getConnection()->select(
			"SELECT IMAGEN, ARTICULO.CODIGO, ARTICULO.NOMBRE, ARTICULO.ID, ARTICULOIMAGEN_ID"
			" FROM ARTICULO LEFT JOIN ARTICULOIMAGEN"
			"   ON ARTICULO.ARTICULOIMAGEN_ID=ARTICULOIMAGEN.ID"
			" INNER JOIN FAMILIA ON FAMILIA.ID=ARTICULO.FAMILIA_ID"
			" WHERE ARTICULO.PARATPV=1 AND ARTICULO.OFERTA=1"
			" ORDER BY ARTICULO.ORDENTPV, ARTICULO.NOMBRE, ARTICULOIMAGEN_ID DESC");
		if( rs->getRowCount() > mPreferredRows * mColumns )
			showPagination(true);
		else
			showPagination(false);
		while( rs->next() ) {
			QGraphicsItem *item = addImage( rs->toBinary((uint)0), rs->toString( 2 ) );
			item->setData(0, toGUI( rs->toString( 1 ) ) );
			item->setData(1, toGUI( rs->toString( 2 ) ) );
		}
		delete rs;
	}

private:
	factu::RecArticulo *pRecArticulo;
};


FrmTpvScreen::FrmTpvScreen( ModalidadTPV modalidad, RecSesionTPV *recsesiontpv, QWidget* parent, WidgetFlags fl )
	: FrmCustom( parent, "TPVScreen", fl ), mModalidad(modalidad), pRecSesionTPV( recsesiontpv ),
		mTipoDocID(0), mFormaPagoID(0), pFrmElegirMesa(0), pFrmChooseAgente(0), mMainWindowWasMaximized(false),
		pPuertoBalanza(0)
{
	DBAPP->waitCursor( true );

	// Se crea al principio para que cargue las mesas
	Xtring mesassvg = FileUtils::findInPath(
		DBAPP->getLocalDataDir()
		+ ":" + ModuleInstance->getModuleLocalDataDir()
		+ ":" + ModuleInstance->getModuleGlobalDataDir()
		+ ":" + DBAPP->getGlobalDataDir(), pRecSesionTPV->getValue("MESASSVG").toString(), "mesas" );
	if( isBar() ) {
		mVendedoraName = "Camarer@";
		// Aquí crea la información de cada mesa, según el SVG leído. Los albaranes no serán creados
		pFrmElegirMesa = new FrmChooseMesa( mDictMesas, mesassvg );
	} else {
		mVendedoraName = "Vendedor@";
		// Aquí se crean las cuentas, equivalentes a las mesas
		fillCuentas( mDictMesas, 2 );
	}
	pushCancel->setText( toGUI( _("Cerrar") ) );
	pushAccept->setText( toGUI( _("Grabar") ) );

	pRecArticulo = static_cast<factu::RecArticulo *>(DBAPP->createRecord("ARTICULO"));
	pRecFamilia = static_cast<factu::RecFamilia *>(DBAPP->createRecord("FAMILIA"));
	pDefRecAgente = static_cast<factu::RecAgente *>(DBAPP->createRecord("AGENTE"));
	pDefRecCliente = static_cast<factu::RecCliente *>(DBAPP->createRecord("CLIENTE"));
#ifdef HAVE_SOCIASMODULE
	mProyectoID = pRecArticulo->getConnection()->selectInt(
		"SELECT ID FROM PROYECTO WHERE CODIGO=" + pRecArticulo->getConnection()->toSQL(
			ModuleInstance->getModuleSetting("CODIGO_PROYECTO").toInt() ) );
#endif
	QFrame *frameSplitters = new QFrame(pControlsFrame);
	frameSplitters->setObjectName(QString::fromUtf8("frameSplitters"));
	frameSplitters->setFrameShape(QFrame::StyledPanel);
	frameSplitters->setFrameShadow(QFrame::Raised);
	QHBoxLayout *frameSplittersLayout = new QHBoxLayout(frameSplitters);
	frameSplittersLayout->setContentsMargins(0, 0, 0, 0);

	pSplitterLeft = new QSplitter(frameSplitters);
	pSplitterLeft->setObjectName( "leftSplitter" );
	pSplitterLeft->setOrientation(Qt::Horizontal);
	pSplitterLeft->setChildrenCollapsible( false );

	QWidget *left_widget = new QWidget(pSplitterLeft);
	QVBoxLayout *left_widgetLayout = new QVBoxLayout(left_widget);
	left_widgetLayout->setContentsMargins(0, 0, 0, 0);

	pLabelAgente = new QLabel( this, toGUI( _("Agente" ) ) );
	pLabelAgente->setObjectName( "LabelAgente" );
	left_widgetLayout->addWidget( pLabelAgente );
	pLabelCliente = new QLabel( this, toGUI( _("Cliente" ) ) );
	pLabelCliente->setObjectName( "LabelCliente" );
	left_widgetLayout->addWidget( pLabelCliente );
	pLabelMesa = new QLabel( this, toGUI( _("Mesa" ) ) );
	pLabelMesa->setObjectName( "LabelMesa" );
	left_widgetLayout->addWidget( pLabelMesa );

	pSearchArticulo = addMultipleSearchField( 0, "ARTICULO", "CODIGO", "NOMBRE", left_widgetLayout );
	connect( pSearchArticulo->getEditCode(), SIGNAL( validate( QWidget *, bool * ) ),
			 this, SLOT( validate_input(QWidget *, bool *) ) );
	pSearchArticulo->getEditCode()->setWidthInChars( 12 );

	pTableTicket = new QTableView( pSplitterLeft );
	pTableTicket->setSelectionMode( QAbstractItemView::SingleSelection );
	pTableTicket->setSelectionBehavior( QAbstractItemView::SelectRows );
	pTableTicket->setAlternatingRowColors( true );
	pTableTicket->verticalHeader()->setDefaultSectionSize( 16 );
	pTicketItemsModel = new TicketItemsModel( this, mDictMesas );
	left_widgetLayout->addWidget( pTableTicket );

	dbTableDefinition *albVentaDefinition = DBAPP->getDatabase()->findTableDefinition( "ALBARANVENTA" );
	QFrame *frameTotals = new QFrame( this );
	frameTotals->setObjectName( "FrameTotals" );
	frameTotals->setFrameShape( QFrame::StyledPanel );
	QHBoxLayout *frameTotals_layout = new QHBoxLayout( frameTotals );
	dbFieldDefinition *fld = albVentaDefinition->findFieldDefinition( "TOTAL" );
	pLabelTotal = addLabel( frameTotals, fld->getCaption(), Xtring::null, Xtring::null, 0, frameTotals_layout  );
	left_widgetLayout->addWidget(frameTotals);

	pCalculator = new FrmCalculator( true, true, this );
	pCalculator->getAcceptButton()->setVisible( false );
	pCalculator->getCancelButton()->setVisible( false );
	left_widgetLayout->addWidget( pCalculator );

	pSplitterLeft->addWidget( left_widget );

    pSplitterRight = new QSplitter(frameSplitters);
	pSplitterRight->setOrientation(Qt::Vertical);
	pSplitterRight->setChildrenCollapsible( false );

	int alto_imagenes = ModuleInstance->getModuleSetting("ALTO_IMAGENES").toInt();
	int ancho_imagenes = ModuleInstance->getModuleSetting("ANCHO_IMAGENES").toInt();
	int familias_filas = ModuleInstance->getModuleSetting("FILAS_FAMILIAS").toInt();
	int familias_columnas = ModuleInstance->getModuleSetting("COLUMNAS_FAMILIAS").toInt();
	int articulos_filas = ModuleInstance->getModuleSetting("FILAS_ARTICULOS").toInt();
	int articulos_columnas = ModuleInstance->getModuleSetting("COLUMNAS_ARTICULOS").toInt();
	pFamiliasGallery = new FamiliasGallery( pSplitterRight, pRecFamilia,
	// El ancho de las imágenes de las familias se calcula según el ancho de las imágenes de los artículos y el número de columnas de los artículos
				( (ancho_imagenes + TPV_GALLERY_PADDING_RIGHT )* articulos_columnas )
					/ (familias_columnas) - TPV_GALLERY_PADDING_RIGHT,
				alto_imagenes, familias_columnas, familias_filas );
	connect( pFamiliasGallery, SIGNAL( imageClicked( QGraphicsItem *) ), this, SLOT( slotImageFamilia( QGraphicsItem *) ) );
	pFamiliasGallery->showCheckedBoard( true, Qt::green );
	pSplitterRight->addWidget( pFamiliasGallery );
	pFamiliasGallery->setMinimumHeight( pFamiliasGallery->getSizeImages().height() * familias_filas + 1 );
	pFamiliasGallery->setMaximumHeight( pFamiliasGallery->getSizeImages().height() * familias_filas + 1);

	if( mModalidad & ModFilaFavoritos ) {
		pFavoritosGallery = new FavoritosGallery( pSplitterRight, pRecArticulo,
			ModuleInstance->getModuleSetting("ANCHO_IMAGENES").toInt(),
			alto_imagenes, articulos_columnas, 1 );
		connect( pFavoritosGallery, SIGNAL( imageClicked( QGraphicsItem *) ), this, SLOT( slotImageArticulo( QGraphicsItem *) ) );
		pFavoritosGallery->showCheckedBoard( true, Qt::lightGray );
		pSplitterRight->addWidget( pFavoritosGallery );
		pFavoritosGallery->setMinimumHeight( pFavoritosGallery->getSizeImages().height() + 2);
		pFavoritosGallery->setMaximumHeight( pFavoritosGallery->getSizeImages().height() + 2);
		articulos_filas--;
	}
	pArticulosGallery = new ArticulosGallery( pSplitterRight, pRecArticulo,
				ModuleInstance->getModuleSetting("ANCHO_IMAGENES").toInt(),
				alto_imagenes, articulos_columnas, articulos_filas );

	connect( pArticulosGallery, SIGNAL( imageClicked( QGraphicsItem *) ), this, SLOT( slotImageArticulo( QGraphicsItem *) ) );
	pArticulosGallery->showCheckedBoard( true, Qt::lightGray );
	pSplitterRight->addWidget( pArticulosGallery );
	pArticulosGallery->setMinimumHeight( -1 + pArticulosGallery->getSizeImages().height() * articulos_filas + 1 );
	pArticulosGallery->setMaximumHeight( +1 + pArticulosGallery->getSizeImages().height() * articulos_filas + 1 );

	pSplitterRight->setMinimumWidth( -1 + pArticulosGallery->getSizeImages().width() * articulos_columnas + TPV_GALLERY_PADDING_RIGHT );
	pSplitterRight->setMaximumWidth( +1 + pArticulosGallery->getSizeImages().width() * articulos_columnas + TPV_GALLERY_PADDING_RIGHT );

	pSplitterLeft->addWidget(pSplitterRight);

	frameSplittersLayout->addWidget(pSplitterLeft);

	QGridLayout *botonesLayout = new QGridLayout();

	pushClear = new TPVButton( _("Limpiar" ), pControlsFrame );
	connect( pushClear, SIGNAL( clicked() ), this, SLOT( slotClear() ) );
	pushSave = new TPVButton( _("Cerrar\ny cobrar" ), pControlsFrame );
	connect( pushSave, SIGNAL( clicked() ), this, SLOT( slotSave() ) );
	if( isCajon() ) {
		pushOpenDrawer = new TPVButton(_("Abrir\ncajón"), pControlsFrame );
		connect( pushOpenDrawer, SIGNAL( clicked() ), this, SLOT( slotOpenDrawer() ) );
	}
	pushDeleteLine = new TPVButton( _("Borrar\nlínea"), pControlsFrame );
	connect( pushDeleteLine, SIGNAL( clicked() ), this, SLOT( slotDeleteLine() ) );
	pushPrintTicket = new TPVButton( _("Imprimir\nTicket"), pControlsFrame );
	connect( pushPrintTicket, SIGNAL( clicked() ), this, SLOT( slotPrintTicket() ) );
	pushChooseAgente = new TPVButton( _(mVendedoraName), pControlsFrame );
	connect( pushChooseAgente, SIGNAL( clicked() ), this, SLOT( slotChooseAgente() ) );
#ifdef HAVE_SOCIASMODULE
	if( isSocias() )
		pushChooseCliente = new TPVButton( DBAPP->getTableDescSingular( "MIEMBRO"), pControlsFrame);
	else
#endif
		pushChooseCliente = new TPVButton( DBAPP->getTableDescSingular( "CLIENTE"), pControlsFrame);
	connect( pushChooseCliente, SIGNAL( clicked() ), this, SLOT( slotChooseCliente() ) );
	pushEditCantidad = new TPVButton( _("Modificar\ncantidad" ), pControlsFrame );
	connect( pushEditCantidad, SIGNAL( clicked() ), this, SLOT( slotEditCantidadOPVP() ) );
	pushEditPVP = new TPVButton( _("Modificar\nPVP" ), pControlsFrame );
	connect( pushEditPVP, SIGNAL( clicked() ), this, SLOT( slotEditCantidadOPVP() ) );
	pushGuardar = new TPVButton( _("Guardar"), pControlsFrame);
	connect( pushGuardar, SIGNAL( clicked() ), this, SLOT( slotGuardar() ) );
	pushHistorico = new TPVButton( _("Histórico"), pControlsFrame);
	connect( pushHistorico, SIGNAL( clicked() ), this, SLOT( slotHistorico() ) );
	if( isBar() ) {
	  pushElegirMesa = new TPVButton( _("Mesas"), pControlsFrame);
	  connect( pushElegirMesa, SIGNAL( clicked() ), this, SLOT( slotElegirMesa() ) );
	  pushCambiarDeMesa = new TPVButton( _("Mover\nde mesa"), pControlsFrame);
	  connect( pushCambiarDeMesa, SIGNAL( clicked() ), this, SLOT( slotCambiarDeMesa() ) );
	} else {
	  pushElegirCuenta = new TPVButton( _("Cuentas"), pControlsFrame);
	  connect( pushElegirCuenta, SIGNAL( clicked() ), this, SLOT( slotElegirCuenta() ) );
	}
	if( isRondas() ) {
		pushNuevaRonda = new TPVButton( _("Nueva\nRonda"), pControlsFrame);
		connect( pushNuevaRonda, SIGNAL( clicked() ), this, SLOT( slotNuevaRonda() ) );
	}
	if( isBalanza() ) {
		pushBalanza = new TPVButton( _("Balanza"), pControlsFrame);
		connect( pushBalanza, SIGNAL( clicked() ), this, SLOT( slotBalanza() ) );
	}
	pushFijarPrecioArticulo = new TPVButton( _("Fijar\nPrecio"), pControlsFrame);
	connect( pushFijarPrecioArticulo, SIGNAL( clicked() ), this, SLOT( slotFijarPrecioArticulo() ) );
	pushAddDescuentoRecargo = new TPVButton( _("Añadir\nDto/recargo"), pControlsFrame);
	connect( pushAddDescuentoRecargo, SIGNAL( clicked() ), this, SLOT( slotAddDescuentoRecargo() ) );

	botonesLayout->addWidget(pushSave, 0, 0, 1, -1 );
	botonesLayout->addWidget(pushGuardar, 1, 0, 1, -1);
	botonesLayout->addWidget(pushPrintTicket, 2, 0 );
	botonesLayout->addWidget(pushHistorico, 2, 1);
	botonesLayout->addWidget(pushDeleteLine, 3, 0 );
	botonesLayout->addWidget(pushClear, 3, 1 );
	botonesLayout->addWidget(pushChooseAgente, 4, 0 );
	botonesLayout->addWidget(pushChooseCliente, 4, 1);
	int nbutton = 10;
	if( isBar() ) {
		botonesLayout->addWidget(pushElegirMesa, nbutton / 2, 0);
		botonesLayout->addWidget(pushCambiarDeMesa, nbutton / 2, 1);
		nbutton += 2;
	} else {
		botonesLayout->addWidget(pushElegirCuenta, nbutton / 2, 0);
		++nbutton;
	}
	if( isBalanza() ) {
		botonesLayout->addWidget(pushBalanza, nbutton / 2, nbutton % 2 );
		++nbutton;
	}
	if( isCajon() ) {
		botonesLayout->addWidget(pushOpenDrawer, nbutton / 2, nbutton % 2 );
		++nbutton;
	}
	if( isRondas() ) {
		botonesLayout->addWidget(pushNuevaRonda, nbutton / 2, nbutton % 2 );
		++nbutton;
	}
	botonesLayout->addWidget(pushFijarPrecioArticulo, nbutton / 2, nbutton % 2 );
	++nbutton;
	botonesLayout->addWidget(pushAddDescuentoRecargo, nbutton / 2, nbutton % 2 );
	++nbutton;

	frameSplittersLayout->addLayout( botonesLayout );

	pCalculator->getGridLayout()->addWidget( pushEditPVP, 3, 6 );
	pCalculator->getGridLayout()->addWidget( pushEditCantidad, 4, 6 );

	pControlsLayout->addWidget(frameSplitters);
 	pControlsLayout->setContentsMargins(0, 0, 0, 0);

	pushAccept->setVisible( false );
	pushCancel->setVisible( false );


	pDefRecAgente->read( "CODIGO=" + pDefRecAgente->getConnection()->toSQL(
		ModuleInstance->getModuleSetting("CODIGO_AGENTE" ).toInt() ) );
	pDefRecCliente->read( pRecSesionTPV->getValue("CLIENTE_ID" ).toUInt() );
	mTipoDocID = pRecArticulo->getConnection()->selectInt(
		"SELECT ID FROM TIPODOC WHERE CODIGO=" + pRecArticulo->getConnection()->toSQL(
			ModuleInstance->getModuleSetting("CODIGO_TIPODOC" ).toInt() ) );
	if( mTipoDocID == 0 ) {
		msgError(this, _("No se ha definido un tipo de documento válido en la configuración.\nNo puedo continuar.") );
		throw std::runtime_error(_("No se ha definido un tipo de documento válido en la configuración."));
	}
	mFormaPagoID = pRecArticulo->getConnection()->selectInt(
		"SELECT ID FROM FORMAPAGO WHERE CODIGO=" + pRecArticulo->getConnection()->toSQL(
			ModuleInstance->getModuleSetting("CODIGO_FORMAPAGO" ).toInt() ) );

	if( isBar() ) {
		if( mDictMesas.size() == 1 && mDictMesas.getKey(0) == Xtring(NOMBRE_MESA_COMUN).upper() )
			DBAPP->showStickyOSD( _("Sesión TPV"), _("No se ha encontrado definición de mesas, trabajando con la mesa común") );
		// Recuperar la sesión anterior
		restoreEstado();
		if( mDictMesas.find( NOMBRE_MESA_COMUN ) != mDictMesas.end() )
			setCurrentMesa( NOMBRE_MESA_COMUN );
		else
			setCurrentMesa( mDictMesas.getKey(0) );
	} else {
		restoreEstado();
		setCurrentMesa( mDictMesas.getKey(0) );
	}

	pTableTicket->setModel( pTicketItemsModel );
	if( pTicketItemsModel->rowCount() > 0 )
		setCurrentRow( pTicketItemsModel->rowCount() - 1);
	pFocusWidget = pSearchArticulo->getEditCode();
	mMainWindowWasMaximized = DBAPP->getMainWindow() ? DBAPP->getMainWindow()->isMaximized() : false;
	pTimer = new QTimer( this );
	connect( pTimer, SIGNAL( timeout() ), this, SLOT( slotTimerShot() ) );
	pTimer->start( 2 * 60 * 1000 ); // Cada 2 minutos
	DBAPP->resetCursor();
}

FrmTpvScreen::~FrmTpvScreen()
{
	for( Dictionary<MesaInfo>::reverse_iterator it = mDictMesas.rbegin();
		it != mDictMesas.rend(); ++it ) {
		if( it->second.albaranventa )
			delete it->second.albaranventa;
		delete it->second.mesaname;
#ifdef HAVE_SOCIASMODULE
		if( it->second.pRecMiembro )
			delete it->second.pRecMiembro;
#endif
	}
	delete pFrmElegirMesa;
	delete pDefRecCliente;
	delete pDefRecAgente;
	delete pRecFamilia;
	delete pRecArticulo;
	delete pRecSesionTPV;
	if( pPuertoBalanza ) {
		pPuertoBalanza->close();
		delete pPuertoBalanza;
	}
}

/**
 * @brief Inicializa el albarán de la mesa actual, después de guardar o al limpiarlo manualmente.
 *
 * 	El agente se conserva, pues se supone que la mesa la lleva el mismo
 *
 * @return void
 **/
void FrmTpvScreen::clear( factu::RecAlbaranVenta *albaranventa )
{
	dbRecordID agente_id = albaranventa->getValue( "AGENTE_ID" ).toUInt();
	albaranventa->setNew( true );
	albaranventa->clear( true );
	albaranventa->setValue( "IVADETALLADO", factu::FldIVADetallado::incluido );
	albaranventa->setValue( "SESIONTPV_ID", pRecSesionTPV->getRecordID() );
 	albaranventa->setValue( "AGENTE_ID", agente_id );
	albaranventa->setValue( "CLIENTE_ID", pDefRecCliente->getRecordID() );
	albaranventa->getRecCliente()->copyRecord( pDefRecCliente );
	albaranventa->setValue( "TIPODOC_ID", mTipoDocID );
	albaranventa->setValue( "FORMAPAGO_ID", mFormaPagoID );
// 	slotGuardar(); // Así nos evitamos tener que pedir un agente
}

void FrmTpvScreen::gather()
{
	factu::RecAlbaranVenta *albaranventa = getCurrentMesa().albaranventa;
	if( albaranventa->getValue( "FECHA" ).toDate().isNull() ) {
		albaranventa->setValue( "FECHA", Date::currentDate() );
		albaranventa->setValue( "HORA", Time::currentTime() );
		albaranventa->setValue( "MESA", getMesaName() );
	}
	if( albaranventa->getValue( "AGENTE_ID" ).toInt() == 0 ) {
		albaranventa->setValue( "AGENTE_ID", pDefRecAgente->getRecordID() );
		albaranventa->getRecAgente()->copyRecord( pDefRecAgente );
	}
	albaranventa->setNextNumero( true );
}

bool FrmTpvScreen::save()
{
	bool ret = false;
	factu::RecAlbaranVenta *albaranventa = getCurrentMesa().albaranventa;
	if( albaranventa->getListAlbaranVentaDet()->size() == 0 ) {
		if( !msgYesNo( this,
					   _("El ticket está vacío. ¿Estás segura de que quieres guardarlo?") ) ) {
			pSearchArticulo->getEditCode()->setFocus();
			return false;
		}
	}
	gather();
	Money total = albaranventa->getValue( "TOTAL" ).toMoney();
	FrmTicketCambio *frmcambio = new FrmTicketCambio( this, total );
	frmcambio->showModalFor( this, true, true );
	if( !frmcambio->wasCancelled() ) {
		openDrawer();
		Money entrega = frmcambio->getEntrega();
		if( entrega < total ) {
			DBAPP->showOSD( _("No se ha pagado el total del ticket"), _("Elige una forma de pago") );
			pagos::RecFormaPago *formapago = static_cast<pagos::RecFormaPago *>(DBAPP->createRecord("FORMAPAGO"));
			dbRecordID formapago_id = DBAPP->choose( 0, formapago, 0, dbApplication::editNone, this );
			if( formapago_id == 0 ) {
				DBAPP->showOSD( _("No se ha pagado el total del ticket"), _("Se vuelve al TPV") );
				return false;
			} else {
				albaranventa->setValue( "ENTREGA", entrega );
				albaranventa->setValue( "RESTO", total - entrega );
				albaranventa->setValue( "FORMAPAGO_ID", formapago->getRecordID() );
				albaranventa->getRecFormaPago()->copyRecord( formapago );
			}
			if( frmcambio->mustPrint() )
				printToPrinter( entrega, 0, total-entrega, formapago->getValue("NOMBRE").toString());
			delete formapago;
		} else {
			albaranventa->setValue( "ENTREGA", total );
			albaranventa->setValue( "RESTO", Money() );
			if( frmcambio->mustPrint() )
				printToPrinter( entrega, entrega - total );
		}
		try {
			if( albaranventa->save(true, true ) ) {
				Money cambio( entrega - total );
				DBAPP->showOSD( Xtring::printf( _("CAMBIO: %s"), cambio.toString( DBAPP->getRegConfig() ).c_str() ),
								Xtring::printf( _("ENTREGADO: %s\nTicket %s guardado correctamente"),
												entrega.toString( DBAPP->getRegConfig() ).c_str(),
												albaranventa->toString( TOSTRING_CODE_AND_DESC ).c_str() ), 5000 );
				clearCurrentMesa();
				actualizaTotal();
				ret = true;
			}
		} catch( dbError &e ) {
			msgError( this, Xtring( _("Ha ocurrido un error al grabar el ticket\n") ) + e.what() );
		}
	}
	delete frmcambio;
	slotGuardar();
	return ret;
}

MesaInfo &FrmTpvScreen::setCurrentMesa(const Xtring &mesaname)
{
	MesaInfo &m = pTicketItemsModel->setCurrentMesa( mesaname );
	actualizaTotal();
	updateAgenteClienteLabel( m );
	updateTitle();
	return m;
}

void FrmTpvScreen::clearCurrentMesa()
{
	MesaInfo &m = getCurrentMesa();
	clear( m.albaranventa );
	m.ronda = -1;
#ifdef HAVE_SOCIASMODULE
	m.pRecMiembro->clear( false );
#endif
	updateAgenteClienteLabel( m );
	pTicketItemsModel->reset();
}

int FrmTpvScreen::getCurrentRow() const
{
	int row = -1;
	const QModelIndexList &selecteds = pTableTicket->selectionModel()->selection().indexes();
	if( selecteds.count() != 0 ) {
		row = selecteds.at(0).row();
		factu::RecAlbaranVenta *albaranventa = getCurrentMesa().albaranventa;
		if( row < 0 || row >= (int)albaranventa->getListAlbaranVentaDet()->size() )
			row = -1;
	}
	return row;
}

void FrmTpvScreen::setCurrentRow(uint row)
{
	if( (int)row < pTicketItemsModel->rowCount() && (int) row > 0)
		pTableTicket->selectionModel()->setCurrentIndex(pTicketItemsModel->index( row, 0),
			QItemSelectionModel::Select | QItemSelectionModel::Rows );
}

void FrmTpvScreen::slotEditCantidadOPVP()
{
	factu::RecAlbaranVenta *albaranventa = getCurrentMesa().albaranventa;
	int row = getCurrentRow(); // albaranventa->getListAlbaranVentaDet()->size()-1;
	if( row == -1 && albaranventa->getListAlbaranVentaDet()->size() > 0 )
		row = albaranventa->getListAlbaranVentaDet()->size() - 1;
	if( row >= 0 ) {
		int column;
		if( sender() == pushEditCantidad ) {
			column = 0;
		} else {
			column = 1;
		}
		double value = pCalculator->getValue();
		if( value == 0.0 ) {
			msgError( this, "Introduce el nuevo valor en la calculadora" );
			return;
		} else {
			pCalculator->clearAll();
		}
		factu::RecAlbaranVentaDet *avdet = static_cast<factu::RecAlbaranVentaDet *>(albaranventa->getListAlbaranVentaDet()->getRecord( row ));
		switch( column ) {
			case 0:
				avdet->setValue( "CANTIDAD", value );
				break;
			case 1:
				avdet->setValue( "PVP", Money(value) );
				avdet->setValue( "PVPSINIVA", Money(avdet->getRecArticulo()->menosIVA( value ),4 ) );
				break;
		}
		avdet->setValue( "IMPORTECONIVA",
							Money(avdet->getValue( "CANTIDAD" ).toDouble() * avdet->getValue( "PVP" ).toDouble() ) );
		avdet->setValue( "IMPORTE",
							Money(avdet->getValue( "CANTIDAD" ).toDouble() * avdet->getValue( "PVPSINIVA" ).toDouble() ) );
		actualizaTotal();
		pTicketItemsModel->refresh(getCurrentRow());
	}
}

void FrmTpvScreen::slotFijarPrecioArticulo()
{
	int row = getCurrentRow(); // albaranventa->getListAlbaranVentaDet()->size()-1;
	if( row == -1 ) {
		return;
	}
	factu::RecAlbaranVenta *albaranventa = getCurrentMesa().albaranventa;
	factu::RecAlbaranVentaDet *avdet = static_cast<factu::RecAlbaranVentaDet *>(albaranventa->getListAlbaranVentaDet()->getRecord( row ));
	double pvp = avdet->getValue("PVP").toDouble();
	if( pvp != 0.0 ) {
		factu::FactuModule::editPVPsArticulo(this, avdet->getRecArticulo(), albaranventa->getRecCliente(), pvp, false);
	}
}

void FrmTpvScreen::slotOpenDrawer()
{
	openDrawer();
}

// Mover a tpvmodule
void FrmTpvScreen::openDrawer()
{
	FileUtils::execProcess( ModuleInstance->getModuleSetting("COMMAND.OPENDRAWER").toString() );
}

void FrmTpvScreen::slotSave()
{
	save();
}

void FrmTpvScreen::slotClear()
{
	if( msgYesNo(this, _("¿Estás segura de borrar los datos de esta mesa?") ) ) {
		clearCurrentMesa();
	}
}

void FrmTpvScreen::slotDeleteLine()
{
	int row = getCurrentRow();
	if( row >= 0 ) {
		pTicketItemsModel->delDetalle( row );
		actualizaTotal();
	}
}

void FrmTpvScreen::slotPrintTicket()
{
	printToPrinter( Money(), Money() );
}

void FrmTpvScreen::slotGuardar()
{
	if( isBar() ) {
		if( mDictMesas.find( NOMBRE_MESA_COMUN ) != mDictMesas.end() )
			setCurrentMesa( NOMBRE_MESA_COMUN );
		else
			setCurrentMesa( mDictMesas.getKey( 0 ) );
	} else {
		setCurrentMesa( mDictMesas.getKey( 0 ) );
	}
	pTicketItemsModel->reset();
	setCurrentRow( pTicketItemsModel->rowCount() - 1);
	updateAgenteClienteLabel( getCurrentMesa() );
	actualizaTotal(); // updates the title there
}

void FrmTpvScreen::slotNuevaRonda()
{
	MesaInfo &m = getCurrentMesa();
	m.ronda = m.albaranventa->getListAlbaranVentaDet()->size() - 1;
	if( m.ronda != -1 ) {
		DBAPP->showOSD( Xtring::printf("Nueva ronda en la mesa %s",
							pTicketItemsModel->getCurrentMesaName().mid(5).c_str() ),
						Xtring::printf("A partir de la línea %d", m.ronda + 1 ) );
		updateAgenteClienteLabel( m );
	}
}

void FrmTpvScreen::slotChooseAgente()
{
	dbRecordID elegido = 0L;
	if( (int)pCalculator->getValue() != 0 ) {
		elegido = ModuleInstance->getConnection()->selectInt( "SELECT ID FROM AGENTE WHERE CODIGO="
			+ ModuleInstance->getConnection()->toSQL( (int)pCalculator->getValue() ) );
	}
	pCalculator->clearAll();
	if( elegido == 0L ) {
		pFrmChooseAgente = new FrmChooseByImage( 0 );
		pFrmChooseAgente->setTitle(Xtring("Elegir ") + mVendedoraName);
		int images = pFrmChooseAgente->addImages( pRecArticulo->getConnection(),
			"SELECT AGENTE.ID, CONTACTOIMAGEN.IMAGEN, AGENTE.CODIGO, AGENTE.RAZONSOCIAL"
			" FROM AGENTE INNER JOIN CONTACTO ON AGENTE.CONTACTO_ID=CONTACTO.ID"
			"  LEFT JOIN CONTACTOIMAGEN ON CONTACTOIMAGEN.ID=CONTACTO.CONTACTOIMAGEN_ID"
			" WHERE AGENTE.PARATPV=1");
		if( images == 0 ) {
			delete pFrmChooseAgente;
			return;
		}
		if( images == 1 )
			elegido = pFrmChooseAgente->getRecordID();
		else {
			pFrmChooseAgente->showModalFor( this, true, true );
			if( !pFrmChooseAgente->wasCancelled() )
				elegido = pFrmChooseAgente->getRecordID();
		}
	}
	if( elegido != 0L ) {
		getCurrentMesa().albaranventa->getRecAgente()->read( elegido );
		getCurrentMesa().albaranventa->setValue( "AGENTE_ID", elegido );
		updateAgenteClienteLabel( getCurrentMesa() );
		DBAPP->showStickyOSD( _("Sesión TPV"), Xtring::printf( _("Se ha elegido la %s %s"),
			mVendedoraName.c_str(),
			getCurrentMesa().albaranventa->getRecAgente()->getValue( "RAZONSOCIAL" ).toString().c_str() ) );
	}
}

int FrmTpvScreen::buscaCliente(const Xtring& nombrecliente)
{
	int elegido = 0;
	FrmChooseByImage *pFrmChooseCliente = new FrmChooseByImage( this );
	pFrmChooseCliente->setTitle("Elegir cliente");
	int images = pFrmChooseCliente->addImages( pRecFamilia->getConnection(),
		"SELECT CLIENTE.ID, CONTACTOIMAGEN.IMAGEN, CLIENTE.CODIGO, CLIENTE.RAZONSOCIAL"
		" FROM CLIENTE INNER JOIN CONTACTO ON CLIENTE.CONTACTO_ID=CONTACTO.ID"
		"  LEFT JOIN CONTACTOIMAGEN ON CONTACTOIMAGEN.ID=CONTACTO.CONTACTOIMAGEN_ID"
		" WHERE " + pRecFamilia->getConnection()->toSQLLike("CLIENTE.RAZONSOCIAL", nombrecliente)
			+ " OR " + pRecFamilia->getConnection()->toSQLLike("CONTACTO.NOMBRE", nombrecliente)
			+ " OR " + pRecFamilia->getConnection()->toSQLLike("CONTACTO.NOMBREALT", nombrecliente) );
	if( images == 1 )
		elegido = pFrmChooseCliente->getCode().toInt();
	else if( images != 0 ) {
		pFrmChooseCliente->showModalFor( this, true, true );
		if( !pFrmChooseCliente->wasCancelled() )
			elegido = pFrmChooseCliente->getCode().toInt();
	}
	delete pFrmChooseCliente;
	return elegido;
}

void FrmTpvScreen::slotChooseCliente()
{
	MesaInfo &m = getCurrentMesa();
	int codigo_elegido = (int)pCalculator->getValue();
	pCalculator->clearAll();
	if( codigo_elegido == 0 ) {
		KeyBoard *kb;
#ifdef HAVE_SOCIASMODULE
		if( isSocias() )
			kb = new KeyBoard( _("Elegir la socia"), this );
		else
#endif
			kb = new KeyBoard( _("Elegir la cliente"), this );
		kb->exec();
		if( kb->result() == QDialog::Accepted ) {
			bool ok;
			codigo_elegido = kb->getValue().toInt( &ok );
			if( !ok ) { /// TODO Buscar por nombre
				codigo_elegido = buscaCliente( kb->getValue() );
			}
		}
	}
	if( codigo_elegido != 0 ) {
		factu::RecAlbaranVenta *albaranventa = m.albaranventa;
		albaranventa->setValue( "CLIENTE_ID", 0 );
#ifdef HAVE_SOCIASMODULE
		if( isSocias() ) {
			if( m.pRecMiembro->read( "NUMEROSOCIA=" + m.pRecMiembro->getConnection()->toSQL( codigo_elegido )
				+ " AND PROYECTO_ID=" + m.pRecMiembro->getConnection()->toSQL( mProyectoID ) ) )
				albaranventa->setValue( "CLIENTE_ID", m.pRecMiembro->getRecCliente()->getRecordID() );
		} else
#endif
			if( albaranventa->getRecCliente()->read(
				"CODIGO=" + pDefRecCliente->getConnection()->toSQL( codigo_elegido ) ) ) {
				albaranventa->setValue( "CLIENTE_ID", albaranventa->getRecCliente()->getRecordID() );
			}
		if( albaranventa->getValue( "CLIENTE_ID" ).toUInt() == 0 ) {
			msgError( this, _("Cliente no encontrado") );
		}
		updateAgenteClienteLabel( m );
	}
}

void FrmTpvScreen::slotElegirMesa()
{
 	pFrmElegirMesa->showMaximized();
	pFrmElegirMesa->drawMesasOcupadas( pDefRecAgente->getRecordID(), pDefRecCliente->getRecordID() );
	pFrmElegirMesa->showModalFor( this, false, true );
	if( !pFrmElegirMesa->wasCancelled() ) {
		MesaInfo &m = setCurrentMesa( pFrmElegirMesa->getChoosen() );
		factu::RecAlbaranVenta *albven = m.albaranventa;
		if( albven->getRecAgente()->getRecordID() == 0 )
			slotChooseAgente();
		pTicketItemsModel->reset();
		setCurrentRow( pTicketItemsModel->rowCount() - 1 );
		actualizaTotal();
		updateAgenteClienteLabel( m );
	}
}

int FrmTpvScreen::fillCuentas(DictMesas &mesas, int ncuentas)
{
	for( int nc = 0; nc < ncuentas; ++nc ) {
		Xtring element_id = "Cuenta " + Xtring::number(nc+1);
		MesaInfo m;
		m.mesaname = strdup(element_id.c_str());
		m.albaranventa = 0;
		m.coords = QRect();
		m.ronda = -1;
#ifdef HAVE_SOCIASMODULE
		m.pRecMiembro = 0;
#endif
		mesas.insert( element_id, m );
	}
	return mesas.size();
}



void FrmTpvScreen::slotElegirCuenta()
{
	class FrmElegirCuenta: public TpvFrmCustom
	{
	public:
		FrmElegirCuenta(const DictMesas &mesas, QWidget *parent=0, WidgetFlags fl = 0 )
				: TpvFrmCustom( parent, "FrmElegirCuenta", fl ) {
			setTitle( _("Elegir cuenta") );
			for( uint nc = 0; nc < mesas.size(); ++nc ) {
				addButton( 0, mesas.seq_at(nc).mesaname );
			}
			pushCancel->setVisible(true);
			pushAccept->setVisible(false);
		}
		virtual void validate_input( QWidget *sender, bool * ) {
			QPushButton *button = static_cast<QPushButton *>(sender);
			mChoosen = fromGUI(button->text());
			accept();
		}
		Xtring getChoosen() const { return mChoosen; }
	private:
		Xtring mChoosen;
	};

	if( mDictMesas.size() == 0 ) {
		msgError(this, "No se han definido las cuentas para esta sesión de TPV");
	} else {
		FrmElegirCuenta *frmElegirCuenta = new FrmElegirCuenta(mDictMesas);
		frmElegirCuenta->showModalFor( this, true, true);
		if( !frmElegirCuenta->wasCancelled() ) {
			MesaInfo &m = setCurrentMesa( frmElegirCuenta->getChoosen() );
			factu::RecAlbaranVenta *albven = m.albaranventa;
			if( albven->getRecAgente()->getRecordID() == 0 )
				slotChooseAgente();
			pTicketItemsModel->reset();
			setCurrentRow( pTicketItemsModel->rowCount() - 1 );
			actualizaTotal();
			updateAgenteClienteLabel( m );
		}
		delete frmElegirCuenta;
	}
}

void FrmTpvScreen::slotHistorico()
{
	MesaInfo &m = getCurrentMesa();
	factu::RecAlbaranVenta *albaranventa = m.albaranventa;
	if( albaranventa->getListAlbaranVentaDet()->size() ) {
		msgError( this, "La mesa actual está ocupada, por favor, elige una mesa vacía.");
		return;
	}
	factu::RecAlbaranVenta *albaran_historico = static_cast<factu::RecAlbaranVenta *>(DBAPP->createRecord( "ALBARANVENTA" ));
	dbViewDefinitionDict albaranviews;
	DBAPP->getDatabase()->getViewsByName( "ALBARANVENTA.PARA_TPV", albaranviews);
	dbRecordDataModel *albarandm =
		new dbRecordDataModel(albaran_historico, albaranviews,
							  "SESIONTPV_ID=" + albaran_historico->getConnection()->toSQL( pRecSesionTPV->getRecordID()));
	dbRecordID av_id = DBAPP->choose( 0, albaran_historico, albarandm, dbApplication::editNone, this );
	if( av_id ) {
		albaranventa->copyRecord( albaran_historico );
		pTicketItemsModel->reset();
		setCurrentRow( pTicketItemsModel->rowCount() - 1 );
		updateAgenteClienteLabel( m );
		actualizaTotal();
	}
}

void FrmTpvScreen::slotCambiarDeMesa()
{
	DBAPP->waitCursor( true );
	DBAPP->showStickyOSD( _("Sesión TPV"), _("Elige la mesa de destino") );
	pFrmElegirMesa->drawMesasOcupadas( pDefRecAgente->getRecordID(), pDefRecCliente->getRecordID() );
	pFrmElegirMesa->showMaximized();
	DBAPP->resetCursor();
	pFrmElegirMesa->showModalFor( this, false, true );
	DBAPP->waitCursor( true );
	if( !pFrmElegirMesa->wasCancelled() ) {
		Xtring orig_mesa_name = pTicketItemsModel->getCurrentMesaName();
		factu::RecAlbaranVenta *albaranorigen = getCurrentMesa().albaranventa;
		 // Para que se cree si no está creado aún
		factu::RecAlbaranVenta *albarandest = setCurrentMesa( pFrmElegirMesa->getChoosen() ).albaranventa;
		setCurrentMesa( orig_mesa_name );
		if( albarandest->getListAlbaranVentaDet()->size() != 0 ) {
			msgError( this, Xtring::printf(_("La mesa '%s' está siendo servida"), pFrmElegirMesa->getChoosen().c_str() ) );
		} else {
			albarandest->copyRecord( albaranorigen, true );
			mDictMesas.at(pFrmElegirMesa->getChoosen()).ronda = pTicketItemsModel->getRonda();
			clearCurrentMesa();
			updateAgenteClienteLabel( setCurrentMesa( pFrmElegirMesa->getChoosen() ) );
			DBAPP->showOSD( "Mesa movida a " + pFrmElegirMesa->getChoosen(), Xtring::null );
		}
	} else
		DBAPP->hideOSD();
	DBAPP->resetCursor();
}

bool FrmTpvScreen::canClose()
{
	for( Dictionary<MesaInfo>::iterator it = mDictMesas.begin();
		it != mDictMesas.end(); ++it ) {
		if( it->second.albaranventa && it->second.albaranventa->getListAlbaranVentaDet()->size() ) {
			saveEstado();
			if( isBar() ) {
				DBAPP->showOSD( _("Se han guardado las mesas servidas en la sesión"), Xtring::null );
			} else {
				DBAPP->showOSD(_("Se han guardado las cuentas en la sesión"), Xtring::null );
			}
			return true;
		}
	}
	pRecSesionTPV->setValue( "ESTADOMESAS", Xtring::null );
	pRecSesionTPV->save(false, false );
	return true;
}


void FrmTpvScreen::updateTitle()
{
	setTitle( Xtring::printf( _("%s: %s. Fecha: %s, Total: %s"),
			  isBar() ? "Mesa" : "Cuenta",
			  getMesaName().c_str(),
			  DateTime::currentDateTime().toString().c_str(),
			  getCurrentMesa().albaranventa->getValue("TOTAL").toMoney().toString().c_str() ) );
}

Xtring FrmTpvScreen::getMesaName() const
{
	Xtring mesa = pTicketItemsModel->getCurrentMesaName();
	if( isBar() ) {
		mesa.replace("MESA_", "").replace("_", " ");
	}
	return mesa;
}

void FrmTpvScreen::updateAgenteClienteLabel( const MesaInfo &mesa )
{
	const factu::RecAlbaranVenta *albaran = mesa.albaranventa;
	pLabelAgente->setText( toGUI( Xtring::printf( "%s: (%d) %s",
			/*DBAPP->getTableDescSingular("AGENTE").c_str()*/mVendedoraName.upper().c_str(),
			albaran->getRecAgente()->getValue( "CODIGO" ).toInt(),
			albaran->getRecAgente()->getValue( "RAZONSOCIAL" ).toString().c_str() ) ) );
	pLabelCliente->setText( toGUI( Xtring::printf( "%s: (%d) %s",
			DBAPP->getTableDescSingular("CLIENTE").upper().c_str(),
			albaran->getRecCliente()->getValue( "CODIGO" ).toInt(),
			albaran->getRecCliente()->getValue( "RAZONSOCIAL" ).toString().c_str() ) ) );
	Xtring txtmesa = Xtring::printf("%s: %s", isBar() ? "MESA" : "CUENTA", Xtring(mesa.mesaname).upper().c_str() );
	if( isRondas() ) {
		if( mesa.ronda >= 0 )
			txtmesa += "(" + Xtring::number(mesa.ronda) + ")";
	}
	pLabelMesa->setText( toGUI( txtmesa ) );
}

bool FrmTpvScreen::print(bool toprinter, Money entrega, Money cambio,
						 Money resto, const Xtring &desc_formapago )
{
#ifdef _GONG_DEBUG
	toprinter = false;
#endif
	DBAPP->waitCursor( true );
	MesaInfo m = getCurrentMesa();
	factu::RecAlbaranVenta *albaranventa = m.albaranventa;
	gather();
	Xtring tickets_filename = ModuleInstance->getModuleSetting("TICKET_RTK" ).toString();
	AppReport *report = new AppReport( *DBAPP, ModuleInstance->getConnection() );
	Xtring defines;
	Dictionary<Variant> properties;
#ifdef HAVE_SOCIASMODULE
	if( m.pRecMiembro && m.pRecMiembro->getValue("NUMEROSOCIA").toInt() != 0  ) {
		defines.append("INC_MIEMBRO,");
		properties.insert( "NUMEROSOCIA", m.pRecMiembro->getValue("NUMEROSOCIA").toString() );
	}
#endif
	if( albaranventa->getRecCliente()->getRecordID() != pDefRecCliente->getRecordID() )
		defines.append("INC_CLIENTE,");
	if( ModuleInstance->getModuleSetting( "NOMBRES_MAYUSCULAS" ).toBool() )
		defines.append("UPPERCASE,");
	if( cambio > 0.0 ) {
		defines.append("INC_CAMBIO,");
		properties.insert( "CAMBIO", cambio );
		properties.insert( "ENTREGA", entrega );
	}
	if( resto > 0.0 ) {
		defines.append("INC_RESTO,");
		properties.insert( "ENTREGA", entrega );
		properties.insert( "RESTO", resto );
		properties.insert( "FORMAPAGO", desc_formapago );
	}
	InputdbRecord *alb_input = new InputdbRecord(
		albaranventa, albaranventa->getListAlbaranVentaDet(), *report, "ticket" );
	bool ret = report->readFile( tickets_filename, defines, alb_input );
	if( ret ) {
		report->setDefaultInput( alb_input );
		if( toprinter )
			ret = report->print( RTK_Printer_Without_Dialog, properties,Xtring::null, Xtring::null,
							 DefaultOrientation, false ); // no pedir parámetros
		else
			ret = report->print( RTK_Screen, properties,Xtring::null, Xtring::null,
							 DefaultOrientation, false ); // no pedir parámetros
	} else {
		if ( report->errorsCount() != 0 ) {
			FrmBase::msgError( this, Xtring::printf( _( "En el informe %.100s:\n%s" ),
				tickets_filename.c_str(), report->lastError()->message() ) );
		}
	}
	delete alb_input;
	DBAPP->resetCursor();
	return ret;
}

void FrmTpvScreen::accept()
{
	save();
}

factu::RecAlbaranVentaDet* FrmTpvScreen::findArticuloInDetalles( factu::RecAlbaranVenta *albaranventa,
									factu::RecArticulo* recart, const Xtring &variante, Money pvp,
									int &row, int ronda )
{
	row = ronda + 1;
	if( row > (int)albaranventa->getListAlbaranVentaDet()->size() ) {
		row = -1;
		return 0;
	} else if( row < 0 )
		row = 0;
	for( dbRecordList::const_iterator det = albaranventa->getListAlbaranVentaDet()->begin() + row;
		det != albaranventa->getListAlbaranVentaDet()->end(); row++, ++det ) {
		if( (*det)->getValue( "ARTICULO_ID" ).toUInt() == recart->getRecordID()
			&& (*det)->getValue( "PVP" ) == pvp
			&& (*det)->getValue( "DESCRIPCION" ) == variante ) {
			return static_cast<factu::RecAlbaranVentaDet *>(*det);
		}
	}
	row = -1;
	return 0;
}

void FrmTpvScreen::validate_input(QWidget *sender, bool *is_valid )
{
	FrmCustom::validate_input( sender, is_valid );
	if( sender == pSearchArticulo || sender == pSearchArticulo->getEditCode() ) {
		List<dbRecordID> &articulo_ids = pSearchArticulo->getRecordIDs();
		if( articulo_ids.size() ) {
			MesaInfo m = mDictMesas[ pTicketItemsModel->getCurrentMesaName() ];
			factu::RecAlbaranVenta *albaranventa = m.albaranventa;
			Money pvp;
			for( uint i=0; i<articulo_ids.size(); ++i ) {
				if( !pRecArticulo->read( articulo_ids[i] ) )
					continue;
				Xtring variante;
				if( !pRecArticulo->getValue( "VARIANTE1" ).toString().isEmpty()
					|| !pRecArticulo->getValue( "VARIANTE2" ).toString().isEmpty() ) {
						if( !chooseVariante( pRecArticulo, variante, pvp ) )
							continue;
				}
				bool add = true;
				double cantidad = pCalculator->getValue();
				if( cantidad == 0.0 )
					cantidad = 1.0;
				int which_row = 0;
				double pvpsiniva;
				if( variante.isEmpty() ) {
					if( albaranventa->getRecCliente()->getValue( "TARIFA" ).toInt() != 0 )
						pvp = Money( pRecArticulo->getPVP( albaranventa->getRecCliente()->getValue( "TARIFA" ).toInt() ) );
					if( pvp == 0.0 )
						pvp = pRecArticulo->getValue( "PVPTPV" ).toMoney();
					if( pvp == 0.0 )
						pvp = pRecArticulo->getValue( "PVP" ).toMoney();
				}
				double dtop100 = albaranventa->getRecCliente()->getValue( "DTOP100" ).toDouble();
				if( dtop100 != 0.0 )
					pvp = pvp * ( 1 - dtop100 / 100 );
				factu::RecAlbaranVentaDet *det = findArticuloInDetalles(
						albaranventa, pRecArticulo, variante, pvp, which_row, m.ronda );
				if( det ) {
					cantidad = det->getValue( "CANTIDAD" ).toDouble() + cantidad;
					det->setValue( "CANTIDAD", cantidad );
					pTicketItemsModel->refresh(which_row);
					pvp = det->getValue( "PVP" ).toMoney();
					pvpsiniva = det->getValue( "PVPSINIVA" ).toDouble();
					add = false;
				}
				if( add ) {
					det = static_cast<factu::RecAlbaranVentaDet *>(DBAPP->createRecord("ALBARANVENTADET"));
					det->setValue( "CANTIDAD", cantidad );
					pRecArticulo->fixPrecios(); // Por si acaso
					pvpsiniva = pRecArticulo->menosIVA( pvp.toDouble() );
					det->setValue( "PVP", pvp );
					det->setValue( "PVPSINIVA", Money(pvpsiniva,4) );
					det->setValue( "TIPOIVA_ID", pRecArticulo->getValue( "TIPOIVA_ID" ) );
					det->getRecTipoIVA()->copyRecord( pRecArticulo->getRecTipoIVA() );
					det->setValue( "ARTICULO_ID", pRecArticulo->getRecordID() );
					det->setValue( "DESCRIPCION", variante );
					det->getRecArticulo()->copyRecord( pRecArticulo );
					pTicketItemsModel->addDetalle( det );
					which_row = pTicketItemsModel->rowCount()-1;
				}
				det->setValue( "IMPORTE", Money( cantidad * pvpsiniva ) );
				det->setValue( "IMPORTECONIVA", Money( cantidad * pvp.toDouble() ) );
				// Select last line
				pTableTicket->clearSelection();
				pTableTicket->selectRow( which_row );
			}
			pSearchArticulo->getEditCode()->setText( Xtring::null );
			pSearchArticulo->getEditDesc()->setText( Xtring::null );
			pSearchArticulo->getEditCode()->setFocus();
			actualizaTotal();
			pCalculator->clearAll();
			articulo_ids.clear();
		}
		// Si se cancela un artículo, lo añade igualmente
		pSearchArticulo->setFocus();
	}
}

void FrmTpvScreen::actualizaTotal()
{
	factu::RecAlbaranVenta *albaranventa = getCurrentMesa().albaranventa;
	albaranventa->actTotales();
	pLabelTotal->setText( toGUI( albaranventa->getValue( "TOTAL" ).toMoney().toString( DBAPP->getRegConfig() ) ) );
	updateTitle();
}

void FrmTpvScreen::slotImageArticulo( QGraphicsItem *item )
{
	Xtring artcode = fromGUI( item->data(0).toString() );
	if( artcode == "{{NEXT}}" )
		pArticulosGallery->nextPage();
	else if( artcode == "{{PREV}}" )
		pArticulosGallery->prevPage();
	else {
		pSearchArticulo->setValue( artcode );
		validate_input( pSearchArticulo, 0 );
	}
}

void FrmTpvScreen::slotImageFamilia(QGraphicsItem *item )
{
	Xtring familycode = fromGUI( item->data(0).toString() );
	if( familycode == "{{NEXT}}" )
		pFamiliasGallery->nextPage();
	else if( familycode == "{{PREV}}" )
		pFamiliasGallery->prevPage();
	else
		pArticulosGallery->reload( familycode );
}

bool FrmTpvScreen::chooseVariante(factu::RecArticulo* articulo, Xtring &variante, Money &pvp)
{
	class FrmVariante: public TpvFrmCustom
	{
	public:
		FrmVariante(factu::RecArticulo *articulo, Money &pvp, QWidget *parent=0, WidgetFlags fl = 0 )
			: TpvFrmCustom( parent, "FrmVariante", fl ), pRecArticulo(articulo), mPVP(pvp) {
			setTitle( _("Variante") );
			pushNombre = addButton( 0, articulo->getValue( "NOMBRE" ).toString() );
			pushNombre->setMinimumHeight( 100 );
			if( !articulo->getValue( "VARIANTE1" ).toString().isEmpty() ) {
				pushVariante1 = addButton( 0, articulo->getValue( "VARIANTE1" ).toString()
					+ ", " + articulo->getValue( "PVPVARIANTE1" ).toMoney().toString() );
				pushVariante1->setMinimumHeight( 100 );
			}
			if( !articulo->getValue( "VARIANTE2" ).toString().isEmpty() ) {
				pushVariante2 = addButton( 0, articulo->getValue( "VARIANTE2" ).toString()
					+ ", " + articulo->getValue( "PVPVARIANTE2" ).toMoney().toString() );
				pushVariante2->setMinimumHeight( 100 );
			}
			pushCancel->setVisible(true);
			pushAccept->setVisible(false);
		}
		virtual void validate_input( QWidget *sender, bool * ) {
			if( sender == pushNombre ) {
				mChoosen = "";
				mPVP = pRecArticulo->getValue("PVP").toMoney();
			} else if( sender == pushVariante1 ) {
				mChoosen = pRecArticulo->getValue( "VARIANTE1" ).toString();
				mPVP = pRecArticulo->getValue("PVPVARIANTE1").toMoney();
			} else {
				mChoosen = pRecArticulo->getValue( "VARIANTE2" ).toString();
				mPVP = pRecArticulo->getValue("PVPVARIANTE2").toMoney();
			}
			accept();
		}
		Xtring getChoosen() const { return mChoosen; }
	private:
		factu::RecArticulo *pRecArticulo;
		QPushButton *pushNombre, *pushVariante1, *pushVariante2;
		Xtring mChoosen;
		Money &mPVP;
	};
	bool ret;
	FrmVariante *frmvariante = new FrmVariante( articulo, pvp, this );
	frmvariante->showModalFor( this, true, true );
	if( !frmvariante->wasCancelled() ) {
		variante = frmvariante->getChoosen();
		ret = true;
	} else {
		pSearchArticulo->getEditCode()->setText( Xtring::null );
		ret = false;
	}
	delete frmvariante;
	return ret;
}

static const char *smesaini = ">>>>>MESA: ";
static const char *smesafin = "<<<<<MESA: ";

void FrmTpvScreen::saveEstado()
{
	Xtring estado;
	for( DictMesas::const_iterator albit = mDictMesas.begin();
		albit != mDictMesas.end(); ++albit ) {
		if( albit->second.albaranventa && albit->second.albaranventa->getListAlbaranVentaDet()->size() != 0 ) {
			estado += smesaini + albit->first + "\n";
			estado += Xtring::number(albit->second.ronda) + "\n";
#ifdef HAVE_SOCIASMODULE
			estado += Xtring::number(albit->second.pRecMiembro->getRecordID()) + "\n";
#else
			estado += "0\n"; // Just in case...
#endif
			estado += albit->second.albaranventa->toString(TOSTRING_FUGIT);
			estado += smesafin + albit->first + "\n";
		}
	}
	if( !estado.isEmpty() && !mLastEstado.isEmpty() && mLastEstado != estado  ) {
		mLastEstado = estado;
		pRecSesionTPV->setValue( "ESTADOMESAS", estado );
		pRecSesionTPV->save(false, true );
	}
}


void FrmTpvScreen::restoreEstado()
{
	Xtring estado = pRecSesionTPV->getValue( "ESTADOMESAS" ).toString();
	bool hasdata = false;
	if( !estado.isEmpty() ) {
		MesaInfo m;
		const char *pestado = estado.c_str();
		while( pestado ) {
			Xtring mesa;
			pestado += strlen(smesaini);
			while( *pestado && *pestado != '\n' )
				mesa += *pestado++;
			if( !*pestado )
				break;
			pestado++;
			// Leer ronda
			Xtring ronda;
			while( *pestado && *pestado!= '\n' )
				ronda += *pestado++;
			if( !*pestado )
				break;
			// Leer miembro_id
			pestado++;
			Xtring smiembroid;
			while( *pestado && *pestado!= '\n' )
				smiembroid += *pestado++;
			if( !*pestado )
				break;
			pestado++;
			// Buscar hasta fin de mesa
			const char *finmesa = strstr( pestado, smesafin );
			if( !finmesa )
				break;
			Xtring salbaran = Xtring( pestado, finmesa-pestado );
			m = setCurrentMesa( mesa );
			if( m.albaranventa != 0 ) {
				pTicketItemsModel->setRonda( ronda.toInt() );
#ifdef HAVE_SOCIASMODULE
				pTicketItemsModel->getRecMiembro()->read( smiembroid.toUInt() );
#endif
				m.albaranventa->clear( false );
				m.albaranventa->fromString( salbaran, TOSTRING_FUGIT );
				if( m.albaranventa->getListAlbaranVentaDet()->size() != 0 )
					hasdata = true;
				// Para que lea las relaciones
				Xtring cli = m.albaranventa->getValue("CLIENTE.RAZONSOCIAL").toString();
				Xtring age = m.albaranventa->getValue("AGENTE.RAZONSOCIAL").toString();
				m.albaranventa->setValue("SESIONTPV_ID", pRecSesionTPV->getRecordID() );
				m.albaranventa->actTotales();
			}
			pestado = strstr( pestado, smesaini );
		}
	}
	if( hasdata )
		DBAPP->showStickyOSD( "Sesión TPV", "Se ha restaurado la sesión anterior" );
}


/**
 * @brief Añado el tamaño de los splitters a la geometría del frmbase
 *
 * @return Xtring
 **/
Xtring FrmTpvScreen::getGeometry() const
{
	Xtring g = FrmBase::getGeometry();
	QList<int> leftsizes = pSplitterLeft->sizes();
	QList<int> rightsizes = pSplitterRight->sizes();
	g += "|" + Xtring::printf("%d,%d,%d,%d", leftsizes[0], leftsizes[1], rightsizes[0], rightsizes[1] );
	g += "|" + fromGUI( pTableTicket->horizontalHeader()->saveState() );
	return g;
}

void FrmTpvScreen::setGeometry(const Xtring& geometry)
{
	XtringList parts;
	geometry.tokenize( parts, "|" );
	if( parts.size() < 1 ) return;
	FrmBase::setGeometry( parts[0] );
	if( parts.size() < 2 ) return;
	int ls0, ls1, rs0, rs1;
	sscanf( parts[1].c_str(), "%d,%d,%d,%d", &ls0, &ls1, &rs0, &rs1 );
	QList<int>sizes;
	sizes << ls0 << ls1;
	pSplitterLeft->setSizes(sizes);
	sizes.clear();
	sizes << rs0 << rs1;
	pSplitterRight->setSizes(sizes);
	if( parts.size() < 3 ) return;
	pTableTicket->horizontalHeader()->restoreState( toGUIByteArray( parts[2] ) );
}

/**
 * @brief Guarda el estado de las sesiones cada cierto tiempo
 *
 * @return void
 **/
void FrmTpvScreen::slotTimerShot()
{
	saveEstado();
}



TicketItemsModel::TicketItemsModel( FrmTpvScreen *tpvscreen, DictMesas &mesas )
	: QAbstractTableModel( tpvscreen ), mMesas( mesas ), pFrmTpvScreen( tpvscreen )
{
	pAlbVentaDetDefinition = DBAPP->getDatabase()->findTableDefinition( "ALBARANVENTADET" );
	mUpperCase = ModuleInstance->getModuleSetting( "NOMBRES_MAYUSCULAS" ).toBool();
}

MesaInfo &TicketItemsModel::getCurrentMesa() const
{
	MesaInfo &mesa = mMesas.at(mCurrentMesaName);
	if( mesa.albaranventa == 0 ) {
		mesa.albaranventa = static_cast<factu::RecAlbaranVenta *>(DBAPP->createRecord( "ALBARANVENTA" ));
#ifdef HAVE_SOCIASMODULE
		mesa.pRecMiembro = static_cast<socias::RecMiembro *>(DBAPP->createRecord( "MIEMBRO" ));
#endif
		pFrmTpvScreen->clear( mesa.albaranventa );
	}
	return mesa;
}

MesaInfo &TicketItemsModel::setCurrentMesa(const Xtring& current_index)
{
	if( mMesas.find( current_index ) != mMesas.end() ) {
		mCurrentMesaName = current_index;
		return mMesas.at(mCurrentMesaName);
	} else {
		mCurrentMesaName = Xtring(mMesas.seq_at(0).mesaname);
		return mMesas.at(mCurrentMesaName);
		throw std::runtime_error("Mesa " + current_index + " no encontrada" );
	}
}

void TicketItemsModel::setRonda(int ronda)
{
	mMesas.at(mCurrentMesaName).ronda = ronda;
}

int TicketItemsModel::getRonda() const
{
	return mMesas[mCurrentMesaName].ronda;
}

#ifdef HAVE_SOCIASMODULE
socias::RecMiembro *TicketItemsModel::getRecMiembro() const
{
	return mMesas[mCurrentMesaName].pRecMiembro;
}
#endif

int TicketItemsModel::rowCount(const QModelIndex& parent) const
{
	return getCurrentMesa().albaranventa->getListAlbaranVentaDet()->size();
}

int TicketItemsModel::columnCount(const QModelIndex& parent) const
{
	if( parent.isValid() )
		return 0;
	return 4;
}

QVariant TicketItemsModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	else if( role == Qt::TextAlignmentRole ) {
		switch( index.column() ) {
		case nombrearticulo:
			return Qt::AlignAuto;
		default:
			return Qt::AlignRight;
		}
	} else if( role == Qt::DisplayRole ) {
		switch( index.column() ) {
		case cantidad:
			return QVariant( toGUI( getCurrentMesa().albaranventa->getListAlbaranVentaDet()->getRecord( index.row() )->getValue( "CANTIDAD" ).toString() ) );
		case pvp:
			return QVariant( toGUI( getCurrentMesa().albaranventa->getListAlbaranVentaDet()->getRecord( index.row() )->getValue( "PVP" ).toMoney().toString( DBAPP->getRegConfig() ) ) );
		case importeconiva:
			return QVariant( toGUI( getCurrentMesa().albaranventa->getListAlbaranVentaDet()->getRecord( index.row() )->getValue( "IMPORTECONIVA" ).toMoney().toString( DBAPP->getRegConfig() ) ) );
		case nombrearticulo:
			{
				Xtring n;
				if( getCurrentMesa().albaranventa->getListAlbaranVentaDet()->getRecord( index.row() )->getValue( "DESCRIPCION" ).toString().isEmpty() )
					n = getCurrentMesa().albaranventa->getListAlbaranVentaDet()->getRecord( index.row() )->getValue( "ARTICULO.NOMBRE" ).toString();
				else
					n = getCurrentMesa().albaranventa->getListAlbaranVentaDet()->getRecord( index.row() )->getValue( "DESCRIPCION" ).toString();
				if( mUpperCase )
					return toGUI( n.upper() );
				else
					return toGUI( n );
			}
		}
	}
	return QVariant();
}

QVariant TicketItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if( orientation == Qt::Horizontal ) {
		if( role == Qt::DisplayRole ) {
			dbFieldDefinition *fld;
			switch( section ) {
				case cantidad:
					fld = pAlbVentaDetDefinition->findFieldDefinition("CANTIDAD");
					if( fld )
						return toGUI( fld->getCaption() );
					else
						return toGUI(_("Cantidad"));
				case pvp:
					fld = pAlbVentaDetDefinition->findFieldDefinition("PVP");
					if( fld )
						return toGUI( fld->getCaption() );
					else
						return toGUI(_("PVP"));
				case importeconiva:
					fld = pAlbVentaDetDefinition->findFieldDefinition("IMPORTECONIVA");
					if( fld )
						return toGUI( fld->getCaption() );
					else
						return _("Importe");
				case nombrearticulo:
					{
						Xtring desc = DBAPP->getTableDescSingular( "ARTICULO" );
						if( !desc.isEmpty() )
							return toGUI(desc);
						else
							return toGUI(_("Artículo"));
					}
			}
		}
	}
	return QVariant();
}

void TicketItemsModel::addDetalle(factu::RecAlbaranVentaDet* detalle)
{
	int row = getCurrentMesa().albaranventa->getListAlbaranVentaDet()->size();
	beginInsertRows( QModelIndex(), row, row );
	getCurrentMesa().albaranventa->getListAlbaranVentaDet()->addRecord( detalle );
	endInsertRows();
}

void TicketItemsModel::delDetalle(int row)
{
	beginRemoveRows( QModelIndex(), row, row );
	getCurrentMesa().albaranventa->getListAlbaranVentaDet()->removeRecord( row );
	endRemoveRows();
}

void TicketItemsModel::refresh(int row)
{
	emit dataChanged( index(row,0), index(row,3) );
}

void FrmTpvScreen::slotBalanza()
{
	if( !pPuertoBalanza ) {
		pPuertoBalanza = new RS232( ModuleInstance->getModuleSetting("RS232.PUERTO").toString() );
		if( pPuertoBalanza->open() ) {
			pPuertoBalanza->setup(9600);
		} else {
			msgError( this, strerror(errno) );
		}
	}
	pCalculator->setValue( 0.0 );
	int tries = 3;
	while( --tries ) {
		if( pPuertoBalanza->write("$") != 1 ) {
			msgError( this, strerror(errno) );
		}
		usleep(1000);
		Xtring peso = pPuertoBalanza->readLine();
		if( peso.size() ) {
			RegExp peso_re = RegExp(".*\\s+([0-9\\.,]+).*");
			RegExpMatchList l;
			if( peso_re.match(peso.c_str(), l) ) {
				Xtring np = l[1].str();
				pCalculator->setValue( np.toDoubleLocIndep() );
				pPuertoBalanza->readLine(); // Lee cualquier cosa que quede ahí
				pPuertoBalanza->readLine(); // Lee cualquier cosa que quede ahí
				return;
			}
		}
	}
}

class FrmAddDescuentoRecargo: public TpvFrmCustom
{
public:
	FrmAddDescuentoRecargo(QWidget *parent=0, WidgetFlags fl = 0 )
		: TpvFrmCustom( parent, "FrmAddDescuentoRecargo", fl ) {
		setTitle( _("Añadir descuento o recargo") );
		editPorcentaje = addInput(this, "Descuento/Recargo (%)", Variant(10.0) );
		editRedondeo = addInput(this, "Céntimos de redondeo", Variant(5) );
		pushDescuento = addButton( this, "Aplicar descuento" );
		pushRecargo = addButton( this, "Aplicar recargo" );
		pushCancel->setVisible(true);
		pushAccept->setVisible(false);
	}
	virtual void validate_input( QWidget *sender, bool * ) {
		if( sender == pushDescuento || sender == pushRecargo ) {
			mDescuento = fabs(editPorcentaje->toDouble());
			if( sender == pushDescuento ) {
				mDescuento = -mDescuento;
			}
			accept();
		}
	}
	double getDescuento() const { return mDescuento; }
	int getRedondeo() const { return editRedondeo->toInt(); }
private:
	double mDescuento;
	LineEdit *editPorcentaje, *editRedondeo;
	QPushButton *pushDescuento, *pushRecargo;
};


void FrmTpvScreen::slotAddDescuentoRecargo()
{
	FrmAddDescuentoRecargo *frmdto = new FrmAddDescuentoRecargo( this );
	frmdto->showModalFor( this, true, true );
	if( !frmdto->wasCancelled() ) {
		double dto = frmdto->getDescuento();
		int redondeo = frmdto->getRedondeo();
		factu::RecAlbaranVenta *albdet = getCurrentMesa().albaranventa;
		factu::RecArticulo *articulo = static_cast<factu::RecArticulo*>(DBAPP->createRecord("ARTICULO"));
		if( dto < 0.0 ) {
			if( articulo->read( "CODIGO=" + articulo->getConnection()->toSQL(factu::ModuleInstance->getModuleSetting("ARTICULO_DESCUENTOS")) ) ) {
				albdet->addDescuentoRecargo( dto, articulo, redondeo);
				pTicketItemsModel->reset();
				setCurrentRow( pTicketItemsModel->rowCount() - 1);
				actualizaTotal();
			} else {
				FrmBase::msgError(this, Xtring::printf(_("No se ha encontrado el artículo para los descuentos con código '%s'"),
									factu::ModuleInstance->getModuleSetting("ARTICULO_DESCUENTOS").toString().c_str() ) );
			}
		} else {
			if( articulo->read( "CODIGO=" + articulo->getConnection()->toSQL(factu::ModuleInstance->getModuleSetting("ARTICULO_RECARGOS")) ) ) {
				albdet->addDescuentoRecargo( dto, articulo, redondeo);
				pTicketItemsModel->reset();
				setCurrentRow( pTicketItemsModel->rowCount() - 1);
				actualizaTotal();
			} else {
				FrmBase::msgError(this, Xtring::printf(_("No se ha encontrado el artículo para los recargos con código '%s'"),
									factu::ModuleInstance->getModuleSetting("ARTICULO_RECARGOS").toString().c_str() ) );
			}
		}
	}
	delete frmdto;
}


} // namespace tpv
} // namespace gong

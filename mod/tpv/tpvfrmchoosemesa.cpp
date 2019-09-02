#include <QtSvg>
#include <gongfileutils.h>
#include <gongguinamespace.h>
#include <factufldivadetallado.h>
#include <dbappdbapplication.h>
#include "tpvfrmchoosemesa.h"

namespace gong {
namespace tpv {

FrmChooseMesa::FrmChooseMesa( DictMesas &mesas, const Xtring &svgfname,
							  QWidget* parent, WidgetFlags f)
	: TpvFrmCustom( parent, "FrmChooseMesa", f ), mMesas( mesas )
{
	setCaption( _("Elegir mesa") );
	pSvgView = new SvgView( this );
	pSvgView->openFile( svgfname.c_str() );
	fillMesas( svgfname.c_str() );
	pControlsLayout->addWidget( pSvgView );
	pushAccept->setVisible(false);
	connect( pSvgView->getSvgItem(), SIGNAL( mouseClicked(QGraphicsSceneMouseEvent *) ),
			 this, SLOT( mouseClicked( QGraphicsSceneMouseEvent *) ) );
}

int FrmChooseMesa::fillMesas( const char *filename )
{
	_GONG_DEBUG_ASSERT( pSvgView->getRenderer() );
	Xtring mSvgFileContents = FileUtils::readFile( filename );
	const char *haystack = mSvgFileContents.c_str();
	const char *needle = "id=\"mesa_";
	const char *start_id;
	while( (start_id = strstr( haystack, needle  )) != NULL ) {
		start_id += strlen( needle );
		const char *end_id = start_id;
		while( *end_id && *end_id != '\"' )
			++end_id;
		Xtring element_id = "mesa_" + Xtring( start_id, end_id - start_id );
		if( pSvgView->getRenderer()->elementExists( toGUI(element_id ) ) ) {
			QRectF rect = pSvgView->getRenderer()->boundsOnElement( toGUI(element_id) );
			MesaInfo m;
			m.mesaname = strdup(element_id.mid(5).c_str());
			m.albaranventa = 0;
			m.coords = rect;
			m.ronda = -1;
#ifdef HAVE_SOCIASMODULE
			m.pRecMiembro = 0;
#endif			
			mMesas.insert( element_id, m );
		}
		haystack = end_id;
	}
	if( mMesas.find( NOMBRE_MESA_COMUN ) == mMesas.end() ) {
		MesaInfo m;
		m.mesaname = strdup(Xtring(NOMBRE_MESA_COMUN).mid(5).c_str());
		m.albaranventa = 0;
		m.coords = QRectF(0,0,100,100);
		m.ronda = -1;
		mMesas.insert( NOMBRE_MESA_COMUN, m );
	}
	return mMesas.size();
}


Xtring FrmChooseMesa::getChoosen() const
{
	return mChoosen;
}

void FrmChooseMesa::mouseClicked(QGraphicsSceneMouseEvent* event)
{
	_GONG_DEBUG_PRINT( 5, Xtring::printf( "%f, %f", event->pos().x(), event->pos().y() ) );
	for( DictMesas::const_iterator it = mMesas.begin();
		it != mMesas.end(); ++ it ) {
		QRectF r = it->second.coords;
		if( r.contains( event->pos() ) ) {
			mChoosen = it->first;
			_GONG_DEBUG_PRINT( 5, Xtring::printf( "Tocado! %s, %f, %f", it->first.c_str(), r.x(), r.y() ) );
			accept();
			return;
		}
	}
	mChoosen = "";
}

void FrmChooseMesa::deleteMesasOcupadas()
{
	for( RectangulosList::reverse_iterator it = mRectangulosMesasOcupadas.rbegin();
		it != mRectangulosMesasOcupadas.rend(); ++ it ) {
		pSvgView->scene()->removeItem( *it );
	}
	mRectangulosMesasOcupadas.clear();
}


/**
 * @brief Las mesas ocupadas tienen productos o bien se ha elegido un cliente o una camarera
 * distinta de las por omisión
 *
 * @param agente_id ...
 * @param cliente_id ...
 * @return void
 **/
void FrmChooseMesa::drawMesasOcupadas( dbRecordID agente_id, dbRecordID cliente_id )
{
	deleteMesasOcupadas();
	for( DictMesas::const_iterator it = mMesas.begin();
		it != mMesas.end(); ++ it ) {
		factu::RecAlbaranVenta *albaranventa = it->second.albaranventa;
		QRectF mesa_rect = it->second.coords;
		if( !albaranventa )
			continue;
		if( albaranventa->getListAlbaranVentaDet()->size() ) {
// 			|| (agente_id != albaranventa->getValue( "AGENTE_ID" ).toUInt()
// 				&&  albaranventa->getValue( "AGENTE_ID" ).toUInt() != 0 )
// 			|| (cliente_id != albaranventa->getValue( "CLIENTE_ID" ).toUInt()
// 				&&  albaranventa->getValue( "CLIENTE_ID" ).toUInt() != 0 ) ) {
			mesa_rect.setX( mesa_rect.x() - 8 );
			mesa_rect.setY( mesa_rect.y() - 8 );
			mesa_rect.setWidth( mesa_rect.width() + 8 );
			mesa_rect.setHeight( mesa_rect.height() + 8 );
			QGraphicsRectItem *rect = new QGraphicsRectItem( mesa_rect );
			QBrush b;
			b.setColor( Qt::red );//*backgroundColor );//rectColor );
			b.setStyle( Qt::DiagCrossPattern);
			rect->setBrush(b);
			pSvgView->scene()->addItem( rect );
			mRectangulosMesasOcupadas.push_back( rect );
		}
	}
}

} // namespace tpv
} // namespace gong

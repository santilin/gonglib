#include <gongdebug.h>
#include <gongguinamespace.h>
#include <gongfileutils.h>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include "tpvimagesgallery.h"
#include "tpvmodule.h"

namespace gong {
namespace tpv {

ImagesGallery::ImagesGallery(QWidget* parent, int sizex, int sizey, int columns,
							 int preferredrows )
	: mRows(0), mColumns(columns), mPreferredRows( preferredrows ), mCurrentRow(0),
		mCurrentPage(-1), mCurrentColumn(0), mSizeX(sizex), mSizeY(sizey),
		mPaddingBottom( TPV_GALLERY_PADDING_BOTTOM ), mPaddingRight( TPV_GALLERY_PADDING_RIGHT ),
		mShowNames( false ), mShowPagination(false), mIsSelectable( false )
{
	pScene = new QGraphicsScene();
	setScene( pScene );
	setAlignment( Qt::AlignLeft | Qt::AlignTop );
	setResizeAnchor( QGraphicsView::NoAnchor );
	mUpperCase = ModuleInstance->getModuleSetting( "NOMBRES_MAYUSCULAS" ).toBool();
}

ImagesGallery::~ImagesGallery()
{
	delete pScene;
}

void ImagesGallery::showCheckedBoard(bool show, const QColor &color)
{
	if( show ) {
		// Prepare background check-board pattern
		QPixmap tilePixmap(2*(mSizeX + mPaddingRight), 2*(mSizeY + mPaddingBottom));
		tilePixmap.fill(Qt::white);
		QPainter tilePainter(&tilePixmap);
		tilePainter.fillRect(0, 0, mSizeX + mPaddingRight, mSizeY + mPaddingBottom, color);
		tilePainter.fillRect(mSizeX + mPaddingRight, mSizeY + mPaddingBottom,
							 mSizeX + mPaddingRight, mSizeY + mPaddingBottom, color);
		tilePainter.end();
		setBackgroundBrush(tilePixmap);
	}
}

void ImagesGallery::drawBackground(QPainter* p, const QRectF& rect)
{
	p->save();
	p->drawTiledPixmap(sceneRect(), backgroundBrush().texture());
	p->restore();
}

void ImagesGallery::showPagination(bool show)
{
	if( (mShowPagination = show) ) {
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	} else {
		setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}

QSize ImagesGallery::getSizeImages() const
{
	return QSize( mSizeX + mPaddingRight + 1, mSizeY + mPaddingBottom + 1);
}


QSize ImagesGallery::sizeHint() const
{
	return QSize( mColumns * (mSizeX + mPaddingRight), mPreferredRows * (mSizeY + mPaddingBottom) );
}


void ImagesGallery::fixRowAndColumn(int& row, int& column)
{
	if( column == -1 )
		column = mCurrentColumn;
	while( column >= (int)mColumns ) {
		column -= mColumns;
		row++;
	}
	if( row == -1 )
		row = mCurrentRow;
	if( row > (int)mRows )
		row = mRows;
}

void ImagesGallery::incRowAndColumn()
{
	if( ++mCurrentColumn >= (int)mColumns ) {
		mCurrentColumn = 0;
		mCurrentRow ++;
		mRows++;
	}
}

void ImagesGallery::addPagination(int& row, int& column)
{
	if( ( row != 0 && (row % mPreferredRows) == (mPreferredRows -1) ) // Last row in page
		&& ( column != 0 && (column % mColumns ) == (mColumns - 1) ) ) { // Last column in row
		pScene->addRect( column * (mSizeX + mPaddingRight), row * (mSizeY + mPaddingBottom),
					mSizeX + mPaddingRight - 1, mSizeY + mPaddingBottom - 1 );
		QGraphicsItem *g = 0;
		Xtring nextprevfname = FileUtils::findInPath(
			ModuleInstance->getModuleLocalDataDir() + ":" + ModuleInstance->getModuleGlobalDataDir(),
			"nextpage.jpg", "images" );
		if( !nextprevfname.isEmpty() ) {
			QPixmap pixmap(nextprevfname.c_str());
			g = pScene->addPixmap( pixmap.scaled( QSize( mSizeX-1, mSizeY + mPaddingBottom -1), Qt::IgnoreAspectRatio ) );
		} else {
			// Describe a closed triangle
			QPolygonF Triangle;
			Triangle.append(QPointF(0, 0));
			Triangle.append(QPointF(mSizeX,(mSizeY+mPaddingBottom)/2));
			Triangle.append(QPointF(mSizeX,mSizeY+mPaddingBottom));
			Triangle.append(QPointF(0,0));
			g = pScene->addPolygon(Triangle);
		}
		g->setData(0, "{{NEXT}}");
		g->setPos( column * (mSizeX + mPaddingRight) + 1, row * (mSizeY + mPaddingBottom) + 1 );
		incRowAndColumn();
		row = mCurrentRow;
		column = mCurrentColumn;

		nextprevfname = FileUtils::findInPath(
			ModuleInstance->getModuleLocalDataDir() + ":" + ModuleInstance->getModuleGlobalDataDir(),
			"prevpage.jpg", "images" );
		if( !nextprevfname.isEmpty() ) {
			QPixmap pixmap(nextprevfname.c_str());
			g = pScene->addPixmap( pixmap.scaled( QSize( mSizeX-1, mSizeY+mPaddingBottom-1), Qt::IgnoreAspectRatio ) );
		} else {
			// Describe a closed triangle
			QPolygonF Triangle;
			Triangle.append(QPointF(0, mSizeX));
			Triangle.append(QPointF(0,(mSizeY+mPaddingBottom)/2));
			Triangle.append(QPointF(0,mSizeY+mPaddingBottom));
			Triangle.append(QPointF(0, mSizeX));
			g = pScene->addPolygon(Triangle);
		}
		g->setData(0, "{{PREV}}");
		g->setPos( column * (mSizeX + mPaddingRight) + 1, row * (mSizeY + mPaddingBottom) + 1 );
		incRowAndColumn();
		row = mCurrentRow;
		column = mCurrentColumn;
	}
}

QGraphicsItem *ImagesGallery::addNoImage( const Xtring &caption, int row, int column )
{
	fixRowAndColumn(row, column);
	if( mShowPagination )
		addPagination(row,column);
	pScene->addRect( column * (mSizeX + mPaddingRight), row * (mSizeY + mPaddingBottom),
					 mSizeX + mPaddingRight - 1, mSizeY + mPaddingBottom - 1 );
	GraphicsTextBoxItem *t = new GraphicsTextBoxItem( mSizeX-1, mSizeY-1,
						mUpperCase ? caption.upper() : caption );
	if( mIsSelectable )
		t->setFlags( QGraphicsItem::ItemIsSelectable );
	pScene->addItem( t );
	t->setTextWordWrap( Qt::TextWordWrap );
	t->setPos( column * (mSizeX + mPaddingRight) + 1, row * (mSizeY + mPaddingBottom ) + 1 );
	incRowAndColumn();
	return t;
}

QGraphicsItem* ImagesGallery::addImage(const QPixmap &pixmap, const Xtring &caption,
									   int row, int column)
{
	fixRowAndColumn(row, column);
	if( mShowPagination )
		addPagination(row,column);
	pScene->addRect( column * (mSizeX + mPaddingRight), row * (mSizeY + mPaddingBottom),
					 mSizeX + mPaddingRight - 1, mSizeY + mPaddingBottom - 1 );
	QGraphicsPixmapItem *p = pScene->addPixmap(
		pixmap.scaled( QSize( mSizeX-1, mSizeY-1), Qt::IgnoreAspectRatio ) );
	p->setPos( column * (mSizeX + mPaddingRight ) + (mPaddingRight / 2), row * (mSizeY + mPaddingBottom) + 1 );
	if( mIsSelectable )
		p->setFlags( QGraphicsItem::ItemIsSelectable );
	if( mShowNames && !caption.isEmpty() ) {
		GraphicsTextBoxItem *t = new GraphicsTextBoxItem( mSizeX, 10,
						mUpperCase ? caption.upper() : caption );
		pScene->addItem( t );
		t->setTextWordWrap( Qt::TextSingleLine );
		t->setPos( column * (mSizeX + mPaddingRight), ((row+1)*(mSizeY+mPaddingBottom)-mPaddingBottom));
	}
	incRowAndColumn();
	return p;
}

QGraphicsItem* ImagesGallery::addImage(const Variant& binary, const Xtring &caption,
									   int row, int column)
{
	QImage imagefromdata;
	QGraphicsItem *ret = 0;
	if( !binary.isEmpty() ) {
		QByteArray a( binary.toString().data(), binary.toString().size() );
		if( ! imagefromdata.loadFromData( a, "PNG") ) {
			_GONG_DEBUG_WARNING( "Could not read PNG image" );
		} else {
			QPixmap *pixmap = new QPixmap();
			if( pixmap->convertFromImage( imagefromdata ) ) {
				ret = addImage( *pixmap, caption, row, column );
			} else {
				_GONG_DEBUG_WARNING( "Could not convert PNG Image" );
			}
			delete pixmap;
		}
	}
	if( !ret )
		ret = addNoImage( caption, row, column );
	return ret;
}

void ImagesGallery::nextPage()
{
	if( ++mCurrentPage > int(mRows / mPreferredRows) )
		mCurrentPage = 0;
	setSceneRect( 0, mCurrentPage * mPreferredRows * ( mSizeY + mPaddingBottom ),
				  sizeHint().width(), ( mSizeY + mPaddingBottom ) * mPreferredRows );
}

void ImagesGallery::prevPage()
{
	if( --mCurrentPage < 0 )
		mCurrentPage = int( mRows / mPreferredRows );
	setSceneRect( 0, mCurrentPage * mPreferredRows * ( mSizeY + mPaddingBottom ),
				  sizeHint().width(), ( mSizeY + mPaddingBottom ) * mPreferredRows );
}


QGraphicsItem* ImagesGallery::getImage(int row, int column)
{
	return itemAt( column * (mSizeX + mPaddingRight), row * (mSizeY + mPaddingBottom) );
}

void ImagesGallery::mouseReleaseEvent(QMouseEvent* event)
{
	if( QGraphicsItem *item = itemAt(event->pos()) )
		emit imageClicked( item );
}

void ImagesGallery::clear()
{
	pScene->clear();
	mCurrentColumn = mCurrentRow = mRows = 0;
	mCurrentPage = -1;
}

void GraphicsTextBoxItem::paint(QPainter* painter,
								const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QRectF r = boundingRect();
	if( mTextWordWrap != Qt::TextWordWrap ) {
		float factor = r.width() / painter->fontMetrics().width(mText);
		if (factor < 1) {
			QFont f = painter->font();
			f.setPointSizeF(f.pointSizeF()*factor);
			painter->setFont(f);
		}
	}
	painter->drawText( QRect(0, 0, mSizeX, mSizeY),
						Qt::AlignHCenter | Qt::AlignVCenter | mTextWordWrap,
						mText, &r );
}


} // namespace tpv
} // namespace gong

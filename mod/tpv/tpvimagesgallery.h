#ifndef _TPV_IMAGESGALLERY_H
#define _TPV_IMAGESGALLERY_H

#include <gongvariant.h>
#include <gongguinamespace.h>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>

namespace gong {
namespace tpv {


class GraphicsTextBoxItem: public QGraphicsItem
{
public:
	GraphicsTextBoxItem(int sizex, int sizey, const Xtring &text, QGraphicsItem *parent = 0)
		: QGraphicsItem(parent), mSizeX(sizex), mSizeY(sizey), mText( toGUI(text) ),
		mTextWordWrap( Qt::TextWordWrap ) {}
	virtual QRectF boundingRect() const { return QRectF(0, 0, mSizeX, mSizeY); }
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem * option,
					   QWidget * widget = 0 );
	void setTextWordWrap( Qt::TextFlag ww ) { mTextWordWrap = ww; }
private:
	int mSizeX, mSizeY;
	QString mText;
	Qt::TextFlag mTextWordWrap;
};

class ImagesGallery: public QGraphicsView
{
	Q_OBJECT
public:
	ImagesGallery( QWidget *parent, int sizex, int sizey, int columns,
				   int preferredrows = 4 );
	~ImagesGallery();
	QGraphicsItem* addImage( const QPixmap &pixmap, const Xtring &caption, int row = -1, int column = -1 );
	QGraphicsItem* addImage( const Variant &binary, const Xtring &caption, int row = -1, int column = -1 );
	QGraphicsItem *addNoImage( const Xtring &caption, int row = -1, int column = -1 );
	QGraphicsItem* getImage( int row, int column );
	int getNRows() const { return mRows; }
	int getNColumns() const { return mColumns; }
	void clear();
	virtual QSize sizeHint() const; // from QWidget
	void setPaddingRight( int p ) { mPaddingRight = p; }
	void setPaddingBottom( int p ) { mPaddingBottom = p; }
	void setShowNames( bool show = true ) { mShowNames = show; }
	void showPagination(  bool show = true );
	void showCheckedBoard( bool show = true, const QColor &color = Qt::gray );
	void setSelectable( bool selectable ) { mIsSelectable = selectable; }
	bool isSelectable() const { return mIsSelectable; }
	QSize getSizeImages() const;

signals:
	void imageClicked( QGraphicsItem *sender );
public slots:
	void nextPage();
	void prevPage();

protected:
	void mouseReleaseEvent(QMouseEvent* event);
	void drawBackground(QPainter *p, const QRectF &rect);
	void fixRowAndColumn(int &row, int &column);
	void addPagination(int &row, int &column);
	void incRowAndColumn();

	uint mRows, mColumns, mPreferredRows;
	int mCurrentRow, mCurrentPage, mCurrentColumn;
	uint mSizeX, mSizeY, mPaddingBottom, mPaddingRight;
	bool mShowNames, mShowPagination, mUpperCase, mIsSelectable;
	QGraphicsScene *pScene;
};


} // namespace tpv
} // namespace gong

#endif // _TPV_IMAGESGALLERY_H


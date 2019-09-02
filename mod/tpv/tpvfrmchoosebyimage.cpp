#include "tpvimagesgallery.h"
#include "tpvfrmchoosebyimage.h"

namespace gong {
namespace tpv {

FrmChooseByImage::FrmChooseByImage(QWidget* parent, const char *name, WidgetFlags f)
	: FrmCustom(parent, name, f)
{
	pImagesGallery = new ImagesGallery( this, 80, 80, 5, 5);
	connect( pImagesGallery, SIGNAL( imageClicked( QGraphicsItem *) ), this, SLOT( slotImageChosen( QGraphicsItem *) ) );
	pImagesGallery->showCheckedBoard( true, Qt::green );
	pImagesGallery->setShowNames( true );
	pImagesGallery->showPagination( true );
	pControlsLayout->addWidget( pImagesGallery );
}

void FrmChooseByImage::clear()
{
	pImagesGallery->clear();
}

int FrmChooseByImage::addImages(dbConnection *conn, const Xtring& query)
{
	dbResultSet *rs = conn->select( query );
	while( rs->next() ) {
		Xtring code = rs->toString(2);
		Xtring name = rs->toString(3);
		dbRecordID recid = rs->toUInt((uint)0);
		QGraphicsItem *item = pImagesGallery->addImage( rs->toBinary(1), name );
		item->setData(0, toGUI(code) );
		item->setData(1, toGUI(name) );
		item->setData(2, QVariant(recid));
	}
	int ret = rs->getRowCount();
	if( ret == 0 ) {
		mRecordID = 0;
		mCode = Xtring::null;
		mName = Xtring::null;
	} else if( ret == 1 ) {
		mRecordID = rs->getValue(0, 0).toUInt();
		mCode = rs->getValue(0, 2).toString();
		mName = rs->getValue(0, 3).toString();
	}
	delete rs;
	return ret;
}

void FrmChooseByImage::slotImageChosen(QGraphicsItem *item)
{
	mRecordID = item->data(2).toUInt();
	mCode = fromGUI(item->data(0).toString());
	mName = fromGUI(item->data(1).toString());
	accept();
}

} // namespace tpv
} // namespace gong

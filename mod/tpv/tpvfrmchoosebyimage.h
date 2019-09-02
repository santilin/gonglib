#ifndef TPV_FRMCHOOSEBYIMAGE_H
#define TPV_FRMCHOOSEBYIMAGE_H
#include <dbappfrmcustom.h>

class QGraphicsItem;

namespace gong {
namespace tpv {

class ImagesGallery;

class FrmChooseByImage: public FrmCustom
{
	Q_OBJECT
public:
	FrmChooseByImage( QWidget *parent=0, const char *name = 0, WidgetFlags f = 0 );
	Xtring getCode() const { return mCode; }
	Xtring getName() const { return mName; }
	dbRecordID getRecordID() const { return mRecordID; }
	int addImages( dbConnection *conn, const Xtring &query );
	void clear();

protected slots:
	void slotImageChosen(QGraphicsItem *);

protected:
	ImagesGallery *pImagesGallery;
	dbRecordID mRecordID;
	Xtring mCode, mName;
};

} // namespace tpv
} // namespace gong

#endif // TPV_FRMCHOOSEBYIMAGE_H

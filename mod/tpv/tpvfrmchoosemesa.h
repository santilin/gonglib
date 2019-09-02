#ifndef _TPV_FRMCHOOSEMESA_H
#define _TPV_FRMCHOOSEMESA_H

#include "tpvfrmtpvscreen.h"
#include "tpvfrmcustom.h"
#include "tpvsvgview.h"

#define NOMBRE_MESA_COMUN "mesa_comun"

namespace gong {

namespace factu { class RecAlbaranVenta; }

namespace tpv {

class FrmChooseMesa: public TpvFrmCustom
{
	Q_OBJECT
public:
	typedef std::vector<QGraphicsRectItem *> RectangulosList;
	FrmChooseMesa( DictMesas &mesas, const Xtring &svgfilename,
				   QWidget *parent = 0, WidgetFlags f = 0 );
	Xtring getChoosen() const;
	int fillMesas( const char *filename );
	void drawMesasOcupadas( dbRecordID agente_id, dbRecordID cliente_id );
	void deleteMesasOcupadas();

protected slots:
	void mouseClicked(QGraphicsSceneMouseEvent* event); // from QWidget

private:
	DictMesas &mMesas;
	SvgView *pSvgView;
	Xtring mChoosen;
	RectangulosList mRectangulosMesasOcupadas;
};

} // namespace tpv
} // namespace gong


#endif // _TPV_FRMCHOOSEMESA_H

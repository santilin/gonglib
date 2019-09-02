/*<<<<<COPYLEFT*/
/** @file ofertasbotonpasaraofertabehavior.h Behavior para añadir un botón para pasar a oferta un albarán o factura
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<FRMEDITBOTONPASARAOFERTABEHAVIOR_PREAMBLE*/
#ifndef _OFERTAS_OFERTASBOTONPASARAOFERTABEHAVIOR_H
#define _OFERTAS_OFERTASBOTONPASARAOFERTABEHAVIOR_H
/*>>>>>FRMEDITBOTONPASARAOFERTABEHAVIOR_PREAMBLE*/
/*<<<<<FRMEDITBOTONPASARAOFERTABEHAVIOR_INCLUDES*/
#include <dbappfrmeditrec.h>
#include <dbappfrmeditrecbehavior.h>
/*>>>>>FRMEDITBOTONPASARAOFERTABEHAVIOR_INCLUDES*/

namespace gong {

class PushButton;

namespace ofertas {


#define FrmEditBotonPasarAOfertaBehavior BotonPasarAOfertaBehavior
/*<<<<<FRMEDITBOTONPASARAOFERTABEHAVIOR_CLASS*/
class FrmEditBotonPasarAOfertaBehavior: public FrmEditRecBehavior
{
	Q_OBJECT
public:
	virtual void initGUI();
	//RecBotonPasarAOferta *getRecBotonPasarAOferta() const { return static_cast<RecBotonPasarAOferta *>(pTheForm->getRecord()); }
	FrmEditBotonPasarAOfertaBehavior( FrmEditRec *theform )
		: FrmEditRecBehavior( theform )
/*>>>>>FRMEDITBOTONPASARAOFERTABEHAVIOR_CLASS*/
    {}

    virtual void updateStatus();
    // virtual bool canClose();
/*<<<<<FRMEDITBOTONPASARAOFERTABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
protected:
	virtual void scatterFields( bool is_pre );
	virtual void gatherFields();
	virtual void validateFields( bool is_pre,
		QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITBOTONPASARAOFERTABEHAVIOR_VIRTUALS_FROM_FRMEDITRECBEHAVIOR*/
    dbRecordList *getDetalles(dbRecord *record, uint pos = 0);

protected slots:
    void pasarAOferta_clicked();

/*<<<<<FRMEDITBOTONPASARAOFERTABEHAVIOR_CONTROLS*/
protected:
/*>>>>>FRMEDITBOTONPASARAOFERTABEHAVIOR_CONTROLS*/
    PushButton *pushPasarAOferta;
};

/*<<<<<FRMEDITBOTONPASARAOFERTABEHAVIOR_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASBOTONPASARAOFERTABEHAVIOR_H
/*>>>>>FRMEDITBOTONPASARAOFERTABEHAVIOR_POSTAMBLE*/


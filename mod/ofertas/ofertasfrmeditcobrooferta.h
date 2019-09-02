/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditcobrooferta.h Fichero de edición de cobros de ofertas
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
#ifndef _OFERTAS_OFERTASFRMEDITCOBROOFERTA_H
#define _OFERTAS_OFERTASFRMEDITCOBROOFERTA_H

#include <pagosfrmeditcobro.h>
#include "ofertasreccobrooferta.h"

namespace gong {
namespace ofertas {

class FrmEditCobroOferta: public pagos::FrmEditCobro
{
public:
    FrmEditCobroOferta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                       EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                       QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
protected:	
    virtual void cambiaFields(); // from FrmEditCobro

};

} // namespace ofertas
} // namespace gong

#endif // OFERTAS_OFERTASFRMEDITCOBROOFERTA_H


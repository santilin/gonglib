/*<<<<<COPYLEFT*/
/** @file facturecarticulo.h Registro de artículos
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
/*<<<<<ARTICULO_PREAMBLE*/
#ifndef _FACTU_FACTURECARTICULO_H
#define _FACTU_FACTURECARTICULO_H
/*>>>>>ARTICULO_PREAMBLE*/

/*<<<<<ARTICULO_INCLUDES*/
#include <dbappdbapplication.h>
#include <gongdbvalidresult.h>

#include "facturecfamilia.h"
#include "empresarectipoiva.h"
#include "facturecproveedora.h"
#include "facturecarticuloimagen.h"
/*>>>>>ARTICULO_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<ARTICULO_CONSTRUCTOR*/
class RecArticulo: public dbRecord
{
public:
	RecArticulo(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("ARTICULO"), recid, user)
/*>>>>>ARTICULO_CONSTRUCTOR*/
    {}
    enum GenCodArt { GenCodArtFijo = 0, GenCodArtNextProv, GenCodArtNextFam, GenCodArtNextProvFam,
                     GenCodArtNextArt, GenCodArtEAN, GenCodArtISBN, GenCodArtQR };
    /*<<<<<ARTICULO_RELATIONS*/
	RecFamilia *getRecFamilia() const;
	empresa::RecTipoIVA *getRecTipoIVA() const;
	RecProveedora *getRecProveedora() const;
	RecArticuloImagen *getRecArticuloImagen() const;
/*>>>>>ARTICULO_RELATIONS*/
    /*<<<<<ARTICULO_MEMBERS*/
	virtual bool validate(ValidResult::Context context); // from dbRecord
/*>>>>>ARTICULO_MEMBERS*/

    void actStocks(bool savingdetail, bool compra, double cantidad);
    void fixMargenYDescuento();
    void fixPrecios( const Xtring &fldchanged = Xtring::null );
    double masIVA( double cantidad ) const;
    double menosIVA( double cantidad ) const;
    double getPVP( int tarifacliente ) const;
    void setPVP( int tarifacliente, double pvp );

    static XtringList sModalidades;
    Xtring genCodigoArticulo(int modalidad, const Xtring &formato) const;
    Xtring formatCodigoArticulo(const Xtring &last, const Xtring &formato ) const;

}; // end class

/*<<<<<ARTICULO_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECARTICULO_H
/*>>>>>ARTICULO_POSTAMBLE*/

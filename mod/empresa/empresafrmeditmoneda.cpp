/*<<<<<COPYLEFT*/
/** @file empresafrmeditmoneda.cpp Fichero de edición de monedas
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de monedas
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD Simbolo string - simbolo
// FIELD Decimales int - simbolo
// FIELD PuntoDecimal string - puntoLayout
// FIELD SeparadorMiles string - puntoLayout
// FIELD Actualizacion date - tasa
// FIELD TasaCambio double - tasa
// TYPE FrmEditRecMaster empresa::Moneda ValidCodeAndDesc IncCode CodeNotFound
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITMONEDA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "empresafrmeditmoneda.h"
/*>>>>>FRMEDITMONEDA_INCLUDES*/

namespace gong
{
namespace empresa {

/*<<<<<FRMEDITMONEDA_CONSTRUCTOR*/
FrmEditMoneda::FrmEditMoneda(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditMoneda" );
/*>>>>>FRMEDITMONEDA_CONSTRUCTOR*/
    /*<<<<<FRMEDITMONEDA_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *simboloLayout = new QHBoxLayout(0, 0, 6, "simboloLayout");
	QHBoxLayout *puntoLayout = new QHBoxLayout(0, 0, 6, "puntoLayout");
	QHBoxLayout *tasaLayout = new QHBoxLayout(0, 0, 6, "tasaLayout");
	editCodigo = addEditField( pControlsFrame, "MONEDA", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "MONEDA", "NOMBRE", codigoLayout );
	editSimbolo = addEditField( pControlsFrame, "MONEDA", "SIMBOLO", simboloLayout );
	editDecimales = addEditField( pControlsFrame, "MONEDA", "DECIMALES", simboloLayout );
	editPuntoDecimal = addEditField( pControlsFrame, "MONEDA", "PUNTODECIMAL", puntoLayout );
	editSeparadorMiles = addEditField( pControlsFrame, "MONEDA", "SEPARADORMILES", puntoLayout );
	editActualizacion = addEditField( pControlsFrame, "MONEDA", "ACTUALIZACION", tasaLayout );
	editTasaCambio = addEditField( pControlsFrame, "MONEDA", "TASACAMBIO", tasaLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( simboloLayout );
	pControlsLayout->addLayout( puntoLayout );
	pControlsLayout->addLayout( tasaLayout );
/*>>>>>FRMEDITMONEDA_INIT_CONTROLS*/
    Q3HBoxLayout *ejemploMonedaLayout = new Q3HBoxLayout( 0, 0, 6, "ejemploMonedaLayout" );
    lblEjemplosMoneda = new QLabel( pControlsFrame, "lblPositiva" );
    ejemploMonedaLayout->addWidget( lblEjemplosMoneda );
    pControlsLayout->addLayout( ejemploMonedaLayout );
}

void FrmEditMoneda::scatterFields()
{
    /*<<<<<FRMEDITMONEDA_SCATTER*/
	editCodigo->setText(getRecMoneda()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecMoneda()->getValue("NOMBRE").toString());
	editSimbolo->setText(getRecMoneda()->getValue("SIMBOLO").toString());
	editDecimales->setText(getRecMoneda()->getValue("DECIMALES").toInt());
	editPuntoDecimal->setText(getRecMoneda()->getValue("PUNTODECIMAL").toString());
	editSeparadorMiles->setText(getRecMoneda()->getValue("SEPARADORMILES").toString());
	editActualizacion->setText(getRecMoneda()->getValue("ACTUALIZACION").toDate());
	editTasaCambio->setText(getRecMoneda()->getValue("TASACAMBIO").toDouble());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
	if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
		if( DBAPP->codeNotFound().toInt() != 0 ) {
			editCodigo->setText( DBAPP->codeNotFound() );
			editCodigo->setJustEdited( true );
		} else {
		editNombre->setText( DBAPP->codeNotFound() );
		editNombre->setJustEdited( true );
		}
	}
/*>>>>>FRMEDITMONEDA_SCATTER*/
    if ( isInserting() ) {
        editSimbolo->setText( DBAPP->getRegConfig()->getCurrencySymbol() );
        editDecimales->setText( DBAPP->getRegConfig()->getCurrencyFracDigits() );
        editPuntoDecimal->setText( Xtring( 1, DBAPP->getRegConfig()->getCurrencyDecimalPoint() ) );
        editSeparadorMiles->setText( Xtring( 1, DBAPP->getRegConfig()->getCurrencyThousandsSep() ) );
    }

}

void FrmEditMoneda::gatherFields()
{
    /*<<<<<FRMEDITMONEDA_GATHER*/
	getRecMoneda()->setValue( "CODIGO", editCodigo->toInt());
	getRecMoneda()->setValue( "NOMBRE", editNombre->toString());
	getRecMoneda()->setValue( "SIMBOLO", editSimbolo->toString());
	getRecMoneda()->setValue( "DECIMALES", editDecimales->toInt());
	getRecMoneda()->setValue( "PUNTODECIMAL", editPuntoDecimal->toString());
	getRecMoneda()->setValue( "SEPARADORMILES", editSeparadorMiles->toString());
	getRecMoneda()->setValue( "ACTUALIZACION", editActualizacion->toDate());
	getRecMoneda()->setValue( "TASACAMBIO", editTasaCambio->toDouble());
/*>>>>>FRMEDITMONEDA_GATHER*/
}

void FrmEditMoneda::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITMONEDA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITMONEDA_VALIDATE*/
    if ( sender == editPuntoDecimal || sender == editDecimales
            || sender == editSeparadorMiles || sender == editSimbolo ) {
        RegConfig regconfig;
        regconfig.setCurrencyFormat(
            editSimbolo->toString(),
            editDecimales->toInt(),
            editPuntoDecimal->toString().isEmpty() ? '.' : editPuntoDecimal->toString().at( 0 ),
            editSeparadorMiles->toString().isEmpty() ? ',' : editSeparadorMiles->toString().at( 0 ) );
        Formatter f( regconfig );
        lblEjemplosMoneda->setText( QString::fromLocal8Bit( Xtring::printf( _( "Ejemplo: Cero: %s  Positivo: %s   Negativo:%s" ),
                                    f.format( Money( 0, regconfig.getCurrencyFracDigits() ), 0, 0 ).c_str(),
                                    f.format( Money( 1234.56, regconfig.getCurrencyFracDigits() ), 0, 0 ).c_str(),
                                    f.format( Money( -1234.56, regconfig.getCurrencyFracDigits() ), 0, 0 ).c_str() ).c_str() ) );
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITMONEDA_FIN*/
} // namespace empresa
} // namespace gong
/*>>>>>FRMEDITMONEDA_FIN*/


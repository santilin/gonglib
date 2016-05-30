/*<<<<<MODULE_INFO*/
// COPYLEFT Formulario de edición de tipos de socia
// FIELD Codigo int - codigo
// FIELD Nombre string - codigo
// FIELD PorcentajeCuota double
// FIELD Notas text
// TYPE FrmEditRecMaster socias::TipoSocia validCodeAndDesc IncCode
/*>>>>>MODULE_INFO*/
/*<<<<<FRMEDITTIPOSOCIA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "sociasfrmedittiposocia.h"
/*>>>>>FRMEDITTIPOSOCIA_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<FRMEDITTIPOSOCIA_CONSTRUCTOR*/
FrmEditTipoSocia::FrmEditTipoSocia(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditTipoSocia" );
/*>>>>>FRMEDITTIPOSOCIA_CONSTRUCTOR*/
    /*<<<<<FRMEDITTIPOSOCIA_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *porcentajecuotaLayout = new QHBoxLayout(0, 0, 6, "porcentajecuotaLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "TIPOSOCIA", "CODIGO", codigoLayout );
	editNombre = addEditField( pControlsFrame, "TIPOSOCIA", "NOMBRE", codigoLayout );
	editPorcentajeCuota = addEditField( pControlsFrame, "TIPOSOCIA", "PORCENTAJECUOTA", porcentajecuotaLayout );
	editNotas = addTextField( pControlsFrame, "TIPOSOCIA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( porcentajecuotaLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITTIPOSOCIA_INIT_CONTROLS*/
}

void FrmEditTipoSocia::scatterFields()
{
    /*<<<<<FRMEDITTIPOSOCIA_SCATTER*/
	editCodigo->setText(getRecTipoSocia()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecTipoSocia()->getValue("NOMBRE").toString());
	editPorcentajeCuota->setText(getRecTipoSocia()->getValue("PORCENTAJECUOTA").toDouble());
	editNotas->setText(getRecTipoSocia()->getValue("NOTAS").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
/*>>>>>FRMEDITTIPOSOCIA_SCATTER*/
    if( isInserting() )
        editPorcentajeCuota->setText( 100.0 );
}

void FrmEditTipoSocia::gatherFields()
{
    /*<<<<<FRMEDITTIPOSOCIA_GATHER*/
	getRecTipoSocia()->setValue( "CODIGO", editCodigo->toInt());
	getRecTipoSocia()->setValue( "NOMBRE", editNombre->toString());
	getRecTipoSocia()->setValue( "PORCENTAJECUOTA", editPorcentajeCuota->toDouble());
	getRecTipoSocia()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITTIPOSOCIA_GATHER*/
}

void FrmEditTipoSocia::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITTIPOSOCIA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITTIPOSOCIA_VALIDATE*/
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITTIPOSOCIA_FIN*/
} // namespace socias
} // namespace gong
/*>>>>>FRMEDITTIPOSOCIA_FIN*/

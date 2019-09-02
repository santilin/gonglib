/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de detalles de liquidaciones de compra
// FIELD NLinea int - pre
// FIELD TipoIVA_ID Reference(empresa::TipoIVA,Codigo,Nombre) - pre
// FIELD Cantidad double - cantidad
// FIELD Articulo_ID Reference(factu::Articulo,Codigo,Nombre,dbRecord::SeekCodeInDesc) - cantidad
// FIELD CosteSinIVA money - precios
// FIELD Coste money - precios
// FIELD DtoP100 double - precios
// FIELD Importe money - precios
// FIELD ImporteConIVA money - precios
// FIELD Descripcion text
// FIELD Notas text
// TYPE FrmEditRecDetail deposit::LiquidacionCompraDet PedidoDet Compra
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITLIQUIDACIONCOMPRADET_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "depositfrmeditliquidacioncompradet.h"
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_INCLUDES*/
#include <empresamodule.h>
#include <facturecalbarancompra.h>
#include <factufldivadetallado.h>
#include <factumodule.h>
#include "depositfrmeditliquidacioncompra.h"

namespace gong {
namespace deposit {

/*<<<<<FRMEDITLIQUIDACIONCOMPRADET_CONSTRUCTOR*/
FrmEditLiquidacionCompraDet::FrmEditLiquidacionCompraDet(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: FrmEditRecDetail( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditLiquidacionCompraDet" );
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_CONSTRUCTOR*/
    /*<<<<<FRMEDITLIQUIDACIONCOMPRADET_INIT_CONTROLS*/
	QHBoxLayout *preLayout = new QHBoxLayout(0, 0, 6, "preLayout");
	QHBoxLayout *cantidadLayout = new QHBoxLayout(0, 0, 6, "cantidadLayout");
	QHBoxLayout *preciosLayout = new QHBoxLayout(0, 0, 6, "preciosLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editNLinea = addEditField( pControlsFrame, "LIQUIDACIONCOMPRADET", "NLINEA", preLayout );

	searchTipoIVACodigo = addSearchField( pControlsFrame, "TIPOIVA_ID", "TIPOIVA", "CODIGO", "NOMBRE", preLayout );
	pushTipoIVACodigo = searchTipoIVACodigo->getButton();
	connect( pushTipoIVACodigo, SIGNAL( clicked() ), this, SLOT( pushTipoIVACodigo_clicked() ) );
	editTipoIVACodigo = searchTipoIVACodigo->getEditCode();
	editTipoIVANombre = searchTipoIVACodigo->getEditDesc();
	editCantidad = addEditField( pControlsFrame, "LIQUIDACIONCOMPRADET", "CANTIDAD", cantidadLayout );

	searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", cantidadLayout );
	pushArticuloCodigo = searchArticuloCodigo->getButton();
	connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
	editArticuloCodigo = searchArticuloCodigo->getEditCode();
	editArticuloNombre = searchArticuloCodigo->getEditDesc();
	editCosteSinIVA = addEditField( pControlsFrame, "LIQUIDACIONCOMPRADET", "COSTESINIVA", preciosLayout );
	editCoste = addEditField( pControlsFrame, "LIQUIDACIONCOMPRADET", "COSTE", preciosLayout );
	editDtoP100 = addEditField( pControlsFrame, "LIQUIDACIONCOMPRADET", "DTOP100", preciosLayout );
	editImporte = addEditField( pControlsFrame, "LIQUIDACIONCOMPRADET", "IMPORTE", preciosLayout );
	editImporteConIVA = addEditField( pControlsFrame, "LIQUIDACIONCOMPRADET", "IMPORTECONIVA", preciosLayout );
	editDescripcion = addTextField( pControlsFrame, "LIQUIDACIONCOMPRADET", "DESCRIPCION", descripcionLayout );
	editNotas = addTextField( pControlsFrame, "LIQUIDACIONCOMPRADET", "NOTAS", notasLayout );
	pControlsLayout->addLayout( preLayout );
	pControlsLayout->addLayout( cantidadLayout );
	pControlsLayout->addLayout( preciosLayout );
	pControlsLayout->addLayout( descripcionLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_INIT_CONTROLS*/
	editNLinea->setMustBeReadOnly( true );
	editArticuloCodigo->setWidthInChars(12);

	pushActPrecioArticulo = new QPushButton(this, "pushActPrecioArticulo" );
	pushActPrecioArticulo->setText( toGUI( _( "&Act. coste artículo" ) ) );
    connect( pushActPrecioArticulo, SIGNAL( clicked() ), this, SLOT( slotActPrecioArticulo_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushActPrecioArticulo );

	pushInsertTable = new QPushButton(this, "pushInsertTable" );
	pushInsertTable->setText( toGUI( _( "Insertar..." ) ) );
    connect( pushInsertTable, SIGNAL( clicked() ), this, SLOT( slotInsertTable_clicked() ) );
    pButtonsLayout->insertWidget( 2, pushInsertTable );
}

void FrmEditLiquidacionCompraDet::scatterFields()
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRADET_SCATTER*/
	editNLinea->setText(getRecLiquidacionCompraDet()->getValue("NLINEA").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNLinea;
	editCantidad->setText(getRecLiquidacionCompraDet()->getValue("CANTIDAD").toDouble());
	editCosteSinIVA->setText(getRecLiquidacionCompraDet()->getValue("COSTESINIVA").toMoney());
	editCoste->setText(getRecLiquidacionCompraDet()->getValue("COSTE").toMoney());
	editDtoP100->setText(getRecLiquidacionCompraDet()->getValue("DTOP100").toDouble());
	editImporte->setText(getRecLiquidacionCompraDet()->getValue("IMPORTE").toMoney());
	editImporteConIVA->setText(getRecLiquidacionCompraDet()->getValue("IMPORTECONIVA").toMoney());
	editDescripcion->setText(getRecLiquidacionCompraDet()->getValue("DESCRIPCION").toString());
	editNotas->setText(getRecLiquidacionCompraDet()->getValue("NOTAS").toString());
	scatterTipoIVA();
	scatterArticulo();
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_SCATTER*/
}

void FrmEditLiquidacionCompraDet::gatherFields()
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRADET_GATHER*/
	getRecLiquidacionCompraDet()->setValue( "NLINEA", editNLinea->toInt());
	getRecLiquidacionCompraDet()->setValue( "TIPOIVA_ID", getRecTipoIVA()->getRecordID() );
	getRecLiquidacionCompraDet()->setValue( "CANTIDAD", editCantidad->toDouble());
	getRecLiquidacionCompraDet()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
	getRecLiquidacionCompraDet()->setValue( "COSTESINIVA", editCosteSinIVA->toMoney());
	getRecLiquidacionCompraDet()->setValue( "COSTE", editCoste->toMoney());
	getRecLiquidacionCompraDet()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecLiquidacionCompraDet()->setValue( "IMPORTE", editImporte->toMoney());
	getRecLiquidacionCompraDet()->setValue( "IMPORTECONIVA", editImporteConIVA->toMoney());
	getRecLiquidacionCompraDet()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecLiquidacionCompraDet()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_GATHER*/
}

void FrmEditLiquidacionCompraDet::scatterTipoIVA()
{
/*<<<<<FRMEDITLIQUIDACIONCOMPRADET_SCATTER_TIPOIVA*/
	editTipoIVACodigo->setText( getRecTipoIVA()->getValue("CODIGO") );
	editTipoIVANombre->setText( getRecTipoIVA()->getValue("NOMBRE") );
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_SCATTER_TIPOIVA*/
}

void FrmEditLiquidacionCompraDet::pushTipoIVACodigo_clicked()
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchTipoIVACodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editTipoIVACodigo->setJustEdited( false );
			editTipoIVACodigo->setCancelling();
			if( DBAPP->choose(this, getRecTipoIVA(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterTipoIVA();
				editTipoIVACodigo->setJustEdited( true );
				setWiseFocus(editTipoIVACodigo);
			}
			break;
		case 'M':
			{
				if( getRecTipoIVA()->getRecordID() ) {
					editTipoIVACodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecTipoIVA(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editTipoIVACodigo->setJustEdited( true );
						scatterTipoIVA();
					}
				setWiseFocus(editTipoIVACodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecTipoIVA()->getRecordID() != 0 ) {
					editTipoIVACodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecTipoIVA(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				empresa::RecTipoIVA *tmprec = static_cast<empresa::RecTipoIVA *>(DBAPP->createRecord( "TipoIVA" ));
				editTipoIVACodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editTipoIVACodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editTipoIVACodigo->setJustEdited( true );
					getRecTipoIVA()->copyRecord( tmprec );
					scatterTipoIVA();
				}
				setWiseFocus(editTipoIVACodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_PUSH_TIPOIVA_CODIGO_CLICKED*/
}

void FrmEditLiquidacionCompraDet::scatterArticulo()
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRADET_SCATTER_ARTICULO*/
	editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
	editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_SCATTER_ARTICULO*/
}
void FrmEditLiquidacionCompraDet::pushArticuloCodigo_clicked()
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRADET_PUSH_ARTICULO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchArticuloCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editArticuloCodigo->setJustEdited( false );
			editArticuloCodigo->setCancelling();
			if( DBAPP->choose(this, getRecArticulo(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterArticulo();
				editArticuloCodigo->setJustEdited( true );
				setWiseFocus(editArticuloCodigo);
			}
			break;
		case 'M':
			{
				if( getRecArticulo()->getRecordID() ) {
					editArticuloCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecArticulo(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editArticuloCodigo->setJustEdited( true );
						scatterArticulo();
					}
				setWiseFocus(editArticuloCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecArticulo()->getRecordID() != 0 ) {
					editArticuloCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecArticulo(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				factu::RecArticulo *tmprec = static_cast<factu::RecArticulo *>(DBAPP->createRecord( "Articulo" ));
				editArticuloCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editArticuloCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editArticuloCodigo->setJustEdited( true );
					getRecArticulo()->copyRecord( tmprec );
					scatterArticulo();
				}
				setWiseFocus(editArticuloCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_PUSH_ARTICULO_CODIGO_CLICKED*/
}
void FrmEditLiquidacionCompraDet::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRADET_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecDetail::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoIVACodigo )
		pushTipoIVACodigo_clicked();
	if( sender == editArticuloCodigo )
		pushArticuloCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_SPECIALACTION*/
}

void FrmEditLiquidacionCompraDet::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRADET_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( focusWidget() != pushTipoIVACodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTipoIVACodigo, editTipoIVANombre,
		getRecTipoIVA(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTipoIVA();
	if( focusWidget() != pushArticuloCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editArticuloCodigo, editArticuloNombre,
		getRecArticulo(), "CODIGO", "NOMBRE", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::SeekCodeInDesc )) )
		scatterArticulo();
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_VALIDATE*/
/*<<<<<FRMEDITLIQUIDACIONCOMPRADET_DETALLE_VALIDATE*/
	bool actualiza = (sender && (sender == pFocusWidget) );
	double cantidad = editCantidad->toDouble();
	double costesiniva = editCosteSinIVA->toDouble();
	double costeconiva = editCoste->toDouble();
	double dtop100 = editDtoP100->toDouble();
	double importesiniva =  cantidad * costesiniva * ( 1 - dtop100 / 100 );
	if ( sender == editCantidad && editCantidad->isJustEdited() ) {
		actualiza = true;
	}
	if ( sender == editArticuloCodigo && editArticuloCodigo->isJustEdited() ) {
		actualiza = true;
		editTipoIVACodigo->setText( getRecArticulo()->getRecTipoIVA()->getValue( "CODIGO" ) );
		editTipoIVACodigo->setJustEdited( true );
		if ( validSeekCode( editTipoIVACodigo, isvalid, *validresult, editTipoIVACodigo, editTipoIVANombre,
		                    getRecTipoIVA(), "CODIGO", "NOMBRE", Xtring::null ) )
			scatterTipoIVA();
		costeconiva = getRecArticulo()->getValue( "Coste" ).toDouble();
		// El precio con IVA se calcula tomando el tipo de iva de este detalle
		costesiniva = costeconiva / ( 1 + getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 );
	}
	if( (sender == editTipoIVACodigo && editTipoIVACodigo->isJustEdited())
		|| (sender == editCosteSinIVA && editCosteSinIVA->isJustEdited()) ) {
		actualiza = true;
		costesiniva = editCosteSinIVA->toDouble();
		costeconiva = costesiniva * ( 1 + getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 );
	}
	if ( sender == editCoste && editCoste->isJustEdited() ) {
		actualiza = true;
		costeconiva = editCoste->toDouble();
		costesiniva = costeconiva / ( 1 + getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 );
	}
	if ( sender == editDtoP100 && editDtoP100->isJustEdited() ) {
		actualiza = true;
		dtop100 = editDtoP100->toDouble();
	}
	if( sender == editImporte && editImporte->isJustEdited() ) {
		importesiniva = editImporte->toDouble();
		if( cantidad != 0.0 && costesiniva != 0.0 ) {
			dtop100 = 100 * ( 1 - importesiniva / (cantidad * costesiniva) );
			editDtoP100->setText( dtop100 );
		}
		actualiza = true;
	}
	if( sender == editImporteConIVA && editImporteConIVA->isJustEdited() ) {
		if( cantidad != 0.0 && costesiniva != 0.0 ) {
			dtop100 = 100 * ( 1 - editImporteConIVA->toDouble() / (cantidad * costeconiva) );
			editDtoP100->setText( dtop100 );
		}
		actualiza = true;
	}
	if( actualiza ) {
		editCosteSinIVA->setText( costesiniva );
		editCoste->setText( costeconiva );
		int ndec = empresa::ModuleInstance->getDecimalesMoneda();
		if( getFrmMaster()->getControlValue( "IVADETALLADO" ).toInt() == factu::FldIVADetallado::incluido )
			ndec += 2;
		Money importesiniva(cantidad * costesiniva * ( 1 - dtop100 / 100 ), ndec);
		Money importeconiva = Money( cantidad * costeconiva * ( 1 - dtop100 / 100 ),
			empresa::ModuleInstance->getDecimalesMoneda() );
		editImporte->setText( importesiniva );
		editImporteConIVA->setText( importeconiva );
	}
	if (( sender == editArticuloCodigo && editArticuloCodigo->isJustEdited() ) ||
		    ( sender == editCantidad && editCantidad->isJustEdited() ) ) {
		if ( !getRecArticulo()->isNew() ) {
			Xtring stock_msg;
			if( getRecArticulo()->getValue( "USARSTOCKS" ).toBool() )
				stock_msg = getRecArticulo()->getValue( "STOCK" ).toString();
			else
				stock_msg = _("No se usan");
			Xtring precios_msg;
			precios_msg = _( "%s: PVP: %s, Coste: %s, Existencias: %s" );
			DBAPP->showOSD( getTitle(), Xtring::printf( precios_msg,
		                       getRecArticulo()->getValue( "CODIGO" ).toString().c_str(),
		                       getRecArticulo()->getValue( "PVP" ).toMoney().toString( DBAPP->getRegConfig() ).c_str(),
		                       editCoste->toMoney().toString( DBAPP->getRegConfig() ).c_str(),
		                       stock_msg.c_str() ) );
		}
	}
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_DETALLE_VALIDATE*/
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditLiquidacionCompraDet::slotActPrecioArticulo_clicked()
{
	if( factu::FactuModule::editCostesArticulo(this, getRecArticulo(), 
		editCosteSinIVA->toDouble() ) ) {
		searchArticuloCodigo->setValue( getRecArticulo()->getValue("CODIGO").toString() );
	}
}

void FrmEditLiquidacionCompraDet::slotInsertTable_clicked()
{
	factu::ModuleInstance->insertDetails( getFrmMaster(), this );
}

void FrmEditLiquidacionCompraDet::updateStatus( bool callbehaviors )
{
	FrmEditRecDetail::updateStatus(callbehaviors);
	if( pMenuRecordAdd->isEnabled() ) {
		pushInsertTable->setVisible(true);
	} else {
		pushInsertTable->setVisible(false);
	}
}


/*<<<<<FRMEDITLIQUIDACIONCOMPRADET_FIN*/
} // namespace deposit
} // namespace gong
/*>>>>>FRMEDITLIQUIDACIONCOMPRADET_FIN*/

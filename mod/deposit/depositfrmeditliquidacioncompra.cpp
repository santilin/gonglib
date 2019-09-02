/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de liquidaciones de compras
// FIELD Fecha date - cabecera
// FIELD TipoDoc_ID Reference(factu::TipoDoc,Codigo,Nombre) - cabecera
// FIELD Contador int - cabecera
// FIELD Numero string - cabecera
// FIELD Proveedora_ID Reference(factu::Proveedora,Codigo,RazonSocial,dbRecord::InsertIfNotFound) - cabecera2
// FIELD IVADetallado comboint - cabecera2
// FIELD LiquidacionCompraDet FrmEditRecDetail
// FIELD SumaImportes money - noaddrightSumaImportes
// FIELD DtoP100 double - noaddrightDtoP100
// FIELD Descuento money - noaddrightDescuento
// FIELD BaseImponible money - noaddrightBaseImponible
// FIELD IVA money - noaddrightIVA
// FIELD RecargoEquivalencia money - noaddrightRecargoEquivalencia
// FIELD Total money - noaddrightTotal
// FIELD Notas text
// TYPE FrmEditRecMaster deposit::LiquidacionCompra Pedido Compra
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "depositfrmeditliquidacioncompra.h"
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_INCLUDES*/
#include <depositmodule.h>
#include <factumodule.h>
#include <facturecalbarancompra.h>
#include <facturecalbarancompradet.h>
#include <factufldivadetallado.h>

namespace gong {
namespace deposit {

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CONSTRUCTOR*/
FrmEditLiquidacionCompra::FrmEditLiquidacionCompra(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditLiquidacionCompra" );
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CONSTRUCTOR*/
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_INIT_CONTROLS*/
	QHBoxLayout *cabeceraLayout = new QHBoxLayout(0, 0, 6, "cabeceraLayout");
	QHBoxLayout *cabecera2Layout = new QHBoxLayout(0, 0, 6, "cabecera2Layout");
	QHBoxLayout *liquidacioncompradetLayout = new QHBoxLayout(0, 0, 6, "liquidacioncompradetLayout");
	QHBoxLayout *rightSumaImportesLayout = new QHBoxLayout(0, 0, 6, "rightSumaImportesLayout");
	QHBoxLayout *rightDtoP100Layout = new QHBoxLayout(0, 0, 6, "rightDtoP100Layout");
	QHBoxLayout *rightDescuentoLayout = new QHBoxLayout(0, 0, 6, "rightDescuentoLayout");
	QHBoxLayout *rightBaseImponibleLayout = new QHBoxLayout(0, 0, 6, "rightBaseImponibleLayout");
	QHBoxLayout *rightIVALayout = new QHBoxLayout(0, 0, 6, "rightIVALayout");
	QHBoxLayout *rightRecargoEquivalenciaLayout = new QHBoxLayout(0, 0, 6, "rightRecargoEquivalenciaLayout");
	QHBoxLayout *rightTotalLayout = new QHBoxLayout(0, 0, 6, "rightTotalLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editFecha = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "FECHA", cabeceraLayout );

	searchTipoDocCodigo = addSearchField( pControlsFrame, "TIPODOC_ID", "TIPODOC", "CODIGO", "NOMBRE", cabeceraLayout );
	pushTipoDocCodigo = searchTipoDocCodigo->getButton();
	connect( pushTipoDocCodigo, SIGNAL( clicked() ), this, SLOT( pushTipoDocCodigo_clicked() ) );
	editTipoDocCodigo = searchTipoDocCodigo->getEditCode();
	editTipoDocNombre = searchTipoDocCodigo->getEditDesc();
	editContador = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "CONTADOR", cabeceraLayout );
	editNumero = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "NUMERO", cabeceraLayout );

	searchProveedoraCodigo = addSearchField( pControlsFrame, "PROVEEDORA_ID", "PROVEEDORA", "CODIGO", "RAZONSOCIAL", cabecera2Layout );
	pushProveedoraCodigo = searchProveedoraCodigo->getButton();
	connect( pushProveedoraCodigo, SIGNAL( clicked() ), this, SLOT( pushProveedoraCodigo_clicked() ) );
	editProveedoraCodigo = searchProveedoraCodigo->getEditCode();
	editProveedoraRazonSocial = searchProveedoraCodigo->getEditDesc();
	comboIVADetallado = addComboIntField( pControlsFrame, "LIQUIDACIONCOMPRA", "IVADETALLADO", cabecera2Layout );

// frmDetails: LiquidacionCompraDet
	QFrame *liquidacioncompradetFrame = new QFrame(this);
	liquidacioncompradetFrame->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	liquidacioncompradetFrame->setLineWidth(4);
	QVBoxLayout *liquidacioncompradetFrameLayout = new QVBoxLayout(liquidacioncompradetFrame);
	RecLiquidacionCompraDet *liquidacioncompradet = getRecLiquidacionCompra()->getRecLiquidacionCompraDet(-1);
	dbViewDefinitionDict liquidacioncompradetviews;
	DBAPP->getDatabase()->getViewsForTable( liquidacioncompradet, liquidacioncompradetviews );
	dbRecordListDataModel *liquidacioncompradetdm =
		new dbRecordListDataModel(getRecLiquidacionCompra()->getListLiquidacionCompraDet(), liquidacioncompradet, liquidacioncompradetviews, "1=0");
	pFrmLiquidacionCompraDet = static_cast<FrmEditLiquidacionCompraDet *>(
		DBAPP->createEditDetailForm(this,
		-1, liquidacioncompradet, "LIQUIDACIONCOMPRADET",liquidacioncompradetdm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		pControlsFrame) );
	pFrmLiquidacionCompraDet->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
	liquidacioncompradetFrameLayout->addWidget( pFrmLiquidacionCompraDet );
	liquidacioncompradetLayout->addWidget(liquidacioncompradetFrame);
	editSumaImportes = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "SUMAIMPORTES", rightSumaImportesLayout );
	editDtoP100 = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "DTOP100", rightDtoP100Layout );
	editDescuento = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "DESCUENTO", rightDescuentoLayout );
	editBaseImponible = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "BASEIMPONIBLE", rightBaseImponibleLayout );
	editIVA = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "IVA", rightIVALayout );
	editRecargoEquivalencia = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "RECARGOEQUIVALENCIA", rightRecargoEquivalenciaLayout );
	editTotal = addEditField( pControlsFrame, "LIQUIDACIONCOMPRA", "TOTAL", rightTotalLayout );
	editNotas = addTextField( pControlsFrame, "LIQUIDACIONCOMPRA", "NOTAS", notasLayout );
	pControlsLayout->addLayout( cabeceraLayout );
	pControlsLayout->addLayout( cabecera2Layout );
	pControlsLayout->addLayout( liquidacioncompradetLayout );
	alignLayout( rightSumaImportesLayout, false);
	alignLayout( rightDtoP100Layout, false);
	alignLayout( rightDescuentoLayout, false);
	alignLayout( rightBaseImponibleLayout, false);
	alignLayout( rightIVALayout, false);
	alignLayout( rightRecargoEquivalenciaLayout, false);
	alignLayout( rightTotalLayout, false);
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_INIT_CONTROLS*/

	QGridLayout *gridlayout = new QGridLayout();
	gridlayout->addLayout( rightSumaImportesLayout, 0, 1 );
	gridlayout->addLayout( rightDtoP100Layout, 0, 2 );
	gridlayout->addLayout( rightDescuentoLayout, 0, 3 );
	gridlayout->addLayout( rightBaseImponibleLayout, 1, 0 );
	gridlayout->addLayout( rightIVALayout, 1, 1 );
	gridlayout->addLayout( rightRecargoEquivalenciaLayout, 1, 2 );
	gridlayout->addLayout( rightTotalLayout, 1, 3 );
	pControlsLayout->addLayout( gridlayout );

	editNumero->setMustBeReadOnly( true );
	editSumaImportes->setMustBeReadOnly( true );
	editBaseImponible->setMustBeReadOnly( true );
	editRecargoEquivalencia->setMustBeReadOnly( true );
	editIVA->setMustBeReadOnly( true );
 	editProveedoraCodigo->setWidthInChars(8);
	editNotas->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void FrmEditLiquidacionCompra::scatterFields()
{
	if( isDuplicating() ) {
		getRecLiquidacionCompra()->setValue( "NUMERO", "" );
		getRecLiquidacionCompra()->setValue( "CONTADOR", 0 );
	}
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_SCATTER*/
	editFecha->setText(getRecLiquidacionCompra()->getValue("FECHA").toDate());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFecha;
	editContador->setText(getRecLiquidacionCompra()->getValue("CONTADOR").toInt());
	editNumero->setText(getRecLiquidacionCompra()->getValue("NUMERO").toString());
	comboIVADetallado->setCurrentItemByValue(getRecLiquidacionCompra()->getValue("IVADETALLADO").toInt());
	editSumaImportes->setText(getRecLiquidacionCompra()->getValue("SUMAIMPORTES").toMoney());
	editDtoP100->setText(getRecLiquidacionCompra()->getValue("DTOP100").toDouble());
	editDescuento->setText(getRecLiquidacionCompra()->getValue("DESCUENTO").toMoney());
	editBaseImponible->setText(getRecLiquidacionCompra()->getValue("BASEIMPONIBLE").toMoney());
	editIVA->setText(getRecLiquidacionCompra()->getValue("IVA").toMoney());
	editRecargoEquivalencia->setText(getRecLiquidacionCompra()->getValue("RECARGOEQUIVALENCIA").toMoney());
	editTotal->setText(getRecLiquidacionCompra()->getValue("TOTAL").toMoney());
	editNotas->setText(getRecLiquidacionCompra()->getValue("NOTAS").toString());
	scatterTipoDoc();
	scatterProveedora();
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_SCATTER*/
	if( isInserting() ) {
		if( !isDuplicating() && editFecha->toDate().isNull() )
			editFecha->setText( factu::ModuleInstance->getWorkingDate() );
		empresa::EmpresaModule *em = static_cast<empresa::EmpresaModule *>(DBAPP->findModule("empresa"));
		if( em && em->getRecEmpresa() ) {
			if( em->getRecEmpresa()->getValue("RECARGOEQUIVALENCIA").toBool() )
				comboIVADetallado->setCurrentItemByValue( factu::FldIVADetallado::con_recargo );
		}
		if( isDuplicating() ) {
			editTipoDocCodigo->setJustEdited( true );
			validateFields( editTipoDocCodigo, 0 );
		}
	} else if( isUpdating() ) {
		pFocusWidget = pFrmLiquidacionCompraDet;
	}
	pFrmLiquidacionCompraDet->addDetailIfNeeded();
}

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CABECERA_GENNUMDOC$s/ModuleInstance->/factu::ModuleInstance->/$*/
void FrmEditLiquidacionCompra::genNumeroDocumento()
{
		editContador->setText( factu::ModuleInstance->getMaxContador( "LIQUIDACIONCOMPRA",
			getRecord()->getValue("EMPRESA_ID").toInt(),
			factu::ModuleInstance->getEmpresaModule()->getEjercicio(),
			getRecTipoDoc()->getValue("SERIE").toString() ) );
		editContador->setJustEdited( true );
		validateFields( editContador, 0 );
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CABECERA_GENNUMDOC*/
}


void FrmEditLiquidacionCompra::gatherFields()
{
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_GATHER*/
	getRecLiquidacionCompra()->setValue( "FECHA", editFecha->toDate());
	getRecLiquidacionCompra()->setValue( "TIPODOC_ID", getRecTipoDoc()->getRecordID() );
	getRecLiquidacionCompra()->setValue( "CONTADOR", editContador->toInt());
	getRecLiquidacionCompra()->setValue( "NUMERO", editNumero->toString());
	getRecLiquidacionCompra()->setValue( "PROVEEDORA_ID", getRecProveedora()->getRecordID() );
	getRecLiquidacionCompra()->setValue( "IVADETALLADO", comboIVADetallado->getCurrentItemValue());
	getRecLiquidacionCompra()->setValue( "SUMAIMPORTES", editSumaImportes->toMoney());
	getRecLiquidacionCompra()->setValue( "DTOP100", editDtoP100->toDouble());
	getRecLiquidacionCompra()->setValue( "DESCUENTO", editDescuento->toMoney());
	getRecLiquidacionCompra()->setValue( "BASEIMPONIBLE", editBaseImponible->toMoney());
	getRecLiquidacionCompra()->setValue( "IVA", editIVA->toMoney());
	getRecLiquidacionCompra()->setValue( "RECARGOEQUIVALENCIA", editRecargoEquivalencia->toMoney());
	getRecLiquidacionCompra()->setValue( "TOTAL", editTotal->toMoney());
	getRecLiquidacionCompra()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_GATHER*/
	factu::ModuleInstance->setWorkingDate( editFecha->toDate() );
}

void FrmEditLiquidacionCompra::scatterTipoDoc()
{
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_SCATTER_TIPODOC*/
	editTipoDocCodigo->setText( getRecTipoDoc()->getValue("CODIGO") );
	editTipoDocNombre->setText( getRecTipoDoc()->getValue("NOMBRE") );
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_SCATTER_TIPODOC*/
	if( editTipoDocCodigo->isJustEdited() ) {
		if( getRecTipoDoc()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecTipoDoc()->getValue("IVADetallado").toInt() );
		genNumeroDocumento();
	}
}

void FrmEditLiquidacionCompra::pushTipoDocCodigo_clicked()
{
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchTipoDocCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editTipoDocCodigo->setJustEdited( false );
			editTipoDocCodigo->setCancelling();
			if( DBAPP->choose(this, getRecTipoDoc(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterTipoDoc();
				editTipoDocCodigo->setJustEdited( true );
				setWiseFocus(editTipoDocCodigo);
			}
			break;
		case 'M':
			{
				if( getRecTipoDoc()->getRecordID() ) {
					editTipoDocCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecTipoDoc(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editTipoDocCodigo->setJustEdited( true );
						scatterTipoDoc();
					}
				setWiseFocus(editTipoDocCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecTipoDoc()->getRecordID() != 0 ) {
					editTipoDocCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecTipoDoc(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				factu::RecTipoDoc *tmprec = static_cast<factu::RecTipoDoc *>(DBAPP->createRecord( "TipoDoc" ));
				editTipoDocCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editTipoDocCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editTipoDocCodigo->setJustEdited( true );
					getRecTipoDoc()->copyRecord( tmprec );
					scatterTipoDoc();
				}
				setWiseFocus(editTipoDocCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_PUSH_TIPODOC_CODIGO_CLICKED*/
}

void FrmEditLiquidacionCompra::scatterProveedora()
{
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_SCATTER_PROVEEDORA*/
	editProveedoraCodigo->setText( getRecProveedora()->getValue("CODIGO") );
	editProveedoraRazonSocial->setText( getRecProveedora()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_SCATTER_PROVEEDORA*/
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CABECERA_SCATTER_PROVEEDORA*/
	if( editProveedoraCodigo->isJustEdited() ) {
		if( getRecProveedora()->getValue("IVADetallado").toInt() != 0 )
			comboIVADetallado->setCurrentItemByValue( getRecProveedora()->getValue("IVADetallado").toInt() );
	}
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CABECERA_SCATTER_PROVEEDORA*/
}

void FrmEditLiquidacionCompra::pushProveedoraCodigo_clicked()
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchProveedoraCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editProveedoraCodigo->setJustEdited( false );
			editProveedoraCodigo->setCancelling();
			if( DBAPP->choose(this, getRecProveedora(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterProveedora();
				editProveedoraCodigo->setJustEdited( true );
				setWiseFocus(editProveedoraCodigo);
			}
			break;
		case 'M':
			{
				if( getRecProveedora()->getRecordID() ) {
					editProveedoraCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecProveedora(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editProveedoraCodigo->setJustEdited( true );
						scatterProveedora();
					}
				setWiseFocus(editProveedoraCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecProveedora()->getRecordID() != 0 ) {
					editProveedoraCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecProveedora(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				factu::RecProveedora *tmprec = static_cast<factu::RecProveedora *>(DBAPP->createRecord( "Proveedora" ));
				editProveedoraCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editProveedoraCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editProveedoraCodigo->setJustEdited( true );
					getRecProveedora()->copyRecord( tmprec );
					scatterProveedora();
				}
				setWiseFocus(editProveedoraCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_PUSH_PROVEEDORA_CODIGO_CLICKED*/
}

void FrmEditLiquidacionCompra::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editTipoDocCodigo )
		pushTipoDocCodigo_clicked();
	if( sender == editProveedoraCodigo )
		pushProveedoraCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_SPECIALACTION*/
}

void FrmEditLiquidacionCompra::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITLIQUIDACIONCOMPRA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( focusWidget() != pushTipoDocCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTipoDocCodigo, editTipoDocNombre,
		getRecTipoDoc(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTipoDoc();
	if( focusWidget() != pushProveedoraCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProveedoraCodigo, editProveedoraRazonSocial,
		getRecProveedora(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterProveedora();
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_VALIDATE*/
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CABECERA_VALIDATE*/
	if( sender == editDtoP100 && editDtoP100->isJustEdited() ) {
		editDescuento->setText( 0.0 );
		actTotales();
	}
	if( sender == comboIVADetallado ) {
		if( comboIVADetallado->getCurrentItemValue() == factu::FldIVADetallado::con_recargo ) {
			editRecargoEquivalencia->setVisible( true );
			editRecargoEquivalencia->getLabel()->setVisible( true );
		} else {
			editRecargoEquivalencia->setVisible( false );
			editRecargoEquivalencia->getLabel()->setVisible( false );
		}
		actTotales();
	}
	if( sender == editFecha || !sender ) {
		if( editFecha->toDate().getYear() != factu::ModuleInstance->getEmpresaModule()->getEjercicio() ) {
			validresult->addWarning( Xtring::printf(_("La fecha no es del ejercicio actual: %d"),
				factu::ModuleInstance->getEmpresaModule()->getEjercicio()),
				"FECHA" );
		}
	}
	if( sender == editTotal && editTotal->isJustEdited() ) {
		if( editBaseImponible->toDouble() != 0.0 ) {
			double nuevo_total = editTotal->toDouble();
			editDtoP100->setText( 0.0 );
			editDescuento->setText( 0.0 );
			actTotales();
			double orig_total = editTotal->toDouble();
			if( orig_total != nuevo_total ) {
				double orig_sumaimp = editSumaImportes->toDouble();
				double nuevo_sumaimp =  (nuevo_total * orig_sumaimp) / orig_total;
				double dtop100 = 100 - (100 * nuevo_sumaimp / orig_sumaimp);
				editDtoP100->setText( dtop100 );
				actTotales();
			}
		}
	}
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CABECERA_VALIDATE*/
	if( sender == editContador ) {
		editNumero->setText( factu::RecTipoDoc::formatNumDocumento(
				empresa::ModuleInstance->getRecEmpresa()->getValue( "CODIGO" ).toInt(),
				empresa::ModuleInstance->getEjercicio(),
				editContador->toInt(), 0,
				getRecTipoDoc()->getValue("FORMATO").toString()) );
	}
	if( !sender || sender == editContador ) {
		if( !validCodeAndDesc( ( sender == editContador ? editNumero : sender ),
				*validresult, editNumero, 0, "NUMERO", "") )
			if( !sender )
				*isvalid = false;
	}
	if( sender == editTotal && editTotal->isJustEdited() ) {
		if( editBaseImponible->toDouble() != 0.0 ) {
			double nuevo_total = editTotal->toDouble();
			double viejo_total = editTotal->getPreviousValue().toDouble();
			if( nuevo_total != viejo_total ) {
				double viejo_sumaimp = editSumaImportes->toDouble();
				double nuevo_sumaimp =  (nuevo_total * viejo_sumaimp) / viejo_total;
				double dtop100 = 100 - (100 * nuevo_sumaimp / viejo_sumaimp);
				editDtoP100->setText( dtop100 );
				actTotales();
			}
		}
	}
	if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CABECERA_UPDATEFROMDETAILS*/
void FrmEditLiquidacionCompra::updateFromDetails( class FrmEditRecDetail *detailfrm )
{
	actTotales();
	numeraLineas();
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CABECERA_UPDATEFROMDETAILS*/
}

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CABECERA_NUMERALINEAS*/
void FrmEditLiquidacionCompra::numeraLineas()
{
	dbRecordList *reclst = getRecLiquidacionCompra()->getListLiquidacionCompraDet();
	for ( unsigned int i = 0; i < reclst->size(); i++ ) {
		RecLiquidacionCompraDet *detalle = static_cast<RecLiquidacionCompraDet *>( reclst->at( i ) );
		if( !detalle->isEmpty() ) // No numerar detalles vacíos 
			detalle->setValue( "NLINEA", i+1 );
	}
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CABECERA_NUMERALINEAS*/
}

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CABECERA_ACTTOTALES*/
void FrmEditLiquidacionCompra::actTotales()
{
	getRecLiquidacionCompra()->gather( this );
	getRecLiquidacionCompra()->actTotales();
	getRecLiquidacionCompra()->scatter( this );
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CABECERA_ACTTOTALES*/
}

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_FIN*/
} // namespace deposit
} // namespace gong
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_FIN*/


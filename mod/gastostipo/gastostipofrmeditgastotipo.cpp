/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de definiciones de gastos e ingresos
// FIELD EsGasto bool - gasto
// FIELD Nombre string - gasto
// FIELD Revisar bool - gasto
// FIELD CategoriaGasto comboint - tablename
// FIELD TableName combostring - tablename
// FIELD PedirTercero comboint - tercero
// FIELD Cliente_ID Reference(factu::Cliente,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - tercero if(hasFactu())
// FIELD Proveedora_ID Reference(factu::Proveedora,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - tercero if(hasFactu())
// FIELD CuentaTercero_ID Reference(contab::Cuenta,Cuenta,Descripcion) - tercero if(hasContab())
// FIELD PedirConcepto comboint - concepto
// FIELD Articulo_ID Reference(factu::Articulo,Codigo,Nombre,dbApplication::FindCodeInDesc) - concepto if(hasFactu())
// FIELD CuentaConcepto_ID Reference(contab::Cuenta,Cuenta,Descripcion) - concepto if(hasContab())
// FIELD PedirImporte comboint - leftImporte
// FIELD Importe money - leftImporte
// FIELD PedirDescripcion comboint - leftDesc
// FIELD Descripcion string - leftDesc
// FIELD PedirNotas comboint - leftNotas
// FIELD Notas text - leftNotas
// TYPE FrmEditRecMaster gastostipo::GastoTipo
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITGASTOTIPO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "gastostipofrmeditgastotipo.h"
/*>>>>>FRMEDITGASTOTIPO_INCLUDES*/
#include <dbappfrmcustom.h>
#include <factureccliente.h>
#include <facturecproveedora.h>
#include <facturecarticulo.h>
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include <contabreccuenta.h>
#endif

namespace gong {
namespace gastostipo {

/*<<<<<FRMEDITGASTOTIPO_CONSTRUCTOR*/
FrmEditGastoTipo::FrmEditGastoTipo(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditGastoTipo" );
/*>>>>>FRMEDITGASTOTIPO_CONSTRUCTOR*/
/*<<<<<FRMEDITGASTOTIPO_INIT_CONTROLS*/
	QHBoxLayout *gastoLayout = new QHBoxLayout(0, 0, 6, "gastoLayout");
	QHBoxLayout *tablenameLayout = new QHBoxLayout(0, 0, 6, "tablenameLayout");
	QHBoxLayout *terceroLayout = new QHBoxLayout(0, 0, 6, "terceroLayout");
	QHBoxLayout *conceptoLayout = new QHBoxLayout(0, 0, 6, "conceptoLayout");
	QHBoxLayout *leftImporteLayout = new QHBoxLayout(0, 0, 6, "leftImporteLayout");
	QHBoxLayout *leftDescLayout = new QHBoxLayout(0, 0, 6, "leftDescLayout");
	QHBoxLayout *leftNotasLayout = new QHBoxLayout(0, 0, 6, "leftNotasLayout");
	checkEsGasto = addCheckField( pControlsFrame, "GASTOTIPO", "ESGASTO", gastoLayout );
	editNombre = addEditField( pControlsFrame, "GASTOTIPO", "NOMBRE", gastoLayout );
	checkRevisar = addCheckField( pControlsFrame, "GASTOTIPO", "REVISAR", gastoLayout );
	comboCategoriaGasto = addComboIntField( pControlsFrame, "GASTOTIPO", "CATEGORIAGASTO", tablenameLayout );
	comboTableName = addComboXtringField( pControlsFrame, "GASTOTIPO", "TABLENAME", tablenameLayout );
	comboPedirTercero = addComboIntField( pControlsFrame, "GASTOTIPO", "PEDIRTERCERO", terceroLayout );

if(hasFactu()){
	searchClienteCodigo = addSearchField( pControlsFrame, "CLIENTE_ID", "CLIENTE", "CODIGO", "RAZONSOCIAL", terceroLayout );
	pushClienteCodigo = searchClienteCodigo->getButton();
	connect( pushClienteCodigo, SIGNAL( clicked() ), this, SLOT( pushClienteCodigo_clicked() ) );
	editClienteCodigo = searchClienteCodigo->getEditCode();
	editClienteRazonSocial = searchClienteCodigo->getEditDesc();
}

if(hasFactu()){
	searchProveedoraCodigo = addSearchField( pControlsFrame, "PROVEEDORA_ID", "PROVEEDORA", "CODIGO", "RAZONSOCIAL", terceroLayout );
	pushProveedoraCodigo = searchProveedoraCodigo->getButton();
	connect( pushProveedoraCodigo, SIGNAL( clicked() ), this, SLOT( pushProveedoraCodigo_clicked() ) );
	editProveedoraCodigo = searchProveedoraCodigo->getEditCode();
	editProveedoraRazonSocial = searchProveedoraCodigo->getEditDesc();
}

if(hasContab()){
	searchCuentaTerceroCuenta = addSearchField( pControlsFrame, "CUENTATERCERO_ID", "CUENTA", "CUENTA", "DESCRIPCION", terceroLayout );
	pushCuentaTerceroCuenta = searchCuentaTerceroCuenta->getButton();
	connect( pushCuentaTerceroCuenta, SIGNAL( clicked() ), this, SLOT( pushCuentaTerceroCuenta_clicked() ) );
	editCuentaTerceroCuenta = searchCuentaTerceroCuenta->getEditCode();
	editCuentaTerceroDescripcion = searchCuentaTerceroCuenta->getEditDesc();
}
	comboPedirConcepto = addComboIntField( pControlsFrame, "GASTOTIPO", "PEDIRCONCEPTO", conceptoLayout );

if(hasFactu()){
	searchArticuloCodigo = addSearchField( pControlsFrame, "ARTICULO_ID", "ARTICULO", "CODIGO", "NOMBRE", conceptoLayout );
	pushArticuloCodigo = searchArticuloCodigo->getButton();
	connect( pushArticuloCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloCodigo_clicked() ) );
	editArticuloCodigo = searchArticuloCodigo->getEditCode();
	editArticuloNombre = searchArticuloCodigo->getEditDesc();
}

if(hasContab()){
	searchCuentaConceptoCuenta = addSearchField( pControlsFrame, "CUENTACONCEPTO_ID", "CUENTA", "CUENTA", "DESCRIPCION", conceptoLayout );
	pushCuentaConceptoCuenta = searchCuentaConceptoCuenta->getButton();
	connect( pushCuentaConceptoCuenta, SIGNAL( clicked() ), this, SLOT( pushCuentaConceptoCuenta_clicked() ) );
	editCuentaConceptoCuenta = searchCuentaConceptoCuenta->getEditCode();
	editCuentaConceptoDescripcion = searchCuentaConceptoCuenta->getEditDesc();
}
	comboPedirImporte = addComboIntField( pControlsFrame, "GASTOTIPO", "PEDIRIMPORTE", leftImporteLayout );
	editImporte = addEditField( pControlsFrame, "GASTOTIPO", "IMPORTE", leftImporteLayout );
	comboPedirDescripcion = addComboIntField( pControlsFrame, "GASTOTIPO", "PEDIRDESCRIPCION", leftDescLayout );
	editDescripcion = addEditField( pControlsFrame, "GASTOTIPO", "DESCRIPCION", leftDescLayout );
	comboPedirNotas = addComboIntField( pControlsFrame, "GASTOTIPO", "PEDIRNOTAS", leftNotasLayout );
	editNotas = addTextField( pControlsFrame, "GASTOTIPO", "NOTAS", leftNotasLayout );
	pControlsLayout->addLayout( gastoLayout );
	pControlsLayout->addLayout( tablenameLayout );
	pControlsLayout->addLayout( terceroLayout );
	pControlsLayout->addLayout( conceptoLayout );
	pControlsLayout->addLayout( leftImporteLayout );
	alignLayout( leftImporteLayout, true );
	pControlsLayout->addLayout( leftDescLayout );
	alignLayout( leftDescLayout, true );
	pControlsLayout->addLayout( leftNotasLayout );
	alignLayout( leftNotasLayout, true );
/*>>>>>FRMEDITGASTOTIPO_INIT_CONTROLS*/
	pushUtilizar = new QPushButton( this );
	pushUtilizar->setText( toGUI( _("Utilizar" ) ) );
	pButtonsLayout->addWidget( pushUtilizar );
	connect( pushUtilizar, SIGNAL( clicked() ), this, SLOT( slotUtilizar() ) );
	comboPedirConcepto->getLabel()->setText( "" );
	comboPedirConcepto->setMaximumWidth( 100 );
// 	delete comboPedirDescripcion->getLabel();
	comboPedirDescripcion->setMaximumWidth( 100 );
// 	delete comboPedirImporte->getLabel();
	comboPedirImporte->setMaximumWidth( 100 );
// 	delete comboPedirNotas->getLabel();
	comboPedirNotas->setMaximumWidth( 100 );
// 	delete comboPedirTercero->getLabel();
	comboPedirTercero->setMaximumWidth( 100 );
}

void FrmEditGastoTipo::updateStatus( bool callbehaviors )
{
	FrmEditRecMaster::updateStatus( callbehaviors );
	pushUtilizar->setEnabled( isBrowsing() );
}

void FrmEditGastoTipo::scatterFields()
{
/*<<<<<FRMEDITGASTOTIPO_SCATTER*/
	checkEsGasto->setChecked(getRecGastoTipo()->getValue("ESGASTO").toBool());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = checkEsGasto;
	editNombre->setText(getRecGastoTipo()->getValue("NOMBRE").toString());
	checkRevisar->setChecked(getRecGastoTipo()->getValue("REVISAR").toBool());
	comboCategoriaGasto->setCurrentItemByValue(getRecGastoTipo()->getValue("CATEGORIAGASTO").toInt());
	comboTableName->setText(getRecGastoTipo()->getValue("TABLENAME").toString());
	comboPedirTercero->setCurrentItemByValue(getRecGastoTipo()->getValue("PEDIRTERCERO").toInt());
	comboPedirConcepto->setCurrentItemByValue(getRecGastoTipo()->getValue("PEDIRCONCEPTO").toInt());
	comboPedirImporte->setCurrentItemByValue(getRecGastoTipo()->getValue("PEDIRIMPORTE").toInt());
	editImporte->setText(getRecGastoTipo()->getValue("IMPORTE").toMoney());
	comboPedirDescripcion->setCurrentItemByValue(getRecGastoTipo()->getValue("PEDIRDESCRIPCION").toInt());
	editDescripcion->setText(getRecGastoTipo()->getValue("DESCRIPCION").toString());
	comboPedirNotas->setCurrentItemByValue(getRecGastoTipo()->getValue("PEDIRNOTAS").toInt());
	editNotas->setText(getRecGastoTipo()->getValue("NOTAS").toString());
if(hasFactu()){
	scatterCliente();
}
if(hasFactu()){
	scatterProveedora();
}
if(hasContab()){
	scatterCuentaTercero();
}
if(hasFactu()){
	scatterArticulo();
}
if(hasContab()){
	scatterCuentaConcepto();
}
/*>>>>>FRMEDITGASTOTIPO_SCATTER*/
	if( isInserting() ) {
		comboPedirConcepto->setCurrentItemByValue( 1 );
		comboPedirDescripcion->setCurrentItemByValue( 1 );
		comboPedirImporte->setCurrentItemByValue( 1 );
		comboPedirNotas->setCurrentItemByValue( 1 );
		comboPedirTercero->setCurrentItemByValue( 1 );
	}
}

void FrmEditGastoTipo::gatherFields()
{
/*<<<<<FRMEDITGASTOTIPO_GATHER*/
	getRecGastoTipo()->setValue( "ESGASTO", checkEsGasto->isChecked());
	getRecGastoTipo()->setValue( "NOMBRE", editNombre->toString());
	getRecGastoTipo()->setValue( "REVISAR", checkRevisar->isChecked());
	getRecGastoTipo()->setValue( "CATEGORIAGASTO", comboCategoriaGasto->getCurrentItemValue());
	getRecGastoTipo()->setValue( "TABLENAME", comboTableName->toString());
	getRecGastoTipo()->setValue( "PEDIRTERCERO", comboPedirTercero->getCurrentItemValue());
if(hasFactu()){
	getRecGastoTipo()->setValue( "CLIENTE_ID", getRecCliente()->getRecordID() );
}
if(hasFactu()){
	getRecGastoTipo()->setValue( "PROVEEDORA_ID", getRecProveedora()->getRecordID() );
}
if(hasContab()){
	getRecGastoTipo()->setValue( "CUENTATERCERO_ID", getRecCuentaTercero()->getRecordID() );
}
	getRecGastoTipo()->setValue( "PEDIRCONCEPTO", comboPedirConcepto->getCurrentItemValue());
if(hasFactu()){
	getRecGastoTipo()->setValue( "ARTICULO_ID", getRecArticulo()->getRecordID() );
}
if(hasContab()){
	getRecGastoTipo()->setValue( "CUENTACONCEPTO_ID", getRecCuentaConcepto()->getRecordID() );
}
	getRecGastoTipo()->setValue( "PEDIRIMPORTE", comboPedirImporte->getCurrentItemValue());
	getRecGastoTipo()->setValue( "IMPORTE", editImporte->toMoney());
	getRecGastoTipo()->setValue( "PEDIRDESCRIPCION", comboPedirDescripcion->getCurrentItemValue());
	getRecGastoTipo()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecGastoTipo()->setValue( "PEDIRNOTAS", comboPedirNotas->getCurrentItemValue());
	getRecGastoTipo()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITGASTOTIPO_GATHER*/
}

void FrmEditGastoTipo::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
/*<<<<<FRMEDITGASTOTIPO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
if(hasFactu()){
	if( focusWidget() != pushClienteCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editClienteCodigo, editClienteRazonSocial,
		getRecCliente(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterCliente();
}
if(hasFactu()){
	if( focusWidget() != pushProveedoraCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProveedoraCodigo, editProveedoraRazonSocial,
		getRecProveedora(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterProveedora();
}
if(hasContab()){
	if( focusWidget() != pushCuentaTerceroCuenta) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaTerceroCuenta, editCuentaTerceroDescripcion,
		getRecCuentaTercero(), "CUENTA", "DESCRIPCION", Xtring::null) )
		scatterCuentaTercero();
}
if(hasFactu()){
	if( focusWidget() != pushArticuloCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editArticuloCodigo, editArticuloNombre,
		getRecArticulo(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::FindCodeInDesc )) )
		scatterArticulo();
}
if(hasContab()){
	if( focusWidget() != pushCuentaConceptoCuenta) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editCuentaConceptoCuenta, editCuentaConceptoDescripcion,
		getRecCuentaConcepto(), "CUENTA", "DESCRIPCION", Xtring::null) )
		scatterCuentaConcepto();
}
/*>>>>>FRMEDITGASTOTIPO_VALIDATE*/
	if( !ir ) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

enum Pedir { NOPEDIR = 1, PEDIR = 2, FIJAR = 3 };

void FrmEditGastoTipo::slotUtilizar()
{
	if( mMustRead )
		read();
	FrmCustom *frmgasto = new FrmCustom( this );
	frmgasto->setTitle( getRecord()->getValue( "NOMBRE" ).toString() );
	frmgasto->addInput( 0, "Fecha", Date::currentDate() );
	SearchBox *sbTercero = 0;
	Xtring tablename = getRecord()->getValue( "TABLENAME" ).toString();
	dbRecord *r = DBAPP->createRecord( tablename );
	switch( getRecord()->getValue( "PEDIRTERCERO" ).toInt() ) {
	case PEDIR:
		if( tablename == "ALBARANVENTA" || tablename == "FACTURAVENTA" )
			sbTercero = frmgasto->addSearchField( 0, "CLIENTE", "CODIGO", "RAZONSOCIAL" );
		else if( tablename == "ALBARANCOMPRA" || tablename == "FACTURACOMPRA" )
			sbTercero = frmgasto->addSearchField( 0, "PROVEEDORA", "CODIGO", "RAZONSOCIAL" );
	break;
	case FIJAR:
		if( tablename == "ALBARANVENTA" || tablename == "FACTURAVENTA" )
			r->setValue( "CLIENTE_ID", getRecord()->getValue( "CLIENTE_ID" ) );
		else if( tablename == "ALBARANCOMPRA" || tablename == "FACTURACOMPRA" )
			r->setValue( "PROVEEDORA_ID", getRecord()->getValue( "PROVEEDORA_ID" ) );
	}

	SearchBox *sbConcepto = 0;
	switch( getRecord()->getValue( "PEDIRCONCEPTO" ).toInt() ) {
	case PEDIR:
		if( tablename == "ALBARANVENTA" || tablename == "FACTURAVENTA"
			|| tablename == "ALBARANCOMPRA" || tablename == "FACTURACOMPRA" )
			sbConcepto = frmgasto->addSearchField( 0, "ARTICULO", "CODIGO", "NOMBRE" );
	break;
	case FIJAR:
		if( tablename == "ALBARANVENTA" || tablename == "FACTURAVENTA"
			|| tablename == "ALBARANCOMPRA" || tablename == "FACTURACOMPRA" )
			r->setValue( "ARTICULO_ID", getRecord()->getValue( "ARTICULO_ID" ) );
	}
	LineEdit *editImporte = 0;
	switch( getRecord()->getValue( "PEDIRIMPORTE" ).toInt() ) {
		case PEDIR:
			editImporte = frmgasto->addInput(0, "Importe", 0.0 );
			break;
	}
	frmgasto->showModalFor( this, true, true );
	if( !frmgasto->wasCancelled() ) {
		FrmEditRecMaster *frm = static_cast<FrmEditRecMaster *>
			( DBAPP->createEditForm( this, r, 0, DataTable::inserting, dbApplication::simpleEdition ) );
		frm->show();
		frm->updateFromDetails(0);
		frm->hide();
		DBAPP->getMainWindow()->createClient ( frm );
	}
}

bool FrmEditGastoTipo::hasContab() const
{
#ifdef HAVE_CONTABMODULE
	return !hasFactu() && contab::ModuleInstance->isContabActive();
#else
	return false;
#endif
}

bool FrmEditGastoTipo::hasFactu() const
{
	return true;
}

void FrmEditGastoTipo::scatterCliente()
{
/*<<<<<FRMEDITGASTOTIPO_SCATTER_CLIENTE*/
	editClienteCodigo->setText( getRecCliente()->getValue("CODIGO") );
	editClienteRazonSocial->setText( getRecCliente()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITGASTOTIPO_SCATTER_CLIENTE*/
}
void FrmEditGastoTipo::pushClienteCodigo_clicked()
{
/*<<<<<FRMEDITGASTOTIPO_PUSH_CLIENTE_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchClienteCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editClienteCodigo->setJustEdited( false );
			editClienteCodigo->setCancelling();
			if( DBAPP->choose(this, getRecCliente(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCliente();
				editClienteCodigo->setJustEdited( true );
				editClienteCodigo->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecCliente()->getRecordID() ) {
					editClienteCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCliente(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editClienteCodigo->setJustEdited( true );
						scatterCliente();
					}
					editClienteCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecCliente()->getRecordID() != 0 ) {
					editClienteCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCliente(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				factu::RecCliente *tmprec = static_cast<factu::RecCliente *>(DBAPP->createRecord( "Cliente" ));
				editClienteCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editClienteCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editClienteCodigo->setJustEdited( true );
					getRecCliente()->copyRecord( tmprec );
					scatterCliente();
				}
				editClienteCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITGASTOTIPO_PUSH_CLIENTE_CODIGO_CLICKED*/
}

void FrmEditGastoTipo::scatterProveedora()
{
/*<<<<<FRMEDITGASTOTIPO_SCATTER_PROVEEDORA*/
	editProveedoraCodigo->setText( getRecProveedora()->getValue("CODIGO") );
	editProveedoraRazonSocial->setText( getRecProveedora()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITGASTOTIPO_SCATTER_PROVEEDORA*/
}
void FrmEditGastoTipo::pushProveedoraCodigo_clicked()
{
/*<<<<<FRMEDITGASTOTIPO_PUSH_PROVEEDORA_CODIGO_CLICKED*/
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
				editProveedoraCodigo->setFocus();
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
					editProveedoraCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecProveedora()->getRecordID() != 0 ) {
					editProveedoraCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecProveedora(),
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
				editProveedoraCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITGASTOTIPO_PUSH_PROVEEDORA_CODIGO_CLICKED*/
}

void FrmEditGastoTipo::scatterCuentaTercero()
{
/*<<<<<FRMEDITGASTOTIPO_SCATTER_CUENTATERCERO*/
	editCuentaTerceroCuenta->setText( getRecCuentaTercero()->getValue("CUENTA") );
	editCuentaTerceroDescripcion->setText( getRecCuentaTercero()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITGASTOTIPO_SCATTER_CUENTATERCERO*/
}
void FrmEditGastoTipo::pushCuentaTerceroCuenta_clicked()
{
/*<<<<<FRMEDITGASTOTIPO_PUSH_CUENTATERCERO_CUENTA_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaTerceroCuenta->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaTerceroCuenta->setJustEdited( false );
			editCuentaTerceroCuenta->setCancelling();
			if( DBAPP->choose(this, getRecCuentaTercero(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaTercero();
				editCuentaTerceroCuenta->setJustEdited( true );
				editCuentaTerceroCuenta->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecCuentaTercero()->getRecordID() ) {
					editCuentaTerceroCuenta->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaTercero(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaTerceroCuenta->setJustEdited( true );
						scatterCuentaTercero();
					}
					editCuentaTerceroCuenta->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaTercero()->getRecordID() != 0 ) {
					editCuentaTerceroCuenta->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCuentaTercero(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				contab::RecCuentaTercero *tmprec = static_cast<contab::RecCuentaTercero *>(DBAPP->createRecord( "CuentaTercero" ));
				editCuentaTerceroCuenta->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaTerceroCuenta->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaTerceroCuenta->setJustEdited( true );
					getRecCuentaTercero()->copyRecord( tmprec );
					scatterCuentaTercero();
				}
				editCuentaTerceroCuenta->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITGASTOTIPO_PUSH_CUENTATERCERO_CUENTA_CLICKED*/
}

void FrmEditGastoTipo::scatterArticulo()
{
/*<<<<<FRMEDITGASTOTIPO_SCATTER_ARTICULO*/
	editArticuloCodigo->setText( getRecArticulo()->getValue("CODIGO") );
	editArticuloNombre->setText( getRecArticulo()->getValue("NOMBRE") );
/*>>>>>FRMEDITGASTOTIPO_SCATTER_ARTICULO*/
}
void FrmEditGastoTipo::pushArticuloCodigo_clicked()
{
/*<<<<<FRMEDITGASTOTIPO_PUSH_ARTICULO_CODIGO_CLICKED*/
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
				editArticuloCodigo->setFocus();
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
					editArticuloCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecArticulo()->getRecordID() != 0 ) {
					editArticuloCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecArticulo(),
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
				editArticuloCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITGASTOTIPO_PUSH_ARTICULO_CODIGO_CLICKED*/
}

void FrmEditGastoTipo::scatterCuentaConcepto()
{
/*<<<<<FRMEDITGASTOTIPO_SCATTER_CUENTACONCEPTO*/
	editCuentaConceptoCuenta->setText( getRecCuentaConcepto()->getValue("CUENTA") );
	editCuentaConceptoDescripcion->setText( getRecCuentaConcepto()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITGASTOTIPO_SCATTER_CUENTACONCEPTO*/
}
void FrmEditGastoTipo::pushCuentaConceptoCuenta_clicked()
{
/*<<<<<FRMEDITGASTOTIPO_PUSH_CUENTACONCEPTO_CUENTA_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchCuentaConceptoCuenta->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editCuentaConceptoCuenta->setJustEdited( false );
			editCuentaConceptoCuenta->setCancelling();
			if( DBAPP->choose(this, getRecCuentaConcepto(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterCuentaConcepto();
				editCuentaConceptoCuenta->setJustEdited( true );
				editCuentaConceptoCuenta->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecCuentaConcepto()->getRecordID() ) {
					editCuentaConceptoCuenta->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecCuentaConcepto(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editCuentaConceptoCuenta->setJustEdited( true );
						scatterCuentaConcepto();
					}
					editCuentaConceptoCuenta->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecCuentaConcepto()->getRecordID() != 0 ) {
					editCuentaConceptoCuenta->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecCuentaConcepto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				contab::RecCuentaConcepto *tmprec = static_cast<contab::RecCuentaConcepto *>(DBAPP->createRecord( "CuentaConcepto" ));
				editCuentaConceptoCuenta->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editCuentaConceptoCuenta->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editCuentaConceptoCuenta->setJustEdited( true );
					getRecCuentaConcepto()->copyRecord( tmprec );
					scatterCuentaConcepto();
				}
				editCuentaConceptoCuenta->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITGASTOTIPO_PUSH_CUENTACONCEPTO_CUENTA_CLICKED*/
}
void FrmEditGastoTipo::specialControlKeyPressed(QWidget *sender, char key)
{
/*<<<<<FRMEDITGASTOTIPO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
if(hasFactu()){
	if( sender == editClienteCodigo )
		pushClienteCodigo_clicked();
}
if(hasFactu()){
	if( sender == editProveedoraCodigo )
		pushProveedoraCodigo_clicked();
}
if(hasContab()){
	if( sender == editCuentaTerceroCuenta )
		pushCuentaTerceroCuenta_clicked();
}
if(hasFactu()){
	if( sender == editArticuloCodigo )
		pushArticuloCodigo_clicked();
}
if(hasContab()){
	if( sender == editCuentaConceptoCuenta )
		pushCuentaConceptoCuenta_clicked();
}
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITGASTOTIPO_SPECIALACTION*/
}

factu::RecCliente* FrmEditGastoTipo::getRecCliente() const
{
	return static_cast<factu::RecCliente *>( getRecord()->findRelatedRecord( "CLIENTE_ID" ) );
}

factu::RecArticulo* FrmEditGastoTipo::getRecArticulo() const
{
	return static_cast<factu::RecArticulo *>( getRecord()->findRelatedRecord( "ARTICULO_ID" ) );
}

factu::RecProveedora* FrmEditGastoTipo::getRecProveedora() const
{
	return static_cast<factu::RecProveedora *>( getRecord()->findRelatedRecord( "PROVEEDORA_ID" ) );
}

contab::RecCuentaConcepto* FrmEditGastoTipo::getRecCuentaConcepto() const
{
	return static_cast<contab::RecCuentaConcepto *>( getRecord()->findRelatedRecord( "CUENTACONCEPTO_ID" ) );
}

contab::RecCuentaTercero* FrmEditGastoTipo::getRecCuentaTercero() const
{
	return static_cast<contab::RecCuentaTercero *>( getRecord()->findRelatedRecord( "CUENTATERCERO_ID" ) );
}


/*<<<<<FRMEDITGASTOTIPO_FIN*/
} // namespace gastostipo
} // namespace gong
/*>>>>>FRMEDITGASTOTIPO_FIN*/


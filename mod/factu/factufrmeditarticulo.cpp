/*<<<<<COPYLEFT*/
/** @file factufrmeditarticulo.cpp Fichero de edición de artículos
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
// COPYLEFT Fichero de edición de artículos
// FIELD Familia_ID Reference(Familia,Codigo,Nombre,dbApplication::InsertIfNotFound) - familia
// FIELD Proveedora_ID Reference(Proveedora,Codigo,RazonSocial,dbApplication::InsertIfNotFound) - proveedora
// FIELD Nombre string - nombre
// FIELD Codigo string - codigo
// FIELD TipoIVA_ID Reference(empresa::TipoIVA,Codigo,Nombre) - tipoiva
// FIELD Fabricante string - fabricante
// FIELD CodigoInterno string - codigointerno
// FIELD CosteSinIVA money - costes
// FIELD Coste money - costes
// FIELD MargenComercial double - costes
// FIELD PVPSinIVA money - pvps
// FIELD PVP money - pvps
// FIELD DtoComercial double - pvps
// FIELD PVP1 money - pvpn
// FIELD PVP2 money - pvpn
// FIELD PVP3 money - pvpn
// FIELD UsarStocks bool - Stock
// FIELD Stock double - Stock
// FIELD StockMaximo double - Stock
// FIELD StockMinimo double - Stock
// FIELD EnUso bool - flags
// FIELD Novedad bool - flags
// FIELD Oferta bool - flags
// FIELD Notas text tabExtra notas
// FIELD Descripcion RichTextBox tabExtra
// FIELD MultiploBase double tabExtra base
// FIELD Subcuenta string tabExtra subcuentas MODULE_INCLUDED(Contab)
// FIELD ArticuloImagen.Imagen image tabImagen imagenLayout
// TYPE FrmEditRecMaster factu::Articulo CodeNotFound
/*>>>>>MODULE_INFO*/

#include <gongimagebox.h>
/*<<<<<FRMEDITARTICULO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "factufrmeditarticulo.h"
/*>>>>>FRMEDITARTICULO_INCLUDES*/
#include "factumodule.h"
#include "factufrmgencodart.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITARTICULO_CONSTRUCTOR*/
FrmEditArticulo::FrmEditArticulo(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditArticulo" );
/*>>>>>FRMEDITARTICULO_CONSTRUCTOR*/

    /*<<<<<FRMEDITARTICULO_INIT_CONTROLS*/
	QHBoxLayout *familiaLayout = new QHBoxLayout(0, 0, 6, "familiaLayout");
	QHBoxLayout *proveedoraLayout = new QHBoxLayout(0, 0, 6, "proveedoraLayout");
	QHBoxLayout *nombreLayout = new QHBoxLayout(0, 0, 6, "nombreLayout");
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *tipoivaLayout = new QHBoxLayout(0, 0, 6, "tipoivaLayout");
	QHBoxLayout *fabricanteLayout = new QHBoxLayout(0, 0, 6, "fabricanteLayout");
	QHBoxLayout *codigointernoLayout = new QHBoxLayout(0, 0, 6, "codigointernoLayout");
	QHBoxLayout *costesLayout = new QHBoxLayout(0, 0, 6, "costesLayout");
	QHBoxLayout *pvpsLayout = new QHBoxLayout(0, 0, 6, "pvpsLayout");
	QHBoxLayout *pvpnLayout = new QHBoxLayout(0, 0, 6, "pvpnLayout");
	QHBoxLayout *StockLayout = new QHBoxLayout(0, 0, 6, "StockLayout");
	QHBoxLayout *flagsLayout = new QHBoxLayout(0, 0, 6, "flagsLayout");
	showTabs(true);
	QWidget *tabExtra = new QWidget( pTabWidget, "tabExtra" );
	QVBoxLayout *tabExtraLayout = new QVBoxLayout(tabExtra, 11, 6, "tabExtraLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	QHBoxLayout *descripcionLayout = new QHBoxLayout(0, 0, 6, "descripcionLayout");
	QHBoxLayout *baseLayout = new QHBoxLayout(0, 0, 6, "baseLayout");
	QHBoxLayout *subcuentasLayout = new QHBoxLayout(0, 0, 6, "subcuentasLayout");
	QWidget *tabImagen = new QWidget( pTabWidget, "tabImagen" );
	QVBoxLayout *tabImagenLayout = new QVBoxLayout(tabImagen, 11, 6, "tabImagenLayout");
	QHBoxLayout *imagenLayout = new QHBoxLayout(0, 0, 6, "imagenLayout");

	searchFamiliaCodigo = addSearchField( pControlsFrame, "FAMILIA_ID", "FAMILIA", "CODIGO", "NOMBRE", familiaLayout );
	pushFamiliaCodigo = searchFamiliaCodigo->getButton();
	connect( pushFamiliaCodigo, SIGNAL( clicked() ), this, SLOT( pushFamiliaCodigo_clicked() ) );
	editFamiliaCodigo = searchFamiliaCodigo->getEditCode();
	editFamiliaNombre = searchFamiliaCodigo->getEditDesc();

	searchProveedoraCodigo = addSearchField( pControlsFrame, "PROVEEDORA_ID", "PROVEEDORA", "CODIGO", "RAZONSOCIAL", proveedoraLayout );
	pushProveedoraCodigo = searchProveedoraCodigo->getButton();
	connect( pushProveedoraCodigo, SIGNAL( clicked() ), this, SLOT( pushProveedoraCodigo_clicked() ) );
	editProveedoraCodigo = searchProveedoraCodigo->getEditCode();
	editProveedoraRazonSocial = searchProveedoraCodigo->getEditDesc();
	editNombre = addEditField( pControlsFrame, "ARTICULO", "NOMBRE", nombreLayout );
	editCodigo = addEditField( pControlsFrame, "ARTICULO", "CODIGO", codigoLayout );

	searchTipoIVACodigo = addSearchField( pControlsFrame, "TIPOIVA_ID", "TIPOIVA", "CODIGO", "NOMBRE", tipoivaLayout );
	pushTipoIVACodigo = searchTipoIVACodigo->getButton();
	connect( pushTipoIVACodigo, SIGNAL( clicked() ), this, SLOT( pushTipoIVACodigo_clicked() ) );
	editTipoIVACodigo = searchTipoIVACodigo->getEditCode();
	editTipoIVANombre = searchTipoIVACodigo->getEditDesc();
	editFabricante = addEditField( pControlsFrame, "ARTICULO", "FABRICANTE", fabricanteLayout );
	editCodigoInterno = addEditField( pControlsFrame, "ARTICULO", "CODIGOINTERNO", codigointernoLayout );
	editCosteSinIVA = addEditField( pControlsFrame, "ARTICULO", "COSTESINIVA", costesLayout );
	editCoste = addEditField( pControlsFrame, "ARTICULO", "COSTE", costesLayout );
	editMargenComercial = addEditField( pControlsFrame, "ARTICULO", "MARGENCOMERCIAL", costesLayout );
	editPVPSinIVA = addEditField( pControlsFrame, "ARTICULO", "PVPSINIVA", pvpsLayout );
	editPVP = addEditField( pControlsFrame, "ARTICULO", "PVP", pvpsLayout );
	editDtoComercial = addEditField( pControlsFrame, "ARTICULO", "DTOCOMERCIAL", pvpsLayout );
	editPVP1 = addEditField( pControlsFrame, "ARTICULO", "PVP1", pvpnLayout );
	editPVP2 = addEditField( pControlsFrame, "ARTICULO", "PVP2", pvpnLayout );
	editPVP3 = addEditField( pControlsFrame, "ARTICULO", "PVP3", pvpnLayout );
	checkUsarStocks = addCheckField( pControlsFrame, "ARTICULO", "USARSTOCKS", StockLayout );
	editStock = addEditField( pControlsFrame, "ARTICULO", "STOCK", StockLayout );
	editStockMaximo = addEditField( pControlsFrame, "ARTICULO", "STOCKMAXIMO", StockLayout );
	editStockMinimo = addEditField( pControlsFrame, "ARTICULO", "STOCKMINIMO", StockLayout );
	checkEnUso = addCheckField( pControlsFrame, "ARTICULO", "ENUSO", flagsLayout );
	checkNovedad = addCheckField( pControlsFrame, "ARTICULO", "NOVEDAD", flagsLayout );
	checkOferta = addCheckField( pControlsFrame, "ARTICULO", "OFERTA", flagsLayout );
	editNotas = addTextField( tabExtra, "ARTICULO", "NOTAS", notasLayout );
	editDescripcion = addRichTextField( tabExtra, "ARTICULO", "DESCRIPCION", descripcionLayout );
	editMultiploBase = addEditField( tabExtra, "ARTICULO", "MULTIPLOBASE", baseLayout );
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuenta = addEditField( tabExtra, "ARTICULO", "SUBCUENTA", subcuentasLayout );
}
#endif
	editArticuloImagen_Imagen = addImageField( tabImagen, "ARTICULOIMAGEN", "IMAGEN", imagenLayout );
	pControlsLayout->addLayout( familiaLayout );
	pControlsLayout->addLayout( proveedoraLayout );
	pControlsLayout->addLayout( nombreLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( tipoivaLayout );
	pControlsLayout->addLayout( fabricanteLayout );
	pControlsLayout->addLayout( codigointernoLayout );
	pControlsLayout->addLayout( costesLayout );
	pControlsLayout->addLayout( pvpsLayout );
	pControlsLayout->addLayout( pvpnLayout );
	pControlsLayout->addLayout( StockLayout );
	pControlsLayout->addLayout( flagsLayout );
	tabExtraLayout->addLayout( notasLayout );
	tabExtraLayout->addLayout( descripcionLayout );
	tabExtraLayout->addLayout( baseLayout );
	tabExtraLayout->addLayout( subcuentasLayout );
	tabImagenLayout->addLayout( imagenLayout );
/*>>>>>FRMEDITARTICULO_INIT_CONTROLS*/
    pExtraLayout = tabExtraLayout;
    pRecArticuloBase = static_cast<RecArticulo *>( DBAPP->createRecord( "ARTICULO" ) );
    searchArticuloBaseCodigo = addSearchField( tabExtra, "ARTICULOBASE_ID", "ARTICULO", "CODIGO", "NOMBRE", baseLayout );
    pushArticuloBaseCodigo = searchArticuloBaseCodigo->getButton();
    connect( pushArticuloBaseCodigo, SIGNAL( clicked() ), this, SLOT( pushArticuloBaseCodigo_clicked() ) );
    editArticuloBaseCodigo = searchArticuloBaseCodigo->getEditCode();
    editArticuloBaseNombre = searchArticuloBaseCodigo->getEditDesc();

    pushGenCodArt = new QPushButton(this, "pushGenCodArt" );
    pushGenCodArt->setText( toGUI( _("Generar código") ) );
    connect( pushGenCodArt, SIGNAL( clicked() ), this, SLOT( pushGenCodArt_clicked() ) );
    codigoLayout->addWidget( pushGenCodArt );
    connect( editArticuloImagen_Imagen, SIGNAL( clicked() ), this, SLOT( editArticuloImagen_clicked() ) );
    editCodigo->setSelectedOnEntry( true );
    editNombre->setSelectedOnEntry( true );
    editStock->setMustBeReadOnly( true );
    editNotas->setHeightInLines(5);
    pTabWidget->insertTab( tabExtra, toGUI( _( "&Extra" ) ) );
    pTabWidget->insertTab( tabImagen, toGUI( _( "&Imagen" ) ) );
}

FrmEditArticulo::~FrmEditArticulo()
{
    delete pRecArticuloBase;
}


void FrmEditArticulo::scatterFields()
{
    if( !edited() )
        getRecArticulo()->fixPrecios();
    /*<<<<<FRMEDITARTICULO_SCATTER*/
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editFamiliaCodigo;
	editNombre->setText(getRecArticulo()->getValue("NOMBRE").toString());
	editCodigo->setText(getRecArticulo()->getValue("CODIGO").toString());
	editFabricante->setText(getRecArticulo()->getValue("FABRICANTE").toString());
	editCodigoInterno->setText(getRecArticulo()->getValue("CODIGOINTERNO").toString());
	editCosteSinIVA->setText(getRecArticulo()->getValue("COSTESINIVA").toMoney());
	editCoste->setText(getRecArticulo()->getValue("COSTE").toMoney());
	editMargenComercial->setText(getRecArticulo()->getValue("MARGENCOMERCIAL").toDouble());
	editPVPSinIVA->setText(getRecArticulo()->getValue("PVPSINIVA").toMoney());
	editPVP->setText(getRecArticulo()->getValue("PVP").toMoney());
	editDtoComercial->setText(getRecArticulo()->getValue("DTOCOMERCIAL").toDouble());
	editPVP1->setText(getRecArticulo()->getValue("PVP1").toMoney());
	editPVP2->setText(getRecArticulo()->getValue("PVP2").toMoney());
	editPVP3->setText(getRecArticulo()->getValue("PVP3").toMoney());
	checkUsarStocks->setChecked(getRecArticulo()->getValue("USARSTOCKS").toBool());
	editStock->setText(getRecArticulo()->getValue("STOCK").toDouble());
	editStockMaximo->setText(getRecArticulo()->getValue("STOCKMAXIMO").toDouble());
	editStockMinimo->setText(getRecArticulo()->getValue("STOCKMINIMO").toDouble());
	checkEnUso->setChecked(getRecArticulo()->getValue("ENUSO").toBool());
	checkNovedad->setChecked(getRecArticulo()->getValue("NOVEDAD").toBool());
	checkOferta->setChecked(getRecArticulo()->getValue("OFERTA").toBool());
	editNotas->setText(getRecArticulo()->getValue("NOTAS").toString());
	editDescripcion->setText(getRecArticulo()->getValue("DESCRIPCION").toString());
	editMultiploBase->setText(getRecArticulo()->getValue("MULTIPLOBASE").toDouble());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	editSubcuenta->setText(getRecArticulo()->getValue("SUBCUENTA").toString());
}
#endif
	editArticuloImagen_Imagen->setImageData(getRecArticulo()->getValue("ARTICULOIMAGEN.IMAGEN"));
	if( isInserting() && !isDuplicating() && !DBAPP->codeNotFound().isEmpty() ) {
		editCodigo->setText( DBAPP->codeNotFound() );
		editCodigo->setJustEdited( true );
	}
	scatterFamilia();
	scatterProveedora();
	scatterTipoIVA();
/*>>>>>FRMEDITARTICULO_SCATTER*/
    if( isDuplicating() ) {
        editCodigo->setText( Xtring::null );
        editStock->setText( 0.0 );
    }
    scatterArticuloBase();
    setStockControls();
    editArticuloImagen_Imagen->setProposedFileName( editCodigo->toString() + ".jpg" );
}

void FrmEditArticulo::gatherFields()
{
/*<<<<<FRMEDITARTICULO_GATHER*/
	getRecArticulo()->setValue( "FAMILIA_ID", getRecFamilia()->getRecordID() );
	getRecArticulo()->setValue( "PROVEEDORA_ID", getRecProveedora()->getRecordID() );
	getRecArticulo()->setValue( "NOMBRE", editNombre->toString());
	getRecArticulo()->setValue( "CODIGO", editCodigo->toString());
	getRecArticulo()->setValue( "TIPOIVA_ID", getRecTipoIVA()->getRecordID() );
	getRecArticulo()->setValue( "FABRICANTE", editFabricante->toString());
	getRecArticulo()->setValue( "CODIGOINTERNO", editCodigoInterno->toString());
	getRecArticulo()->setValue( "COSTESINIVA", editCosteSinIVA->toMoney());
	getRecArticulo()->setValue( "COSTE", editCoste->toMoney());
	getRecArticulo()->setValue( "MARGENCOMERCIAL", editMargenComercial->toDouble());
	getRecArticulo()->setValue( "PVPSINIVA", editPVPSinIVA->toMoney());
	getRecArticulo()->setValue( "PVP", editPVP->toMoney());
	getRecArticulo()->setValue( "DTOCOMERCIAL", editDtoComercial->toDouble());
	getRecArticulo()->setValue( "PVP1", editPVP1->toMoney());
	getRecArticulo()->setValue( "PVP2", editPVP2->toMoney());
	getRecArticulo()->setValue( "PVP3", editPVP3->toMoney());
	getRecArticulo()->setValue( "USARSTOCKS", checkUsarStocks->isChecked());
	getRecArticulo()->setValue( "STOCK", editStock->toDouble());
	getRecArticulo()->setValue( "STOCKMAXIMO", editStockMaximo->toDouble());
	getRecArticulo()->setValue( "STOCKMINIMO", editStockMinimo->toDouble());
	getRecArticulo()->setValue( "ENUSO", checkEnUso->isChecked());
	getRecArticulo()->setValue( "NOVEDAD", checkNovedad->isChecked());
	getRecArticulo()->setValue( "OFERTA", checkOferta->isChecked());
	getRecArticulo()->setValue( "NOTAS", editNotas->toString());
	getRecArticulo()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecArticulo()->setValue( "MULTIPLOBASE", editMultiploBase->toDouble());
#ifdef HAVE_CONTABMODULE
if( ModuleInstance->getContabModule() ) {
	getRecArticulo()->setValue( "SUBCUENTA", editSubcuenta->toString());
}
#endif
	getRecArticulo()->setValue( "ARTICULOIMAGEN.IMAGEN", editArticuloImagen_Imagen->toData());
/*>>>>>FRMEDITARTICULO_GATHER*/
    getRecArticulo()->setValue( "ARTICULOBASE_ID", getRecArticuloBase()->getRecordID() );
}

void FrmEditArticulo::scatterTipoIVA()
{
    /*<<<<<FRMEDITARTICULO_SCATTER_TIPOIVA*/
	editTipoIVACodigo->setText( getRecTipoIVA()->getValue("CODIGO") );
	editTipoIVANombre->setText( getRecTipoIVA()->getValue("NOMBRE") );
/*>>>>>FRMEDITARTICULO_SCATTER_TIPOIVA*/
}
void FrmEditArticulo::pushTipoIVACodigo_clicked()
{
    /*<<<<<FRMEDITARTICULO_PUSH_TIPOIVA_CODIGO_CLICKED*/
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
				editTipoIVACodigo->setFocus();
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
					editTipoIVACodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecTipoIVA()->getRecordID() != 0 ) {
					editTipoIVACodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecTipoIVA(),
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
				editTipoIVACodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITARTICULO_PUSH_TIPOIVA_CODIGO_CLICKED*/
}

void FrmEditArticulo::scatterProveedora()
{
    /*<<<<<FRMEDITARTICULO_SCATTER_PROVEEDORA*/
	editProveedoraCodigo->setText( getRecProveedora()->getValue("CODIGO") );
	editProveedoraRazonSocial->setText( getRecProveedora()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITARTICULO_SCATTER_PROVEEDORA*/
}

void FrmEditArticulo::pushProveedoraCodigo_clicked()
{
    /*<<<<<FRMEDITARTICULO_PUSH_PROVEEDORA_CODIGO_CLICKED*/
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
				RecProveedora *tmprec = static_cast<RecProveedora *>(DBAPP->createRecord( "Proveedora" ));
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
/*>>>>>FRMEDITARTICULO_PUSH_PROVEEDORA_CODIGO_CLICKED*/
}
void FrmEditArticulo::specialControlKeyPressed( QWidget *sender, char key )
{
    _GONG_DEBUG_TRACE(0);
    /*<<<<<FRMEDITARTICULO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editFamiliaCodigo )
		pushFamiliaCodigo_clicked();
	if( sender == editProveedoraCodigo )
		pushProveedoraCodigo_clicked();
	if( sender == editTipoIVACodigo )
		pushTipoIVACodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITARTICULO_SPECIALACTION*/
    if( sender == editArticuloBaseCodigo ) {
        mControlKeyPressed = key;
        pushArticuloBaseCodigo_clicked();
    }
}

void FrmEditArticulo::scatterFamilia()
{
    /*<<<<<FRMEDITARTICULO_SCATTER_FAMILIA*/
	editFamiliaCodigo->setText( getRecFamilia()->getValue("CODIGO") );
	editFamiliaNombre->setText( getRecFamilia()->getValue("NOMBRE") );
/*>>>>>FRMEDITARTICULO_SCATTER_FAMILIA*/
}

void FrmEditArticulo::pushFamiliaCodigo_clicked()
{
    /*<<<<<FRMEDITARTICULO_PUSH_FAMILIA_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchFamiliaCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editFamiliaCodigo->setJustEdited( false );
			editFamiliaCodigo->setCancelling();
			if( DBAPP->choose(this, getRecFamilia(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterFamilia();
				editFamiliaCodigo->setJustEdited( true );
				editFamiliaCodigo->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecFamilia()->getRecordID() ) {
					editFamiliaCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecFamilia(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editFamiliaCodigo->setJustEdited( true );
						scatterFamilia();
					}
					editFamiliaCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecFamilia()->getRecordID() != 0 ) {
					editFamiliaCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecFamilia(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecFamilia *tmprec = static_cast<RecFamilia *>(DBAPP->createRecord( "Familia" ));
				editFamiliaCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editFamiliaCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editFamiliaCodigo->setJustEdited( true );
					getRecFamilia()->copyRecord( tmprec );
					scatterFamilia();
				}
				editFamiliaCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITARTICULO_PUSH_FAMILIA_CODIGO_CLICKED*/
}

void FrmEditArticulo::scatterArticuloBase()
{
    // Puesto que no es relación, hay que leer a mano
    if( getRecArticulo()->getValue( "ARTICULOBASE_ID" ).toInt() != (int)getRecArticuloBase()->getRecordID() )
        getRecArticuloBase()->read( getRecArticulo()->getValue( "ARTICULOBASE_ID" ).toInt() );
    editArticuloBaseCodigo->setText( getRecArticuloBase()->getValue("CODIGO") );
    editArticuloBaseNombre->setText( getRecArticuloBase()->getValue("NOMBRE") );
}

void FrmEditArticulo::pushArticuloBaseCodigo_clicked()
{
    char action = mControlKeyPressed;
    if( !isEditing() )
        action = 'E';
    switch( action ) {
    case 'F':
    case '\0':
        editArticuloBaseCodigo->setJustEdited( false );
        editArticuloBaseCodigo->setCancelling();
        if( DBAPP->choose(this, getRecArticuloBase(), 0, dbApplication::editNone, this ) ) {
            setEdited(true);
            getRecArticulo()->setValue( "ARTICULOBASE_ID", getRecArticuloBase()->getRecordID() );
            scatterArticuloBase();
            editArticuloBaseCodigo->setJustEdited( true );
            editArticuloBaseCodigo->setFocus();
        }
        break;
    case 'M':
    {
        if( getRecArticuloBase()->getRecordID() ) {
            editArticuloBaseCodigo->setJustEdited( false );
            if( DBAPP->editRecord(this, getRecArticuloBase(), 0, DataTable::updating,
                                  dbApplication::simpleEdition, this ) ) {
                editArticuloBaseCodigo->setJustEdited( true );
                getRecArticulo()->setValue( "ARTICULOBASE_ID", getRecArticuloBase()->getRecordID() );
                scatterArticuloBase();
            }
            editArticuloBaseCodigo->setFocus();
        }
    }
    break;
    case 'E':
    {
        editArticuloBaseCodigo->setJustEdited( false );
        DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this,
                                              getRecArticuloBase(), 0, DataTable::selecting,
                                              dbApplication::simpleEdition, this ) );
    }
    break;
    case 'A':
    {
        RecArticulo *tmprec = static_cast<RecArticulo *>(DBAPP->createRecord( "ARTICULO" ));
        editArticuloBaseCodigo->setJustEdited( false );
        tmprec->clear( true ); // set default values
        DBAPP->setCodeNotFound( editArticuloBaseCodigo->toString() );
        if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
                              dbApplication::simpleEdition, this ) ) {
            editArticuloBaseCodigo->setJustEdited( true );
            getRecArticuloBase()->copyRecord( tmprec );
            getRecArticulo()->setValue( "ARTICULOBASE_ID", getRecArticuloBase()->getRecordID() );
            scatterArticuloBase();
        }
        editArticuloBaseCodigo->setFocus();
        DBAPP->setCodeNotFound( Xtring() );
    }
    break;
    }
}

void FrmEditArticulo::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITARTICULO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushFamiliaCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editFamiliaCodigo, editFamiliaNombre,
		getRecFamilia(), "CODIGO", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterFamilia();
	if( focusWidget() != pushProveedoraCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProveedoraCodigo, editProveedoraRazonSocial,
		getRecProveedora(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterProveedora();
	if( focusWidget() != pushTipoIVACodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editTipoIVACodigo, editTipoIVANombre,
		getRecTipoIVA(), "CODIGO", "NOMBRE", Xtring::null) )
		scatterTipoIVA();
/*>>>>>FRMEDITARTICULO_VALIDATE*/
	if( sender == editCodigo && editCodigo->isJustEdited() )
		editArticuloImagen_Imagen->setProposedFileName( editCodigo->toString() + ".jpg" );
    if( focusWidget() != pushArticuloBaseCodigo) // To avoid triggering the validate event if the button is pressed
        if( validSeekCode( sender, isvalid, *validresult, editArticuloBaseCodigo, editArticuloBaseNombre,
                           getRecArticuloBase(), "CODIGO", "NOMBRE", Xtring::null) ) {
            getRecArticulo()->setValue( "ARTICULOBASE_ID", getRecArticuloBase()->getRecordID() );
            scatterArticuloBase();
        }
    if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) ) {
        if( !sender )
            *isvalid = false;
    } else {
        if( sender == editNombre && editNombre->isJustEdited() && editCodigo->toString().isEmpty() ) {
            editCodigo->setText( getRecArticulo()->genCodigoArticulo(
                                     getRecProveedora()->getValue( "GENCODARTICULO" ).toInt(),
                                     getRecProveedora()->getValue( "FORMATCODARTICULO" ).toString(),
                                     editNombre->toString(),
                                     getRecFamilia()->getValue( "CODIGO" ).toInt(),
                                     getRecFamilia()->getValue( "NOMBRE" ).toString(),
                                     getRecProveedora()->getValue( "CODIGO" ).toInt(),
                                     getRecProveedora()->getValue( "RAZONSOCIAL" ).toString() ) );
        }
    }

    if( sender == checkUsarStocks )
        setStockControls();
    if( sender == editTipoIVACodigo && editTipoIVACodigo->isJustEdited() ) {
        fixPrecios( Xtring::null );
    }
    if( sender == editFamiliaCodigo && editFamiliaCodigo->isJustEdited() ) {
        if ( getRecFamilia()->getValue( "TIPOIVA_ID" ).toInt() != 0 ) {
            editTipoIVACodigo->setText( getRecFamilia()->getRecTipoIVA()->getValue( "CODIGO" ) );
            editTipoIVACodigo->setJustEdited( true );
            validate( editTipoIVACodigo, 0 );
        }
    }
    if( sender == editCosteSinIVA && editCosteSinIVA->isJustEdited() ) {
        editCoste->setText( Money( editCosteSinIVA->toDouble()
                                   * ( 1 + ( getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 ) ), 2 ) );
        editPVPSinIVA->setText( Money( editCosteSinIVA->toDouble()
                                       * ( 1 + ( editMargenComercial->toDouble() / 100 ) ), 4 ) );
        editPVP->setText( Money( editPVPSinIVA->toDouble()
                                 * ( 1 + ( getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 ) ), 2 ) );
        fixPrecios( "COSTESINIVA" );
    } else if ( sender == editCoste && editCoste->isJustEdited() ) {
        editCosteSinIVA->setText( getRecTipoIVA()->menosIVA( Money(editCoste->toDouble(), 4) ) );
        editPVPSinIVA->setText( Money( editCosteSinIVA->toDouble()
                                       * ( 1 + ( editMargenComercial->toDouble() / 100 ) ), 4 ) );
        editPVP->setText( getRecTipoIVA()->masIVA( editPVPSinIVA->toDouble() ) );
        fixPrecios( "COSTE" );
    } else if ( sender == editMargenComercial && editMargenComercial->isJustEdited() ) {
        editPVPSinIVA->setText( Money( editCosteSinIVA->toDouble()
                                       * ( 1 + ( editMargenComercial->toDouble() / 100 ) ), 4 ) );
        editPVP->setText( getRecTipoIVA()->masIVA( editPVPSinIVA->toDouble() ) );
        fixPrecios( "MARGENCOMERCIAL" );
    } else if ( sender == editPVPSinIVA && editPVPSinIVA->isJustEdited() ) {
        editPVP->setText( Money( editPVPSinIVA->toDouble()
                                 * ( 1 + ( getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 ) ), 2 ) );
//         editCoste->setText( Money( editPVP->toDouble()
//                                  * ( 1 - ( editDtoComercial->toDouble() / 100 ) ), 4 ) );
//  		editCosteSinIVA->setText( getRecTipoIVA()->menosIVA( Money(editCoste->toDouble(),4) ) );
        fixPrecios( "PVPSINIVA" );
    } else if ( sender == editPVP && editPVP->isJustEdited() ) {
        editPVPSinIVA->setText( Money( editPVP->toDouble()
                                       / ( 1 + ( getRecTipoIVA()->getValue( "IVA" ).toDouble() / 100 ) ), 4 ) );
//         editCoste->setText( Money(editPVP->toDouble()
//                                 * ( 1 - ( editDtoComercial->toDouble() / 100 ) ) ) );
//  		editCosteSinIVA->setText( getRecTipoIVA()->menosIVA( Money(editCoste->toDouble(), 4 ) ) );
        fixPrecios( "COSTESINIVA" );
    } else if ( sender == editDtoComercial && editDtoComercial->isJustEdited() ) {
        editCoste->setText( editPVP->toDouble()
                            * ( 1 - ( editDtoComercial->toDouble() / 100 ) ) );
        editCosteSinIVA->setText( getRecTipoIVA()->menosIVA( Money(editCoste->toDouble(),4) ) );
        fixPrecios( "DTOCOMERCIAL" );
    }
    if( !sender ) {
        if( !editArticuloBaseCodigo->toString().isEmpty() && editMultiploBase->toDouble() == 0.0 ) {
            validresult->addError( _("Introduce un valor en el múltiplo de la base."),
                                   "MULTIPLOBASE" );
            *isvalid = false;
        } else if( editArticuloBaseCodigo->toString().isEmpty() && editMultiploBase->toDouble() != 0.0 ) {
            validresult->addError( _("El múltiplo de la base debe de estar a cero."),
                                   "MULTIPLOBASE" );
            *isvalid = false;
        }
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditArticulo::editArticuloImagen_clicked()
{
    Xtring fname = GuiApplication::getOpenFileName(
                       Xtring::printf( _( "Elige una imagen para %s"),
                                       DBAPP->getTableDescSingular("CONTACTO", "esta" ).c_str() ),
                       Xtring::null,
                       _( "Imágenes (*.png *.gif *.jpg)" ),
                       this );
    if ( !fname.isEmpty() ) {
        if ( editArticuloImagen_Imagen->loadFromFile( fname ) )
            editArticuloImagen_Imagen->update();
    }
}

void FrmEditArticulo::fixPrecios(const Xtring &changedfld )
{
    getRecArticulo()->setValue( "COSTESINIVA", editCosteSinIVA->toMoney());
    getRecArticulo()->setValue( "COSTE", editCoste->toMoney());
    getRecArticulo()->setValue( "MARGENCOMERCIAL", editMargenComercial->toDouble());
    getRecArticulo()->setValue( "PVPSINIVA", editPVPSinIVA->toMoney());
    getRecArticulo()->setValue( "PVP", editPVP->toMoney());
    getRecArticulo()->setValue( "DTOCOMERCIAL", editDtoComercial->toDouble());
    getRecArticulo()->fixPrecios( changedfld );
    editCosteSinIVA->setText(getRecArticulo()->getValue("COSTESINIVA").toMoney());
    editCoste->setText(getRecArticulo()->getValue("COSTE").toMoney());
    editMargenComercial->setText(getRecArticulo()->getValue("MARGENCOMERCIAL").toDouble());
    editPVPSinIVA->setText(getRecArticulo()->getValue("PVPSINIVA").toMoney());
    editPVP->setText(getRecArticulo()->getValue("PVP").toMoney());
    editDtoComercial->setText(getRecArticulo()->getValue("DTOCOMERCIAL").toDouble());
}

void FrmEditArticulo::setStockControls()
{
    editStock->setEnabled( checkUsarStocks->isOn() );
    editStockMaximo->setEnabled( checkUsarStocks->isOn() );
    editStockMinimo->setEnabled( checkUsarStocks->isOn() );
}

void FrmEditArticulo::pushGenCodArt_clicked()
{
    gatherFields();
    FrmGenCodArt *fgca = new FrmGenCodArt( getRecArticulo() );
    fgca->showModalFor(this, true, true );
    if( !fgca->getCodArt().isEmpty() )
        editCodigo->setText( fgca->getCodArt() );
    delete fgca;
}

/*<<<<<FRMEDITARTICULO_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITARTICULO_FIN*/



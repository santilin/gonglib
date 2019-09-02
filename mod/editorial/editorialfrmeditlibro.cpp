/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de editorial
// FIELD ISBN13 string tabLibro isbn
// FIELD ISBN10 string tabLibro isbn
// FIELD DepositoLegal string tabLibro isbn
// FIELD Titulo string tabLibro titulo
// FIELD Subtitulo text tabLibro subtitulo
// FIELD Autoras text tabLibro autoras
// FIELD Editorial_ID Reference(Editorial,Codigo,RazonSocial,dbRecord::InsertIfNotFound) tabLibro editorial
// FIELD Ano int tabLibro ano
// FIELD Edicion string tabLibro ano
// FIELD FechaEdicion string tabLibro ano
// FIELD Paginas int tabLibro ano
// FIELD Lengua string tabLibro coleccion
// FIELD Coleccion string tabLibro coleccion
// FIELD CDU string tabLibro coleccion
// FIELD LenguasTraduccion string tabLibro traduccion
// FIELD TituloOriginal string tabLibro traduccion
// FIELD Impresor string tabLibro impresor
// FIELD FechaImpresion string tabLibro impresor
// FIELD LugarImpresion string tabLibro impresor
// FIELD Encuadernacion string tabLibro encuadernacion
// FIELD Creditos text tabLibro
// INHERIT factu::Articulo
// TYPE FrmEditArticulo editorial::Libro
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITLIBRO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "editorialfrmeditlibro.h"
/*>>>>>FRMEDITLIBRO_INCLUDES*/
#include "editorialisbnsearch.h"

namespace gong {
namespace editorial {

/*<<<<<FRMEDITLIBRO_CONSTRUCTOR*/
FrmEditLibro::FrmEditLibro(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : factu::FrmEditArticulo( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditLibro" );
/*>>>>>FRMEDITLIBRO_CONSTRUCTOR*/
	guiInit();
	mEsLibro = false;
}

void FrmEditLibro::guiInit()
{
/*<<<<<FRMEDITLIBRO_INIT_CONTROLS*/
	showTabs(true);
	QWidget *tabLibro = new QWidget( pTabWidget, "tabLibro" );
	QVBoxLayout *tabLibroLayout = new QVBoxLayout(tabLibro, 11, 6, "tabLibroLayout");
	QHBoxLayout *isbnLayout = new QHBoxLayout(0, 0, 6, "isbnLayout");
	QHBoxLayout *tituloLayout = new QHBoxLayout(0, 0, 6, "tituloLayout");
	QHBoxLayout *subtituloLayout = new QHBoxLayout(0, 0, 6, "subtituloLayout");
	QHBoxLayout *autorasLayout = new QHBoxLayout(0, 0, 6, "autorasLayout");
	QHBoxLayout *editorialLayout = new QHBoxLayout(0, 0, 6, "editorialLayout");
	QHBoxLayout *anoLayout = new QHBoxLayout(0, 0, 6, "anoLayout");
	QHBoxLayout *coleccionLayout = new QHBoxLayout(0, 0, 6, "coleccionLayout");
	QHBoxLayout *traduccionLayout = new QHBoxLayout(0, 0, 6, "traduccionLayout");
	QHBoxLayout *impresorLayout = new QHBoxLayout(0, 0, 6, "impresorLayout");
	QHBoxLayout *encuadernacionLayout = new QHBoxLayout(0, 0, 6, "encuadernacionLayout");
	QHBoxLayout *creditosLayout = new QHBoxLayout(0, 0, 6, "creditosLayout");
	editISBN13 = addEditField( tabLibro, "ARTICULO", "ISBN13", isbnLayout );
	editISBN10 = addEditField( tabLibro, "ARTICULO", "ISBN10", isbnLayout );
	editDepositoLegal = addEditField( tabLibro, "ARTICULO", "DEPOSITOLEGAL", isbnLayout );
	editTitulo = addEditField( tabLibro, "ARTICULO", "TITULO", tituloLayout );
	editSubtitulo = addTextField( tabLibro, "ARTICULO", "SUBTITULO", subtituloLayout );
	editAutoras = addTextField( tabLibro, "ARTICULO", "AUTORAS", autorasLayout );

	searchEditorialCodigo = addSearchField( tabLibro, "EDITORIAL_ID", "EDITORIAL", "CODIGO", "RAZONSOCIAL", editorialLayout );
	pushEditorialCodigo = searchEditorialCodigo->getButton();
	connect( pushEditorialCodigo, SIGNAL( clicked() ), this, SLOT( pushEditorialCodigo_clicked() ) );
	editEditorialCodigo = searchEditorialCodigo->getEditCode();
	editEditorialRazonSocial = searchEditorialCodigo->getEditDesc();
	editAno = addEditField( tabLibro, "ARTICULO", "ANO", anoLayout );
	editEdicion = addEditField( tabLibro, "ARTICULO", "EDICION", anoLayout );
	editFechaEdicion = addEditField( tabLibro, "ARTICULO", "FECHAEDICION", anoLayout );
	editPaginas = addEditField( tabLibro, "ARTICULO", "PAGINAS", anoLayout );
	editLengua = addEditField( tabLibro, "ARTICULO", "LENGUA", coleccionLayout );
	editColeccion = addEditField( tabLibro, "ARTICULO", "COLECCION", coleccionLayout );
	editCDU = addEditField( tabLibro, "ARTICULO", "CDU", coleccionLayout );
	editLenguasTraduccion = addEditField( tabLibro, "ARTICULO", "LENGUASTRADUCCION", traduccionLayout );
	editTituloOriginal = addEditField( tabLibro, "ARTICULO", "TITULOORIGINAL", traduccionLayout );
	editImpresor = addEditField( tabLibro, "ARTICULO", "IMPRESOR", impresorLayout );
	editFechaImpresion = addEditField( tabLibro, "ARTICULO", "FECHAIMPRESION", impresorLayout );
	editLugarImpresion = addEditField( tabLibro, "ARTICULO", "LUGARIMPRESION", impresorLayout );
	editEncuadernacion = addEditField( tabLibro, "ARTICULO", "ENCUADERNACION", encuadernacionLayout );
	editCreditos = addTextField( tabLibro, "ARTICULO", "CREDITOS", creditosLayout );
	tabLibroLayout->addLayout( isbnLayout );
	tabLibroLayout->addLayout( tituloLayout );
	tabLibroLayout->addLayout( subtituloLayout );
	tabLibroLayout->addLayout( autorasLayout );
	tabLibroLayout->addLayout( editorialLayout );
	tabLibroLayout->addLayout( anoLayout );
	tabLibroLayout->addLayout( coleccionLayout );
	tabLibroLayout->addLayout( traduccionLayout );
	tabLibroLayout->addLayout( impresorLayout );
	tabLibroLayout->addLayout( encuadernacionLayout );
	tabLibroLayout->addLayout( creditosLayout );
/*>>>>>FRMEDITLIBRO_INIT_CONTROLS*/
	pushSearchISBN = new QPushButton( toGUI( _("Buscar ISBN") ), this, "pushSearchISBN" );
	connect ( pushSearchISBN, SIGNAL ( clicked() ), this, SLOT ( pushSearchISBN_clicked() ) );
	isbnLayout->addWidget( pushSearchISBN, 1 );
	pTabWidget->addTab( tabLibro, "&Libro" );
	mTabLibroPos = pTabWidget->count() - 1;
	editCreditos->setHeightInLines(3);
	editSubtitulo->setHeightInLines(3);
}

void FrmEditLibro::scatterFields()
{
/*<<<<<FRMEDITLIBRO_SCATTER*/
	factu::FrmEditArticulo::scatterFields();
	editISBN13->setText(getRecLibro()->getValue("ISBN13").toString());
	editISBN10->setText(getRecLibro()->getValue("ISBN10").toString());
	editDepositoLegal->setText(getRecLibro()->getValue("DEPOSITOLEGAL").toString());
	editTitulo->setText(getRecLibro()->getValue("TITULO").toString());
	editSubtitulo->setText(getRecLibro()->getValue("SUBTITULO").toString());
	editAutoras->setText(getRecLibro()->getValue("AUTORAS").toString());
	editAno->setText(getRecLibro()->getValue("ANO").toInt());
	editEdicion->setText(getRecLibro()->getValue("EDICION").toString());
	editFechaEdicion->setText(getRecLibro()->getValue("FECHAEDICION").toString());
	editPaginas->setText(getRecLibro()->getValue("PAGINAS").toInt());
	editLengua->setText(getRecLibro()->getValue("LENGUA").toString());
	editColeccion->setText(getRecLibro()->getValue("COLECCION").toString());
	editCDU->setText(getRecLibro()->getValue("CDU").toString());
	editLenguasTraduccion->setText(getRecLibro()->getValue("LENGUASTRADUCCION").toString());
	editTituloOriginal->setText(getRecLibro()->getValue("TITULOORIGINAL").toString());
	editImpresor->setText(getRecLibro()->getValue("IMPRESOR").toString());
	editFechaImpresion->setText(getRecLibro()->getValue("FECHAIMPRESION").toString());
	editLugarImpresion->setText(getRecLibro()->getValue("LUGARIMPRESION").toString());
	editEncuadernacion->setText(getRecLibro()->getValue("ENCUADERNACION").toString());
	editCreditos->setText(getRecLibro()->getValue("CREDITOS").toString());
	scatterEditorial();
/*>>>>>FRMEDITLIBRO_SCATTER*/
	if( isInserting() && !DBAPP->codeNotFound().isEmpty() ) {
		if( esLibro() ) {
			bool ok = false;
			DBAPP->codeNotFound().toLong(&ok);
			if( ok )
				editISBN13->setText( DBAPP->codeNotFound() );
			else
				editTitulo->setText( DBAPP->codeNotFound() );
		} else {
			editDescripcion->setText( DBAPP->codeNotFound() );
		}
	}
	if( isDuplicating() ) // Para que al duplicar no piense que es un libro
		editDepositoLegal->setText( editCodigo->toString() );

	updateControls();
// 	if( esLibro() ) {
// 		pFocusWidget = editISBN13;
// 		tabFrameEdit->setCurrentIndex( 1 );
// 	} else {
		pTabWidget->setCurrentIndex( 0 );
		pFocusWidget = editFamiliaCodigo;
// 	}
}

void FrmEditLibro::gatherFields()
{
/*<<<<<FRMEDITLIBRO_GATHER*/
	factu::FrmEditArticulo::gatherFields();
	getRecLibro()->setValue( "ISBN13", editISBN13->toString());
	getRecLibro()->setValue( "ISBN10", editISBN10->toString());
	getRecLibro()->setValue( "DEPOSITOLEGAL", editDepositoLegal->toString());
	getRecLibro()->setValue( "TITULO", editTitulo->toString());
	getRecLibro()->setValue( "SUBTITULO", editSubtitulo->toString());
	getRecLibro()->setValue( "AUTORAS", editAutoras->toString());
	getRecLibro()->setValue( "EDITORIAL_ID", getRecEditorial()->getRecordID() );
	getRecLibro()->setValue( "ANO", editAno->toInt());
	getRecLibro()->setValue( "EDICION", editEdicion->toString());
	getRecLibro()->setValue( "FECHAEDICION", editFechaEdicion->toString());
	getRecLibro()->setValue( "PAGINAS", editPaginas->toInt());
	getRecLibro()->setValue( "LENGUA", editLengua->toString());
	getRecLibro()->setValue( "COLECCION", editColeccion->toString());
	getRecLibro()->setValue( "CDU", editCDU->toString());
	getRecLibro()->setValue( "LENGUASTRADUCCION", editLenguasTraduccion->toString());
	getRecLibro()->setValue( "TITULOORIGINAL", editTituloOriginal->toString());
	getRecLibro()->setValue( "IMPRESOR", editImpresor->toString());
	getRecLibro()->setValue( "FECHAIMPRESION", editFechaImpresion->toString());
	getRecLibro()->setValue( "LUGARIMPRESION", editLugarImpresion->toString());
	getRecLibro()->setValue( "ENCUADERNACION", editEncuadernacion->toString());
	getRecLibro()->setValue( "CREDITOS", editCreditos->toString());
/*>>>>>FRMEDITLIBRO_GATHER*/
	if( !esLibro() ) {
		getRecLibro()->setValue( "EDITORIAL_ID", 0 );
		getRecLibro()->setValue( "TITULO", editNombre->toString() );
		getRecLibro()->setValue( "DEPOSITOLEGAL", editCodigo->toString() );
	}
}

void FrmEditLibro::scatterEditorial()
{
/*<<<<<FRMEDITLIBRO_SCATTER_EDITORIAL*/
	editEditorialCodigo->setText( getRecEditorial()->getValue("CODIGO") );
	editEditorialRazonSocial->setText( getRecEditorial()->getValue("RAZONSOCIAL") );
/*>>>>>FRMEDITLIBRO_SCATTER_EDITORIAL*/
}

void FrmEditLibro::pushEditorialCodigo_clicked()
{
/*<<<<<FRMEDITLIBRO_PUSH_EDITORIAL_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchEditorialCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editEditorialCodigo->setJustEdited( false );
			editEditorialCodigo->setCancelling();
			if( DBAPP->choose(this, getRecEditorial(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterEditorial();
				editEditorialCodigo->setJustEdited( true );
				setWiseFocus(editEditorialCodigo);
			}
			break;
		case 'M':
			{
				if( getRecEditorial()->getRecordID() ) {
					editEditorialCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecEditorial(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editEditorialCodigo->setJustEdited( true );
						scatterEditorial();
					}
				setWiseFocus(editEditorialCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecEditorial()->getRecordID() != 0 ) {
					editEditorialCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecEditorial(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecEditorial *tmprec = static_cast<RecEditorial *>(DBAPP->createRecord( "Editorial" ));
				editEditorialCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editEditorialCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editEditorialCodigo->setJustEdited( true );
					getRecEditorial()->copyRecord( tmprec );
					scatterEditorial();
				}
				setWiseFocus(editEditorialCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITLIBRO_PUSH_EDITORIAL_CODIGO_CLICKED*/
}
void FrmEditLibro::specialControlKeyPressed( QWidget *sender, char key )
{
/*<<<<<FRMEDITLIBRO_SPECIALACTION*/
	mControlKeyPressed = key;
	factu::FrmEditArticulo::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editEditorialCodigo )
		pushEditorialCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITLIBRO_SPECIALACTION*/
}

void FrmEditLibro::pushSearchISBN_clicked()
{
	Xtring isbn = editISBN13->toString();
	if( isbn.isEmpty() )
		isbn = editISBN10->toString();
	if( !isbn.isEmpty() ) {
		ISBNInfo ii;
		DBAPP->waitCursor(true);
		Xtring error = ISBNSearch("mcu.es", isbn, ii );
		if( error != Xtring::null ) {
			msgError(this, "El ISBN " + isbn + " no se ha encontrado.\n" + error);
			DBAPP->resetCursor();
			return;
		} else {
			Xtring msg = "Información del ISBN de www.mcu.es\n\n";
			msg += "Título: " + ii.titulo + "\n";
			msg += "Autor/as: " + ii.autoras + "\n";
			msg += "Editorial: " + ii.editorial + "\n";
			msg += "Precio venta público: " + Xtring::number(getRecTipoIVA()->masIVA(ii.precio.toDouble()).toDouble()) + "\n";
			if( msgYesNoLarge( "Búsqueda de ISBN",
			              "¿Quieres copiar los datos a la ficha del libro?",
			              msg ) ) {
				if( !ii.isbn10.isEmpty() ) {
					editISBN10->setText( ii.isbn10 );
					editCodigo->setText( ii.isbn10 );
				}
				if( !ii.isbn13.isEmpty() ) {
					editISBN13->setText( ii.isbn13 );
					editCodigo->setText( ii.isbn13 );
				}
				Xtring titulo, subtitulo;
				ii.titulo.splitIn2(titulo, subtitulo, ":");
				titulo = titulo.trim();
				subtitulo = subtitulo.trim();
		    	editTitulo->setText( titulo );
				editNombre->setText( titulo );
		    	editSubtitulo->setText( subtitulo.trim().proper() );
				editAutoras->setText( ii.autoras );
				editAno->setText( ii.anyo );
				editLengua->setText( ii.lengua );
				editEdicion->setText( ii.edicion );
				editFechaEdicion->setText( ii.fechaedicion );
				editFechaImpresion->setText( ii.fechaimpresion );
				editColeccion->setText( ii.coleccion );
				editEditorialCodigo->setText( ii.editorial );
				editEditorialCodigo->setJustEdited( true );
				validateFields( editEditorialCodigo, 0 );
				editFabricante->setText( editEditorialRazonSocial );
				Xtring CDU, descCDU;
				ii.materias.splitIn2( CDU, descCDU, "-");
				editCDU->setText( CDU );
				editLenguasTraduccion->setText( ii.lenguastraduccion );
				editPaginas->setText( ii.descripcion.toInt() );
				editEncuadernacion->setText( ii.descripcion + " " + ii.encuadernacion );
				editPVP->setText( getRecTipoIVA()->masIVA(ii.precio.toDouble()) );
				fixPrecios( "PVP" );
		    }
		}
		DBAPP->resetCursor();
	}
	editTitulo->setFocus();
}

void FrmEditLibro::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
	if( esLibro() ) {
		if( sender == editISBN13 || sender == editISBN10 || sender == editDepositoLegal )
			editCodigo->setText( editISBN13->toString().isEmpty()
					? (editISBN10->toString().isEmpty()
						? editDepositoLegal->toString() : editISBN10->toString() )
					: editISBN13->toString() );
		if( sender == editTitulo )
			editNombre->setText( editTitulo->toString() );
		if( sender == editEditorialCodigo )
			editFabricante->setText( editEditorialRazonSocial->toString() );
	} else {
		if( sender == editCodigo && editCodigo->isJustEdited() ) {
			editDepositoLegal->setText( Xtring::null );
			editISBN10->setText(Xtring::null);
			editISBN13->setText(Xtring::null);
		}
		if( sender == editNombre && editNombre->isJustEdited() ) {
			editTitulo->setText( Xtring::null );
		}
	}
/*<<<<<FRMEDITLIBRO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	factu::FrmEditArticulo::validateFields(sender, isvalid, validresult);
	if( focusWidget() != pushEditorialCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editEditorialCodigo, editEditorialRazonSocial,
		getRecEditorial(), "CODIGO", "RAZONSOCIAL", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterEditorial();
/*>>>>>FRMEDITLIBRO_VALIDATE*/
// {capel}  añadir if( mEsLibro ) { detrás de factu::FrmEditArticulo::validateFields...
	if( !sender ) {
		if( editISBN13->toString().isEmpty() && editISBN10->toString().isEmpty() )
			validresult->addWarning("El ISBN no debería estar vacío", "ISBN13");
	}
	if( !editISBN13->toString().isEmpty() )
		if( !validCodeAndDesc( sender, *validresult, editISBN13, 0,
	                      "ISBN13", Xtring::null ) )
			*isvalid = false;
	if( !editISBN10->toString().isEmpty() )
		if( !validCodeAndDesc( sender, *validresult, editISBN10, 0,
	                      "ISBN10", Xtring::null ) )
			*isvalid = false;
	if( !validCodeAndDesc( sender, *validresult, editDepositoLegal, 0,
	                      "depositolegal", Xtring::null ) )
			*isvalid = false;
}
	if( sender == editFamiliaCodigo && editFamiliaCodigo->isJustEdited() ) {
		scatterFamilia();
		updateControls();
	}
	if( !esLibro() ) {
		if( (!editISBN10->toString().isEmpty() && editISBN10->toString() != editCodigo->toString() )
			|| (!editISBN13->toString().isEmpty() && editISBN13->toString() != editCodigo->toString() )
			|| (!editDepositoLegal->toString().isEmpty() && editDepositoLegal->toString() != editCodigo->toString() )
			|| (!editTitulo->toString().isEmpty() && editTitulo->toString() != editNombre->toString() )
			|| (editEditorialCodigo->toInt() != 0) ) {

			_GONG_DEBUG_PRINT(0, editCodigo->toString() );
			_GONG_DEBUG_PRINT(0, editNombre->toString() );
			_GONG_DEBUG_PRINT(0, editISBN10->toString() );
			_GONG_DEBUG_PRINT(0, editISBN13->toString() );
			_GONG_DEBUG_PRINT(0, editDepositoLegal->toString() );
			_GONG_DEBUG_PRINT(0, editTitulo->toString() );
			_GONG_DEBUG_PRINT(0, editEditorialCodigo->toInt() );
			if( msgYesNo(this,
					 "Este artículo era un libro. ¿Quieres borrar los datos propios del libro?") ) {
				editISBN13->clear();
				editISBN10->clear();
				editDepositoLegal->clear();
				editTitulo->clear();
				editSubtitulo->clear();
				editAutoras->clear();
				editEditorialCodigo->setText("");
				editEditorialRazonSocial->setText("");
				getRecEditorial()->clear(false);
				getRecord()->setValue("EDITORIAL_ID", 0 );
				editAno->clear();
				editEdicion->clear();
				editFechaEdicion->clear();
				editLengua->clear();
				editColeccion->clear();
				editCDU->clear();
				editLenguasTraduccion->clear();
				editTituloOriginal->clear();
				editImpresor->clear();
				editFechaImpresion->clear();
				editLugarImpresion->clear();
				editDescripcion->clear();
				editPaginas->clear();
				editCreditos->clear();
			}
		}
	}
	if (!ir) {
		showValidMessages(isvalid, *validresult, sender);
		delete validresult;
	}
}

void FrmEditLibro::updateControls()
{
	mEsLibro = getRecFamilia()->getValue("LIBROS").toBool();
	if( mEsLibro ) {
		editCodigo->setEnabled(false);
		editNombre->setEnabled(false);
		editFabricante->setEnabled(false);
		pTabWidget->setTabEnabled(mTabLibroPos, true);
	} else {
		editCodigo->setEnabled(true);
		editNombre->setEnabled(true);
		editFabricante->setEnabled(true);
		pTabWidget->setTabEnabled(mTabLibroPos, false);
	}
}

/*<<<<<FRMEDITLIBRO_FIN*/
} // namespace editorial
} // namespace gong
/*>>>>>FRMEDITLIBRO_FIN*/

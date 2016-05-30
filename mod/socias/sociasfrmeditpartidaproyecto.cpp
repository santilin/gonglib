/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de partidas de proyectos
// FIELD Codigo string - codigoLayout
// FIELD Descripcion string - codigoLayout
// FIELD Proyecto_ID Reference(Proyecto,Codigo,Descripcion) - proyectoLayout
// FIELD Madre string - tipoLayout
// FIELD Tipo string - tipoLayout
// FIELD Orden int - tipoLayout
// FIELD Presupuesto Money - importeLayout
// FIELD Importe money - importeLayout
// FIELD Notas text - notasLayout
// TYPE FrmEditRecMaster socias::PartidaProyecto
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITPARTIDAPROYECTO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "sociasfrmeditpartidaproyecto.h"
/*>>>>>FRMEDITPARTIDAPROYECTO_INCLUDES*/
#include <pagosmodule.h>
#include "sociasfrmeditproyecto.h"

namespace gong {
namespace socias {

/*<<<<<FRMEDITPARTIDAPROYECTO_CONSTRUCTOR*/
FrmEditPartidaProyecto::FrmEditPartidaProyecto(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditPartidaProyecto" );
/*>>>>>FRMEDITPARTIDAPROYECTO_CONSTRUCTOR*/
    searchProyectoCodigo = 0;
    /*<<<<<FRMEDITPARTIDAPROYECTO_INIT_CONTROLS*/
	QHBoxLayout *codigoLayout = new QHBoxLayout(0, 0, 6, "codigoLayout");
	QHBoxLayout *proyectoLayout = new QHBoxLayout(0, 0, 6, "proyectoLayout");
	QHBoxLayout *tipoLayout = new QHBoxLayout(0, 0, 6, "tipoLayout");
	QHBoxLayout *importeLayout = new QHBoxLayout(0, 0, 6, "importeLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editCodigo = addEditField( pControlsFrame, "PARTIDAPROYECTO", "CODIGO", codigoLayout );
	editDescripcion = addEditField( pControlsFrame, "PARTIDAPROYECTO", "DESCRIPCION", codigoLayout );

	searchProyectoCodigo = addSearchField( pControlsFrame, "PROYECTO_ID", "PROYECTO", "CODIGO", "DESCRIPCION", proyectoLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoDescripcion = searchProyectoCodigo->getEditDesc();
	editMadre = addEditField( pControlsFrame, "PARTIDAPROYECTO", "MADRE", tipoLayout );
	editTipo = addEditField( pControlsFrame, "PARTIDAPROYECTO", "TIPO", tipoLayout );
	editOrden = addEditField( pControlsFrame, "PARTIDAPROYECTO", "ORDEN", tipoLayout );
	editPresupuesto = addEditField( pControlsFrame, "PARTIDAPROYECTO", "PRESUPUESTO", importeLayout );
	editImporte = addEditField( pControlsFrame, "PARTIDAPROYECTO", "IMPORTE", importeLayout );
	editNotas = addTextField( pControlsFrame, "PARTIDAPROYECTO", "NOTAS", notasLayout );
	pControlsLayout->addLayout( codigoLayout );
	pControlsLayout->addLayout( proyectoLayout );
	pControlsLayout->addLayout( tipoLayout );
	pControlsLayout->addLayout( importeLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITPARTIDAPROYECTO_INIT_CONTROLS*/
    if( searchProyectoCodigo )
        searchProyectoCodigo->setMustBeReadOnly(true);
}

void FrmEditPartidaProyecto::scatterProyecto()
{
    /*<<<<<FRMEDITPARTIDAPROYECTO_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoDescripcion->setText( getRecProyecto()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITPARTIDAPROYECTO_SCATTER_PROYECTO*/
}

void FrmEditPartidaProyecto::pushProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITPARTIDAPROYECTO_PUSH_PROYECTO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchProyectoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editProyectoCodigo->setJustEdited( false );
			editProyectoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecProyecto(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterProyecto();
				editProyectoCodigo->setJustEdited( true );
				setWiseFocus(editProyectoCodigo);
			}
			break;
		case 'M':
			{
				if( getRecProyecto()->getRecordID() ) {
					editProyectoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecProyecto(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editProyectoCodigo->setJustEdited( true );
						scatterProyecto();
					}
				setWiseFocus(editProyectoCodigo);
				}
			}
			break;
		case 'E':
			{
				if( getRecProyecto()->getRecordID() != 0 ) {
					editProyectoCodigo->setJustEdited( false );
					DBAPP->createClient( DBAPP->createEditForm(this, getRecProyecto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecProyecto *tmprec = static_cast<RecProyecto *>(DBAPP->createRecord( "Proyecto" ));
				editProyectoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editProyectoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editProyectoCodigo->setJustEdited( true );
					getRecProyecto()->copyRecord( tmprec );
					scatterProyecto();
				}
				setWiseFocus(editProyectoCodigo);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITPARTIDAPROYECTO_PUSH_PROYECTO_CODIGO_CLICKED*/
}
void FrmEditPartidaProyecto::scatterFields()
{
    /*<<<<<FRMEDITPARTIDAPROYECTO_SCATTER*/
	editCodigo->setText(getRecPartidaProyecto()->getValue("CODIGO").toString());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editDescripcion->setText(getRecPartidaProyecto()->getValue("DESCRIPCION").toString());
	editMadre->setText(getRecPartidaProyecto()->getValue("MADRE").toString());
	editTipo->setText(getRecPartidaProyecto()->getValue("TIPO").toString());
	editOrden->setText(getRecPartidaProyecto()->getValue("ORDEN").toInt());
	editPresupuesto->setText(getRecPartidaProyecto()->getValue("PRESUPUESTO").toMoney());
	editImporte->setText(getRecPartidaProyecto()->getValue("IMPORTE").toMoney());
	editNotas->setText(getRecPartidaProyecto()->getValue("NOTAS").toString());
	scatterProyecto();
/*>>>>>FRMEDITPARTIDAPROYECTO_SCATTER*/
    if( isInserting() || isDuplicating() ) {
        if( isDuplicating() ) {
            editPresupuesto->setText( 0.0 );
            editImporte->setText( 0.0 );
        } else {
            dbRecord *r = pagos::ModuleInstance->getRecProyectoPadre( this );
            if( r ) {
                editProyectoCodigo->setText( r->getValue( "CODIGO" ) );
                validate( editProyectoCodigo, 0 );
            }
        }
    }


}
void FrmEditPartidaProyecto::gatherFields()
{
    /*<<<<<FRMEDITPARTIDAPROYECTO_GATHER*/
	getRecPartidaProyecto()->setValue( "CODIGO", editCodigo->toString());
	getRecPartidaProyecto()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecPartidaProyecto()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
	getRecPartidaProyecto()->setValue( "MADRE", editMadre->toString());
	getRecPartidaProyecto()->setValue( "TIPO", editTipo->toString());
	getRecPartidaProyecto()->setValue( "ORDEN", editOrden->toInt());
	getRecPartidaProyecto()->setValue( "PRESUPUESTO", editPresupuesto->toMoney());
	getRecPartidaProyecto()->setValue( "IMPORTE", editImporte->toMoney());
	getRecPartidaProyecto()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPARTIDAPROYECTO_GATHER*/
}
void FrmEditPartidaProyecto::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITPARTIDAPROYECTO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITPARTIDAPROYECTO_SPECIALACTION*/
}

void FrmEditPartidaProyecto::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPARTIDAPROYECTO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->validate( ValidResult::editing) ) {
		validresult->append( pRecord->getErrors() );
		*isvalid = false;
	}
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoDescripcion,
		getRecProyecto(), "CODIGO", "DESCRIPCION", Xtring::null) )
		scatterProyecto();
/*>>>>>FRMEDITPARTIDAPROYECTO_VALIDATE*/
    if( !validCodeAndDesc( sender, *validresult, editCodigo, editDescripcion,
                           "codigo", "descripcion", getFormFilter() ) )
        if( !sender )
            *isvalid = false;
    if( sender == editTipo || !sender ) {
        if( editTipo->toString().upper() != "G"
                && editTipo->toString().upper() != "I"
                && editTipo->toString().upper() != "T" ) {
            validresult->addError( _("El tipo de partida tiene que ser G (Gasto), I (Ingreso) o T (Total)"), "TIPO" );
            *isvalid = false;
        }
    }
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITPARTIDAPROYECTO_FIN*/
} // namespace socias
} // namespace gong
/*>>>>>FRMEDITPARTIDAPROYECTO_FIN*/


/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de proyectos
// FIELD Codigo int - proyectoCodigo
// FIELD Nombre string - proyectoCodigo
// FIELD Descripcion text - proyectoDescLarga
// FIELD FechaInicio date - proyectoFechas
// FIELD FechaFin date - proyectoFechas
// FIELD SociasEstado comboint - proyectoFechas
// FIELD NumCuotas int - proyectoCuotas
// FIELD Importe money - proyectoCuotas
// FIELD Periodicidad comboint - proyectoCuotas
// FIELD FormatoNumRecibo string - formato
// FIELD Notas text - proyectoNotas
// FIELD Miembro FrmEditRecMaster tabMiembros miembrosLayout
// FIELD RemesaCobro FrmEditRecMaster tabRemesas remesasLayout
// FIELD PartidaProyecto FrmEditRecMaster tabPartidas partidaLayout
// TYPE FrmEditRecMaster socias::Proyecto validCodeAndDesc IncCode
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITPROYECTO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "sociasfrmeditproyecto.h"
/*>>>>>FRMEDITPROYECTO_INCLUDES*/
#include "sociasmodule.h"

namespace gong {
namespace socias {

/*<<<<<FRMEDITPROYECTO_CONSTRUCTOR*/
FrmEditProyecto::FrmEditProyecto(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditProyecto" );
/*>>>>>FRMEDITPROYECTO_CONSTRUCTOR*/

    /*<<<<<FRMEDITPROYECTO_INIT_CONTROLS*/
	QHBoxLayout *proyectoCodigoLayout = new QHBoxLayout(0, 0, 6, "proyectoCodigoLayout");
	QHBoxLayout *proyectoDescLargaLayout = new QHBoxLayout(0, 0, 6, "proyectoDescLargaLayout");
	QHBoxLayout *proyectoFechasLayout = new QHBoxLayout(0, 0, 6, "proyectoFechasLayout");
	QHBoxLayout *proyectoCuotasLayout = new QHBoxLayout(0, 0, 6, "proyectoCuotasLayout");
	QHBoxLayout *formatoLayout = new QHBoxLayout(0, 0, 6, "formatoLayout");
	QHBoxLayout *proyectoNotasLayout = new QHBoxLayout(0, 0, 6, "proyectoNotasLayout");
	showTabs(true);
	QWidget *tabMiembros = new QWidget( pTabWidget, "tabMiembros" );
	QVBoxLayout *tabMiembrosLayout = new QVBoxLayout(tabMiembros, 11, 6, "tabMiembrosLayout");
	QHBoxLayout *miembrosLayout = new QHBoxLayout(0, 0, 6, "miembrosLayout");
	QWidget *tabRemesas = new QWidget( pTabWidget, "tabRemesas" );
	QVBoxLayout *tabRemesasLayout = new QVBoxLayout(tabRemesas, 11, 6, "tabRemesasLayout");
	QHBoxLayout *remesasLayout = new QHBoxLayout(0, 0, 6, "remesasLayout");
	QWidget *tabPartidas = new QWidget( pTabWidget, "tabPartidas" );
	QVBoxLayout *tabPartidasLayout = new QVBoxLayout(tabPartidas, 11, 6, "tabPartidasLayout");
	QHBoxLayout *partidaLayout = new QHBoxLayout(0, 0, 6, "partidaLayout");
	editCodigo = addEditField( pControlsFrame, "PROYECTO", "CODIGO", proyectoCodigoLayout );
	editNombre = addEditField( pControlsFrame, "PROYECTO", "NOMBRE", proyectoCodigoLayout );
	editDescripcion = addTextField( pControlsFrame, "PROYECTO", "DESCRIPCION", proyectoDescLargaLayout );
	editFechaInicio = addEditField( pControlsFrame, "PROYECTO", "FECHAINICIO", proyectoFechasLayout );
	editFechaFin = addEditField( pControlsFrame, "PROYECTO", "FECHAFIN", proyectoFechasLayout );
	comboSociasEstado = addComboIntField( pControlsFrame, "PROYECTO", "SOCIASESTADO", proyectoFechasLayout );
	editNumCuotas = addEditField( pControlsFrame, "PROYECTO", "NUMCUOTAS", proyectoCuotasLayout );
	editImporte = addEditField( pControlsFrame, "PROYECTO", "IMPORTE", proyectoCuotasLayout );
	comboPeriodicidad = addComboIntField( pControlsFrame, "PROYECTO", "PERIODICIDAD", proyectoCuotasLayout );
	editFormatoNumRecibo = addEditField( pControlsFrame, "PROYECTO", "FORMATONUMRECIBO", formatoLayout );
	editNotas = addTextField( pControlsFrame, "PROYECTO", "NOTAS", proyectoNotasLayout );

	RecMiembro *miembro = static_cast<RecMiembro*>(DBAPP->createRecord("MIEMBRO"));
	dbViewDefinitionsList miembroviews;
	DBAPP->getDatabase()->getViewsForTable( miembro, miembroviews );
	dbRecordDataModel *miembrodm = new dbRecordDataModel(miembro, miembroviews);
	pFrmMiembro = static_cast<FrmEditMiembro *>(
		DBAPP->createEditForm(this, miembro, miembrodm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		tabMiembros) );
	miembrosLayout->addWidget(pFrmMiembro);

	RecRemesaCobro *remesacobro = static_cast<RecRemesaCobro*>(DBAPP->createRecord("REMESACOBRO"));
	dbViewDefinitionsList remesacobroviews;
	DBAPP->getDatabase()->getViewsForTable( remesacobro, remesacobroviews );
	dbRecordDataModel *remesacobrodm = new dbRecordDataModel(remesacobro, remesacobroviews);
	pFrmRemesaCobro = static_cast<FrmEditRemesaCobro *>(
		DBAPP->createEditForm(this, remesacobro, remesacobrodm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		tabRemesas) );
	remesasLayout->addWidget(pFrmRemesaCobro);

	RecPartidaProyecto *partidaproyecto = static_cast<RecPartidaProyecto*>(DBAPP->createRecord("PARTIDAPROYECTO"));
	dbViewDefinitionsList partidaproyectoviews;
	DBAPP->getDatabase()->getViewsForTable( partidaproyecto, partidaproyectoviews );
	dbRecordDataModel *partidaproyectodm = new dbRecordDataModel(partidaproyecto, partidaproyectoviews);
	pFrmPartidaProyecto = static_cast<FrmEditPartidaProyecto *>(
		DBAPP->createEditForm(this, partidaproyecto, partidaproyectodm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		tabPartidas) );
	partidaLayout->addWidget(pFrmPartidaProyecto);
	pControlsLayout->addLayout( proyectoCodigoLayout );
	pControlsLayout->addLayout( proyectoDescLargaLayout );
	pControlsLayout->addLayout( proyectoFechasLayout );
	pControlsLayout->addLayout( proyectoCuotasLayout );
	pControlsLayout->addLayout( formatoLayout );
	pControlsLayout->addLayout( proyectoNotasLayout );
	tabMiembrosLayout->addLayout( miembrosLayout );
	tabRemesasLayout->addLayout( remesasLayout );
	tabPartidasLayout->addLayout( partidaLayout );
/*>>>>>FRMEDITPROYECTO_INIT_CONTROLS*/
    pTabMiembros = tabMiembros;
    pTabRemesas = tabRemesas;
    pTabPartidas = tabPartidas;

    pTabWidget->insertTab( tabMiembros, "&Miembros" );
    pTabWidget->insertTab( tabRemesas, "&Remesas" );
    pTabWidget->insertTab( tabPartidas, "&Partidas" );
}

void FrmEditProyecto::scatterFields()
{
    /*<<<<<FRMEDITPROYECTO_SCATTER*/
	editCodigo->setText(getRecProyecto()->getValue("CODIGO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editCodigo;
	editNombre->setText(getRecProyecto()->getValue("NOMBRE").toString());
	editDescripcion->setText(getRecProyecto()->getValue("DESCRIPCION").toString());
	editFechaInicio->setText(getRecProyecto()->getValue("FECHAINICIO").toDate());
	editFechaFin->setText(getRecProyecto()->getValue("FECHAFIN").toDate());
	comboSociasEstado->setCurrentItemByValue(getRecProyecto()->getValue("SOCIASESTADO").toInt());
	editNumCuotas->setText(getRecProyecto()->getValue("NUMCUOTAS").toInt());
	editImporte->setText(getRecProyecto()->getValue("IMPORTE").toMoney());
	comboPeriodicidad->setCurrentItemByValue(getRecProyecto()->getValue("PERIODICIDAD").toInt());
	editFormatoNumRecibo->setText(getRecProyecto()->getValue("FORMATONUMRECIBO").toString());
	editNotas->setText(getRecProyecto()->getValue("NOTAS").toString());
	if( isInserting() && editCodigo->toInt() == 0 ) {
		editCodigo->setText( getRecord()->selectNextInt( "CODIGO" ) );
	}
/*>>>>>FRMEDITPROYECTO_SCATTER*/
    if( !isInserting() ) {
        pFrmMiembro->setFormFilter( "MIEMBRO.PROYECTO_ID="
                                    + getRecord()->getConnection()->toSQL( getRecProyecto()->getRecordID() ) );
        pFrmRemesaCobro->setFormFilter( "REMESACOBRO.PROYECTO_ID="
                                        + getRecord()->getConnection()->toSQL( getRecProyecto()->getRecordID() ));
        pFrmPartidaProyecto->setFormFilter( "PARTIDAPROYECTO.PROYECTO_ID="
                                            + getRecord()->getConnection()->toSQL( getRecProyecto()->getRecordID() ));
        pTabMiembros->setEnabled(true);
        pTabRemesas->setEnabled(true);
        pTabPartidas->setEnabled(true);
    } else {
        if( editFechaInicio->toDate() == Date::NullDate )
            editFechaInicio->setText( Date::currentDate() );
        comboSociasEstado->setCurrentItemByValue( 1 );
        pTabMiembros->setEnabled(false);
        pTabRemesas->setEnabled(false);
        pTabPartidas->setEnabled(false);
    }
}

void FrmEditProyecto::gatherFields()
{
    /*<<<<<FRMEDITPROYECTO_GATHER*/
	getRecProyecto()->setValue( "CODIGO", editCodigo->toInt());
	getRecProyecto()->setValue( "NOMBRE", editNombre->toString());
	getRecProyecto()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecProyecto()->setValue( "FECHAINICIO", editFechaInicio->toDate());
	getRecProyecto()->setValue( "FECHAFIN", editFechaFin->toDate());
	getRecProyecto()->setValue( "SOCIASESTADO", comboSociasEstado->getCurrentItemValue());
	getRecProyecto()->setValue( "NUMCUOTAS", editNumCuotas->toInt());
	getRecProyecto()->setValue( "IMPORTE", editImporte->toMoney());
	getRecProyecto()->setValue( "PERIODICIDAD", comboPeriodicidad->getCurrentItemValue());
	getRecProyecto()->setValue( "FORMATONUMRECIBO", editFormatoNumRecibo->toString());
	getRecProyecto()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITPROYECTO_GATHER*/
}

void FrmEditProyecto::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITPROYECTO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( !validCodeAndDesc( sender, *validresult, editCodigo, editNombre, "codigo", "nombre" ) )
		if( !sender )
			*isvalid = false;
/*>>>>>FRMEDITPROYECTO_VALIDATE*/
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

/*<<<<<FRMEDITPROYECTO_FIN*/
} // namespace socias
} // namespace gong
/*>>>>>FRMEDITPROYECTO_FIN*/

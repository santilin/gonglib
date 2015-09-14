/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de remesas de recibos de proyectos
// FIELD Numero int tabRemesa remesaNumero
// FIELD Proyecto_ID Reference(empresa::Proyecto,Codigo,Nombre,dbRecord::InsertIfNotFound) tabRemesa proyecto if(pedirProyecto())
// FIELD Descripcion string tabRemesa remesaNumero
// FIELD Cerrada bool tabRemesa remesaFechas
// FIELD FechaEmision date tabRemesa remesaFechas
// FIELD FechaCargo date tabRemesa remesaFechas
// FIELD CuentaBancoAbono string tabRemesa remesaAbono
// FIELD NRecibos int tabRemesa remesaTotales
// FIELD Importe money tabRemesa remesaTotales
// FIELD Resto money tabRemesa remesaTotales
// FIELD Notas text tabRemesa remesaNotas
// FIELD Cobro FrmEditRecMaster tabRecibos recibos
// TYPE FrmEditRecMaster pagos::RemesaCobro IncCode
/*>>>>>MODULE_INFO*/

#include <memory>
#include <qaction.h>
#include <qfiledialog.h>
#include <gongfileutils.h>
/*<<<<<FRMEDITREMESACOBRO_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "pagosfrmeditremesacobro.h"
/*>>>>>FRMEDITREMESACOBRO_INCLUDES*/
#include <gongdbrecordpermissions.h>
#include <empresamodule.h>
#include <empresafrmeditproyecto.h>
#include <pagosmodule.h>
#include "pagoscuaderno19.h"


namespace gong {
namespace pagos {

/*<<<<<FRMEDITREMESACOBRO_CONSTRUCTOR*/
FrmEditRemesaCobro::FrmEditRemesaCobro(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditRemesaCobro" );
/*>>>>>FRMEDITREMESACOBRO_CONSTRUCTOR*/
    QTabWidget *tabFrameEdit = new QTabWidget( pControlsFrame, "tabFrameEdit" );
    searchProyectoCodigo = 0;
    /*<<<<<FRMEDITREMESACOBRO_INIT_CONTROLS*/
	showTabs(true);
	QWidget *tabRemesa = new QWidget( pTabWidget, "tabRemesa" );
	QVBoxLayout *tabRemesaLayout = new QVBoxLayout(tabRemesa, 11, 6, "tabRemesaLayout");
	QHBoxLayout *remesaNumeroLayout = new QHBoxLayout(0, 0, 6, "remesaNumeroLayout");
	QHBoxLayout *proyectoLayout = new QHBoxLayout(0, 0, 6, "proyectoLayout");
	QHBoxLayout *remesaFechasLayout = new QHBoxLayout(0, 0, 6, "remesaFechasLayout");
	QHBoxLayout *remesaAbonoLayout = new QHBoxLayout(0, 0, 6, "remesaAbonoLayout");
	QHBoxLayout *remesaTotalesLayout = new QHBoxLayout(0, 0, 6, "remesaTotalesLayout");
	QHBoxLayout *remesaNotasLayout = new QHBoxLayout(0, 0, 6, "remesaNotasLayout");
	QWidget *tabRecibos = new QWidget( pTabWidget, "tabRecibos" );
	QVBoxLayout *tabRecibosLayout = new QVBoxLayout(tabRecibos, 11, 6, "tabRecibosLayout");
	QHBoxLayout *recibosLayout = new QHBoxLayout(0, 0, 6, "recibosLayout");
	editNumero = addEditField( tabRemesa, "REMESACOBRO", "NUMERO", remesaNumeroLayout );

if(pedirProyecto()){
	searchProyectoCodigo = addSearchField( tabRemesa, "PROYECTO_ID", "PROYECTO", "CODIGO", "NOMBRE", proyectoLayout );
	pushProyectoCodigo = searchProyectoCodigo->getButton();
	connect( pushProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushProyectoCodigo_clicked() ) );
	editProyectoCodigo = searchProyectoCodigo->getEditCode();
	editProyectoNombre = searchProyectoCodigo->getEditDesc();
}
	editDescripcion = addEditField( tabRemesa, "REMESACOBRO", "DESCRIPCION", remesaNumeroLayout );
	checkCerrada = addCheckField( tabRemesa, "REMESACOBRO", "CERRADA", remesaFechasLayout );
	editFechaEmision = addEditField( tabRemesa, "REMESACOBRO", "FECHAEMISION", remesaFechasLayout );
	editFechaCargo = addEditField( tabRemesa, "REMESACOBRO", "FECHACARGO", remesaFechasLayout );
	editCuentaBancoAbono = addEditField( tabRemesa, "REMESACOBRO", "CUENTABANCOABONO", remesaAbonoLayout );
	editNRecibos = addEditField( tabRemesa, "REMESACOBRO", "NRECIBOS", remesaTotalesLayout );
	editImporte = addEditField( tabRemesa, "REMESACOBRO", "IMPORTE", remesaTotalesLayout );
	editResto = addEditField( tabRemesa, "REMESACOBRO", "RESTO", remesaTotalesLayout );
	editNotas = addTextField( tabRemesa, "REMESACOBRO", "NOTAS", remesaNotasLayout );

	RecCobro *cobro = static_cast<RecCobro*>(DBAPP->createRecord("COBRO"));
	dbViewDefinitionDict cobroviews;
	DBAPP->getDatabase()->getViewsForTable( cobro, cobroviews );
	dbRecordDataModel *cobrodm = new dbRecordDataModel(cobro, cobroviews);
	pFrmCobro = static_cast<FrmEditCobro *>(
		DBAPP->createEditForm(this, cobro, cobrodm, DataTable::browsing,
		static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
		tabRecibos) );
	recibosLayout->addWidget(pFrmCobro);
	tabRemesaLayout->addLayout( remesaNumeroLayout );
	tabRemesaLayout->addLayout( proyectoLayout );
	tabRemesaLayout->addLayout( remesaFechasLayout );
	tabRemesaLayout->addLayout( remesaAbonoLayout );
	tabRemesaLayout->addLayout( remesaTotalesLayout );
	tabRemesaLayout->addLayout( remesaNotasLayout );
	tabRecibosLayout->addLayout( recibosLayout );
/*>>>>>FRMEDITREMESACOBRO_INIT_CONTROLS*/
    pTabRecibos = tabRecibos;
    tabFrameEdit->insertTab( tabRemesa, "R&emesa" );
    tabFrameEdit->insertTab( tabRecibos, "&Recibos" );
    pControlsLayout->addWidget( tabFrameEdit );
    pushGenerarCB19 = new QPushButton( QString::fromUtf8("&Generar CB19"), this, "pushGenerarCB19" );
    pButtonsLayout->insertWidget(0, pushGenerarCB19 );
    connect( pushGenerarCB19, SIGNAL( clicked() ), this, SLOT( pushGenerarCB19_clicked() ));
}

bool FrmEditRemesaCobro::pedirProyecto() const
{
    return pRecord->getTableDefinition()->findFieldDefinition("PROYECTO_ID");
}

void FrmEditRemesaCobro::scatterFields()
{
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNumero;
/*<<<<<FRMEDITREMESACOBRO_SCATTER*/
	editNumero->setText(getRecRemesaCobro()->getValue("NUMERO").toInt());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editNumero;
	editDescripcion->setText(getRecRemesaCobro()->getValue("DESCRIPCION").toString());
	checkCerrada->setChecked(getRecRemesaCobro()->getValue("CERRADA").toBool());
	editFechaEmision->setText(getRecRemesaCobro()->getValue("FECHAEMISION").toDate());
	editFechaCargo->setText(getRecRemesaCobro()->getValue("FECHACARGO").toDate());
	editCuentaBancoAbono->setText(getRecRemesaCobro()->getValue("CUENTABANCOABONO").toString());
	editNRecibos->setText(getRecRemesaCobro()->getValue("NRECIBOS").toInt());
	editImporte->setText(getRecRemesaCobro()->getValue("IMPORTE").toMoney());
	editResto->setText(getRecRemesaCobro()->getValue("RESTO").toMoney());
	editNotas->setText(getRecRemesaCobro()->getValue("NOTAS").toString());
	if( isInserting() && editNumero->toInt() == 0 ) {
		editNumero->setText( getRecord()->selectNextInt( "NUMERO" ) );
	}
if(pedirProyecto()){
	scatterProyecto();
}
/*>>>>>FRMEDITREMESACOBRO_SCATTER*/
    if( !isInserting() && !isDuplicating() ) {
        pFrmCobro->setFormFilter( "COBRO.REMESACOBRO_ID="
                                  + getRecord()->getConnection()->toSQL( getRecRemesaCobro()->getRecordID() ) );
        pTabRecibos->setEnabled( true );
    } else {
        pTabRecibos->setEnabled( false );
        editNRecibos->setText( 0 );
        editImporte->setText( 0.0 );
        editResto->setText( 0.0 );
        if( editFechaEmision->toDate() == Date::NullDate )
            editFechaEmision->setText( Date::currentDate() );
        if( editFechaCargo->toDate() == Date::NullDate )
            editFechaCargo->setText( Date::currentDate() );
        if( ModuleInstance->getRecProyectoPadre( this ) ) {
            getRecProyecto()->copyRecord( ModuleInstance->getRecProyectoPadre( this ) );
			searchProyectoCodigo->setMustBeReadOnly(true);
			scatterProyecto();
        }
    }
}

void FrmEditRemesaCobro::gatherFields()
{
    /*<<<<<FRMEDITREMESACOBRO_GATHER*/
	getRecRemesaCobro()->setValue( "NUMERO", editNumero->toInt());
if(pedirProyecto()){
	getRecRemesaCobro()->setValue( "PROYECTO_ID", getRecProyecto()->getRecordID() );
}
	getRecRemesaCobro()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecRemesaCobro()->setValue( "CERRADA", checkCerrada->isChecked());
	getRecRemesaCobro()->setValue( "FECHAEMISION", editFechaEmision->toDate());
	getRecRemesaCobro()->setValue( "FECHACARGO", editFechaCargo->toDate());
	getRecRemesaCobro()->setValue( "CUENTABANCOABONO", editCuentaBancoAbono->toString());
	getRecRemesaCobro()->setValue( "NRECIBOS", editNRecibos->toInt());
	getRecRemesaCobro()->setValue( "IMPORTE", editImporte->toMoney());
	getRecRemesaCobro()->setValue( "RESTO", editResto->toMoney());
	getRecRemesaCobro()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITREMESACOBRO_GATHER*/
}

void FrmEditRemesaCobro::validateFields( QWidget *sender, bool *isvalid, ValidResult *ir )
{
    /*<<<<<FRMEDITREMESACOBRO_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
if(pedirProyecto()){
	if( focusWidget() != pushProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editProyectoCodigo, editProyectoNombre,
		getRecProyecto(), "CODIGO", "NOMBRE", Xtring::null, dbRecord::SeekCodeFlags( dbRecord::InsertIfNotFound )) )
		scatterProyecto();
}
/*>>>>>FRMEDITREMESACOBRO_VALIDATE*/
    if( !validCodeAndDesc( sender, *validresult, editNumero, editDescripcion, "numero", "descripcion" ) )
        if( !sender )
            *isvalid = false;
    if ( !ir ) {
        showValidMessages( isvalid, *validresult, sender );
        delete validresult;
    }
}

void FrmEditRemesaCobro::scatterProyecto()
{
/*<<<<<FRMEDITREMESACOBRO_SCATTER_PROYECTO*/
	editProyectoCodigo->setText( getRecProyecto()->getValue("CODIGO") );
	editProyectoNombre->setText( getRecProyecto()->getValue("NOMBRE") );
/*>>>>>FRMEDITREMESACOBRO_SCATTER_PROYECTO*/
}

void FrmEditRemesaCobro::pushProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITREMESACOBRO_PUSH_PROYECTO_CODIGO_CLICKED*/
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
				empresa::RecProyecto *tmprec = static_cast<empresa::RecProyecto *>(DBAPP->createRecord( "Proyecto" ));
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
/*>>>>>FRMEDITREMESACOBRO_PUSH_PROYECTO_CODIGO_CLICKED*/
}

void FrmEditRemesaCobro::specialControlKeyPressed( QWidget *sender, char key )
{
    /*<<<<<FRMEDITREMESACOBRO_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
if(pedirProyecto()){
	if( sender == editProyectoCodigo )
		pushProyectoCodigo_clicked();
}
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITREMESACOBRO_SPECIALACTION*/
}

/**
 * Mostrar los botones para crear cb19 y para añadir registros solo en browse si hay remesas
 */
void FrmEditRemesaCobro::updateStatus()
{
    FrmEditRecMaster::updateStatus();
    dbRecordPermissions perm = getPermissions();
    if( perm.canView )
        pushGenerarCB19->setEnabled( isBrowsing() );
    else
        pushGenerarCB19->setEnabled( false );
    if( isInserting() || isDuplicating() )
        pushGenerarCB19->setEnabled( false );
}


void FrmEditRemesaCobro::actTotales()
{
    getRecRemesaCobro()->actTotales();
    editNRecibos->setText( getRecRemesaCobro()->getValue( "NRECIBOS" ).toInt() );
    editImporte->setText( getRecRemesaCobro()->getValue( "IMPORTE" ).toMoney() );
    editResto->setText( getRecRemesaCobro()->getValue( "RESTO" ).toMoney() );
}

void FrmEditRemesaCobro::pushGenerarCB19_clicked()
{
#ifdef FROMSOCIAS
    std::vector<dbRecordID> v = pFrmReciboProyecto->getDataTable()->getSelectedIDs();
    bool all = true;
#if 0
    if( v.size() <= 1 ) {
        all = FrmBase::msgYesNo( this, Xtring("No hay recibos seleccionados, ¿quieres seleccionarlos todos?" ));
        if( !all )
            return;
    }
#endif
    unsigned int nrecibos = 0, nrecibossincuenta = 0;
    Xtring sql= "SELECT RECIBOPROYECTO.NUMERO, CONTACTO.NOMBRE, MIEMBRO.CUENTABANCARIA, RECIBOPROYECTO.IMPORTE, RECIBOPROYECTO.DESCRIPCION, MIEMBRO.ESTADO"
                " FROM RECIBOPROYECTO INNER JOIN MIEMBRO ON RECIBOPROYECTO.MIEMBRO_ID=MIEMBRO.ID INNER JOIN CONTACTO ON CONTACTO.ID=MIEMBRO.CONTACTO_ID";
    if( !all ) {
        sql += " AND RECIBOPROYECTO.ID IN(";
        for( unsigned int i=0; i<v.size(); i++ ) {
            if( i != 0 )
                sql += ",";
            sql += Xtring::number(v[i]);
        }
        sql += ")";
    } else {
        sql += " WHERE RECIBOPROYECTO.REMESACOBRO_ID="
               + getRecord()->getConnection()->toSQL( getRecRemesaCobro()->getRecordID() );
    }
    DBAPP->waitCursor( true );
    dbResultSet *rs = getRecord()->getConnection()->select( sql );
    Cuaderno19 cb19(2);
    empresa::RecEmpresa *asociacion = empresa::ModuleInstance->getRecEmpresa();
    Xtring cuentaabono = getRecRemesaCobro()->getValue("CUENTABANCOABONO").toString();
    cb19.addCabeceraPresentador(asociacion->getRecContacto()->getValue( "CIF").toString(),
                                asociacion->getRecContacto()->getValue( "NOMBRE").toString(),
                                Date::currentDate(), cuentaabono.left(4), cuentaabono.mid(4,4));
    cb19.addCabeceraOrdenante(asociacion->getRecContacto()->getValue( "CIF").toString(),
                              asociacion->getRecContacto()->getValue( "NOMBRE").toString(),
                              Date::currentDate(), Date::currentDate() + 1, cuentaabono);
    while( rs->next() ) {
        Xtring cuentabancaria = Xtring(rs->toString(2)).replace(" ", "\0") ;
        SociasModule::EstadosActivo estado = static_cast<SociasModule::EstadosActivo>(rs->toInt(5));
        if( !cuentabancaria.isEmpty() && estado == SociasModule::activo ) {
            cb19.addRegistroIndividual( rs->toInt(0),
                                        rs->toString(1) /*nombre*/,
                                        cuentabancaria, /*cuentabancaria*/
                                        rs->toDouble(3) /* importe */,
                                        "", "",
                                        rs->toString(4) /*descripcion*/ );
            nrecibos++;
        } else {
            nrecibossincuenta++;
        }
    }
    delete rs;
    cb19.addRegistroTotalOrdenante();
    cb19.addRegistroTotalGeneral();
    Xtring filename = DBAPP->getSaveFileName( _("Elige el fichero de destino"), 
				_( "Ficheros Cb19 (*.c19);;Todos los ficheros(*)" ), 
				"saveq19file", this );
    if( !filename.isEmpty() ) {
        if( FileUtils::extension( filename ).isEmpty() )
            filename += ".c19";
        DBAPP->waitCursor( true );
        cb19.save( filename );
        FrmBase::msgOk( this,
                        Xtring::printf("Se han generado %d recibos en el fichero %s. Había %d recibos sin cuenta bancaria o de baja.",
                                       nrecibos, filename.c_str(), nrecibossincuenta) );
    }
#endif
    DBAPP->resetCursor();
}

void FrmEditRemesaCobro::refresh()
{
    FrmEditRecMaster::refresh();
    actTotales();
}

bool FrmEditRemesaCobro::canBeginEdit(EditMode newmode)
{
	if( newmode == DataTable::inserting ) {
		pTabRecibos->hide();
	} else {
		pTabRecibos->show();
	}
	return true;
}



/*<<<<<FRMEDITREMESACOBRO_FIN*/
} // namespace pagos
} // namespace gong
/*>>>>>FRMEDITREMESACOBRO_FIN*/



